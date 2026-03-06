#include "../unity/unity.h"
#include "../../src/systems/astrology/observer.h"

#define DEG_EPSILON 1.0   /* 1 degree tolerance for ascendant */
#define OBL 23.44         /* approximate J2000 obliquity */

void setUp(void) { }
void tearDown(void) { }

/* ===== ascendant_longitude ===== */

void test_asc_lst0_equator(void) {
    /* LST=0, lat=0: ASC = atan2(-1, 0) = -90 -> 270 (Capricorn) */
    double asc = ascendant_longitude(0.0, OBL, 0.0);
    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 270.0, asc);
}

void test_asc_lst90_equator(void) {
    /* LST=90, lat=0: ASC = atan2(0, cos(obl)) = 0 (Aries) */
    double asc = ascendant_longitude(90.0, OBL, 0.0);
    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 0.0, asc);
}

void test_asc_lst180_equator(void) {
    /* LST=180, lat=0: ASC = atan2(1, 0) = 90 (Cancer) */
    double asc = ascendant_longitude(180.0, OBL, 0.0);
    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 90.0, asc);
}

void test_asc_lst270_equator(void) {
    /* LST=270, lat=0: ASC = atan2(0, -cos(obl)) = 180 (Libra) */
    double asc = ascendant_longitude(270.0, OBL, 0.0);
    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 180.0, asc);
}

void test_asc_different_latitudes(void) {
    /* Same LST, different latitudes -> different ascendants */
    double asc_eq = ascendant_longitude(45.0, OBL, 0.0);
    double asc_40 = ascendant_longitude(45.0, OBL, 40.0);
    double diff = asc_eq - asc_40;
    if (diff < 0.0) diff = -diff;
    TEST_ASSERT_TRUE(diff > 1.0);
}

void test_asc_always_in_range(void) {
    /* Ascendant must be in [0, 360) for various inputs */
    double lsts[] = {0, 30, 60, 90, 120, 150, 180, 210, 240, 270, 300, 330};
    double lats[] = {0, 20, 40, -20, -40, 51.5, -33.9};
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 7; j++) {
            double asc = ascendant_longitude(lsts[i], OBL, lats[j]);
            TEST_ASSERT_TRUE(asc >= 0.0);
            TEST_ASSERT_TRUE(asc < 360.0);
        }
    }
}

void test_asc_changes_with_lst(void) {
    /* Two different LSTs at same location give different ascendants */
    double asc1 = ascendant_longitude(30.0, OBL, 45.0);
    double asc2 = ascendant_longitude(60.0, OBL, 45.0);
    double diff = asc1 - asc2;
    if (diff < 0.0) diff = -diff;
    TEST_ASSERT_TRUE(diff > 1.0);
}

void test_asc_non_cardinal_equator(void) {
    /* LST=45, lat=0, obl=23.44 -> ASC ~ 312.5 */
    double asc = ascendant_longitude(45.0, OBL, 0.0);
    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 312.5, asc);
}

void test_asc_purity(void) {
    /* Same inputs always produce same output */
    double a = ascendant_longitude(123.4, OBL, 37.8);
    double b = ascendant_longitude(123.4, OBL, 37.8);
    TEST_ASSERT_EQUAL_FLOAT(a, b);
}

void test_asc_london_differs_from_equator(void) {
    /* London (lat=51.5) at LST=0 differs from equator at LST=0 */
    double asc_london = ascendant_longitude(0.0, OBL, 51.5);
    double asc_equator = ascendant_longitude(0.0, OBL, 0.0);
    double diff = asc_london - asc_equator;
    if (diff < 0.0) diff = -diff;
    TEST_ASSERT_TRUE(diff > 5.0);
}

void test_asc_southern_hemisphere(void) {
    /* Southern hemisphere should also produce valid results */
    double asc = ascendant_longitude(90.0, OBL, -33.9);
    TEST_ASSERT_TRUE(asc >= 0.0);
    TEST_ASSERT_TRUE(asc < 360.0);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_asc_lst0_equator);
    RUN_TEST(test_asc_lst90_equator);
    RUN_TEST(test_asc_lst180_equator);
    RUN_TEST(test_asc_lst270_equator);
    RUN_TEST(test_asc_different_latitudes);
    RUN_TEST(test_asc_always_in_range);
    RUN_TEST(test_asc_changes_with_lst);
    RUN_TEST(test_asc_non_cardinal_equator);
    RUN_TEST(test_asc_purity);
    RUN_TEST(test_asc_london_differs_from_equator);
    RUN_TEST(test_asc_southern_hemisphere);
    return UNITY_END();
}
