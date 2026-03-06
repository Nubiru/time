#include "../unity/unity.h"
#include "../../src/ui/fmt.h"
#include <string.h>

void setUp(void) { }
void tearDown(void) { }

/* --- DMS tests --- */

void test_dms_zero(void) {
    fmt_buf_t r = fmt_degrees_dms(0.0);
    TEST_ASSERT_EQUAL_STRING("0\xc2\xb0 00' 00.0\"", r.buf);
}

void test_dms_123_456(void) {
    fmt_buf_t r = fmt_degrees_dms(123.456);
    TEST_ASSERT_NOT_NULL(strstr(r.buf, "123"));
}

void test_dms_359_999(void) {
    fmt_buf_t r = fmt_degrees_dms(359.999);
    TEST_ASSERT_NOT_NULL(strstr(r.buf, "359"));
}

/* --- Hours tests --- */

void test_hours_zero(void) {
    fmt_buf_t r = fmt_hours_hms(0.0);
    TEST_ASSERT_EQUAL_STRING("00h 00m 00s", r.buf);
}

void test_hours_12_5(void) {
    fmt_buf_t r = fmt_hours_hms(12.5);
    TEST_ASSERT_EQUAL_STRING("12h 30m 00s", r.buf);
}

void test_right_ascension_180(void) {
    fmt_buf_t r = fmt_right_ascension(180.0);
    TEST_ASSERT_NOT_NULL(strstr(r.buf, "12h"));
}

/* --- Declination tests --- */

void test_declination_positive(void) {
    fmt_buf_t r = fmt_declination(23.4393);
    TEST_ASSERT_TRUE(r.buf[0] == '+');
    TEST_ASSERT_NOT_NULL(strstr(r.buf, "23"));
}

void test_declination_negative(void) {
    fmt_buf_t r = fmt_declination(-15.5);
    TEST_ASSERT_TRUE(r.buf[0] == '-');
}

void test_declination_obliquity(void) {
    fmt_buf_t r = fmt_declination(23.4393);
    TEST_ASSERT_NOT_NULL(strstr(r.buf, "23"));
}

/* --- Number tests --- */

void test_padded_int_007(void) {
    fmt_buf_t r = fmt_padded_int(7, 3);
    TEST_ASSERT_EQUAL_STRING("007", r.buf);
}

void test_padded_int_42(void) {
    fmt_buf_t r = fmt_padded_int(42, 2);
    TEST_ASSERT_EQUAL_STRING("42", r.buf);
}

void test_signed_float_positive(void) {
    fmt_buf_t r = fmt_signed_float(12.345, 2);
    TEST_ASSERT_EQUAL_STRING("+12.35", r.buf);
}

void test_signed_float_negative(void) {
    fmt_buf_t r = fmt_signed_float(-0.5, 1);
    TEST_ASSERT_EQUAL_STRING("-0.5", r.buf);
}

/* --- Ordinal tests --- */

void test_ordinal_1st(void) {
    TEST_ASSERT_EQUAL_STRING("1st", fmt_ordinal(1).buf);
}

void test_ordinal_2nd(void) {
    TEST_ASSERT_EQUAL_STRING("2nd", fmt_ordinal(2).buf);
}

void test_ordinal_3rd(void) {
    TEST_ASSERT_EQUAL_STRING("3rd", fmt_ordinal(3).buf);
}

void test_ordinal_11th(void) {
    TEST_ASSERT_EQUAL_STRING("11th", fmt_ordinal(11).buf);
}

void test_ordinal_21st(void) {
    TEST_ASSERT_EQUAL_STRING("21st", fmt_ordinal(21).buf);
}

void test_ordinal_12th(void) {
    TEST_ASSERT_EQUAL_STRING("12th", fmt_ordinal(12).buf);
}

void test_ordinal_13th(void) {
    TEST_ASSERT_EQUAL_STRING("13th", fmt_ordinal(13).buf);
}

/* --- Zodiac position test --- */

void test_zodiac_position_45_5(void) {
    /* 45.5 degrees = Taurus (sign 1), 15.5 degrees in sign */
    fmt_buf_t r = fmt_zodiac_position(45.5);
    TEST_ASSERT_NOT_NULL(strstr(r.buf, "15"));
    TEST_ASSERT_NOT_NULL(strstr(r.buf, "\u2649")); /* Taurus */
}

/* --- Date/time tests --- */

void test_iso_date(void) {
    fmt_buf_t r = fmt_iso_date(2000, 1, 1);
    TEST_ASSERT_EQUAL_STRING("2000-01-01", r.buf);
}

void test_time_24h(void) {
    fmt_buf_t r = fmt_time_24h(14, 5, 9);
    TEST_ASSERT_EQUAL_STRING("14:05:09", r.buf);
}

void test_julian_day(void) {
    fmt_buf_t r = fmt_julian_day(2451545.0, 1);
    TEST_ASSERT_NOT_NULL(strstr(r.buf, "2451545.0"));
}

/* --- Duration tests --- */

void test_duration_hms(void) {
    fmt_buf_t r = fmt_duration_hms(8130.0); /* 2h 15m 30s */
    TEST_ASSERT_EQUAL_STRING("2h 15m 30s", r.buf);
}

void test_duration_days_short(void) {
    fmt_buf_t r = fmt_duration_days(30.0);
    TEST_ASSERT_NOT_NULL(strstr(r.buf, "30.00 days"));
}

void test_duration_days_years(void) {
    fmt_buf_t r = fmt_duration_days(730.5);
    TEST_ASSERT_NOT_NULL(strstr(r.buf, "years"));
}

/* --- Purity test --- */

void test_purity_same_input_same_output(void) {
    fmt_buf_t a = fmt_degrees_dms(123.456);
    fmt_buf_t b = fmt_degrees_dms(123.456);
    TEST_ASSERT_EQUAL_STRING(a.buf, b.buf);

    fmt_buf_t c = fmt_ordinal(21);
    fmt_buf_t d = fmt_ordinal(21);
    TEST_ASSERT_EQUAL_STRING(c.buf, d.buf);
}

/* --- Runner --- */

int main(void) {
    UNITY_BEGIN();

    /* DMS */
    RUN_TEST(test_dms_zero);
    RUN_TEST(test_dms_123_456);
    RUN_TEST(test_dms_359_999);

    /* Hours */
    RUN_TEST(test_hours_zero);
    RUN_TEST(test_hours_12_5);
    RUN_TEST(test_right_ascension_180);

    /* Declination */
    RUN_TEST(test_declination_positive);
    RUN_TEST(test_declination_negative);
    RUN_TEST(test_declination_obliquity);

    /* Numbers */
    RUN_TEST(test_padded_int_007);
    RUN_TEST(test_padded_int_42);
    RUN_TEST(test_signed_float_positive);
    RUN_TEST(test_signed_float_negative);

    /* Ordinals */
    RUN_TEST(test_ordinal_1st);
    RUN_TEST(test_ordinal_2nd);
    RUN_TEST(test_ordinal_3rd);
    RUN_TEST(test_ordinal_11th);
    RUN_TEST(test_ordinal_21st);
    RUN_TEST(test_ordinal_12th);
    RUN_TEST(test_ordinal_13th);

    /* Zodiac */
    RUN_TEST(test_zodiac_position_45_5);

    /* Date/time */
    RUN_TEST(test_iso_date);
    RUN_TEST(test_time_24h);
    RUN_TEST(test_julian_day);

    /* Duration */
    RUN_TEST(test_duration_hms);
    RUN_TEST(test_duration_days_short);
    RUN_TEST(test_duration_days_years);

    /* Purity */
    RUN_TEST(test_purity_same_input_same_output);

    return UNITY_END();
}
