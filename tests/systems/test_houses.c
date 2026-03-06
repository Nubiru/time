#include "../unity/unity.h"
#include "../../src/systems/astrology/houses.h"

void setUp(void) { }
void tearDown(void) { }

/* ===== houses_whole_sign ===== */

void test_houses_aries_ascending(void) {
    /* ASC at 15° Aries: house 1=Aries, 2=Taurus, ..., 12=Pisces */
    house_system_t h = houses_whole_sign(15.0);
    TEST_ASSERT_EQUAL_INT(0, h.signs[0]);   /* Aries */
    TEST_ASSERT_EQUAL_INT(1, h.signs[1]);   /* Taurus */
    TEST_ASSERT_EQUAL_INT(11, h.signs[11]); /* Pisces */
}

void test_houses_taurus_ascending(void) {
    /* ASC at 45° (Taurus): house 1=Taurus, house 7=Scorpio */
    house_system_t h = houses_whole_sign(45.0);
    TEST_ASSERT_EQUAL_INT(1, h.signs[0]);  /* Taurus */
    TEST_ASSERT_EQUAL_INT(7, h.signs[6]);  /* Scorpio */
}

void test_houses_pisces_wraps(void) {
    /* ASC at 350° (Pisces): house 1=Pisces, house 2=Aries (wraps) */
    house_system_t h = houses_whole_sign(350.0);
    TEST_ASSERT_EQUAL_INT(11, h.signs[0]); /* Pisces */
    TEST_ASSERT_EQUAL_INT(0, h.signs[1]);  /* Aries */
}

void test_houses_cusp_aries(void) {
    /* cusps[0] for Aries ascendant = 0.0 */
    house_system_t h = houses_whole_sign(15.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 0.0, h.cusps[0]);
}

void test_houses_cusp_taurus(void) {
    /* cusps[0] for Taurus ascendant = 30.0 */
    house_system_t h = houses_whole_sign(45.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 30.0, h.cusps[0]);
}

void test_houses_all_signs_present(void) {
    /* All 12 signs represented exactly once */
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
    /* Each sign is 1 more than the previous (mod 12) */
    house_system_t h = houses_whole_sign(200.0);
    for (int i = 1; i < 12; i++) {
        TEST_ASSERT_EQUAL_INT((h.signs[i - 1] + 1) % 12, h.signs[i]);
    }
}

void test_houses_ascendant_stored(void) {
    /* ascendant field matches input */
    house_system_t h = houses_whole_sign(123.456);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 123.456, h.ascendant);
}

void test_houses_cusps_multiples_of_30(void) {
    /* Every cusp is a multiple of 30 */
    house_system_t h = houses_whole_sign(77.7);
    for (int i = 0; i < 12; i++) {
        double rem = h.cusps[i] - 30.0 * (int)(h.cusps[i] / 30.0);
        TEST_ASSERT_FLOAT_WITHIN(0.01, 0.0, rem);
    }
}

void test_houses_purity(void) {
    /* Same input always gives same output */
    house_system_t a = houses_whole_sign(222.0);
    house_system_t b = houses_whole_sign(222.0);
    TEST_ASSERT_EQUAL_INT(a.signs[0], b.signs[0]);
    TEST_ASSERT_EQUAL_INT(a.signs[6], b.signs[6]);
    TEST_ASSERT_EQUAL_FLOAT(a.cusps[0], b.cusps[0]);
    TEST_ASSERT_EQUAL_FLOAT(a.ascendant, b.ascendant);
}

void test_houses_leo_ascending(void) {
    /* ASC at 130° (Leo): house 1=Leo(4), house 4=Scorpio(7), house 10=Taurus(1) */
    house_system_t h = houses_whole_sign(130.0);
    TEST_ASSERT_EQUAL_INT(4, h.signs[0]);  /* Leo */
    TEST_ASSERT_EQUAL_INT(7, h.signs[3]);  /* Scorpio */
    TEST_ASSERT_EQUAL_INT(1, h.signs[9]);  /* Taurus */
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
    return UNITY_END();
}
