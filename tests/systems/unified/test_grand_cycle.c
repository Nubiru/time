/* test_grand_cycle.c — Tests for Grand Cycle period calculator.
 *
 * Tests gc_gcd, gc_lcm, gc_lcm_array, gc_get_cycles,
 * gc_compute, gc_compute_subset. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/grand_cycle.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===================================================================
 * gc_gcd
 * =================================================================== */

void test_gcd_basic(void) {
    TEST_ASSERT_EQUAL_INT64(6, gc_gcd(12, 18));
}

void test_gcd_coprime(void) {
    TEST_ASSERT_EQUAL_INT64(1, gc_gcd(7, 13));
}

void test_gcd_same(void) {
    TEST_ASSERT_EQUAL_INT64(42, gc_gcd(42, 42));
}

void test_gcd_one_is_one(void) {
    TEST_ASSERT_EQUAL_INT64(1, gc_gcd(1, 100));
}

void test_gcd_with_zero(void) {
    TEST_ASSERT_EQUAL_INT64(5, gc_gcd(5, 0));
    TEST_ASSERT_EQUAL_INT64(5, gc_gcd(0, 5));
}

void test_gcd_negative_handled(void) {
    TEST_ASSERT_EQUAL_INT64(6, gc_gcd(-12, 18));
    TEST_ASSERT_EQUAL_INT64(6, gc_gcd(12, -18));
}

void test_gcd_large_values(void) {
    /* GCD(18980, 21915) — Calendar Round and Sexagenary Cycle */
    int64_t g = gc_gcd(18980, 21915);
    TEST_ASSERT_TRUE(g > 0);
    TEST_ASSERT_EQUAL_INT64(0, 18980 % g);
    TEST_ASSERT_EQUAL_INT64(0, 21915 % g);
}

/* ===================================================================
 * gc_lcm
 * =================================================================== */

void test_lcm_basic(void) {
    TEST_ASSERT_EQUAL_INT64(36, gc_lcm(12, 18));
}

void test_lcm_coprime(void) {
    TEST_ASSERT_EQUAL_INT64(91, gc_lcm(7, 13));
}

void test_lcm_same(void) {
    TEST_ASSERT_EQUAL_INT64(42, gc_lcm(42, 42));
}

void test_lcm_with_zero(void) {
    TEST_ASSERT_EQUAL_INT64(0, gc_lcm(0, 5));
    TEST_ASSERT_EQUAL_INT64(0, gc_lcm(5, 0));
}

void test_lcm_one(void) {
    TEST_ASSERT_EQUAL_INT64(100, gc_lcm(1, 100));
}

void test_lcm_tzolkin_haab(void) {
    /* LCM(260, 365) = 18980 (Calendar Round) */
    TEST_ASSERT_EQUAL_INT64(18980, gc_lcm(260, 365));
}

/* ===================================================================
 * gc_lcm_array
 * =================================================================== */

void test_lcm_array_single(void) {
    int64_t p[] = { 42 };
    TEST_ASSERT_EQUAL_INT64(42, gc_lcm_array(p, 1));
}

void test_lcm_array_two(void) {
    int64_t p[] = { 260, 365 };
    TEST_ASSERT_EQUAL_INT64(18980, gc_lcm_array(p, 2));
}

void test_lcm_array_three(void) {
    int64_t p[] = { 6, 10, 64 };
    /* LCM(6,10) = 30, LCM(30,64) = 960 */
    TEST_ASSERT_EQUAL_INT64(960, gc_lcm_array(p, 3));
}

void test_lcm_array_null(void) {
    TEST_ASSERT_EQUAL_INT64(0, gc_lcm_array(NULL, 3));
}

void test_lcm_array_zero_count(void) {
    int64_t p[] = { 42 };
    TEST_ASSERT_EQUAL_INT64(0, gc_lcm_array(p, 0));
}

void test_lcm_array_divisible(void) {
    /* All divide evenly: LCM should be the largest */
    int64_t p[] = { 2, 4, 8, 16 };
    TEST_ASSERT_EQUAL_INT64(16, gc_lcm_array(p, 4));
}

/* ===================================================================
 * gc_get_cycles
 * =================================================================== */

void test_get_cycles_returns_count(void) {
    gc_cycle_t buf[GC_MAX_CYCLES];
    int n = gc_get_cycles(buf, GC_MAX_CYCLES);
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_TRUE(n <= GC_MAX_CYCLES);
}

void test_get_cycles_first_is_tzolkin(void) {
    gc_cycle_t buf[GC_MAX_CYCLES];
    gc_get_cycles(buf, GC_MAX_CYCLES);
    TEST_ASSERT_EQUAL_STRING("Tzolkin", buf[0].name);
    TEST_ASSERT_EQUAL_INT64(260, buf[0].period_days);
}

void test_get_cycles_null_safe(void) {
    TEST_ASSERT_EQUAL_INT(0, gc_get_cycles(NULL, 10));
}

void test_get_cycles_limited(void) {
    gc_cycle_t buf[2];
    int n = gc_get_cycles(buf, 2);
    TEST_ASSERT_EQUAL_INT(2, n);
}

void test_get_cycles_all_positive_periods(void) {
    gc_cycle_t buf[GC_MAX_CYCLES];
    int n = gc_get_cycles(buf, GC_MAX_CYCLES);
    for (int i = 0; i < n; i++) {
        TEST_ASSERT_TRUE(buf[i].period_days > 0);
    }
}

/* ===================================================================
 * gc_compute
 * =================================================================== */

void test_compute_null_safe(void) {
    TEST_ASSERT_EQUAL_INT(0, gc_compute(NULL));
}

void test_compute_returns_success(void) {
    gc_result_t r;
    TEST_ASSERT_EQUAL_INT(1, gc_compute(&r));
}

void test_compute_lcm_positive(void) {
    gc_result_t r;
    gc_compute(&r);
    TEST_ASSERT_TRUE(r.lcm_days > 0);
}

void test_compute_lcm_divisible_by_all(void) {
    gc_result_t r;
    gc_compute(&r);

    gc_cycle_t buf[GC_MAX_CYCLES];
    int n = gc_get_cycles(buf, GC_MAX_CYCLES);

    for (int i = 0; i < n; i++) {
        if (buf[i].scale == 1) {
            TEST_ASSERT_EQUAL_INT64(0, r.lcm_days % buf[i].period_days);
        }
    }
}

void test_compute_years_consistent(void) {
    gc_result_t r;
    gc_compute(&r);
    double expected_years = (double)r.lcm_days / 365.25;
    TEST_ASSERT_FLOAT_WITHIN(0.01, expected_years, r.lcm_years);
}

void test_compute_cycle_count(void) {
    gc_result_t r;
    gc_compute(&r);
    TEST_ASSERT_TRUE(r.cycle_count >= 9);  /* at least the 9 built-in */
}

void test_compute_has_description(void) {
    gc_result_t r;
    gc_compute(&r);
    TEST_ASSERT_NOT_NULL(r.description);
    TEST_ASSERT_TRUE(strlen(r.description) > 0);
}

/* ===================================================================
 * gc_compute_subset
 * =================================================================== */

void test_subset_null_out(void) {
    gc_cycle_t c = { "Test", 10, 1 };
    TEST_ASSERT_EQUAL_INT(0, gc_compute_subset(&c, 1, NULL));
}

void test_subset_null_cycles(void) {
    gc_result_t r;
    TEST_ASSERT_EQUAL_INT(0, gc_compute_subset(NULL, 1, &r));
    TEST_ASSERT_EQUAL_INT64(0, r.lcm_days);
}

void test_subset_zero_count(void) {
    gc_cycle_t c = { "Test", 10, 1 };
    gc_result_t r;
    TEST_ASSERT_EQUAL_INT(0, gc_compute_subset(&c, 0, &r));
}

void test_subset_single(void) {
    gc_cycle_t c = { "Test", 42, 1 };
    gc_result_t r;
    gc_compute_subset(&c, 1, &r);
    TEST_ASSERT_EQUAL_INT64(42, r.lcm_days);
    TEST_ASSERT_EQUAL_INT(1, r.cycle_count);
}

void test_subset_two_cycles(void) {
    gc_cycle_t c[] = {
        { "A", 260, 1 },
        { "B", 365, 1 },
    };
    gc_result_t r;
    gc_compute_subset(c, 2, &r);
    TEST_ASSERT_EQUAL_INT64(18980, r.lcm_days);
}

void test_subset_scaled_cycles(void) {
    /* 29.53 days as 2953 hundredths, and 365 days */
    gc_cycle_t c[] = {
        { "Synodic Month", 2953, 100 },
        { "Haab",           365,   1 },
    };
    gc_result_t r;
    gc_compute_subset(c, 2, &r);
    TEST_ASSERT_TRUE(r.lcm_days > 0);
    /* Must be divisible by 365 */
    TEST_ASSERT_EQUAL_INT64(0, r.lcm_days % 365);
}

/* ===================================================================
 * Runner
 * =================================================================== */

int main(void) {
    UNITY_BEGIN();
    /* gcd */
    RUN_TEST(test_gcd_basic);
    RUN_TEST(test_gcd_coprime);
    RUN_TEST(test_gcd_same);
    RUN_TEST(test_gcd_one_is_one);
    RUN_TEST(test_gcd_with_zero);
    RUN_TEST(test_gcd_negative_handled);
    RUN_TEST(test_gcd_large_values);
    /* lcm */
    RUN_TEST(test_lcm_basic);
    RUN_TEST(test_lcm_coprime);
    RUN_TEST(test_lcm_same);
    RUN_TEST(test_lcm_with_zero);
    RUN_TEST(test_lcm_one);
    RUN_TEST(test_lcm_tzolkin_haab);
    /* lcm_array */
    RUN_TEST(test_lcm_array_single);
    RUN_TEST(test_lcm_array_two);
    RUN_TEST(test_lcm_array_three);
    RUN_TEST(test_lcm_array_null);
    RUN_TEST(test_lcm_array_zero_count);
    RUN_TEST(test_lcm_array_divisible);
    /* get_cycles */
    RUN_TEST(test_get_cycles_returns_count);
    RUN_TEST(test_get_cycles_first_is_tzolkin);
    RUN_TEST(test_get_cycles_null_safe);
    RUN_TEST(test_get_cycles_limited);
    RUN_TEST(test_get_cycles_all_positive_periods);
    /* compute */
    RUN_TEST(test_compute_null_safe);
    RUN_TEST(test_compute_returns_success);
    RUN_TEST(test_compute_lcm_positive);
    RUN_TEST(test_compute_lcm_divisible_by_all);
    RUN_TEST(test_compute_years_consistent);
    RUN_TEST(test_compute_cycle_count);
    RUN_TEST(test_compute_has_description);
    /* compute_subset */
    RUN_TEST(test_subset_null_out);
    RUN_TEST(test_subset_null_cycles);
    RUN_TEST(test_subset_zero_count);
    RUN_TEST(test_subset_single);
    RUN_TEST(test_subset_two_cycles);
    RUN_TEST(test_subset_scaled_cycles);
    return UNITY_END();
}
