/* test_human_population.c — Tests for human population through history.
 * TDD RED phase: all tests written before implementation.
 * Data source: Population Reference Bureau (2023), UN estimates. */

#include "../../unity/unity.h"
#include "../../../src/systems/earth/human_population.h"

#include <string.h>

void setUp(void) { }
void tearDown(void) { }

/* ===== Milestone Data ===== */

void test_milestone_count_is_8(void) {
    TEST_ASSERT_EQUAL_INT(8, pop_milestone_count());
}

void test_milestone_count_matches_define(void) {
    TEST_ASSERT_EQUAL_INT(POP_MILESTONE_COUNT, pop_milestone_count());
}

void test_milestone_first_is_1_billion(void) {
    pop_milestone_t m = pop_milestone_get(0);
    TEST_ASSERT_EQUAL_INT(1, m.billions);
    TEST_ASSERT_EQUAL_INT(1804, m.year);
}

void test_milestone_last_is_8_billion(void) {
    pop_milestone_t m = pop_milestone_get(7);
    TEST_ASSERT_EQUAL_INT(8, m.billions);
    TEST_ASSERT_EQUAL_INT(2022, m.year);
}

void test_milestone_2_billion(void) {
    pop_milestone_t m = pop_milestone_get(1);
    TEST_ASSERT_EQUAL_INT(2, m.billions);
    TEST_ASSERT_EQUAL_INT(1927, m.year);
}

void test_milestone_context_non_null(void) {
    for (int i = 0; i < POP_MILESTONE_COUNT; i++) {
        pop_milestone_t m = pop_milestone_get(i);
        TEST_ASSERT_NOT_NULL(m.context);
        TEST_ASSERT_TRUE(strlen(m.context) > 0);
    }
}

void test_milestone_years_to_next_decreasing(void) {
    /* The time between billion milestones should generally decrease
     * (accelerating growth) */
    pop_milestone_t m0 = pop_milestone_get(0);
    pop_milestone_t m5 = pop_milestone_get(5);
    TEST_ASSERT_TRUE(m0.years_to_next > m5.years_to_next);
}

void test_milestone_last_years_to_next_is_zero(void) {
    pop_milestone_t m = pop_milestone_get(7);
    TEST_ASSERT_EQUAL_INT(0, m.years_to_next);
}

void test_milestone_ascending_order(void) {
    for (int i = 1; i < POP_MILESTONE_COUNT; i++) {
        pop_milestone_t prev = pop_milestone_get(i - 1);
        pop_milestone_t curr = pop_milestone_get(i);
        TEST_ASSERT_TRUE(curr.billions > prev.billions);
        TEST_ASSERT_TRUE(curr.year > prev.year);
    }
}

void test_milestone_invalid_negative(void) {
    pop_milestone_t m = pop_milestone_get(-1);
    TEST_ASSERT_EQUAL_INT(0, m.billions);
}

void test_milestone_invalid_high(void) {
    pop_milestone_t m = pop_milestone_get(8);
    TEST_ASSERT_EQUAL_INT(0, m.billions);
}

/* ===== Data Point Data ===== */

void test_datapoint_count_is_26(void) {
    TEST_ASSERT_EQUAL_INT(26, pop_datapoint_count());
}

void test_datapoint_count_matches_define(void) {
    TEST_ASSERT_EQUAL_INT(POP_DATAPOINT_COUNT, pop_datapoint_count());
}

void test_datapoint_first_is_10000_bce(void) {
    pop_datapoint_t d = pop_datapoint_get(0);
    TEST_ASSERT_EQUAL_INT(-10000, d.year);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 5.0f, (float)d.population);
}

void test_datapoint_last_is_2024(void) {
    pop_datapoint_t d = pop_datapoint_get(25);
    TEST_ASSERT_EQUAL_INT(2024, d.year);
    TEST_ASSERT_FLOAT_WITHIN(200.0f, 8100.0f, (float)d.population);
}

void test_datapoint_1_ce(void) {
    /* 1 CE ≈ 300 million */
    pop_datapoint_t d = pop_datapoint_get(5);
    TEST_ASSERT_EQUAL_INT(1, d.year);
    TEST_ASSERT_FLOAT_WITHIN(10.0f, 300.0f, (float)d.population);
}

void test_datapoint_chronological_order(void) {
    for (int i = 1; i < POP_DATAPOINT_COUNT; i++) {
        pop_datapoint_t prev = pop_datapoint_get(i - 1);
        pop_datapoint_t curr = pop_datapoint_get(i);
        TEST_ASSERT_TRUE(curr.year > prev.year);
    }
}

void test_datapoint_all_strings_non_null(void) {
    for (int i = 0; i < POP_DATAPOINT_COUNT; i++) {
        pop_datapoint_t d = pop_datapoint_get(i);
        TEST_ASSERT_NOT_NULL(d.era);
        TEST_ASSERT_NOT_NULL(d.note);
    }
}

void test_datapoint_invalid_negative(void) {
    pop_datapoint_t d = pop_datapoint_get(-1);
    TEST_ASSERT_EQUAL_INT(0, d.year);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.0f, (float)d.population);
}

void test_datapoint_invalid_high(void) {
    pop_datapoint_t d = pop_datapoint_get(26);
    TEST_ASSERT_EQUAL_INT(0, d.year);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.0f, (float)d.population);
}

void test_datapoint_black_death_decline(void) {
    /* 1340 CE (~443M) -> 1400 CE (~350M): population declined */
    pop_datapoint_t pre = pop_datapoint_get(9);   /* 1340 */
    pop_datapoint_t post = pop_datapoint_get(10);  /* 1400 */
    TEST_ASSERT_EQUAL_INT(1340, pre.year);
    TEST_ASSERT_EQUAL_INT(1400, post.year);
    TEST_ASSERT_TRUE(post.population < pre.population);
}

/* ===== pop_alive_at_year ===== */

void test_alive_at_known_datapoint_1_ce(void) {
    /* 1 CE ≈ 300 million */
    double pop = pop_alive_at_year(1);
    TEST_ASSERT_FLOAT_WITHIN(10.0f, 300.0f, (float)pop);
}

void test_alive_at_known_datapoint_2024(void) {
    /* 2024 ≈ 8100 million */
    double pop = pop_alive_at_year(2024);
    TEST_ASSERT_FLOAT_WITHIN(200.0f, 8100.0f, (float)pop);
}

void test_alive_at_known_datapoint_1900(void) {
    /* 1900 ≈ 1600 million */
    double pop = pop_alive_at_year(1900);
    TEST_ASSERT_FLOAT_WITHIN(50.0f, 1600.0f, (float)pop);
}

void test_alive_interpolated_between_points(void) {
    /* 1950 should be between 1900 (~1600M) and 1960 (~3000M) values */
    double pop = pop_alive_at_year(1950);
    TEST_ASSERT_TRUE(pop >= 1600.0);
    TEST_ASSERT_TRUE(pop <= 3000.0);
    /* Known value: ~2500M */
    TEST_ASSERT_FLOAT_WITHIN(200.0f, 2500.0f, (float)pop);
}

void test_alive_very_old_before_data(void) {
    /* Before -10000: returns ~1 million */
    double pop = pop_alive_at_year(-50000);
    TEST_ASSERT_FLOAT_WITHIN(0.5f, 1.0f, (float)pop);
}

void test_alive_future_extrapolation(void) {
    /* After 2024: should be > 8100 but not absurdly large */
    double pop = pop_alive_at_year(2030);
    TEST_ASSERT_TRUE(pop > 8100.0);
    TEST_ASSERT_TRUE(pop < 10000.0);
}

void test_alive_at_10000_bce(void) {
    /* Exact data point: ~5M */
    double pop = pop_alive_at_year(-10000);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 5.0f, (float)pop);
}

void test_alive_monotonic_modern_era(void) {
    /* From 1400 to 2024, population should generally increase */
    double prev = pop_alive_at_year(1400);
    for (int y = 1500; y <= 2024; y += 100) {
        double curr = pop_alive_at_year(y);
        TEST_ASSERT_TRUE(curr > prev);
        prev = curr;
    }
}

/* ===== pop_ever_born_at_year ===== */

void test_ever_born_at_2024(void) {
    /* ~109 billion */
    double eb = pop_ever_born_at_year(2024);
    TEST_ASSERT_FLOAT_WITHIN(5.0f, 109.0f, (float)eb);
}

void test_ever_born_at_1_ce(void) {
    /* ~36 billion */
    double eb = pop_ever_born_at_year(1);
    TEST_ASSERT_FLOAT_WITHIN(5.0f, 36.0f, (float)eb);
}

void test_ever_born_at_1800(void) {
    /* ~73 billion */
    double eb = pop_ever_born_at_year(1800);
    TEST_ASSERT_FLOAT_WITHIN(5.0f, 73.0f, (float)eb);
}

void test_ever_born_monotonically_increasing(void) {
    /* Cumulative births can only increase */
    double prev = pop_ever_born_at_year(-10000);
    int years[] = { -5000, -1000, 1, 500, 1000, 1500, 1800, 1900, 2000, 2024 };
    for (int i = 0; i < 10; i++) {
        double curr = pop_ever_born_at_year(years[i]);
        TEST_ASSERT_TRUE(curr >= prev);
        prev = curr;
    }
}

void test_ever_born_before_data(void) {
    /* Very early: should be small but positive */
    double eb = pop_ever_born_at_year(-50000);
    TEST_ASSERT_TRUE(eb > 0.0);
    TEST_ASSERT_TRUE(eb < 5.0);
}

/* ===== pop_summary ===== */

void test_summary_2024(void) {
    pop_summary_t s = pop_summary(2024);
    TEST_ASSERT_FLOAT_WITHIN(200.0f, 8100.0f, (float)s.alive_millions);
    TEST_ASSERT_FLOAT_WITHIN(5.0f, 109.0f, (float)s.ever_born_billions);
    TEST_ASSERT_EQUAL_INT(8, s.nearest_milestone_billions);
    TEST_ASSERT_NOT_NULL(s.era_name);
    TEST_ASSERT_TRUE(strlen(s.summary) > 0);
}

void test_summary_1_ce(void) {
    pop_summary_t s = pop_summary(1);
    TEST_ASSERT_FLOAT_WITHIN(10.0f, 300.0f, (float)s.alive_millions);
    TEST_ASSERT_NOT_NULL(s.era_name);
}

void test_summary_nearest_milestone(void) {
    /* At 1950 (~2.5B), nearest milestone should be 2 (reached 1927) or 3 */
    pop_summary_t s = pop_summary(1950);
    TEST_ASSERT_TRUE(s.nearest_milestone_billions >= 2);
    TEST_ASSERT_TRUE(s.nearest_milestone_billions <= 3);
}

/* ===== pop_format_number ===== */

void test_format_billions(void) {
    char buf[64];
    int n = pop_format_number(8100.0, buf, 64);
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_EQUAL_STRING("8.1 billion", buf);
}

void test_format_billions_round(void) {
    char buf[64];
    pop_format_number(2000.0, buf, 64);
    TEST_ASSERT_EQUAL_STRING("2.0 billion", buf);
}

void test_format_millions(void) {
    char buf[64];
    pop_format_number(300.0, buf, 64);
    TEST_ASSERT_EQUAL_STRING("300 million", buf);
}

void test_format_small_millions(void) {
    char buf[64];
    pop_format_number(5.0, buf, 64);
    TEST_ASSERT_EQUAL_STRING("5 million", buf);
}

void test_format_sub_million(void) {
    char buf[64];
    pop_format_number(0.5, buf, 64);
    /* 0.5 million = 500,000 */
    TEST_ASSERT_TRUE(strlen(buf) > 0);
}

void test_format_zero_length_buffer(void) {
    char buf[1] = {0};
    int n = pop_format_number(8100.0, buf, 0);
    TEST_ASSERT_EQUAL_INT(0, n);
}

/* ===== pop_year_of_billion ===== */

void test_year_of_billion_1(void) {
    TEST_ASSERT_EQUAL_INT(1804, pop_year_of_billion(1));
}

void test_year_of_billion_2(void) {
    TEST_ASSERT_EQUAL_INT(1927, pop_year_of_billion(2));
}

void test_year_of_billion_8(void) {
    TEST_ASSERT_EQUAL_INT(2022, pop_year_of_billion(8));
}

void test_year_of_billion_invalid_0(void) {
    TEST_ASSERT_EQUAL_INT(0, pop_year_of_billion(0));
}

void test_year_of_billion_invalid_9(void) {
    TEST_ASSERT_EQUAL_INT(0, pop_year_of_billion(9));
}

void test_year_of_billion_invalid_negative(void) {
    TEST_ASSERT_EQUAL_INT(0, pop_year_of_billion(-1));
}

/* --- Runner --- */

int main(void) {
    UNITY_BEGIN();

    /* Milestone data */
    RUN_TEST(test_milestone_count_is_8);
    RUN_TEST(test_milestone_count_matches_define);
    RUN_TEST(test_milestone_first_is_1_billion);
    RUN_TEST(test_milestone_last_is_8_billion);
    RUN_TEST(test_milestone_2_billion);
    RUN_TEST(test_milestone_context_non_null);
    RUN_TEST(test_milestone_years_to_next_decreasing);
    RUN_TEST(test_milestone_last_years_to_next_is_zero);
    RUN_TEST(test_milestone_ascending_order);
    RUN_TEST(test_milestone_invalid_negative);
    RUN_TEST(test_milestone_invalid_high);

    /* Data point data */
    RUN_TEST(test_datapoint_count_is_26);
    RUN_TEST(test_datapoint_count_matches_define);
    RUN_TEST(test_datapoint_first_is_10000_bce);
    RUN_TEST(test_datapoint_last_is_2024);
    RUN_TEST(test_datapoint_1_ce);
    RUN_TEST(test_datapoint_chronological_order);
    RUN_TEST(test_datapoint_all_strings_non_null);
    RUN_TEST(test_datapoint_invalid_negative);
    RUN_TEST(test_datapoint_invalid_high);
    RUN_TEST(test_datapoint_black_death_decline);

    /* Population alive at year */
    RUN_TEST(test_alive_at_known_datapoint_1_ce);
    RUN_TEST(test_alive_at_known_datapoint_2024);
    RUN_TEST(test_alive_at_known_datapoint_1900);
    RUN_TEST(test_alive_interpolated_between_points);
    RUN_TEST(test_alive_very_old_before_data);
    RUN_TEST(test_alive_future_extrapolation);
    RUN_TEST(test_alive_at_10000_bce);
    RUN_TEST(test_alive_monotonic_modern_era);

    /* Ever born */
    RUN_TEST(test_ever_born_at_2024);
    RUN_TEST(test_ever_born_at_1_ce);
    RUN_TEST(test_ever_born_at_1800);
    RUN_TEST(test_ever_born_monotonically_increasing);
    RUN_TEST(test_ever_born_before_data);

    /* Summary */
    RUN_TEST(test_summary_2024);
    RUN_TEST(test_summary_1_ce);
    RUN_TEST(test_summary_nearest_milestone);

    /* Format number */
    RUN_TEST(test_format_billions);
    RUN_TEST(test_format_billions_round);
    RUN_TEST(test_format_millions);
    RUN_TEST(test_format_small_millions);
    RUN_TEST(test_format_sub_million);
    RUN_TEST(test_format_zero_length_buffer);

    /* Year of billion */
    RUN_TEST(test_year_of_billion_1);
    RUN_TEST(test_year_of_billion_2);
    RUN_TEST(test_year_of_billion_8);
    RUN_TEST(test_year_of_billion_invalid_0);
    RUN_TEST(test_year_of_billion_invalid_9);
    RUN_TEST(test_year_of_billion_invalid_negative);

    return UNITY_END();
}
