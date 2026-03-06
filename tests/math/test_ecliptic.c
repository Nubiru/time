#include "../unity/unity.h"
#include "../../src/math/ecliptic.h"
#include <math.h>

#define DEG_EPSILON 0.1    /* 0.1 degree tolerance for precise cases */
#define APPROX_DEG  5.0    /* wider tolerance for approximate star positions */
#define OBL_EPSILON 0.001  /* obliquity tolerance */

/* J2000.0 obliquity for ecliptic-equatorial tests */
#define J2000_OBL 23.439291111
#define J2000_JD  2451545.0

void setUp(void) { }
void tearDown(void) { }

/* ===== ecliptic_to_equatorial ===== */

void test_vernal_equinox(void) {
    /* lon=0, lat=0 -> RA=0, Dec=0 (definition of the vernal equinox) */
    equatorial_t eq = ecliptic_to_equatorial(0.0, 0.0, J2000_OBL);
    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 0.0, eq.ra);
    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 0.0, eq.dec);
}

void test_summer_solstice(void) {
    /* lon=90, lat=0 -> RA=90, Dec=obliquity */
    equatorial_t eq = ecliptic_to_equatorial(90.0, 0.0, J2000_OBL);
    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 90.0, eq.ra);
    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, J2000_OBL, eq.dec);
}

void test_autumnal_equinox(void) {
    /* lon=180, lat=0 -> RA=180, Dec=0 */
    equatorial_t eq = ecliptic_to_equatorial(180.0, 0.0, J2000_OBL);
    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 180.0, eq.ra);
    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 0.0, eq.dec);
}

void test_winter_solstice(void) {
    /* lon=270, lat=0 -> RA=270, Dec=-obliquity */
    equatorial_t eq = ecliptic_to_equatorial(270.0, 0.0, J2000_OBL);
    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 270.0, eq.ra);
    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, -J2000_OBL, eq.dec);
}

void test_ecliptic_north_pole(void) {
    /* lat=90, lon=0 -> RA=270, Dec=90-obliquity=66.56 */
    equatorial_t eq = ecliptic_to_equatorial(0.0, 90.0, J2000_OBL);
    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 270.0, eq.ra);
    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 90.0 - J2000_OBL, eq.dec);
}

void test_ra_always_in_range(void) {
    /* RA must be in [0, 360) for various inputs */
    double lons[] = {0, 45, 90, 135, 180, 225, 270, 315, 359.9};
    double lats[] = {0, 30, -30, 60, -60};
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 5; j++) {
            equatorial_t eq = ecliptic_to_equatorial(lons[i], lats[j], J2000_OBL);
            TEST_ASSERT_TRUE(eq.ra >= 0.0);
            TEST_ASSERT_TRUE(eq.ra < 360.0);
        }
    }
}

void test_antares_approximate(void) {
    /* Antares: ecliptic lon ~247.96, lat ~-4.57 -> RA ~247.35, Dec ~-26.43 */
    equatorial_t eq = ecliptic_to_equatorial(247.96, -4.57, J2000_OBL);
    TEST_ASSERT_FLOAT_WITHIN(APPROX_DEG, 247.35, eq.ra);
    TEST_ASSERT_FLOAT_WITHIN(APPROX_DEG, -26.43, eq.dec);
}

void test_transform_is_pure(void) {
    /* Same input always gives same output */
    equatorial_t a = ecliptic_to_equatorial(123.456, 12.34, J2000_OBL);
    equatorial_t b = ecliptic_to_equatorial(123.456, 12.34, J2000_OBL);
    TEST_ASSERT_EQUAL_FLOAT(a.ra, b.ra);
    TEST_ASSERT_EQUAL_FLOAT(a.dec, b.dec);
}

/* ===== mean_obliquity ===== */

void test_obliquity_at_j2000(void) {
    /* At J2000.0 (JD 2451545.0), obliquity ~23.4393 */
    double obl = mean_obliquity(J2000_JD);
    TEST_ASSERT_FLOAT_WITHIN(OBL_EPSILON, 23.4393, obl);
}

void test_obliquity_at_j1900(void) {
    /* At J1900.0 (JD 2415020.0), T=-1, obliquity ~23.4523 */
    double obl = mean_obliquity(2415020.0);
    TEST_ASSERT_FLOAT_WITHIN(OBL_EPSILON, 23.4523, obl);
}

void test_obliquity_decreasing_recent_centuries(void) {
    /* Obliquity has been decreasing over recent centuries */
    double obl_1900 = mean_obliquity(2415020.0);
    double obl_2000 = mean_obliquity(J2000_JD);
    double obl_2100 = mean_obliquity(J2000_JD + 36525.0);
    TEST_ASSERT_TRUE(obl_1900 > obl_2000);
    TEST_ASSERT_TRUE(obl_2000 > obl_2100);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_vernal_equinox);
    RUN_TEST(test_summer_solstice);
    RUN_TEST(test_autumnal_equinox);
    RUN_TEST(test_winter_solstice);
    RUN_TEST(test_ecliptic_north_pole);
    RUN_TEST(test_ra_always_in_range);
    RUN_TEST(test_antares_approximate);
    RUN_TEST(test_transform_is_pure);
    RUN_TEST(test_obliquity_at_j2000);
    RUN_TEST(test_obliquity_at_j1900);
    RUN_TEST(test_obliquity_decreasing_recent_centuries);
    return UNITY_END();
}
