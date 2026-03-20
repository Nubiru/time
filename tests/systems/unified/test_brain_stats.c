/* test_brain_stats.c — Tests for statistical convergence analysis.
 *
 * Tests br_stats_percentile, br_stats_year_score,
 * br_stats_year_compare, br_stats_signature_last,
 * br_stats_signature_next. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/brain_stats.h"
#include "../../../src/math/julian.h"
#include "../../../src/systems/unified/convergence_detect.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* Helper: JD for a Gregorian date */
static double jd_from(int y, int m, int d) {
    return gregorian_to_jd(y, m, (double)d);
}

/* ===================================================================
 * br_stats_percentile
 * =================================================================== */

void test_percentile_null_returns_zero(void) {
    TEST_ASSERT_EQUAL_INT(0, br_stats_percentile(jd_from(2026, 3, 20), NULL));
}

void test_percentile_2026_march_20(void) {
    br_stats_percentile_t p;
    int ok = br_stats_percentile(jd_from(2026, 3, 20), &p);
    TEST_ASSERT_EQUAL_INT(1, ok);
    TEST_ASSERT_TRUE(p.system_count >= 0);
    TEST_ASSERT_TRUE(p.rank >= 1);
    TEST_ASSERT_TRUE(p.total_days == 365);
}

void test_percentile_range_0_to_100(void) {
    br_stats_percentile_t p;
    br_stats_percentile(jd_from(2026, 3, 20), &p);
    TEST_ASSERT_TRUE(p.percentile >= 0.0);
    TEST_ASSERT_TRUE(p.percentile <= 100.0);
}

void test_percentile_rank_within_total(void) {
    br_stats_percentile_t p;
    br_stats_percentile(jd_from(2026, 3, 20), &p);
    TEST_ASSERT_TRUE(p.rank >= 1);
    TEST_ASSERT_TRUE(p.rank <= p.total_days);
}

void test_percentile_jd_stored(void) {
    br_stats_percentile_t p;
    double jd = jd_from(2026, 3, 20);
    br_stats_percentile(jd, &p);
    TEST_ASSERT_FLOAT_WITHIN(0.001, jd, p.jd);
}

void test_percentile_system_count_matches_cd(void) {
    /* Verify the percentile's system_count matches a direct cd_scan */
    double jd = jd_from(2026, 6, 15);
    br_stats_percentile_t p;
    br_stats_percentile(jd, &p);

    cd_result_t cd = cd_scan(jd);
    int expected = (cd.count > 0) ? cd.events[0].system_count : 0;
    TEST_ASSERT_EQUAL_INT(expected, p.system_count);
}

/* ===================================================================
 * br_stats_year_score
 * =================================================================== */

void test_year_score_null_returns_zero(void) {
    TEST_ASSERT_EQUAL_INT(0, br_stats_year_score(2026, NULL));
}

void test_year_score_2026(void) {
    br_stats_year_score_t ys;
    int ok = br_stats_year_score(2026, &ys);
    TEST_ASSERT_EQUAL_INT(1, ok);
    TEST_ASSERT_EQUAL_INT(2026, ys.year);
    TEST_ASSERT_TRUE(ys.total_convergence > 0);
}

void test_year_score_notable_days_positive(void) {
    br_stats_year_score_t ys;
    br_stats_year_score(2026, &ys);
    /* A full year should have some days with 3+ systems */
    TEST_ASSERT_TRUE(ys.notable_days > 0);
}

void test_year_score_max_systems_at_least_2(void) {
    br_stats_year_score_t ys;
    br_stats_year_score(2026, &ys);
    TEST_ASSERT_TRUE(ys.max_systems >= 2);
}

void test_year_score_rare_days_leq_notable(void) {
    br_stats_year_score_t ys;
    br_stats_year_score(2026, &ys);
    /* rare (5+) is a subset of notable (3+) */
    TEST_ASSERT_TRUE(ys.rare_days <= ys.notable_days);
}

/* ===================================================================
 * br_stats_year_compare
 * =================================================================== */

void test_year_compare_null_returns_zero(void) {
    TEST_ASSERT_EQUAL_INT(0, br_stats_year_compare(2026, 2, NULL));
}

void test_year_compare_zero_count_returns_zero(void) {
    br_stats_year_compare_t cmp;
    TEST_ASSERT_EQUAL_INT(0, br_stats_year_compare(2026, 0, &cmp));
}

void test_year_compare_negative_count_returns_zero(void) {
    br_stats_year_compare_t cmp;
    TEST_ASSERT_EQUAL_INT(0, br_stats_year_compare(2026, -3, &cmp));
}

void test_year_compare_two_years(void) {
    br_stats_year_compare_t cmp;
    int n = br_stats_year_compare(2025, 2, &cmp);
    TEST_ASSERT_EQUAL_INT(2, n);
    TEST_ASSERT_EQUAL_INT(2, cmp.count);
    TEST_ASSERT_EQUAL_INT(2025, cmp.years[0].year);
    TEST_ASSERT_EQUAL_INT(2026, cmp.years[1].year);
}

void test_year_compare_best_worst_valid(void) {
    br_stats_year_compare_t cmp;
    br_stats_year_compare(2025, 2, &cmp);
    TEST_ASSERT_TRUE(cmp.best_year_index >= 0);
    TEST_ASSERT_TRUE(cmp.best_year_index < cmp.count);
    TEST_ASSERT_TRUE(cmp.worst_year_index >= 0);
    TEST_ASSERT_TRUE(cmp.worst_year_index < cmp.count);
}

void test_year_compare_best_geq_worst(void) {
    br_stats_year_compare_t cmp;
    br_stats_year_compare(2025, 2, &cmp);
    int best_total = cmp.years[cmp.best_year_index].total_convergence;
    int worst_total = cmp.years[cmp.worst_year_index].total_convergence;
    TEST_ASSERT_TRUE(best_total >= worst_total);
}

void test_year_compare_capped_at_max(void) {
    br_stats_year_compare_t cmp;
    int n = br_stats_year_compare(2000, 100, &cmp);
    TEST_ASSERT_EQUAL_INT(BR_STATS_MAX_YEARS, n);
    TEST_ASSERT_EQUAL_INT(BR_STATS_MAX_YEARS, cmp.count);
}

/* ===================================================================
 * br_stats_signature_last
 * =================================================================== */

void test_signature_last_null_systems_returns_zero(void) {
    double result = br_stats_signature_last(jd_from(2026, 3, 20), NULL, 2, 30);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.0, result);
}

void test_signature_last_zero_count_returns_zero(void) {
    int sys[] = {CD_SYS_ASTRONOMY};
    double result = br_stats_signature_last(jd_from(2026, 3, 20), sys, 0, 30);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.0, result);
}

void test_signature_last_zero_maxdays_returns_zero(void) {
    int sys[] = {CD_SYS_ASTRONOMY};
    double result = br_stats_signature_last(jd_from(2026, 3, 20), sys, 1, 0);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.0, result);
}

void test_signature_last_single_system_in_year(void) {
    /* Search for any day with Astronomy in the past 365 days.
     * Astronomy is very common (lunar phases), should find something. */
    int sys[] = {CD_SYS_ASTRONOMY};
    double result = br_stats_signature_last(jd_from(2026, 3, 20), sys, 1, 365);
    /* May or may not find — just ensure no crash and valid JD if found */
    if (result > 0.0) {
        TEST_ASSERT_TRUE(result < jd_from(2026, 3, 20));
    }
}

void test_signature_last_result_is_before_jd(void) {
    /* If we find any match, it must be strictly before the query JD */
    int sys[] = {CD_SYS_HEBREW};
    double jd = jd_from(2026, 3, 20);
    double result = br_stats_signature_last(jd, sys, 1, 365);
    if (result > 0.0) {
        TEST_ASSERT_TRUE(result < jd);
    }
}

/* ===================================================================
 * br_stats_signature_next
 * =================================================================== */

void test_signature_next_null_systems_returns_zero(void) {
    double result = br_stats_signature_next(jd_from(2026, 3, 20), NULL, 2, 30);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.0, result);
}

void test_signature_next_zero_count_returns_zero(void) {
    int sys[] = {CD_SYS_ASTRONOMY};
    double result = br_stats_signature_next(jd_from(2026, 3, 20), sys, 0, 30);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.0, result);
}

void test_signature_next_zero_maxdays_returns_zero(void) {
    int sys[] = {CD_SYS_ASTRONOMY};
    double result = br_stats_signature_next(jd_from(2026, 3, 20), sys, 1, 0);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.0, result);
}

void test_signature_next_single_system_in_year(void) {
    /* Search forward for any day with Islamic in next 365 days */
    int sys[] = {CD_SYS_ISLAMIC};
    double result = br_stats_signature_next(jd_from(2026, 3, 20), sys, 1, 365);
    if (result > 0.0) {
        TEST_ASSERT_TRUE(result > jd_from(2026, 3, 20));
    }
}

void test_signature_next_result_is_after_jd(void) {
    /* If we find any match, it must be strictly after the query JD */
    int sys[] = {CD_SYS_TZOLKIN};
    double jd = jd_from(2026, 1, 1);
    double result = br_stats_signature_next(jd, sys, 1, 365);
    if (result > 0.0) {
        TEST_ASSERT_TRUE(result > jd);
    }
}

/* ===================================================================
 * Main
 * =================================================================== */

int main(void) {
    UNITY_BEGIN();

    /* Percentile */
    RUN_TEST(test_percentile_null_returns_zero);
    RUN_TEST(test_percentile_2026_march_20);
    RUN_TEST(test_percentile_range_0_to_100);
    RUN_TEST(test_percentile_rank_within_total);
    RUN_TEST(test_percentile_jd_stored);
    RUN_TEST(test_percentile_system_count_matches_cd);

    /* Year score */
    RUN_TEST(test_year_score_null_returns_zero);
    RUN_TEST(test_year_score_2026);
    RUN_TEST(test_year_score_notable_days_positive);
    RUN_TEST(test_year_score_max_systems_at_least_2);
    RUN_TEST(test_year_score_rare_days_leq_notable);

    /* Year compare */
    RUN_TEST(test_year_compare_null_returns_zero);
    RUN_TEST(test_year_compare_zero_count_returns_zero);
    RUN_TEST(test_year_compare_negative_count_returns_zero);
    RUN_TEST(test_year_compare_two_years);
    RUN_TEST(test_year_compare_best_worst_valid);
    RUN_TEST(test_year_compare_best_geq_worst);
    RUN_TEST(test_year_compare_capped_at_max);

    /* Signature last */
    RUN_TEST(test_signature_last_null_systems_returns_zero);
    RUN_TEST(test_signature_last_zero_count_returns_zero);
    RUN_TEST(test_signature_last_zero_maxdays_returns_zero);
    RUN_TEST(test_signature_last_single_system_in_year);
    RUN_TEST(test_signature_last_result_is_before_jd);

    /* Signature next */
    RUN_TEST(test_signature_next_null_systems_returns_zero);
    RUN_TEST(test_signature_next_zero_count_returns_zero);
    RUN_TEST(test_signature_next_zero_maxdays_returns_zero);
    RUN_TEST(test_signature_next_single_system_in_year);
    RUN_TEST(test_signature_next_result_is_after_jd);

    return UNITY_END();
}
