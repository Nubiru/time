#include "../unity/unity.h"
#include "../../src/systems/astrology/zodiac.h"
#include <string.h>

void setUp(void) { }
void tearDown(void) { }

/* ===== zodiac_sign ===== */

void test_zodiac_sign_aries_start(void) {
    /* 0 degrees = Aries */
    TEST_ASSERT_EQUAL_INT(0, zodiac_sign(0.0));
}

void test_zodiac_sign_aries_end(void) {
    /* 29.99 degrees = still Aries */
    TEST_ASSERT_EQUAL_INT(0, zodiac_sign(29.99));
}

void test_zodiac_sign_taurus_start(void) {
    /* 30 degrees = Taurus */
    TEST_ASSERT_EQUAL_INT(1, zodiac_sign(30.0));
}

void test_zodiac_sign_pisces(void) {
    /* 350 degrees = Pisces (sign 11) */
    TEST_ASSERT_EQUAL_INT(11, zodiac_sign(350.0));
}

void test_zodiac_sign_capricorn(void) {
    /* 280 degrees = Capricorn (sign 9) */
    TEST_ASSERT_EQUAL_INT(9, zodiac_sign(280.0));
}

void test_zodiac_sign_wraps_360(void) {
    /* 360.0 should wrap to Aries (0) */
    TEST_ASSERT_EQUAL_INT(0, zodiac_sign(360.0));
}

/* ===== zodiac_sign_name ===== */

void test_zodiac_name_aries(void) {
    TEST_ASSERT_EQUAL_STRING("Aries", zodiac_sign_name(0));
}

void test_zodiac_name_pisces(void) {
    TEST_ASSERT_EQUAL_STRING("Pisces", zodiac_sign_name(11));
}

void test_zodiac_name_invalid(void) {
    TEST_ASSERT_EQUAL_STRING("?", zodiac_sign_name(12));
    TEST_ASSERT_EQUAL_STRING("?", zodiac_sign_name(-1));
}

/* ===== zodiac_degree ===== */

void test_zodiac_degree_start(void) {
    /* 0.0 degrees ecliptic = 0.0 within Aries */
    TEST_ASSERT_FLOAT_WITHIN(0.01, 0.0, zodiac_degree(0.0));
}

void test_zodiac_degree_mid_taurus(void) {
    /* 45.5 degrees ecliptic = 15.5 within Taurus */
    TEST_ASSERT_FLOAT_WITHIN(0.01, 15.5, zodiac_degree(45.5));
}

void test_zodiac_degree_end_of_sign(void) {
    /* 59.99 degrees ecliptic = 29.99 within Taurus */
    TEST_ASSERT_FLOAT_WITHIN(0.01, 29.99, zodiac_degree(59.99));
}

/* ===== zodiac_sign_symbol ===== */

void test_zodiac_symbol_aries(void) {
    const char *sym = zodiac_sign_symbol(0);
    TEST_ASSERT_NOT_NULL(sym);
    TEST_ASSERT_TRUE(strlen(sym) > 0);
}

void test_zodiac_symbol_invalid(void) {
    TEST_ASSERT_EQUAL_STRING("?", zodiac_sign_symbol(12));
}

/* ===== zodiac_decan ===== */

void test_zodiac_decan_first(void) {
    /* 0-9.99 degrees within sign = decan 1 */
    TEST_ASSERT_EQUAL_INT(1, zodiac_decan(5.0));
}

void test_zodiac_decan_second(void) {
    /* 10-19.99 degrees = decan 2 */
    TEST_ASSERT_EQUAL_INT(2, zodiac_decan(45.0));
}

void test_zodiac_decan_third(void) {
    /* 20-29.99 degrees = decan 3 */
    TEST_ASSERT_EQUAL_INT(3, zodiac_decan(85.0));
}

int main(void) {
    UNITY_BEGIN();
    /* zodiac_sign */
    RUN_TEST(test_zodiac_sign_aries_start);
    RUN_TEST(test_zodiac_sign_aries_end);
    RUN_TEST(test_zodiac_sign_taurus_start);
    RUN_TEST(test_zodiac_sign_pisces);
    RUN_TEST(test_zodiac_sign_capricorn);
    RUN_TEST(test_zodiac_sign_wraps_360);
    /* zodiac_sign_name */
    RUN_TEST(test_zodiac_name_aries);
    RUN_TEST(test_zodiac_name_pisces);
    RUN_TEST(test_zodiac_name_invalid);
    /* zodiac_degree */
    RUN_TEST(test_zodiac_degree_start);
    RUN_TEST(test_zodiac_degree_mid_taurus);
    RUN_TEST(test_zodiac_degree_end_of_sign);
    /* zodiac_sign_symbol */
    RUN_TEST(test_zodiac_symbol_aries);
    RUN_TEST(test_zodiac_symbol_invalid);
    /* zodiac_decan */
    RUN_TEST(test_zodiac_decan_first);
    RUN_TEST(test_zodiac_decan_second);
    RUN_TEST(test_zodiac_decan_third);
    return UNITY_END();
}
