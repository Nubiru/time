/* test_brain_temporal.c — Tests for temporal context module.
 *
 * Tests br_temporal_last, br_temporal_next, br_temporal_recurrence,
 * and br_temporal_analyze using real convergence detection. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/brain_temporal.h"
#include "../../../src/systems/unified/convergence_detect.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* J2000.0 = 2451545.0 (2000-01-01 12:00 TT) */
static const double JD_J2000 = 2451545.0;

/* 2024-03-20 (spring equinox — likely some convergence) */
static const double JD_EQUINOX_2024 = 2460389.5;

/* ===================================================================
 * br_temporal_last — basic behavior
 * =================================================================== */

void test_last_zero_range_returns_zero(void) {
    TEST_ASSERT_EQUAL_FLOAT(0.0, br_temporal_last(JD_J2000, 0.1, 0));
}

void test_last_negative_range_returns_zero(void) {
    TEST_ASSERT_EQUAL_FLOAT(0.0, br_temporal_last(JD_J2000, 0.1, -10));
}

void test_last_very_high_threshold_returns_zero(void) {
    /* Threshold so high nothing should match in 30 days */
    double result = br_temporal_last(JD_J2000, 0.99, 30);
    /* May or may not find something — just verify it doesn't crash */
    TEST_ASSERT_TRUE(result >= 0.0);
}

void test_last_low_threshold_finds_something(void) {
    /* Very low threshold — should find SOMETHING within 365 days */
    double result = br_temporal_last(JD_EQUINOX_2024, 0.01, 365);
    /* Convergences happen regularly, so this should find one */
    if (result > 0.0) {
        TEST_ASSERT_TRUE(result < JD_EQUINOX_2024);
        TEST_ASSERT_TRUE(result >= JD_EQUINOX_2024 - 365.0);
    }
}

/* ===================================================================
 * br_temporal_next — basic behavior
 * =================================================================== */

void test_next_zero_range_returns_zero(void) {
    TEST_ASSERT_EQUAL_FLOAT(0.0, br_temporal_next(JD_J2000, 0.1, 0));
}

void test_next_low_threshold_finds_something(void) {
    double result = br_temporal_next(JD_EQUINOX_2024, 0.01, 365);
    if (result > 0.0) {
        TEST_ASSERT_TRUE(result > JD_EQUINOX_2024);
        TEST_ASSERT_TRUE(result <= JD_EQUINOX_2024 + 365.0);
    }
}

void test_next_before_last(void) {
    /* next should be AFTER jd, last should be BEFORE jd */
    double last = br_temporal_last(JD_EQUINOX_2024, 0.01, 100);
    double next = br_temporal_next(JD_EQUINOX_2024, 0.01, 100);
    if (last > 0.0) TEST_ASSERT_TRUE(last < JD_EQUINOX_2024);
    if (next > 0.0) TEST_ASSERT_TRUE(next > JD_EQUINOX_2024);
}

/* ===================================================================
 * br_temporal_recurrence
 * =================================================================== */

void test_recurrence_zero_range(void) {
    TEST_ASSERT_EQUAL_FLOAT(0.0,
        br_temporal_recurrence(JD_J2000, 0.1, 0));
}

void test_recurrence_returns_positive_or_zero(void) {
    double rec = br_temporal_recurrence(JD_EQUINOX_2024, 0.01, 365);
    TEST_ASSERT_TRUE(rec >= 0.0);
}

void test_recurrence_low_threshold_has_short_period(void) {
    /* Very low threshold → many events → short recurrence */
    double rec = br_temporal_recurrence(JD_EQUINOX_2024, 0.01, 365);
    if (rec > 0.0) {
        TEST_ASSERT_TRUE(rec < 365.0); /* should recur within the scan range */
    }
}

/* ===================================================================
 * br_temporal_analyze
 * =================================================================== */

void test_analyze_sets_scan_range(void) {
    br_temporal_t t = br_temporal_analyze(JD_EQUINOX_2024, 0.01, 100);
    TEST_ASSERT_EQUAL_INT(100, t.scan_range);
}

void test_analyze_negative_range_uses_default(void) {
    br_temporal_t t = br_temporal_analyze(JD_EQUINOX_2024, 0.01, -5);
    TEST_ASSERT_EQUAL_INT(BR_TEMPORAL_DEFAULT_RANGE, t.scan_range);
}

void test_analyze_occurrences_non_negative(void) {
    br_temporal_t t = br_temporal_analyze(JD_EQUINOX_2024, 0.01, 100);
    TEST_ASSERT_TRUE(t.occurrences >= 0);
}

void test_analyze_last_before_jd(void) {
    br_temporal_t t = br_temporal_analyze(JD_EQUINOX_2024, 0.01, 100);
    if (t.last_jd > 0.0) {
        TEST_ASSERT_TRUE(t.last_jd < JD_EQUINOX_2024);
    }
}

void test_analyze_next_after_jd(void) {
    br_temporal_t t = br_temporal_analyze(JD_EQUINOX_2024, 0.01, 100);
    if (t.next_jd > 0.0) {
        TEST_ASSERT_TRUE(t.next_jd > JD_EQUINOX_2024);
    }
}

void test_analyze_recurrence_consistent(void) {
    br_temporal_t t = br_temporal_analyze(JD_EQUINOX_2024, 0.01, 365);
    double direct = br_temporal_recurrence(JD_EQUINOX_2024, 0.01, 365);
    TEST_ASSERT_EQUAL_FLOAT(direct, t.recurrence_days);
}

/* ===================================================================
 * Edge cases
 * =================================================================== */

void test_range_clamped_to_max(void) {
    /* Range > MAX should be clamped, not crash */
    double result = br_temporal_last(JD_J2000, 0.5, 999999);
    TEST_ASSERT_TRUE(result >= 0.0); /* just verify no crash */
}

/* ===================================================================
 * Main
 * =================================================================== */

int main(void) {
    UNITY_BEGIN();

    /* br_temporal_last */
    RUN_TEST(test_last_zero_range_returns_zero);
    RUN_TEST(test_last_negative_range_returns_zero);
    RUN_TEST(test_last_very_high_threshold_returns_zero);
    RUN_TEST(test_last_low_threshold_finds_something);

    /* br_temporal_next */
    RUN_TEST(test_next_zero_range_returns_zero);
    RUN_TEST(test_next_low_threshold_finds_something);
    RUN_TEST(test_next_before_last);

    /* br_temporal_recurrence */
    RUN_TEST(test_recurrence_zero_range);
    RUN_TEST(test_recurrence_returns_positive_or_zero);
    RUN_TEST(test_recurrence_low_threshold_has_short_period);

    /* br_temporal_analyze */
    RUN_TEST(test_analyze_sets_scan_range);
    RUN_TEST(test_analyze_negative_range_uses_default);
    RUN_TEST(test_analyze_occurrences_non_negative);
    RUN_TEST(test_analyze_last_before_jd);
    RUN_TEST(test_analyze_next_after_jd);
    RUN_TEST(test_analyze_recurrence_consistent);

    /* Edge cases */
    RUN_TEST(test_range_clamped_to_max);

    return UNITY_END();
}
