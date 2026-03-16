#include "../unity/unity.h"
#include "../../src/systems/astrology/houses.h"

void setUp(void) { }
void tearDown(void) { }

/* ===== houses_whole_sign ===== */

void test_houses_aries_ascending(void) {
    house_system_t h = houses_whole_sign(15.0);
    TEST_ASSERT_EQUAL_INT(0, h.signs[0]);
    TEST_ASSERT_EQUAL_INT(1, h.signs[1]);
    TEST_ASSERT_EQUAL_INT(11, h.signs[11]);
}

void test_houses_taurus_ascending(void) {
    house_system_t h = houses_whole_sign(45.0);
    TEST_ASSERT_EQUAL_INT(1, h.signs[0]);
    TEST_ASSERT_EQUAL_INT(7, h.signs[6]);
}

void test_houses_pisces_wraps(void) {
    house_system_t h = houses_whole_sign(350.0);
    TEST_ASSERT_EQUAL_INT(11, h.signs[0]);
    TEST_ASSERT_EQUAL_INT(0, h.signs[1]);
}

void test_houses_cusp_aries(void) {
    house_system_t h = houses_whole_sign(15.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 0.0, h.cusps[0]);
}

void test_houses_cusp_taurus(void) {
    house_system_t h = houses_whole_sign(45.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 30.0, h.cusps[0]);
}

void test_houses_all_signs_present(void) {
    house_system_t h = houses_whole_sign(100.0);
    int seen[12] = {0};
    for (int i = 0; i < 12; i++) {
        TEST_ASSERT_TRUE(h.signs[i] >= 0 && h.signs[i] <= 11);
        seen[h.signs[i]]++;
    }
    for (int i = 0; i < 12; i++) {
        TEST_ASSERT_EQUAL_INT(1, seen[i]);
    }
}

void test_houses_signs_consecutive(void) {
    house_system_t h = houses_whole_sign(200.0);
    for (int i = 1; i < 12; i++) {
        TEST_ASSERT_EQUAL_INT((h.signs[i - 1] + 1) % 12, h.signs[i]);
    }
}

void test_houses_ascendant_stored(void) {
    house_system_t h = houses_whole_sign(123.456);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 123.456, h.ascendant);
}

void test_houses_cusps_multiples_of_30(void) {
    house_system_t h = houses_whole_sign(77.7);
    for (int i = 0; i < 12; i++) {
        double rem = h.cusps[i] - 30.0 * (int)(h.cusps[i] / 30.0);
        TEST_ASSERT_FLOAT_WITHIN(0.01, 0.0, rem);
    }
}

void test_houses_purity(void) {
    house_system_t a = houses_whole_sign(222.0);
    house_system_t b = houses_whole_sign(222.0);
    TEST_ASSERT_EQUAL_INT(a.signs[0], b.signs[0]);
    TEST_ASSERT_EQUAL_INT(a.signs[6], b.signs[6]);
    TEST_ASSERT_EQUAL_FLOAT(a.cusps[0], b.cusps[0]);
    TEST_ASSERT_EQUAL_FLOAT(a.ascendant, b.ascendant);
}

void test_houses_leo_ascending(void) {
    house_system_t h = houses_whole_sign(130.0);
    TEST_ASSERT_EQUAL_INT(4, h.signs[0]);
    TEST_ASSERT_EQUAL_INT(7, h.signs[3]);
    TEST_ASSERT_EQUAL_INT(1, h.signs[9]);
}

/* === NEW TESTS === */

void test_houses_gemini_ascending(void) {
    house_system_t h = houses_whole_sign(75.0);
    TEST_ASSERT_EQUAL_INT(2, h.signs[0]);
    TEST_ASSERT_EQUAL_INT(8, h.signs[6]);
}

void test_houses_cancer_ascending(void) {
    house_system_t h = houses_whole_sign(95.0);
    TEST_ASSERT_EQUAL_INT(3, h.signs[0]);
    TEST_ASSERT_EQUAL_INT(0, h.signs[9]);
}

void test_houses_virgo_ascending(void) {
    house_system_t h = houses_whole_sign(170.0);
    TEST_ASSERT_EQUAL_INT(5, h.signs[0]);
}

void test_houses_scorpio_ascending(void) {
    house_system_t h = houses_whole_sign(220.0);
    TEST_ASSERT_EQUAL_INT(7, h.signs[0]);
}

void test_houses_sagittarius_ascending(void) {
    house_system_t h = houses_whole_sign(250.0);
    TEST_ASSERT_EQUAL_INT(8, h.signs[0]);
}

void test_houses_aquarius_ascending(void) {
    house_system_t h = houses_whole_sign(310.0);
    TEST_ASSERT_EQUAL_INT(10, h.signs[0]);
}

void test_houses_at_exact_0_degrees(void) {
    house_system_t h = houses_whole_sign(0.0);
    TEST_ASSERT_EQUAL_INT(0, h.signs[0]);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 0.0, h.cusps[0]);
}

void test_houses_at_exact_30_degrees(void) {
    house_system_t h = houses_whole_sign(30.0);
    TEST_ASSERT_EQUAL_INT(1, h.signs[0]);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 30.0, h.cusps[0]);
}

void test_houses_at_exact_29_999(void) {
    house_system_t h = houses_whole_sign(29.999);
    TEST_ASSERT_EQUAL_INT(0, h.signs[0]);
}

void test_houses_at_exact_359(void) {
    house_system_t h = houses_whole_sign(359.0);
    TEST_ASSERT_EQUAL_INT(11, h.signs[0]);
}

void test_houses_cusps_sequential(void) {
    house_system_t h = houses_whole_sign(150.0);
    for (int i = 1; i < 12; i++) {
        double expected = h.cusps[i - 1] + 30.0;
        if (expected >= 360.0) expected -= 360.0;
        TEST_ASSERT_FLOAT_WITHIN(0.01, expected, h.cusps[i]);
    }
}

void test_houses_cusp_6th_house(void) {
    house_system_t h = houses_whole_sign(15.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 150.0, h.cusps[5]);
}

void test_houses_cusp_wraps_correctly(void) {
    house_system_t h = houses_whole_sign(350.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 330.0, h.cusps[0]);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 0.0, h.cusps[1]);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 30.0, h.cusps[2]);
}

void test_houses_7th_house_opposite(void) {
    for (int lon = 0; lon < 360; lon += 30) {
        house_system_t h = houses_whole_sign((double)lon);
        TEST_ASSERT_EQUAL_INT((h.signs[0] + 6) % 12, h.signs[6]);
    }
}

void test_houses_all_twelve_ascending(void) {
    for (int sign = 0; sign < 12; sign++) {
        double lon = sign * 30.0 + 15.0;
        house_system_t h = houses_whole_sign(lon);
        TEST_ASSERT_EQUAL_INT(sign, h.signs[0]);
    }
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_houses_aries_ascending);
    RUN_TEST(test_houses_taurus_ascending);
    RUN_TEST(test_houses_pisces_wraps);
    RUN_TEST(test_houses_cusp_aries);
    RUN_TEST(test_houses_cusp_taurus);
    RUN_TEST(test_houses_all_signs_present);
    RUN_TEST(test_houses_signs_consecutive);
    RUN_TEST(test_houses_ascendant_stored);
    RUN_TEST(test_houses_cusps_multiples_of_30);
    RUN_TEST(test_houses_purity);
    RUN_TEST(test_houses_leo_ascending);
    RUN_TEST(test_houses_gemini_ascending);
    RUN_TEST(test_houses_cancer_ascending);
    RUN_TEST(test_houses_virgo_ascending);
    RUN_TEST(test_houses_scorpio_ascending);
    RUN_TEST(test_houses_sagittarius_ascending);
    RUN_TEST(test_houses_aquarius_ascending);
    RUN_TEST(test_houses_at_exact_0_degrees);
    RUN_TEST(test_houses_at_exact_30_degrees);
    RUN_TEST(test_houses_at_exact_29_999);
    RUN_TEST(test_houses_at_exact_359);
    RUN_TEST(test_houses_cusps_sequential);
    RUN_TEST(test_houses_cusp_6th_house);
    RUN_TEST(test_houses_cusp_wraps_correctly);
    RUN_TEST(test_houses_7th_house_opposite);
    RUN_TEST(test_houses_all_twelve_ascending);
    return UNITY_END();
}
