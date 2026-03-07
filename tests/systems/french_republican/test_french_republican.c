/* test_french_republican.c — French Republican calendar tests
 * TDD RED phase: tests written before implementation.
 * Calendar: 12 months x 30 days + 5/6 Sansculottides.
 * Epoch: 1 Vendemiaire An I = Sep 22, 1792 CE = JD 2375839.5.
 * Decimal time: 10h x 100m x 100s per day. */

#include "../../unity/unity.h"
#include "../../../src/systems/french_republican/french_republican.h"
#include <math.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== Month names (12 + Sansculottides + invalid) ===== */

void test_month_name_vendemiaire(void)
{
    TEST_ASSERT_EQUAL_STRING("Vendemiaire", fr_month_name(1));
}

void test_month_name_brumaire(void)
{
    TEST_ASSERT_EQUAL_STRING("Brumaire", fr_month_name(2));
}

void test_month_name_frimaire(void)
{
    TEST_ASSERT_EQUAL_STRING("Frimaire", fr_month_name(3));
}

void test_month_name_nivose(void)
{
    TEST_ASSERT_EQUAL_STRING("Nivose", fr_month_name(4));
}

void test_month_name_pluviose(void)
{
    TEST_ASSERT_EQUAL_STRING("Pluviose", fr_month_name(5));
}

void test_month_name_ventose(void)
{
    TEST_ASSERT_EQUAL_STRING("Ventose", fr_month_name(6));
}

void test_month_name_germinal(void)
{
    TEST_ASSERT_EQUAL_STRING("Germinal", fr_month_name(7));
}

void test_month_name_floreal(void)
{
    TEST_ASSERT_EQUAL_STRING("Floreal", fr_month_name(8));
}

void test_month_name_prairial(void)
{
    TEST_ASSERT_EQUAL_STRING("Prairial", fr_month_name(9));
}

void test_month_name_messidor(void)
{
    TEST_ASSERT_EQUAL_STRING("Messidor", fr_month_name(10));
}

void test_month_name_thermidor(void)
{
    TEST_ASSERT_EQUAL_STRING("Thermidor", fr_month_name(11));
}

void test_month_name_fructidor(void)
{
    TEST_ASSERT_EQUAL_STRING("Fructidor", fr_month_name(12));
}

void test_month_name_sansculottides(void)
{
    TEST_ASSERT_EQUAL_STRING("Sansculottides", fr_month_name(0));
}

void test_month_name_invalid_negative(void)
{
    TEST_ASSERT_EQUAL_STRING("?", fr_month_name(-1));
}

void test_month_name_invalid_high(void)
{
    TEST_ASSERT_EQUAL_STRING("?", fr_month_name(13));
}

/* ===== Month meanings (12 + Sansculottides + invalid) ===== */

void test_month_meaning_vendemiaire(void)
{
    TEST_ASSERT_EQUAL_STRING("Grape Harvest", fr_month_meaning(1));
}

void test_month_meaning_brumaire(void)
{
    TEST_ASSERT_EQUAL_STRING("Fog", fr_month_meaning(2));
}

void test_month_meaning_frimaire(void)
{
    TEST_ASSERT_EQUAL_STRING("Frost", fr_month_meaning(3));
}

void test_month_meaning_nivose(void)
{
    TEST_ASSERT_EQUAL_STRING("Snow", fr_month_meaning(4));
}

void test_month_meaning_pluviose(void)
{
    TEST_ASSERT_EQUAL_STRING("Rain", fr_month_meaning(5));
}

void test_month_meaning_ventose(void)
{
    TEST_ASSERT_EQUAL_STRING("Wind", fr_month_meaning(6));
}

void test_month_meaning_germinal(void)
{
    TEST_ASSERT_EQUAL_STRING("Germination", fr_month_meaning(7));
}

void test_month_meaning_floreal(void)
{
    TEST_ASSERT_EQUAL_STRING("Flower", fr_month_meaning(8));
}

void test_month_meaning_prairial(void)
{
    TEST_ASSERT_EQUAL_STRING("Meadow", fr_month_meaning(9));
}

void test_month_meaning_messidor(void)
{
    TEST_ASSERT_EQUAL_STRING("Harvest", fr_month_meaning(10));
}

void test_month_meaning_thermidor(void)
{
    TEST_ASSERT_EQUAL_STRING("Heat", fr_month_meaning(11));
}

void test_month_meaning_fructidor(void)
{
    TEST_ASSERT_EQUAL_STRING("Fruit", fr_month_meaning(12));
}

void test_month_meaning_sansculottides(void)
{
    TEST_ASSERT_EQUAL_STRING("", fr_month_meaning(0));
}

void test_month_meaning_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", fr_month_meaning(13));
}

/* ===== Sansculottide day names ===== */

void test_sansculottide_name_vertu(void)
{
    TEST_ASSERT_EQUAL_STRING("Vertu", fr_sansculottide_name(1));
}

void test_sansculottide_name_genie(void)
{
    TEST_ASSERT_EQUAL_STRING("Genie", fr_sansculottide_name(2));
}

void test_sansculottide_name_travail(void)
{
    TEST_ASSERT_EQUAL_STRING("Travail", fr_sansculottide_name(3));
}

void test_sansculottide_name_opinion(void)
{
    TEST_ASSERT_EQUAL_STRING("Opinion", fr_sansculottide_name(4));
}

void test_sansculottide_name_recompenses(void)
{
    TEST_ASSERT_EQUAL_STRING("Recompenses", fr_sansculottide_name(5));
}

void test_sansculottide_name_revolution(void)
{
    TEST_ASSERT_EQUAL_STRING("Revolution", fr_sansculottide_name(6));
}

void test_sansculottide_name_invalid_zero(void)
{
    TEST_ASSERT_EQUAL_STRING("?", fr_sansculottide_name(0));
}

void test_sansculottide_name_invalid_high(void)
{
    TEST_ASSERT_EQUAL_STRING("?", fr_sansculottide_name(7));
}

/* ===== Decade day names ===== */

void test_decade_day_name_primidi(void)
{
    TEST_ASSERT_EQUAL_STRING("Primidi", fr_decade_day_name(1));
}

void test_decade_day_name_duodi(void)
{
    TEST_ASSERT_EQUAL_STRING("Duodi", fr_decade_day_name(2));
}

void test_decade_day_name_tridi(void)
{
    TEST_ASSERT_EQUAL_STRING("Tridi", fr_decade_day_name(3));
}

void test_decade_day_name_quartidi(void)
{
    TEST_ASSERT_EQUAL_STRING("Quartidi", fr_decade_day_name(4));
}

void test_decade_day_name_quintidi(void)
{
    TEST_ASSERT_EQUAL_STRING("Quintidi", fr_decade_day_name(5));
}

void test_decade_day_name_sextidi(void)
{
    TEST_ASSERT_EQUAL_STRING("Sextidi", fr_decade_day_name(6));
}

void test_decade_day_name_septidi(void)
{
    TEST_ASSERT_EQUAL_STRING("Septidi", fr_decade_day_name(7));
}

void test_decade_day_name_octidi(void)
{
    TEST_ASSERT_EQUAL_STRING("Octidi", fr_decade_day_name(8));
}

void test_decade_day_name_nonidi(void)
{
    TEST_ASSERT_EQUAL_STRING("Nonidi", fr_decade_day_name(9));
}

void test_decade_day_name_decadi(void)
{
    TEST_ASSERT_EQUAL_STRING("Decadi", fr_decade_day_name(10));
}

void test_decade_day_name_invalid_zero(void)
{
    TEST_ASSERT_EQUAL_STRING("?", fr_decade_day_name(0));
}

void test_decade_day_name_invalid_high(void)
{
    TEST_ASSERT_EQUAL_STRING("?", fr_decade_day_name(11));
}

/* ===== Leap year ===== */

void test_leap_year_an_1_not_leap(void)
{
    /* An I: (1+1)=2, 2%4!=0 -> not leap */
    TEST_ASSERT_FALSE(fr_is_leap_year(1));
}

void test_leap_year_an_3_leap(void)
{
    /* An III: (3+1)=4, 4%4=0 -> leap (historical) */
    TEST_ASSERT_TRUE(fr_is_leap_year(3));
}

void test_leap_year_an_7_leap(void)
{
    /* An VII: (7+1)=8, 8%4=0 -> leap (historical) */
    TEST_ASSERT_TRUE(fr_is_leap_year(7));
}

void test_leap_year_an_11_leap(void)
{
    /* An XI: (11+1)=12, 12%4=0 -> leap (historical) */
    TEST_ASSERT_TRUE(fr_is_leap_year(11));
}

void test_leap_year_an_4_not_leap(void)
{
    TEST_ASSERT_FALSE(fr_is_leap_year(4));
}

void test_leap_year_an_8_not_leap(void)
{
    TEST_ASSERT_FALSE(fr_is_leap_year(8));
}

void test_leap_year_an_99_century_not_leap(void)
{
    /* An 99: (99+1)=100, 100%4=0 but 100%100=0, 100%400!=0 -> not leap */
    TEST_ASSERT_FALSE(fr_is_leap_year(99));
}

void test_leap_year_an_399_century_leap(void)
{
    /* An 399: (399+1)=400, 400%400=0 -> leap */
    TEST_ASSERT_TRUE(fr_is_leap_year(399));
}

/* ===== Year days ===== */

void test_year_days_normal(void)
{
    TEST_ASSERT_EQUAL_INT(365, fr_year_days(1));
}

void test_year_days_leap(void)
{
    TEST_ASSERT_EQUAL_INT(366, fr_year_days(3));
}

/* ===== Month days ===== */

void test_month_days_regular_month(void)
{
    /* All regular months have 30 days */
    for (int m = 1; m <= 12; m++) {
        TEST_ASSERT_EQUAL_INT(30, fr_month_days(1, m));
    }
}

void test_month_days_sansculottides_normal(void)
{
    /* Non-leap year: 5 Sansculottide days */
    TEST_ASSERT_EQUAL_INT(5, fr_month_days(1, 0));
}

void test_month_days_sansculottides_leap(void)
{
    /* Leap year: 6 Sansculottide days */
    TEST_ASSERT_EQUAL_INT(6, fr_month_days(3, 0));
}

/* ===== Epoch conversion: fr_to_jd ===== */

void test_to_jd_epoch(void)
{
    /* 1 Vendemiaire An I = Sep 22, 1792 = JD 2375839.5 */
    fr_date_t d = {1, 1, 1};
    TEST_ASSERT_FLOAT_WITHIN(0.01, 2375839.5, fr_to_jd(d));
}

void test_to_jd_thermidor_an_2(void)
{
    /* 9 Thermidor An II = July 27, 1794 = JD 2376512.5 */
    fr_date_t d = {2, 11, 9};
    TEST_ASSERT_FLOAT_WITHIN(0.01, 2376512.5, fr_to_jd(d));
}

void test_to_jd_brumaire_an_8(void)
{
    /* 18 Brumaire An VIII = Nov 9, 1799 = JD 2378443.5 */
    fr_date_t d = {8, 2, 18};
    TEST_ASSERT_FLOAT_WITHIN(0.01, 2378443.5, fr_to_jd(d));
}

void test_to_jd_nivose_an_14(void)
{
    /* 10 Nivose An XIV = Dec 31, 1805 = JD 2380686.5 */
    fr_date_t d = {14, 4, 10};
    TEST_ASSERT_FLOAT_WITHIN(0.01, 2380686.5, fr_to_jd(d));
}

void test_to_jd_sansculottide_normal_year(void)
{
    /* 1st Sansculottide An I (non-leap): day 361 of year */
    fr_date_t d = {1, 0, 1};
    /* 360 regular days + 0 offset = day 361 */
    TEST_ASSERT_FLOAT_WITHIN(0.01, 2375839.5 + 360.0, fr_to_jd(d));
}

void test_to_jd_sansculottide_5th_day(void)
{
    /* 5th Sansculottide An I: day 365 */
    fr_date_t d = {1, 0, 5};
    TEST_ASSERT_FLOAT_WITHIN(0.01, 2375839.5 + 364.0, fr_to_jd(d));
}

void test_to_jd_sansculottide_leap_6th_day(void)
{
    /* 6th Sansculottide An III (leap): day 366 */
    fr_date_t d = {3, 0, 6};
    double an3_start = 2375839.5 + 365.0 + 365.0; /* An I: 365, An II: 365 */
    TEST_ASSERT_FLOAT_WITHIN(0.01, an3_start + 365.0, fr_to_jd(d));
}

void test_to_jd_last_day_of_fructidor(void)
{
    /* 30 Fructidor An I (last regular day before Sansculottides) */
    fr_date_t d = {1, 12, 30};
    /* (12-1)*30 + (30-1) = 330 + 29 = 359 days offset from epoch */
    TEST_ASSERT_FLOAT_WITHIN(0.01, 2375839.5 + 359.0, fr_to_jd(d));
}

/* ===== Reverse conversion: fr_from_jd ===== */

void test_from_jd_epoch(void)
{
    /* JD 2375839.5 = 1 Vendemiaire An I */
    fr_date_t d = fr_from_jd(2375839.5);
    TEST_ASSERT_EQUAL_INT(1, d.year);
    TEST_ASSERT_EQUAL_INT(1, d.month);
    TEST_ASSERT_EQUAL_INT(1, d.day);
}

void test_from_jd_thermidor_an_2(void)
{
    /* JD 2376512.5 = 9 Thermidor An II */
    fr_date_t d = fr_from_jd(2376512.5);
    TEST_ASSERT_EQUAL_INT(2, d.year);
    TEST_ASSERT_EQUAL_INT(11, d.month);
    TEST_ASSERT_EQUAL_INT(9, d.day);
}

void test_from_jd_brumaire_an_8(void)
{
    /* JD 2378443.5 = 18 Brumaire An VIII */
    fr_date_t d = fr_from_jd(2378443.5);
    TEST_ASSERT_EQUAL_INT(8, d.year);
    TEST_ASSERT_EQUAL_INT(2, d.month);
    TEST_ASSERT_EQUAL_INT(18, d.day);
}

void test_from_jd_nivose_an_14(void)
{
    /* JD 2380686.5 = 10 Nivose An XIV */
    fr_date_t d = fr_from_jd(2380686.5);
    TEST_ASSERT_EQUAL_INT(14, d.year);
    TEST_ASSERT_EQUAL_INT(4, d.month);
    TEST_ASSERT_EQUAL_INT(10, d.day);
}

void test_from_jd_sansculottide(void)
{
    /* 1st Sansculottide An I */
    fr_date_t d = fr_from_jd(2375839.5 + 360.0);
    TEST_ASSERT_EQUAL_INT(1, d.year);
    TEST_ASSERT_EQUAL_INT(0, d.month);
    TEST_ASSERT_EQUAL_INT(1, d.day);
}

void test_from_jd_5th_sansculottide(void)
{
    /* 5th Sansculottide An I (last day of normal year) */
    fr_date_t d = fr_from_jd(2375839.5 + 364.0);
    TEST_ASSERT_EQUAL_INT(1, d.year);
    TEST_ASSERT_EQUAL_INT(0, d.month);
    TEST_ASSERT_EQUAL_INT(5, d.day);
}

void test_from_jd_6th_sansculottide_leap(void)
{
    /* 6th Sansculottide An III (leap year) */
    double an3_start = 2375839.5 + 365.0 + 365.0;
    fr_date_t d = fr_from_jd(an3_start + 365.0);
    TEST_ASSERT_EQUAL_INT(3, d.year);
    TEST_ASSERT_EQUAL_INT(0, d.month);
    TEST_ASSERT_EQUAL_INT(6, d.day);
}

void test_from_jd_first_day_an_2(void)
{
    /* 1 Vendemiaire An II (next year after non-leap An I) */
    fr_date_t d = fr_from_jd(2375839.5 + 365.0);
    TEST_ASSERT_EQUAL_INT(2, d.year);
    TEST_ASSERT_EQUAL_INT(1, d.month);
    TEST_ASSERT_EQUAL_INT(1, d.day);
}

void test_from_jd_first_day_an_4(void)
{
    /* 1 Vendemiaire An IV (after leap An III) */
    double an4_start = 2375839.5 + 365.0 + 365.0 + 366.0;
    fr_date_t d = fr_from_jd(an4_start);
    TEST_ASSERT_EQUAL_INT(4, d.year);
    TEST_ASSERT_EQUAL_INT(1, d.month);
    TEST_ASSERT_EQUAL_INT(1, d.day);
}

/* ===== Round-trip tests ===== */

void test_roundtrip_epoch(void)
{
    fr_date_t d = {1, 1, 1};
    fr_date_t r = fr_from_jd(fr_to_jd(d));
    TEST_ASSERT_EQUAL_INT(d.year, r.year);
    TEST_ASSERT_EQUAL_INT(d.month, r.month);
    TEST_ASSERT_EQUAL_INT(d.day, r.day);
}

void test_roundtrip_mid_year(void)
{
    fr_date_t d = {5, 7, 15};
    fr_date_t r = fr_from_jd(fr_to_jd(d));
    TEST_ASSERT_EQUAL_INT(d.year, r.year);
    TEST_ASSERT_EQUAL_INT(d.month, r.month);
    TEST_ASSERT_EQUAL_INT(d.day, r.day);
}

void test_roundtrip_sansculottide(void)
{
    fr_date_t d = {1, 0, 3};
    fr_date_t r = fr_from_jd(fr_to_jd(d));
    TEST_ASSERT_EQUAL_INT(d.year, r.year);
    TEST_ASSERT_EQUAL_INT(d.month, r.month);
    TEST_ASSERT_EQUAL_INT(d.day, r.day);
}

void test_roundtrip_leap_sansculottide(void)
{
    fr_date_t d = {3, 0, 6};
    fr_date_t r = fr_from_jd(fr_to_jd(d));
    TEST_ASSERT_EQUAL_INT(d.year, r.year);
    TEST_ASSERT_EQUAL_INT(d.month, r.month);
    TEST_ASSERT_EQUAL_INT(d.day, r.day);
}

void test_roundtrip_last_regular_day(void)
{
    fr_date_t d = {2, 12, 30};
    fr_date_t r = fr_from_jd(fr_to_jd(d));
    TEST_ASSERT_EQUAL_INT(d.year, r.year);
    TEST_ASSERT_EQUAL_INT(d.month, r.month);
    TEST_ASSERT_EQUAL_INT(d.day, r.day);
}

void test_roundtrip_first_day_month(void)
{
    fr_date_t d = {10, 6, 1};
    fr_date_t r = fr_from_jd(fr_to_jd(d));
    TEST_ASSERT_EQUAL_INT(d.year, r.year);
    TEST_ASSERT_EQUAL_INT(d.month, r.month);
    TEST_ASSERT_EQUAL_INT(d.day, r.day);
}

/* ===== Decade day ===== */

void test_decade_day_first_of_month(void)
{
    /* Day 1 of month -> decade position 1 (Primidi) */
    fr_date_t d = {1, 1, 1};
    TEST_ASSERT_EQUAL_INT(1, fr_decade_day(d));
}

void test_decade_day_tenth(void)
{
    /* Day 10 -> decade position 10 (Decadi) */
    fr_date_t d = {1, 1, 10};
    TEST_ASSERT_EQUAL_INT(10, fr_decade_day(d));
}

void test_decade_day_eleventh(void)
{
    /* Day 11 -> decade position 1 (start of 2nd decade) */
    fr_date_t d = {1, 1, 11};
    TEST_ASSERT_EQUAL_INT(1, fr_decade_day(d));
}

void test_decade_day_thirtieth(void)
{
    /* Day 30 -> decade position 10 (end of 3rd decade) */
    fr_date_t d = {1, 1, 30};
    TEST_ASSERT_EQUAL_INT(10, fr_decade_day(d));
}

void test_decade_day_sansculottide_returns_zero(void)
{
    /* Sansculottides are outside the decade system -> 0 */
    fr_date_t d = {1, 0, 1};
    TEST_ASSERT_EQUAL_INT(0, fr_decade_day(d));
}

/* ===== Decimal time ===== */

void test_decimal_time_midnight(void)
{
    fr_decimal_time_t dt = fr_decimal_time(0.0);
    TEST_ASSERT_EQUAL_INT(0, dt.hours);
    TEST_ASSERT_EQUAL_INT(0, dt.minutes);
    TEST_ASSERT_EQUAL_INT(0, dt.seconds);
}

void test_decimal_time_noon(void)
{
    /* 0.5 day fraction = 50000 decimal seconds = 5h 00m 00s */
    fr_decimal_time_t dt = fr_decimal_time(0.5);
    TEST_ASSERT_EQUAL_INT(5, dt.hours);
    TEST_ASSERT_EQUAL_INT(0, dt.minutes);
    TEST_ASSERT_EQUAL_INT(0, dt.seconds);
}

void test_decimal_time_quarter_day(void)
{
    /* 0.25 = 25000 dec seconds = 2h 50m 00s */
    fr_decimal_time_t dt = fr_decimal_time(0.25);
    TEST_ASSERT_EQUAL_INT(2, dt.hours);
    TEST_ASSERT_EQUAL_INT(50, dt.minutes);
    TEST_ASSERT_EQUAL_INT(0, dt.seconds);
}

void test_decimal_time_end_of_day(void)
{
    /* 0.99999 -> 9h 99m 99s */
    fr_decimal_time_t dt = fr_decimal_time(0.99999);
    TEST_ASSERT_EQUAL_INT(9, dt.hours);
    TEST_ASSERT_EQUAL_INT(99, dt.minutes);
    TEST_ASSERT_EQUAL_INT(99, dt.seconds);
}

void test_decimal_time_one_decimal_second(void)
{
    /* 1 decimal second = 1/100000 of a day */
    fr_decimal_time_t dt = fr_decimal_time(0.00001);
    TEST_ASSERT_EQUAL_INT(0, dt.hours);
    TEST_ASSERT_EQUAL_INT(0, dt.minutes);
    TEST_ASSERT_EQUAL_INT(1, dt.seconds);
}

void test_decimal_time_arbitrary(void)
{
    /* 0.3 = 30000 dec seconds = 3h 00m 00s */
    fr_decimal_time_t dt = fr_decimal_time(0.3);
    TEST_ASSERT_EQUAL_INT(3, dt.hours);
    TEST_ASSERT_EQUAL_INT(0, dt.minutes);
    TEST_ASSERT_EQUAL_INT(0, dt.seconds);
}

/* ===== Decimal to standard conversion ===== */

void test_decimal_to_standard_midnight(void)
{
    fr_decimal_time_t dt = {0, 0, 0};
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.0, fr_decimal_to_standard(dt));
}

void test_decimal_to_standard_noon(void)
{
    /* 5h 00m 00s decimal = 50000 dec_sec / 100000 = 0.5 day fraction */
    fr_decimal_time_t dt = {5, 0, 0};
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.5, fr_decimal_to_standard(dt));
}

void test_decimal_to_standard_quarter(void)
{
    /* 2h 50m 00s decimal = 25000 dec_sec / 100000 = 0.25 */
    fr_decimal_time_t dt = {2, 50, 0};
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.25, fr_decimal_to_standard(dt));
}

/* ===== is_sansculottide ===== */

void test_is_sansculottide_true(void)
{
    fr_date_t d = {1, 0, 3};
    TEST_ASSERT_TRUE(fr_is_sansculottide(d));
}

void test_is_sansculottide_false(void)
{
    fr_date_t d = {1, 5, 15};
    TEST_ASSERT_FALSE(fr_is_sansculottide(d));
}

/* ===== CE year approximation ===== */

void test_ce_year_approx_an_1(void)
{
    /* An I started in 1792 CE */
    TEST_ASSERT_EQUAL_INT(1792, fr_ce_year_approx(1));
}

void test_ce_year_approx_an_14(void)
{
    /* An XIV started in 1805 CE */
    TEST_ASSERT_EQUAL_INT(1805, fr_ce_year_approx(14));
}

void test_ce_year_approx_an_100(void)
{
    /* An 100 ~ 1891 CE */
    TEST_ASSERT_EQUAL_INT(1891, fr_ce_year_approx(100));
}

/* ===== Rep year approximation ===== */

void test_rep_year_approx_1792(void)
{
    TEST_ASSERT_EQUAL_INT(1, fr_rep_year_approx(1792));
}

void test_rep_year_approx_1805(void)
{
    TEST_ASSERT_EQUAL_INT(14, fr_rep_year_approx(1805));
}

void test_rep_year_approx_2000(void)
{
    TEST_ASSERT_EQUAL_INT(209, fr_rep_year_approx(2000));
}

/* ===== Additional boundary tests ===== */

void test_from_jd_end_of_month_boundary(void)
{
    /* 30 Vendemiaire An I (last day of first month) */
    fr_date_t d = fr_from_jd(2375839.5 + 29.0);
    TEST_ASSERT_EQUAL_INT(1, d.year);
    TEST_ASSERT_EQUAL_INT(1, d.month);
    TEST_ASSERT_EQUAL_INT(30, d.day);
}

void test_from_jd_start_of_second_month(void)
{
    /* 1 Brumaire An I (first day of second month) */
    fr_date_t d = fr_from_jd(2375839.5 + 30.0);
    TEST_ASSERT_EQUAL_INT(1, d.year);
    TEST_ASSERT_EQUAL_INT(2, d.month);
    TEST_ASSERT_EQUAL_INT(1, d.day);
}

void test_to_jd_and_from_jd_all_months_an_1(void)
{
    /* Round-trip first day of every month in An I */
    for (int m = 1; m <= 12; m++) {
        fr_date_t d = {1, m, 1};
        fr_date_t r = fr_from_jd(fr_to_jd(d));
        TEST_ASSERT_EQUAL_INT(1, r.year);
        TEST_ASSERT_EQUAL_INT(m, r.month);
        TEST_ASSERT_EQUAL_INT(1, r.day);
    }
}

void test_decade_day_day_20(void)
{
    /* Day 20 -> decade position 10 (end of 2nd decade) */
    fr_date_t d = {1, 3, 20};
    TEST_ASSERT_EQUAL_INT(10, fr_decade_day(d));
}

void test_decade_day_day_21(void)
{
    /* Day 21 -> decade position 1 (start of 3rd decade) */
    fr_date_t d = {1, 3, 21};
    TEST_ASSERT_EQUAL_INT(1, fr_decade_day(d));
}

void test_decimal_time_clamped_negative(void)
{
    /* Negative fraction -> treated as 0 */
    fr_decimal_time_t dt = fr_decimal_time(-0.5);
    TEST_ASSERT_EQUAL_INT(0, dt.hours);
    TEST_ASSERT_EQUAL_INT(0, dt.minutes);
    TEST_ASSERT_EQUAL_INT(0, dt.seconds);
}

void test_decimal_time_clamped_above_one(void)
{
    /* Fraction >= 1.0 -> clamped to 9h 99m 99s */
    fr_decimal_time_t dt = fr_decimal_time(1.5);
    TEST_ASSERT_EQUAL_INT(9, dt.hours);
    TEST_ASSERT_EQUAL_INT(99, dt.minutes);
    TEST_ASSERT_EQUAL_INT(99, dt.seconds);
}

void test_leap_year_an_15_leap(void)
{
    /* An XV: (15+1)=16, 16%4=0 -> leap */
    TEST_ASSERT_TRUE(fr_is_leap_year(15));
}

void test_leap_year_an_199_century_not_leap(void)
{
    /* An 199: (199+1)=200, 200%100=0, 200%400!=0 -> not leap */
    TEST_ASSERT_FALSE(fr_is_leap_year(199));
}

/* ===== Runner ===== */

int main(void)
{
    UNITY_BEGIN();

    /* Month names (15 tests) */
    RUN_TEST(test_month_name_vendemiaire);
    RUN_TEST(test_month_name_brumaire);
    RUN_TEST(test_month_name_frimaire);
    RUN_TEST(test_month_name_nivose);
    RUN_TEST(test_month_name_pluviose);
    RUN_TEST(test_month_name_ventose);
    RUN_TEST(test_month_name_germinal);
    RUN_TEST(test_month_name_floreal);
    RUN_TEST(test_month_name_prairial);
    RUN_TEST(test_month_name_messidor);
    RUN_TEST(test_month_name_thermidor);
    RUN_TEST(test_month_name_fructidor);
    RUN_TEST(test_month_name_sansculottides);
    RUN_TEST(test_month_name_invalid_negative);
    RUN_TEST(test_month_name_invalid_high);

    /* Month meanings (14 tests) */
    RUN_TEST(test_month_meaning_vendemiaire);
    RUN_TEST(test_month_meaning_brumaire);
    RUN_TEST(test_month_meaning_frimaire);
    RUN_TEST(test_month_meaning_nivose);
    RUN_TEST(test_month_meaning_pluviose);
    RUN_TEST(test_month_meaning_ventose);
    RUN_TEST(test_month_meaning_germinal);
    RUN_TEST(test_month_meaning_floreal);
    RUN_TEST(test_month_meaning_prairial);
    RUN_TEST(test_month_meaning_messidor);
    RUN_TEST(test_month_meaning_thermidor);
    RUN_TEST(test_month_meaning_fructidor);
    RUN_TEST(test_month_meaning_sansculottides);
    RUN_TEST(test_month_meaning_invalid);

    /* Sansculottide names (8 tests) */
    RUN_TEST(test_sansculottide_name_vertu);
    RUN_TEST(test_sansculottide_name_genie);
    RUN_TEST(test_sansculottide_name_travail);
    RUN_TEST(test_sansculottide_name_opinion);
    RUN_TEST(test_sansculottide_name_recompenses);
    RUN_TEST(test_sansculottide_name_revolution);
    RUN_TEST(test_sansculottide_name_invalid_zero);
    RUN_TEST(test_sansculottide_name_invalid_high);

    /* Decade day names (12 tests) */
    RUN_TEST(test_decade_day_name_primidi);
    RUN_TEST(test_decade_day_name_duodi);
    RUN_TEST(test_decade_day_name_tridi);
    RUN_TEST(test_decade_day_name_quartidi);
    RUN_TEST(test_decade_day_name_quintidi);
    RUN_TEST(test_decade_day_name_sextidi);
    RUN_TEST(test_decade_day_name_septidi);
    RUN_TEST(test_decade_day_name_octidi);
    RUN_TEST(test_decade_day_name_nonidi);
    RUN_TEST(test_decade_day_name_decadi);
    RUN_TEST(test_decade_day_name_invalid_zero);
    RUN_TEST(test_decade_day_name_invalid_high);

    /* Leap year (10 tests) */
    RUN_TEST(test_leap_year_an_1_not_leap);
    RUN_TEST(test_leap_year_an_3_leap);
    RUN_TEST(test_leap_year_an_7_leap);
    RUN_TEST(test_leap_year_an_11_leap);
    RUN_TEST(test_leap_year_an_4_not_leap);
    RUN_TEST(test_leap_year_an_8_not_leap);
    RUN_TEST(test_leap_year_an_99_century_not_leap);
    RUN_TEST(test_leap_year_an_399_century_leap);
    RUN_TEST(test_leap_year_an_15_leap);
    RUN_TEST(test_leap_year_an_199_century_not_leap);

    /* Year/month days (4 tests) */
    RUN_TEST(test_year_days_normal);
    RUN_TEST(test_year_days_leap);
    RUN_TEST(test_month_days_regular_month);
    RUN_TEST(test_month_days_sansculottides_normal);
    RUN_TEST(test_month_days_sansculottides_leap);

    /* Forward conversion: to_jd (8 tests) */
    RUN_TEST(test_to_jd_epoch);
    RUN_TEST(test_to_jd_thermidor_an_2);
    RUN_TEST(test_to_jd_brumaire_an_8);
    RUN_TEST(test_to_jd_nivose_an_14);
    RUN_TEST(test_to_jd_sansculottide_normal_year);
    RUN_TEST(test_to_jd_sansculottide_5th_day);
    RUN_TEST(test_to_jd_sansculottide_leap_6th_day);
    RUN_TEST(test_to_jd_last_day_of_fructidor);

    /* Reverse conversion: from_jd (11 tests) */
    RUN_TEST(test_from_jd_epoch);
    RUN_TEST(test_from_jd_thermidor_an_2);
    RUN_TEST(test_from_jd_brumaire_an_8);
    RUN_TEST(test_from_jd_nivose_an_14);
    RUN_TEST(test_from_jd_sansculottide);
    RUN_TEST(test_from_jd_5th_sansculottide);
    RUN_TEST(test_from_jd_6th_sansculottide_leap);
    RUN_TEST(test_from_jd_first_day_an_2);
    RUN_TEST(test_from_jd_first_day_an_4);
    RUN_TEST(test_from_jd_end_of_month_boundary);
    RUN_TEST(test_from_jd_start_of_second_month);

    /* Round-trip (7 tests) */
    RUN_TEST(test_roundtrip_epoch);
    RUN_TEST(test_roundtrip_mid_year);
    RUN_TEST(test_roundtrip_sansculottide);
    RUN_TEST(test_roundtrip_leap_sansculottide);
    RUN_TEST(test_roundtrip_last_regular_day);
    RUN_TEST(test_roundtrip_first_day_month);
    RUN_TEST(test_to_jd_and_from_jd_all_months_an_1);

    /* Decade day (5 tests) */
    RUN_TEST(test_decade_day_first_of_month);
    RUN_TEST(test_decade_day_tenth);
    RUN_TEST(test_decade_day_eleventh);
    RUN_TEST(test_decade_day_thirtieth);
    RUN_TEST(test_decade_day_sansculottide_returns_zero);
    RUN_TEST(test_decade_day_day_20);
    RUN_TEST(test_decade_day_day_21);

    /* Decimal time (9 tests) */
    RUN_TEST(test_decimal_time_midnight);
    RUN_TEST(test_decimal_time_noon);
    RUN_TEST(test_decimal_time_quarter_day);
    RUN_TEST(test_decimal_time_end_of_day);
    RUN_TEST(test_decimal_time_one_decimal_second);
    RUN_TEST(test_decimal_time_arbitrary);
    RUN_TEST(test_decimal_time_clamped_negative);
    RUN_TEST(test_decimal_time_clamped_above_one);

    /* Decimal to standard (3 tests) */
    RUN_TEST(test_decimal_to_standard_midnight);
    RUN_TEST(test_decimal_to_standard_noon);
    RUN_TEST(test_decimal_to_standard_quarter);

    /* is_sansculottide (2 tests) */
    RUN_TEST(test_is_sansculottide_true);
    RUN_TEST(test_is_sansculottide_false);

    /* CE/Rep year approx (5 tests) */
    RUN_TEST(test_ce_year_approx_an_1);
    RUN_TEST(test_ce_year_approx_an_14);
    RUN_TEST(test_ce_year_approx_an_100);
    RUN_TEST(test_rep_year_approx_1792);
    RUN_TEST(test_rep_year_approx_1805);
    RUN_TEST(test_rep_year_approx_2000);

    return UNITY_END();
}
