/* test_hebrew_today.c — Tests for Hebrew "Today" page module.
 * TDD RED phase: tests written before implementation.
 *
 * Test dates:
 *   JD 2460394.5 => 15 Adar II 5784 (leap year, deficient 383 days)
 *   JD 2460586.5 =>  1 Tishrei 5785 (common year, abundant 355 days)
 *   JD 2459848.5 =>  1 Tishrei 5783 (common year, abundant 355 days)
 *
 * Shemitah checks:
 *   5782 % 7 == 0 => Shemitah year
 *   5784 % 7 == 2 => not Shemitah (year 2 of 7)
 */

#include "../../unity/unity.h"
#include "../../../src/systems/hebrew/hebrew_today.h"
#include "../../../src/systems/hebrew/hebrew.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* === Known JD constants === */

/* 15 Adar II 5784 (leap, deficient 383) */
static const double JD_ADAR2_5784 = 2460394.5;

/* 1 Tishrei 5785 (common, abundant 355) */
static const double JD_TISHREI_5785 = 2460586.5;

/* 1 Tishrei 5783 (common, abundant 355) */
static const double JD_TISHREI_5783 = 2459848.5;

/* ===== ht_today — has_data and section_count ===== */

void test_today_has_data(void) {
    ht_page_t p = ht_today(JD_ADAR2_5784);
    TEST_ASSERT_EQUAL_INT(1, p.has_data);
}

void test_today_section_count(void) {
    ht_page_t p = ht_today(JD_ADAR2_5784);
    TEST_ASSERT_EQUAL_INT(5, p.section_count);
}

/* ===== page_title ===== */

void test_title_contains_hebrew(void) {
    ht_page_t p = ht_today(JD_ADAR2_5784);
    TEST_ASSERT_NOT_NULL(strstr(p.page_title, "Hebrew"));
}

void test_title_contains_year_5784(void) {
    ht_page_t p = ht_today(JD_ADAR2_5784);
    TEST_ASSERT_NOT_NULL(strstr(p.page_title, "5784"));
}

void test_title_contains_adar_ii(void) {
    ht_page_t p = ht_today(JD_ADAR2_5784);
    TEST_ASSERT_NOT_NULL(strstr(p.page_title, "Adar II"));
}

void test_title_contains_year_5785(void) {
    ht_page_t p = ht_today(JD_TISHREI_5785);
    TEST_ASSERT_NOT_NULL(strstr(p.page_title, "5785"));
}

void test_title_contains_tishrei(void) {
    ht_page_t p = ht_today(JD_TISHREI_5785);
    TEST_ASSERT_NOT_NULL(strstr(p.page_title, "Tishrei"));
}

/* ===== date_section ===== */

void test_date_section_contains_date(void) {
    ht_page_t p = ht_today(JD_ADAR2_5784);
    TEST_ASSERT_NOT_NULL(strstr(p.date_section, "Date:"));
}

void test_date_section_contains_month_name(void) {
    ht_page_t p = ht_today(JD_ADAR2_5784);
    TEST_ASSERT_NOT_NULL(strstr(p.date_section, "Adar II"));
}

void test_date_section_contains_year_type_label(void) {
    ht_page_t p = ht_today(JD_ADAR2_5784);
    TEST_ASSERT_NOT_NULL(strstr(p.date_section, "Year type:"));
}

void test_date_section_deficient_leap(void) {
    /* 5784 is deficient leap (383 days) */
    ht_page_t p = ht_today(JD_ADAR2_5784);
    TEST_ASSERT_NOT_NULL(strstr(p.date_section, "Deficient"));
}

void test_date_section_contains_leap_label(void) {
    ht_page_t p = ht_today(JD_ADAR2_5784);
    TEST_ASSERT_NOT_NULL(strstr(p.date_section, "Leap year:"));
}

void test_date_section_leap_yes(void) {
    /* 5784 is leap */
    ht_page_t p = ht_today(JD_ADAR2_5784);
    TEST_ASSERT_NOT_NULL(strstr(p.date_section, "Yes"));
}

void test_date_section_non_leap_no(void) {
    /* 5785 is common (not leap) */
    ht_page_t p = ht_today(JD_TISHREI_5785);
    TEST_ASSERT_NOT_NULL(strstr(p.date_section, "No"));
}

void test_date_section_abundant_common(void) {
    /* 5785 is abundant common (355 days) */
    ht_page_t p = ht_today(JD_TISHREI_5785);
    TEST_ASSERT_NOT_NULL(strstr(p.date_section, "Abundant"));
}

void test_date_section_contains_day_of_year(void) {
    ht_page_t p = ht_today(JD_ADAR2_5784);
    /* Should have "Day NNN of 383" */
    TEST_ASSERT_NOT_NULL(strstr(p.date_section, "of 383"));
}

void test_date_section_tishrei_1_day_of_year(void) {
    /* 1 Tishrei should be day 1 */
    ht_page_t p = ht_today(JD_TISHREI_5785);
    TEST_ASSERT_NOT_NULL(strstr(p.date_section, "Day 1 of"));
}

/* ===== month_section ===== */

void test_month_section_contains_month_label(void) {
    ht_page_t p = ht_today(JD_ADAR2_5784);
    TEST_ASSERT_NOT_NULL(strstr(p.month_section, "Month:"));
}

void test_month_section_contains_letter(void) {
    ht_page_t p = ht_today(JD_ADAR2_5784);
    TEST_ASSERT_NOT_NULL(strstr(p.month_section, "Hebrew Letter:"));
}

void test_month_section_contains_tribe(void) {
    ht_page_t p = ht_today(JD_ADAR2_5784);
    TEST_ASSERT_NOT_NULL(strstr(p.month_section, "Tribe:"));
}

void test_month_section_contains_sense(void) {
    ht_page_t p = ht_today(JD_ADAR2_5784);
    TEST_ASSERT_NOT_NULL(strstr(p.month_section, "Sense:"));
}

void test_month_section_contains_quality(void) {
    ht_page_t p = ht_today(JD_ADAR2_5784);
    TEST_ASSERT_NOT_NULL(strstr(p.month_section, "Quality:"));
}

void test_month_section_contains_meaning(void) {
    ht_page_t p = ht_today(JD_ADAR2_5784);
    TEST_ASSERT_NOT_NULL(strstr(p.month_section, "Meaning:"));
}

void test_month_section_adar2_letter_qof(void) {
    /* Adar II (month 13) has letter Qof */
    ht_page_t p = ht_today(JD_ADAR2_5784);
    TEST_ASSERT_NOT_NULL(strstr(p.month_section, "Qof"));
}

void test_month_section_tishrei_letter_lamed(void) {
    /* Tishrei (month 7) has letter Lamed */
    ht_page_t p = ht_today(JD_TISHREI_5785);
    TEST_ASSERT_NOT_NULL(strstr(p.month_section, "Lamed"));
}

/* ===== sabbatical_section ===== */

void test_sabbatical_contains_shemitah(void) {
    ht_page_t p = ht_today(JD_ADAR2_5784);
    TEST_ASSERT_NOT_NULL(strstr(p.sabbatical_section, "Shemitah"));
}

void test_sabbatical_contains_year_of_7(void) {
    ht_page_t p = ht_today(JD_ADAR2_5784);
    TEST_ASSERT_NOT_NULL(strstr(p.sabbatical_section, "of 7"));
}

void test_sabbatical_5784_year_in_cycle(void) {
    /* 5784: (5784-1)%7+1 = 5783%7+1 = 0+1 = 1... wait:
     * 5782 is shemitah (5782%7==0). 5783: year 1. 5784: year 2. */
    ht_page_t p = ht_today(JD_ADAR2_5784);
    TEST_ASSERT_NOT_NULL(strstr(p.sabbatical_section, "Year 2 of 7"));
}

void test_sabbatical_contains_next(void) {
    ht_page_t p = ht_today(JD_ADAR2_5784);
    TEST_ASSERT_NOT_NULL(strstr(p.sabbatical_section, "Next"));
}

/* ===== jubilee_section ===== */

void test_jubilee_contains_jubilee(void) {
    ht_page_t p = ht_today(JD_ADAR2_5784);
    TEST_ASSERT_NOT_NULL(strstr(p.jubilee_section, "Jubilee"));
}

void test_jubilee_contains_of_50(void) {
    ht_page_t p = ht_today(JD_ADAR2_5784);
    TEST_ASSERT_NOT_NULL(strstr(p.jubilee_section, "of 50"));
}

void test_jubilee_contains_sabbatical_count(void) {
    ht_page_t p = ht_today(JD_ADAR2_5784);
    TEST_ASSERT_NOT_NULL(strstr(p.jubilee_section, "Sabbatical count:"));
}

void test_jubilee_contains_next(void) {
    ht_page_t p = ht_today(JD_ADAR2_5784);
    TEST_ASSERT_NOT_NULL(strstr(p.jubilee_section, "Next"));
}

/* ===== holiday_section ===== */

void test_holiday_section_contains_upcoming(void) {
    ht_page_t p = ht_today(JD_ADAR2_5784);
    TEST_ASSERT_NOT_NULL(strstr(p.holiday_section, "Upcoming"));
}

void test_holiday_section_has_content(void) {
    ht_page_t p = ht_today(JD_ADAR2_5784);
    TEST_ASSERT_TRUE(strlen(p.holiday_section) > 20);
}

void test_holiday_section_tishrei_1(void) {
    /* On 1 Tishrei (Rosh Hashanah), should show upcoming holidays */
    ht_page_t p = ht_today(JD_TISHREI_5785);
    TEST_ASSERT_NOT_NULL(strstr(p.holiday_section, "Upcoming"));
}

/* ===== ht_format ===== */

void test_format_produces_output(void) {
    ht_page_t p = ht_today(JD_ADAR2_5784);
    char buf[4096];
    int len = ht_format(&p, buf, (int)sizeof(buf));
    TEST_ASSERT_TRUE(len > 0);
    TEST_ASSERT_TRUE(strlen(buf) > 0);
}

void test_format_contains_title(void) {
    ht_page_t p = ht_today(JD_ADAR2_5784);
    char buf[4096];
    ht_format(&p, buf, (int)sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Hebrew"));
}

void test_format_contains_date_section(void) {
    ht_page_t p = ht_today(JD_ADAR2_5784);
    char buf[4096];
    ht_format(&p, buf, (int)sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Date:"));
}

void test_format_contains_month_section(void) {
    ht_page_t p = ht_today(JD_ADAR2_5784);
    char buf[4096];
    ht_format(&p, buf, (int)sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Month:"));
}

void test_format_null_page(void) {
    char buf[256];
    int len = ht_format(NULL, buf, (int)sizeof(buf));
    TEST_ASSERT_EQUAL_INT(0, len);
}

void test_format_null_buf(void) {
    ht_page_t p = ht_today(JD_ADAR2_5784);
    int len = ht_format(&p, NULL, 256);
    TEST_ASSERT_EQUAL_INT(0, len);
}

void test_format_zero_buf_size(void) {
    ht_page_t p = ht_today(JD_ADAR2_5784);
    char buf[1] = {'X'};
    int len = ht_format(&p, buf, 0);
    TEST_ASSERT_EQUAL_INT(0, len);
}

void test_format_small_buffer(void) {
    ht_page_t p = ht_today(JD_ADAR2_5784);
    char buf[32];
    int len = ht_format(&p, buf, (int)sizeof(buf));
    TEST_ASSERT_TRUE(len > 0);
    TEST_ASSERT_TRUE(len < 32);
    TEST_ASSERT_TRUE(strlen(buf) < 32);
}

/* ===== Shemitah year test ===== */

void test_shemitah_year_5782(void) {
    /* 5782 is Shemitah. Use 1 Tishrei 5782.
     * We don't have a precomputed JD, so build it from hebrew_to_jd. */
    hebrew_date_t d = { .year = 5782, .month = 7, .day = 1 };
    double jd = hebrew_to_jd(d);
    ht_page_t p = ht_today(jd);
    TEST_ASSERT_NOT_NULL(strstr(p.sabbatical_section, "Year 7 of 7"));
}

/* ===== Leap vs non-leap ===== */

void test_leap_year_month_count(void) {
    /* 5784 is leap: date_section should mention 13 months */
    ht_page_t p = ht_today(JD_ADAR2_5784);
    TEST_ASSERT_NOT_NULL(strstr(p.date_section, "13 months"));
}

void test_non_leap_year_month_count(void) {
    /* 5785 is common: date_section should mention 12 months */
    ht_page_t p = ht_today(JD_TISHREI_5785);
    TEST_ASSERT_NOT_NULL(strstr(p.date_section, "12 months"));
}

/* ===== Second common year (5783) ===== */

void test_year_5783_title(void) {
    ht_page_t p = ht_today(JD_TISHREI_5783);
    TEST_ASSERT_NOT_NULL(strstr(p.page_title, "5783"));
}

void test_year_5783_abundant(void) {
    /* 5783 is abundant common (355 days) */
    ht_page_t p = ht_today(JD_TISHREI_5783);
    TEST_ASSERT_NOT_NULL(strstr(p.date_section, "Abundant"));
    TEST_ASSERT_NOT_NULL(strstr(p.date_section, "355"));
}

/* ===== main ===== */

int main(void) {
    UNITY_BEGIN();

    /* has_data / section_count */
    RUN_TEST(test_today_has_data);
    RUN_TEST(test_today_section_count);

    /* page_title */
    RUN_TEST(test_title_contains_hebrew);
    RUN_TEST(test_title_contains_year_5784);
    RUN_TEST(test_title_contains_adar_ii);
    RUN_TEST(test_title_contains_year_5785);
    RUN_TEST(test_title_contains_tishrei);

    /* date_section */
    RUN_TEST(test_date_section_contains_date);
    RUN_TEST(test_date_section_contains_month_name);
    RUN_TEST(test_date_section_contains_year_type_label);
    RUN_TEST(test_date_section_deficient_leap);
    RUN_TEST(test_date_section_contains_leap_label);
    RUN_TEST(test_date_section_leap_yes);
    RUN_TEST(test_date_section_non_leap_no);
    RUN_TEST(test_date_section_abundant_common);
    RUN_TEST(test_date_section_contains_day_of_year);
    RUN_TEST(test_date_section_tishrei_1_day_of_year);

    /* month_section */
    RUN_TEST(test_month_section_contains_month_label);
    RUN_TEST(test_month_section_contains_letter);
    RUN_TEST(test_month_section_contains_tribe);
    RUN_TEST(test_month_section_contains_sense);
    RUN_TEST(test_month_section_contains_quality);
    RUN_TEST(test_month_section_contains_meaning);
    RUN_TEST(test_month_section_adar2_letter_qof);
    RUN_TEST(test_month_section_tishrei_letter_lamed);

    /* sabbatical_section */
    RUN_TEST(test_sabbatical_contains_shemitah);
    RUN_TEST(test_sabbatical_contains_year_of_7);
    RUN_TEST(test_sabbatical_5784_year_in_cycle);
    RUN_TEST(test_sabbatical_contains_next);

    /* jubilee_section */
    RUN_TEST(test_jubilee_contains_jubilee);
    RUN_TEST(test_jubilee_contains_of_50);
    RUN_TEST(test_jubilee_contains_sabbatical_count);
    RUN_TEST(test_jubilee_contains_next);

    /* holiday_section */
    RUN_TEST(test_holiday_section_contains_upcoming);
    RUN_TEST(test_holiday_section_has_content);
    RUN_TEST(test_holiday_section_tishrei_1);

    /* ht_format */
    RUN_TEST(test_format_produces_output);
    RUN_TEST(test_format_contains_title);
    RUN_TEST(test_format_contains_date_section);
    RUN_TEST(test_format_contains_month_section);
    RUN_TEST(test_format_null_page);
    RUN_TEST(test_format_null_buf);
    RUN_TEST(test_format_zero_buf_size);
    RUN_TEST(test_format_small_buffer);

    /* Shemitah year */
    RUN_TEST(test_shemitah_year_5782);

    /* Leap vs non-leap */
    RUN_TEST(test_leap_year_month_count);
    RUN_TEST(test_non_leap_year_month_count);

    /* Second common year (5783) */
    RUN_TEST(test_year_5783_title);
    RUN_TEST(test_year_5783_abundant);

    return UNITY_END();
}
