/* test_contract_julian.c — Exhaustive contract tests for julian.h
 *
 * Contracts verified:
 *   - Round-trip: jd_to_gregorian(gregorian_to_jd(y,m,d)) == (y,m,d) for post-reform dates
 *   - JD is positive for all dates after -4713
 *   - JD increases monotonically with date
 *   - Month output always 1-12, day output always 1-31 */

#include "../unity/unity.h"
#include "../../src/math/julian.h"
#include <math.h>

#define JD_EPSILON 0.0001

void setUp(void) { }
void tearDown(void) { }

void test_roundtrip_sweep(void) {
    int failures = 0;
    for (int y = 1583; y <= 3000; y += 5) {
        for (int m = 1; m <= 12; m += 3) {
            double jd = gregorian_to_jd(y, m, 1.0);
            int ry, rm;
            double rd = jd_to_gregorian(jd, &ry, &rm);
            if (ry != y || rm != m || fabs(rd - 1.0) > JD_EPSILON) {
                failures++;
            }
        }
    }
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, failures,
        "Round-trip failed for some dates in [1583, 3000]");
}

void test_monotonicity_same_month(void) {
    double jd1 = gregorian_to_jd(2000, 1, 1.0);
    double jd2 = gregorian_to_jd(2000, 1, 2.0);
    TEST_ASSERT_TRUE(jd2 > jd1);
}

void test_monotonicity_across_months(void) {
    double jd1 = gregorian_to_jd(2000, 1, 28.0);
    double jd2 = gregorian_to_jd(2000, 2, 1.0);
    TEST_ASSERT_TRUE(jd2 > jd1);
}

void test_monotonicity_across_years(void) {
    double jd1 = gregorian_to_jd(1999, 12, 31.0);
    double jd2 = gregorian_to_jd(2000, 1, 1.0);
    TEST_ASSERT_TRUE(jd2 > jd1);
}

void test_monotonicity_sweep(void) {
    double prev = gregorian_to_jd(-2000, 1, 1.0);
    int failures = 0;
    for (int y = -1999; y <= 3000; y += 10) {
        double jd = gregorian_to_jd(y, 1, 1.0);
        if (jd <= prev) { failures++; }
        prev = jd;
    }
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, failures, "JD failed monotonicity over year sweep");
}

void test_j2000_anchor(void) {
    double jd = gregorian_to_jd(2000, 1, 1.5);
    TEST_ASSERT_FLOAT_WITHIN(JD_EPSILON, 2451545.0, jd);
}

void test_month_range(void) {
    double jd_start = gregorian_to_jd(-1000, 1, 1.0);
    double jd_end   = gregorian_to_jd(3000, 12, 1.0);
    double step     = (jd_end - jd_start) / 100.0;
    int failures = 0;
    for (int i = 0; i <= 100; i++) {
        double jd = jd_start + i * step;
        int year, month;
        jd_to_gregorian(jd, &year, &month);
        if (month < 1 || month > 12) { failures++; }
    }
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, failures, "Month out of [1,12] for some JDs");
}

void test_day_range(void) {
    double jd_start = gregorian_to_jd(0, 1, 1.0);
    double jd_end   = gregorian_to_jd(3000, 12, 1.0);
    double step     = (jd_end - jd_start) / 100.0;
    int failures = 0;
    for (int i = 0; i <= 100; i++) {
        double jd = jd_start + i * step;
        int year, month;
        double day = jd_to_gregorian(jd, &year, &month);
        if (day < 1.0 || day >= 32.0) { failures++; }
    }
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, failures, "Day out of [1.0, 32.0) for some JDs");
}

void test_jd_positive_after_epoch(void) {
    double jd = gregorian_to_jd(-4000, 1, 1.0);
    TEST_ASSERT_TRUE(jd > 0.0);
}

void test_jd_positive_modern(void) {
    double jd = gregorian_to_jd(2026, 3, 16.0);
    TEST_ASSERT_TRUE(jd > 0.0);
}

void test_extreme_year_negative_4713(void) {
    double jd = gregorian_to_jd(-4713, 1, 1.0);
    TEST_ASSERT_TRUE(isfinite(jd));
}

void test_extreme_year_9999(void) {
    double jd = gregorian_to_jd(9999, 12, 31.0);
    TEST_ASSERT_TRUE(isfinite(jd));
    TEST_ASSERT_TRUE(jd > 0.0);
}

void test_fractional_day_preserved(void) {
    double jd = gregorian_to_jd(2026, 6, 15.5);
    int year, month;
    double day = jd_to_gregorian(jd, &year, &month);
    TEST_ASSERT_EQUAL_INT(2026, year);
    TEST_ASSERT_EQUAL_INT(6, month);
    TEST_ASSERT_FLOAT_WITHIN(JD_EPSILON, 15.5, day);
}

void test_consecutive_days_differ_by_one(void) {
    int failures = 0;
    for (int m = 1; m <= 12; m++) {
        double jd1 = gregorian_to_jd(2024, m, 1.0);
        double jd2 = gregorian_to_jd(2024, m, 2.0);
        if (fabs((jd2 - jd1) - 1.0) > JD_EPSILON) { failures++; }
    }
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, failures, "Consecutive days should differ by exactly 1.0 JD");
}

void test_year_boundary_roundtrip(void) {
    double jd = gregorian_to_jd(2025, 12, 31.0);
    int year, month;
    double day = jd_to_gregorian(jd, &year, &month);
    TEST_ASSERT_EQUAL_INT(2025, year);
    TEST_ASSERT_EQUAL_INT(12, month);
    TEST_ASSERT_FLOAT_WITHIN(JD_EPSILON, 31.0, day);
}

void test_bce_produces_finite(void) {
    double jd = gregorian_to_jd(-500, 6, 15.0);
    TEST_ASSERT_TRUE(isfinite(jd));
    int year, month;
    double day = jd_to_gregorian(jd, &year, &month);
    TEST_ASSERT_TRUE(isfinite(day));
    TEST_ASSERT_TRUE(month >= 1 && month <= 12);
    TEST_ASSERT_TRUE(day >= 1.0 && day < 32.0);
}

void test_leap_feb29_roundtrip(void) {
    double jd = gregorian_to_jd(2024, 2, 29.0);
    int year, month;
    double day = jd_to_gregorian(jd, &year, &month);
    TEST_ASSERT_EQUAL_INT(2024, year);
    TEST_ASSERT_EQUAL_INT(2, month);
    TEST_ASSERT_FLOAT_WITHIN(JD_EPSILON, 29.0, day);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_roundtrip_sweep);
    RUN_TEST(test_monotonicity_same_month);
    RUN_TEST(test_monotonicity_across_months);
    RUN_TEST(test_monotonicity_across_years);
    RUN_TEST(test_monotonicity_sweep);
    RUN_TEST(test_j2000_anchor);
    RUN_TEST(test_month_range);
    RUN_TEST(test_day_range);
    RUN_TEST(test_jd_positive_after_epoch);
    RUN_TEST(test_jd_positive_modern);
    RUN_TEST(test_extreme_year_negative_4713);
    RUN_TEST(test_extreme_year_9999);
    RUN_TEST(test_fractional_day_preserved);
    RUN_TEST(test_consecutive_days_differ_by_one);
    RUN_TEST(test_year_boundary_roundtrip);
    RUN_TEST(test_bce_produces_finite);
    RUN_TEST(test_leap_feb29_roundtrip);
    return UNITY_END();
}
