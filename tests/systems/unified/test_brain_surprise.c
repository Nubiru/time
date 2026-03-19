/* test_brain_surprise.c — Tests for convergence anomaly detector.
 *
 * Tests baseline computation, z-score calculation, and surprise detection. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/brain_surprise.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* 2024-03-20 spring equinox */
static const double TEST_JD = 2460389.5;

/* ===================================================================
 * Baseline computation
 * =================================================================== */

void test_baseline_mean_in_range(void) {
    double mean = 0.0, stddev = 0.0;
    br_surprise_baseline(TEST_JD, 100, 7, &mean, &stddev);
    TEST_ASSERT_TRUE(mean >= 0.0);
    TEST_ASSERT_TRUE(mean <= 1.0);
}

void test_baseline_stddev_non_negative(void) {
    double mean = 0.0, stddev = 0.0;
    br_surprise_baseline(TEST_JD, 100, 7, &mean, &stddev);
    TEST_ASSERT_TRUE(stddev >= 0.0);
}

void test_baseline_stddev_less_than_mean_range(void) {
    double mean = 0.0, stddev = 0.0;
    br_surprise_baseline(TEST_JD, 100, 7, &mean, &stddev);
    /* stddev should be less than the full range (0-1) */
    TEST_ASSERT_TRUE(stddev <= 1.0);
}

void test_baseline_null_safe(void) {
    br_surprise_baseline(TEST_JD, 100, 7, NULL, NULL);
    /* should not crash */
}

/* ===================================================================
 * Surprise detection
 * =================================================================== */

void test_detect_null_out_returns_zero(void) {
    TEST_ASSERT_EQUAL_INT(0, br_surprise_detect(TEST_JD, 2.0, NULL));
}

void test_detect_fills_significance(void) {
    br_surprise_t out;
    br_surprise_detect(TEST_JD, 2.0, &out);
    TEST_ASSERT_TRUE(out.significance >= 0.0);
    TEST_ASSERT_TRUE(out.significance <= 1.0);
}

void test_detect_fills_baseline(void) {
    br_surprise_t out;
    br_surprise_detect(TEST_JD, 2.0, &out);
    TEST_ASSERT_TRUE(out.baseline_mean >= 0.0);
    TEST_ASSERT_TRUE(out.baseline_stddev >= 0.0);
}

void test_detect_z_score_reasonable(void) {
    br_surprise_t out;
    br_surprise_detect(TEST_JD, 2.0, &out);
    /* z-score should be finite and not absurdly large */
    TEST_ASSERT_TRUE(out.z_score > -100.0);
    TEST_ASSERT_TRUE(out.z_score < 100.0);
}

void test_detect_headline_not_empty(void) {
    br_surprise_t out;
    br_surprise_detect(TEST_JD, 2.0, &out);
    TEST_ASSERT_TRUE(strlen(out.headline) > 0);
}

void test_detect_is_surprise_binary(void) {
    br_surprise_t out;
    br_surprise_detect(TEST_JD, 2.0, &out);
    TEST_ASSERT_TRUE(out.is_surprise == 0 || out.is_surprise == 1);
}

void test_detect_high_threshold_less_surprises(void) {
    br_surprise_t low, high;
    br_surprise_detect(TEST_JD, 1.0, &low);
    br_surprise_detect(TEST_JD, 5.0, &high);
    /* A higher threshold should result in equal or fewer surprises */
    if (high.is_surprise) {
        TEST_ASSERT_TRUE(low.is_surprise);
    }
}

/* ===================================================================
 * Quick check
 * =================================================================== */

void test_quick_check_returns_binary(void) {
    int result = br_surprise_check(TEST_JD);
    TEST_ASSERT_TRUE(result == 0 || result == 1);
}

/* ===================================================================
 * Surprise headline content
 * =================================================================== */

void test_surprise_headline_mentions_average(void) {
    br_surprise_t out;
    br_surprise_detect(TEST_JD, 99.0, &out); /* very high threshold → not surprise */
    TEST_ASSERT_NOT_NULL(strstr(out.headline, "Average"));
}

void test_surprise_headline_mentions_sigma_when_flagged(void) {
    br_surprise_t out;
    br_surprise_detect(TEST_JD, 0.001, &out); /* very low threshold → likely surprise */
    if (out.is_surprise) {
        TEST_ASSERT_NOT_NULL(strstr(out.headline, "sigma"));
    }
}

/* ===================================================================
 * Main
 * =================================================================== */

int main(void) {
    UNITY_BEGIN();

    /* Baseline */
    RUN_TEST(test_baseline_mean_in_range);
    RUN_TEST(test_baseline_stddev_non_negative);
    RUN_TEST(test_baseline_stddev_less_than_mean_range);
    RUN_TEST(test_baseline_null_safe);

    /* Detection */
    RUN_TEST(test_detect_null_out_returns_zero);
    RUN_TEST(test_detect_fills_significance);
    RUN_TEST(test_detect_fills_baseline);
    RUN_TEST(test_detect_z_score_reasonable);
    RUN_TEST(test_detect_headline_not_empty);
    RUN_TEST(test_detect_is_surprise_binary);
    RUN_TEST(test_detect_high_threshold_less_surprises);

    /* Quick check */
    RUN_TEST(test_quick_check_returns_binary);

    /* Headlines */
    RUN_TEST(test_surprise_headline_mentions_average);
    RUN_TEST(test_surprise_headline_mentions_sigma_when_flagged);

    return UNITY_END();
}
