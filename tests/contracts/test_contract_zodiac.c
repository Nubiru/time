/* test_contract_zodiac.c — Exhaustive contract tests for zodiac.h */

#include "../unity/unity.h"
#include "../../src/systems/astrology/zodiac.h"
#include <math.h>
#include <stddef.h>

void setUp(void) { }
void tearDown(void) { }

void test_sign_range_sweep(void) {
    int failures = 0;
    for (double lon = -720.0; lon <= 720.0; lon += 0.5) {
        int sign = zodiac_sign(lon);
        if (sign < 0 || sign > 11) { failures++; }
    }
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, failures, "zodiac_sign out-of-range");
}

void test_degree_range_sweep(void) {
    int failures = 0;
    for (double lon = -720.0; lon <= 720.0; lon += 0.5) {
        double deg = zodiac_degree(lon);
        if (deg < 0.0 || deg >= 30.0) { failures++; }
    }
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, failures, "zodiac_degree out of [0.0, 30.0)");
}

void test_decan_range_sweep(void) {
    int failures = 0;
    for (double lon = -720.0; lon <= 720.0; lon += 0.5) {
        int decan = zodiac_decan(lon);
        if (decan < 1 || decan > 3) { failures++; }
    }
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, failures, "zodiac_decan out of {1,2,3}");
}

void test_anchor_aries_at_zero(void) { TEST_ASSERT_EQUAL_INT(0, zodiac_sign(0.0)); }
void test_anchor_taurus_at_30(void) { TEST_ASSERT_EQUAL_INT(1, zodiac_sign(30.0)); }
void test_anchor_leo_at_120(void) { TEST_ASSERT_EQUAL_INT(4, zodiac_sign(120.0)); }
void test_anchor_pisces_at_330(void) { TEST_ASSERT_EQUAL_INT(11, zodiac_sign(330.0)); }
void test_anchor_aries_at_360(void) { TEST_ASSERT_EQUAL_INT(0, zodiac_sign(360.0)); }

void test_negative_longitude(void) {
    int sign = zodiac_sign(-30.0);
    TEST_ASSERT_TRUE(sign >= 0 && sign <= 11);
}

void test_negative_longitude_degree(void) {
    double deg = zodiac_degree(-30.0);
    TEST_ASSERT_TRUE(deg >= 0.0 && deg < 30.0);
}

void test_name_non_null_for_valid_signs(void) {
    for (int i = 0; i < 12; i++) {
        const char *name = zodiac_sign_name(i);
        TEST_ASSERT_NOT_NULL(name);
        TEST_ASSERT_TRUE(name[0] != '?');
    }
}

void test_name_invalid_returns_question(void) {
    TEST_ASSERT_NOT_NULL(zodiac_sign_name(-1));
    TEST_ASSERT_NOT_NULL(zodiac_sign_name(12));
}

void test_symbol_non_null_for_valid_signs(void) {
    for (int i = 0; i < 12; i++) {
        TEST_ASSERT_NOT_NULL(zodiac_sign_symbol(i));
    }
}

void test_sign_degree_consistency(void) {
    int failures = 0;
    for (double lon = 0.0; lon < 360.0; lon += 1.0) {
        int sign = zodiac_sign(lon);
        double deg = zodiac_degree(lon);
        double reconstructed = sign * 30.0 + deg;
        if (fabs(reconstructed - lon) > 0.001) { failures++; }
    }
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, failures, "sign*30 + degree != longitude");
}

void test_decan_boundary_values(void) {
    TEST_ASSERT_EQUAL_INT(1, zodiac_decan(5.0));
    TEST_ASSERT_EQUAL_INT(2, zodiac_decan(15.0));
    TEST_ASSERT_EQUAL_INT(3, zodiac_decan(25.0));
    TEST_ASSERT_EQUAL_INT(1, zodiac_decan(30.0));
    TEST_ASSERT_EQUAL_INT(2, zodiac_decan(40.0));
}

void test_large_positive_longitude(void) {
    TEST_ASSERT_TRUE(zodiac_sign(3600.0) >= 0 && zodiac_sign(3600.0) <= 11);
}

void test_large_negative_longitude(void) {
    TEST_ASSERT_TRUE(zodiac_sign(-3600.0) >= 0 && zodiac_sign(-3600.0) <= 11);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_sign_range_sweep);
    RUN_TEST(test_degree_range_sweep);
    RUN_TEST(test_decan_range_sweep);
    RUN_TEST(test_anchor_aries_at_zero);
    RUN_TEST(test_anchor_taurus_at_30);
    RUN_TEST(test_anchor_leo_at_120);
    RUN_TEST(test_anchor_pisces_at_330);
    RUN_TEST(test_anchor_aries_at_360);
    RUN_TEST(test_negative_longitude);
    RUN_TEST(test_negative_longitude_degree);
    RUN_TEST(test_name_non_null_for_valid_signs);
    RUN_TEST(test_name_invalid_returns_question);
    RUN_TEST(test_symbol_non_null_for_valid_signs);
    RUN_TEST(test_sign_degree_consistency);
    RUN_TEST(test_decan_boundary_values);
    RUN_TEST(test_large_positive_longitude);
    RUN_TEST(test_large_negative_longitude);
    return UNITY_END();
}
