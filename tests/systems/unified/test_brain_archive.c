/* test_brain_archive.c — Tests for convergence archive scanner.
 *
 * Tests br_archive_scan, br_archive_notable, br_archive_best,
 * br_archive_count_above, br_archive_year_stats. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/brain_archive.h"
#include "../../../src/math/julian.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* Helper: JD for a Gregorian date */
static double jd_from(int y, int m, int d) {
    return gregorian_to_jd(y, m, (double)d);
}

/* ===================================================================
 * br_archive_scan — basic
 * =================================================================== */

void test_scan_null_returns_zero(void) {
    TEST_ASSERT_EQUAL_INT(0, br_archive_scan(2460000.5, 10, NULL));
}

void test_scan_zero_days_returns_zero(void) {
    br_archive_result_t ar;
    TEST_ASSERT_EQUAL_INT(0, br_archive_scan(2460000.5, 0, &ar));
}

void test_scan_negative_days_returns_zero(void) {
    br_archive_result_t ar;
    TEST_ASSERT_EQUAL_INT(0, br_archive_scan(2460000.5, -5, &ar));
}

void test_scan_one_day(void) {
    br_archive_result_t ar;
    int n = br_archive_scan(jd_from(2026, 3, 20), 1, &ar);
    TEST_ASSERT_EQUAL_INT(1, n);
    TEST_ASSERT_EQUAL_INT(1, ar.count);
    TEST_ASSERT_TRUE(ar.days[0].jd > 0.0);
}

void test_scan_30_days(void) {
    br_archive_result_t ar;
    int n = br_archive_scan(jd_from(2026, 1, 1), 30, &ar);
    TEST_ASSERT_EQUAL_INT(30, n);
    TEST_ASSERT_EQUAL_INT(30, ar.count);
}

void test_scan_jds_sequential(void) {
    br_archive_result_t ar;
    double start = jd_from(2026, 1, 1);
    br_archive_scan(start, 10, &ar);
    for (int i = 0; i < 10; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.001, start + (double)i, ar.days[i].jd);
    }
}

void test_scan_system_count_nonnegative(void) {
    br_archive_result_t ar;
    br_archive_scan(jd_from(2026, 1, 1), 30, &ar);
    for (int i = 0; i < ar.count; i++) {
        TEST_ASSERT_TRUE(ar.days[i].system_count >= 0);
    }
}

void test_scan_significance_range(void) {
    br_archive_result_t ar;
    br_archive_scan(jd_from(2026, 1, 1), 30, &ar);
    for (int i = 0; i < ar.count; i++) {
        TEST_ASSERT_TRUE(ar.days[i].significance >= 0.0);
        TEST_ASSERT_TRUE(ar.days[i].significance <= 1.0);
    }
}

void test_scan_max_systems_tracked(void) {
    br_archive_result_t ar;
    br_archive_scan(jd_from(2026, 1, 1), 90, &ar);
    TEST_ASSERT_TRUE(ar.max_systems >= 0);
    TEST_ASSERT_TRUE(ar.best_jd >= ar.start_jd);
}

void test_scan_capped_at_max_days(void) {
    br_archive_result_t ar;
    int n = br_archive_scan(jd_from(2026, 1, 1), 9999, &ar);
    TEST_ASSERT_EQUAL_INT(BR_ARCHIVE_MAX_DAYS, n);
}

/* ===================================================================
 * br_archive_scan — 2026 full year (365 days)
 * =================================================================== */

void test_scan_2026_full_year(void) {
    br_archive_result_t ar;
    int n = br_archive_scan(jd_from(2026, 1, 1), 365, &ar);
    TEST_ASSERT_EQUAL_INT(365, n);
    TEST_ASSERT_TRUE(ar.notable_count >= 0);
    TEST_ASSERT_TRUE(ar.max_systems >= 2);
}

void test_2026_spring_equinox_notable(void) {
    br_archive_result_t ar;
    double start = jd_from(2026, 1, 1);
    br_archive_scan(start, 365, &ar);
    /* March 20 = day 79 (Jan=31, Feb=28, +20) */
    int equinox_idx = 78; /* 0-based: day 79 is index 78 */
    TEST_ASSERT_TRUE(ar.days[equinox_idx].system_count >= 2);
}

void test_2026_has_notable_days(void) {
    br_archive_result_t ar;
    br_archive_scan(jd_from(2026, 1, 1), 365, &ar);
    /* A full year should have some notable days (3+ systems) */
    TEST_ASSERT_TRUE(ar.notable_count > 0);
}

/* ===================================================================
 * br_archive_notable
 * =================================================================== */

void test_notable_null_returns_zero(void) {
    TEST_ASSERT_EQUAL_INT(0, br_archive_notable(NULL, 3, NULL, 0));
}

void test_notable_filters_correctly(void) {
    br_archive_result_t ar;
    br_archive_scan(jd_from(2026, 1, 1), 90, &ar);
    br_archive_day_t notable[BR_ARCHIVE_MAX_NOTABLE];
    int count = br_archive_notable(&ar, 3, notable, BR_ARCHIVE_MAX_NOTABLE);
    /* All returned days should have 3+ systems */
    for (int i = 0; i < count; i++) {
        TEST_ASSERT_TRUE(notable[i].system_count >= 3);
    }
}

void test_notable_count_matches(void) {
    br_archive_result_t ar;
    br_archive_scan(jd_from(2026, 1, 1), 90, &ar);
    br_archive_day_t notable[BR_ARCHIVE_MAX_NOTABLE];
    int count = br_archive_notable(&ar, 3, notable, BR_ARCHIVE_MAX_NOTABLE);
    /* Should match br_archive_count_above */
    int expected = br_archive_count_above(&ar, 3);
    TEST_ASSERT_EQUAL_INT(expected, count);
}

/* ===================================================================
 * br_archive_best
 * =================================================================== */

void test_best_null_returns_null(void) {
    TEST_ASSERT_NULL(br_archive_best(NULL));
}

void test_best_empty_returns_null(void) {
    br_archive_result_t ar;
    memset(&ar, 0, sizeof(ar));
    TEST_ASSERT_NULL(br_archive_best(&ar));
}

void test_best_returns_highest(void) {
    br_archive_result_t ar;
    br_archive_scan(jd_from(2026, 1, 1), 90, &ar);
    const br_archive_day_t *best = br_archive_best(&ar);
    TEST_ASSERT_NOT_NULL(best);
    /* No other day should have more systems */
    for (int i = 0; i < ar.count; i++) {
        TEST_ASSERT_TRUE(ar.days[i].system_count <= best->system_count);
    }
}

/* ===================================================================
 * br_archive_count_above
 * =================================================================== */

void test_count_above_null_returns_zero(void) {
    TEST_ASSERT_EQUAL_INT(0, br_archive_count_above(NULL, 2));
}

void test_count_above_zero_threshold(void) {
    br_archive_result_t ar;
    br_archive_scan(jd_from(2026, 1, 1), 30, &ar);
    /* Threshold 0: every day qualifies */
    int count = br_archive_count_above(&ar, 0);
    TEST_ASSERT_EQUAL_INT(30, count);
}

void test_count_above_high_threshold(void) {
    br_archive_result_t ar;
    br_archive_scan(jd_from(2026, 1, 1), 30, &ar);
    /* Threshold 22: impossible (max is 21 systems) */
    int count = br_archive_count_above(&ar, 22);
    TEST_ASSERT_EQUAL_INT(0, count);
}

/* ===================================================================
 * br_archive_year_stats
 * =================================================================== */

void test_year_stats_null_safe(void) {
    int counts[5] = {-1, -1, -1, -1, -1};
    br_archive_year_stats(NULL, counts);
    for (int i = 0; i < 5; i++) {
        TEST_ASSERT_EQUAL_INT(0, counts[i]);
    }
}

void test_year_stats_monotonic(void) {
    br_archive_result_t ar;
    br_archive_scan(jd_from(2026, 1, 1), 365, &ar);
    int counts[5];
    br_archive_year_stats(&ar, counts);
    /* Higher thresholds should have fewer or equal days */
    for (int i = 1; i < 5; i++) {
        TEST_ASSERT_TRUE(counts[i] <= counts[i - 1]);
    }
}

/* ===================================================================
 * Main
 * =================================================================== */

int main(void) {
    UNITY_BEGIN();

    /* Basic scan */
    RUN_TEST(test_scan_null_returns_zero);
    RUN_TEST(test_scan_zero_days_returns_zero);
    RUN_TEST(test_scan_negative_days_returns_zero);
    RUN_TEST(test_scan_one_day);
    RUN_TEST(test_scan_30_days);
    RUN_TEST(test_scan_jds_sequential);
    RUN_TEST(test_scan_system_count_nonnegative);
    RUN_TEST(test_scan_significance_range);
    RUN_TEST(test_scan_max_systems_tracked);
    RUN_TEST(test_scan_capped_at_max_days);

    /* 2026 full year */
    RUN_TEST(test_scan_2026_full_year);
    RUN_TEST(test_2026_spring_equinox_notable);
    RUN_TEST(test_2026_has_notable_days);

    /* Notable filter */
    RUN_TEST(test_notable_null_returns_zero);
    RUN_TEST(test_notable_filters_correctly);
    RUN_TEST(test_notable_count_matches);

    /* Best day */
    RUN_TEST(test_best_null_returns_null);
    RUN_TEST(test_best_empty_returns_null);
    RUN_TEST(test_best_returns_highest);

    /* Count above */
    RUN_TEST(test_count_above_null_returns_zero);
    RUN_TEST(test_count_above_zero_threshold);
    RUN_TEST(test_count_above_high_threshold);

    /* Year stats */
    RUN_TEST(test_year_stats_null_safe);
    RUN_TEST(test_year_stats_monotonic);

    return UNITY_END();
}
