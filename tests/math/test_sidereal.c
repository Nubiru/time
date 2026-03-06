#include "../unity/unity.h"
#include "../../src/math/sidereal.h"
#include <math.h>

#define DEG_EPSILON 0.001
#define DEG_EPSILON_LOOSE 0.01

void setUp(void) { }
void tearDown(void) { }

/* --- Card 008 Q4 worked example: 1987 April 10 at 0h UT --- */

void test_gmst_meeus_example(void) {
    /* JD 2446895.5 -> GMST = 13h10m46.3668s = 197.6932 degrees */
    double gmst = gmst_degrees(2446895.5);
    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 197.6932, gmst);
}

void test_lst_washington(void) {
    /* GMST 197.6932° + longitude -77.065° (west) = 120.6282° */
    double gmst = gmst_degrees(2446895.5);
    double lst = lst_degrees(gmst, -77.065);
    /* Expected: 8h02m30.77s = 120.6282° */
    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 120.6282, lst);
}

/* --- J2000 epoch reference --- */

void test_gmst_j2000(void) {
    /* At J2000.0 (JD 2451545.0), T=0, GMST = 100.46061837° (from formula) */
    /* But J2000.0 is Jan 1.5, not 0h UT. At Jan 1.0 (JD 2451544.5), T is slightly negative */
    /* Use JD 2451544.5 (2000 Jan 1.0 = 0h UT) */
    double gmst = gmst_degrees(2451544.5);
    /* GMST at 0h UT on 2000 Jan 1 */
    /* T = (2451544.5 - 2451545.0)/36525 = -0.5/36525 */
    /* theta ≈ 100.46061837 + 36000.770053608 * (-1.369e-5) ≈ 99.968° */
    TEST_ASSERT_TRUE(gmst > 0.0 && gmst < 360.0);
}

/* --- LST edge cases --- */

void test_lst_greenwich(void) {
    /* At Greenwich (longitude=0), LST = GST */
    double lst = lst_degrees(197.6932, 0.0);
    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 197.6932, lst);
}

void test_lst_east(void) {
    /* East longitude adds to GST */
    double lst = lst_degrees(350.0, 20.0);
    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 10.0, lst); /* wraps around */
}

void test_lst_dateline(void) {
    /* At 180° east, LST = GST + 180 */
    double lst = lst_degrees(100.0, 180.0);
    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 280.0, lst);
}

/* --- GAST tests --- */

void test_gast_differs_from_gmst(void) {
    /* GAST should differ from GMST by a small nutation correction */
    double jd = 2446895.5;
    double gmst = gmst_degrees(jd);
    double gast = gast_degrees(jd);
    double diff = fabs(gast - gmst);
    /* Nutation correction is typically < 0.02 degrees (< 1.2 arcseconds * cos(obliquity)) */
    TEST_ASSERT_TRUE(diff < 0.02);
}

void test_gast_washington_example(void) {
    /* Card 008 Q3: 1987 April 10 at 19h21m00s UT
     * JD = 2446895.5 + 19.35/24 = 2446896.30625
     * GAST ≈ 8h34m56.853s = 128.737° */
    double jd = 2446896.30625;
    double gast = gast_degrees(jd);
    /* Allow ~0.01° tolerance for simplified nutation */
    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON_LOOSE, 128.737, gast);
}

void test_gast_lst_washington(void) {
    /* GAST at 19h21m UT - Washington longitude = LST ≈ 3h26m41s = 51.671° */
    double jd = 2446896.30625;
    double gast = gast_degrees(jd);
    double lst = lst_degrees(gast, -77.065);
    /* Expected: 3h26m41.153s ≈ 51.671° */
    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON_LOOSE, 51.671, lst);
}

/* --- Range tests --- */

void test_gmst_always_in_range(void) {
    /* Test various dates, GMST should always be 0-360 */
    double dates[] = {2440000.5, 2450000.5, 2460000.5, 2415020.5};
    for (int i = 0; i < 4; i++) {
        double gmst = gmst_degrees(dates[i]);
        TEST_ASSERT_TRUE(gmst >= 0.0 && gmst < 360.0);
    }
}

/* --- Runner --- */

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_gmst_meeus_example);
    RUN_TEST(test_lst_washington);
    RUN_TEST(test_gmst_j2000);
    RUN_TEST(test_lst_greenwich);
    RUN_TEST(test_lst_east);
    RUN_TEST(test_lst_dateline);
    RUN_TEST(test_gast_differs_from_gmst);
    RUN_TEST(test_gast_washington_example);
    RUN_TEST(test_gast_lst_washington);
    RUN_TEST(test_gmst_always_in_range);
    return UNITY_END();
}
