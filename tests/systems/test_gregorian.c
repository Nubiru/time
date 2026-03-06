#include "../unity/unity.h"
#include "../../src/systems/gregorian/gregorian.h"
#include <string.h>

void setUp(void) { }
void tearDown(void) { }

/* ===== gregorian_format_date ===== */

void test_format_date_j2000(void) {
    /* J2000.0 = 2000 Jan 1.5 = JD 2451545.0 (noon) */
    char buf[32];
    gregorian_format_date(2451545.0, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_STRING("2000-01-01", buf);
}

void test_format_date_unix_epoch(void) {
    /* 1970-01-01 12:00 UT = JD 2440588.0 */
    char buf[32];
    gregorian_format_date(2440588.0, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_STRING("1970-01-01", buf);
}

void test_format_date_2024_leap(void) {
    /* 2024-02-29 12:00 UT = JD 2460370.0 */
    char buf[32];
    gregorian_format_date(2460370.0, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_STRING("2024-02-29", buf);
}

void test_format_date_midnight(void) {
    /* JD 2451544.5 = 2000-01-01 00:00 UT (midnight) */
    char buf[32];
    gregorian_format_date(2451544.5, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_STRING("2000-01-01", buf);
}

/* ===== gregorian_format_time ===== */

void test_format_time_noon(void) {
    /* JD ending in .0 = noon */
    char buf[16];
    gregorian_format_time(2451545.0, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_STRING("12:00:00", buf);
}

void test_format_time_midnight(void) {
    /* JD ending in .5 = midnight */
    char buf[16];
    gregorian_format_time(2451544.5, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_STRING("00:00:00", buf);
}

void test_format_time_6am(void) {
    /* JD ending in .75 = 06:00 UT */
    char buf[16];
    gregorian_format_time(2451544.75, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_STRING("06:00:00", buf);
}

void test_format_time_6pm(void) {
    /* JD ending in .25 = 18:00 UT */
    char buf[16];
    gregorian_format_time(2451545.25, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_STRING("18:00:00", buf);
}

/* ===== gregorian_format_full ===== */

void test_format_full_j2000(void) {
    char buf[64];
    gregorian_format_full(2451545.0, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_STRING("2000-01-01 12:00:00", buf);
}

void test_format_full_midnight(void) {
    char buf[64];
    gregorian_format_full(2451544.5, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_STRING("2000-01-01 00:00:00", buf);
}

/* ===== gregorian_day_of_week ===== */

void test_day_of_week_j2000(void) {
    /* 2000-01-01 was Saturday = 6 (Monday=0) */
    TEST_ASSERT_EQUAL_INT(5, gregorian_day_of_week(2451545.0));
}

void test_day_of_week_unix_epoch(void) {
    /* 1970-01-01 was Thursday = 3 (Monday=0) */
    TEST_ASSERT_EQUAL_INT(3, gregorian_day_of_week(2440588.0));
}

void test_day_of_week_known_monday(void) {
    /* 2024-01-01 was Monday = 0 -> JD 2460310.5 (midnight) */
    TEST_ASSERT_EQUAL_INT(0, gregorian_day_of_week(2460310.5));
}

/* ===== gregorian_day_name ===== */

void test_day_name_monday(void) {
    TEST_ASSERT_EQUAL_STRING("Monday", gregorian_day_name(0));
}

void test_day_name_sunday(void) {
    TEST_ASSERT_EQUAL_STRING("Sunday", gregorian_day_name(6));
}

void test_day_name_invalid(void) {
    TEST_ASSERT_EQUAL_STRING("?", gregorian_day_name(7));
    TEST_ASSERT_EQUAL_STRING("?", gregorian_day_name(-1));
}

int main(void) {
    UNITY_BEGIN();
    /* format_date */
    RUN_TEST(test_format_date_j2000);
    RUN_TEST(test_format_date_unix_epoch);
    RUN_TEST(test_format_date_2024_leap);
    RUN_TEST(test_format_date_midnight);
    /* format_time */
    RUN_TEST(test_format_time_noon);
    RUN_TEST(test_format_time_midnight);
    RUN_TEST(test_format_time_6am);
    RUN_TEST(test_format_time_6pm);
    /* format_full */
    RUN_TEST(test_format_full_j2000);
    RUN_TEST(test_format_full_midnight);
    /* day_of_week */
    RUN_TEST(test_day_of_week_j2000);
    RUN_TEST(test_day_of_week_unix_epoch);
    RUN_TEST(test_day_of_week_known_monday);
    /* day_name */
    RUN_TEST(test_day_name_monday);
    RUN_TEST(test_day_name_sunday);
    RUN_TEST(test_day_name_invalid);
    return UNITY_END();
}
