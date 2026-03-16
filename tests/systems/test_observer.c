#include "../unity/unity.h"
#include "../../src/systems/astrology/observer.h"

#define DEG_EPSILON 1.0   /* 1 degree tolerance for ascendant */
#define OBL 23.44         /* approximate J2000 obliquity */

void setUp(void) { }
void tearDown(void) { }

/* ===== ascendant_longitude ===== */

void test_asc_lst0_equator(void) {
    double asc = ascendant_longitude(0.0, OBL, 0.0);
    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 270.0, asc);
}

void test_asc_lst90_equator(void) {
    double asc = ascendant_longitude(90.0, OBL, 0.0);
    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 0.0, asc);
}

void test_asc_lst180_equator(void) {
    double asc = ascendant_longitude(180.0, OBL, 0.0);
    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 90.0, asc);
}

void test_asc_lst270_equator(void) {
    double asc = ascendant_longitude(270.0, OBL, 0.0);
    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 180.0, asc);
}

void test_asc_different_latitudes(void) {
    double asc_eq = ascendant_longitude(45.0, OBL, 0.0);
    double asc_40 = ascendant_longitude(45.0, OBL, 40.0);
    double diff = asc_eq - asc_40;
    if (diff < 0.0) diff = -diff;
    TEST_ASSERT_TRUE(diff > 1.0);
}

void test_asc_always_in_range(void) {
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
    double asc1 = ascendant_longitude(30.0, OBL, 45.0);
    double asc2 = ascendant_longitude(60.0, OBL, 45.0);
    double diff = asc1 - asc2;
    if (diff < 0.0) diff = -diff;
    TEST_ASSERT_TRUE(diff > 1.0);
}

void test_asc_non_cardinal_equator(void) {
    double asc = ascendant_longitude(45.0, OBL, 0.0);
    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 312.5, asc);
}

void test_asc_purity(void) {
    double a = ascendant_longitude(123.4, OBL, 37.8);
    double b = ascendant_longitude(123.4, OBL, 37.8);
    TEST_ASSERT_EQUAL_FLOAT(a, b);
}

void test_asc_london_differs_from_equator(void) {
    double asc_london = ascendant_longitude(0.0, OBL, 51.5);
    double asc_equator = ascendant_longitude(0.0, OBL, 0.0);
    double diff = asc_london - asc_equator;
    if (diff < 0.0) diff = -diff;
    TEST_ASSERT_TRUE(diff > 5.0);
}

void test_asc_southern_hemisphere(void) {
    double asc = ascendant_longitude(90.0, OBL, -33.9);
    TEST_ASSERT_TRUE(asc >= 0.0);
    TEST_ASSERT_TRUE(asc < 360.0);
}

/* === NEW TESTS === */

void test_asc_new_york(void) {
    for (int lst = 0; lst < 360; lst += 30) {
        double asc = ascendant_longitude((double)lst, OBL, 40.71);
        TEST_ASSERT_TRUE(asc >= 0.0);
        TEST_ASSERT_TRUE(asc < 360.0);
    }
}

void test_asc_tokyo(void) {
    double asc = ascendant_longitude(120.0, OBL, 35.7);
    TEST_ASSERT_TRUE(asc >= 0.0);
    TEST_ASSERT_TRUE(asc < 360.0);
}

void test_asc_buenos_aires(void) {
    double asc = ascendant_longitude(200.0, OBL, -34.6);
    TEST_ASSERT_TRUE(asc >= 0.0);
    TEST_ASSERT_TRUE(asc < 360.0);
}

void test_asc_north_pole(void) {
    double asc = ascendant_longitude(45.0, OBL, 89.0);
    TEST_ASSERT_TRUE(asc >= 0.0);
    TEST_ASSERT_TRUE(asc < 360.0);
}

void test_asc_south_pole(void) {
    double asc = ascendant_longitude(45.0, OBL, -89.0);
    TEST_ASSERT_TRUE(asc >= 0.0);
    TEST_ASSERT_TRUE(asc < 360.0);
}

void test_asc_zero_obliquity(void) {
    double asc = ascendant_longitude(90.0, 0.0, 0.0);
    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 0.0, asc);
}

void test_asc_varies_with_obliquity(void) {
    double asc1 = ascendant_longitude(60.0, 20.0, 30.0);
    double asc2 = ascendant_longitude(60.0, 25.0, 30.0);
    double diff = asc1 - asc2;
    if (diff < 0.0) diff = -diff;
    TEST_ASSERT_TRUE(diff > 0.1);
}

void test_asc_full_lst_sweep_equator(void) {
    double min_asc = 360.0;
    double max_asc = 0.0;
    for (int lst = 0; lst < 360; lst += 5) {
        double asc = ascendant_longitude((double)lst, OBL, 0.0);
        if (asc < min_asc) min_asc = asc;
        if (asc > max_asc) max_asc = asc;
    }
    TEST_ASSERT_TRUE(max_asc - min_asc > 200.0);
}

void test_asc_full_lst_sweep_london(void) {
    for (int lst = 0; lst < 360; lst += 10) {
        double asc = ascendant_longitude((double)lst, OBL, 51.5);
        TEST_ASSERT_TRUE(asc >= 0.0);
        TEST_ASSERT_TRUE(asc < 360.0);
    }
}

void test_asc_equator_180_apart(void) {
    double asc0 = ascendant_longitude(0.0, OBL, 0.0);
    double asc180 = ascendant_longitude(180.0, OBL, 0.0);
    double diff = asc180 - asc0;
    if (diff < 0.0) diff += 360.0;
    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 180.0, diff);
}

void test_asc_equator_90_apart(void) {
    double asc0 = ascendant_longitude(0.0, OBL, 0.0);
    double asc90 = ascendant_longitude(90.0, OBL, 0.0);
    double diff = asc90 - asc0;
    if (diff < 0.0) diff += 360.0;
    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 90.0, diff);
}

void test_asc_continuous_at_equator(void) {
    double prev = ascendant_longitude(0.0, OBL, 0.0);
    for (int lst = 1; lst <= 360; lst++) {
        double curr = ascendant_longitude((double)lst, OBL, 0.0);
        double diff = curr - prev;
        if (diff > 180.0) diff -= 360.0;
        if (diff < -180.0) diff += 360.0;
        TEST_ASSERT_TRUE(diff > -15.0 && diff < 15.0);
        prev = curr;
    }
}

void test_asc_multiple_latitudes_same_lst(void) {
    double lats[] = {0.0, 20.0, 40.0, 60.0};
    double prev = ascendant_longitude(45.0, OBL, lats[0]);
    for (int i = 1; i < 4; i++) {
        double curr = ascendant_longitude(45.0, OBL, lats[i]);
        double diff = curr - prev;
        if (diff < 0.0) diff = -diff;
        if (diff > 180.0) diff = 360.0 - diff;
        TEST_ASSERT_TRUE(diff > 0.5);
        prev = curr;
    }
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
    RUN_TEST(test_asc_new_york);
    RUN_TEST(test_asc_tokyo);
    RUN_TEST(test_asc_buenos_aires);
    RUN_TEST(test_asc_north_pole);
    RUN_TEST(test_asc_south_pole);
    RUN_TEST(test_asc_zero_obliquity);
    RUN_TEST(test_asc_varies_with_obliquity);
    RUN_TEST(test_asc_full_lst_sweep_equator);
    RUN_TEST(test_asc_full_lst_sweep_london);
    RUN_TEST(test_asc_equator_180_apart);
    RUN_TEST(test_asc_equator_90_apart);
    RUN_TEST(test_asc_continuous_at_equator);
    RUN_TEST(test_asc_multiple_latitudes_same_lst);
    return UNITY_END();
}
