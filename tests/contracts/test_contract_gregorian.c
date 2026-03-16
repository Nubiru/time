/* test_contract_gregorian.c — Exhaustive contract tests for gregorian.h */

#include "../unity/unity.h"
#include "../../src/systems/gregorian/gregorian.h"
#include "../../src/math/julian.h"
#include <math.h>
#include <string.h>

void setUp(void) { }
void tearDown(void) { }

void test_day_of_week_range_sweep(void) {
    double jd_start = gregorian_to_jd(1900, 1, 1.0);
    int failures = 0;
    for (int i = 0; i < 1000; i++) {
        double jd = jd_start + i * 3.65;
        int dow = gregorian_day_of_week(jd);
        if (dow < 0 || dow > 6) failures++;
    }
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, failures, "day_of_week out of [0,6]");
}

void test_day_of_week_y2k(void) {
    double jd = gregorian_to_jd(2000, 1, 1.5);
    TEST_ASSERT_EQUAL_INT(5, gregorian_day_of_week(jd));
}

void test_day_of_week_seven_day_cycle(void) {
    double jd_start = gregorian_to_jd(2026, 3, 16.0);
    int seen[7] = {0};
    for (int i = 0; i < 7; i++) {
        int dow = gregorian_day_of_week(jd_start + i);
        TEST_ASSERT_TRUE(dow >= 0 && dow <= 6);
        seen[dow] = 1;
    }
    for (int i = 0; i < 7; i++) TEST_ASSERT_EQUAL_INT(1, seen[i]);
}

void test_day_name_valid_range(void) {
    for (int i = 0; i < 7; i++) {
        const char *name = gregorian_day_name(i);
        TEST_ASSERT_NOT_NULL(name);
        TEST_ASSERT_TRUE(name[0] != '?');
    }
}

void test_day_name_invalid(void) {
    TEST_ASSERT_NOT_NULL(gregorian_day_name(-1));
    TEST_ASSERT_NOT_NULL(gregorian_day_name(7));
}

void test_format_date_pattern(void) {
    char buf[32];
    gregorian_format_date(gregorian_to_jd(2026, 3, 16.0), buf, sizeof(buf));
    TEST_ASSERT_EQUAL_INT(10, (int)strlen(buf));
    TEST_ASSERT_EQUAL_CHAR('-', buf[4]);
    TEST_ASSERT_EQUAL_CHAR('-', buf[7]);
}

void test_format_date_known_date(void) {
    char buf[32];
    gregorian_format_date(gregorian_to_jd(2000, 1, 1.0), buf, sizeof(buf));
    TEST_ASSERT_EQUAL_STRING("2000-01-01", buf);
}

void test_format_time_pattern(void) {
    char buf[32];
    gregorian_format_time(gregorian_to_jd(2026, 3, 16.5), buf, sizeof(buf));
    TEST_ASSERT_EQUAL_INT(8, (int)strlen(buf));
    TEST_ASSERT_EQUAL_CHAR(':', buf[2]);
    TEST_ASSERT_EQUAL_CHAR(':', buf[5]);
}

void test_format_time_midnight(void) {
    char buf[32];
    gregorian_format_time(gregorian_to_jd(2026, 3, 16.0), buf, sizeof(buf));
    TEST_ASSERT_EQUAL_STRING("00:00:00", buf);
}

void test_format_time_noon(void) {
    char buf[32];
    gregorian_format_time(gregorian_to_jd(2026, 3, 16.5), buf, sizeof(buf));
    TEST_ASSERT_EQUAL_STRING("12:00:00", buf);
}

void test_format_full_pattern(void) {
    char buf[64];
    gregorian_format_full(gregorian_to_jd(2026, 3, 16.5), buf, sizeof(buf));
    TEST_ASSERT_EQUAL_INT(19, (int)strlen(buf));
    TEST_ASSERT_EQUAL_CHAR(' ', buf[10]);
}

void test_format_date_small_buffer(void) {
    char buf[4] = {0};
    gregorian_format_date(gregorian_to_jd(2026, 3, 16.0), buf, sizeof(buf));
    TEST_ASSERT_TRUE(strlen(buf) < sizeof(buf));
}

void test_format_time_small_buffer(void) {
    char buf[4] = {0};
    gregorian_format_time(gregorian_to_jd(2026, 3, 16.5), buf, sizeof(buf));
    TEST_ASSERT_TRUE(strlen(buf) < sizeof(buf));
}

void test_day_of_week_modern_sweep(void) {
    int failures = 0;
    for (int y = 2000; y <= 2030; y++) {
        for (int m = 1; m <= 12; m++) {
            int dow = gregorian_day_of_week(gregorian_to_jd(y, m, 15.0));
            if (dow < 0 || dow > 6) failures++;
        }
    }
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, failures, "day_of_week out of range in modern sweep");
}

void test_format_date_various(void) {
    char buf[32];
    gregorian_format_date(gregorian_to_jd(-1, 6, 15.0), buf, sizeof(buf));
    TEST_ASSERT_TRUE(strlen(buf) > 0);
    gregorian_format_date(gregorian_to_jd(9999, 12, 31.0), buf, sizeof(buf));
    TEST_ASSERT_TRUE(strlen(buf) > 0);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_day_of_week_range_sweep);
    RUN_TEST(test_day_of_week_y2k);
    RUN_TEST(test_day_of_week_seven_day_cycle);
    RUN_TEST(test_day_name_valid_range);
    RUN_TEST(test_day_name_invalid);
    RUN_TEST(test_format_date_pattern);
    RUN_TEST(test_format_date_known_date);
    RUN_TEST(test_format_time_pattern);
    RUN_TEST(test_format_time_midnight);
    RUN_TEST(test_format_time_noon);
    RUN_TEST(test_format_full_pattern);
    RUN_TEST(test_format_date_small_buffer);
    RUN_TEST(test_format_time_small_buffer);
    RUN_TEST(test_day_of_week_modern_sweep);
    RUN_TEST(test_format_date_various);
    return UNITY_END();
}
