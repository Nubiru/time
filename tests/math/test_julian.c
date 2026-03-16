#include "../unity/unity.h"
#include "../../src/math/julian.h"
#include <math.h>

#define JD_EPSILON 0.001

void setUp(void) { }
void tearDown(void) { }

void test_j2000_epoch(void) {
    double jd = gregorian_to_jd(2000, 1, 1.5);
    TEST_ASSERT_FLOAT_WITHIN(JD_EPSILON, 2451545.0, jd);
}

void test_sputnik_launch(void) {
    double jd = gregorian_to_jd(1957, 10, 4.81);
    TEST_ASSERT_FLOAT_WITHIN(JD_EPSILON, 2436116.31, jd);
}

void test_julian_calendar_333(void) {
    double jd = julian_cal_to_jd(333, 1, 27.5);
    TEST_ASSERT_FLOAT_WITHIN(JD_EPSILON, 1842713.0, jd);
}

void test_gregorian_reform_date(void) {
    double jd = gregorian_to_jd(1582, 10, 15.0);
    TEST_ASSERT_FLOAT_WITHIN(JD_EPSILON, 2299160.5, jd);
}

void test_unix_epoch(void) {
    double jd = gregorian_to_jd(1970, 1, 1.5);
    TEST_ASSERT_FLOAT_WITHIN(JD_EPSILON, 2440588.0, jd);
}

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
    double jd = gregorian_to_jd(2026, 3, 6.0);
    int year, month;
    double day = jd_to_gregorian(jd, &year, &month);
    TEST_ASSERT_EQUAL_INT(2026, year);
    TEST_ASSERT_EQUAL_INT(3, month);
    TEST_ASSERT_FLOAT_WITHIN(JD_EPSILON, 6.0, day);
}

void test_leap_year_feb_29(void) {
    double jd = gregorian_to_jd(2000, 2, 29.0);
    int year, month;
    double day = jd_to_gregorian(jd, &year, &month);
    TEST_ASSERT_EQUAL_INT(2000, year);
    TEST_ASSERT_EQUAL_INT(2, month);
    TEST_ASSERT_FLOAT_WITHIN(JD_EPSILON, 29.0, day);
}

void test_century_non_leap(void) {
    double jd_feb28 = gregorian_to_jd(1900, 2, 28.0);
    double jd_mar1 = gregorian_to_jd(1900, 3, 1.0);
    TEST_ASSERT_FLOAT_WITHIN(JD_EPSILON, 1.0, jd_mar1 - jd_feb28);
}

void test_meeus_example_1988(void) {
    double jd = gregorian_to_jd(1988, 6, 19.5);
    TEST_ASSERT_FLOAT_WITHIN(JD_EPSILON, 2447332.0, jd);
}

void test_julian_day_negative_year(void) {
    double jd = julian_cal_to_jd(-4712, 1, 1.5);
    TEST_ASSERT_FLOAT_WITHIN(JD_EPSILON, 0.0, jd);
}

void test_far_future_year_9999(void) {
    double jd = gregorian_to_jd(9999, 12, 31.0);
    TEST_ASSERT_TRUE(jd > 5373000.0);
}

void test_roundtrip_negative_year(void) {
    double jd = julian_cal_to_jd(-1000, 6, 15.0);
    TEST_ASSERT_TRUE(jd > 0.0);
}

void test_roundtrip_year_1583(void) {
    /* First full year in the Gregorian calendar */
    double jd = gregorian_to_jd(1583, 6, 15.0);
    int year, month;
    double day = jd_to_gregorian(jd, &year, &month);
    TEST_ASSERT_EQUAL_INT(1583, year);
    TEST_ASSERT_EQUAL_INT(6, month);
    TEST_ASSERT_FLOAT_WITHIN(JD_EPSILON, 15.0, day);
}

void test_roundtrip_leap_century_2000(void) {
    double jd = gregorian_to_jd(2000, 2, 29.5);
    int year, month;
    double day = jd_to_gregorian(jd, &year, &month);
    TEST_ASSERT_EQUAL_INT(2000, year);
    TEST_ASSERT_EQUAL_INT(2, month);
    TEST_ASSERT_FLOAT_WITHIN(JD_EPSILON, 29.5, day);
}

void test_roundtrip_many_dates(void) {
    int years[] = {1900, 1950, 1999, 2000, 2001, 2024, 2100};
    int months[] = {1, 3, 6, 9, 12};
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 5; j++) {
            double jd = gregorian_to_jd(years[i], months[j], 15.0);
            int y, m;
            double d = jd_to_gregorian(jd, &y, &m);
            TEST_ASSERT_EQUAL_INT(years[i], y);
            TEST_ASSERT_EQUAL_INT(months[j], m);
            TEST_ASSERT_FLOAT_WITHIN(JD_EPSILON, 15.0, d);
        }
    }
}

void test_consecutive_days(void) {
    for (int d = 1; d <= 28; d++) {
        double jd1 = gregorian_to_jd(2020, 1, (double)d);
        double jd2 = gregorian_to_jd(2020, 1, (double)(d + 1));
        TEST_ASSERT_FLOAT_WITHIN(JD_EPSILON, 1.0, jd2 - jd1);
    }
}

void test_month_boundaries(void) {
    double jd_jan31 = gregorian_to_jd(2023, 1, 31.0);
    double jd_feb1 = gregorian_to_jd(2023, 2, 1.0);
    TEST_ASSERT_FLOAT_WITHIN(JD_EPSILON, 1.0, jd_feb1 - jd_jan31);
    double jd_feb28 = gregorian_to_jd(2023, 2, 28.0);
    double jd_mar1 = gregorian_to_jd(2023, 3, 1.0);
    TEST_ASSERT_FLOAT_WITHIN(JD_EPSILON, 1.0, jd_mar1 - jd_feb28);
}

void test_leap_year_feb_29_2024(void) {
    double jd_feb28 = gregorian_to_jd(2024, 2, 28.0);
    double jd_feb29 = gregorian_to_jd(2024, 2, 29.0);
    double jd_mar1 = gregorian_to_jd(2024, 3, 1.0);
    TEST_ASSERT_FLOAT_WITHIN(JD_EPSILON, 1.0, jd_feb29 - jd_feb28);
    TEST_ASSERT_FLOAT_WITHIN(JD_EPSILON, 1.0, jd_mar1 - jd_feb29);
}

void test_fractional_day_noon(void) {
    double jd_midnight = gregorian_to_jd(2000, 1, 1.0);
    double jd_noon = gregorian_to_jd(2000, 1, 1.5);
    TEST_ASSERT_FLOAT_WITHIN(JD_EPSILON, 0.5, jd_noon - jd_midnight);
}

void test_fractional_day_quarter(void) {
    double jd_midnight = gregorian_to_jd(2000, 1, 1.0);
    double jd_6h = gregorian_to_jd(2000, 1, 1.25);
    TEST_ASSERT_FLOAT_WITHIN(JD_EPSILON, 0.25, jd_6h - jd_midnight);
}

void test_year_length_non_leap(void) {
    double jd1 = gregorian_to_jd(2023, 1, 1.0);
    double jd2 = gregorian_to_jd(2024, 1, 1.0);
    TEST_ASSERT_FLOAT_WITHIN(JD_EPSILON, 365.0, jd2 - jd1);
}

void test_year_length_leap(void) {
    double jd1 = gregorian_to_jd(2024, 1, 1.0);
    double jd2 = gregorian_to_jd(2025, 1, 1.0);
    TEST_ASSERT_FLOAT_WITHIN(JD_EPSILON, 366.0, jd2 - jd1);
}

void test_century_non_leap_1900_days(void) {
    double jd1 = gregorian_to_jd(1900, 1, 1.0);
    double jd2 = gregorian_to_jd(1901, 1, 1.0);
    TEST_ASSERT_FLOAT_WITHIN(JD_EPSILON, 365.0, jd2 - jd1);
}

void test_century_leap_2000_days(void) {
    double jd1 = gregorian_to_jd(2000, 1, 1.0);
    double jd2 = gregorian_to_jd(2001, 1, 1.0);
    TEST_ASSERT_FLOAT_WITHIN(JD_EPSILON, 366.0, jd2 - jd1);
}

void test_julian_cal_differs_from_gregorian(void) {
    double jd_greg = gregorian_to_jd(1800, 6, 15.0);
    double jd_jul = julian_cal_to_jd(1800, 6, 15.0);
    TEST_ASSERT_TRUE(fabs(jd_greg - jd_jul) > 10.0);
}

void test_roundtrip_december_31(void) {
    double jd = gregorian_to_jd(2025, 12, 31.0);
    int year, month;
    double day = jd_to_gregorian(jd, &year, &month);
    TEST_ASSERT_EQUAL_INT(2025, year);
    TEST_ASSERT_EQUAL_INT(12, month);
    TEST_ASSERT_FLOAT_WITHIN(JD_EPSILON, 31.0, day);
}

void test_roundtrip_march_1(void) {
    double jd = gregorian_to_jd(2024, 3, 1.0);
    int year, month;
    double day = jd_to_gregorian(jd, &year, &month);
    TEST_ASSERT_EQUAL_INT(2024, year);
    TEST_ASSERT_EQUAL_INT(3, month);
    TEST_ASSERT_FLOAT_WITHIN(JD_EPSILON, 1.0, day);
}

void test_jd_monotonically_increasing(void) {
    double prev = gregorian_to_jd(2000, 1, 1.0);
    for (int m = 1; m <= 12; m++) {
        double curr = gregorian_to_jd(2000, m, 15.0);
        TEST_ASSERT_TRUE(curr > prev);
        prev = curr;
    }
}

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
    RUN_TEST(test_meeus_example_1988);
    RUN_TEST(test_julian_day_negative_year);
    RUN_TEST(test_far_future_year_9999);
    RUN_TEST(test_roundtrip_negative_year);
    RUN_TEST(test_roundtrip_year_1583);
    RUN_TEST(test_roundtrip_leap_century_2000);
    RUN_TEST(test_roundtrip_many_dates);
    RUN_TEST(test_consecutive_days);
    RUN_TEST(test_month_boundaries);
    RUN_TEST(test_leap_year_feb_29_2024);
    RUN_TEST(test_fractional_day_noon);
    RUN_TEST(test_fractional_day_quarter);
    RUN_TEST(test_year_length_non_leap);
    RUN_TEST(test_year_length_leap);
    RUN_TEST(test_century_non_leap_1900_days);
    RUN_TEST(test_century_leap_2000_days);
    RUN_TEST(test_julian_cal_differs_from_gregorian);
    RUN_TEST(test_roundtrip_december_31);
    RUN_TEST(test_roundtrip_march_1);
    RUN_TEST(test_jd_monotonically_increasing);
    return UNITY_END();
}
