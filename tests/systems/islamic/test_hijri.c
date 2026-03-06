/* test_hijri.c — Tabular Islamic (Hijri) calendar tests
 * TDD RED phase: tests written before implementation.
 * Algorithm: Dershowitz & Reingold "Calendrical Calculations" (tabular) */

#include "../../unity/unity.h"
#include "../../../src/systems/islamic/hijri.h"
#include <math.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== Epoch conversion tests ===== */

void test_epoch_to_jd(void)
{
    /* 1 Muharram 1 AH = JD 1948439.5 => integer day 1948439 */
    hijri_date_t d = { .year = 1, .month = 1, .day = 1 };
    int jdn = (int)floor(hijri_to_jd(d));
    TEST_ASSERT_EQUAL_INT(1948439, jdn);
}

void test_epoch_from_jd(void)
{
    /* JD 1948439.5 = 1 Muharram 1 AH */
    hijri_date_t d = hijri_from_jd(1948439.5);
    TEST_ASSERT_EQUAL_INT(1, d.year);
    TEST_ASSERT_EQUAL_INT(1, d.month);
    TEST_ASSERT_EQUAL_INT(1, d.day);
}

/* ===== Known date: 1 Muharram 1446 ===== */

void test_muharram_1446_to_jd(void)
{
    /* 1 Muharram 1446 (tabular) = JD 2460499.5 => integer day 2460499
     * 1445*354 + floor((3+11*1446)/30) = 511530 + 530 = 512060
     * JD = 1948439.5 + 512060 = 2460499.5 */
    hijri_date_t d = { .year = 1446, .month = 1, .day = 1 };
    int jdn = (int)floor(hijri_to_jd(d));
    TEST_ASSERT_EQUAL_INT(2460499, jdn);
}

void test_muharram_1446_from_jd(void)
{
    /* JD 2460499.5 = 1 Muharram 1446 */
    hijri_date_t d = hijri_from_jd(2460499.5);
    TEST_ASSERT_EQUAL_INT(1446, d.year);
    TEST_ASSERT_EQUAL_INT(1, d.month);
    TEST_ASSERT_EQUAL_INT(1, d.day);
}

/* ===== Known date: 1 Ramadan 1446 ===== */

void test_ramadan_1446_to_jd(void)
{
    /* 1 Ramadan 1446 = 1 Muharram 1446 + 236 days
     * months 1-8: 30+29+30+29+30+29+30+29 = 236
     * JD = 2460499.5 + 236 = 2460735.5 => integer day 2460735 */
    hijri_date_t d = { .year = 1446, .month = 9, .day = 1 };
    int jdn = (int)floor(hijri_to_jd(d));
    TEST_ASSERT_EQUAL_INT(2460735, jdn);
}

void test_ramadan_1446_from_jd(void)
{
    /* JD 2460735.5 = 1 Ramadan 1446 */
    hijri_date_t d = hijri_from_jd(2460735.5);
    TEST_ASSERT_EQUAL_INT(1446, d.year);
    TEST_ASSERT_EQUAL_INT(9, d.month);
    TEST_ASSERT_EQUAL_INT(1, d.day);
}

/* ===== Known date: 1 Muharram 1445 ===== */

void test_muharram_1445_to_jd(void)
{
    /* 1 Muharram 1445: year 1445 is leap (355 days)
     * So 1 Muharram 1445 = 2460499.5 - 355 = 2460144.5 => integer day 2460144 */
    hijri_date_t d = { .year = 1445, .month = 1, .day = 1 };
    int jdn = (int)floor(hijri_to_jd(d));
    TEST_ASSERT_EQUAL_INT(2460144, jdn);
}

void test_muharram_1445_from_jd(void)
{
    /* JD 2460144.5 = 1 Muharram 1445 */
    hijri_date_t d = hijri_from_jd(2460144.5);
    TEST_ASSERT_EQUAL_INT(1445, d.year);
    TEST_ASSERT_EQUAL_INT(1, d.month);
    TEST_ASSERT_EQUAL_INT(1, d.day);
}

/* ===== Leap year tests ===== */

void test_leap_year_2(void)
{
    /* Year 2: (11*2+14)%30 = 36%30 = 6 < 11 => LEAP */
    TEST_ASSERT_TRUE(hijri_is_leap(2));
}

void test_leap_year_5(void)
{
    /* Year 5: (11*5+14)%30 = 69%30 = 9 < 11 => LEAP */
    TEST_ASSERT_TRUE(hijri_is_leap(5));
}

void test_leap_year_7(void)
{
    /* Year 7: (11*7+14)%30 = 91%30 = 1 < 11 => LEAP */
    TEST_ASSERT_TRUE(hijri_is_leap(7));
}

void test_leap_year_29(void)
{
    /* Year 29: (11*29+14)%30 = 333%30 = 3 < 11 => LEAP */
    TEST_ASSERT_TRUE(hijri_is_leap(29));
}

void test_common_year_1(void)
{
    /* Year 1: (11*1+14)%30 = 25%30 = 25 >= 11 => COMMON */
    TEST_ASSERT_FALSE(hijri_is_leap(1));
}

void test_common_year_3(void)
{
    /* Year 3: (11*3+14)%30 = 47%30 = 17 >= 11 => COMMON */
    TEST_ASSERT_FALSE(hijri_is_leap(3));
}

void test_common_year_4(void)
{
    /* Year 4: (11*4+14)%30 = 58%30 = 28 >= 11 => COMMON */
    TEST_ASSERT_FALSE(hijri_is_leap(4));
}

void test_leap_year_1445(void)
{
    /* Year 1445: (11*1445+14)%30 = 15909%30 = 9 < 11 => LEAP */
    TEST_ASSERT_TRUE(hijri_is_leap(1445));
}

void test_common_year_1446(void)
{
    /* Year 1446: (11*1446+14)%30 = 15920%30 = 20 >= 11 => COMMON */
    TEST_ASSERT_FALSE(hijri_is_leap(1446));
}

/* ===== All 11 leap years in the first 30-year cycle ===== */

void test_leap_years_in_cycle(void)
{
    int leap_years[] = { 2, 5, 7, 10, 13, 16, 18, 21, 24, 26, 29 };
    int i;
    for (i = 0; i < 11; i++) {
        TEST_ASSERT_TRUE(hijri_is_leap(leap_years[i]));
    }
    /* Also verify common years in the cycle */
    int common_years[] = { 1, 3, 4, 6, 8, 9, 11, 12, 14, 15,
                           17, 19, 20, 22, 23, 25, 27, 28, 30 };
    for (i = 0; i < 19; i++) {
        TEST_ASSERT_FALSE(hijri_is_leap(common_years[i]));
    }
}

/* ===== Year days tests ===== */

void test_year_days_common(void)
{
    /* Common year = 354 days */
    TEST_ASSERT_EQUAL_INT(354, hijri_year_days(1));
}

void test_year_days_leap(void)
{
    /* Leap year = 355 days */
    TEST_ASSERT_EQUAL_INT(355, hijri_year_days(2));
}

void test_year_days_1446(void)
{
    /* 1446 is common */
    TEST_ASSERT_EQUAL_INT(354, hijri_year_days(1446));
}

void test_year_days_1445(void)
{
    /* 1445 is leap */
    TEST_ASSERT_EQUAL_INT(355, hijri_year_days(1445));
}

/* ===== Month days tests ===== */

void test_month_days_odd(void)
{
    /* Odd months (1,3,5,7,9,11) = 30 days */
    TEST_ASSERT_EQUAL_INT(30, hijri_month_days(1446, 1));
    TEST_ASSERT_EQUAL_INT(30, hijri_month_days(1446, 3));
    TEST_ASSERT_EQUAL_INT(30, hijri_month_days(1446, 5));
    TEST_ASSERT_EQUAL_INT(30, hijri_month_days(1446, 7));
    TEST_ASSERT_EQUAL_INT(30, hijri_month_days(1446, 9));
    TEST_ASSERT_EQUAL_INT(30, hijri_month_days(1446, 11));
}

void test_month_days_even(void)
{
    /* Even months (2,4,6,8,10) = 29 days */
    TEST_ASSERT_EQUAL_INT(29, hijri_month_days(1446, 2));
    TEST_ASSERT_EQUAL_INT(29, hijri_month_days(1446, 4));
    TEST_ASSERT_EQUAL_INT(29, hijri_month_days(1446, 6));
    TEST_ASSERT_EQUAL_INT(29, hijri_month_days(1446, 8));
    TEST_ASSERT_EQUAL_INT(29, hijri_month_days(1446, 10));
}

void test_month_days_12_common(void)
{
    /* Month 12 in common year = 29 days */
    TEST_ASSERT_EQUAL_INT(29, hijri_month_days(1446, 12));
}

void test_month_days_12_leap(void)
{
    /* Month 12 in leap year = 30 days */
    TEST_ASSERT_EQUAL_INT(30, hijri_month_days(1445, 12));
}

void test_invalid_month_days_zero(void)
{
    /* Month 0 is invalid */
    TEST_ASSERT_EQUAL_INT(0, hijri_month_days(1446, 0));
}

void test_invalid_month_days_13(void)
{
    /* Month 13 is invalid */
    TEST_ASSERT_EQUAL_INT(0, hijri_month_days(1446, 13));
}

/* ===== Month name tests ===== */

void test_month_name_muharram(void)
{
    TEST_ASSERT_EQUAL_STRING("Muharram", hijri_month_name(1));
}

void test_month_name_safar(void)
{
    TEST_ASSERT_EQUAL_STRING("Safar", hijri_month_name(2));
}

void test_month_name_ramadan(void)
{
    TEST_ASSERT_EQUAL_STRING("Ramadan", hijri_month_name(9));
}

void test_month_name_dhul_hijjah(void)
{
    TEST_ASSERT_EQUAL_STRING("Dhul Hijjah", hijri_month_name(12));
}

void test_month_name_invalid_zero(void)
{
    TEST_ASSERT_EQUAL_STRING("?", hijri_month_name(0));
}

void test_month_name_invalid_13(void)
{
    TEST_ASSERT_EQUAL_STRING("?", hijri_month_name(13));
}

/* ===== Day of year tests ===== */

void test_day_of_year_first(void)
{
    /* 1 Muharram = day 1 */
    hijri_date_t d = { .year = 1446, .month = 1, .day = 1 };
    TEST_ASSERT_EQUAL_INT(1, hijri_day_of_year(d));
}

void test_day_of_year_second_month(void)
{
    /* 1 Safar = day 31 (Muharram has 30 days) */
    hijri_date_t d = { .year = 1446, .month = 2, .day = 1 };
    TEST_ASSERT_EQUAL_INT(31, hijri_day_of_year(d));
}

void test_day_of_year_mid(void)
{
    /* 15 Rajab (month 7) = 30+29+30+29+30+29 + 15 = 177 + 15 = 192 */
    hijri_date_t d = { .year = 1446, .month = 7, .day = 15 };
    TEST_ASSERT_EQUAL_INT(192, hijri_day_of_year(d));
}

void test_day_of_year_last_common(void)
{
    /* 29 Dhul Hijjah in common year = day 354 */
    hijri_date_t d = { .year = 1446, .month = 12, .day = 29 };
    TEST_ASSERT_EQUAL_INT(354, hijri_day_of_year(d));
}

void test_day_of_year_last_leap(void)
{
    /* 30 Dhul Hijjah in leap year = day 355 */
    hijri_date_t d = { .year = 1445, .month = 12, .day = 30 };
    TEST_ASSERT_EQUAL_INT(355, hijri_day_of_year(d));
}

/* ===== Format tests ===== */

void test_fmt_output(void)
{
    hijri_date_t d = { .year = 1446, .month = 9, .day = 15 };
    char buf[64];
    hijri_fmt(d, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_STRING("15 Ramadan 1446", buf);
}

void test_fmt_epoch(void)
{
    hijri_date_t d = { .year = 1, .month = 1, .day = 1 };
    char buf[64];
    hijri_fmt(d, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_STRING("1 Muharram 1", buf);
}

void test_fmt_truncation(void)
{
    /* Buffer too small should not overflow */
    hijri_date_t d = { .year = 1446, .month = 9, .day = 15 };
    char buf[8];
    hijri_fmt(d, buf, sizeof(buf));
    /* Should be truncated but null-terminated */
    TEST_ASSERT_EQUAL_INT('\0', buf[7]);
}

/* ===== Round-trip tests ===== */

void test_round_trip_epoch(void)
{
    double jd = 1948439.5;
    hijri_date_t d = hijri_from_jd(jd);
    int jdn_orig = (int)floor(jd);
    int jdn_back = (int)floor(hijri_to_jd(d));
    TEST_ASSERT_EQUAL_INT(jdn_orig, jdn_back);
}

void test_round_trip_1446(void)
{
    double jd = 2460499.5;
    hijri_date_t d = hijri_from_jd(jd);
    int jdn_orig = (int)floor(jd);
    int jdn_back = (int)floor(hijri_to_jd(d));
    TEST_ASSERT_EQUAL_INT(jdn_orig, jdn_back);
}

void test_round_trip_many(void)
{
    /* Test round-trip for 100 consecutive days starting from 1 Muharram 1446 */
    double start_jd = 2460499.5;
    int i;
    for (i = 0; i < 100; i++) {
        double jd = start_jd + (double)i;
        hijri_date_t d = hijri_from_jd(jd);
        int jdn_orig = (int)floor(jd);
        int jdn_back = (int)floor(hijri_to_jd(d));
        TEST_ASSERT_EQUAL_INT(jdn_orig, jdn_back);
    }
}

void test_round_trip_across_years(void)
{
    /* Test round-trip across a full 30-year cycle (10631 days)
     * Sample every 100th day */
    double start_jd = 1948439.5;
    int i;
    for (i = 0; i < 10631; i += 100) {
        double jd = start_jd + (double)i;
        hijri_date_t d = hijri_from_jd(jd);
        int jdn_orig = (int)floor(jd);
        int jdn_back = (int)floor(hijri_to_jd(d));
        TEST_ASSERT_EQUAL_INT(jdn_orig, jdn_back);
    }
}

/* ===== Edge case: last day of year ===== */

void test_edge_last_day_common(void)
{
    /* 29 Dhul Hijjah in common year (1446) */
    hijri_date_t d = { .year = 1446, .month = 12, .day = 29 };
    double jd = hijri_to_jd(d);
    hijri_date_t d2 = hijri_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(1446, d2.year);
    TEST_ASSERT_EQUAL_INT(12, d2.month);
    TEST_ASSERT_EQUAL_INT(29, d2.day);
}

void test_edge_last_day_leap(void)
{
    /* 30 Dhul Hijjah in leap year (1445) */
    hijri_date_t d = { .year = 1445, .month = 12, .day = 30 };
    double jd = hijri_to_jd(d);
    hijri_date_t d2 = hijri_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(1445, d2.year);
    TEST_ASSERT_EQUAL_INT(12, d2.month);
    TEST_ASSERT_EQUAL_INT(30, d2.day);
}

void test_edge_day_after_common_year_end(void)
{
    /* Day after 29 Dhul Hijjah 1446 = 1 Muharram 1447 */
    hijri_date_t d = { .year = 1446, .month = 12, .day = 29 };
    double jd = hijri_to_jd(d) + 1.0;
    hijri_date_t d2 = hijri_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(1447, d2.year);
    TEST_ASSERT_EQUAL_INT(1, d2.month);
    TEST_ASSERT_EQUAL_INT(1, d2.day);
}

void test_edge_day_after_leap_year_end(void)
{
    /* Day after 30 Dhul Hijjah 1445 = 1 Muharram 1446 */
    hijri_date_t d = { .year = 1445, .month = 12, .day = 30 };
    double jd = hijri_to_jd(d) + 1.0;
    hijri_date_t d2 = hijri_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(1446, d2.year);
    TEST_ASSERT_EQUAL_INT(1, d2.month);
    TEST_ASSERT_EQUAL_INT(1, d2.day);
}

/* ===== All month names ===== */

void test_all_month_names(void)
{
    TEST_ASSERT_EQUAL_STRING("Muharram", hijri_month_name(1));
    TEST_ASSERT_EQUAL_STRING("Safar", hijri_month_name(2));
    TEST_ASSERT_EQUAL_STRING("Rabi' al-Awwal", hijri_month_name(3));
    TEST_ASSERT_EQUAL_STRING("Rabi' al-Thani", hijri_month_name(4));
    TEST_ASSERT_EQUAL_STRING("Jumada al-Ula", hijri_month_name(5));
    TEST_ASSERT_EQUAL_STRING("Jumada al-Thani", hijri_month_name(6));
    TEST_ASSERT_EQUAL_STRING("Rajab", hijri_month_name(7));
    TEST_ASSERT_EQUAL_STRING("Sha'ban", hijri_month_name(8));
    TEST_ASSERT_EQUAL_STRING("Ramadan", hijri_month_name(9));
    TEST_ASSERT_EQUAL_STRING("Shawwal", hijri_month_name(10));
    TEST_ASSERT_EQUAL_STRING("Dhul Qi'dah", hijri_month_name(11));
    TEST_ASSERT_EQUAL_STRING("Dhul Hijjah", hijri_month_name(12));
}

int main(void)
{
    UNITY_BEGIN();

    /* Epoch */
    RUN_TEST(test_epoch_to_jd);
    RUN_TEST(test_epoch_from_jd);

    /* Known dates */
    RUN_TEST(test_muharram_1446_to_jd);
    RUN_TEST(test_muharram_1446_from_jd);
    RUN_TEST(test_ramadan_1446_to_jd);
    RUN_TEST(test_ramadan_1446_from_jd);
    RUN_TEST(test_muharram_1445_to_jd);
    RUN_TEST(test_muharram_1445_from_jd);

    /* Leap years */
    RUN_TEST(test_leap_year_2);
    RUN_TEST(test_leap_year_5);
    RUN_TEST(test_leap_year_7);
    RUN_TEST(test_leap_year_29);
    RUN_TEST(test_common_year_1);
    RUN_TEST(test_common_year_3);
    RUN_TEST(test_common_year_4);
    RUN_TEST(test_leap_year_1445);
    RUN_TEST(test_common_year_1446);
    RUN_TEST(test_leap_years_in_cycle);

    /* Year days */
    RUN_TEST(test_year_days_common);
    RUN_TEST(test_year_days_leap);
    RUN_TEST(test_year_days_1446);
    RUN_TEST(test_year_days_1445);

    /* Month days */
    RUN_TEST(test_month_days_odd);
    RUN_TEST(test_month_days_even);
    RUN_TEST(test_month_days_12_common);
    RUN_TEST(test_month_days_12_leap);
    RUN_TEST(test_invalid_month_days_zero);
    RUN_TEST(test_invalid_month_days_13);

    /* Month names */
    RUN_TEST(test_month_name_muharram);
    RUN_TEST(test_month_name_safar);
    RUN_TEST(test_month_name_ramadan);
    RUN_TEST(test_month_name_dhul_hijjah);
    RUN_TEST(test_month_name_invalid_zero);
    RUN_TEST(test_month_name_invalid_13);
    RUN_TEST(test_all_month_names);

    /* Day of year */
    RUN_TEST(test_day_of_year_first);
    RUN_TEST(test_day_of_year_second_month);
    RUN_TEST(test_day_of_year_mid);
    RUN_TEST(test_day_of_year_last_common);
    RUN_TEST(test_day_of_year_last_leap);

    /* Format */
    RUN_TEST(test_fmt_output);
    RUN_TEST(test_fmt_epoch);
    RUN_TEST(test_fmt_truncation);

    /* Round trip */
    RUN_TEST(test_round_trip_epoch);
    RUN_TEST(test_round_trip_1446);
    RUN_TEST(test_round_trip_many);
    RUN_TEST(test_round_trip_across_years);

    /* Edge cases */
    RUN_TEST(test_edge_last_day_common);
    RUN_TEST(test_edge_last_day_leap);
    RUN_TEST(test_edge_day_after_common_year_end);
    RUN_TEST(test_edge_day_after_leap_year_end);

    return UNITY_END();
}
