/* test_lao_calendar.c -- Lao Buddhist Calendar module tests
 * TDD: tests written before implementation.
 * Covers: BE<->CE conversion, JD conversion, month names, abbreviations,
 *         Pi Mai detection, festival detection, month days, leap year,
 *         day of week, Nang Sangkhan, zodiac, roundtrip, edge cases. */

#include "../../unity/unity.h"
#include "../../../src/systems/lao/lao_calendar.h"
#include <math.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== BE <-> CE year conversion ===== */

void test_be_year_2024_ce(void)
{
    TEST_ASSERT_EQUAL_INT(2567, lao_be_year(2024));
}

void test_be_year_2026_ce(void)
{
    TEST_ASSERT_EQUAL_INT(2569, lao_be_year(2026));
}

void test_be_year_1_ce(void)
{
    TEST_ASSERT_EQUAL_INT(544, lao_be_year(1));
}

void test_ce_year_2569_be(void)
{
    TEST_ASSERT_EQUAL_INT(2026, lao_ce_year(2569));
}

void test_ce_year_544_be(void)
{
    TEST_ASSERT_EQUAL_INT(1, lao_ce_year(544));
}

void test_be_ce_roundtrip(void)
{
    int be = 2569;
    TEST_ASSERT_EQUAL_INT(be, lao_be_year(lao_ce_year(be)));
}

/* ===== JD conversion ===== */

void test_from_jd_j2000(void)
{
    /* JD 2451545.0 = 1 Jan 2000 12:00 noon UT */
    lao_date_t d = lao_from_jd(2451545.0);
    TEST_ASSERT_EQUAL_INT(2543, d.be_year);
    TEST_ASSERT_EQUAL_INT(1, d.month);
    TEST_ASSERT_EQUAL_INT(1, d.day);
}

void test_from_jd_known_date(void)
{
    /* JD 2460414.5 = 14 April 2024, BE 2567 (Pi Mai day 1) */
    lao_date_t d = lao_from_jd(2460414.5);
    TEST_ASSERT_EQUAL_INT(2567, d.be_year);
    TEST_ASSERT_EQUAL_INT(4, d.month);
    TEST_ASSERT_EQUAL_INT(14, d.day);
}

void test_to_jd_jan_1_2000(void)
{
    lao_date_t d = { .be_year = 2543, .month = 1, .day = 1 };
    double jd = lao_to_jd(d);
    TEST_ASSERT_FLOAT_WITHIN(0.5, 2451544.5, jd);
}

void test_to_jd_pi_mai(void)
{
    /* April 14, 2026 CE = BE 2569 */
    lao_date_t d = { .be_year = 2569, .month = 4, .day = 14 };
    double jd = lao_to_jd(d);
    lao_date_t d2 = lao_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(4, d2.month);
    TEST_ASSERT_EQUAL_INT(14, d2.day);
}

void test_jd_roundtrip_various(void)
{
    lao_date_t dates[] = {
        { 2543, 1, 1 },   /* 2000-01-01 */
        { 2569, 4, 14 },  /* Pi Mai 2026 */
        { 2569, 12, 2 },  /* National Day 2026 */
        { 2567, 2, 29 },  /* Leap day 2024 */
        { 2569, 6, 15 },  /* Random mid-year */
    };
    int count = (int)(sizeof(dates) / sizeof(dates[0]));
    for (int i = 0; i < count; i++) {
        double jd = lao_to_jd(dates[i]);
        lao_date_t r = lao_from_jd(jd);
        TEST_ASSERT_EQUAL_INT(dates[i].be_year, r.be_year);
        TEST_ASSERT_EQUAL_INT(dates[i].month, r.month);
        TEST_ASSERT_EQUAL_INT(dates[i].day, r.day);
    }
}

/* ===== Month names ===== */

void test_month_name_january(void)
{
    TEST_ASSERT_EQUAL_STRING("Mangkon", lao_month_name(1));
}

void test_month_name_april(void)
{
    TEST_ASSERT_EQUAL_STRING("Meesaa", lao_month_name(4));
}

void test_month_name_december(void)
{
    TEST_ASSERT_EQUAL_STRING("Thanwaa", lao_month_name(12));
}

void test_month_name_all_twelve(void)
{
    const char *expected[] = {
        "Mangkon", "Kumphaa", "Miinaa", "Meesaa",
        "Pheutsaphaa", "Mithunaa", "Kawlakot", "Singhaa",
        "Kanyaa", "Tulaa", "Phachik", "Thanwaa"
    };
    for (int m = 1; m <= 12; m++) {
        TEST_ASSERT_EQUAL_STRING(expected[m - 1], lao_month_name(m));
    }
}

void test_month_name_invalid_zero(void)
{
    TEST_ASSERT_EQUAL_STRING("?", lao_month_name(0));
}

void test_month_name_invalid_13(void)
{
    TEST_ASSERT_EQUAL_STRING("?", lao_month_name(13));
}

void test_month_name_invalid_negative(void)
{
    TEST_ASSERT_EQUAL_STRING("?", lao_month_name(-1));
}

/* ===== Month abbreviations ===== */

void test_month_abbr_january(void)
{
    TEST_ASSERT_EQUAL_STRING("Man", lao_month_abbr(1));
}

void test_month_abbr_april(void)
{
    TEST_ASSERT_EQUAL_STRING("Mee", lao_month_abbr(4));
}

void test_month_abbr_december(void)
{
    TEST_ASSERT_EQUAL_STRING("Thw", lao_month_abbr(12));
}

void test_month_abbr_all_twelve(void)
{
    const char *expected[] = {
        "Man", "Kum", "Mii", "Mee",
        "Phe", "Mit", "Kaw", "Sin",
        "Kan", "Tul", "Pha", "Thw"
    };
    for (int m = 1; m <= 12; m++) {
        TEST_ASSERT_EQUAL_STRING(expected[m - 1], lao_month_abbr(m));
    }
}

void test_month_abbr_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", lao_month_abbr(0));
    TEST_ASSERT_EQUAL_STRING("?", lao_month_abbr(13));
}

/* ===== Pi Mai detection ===== */

void test_pi_mai_april_14(void)
{
    lao_date_t d = { .be_year = 2569, .month = 4, .day = 14 };
    TEST_ASSERT_TRUE(lao_is_pi_mai(d));
}

void test_pi_mai_april_15(void)
{
    lao_date_t d = { .be_year = 2569, .month = 4, .day = 15 };
    TEST_ASSERT_TRUE(lao_is_pi_mai(d));
}

void test_pi_mai_april_16(void)
{
    lao_date_t d = { .be_year = 2569, .month = 4, .day = 16 };
    TEST_ASSERT_TRUE(lao_is_pi_mai(d));
}

void test_not_pi_mai_april_13(void)
{
    lao_date_t d = { .be_year = 2569, .month = 4, .day = 13 };
    TEST_ASSERT_FALSE(lao_is_pi_mai(d));
}

void test_not_pi_mai_april_17(void)
{
    lao_date_t d = { .be_year = 2569, .month = 4, .day = 17 };
    TEST_ASSERT_FALSE(lao_is_pi_mai(d));
}

void test_not_pi_mai_other_month(void)
{
    lao_date_t d = { .be_year = 2569, .month = 1, .day = 14 };
    TEST_ASSERT_FALSE(lao_is_pi_mai(d));
}

/* ===== Festival detection ===== */

void test_festival_pi_mai(void)
{
    lao_date_t d = { .be_year = 2569, .month = 4, .day = 14 };
    TEST_ASSERT_EQUAL_INT(LAO_FEST_PI_MAI, lao_festival(d));
}

void test_festival_national_day(void)
{
    lao_date_t d = { .be_year = 2569, .month = 12, .day = 2 };
    TEST_ASSERT_EQUAL_INT(LAO_FEST_NATIONAL_DAY, lao_festival(d));
}

void test_festival_liberation_day(void)
{
    lao_date_t d = { .be_year = 2569, .month = 10, .day = 12 };
    TEST_ASSERT_EQUAL_INT(LAO_FEST_LIBERATION_DAY, lao_festival(d));
}

void test_festival_none(void)
{
    lao_date_t d = { .be_year = 2569, .month = 3, .day = 15 };
    TEST_ASSERT_EQUAL_INT(LAO_FEST_NONE, lao_festival(d));
}

/* ===== Festival names ===== */

void test_festival_name_pi_mai(void)
{
    TEST_ASSERT_EQUAL_STRING("Pi Mai Lao", lao_festival_name(LAO_FEST_PI_MAI));
}

void test_festival_name_national(void)
{
    TEST_ASSERT_EQUAL_STRING("National Day", lao_festival_name(LAO_FEST_NATIONAL_DAY));
}

void test_festival_name_liberation(void)
{
    TEST_ASSERT_EQUAL_STRING("Liberation Day", lao_festival_name(LAO_FEST_LIBERATION_DAY));
}

void test_festival_name_none(void)
{
    TEST_ASSERT_EQUAL_STRING("", lao_festival_name(LAO_FEST_NONE));
}

void test_festival_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", lao_festival_name(LAO_FEST_COUNT));
}

/* ===== Month days ===== */

void test_month_days_january(void)
{
    TEST_ASSERT_EQUAL_INT(31, lao_month_days(2569, 1));
}

void test_month_days_february_leap(void)
{
    /* BE 2567 = 2024 CE (leap) */
    TEST_ASSERT_EQUAL_INT(29, lao_month_days(2567, 2));
}

void test_month_days_february_common(void)
{
    /* BE 2568 = 2025 CE (not leap) */
    TEST_ASSERT_EQUAL_INT(28, lao_month_days(2568, 2));
}

void test_month_days_april(void)
{
    TEST_ASSERT_EQUAL_INT(30, lao_month_days(2569, 4));
}

void test_month_days_all(void)
{
    /* 2024 CE = BE 2567 (leap year) */
    int expected[] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    for (int m = 1; m <= 12; m++) {
        TEST_ASSERT_EQUAL_INT(expected[m - 1], lao_month_days(2567, m));
    }
}

void test_month_days_invalid(void)
{
    TEST_ASSERT_EQUAL_INT(0, lao_month_days(2569, 0));
    TEST_ASSERT_EQUAL_INT(0, lao_month_days(2569, 13));
}

/* ===== Leap year ===== */

void test_leap_year_2567_be(void)
{
    /* 2024 CE = leap */
    TEST_ASSERT_TRUE(lao_is_leap_year(2567));
}

void test_not_leap_year_2568_be(void)
{
    /* 2025 CE = not leap */
    TEST_ASSERT_FALSE(lao_is_leap_year(2568));
}

void test_not_leap_century_2443_be(void)
{
    /* 1900 CE = not leap (div 100, not 400) */
    TEST_ASSERT_FALSE(lao_is_leap_year(2443));
}

void test_leap_century_2543_be(void)
{
    /* 2000 CE = leap (div 400) */
    TEST_ASSERT_TRUE(lao_is_leap_year(2543));
}

/* ===== Day of week ===== */

void test_day_of_week_known(void)
{
    /* 2026-03-20 is Friday (5) */
    lao_date_t d = { .be_year = 2569, .month = 3, .day = 20 };
    TEST_ASSERT_EQUAL_INT(5, lao_day_of_week(d));
}

void test_day_of_week_j2000(void)
{
    /* 2000-01-01 was Saturday (6) */
    lao_date_t d = { .be_year = 2543, .month = 1, .day = 1 };
    TEST_ASSERT_EQUAL_INT(6, lao_day_of_week(d));
}

/* ===== Day names ===== */

void test_day_name_sunday(void)
{
    TEST_ASSERT_EQUAL_STRING("Wan Aathit", lao_day_name(0));
}

void test_day_name_saturday(void)
{
    TEST_ASSERT_EQUAL_STRING("Wan Sao", lao_day_name(6));
}

void test_day_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", lao_day_name(7));
    TEST_ASSERT_EQUAL_STRING("?", lao_day_name(-1));
}

/* ===== Nang Sangkhan ===== */

void test_sangkhan_returns_valid_maiden(void)
{
    lao_sangkhan_t s = lao_sangkhan(2569);
    TEST_ASSERT_NOT_NULL(s.name);
    TEST_ASSERT_NOT_NULL(s.mount);
    TEST_ASSERT_NOT_NULL(s.offering);
    TEST_ASSERT_TRUE(s.weekday >= 0 && s.weekday <= 6);
}

void test_sangkhan_weekday_matches_april_14(void)
{
    /* The maiden is determined by the weekday of April 14 */
    lao_date_t pi_mai = { .be_year = 2569, .month = 4, .day = 14 };
    int dow = lao_day_of_week(pi_mai);
    lao_sangkhan_t s = lao_sangkhan(2569);
    TEST_ASSERT_EQUAL_INT(dow, s.weekday);
}

void test_sangkhan_different_years(void)
{
    /* Different years should potentially have different maidens */
    lao_sangkhan_t s1 = lao_sangkhan(2569);
    lao_sangkhan_t s2 = lao_sangkhan(2570);
    /* At least verify both are valid */
    TEST_ASSERT_NOT_NULL(s1.name);
    TEST_ASSERT_NOT_NULL(s2.name);
}

/* ===== Zodiac ===== */

void test_zodiac_12_cycle(void)
{
    /* 12 consecutive years should give different animal names
     * (or repeat after 12) */
    const char *animals[12];
    for (int i = 0; i < 12; i++) {
        animals[i] = lao_zodiac_animal(2560 + i);
        TEST_ASSERT_NOT_NULL(animals[i]);
        TEST_ASSERT_TRUE(strcmp(animals[i], "?") != 0);
    }
    /* Year 12 should match year 0 */
    TEST_ASSERT_EQUAL_STRING(animals[0], lao_zodiac_animal(2560 + 12));
}

void test_zodiac_naga_not_dragon(void)
{
    /* Lao uses Naga instead of Dragon (position 5 in cycle).
     * Check a known Naga year: 2024 CE = BE 2567 = Dragon/Naga year */
    const char *animal = lao_zodiac_animal(2567);
    TEST_ASSERT_NOT_NULL(strstr(animal, "Naga"));
}

/* ===== Midnight edge ===== */

void test_from_jd_midnight(void)
{
    /* JD 2451544.5 = 1 Jan 2000 midnight UT */
    lao_date_t d = lao_from_jd(2451544.5);
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
    RUN_TEST(test_be_year_2026_ce);
    RUN_TEST(test_be_year_1_ce);
    RUN_TEST(test_ce_year_2569_be);
    RUN_TEST(test_ce_year_544_be);
    RUN_TEST(test_be_ce_roundtrip);

    /* JD conversion */
    RUN_TEST(test_from_jd_j2000);
    RUN_TEST(test_from_jd_known_date);
    RUN_TEST(test_to_jd_jan_1_2000);
    RUN_TEST(test_to_jd_pi_mai);
    RUN_TEST(test_jd_roundtrip_various);

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

    /* Pi Mai */
    RUN_TEST(test_pi_mai_april_14);
    RUN_TEST(test_pi_mai_april_15);
    RUN_TEST(test_pi_mai_april_16);
    RUN_TEST(test_not_pi_mai_april_13);
    RUN_TEST(test_not_pi_mai_april_17);
    RUN_TEST(test_not_pi_mai_other_month);

    /* Festivals */
    RUN_TEST(test_festival_pi_mai);
    RUN_TEST(test_festival_national_day);
    RUN_TEST(test_festival_liberation_day);
    RUN_TEST(test_festival_none);
    RUN_TEST(test_festival_name_pi_mai);
    RUN_TEST(test_festival_name_national);
    RUN_TEST(test_festival_name_liberation);
    RUN_TEST(test_festival_name_none);
    RUN_TEST(test_festival_name_invalid);

    /* Month days */
    RUN_TEST(test_month_days_january);
    RUN_TEST(test_month_days_february_leap);
    RUN_TEST(test_month_days_february_common);
    RUN_TEST(test_month_days_april);
    RUN_TEST(test_month_days_all);
    RUN_TEST(test_month_days_invalid);

    /* Leap year */
    RUN_TEST(test_leap_year_2567_be);
    RUN_TEST(test_not_leap_year_2568_be);
    RUN_TEST(test_not_leap_century_2443_be);
    RUN_TEST(test_leap_century_2543_be);

    /* Day of week */
    RUN_TEST(test_day_of_week_known);
    RUN_TEST(test_day_of_week_j2000);

    /* Day names */
    RUN_TEST(test_day_name_sunday);
    RUN_TEST(test_day_name_saturday);
    RUN_TEST(test_day_name_invalid);

    /* Nang Sangkhan */
    RUN_TEST(test_sangkhan_returns_valid_maiden);
    RUN_TEST(test_sangkhan_weekday_matches_april_14);
    RUN_TEST(test_sangkhan_different_years);

    /* Zodiac */
    RUN_TEST(test_zodiac_12_cycle);
    RUN_TEST(test_zodiac_naga_not_dragon);

    /* Edge */
    RUN_TEST(test_from_jd_midnight);

    return UNITY_END();
}
