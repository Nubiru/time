/* test_contract_hijri.c — Exhaustive contract tests for hijri.h */

#include "../unity/unity.h"
#include "../../src/systems/islamic/hijri.h"
#include "../../src/math/julian.h"
#include <math.h>
#include <string.h>

void setUp(void) { }
void tearDown(void) { }

void test_month_range_sweep(void) {
    double s = gregorian_to_jd(1900, 1, 1.0), e = gregorian_to_jd(2100, 12, 31.0);
    double step = (e - s) / 500.0;
    int f = 0;
    for (int i = 0; i <= 500; i++) { hijri_date_t hd = hijri_from_jd(s + i * step); if (hd.month < 1 || hd.month > 12) f++; }
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, f, "Hijri month out of [1,12]");
}

void test_day_range_sweep(void) {
    double s = gregorian_to_jd(1900, 1, 1.0), e = gregorian_to_jd(2100, 12, 31.0);
    double step = (e - s) / 500.0;
    int f = 0;
    for (int i = 0; i <= 500; i++) { hijri_date_t hd = hijri_from_jd(s + i * step); if (hd.day < 1 || hd.day > 30) f++; }
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, f, "Hijri day out of [1,30]");
}

void test_year_monotonic(void) {
    double jd_start = gregorian_to_jd(1950, 1, 1.0);
    int prev = hijri_from_jd(jd_start).year, f = 0;
    for (double jd = jd_start + 30.0; jd < jd_start + 36525.0; jd += 30.0) {
        int cur = hijri_from_jd(jd).year;
        if (cur < prev) f++;
        prev = cur;
    }
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, f, "Hijri year decreased");
}

void test_roundtrip(void) {
    double s = gregorian_to_jd(1950, 1, 1.0), e = gregorian_to_jd(2050, 12, 31.0);
    double step = (e - s) / 200.0;
    int f = 0;
    for (int i = 0; i <= 200; i++) {
        double jd = s + i * step;
        double rt = hijri_to_jd(hijri_from_jd(jd));
        if (fabs(rt - jd) > 1.0) f++;
    }
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, f, "Hijri round-trip > 1.0 day");
}

void test_year_days_values(void) {
    int f = 0;
    for (int hy = 1400; hy <= 1500; hy++) { int d = hijri_year_days(hy); if (d != 354 && d != 355) f++; }
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, f, "Hijri year days not 354 or 355");
}

void test_leap_year_355_days(void) {
    int f = 0;
    for (int hy = 1400; hy <= 1500; hy++) {
        bool leap = hijri_is_leap(hy);
        int d = hijri_year_days(hy);
        if (leap && d != 355) f++;
        if (!leap && d != 354) f++;
    }
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, f, "Leap/common year days mismatch");
}

void test_30_year_cycle_11_leaps(void) {
    int lc = 0;
    for (int i = 1; i <= 30; i++) if (hijri_is_leap(1410 + i)) lc++;
    TEST_ASSERT_EQUAL_INT(11, lc);
}

void test_month_days_range(void) {
    int f = 0;
    for (int hy = 1440; hy <= 1450; hy++)
        for (int m = 1; m <= 12; m++) { int d = hijri_month_days(hy, m); if (d != 29 && d != 30) f++; }
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, f, "Hijri month days not 29 or 30");
}

void test_month_days_invalid(void) {
    TEST_ASSERT_EQUAL_INT(0, hijri_month_days(1445, 0));
    TEST_ASSERT_EQUAL_INT(0, hijri_month_days(1445, 13));
}

void test_month_name_valid(void) {
    for (int m = 1; m <= 12; m++) { const char *n = hijri_month_name(m); TEST_ASSERT_NOT_NULL(n); TEST_ASSERT_TRUE(n[0] != '?'); }
}

void test_month_name_invalid(void) {
    TEST_ASSERT_NOT_NULL(hijri_month_name(0));
    TEST_ASSERT_NOT_NULL(hijri_month_name(13));
}

void test_day_of_year_range(void) {
    double jd_start = gregorian_to_jd(2000, 1, 1.0);
    int f = 0;
    for (int i = 0; i < 500; i++) {
        hijri_date_t hd = hijri_from_jd(jd_start + i * 2.5);
        int doy = hijri_day_of_year(hd);
        if (doy < 1 || doy > hijri_year_days(hd.year)) f++;
    }
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, f, "Day of year out of range");
}

void test_fmt_non_empty(void) {
    hijri_date_t hd = {1445, 9, 15};
    char buf[64];
    hijri_fmt(hd, buf, sizeof(buf));
    TEST_ASSERT_TRUE(strlen(buf) > 0);
}

void test_anchor_modern_date(void) {
    hijri_date_t hd = hijri_from_jd(gregorian_to_jd(2024, 7, 8.0));
    TEST_ASSERT_EQUAL_INT(1446, hd.year);
    TEST_ASSERT_EQUAL_INT(1, hd.month);
    TEST_ASSERT_TRUE(hd.day >= 1 && hd.day <= 5);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_month_range_sweep);
    RUN_TEST(test_day_range_sweep);
    RUN_TEST(test_year_monotonic);
    RUN_TEST(test_roundtrip);
    RUN_TEST(test_year_days_values);
    RUN_TEST(test_leap_year_355_days);
    RUN_TEST(test_30_year_cycle_11_leaps);
    RUN_TEST(test_month_days_range);
    RUN_TEST(test_month_days_invalid);
    RUN_TEST(test_month_name_valid);
    RUN_TEST(test_month_name_invalid);
    RUN_TEST(test_day_of_year_range);
    RUN_TEST(test_fmt_non_empty);
    RUN_TEST(test_anchor_modern_date);
    return UNITY_END();
}
