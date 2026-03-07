/* test_persian.c — Persian Solar Hijri (Jalali) calendar tests
 * TDD RED phase: tests written before implementation.
 * Algorithm: Reingold & Dershowitz "Calendrical Calculations" (2018). */

#include "../../unity/unity.h"
#include "../../../src/systems/persian/persian.h"
#include <math.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== Month name tests ===== */

void test_month_name_farvardin(void)
{
    TEST_ASSERT_EQUAL_STRING("Farvardin", persian_month_name(1));
}

void test_month_name_ordibehesht(void)
{
    TEST_ASSERT_EQUAL_STRING("Ordibehesht", persian_month_name(2));
}

void test_month_name_khordad(void)
{
    TEST_ASSERT_EQUAL_STRING("Khordad", persian_month_name(3));
}

void test_month_name_tir(void)
{
    TEST_ASSERT_EQUAL_STRING("Tir", persian_month_name(4));
}

void test_month_name_mordad(void)
{
    TEST_ASSERT_EQUAL_STRING("Mordad", persian_month_name(5));
}

void test_month_name_shahrivar(void)
{
    TEST_ASSERT_EQUAL_STRING("Shahrivar", persian_month_name(6));
}

void test_month_name_mehr(void)
{
    TEST_ASSERT_EQUAL_STRING("Mehr", persian_month_name(7));
}

void test_month_name_aban(void)
{
    TEST_ASSERT_EQUAL_STRING("Aban", persian_month_name(8));
}

void test_month_name_azar(void)
{
    TEST_ASSERT_EQUAL_STRING("Azar", persian_month_name(9));
}

void test_month_name_dey(void)
{
    TEST_ASSERT_EQUAL_STRING("Dey", persian_month_name(10));
}

void test_month_name_bahman(void)
{
    TEST_ASSERT_EQUAL_STRING("Bahman", persian_month_name(11));
}

void test_month_name_esfand(void)
{
    TEST_ASSERT_EQUAL_STRING("Esfand", persian_month_name(12));
}

void test_month_name_invalid_zero(void)
{
    TEST_ASSERT_EQUAL_STRING("?", persian_month_name(0));
}

void test_month_name_invalid_thirteen(void)
{
    TEST_ASSERT_EQUAL_STRING("?", persian_month_name(13));
}

void test_month_name_invalid_negative(void)
{
    TEST_ASSERT_EQUAL_STRING("?", persian_month_name(-1));
}

/* ===== Weekday name tests ===== */

void test_weekday_name_shanbe(void)
{
    TEST_ASSERT_EQUAL_STRING("Shanbe", persian_weekday_name(0));
}

void test_weekday_name_yekshanbe(void)
{
    TEST_ASSERT_EQUAL_STRING("Yekshanbe", persian_weekday_name(1));
}

void test_weekday_name_doshanbe(void)
{
    TEST_ASSERT_EQUAL_STRING("Doshanbe", persian_weekday_name(2));
}

void test_weekday_name_seshanbe(void)
{
    TEST_ASSERT_EQUAL_STRING("Seshanbe", persian_weekday_name(3));
}

void test_weekday_name_chaharshanbe(void)
{
    TEST_ASSERT_EQUAL_STRING("Chaharshanbe", persian_weekday_name(4));
}

void test_weekday_name_panjshanbe(void)
{
    TEST_ASSERT_EQUAL_STRING("Panjshanbe", persian_weekday_name(5));
}

void test_weekday_name_jome(void)
{
    TEST_ASSERT_EQUAL_STRING("Jom'e", persian_weekday_name(6));
}

void test_weekday_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", persian_weekday_name(7));
}

void test_weekday_name_invalid_negative(void)
{
    TEST_ASSERT_EQUAL_STRING("?", persian_weekday_name(-1));
}

/* ===== Leap year tests ===== */

void test_leap_year_1370(void)
{
    TEST_ASSERT_TRUE(persian_is_leap_year(1370));
}

void test_leap_year_1375(void)
{
    TEST_ASSERT_TRUE(persian_is_leap_year(1375));
}

void test_leap_year_1379(void)
{
    TEST_ASSERT_TRUE(persian_is_leap_year(1379));
}

void test_leap_year_1383(void)
{
    TEST_ASSERT_TRUE(persian_is_leap_year(1383));
}

void test_leap_year_1387(void)
{
    TEST_ASSERT_TRUE(persian_is_leap_year(1387));
}

void test_leap_year_1391(void)
{
    TEST_ASSERT_TRUE(persian_is_leap_year(1391));
}

void test_leap_year_1395(void)
{
    TEST_ASSERT_TRUE(persian_is_leap_year(1395));
}

void test_leap_year_1399(void)
{
    TEST_ASSERT_TRUE(persian_is_leap_year(1399));
}

void test_non_leap_1403(void)
{
    /* NOTE: The astronomical Persian calendar considers 1403 leap,
     * but the Reingold/Dershowitz 2820-year arithmetic algorithm does not.
     * We implement the arithmetic algorithm. */
    TEST_ASSERT_FALSE(persian_is_leap_year(1403));
}

void test_leap_year_1404(void)
{
    TEST_ASSERT_TRUE(persian_is_leap_year(1404));
}

void test_non_leap_1371(void)
{
    TEST_ASSERT_FALSE(persian_is_leap_year(1371));
}

void test_non_leap_1372(void)
{
    TEST_ASSERT_FALSE(persian_is_leap_year(1372));
}

void test_non_leap_1373(void)
{
    TEST_ASSERT_FALSE(persian_is_leap_year(1373));
}

void test_non_leap_1374(void)
{
    TEST_ASSERT_FALSE(persian_is_leap_year(1374));
}

void test_non_leap_1376(void)
{
    TEST_ASSERT_FALSE(persian_is_leap_year(1376));
}

void test_non_leap_1400(void)
{
    TEST_ASSERT_FALSE(persian_is_leap_year(1400));
}

void test_non_leap_1401(void)
{
    TEST_ASSERT_FALSE(persian_is_leap_year(1401));
}

void test_non_leap_1402(void)
{
    TEST_ASSERT_FALSE(persian_is_leap_year(1402));
}

/* ===== Month days tests ===== */

void test_month_days_first_six_31(void)
{
    /* Months 1-6 always have 31 days */
    for (int m = 1; m <= 6; m++) {
        TEST_ASSERT_EQUAL_INT(31, persian_month_days(1400, m));
    }
}

void test_month_days_next_five_30(void)
{
    /* Months 7-11 always have 30 days */
    for (int m = 7; m <= 11; m++) {
        TEST_ASSERT_EQUAL_INT(30, persian_month_days(1400, m));
    }
}

void test_month_days_esfand_non_leap(void)
{
    /* Month 12 has 29 days in a non-leap year */
    TEST_ASSERT_EQUAL_INT(29, persian_month_days(1400, 12));
}

void test_month_days_esfand_leap(void)
{
    /* Month 12 has 30 days in a leap year */
    TEST_ASSERT_EQUAL_INT(30, persian_month_days(1399, 12));
}

void test_month_days_invalid_zero(void)
{
    TEST_ASSERT_EQUAL_INT(0, persian_month_days(1400, 0));
}

void test_month_days_invalid_thirteen(void)
{
    TEST_ASSERT_EQUAL_INT(0, persian_month_days(1400, 13));
}

/* ===== Year days tests ===== */

void test_year_days_non_leap(void)
{
    TEST_ASSERT_EQUAL_INT(365, persian_year_days(1400));
}

void test_year_days_leap(void)
{
    TEST_ASSERT_EQUAL_INT(366, persian_year_days(1399));
}

/* ===== Persian to JD conversion tests ===== */

void test_epoch_to_jd(void)
{
    /* 1 Farvardin 1 SH = JD 1948320.5 => integer day 1948320 */
    persian_date_t d = { .year = 1, .month = 1, .day = 1 };
    int jdn = (int)floor(persian_to_jd(d));
    TEST_ASSERT_EQUAL_INT(1948320, jdn);
}

void test_nowruz_1400_to_jd_roundtrip(void)
{
    /* 1 Farvardin 1400 SH: verify round-trip */
    persian_date_t d = { .year = 1400, .month = 1, .day = 1 };
    double jd = persian_to_jd(d);
    persian_date_t back = persian_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(1400, back.year);
    TEST_ASSERT_EQUAL_INT(1, back.month);
    TEST_ASSERT_EQUAL_INT(1, back.day);
}

void test_last_day_1399_to_jd(void)
{
    /* 30 Esfand 1399 (leap year) should be exactly 1 day before
     * 1 Farvardin 1400. Check via integer JD difference. */
    persian_date_t last = { .year = 1399, .month = 12, .day = 30 };
    persian_date_t first = { .year = 1400, .month = 1, .day = 1 };
    int jdn_last = (int)floor(persian_to_jd(last));
    int jdn_first = (int)floor(persian_to_jd(first));
    TEST_ASSERT_EQUAL_INT(1, jdn_first - jdn_last);
}

void test_end_of_year_non_leap(void)
{
    /* 29 Esfand 1400 (non-leap) should be 1 day before 1 Farvardin 1401 */
    persian_date_t last = { .year = 1400, .month = 12, .day = 29 };
    persian_date_t first = { .year = 1401, .month = 1, .day = 1 };
    int jdn_last = (int)floor(persian_to_jd(last));
    int jdn_first = (int)floor(persian_to_jd(first));
    TEST_ASSERT_EQUAL_INT(1, jdn_first - jdn_last);
}

/* ===== JD to Persian conversion tests ===== */

void test_epoch_from_jd(void)
{
    /* JD 1948320.5 = 1 Farvardin 1 SH */
    persian_date_t d = persian_from_jd(1948320.5);
    TEST_ASSERT_EQUAL_INT(1, d.year);
    TEST_ASSERT_EQUAL_INT(1, d.month);
    TEST_ASSERT_EQUAL_INT(1, d.day);
}

void test_from_jd_known_modern_date(void)
{
    /* Cross-check: epoch + 365 days = 1 Farvardin 2 SH (year 1 is not leap) */
    persian_date_t d1 = { .year = 1, .month = 1, .day = 1 };
    double jd_epoch = persian_to_jd(d1);
    persian_date_t d2 = persian_from_jd(jd_epoch + 365.0);
    TEST_ASSERT_EQUAL_INT(2, d2.year);
    TEST_ASSERT_EQUAL_INT(1, d2.month);
    TEST_ASSERT_EQUAL_INT(1, d2.day);
}

/* ===== Round-trip conversion tests ===== */

void test_roundtrip_epoch(void)
{
    persian_date_t d = { .year = 1, .month = 1, .day = 1 };
    double jd = persian_to_jd(d);
    persian_date_t back = persian_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(1, back.year);
    TEST_ASSERT_EQUAL_INT(1, back.month);
    TEST_ASSERT_EQUAL_INT(1, back.day);
}

void test_roundtrip_modern_1400(void)
{
    persian_date_t d = { .year = 1400, .month = 7, .day = 15 };
    double jd = persian_to_jd(d);
    persian_date_t back = persian_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(1400, back.year);
    TEST_ASSERT_EQUAL_INT(7, back.month);
    TEST_ASSERT_EQUAL_INT(15, back.day);
}

void test_roundtrip_leap_esfand_30(void)
{
    /* 30 Esfand in a leap year (1399) */
    persian_date_t d = { .year = 1399, .month = 12, .day = 30 };
    double jd = persian_to_jd(d);
    persian_date_t back = persian_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(1399, back.year);
    TEST_ASSERT_EQUAL_INT(12, back.month);
    TEST_ASSERT_EQUAL_INT(30, back.day);
}

void test_roundtrip_non_leap_esfand_29(void)
{
    /* 29 Esfand in a non-leap year (1400) */
    persian_date_t d = { .year = 1400, .month = 12, .day = 29 };
    double jd = persian_to_jd(d);
    persian_date_t back = persian_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(1400, back.year);
    TEST_ASSERT_EQUAL_INT(12, back.month);
    TEST_ASSERT_EQUAL_INT(29, back.day);
}

void test_roundtrip_month_boundaries(void)
{
    /* Test last day of month 6 (last 31-day month) */
    persian_date_t d1 = { .year = 1403, .month = 6, .day = 31 };
    double jd1 = persian_to_jd(d1);
    persian_date_t back1 = persian_from_jd(jd1);
    TEST_ASSERT_EQUAL_INT(1403, back1.year);
    TEST_ASSERT_EQUAL_INT(6, back1.month);
    TEST_ASSERT_EQUAL_INT(31, back1.day);

    /* Month 7 (first 30-day month) day 1 */
    persian_date_t d2 = { .year = 1403, .month = 7, .day = 1 };
    double jd2 = persian_to_jd(d2);
    persian_date_t back2 = persian_from_jd(jd2);
    TEST_ASSERT_EQUAL_INT(1403, back2.year);
    TEST_ASSERT_EQUAL_INT(7, back2.month);
    TEST_ASSERT_EQUAL_INT(1, back2.day);

    /* day after last 31-day month = first of next (1 day apart) */
    TEST_ASSERT_EQUAL_INT(1, (int)floor(jd2) - (int)floor(jd1));
}

void test_roundtrip_all_months(void)
{
    /* Test day 1 of every month in year 1403 */
    for (int m = 1; m <= 12; m++) {
        persian_date_t d = { .year = 1403, .month = m, .day = 1 };
        double jd = persian_to_jd(d);
        persian_date_t back = persian_from_jd(jd);
        TEST_ASSERT_EQUAL_INT(1403, back.year);
        TEST_ASSERT_EQUAL_INT(m, back.month);
        TEST_ASSERT_EQUAL_INT(1, back.day);
    }
}

void test_roundtrip_year_1(void)
{
    /* Day 2 of epoch year */
    persian_date_t d = { .year = 1, .month = 1, .day = 2 };
    double jd = persian_to_jd(d);
    persian_date_t back = persian_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(1, back.year);
    TEST_ASSERT_EQUAL_INT(1, back.month);
    TEST_ASSERT_EQUAL_INT(2, back.day);
}

void test_roundtrip_year_474(void)
{
    /* Year 474 is the canonical reference year in the algorithm */
    persian_date_t d = { .year = 474, .month = 6, .day = 15 };
    double jd = persian_to_jd(d);
    persian_date_t back = persian_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(474, back.year);
    TEST_ASSERT_EQUAL_INT(6, back.month);
    TEST_ASSERT_EQUAL_INT(15, back.day);
}

void test_roundtrip_year_1404(void)
{
    /* SH 1404 = ~2025-2026 CE */
    persian_date_t d = { .year = 1404, .month = 1, .day = 1 };
    double jd = persian_to_jd(d);
    persian_date_t back = persian_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(1404, back.year);
    TEST_ASSERT_EQUAL_INT(1, back.month);
    TEST_ASSERT_EQUAL_INT(1, back.day);
}

void test_roundtrip_stress_many_years(void)
{
    /* Round-trip 1 Farvardin for years 1 to 1500 at intervals of 33 */
    for (int y = 1; y <= 1500; y += 33) {
        persian_date_t d = { .year = y, .month = 1, .day = 1 };
        double jd = persian_to_jd(d);
        persian_date_t back = persian_from_jd(jd);
        TEST_ASSERT_EQUAL_INT(y, back.year);
        TEST_ASSERT_EQUAL_INT(1, back.month);
        TEST_ASSERT_EQUAL_INT(1, back.day);
    }
}

void test_roundtrip_stress_mid_year(void)
{
    /* Round-trip 15 Mehr (month 7) for various years */
    int years[] = { 1, 100, 474, 900, 1200, 1399, 1400, 1403, 1404 };
    for (int i = 0; i < 9; i++) {
        persian_date_t d = { .year = years[i], .month = 7, .day = 15 };
        double jd = persian_to_jd(d);
        persian_date_t back = persian_from_jd(jd);
        TEST_ASSERT_EQUAL_INT(years[i], back.year);
        TEST_ASSERT_EQUAL_INT(7, back.month);
        TEST_ASSERT_EQUAL_INT(15, back.day);
    }
}

/* ===== Weekday tests ===== */

void test_weekday_from_jd(void)
{
    /* JD 2451545.5 = January 1, 2000 CE (Gregorian) = Saturday
     * Saturday = 0 in our mapping */
    int wd = persian_weekday(2451545.5);
    TEST_ASSERT_EQUAL_INT(0, wd);  /* Shanbe = Saturday */
}

void test_weekday_sunday(void)
{
    /* JD 2451546.5 = January 2, 2000 = Sunday = 1 */
    int wd = persian_weekday(2451546.5);
    TEST_ASSERT_EQUAL_INT(1, wd);
}

void test_weekday_friday(void)
{
    /* JD 2451551.5 = January 7, 2000 = Friday = 6 */
    int wd = persian_weekday(2451551.5);
    TEST_ASSERT_EQUAL_INT(6, wd);
}

void test_weekday_consecutive_week(void)
{
    /* Starting from a known Saturday (JD 2451545.5), check all 7 days */
    double jd_saturday = 2451545.5;
    for (int i = 0; i < 7; i++) {
        TEST_ASSERT_EQUAL_INT(i, persian_weekday(jd_saturday + i));
    }
}

/* ===== Festival detection tests ===== */

void test_festival_nowruz(void)
{
    persian_date_t d = { .year = 1403, .month = 1, .day = 1 };
    TEST_ASSERT_EQUAL_INT(PERSIAN_FEST_NOWRUZ, persian_festival(d));
}

void test_festival_sizdah_bedar(void)
{
    persian_date_t d = { .year = 1403, .month = 1, .day = 13 };
    TEST_ASSERT_EQUAL_INT(PERSIAN_FEST_SIZDAH_BEDAR, persian_festival(d));
}

void test_festival_yalda(void)
{
    persian_date_t d = { .year = 1403, .month = 9, .day = 30 };
    TEST_ASSERT_EQUAL_INT(PERSIAN_FEST_YALDA, persian_festival(d));
}

void test_festival_chaharshanbe_suri(void)
{
    /* Chaharshanbe Suri = last Wednesday of Esfand.
     * Scan all days of Esfand, find exactly one match on a Wednesday. */
    int found_count = 0;
    for (int day = 1; day <= 29; day++) {
        persian_date_t d = { .year = 1400, .month = 12, .day = day };
        if (persian_festival(d) == PERSIAN_FEST_CHAHARSHANBE_SURI) {
            found_count++;
            /* The detected day must be a Wednesday (4) */
            double jd = persian_to_jd(d);
            TEST_ASSERT_EQUAL_INT(4, persian_weekday(jd));
            /* It must be in the last 7 days of the month */
            TEST_ASSERT_TRUE(day > 22);
        }
    }
    /* There should be exactly one last Wednesday */
    TEST_ASSERT_EQUAL_INT(1, found_count);
}

void test_festival_none(void)
{
    persian_date_t d = { .year = 1403, .month = 5, .day = 15 };
    TEST_ASSERT_EQUAL_INT(PERSIAN_FEST_NONE, persian_festival(d));
}

void test_festival_chaharshanbe_suri_leap(void)
{
    /* In leap year 1399, Esfand has 30 days */
    int found_count = 0;
    for (int day = 1; day <= 30; day++) {
        persian_date_t d = { .year = 1399, .month = 12, .day = day };
        if (persian_festival(d) == PERSIAN_FEST_CHAHARSHANBE_SURI) {
            found_count++;
            double jd = persian_to_jd(d);
            TEST_ASSERT_EQUAL_INT(4, persian_weekday(jd));
            TEST_ASSERT_TRUE(day > 23);
        }
    }
    TEST_ASSERT_EQUAL_INT(1, found_count);
}

/* ===== Festival name tests ===== */

void test_festival_name_none(void)
{
    TEST_ASSERT_EQUAL_STRING("", persian_festival_name(PERSIAN_FEST_NONE));
}

void test_festival_name_nowruz(void)
{
    TEST_ASSERT_EQUAL_STRING("Nowruz", persian_festival_name(PERSIAN_FEST_NOWRUZ));
}

void test_festival_name_sizdah_bedar(void)
{
    TEST_ASSERT_EQUAL_STRING("Sizdah Bedar",
                             persian_festival_name(PERSIAN_FEST_SIZDAH_BEDAR));
}

void test_festival_name_yalda(void)
{
    TEST_ASSERT_EQUAL_STRING("Yalda", persian_festival_name(PERSIAN_FEST_YALDA));
}

void test_festival_name_chaharshanbe_suri(void)
{
    TEST_ASSERT_EQUAL_STRING("Chaharshanbe Suri",
                             persian_festival_name(PERSIAN_FEST_CHAHARSHANBE_SURI));
}

void test_festival_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", persian_festival_name(PERSIAN_FEST_COUNT));
}

void test_festival_name_invalid_negative(void)
{
    TEST_ASSERT_EQUAL_STRING("?", persian_festival_name(-1));
}

/* ===== CE/SH year approximation tests ===== */

void test_ce_year_approx_1400(void)
{
    /* SH 1400 ~ 2021-2022 CE */
    int ce = persian_ce_year_approx(1400);
    TEST_ASSERT_TRUE(ce >= 2021 && ce <= 2022);
}

void test_ce_year_approx_1404(void)
{
    /* SH 1404 ~ 2025-2026 CE */
    int ce = persian_ce_year_approx(1404);
    TEST_ASSERT_TRUE(ce >= 2025 && ce <= 2026);
}

void test_ce_year_approx_1(void)
{
    /* SH 1 ~ 622 CE */
    int ce = persian_ce_year_approx(1);
    TEST_ASSERT_TRUE(ce >= 621 && ce <= 623);
}

void test_sh_year_approx_2025(void)
{
    /* CE 2025 ~ SH 1403-1404 */
    int sh = persian_sh_year_approx(2025);
    TEST_ASSERT_TRUE(sh >= 1403 && sh <= 1405);
}

void test_sh_year_approx_622(void)
{
    /* CE 622 ~ SH 1 */
    int sh = persian_sh_year_approx(622);
    TEST_ASSERT_TRUE(sh >= 0 && sh <= 2);
}

void test_sh_year_approx_2000(void)
{
    /* CE 2000 ~ SH 1378-1379 */
    int sh = persian_sh_year_approx(2000);
    TEST_ASSERT_TRUE(sh >= 1378 && sh <= 1380);
}

/* ===== Year length consistency tests ===== */

void test_year_length_via_jd(void)
{
    /* For year 1400 (non-leap), the distance between
     * 1 Farvardin 1400 and 1 Farvardin 1401 should be 365 days */
    persian_date_t d1 = { .year = 1400, .month = 1, .day = 1 };
    persian_date_t d2 = { .year = 1401, .month = 1, .day = 1 };
    int diff = (int)floor(persian_to_jd(d2)) - (int)floor(persian_to_jd(d1));
    TEST_ASSERT_EQUAL_INT(365, diff);
}

void test_leap_year_length_via_jd(void)
{
    /* For year 1399 (leap), the distance should be 366 days */
    persian_date_t d1 = { .year = 1399, .month = 1, .day = 1 };
    persian_date_t d2 = { .year = 1400, .month = 1, .day = 1 };
    int diff = (int)floor(persian_to_jd(d2)) - (int)floor(persian_to_jd(d1));
    TEST_ASSERT_EQUAL_INT(366, diff);
}

void test_year_length_via_jd_1403_non_leap(void)
{
    /* 1403 is not a leap year in the arithmetic algorithm */
    persian_date_t d1 = { .year = 1403, .month = 1, .day = 1 };
    persian_date_t d2 = { .year = 1404, .month = 1, .day = 1 };
    int diff = (int)floor(persian_to_jd(d2)) - (int)floor(persian_to_jd(d1));
    TEST_ASSERT_EQUAL_INT(365, diff);
}

void test_year_length_via_jd_1404_leap(void)
{
    /* 1404 is a leap year */
    persian_date_t d1 = { .year = 1404, .month = 1, .day = 1 };
    persian_date_t d2 = { .year = 1405, .month = 1, .day = 1 };
    int diff = (int)floor(persian_to_jd(d2)) - (int)floor(persian_to_jd(d1));
    TEST_ASSERT_EQUAL_INT(366, diff);
}

/* ===== 2820-year grand cycle tests ===== */

void test_grand_cycle_length(void)
{
    /* A 2820-year cycle should contain exactly 1029983 days.
     * Check via JD: year Y and Y+2820 should differ by 1029983 days. */
    persian_date_t d1 = { .year = 474, .month = 1, .day = 1 };
    persian_date_t d2 = { .year = 474 + 2820, .month = 1, .day = 1 };
    int diff = (int)floor(persian_to_jd(d2)) - (int)floor(persian_to_jd(d1));
    TEST_ASSERT_EQUAL_INT(1029983, diff);
}

/* ===== Edge case tests ===== */

void test_consecutive_days_across_month_boundary(void)
{
    /* 31 Shahrivar (month 6, last 31-day month) -> 1 Mehr (month 7) */
    persian_date_t d1 = { .year = 1400, .month = 6, .day = 31 };
    persian_date_t d2 = { .year = 1400, .month = 7, .day = 1 };
    int diff = (int)floor(persian_to_jd(d2)) - (int)floor(persian_to_jd(d1));
    TEST_ASSERT_EQUAL_INT(1, diff);
}

void test_consecutive_days_across_year_boundary(void)
{
    /* 29 Esfand 1401 (non-leap) -> 1 Farvardin 1402 */
    persian_date_t d1 = { .year = 1401, .month = 12, .day = 29 };
    persian_date_t d2 = { .year = 1402, .month = 1, .day = 1 };
    int diff = (int)floor(persian_to_jd(d2)) - (int)floor(persian_to_jd(d1));
    TEST_ASSERT_EQUAL_INT(1, diff);
}

void test_first_month_has_31_days(void)
{
    /* Verify 31 Farvardin exists and 32 would be next month */
    persian_date_t d31 = { .year = 1400, .month = 1, .day = 31 };
    persian_date_t d_next = { .year = 1400, .month = 2, .day = 1 };
    int diff = (int)floor(persian_to_jd(d_next)) - (int)floor(persian_to_jd(d31));
    TEST_ASSERT_EQUAL_INT(1, diff);
}

/* ===== Main ===== */

int main(void)
{
    UNITY_BEGIN();

    /* Month names (15) */
    RUN_TEST(test_month_name_farvardin);
    RUN_TEST(test_month_name_ordibehesht);
    RUN_TEST(test_month_name_khordad);
    RUN_TEST(test_month_name_tir);
    RUN_TEST(test_month_name_mordad);
    RUN_TEST(test_month_name_shahrivar);
    RUN_TEST(test_month_name_mehr);
    RUN_TEST(test_month_name_aban);
    RUN_TEST(test_month_name_azar);
    RUN_TEST(test_month_name_dey);
    RUN_TEST(test_month_name_bahman);
    RUN_TEST(test_month_name_esfand);
    RUN_TEST(test_month_name_invalid_zero);
    RUN_TEST(test_month_name_invalid_thirteen);
    RUN_TEST(test_month_name_invalid_negative);

    /* Weekday names (9) */
    RUN_TEST(test_weekday_name_shanbe);
    RUN_TEST(test_weekday_name_yekshanbe);
    RUN_TEST(test_weekday_name_doshanbe);
    RUN_TEST(test_weekday_name_seshanbe);
    RUN_TEST(test_weekday_name_chaharshanbe);
    RUN_TEST(test_weekday_name_panjshanbe);
    RUN_TEST(test_weekday_name_jome);
    RUN_TEST(test_weekday_name_invalid);
    RUN_TEST(test_weekday_name_invalid_negative);

    /* Leap years (18) */
    RUN_TEST(test_leap_year_1370);
    RUN_TEST(test_leap_year_1375);
    RUN_TEST(test_leap_year_1379);
    RUN_TEST(test_leap_year_1383);
    RUN_TEST(test_leap_year_1387);
    RUN_TEST(test_leap_year_1391);
    RUN_TEST(test_leap_year_1395);
    RUN_TEST(test_leap_year_1399);
    RUN_TEST(test_non_leap_1403);
    RUN_TEST(test_leap_year_1404);
    RUN_TEST(test_non_leap_1371);
    RUN_TEST(test_non_leap_1372);
    RUN_TEST(test_non_leap_1373);
    RUN_TEST(test_non_leap_1374);
    RUN_TEST(test_non_leap_1376);
    RUN_TEST(test_non_leap_1400);
    RUN_TEST(test_non_leap_1401);
    RUN_TEST(test_non_leap_1402);

    /* Month/year days (8) */
    RUN_TEST(test_month_days_first_six_31);
    RUN_TEST(test_month_days_next_five_30);
    RUN_TEST(test_month_days_esfand_non_leap);
    RUN_TEST(test_month_days_esfand_leap);
    RUN_TEST(test_month_days_invalid_zero);
    RUN_TEST(test_month_days_invalid_thirteen);
    RUN_TEST(test_year_days_non_leap);
    RUN_TEST(test_year_days_leap);

    /* Persian -> JD (4) */
    RUN_TEST(test_epoch_to_jd);
    RUN_TEST(test_nowruz_1400_to_jd_roundtrip);
    RUN_TEST(test_last_day_1399_to_jd);
    RUN_TEST(test_end_of_year_non_leap);

    /* JD -> Persian (2) */
    RUN_TEST(test_epoch_from_jd);
    RUN_TEST(test_from_jd_known_modern_date);

    /* Round-trip (11) */
    RUN_TEST(test_roundtrip_epoch);
    RUN_TEST(test_roundtrip_modern_1400);
    RUN_TEST(test_roundtrip_leap_esfand_30);
    RUN_TEST(test_roundtrip_non_leap_esfand_29);
    RUN_TEST(test_roundtrip_month_boundaries);
    RUN_TEST(test_roundtrip_all_months);
    RUN_TEST(test_roundtrip_year_1);
    RUN_TEST(test_roundtrip_year_474);
    RUN_TEST(test_roundtrip_year_1404);
    RUN_TEST(test_roundtrip_stress_many_years);
    RUN_TEST(test_roundtrip_stress_mid_year);

    /* Weekday (4) */
    RUN_TEST(test_weekday_from_jd);
    RUN_TEST(test_weekday_sunday);
    RUN_TEST(test_weekday_friday);
    RUN_TEST(test_weekday_consecutive_week);

    /* Festivals (9) */
    RUN_TEST(test_festival_nowruz);
    RUN_TEST(test_festival_sizdah_bedar);
    RUN_TEST(test_festival_yalda);
    RUN_TEST(test_festival_chaharshanbe_suri);
    RUN_TEST(test_festival_none);
    RUN_TEST(test_festival_chaharshanbe_suri_leap);
    RUN_TEST(test_festival_name_none);
    RUN_TEST(test_festival_name_nowruz);
    RUN_TEST(test_festival_name_sizdah_bedar);
    RUN_TEST(test_festival_name_yalda);
    RUN_TEST(test_festival_name_chaharshanbe_suri);
    RUN_TEST(test_festival_name_invalid);
    RUN_TEST(test_festival_name_invalid_negative);

    /* CE/SH approximation (6) */
    RUN_TEST(test_ce_year_approx_1400);
    RUN_TEST(test_ce_year_approx_1404);
    RUN_TEST(test_ce_year_approx_1);
    RUN_TEST(test_sh_year_approx_2025);
    RUN_TEST(test_sh_year_approx_622);
    RUN_TEST(test_sh_year_approx_2000);

    /* Year length via JD (3) */
    RUN_TEST(test_year_length_via_jd);
    RUN_TEST(test_leap_year_length_via_jd);
    RUN_TEST(test_year_length_via_jd_1403_non_leap);
    RUN_TEST(test_year_length_via_jd_1404_leap);

    /* Grand cycle (1) */
    RUN_TEST(test_grand_cycle_length);

    /* Edge cases (3) */
    RUN_TEST(test_consecutive_days_across_month_boundary);
    RUN_TEST(test_consecutive_days_across_year_boundary);
    RUN_TEST(test_first_month_has_31_days);

    return UNITY_END();
}
