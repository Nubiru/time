/* test_time_entropy.c — Tests for information-theoretic time analysis.
 *
 * Tests te_binary_entropy, te_entropy, te_interpret. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/time_entropy.h"
#include "../../../src/math/julian.h"
#include <math.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* Helper: JD for a Gregorian date */
static double jd_from(int y, int m, int d) {
    return gregorian_to_jd(y, m, (double)d);
}

/* ===================================================================
 * te_binary_entropy — pure math
 * =================================================================== */

void test_binary_entropy_all_zero(void) {
    /* 0 out of N significant => H = 0 */
    double h = te_binary_entropy(0, 21);
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 0.0, h);
}

void test_binary_entropy_all_one(void) {
    /* N out of N significant => H = 0 */
    double h = te_binary_entropy(21, 21);
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 0.0, h);
}

void test_binary_entropy_half(void) {
    /* Half significant => maximum binary entropy = 1.0 bit */
    double h = te_binary_entropy(10, 20);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 1.0, h);
}

void test_binary_entropy_one_of_many(void) {
    /* 1 out of 21: low entropy, but non-zero */
    double h = te_binary_entropy(1, 21);
    TEST_ASSERT_TRUE(h > 0.0);
    TEST_ASSERT_TRUE(h < 1.0);
}

void test_binary_entropy_symmetric(void) {
    /* H(k, n) == H(n-k, n) */
    double h1 = te_binary_entropy(3, 21);
    double h2 = te_binary_entropy(18, 21);
    TEST_ASSERT_FLOAT_WITHIN(0.0001, h1, h2);
}

void test_binary_entropy_zero_total(void) {
    double h = te_binary_entropy(0, 0);
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 0.0, h);
}

void test_binary_entropy_negative_total(void) {
    double h = te_binary_entropy(0, -1);
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 0.0, h);
}

void test_binary_entropy_monotonic_to_half(void) {
    /* Entropy should increase from 0 toward half */
    double prev = 0.0;
    for (int k = 1; k <= 10; k++) {
        double h = te_binary_entropy(k, 20);
        TEST_ASSERT_TRUE(h >= prev);
        prev = h;
    }
}

/* ===================================================================
 * te_entropy — JD-based entropy
 * =================================================================== */

void test_entropy_returns_nonneg(void) {
    double h = te_entropy(jd_from(2026, 3, 20), NULL);
    TEST_ASSERT_TRUE(h >= 0.0);
}

void test_entropy_result_filled(void) {
    te_result_t r;
    te_entropy(jd_from(2026, 3, 20), &r);
    TEST_ASSERT_TRUE(r.total_systems == CD_SYS_COUNT);
    TEST_ASSERT_TRUE(r.significant_count >= 0);
    TEST_ASSERT_TRUE(r.significant_count <= CD_SYS_COUNT);
}

void test_entropy_max_entropy_positive(void) {
    te_result_t r;
    te_entropy(jd_from(2026, 6, 15), &r);
    TEST_ASSERT_TRUE(r.max_entropy > 0.0);
    /* max_entropy = log2(CD_SYS_COUNT) */
    double expected = log2((double)CD_SYS_COUNT);
    TEST_ASSERT_FLOAT_WITHIN(0.001, expected, r.max_entropy);
}

void test_entropy_normalized_bounded(void) {
    te_result_t r;
    te_entropy(jd_from(2026, 3, 20), &r);
    TEST_ASSERT_TRUE(r.normalized >= 0.0);
    TEST_ASSERT_TRUE(r.normalized <= 1.0);
}

void test_entropy_null_out_safe(void) {
    /* Should not crash with NULL out */
    double h = te_entropy(jd_from(2026, 1, 1), NULL);
    TEST_ASSERT_TRUE(h >= 0.0);
}

void test_entropy_consistency(void) {
    /* Entropy result should match binary_entropy of the sig count */
    te_result_t r;
    te_entropy(jd_from(2026, 7, 4), &r);
    double expected = te_binary_entropy(r.significant_count, r.total_systems);
    TEST_ASSERT_FLOAT_WITHIN(0.0001, expected, r.entropy);
}

/* ===================================================================
 * te_interpret
 * =================================================================== */

void test_interpret_ordered(void) {
    TEST_ASSERT_EQUAL_STRING("ordered", te_interpret(0.0));
    TEST_ASSERT_EQUAL_STRING("ordered", te_interpret(0.14));
}

void test_interpret_structured(void) {
    TEST_ASSERT_EQUAL_STRING("structured", te_interpret(0.15));
    TEST_ASSERT_EQUAL_STRING("structured", te_interpret(0.34));
}

void test_interpret_balanced(void) {
    TEST_ASSERT_EQUAL_STRING("balanced", te_interpret(0.35));
    TEST_ASSERT_EQUAL_STRING("balanced", te_interpret(0.64));
}

void test_interpret_complex(void) {
    TEST_ASSERT_EQUAL_STRING("complex", te_interpret(0.65));
    TEST_ASSERT_EQUAL_STRING("complex", te_interpret(1.0));
}

/* ===================================================================
 * Runner
 * =================================================================== */

int main(void) {
    UNITY_BEGIN();
    /* binary_entropy */
    RUN_TEST(test_binary_entropy_all_zero);
    RUN_TEST(test_binary_entropy_all_one);
    RUN_TEST(test_binary_entropy_half);
    RUN_TEST(test_binary_entropy_one_of_many);
    RUN_TEST(test_binary_entropy_symmetric);
    RUN_TEST(test_binary_entropy_zero_total);
    RUN_TEST(test_binary_entropy_negative_total);
    RUN_TEST(test_binary_entropy_monotonic_to_half);
    /* te_entropy */
    RUN_TEST(test_entropy_returns_nonneg);
    RUN_TEST(test_entropy_result_filled);
    RUN_TEST(test_entropy_max_entropy_positive);
    RUN_TEST(test_entropy_normalized_bounded);
    RUN_TEST(test_entropy_null_out_safe);
    RUN_TEST(test_entropy_consistency);
    /* te_interpret */
    RUN_TEST(test_interpret_ordered);
    RUN_TEST(test_interpret_structured);
    RUN_TEST(test_interpret_balanced);
    RUN_TEST(test_interpret_complex);
    return UNITY_END();
}
