#include "../unity/unity.h"
#include "../../src/math/sidereal.h"
#include <math.h>
#define DEG_EPSILON 0.001
#define DEG_EPSILON_LOOSE 0.01
void setUp(void) { }
void tearDown(void) { }
void test_gmst_meeus_example(void) { double gmst = gmst_degrees(2446895.5); TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 197.6932, gmst); }
void test_lst_washington(void) { double gmst = gmst_degrees(2446895.5); double lst = lst_degrees(gmst, -77.065); TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 120.6282, lst); }
void test_gmst_j2000(void) { double gmst = gmst_degrees(2451544.5); TEST_ASSERT_TRUE(gmst > 0.0 && gmst < 360.0); }
void test_lst_greenwich(void) { double lst = lst_degrees(197.6932, 0.0); TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 197.6932, lst); }
void test_lst_east(void) { double lst = lst_degrees(350.0, 20.0); TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 10.0, lst); }
void test_lst_dateline(void) { double lst = lst_degrees(100.0, 180.0); TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 280.0, lst); }
void test_gast_differs_from_gmst(void) { double jd = 2446895.5; double gmst = gmst_degrees(jd); double gast = gast_degrees(jd); double diff = fabs(gast - gmst); TEST_ASSERT_TRUE(diff < 0.02); }
void test_gast_washington_example(void) { double jd = 2446896.30625; double gast = gast_degrees(jd); TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON_LOOSE, 128.737, gast); }
void test_gast_lst_washington(void) { double jd = 2446896.30625; double gast = gast_degrees(jd); double lst = lst_degrees(gast, -77.065); TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON_LOOSE, 51.671, lst); }
void test_gmst_always_in_range(void) { double dates[] = {2440000.5, 2450000.5, 2460000.5, 2415020.5}; for (int i = 0; i < 4; i++) { double gmst = gmst_degrees(dates[i]); TEST_ASSERT_TRUE(gmst >= 0.0 && gmst < 360.0); } }
void test_gmst_increases_with_date(void) { double g1 = gmst_degrees(2451544.5); double g2 = gmst_degrees(2451545.5); double diff = g2 - g1; if (diff < 0.0) diff += 360.0; TEST_ASSERT_FLOAT_WITHIN(0.01, 0.9856, diff); }
void test_gmst_formula_at_j2000_0h(void) { double gmst = gmst_degrees(2451544.5); TEST_ASSERT_TRUE(gmst > 99.0 && gmst < 101.0); }
void test_gmst_is_pure(void) { double a = gmst_degrees(2446895.5); double b = gmst_degrees(2446895.5); TEST_ASSERT_EQUAL_FLOAT(a, b); }
void test_lst_negative_wraps(void) { double lst = lst_degrees(10.0, -20.0); TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 350.0, lst); }
void test_lst_full_circle(void) { double l0 = lst_degrees(100.0, 0.0); double l360 = lst_degrees(100.0, 360.0); TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, l0, l360); }
void test_lst_always_in_range(void) { double gsts[] = {0.0, 90.0, 180.0, 270.0, 359.9}; double lons[] = {-180.0, -90.0, 0.0, 90.0, 180.0}; for (int i = 0; i < 5; i++) { for (int j = 0; j < 5; j++) { double lst = lst_degrees(gsts[i], lons[j]); TEST_ASSERT_TRUE(lst >= 0.0); TEST_ASSERT_TRUE(lst < 360.0); } } }
void test_lst_west_is_earlier(void) { double le = lst_degrees(200.0, 30.0); double lw = lst_degrees(200.0, -30.0); double diff = le - lw; if (diff < 0.0) diff += 360.0; TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 60.0, diff); }
void test_gast_is_pure(void) { double a = gast_degrees(2446895.5); double b = gast_degrees(2446895.5); TEST_ASSERT_EQUAL_FLOAT(a, b); }
void test_gast_always_in_range(void) { double dates[] = {2440000.5, 2446895.5, 2451545.0, 2460000.0}; for (int i = 0; i < 4; i++) { double gast = gast_degrees(dates[i]); TEST_ASSERT_TRUE(gast >= 0.0); TEST_ASSERT_TRUE(gast < 360.0); } }
void test_gmst_1900_epoch(void) { double gmst = gmst_degrees(2415020.0); TEST_ASSERT_TRUE(gmst >= 0.0 && gmst < 360.0); }
void test_lst_tokyo(void) { double gmst = gmst_degrees(2446895.5); double lst = lst_degrees(gmst, 139.7); double expected = fmod(gmst + 139.7, 360.0); if (expected < 0.0) expected += 360.0; TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, expected, lst); }
void test_lst_sydney(void) { double lst = lst_degrees(100.0, 151.2); TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 251.2, lst); }
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_gmst_meeus_example); RUN_TEST(test_lst_washington); RUN_TEST(test_gmst_j2000); RUN_TEST(test_lst_greenwich); RUN_TEST(test_lst_east); RUN_TEST(test_lst_dateline); RUN_TEST(test_gast_differs_from_gmst); RUN_TEST(test_gast_washington_example); RUN_TEST(test_gast_lst_washington); RUN_TEST(test_gmst_always_in_range);
    RUN_TEST(test_gmst_increases_with_date); RUN_TEST(test_gmst_formula_at_j2000_0h); RUN_TEST(test_gmst_is_pure); RUN_TEST(test_lst_negative_wraps); RUN_TEST(test_lst_full_circle); RUN_TEST(test_lst_always_in_range); RUN_TEST(test_lst_west_is_earlier); RUN_TEST(test_gast_is_pure); RUN_TEST(test_gast_always_in_range); RUN_TEST(test_gmst_1900_epoch); RUN_TEST(test_lst_tokyo); RUN_TEST(test_lst_sydney);
    return UNITY_END();
}
