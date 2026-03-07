/* test_myanmar.c — Myanmar (Burmese) lunisolar calendar tests
 * TDD RED phase: tests written before implementation.
 *
 * Myanmar Era (ME) epoch: 22 March 638 CE (Julian), JD ~1954168.050623
 * Solar year (Surya Siddhanta) = 1577917828 / 4320000 ~= 365.2587565 days
 * Three year types: common (354), small watat (384), big watat (385)
 * Watat years in 19-year cycle at positions: 2, 5, 7, 10, 13, 15, 18
 * Big watat at positions: 7, 18
 *
 * Algorithm: Yan Naing Aye, Myanmar Calendar Algorithm */

#include "../../unity/unity.h"
#include "../../../src/systems/myanmar/myanmar.h"
#include <math.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== Year type detection ===== */

void test_year_type_common_year_0(void)
{
    /* ME 0: 0 % 19 = 0, not in watat set -> common */
    TEST_ASSERT_EQUAL_INT(MY_YEAR_COMMON, myanmar_year_type(0));
}

void test_year_type_common_year_1(void)
{
    /* ME 1: 1 % 19 = 1, not in watat set -> common */
    TEST_ASSERT_EQUAL_INT(MY_YEAR_COMMON, myanmar_year_type(1));
}

void test_year_type_small_watat_pos2(void)
{
    /* ME 2: 2 % 19 = 2, in watat set, not big -> small watat */
    TEST_ASSERT_EQUAL_INT(MY_YEAR_SMALL_WATAT, myanmar_year_type(2));
}

void test_year_type_small_watat_pos5(void)
{
    /* ME 5: 5 % 19 = 5, in watat set, not big -> small watat */
    TEST_ASSERT_EQUAL_INT(MY_YEAR_SMALL_WATAT, myanmar_year_type(5));
}

void test_year_type_big_watat_pos7(void)
{
    /* ME 7: 7 % 19 = 7, in watat set AND big -> big watat */
    TEST_ASSERT_EQUAL_INT(MY_YEAR_BIG_WATAT, myanmar_year_type(7));
}

void test_year_type_small_watat_pos10(void)
{
    /* ME 10: 10 % 19 = 10, in watat set, not big -> small watat */
    TEST_ASSERT_EQUAL_INT(MY_YEAR_SMALL_WATAT, myanmar_year_type(10));
}

void test_year_type_small_watat_pos13(void)
{
    /* ME 13: 13 % 19 = 13, in watat set, not big -> small watat */
    TEST_ASSERT_EQUAL_INT(MY_YEAR_SMALL_WATAT, myanmar_year_type(13));
}

void test_year_type_small_watat_pos15(void)
{
    /* ME 15: 15 % 19 = 15, in watat set, not big -> small watat */
    TEST_ASSERT_EQUAL_INT(MY_YEAR_SMALL_WATAT, myanmar_year_type(15));
}

void test_year_type_big_watat_pos18(void)
{
    /* ME 18: 18 % 19 = 18, in watat set AND big -> big watat */
    TEST_ASSERT_EQUAL_INT(MY_YEAR_BIG_WATAT, myanmar_year_type(18));
}

void test_year_type_common_pos3(void)
{
    /* ME 3: 3 % 19 = 3, not in watat set -> common */
    TEST_ASSERT_EQUAL_INT(MY_YEAR_COMMON, myanmar_year_type(3));
}

void test_year_type_common_pos19(void)
{
    /* ME 19: 19 % 19 = 0, not in watat set -> common (start of new cycle) */
    TEST_ASSERT_EQUAL_INT(MY_YEAR_COMMON, myanmar_year_type(19));
}

void test_year_type_second_cycle_pos2(void)
{
    /* ME 21: 21 % 19 = 2, in watat set -> small watat */
    TEST_ASSERT_EQUAL_INT(MY_YEAR_SMALL_WATAT, myanmar_year_type(21));
}

void test_year_type_second_cycle_big(void)
{
    /* ME 26: 26 % 19 = 7, big watat */
    TEST_ASSERT_EQUAL_INT(MY_YEAR_BIG_WATAT, myanmar_year_type(26));
}

void test_year_type_large_common(void)
{
    /* ME 1380: 1380 % 19 = 1380 - 72*19 = 1380 - 1368 = 12, not in set -> common */
    TEST_ASSERT_EQUAL_INT(MY_YEAR_COMMON, myanmar_year_type(1380));
}

void test_year_type_large_watat(void)
{
    /* ME 1388: 1388 % 19 = 1388 - 73*19 = 1388 - 1387 = 1, not in set -> common */
    TEST_ASSERT_EQUAL_INT(MY_YEAR_COMMON, myanmar_year_type(1388));
}

/* ===== Watat detection ===== */

void test_is_watat_true_small(void)
{
    TEST_ASSERT_TRUE(myanmar_is_watat(2));
}

void test_is_watat_true_big(void)
{
    TEST_ASSERT_TRUE(myanmar_is_watat(7));
}

void test_is_watat_false(void)
{
    TEST_ASSERT_FALSE(myanmar_is_watat(0));
}

void test_is_watat_false_pos4(void)
{
    TEST_ASSERT_FALSE(myanmar_is_watat(4));
}

void test_is_watat_false_pos6(void)
{
    TEST_ASSERT_FALSE(myanmar_is_watat(6));
}

/* ===== Month names ===== */

void test_month_name_tagu(void)
{
    TEST_ASSERT_EQUAL_STRING("Tagu", myanmar_month_name(MY_TAGU));
}

void test_month_name_kason(void)
{
    TEST_ASSERT_EQUAL_STRING("Kason", myanmar_month_name(MY_KASON));
}

void test_month_name_nayon(void)
{
    TEST_ASSERT_EQUAL_STRING("Nayon", myanmar_month_name(MY_NAYON));
}

void test_month_name_waso(void)
{
    TEST_ASSERT_EQUAL_STRING("Waso", myanmar_month_name(MY_WASO));
}

void test_month_name_second_waso(void)
{
    TEST_ASSERT_EQUAL_STRING("Second Waso", myanmar_month_name(MY_SECOND_WASO));
}

void test_month_name_wagaung(void)
{
    TEST_ASSERT_EQUAL_STRING("Wagaung", myanmar_month_name(MY_WAGAUNG));
}

void test_month_name_tawthalin(void)
{
    TEST_ASSERT_EQUAL_STRING("Tawthalin", myanmar_month_name(MY_TAWTHALIN));
}

void test_month_name_thadingyut(void)
{
    TEST_ASSERT_EQUAL_STRING("Thadingyut", myanmar_month_name(MY_THADINGYUT));
}

void test_month_name_tazaungmon(void)
{
    TEST_ASSERT_EQUAL_STRING("Tazaungmon", myanmar_month_name(MY_TAZAUNGMON));
}

void test_month_name_nadaw(void)
{
    TEST_ASSERT_EQUAL_STRING("Nadaw", myanmar_month_name(MY_NADAW));
}

void test_month_name_pyatho(void)
{
    TEST_ASSERT_EQUAL_STRING("Pyatho", myanmar_month_name(MY_PYATHO));
}

void test_month_name_tabodwe(void)
{
    TEST_ASSERT_EQUAL_STRING("Tabodwe", myanmar_month_name(MY_TABODWE));
}

void test_month_name_tabaung(void)
{
    TEST_ASSERT_EQUAL_STRING("Tabaung", myanmar_month_name(MY_TABAUNG));
}

void test_month_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", myanmar_month_name(MY_MONTH_COUNT));
    TEST_ASSERT_EQUAL_STRING("?", myanmar_month_name(-1));
}

/* ===== Year type names ===== */

void test_year_type_name_common(void)
{
    TEST_ASSERT_EQUAL_STRING("Common", myanmar_year_type_name(MY_YEAR_COMMON));
}

void test_year_type_name_small_watat(void)
{
    TEST_ASSERT_EQUAL_STRING("Small Watat", myanmar_year_type_name(MY_YEAR_SMALL_WATAT));
}

void test_year_type_name_big_watat(void)
{
    TEST_ASSERT_EQUAL_STRING("Big Watat", myanmar_year_type_name(MY_YEAR_BIG_WATAT));
}

void test_year_type_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", myanmar_year_type_name(MY_YEAR_TYPE_COUNT));
    TEST_ASSERT_EQUAL_STRING("?", myanmar_year_type_name(-1));
}

/* ===== Month days ===== */

void test_month_days_tagu_common(void)
{
    /* Tagu always 29 */
    TEST_ASSERT_EQUAL_INT(29, myanmar_month_days(0, MY_TAGU));
}

void test_month_days_kason_common(void)
{
    /* Kason always 30 */
    TEST_ASSERT_EQUAL_INT(30, myanmar_month_days(0, MY_KASON));
}

void test_month_days_nayon_common(void)
{
    /* Nayon 29 in common year */
    TEST_ASSERT_EQUAL_INT(29, myanmar_month_days(0, MY_NAYON));
}

void test_month_days_nayon_big_watat(void)
{
    /* Nayon 30 in big watat year (ME 7, pos 7) */
    TEST_ASSERT_EQUAL_INT(30, myanmar_month_days(7, MY_NAYON));
}

void test_month_days_nayon_small_watat(void)
{
    /* Nayon 29 in small watat year (ME 2, pos 2) */
    TEST_ASSERT_EQUAL_INT(29, myanmar_month_days(2, MY_NAYON));
}

void test_month_days_waso(void)
{
    /* Waso always 30 */
    TEST_ASSERT_EQUAL_INT(30, myanmar_month_days(0, MY_WASO));
}

void test_month_days_second_waso_common(void)
{
    /* Second Waso 0 in common year (skipped) */
    TEST_ASSERT_EQUAL_INT(0, myanmar_month_days(0, MY_SECOND_WASO));
}

void test_month_days_second_waso_watat(void)
{
    /* Second Waso 30 in watat year */
    TEST_ASSERT_EQUAL_INT(30, myanmar_month_days(2, MY_SECOND_WASO));
}

void test_month_days_second_waso_big_watat(void)
{
    /* Second Waso 30 in big watat year */
    TEST_ASSERT_EQUAL_INT(30, myanmar_month_days(7, MY_SECOND_WASO));
}

void test_month_days_wagaung(void)
{
    TEST_ASSERT_EQUAL_INT(29, myanmar_month_days(0, MY_WAGAUNG));
}

void test_month_days_tawthalin(void)
{
    TEST_ASSERT_EQUAL_INT(30, myanmar_month_days(0, MY_TAWTHALIN));
}

void test_month_days_thadingyut(void)
{
    TEST_ASSERT_EQUAL_INT(29, myanmar_month_days(0, MY_THADINGYUT));
}

void test_month_days_tazaungmon(void)
{
    TEST_ASSERT_EQUAL_INT(30, myanmar_month_days(0, MY_TAZAUNGMON));
}

void test_month_days_nadaw(void)
{
    TEST_ASSERT_EQUAL_INT(29, myanmar_month_days(0, MY_NADAW));
}

void test_month_days_pyatho(void)
{
    TEST_ASSERT_EQUAL_INT(30, myanmar_month_days(0, MY_PYATHO));
}

void test_month_days_tabodwe(void)
{
    TEST_ASSERT_EQUAL_INT(29, myanmar_month_days(0, MY_TABODWE));
}

void test_month_days_tabaung(void)
{
    TEST_ASSERT_EQUAL_INT(30, myanmar_month_days(0, MY_TABAUNG));
}

void test_month_days_invalid(void)
{
    TEST_ASSERT_EQUAL_INT(0, myanmar_month_days(0, MY_MONTH_COUNT));
    TEST_ASSERT_EQUAL_INT(0, myanmar_month_days(0, -1));
}

/* ===== Year days ===== */

void test_year_days_common(void)
{
    /* Common year: 354 days */
    TEST_ASSERT_EQUAL_INT(354, myanmar_year_days(0));
}

void test_year_days_small_watat(void)
{
    /* Small watat year: 384 days (ME 2) */
    TEST_ASSERT_EQUAL_INT(384, myanmar_year_days(2));
}

void test_year_days_big_watat(void)
{
    /* Big watat year: 385 days (ME 7) */
    TEST_ASSERT_EQUAL_INT(385, myanmar_year_days(7));
}

void test_year_days_common_pos1(void)
{
    TEST_ASSERT_EQUAL_INT(354, myanmar_year_days(1));
}

void test_year_days_big_watat_pos18(void)
{
    /* ME 18: pos 18 -> big watat -> 385 */
    TEST_ASSERT_EQUAL_INT(385, myanmar_year_days(18));
}

/* ===== Gregorian year conversion ===== */

void test_to_gregorian_year_me0(void)
{
    /* ME 0 -> ~638 CE */
    TEST_ASSERT_EQUAL_INT(638, myanmar_to_gregorian_year(0));
}

void test_to_gregorian_year_me1(void)
{
    TEST_ASSERT_EQUAL_INT(639, myanmar_to_gregorian_year(1));
}

void test_to_gregorian_year_me1000(void)
{
    /* ME 1000 -> ~1638 CE */
    TEST_ASSERT_EQUAL_INT(1638, myanmar_to_gregorian_year(1000));
}

void test_to_gregorian_year_me1380(void)
{
    /* ME 1380 -> ~2018 CE */
    TEST_ASSERT_EQUAL_INT(2018, myanmar_to_gregorian_year(1380));
}

/* ===== Thingyan (water festival) ===== */

void test_thingyan_num_days(void)
{
    /* Thingyan lasts 4 days by default */
    myanmar_thingyan_t t = myanmar_thingyan(1380);
    TEST_ASSERT_EQUAL_INT(4, t.num_days);
}

void test_thingyan_akyo_before_new_year(void)
{
    /* akyo_jd should be 4 days before new year */
    myanmar_thingyan_t t = myanmar_thingyan(1380);
    double diff = t.new_year_jd - t.akyo_jd;
    TEST_ASSERT_TRUE(diff > 3.99 && diff < 4.01);
}

void test_thingyan_akya_before_new_year(void)
{
    /* akya_jd should be 1 day before new year */
    myanmar_thingyan_t t = myanmar_thingyan(1380);
    double diff = t.new_year_jd - t.akya_jd;
    TEST_ASSERT_TRUE(diff > 0.99 && diff < 1.01);
}

void test_thingyan_new_year_jd_reasonable(void)
{
    /* ME 1380 New Year should be around April 2018
     * April 17, 2018 Greg -> JD ~2458224.5
     * Allow generous tolerance since this is approximate */
    myanmar_thingyan_t t = myanmar_thingyan(1380);
    TEST_ASSERT_TRUE(t.new_year_jd > 2458220.0 && t.new_year_jd < 2458230.0);
}

void test_thingyan_year_0(void)
{
    /* ME 0: new year JD should be near epoch */
    myanmar_thingyan_t t = myanmar_thingyan(0);
    TEST_ASSERT_TRUE(t.new_year_jd > 1954167.0 && t.new_year_jd < 1954169.0);
}

void test_thingyan_year_1(void)
{
    /* ME 1: new year should be ~365.26 days after ME 0 new year */
    myanmar_thingyan_t t0 = myanmar_thingyan(0);
    myanmar_thingyan_t t1 = myanmar_thingyan(1);
    double diff = t1.new_year_jd - t0.new_year_jd;
    TEST_ASSERT_TRUE(diff > 365.0 && diff < 366.0);
}

/* ===== Myanmar to JD conversion ===== */

void test_to_jd_first_day_me0(void)
{
    /* ME 0, 1 Tagu = New Year JD for ME 0 */
    myanmar_date_t d = { .year = 0, .month = MY_TAGU, .day = 1,
                         .is_watat_year = false };
    double jd = myanmar_to_jd(d);
    myanmar_thingyan_t t = myanmar_thingyan(0);
    double diff = jd - t.new_year_jd;
    TEST_ASSERT_TRUE(diff > -0.01 && diff < 0.01);
}

void test_to_jd_second_day_me0(void)
{
    /* ME 0, 2 Tagu = New Year JD + 1 */
    myanmar_date_t d = { .year = 0, .month = MY_TAGU, .day = 2,
                         .is_watat_year = false };
    double jd = myanmar_to_jd(d);
    myanmar_thingyan_t t = myanmar_thingyan(0);
    double diff = jd - t.new_year_jd;
    TEST_ASSERT_TRUE(diff > 0.99 && diff < 1.01);
}

void test_to_jd_kason_1(void)
{
    /* ME 0, 1 Kason = New Year + 29 days (Tagu has 29 days) */
    myanmar_date_t d = { .year = 0, .month = MY_KASON, .day = 1,
                         .is_watat_year = false };
    double jd = myanmar_to_jd(d);
    myanmar_thingyan_t t = myanmar_thingyan(0);
    double diff = jd - t.new_year_jd;
    TEST_ASSERT_TRUE(diff > 28.99 && diff < 29.01);
}

/* ===== JD to Myanmar conversion ===== */

void test_from_jd_epoch_area(void)
{
    /* JD near epoch should give ME year near 0 */
    myanmar_date_t d = myanmar_from_jd(1954168.050623);
    TEST_ASSERT_TRUE(d.year >= 0 && d.year <= 1);
}

void test_from_jd_month_range(void)
{
    /* Any valid JD should give a valid month */
    myanmar_date_t d = myanmar_from_jd(2451545.0);
    TEST_ASSERT_TRUE(d.month >= MY_TAGU && d.month < MY_MONTH_COUNT);
}

void test_from_jd_day_range(void)
{
    /* Day should be 1-30 */
    myanmar_date_t d = myanmar_from_jd(2451545.0);
    TEST_ASSERT_TRUE(d.day >= 1 && d.day <= 30);
}

/* ===== Roundtrip tests ===== */

void test_roundtrip_common_year_tagu(void)
{
    /* ME 0 (common), 1 Tagu */
    myanmar_date_t orig = { .year = 0, .month = MY_TAGU, .day = 1,
                            .is_watat_year = false };
    double jd = myanmar_to_jd(orig);
    myanmar_date_t back = myanmar_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(orig.year, back.year);
    TEST_ASSERT_EQUAL_INT(orig.month, back.month);
    TEST_ASSERT_EQUAL_INT(orig.day, back.day);
}

void test_roundtrip_common_year_tabaung(void)
{
    /* ME 0, 15 Tabaung (last month) */
    myanmar_date_t orig = { .year = 0, .month = MY_TABAUNG, .day = 15,
                            .is_watat_year = false };
    double jd = myanmar_to_jd(orig);
    myanmar_date_t back = myanmar_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(orig.year, back.year);
    TEST_ASSERT_EQUAL_INT(orig.month, back.month);
    TEST_ASSERT_EQUAL_INT(orig.day, back.day);
}

void test_roundtrip_small_watat_second_waso(void)
{
    /* ME 2 (small watat), 15 Second Waso */
    myanmar_date_t orig = { .year = 2, .month = MY_SECOND_WASO, .day = 15,
                            .is_watat_year = true };
    double jd = myanmar_to_jd(orig);
    myanmar_date_t back = myanmar_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(orig.year, back.year);
    TEST_ASSERT_EQUAL_INT(MY_SECOND_WASO, back.month);
    TEST_ASSERT_EQUAL_INT(15, back.day);
}

void test_roundtrip_big_watat_nayon_30(void)
{
    /* ME 7 (big watat), 30 Nayon (extra day) */
    myanmar_date_t orig = { .year = 7, .month = MY_NAYON, .day = 30,
                            .is_watat_year = true };
    double jd = myanmar_to_jd(orig);
    myanmar_date_t back = myanmar_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(7, back.year);
    TEST_ASSERT_EQUAL_INT(MY_NAYON, back.month);
    TEST_ASSERT_EQUAL_INT(30, back.day);
}

void test_roundtrip_mid_year(void)
{
    /* ME 100, 15 Thadingyut */
    myanmar_date_t orig = { .year = 100, .month = MY_THADINGYUT, .day = 15,
                            .is_watat_year = false };
    double jd = myanmar_to_jd(orig);
    myanmar_date_t back = myanmar_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(100, back.year);
    TEST_ASSERT_EQUAL_INT(MY_THADINGYUT, back.month);
    TEST_ASSERT_EQUAL_INT(15, back.day);
}

void test_roundtrip_large_year(void)
{
    /* ME 1380, 10 Pyatho */
    myanmar_date_t orig = { .year = 1380, .month = MY_PYATHO, .day = 10,
                            .is_watat_year = false };
    double jd = myanmar_to_jd(orig);
    myanmar_date_t back = myanmar_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(1380, back.year);
    TEST_ASSERT_EQUAL_INT(MY_PYATHO, back.month);
    TEST_ASSERT_EQUAL_INT(10, back.day);
}

void test_roundtrip_watat_wagaung(void)
{
    /* ME 2 (small watat), 20 Wagaung — after Second Waso */
    myanmar_date_t orig = { .year = 2, .month = MY_WAGAUNG, .day = 20,
                            .is_watat_year = true };
    double jd = myanmar_to_jd(orig);
    myanmar_date_t back = myanmar_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(2, back.year);
    TEST_ASSERT_EQUAL_INT(MY_WAGAUNG, back.month);
    TEST_ASSERT_EQUAL_INT(20, back.day);
}

/* ===== Day of year ===== */

void test_day_of_year_first(void)
{
    myanmar_date_t d = { .year = 0, .month = MY_TAGU, .day = 1,
                         .is_watat_year = false };
    TEST_ASSERT_EQUAL_INT(1, myanmar_day_of_year(d));
}

void test_day_of_year_kason_1(void)
{
    /* Tagu = 29 days, so Kason 1 = day 30 */
    myanmar_date_t d = { .year = 0, .month = MY_KASON, .day = 1,
                         .is_watat_year = false };
    TEST_ASSERT_EQUAL_INT(30, myanmar_day_of_year(d));
}

void test_day_of_year_last_common(void)
{
    /* Common year, last day = 30 Tabaung = day 354 */
    myanmar_date_t d = { .year = 0, .month = MY_TABAUNG, .day = 30,
                         .is_watat_year = false };
    TEST_ASSERT_EQUAL_INT(354, myanmar_day_of_year(d));
}

void test_day_of_year_big_watat_last(void)
{
    /* Big watat year, last day = 30 Tabaung = day 385 */
    myanmar_date_t d = { .year = 7, .month = MY_TABAUNG, .day = 30,
                         .is_watat_year = true };
    TEST_ASSERT_EQUAL_INT(385, myanmar_day_of_year(d));
}

void test_day_of_year_small_watat_last(void)
{
    /* Small watat year, last day = 30 Tabaung = day 384 */
    myanmar_date_t d = { .year = 2, .month = MY_TABAUNG, .day = 30,
                         .is_watat_year = true };
    TEST_ASSERT_EQUAL_INT(384, myanmar_day_of_year(d));
}

/* ===== myanmar_year convenience ===== */

void test_myanmar_year_epoch(void)
{
    /* JD near epoch -> ME ~0 */
    int y = myanmar_year(1954168.050623);
    TEST_ASSERT_TRUE(y >= 0 && y <= 1);
}

void test_myanmar_year_j2000(void)
{
    /* JD 2451545.0 (1 Jan 2000) -> ME ~1361 */
    int y = myanmar_year(2451545.0);
    TEST_ASSERT_TRUE(y >= 1360 && y <= 1362);
}

/* ===== Watat year flag in from_jd ===== */

void test_from_jd_watat_flag_common(void)
{
    /* A date in a common year should have is_watat_year = false */
    /* ME 0 is common (0 % 19 = 0, not in watat set) */
    myanmar_thingyan_t t = myanmar_thingyan(0);
    myanmar_date_t d = myanmar_from_jd(t.new_year_jd + 10.0);
    TEST_ASSERT_FALSE(d.is_watat_year);
}

void test_from_jd_watat_flag_watat(void)
{
    /* A date in a watat year should have is_watat_year = true */
    /* ME 2 is small watat */
    myanmar_thingyan_t t = myanmar_thingyan(2);
    myanmar_date_t d = myanmar_from_jd(t.new_year_jd + 10.0);
    TEST_ASSERT_TRUE(d.is_watat_year);
}

/* ===== Additional edge cases ===== */

void test_month_days_all_common_sum(void)
{
    /* Sum of all month days in a common year should be 354 */
    int total = 0;
    for (int m = MY_TAGU; m < MY_MONTH_COUNT; m++) {
        total += myanmar_month_days(0, m);
    }
    TEST_ASSERT_EQUAL_INT(354, total);
}

void test_month_days_all_small_watat_sum(void)
{
    /* Sum of all month days in small watat should be 384 */
    int total = 0;
    for (int m = MY_TAGU; m < MY_MONTH_COUNT; m++) {
        total += myanmar_month_days(2, m);
    }
    TEST_ASSERT_EQUAL_INT(384, total);
}

void test_month_days_all_big_watat_sum(void)
{
    /* Sum of all month days in big watat should be 385 */
    int total = 0;
    for (int m = MY_TAGU; m < MY_MONTH_COUNT; m++) {
        total += myanmar_month_days(7, m);
    }
    TEST_ASSERT_EQUAL_INT(385, total);
}

void test_year_type_count_in_cycle(void)
{
    /* In a 19-year cycle, there should be exactly 7 watat years */
    int watat_count = 0;
    for (int y = 0; y < 19; y++) {
        if (myanmar_is_watat(y)) {
            watat_count++;
        }
    }
    TEST_ASSERT_EQUAL_INT(7, watat_count);
}

void test_big_watat_count_in_cycle(void)
{
    /* In a 19-year cycle, there should be exactly 2 big watat years */
    int big_count = 0;
    for (int y = 0; y < 19; y++) {
        if (myanmar_year_type(y) == MY_YEAR_BIG_WATAT) {
            big_count++;
        }
    }
    TEST_ASSERT_EQUAL_INT(2, big_count);
}

/* ===== Runner ===== */

int main(void)
{
    UNITY_BEGIN();

    /* Year type detection */
    RUN_TEST(test_year_type_common_year_0);
    RUN_TEST(test_year_type_common_year_1);
    RUN_TEST(test_year_type_small_watat_pos2);
    RUN_TEST(test_year_type_small_watat_pos5);
    RUN_TEST(test_year_type_big_watat_pos7);
    RUN_TEST(test_year_type_small_watat_pos10);
    RUN_TEST(test_year_type_small_watat_pos13);
    RUN_TEST(test_year_type_small_watat_pos15);
    RUN_TEST(test_year_type_big_watat_pos18);
    RUN_TEST(test_year_type_common_pos3);
    RUN_TEST(test_year_type_common_pos19);
    RUN_TEST(test_year_type_second_cycle_pos2);
    RUN_TEST(test_year_type_second_cycle_big);
    RUN_TEST(test_year_type_large_common);
    RUN_TEST(test_year_type_large_watat);

    /* Watat detection */
    RUN_TEST(test_is_watat_true_small);
    RUN_TEST(test_is_watat_true_big);
    RUN_TEST(test_is_watat_false);
    RUN_TEST(test_is_watat_false_pos4);
    RUN_TEST(test_is_watat_false_pos6);

    /* Month names */
    RUN_TEST(test_month_name_tagu);
    RUN_TEST(test_month_name_kason);
    RUN_TEST(test_month_name_nayon);
    RUN_TEST(test_month_name_waso);
    RUN_TEST(test_month_name_second_waso);
    RUN_TEST(test_month_name_wagaung);
    RUN_TEST(test_month_name_tawthalin);
    RUN_TEST(test_month_name_thadingyut);
    RUN_TEST(test_month_name_tazaungmon);
    RUN_TEST(test_month_name_nadaw);
    RUN_TEST(test_month_name_pyatho);
    RUN_TEST(test_month_name_tabodwe);
    RUN_TEST(test_month_name_tabaung);
    RUN_TEST(test_month_name_invalid);

    /* Year type names */
    RUN_TEST(test_year_type_name_common);
    RUN_TEST(test_year_type_name_small_watat);
    RUN_TEST(test_year_type_name_big_watat);
    RUN_TEST(test_year_type_name_invalid);

    /* Month days */
    RUN_TEST(test_month_days_tagu_common);
    RUN_TEST(test_month_days_kason_common);
    RUN_TEST(test_month_days_nayon_common);
    RUN_TEST(test_month_days_nayon_big_watat);
    RUN_TEST(test_month_days_nayon_small_watat);
    RUN_TEST(test_month_days_waso);
    RUN_TEST(test_month_days_second_waso_common);
    RUN_TEST(test_month_days_second_waso_watat);
    RUN_TEST(test_month_days_second_waso_big_watat);
    RUN_TEST(test_month_days_wagaung);
    RUN_TEST(test_month_days_tawthalin);
    RUN_TEST(test_month_days_thadingyut);
    RUN_TEST(test_month_days_tazaungmon);
    RUN_TEST(test_month_days_nadaw);
    RUN_TEST(test_month_days_pyatho);
    RUN_TEST(test_month_days_tabodwe);
    RUN_TEST(test_month_days_tabaung);
    RUN_TEST(test_month_days_invalid);

    /* Year days */
    RUN_TEST(test_year_days_common);
    RUN_TEST(test_year_days_small_watat);
    RUN_TEST(test_year_days_big_watat);
    RUN_TEST(test_year_days_common_pos1);
    RUN_TEST(test_year_days_big_watat_pos18);

    /* Gregorian year conversion */
    RUN_TEST(test_to_gregorian_year_me0);
    RUN_TEST(test_to_gregorian_year_me1);
    RUN_TEST(test_to_gregorian_year_me1000);
    RUN_TEST(test_to_gregorian_year_me1380);

    /* Thingyan */
    RUN_TEST(test_thingyan_num_days);
    RUN_TEST(test_thingyan_akyo_before_new_year);
    RUN_TEST(test_thingyan_akya_before_new_year);
    RUN_TEST(test_thingyan_new_year_jd_reasonable);
    RUN_TEST(test_thingyan_year_0);
    RUN_TEST(test_thingyan_year_1);

    /* Myanmar to JD */
    RUN_TEST(test_to_jd_first_day_me0);
    RUN_TEST(test_to_jd_second_day_me0);
    RUN_TEST(test_to_jd_kason_1);

    /* JD to Myanmar */
    RUN_TEST(test_from_jd_epoch_area);
    RUN_TEST(test_from_jd_month_range);
    RUN_TEST(test_from_jd_day_range);

    /* Roundtrips */
    RUN_TEST(test_roundtrip_common_year_tagu);
    RUN_TEST(test_roundtrip_common_year_tabaung);
    RUN_TEST(test_roundtrip_small_watat_second_waso);
    RUN_TEST(test_roundtrip_big_watat_nayon_30);
    RUN_TEST(test_roundtrip_mid_year);
    RUN_TEST(test_roundtrip_large_year);
    RUN_TEST(test_roundtrip_watat_wagaung);

    /* Day of year */
    RUN_TEST(test_day_of_year_first);
    RUN_TEST(test_day_of_year_kason_1);
    RUN_TEST(test_day_of_year_last_common);
    RUN_TEST(test_day_of_year_big_watat_last);
    RUN_TEST(test_day_of_year_small_watat_last);

    /* Myanmar year convenience */
    RUN_TEST(test_myanmar_year_epoch);
    RUN_TEST(test_myanmar_year_j2000);

    /* Watat flag */
    RUN_TEST(test_from_jd_watat_flag_common);
    RUN_TEST(test_from_jd_watat_flag_watat);

    /* Structural checks */
    RUN_TEST(test_month_days_all_common_sum);
    RUN_TEST(test_month_days_all_small_watat_sum);
    RUN_TEST(test_month_days_all_big_watat_sum);
    RUN_TEST(test_year_type_count_in_cycle);
    RUN_TEST(test_big_watat_count_in_cycle);

    return UNITY_END();
}
