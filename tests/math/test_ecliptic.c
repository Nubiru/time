#include "../unity/unity.h"
#include "../../src/math/ecliptic.h"
#include <math.h>
#define DEG_EPSILON 0.1
#define APPROX_DEG  5.0
#define OBL_EPSILON 0.001
#define J2000_OBL 23.439291111
#define J2000_JD  2451545.0
void setUp(void) { }
void tearDown(void) { }
void test_vernal_equinox(void) { equatorial_t eq = ecliptic_to_equatorial(0.0, 0.0, J2000_OBL); TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 0.0, eq.ra); TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 0.0, eq.dec); }
void test_summer_solstice(void) { equatorial_t eq = ecliptic_to_equatorial(90.0, 0.0, J2000_OBL); TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 90.0, eq.ra); TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, J2000_OBL, eq.dec); }
void test_autumnal_equinox(void) { equatorial_t eq = ecliptic_to_equatorial(180.0, 0.0, J2000_OBL); TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 180.0, eq.ra); TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 0.0, eq.dec); }
void test_winter_solstice(void) { equatorial_t eq = ecliptic_to_equatorial(270.0, 0.0, J2000_OBL); TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 270.0, eq.ra); TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, -J2000_OBL, eq.dec); }
void test_ecliptic_north_pole(void) { equatorial_t eq = ecliptic_to_equatorial(0.0, 90.0, J2000_OBL); TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 270.0, eq.ra); TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 90.0 - J2000_OBL, eq.dec); }
void test_ra_always_in_range(void) { double lons[] = {0, 45, 90, 135, 180, 225, 270, 315, 359.9}; double lats[] = {0, 30, -30, 60, -60}; for (int i = 0; i < 9; i++) { for (int j = 0; j < 5; j++) { equatorial_t eq = ecliptic_to_equatorial(lons[i], lats[j], J2000_OBL); TEST_ASSERT_TRUE(eq.ra >= 0.0); TEST_ASSERT_TRUE(eq.ra < 360.0); } } }
void test_antares_approximate(void) { equatorial_t eq = ecliptic_to_equatorial(247.96, -4.57, J2000_OBL); TEST_ASSERT_FLOAT_WITHIN(APPROX_DEG, 247.35, eq.ra); TEST_ASSERT_FLOAT_WITHIN(APPROX_DEG, -26.43, eq.dec); }
void test_transform_is_pure(void) { equatorial_t a = ecliptic_to_equatorial(123.456, 12.34, J2000_OBL); equatorial_t b = ecliptic_to_equatorial(123.456, 12.34, J2000_OBL); TEST_ASSERT_EQUAL_FLOAT(a.ra, b.ra); TEST_ASSERT_EQUAL_FLOAT(a.dec, b.dec); }
void test_obliquity_at_j2000(void) { double obl = mean_obliquity(J2000_JD); TEST_ASSERT_FLOAT_WITHIN(OBL_EPSILON, 23.4393, obl); }
void test_obliquity_at_j1900(void) { double obl = mean_obliquity(2415020.0); TEST_ASSERT_FLOAT_WITHIN(OBL_EPSILON, 23.4523, obl); }
void test_obliquity_decreasing_recent_centuries(void) { double o1 = mean_obliquity(2415020.0); double o2 = mean_obliquity(J2000_JD); double o3 = mean_obliquity(J2000_JD + 36525.0); TEST_ASSERT_TRUE(o1 > o2); TEST_ASSERT_TRUE(o2 > o3); }
void test_ecliptic_south_pole(void) { equatorial_t eq = ecliptic_to_equatorial(0.0, -90.0, J2000_OBL); TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 90.0, eq.ra); TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, -(90.0 - J2000_OBL), eq.dec); }
void test_ecliptic_45_degrees(void) { equatorial_t eq = ecliptic_to_equatorial(45.0, 0.0, J2000_OBL); TEST_ASSERT_TRUE(eq.ra >= 0.0 && eq.ra < 360.0); TEST_ASSERT_TRUE(eq.ra < 45.0); }
void test_ecliptic_135_degrees(void) { equatorial_t eq = ecliptic_to_equatorial(135.0, 0.0, J2000_OBL); TEST_ASSERT_TRUE(eq.ra > 135.0); }
void test_dec_never_exceeds_90(void) { double lons[] = {0, 45, 90, 135, 180, 225, 270, 315}; double lats[] = {-89, -60, -30, 0, 30, 60, 89}; for (int i = 0; i < 8; i++) { for (int j = 0; j < 7; j++) { equatorial_t eq = ecliptic_to_equatorial(lons[i], lats[j], J2000_OBL); TEST_ASSERT_TRUE(eq.dec >= -90.0); TEST_ASSERT_TRUE(eq.dec <= 90.0); } } }
void test_equinox_lat0_dec0(void) { equatorial_t e1 = ecliptic_to_equatorial(0.0, 0.0, J2000_OBL); equatorial_t e2 = ecliptic_to_equatorial(180.0, 0.0, J2000_OBL); TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 0.0, e1.dec); TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 0.0, e2.dec); }
void test_zero_obliquity(void) { equatorial_t eq = ecliptic_to_equatorial(123.0, 45.0, 0.0); TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 123.0, eq.ra); TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 45.0, eq.dec); }
void test_zero_obliquity_negative_lat(void) { equatorial_t eq = ecliptic_to_equatorial(200.0, -30.0, 0.0); TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 200.0, eq.ra); TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, -30.0, eq.dec); }
void test_ecliptic_negative_longitude(void) { equatorial_t en = ecliptic_to_equatorial(-30.0, 0.0, J2000_OBL); equatorial_t ep = ecliptic_to_equatorial(330.0, 0.0, J2000_OBL); TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, ep.ra, en.ra); TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, ep.dec, en.dec); }
void test_obliquity_at_j2100(void) { double obl = mean_obliquity(J2000_JD + 36525.0); TEST_ASSERT_FLOAT_WITHIN(OBL_EPSILON, 23.4263, obl); }
void test_obliquity_always_positive(void) { double d[] = {2415020.0, 2430000.0, 2451545.0, 2460000.0, 2470000.0}; for (int i = 0; i < 5; i++) { double obl = mean_obliquity(d[i]); TEST_ASSERT_TRUE(obl > 22.0); TEST_ASSERT_TRUE(obl < 24.0); } }
void test_obliquity_is_pure(void) { double a = mean_obliquity(2451545.0); double b = mean_obliquity(2451545.0); TEST_ASSERT_EQUAL_FLOAT(a, b); }
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_vernal_equinox); RUN_TEST(test_summer_solstice); RUN_TEST(test_autumnal_equinox); RUN_TEST(test_winter_solstice); RUN_TEST(test_ecliptic_north_pole); RUN_TEST(test_ra_always_in_range); RUN_TEST(test_antares_approximate); RUN_TEST(test_transform_is_pure); RUN_TEST(test_obliquity_at_j2000); RUN_TEST(test_obliquity_at_j1900); RUN_TEST(test_obliquity_decreasing_recent_centuries);
    RUN_TEST(test_ecliptic_south_pole); RUN_TEST(test_ecliptic_45_degrees); RUN_TEST(test_ecliptic_135_degrees); RUN_TEST(test_dec_never_exceeds_90); RUN_TEST(test_equinox_lat0_dec0); RUN_TEST(test_zero_obliquity); RUN_TEST(test_zero_obliquity_negative_lat); RUN_TEST(test_ecliptic_negative_longitude); RUN_TEST(test_obliquity_at_j2100); RUN_TEST(test_obliquity_always_positive); RUN_TEST(test_obliquity_is_pure);
    return UNITY_END();
}
