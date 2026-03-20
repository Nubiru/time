/* test_earth_stats.c — Tests for Earth/human statistics display module.
 * TDD RED phase: all tests written before implementation.
 * S91: "Humans have existed for X% of Earth's history"
 * S92: "Y% of all humans who ever lived are alive right now" */

#include "../../unity/unity.h"
#include "../../../src/systems/earth/earth_stats.h"

#include <string.h>

void setUp(void) { }
void tearDown(void) { }

/* ===== es_existence — S91 ===== */

void test_existence_earth_age(void) {
    es_existence_t ex = es_existence();
    TEST_ASSERT_FLOAT_WITHIN(1.0e8, 4.54e9, ex.earth_age_years);
}

void test_existence_human_age(void) {
    es_existence_t ex = es_existence();
    TEST_ASSERT_FLOAT_WITHIN(50000.0, 300000.0, ex.human_age_years);
}

void test_existence_fraction_range(void) {
    es_existence_t ex = es_existence();
    /* 300000 / 4.54e9 ≈ 0.0000661 */
    TEST_ASSERT_TRUE(ex.fraction > 0.00005);
    TEST_ASSERT_TRUE(ex.fraction < 0.00008);
}

void test_existence_percentage_range(void) {
    es_existence_t ex = es_existence();
    /* ~0.00661% */
    TEST_ASSERT_TRUE(ex.percentage > 0.005);
    TEST_ASSERT_TRUE(ex.percentage < 0.008);
}

void test_existence_fraction_equals_percentage_div_100(void) {
    es_existence_t ex = es_existence();
    TEST_ASSERT_FLOAT_WITHIN(1.0e-8, ex.fraction, ex.percentage / 100.0);
}

void test_existence_fraction_text_contains_percent(void) {
    es_existence_t ex = es_existence();
    TEST_ASSERT_NOT_NULL(strstr(ex.fraction_text, "%"));
}

void test_existence_fraction_text_contains_earth(void) {
    es_existence_t ex = es_existence();
    TEST_ASSERT_NOT_NULL(strstr(ex.fraction_text, "Earth"));
}

void test_existence_fraction_text_nonempty(void) {
    es_existence_t ex = es_existence();
    TEST_ASSERT_TRUE(strlen(ex.fraction_text) > 10);
}

void test_existence_ruler_analogy_contains_ruler(void) {
    es_existence_t ex = es_existence();
    TEST_ASSERT_NOT_NULL(strstr(ex.ruler_analogy, "ruler"));
}

void test_existence_ruler_analogy_contains_mm(void) {
    es_existence_t ex = es_existence();
    TEST_ASSERT_NOT_NULL(strstr(ex.ruler_analogy, "mm"));
}

void test_existence_day_analogy_contains_24_hours(void) {
    es_existence_t ex = es_existence();
    TEST_ASSERT_NOT_NULL(strstr(ex.day_analogy, "24 hours"));
}

void test_existence_day_analogy_contains_seconds(void) {
    es_existence_t ex = es_existence();
    TEST_ASSERT_NOT_NULL(strstr(ex.day_analogy, "seconds"));
}

void test_existence_book_analogy_contains_book_or_page(void) {
    es_existence_t ex = es_existence();
    int has_book = strstr(ex.book_analogy, "book") != NULL;
    int has_page = strstr(ex.book_analogy, "page") != NULL;
    TEST_ASSERT_TRUE(has_book || has_page);
}

void test_existence_book_analogy_nonempty(void) {
    es_existence_t ex = es_existence();
    TEST_ASSERT_TRUE(strlen(ex.book_analogy) > 10);
}

/* ===== es_alive — S92 ===== */

void test_alive_2024_billions_range(void) {
    es_alive_t al = es_alive(2024);
    /* pop_alive_at_year(2024) = 8100 millions = 8.1 billions */
    TEST_ASSERT_TRUE(al.alive_billions >= 8.0);
    TEST_ASSERT_TRUE(al.alive_billions <= 8.2);
}

void test_alive_2024_ever_born_range(void) {
    es_alive_t al = es_alive(2024);
    /* pop_ever_born_at_year(2024) = ~109 billions */
    TEST_ASSERT_TRUE(al.ever_born_billions >= 105.0);
    TEST_ASSERT_TRUE(al.ever_born_billions <= 115.0);
}

void test_alive_2024_fraction(void) {
    es_alive_t al = es_alive(2024);
    /* ~8.1 / 109 ≈ 0.074 */
    TEST_ASSERT_TRUE(al.alive_fraction > 0.05);
    TEST_ASSERT_TRUE(al.alive_fraction < 0.10);
}

void test_alive_2024_percentage_range(void) {
    es_alive_t al = es_alive(2024);
    /* ~7.4% */
    TEST_ASSERT_TRUE(al.alive_percentage >= 5.0);
    TEST_ASSERT_TRUE(al.alive_percentage <= 10.0);
}

void test_alive_percentage_equals_fraction_times_100(void) {
    es_alive_t al = es_alive(2024);
    TEST_ASSERT_FLOAT_WITHIN(0.001, al.alive_fraction * 100.0,
                             al.alive_percentage);
}

void test_alive_text_contains_percent(void) {
    es_alive_t al = es_alive(2024);
    TEST_ASSERT_NOT_NULL(strstr(al.alive_text, "%"));
}

void test_alive_text_contains_alive(void) {
    es_alive_t al = es_alive(2024);
    TEST_ASSERT_NOT_NULL(strstr(al.alive_text, "alive"));
}

void test_alive_text_nonempty(void) {
    es_alive_t al = es_alive(2024);
    TEST_ASSERT_TRUE(strlen(al.alive_text) > 10);
}

void test_alive_moment_text_contains_most_populated(void) {
    es_alive_t al = es_alive(2024);
    TEST_ASSERT_NOT_NULL(strstr(al.moment_text, "most populated"));
}

void test_alive_per_dead_text_contains_lived_and_died(void) {
    es_alive_t al = es_alive(2024);
    TEST_ASSERT_NOT_NULL(strstr(al.per_dead_text, "lived and died"));
}

void test_alive_per_dead_text_nonempty(void) {
    es_alive_t al = es_alive(2024);
    TEST_ASSERT_TRUE(strlen(al.per_dead_text) > 10);
}

/* ===== es_alive — historical years ===== */

void test_alive_1900_lower_percentage(void) {
    /* 1900: ~1600M alive, ~83B born -> ~1.9% */
    es_alive_t al = es_alive(1900);
    TEST_ASSERT_TRUE(al.alive_percentage > 0.5);
    TEST_ASSERT_TRUE(al.alive_percentage < 5.0);
}

void test_alive_1_ce_tiny_percentage(void) {
    /* 1 CE: ~300M alive, ~36B born -> ~0.83% */
    es_alive_t al = es_alive(1);
    TEST_ASSERT_TRUE(al.alive_percentage > 0.1);
    TEST_ASSERT_TRUE(al.alive_percentage < 3.0);
}

/* ===== es_format ===== */

void test_format_produces_nonempty(void) {
    es_existence_t ex = es_existence();
    es_alive_t al = es_alive(2024);
    char buf[1024];
    int n = es_format(&ex, &al, buf, 1024);
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_TRUE(strlen(buf) > 0);
}

void test_format_contains_existence_info(void) {
    es_existence_t ex = es_existence();
    es_alive_t al = es_alive(2024);
    char buf[1024];
    es_format(&ex, &al, buf, 1024);
    TEST_ASSERT_NOT_NULL(strstr(buf, "Earth"));
}

void test_format_contains_alive_info(void) {
    es_existence_t ex = es_existence();
    es_alive_t al = es_alive(2024);
    char buf[1024];
    es_format(&ex, &al, buf, 1024);
    TEST_ASSERT_NOT_NULL(strstr(buf, "alive"));
}

void test_format_null_existence(void) {
    es_alive_t al = es_alive(2024);
    char buf[256];
    int n = es_format(NULL, &al, buf, 256);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_format_null_alive(void) {
    es_existence_t ex = es_existence();
    char buf[256];
    int n = es_format(&ex, NULL, buf, 256);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_format_null_buffer(void) {
    es_existence_t ex = es_existence();
    es_alive_t al = es_alive(2024);
    int n = es_format(&ex, &al, NULL, 256);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_format_zero_buffer(void) {
    es_existence_t ex = es_existence();
    es_alive_t al = es_alive(2024);
    char buf[4];
    int n = es_format(&ex, &al, buf, 0);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_format_small_buffer_no_crash(void) {
    es_existence_t ex = es_existence();
    es_alive_t al = es_alive(2024);
    char buf[16];
    int n = es_format(&ex, &al, buf, 16);
    /* Should truncate gracefully, not crash */
    TEST_ASSERT_TRUE(n >= 0);
    TEST_ASSERT_TRUE(n < 16);
}

/* --- Runner --- */

int main(void) {
    UNITY_BEGIN();

    /* S91: Existence fraction */
    RUN_TEST(test_existence_earth_age);
    RUN_TEST(test_existence_human_age);
    RUN_TEST(test_existence_fraction_range);
    RUN_TEST(test_existence_percentage_range);
    RUN_TEST(test_existence_fraction_equals_percentage_div_100);
    RUN_TEST(test_existence_fraction_text_contains_percent);
    RUN_TEST(test_existence_fraction_text_contains_earth);
    RUN_TEST(test_existence_fraction_text_nonempty);
    RUN_TEST(test_existence_ruler_analogy_contains_ruler);
    RUN_TEST(test_existence_ruler_analogy_contains_mm);
    RUN_TEST(test_existence_day_analogy_contains_24_hours);
    RUN_TEST(test_existence_day_analogy_contains_seconds);
    RUN_TEST(test_existence_book_analogy_contains_book_or_page);
    RUN_TEST(test_existence_book_analogy_nonempty);

    /* S92: Alive stats */
    RUN_TEST(test_alive_2024_billions_range);
    RUN_TEST(test_alive_2024_ever_born_range);
    RUN_TEST(test_alive_2024_fraction);
    RUN_TEST(test_alive_2024_percentage_range);
    RUN_TEST(test_alive_percentage_equals_fraction_times_100);
    RUN_TEST(test_alive_text_contains_percent);
    RUN_TEST(test_alive_text_contains_alive);
    RUN_TEST(test_alive_text_nonempty);
    RUN_TEST(test_alive_moment_text_contains_most_populated);
    RUN_TEST(test_alive_per_dead_text_contains_lived_and_died);
    RUN_TEST(test_alive_per_dead_text_nonempty);

    /* Historical alive stats */
    RUN_TEST(test_alive_1900_lower_percentage);
    RUN_TEST(test_alive_1_ce_tiny_percentage);

    /* Format */
    RUN_TEST(test_format_produces_nonempty);
    RUN_TEST(test_format_contains_existence_info);
    RUN_TEST(test_format_contains_alive_info);
    RUN_TEST(test_format_null_existence);
    RUN_TEST(test_format_null_alive);
    RUN_TEST(test_format_null_buffer);
    RUN_TEST(test_format_zero_buffer);
    RUN_TEST(test_format_small_buffer_no_crash);

    return UNITY_END();
}
