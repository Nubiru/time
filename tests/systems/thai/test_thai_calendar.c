/* test_thai_calendar.c -- Thai Solar Calendar module tests
 * TDD: tests written before implementation.
 * Covers: BE<->CE conversion, JD conversion, month names, abbreviations,
 *         Songkran detection, festival detection, pre-reform check,
 *         month days, leap year, roundtrip JD, edge cases. */

#include "../../unity/unity.h"
#include "../../../src/systems/thai/thai_calendar.h"
#include <math.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== BE <-> CE year conversion ===== */

void test_be_year_2024_ce(void)
{
    TEST_ASSERT_EQUAL_INT(2567, thai_be_year(2024));
}

void test_be_year_2000_ce(void)
{
    TEST_ASSERT_EQUAL_INT(2543, thai_be_year(2000));
}

void test_be_year_1_ce(void)
{
    TEST_ASSERT_EQUAL_INT(544, thai_be_year(1));
}

void test_be_year_1941_ce(void)
{
    /* Reform year */
    TEST_ASSERT_EQUAL_INT(2484, thai_be_year(1941));
}

void test_ce_year_2567_be(void)
{
    TEST_ASSERT_EQUAL_INT(2024, thai_ce_year(2567));
}

void test_ce_year_2543_be(void)
{
    TEST_ASSERT_EQUAL_INT(2000, thai_ce_year(2543));
}

void test_ce_year_544_be(void)
{
    TEST_ASSERT_EQUAL_INT(1, thai_ce_year(544));
}

void test_be_ce_roundtrip(void)
{
    /* BE -> CE -> BE should be identity */
    int be = 2567;
    TEST_ASSERT_EQUAL_INT(be, thai_be_year(thai_ce_year(be)));
}

/* ===== JD conversion ===== */

void test_from_jd_j2000(void)
{
    /* J2000.0 = JD 2451545.0 = 1 Jan 2000 12:00 UT = BE 2543 */
    thai_date_t d = thai_from_jd(2451545.0);
    TEST_ASSERT_EQUAL_INT(2543, d.be_year);
    TEST_ASSERT_EQUAL_INT(1, d.month);
    TEST_ASSERT_EQUAL_INT(1, d.day);
}

void test_from_jd_known_date(void)
{
    /* 15 April 2024 = JD 2460415.5 (midnight UT)
     * gregorian_to_jd(2024, 4, 15.0) = 2460415.5 */
    double jd = 2460415.5;
    thai_date_t d = thai_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(2567, d.be_year);
    TEST_ASSERT_EQUAL_INT(4, d.month);
    TEST_ASSERT_EQUAL_INT(15, d.day);
}

void test_from_jd_dec_31(void)
{
    /* 31 Dec 1999 midnight UT = JD 2451543.5
     * (J2000.0 = JD 2451545.0 = 1 Jan 2000 noon, so midnight Jan 1 = 2451544.5,
     *  midnight Dec 31 = 2451543.5) */
    double jd = 2451543.5;
    thai_date_t d = thai_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(2542, d.be_year);
    TEST_ASSERT_EQUAL_INT(12, d.month);
    TEST_ASSERT_EQUAL_INT(31, d.day);
}

void test_to_jd_jan_1_2000(void)
{
    /* 1 Jan 2000 = BE 2543, month 1, day 1 -> JD should be 2451544.5 (midnight) */
    thai_date_t d = { .be_year = 2543, .month = 1, .day = 1 };
    double jd = thai_to_jd(d);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, (float)2451544.5, (float)jd);
}

void test_to_jd_songkran(void)
{
    /* 13 April 2024 (BE 2567) */
    thai_date_t d = { .be_year = 2567, .month = 4, .day = 13 };
    double jd = thai_to_jd(d);
    /* Verify by converting back */
    thai_date_t back = thai_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(2567, back.be_year);
    TEST_ASSERT_EQUAL_INT(4, back.month);
    TEST_ASSERT_EQUAL_INT(13, back.day);
}

void test_jd_roundtrip_various_dates(void)
{
    /* Test multiple roundtrips: to_jd -> from_jd */
    thai_date_t dates[] = {
        { .be_year = 2543, .month = 1, .day = 1 },   /* 2000 CE */
        { .be_year = 2567, .month = 7, .day = 28 },   /* Jul 28 2024 */
        { .be_year = 2484, .month = 12, .day = 31 },  /* Dec 31 1941 */
        { .be_year = 2500, .month = 6, .day = 15 },   /* Jun 15 1957 */
        { .be_year = 2544, .month = 2, .day = 29 },   /* Feb 29 2001? No, 2001 not leap */
    };
    /* Skip the last one if invalid -- test valid dates */
    for (int i = 0; i < 4; i++) {
        double jd = thai_to_jd(dates[i]);
        thai_date_t back = thai_from_jd(jd);
        TEST_ASSERT_EQUAL_INT(dates[i].be_year, back.be_year);
        TEST_ASSERT_EQUAL_INT(dates[i].month, back.month);
        TEST_ASSERT_EQUAL_INT(dates[i].day, back.day);
    }
}

/* ===== Month names ===== */

void test_month_name_january(void)
{
    TEST_ASSERT_EQUAL_STRING("Mokarakhom", thai_month_name(1));
}

void test_month_name_april(void)
{
    TEST_ASSERT_EQUAL_STRING("Mesayon", thai_month_name(4));
}

void test_month_name_december(void)
{
    TEST_ASSERT_EQUAL_STRING("Thanwakhom", thai_month_name(12));
}

void test_month_name_all_twelve(void)
{
    const char *expected[] = {
        "Mokarakhom", "Kumphaphan", "Minakhom", "Mesayon",
        "Phruetsaphakhom", "Mithunayon", "Karakadakhom", "Singhakhom",
        "Kanyayon", "Tulakhom", "Phruetsachikayon", "Thanwakhom"
    };
    for (int m = 1; m <= 12; m++) {
        TEST_ASSERT_EQUAL_STRING(expected[m - 1], thai_month_name(m));
    }
}

void test_month_name_invalid_zero(void)
{
    TEST_ASSERT_EQUAL_STRING("?", thai_month_name(0));
}

void test_month_name_invalid_13(void)
{
    TEST_ASSERT_EQUAL_STRING("?", thai_month_name(13));
}

void test_month_name_invalid_negative(void)
{
    TEST_ASSERT_EQUAL_STRING("?", thai_month_name(-1));
}

/* ===== Month abbreviations ===== */

void test_month_abbr_january(void)
{
    TEST_ASSERT_EQUAL_STRING("Mok", thai_month_abbr(1));
}

void test_month_abbr_april(void)
{
    TEST_ASSERT_EQUAL_STRING("Mes", thai_month_abbr(4));
}

void test_month_abbr_december(void)
{
    TEST_ASSERT_EQUAL_STRING("Thw", thai_month_abbr(12));
}

void test_month_abbr_all_twelve(void)
{
    const char *expected[] = {
        "Mok", "Kum", "Min", "Mes",
        "Phr", "Mit", "Kar", "Sin",
        "Kan", "Tul", "Pch", "Thw"
    };
    for (int m = 1; m <= 12; m++) {
        TEST_ASSERT_EQUAL_STRING(expected[m - 1], thai_month_abbr(m));
    }
}

void test_month_abbr_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", thai_month_abbr(0));
    TEST_ASSERT_EQUAL_STRING("?", thai_month_abbr(13));
}

/* ===== Songkran detection ===== */

void test_songkran_april_13(void)
{
    thai_date_t d = { .be_year = 2567, .month = 4, .day = 13 };
    TEST_ASSERT_TRUE(thai_is_songkran(d));
}

void test_songkran_april_14(void)
{
    thai_date_t d = { .be_year = 2567, .month = 4, .day = 14 };
    TEST_ASSERT_TRUE(thai_is_songkran(d));
}

void test_songkran_april_15(void)
{
    thai_date_t d = { .be_year = 2567, .month = 4, .day = 15 };
    TEST_ASSERT_TRUE(thai_is_songkran(d));
}

void test_not_songkran_april_12(void)
{
    thai_date_t d = { .be_year = 2567, .month = 4, .day = 12 };
    TEST_ASSERT_FALSE(thai_is_songkran(d));
}

void test_not_songkran_april_16(void)
{
    thai_date_t d = { .be_year = 2567, .month = 4, .day = 16 };
    TEST_ASSERT_FALSE(thai_is_songkran(d));
}

void test_not_songkran_other_month(void)
{
    thai_date_t d = { .be_year = 2567, .month = 1, .day = 13 };
    TEST_ASSERT_FALSE(thai_is_songkran(d));
}

/* ===== Festival detection ===== */

void test_festival_songkran(void)
{
    thai_date_t d = { .be_year = 2567, .month = 4, .day = 13 };
    TEST_ASSERT_EQUAL_INT(THAI_FEST_SONGKRAN, thai_festival(d));
}

void test_festival_chakri_day(void)
{
    thai_date_t d = { .be_year = 2567, .month = 4, .day = 6 };
    TEST_ASSERT_EQUAL_INT(THAI_FEST_CHAKRI_DAY, thai_festival(d));
}

void test_festival_constitution_day(void)
{
    thai_date_t d = { .be_year = 2567, .month = 12, .day = 10 };
    TEST_ASSERT_EQUAL_INT(THAI_FEST_CONSTITUTION, thai_festival(d));
}

void test_festival_none(void)
{
    thai_date_t d = { .be_year = 2567, .month = 3, .day = 15 };
    TEST_ASSERT_EQUAL_INT(THAI_FEST_NONE, thai_festival(d));
}

/* ===== Festival names ===== */

void test_festival_name_songkran(void)
{
    TEST_ASSERT_EQUAL_STRING("Songkran", thai_festival_name(THAI_FEST_SONGKRAN));
}

void test_festival_name_chakri(void)
{
    TEST_ASSERT_EQUAL_STRING("Chakri Memorial Day", thai_festival_name(THAI_FEST_CHAKRI_DAY));
}

void test_festival_name_constitution(void)
{
    TEST_ASSERT_EQUAL_STRING("Constitution Day", thai_festival_name(THAI_FEST_CONSTITUTION));
}

void test_festival_name_none(void)
{
    TEST_ASSERT_EQUAL_STRING("", thai_festival_name(THAI_FEST_NONE));
}

void test_festival_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", thai_festival_name(THAI_FEST_COUNT));
}

/* ===== Pre-reform detection ===== */

void test_pre_reform_1940(void)
{
    TEST_ASSERT_TRUE(thai_is_pre_reform(1940));
}

void test_pre_reform_1941(void)
{
    /* 1941 is the reform year -- new year moved to Jan 1 */
    TEST_ASSERT_FALSE(thai_is_pre_reform(1941));
}

void test_pre_reform_1942(void)
{
    TEST_ASSERT_FALSE(thai_is_pre_reform(1942));
}

void test_pre_reform_1800(void)
{
    TEST_ASSERT_TRUE(thai_is_pre_reform(1800));
}

void test_pre_reform_2024(void)
{
    TEST_ASSERT_FALSE(thai_is_pre_reform(2024));
}

/* ===== Month days ===== */

void test_month_days_january(void)
{
    /* BE 2567 = 2024 CE */
    TEST_ASSERT_EQUAL_INT(31, thai_month_days(2567, 1));
}

void test_month_days_february_leap(void)
{
    /* BE 2543 = 2000 CE (leap) */
    TEST_ASSERT_EQUAL_INT(29, thai_month_days(2543, 2));
}

void test_month_days_february_common(void)
{
    /* BE 2544 = 2001 CE (not leap) */
    TEST_ASSERT_EQUAL_INT(28, thai_month_days(2544, 2));
}

void test_month_days_april(void)
{
    TEST_ASSERT_EQUAL_INT(30, thai_month_days(2567, 4));
}

void test_month_days_all(void)
{
    /* 2024 CE = BE 2567 (leap year) */
    int expected[] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    for (int m = 1; m <= 12; m++) {
        TEST_ASSERT_EQUAL_INT(expected[m - 1], thai_month_days(2567, m));
    }
}

/* ===== Leap year ===== */

void test_leap_year_2543_be(void)
{
    /* 2000 CE = leap */
    TEST_ASSERT_TRUE(thai_is_leap_year(2543));
}

void test_leap_year_2544_be(void)
{
    /* 2001 CE = not leap */
    TEST_ASSERT_FALSE(thai_is_leap_year(2544));
}

void test_leap_year_2443_be(void)
{
    /* 1900 CE = not leap (divisible by 100 but not 400) */
    TEST_ASSERT_FALSE(thai_is_leap_year(2443));
}

void test_leap_year_2567_be(void)
{
    /* 2024 CE = leap */
    TEST_ASSERT_TRUE(thai_is_leap_year(2567));
}

void test_leap_year_century_400(void)
{
    /* BE 2543 = 2000 CE, divisible by 400 = leap */
    TEST_ASSERT_TRUE(thai_is_leap_year(2543));
}

/* ===== from_jd noon vs midnight edge ===== */

void test_from_jd_noon_gives_same_day(void)
{
    /* JD 2451545.0 = 1 Jan 2000 12:00 noon UT */
    thai_date_t d = thai_from_jd(2451545.0);
    TEST_ASSERT_EQUAL_INT(1, d.day);
    TEST_ASSERT_EQUAL_INT(1, d.month);
}

void test_from_jd_midnight(void)
{
    /* JD 2451544.5 = 1 Jan 2000 midnight UT (half day before J2000.0 noon) */
    thai_date_t d = thai_from_jd(2451544.5);
    TEST_ASSERT_EQUAL_INT(1, d.day);
    TEST_ASSERT_EQUAL_INT(1, d.month);
    TEST_ASSERT_EQUAL_INT(2543, d.be_year);
}

/* ===== main ===== */

int main(void)
{
    UNITY_BEGIN();

    /* BE <-> CE */
    RUN_TEST(test_be_year_2024_ce);
    RUN_TEST(test_be_year_2000_ce);
    RUN_TEST(test_be_year_1_ce);
    RUN_TEST(test_be_year_1941_ce);
    RUN_TEST(test_ce_year_2567_be);
    RUN_TEST(test_ce_year_2543_be);
    RUN_TEST(test_ce_year_544_be);
    RUN_TEST(test_be_ce_roundtrip);

    /* JD conversion */
    RUN_TEST(test_from_jd_j2000);
    RUN_TEST(test_from_jd_known_date);
    RUN_TEST(test_from_jd_dec_31);
    RUN_TEST(test_to_jd_jan_1_2000);
    RUN_TEST(test_to_jd_songkran);
    RUN_TEST(test_jd_roundtrip_various_dates);

    /* Month names */
    RUN_TEST(test_month_name_january);
    RUN_TEST(test_month_name_april);
    RUN_TEST(test_month_name_december);
    RUN_TEST(test_month_name_all_twelve);
    RUN_TEST(test_month_name_invalid_zero);
    RUN_TEST(test_month_name_invalid_13);
    RUN_TEST(test_month_name_invalid_negative);

    /* Month abbreviations */
    RUN_TEST(test_month_abbr_january);
    RUN_TEST(test_month_abbr_april);
    RUN_TEST(test_month_abbr_december);
    RUN_TEST(test_month_abbr_all_twelve);
    RUN_TEST(test_month_abbr_invalid);

    /* Songkran */
    RUN_TEST(test_songkran_april_13);
    RUN_TEST(test_songkran_april_14);
    RUN_TEST(test_songkran_april_15);
    RUN_TEST(test_not_songkran_april_12);
    RUN_TEST(test_not_songkran_april_16);
    RUN_TEST(test_not_songkran_other_month);

    /* Festivals */
    RUN_TEST(test_festival_songkran);
    RUN_TEST(test_festival_chakri_day);
    RUN_TEST(test_festival_constitution_day);
    RUN_TEST(test_festival_none);
    RUN_TEST(test_festival_name_songkran);
    RUN_TEST(test_festival_name_chakri);
    RUN_TEST(test_festival_name_constitution);
    RUN_TEST(test_festival_name_none);
    RUN_TEST(test_festival_name_invalid);

    /* Pre-reform */
    RUN_TEST(test_pre_reform_1940);
    RUN_TEST(test_pre_reform_1941);
    RUN_TEST(test_pre_reform_1942);
    RUN_TEST(test_pre_reform_1800);
    RUN_TEST(test_pre_reform_2024);

    /* Month days */
    RUN_TEST(test_month_days_january);
    RUN_TEST(test_month_days_february_leap);
    RUN_TEST(test_month_days_february_common);
    RUN_TEST(test_month_days_april);
    RUN_TEST(test_month_days_all);

    /* Leap year */
    RUN_TEST(test_leap_year_2543_be);
    RUN_TEST(test_leap_year_2544_be);
    RUN_TEST(test_leap_year_2443_be);
    RUN_TEST(test_leap_year_2567_be);
    RUN_TEST(test_leap_year_century_400);

    /* Edge cases */
    RUN_TEST(test_from_jd_noon_gives_same_day);
    RUN_TEST(test_from_jd_midnight);

    return UNITY_END();
}
