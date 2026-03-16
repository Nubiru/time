/* test_contract_hebrew.c — Exhaustive contract tests for hebrew.h */

#include "../unity/unity.h"
#include "../../src/systems/hebrew/hebrew.h"
#include "../../src/math/julian.h"
#include <math.h>
#include <stdbool.h>

void setUp(void) { }
void tearDown(void) { }

void test_month_range_sweep(void) {
    double s = gregorian_to_jd(1900, 1, 1.0), e = gregorian_to_jd(2100, 12, 31.0);
    double step = (e - s) / 500.0;
    int f = 0;
    for (int i = 0; i <= 500; i++) { hebrew_date_t hd = hebrew_from_jd(s + i * step); if (hd.month < 1 || hd.month > 13) f++; }
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, f, "Hebrew month out of [1,13]");
}

void test_day_range_sweep(void) {
    double s = gregorian_to_jd(1900, 1, 1.0), e = gregorian_to_jd(2100, 12, 31.0);
    double step = (e - s) / 500.0;
    int f = 0;
    for (int i = 0; i <= 500; i++) { hebrew_date_t hd = hebrew_from_jd(s + i * step); if (hd.day < 1 || hd.day > 30) f++; }
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, f, "Hebrew day out of [1,30]");
}

void test_leap_year_has_13_months(void) {
    int f = 0;
    for (int hy = 5780; hy <= 5800; hy++) {
        bool leap = hebrew_is_leap(hy);
        int months = hebrew_months_in_year(hy);
        if (leap && months != 13) f++;
        if (!leap && months != 12) f++;
    }
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, f, "months_in_year inconsistent with is_leap");
}

void test_month_13_only_in_leap_year(void) {
    double s = gregorian_to_jd(1950, 1, 1.0), e = gregorian_to_jd(2050, 12, 31.0);
    double step = (e - s) / 500.0;
    int f = 0;
    for (int i = 0; i <= 500; i++) { hebrew_date_t hd = hebrew_from_jd(s + i * step); if (hd.month == 13 && !hebrew_is_leap(hd.year)) f++; }
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, f, "Month 13 in non-leap year");
}

void test_year_monotonic(void) {
    double jd_start = gregorian_to_jd(1950, 1, 1.0);
    int prev = hebrew_from_jd(jd_start).year, f = 0;
    for (double jd = jd_start + 30.0; jd < jd_start + 36525.0; jd += 30.0) {
        int cur = hebrew_from_jd(jd).year;
        if (cur < prev) f++;
        prev = cur;
    }
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, f, "Hebrew year decreased");
}

void test_roundtrip(void) {
    double s = gregorian_to_jd(1950, 1, 1.0), e = gregorian_to_jd(2050, 12, 31.0);
    double step = (e - s) / 200.0;
    int f = 0;
    for (int i = 0; i <= 200; i++) {
        double jd = s + i * step;
        double rt = hebrew_to_jd(hebrew_from_jd(jd));
        if (fabs(rt - jd) > 1.0) f++;
    }
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, f, "Hebrew round-trip > 1.0 day");
}

void test_year_days_values(void) {
    int f = 0;
    for (int hy = 5750; hy <= 5850; hy++) {
        int d = hebrew_year_days(hy);
        bool leap = hebrew_is_leap(hy);
        if (leap) { if (d < 382 || d > 386) f++; }
        else { if (d < 352 || d > 356) f++; }
    }
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, f, "Hebrew year days outside bounds");
}

void test_year_type_consistent(void) {
    int f = 0;
    for (int hy = 5750; hy <= 5850; hy++) {
        int d = hebrew_year_days(hy);
        hebrew_year_type_t t = hebrew_year_type(hy);
        if ((int)t != (d % 10) + 350) f++;
    }
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, f, "Year type inconsistent with days");
}

void test_metonic_cycle_7_leaps_in_19(void) {
    for (int start = 5780; start <= 5800; start++) {
        int lc = 0;
        for (int i = 0; i < 19; i++) if (hebrew_is_leap(start + i)) lc++;
        TEST_ASSERT_EQUAL_INT(7, lc);
    }
}

void test_month_days_range(void) {
    int f = 0;
    for (int hy = 5780; hy <= 5790; hy++) {
        int months = hebrew_months_in_year(hy);
        for (int m = 1; m <= months; m++) {
            int d = hebrew_month_days(hy, m);
            if (d < 29 || d > 30) f++;
        }
    }
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, f, "Hebrew month days not 29 or 30");
}

void test_month_name_valid(void) {
    for (int m = 1; m <= 13; m++) {
        TEST_ASSERT_NOT_NULL(hebrew_month_name(m, true));
        TEST_ASSERT_NOT_NULL(hebrew_month_name(m, false));
    }
}

void test_anchor_rosh_hashana_5785(void) {
    hebrew_date_t hd = hebrew_from_jd(gregorian_to_jd(2024, 10, 3.0));
    TEST_ASSERT_EQUAL_INT(5785, hd.year);
    TEST_ASSERT_EQUAL_INT(7, hd.month);
    TEST_ASSERT_EQUAL_INT(1, hd.day);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_month_range_sweep);
    RUN_TEST(test_day_range_sweep);
    RUN_TEST(test_leap_year_has_13_months);
    RUN_TEST(test_month_13_only_in_leap_year);
    RUN_TEST(test_year_monotonic);
    RUN_TEST(test_roundtrip);
    RUN_TEST(test_year_days_values);
    RUN_TEST(test_year_type_consistent);
    RUN_TEST(test_metonic_cycle_7_leaps_in_19);
    RUN_TEST(test_month_days_range);
    RUN_TEST(test_month_name_valid);
    RUN_TEST(test_anchor_rosh_hashana_5785);
    return UNITY_END();
}
