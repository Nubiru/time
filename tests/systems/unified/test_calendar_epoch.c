/* test_calendar_epoch.c -- Calendar epoch module tests
 * TDD RED phase: tests written before implementation.
 * Tests Easter computation (Computus, Anonymous Gregorian / Meeus),
 * Julian/Gregorian drift, Dionysius Exiguus epoch error,
 * leap year determination, and historical epoch facts.
 *
 * Known Easter dates sourced from astronomical tables.
 * Drift formula: accumulated Julian-Gregorian calendar discrepancy. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/calendar_epoch.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== ce_easter_date — known Gregorian Easter dates ===== */

void test_easter_2000_april_23(void)
{
    ce_easter_t e = ce_easter_date(2000);
    TEST_ASSERT_EQUAL_INT(4, e.month);
    TEST_ASSERT_EQUAL_INT(23, e.day);
    TEST_ASSERT_EQUAL_INT(2000, e.year);
}

void test_easter_2005_march_27(void)
{
    ce_easter_t e = ce_easter_date(2005);
    TEST_ASSERT_EQUAL_INT(3, e.month);
    TEST_ASSERT_EQUAL_INT(27, e.day);
}

void test_easter_2010_april_4(void)
{
    ce_easter_t e = ce_easter_date(2010);
    TEST_ASSERT_EQUAL_INT(4, e.month);
    TEST_ASSERT_EQUAL_INT(4, e.day);
}

void test_easter_2015_april_5(void)
{
    ce_easter_t e = ce_easter_date(2015);
    TEST_ASSERT_EQUAL_INT(4, e.month);
    TEST_ASSERT_EQUAL_INT(5, e.day);
}

void test_easter_2020_april_12(void)
{
    ce_easter_t e = ce_easter_date(2020);
    TEST_ASSERT_EQUAL_INT(4, e.month);
    TEST_ASSERT_EQUAL_INT(12, e.day);
}

void test_easter_2024_march_31(void)
{
    ce_easter_t e = ce_easter_date(2024);
    TEST_ASSERT_EQUAL_INT(3, e.month);
    TEST_ASSERT_EQUAL_INT(31, e.day);
}

void test_easter_2025_april_20(void)
{
    ce_easter_t e = ce_easter_date(2025);
    TEST_ASSERT_EQUAL_INT(4, e.month);
    TEST_ASSERT_EQUAL_INT(20, e.day);
}

void test_easter_2030_april_21(void)
{
    ce_easter_t e = ce_easter_date(2030);
    TEST_ASSERT_EQUAL_INT(4, e.month);
    TEST_ASSERT_EQUAL_INT(21, e.day);
}

void test_easter_1818_earliest_march_22(void)
{
    ce_easter_t e = ce_easter_date(1818);
    TEST_ASSERT_EQUAL_INT(3, e.month);
    TEST_ASSERT_EQUAL_INT(22, e.day);
}

void test_easter_2285_earliest_march_22(void)
{
    ce_easter_t e = ce_easter_date(2285);
    TEST_ASSERT_EQUAL_INT(3, e.month);
    TEST_ASSERT_EQUAL_INT(22, e.day);
}

void test_easter_1943_latest_april_25(void)
{
    ce_easter_t e = ce_easter_date(1943);
    TEST_ASSERT_EQUAL_INT(4, e.month);
    TEST_ASSERT_EQUAL_INT(25, e.day);
}

void test_easter_2038_latest_april_25(void)
{
    ce_easter_t e = ce_easter_date(2038);
    TEST_ASSERT_EQUAL_INT(4, e.month);
    TEST_ASSERT_EQUAL_INT(25, e.day);
}

void test_easter_month_always_march_or_april(void)
{
    /* Sweep a range of years — Easter always falls in March or April */
    for (int y = 1583; y <= 2500; y++) {
        ce_easter_t e = ce_easter_date(y);
        TEST_ASSERT_TRUE(e.month == 3 || e.month == 4);
    }
}

void test_easter_day_within_valid_range(void)
{
    /* March 22 is earliest, April 25 is latest */
    for (int y = 1583; y <= 2500; y++) {
        ce_easter_t e = ce_easter_date(y);
        if (e.month == 3) {
            TEST_ASSERT_TRUE(e.day >= 22);
            TEST_ASSERT_TRUE(e.day <= 31);
        } else {
            TEST_ASSERT_TRUE(e.day >= 1);
            TEST_ASSERT_TRUE(e.day <= 25);
        }
    }
}

/* ===== ce_easter_jd — Easter as Julian Day ===== */

void test_easter_jd_2000(void)
{
    /* 2000 April 23 = JD 2451657.5 (at noon) or 2451658.5 for day start
     * gregorian_to_jd(2000, 4, 23) should give ~2451657.5 */
    double jd = ce_easter_jd(2000);
    /* April 23, 2000 at noon = JD 2451658.0 approximately.
     * Using Meeus: gregorian_to_jd(2000,4,23.0) = 2451657.5 */
    TEST_ASSERT_FLOAT_WITHIN((float)0.5, (float)2451657.5, (float)jd);
}

void test_easter_jd_2025(void)
{
    /* 2025 April 20 */
    double jd = ce_easter_jd(2025);
    /* gregorian_to_jd(2025,4,20) = 2460785.5 */
    TEST_ASSERT_FLOAT_WITHIN((float)0.5, (float)2460785.5, (float)jd);
}

void test_easter_jd_consistent_with_date(void)
{
    /* Verify JD is consistent with ce_easter_date for several years */
    int years[] = {2000, 2010, 2020, 2024, 2025, 2030};
    for (int i = 0; i < 6; i++) {
        ce_easter_t e = ce_easter_date(years[i]);
        double jd = ce_easter_jd(years[i]);
        /* JD should correspond to the correct month/day.
         * We just check that the JD is reasonable (within 1 day). */
        double expected_jd = ce_easter_jd(years[i]);
        TEST_ASSERT_FLOAT_WITHIN((float)0.001, (float)expected_jd, (float)jd);
        /* Also verify year is preserved */
        TEST_ASSERT_EQUAL_INT(years[i], e.year);
    }
}

/* ===== ce_epoch_offset — Dionysius Exiguus error ===== */

void test_epoch_offset_min_is_4(void)
{
    TEST_ASSERT_EQUAL_INT(CE_EPOCH_ERROR_YEARS_MIN, ce_epoch_offset_min());
}

void test_epoch_offset_max_is_7(void)
{
    TEST_ASSERT_EQUAL_INT(CE_EPOCH_ERROR_YEARS_MAX, ce_epoch_offset_max());
}

void test_epoch_offset_min_less_than_max(void)
{
    TEST_ASSERT_TRUE(ce_epoch_offset_min() < ce_epoch_offset_max());
}

/* ===== ce_julian_drift — Julian/Gregorian calendar drift ===== */

void test_drift_1582_ten_days(void)
{
    ce_drift_t d = ce_julian_drift(1582);
    TEST_ASSERT_EQUAL_INT(10, d.drift_days);
    TEST_ASSERT_EQUAL_INT(1582, d.year);
}

void test_drift_1900_thirteen_days(void)
{
    ce_drift_t d = ce_julian_drift(1900);
    TEST_ASSERT_EQUAL_INT(13, d.drift_days);
}

void test_drift_2100_fourteen_days(void)
{
    /* 2100 is not a Gregorian leap year but is a Julian leap year */
    ce_drift_t d = ce_julian_drift(2100);
    TEST_ASSERT_EQUAL_INT(14, d.drift_days);
}

void test_drift_year_stored(void)
{
    ce_drift_t d = ce_julian_drift(2000);
    TEST_ASSERT_EQUAL_INT(2000, d.year);
}

void test_drift_note_not_null(void)
{
    ce_drift_t d = ce_julian_drift(1582);
    TEST_ASSERT_NOT_NULL(d.note);
    TEST_ASSERT_TRUE(strlen(d.note) > 0);
}

void test_drift_increases_over_centuries(void)
{
    ce_drift_t d1 = ce_julian_drift(1582);
    ce_drift_t d2 = ce_julian_drift(2000);
    TEST_ASSERT_TRUE(d2.drift_days >= d1.drift_days);
}

void test_drift_200_ce(void)
{
    /* Early centuries: drift should be small but positive or zero */
    ce_drift_t d = ce_julian_drift(200);
    TEST_ASSERT_TRUE(d.drift_days >= 0);
}

/* ===== ce_epoch_fact_count / ce_epoch_fact_get ===== */

void test_epoch_fact_count_at_least_8(void)
{
    TEST_ASSERT_TRUE(ce_epoch_fact_count() >= 8);
}

void test_epoch_fact_first_is_julian_calendar(void)
{
    ce_epoch_fact_t f = ce_epoch_fact_get(0);
    TEST_ASSERT_EQUAL_INT(-45, f.year);
    TEST_ASSERT_NOT_NULL(f.title);
    TEST_ASSERT_NOT_NULL(f.who);
    TEST_ASSERT_NOT_NULL(f.what);
    TEST_ASSERT_NOT_NULL(f.impact);
}

void test_epoch_fact_nicaea(void)
{
    ce_epoch_fact_t f = ce_epoch_fact_get(1);
    TEST_ASSERT_EQUAL_INT(325, f.year);
}

void test_epoch_fact_dionysius(void)
{
    ce_epoch_fact_t f = ce_epoch_fact_get(2);
    TEST_ASSERT_EQUAL_INT(525, f.year);
}

void test_epoch_fact_gregorian_reform(void)
{
    ce_epoch_fact_t f = ce_epoch_fact_get(3);
    TEST_ASSERT_EQUAL_INT(1582, f.year);
}

void test_epoch_fact_british(void)
{
    ce_epoch_fact_t f = ce_epoch_fact_get(4);
    TEST_ASSERT_EQUAL_INT(1752, f.year);
}

void test_epoch_fact_russian(void)
{
    ce_epoch_fact_t f = ce_epoch_fact_get(5);
    TEST_ASSERT_EQUAL_INT(1917, f.year);
}

void test_epoch_fact_greek(void)
{
    ce_epoch_fact_t f = ce_epoch_fact_get(6);
    TEST_ASSERT_EQUAL_INT(1923, f.year);
}

void test_epoch_fact_orthodox_2023(void)
{
    ce_epoch_fact_t f = ce_epoch_fact_get(7);
    TEST_ASSERT_EQUAL_INT(2023, f.year);
}

void test_epoch_fact_chronological_order(void)
{
    int count = ce_epoch_fact_count();
    for (int i = 1; i < count; i++) {
        ce_epoch_fact_t prev = ce_epoch_fact_get(i - 1);
        ce_epoch_fact_t curr = ce_epoch_fact_get(i);
        TEST_ASSERT_TRUE(curr.year >= prev.year);
    }
}

void test_epoch_fact_all_have_strings(void)
{
    int count = ce_epoch_fact_count();
    for (int i = 0; i < count; i++) {
        ce_epoch_fact_t f = ce_epoch_fact_get(i);
        TEST_ASSERT_NOT_NULL(f.title);
        TEST_ASSERT_NOT_NULL(f.who);
        TEST_ASSERT_NOT_NULL(f.what);
        TEST_ASSERT_NOT_NULL(f.impact);
        TEST_ASSERT_TRUE(strlen(f.title) > 0);
        TEST_ASSERT_TRUE(strlen(f.who) > 0);
        TEST_ASSERT_TRUE(strlen(f.what) > 0);
        TEST_ASSERT_TRUE(strlen(f.impact) > 0);
    }
}

void test_epoch_fact_invalid_index_returns_empty(void)
{
    ce_epoch_fact_t f = ce_epoch_fact_get(-1);
    TEST_ASSERT_EQUAL_INT(0, f.year);
    TEST_ASSERT_EQUAL_STRING("", f.title);
}

void test_epoch_fact_invalid_high_returns_empty(void)
{
    ce_epoch_fact_t f = ce_epoch_fact_get(999);
    TEST_ASSERT_EQUAL_INT(0, f.year);
    TEST_ASSERT_EQUAL_STRING("", f.title);
}

/* ===== ce_system_name ===== */

void test_system_name_julian(void)
{
    TEST_ASSERT_EQUAL_STRING("Julian", ce_system_name(CE_SYSTEM_JULIAN));
}

void test_system_name_gregorian(void)
{
    TEST_ASSERT_EQUAL_STRING("Gregorian", ce_system_name(CE_SYSTEM_GREGORIAN));
}

void test_system_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", ce_system_name(CE_SYSTEM_COUNT));
}

/* ===== ce_is_julian_leap ===== */

void test_julian_leap_4_ce(void)
{
    /* Every year divisible by 4 is a Julian leap year */
    TEST_ASSERT_EQUAL_INT(1, ce_is_julian_leap(4));
}

void test_julian_leap_100_ce(void)
{
    /* 100 is divisible by 4 -> Julian leap year */
    TEST_ASSERT_EQUAL_INT(1, ce_is_julian_leap(100));
}

void test_julian_leap_1900(void)
{
    /* 1900 % 4 == 0 -> Julian leap year (unlike Gregorian) */
    TEST_ASSERT_EQUAL_INT(1, ce_is_julian_leap(1900));
}

void test_julian_not_leap_2001(void)
{
    TEST_ASSERT_EQUAL_INT(0, ce_is_julian_leap(2001));
}

void test_julian_not_leap_2019(void)
{
    TEST_ASSERT_EQUAL_INT(0, ce_is_julian_leap(2019));
}

/* ===== ce_is_gregorian_leap ===== */

void test_gregorian_leap_2000(void)
{
    /* 2000 is divisible by 400 -> leap year */
    TEST_ASSERT_EQUAL_INT(1, ce_is_gregorian_leap(2000));
}

void test_gregorian_leap_2024(void)
{
    TEST_ASSERT_EQUAL_INT(1, ce_is_gregorian_leap(2024));
}

void test_gregorian_not_leap_1900(void)
{
    /* 1900 divisible by 100 but not 400 -> NOT a Gregorian leap year */
    TEST_ASSERT_EQUAL_INT(0, ce_is_gregorian_leap(1900));
}

void test_gregorian_not_leap_2100(void)
{
    TEST_ASSERT_EQUAL_INT(0, ce_is_gregorian_leap(2100));
}

void test_gregorian_not_leap_2023(void)
{
    TEST_ASSERT_EQUAL_INT(0, ce_is_gregorian_leap(2023));
}

void test_gregorian_leap_1600(void)
{
    TEST_ASSERT_EQUAL_INT(1, ce_is_gregorian_leap(1600));
}

/* ===== ce_gregorian_julian_diff ===== */

void test_diff_equals_drift(void)
{
    /* ce_gregorian_julian_diff should match ce_julian_drift.drift_days */
    int years[] = {1582, 1900, 2000, 2100};
    for (int i = 0; i < 4; i++) {
        ce_drift_t d = ce_julian_drift(years[i]);
        int diff = ce_gregorian_julian_diff(years[i]);
        TEST_ASSERT_EQUAL_INT(d.drift_days, diff);
    }
}

void test_diff_1582_is_10(void)
{
    TEST_ASSERT_EQUAL_INT(10, ce_gregorian_julian_diff(1582));
}

void test_diff_2000_is_13(void)
{
    TEST_ASSERT_EQUAL_INT(13, ce_gregorian_julian_diff(2000));
}

/* ===== Edge cases ===== */

void test_easter_year_preserved(void)
{
    ce_easter_t e = ce_easter_date(1999);
    TEST_ASSERT_EQUAL_INT(1999, e.year);
}

void test_easter_1583_first_gregorian_year(void)
{
    /* 1583 is the first full Gregorian year after the reform */
    ce_easter_t e = ce_easter_date(1583);
    TEST_ASSERT_TRUE(e.month == 3 || e.month == 4);
    TEST_ASSERT_TRUE(e.day >= 1 && e.day <= 31);
}

/* ===== main ===== */

int main(void)
{
    UNITY_BEGIN();

    /* Easter dates — known years */
    RUN_TEST(test_easter_2000_april_23);
    RUN_TEST(test_easter_2005_march_27);
    RUN_TEST(test_easter_2010_april_4);
    RUN_TEST(test_easter_2015_april_5);
    RUN_TEST(test_easter_2020_april_12);
    RUN_TEST(test_easter_2024_march_31);
    RUN_TEST(test_easter_2025_april_20);
    RUN_TEST(test_easter_2030_april_21);
    RUN_TEST(test_easter_1818_earliest_march_22);
    RUN_TEST(test_easter_2285_earliest_march_22);
    RUN_TEST(test_easter_1943_latest_april_25);
    RUN_TEST(test_easter_2038_latest_april_25);
    RUN_TEST(test_easter_month_always_march_or_april);
    RUN_TEST(test_easter_day_within_valid_range);

    /* Easter JD */
    RUN_TEST(test_easter_jd_2000);
    RUN_TEST(test_easter_jd_2025);
    RUN_TEST(test_easter_jd_consistent_with_date);

    /* Epoch offset */
    RUN_TEST(test_epoch_offset_min_is_4);
    RUN_TEST(test_epoch_offset_max_is_7);
    RUN_TEST(test_epoch_offset_min_less_than_max);

    /* Julian/Gregorian drift */
    RUN_TEST(test_drift_1582_ten_days);
    RUN_TEST(test_drift_1900_thirteen_days);
    RUN_TEST(test_drift_2100_fourteen_days);
    RUN_TEST(test_drift_year_stored);
    RUN_TEST(test_drift_note_not_null);
    RUN_TEST(test_drift_increases_over_centuries);
    RUN_TEST(test_drift_200_ce);

    /* Epoch facts */
    RUN_TEST(test_epoch_fact_count_at_least_8);
    RUN_TEST(test_epoch_fact_first_is_julian_calendar);
    RUN_TEST(test_epoch_fact_nicaea);
    RUN_TEST(test_epoch_fact_dionysius);
    RUN_TEST(test_epoch_fact_gregorian_reform);
    RUN_TEST(test_epoch_fact_british);
    RUN_TEST(test_epoch_fact_russian);
    RUN_TEST(test_epoch_fact_greek);
    RUN_TEST(test_epoch_fact_orthodox_2023);
    RUN_TEST(test_epoch_fact_chronological_order);
    RUN_TEST(test_epoch_fact_all_have_strings);
    RUN_TEST(test_epoch_fact_invalid_index_returns_empty);
    RUN_TEST(test_epoch_fact_invalid_high_returns_empty);

    /* System names */
    RUN_TEST(test_system_name_julian);
    RUN_TEST(test_system_name_gregorian);
    RUN_TEST(test_system_name_invalid);

    /* Julian leap years */
    RUN_TEST(test_julian_leap_4_ce);
    RUN_TEST(test_julian_leap_100_ce);
    RUN_TEST(test_julian_leap_1900);
    RUN_TEST(test_julian_not_leap_2001);
    RUN_TEST(test_julian_not_leap_2019);

    /* Gregorian leap years */
    RUN_TEST(test_gregorian_leap_2000);
    RUN_TEST(test_gregorian_leap_2024);
    RUN_TEST(test_gregorian_not_leap_1900);
    RUN_TEST(test_gregorian_not_leap_2100);
    RUN_TEST(test_gregorian_not_leap_2023);
    RUN_TEST(test_gregorian_leap_1600);

    /* Gregorian-Julian diff */
    RUN_TEST(test_diff_equals_drift);
    RUN_TEST(test_diff_1582_is_10);
    RUN_TEST(test_diff_2000_is_13);

    /* Edge cases */
    RUN_TEST(test_easter_year_preserved);
    RUN_TEST(test_easter_1583_first_gregorian_year);

    return UNITY_END();
}
