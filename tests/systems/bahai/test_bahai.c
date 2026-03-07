/* test_bahai.c — Baha'i (Badi') calendar tests
 * TDD RED phase: tests written before implementation.
 * Calendar: 19 months x 19 days + Ayyam-i-Ha intercalary days.
 * Epoch: Naw-Ruz 1 BE = March 20, 1844 CE = JD 2394645.5 */

#include "../../unity/unity.h"
#include "../../../src/systems/bahai/bahai.h"
#include <math.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== Month name lookup (all 19 + Ayyam-i-Ha + invalid) ===== */

void test_month_name_baha(void)
{
    TEST_ASSERT_EQUAL_STRING("Baha", bahai_month_name(1));
}

void test_month_name_jalal(void)
{
    TEST_ASSERT_EQUAL_STRING("Jalal", bahai_month_name(2));
}

void test_month_name_jamal(void)
{
    TEST_ASSERT_EQUAL_STRING("Jamal", bahai_month_name(3));
}

void test_month_name_azamat(void)
{
    TEST_ASSERT_EQUAL_STRING("Azamat", bahai_month_name(4));
}

void test_month_name_nur(void)
{
    TEST_ASSERT_EQUAL_STRING("Nur", bahai_month_name(5));
}

void test_month_name_rahmat(void)
{
    TEST_ASSERT_EQUAL_STRING("Rahmat", bahai_month_name(6));
}

void test_month_name_kalimat(void)
{
    TEST_ASSERT_EQUAL_STRING("Kalimat", bahai_month_name(7));
}

void test_month_name_kamal(void)
{
    TEST_ASSERT_EQUAL_STRING("Kamal", bahai_month_name(8));
}

void test_month_name_asma(void)
{
    TEST_ASSERT_EQUAL_STRING("Asma", bahai_month_name(9));
}

void test_month_name_izzat(void)
{
    TEST_ASSERT_EQUAL_STRING("Izzat", bahai_month_name(10));
}

void test_month_name_mashiyyat(void)
{
    TEST_ASSERT_EQUAL_STRING("Mashiyyat", bahai_month_name(11));
}

void test_month_name_ilm(void)
{
    TEST_ASSERT_EQUAL_STRING("Ilm", bahai_month_name(12));
}

void test_month_name_qudrat(void)
{
    TEST_ASSERT_EQUAL_STRING("Qudrat", bahai_month_name(13));
}

void test_month_name_qawl(void)
{
    TEST_ASSERT_EQUAL_STRING("Qawl", bahai_month_name(14));
}

void test_month_name_masail(void)
{
    TEST_ASSERT_EQUAL_STRING("Masail", bahai_month_name(15));
}

void test_month_name_sharaf(void)
{
    TEST_ASSERT_EQUAL_STRING("Sharaf", bahai_month_name(16));
}

void test_month_name_sultan(void)
{
    TEST_ASSERT_EQUAL_STRING("Sultan", bahai_month_name(17));
}

void test_month_name_mulk(void)
{
    TEST_ASSERT_EQUAL_STRING("Mulk", bahai_month_name(18));
}

void test_month_name_ala(void)
{
    TEST_ASSERT_EQUAL_STRING("Ala", bahai_month_name(19));
}

void test_month_name_ayyam_i_ha(void)
{
    TEST_ASSERT_EQUAL_STRING("Ayyam-i-Ha", bahai_month_name(0));
}

void test_month_name_invalid_negative(void)
{
    TEST_ASSERT_EQUAL_STRING("?", bahai_month_name(-1));
}

void test_month_name_invalid_high(void)
{
    TEST_ASSERT_EQUAL_STRING("?", bahai_month_name(20));
}

/* ===== Month meaning lookup (all 19 + Ayyam-i-Ha + invalid) ===== */

void test_month_meaning_baha(void)
{
    TEST_ASSERT_EQUAL_STRING("Splendour", bahai_month_meaning(1));
}

void test_month_meaning_jalal(void)
{
    TEST_ASSERT_EQUAL_STRING("Glory", bahai_month_meaning(2));
}

void test_month_meaning_nur(void)
{
    TEST_ASSERT_EQUAL_STRING("Light", bahai_month_meaning(5));
}

void test_month_meaning_kamal(void)
{
    TEST_ASSERT_EQUAL_STRING("Perfection", bahai_month_meaning(8));
}

void test_month_meaning_ilm(void)
{
    TEST_ASSERT_EQUAL_STRING("Knowledge", bahai_month_meaning(12));
}

void test_month_meaning_ala(void)
{
    TEST_ASSERT_EQUAL_STRING("Loftiness", bahai_month_meaning(19));
}

void test_month_meaning_ayyam_i_ha(void)
{
    TEST_ASSERT_EQUAL_STRING("", bahai_month_meaning(0));
}

void test_month_meaning_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", bahai_month_meaning(20));
}

/* ===== Leap year verification ===== */

void test_leap_year_4(void)
{
    /* Year 4 BE: Ayyam-i-Ha in GY 1848 (4+1844). 1848 % 4 = 0 => LEAP */
    TEST_ASSERT_TRUE(bahai_is_leap_year(4));
}

void test_leap_year_8(void)
{
    /* Year 8 BE: Ayyam-i-Ha in GY 1852 (8+1844). 1852 % 4 = 0 => LEAP */
    TEST_ASSERT_TRUE(bahai_is_leap_year(8));
}

void test_not_leap_year_1(void)
{
    /* Year 1 BE: Ayyam-i-Ha in GY 1845 (1+1844). 1845 % 4 = 1 => NOT LEAP */
    TEST_ASSERT_FALSE(bahai_is_leap_year(1));
}

void test_not_leap_year_2(void)
{
    /* Year 2 BE: Ayyam-i-Ha in GY 1846 (2+1844). 1846 % 4 = 2 => NOT LEAP */
    TEST_ASSERT_FALSE(bahai_is_leap_year(2));
}

void test_leap_year_century_not_leap(void)
{
    /* Year 56 BE: Ayyam-i-Ha in GY 1900 (56+1844). 1900 % 100 = 0 => NOT LEAP */
    TEST_ASSERT_FALSE(bahai_is_leap_year(56));
}

void test_leap_year_400_cycle(void)
{
    /* Year 156 BE: Ayyam-i-Ha in GY 2000 (156+1844). 2000 % 400 = 0 => LEAP */
    TEST_ASSERT_TRUE(bahai_is_leap_year(156));
}

void test_leap_year_182(void)
{
    /* Year 182 BE: Ayyam-i-Ha in GY 2026 (182+1844). 2026 % 4 = 2 => NOT LEAP */
    TEST_ASSERT_FALSE(bahai_is_leap_year(182));
}

void test_leap_year_180(void)
{
    /* Year 180 BE: Ayyam-i-Ha in GY 2024 (180+1844). 2024 % 4 = 0 => LEAP */
    TEST_ASSERT_TRUE(bahai_is_leap_year(180));
}

/* ===== Month days ===== */

void test_month_days_regular(void)
{
    /* All months 1-18 have 19 days */
    for (int m = 1; m <= 18; m++) {
        TEST_ASSERT_EQUAL_INT(19, bahai_month_days(2, m));
    }
}

void test_month_days_month_19(void)
{
    /* Month 19 always has 19 days */
    TEST_ASSERT_EQUAL_INT(19, bahai_month_days(2, 19));
}

void test_month_days_ayyam_i_ha_common(void)
{
    /* Non-leap year: 4 Ayyam-i-Ha days */
    TEST_ASSERT_EQUAL_INT(4, bahai_month_days(2, 0));
}

void test_month_days_ayyam_i_ha_leap(void)
{
    /* Year 4: Ayyam-i-Ha in GY 1848 (leap) => 5 days */
    TEST_ASSERT_EQUAL_INT(5, bahai_month_days(4, 0));
}

/* ===== Year days ===== */

void test_year_days_common(void)
{
    TEST_ASSERT_EQUAL_INT(365, bahai_year_days(2));
}

void test_year_days_leap(void)
{
    TEST_ASSERT_EQUAL_INT(366, bahai_year_days(4));
}

/* ===== Ayyam-i-Ha days count ===== */

void test_ayyam_days_common(void)
{
    TEST_ASSERT_EQUAL_INT(4, bahai_ayyam_i_ha_days(2));
}

void test_ayyam_days_leap(void)
{
    TEST_ASSERT_EQUAL_INT(5, bahai_ayyam_i_ha_days(4));
}

/* ===== Baha'i -> JD conversion ===== */

void test_to_jd_epoch(void)
{
    /* 1 Baha 1 BE = March 20, 1844 = JD 2394645.5 (Meeus) */
    bahai_date_t d = { .year = 1, .month = 1, .day = 1 };
    double jd = bahai_to_jd(d);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 2394645.5, jd);
}

void test_to_jd_second_day(void)
{
    /* 2 Baha 1 BE = March 21, 1844 = epoch + 1 */
    bahai_date_t d = { .year = 1, .month = 1, .day = 2 };
    double jd = bahai_to_jd(d);
    bahai_date_t d1 = { .year = 1, .month = 1, .day = 1 };
    double jd1 = bahai_to_jd(d1);
    TEST_ASSERT_FLOAT_WITHIN(0.01, jd1 + 1.0, jd);
}

void test_to_jd_month_2_day_1(void)
{
    /* 1 Jalal 1 BE = 20th day of year = epoch + 19 */
    bahai_date_t d = { .year = 1, .month = 2, .day = 1 };
    double jd = bahai_to_jd(d);
    bahai_date_t d1 = { .year = 1, .month = 1, .day = 1 };
    double jd1 = bahai_to_jd(d1);
    TEST_ASSERT_FLOAT_WITHIN(0.01, jd1 + 19.0, jd);
}

void test_to_jd_ayyam_i_ha_day_1(void)
{
    /* Ayyam-i-Ha day 1 = day 343 of year = epoch + 342 */
    bahai_date_t d = { .year = 1, .month = 0, .day = 1 };
    double jd = bahai_to_jd(d);
    bahai_date_t d1 = { .year = 1, .month = 1, .day = 1 };
    double jd1 = bahai_to_jd(d1);
    TEST_ASSERT_FLOAT_WITHIN(0.01, jd1 + 342.0, jd);
}

void test_to_jd_month_19_day_1_leap(void)
{
    /* Month 19, day 1 in leap year 4: after 18*19 + 5 ayyam days = 347 */
    bahai_date_t d = { .year = 4, .month = 19, .day = 1 };
    double jd = bahai_to_jd(d);
    bahai_date_t d1 = { .year = 4, .month = 1, .day = 1 };
    double jd1 = bahai_to_jd(d1);
    TEST_ASSERT_FLOAT_WITHIN(0.01, jd1 + 342.0 + 5.0, jd);
}

void test_to_jd_month_19_day_1_common(void)
{
    /* Month 19, day 1 in common year: after 18*19 + 4 ayyam days = 346 */
    bahai_date_t d = { .year = 2, .month = 19, .day = 1 };
    double jd = bahai_to_jd(d);
    bahai_date_t d1 = { .year = 2, .month = 1, .day = 1 };
    double jd1 = bahai_to_jd(d1);
    TEST_ASSERT_FLOAT_WITHIN(0.01, jd1 + 342.0 + 4.0, jd);
}

void test_to_jd_last_day_of_year_leap(void)
{
    /* 19 Ala 4 BE = last day of leap year (day 366) = naw_ruz_y4 + 365 */
    bahai_date_t d = { .year = 4, .month = 19, .day = 19 };
    double jd = bahai_to_jd(d);
    bahai_date_t d1 = { .year = 4, .month = 1, .day = 1 };
    double jd1 = bahai_to_jd(d1);
    TEST_ASSERT_FLOAT_WITHIN(0.01, jd1 + 365.0, jd);
}

void test_to_jd_last_day_of_year_common(void)
{
    /* 19 Ala 2 BE = last day of common year (day 365) = naw_ruz_y2 + 364 */
    bahai_date_t d = { .year = 2, .month = 19, .day = 19 };
    double jd = bahai_to_jd(d);
    bahai_date_t d1 = { .year = 2, .month = 1, .day = 1 };
    double jd1 = bahai_to_jd(d1);
    TEST_ASSERT_FLOAT_WITHIN(0.01, jd1 + 364.0, jd);
}

/* ===== JD -> Baha'i conversion ===== */

void test_from_jd_epoch(void)
{
    /* JD 2394645.5 = 1 Baha 1 BE (Meeus: March 20, 1844 midnight UT) */
    bahai_date_t d = bahai_from_jd(2394645.5);
    TEST_ASSERT_EQUAL_INT(1, d.year);
    TEST_ASSERT_EQUAL_INT(1, d.month);
    TEST_ASSERT_EQUAL_INT(1, d.day);
}

void test_from_jd_second_day(void)
{
    bahai_date_t d = bahai_from_jd(2394646.5);
    TEST_ASSERT_EQUAL_INT(1, d.year);
    TEST_ASSERT_EQUAL_INT(1, d.month);
    TEST_ASSERT_EQUAL_INT(2, d.day);
}

void test_from_jd_month_boundary(void)
{
    /* Day 20 of year = 1 Jalal (month 2, day 1) */
    bahai_date_t d = bahai_from_jd(2394645.5 + 19.0);
    TEST_ASSERT_EQUAL_INT(1, d.year);
    TEST_ASSERT_EQUAL_INT(2, d.month);
    TEST_ASSERT_EQUAL_INT(1, d.day);
}

void test_from_jd_ayyam_i_ha(void)
{
    /* Day 343 = Ayyam-i-Ha day 1 */
    bahai_date_t d = bahai_from_jd(2394645.5 + 342.0);
    TEST_ASSERT_EQUAL_INT(1, d.year);
    TEST_ASSERT_EQUAL_INT(0, d.month);
    TEST_ASSERT_EQUAL_INT(1, d.day);
}

void test_from_jd_ayyam_i_ha_last_common(void)
{
    /* Year 1 is common (4 Ayyam-i-Ha). Day 346 = Ayyam-i-Ha day 4 */
    bahai_date_t d = bahai_from_jd(2394645.5 + 345.0);
    TEST_ASSERT_EQUAL_INT(1, d.year);
    TEST_ASSERT_EQUAL_INT(0, d.month);
    TEST_ASSERT_EQUAL_INT(4, d.day);
}

void test_from_jd_month_19_after_ayyam(void)
{
    /* Year 1 common: 1 Ala = day 347 = naw_ruz + 346 */
    bahai_date_t d = bahai_from_jd(2394645.5 + 346.0);
    TEST_ASSERT_EQUAL_INT(1, d.year);
    TEST_ASSERT_EQUAL_INT(19, d.month);
    TEST_ASSERT_EQUAL_INT(1, d.day);
}

void test_from_jd_last_day_common(void)
{
    /* Year 1 common: 19 Ala = day 365 = naw_ruz + 364 */
    bahai_date_t d = bahai_from_jd(2394645.5 + 364.0);
    TEST_ASSERT_EQUAL_INT(1, d.year);
    TEST_ASSERT_EQUAL_INT(19, d.month);
    TEST_ASSERT_EQUAL_INT(19, d.day);
}

void test_from_jd_next_year(void)
{
    /* Year 1 common (365 days): next year starts at naw_ruz + 365 */
    bahai_date_t d = bahai_from_jd(2394645.5 + 365.0);
    TEST_ASSERT_EQUAL_INT(2, d.year);
    TEST_ASSERT_EQUAL_INT(1, d.month);
    TEST_ASSERT_EQUAL_INT(1, d.day);
}

/* ===== Round-trip verification ===== */

void test_round_trip_epoch(void)
{
    bahai_date_t orig = { .year = 1, .month = 1, .day = 1 };
    double jd = bahai_to_jd(orig);
    bahai_date_t back = bahai_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(orig.year, back.year);
    TEST_ASSERT_EQUAL_INT(orig.month, back.month);
    TEST_ASSERT_EQUAL_INT(orig.day, back.day);
}

void test_round_trip_ayyam_i_ha(void)
{
    bahai_date_t orig = { .year = 1, .month = 0, .day = 3 };
    double jd = bahai_to_jd(orig);
    bahai_date_t back = bahai_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(orig.year, back.year);
    TEST_ASSERT_EQUAL_INT(orig.month, back.month);
    TEST_ASSERT_EQUAL_INT(orig.day, back.day);
}

void test_round_trip_month_19(void)
{
    bahai_date_t orig = { .year = 2, .month = 19, .day = 10 };
    double jd = bahai_to_jd(orig);
    bahai_date_t back = bahai_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(orig.year, back.year);
    TEST_ASSERT_EQUAL_INT(orig.month, back.month);
    TEST_ASSERT_EQUAL_INT(orig.day, back.day);
}

void test_round_trip_mid_year(void)
{
    bahai_date_t orig = { .year = 100, .month = 10, .day = 15 };
    double jd = bahai_to_jd(orig);
    bahai_date_t back = bahai_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(orig.year, back.year);
    TEST_ASSERT_EQUAL_INT(orig.month, back.month);
    TEST_ASSERT_EQUAL_INT(orig.day, back.day);
}

void test_round_trip_modern_date(void)
{
    bahai_date_t orig = { .year = 182, .month = 7, .day = 12 };
    double jd = bahai_to_jd(orig);
    bahai_date_t back = bahai_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(orig.year, back.year);
    TEST_ASSERT_EQUAL_INT(orig.month, back.month);
    TEST_ASSERT_EQUAL_INT(orig.day, back.day);
}

void test_round_trip_ayyam_leap_day_5(void)
{
    /* 5th Ayyam-i-Ha in leap year 180 (GY 2024 is leap) */
    bahai_date_t orig = { .year = 180, .month = 0, .day = 5 };
    double jd = bahai_to_jd(orig);
    bahai_date_t back = bahai_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(orig.year, back.year);
    TEST_ASSERT_EQUAL_INT(orig.month, back.month);
    TEST_ASSERT_EQUAL_INT(orig.day, back.day);
}

void test_round_trip_last_day_common(void)
{
    bahai_date_t orig = { .year = 2, .month = 19, .day = 19 };
    double jd = bahai_to_jd(orig);
    bahai_date_t back = bahai_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(orig.year, back.year);
    TEST_ASSERT_EQUAL_INT(orig.month, back.month);
    TEST_ASSERT_EQUAL_INT(orig.day, back.day);
}

void test_round_trip_sweep(void)
{
    /* Sweep through all days of year 4 (leap year, 5 Ayyam-i-Ha) */
    bahai_date_t d;
    for (int m = 1; m <= 18; m++) {
        for (int dy = 1; dy <= 19; dy++) {
            d.year = 4; d.month = m; d.day = dy;
            double jd = bahai_to_jd(d);
            bahai_date_t back = bahai_from_jd(jd);
            TEST_ASSERT_EQUAL_INT(d.year, back.year);
            TEST_ASSERT_EQUAL_INT(d.month, back.month);
            TEST_ASSERT_EQUAL_INT(d.day, back.day);
        }
    }
    /* Ayyam-i-Ha (5 days in leap year) */
    for (int dy = 1; dy <= 5; dy++) {
        d.year = 4; d.month = 0; d.day = dy;
        double jd = bahai_to_jd(d);
        bahai_date_t back = bahai_from_jd(jd);
        TEST_ASSERT_EQUAL_INT(d.year, back.year);
        TEST_ASSERT_EQUAL_INT(d.month, back.month);
        TEST_ASSERT_EQUAL_INT(d.day, back.day);
    }
    /* Month 19 */
    for (int dy = 1; dy <= 19; dy++) {
        d.year = 4; d.month = 19; d.day = dy;
        double jd = bahai_to_jd(d);
        bahai_date_t back = bahai_from_jd(jd);
        TEST_ASSERT_EQUAL_INT(d.year, back.year);
        TEST_ASSERT_EQUAL_INT(d.month, back.month);
        TEST_ASSERT_EQUAL_INT(d.day, back.day);
    }
}

/* ===== Vahid + Kull-i-Shay' computation ===== */

void test_vahid_year_1(void)
{
    TEST_ASSERT_EQUAL_INT(1, bahai_vahid(1));
}

void test_vahid_year_19(void)
{
    TEST_ASSERT_EQUAL_INT(1, bahai_vahid(19));
}

void test_vahid_year_20(void)
{
    TEST_ASSERT_EQUAL_INT(2, bahai_vahid(20));
}

void test_vahid_year_361(void)
{
    TEST_ASSERT_EQUAL_INT(19, bahai_vahid(361));
}

void test_vahid_year_362(void)
{
    /* ((362-1)/19)+1 = 20. Vahid is absolute, not wrapped within Kull-i-Shay. */
    TEST_ASSERT_EQUAL_INT(20, bahai_vahid(362));
}

void test_kull_i_shay_year_1(void)
{
    TEST_ASSERT_EQUAL_INT(1, bahai_kull_i_shay(1));
}

void test_kull_i_shay_year_361(void)
{
    TEST_ASSERT_EQUAL_INT(1, bahai_kull_i_shay(361));
}

void test_kull_i_shay_year_362(void)
{
    TEST_ASSERT_EQUAL_INT(2, bahai_kull_i_shay(362));
}

/* ===== Year in Vahid ===== */

void test_year_in_vahid_1(void)
{
    TEST_ASSERT_EQUAL_INT(1, bahai_year_in_vahid(1));
}

void test_year_in_vahid_19(void)
{
    TEST_ASSERT_EQUAL_INT(19, bahai_year_in_vahid(19));
}

void test_year_in_vahid_20(void)
{
    TEST_ASSERT_EQUAL_INT(1, bahai_year_in_vahid(20));
}

void test_year_in_vahid_38(void)
{
    TEST_ASSERT_EQUAL_INT(19, bahai_year_in_vahid(38));
}

/* ===== Holy day detection ===== */

void test_holy_naw_ruz(void)
{
    bahai_date_t d = { .year = 1, .month = 1, .day = 1 };
    TEST_ASSERT_EQUAL_INT(BAHAI_HOLY_NAW_RUZ, bahai_holy_day(d));
}

void test_holy_ridvan_1(void)
{
    bahai_date_t d = { .year = 1, .month = 2, .day = 13 };
    TEST_ASSERT_EQUAL_INT(BAHAI_HOLY_RIDVAN_1, bahai_holy_day(d));
}

void test_holy_ridvan_9(void)
{
    bahai_date_t d = { .year = 1, .month = 4, .day = 2 };
    TEST_ASSERT_EQUAL_INT(BAHAI_HOLY_RIDVAN_9, bahai_holy_day(d));
}

void test_holy_ridvan_12(void)
{
    bahai_date_t d = { .year = 1, .month = 4, .day = 5 };
    TEST_ASSERT_EQUAL_INT(BAHAI_HOLY_RIDVAN_12, bahai_holy_day(d));
}

void test_holy_bab_declaration(void)
{
    bahai_date_t d = { .year = 1, .month = 4, .day = 8 };
    TEST_ASSERT_EQUAL_INT(BAHAI_HOLY_BAB_DECLARATION, bahai_holy_day(d));
}

void test_holy_bahaullah_ascension(void)
{
    bahai_date_t d = { .year = 1, .month = 4, .day = 13 };
    TEST_ASSERT_EQUAL_INT(BAHAI_HOLY_BAHAULLAH_ASCENSION, bahai_holy_day(d));
}

void test_holy_bab_martyrdom(void)
{
    bahai_date_t d = { .year = 1, .month = 6, .day = 17 };
    TEST_ASSERT_EQUAL_INT(BAHAI_HOLY_BAB_MARTYRDOM, bahai_holy_day(d));
}

void test_holy_none(void)
{
    bahai_date_t d = { .year = 1, .month = 3, .day = 5 };
    TEST_ASSERT_EQUAL_INT(BAHAI_HOLY_NONE, bahai_holy_day(d));
}

void test_holy_none_ayyam(void)
{
    bahai_date_t d = { .year = 1, .month = 0, .day = 2 };
    TEST_ASSERT_EQUAL_INT(BAHAI_HOLY_NONE, bahai_holy_day(d));
}

/* ===== Holy day names ===== */

void test_holy_day_name_naw_ruz(void)
{
    TEST_ASSERT_EQUAL_STRING("Naw-Ruz", bahai_holy_day_name(BAHAI_HOLY_NAW_RUZ));
}

void test_holy_day_name_ridvan_1(void)
{
    TEST_ASSERT_EQUAL_STRING("First Day of Ridvan", bahai_holy_day_name(BAHAI_HOLY_RIDVAN_1));
}

void test_holy_day_name_ridvan_9(void)
{
    TEST_ASSERT_EQUAL_STRING("Ninth Day of Ridvan", bahai_holy_day_name(BAHAI_HOLY_RIDVAN_9));
}

void test_holy_day_name_ridvan_12(void)
{
    TEST_ASSERT_EQUAL_STRING("Twelfth Day of Ridvan", bahai_holy_day_name(BAHAI_HOLY_RIDVAN_12));
}

void test_holy_day_name_bab_declaration(void)
{
    TEST_ASSERT_EQUAL_STRING("Declaration of the Bab",
                             bahai_holy_day_name(BAHAI_HOLY_BAB_DECLARATION));
}

void test_holy_day_name_bahaullah(void)
{
    TEST_ASSERT_EQUAL_STRING("Ascension of Bahaullah",
                             bahai_holy_day_name(BAHAI_HOLY_BAHAULLAH_ASCENSION));
}

void test_holy_day_name_bab_martyrdom(void)
{
    TEST_ASSERT_EQUAL_STRING("Martyrdom of the Bab",
                             bahai_holy_day_name(BAHAI_HOLY_BAB_MARTYRDOM));
}

void test_holy_day_name_none(void)
{
    TEST_ASSERT_EQUAL_STRING("", bahai_holy_day_name(BAHAI_HOLY_NONE));
}

void test_holy_day_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", bahai_holy_day_name(BAHAI_HOLY_COUNT));
}

/* ===== is_ayyam_i_ha check ===== */

void test_is_ayyam_i_ha_true(void)
{
    bahai_date_t d = { .year = 1, .month = 0, .day = 3 };
    TEST_ASSERT_TRUE(bahai_is_ayyam_i_ha(d));
}

void test_is_ayyam_i_ha_false_month_1(void)
{
    bahai_date_t d = { .year = 1, .month = 1, .day = 1 };
    TEST_ASSERT_FALSE(bahai_is_ayyam_i_ha(d));
}

void test_is_ayyam_i_ha_false_month_19(void)
{
    bahai_date_t d = { .year = 1, .month = 19, .day = 1 };
    TEST_ASSERT_FALSE(bahai_is_ayyam_i_ha(d));
}

/* ===== Edge cases ===== */

void test_from_jd_common_year_ayyam_boundary(void)
{
    /* In common year 2, Ayyam-i-Ha has 4 days, then month 19 starts */
    /* Day 343..346 = Ayyam-i-Ha 1..4, Day 347 = 1 Ala */
    bahai_date_t naw_ruz_2 = { .year = 2, .month = 1, .day = 1 };
    double naw_ruz_2_jd = bahai_to_jd(naw_ruz_2);

    /* Last Ayyam-i-Ha day in common year */
    bahai_date_t d = bahai_from_jd(naw_ruz_2_jd + 345.0); /* day 346 */
    TEST_ASSERT_EQUAL_INT(2, d.year);
    TEST_ASSERT_EQUAL_INT(0, d.month);
    TEST_ASSERT_EQUAL_INT(4, d.day);

    /* First day of month 19 */
    bahai_date_t d2 = bahai_from_jd(naw_ruz_2_jd + 346.0); /* day 347 */
    TEST_ASSERT_EQUAL_INT(2, d2.year);
    TEST_ASSERT_EQUAL_INT(19, d2.month);
    TEST_ASSERT_EQUAL_INT(1, d2.day);
}

void test_consecutive_naw_ruz(void)
{
    /* Year 1 is common (365 days) */
    bahai_date_t y1 = { .year = 1, .month = 1, .day = 1 };
    bahai_date_t y2 = { .year = 2, .month = 1, .day = 1 };
    double jd1 = bahai_to_jd(y1);
    double jd2 = bahai_to_jd(y2);
    double diff = jd2 - jd1;
    TEST_ASSERT_FLOAT_WITHIN(0.01, 365.0, diff);
}

void test_consecutive_naw_ruz_common(void)
{
    /* Year 2 is common (1845), so diff should be 365 */
    bahai_date_t y2 = { .year = 2, .month = 1, .day = 1 };
    bahai_date_t y3 = { .year = 3, .month = 1, .day = 1 };
    double jd2 = bahai_to_jd(y2);
    double jd3 = bahai_to_jd(y3);
    double diff = jd3 - jd2;
    TEST_ASSERT_FLOAT_WITHIN(0.01, 365.0, diff);
}

void test_month_18_to_ayyam_boundary(void)
{
    /* Last day of month 18 (day 342), then Ayyam-i-Ha day 1 (day 343) */
    bahai_date_t m18_last = { .year = 1, .month = 18, .day = 19 };
    bahai_date_t ayyam_1 = { .year = 1, .month = 0, .day = 1 };
    double jd_m18 = bahai_to_jd(m18_last);
    double jd_ayyam = bahai_to_jd(ayyam_1);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 1.0, jd_ayyam - jd_m18);
}

void test_round_trip_century_later(void)
{
    /* Year 200 BE = 2043 CE */
    bahai_date_t orig = { .year = 200, .month = 15, .day = 7 };
    double jd = bahai_to_jd(orig);
    bahai_date_t back = bahai_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(orig.year, back.year);
    TEST_ASSERT_EQUAL_INT(orig.month, back.month);
    TEST_ASSERT_EQUAL_INT(orig.day, back.day);
}

void test_vahid_current_era(void)
{
    /* Year 182 BE (2025): vahid = ((182-1)/19)+1 = (181/19)+1 = 9+1 = 10 */
    TEST_ASSERT_EQUAL_INT(10, bahai_vahid(182));
}

void test_year_in_vahid_current(void)
{
    /* Year 182 BE: ((182-1)%19)+1 = (181%19)+1 = 10+1 = 11 */
    TEST_ASSERT_EQUAL_INT(11, bahai_year_in_vahid(182));
}

void test_kull_i_shay_current(void)
{
    /* Year 182 BE: ((182-1)/361)+1 = (181/361)+1 = 0+1 = 1 */
    TEST_ASSERT_EQUAL_INT(1, bahai_kull_i_shay(182));
}

/* ===== main ===== */

int main(void)
{
    UNITY_BEGIN();

    /* Month names */
    RUN_TEST(test_month_name_baha);
    RUN_TEST(test_month_name_jalal);
    RUN_TEST(test_month_name_jamal);
    RUN_TEST(test_month_name_azamat);
    RUN_TEST(test_month_name_nur);
    RUN_TEST(test_month_name_rahmat);
    RUN_TEST(test_month_name_kalimat);
    RUN_TEST(test_month_name_kamal);
    RUN_TEST(test_month_name_asma);
    RUN_TEST(test_month_name_izzat);
    RUN_TEST(test_month_name_mashiyyat);
    RUN_TEST(test_month_name_ilm);
    RUN_TEST(test_month_name_qudrat);
    RUN_TEST(test_month_name_qawl);
    RUN_TEST(test_month_name_masail);
    RUN_TEST(test_month_name_sharaf);
    RUN_TEST(test_month_name_sultan);
    RUN_TEST(test_month_name_mulk);
    RUN_TEST(test_month_name_ala);
    RUN_TEST(test_month_name_ayyam_i_ha);
    RUN_TEST(test_month_name_invalid_negative);
    RUN_TEST(test_month_name_invalid_high);

    /* Month meanings */
    RUN_TEST(test_month_meaning_baha);
    RUN_TEST(test_month_meaning_jalal);
    RUN_TEST(test_month_meaning_nur);
    RUN_TEST(test_month_meaning_kamal);
    RUN_TEST(test_month_meaning_ilm);
    RUN_TEST(test_month_meaning_ala);
    RUN_TEST(test_month_meaning_ayyam_i_ha);
    RUN_TEST(test_month_meaning_invalid);

    /* Leap year */
    RUN_TEST(test_leap_year_4);
    RUN_TEST(test_leap_year_8);
    RUN_TEST(test_not_leap_year_1);
    RUN_TEST(test_not_leap_year_2);
    RUN_TEST(test_leap_year_century_not_leap);
    RUN_TEST(test_leap_year_400_cycle);
    RUN_TEST(test_leap_year_182);
    RUN_TEST(test_leap_year_180);

    /* Month days */
    RUN_TEST(test_month_days_regular);
    RUN_TEST(test_month_days_month_19);
    RUN_TEST(test_month_days_ayyam_i_ha_common);
    RUN_TEST(test_month_days_ayyam_i_ha_leap);

    /* Year days */
    RUN_TEST(test_year_days_common);
    RUN_TEST(test_year_days_leap);

    /* Ayyam-i-Ha days */
    RUN_TEST(test_ayyam_days_common);
    RUN_TEST(test_ayyam_days_leap);

    /* Baha'i -> JD */
    RUN_TEST(test_to_jd_epoch);
    RUN_TEST(test_to_jd_second_day);
    RUN_TEST(test_to_jd_month_2_day_1);
    RUN_TEST(test_to_jd_ayyam_i_ha_day_1);
    RUN_TEST(test_to_jd_month_19_day_1_leap);
    RUN_TEST(test_to_jd_month_19_day_1_common);
    RUN_TEST(test_to_jd_last_day_of_year_leap);
    RUN_TEST(test_to_jd_last_day_of_year_common);

    /* JD -> Baha'i */
    RUN_TEST(test_from_jd_epoch);
    RUN_TEST(test_from_jd_second_day);
    RUN_TEST(test_from_jd_month_boundary);
    RUN_TEST(test_from_jd_ayyam_i_ha);
    RUN_TEST(test_from_jd_ayyam_i_ha_last_common);
    RUN_TEST(test_from_jd_month_19_after_ayyam);
    RUN_TEST(test_from_jd_last_day_common);
    RUN_TEST(test_from_jd_next_year);

    /* Round-trip */
    RUN_TEST(test_round_trip_epoch);
    RUN_TEST(test_round_trip_ayyam_i_ha);
    RUN_TEST(test_round_trip_month_19);
    RUN_TEST(test_round_trip_mid_year);
    RUN_TEST(test_round_trip_modern_date);
    RUN_TEST(test_round_trip_ayyam_leap_day_5);
    RUN_TEST(test_round_trip_last_day_common);
    RUN_TEST(test_round_trip_sweep);

    /* Vahid + Kull-i-Shay' */
    RUN_TEST(test_vahid_year_1);
    RUN_TEST(test_vahid_year_19);
    RUN_TEST(test_vahid_year_20);
    RUN_TEST(test_vahid_year_361);
    RUN_TEST(test_vahid_year_362);
    RUN_TEST(test_kull_i_shay_year_1);
    RUN_TEST(test_kull_i_shay_year_361);
    RUN_TEST(test_kull_i_shay_year_362);

    /* Year in Vahid */
    RUN_TEST(test_year_in_vahid_1);
    RUN_TEST(test_year_in_vahid_19);
    RUN_TEST(test_year_in_vahid_20);
    RUN_TEST(test_year_in_vahid_38);

    /* Holy days */
    RUN_TEST(test_holy_naw_ruz);
    RUN_TEST(test_holy_ridvan_1);
    RUN_TEST(test_holy_ridvan_9);
    RUN_TEST(test_holy_ridvan_12);
    RUN_TEST(test_holy_bab_declaration);
    RUN_TEST(test_holy_bahaullah_ascension);
    RUN_TEST(test_holy_bab_martyrdom);
    RUN_TEST(test_holy_none);
    RUN_TEST(test_holy_none_ayyam);

    /* Holy day names */
    RUN_TEST(test_holy_day_name_naw_ruz);
    RUN_TEST(test_holy_day_name_ridvan_1);
    RUN_TEST(test_holy_day_name_ridvan_9);
    RUN_TEST(test_holy_day_name_ridvan_12);
    RUN_TEST(test_holy_day_name_bab_declaration);
    RUN_TEST(test_holy_day_name_bahaullah);
    RUN_TEST(test_holy_day_name_bab_martyrdom);
    RUN_TEST(test_holy_day_name_none);
    RUN_TEST(test_holy_day_name_invalid);

    /* is_ayyam_i_ha */
    RUN_TEST(test_is_ayyam_i_ha_true);
    RUN_TEST(test_is_ayyam_i_ha_false_month_1);
    RUN_TEST(test_is_ayyam_i_ha_false_month_19);

    /* Edge cases */
    RUN_TEST(test_from_jd_common_year_ayyam_boundary);
    RUN_TEST(test_consecutive_naw_ruz);
    RUN_TEST(test_consecutive_naw_ruz_common);
    RUN_TEST(test_month_18_to_ayyam_boundary);
    RUN_TEST(test_round_trip_century_later);
    RUN_TEST(test_vahid_current_era);
    RUN_TEST(test_year_in_vahid_current);
    RUN_TEST(test_kull_i_shay_current);

    return UNITY_END();
}
