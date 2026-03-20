/* test_heartbeat_counter.c — Tests for global heartbeat counter.
 * TDD RED phase: all tests written before implementation.
 * Estimates total heartbeats of all humans since first Homo sapiens. */

#include "../../unity/unity.h"
#include "../../../src/systems/earth/heartbeat_counter.h"

#include <string.h>

void setUp(void) { }
void tearDown(void) { }

/* ===== hb_beats_per_second ===== */

void test_bps_2024_roughly_9_7_billion(void) {
    /* 8100M people * 72 bpm / 60 s = 9.72e9 */
    double bps = hb_beats_per_second(2024);
    TEST_ASSERT_FLOAT_WITHIN(1.0e9, 9.72e9, bps);
}

void test_bps_1_ce_roughly_360_million(void) {
    /* 300M people * 72 / 60 = 3.6e8 */
    double bps = hb_beats_per_second(1);
    TEST_ASSERT_FLOAT_WITHIN(0.5e8, 3.6e8, bps);
}

void test_bps_1900_between_1_ce_and_2024(void) {
    double bps_1    = hb_beats_per_second(1);
    double bps_1900 = hb_beats_per_second(1900);
    double bps_2024 = hb_beats_per_second(2024);
    TEST_ASSERT_TRUE(bps_1900 > bps_1);
    TEST_ASSERT_TRUE(bps_1900 < bps_2024);
}

void test_bps_always_positive(void) {
    TEST_ASSERT_TRUE(hb_beats_per_second(-300000) > 0.0);
    TEST_ASSERT_TRUE(hb_beats_per_second(-10000) > 0.0);
    TEST_ASSERT_TRUE(hb_beats_per_second(2024) > 0.0);
}

void test_bps_prehistoric_very_small(void) {
    /* Before -10000, population ~1 million -> ~1.2e6 bps */
    double bps = hb_beats_per_second(-50000);
    TEST_ASSERT_TRUE(bps < 1.0e8);
}

/* ===== hb_total_exabeats ===== */

void test_total_at_minus_300000_near_zero(void) {
    double total = hb_total_exabeats(-300000);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 0.0, total);
}

void test_total_at_2024_positive(void) {
    double total = hb_total_exabeats(2024);
    TEST_ASSERT_TRUE(total > 0.0);
}

void test_total_at_2024_in_reasonable_range(void) {
    /* Should be in the thousands of exabeats (rough estimate ~3500) */
    double total = hb_total_exabeats(2024);
    TEST_ASSERT_TRUE(total > 500.0);
    TEST_ASSERT_TRUE(total < 50000.0);
}

void test_total_monotonically_increasing(void) {
    int years[] = { -300000, -10000, -1000, 1, 1000, 1500, 1800, 1900, 1950, 2000, 2024 };
    int count = (int)(sizeof(years) / sizeof(years[0]));
    double prev = hb_total_exabeats(years[0]);
    for (int i = 1; i < count; i++) {
        double curr = hb_total_exabeats(years[i]);
        TEST_ASSERT_TRUE_MESSAGE(curr >= prev,
            "Cumulative exabeats must be monotonically non-decreasing");
        prev = curr;
    }
}

void test_total_2024_greater_than_2000(void) {
    TEST_ASSERT_TRUE(hb_total_exabeats(2024) > hb_total_exabeats(2000));
}

void test_total_2000_greater_than_1900(void) {
    TEST_ASSERT_TRUE(hb_total_exabeats(2000) > hb_total_exabeats(1900));
}

void test_total_1900_greater_than_1_ce(void) {
    TEST_ASSERT_TRUE(hb_total_exabeats(1900) > hb_total_exabeats(1));
}

void test_total_interpolation_midpoint(void) {
    /* Value between two anchor points should be between them */
    double at_1900 = hb_total_exabeats(1900);
    double at_1950 = hb_total_exabeats(1950);
    double at_1925 = hb_total_exabeats(1925);
    TEST_ASSERT_TRUE(at_1925 >= at_1900);
    TEST_ASSERT_TRUE(at_1925 <= at_1950);
}

/* ===== hb_compute ===== */

void test_compute_2024_all_fields_populated(void) {
    hb_counter_t c = hb_compute(2024);
    TEST_ASSERT_TRUE(c.total_exabeats > 0.0);
    TEST_ASSERT_TRUE(c.beats_per_second > 0.0);
    TEST_ASSERT_TRUE(c.beats_per_year > 0.0);
    TEST_ASSERT_TRUE(strlen(c.total_text) > 0);
    TEST_ASSERT_TRUE(strlen(c.rate_text) > 0);
    TEST_ASSERT_TRUE(strlen(c.poetic_text) > 0);
}

void test_compute_beats_per_year_consistent(void) {
    /* beats_per_year should be ~beats_per_second * seconds_per_year */
    hb_counter_t c = hb_compute(2024);
    double expected_bpy = c.beats_per_second * 365.25 * 24.0 * 3600.0;
    double ratio = c.beats_per_year / expected_bpy;
    TEST_ASSERT_FLOAT_WITHIN(0.01, 1.0, ratio);
}

void test_compute_total_text_contains_metric_prefix(void) {
    hb_counter_t c = hb_compute(2024);
    /* Should contain a metric prefix indication */
    int has_exa = (strstr(c.total_text, "exa") != NULL);
    int has_zetta = (strstr(c.total_text, "zetta") != NULL);
    int has_peta = (strstr(c.total_text, "peta") != NULL);
    int has_times = (strstr(c.total_text, "\xc3\x97") != NULL); /* UTF-8 × */
    TEST_ASSERT_TRUE(has_exa || has_zetta || has_peta || has_times);
}

void test_compute_rate_text_contains_billion(void) {
    hb_counter_t c = hb_compute(2024);
    TEST_ASSERT_NOT_NULL(strstr(c.rate_text, "billion"));
}

void test_compute_rate_text_contains_per_second(void) {
    hb_counter_t c = hb_compute(2024);
    TEST_ASSERT_NOT_NULL(strstr(c.rate_text, "per second"));
}

void test_compute_poetic_text_contains_hearts_or_beat(void) {
    hb_counter_t c = hb_compute(2024);
    int has_hearts = (strstr(c.poetic_text, "heart") != NULL);
    int has_beat = (strstr(c.poetic_text, "beat") != NULL);
    TEST_ASSERT_TRUE(has_hearts || has_beat);
}

void test_compute_1_ce_rate_text_contains_million(void) {
    hb_counter_t c = hb_compute(1);
    TEST_ASSERT_NOT_NULL(strstr(c.rate_text, "million"));
}

/* ===== hb_format ===== */

void test_format_produces_output(void) {
    hb_counter_t c = hb_compute(2024);
    char buf[512];
    int n = hb_format(&c, buf, 512);
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_TRUE(strlen(buf) > 0);
}

void test_format_null_counter_returns_zero(void) {
    char buf[64];
    int n = hb_format(NULL, buf, 64);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_format_null_buffer_returns_zero(void) {
    hb_counter_t c = hb_compute(2024);
    int n = hb_format(&c, NULL, 64);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_format_zero_size_returns_zero(void) {
    hb_counter_t c = hb_compute(2024);
    char buf[64];
    int n = hb_format(&c, buf, 0);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_format_small_buffer_truncates(void) {
    hb_counter_t c = hb_compute(2024);
    char buf[16];
    int n = hb_format(&c, buf, 16);
    /* Should still return something, capped to buffer */
    TEST_ASSERT_TRUE(n >= 0);
    TEST_ASSERT_TRUE(strlen(buf) < 16);
}

/* ===== Edge cases ===== */

void test_compute_far_future(void) {
    hb_counter_t c = hb_compute(3000);
    TEST_ASSERT_TRUE(c.total_exabeats > hb_total_exabeats(2024));
    TEST_ASSERT_TRUE(c.beats_per_second > 0.0);
}

void test_total_exabeats_text_max_define(void) {
    /* Verify HB_TEXT_MAX is defined and reasonable */
    TEST_ASSERT_TRUE(HB_TEXT_MAX >= 64);
    TEST_ASSERT_TRUE(HB_TEXT_MAX <= 1024);
}

/* --- Runner --- */

int main(void) {
    UNITY_BEGIN();

    /* beats per second */
    RUN_TEST(test_bps_2024_roughly_9_7_billion);
    RUN_TEST(test_bps_1_ce_roughly_360_million);
    RUN_TEST(test_bps_1900_between_1_ce_and_2024);
    RUN_TEST(test_bps_always_positive);
    RUN_TEST(test_bps_prehistoric_very_small);

    /* total exabeats */
    RUN_TEST(test_total_at_minus_300000_near_zero);
    RUN_TEST(test_total_at_2024_positive);
    RUN_TEST(test_total_at_2024_in_reasonable_range);
    RUN_TEST(test_total_monotonically_increasing);
    RUN_TEST(test_total_2024_greater_than_2000);
    RUN_TEST(test_total_2000_greater_than_1900);
    RUN_TEST(test_total_1900_greater_than_1_ce);
    RUN_TEST(test_total_interpolation_midpoint);

    /* compute */
    RUN_TEST(test_compute_2024_all_fields_populated);
    RUN_TEST(test_compute_beats_per_year_consistent);
    RUN_TEST(test_compute_total_text_contains_metric_prefix);
    RUN_TEST(test_compute_rate_text_contains_billion);
    RUN_TEST(test_compute_rate_text_contains_per_second);
    RUN_TEST(test_compute_poetic_text_contains_hearts_or_beat);
    RUN_TEST(test_compute_1_ce_rate_text_contains_million);

    /* format */
    RUN_TEST(test_format_produces_output);
    RUN_TEST(test_format_null_counter_returns_zero);
    RUN_TEST(test_format_null_buffer_returns_zero);
    RUN_TEST(test_format_zero_size_returns_zero);
    RUN_TEST(test_format_small_buffer_truncates);

    /* edge cases */
    RUN_TEST(test_compute_far_future);
    RUN_TEST(test_total_exabeats_text_max_define);

    return UNITY_END();
}
