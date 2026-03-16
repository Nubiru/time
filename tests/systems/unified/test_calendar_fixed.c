/* test_calendar_fixed.c — Tests for shared fixed-year calendar helpers
 *
 * Validates both non-leap (Egyptian/Zoroastrian) and leap-4 (Coptic/Ethiopian)
 * conversion functions, plus cross-verification against existing calendar APIs.
 *
 * Sources: Dershowitz & Reingold, "Calendrical Calculations" (2018). */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/calendar_fixed.h"
#include "../../../src/systems/egyptian/egyptian.h"
#include "../../../src/systems/coptic/coptic.h"
#include "../../../src/systems/ethiopian/ethiopian.h"
#include "../../../src/systems/zoroastrian/zoroastrian.h"
#include <math.h>

/* Epoch JDs */
#define EGYPT_EPOCH_JD      1448638.0
#define ZORO_EPOCH_JD       1952063.5
#define COPTIC_EPOCH_JD     1825029.5
#define ETHIOPIAN_EPOCH_JD  1724220.5

#define JD_EPSILON 0.0001

void setUp(void) {}
void tearDown(void) {}

/* ===== Non-leap from_jd ===== */

void test_fixed_from_jd_epoch(void)
{
    /* Epoch -> year 1, month 1, day 1 */
    cf_date_t d = cf_fixed_from_jd(EGYPT_EPOCH_JD, EGYPT_EPOCH_JD);
    TEST_ASSERT_EQUAL_INT(1, d.year);
    TEST_ASSERT_EQUAL_INT(1, d.month);
    TEST_ASSERT_EQUAL_INT(1, d.day);
}

/* ===== Non-leap to_jd ===== */

void test_fixed_to_jd_epoch(void)
{
    /* Year 1, month 1, day 1 -> epoch */
    cf_date_t d = { 1, 1, 1 };
    double jd = cf_fixed_to_jd(d, EGYPT_EPOCH_JD);
    TEST_ASSERT_FLOAT_WITHIN(JD_EPSILON, EGYPT_EPOCH_JD, jd);
}

/* ===== Non-leap round-trip ===== */

void test_fixed_roundtrip_500(void)
{
    /* 500 dates sweep: to_jd -> from_jd must recover original */
    for (int i = 0; i < 500; i++) {
        cf_date_t orig;
        orig.year = 1 + (i * 7) % 3000;
        orig.month = 1 + (i * 3) % 12;
        orig.day = 1 + (i * 5) % 30;

        double jd = cf_fixed_to_jd(orig, EGYPT_EPOCH_JD);
        cf_date_t back = cf_fixed_from_jd(jd, EGYPT_EPOCH_JD);

        TEST_ASSERT_EQUAL_INT(orig.year, back.year);
        TEST_ASSERT_EQUAL_INT(orig.month, back.month);
        TEST_ASSERT_EQUAL_INT(orig.day, back.day);
    }
}

/* ===== Non-leap epagomenal ===== */

void test_fixed_epagomenal(void)
{
    /* Days 361-365 -> month 13 */
    for (int i = 0; i < 5; i++) {
        cf_date_t d = cf_fixed_from_jd(EGYPT_EPOCH_JD + 360.0 + i, EGYPT_EPOCH_JD);
        TEST_ASSERT_EQUAL_INT(1, d.year);
        TEST_ASSERT_EQUAL_INT(13, d.month);
        TEST_ASSERT_EQUAL_INT(i + 1, d.day);
    }
}

/* ===== Non-leap negative years ===== */

void test_fixed_negative_years(void)
{
    /* One day before epoch -> year 0 or negative, last day of prior year */
    cf_date_t d = cf_fixed_from_jd(EGYPT_EPOCH_JD - 1.0, EGYPT_EPOCH_JD);
    TEST_ASSERT_EQUAL_INT(0, d.year);
    TEST_ASSERT_EQUAL_INT(13, d.month);
    TEST_ASSERT_EQUAL_INT(5, d.day);
}

/* ===== Non-leap to_jd last day of year 1 ===== */

void test_fixed_to_jd_last_day(void)
{
    /* Last day of year 1 (month 13, day 5) -> epoch + 364 */
    cf_date_t d = { 1, 13, 5 };
    double jd = cf_fixed_to_jd(d, EGYPT_EPOCH_JD);
    TEST_ASSERT_FLOAT_WITHIN(JD_EPSILON, EGYPT_EPOCH_JD + 364.0, jd);
}

/* ===== Leap4 from_jd ===== */

void test_leap4_from_jd_epoch(void)
{
    /* Coptic epoch -> year 1, month 1, day 1 */
    cf_date_t d = cf_leap4_from_jd(COPTIC_EPOCH_JD, COPTIC_EPOCH_JD);
    TEST_ASSERT_EQUAL_INT(1, d.year);
    TEST_ASSERT_EQUAL_INT(1, d.month);
    TEST_ASSERT_EQUAL_INT(1, d.day);
}

/* ===== Leap4 to_jd ===== */

void test_leap4_to_jd_epoch(void)
{
    /* Year 1, month 1, day 1 -> Coptic epoch */
    cf_date_t d = { 1, 1, 1 };
    double jd = cf_leap4_to_jd(d, COPTIC_EPOCH_JD);
    TEST_ASSERT_FLOAT_WITHIN(JD_EPSILON, COPTIC_EPOCH_JD, jd);
}

/* ===== Leap4 round-trip ===== */

void test_leap4_roundtrip_500(void)
{
    /* 500 dates sweep: to_jd -> from_jd must recover original */
    for (int i = 0; i < 500; i++) {
        cf_date_t orig;
        orig.year = 1 + (i * 7) % 2000;
        orig.month = 1 + (i * 3) % 12;
        orig.day = 1 + (i * 5) % 30;

        double jd = cf_leap4_to_jd(orig, COPTIC_EPOCH_JD);
        cf_date_t back = cf_leap4_from_jd(jd, COPTIC_EPOCH_JD);

        TEST_ASSERT_EQUAL_INT(orig.year, back.year);
        TEST_ASSERT_EQUAL_INT(orig.month, back.month);
        TEST_ASSERT_EQUAL_INT(orig.day, back.day);
    }
}

/* ===== Leap4 is_leap ===== */

void test_leap4_is_leap(void)
{
    /* Year 3 = leap (3%4==3), year 4 = not (4%4==0) */
    TEST_ASSERT_TRUE(cf_is_leap4(3));
    TEST_ASSERT_FALSE(cf_is_leap4(4));
    TEST_ASSERT_TRUE(cf_is_leap4(7));
    TEST_ASSERT_FALSE(cf_is_leap4(1));
    TEST_ASSERT_FALSE(cf_is_leap4(2));
    TEST_ASSERT_TRUE(cf_is_leap4(11));
}

/* ===== Leap4 epagomenal ===== */

void test_leap4_epagomenal(void)
{
    /* Leap year (year 3) has 6 epagomenal days, common year has 5 */
    /* Year 3: last day should be month 13, day 6 */
    cf_date_t d_leap = { 3, 13, 6 };
    double jd_leap = cf_leap4_to_jd(d_leap, COPTIC_EPOCH_JD);
    cf_date_t back_leap = cf_leap4_from_jd(jd_leap, COPTIC_EPOCH_JD);
    TEST_ASSERT_EQUAL_INT(3, back_leap.year);
    TEST_ASSERT_EQUAL_INT(13, back_leap.month);
    TEST_ASSERT_EQUAL_INT(6, back_leap.day);

    /* Year 2: month 13 day 5 is last day */
    cf_date_t d_common = { 2, 13, 5 };
    double jd_common = cf_leap4_to_jd(d_common, COPTIC_EPOCH_JD);
    cf_date_t back_common = cf_leap4_from_jd(jd_common, COPTIC_EPOCH_JD);
    TEST_ASSERT_EQUAL_INT(2, back_common.year);
    TEST_ASSERT_EQUAL_INT(13, back_common.month);
    TEST_ASSERT_EQUAL_INT(5, back_common.day);
}

/* ===== Leap4 year_days ===== */

void test_leap4_year_days(void)
{
    TEST_ASSERT_EQUAL_INT(366, cf_year_days_leap4(3));
    TEST_ASSERT_EQUAL_INT(365, cf_year_days_leap4(4));
    TEST_ASSERT_EQUAL_INT(365, cf_year_days_leap4(1));
    TEST_ASSERT_EQUAL_INT(366, cf_year_days_leap4(7));
}

/* ===== cf_day_of_year ===== */

void test_cf_day_of_year(void)
{
    /* month 1, day 1 -> 1 */
    TEST_ASSERT_EQUAL_INT(1, cf_day_of_year(1, 1));
    /* month 1, day 30 -> 30 */
    TEST_ASSERT_EQUAL_INT(30, cf_day_of_year(1, 30));
    /* month 2, day 1 -> 31 */
    TEST_ASSERT_EQUAL_INT(31, cf_day_of_year(2, 1));
    /* month 12, day 30 -> 360 */
    TEST_ASSERT_EQUAL_INT(360, cf_day_of_year(12, 30));
    /* month 13, day 1 -> 361 */
    TEST_ASSERT_EQUAL_INT(361, cf_day_of_year(13, 1));
    /* month 13, day 5 -> 365 */
    TEST_ASSERT_EQUAL_INT(365, cf_day_of_year(13, 5));
}

/* ===== cf_month_days ===== */

void test_cf_month_days(void)
{
    /* Regular months 1-12 = 30 */
    for (int m = 1; m <= 12; m++) {
        TEST_ASSERT_EQUAL_INT(30, cf_month_days(1, m, 0));
        TEST_ASSERT_EQUAL_INT(30, cf_month_days(1, m, 1));
    }
    /* Month 13 non-leap calendar = 5 */
    TEST_ASSERT_EQUAL_INT(5, cf_month_days(1, 13, 0));
    TEST_ASSERT_EQUAL_INT(5, cf_month_days(3, 13, 0));

    /* Month 13 leap calendar: year 3 = 6, year 4 = 5 */
    TEST_ASSERT_EQUAL_INT(6, cf_month_days(3, 13, 1));
    TEST_ASSERT_EQUAL_INT(5, cf_month_days(4, 13, 1));

    /* Invalid month = 0 */
    TEST_ASSERT_EQUAL_INT(0, cf_month_days(1, 0, 0));
    TEST_ASSERT_EQUAL_INT(0, cf_month_days(1, 14, 0));
}

/* ===== Egyptian epoch verification ===== */

void test_egyptian_epoch_verify(void)
{
    cf_date_t cf = cf_fixed_from_jd(EGYPT_EPOCH_JD, EGYPT_EPOCH_JD);
    egypt_date_t eg = egypt_from_jd(EGYPT_EPOCH_JD);
    TEST_ASSERT_EQUAL_INT(eg.year, cf.year);
    TEST_ASSERT_EQUAL_INT(eg.month, cf.month);
    TEST_ASSERT_EQUAL_INT(eg.day, cf.day);
}

/* ===== Zoroastrian epoch verification ===== */

void test_zoroastrian_epoch_verify(void)
{
    cf_date_t cf = cf_fixed_from_jd(ZORO_EPOCH_JD, ZORO_EPOCH_JD);
    zoro_date_t zd = zoro_from_jd(ZORO_EPOCH_JD);
    TEST_ASSERT_EQUAL_INT(zd.year, cf.year);
    TEST_ASSERT_EQUAL_INT(zd.month, cf.month);
    TEST_ASSERT_EQUAL_INT(zd.day, cf.day);
}

/* ===== Coptic epoch verification ===== */

void test_coptic_epoch_verify(void)
{
    cf_date_t cf = cf_leap4_from_jd(COPTIC_EPOCH_JD, COPTIC_EPOCH_JD);
    coptic_date_t cd = coptic_from_jd(COPTIC_EPOCH_JD);
    TEST_ASSERT_EQUAL_INT(cd.year, cf.year);
    TEST_ASSERT_EQUAL_INT(cd.month, cf.month);
    TEST_ASSERT_EQUAL_INT(cd.day, cf.day);
}

/* ===== Ethiopian epoch verification ===== */

void test_ethiopian_epoch_verify(void)
{
    cf_date_t cf = cf_leap4_from_jd(ETHIOPIAN_EPOCH_JD, ETHIOPIAN_EPOCH_JD);
    ethiopian_date_t ed = ethiopian_from_jd(ETHIOPIAN_EPOCH_JD);
    TEST_ASSERT_EQUAL_INT(ed.year, cf.year);
    TEST_ASSERT_EQUAL_INT(ed.month, cf.month);
    TEST_ASSERT_EQUAL_INT(ed.day, cf.day);
}

/* ===== Cross-verification: Egyptian ===== */

void test_cross_verify_egyptian(void)
{
    /* 100 dates starting from Egyptian epoch */
    for (int i = 0; i < 100; i++) {
        double jd = EGYPT_EPOCH_JD + (double)(i * 137);
        egypt_date_t eg = egypt_from_jd(jd);
        cf_date_t cf = cf_fixed_from_jd(jd, EGYPT_EPOCH_JD);
        TEST_ASSERT_EQUAL_INT(eg.year, cf.year);
        TEST_ASSERT_EQUAL_INT(eg.month, cf.month);
        TEST_ASSERT_EQUAL_INT(eg.day, cf.day);
    }
}

/* ===== Cross-verification: Coptic ===== */

void test_cross_verify_coptic(void)
{
    /* 100 dates starting from Coptic epoch */
    for (int i = 0; i < 100; i++) {
        double jd = COPTIC_EPOCH_JD + (double)(i * 137);
        coptic_date_t cd = coptic_from_jd(jd);
        cf_date_t cf = cf_leap4_from_jd(jd, COPTIC_EPOCH_JD);
        TEST_ASSERT_EQUAL_INT(cd.year, cf.year);
        TEST_ASSERT_EQUAL_INT(cd.month, cf.month);
        TEST_ASSERT_EQUAL_INT(cd.day, cf.day);
    }
}

/* ===== Additional: Zoroastrian cross-verification ===== */

void test_cross_verify_zoroastrian(void)
{
    /* 100 dates starting from Zoroastrian epoch */
    for (int i = 0; i < 100; i++) {
        double jd = ZORO_EPOCH_JD + (double)(i * 137);
        zoro_date_t zd = zoro_from_jd(jd);
        cf_date_t cf = cf_fixed_from_jd(jd, ZORO_EPOCH_JD);
        TEST_ASSERT_EQUAL_INT(zd.year, cf.year);
        TEST_ASSERT_EQUAL_INT(zd.month, cf.month);
        TEST_ASSERT_EQUAL_INT(zd.day, cf.day);
    }
}

/* ===== Additional: Ethiopian cross-verification ===== */

void test_cross_verify_ethiopian(void)
{
    /* 100 dates starting from Ethiopian epoch */
    for (int i = 0; i < 100; i++) {
        double jd = ETHIOPIAN_EPOCH_JD + (double)(i * 137);
        ethiopian_date_t ed = ethiopian_from_jd(jd);
        cf_date_t cf = cf_leap4_from_jd(jd, ETHIOPIAN_EPOCH_JD);
        TEST_ASSERT_EQUAL_INT(ed.year, cf.year);
        TEST_ASSERT_EQUAL_INT(ed.month, cf.month);
        TEST_ASSERT_EQUAL_INT(ed.day, cf.day);
    }
}

/* ===== Non-leap boundary: year transition ===== */

void test_fixed_year_boundary(void)
{
    /* Day before year 2 should be year 1 epag 5 */
    cf_date_t d1 = cf_fixed_from_jd(EGYPT_EPOCH_JD + 364.0, EGYPT_EPOCH_JD);
    TEST_ASSERT_EQUAL_INT(1, d1.year);
    TEST_ASSERT_EQUAL_INT(13, d1.month);
    TEST_ASSERT_EQUAL_INT(5, d1.day);

    /* First day of year 2 */
    cf_date_t d2 = cf_fixed_from_jd(EGYPT_EPOCH_JD + 365.0, EGYPT_EPOCH_JD);
    TEST_ASSERT_EQUAL_INT(2, d2.year);
    TEST_ASSERT_EQUAL_INT(1, d2.month);
    TEST_ASSERT_EQUAL_INT(1, d2.day);
}

/* ===== Leap4 boundary: leap year transition ===== */

void test_leap4_year_boundary(void)
{
    /* Year 3 is a leap year (366 days). Check boundary. */
    cf_date_t d_y3_start = { 3, 1, 1 };
    double jd_y3 = cf_leap4_to_jd(d_y3_start, COPTIC_EPOCH_JD);

    /* Last day of year 3 (366th day = month 13, day 6) */
    cf_date_t d_y3_end = cf_leap4_from_jd(jd_y3 + 365.0, COPTIC_EPOCH_JD);
    TEST_ASSERT_EQUAL_INT(3, d_y3_end.year);
    TEST_ASSERT_EQUAL_INT(13, d_y3_end.month);
    TEST_ASSERT_EQUAL_INT(6, d_y3_end.day);

    /* First day of year 4 (day after leap year end) */
    cf_date_t d_y4_start = cf_leap4_from_jd(jd_y3 + 366.0, COPTIC_EPOCH_JD);
    TEST_ASSERT_EQUAL_INT(4, d_y4_start.year);
    TEST_ASSERT_EQUAL_INT(1, d_y4_start.month);
    TEST_ASSERT_EQUAL_INT(1, d_y4_start.day);
}

/* ===== Runner ===== */

int main(void)
{
    UNITY_BEGIN();

    /* Non-leap conversions */
    RUN_TEST(test_fixed_from_jd_epoch);
    RUN_TEST(test_fixed_to_jd_epoch);
    RUN_TEST(test_fixed_roundtrip_500);
    RUN_TEST(test_fixed_epagomenal);
    RUN_TEST(test_fixed_negative_years);
    RUN_TEST(test_fixed_to_jd_last_day);

    /* Leap-4 conversions */
    RUN_TEST(test_leap4_from_jd_epoch);
    RUN_TEST(test_leap4_to_jd_epoch);
    RUN_TEST(test_leap4_roundtrip_500);
    RUN_TEST(test_leap4_is_leap);
    RUN_TEST(test_leap4_epagomenal);
    RUN_TEST(test_leap4_year_days);

    /* Shared utilities */
    RUN_TEST(test_cf_day_of_year);
    RUN_TEST(test_cf_month_days);

    /* Epoch verifications */
    RUN_TEST(test_egyptian_epoch_verify);
    RUN_TEST(test_zoroastrian_epoch_verify);
    RUN_TEST(test_coptic_epoch_verify);
    RUN_TEST(test_ethiopian_epoch_verify);

    /* Cross-verifications */
    RUN_TEST(test_cross_verify_egyptian);
    RUN_TEST(test_cross_verify_coptic);
    RUN_TEST(test_cross_verify_zoroastrian);
    RUN_TEST(test_cross_verify_ethiopian);

    /* Boundary tests */
    RUN_TEST(test_fixed_year_boundary);
    RUN_TEST(test_leap4_year_boundary);

    return UNITY_END();
}
