#include "../../unity/unity.h"
#include "../../../src/systems/gregorian/gregorian_today.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ============================================================
 * Reference JD: 2461119.5 = 2026-03-20 00:00 UTC (Friday)
 *   day_of_year  = 79 (31+28+20, non-leap)
 *   week_number  = 12  ((79-1)/7 + 1)
 *   quarter      = 1   ((3-1)/3 + 1)
 *   season       = 0   (Spring: Mar-May)
 *   leap         = no  (2026 % 4 != 0)
 * ============================================================ */

#define JD_2026_MAR_20  2461119.5

/* ============================================================
 * gt_today — basic fields
 * ============================================================ */

void test_today_has_data(void) {
    gt_page_t p = gt_today(JD_2026_MAR_20);
    TEST_ASSERT_EQUAL_INT(1, p.has_data);
}

void test_today_year(void) {
    gt_page_t p = gt_today(JD_2026_MAR_20);
    TEST_ASSERT_EQUAL_INT(2026, p.year);
}

void test_today_month(void) {
    gt_page_t p = gt_today(JD_2026_MAR_20);
    TEST_ASSERT_EQUAL_INT(3, p.month);
}

void test_today_day(void) {
    gt_page_t p = gt_today(JD_2026_MAR_20);
    TEST_ASSERT_EQUAL_INT(20, p.day);
}

void test_today_day_of_week(void) {
    gt_page_t p = gt_today(JD_2026_MAR_20);
    TEST_ASSERT_EQUAL_INT(4, p.day_of_week); /* Friday=4 */
}

void test_today_section_count(void) {
    gt_page_t p = gt_today(JD_2026_MAR_20);
    TEST_ASSERT_EQUAL_INT(5, p.section_count);
}

/* ============================================================
 * page_title
 * ============================================================ */

void test_title_contains_gregorian(void) {
    gt_page_t p = gt_today(JD_2026_MAR_20);
    TEST_ASSERT_NOT_NULL(strstr(p.page_title, "Gregorian"));
}

void test_title_contains_march(void) {
    gt_page_t p = gt_today(JD_2026_MAR_20);
    TEST_ASSERT_NOT_NULL(strstr(p.page_title, "March"));
}

void test_title_contains_2026(void) {
    gt_page_t p = gt_today(JD_2026_MAR_20);
    TEST_ASSERT_NOT_NULL(strstr(p.page_title, "2026"));
}

void test_title_contains_friday(void) {
    gt_page_t p = gt_today(JD_2026_MAR_20);
    TEST_ASSERT_NOT_NULL(strstr(p.page_title, "Friday"));
}

/* ============================================================
 * date_section
 * ============================================================ */

void test_date_section_day_of_year(void) {
    gt_page_t p = gt_today(JD_2026_MAR_20);
    TEST_ASSERT_NOT_NULL(strstr(p.date_section, "Day of Year:"));
    TEST_ASSERT_NOT_NULL(strstr(p.date_section, "79"));
}

void test_date_section_week(void) {
    gt_page_t p = gt_today(JD_2026_MAR_20);
    TEST_ASSERT_NOT_NULL(strstr(p.date_section, "Week:"));
}

void test_date_section_quarter(void) {
    gt_page_t p = gt_today(JD_2026_MAR_20);
    TEST_ASSERT_NOT_NULL(strstr(p.date_section, "Quarter:"));
    TEST_ASSERT_NOT_NULL(strstr(p.date_section, "Q1"));
}

/* ============================================================
 * month_section
 * ============================================================ */

void test_month_section_label(void) {
    gt_page_t p = gt_today(JD_2026_MAR_20);
    TEST_ASSERT_NOT_NULL(strstr(p.month_section, "Month:"));
    TEST_ASSERT_NOT_NULL(strstr(p.month_section, "March"));
}

void test_month_section_origin(void) {
    gt_page_t p = gt_today(JD_2026_MAR_20);
    TEST_ASSERT_NOT_NULL(strstr(p.month_section, "Origin:"));
}

/* ============================================================
 * day_section
 * ============================================================ */

void test_day_section_label(void) {
    gt_page_t p = gt_today(JD_2026_MAR_20);
    TEST_ASSERT_NOT_NULL(strstr(p.day_section, "Day:"));
}

void test_day_section_ruling_planet(void) {
    gt_page_t p = gt_today(JD_2026_MAR_20);
    TEST_ASSERT_NOT_NULL(strstr(p.day_section, "Ruling Planet:"));
    TEST_ASSERT_NOT_NULL(strstr(p.day_section, "Venus")); /* Friday=Venus */
}

/* ============================================================
 * season_section
 * ============================================================ */

void test_season_section_label(void) {
    gt_page_t p = gt_today(JD_2026_MAR_20);
    TEST_ASSERT_NOT_NULL(strstr(p.season_section, "Season:"));
}

void test_season_section_spring(void) {
    gt_page_t p = gt_today(JD_2026_MAR_20);
    TEST_ASSERT_NOT_NULL(strstr(p.season_section, "Spring"));
}

/* ============================================================
 * calendar_section
 * ============================================================ */

void test_calendar_section_progress(void) {
    gt_page_t p = gt_today(JD_2026_MAR_20);
    TEST_ASSERT_NOT_NULL(strstr(p.calendar_section, "Year Progress:"));
}

void test_calendar_section_remaining(void) {
    gt_page_t p = gt_today(JD_2026_MAR_20);
    TEST_ASSERT_NOT_NULL(strstr(p.calendar_section, "Days Remaining:"));
    TEST_ASSERT_NOT_NULL(strstr(p.calendar_section, "286")); /* 365 - 79 */
}

void test_calendar_section_leap_no(void) {
    gt_page_t p = gt_today(JD_2026_MAR_20);
    TEST_ASSERT_NOT_NULL(strstr(p.calendar_section, "Leap Year: No"));
}

/* ============================================================
 * Different season: summer date (July 4, 2026, Saturday)
 * ============================================================ */

#define JD_2026_JUL_04  2461225.5

void test_summer_season(void) {
    gt_page_t p = gt_today(JD_2026_JUL_04);
    TEST_ASSERT_EQUAL_INT(7, p.month);
    TEST_ASSERT_NOT_NULL(strstr(p.season_section, "Summer"));
}

void test_summer_quarter(void) {
    gt_page_t p = gt_today(JD_2026_JUL_04);
    TEST_ASSERT_NOT_NULL(strstr(p.date_section, "Q3"));
}

/* ============================================================
 * Leap year: 2024 (2024-02-29)
 * JD for 2024-02-29 = gregorian_to_jd(2024, 2, 29) ~= 2460369.5
 * ============================================================ */

#define JD_2024_FEB_29  2460369.5

void test_leap_year_2024(void) {
    gt_page_t p = gt_today(JD_2024_FEB_29);
    TEST_ASSERT_EQUAL_INT(2024, p.year);
    TEST_ASSERT_EQUAL_INT(2, p.month);
    TEST_ASSERT_EQUAL_INT(29, p.day);
    TEST_ASSERT_NOT_NULL(strstr(p.calendar_section, "Leap Year: Yes"));
}

void test_leap_year_total_days(void) {
    gt_page_t p = gt_today(JD_2024_FEB_29);
    TEST_ASSERT_NOT_NULL(strstr(p.calendar_section, "366"));
}

/* ============================================================
 * gt_format — output formatting
 * ============================================================ */

void test_format_returns_positive(void) {
    gt_page_t p = gt_today(JD_2026_MAR_20);
    char buf[4096];
    int len = gt_format(&p, buf, sizeof(buf));
    TEST_ASSERT_GREATER_THAN(0, len);
}

void test_format_contains_title(void) {
    gt_page_t p = gt_today(JD_2026_MAR_20);
    char buf[4096];
    gt_format(&p, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Gregorian"));
}

void test_format_null_page(void) {
    char buf[256];
    int len = gt_format(NULL, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_INT(0, len);
}

void test_format_null_buf(void) {
    gt_page_t p = gt_today(JD_2026_MAR_20);
    int len = gt_format(&p, NULL, 0);
    TEST_ASSERT_EQUAL_INT(0, len);
}

void test_format_small_buf(void) {
    gt_page_t p = gt_today(JD_2026_MAR_20);
    char buf[8];
    int len = gt_format(&p, buf, sizeof(buf));
    TEST_ASSERT_GREATER_THAN(0, len);
    TEST_ASSERT_TRUE(len < 8);
}

/* ============================================================
 * Winter date: December 25, 2026 (Friday)
 * ============================================================ */

#define JD_2026_DEC_25  2461399.5

void test_winter_season(void) {
    gt_page_t p = gt_today(JD_2026_DEC_25);
    TEST_ASSERT_EQUAL_INT(12, p.month);
    TEST_ASSERT_NOT_NULL(strstr(p.season_section, "Winter"));
}

/* ============================================================
 * Autumn date: October 15, 2026 (Thursday)
 * ============================================================ */

#define JD_2026_OCT_15  2461328.5

void test_autumn_season(void) {
    gt_page_t p = gt_today(JD_2026_OCT_15);
    TEST_ASSERT_EQUAL_INT(10, p.month);
    TEST_ASSERT_NOT_NULL(strstr(p.season_section, "Autumn"));
}

/* ============================================================
 * Runner
 * ============================================================ */

int main(void) {
    UNITY_BEGIN();

    /* basic fields */
    RUN_TEST(test_today_has_data);
    RUN_TEST(test_today_year);
    RUN_TEST(test_today_month);
    RUN_TEST(test_today_day);
    RUN_TEST(test_today_day_of_week);
    RUN_TEST(test_today_section_count);

    /* page_title */
    RUN_TEST(test_title_contains_gregorian);
    RUN_TEST(test_title_contains_march);
    RUN_TEST(test_title_contains_2026);
    RUN_TEST(test_title_contains_friday);

    /* date_section */
    RUN_TEST(test_date_section_day_of_year);
    RUN_TEST(test_date_section_week);
    RUN_TEST(test_date_section_quarter);

    /* month_section */
    RUN_TEST(test_month_section_label);
    RUN_TEST(test_month_section_origin);

    /* day_section */
    RUN_TEST(test_day_section_label);
    RUN_TEST(test_day_section_ruling_planet);

    /* season_section */
    RUN_TEST(test_season_section_label);
    RUN_TEST(test_season_section_spring);

    /* calendar_section */
    RUN_TEST(test_calendar_section_progress);
    RUN_TEST(test_calendar_section_remaining);
    RUN_TEST(test_calendar_section_leap_no);

    /* different season */
    RUN_TEST(test_summer_season);
    RUN_TEST(test_summer_quarter);

    /* leap year */
    RUN_TEST(test_leap_year_2024);
    RUN_TEST(test_leap_year_total_days);

    /* gt_format */
    RUN_TEST(test_format_returns_positive);
    RUN_TEST(test_format_contains_title);
    RUN_TEST(test_format_null_page);
    RUN_TEST(test_format_null_buf);
    RUN_TEST(test_format_small_buf);

    /* winter / autumn */
    RUN_TEST(test_winter_season);
    RUN_TEST(test_autumn_season);

    return UNITY_END();
}
