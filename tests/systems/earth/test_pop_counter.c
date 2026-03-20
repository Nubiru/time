/* test_pop_counter.c — Tests for live population counter.
 * TDD RED phase: all tests written before implementation.
 * Smooth per-frame population estimate from growth rates. */

#include "../../unity/unity.h"
#include "../../../src/systems/earth/pop_counter.h"

#include <string.h>

void setUp(void) { }
void tearDown(void) { }

/* ===== pc_estimate ===== */

void test_estimate_2024_roughly_8_billion(void) {
    double pop = pc_estimate(2024, 0.0);
    TEST_ASSERT_TRUE(pop >= 8.0e9);
    TEST_ASSERT_TRUE(pop <= 8.2e9);
}

void test_estimate_next_day_is_more(void) {
    double day0 = pc_estimate(2024, 0.0);
    double day1 = pc_estimate(2024, 86400.0);
    TEST_ASSERT_TRUE(day1 > day0);
}

void test_estimate_increases_with_seconds(void) {
    double t0 = pc_estimate(2024, 0.0);
    double t1 = pc_estimate(2024, 1.0);
    double t2 = pc_estimate(2024, 2.0);
    TEST_ASSERT_TRUE(t1 > t0);
    TEST_ASSERT_TRUE(t2 > t1);
}

void test_estimate_1900_roughly_1_6_billion(void) {
    double pop = pc_estimate(1900, 0.0);
    TEST_ASSERT_TRUE(pop >= 1.4e9);
    TEST_ASSERT_TRUE(pop <= 1.8e9);
}

void test_estimate_1950_roughly_2_5_billion(void) {
    double pop = pc_estimate(1950, 0.0);
    TEST_ASSERT_TRUE(pop >= 2.0e9);
    TEST_ASSERT_TRUE(pop <= 3.0e9);
}

void test_estimate_2000_roughly_6_billion(void) {
    double pop = pc_estimate(2000, 0.0);
    /* pop_alive_at_year(2000) interpolates between 1999(6000) and 2011(7000) */
    TEST_ASSERT_TRUE(pop >= 5.5e9);
    TEST_ASSERT_TRUE(pop <= 7.0e9);
}

void test_estimate_zero_offset_equals_base(void) {
    /* With 0 seconds offset, should just be base population */
    double pop = pc_estimate(2024, 0.0);
    double base = 8100.0 * 1e6; /* pop_alive_at_year(2024) = 8100 millions */
    TEST_ASSERT_FLOAT_WITHIN(1e6, base, pop);
}

/* ===== pc_growth_rate ===== */

void test_growth_rate_2024_roughly_2_to_3(void) {
    double rate = pc_growth_rate(2024);
    TEST_ASSERT_TRUE(rate >= 2.0);
    TEST_ASSERT_TRUE(rate <= 3.5);
}

void test_growth_rate_positive(void) {
    /* Modern population is growing */
    TEST_ASSERT_TRUE(pc_growth_rate(2024) > 0.0);
    TEST_ASSERT_TRUE(pc_growth_rate(1950) > 0.0);
    TEST_ASSERT_TRUE(pc_growth_rate(1900) > 0.0);
}

void test_growth_rate_1900_different_from_2024(void) {
    double r1900 = pc_growth_rate(1900);
    double r2024 = pc_growth_rate(2024);
    /* Different eras have different growth rates */
    TEST_ASSERT_TRUE(r1900 != r2024);
}

/* ===== pc_compute ===== */

void test_compute_populates_all_fields(void) {
    pc_counter_t c = pc_compute(2024, 0.0);
    TEST_ASSERT_TRUE(c.population > 0.0);
    TEST_ASSERT_TRUE(c.births_per_second > 0.0);
    TEST_ASSERT_TRUE(c.deaths_per_second > 0.0);
    TEST_ASSERT_TRUE(c.net_per_second > 0.0);
    TEST_ASSERT_TRUE(c.net_per_day > 0.0);
    TEST_ASSERT_TRUE(strlen(c.count_text) > 0);
    TEST_ASSERT_TRUE(strlen(c.rate_text) > 0);
    TEST_ASSERT_TRUE(strlen(c.daily_text) > 0);
}

void test_compute_births_greater_than_deaths(void) {
    pc_counter_t c = pc_compute(2024, 0.0);
    TEST_ASSERT_TRUE(c.births_per_second > c.deaths_per_second);
}

void test_compute_net_is_births_minus_deaths(void) {
    pc_counter_t c = pc_compute(2024, 0.0);
    double expected = c.births_per_second - c.deaths_per_second;
    TEST_ASSERT_FLOAT_WITHIN(0.001, expected, c.net_per_second);
}

void test_compute_net_per_day_range(void) {
    pc_counter_t c = pc_compute(2024, 0.0);
    TEST_ASSERT_TRUE(c.net_per_day >= 150000.0);
    TEST_ASSERT_TRUE(c.net_per_day <= 300000.0);
}

void test_compute_net_per_day_is_net_per_second_times_86400(void) {
    pc_counter_t c = pc_compute(2024, 0.0);
    double expected = c.net_per_second * 86400.0;
    TEST_ASSERT_FLOAT_WITHIN(1.0, expected, c.net_per_day);
}

void test_compute_count_text_has_commas(void) {
    pc_counter_t c = pc_compute(2024, 0.0);
    /* 8 billion+ should have commas */
    const char *p = c.count_text;
    int comma_count = 0;
    while (*p) {
        if (*p == ',') comma_count++;
        p++;
    }
    TEST_ASSERT_TRUE(comma_count >= 2); /* e.g., "8,100,000,000" has 3 */
}

void test_compute_rate_text_contains_per_second(void) {
    pc_counter_t c = pc_compute(2024, 0.0);
    TEST_ASSERT_NOT_NULL(strstr(c.rate_text, "per second"));
}

void test_compute_daily_text_contains_relevant_word(void) {
    pc_counter_t c = pc_compute(2024, 0.0);
    /* Should contain "today", "yesterday", or "per day" */
    int found = (strstr(c.daily_text, "today") != NULL) ||
                (strstr(c.daily_text, "yesterday") != NULL) ||
                (strstr(c.daily_text, "per day") != NULL);
    TEST_ASSERT_TRUE(found);
}

/* ===== pc_format_count ===== */

void test_format_count_1234567(void) {
    char buf[PC_TEXT_MAX];
    int n = pc_format_count(1234567.0, buf, PC_TEXT_MAX);
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_EQUAL_STRING("1,234,567", buf);
}

void test_format_count_zero(void) {
    char buf[PC_TEXT_MAX];
    pc_format_count(0.0, buf, PC_TEXT_MAX);
    TEST_ASSERT_EQUAL_STRING("0", buf);
}

void test_format_count_999(void) {
    char buf[PC_TEXT_MAX];
    pc_format_count(999.0, buf, PC_TEXT_MAX);
    TEST_ASSERT_EQUAL_STRING("999", buf);
}

void test_format_count_1000(void) {
    char buf[PC_TEXT_MAX];
    pc_format_count(1000.0, buf, PC_TEXT_MAX);
    TEST_ASSERT_EQUAL_STRING("1,000", buf);
}

void test_format_count_1000000(void) {
    char buf[PC_TEXT_MAX];
    pc_format_count(1000000.0, buf, PC_TEXT_MAX);
    TEST_ASSERT_EQUAL_STRING("1,000,000", buf);
}

void test_format_count_8_billion(void) {
    char buf[PC_TEXT_MAX];
    pc_format_count(8100000000.0, buf, PC_TEXT_MAX);
    TEST_ASSERT_EQUAL_STRING("8,100,000,000", buf);
}

void test_format_count_null_buffer(void) {
    int n = pc_format_count(1234.0, NULL, 0);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_format_count_small_buffer(void) {
    char buf[4];
    int n = pc_format_count(1234567.0, buf, 4);
    /* Should write what fits but not overflow */
    TEST_ASSERT_TRUE(n >= 0);
    TEST_ASSERT_TRUE(strlen(buf) < 4);
}

/* ===== pc_format ===== */

void test_format_output_non_empty(void) {
    pc_counter_t c = pc_compute(2024, 0.0);
    char buf[512];
    int n = pc_format(&c, buf, 512);
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_TRUE(strlen(buf) > 0);
}

void test_format_null_counter(void) {
    char buf[64];
    int n = pc_format(NULL, buf, 64);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_format_null_buffer(void) {
    pc_counter_t c = pc_compute(2024, 0.0);
    int n = pc_format(&c, NULL, 0);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_format_small_buffer(void) {
    pc_counter_t c = pc_compute(2024, 0.0);
    char buf[8];
    int n = pc_format(&c, buf, 8);
    /* Should write something but not overflow */
    TEST_ASSERT_TRUE(n >= 0);
}

/* ===== Edge cases ===== */

void test_estimate_large_seconds_offset(void) {
    /* Full year of seconds: 365.25 * 86400 ≈ 31,557,600 */
    double pop = pc_estimate(2024, 31557600.0);
    /* Should be roughly 1 year's growth more than base */
    double base = pc_estimate(2024, 0.0);
    double diff = pop - base;
    /* ~80 million growth per year */
    TEST_ASSERT_TRUE(diff > 50e6);
    TEST_ASSERT_TRUE(diff < 120e6);
}

void test_compute_births_per_second_2024_roughly_4_to_5(void) {
    pc_counter_t c = pc_compute(2024, 0.0);
    TEST_ASSERT_TRUE(c.births_per_second >= 3.5);
    TEST_ASSERT_TRUE(c.births_per_second <= 5.5);
}

void test_compute_deaths_per_second_2024_roughly_1_to_2_5(void) {
    pc_counter_t c = pc_compute(2024, 0.0);
    TEST_ASSERT_TRUE(c.deaths_per_second >= 1.0);
    TEST_ASSERT_TRUE(c.deaths_per_second <= 2.5);
}

/* --- Runner --- */

int main(void) {
    UNITY_BEGIN();

    /* pc_estimate */
    RUN_TEST(test_estimate_2024_roughly_8_billion);
    RUN_TEST(test_estimate_next_day_is_more);
    RUN_TEST(test_estimate_increases_with_seconds);
    RUN_TEST(test_estimate_1900_roughly_1_6_billion);
    RUN_TEST(test_estimate_1950_roughly_2_5_billion);
    RUN_TEST(test_estimate_2000_roughly_6_billion);
    RUN_TEST(test_estimate_zero_offset_equals_base);

    /* pc_growth_rate */
    RUN_TEST(test_growth_rate_2024_roughly_2_to_3);
    RUN_TEST(test_growth_rate_positive);
    RUN_TEST(test_growth_rate_1900_different_from_2024);

    /* pc_compute */
    RUN_TEST(test_compute_populates_all_fields);
    RUN_TEST(test_compute_births_greater_than_deaths);
    RUN_TEST(test_compute_net_is_births_minus_deaths);
    RUN_TEST(test_compute_net_per_day_range);
    RUN_TEST(test_compute_net_per_day_is_net_per_second_times_86400);
    RUN_TEST(test_compute_count_text_has_commas);
    RUN_TEST(test_compute_rate_text_contains_per_second);
    RUN_TEST(test_compute_daily_text_contains_relevant_word);

    /* pc_format_count */
    RUN_TEST(test_format_count_1234567);
    RUN_TEST(test_format_count_zero);
    RUN_TEST(test_format_count_999);
    RUN_TEST(test_format_count_1000);
    RUN_TEST(test_format_count_1000000);
    RUN_TEST(test_format_count_8_billion);
    RUN_TEST(test_format_count_null_buffer);
    RUN_TEST(test_format_count_small_buffer);

    /* pc_format */
    RUN_TEST(test_format_output_non_empty);
    RUN_TEST(test_format_null_counter);
    RUN_TEST(test_format_null_buffer);
    RUN_TEST(test_format_small_buffer);

    /* Edge cases */
    RUN_TEST(test_estimate_large_seconds_offset);
    RUN_TEST(test_compute_births_per_second_2024_roughly_4_to_5);
    RUN_TEST(test_compute_deaths_per_second_2024_roughly_1_to_2_5);

    return UNITY_END();
}
