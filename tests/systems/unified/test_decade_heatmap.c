/* test_decade_heatmap.c — Tests for decade convergence heatmap data.
 *
 * TDD: tests for the S88 decade heatmap module.
 * Tests dh_compute, dh_row_total, dh_col_max, dh_year_average,
 * dh_year_summary. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/decade_heatmap.h"
#include "../../../src/math/julian.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===================================================================
 * dh_compute — basic behavior
 * =================================================================== */

void test_compute_null_returns_zero(void) {
    TEST_ASSERT_EQUAL_INT(0, dh_compute(2026, 1, NULL));
}

void test_compute_zero_years_returns_zero(void) {
    dh_heatmap_t map;
    TEST_ASSERT_EQUAL_INT(0, dh_compute(2026, 0, &map));
}

void test_compute_single_year(void) {
    dh_heatmap_t map;
    int count = dh_compute(2026, 1, &map);
    TEST_ASSERT_EQUAL_INT(1, count);
    TEST_ASSERT_EQUAL_INT(2026, map.start_year);
    TEST_ASSERT_EQUAL_INT(1, map.year_count);
    TEST_ASSERT_EQUAL_INT(365, map.days_in_year[0]);
}

void test_compute_leap_year(void) {
    dh_heatmap_t map;
    dh_compute(2024, 1, &map);
    TEST_ASSERT_EQUAL_INT(366, map.days_in_year[0]);
}

void test_compute_three_years(void) {
    dh_heatmap_t map;
    int count = dh_compute(2024, 3, &map);
    TEST_ASSERT_EQUAL_INT(3, count);
    TEST_ASSERT_EQUAL_INT(2024, map.start_year);
    TEST_ASSERT_EQUAL_INT(366, map.days_in_year[0]); /* 2024 leap */
    TEST_ASSERT_EQUAL_INT(365, map.days_in_year[1]); /* 2025 */
    TEST_ASSERT_EQUAL_INT(365, map.days_in_year[2]); /* 2026 */
}

void test_compute_clamps_to_max(void) {
    dh_heatmap_t map;
    int count = dh_compute(2020, 20, &map);
    TEST_ASSERT_EQUAL_INT(DH_MAX_YEARS, count);
    TEST_ASSERT_EQUAL_INT(DH_MAX_YEARS, map.year_count);
}

/* ===================================================================
 * Grid data
 * =================================================================== */

void test_grid_has_nonnegative_values(void) {
    dh_heatmap_t map;
    dh_compute(2026, 1, &map);
    for (int d = 0; d < map.days_in_year[0]; d++) {
        TEST_ASSERT_TRUE(map.grid[0][d] >= 0);
    }
}

void test_grid_max_system_count_positive(void) {
    dh_heatmap_t map;
    dh_compute(2026, 1, &map);
    TEST_ASSERT_TRUE(map.max_system_count > 0);
}

void test_best_day_within_range(void) {
    dh_heatmap_t map;
    dh_compute(2026, 1, &map);
    TEST_ASSERT_TRUE(map.best_day_year >= 0);
    TEST_ASSERT_TRUE(map.best_day_year < map.year_count);
    TEST_ASSERT_TRUE(map.best_day_doy >= 0);
    TEST_ASSERT_TRUE(map.best_day_doy < map.days_in_year[map.best_day_year]);
}

void test_best_day_equals_max(void) {
    dh_heatmap_t map;
    dh_compute(2026, 1, &map);
    int best_val = map.grid[map.best_day_year][map.best_day_doy];
    TEST_ASSERT_EQUAL_INT(map.max_system_count, best_val);
}

/* ===================================================================
 * dh_row_total
 * =================================================================== */

void test_row_total_positive(void) {
    dh_heatmap_t map;
    dh_compute(2026, 1, &map);
    int total = dh_row_total(&map, 0);
    TEST_ASSERT_TRUE(total > 0);
}

void test_row_total_invalid_index(void) {
    dh_heatmap_t map;
    dh_compute(2026, 1, &map);
    TEST_ASSERT_EQUAL_INT(0, dh_row_total(&map, -1));
    TEST_ASSERT_EQUAL_INT(0, dh_row_total(&map, 1));
    TEST_ASSERT_EQUAL_INT(0, dh_row_total(NULL, 0));
}

/* ===================================================================
 * dh_col_max
 * =================================================================== */

void test_col_max_nonnegative(void) {
    dh_heatmap_t map;
    dh_compute(2026, 2, &map);
    for (int d = 0; d < 365; d++) {
        TEST_ASSERT_TRUE(dh_col_max(&map, d) >= 0);
    }
}

void test_col_max_invalid(void) {
    dh_heatmap_t map;
    dh_compute(2026, 1, &map);
    TEST_ASSERT_EQUAL_INT(0, dh_col_max(&map, -1));
    TEST_ASSERT_EQUAL_INT(0, dh_col_max(NULL, 0));
}

/* ===================================================================
 * dh_year_average
 * =================================================================== */

void test_year_average_positive(void) {
    dh_heatmap_t map;
    dh_compute(2026, 1, &map);
    double avg = dh_year_average(&map, 0);
    TEST_ASSERT_TRUE(avg > 0.0);
}

void test_year_average_consistent_with_total(void) {
    dh_heatmap_t map;
    dh_compute(2026, 1, &map);
    TEST_ASSERT_FLOAT_WITHIN(0.001,
        (double)dh_row_total(&map, 0) / 365.0,
        dh_year_average(&map, 0));
}

void test_year_average_invalid(void) {
    dh_heatmap_t map;
    dh_compute(2026, 1, &map);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.0, dh_year_average(&map, 5));
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.0, dh_year_average(NULL, 0));
}

/* ===================================================================
 * dh_year_summary
 * =================================================================== */

void test_year_summary_basic(void) {
    dh_heatmap_t map;
    dh_compute(2026, 1, &map);
    dh_year_summary_t s;
    int ok = dh_year_summary(&map, 0, &s);
    TEST_ASSERT_EQUAL_INT(1, ok);
    TEST_ASSERT_EQUAL_INT(2026, s.year);
    TEST_ASSERT_TRUE(s.total_convergence > 0);
    TEST_ASSERT_TRUE(s.max_day_count > 0);
    TEST_ASSERT_TRUE(s.average > 0.0);
}

void test_year_summary_notable_days(void) {
    dh_heatmap_t map;
    dh_compute(2026, 1, &map);
    dh_year_summary_t s;
    dh_year_summary(&map, 0, &s);
    TEST_ASSERT_TRUE(s.notable_days >= 0);
    TEST_ASSERT_TRUE(s.notable_days <= 365);
}

void test_year_summary_invalid(void) {
    dh_heatmap_t map;
    dh_compute(2026, 1, &map);
    dh_year_summary_t s;
    TEST_ASSERT_EQUAL_INT(0, dh_year_summary(&map, 5, &s));
    TEST_ASSERT_EQUAL_INT(0, dh_year_summary(NULL, 0, &s));
    TEST_ASSERT_EQUAL_INT(0, dh_year_summary(&map, 0, NULL));
}

/* ===================================================================
 * Runner
 * =================================================================== */

int main(void) {
    UNITY_BEGIN();

    /* dh_compute */
    RUN_TEST(test_compute_null_returns_zero);
    RUN_TEST(test_compute_zero_years_returns_zero);
    RUN_TEST(test_compute_single_year);
    RUN_TEST(test_compute_leap_year);
    RUN_TEST(test_compute_three_years);
    RUN_TEST(test_compute_clamps_to_max);

    /* grid data */
    RUN_TEST(test_grid_has_nonnegative_values);
    RUN_TEST(test_grid_max_system_count_positive);
    RUN_TEST(test_best_day_within_range);
    RUN_TEST(test_best_day_equals_max);

    /* dh_row_total */
    RUN_TEST(test_row_total_positive);
    RUN_TEST(test_row_total_invalid_index);

    /* dh_col_max */
    RUN_TEST(test_col_max_nonnegative);
    RUN_TEST(test_col_max_invalid);

    /* dh_year_average */
    RUN_TEST(test_year_average_positive);
    RUN_TEST(test_year_average_consistent_with_total);
    RUN_TEST(test_year_average_invalid);

    /* dh_year_summary */
    RUN_TEST(test_year_summary_basic);
    RUN_TEST(test_year_summary_notable_days);
    RUN_TEST(test_year_summary_invalid);

    return UNITY_END();
}
