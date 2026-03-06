/* test_hebrew.c — Hebrew lunisolar calendar tests
 * TDD RED phase: tests written before implementation.
 * Algorithm: Dershowitz & Reingold "Calendrical Calculations" */

#include "../../unity/unity.h"
#include "../../../src/systems/hebrew/hebrew.h"
#include <math.h>
#include <string.h>

/* R.D. to JD offset (same constant used in implementation) */
static const double RD_JD_OFFSET = 1721424.5;

void setUp(void) {}
void tearDown(void) {}

/* ===== Leap year tests (Metonic cycle) ===== */

void test_leap_year_5784(void)
{
    /* (7*5784+1)%19 = 0 < 7 => LEAP */
    TEST_ASSERT_TRUE(hebrew_is_leap(5784));
}

void test_common_year_5785(void)
{
    /* (7*5785+1)%19 = 7, not < 7 => COMMON */
    TEST_ASSERT_FALSE(hebrew_is_leap(5785));
}

void test_leap_year_5782(void)
{
    /* (7*5782+1)%19 = 5 < 7 => LEAP */
    TEST_ASSERT_TRUE(hebrew_is_leap(5782));
}

void test_common_year_5786(void)
{
    /* (7*5786+1)%19 = 14, not < 7 => COMMON */
    TEST_ASSERT_FALSE(hebrew_is_leap(5786));
}

void test_leap_year_5787(void)
{
    /* (7*5787+1)%19 = 2 < 7 => LEAP */
    TEST_ASSERT_TRUE(hebrew_is_leap(5787));
}

/* ===== Months in year ===== */

void test_months_common(void)
{
    TEST_ASSERT_EQUAL_INT(12, hebrew_months_in_year(5785));
}

void test_months_leap(void)
{
    TEST_ASSERT_EQUAL_INT(13, hebrew_months_in_year(5784));
}

/* ===== New year (1 Tishrei) R.D. values ===== */

void test_new_year_5783(void)
{
    /* 1 Tishrei 5783 = Sep 26, 2022 = JD 2459848.5 => RD 738424 */
    int ny = hebrew_new_year(5783);
    TEST_ASSERT_EQUAL_INT(738424, ny);
}

void test_new_year_5784(void)
{
    /* 1 Tishrei 5784 = Sep 16, 2023 = JD 2460203.5 => RD 738779 */
    int ny = hebrew_new_year(5784);
    TEST_ASSERT_EQUAL_INT(738779, ny);
}

void test_new_year_5785(void)
{
    /* 1 Tishrei 5785 = Oct 3, 2024 = JD 2460586.5 => RD 739162 */
    int ny = hebrew_new_year(5785);
    TEST_ASSERT_EQUAL_INT(739162, ny);
}

void test_new_year_5776(void)
{
    /* 1 Tishrei 5776 = Sep 14, 2015 = JD 2457279.5 => RD 735855 */
    int ny = hebrew_new_year(5776);
    TEST_ASSERT_EQUAL_INT(735855, ny);
}

/* ===== Year days and year type ===== */

void test_year_days_5783(void)
{
    /* 5783 = 355 days (abundant common) */
    TEST_ASSERT_EQUAL_INT(355, hebrew_year_days(5783));
}

void test_year_days_5784(void)
{
    /* 5784 = 383 days (deficient leap) */
    TEST_ASSERT_EQUAL_INT(383, hebrew_year_days(5784));
}

void test_year_days_5785(void)
{
    /* 5785 = 355 days (abundant common) */
    TEST_ASSERT_EQUAL_INT(355, hebrew_year_days(5785));
}

void test_year_type_abundant(void)
{
    /* 5783: 355 days => ABUNDANT */
    TEST_ASSERT_EQUAL_INT(HEBREW_ABUNDANT, hebrew_year_type(5783));
}

void test_year_type_deficient_leap(void)
{
    /* 5784: 383 days => 383%10=3, 3+350=353 => DEFICIENT */
    TEST_ASSERT_EQUAL_INT(HEBREW_DEFICIENT, hebrew_year_type(5784));
}

void test_year_type_abundant_5785(void)
{
    /* 5785: 355 days => ABUNDANT */
    TEST_ASSERT_EQUAL_INT(HEBREW_ABUNDANT, hebrew_year_type(5785));
}

/* ===== from_jd tests ===== */

void test_from_jd_rosh_hashana_5785(void)
{
    /* JD 2460586.5 => 1 Tishrei 5785 */
    hebrew_date_t d = hebrew_from_jd(2460586.5);
    TEST_ASSERT_EQUAL_INT(5785, d.year);
    TEST_ASSERT_EQUAL_INT(7, d.month);
    TEST_ASSERT_EQUAL_INT(1, d.day);
}

void test_from_jd_rosh_hashana_5784(void)
{
    /* JD 2460203.5 => 1 Tishrei 5784 */
    hebrew_date_t d = hebrew_from_jd(2460203.5);
    TEST_ASSERT_EQUAL_INT(5784, d.year);
    TEST_ASSERT_EQUAL_INT(7, d.month);
    TEST_ASSERT_EQUAL_INT(1, d.day);
}

void test_from_jd_rosh_hashana_5783(void)
{
    /* JD 2459848.5 => 1 Tishrei 5783 */
    hebrew_date_t d = hebrew_from_jd(2459848.5);
    TEST_ASSERT_EQUAL_INT(5783, d.year);
    TEST_ASSERT_EQUAL_INT(7, d.month);
    TEST_ASSERT_EQUAL_INT(1, d.day);
}

void test_from_jd_passover_5784(void)
{
    /* JD 2460423.5 => 15 Nisan 5784 (Passover) */
    hebrew_date_t d = hebrew_from_jd(2460423.5);
    TEST_ASSERT_EQUAL_INT(5784, d.year);
    TEST_ASSERT_EQUAL_INT(1, d.month);
    TEST_ASSERT_EQUAL_INT(15, d.day);
}

void test_from_jd_yom_kippur_5785(void)
{
    /* JD 2460595.5 => 10 Tishrei 5785 (Yom Kippur) */
    hebrew_date_t d = hebrew_from_jd(2460595.5);
    TEST_ASSERT_EQUAL_INT(5785, d.year);
    TEST_ASSERT_EQUAL_INT(7, d.month);
    TEST_ASSERT_EQUAL_INT(10, d.day);
}

/* ===== to_jd tests ===== */

void test_to_jd_rosh_hashana_5785(void)
{
    hebrew_date_t d = { .year = 5785, .month = 7, .day = 1 };
    /* Expected JD 2460586.5 => RD 739162 */
    int rd = (int)floor(hebrew_to_jd(d) - RD_JD_OFFSET);
    TEST_ASSERT_EQUAL_INT(739162, rd);
}

void test_to_jd_rosh_hashana_5784(void)
{
    hebrew_date_t d = { .year = 5784, .month = 7, .day = 1 };
    int rd = (int)floor(hebrew_to_jd(d) - RD_JD_OFFSET);
    TEST_ASSERT_EQUAL_INT(738779, rd);
}

void test_to_jd_passover_5784(void)
{
    hebrew_date_t d = { .year = 5784, .month = 1, .day = 15 };
    /* Expected JD 2460423.5 => RD 738999 */
    int rd = (int)floor(hebrew_to_jd(d) - RD_JD_OFFSET);
    TEST_ASSERT_EQUAL_INT(738999, rd);
}

void test_to_jd_yom_kippur_5785(void)
{
    hebrew_date_t d = { .year = 5785, .month = 7, .day = 10 };
    /* Expected JD 2460595.5 => RD 739171 */
    int rd = (int)floor(hebrew_to_jd(d) - RD_JD_OFFSET);
    TEST_ASSERT_EQUAL_INT(739171, rd);
}

/* ===== Round trip ===== */

void test_round_trip_rosh_hashana(void)
{
    double jd_orig = 2460586.5;
    hebrew_date_t d = hebrew_from_jd(jd_orig);
    int rd_orig = (int)floor(jd_orig - RD_JD_OFFSET);
    int rd_back = (int)floor(hebrew_to_jd(d) - RD_JD_OFFSET);
    TEST_ASSERT_EQUAL_INT(rd_orig, rd_back);
}

void test_round_trip_passover(void)
{
    double jd_orig = 2460423.5;
    hebrew_date_t d = hebrew_from_jd(jd_orig);
    int rd_orig = (int)floor(jd_orig - RD_JD_OFFSET);
    int rd_back = (int)floor(hebrew_to_jd(d) - RD_JD_OFFSET);
    TEST_ASSERT_EQUAL_INT(rd_orig, rd_back);
}

void test_round_trip_many_dates(void)
{
    /* Test round-trip for 100 consecutive days starting from 1 Tishrei 5784 */
    double start_jd = 2460203.5;
    int i;
    for (i = 0; i < 100; i++) {
        double jd = start_jd + (double)i;
        hebrew_date_t d = hebrew_from_jd(jd);
        int rd_orig = (int)floor(jd - RD_JD_OFFSET);
        int rd_back = (int)floor(hebrew_to_jd(d) - RD_JD_OFFSET);
        TEST_ASSERT_EQUAL_INT(rd_orig, rd_back);
    }
}

/* ===== Month name tests ===== */

void test_month_name_tishrei(void)
{
    const char *name = hebrew_month_name(7, false);
    TEST_ASSERT_EQUAL_STRING("Tishrei", name);
}

void test_month_name_nisan(void)
{
    const char *name = hebrew_month_name(1, false);
    TEST_ASSERT_EQUAL_STRING("Nisan", name);
}

void test_month_name_adar_common(void)
{
    const char *name = hebrew_month_name(12, false);
    TEST_ASSERT_EQUAL_STRING("Adar", name);
}

void test_month_name_adar_leap(void)
{
    const char *name = hebrew_month_name(12, true);
    TEST_ASSERT_EQUAL_STRING("Adar I", name);
}

void test_month_name_adar2(void)
{
    const char *name = hebrew_month_name(13, true);
    TEST_ASSERT_EQUAL_STRING("Adar II", name);
}

/* ===== Month days tests ===== */

void test_month_days_tishrei(void)
{
    /* Tishrei always 30 */
    TEST_ASSERT_EQUAL_INT(30, hebrew_month_days(5783, 7));
}

void test_month_days_nisan(void)
{
    /* Nisan always 30 */
    TEST_ASSERT_EQUAL_INT(30, hebrew_month_days(5783, 1));
}

void test_month_days_cheshvan_abundant(void)
{
    /* 5783 is abundant (355): Cheshvan = 30 */
    TEST_ASSERT_EQUAL_INT(30, hebrew_month_days(5783, 8));
}

void test_month_days_cheshvan_deficient(void)
{
    /* 5784 is deficient (383 leap): Cheshvan = 29 */
    TEST_ASSERT_EQUAL_INT(29, hebrew_month_days(5784, 8));
}

void test_month_days_kislev_abundant(void)
{
    /* 5783 is abundant: Kislev = 30 */
    TEST_ASSERT_EQUAL_INT(30, hebrew_month_days(5783, 9));
}

void test_month_days_kislev_deficient(void)
{
    /* 5784 is deficient: Kislev = 29 */
    TEST_ASSERT_EQUAL_INT(29, hebrew_month_days(5784, 9));
}

void test_month_days_adar_common(void)
{
    /* Adar in common year = 29 */
    TEST_ASSERT_EQUAL_INT(29, hebrew_month_days(5785, 12));
}

void test_month_days_adar_leap(void)
{
    /* Adar I in leap year = 30 */
    TEST_ASSERT_EQUAL_INT(30, hebrew_month_days(5784, 12));
}

void test_month_days_adar2(void)
{
    /* Adar II in leap year = 29 */
    TEST_ASSERT_EQUAL_INT(29, hebrew_month_days(5784, 13));
}

/* ===== Molad tests ===== */

void test_molad_tishrei_5785_positive(void)
{
    double molad = hebrew_molad_tishrei(5785);
    TEST_ASSERT_TRUE(molad > 0.0);
}

void test_molad_tishrei_increases(void)
{
    /* Molad of Tishrei should increase each year */
    double m1 = hebrew_molad_tishrei(5784);
    double m2 = hebrew_molad_tishrei(5785);
    TEST_ASSERT_TRUE(m2 > m1);
}

/* ===== Edge cases ===== */

void test_invalid_month_zero(void)
{
    /* Month 0 should return safe default name */
    const char *name = hebrew_month_name(0, false);
    TEST_ASSERT_NOT_NULL(name);
}

void test_invalid_month_14(void)
{
    /* Month 14 should return safe default name */
    const char *name = hebrew_month_name(14, false);
    TEST_ASSERT_NOT_NULL(name);
}

void test_month_days_invalid_returns_zero(void)
{
    /* Invalid month should return 0 */
    TEST_ASSERT_EQUAL_INT(0, hebrew_month_days(5783, 0));
    TEST_ASSERT_EQUAL_INT(0, hebrew_month_days(5783, 14));
}

void test_month_13_common_year(void)
{
    /* Month 13 in common year has 0 days (doesn't exist) */
    TEST_ASSERT_EQUAL_INT(0, hebrew_month_days(5785, 13));
}

int main(void)
{
    UNITY_BEGIN();

    /* Leap year */
    RUN_TEST(test_leap_year_5784);
    RUN_TEST(test_common_year_5785);
    RUN_TEST(test_leap_year_5782);
    RUN_TEST(test_common_year_5786);
    RUN_TEST(test_leap_year_5787);

    /* Months in year */
    RUN_TEST(test_months_common);
    RUN_TEST(test_months_leap);

    /* New year */
    RUN_TEST(test_new_year_5783);
    RUN_TEST(test_new_year_5784);
    RUN_TEST(test_new_year_5785);
    RUN_TEST(test_new_year_5776);

    /* Year days and type */
    RUN_TEST(test_year_days_5783);
    RUN_TEST(test_year_days_5784);
    RUN_TEST(test_year_days_5785);
    RUN_TEST(test_year_type_abundant);
    RUN_TEST(test_year_type_deficient_leap);
    RUN_TEST(test_year_type_abundant_5785);

    /* from_jd */
    RUN_TEST(test_from_jd_rosh_hashana_5785);
    RUN_TEST(test_from_jd_rosh_hashana_5784);
    RUN_TEST(test_from_jd_rosh_hashana_5783);
    RUN_TEST(test_from_jd_passover_5784);
    RUN_TEST(test_from_jd_yom_kippur_5785);

    /* to_jd */
    RUN_TEST(test_to_jd_rosh_hashana_5785);
    RUN_TEST(test_to_jd_rosh_hashana_5784);
    RUN_TEST(test_to_jd_passover_5784);
    RUN_TEST(test_to_jd_yom_kippur_5785);

    /* Round trip */
    RUN_TEST(test_round_trip_rosh_hashana);
    RUN_TEST(test_round_trip_passover);
    RUN_TEST(test_round_trip_many_dates);

    /* Month names */
    RUN_TEST(test_month_name_tishrei);
    RUN_TEST(test_month_name_nisan);
    RUN_TEST(test_month_name_adar_common);
    RUN_TEST(test_month_name_adar_leap);
    RUN_TEST(test_month_name_adar2);

    /* Month days */
    RUN_TEST(test_month_days_tishrei);
    RUN_TEST(test_month_days_nisan);
    RUN_TEST(test_month_days_cheshvan_abundant);
    RUN_TEST(test_month_days_cheshvan_deficient);
    RUN_TEST(test_month_days_kislev_abundant);
    RUN_TEST(test_month_days_kislev_deficient);
    RUN_TEST(test_month_days_adar_common);
    RUN_TEST(test_month_days_adar_leap);
    RUN_TEST(test_month_days_adar2);

    /* Molad */
    RUN_TEST(test_molad_tishrei_5785_positive);
    RUN_TEST(test_molad_tishrei_increases);

    /* Edge cases */
    RUN_TEST(test_invalid_month_zero);
    RUN_TEST(test_invalid_month_14);
    RUN_TEST(test_month_days_invalid_returns_zero);
    RUN_TEST(test_month_13_common_year);

    return UNITY_END();
}
