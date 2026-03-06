#include "../unity/unity.h"
#include "../../src/core/date_parse.h"

#include <math.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ISO date parse: 1-3 */

void test_parse_iso_date(void)
{
    parsed_date_t d = date_parse("2024-04-08");
    TEST_ASSERT_EQUAL_INT(1, d.valid);
    TEST_ASSERT_EQUAL_INT(2024, d.year);
    TEST_ASSERT_EQUAL_INT(4, d.month);
    TEST_ASSERT_EQUAL_INT(8, d.day);
    TEST_ASSERT_EQUAL_INT(0, d.hour);
    TEST_ASSERT_EQUAL_INT(0, d.minute);
    TEST_ASSERT_EQUAL_INT(0, d.second);
}

void test_parse_j2000_date(void)
{
    parsed_date_t d = date_parse("2000-01-01");
    TEST_ASSERT_EQUAL_INT(1, d.valid);
    TEST_ASSERT_EQUAL_INT(2000, d.year);
    TEST_ASSERT_EQUAL_INT(1, d.month);
    TEST_ASSERT_EQUAL_INT(1, d.day);
}

void test_parse_end_of_year(void)
{
    parsed_date_t d = date_parse("2025-12-31");
    TEST_ASSERT_EQUAL_INT(1, d.valid);
    TEST_ASSERT_EQUAL_INT(12, d.month);
    TEST_ASSERT_EQUAL_INT(31, d.day);
}

/* Datetime parse: 4-6 */

void test_parse_datetime_hm(void)
{
    parsed_date_t d = date_parse("2024-04-08 14:30");
    TEST_ASSERT_EQUAL_INT(1, d.valid);
    TEST_ASSERT_EQUAL_INT(14, d.hour);
    TEST_ASSERT_EQUAL_INT(30, d.minute);
    TEST_ASSERT_EQUAL_INT(0, d.second);
}

void test_parse_datetime_hms(void)
{
    parsed_date_t d = date_parse("2024-04-08 14:30:45");
    TEST_ASSERT_EQUAL_INT(1, d.valid);
    TEST_ASSERT_EQUAL_INT(14, d.hour);
    TEST_ASSERT_EQUAL_INT(30, d.minute);
    TEST_ASSERT_EQUAL_INT(45, d.second);
}

void test_parse_datetime_t_separator(void)
{
    parsed_date_t d = date_parse("2024-04-08T14:30:00");
    TEST_ASSERT_EQUAL_INT(1, d.valid);
    TEST_ASSERT_EQUAL_INT(14, d.hour);
}

/* Negative year: 7 */

void test_parse_negative_year(void)
{
    parsed_date_t d = date_parse("-4712-01-01");
    TEST_ASSERT_EQUAL_INT(1, d.valid);
    TEST_ASSERT_EQUAL_INT(-4712, d.year);
    TEST_ASSERT_EQUAL_INT(1, d.month);
    TEST_ASSERT_EQUAL_INT(1, d.day);
}

/* Invalid strings: 8-11 */

void test_parse_null(void)
{
    parsed_date_t d = date_parse(NULL);
    TEST_ASSERT_EQUAL_INT(0, d.valid);
}

void test_parse_empty(void)
{
    parsed_date_t d = date_parse("");
    TEST_ASSERT_EQUAL_INT(0, d.valid);
}

void test_parse_garbage(void)
{
    parsed_date_t d = date_parse("not a date");
    TEST_ASSERT_EQUAL_INT(0, d.valid);
}

void test_parse_invalid_month(void)
{
    parsed_date_t d = date_parse("2024-13-01");
    TEST_ASSERT_EQUAL_INT(0, d.valid);
}

void test_parse_invalid_day(void)
{
    parsed_date_t d = date_parse("2024-02-30");
    TEST_ASSERT_EQUAL_INT(0, d.valid);
}

void test_parse_invalid_hour(void)
{
    parsed_date_t d = date_parse("2024-01-01 25:00");
    TEST_ASSERT_EQUAL_INT(0, d.valid);
}

/* Round-trip: 14-15 */

void test_roundtrip_date(void)
{
    double jd = date_parse_to_jd("2024-04-08");
    TEST_ASSERT_TRUE(jd > 0.0);
    char buf[32];
    date_format_iso(jd, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_STRING("2024-04-08", buf);
}

void test_roundtrip_datetime(void)
{
    double jd = date_parse_to_jd("2024-04-08 12:00:00");
    TEST_ASSERT_TRUE(jd > 0.0);
    char buf[32];
    date_format_full(jd, buf, sizeof(buf));
    TEST_ASSERT_TRUE(strstr(buf, "2024-04-08") != NULL);
    TEST_ASSERT_TRUE(strstr(buf, "12:00:00") != NULL);
}

/* Leap year: 16-18 */

void test_leap_year_2000(void)
{
    TEST_ASSERT_TRUE(date_is_leap_year(2000));
}

void test_not_leap_year_1900(void)
{
    TEST_ASSERT_FALSE(date_is_leap_year(1900));
}

void test_leap_year_2024(void)
{
    TEST_ASSERT_TRUE(date_is_leap_year(2024));
}

void test_not_leap_year_2023(void)
{
    TEST_ASSERT_FALSE(date_is_leap_year(2023));
}

/* Days in month: 19-21 */

void test_days_january(void)
{
    TEST_ASSERT_EQUAL_INT(31, date_days_in_month(2024, 1));
}

void test_days_february_leap(void)
{
    TEST_ASSERT_EQUAL_INT(29, date_days_in_month(2024, 2));
}

void test_days_february_normal(void)
{
    TEST_ASSERT_EQUAL_INT(28, date_days_in_month(2023, 2));
}

void test_days_all_months(void)
{
    int expected[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    for (int m = 1; m <= 12; m++) {
        TEST_ASSERT_EQUAL_INT(expected[m - 1], date_days_in_month(2023, m));
    }
}

void test_days_invalid_month(void)
{
    TEST_ASSERT_EQUAL_INT(0, date_days_in_month(2024, 0));
    TEST_ASSERT_EQUAL_INT(0, date_days_in_month(2024, 13));
}

/* Null safety for format: 22 */

void test_format_null_safe(void)
{
    date_format_iso(2451545.0, NULL, 0);
    date_format_full(2451545.0, NULL, 0);
    /* Should not crash */
}

/* Convenience wrapper: 23 */

void test_parse_to_jd_invalid(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.01f, -1.0f, (float)date_parse_to_jd("garbage"));
    TEST_ASSERT_FLOAT_WITHIN(0.01f, -1.0f, (float)date_parse_to_jd(NULL));
}

/* Purity: 24 */

void test_purity(void)
{
    parsed_date_t a = date_parse("2024-04-08 14:30");
    parsed_date_t b = date_parse("2024-04-08 14:30");
    TEST_ASSERT_EQUAL_INT(a.valid, b.valid);
    TEST_ASSERT_EQUAL_INT(a.year, b.year);
    TEST_ASSERT_EQUAL_INT(a.hour, b.hour);

    double ja = date_parse_to_jd("2024-04-08");
    double jb = date_parse_to_jd("2024-04-08");
    TEST_ASSERT_EQUAL_FLOAT((float)ja, (float)jb);
}

/* Validate: 25 */

void test_validate(void)
{
    TEST_ASSERT_TRUE(date_validate(2024, 4, 8, 14, 30, 0));
    TEST_ASSERT_FALSE(date_validate(2024, 0, 1, 0, 0, 0));
    TEST_ASSERT_FALSE(date_validate(2024, 1, 32, 0, 0, 0));
    TEST_ASSERT_FALSE(date_validate(2024, 1, 1, 0, 60, 0));
    TEST_ASSERT_FALSE(date_validate(2024, 1, 1, 0, 0, 60));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_parse_iso_date);
    RUN_TEST(test_parse_j2000_date);
    RUN_TEST(test_parse_end_of_year);
    RUN_TEST(test_parse_datetime_hm);
    RUN_TEST(test_parse_datetime_hms);
    RUN_TEST(test_parse_datetime_t_separator);
    RUN_TEST(test_parse_negative_year);
    RUN_TEST(test_parse_null);
    RUN_TEST(test_parse_empty);
    RUN_TEST(test_parse_garbage);
    RUN_TEST(test_parse_invalid_month);
    RUN_TEST(test_parse_invalid_day);
    RUN_TEST(test_parse_invalid_hour);
    RUN_TEST(test_roundtrip_date);
    RUN_TEST(test_roundtrip_datetime);
    RUN_TEST(test_leap_year_2000);
    RUN_TEST(test_not_leap_year_1900);
    RUN_TEST(test_leap_year_2024);
    RUN_TEST(test_not_leap_year_2023);
    RUN_TEST(test_days_january);
    RUN_TEST(test_days_february_leap);
    RUN_TEST(test_days_february_normal);
    RUN_TEST(test_days_all_months);
    RUN_TEST(test_days_invalid_month);
    RUN_TEST(test_format_null_safe);
    RUN_TEST(test_parse_to_jd_invalid);
    RUN_TEST(test_purity);
    RUN_TEST(test_validate);
    return UNITY_END();
}
