#include "../unity/unity.h"
#include "../../src/math/julian.h"
#include <math.h>

#define JD_EPSILON 0.001

void setUp(void) { }
void tearDown(void) { }

/* --- Meeus worked examples (card 006 Q3) --- */

void test_j2000_epoch(void) {
    /* 2000 January 1.5 = JD 2451545.0 */
    double jd = gregorian_to_jd(2000, 1, 1.5);
    TEST_ASSERT_FLOAT_WITHIN(JD_EPSILON, 2451545.0, jd);
}

void test_sputnik_launch(void) {
    /* 1957 October 4.81 = JD 2436116.31 */
    double jd = gregorian_to_jd(1957, 10, 4.81);
    TEST_ASSERT_FLOAT_WITHIN(JD_EPSILON, 2436116.31, jd);
}

void test_julian_calendar_333(void) {
    /* 333 January 27.5 (Julian calendar) = JD 1842713.0 */
    double jd = julian_cal_to_jd(333, 1, 27.5);
    TEST_ASSERT_FLOAT_WITHIN(JD_EPSILON, 1842713.0, jd);
}

/* --- Additional known dates --- */

void test_gregorian_reform_date(void) {
    /* 1582 October 15.0 = JD 2299160.5 (first day of Gregorian calendar) */
    double jd = gregorian_to_jd(1582, 10, 15.0);
    TEST_ASSERT_FLOAT_WITHIN(JD_EPSILON, 2299160.5, jd);
}

void test_unix_epoch(void) {
    /* 1970 January 1.5 = JD 2440588.0 */
    double jd = gregorian_to_jd(1970, 1, 1.5);
    TEST_ASSERT_FLOAT_WITHIN(JD_EPSILON, 2440588.0, jd);
}

/* --- Round-trip tests --- */

void test_roundtrip_j2000(void) {
    int year, month;
    double day = jd_to_gregorian(2451545.0, &year, &month);
    TEST_ASSERT_EQUAL_INT(2000, year);
    TEST_ASSERT_EQUAL_INT(1, month);
    TEST_ASSERT_FLOAT_WITHIN(JD_EPSILON, 1.5, day);
}

void test_roundtrip_sputnik(void) {
    int year, month;
    double day = jd_to_gregorian(2436116.31, &year, &month);
    TEST_ASSERT_EQUAL_INT(1957, year);
    TEST_ASSERT_EQUAL_INT(10, month);
    TEST_ASSERT_FLOAT_WITHIN(JD_EPSILON, 4.81, day);
}

void test_roundtrip_today(void) {
    /* Encode then decode a recent date */
    double jd = gregorian_to_jd(2026, 3, 6.0);
    int year, month;
    double day = jd_to_gregorian(jd, &year, &month);
    TEST_ASSERT_EQUAL_INT(2026, year);
    TEST_ASSERT_EQUAL_INT(3, month);
    TEST_ASSERT_FLOAT_WITHIN(JD_EPSILON, 6.0, day);
}

/* --- February edge cases --- */

void test_leap_year_feb_29(void) {
    double jd = gregorian_to_jd(2000, 2, 29.0);
    int year, month;
    double day = jd_to_gregorian(jd, &year, &month);
    TEST_ASSERT_EQUAL_INT(2000, year);
    TEST_ASSERT_EQUAL_INT(2, month);
    TEST_ASSERT_FLOAT_WITHIN(JD_EPSILON, 29.0, day);
}

void test_century_non_leap(void) {
    /* 1900 is NOT a leap year in Gregorian */
    double jd_feb28 = gregorian_to_jd(1900, 2, 28.0);
    double jd_mar1 = gregorian_to_jd(1900, 3, 1.0);
    /* Should be 1 day apart (no Feb 29) */
    TEST_ASSERT_FLOAT_WITHIN(JD_EPSILON, 1.0, jd_mar1 - jd_feb28);
}

/* --- Runner --- */

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_j2000_epoch);
    RUN_TEST(test_sputnik_launch);
    RUN_TEST(test_julian_calendar_333);
    RUN_TEST(test_gregorian_reform_date);
    RUN_TEST(test_unix_epoch);
    RUN_TEST(test_roundtrip_j2000);
    RUN_TEST(test_roundtrip_sputnik);
    RUN_TEST(test_roundtrip_today);
    RUN_TEST(test_leap_year_feb_29);
    RUN_TEST(test_century_non_leap);
    return UNITY_END();
}
