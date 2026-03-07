/* test_ethiopian.c — Ethiopian calendar tests
 * TDD RED phase: tests written before implementation.
 * Epoch: JD 1724220.5 = 29 Aug 8 CE (Julian) = Year 1, 1 Meskerem
 * 12 months x 30 days + Pagume (5 or 6 epagomenal) = 365 or 366 days.
 * Leap year when (year % 4) == 3.
 *
 * Structurally identical to the Coptic calendar, but with different
 * epoch (Coptic: JD 1825029.5) and Ge'ez month names. */

#include "../../unity/unity.h"
#include "../../../src/systems/ethiopian/ethiopian.h"
#include <math.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== Epoch conversion tests ===== */

void test_epoch_to_jd(void)
{
    /* Year 1, 1 Meskerem = JD 1724220.5 => integer day 1724220 */
    ethiopian_date_t d = { .year = 1, .month = 1, .day = 1 };
    int jdn = (int)floor(ethiopian_to_jd(d));
    TEST_ASSERT_EQUAL_INT(1724220, jdn);
}

void test_epoch_from_jd(void)
{
    /* JD 1724220.5 = Year 1, 1 Meskerem */
    ethiopian_date_t d = ethiopian_from_jd(1724220.5);
    TEST_ASSERT_EQUAL_INT(1, d.year);
    TEST_ASSERT_EQUAL_INT(1, d.month);
    TEST_ASSERT_EQUAL_INT(1, d.day);
}

/* ===== End of year 1 (common year) ===== */

void test_last_day_of_year1(void)
{
    /* Year 1: (1 % 4 == 1) not a leap year -> Pagume has 5 days
     * Last day = day 365 = month 13, day 5
     * JD = 1724220.5 + 364 */
    ethiopian_date_t d = ethiopian_from_jd(1724220.5 + 364.0);
    TEST_ASSERT_EQUAL_INT(1, d.year);
    TEST_ASSERT_EQUAL_INT(13, d.month);
    TEST_ASSERT_EQUAL_INT(5, d.day);
}

void test_last_day_of_year1_to_jd(void)
{
    /* Year 1, Pagume day 5 = JD 1724220.5 + 364 => integer day 1724584 */
    ethiopian_date_t d = { .year = 1, .month = 13, .day = 5 };
    int jdn = (int)floor(ethiopian_to_jd(d));
    TEST_ASSERT_EQUAL_INT(1724584, jdn);
}

/* ===== Start of year 2 ===== */

void test_first_day_of_year2(void)
{
    /* JD = 1724220.5 + 365 = 1724585.5 = Year 2, 1 Meskerem */
    ethiopian_date_t d = ethiopian_from_jd(1724220.5 + 365.0);
    TEST_ASSERT_EQUAL_INT(2, d.year);
    TEST_ASSERT_EQUAL_INT(1, d.month);
    TEST_ASSERT_EQUAL_INT(1, d.day);
}

void test_first_day_of_year2_to_jd(void)
{
    /* Year 2, 1 Meskerem = JD 1724220.5 + 365 => integer day 1724585 */
    ethiopian_date_t d = { .year = 2, .month = 1, .day = 1 };
    int jdn = (int)floor(ethiopian_to_jd(d));
    TEST_ASSERT_EQUAL_INT(1724585, jdn);
}

/* ===== Leap year detection ===== */

void test_leap_year_3(void)
{
    /* 3 % 4 == 3 -> leap */
    TEST_ASSERT_TRUE(ethiopian_is_leap(3));
}

void test_leap_year_4(void)
{
    /* 4 % 4 == 0 -> not leap */
    TEST_ASSERT_FALSE(ethiopian_is_leap(4));
}

void test_leap_year_7(void)
{
    /* 7 % 4 == 3 -> leap */
    TEST_ASSERT_TRUE(ethiopian_is_leap(7));
}

void test_leap_year_1(void)
{
    /* 1 % 4 == 1 -> not leap */
    TEST_ASSERT_FALSE(ethiopian_is_leap(1));
}

void test_leap_year_11(void)
{
    /* 11 % 4 == 3 -> leap */
    TEST_ASSERT_TRUE(ethiopian_is_leap(11));
}

void test_leap_year_2(void)
{
    /* 2 % 4 == 2 -> not leap */
    TEST_ASSERT_FALSE(ethiopian_is_leap(2));
}

void test_leap_year_2015(void)
{
    /* 2015 % 4 == 3 -> leap */
    TEST_ASSERT_TRUE(ethiopian_is_leap(2015));
}

void test_leap_year_2016(void)
{
    /* 2016 % 4 == 0 -> not leap */
    TEST_ASSERT_FALSE(ethiopian_is_leap(2016));
}

/* ===== Month days ===== */

void test_month_days_regular(void)
{
    /* Months 1-12 always have 30 days */
    for (int m = 1; m <= 12; m++) {
        TEST_ASSERT_EQUAL_INT(30, ethiopian_month_days(1, m));
    }
}

void test_month_days_pagume_common(void)
{
    /* Pagume in common year (Year 1) has 5 days */
    TEST_ASSERT_EQUAL_INT(5, ethiopian_month_days(1, 13));
}

void test_month_days_pagume_leap(void)
{
    /* Pagume in leap year (Year 3) has 6 days */
    TEST_ASSERT_EQUAL_INT(6, ethiopian_month_days(3, 13));
}

void test_month_days_invalid_zero(void)
{
    TEST_ASSERT_EQUAL_INT(0, ethiopian_month_days(1, 0));
}

void test_month_days_invalid_14(void)
{
    TEST_ASSERT_EQUAL_INT(0, ethiopian_month_days(1, 14));
}

void test_month_days_invalid_negative(void)
{
    TEST_ASSERT_EQUAL_INT(0, ethiopian_month_days(1, -1));
}

/* ===== Year days ===== */

void test_year_days_common(void)
{
    /* Year 1: common year = 365 days */
    TEST_ASSERT_EQUAL_INT(365, ethiopian_year_days(1));
}

void test_year_days_leap(void)
{
    /* Year 3: leap year = 366 days */
    TEST_ASSERT_EQUAL_INT(366, ethiopian_year_days(3));
}

/* ===== Leap year boundary: Year 3 has 366 days ===== */

void test_year3_last_day_pagume6(void)
{
    /* Year 3 is leap -> Pagume has 6 days -> last day = Pagume 6
     * to_jd: epoch + 365*(3-1) + 3/4 + 30*(13-1) + 6 - 1
     *       = epoch + 730 + 0 + 360 + 5 = epoch + 1095 */
    ethiopian_date_t d = { .year = 3, .month = 13, .day = 6 };
    double jd = ethiopian_to_jd(d);
    ethiopian_date_t back = ethiopian_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(3, back.year);
    TEST_ASSERT_EQUAL_INT(13, back.month);
    TEST_ASSERT_EQUAL_INT(6, back.day);
}

void test_year4_first_day_after_leap(void)
{
    /* Year 4, 1 Meskerem should be day after Year 3, Pagume 6
     * to_jd(3,13,6) + 1 == to_jd(4,1,1) */
    ethiopian_date_t leap_end = { .year = 3, .month = 13, .day = 6 };
    ethiopian_date_t next_year = { .year = 4, .month = 1, .day = 1 };
    double diff = ethiopian_to_jd(next_year) - ethiopian_to_jd(leap_end);
    TEST_ASSERT_TRUE(diff > 0.999 && diff < 1.001);
}

/* ===== Month names ===== */

void test_month_name_meskerem(void)
{
    TEST_ASSERT_EQUAL_STRING("Meskerem", ethiopian_month_name(1));
}

void test_month_name_tikimt(void)
{
    TEST_ASSERT_EQUAL_STRING("Tikimt", ethiopian_month_name(2));
}

void test_month_name_hidar(void)
{
    TEST_ASSERT_EQUAL_STRING("Hidar", ethiopian_month_name(3));
}

void test_month_name_tahsas(void)
{
    TEST_ASSERT_EQUAL_STRING("Tahsas", ethiopian_month_name(4));
}

void test_month_name_tir(void)
{
    TEST_ASSERT_EQUAL_STRING("Tir", ethiopian_month_name(5));
}

void test_month_name_yekatit(void)
{
    TEST_ASSERT_EQUAL_STRING("Yekatit", ethiopian_month_name(6));
}

void test_month_name_megabit(void)
{
    TEST_ASSERT_EQUAL_STRING("Megabit", ethiopian_month_name(7));
}

void test_month_name_miyazia(void)
{
    TEST_ASSERT_EQUAL_STRING("Miyazia", ethiopian_month_name(8));
}

void test_month_name_ginbot(void)
{
    TEST_ASSERT_EQUAL_STRING("Ginbot", ethiopian_month_name(9));
}

void test_month_name_sene(void)
{
    TEST_ASSERT_EQUAL_STRING("Sene", ethiopian_month_name(10));
}

void test_month_name_hamle(void)
{
    TEST_ASSERT_EQUAL_STRING("Hamle", ethiopian_month_name(11));
}

void test_month_name_nehase(void)
{
    TEST_ASSERT_EQUAL_STRING("Nehase", ethiopian_month_name(12));
}

void test_month_name_pagume(void)
{
    TEST_ASSERT_EQUAL_STRING("Pagume", ethiopian_month_name(13));
}

void test_month_name_invalid_zero(void)
{
    TEST_ASSERT_EQUAL_STRING("?", ethiopian_month_name(0));
}

void test_month_name_invalid_14(void)
{
    TEST_ASSERT_EQUAL_STRING("?", ethiopian_month_name(14));
}

void test_month_name_invalid_negative(void)
{
    TEST_ASSERT_EQUAL_STRING("?", ethiopian_month_name(-1));
}

/* ===== Feast detection ===== */

void test_feast_enkutatash(void)
{
    /* Enkutatash (New Year): 1 Meskerem */
    ethiopian_date_t d = { .year = 2016, .month = 1, .day = 1 };
    TEST_ASSERT_EQUAL_INT(ETH_FEAST_ENKUTATASH, ethiopian_feast(d));
}

void test_feast_meskel(void)
{
    /* Meskel (Finding of True Cross): 17 Meskerem */
    ethiopian_date_t d = { .year = 2016, .month = 1, .day = 17 };
    TEST_ASSERT_EQUAL_INT(ETH_FEAST_MESKEL, ethiopian_feast(d));
}

void test_feast_genna(void)
{
    /* Genna (Christmas): 29 Tahsas (month 4) */
    ethiopian_date_t d = { .year = 2016, .month = 4, .day = 29 };
    TEST_ASSERT_EQUAL_INT(ETH_FEAST_GENNA, ethiopian_feast(d));
}

void test_feast_timkat(void)
{
    /* Timkat (Epiphany): 11 Tir (month 5) */
    ethiopian_date_t d = { .year = 2016, .month = 5, .day = 11 };
    TEST_ASSERT_EQUAL_INT(ETH_FEAST_TIMKAT, ethiopian_feast(d));
}

void test_feast_none_normal_day(void)
{
    /* Normal day, no feast */
    ethiopian_date_t d = { .year = 2016, .month = 3, .day = 15 };
    TEST_ASSERT_EQUAL_INT(ETH_FEAST_NONE, ethiopian_feast(d));
}

void test_feast_none_pagume(void)
{
    /* Pagume day, no feast */
    ethiopian_date_t d = { .year = 2016, .month = 13, .day = 3 };
    TEST_ASSERT_EQUAL_INT(ETH_FEAST_NONE, ethiopian_feast(d));
}

void test_feast_none_day2_meskerem(void)
{
    /* Day 2 of Meskerem — not a feast */
    ethiopian_date_t d = { .year = 2016, .month = 1, .day = 2 };
    TEST_ASSERT_EQUAL_INT(ETH_FEAST_NONE, ethiopian_feast(d));
}

/* ===== Feast names ===== */

void test_feast_name_enkutatash(void)
{
    TEST_ASSERT_EQUAL_STRING("Enkutatash",
                             ethiopian_feast_name(ETH_FEAST_ENKUTATASH));
}

void test_feast_name_meskel(void)
{
    TEST_ASSERT_EQUAL_STRING("Meskel",
                             ethiopian_feast_name(ETH_FEAST_MESKEL));
}

void test_feast_name_genna(void)
{
    TEST_ASSERT_EQUAL_STRING("Genna",
                             ethiopian_feast_name(ETH_FEAST_GENNA));
}

void test_feast_name_timkat(void)
{
    TEST_ASSERT_EQUAL_STRING("Timkat",
                             ethiopian_feast_name(ETH_FEAST_TIMKAT));
}

void test_feast_name_fasika(void)
{
    /* Fasika is movable — name exists but feast detection won't produce it */
    TEST_ASSERT_EQUAL_STRING("Fasika",
                             ethiopian_feast_name(ETH_FEAST_FASIKA));
}

void test_feast_name_none(void)
{
    TEST_ASSERT_EQUAL_STRING("", ethiopian_feast_name(ETH_FEAST_NONE));
}

void test_feast_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", ethiopian_feast_name(ETH_FEAST_COUNT));
    TEST_ASSERT_EQUAL_STRING("?", ethiopian_feast_name(-1));
}

/* ===== Roundtrip tests ===== */

void test_roundtrip_epoch(void)
{
    ethiopian_date_t orig = { .year = 1, .month = 1, .day = 1 };
    double jd = ethiopian_to_jd(orig);
    ethiopian_date_t back = ethiopian_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(orig.year, back.year);
    TEST_ASSERT_EQUAL_INT(orig.month, back.month);
    TEST_ASSERT_EQUAL_INT(orig.day, back.day);
}

void test_roundtrip_pagume(void)
{
    ethiopian_date_t orig = { .year = 500, .month = 13, .day = 3 };
    double jd = ethiopian_to_jd(orig);
    ethiopian_date_t back = ethiopian_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(orig.year, back.year);
    TEST_ASSERT_EQUAL_INT(orig.month, back.month);
    TEST_ASSERT_EQUAL_INT(orig.day, back.day);
}

void test_roundtrip_mid_year(void)
{
    ethiopian_date_t orig = { .year = 1000, .month = 7, .day = 15 };
    double jd = ethiopian_to_jd(orig);
    ethiopian_date_t back = ethiopian_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(orig.year, back.year);
    TEST_ASSERT_EQUAL_INT(orig.month, back.month);
    TEST_ASSERT_EQUAL_INT(orig.day, back.day);
}

void test_roundtrip_last_regular_day(void)
{
    ethiopian_date_t orig = { .year = 2016, .month = 12, .day = 30 };
    double jd = ethiopian_to_jd(orig);
    ethiopian_date_t back = ethiopian_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(orig.year, back.year);
    TEST_ASSERT_EQUAL_INT(orig.month, back.month);
    TEST_ASSERT_EQUAL_INT(orig.day, back.day);
}

void test_roundtrip_leap_pagume6(void)
{
    /* Year 3 is leap -> Pagume day 6 must roundtrip */
    ethiopian_date_t orig = { .year = 3, .month = 13, .day = 6 };
    double jd = ethiopian_to_jd(orig);
    ethiopian_date_t back = ethiopian_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(orig.year, back.year);
    TEST_ASSERT_EQUAL_INT(orig.month, back.month);
    TEST_ASSERT_EQUAL_INT(orig.day, back.day);
}

void test_roundtrip_large_year(void)
{
    ethiopian_date_t orig = { .year = 2018, .month = 9, .day = 22 };
    double jd = ethiopian_to_jd(orig);
    ethiopian_date_t back = ethiopian_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(orig.year, back.year);
    TEST_ASSERT_EQUAL_INT(orig.month, back.month);
    TEST_ASSERT_EQUAL_INT(orig.day, back.day);
}

void test_roundtrip_first_month(void)
{
    ethiopian_date_t orig = { .year = 1500, .month = 1, .day = 30 };
    double jd = ethiopian_to_jd(orig);
    ethiopian_date_t back = ethiopian_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(orig.year, back.year);
    TEST_ASSERT_EQUAL_INT(orig.month, back.month);
    TEST_ASSERT_EQUAL_INT(orig.day, back.day);
}

void test_roundtrip_year7_leap(void)
{
    /* Year 7 is leap (7%4==3) */
    ethiopian_date_t orig = { .year = 7, .month = 13, .day = 6 };
    double jd = ethiopian_to_jd(orig);
    ethiopian_date_t back = ethiopian_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(orig.year, back.year);
    TEST_ASSERT_EQUAL_INT(orig.month, back.month);
    TEST_ASSERT_EQUAL_INT(orig.day, back.day);
}

/* ===== Day of year ===== */

void test_day_of_year_first(void)
{
    ethiopian_date_t d = { .year = 1, .month = 1, .day = 1 };
    TEST_ASSERT_EQUAL_INT(1, ethiopian_day_of_year(d));
}

void test_day_of_year_month2(void)
{
    /* month 2, day 1 = day 31 */
    ethiopian_date_t d = { .year = 1, .month = 2, .day = 1 };
    TEST_ASSERT_EQUAL_INT(31, ethiopian_day_of_year(d));
}

void test_day_of_year_last_regular(void)
{
    /* month 12, day 30 = day 360 */
    ethiopian_date_t d = { .year = 1, .month = 12, .day = 30 };
    TEST_ASSERT_EQUAL_INT(360, ethiopian_day_of_year(d));
}

void test_day_of_year_pagume1(void)
{
    /* month 13, day 1 = day 361 */
    ethiopian_date_t d = { .year = 1, .month = 13, .day = 1 };
    TEST_ASSERT_EQUAL_INT(361, ethiopian_day_of_year(d));
}

void test_day_of_year_pagume5(void)
{
    /* month 13, day 5 = day 365 */
    ethiopian_date_t d = { .year = 1, .month = 13, .day = 5 };
    TEST_ASSERT_EQUAL_INT(365, ethiopian_day_of_year(d));
}

void test_day_of_year_pagume6_leap(void)
{
    /* month 13, day 6 in a leap year = day 366 */
    ethiopian_date_t d = { .year = 3, .month = 13, .day = 6 };
    TEST_ASSERT_EQUAL_INT(366, ethiopian_day_of_year(d));
}

void test_day_of_year_mid_month6(void)
{
    /* month 6, day 15 = 5*30 + 15 = 165 */
    ethiopian_date_t d = { .year = 1, .month = 6, .day = 15 };
    TEST_ASSERT_EQUAL_INT(165, ethiopian_day_of_year(d));
}

/* ===== ethiopian_year convenience ===== */

void test_ethiopian_year_epoch(void)
{
    TEST_ASSERT_EQUAL_INT(1, ethiopian_year(1724220.5));
}

void test_ethiopian_year_modern(void)
{
    /* Year 2016, 1 Meskerem */
    ethiopian_date_t d = { .year = 2016, .month = 1, .day = 1 };
    double jd = ethiopian_to_jd(d);
    TEST_ASSERT_EQUAL_INT(2016, ethiopian_year(jd));
}

/* ===== Mid-month and boundary tests ===== */

void test_mid_year_date(void)
{
    /* Year 1, day 31 = month 2, day 1 (Tikimt) */
    ethiopian_date_t d = ethiopian_from_jd(1724220.5 + 30.0);
    TEST_ASSERT_EQUAL_INT(1, d.year);
    TEST_ASSERT_EQUAL_INT(2, d.month);
    TEST_ASSERT_EQUAL_INT(1, d.day);
}

void test_pagume_day1(void)
{
    /* Year 1: day 361 = first Pagume day (month 13, day 1)
     * JD = 1724220.5 + 360 */
    ethiopian_date_t d = ethiopian_from_jd(1724220.5 + 360.0);
    TEST_ASSERT_EQUAL_INT(1, d.year);
    TEST_ASSERT_EQUAL_INT(13, d.month);
    TEST_ASSERT_EQUAL_INT(1, d.day);
}

/* ===== Cross-validation with Coptic ===== */

void test_epoch_offset_from_coptic(void)
{
    /* Ethiopian epoch (JD 1724220.5) is exactly 100809 days before
     * Coptic epoch (JD 1825029.5). 100809 = 276 * 365.25 */
    int diff = 1825029 - 1724220;
    TEST_ASSERT_EQUAL_INT(100809, diff);
}

/* ===== Sequential day progression ===== */

void test_sequential_days_month_boundary(void)
{
    /* Last day of month 6 -> first day of month 7 */
    ethiopian_date_t d1 = { .year = 100, .month = 6, .day = 30 };
    ethiopian_date_t d2 = { .year = 100, .month = 7, .day = 1 };
    double diff = ethiopian_to_jd(d2) - ethiopian_to_jd(d1);
    TEST_ASSERT_TRUE(diff > 0.999 && diff < 1.001);
}

void test_sequential_days_year_boundary_common(void)
{
    /* Last day of common year 1 -> first day of year 2 */
    ethiopian_date_t d1 = { .year = 1, .month = 13, .day = 5 };
    ethiopian_date_t d2 = { .year = 2, .month = 1, .day = 1 };
    double diff = ethiopian_to_jd(d2) - ethiopian_to_jd(d1);
    TEST_ASSERT_TRUE(diff > 0.999 && diff < 1.001);
}

void test_sequential_days_year_boundary_leap(void)
{
    /* Last day of leap year 7 -> first day of year 8 */
    ethiopian_date_t d1 = { .year = 7, .month = 13, .day = 6 };
    ethiopian_date_t d2 = { .year = 8, .month = 1, .day = 1 };
    double diff = ethiopian_to_jd(d2) - ethiopian_to_jd(d1);
    TEST_ASSERT_TRUE(diff > 0.999 && diff < 1.001);
}

/* ===== 4-year cycle completeness ===== */

void test_four_year_cycle_days(void)
{
    /* Years 1-4 should total 1461 days (3*365 + 366)
     * Year 1: 365, Year 2: 365, Year 3: 366, Year 4: 365 */
    int total = ethiopian_year_days(1) + ethiopian_year_days(2)
              + ethiopian_year_days(3) + ethiopian_year_days(4);
    TEST_ASSERT_EQUAL_INT(1461, total);
}

void test_four_year_cycle_jd_span(void)
{
    /* JD span from year 1 day 1 to year 5 day 1 = 1461 days */
    ethiopian_date_t start = { .year = 1, .month = 1, .day = 1 };
    ethiopian_date_t end = { .year = 5, .month = 1, .day = 1 };
    double span = ethiopian_to_jd(end) - ethiopian_to_jd(start);
    TEST_ASSERT_TRUE(span > 1460.999 && span < 1461.001);
}

/* ===== Runner ===== */

int main(void)
{
    UNITY_BEGIN();

    /* Epoch and basic conversions */
    RUN_TEST(test_epoch_to_jd);
    RUN_TEST(test_epoch_from_jd);
    RUN_TEST(test_last_day_of_year1);
    RUN_TEST(test_last_day_of_year1_to_jd);
    RUN_TEST(test_first_day_of_year2);
    RUN_TEST(test_first_day_of_year2_to_jd);
    RUN_TEST(test_mid_year_date);
    RUN_TEST(test_pagume_day1);

    /* Leap year detection */
    RUN_TEST(test_leap_year_3);
    RUN_TEST(test_leap_year_4);
    RUN_TEST(test_leap_year_7);
    RUN_TEST(test_leap_year_1);
    RUN_TEST(test_leap_year_11);
    RUN_TEST(test_leap_year_2);
    RUN_TEST(test_leap_year_2015);
    RUN_TEST(test_leap_year_2016);

    /* Month days */
    RUN_TEST(test_month_days_regular);
    RUN_TEST(test_month_days_pagume_common);
    RUN_TEST(test_month_days_pagume_leap);
    RUN_TEST(test_month_days_invalid_zero);
    RUN_TEST(test_month_days_invalid_14);
    RUN_TEST(test_month_days_invalid_negative);

    /* Year days */
    RUN_TEST(test_year_days_common);
    RUN_TEST(test_year_days_leap);

    /* Leap boundary */
    RUN_TEST(test_year3_last_day_pagume6);
    RUN_TEST(test_year4_first_day_after_leap);

    /* Month names */
    RUN_TEST(test_month_name_meskerem);
    RUN_TEST(test_month_name_tikimt);
    RUN_TEST(test_month_name_hidar);
    RUN_TEST(test_month_name_tahsas);
    RUN_TEST(test_month_name_tir);
    RUN_TEST(test_month_name_yekatit);
    RUN_TEST(test_month_name_megabit);
    RUN_TEST(test_month_name_miyazia);
    RUN_TEST(test_month_name_ginbot);
    RUN_TEST(test_month_name_sene);
    RUN_TEST(test_month_name_hamle);
    RUN_TEST(test_month_name_nehase);
    RUN_TEST(test_month_name_pagume);
    RUN_TEST(test_month_name_invalid_zero);
    RUN_TEST(test_month_name_invalid_14);
    RUN_TEST(test_month_name_invalid_negative);

    /* Feast detection */
    RUN_TEST(test_feast_enkutatash);
    RUN_TEST(test_feast_meskel);
    RUN_TEST(test_feast_genna);
    RUN_TEST(test_feast_timkat);
    RUN_TEST(test_feast_none_normal_day);
    RUN_TEST(test_feast_none_pagume);
    RUN_TEST(test_feast_none_day2_meskerem);

    /* Feast names */
    RUN_TEST(test_feast_name_enkutatash);
    RUN_TEST(test_feast_name_meskel);
    RUN_TEST(test_feast_name_genna);
    RUN_TEST(test_feast_name_timkat);
    RUN_TEST(test_feast_name_fasika);
    RUN_TEST(test_feast_name_none);
    RUN_TEST(test_feast_name_invalid);

    /* Roundtrips */
    RUN_TEST(test_roundtrip_epoch);
    RUN_TEST(test_roundtrip_pagume);
    RUN_TEST(test_roundtrip_mid_year);
    RUN_TEST(test_roundtrip_last_regular_day);
    RUN_TEST(test_roundtrip_leap_pagume6);
    RUN_TEST(test_roundtrip_large_year);
    RUN_TEST(test_roundtrip_first_month);
    RUN_TEST(test_roundtrip_year7_leap);

    /* Day of year */
    RUN_TEST(test_day_of_year_first);
    RUN_TEST(test_day_of_year_month2);
    RUN_TEST(test_day_of_year_last_regular);
    RUN_TEST(test_day_of_year_pagume1);
    RUN_TEST(test_day_of_year_pagume5);
    RUN_TEST(test_day_of_year_pagume6_leap);
    RUN_TEST(test_day_of_year_mid_month6);

    /* Convenience functions */
    RUN_TEST(test_ethiopian_year_epoch);
    RUN_TEST(test_ethiopian_year_modern);

    /* Cross-validation */
    RUN_TEST(test_epoch_offset_from_coptic);

    /* Sequential day progression */
    RUN_TEST(test_sequential_days_month_boundary);
    RUN_TEST(test_sequential_days_year_boundary_common);
    RUN_TEST(test_sequential_days_year_boundary_leap);

    /* 4-year cycle */
    RUN_TEST(test_four_year_cycle_days);
    RUN_TEST(test_four_year_cycle_jd_span);

    return UNITY_END();
}
