/* test_pop_today.c — Tests for the population today-page module.
 * Validates page composition from human_population data. */

#include "../../unity/unity.h"
#include "../../../src/systems/earth/pop_today.h"
#include <string.h>

/* JD 2460310.5 ~ 2024-01-13, modern era with ~8.1 billion alive. */
static const double JD_2024 = 2460310.5;

/* JD for ~1800: 2451545.0 - 200*365.25 = 2378495.0 */
static const double JD_1800 = 2378495.0;

/* JD for ~year 1: 2451545.0 - 1999*365.25 = 1721424.75 */
static const double JD_YEAR1 = 1721424.75;

void setUp(void) {}
void tearDown(void) {}

/* === pt_today basic (2024) === */

void test_pt_today_has_data(void) {
    pt_page_t p = pt_today(JD_2024);
    TEST_ASSERT_EQUAL_INT(1, p.has_data);
}

void test_pt_today_section_count(void) {
    pt_page_t p = pt_today(JD_2024);
    TEST_ASSERT_EQUAL_INT(5, p.section_count);
}

/* === page_title === */

void test_title_contains_population(void) {
    pt_page_t p = pt_today(JD_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.page_title, "Population"));
}

void test_title_contains_billion(void) {
    pt_page_t p = pt_today(JD_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.page_title, "billion"));
}

void test_title_contains_alive(void) {
    pt_page_t p = pt_today(JD_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.page_title, "alive"));
}

/* === current_section === */

void test_current_section_nonempty(void) {
    pt_page_t p = pt_today(JD_2024);
    TEST_ASSERT_TRUE(strlen(p.current_section) > 0);
}

void test_current_section_has_era(void) {
    pt_page_t p = pt_today(JD_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.current_section, "Era:"));
}

void test_current_section_has_year(void) {
    pt_page_t p = pt_today(JD_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.current_section, "Year:"));
}

/* === fraction_section === */

void test_fraction_section_nonempty(void) {
    pt_page_t p = pt_today(JD_2024);
    TEST_ASSERT_TRUE(strlen(p.fraction_section) > 0);
}

void test_fraction_section_has_percent(void) {
    pt_page_t p = pt_today(JD_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.fraction_section, "%"));
}

void test_fraction_section_has_right_now(void) {
    pt_page_t p = pt_today(JD_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.fraction_section, "RIGHT NOW"));
}

void test_fraction_section_has_billion(void) {
    pt_page_t p = pt_today(JD_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.fraction_section, "billion"));
}

/* === milestone_section === */

void test_milestone_section_nonempty(void) {
    pt_page_t p = pt_today(JD_2024);
    TEST_ASSERT_TRUE(strlen(p.milestone_section) > 0);
}

void test_milestone_has_latest(void) {
    pt_page_t p = pt_today(JD_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.milestone_section, "Latest Milestone"));
}

void test_milestone_has_8_billion(void) {
    pt_page_t p = pt_today(JD_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.milestone_section, "8 billion"));
}

/* === perspective_section === */

void test_perspective_section_nonempty(void) {
    pt_page_t p = pt_today(JD_2024);
    TEST_ASSERT_TRUE(strlen(p.perspective_section) > 0);
}

void test_perspective_has_pre_agriculture(void) {
    pt_page_t p = pt_today(JD_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.perspective_section, "Pre-agriculture"));
}

void test_perspective_has_year1(void) {
    pt_page_t p = pt_today(JD_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.perspective_section, "Year 1 CE"));
}

void test_perspective_has_growth_factor(void) {
    pt_page_t p = pt_today(JD_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.perspective_section, "Growth factor"));
}

/* === pt_format === */

void test_format_positive_chars(void) {
    pt_page_t p = pt_today(JD_2024);
    char buf[4096];
    int n = pt_format(&p, buf, (int)sizeof(buf));
    TEST_ASSERT_TRUE(n > 0);
}

void test_format_contains_population(void) {
    pt_page_t p = pt_today(JD_2024);
    char buf[4096];
    pt_format(&p, buf, (int)sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Population"));
}

void test_format_null_page(void) {
    char buf[256];
    int n = pt_format(NULL, buf, (int)sizeof(buf));
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_format_null_buf(void) {
    pt_page_t p = pt_today(JD_2024);
    int n = pt_format(&p, NULL, 0);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_format_tiny_buf(void) {
    pt_page_t p = pt_today(JD_2024);
    char buf[16];
    int n = pt_format(&p, buf, (int)sizeof(buf));
    /* Should truncate gracefully */
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_TRUE(n <= 15);
}

void test_format_no_data(void) {
    pt_page_t p;
    memset(&p, 0, sizeof(p));
    p.has_data = 0;
    char buf[256];
    int n = pt_format(&p, buf, (int)sizeof(buf));
    TEST_ASSERT_EQUAL_INT(0, n);
}

/* === Historical JD: ~1800 (pre-billion era) === */

void test_1800_has_data(void) {
    pt_page_t p = pt_today(JD_1800);
    TEST_ASSERT_EQUAL_INT(1, p.has_data);
}

void test_1800_section_count(void) {
    pt_page_t p = pt_today(JD_1800);
    TEST_ASSERT_EQUAL_INT(5, p.section_count);
}

void test_1800_milestone_pre_billion(void) {
    /* In 1800 the population was ~989 million, below 1 billion.
     * nearest_milestone_billions should be 0 -> "No billion milestone" */
    pt_page_t p = pt_today(JD_1800);
    /* The milestone section should still be non-empty */
    TEST_ASSERT_TRUE(strlen(p.milestone_section) > 0);
}

/* === Historical JD: ~year 1 === */

void test_year1_has_data(void) {
    pt_page_t p = pt_today(JD_YEAR1);
    TEST_ASSERT_EQUAL_INT(1, p.has_data);
}

void test_year1_section_count(void) {
    pt_page_t p = pt_today(JD_YEAR1);
    TEST_ASSERT_EQUAL_INT(5, p.section_count);
}

void test_year1_title_contains_population(void) {
    pt_page_t p = pt_today(JD_YEAR1);
    TEST_ASSERT_NOT_NULL(strstr(p.page_title, "Population"));
}

void test_year1_perspective_nonempty(void) {
    pt_page_t p = pt_today(JD_YEAR1);
    TEST_ASSERT_TRUE(strlen(p.perspective_section) > 0);
}

int main(void) {
    UNITY_BEGIN();

    /* pt_today basic (2024) */
    RUN_TEST(test_pt_today_has_data);
    RUN_TEST(test_pt_today_section_count);

    /* page_title */
    RUN_TEST(test_title_contains_population);
    RUN_TEST(test_title_contains_billion);
    RUN_TEST(test_title_contains_alive);

    /* current_section */
    RUN_TEST(test_current_section_nonempty);
    RUN_TEST(test_current_section_has_era);
    RUN_TEST(test_current_section_has_year);

    /* fraction_section */
    RUN_TEST(test_fraction_section_nonempty);
    RUN_TEST(test_fraction_section_has_percent);
    RUN_TEST(test_fraction_section_has_right_now);
    RUN_TEST(test_fraction_section_has_billion);

    /* milestone_section */
    RUN_TEST(test_milestone_section_nonempty);
    RUN_TEST(test_milestone_has_latest);
    RUN_TEST(test_milestone_has_8_billion);

    /* perspective_section */
    RUN_TEST(test_perspective_section_nonempty);
    RUN_TEST(test_perspective_has_pre_agriculture);
    RUN_TEST(test_perspective_has_year1);
    RUN_TEST(test_perspective_has_growth_factor);

    /* pt_format */
    RUN_TEST(test_format_positive_chars);
    RUN_TEST(test_format_contains_population);
    RUN_TEST(test_format_null_page);
    RUN_TEST(test_format_null_buf);
    RUN_TEST(test_format_tiny_buf);
    RUN_TEST(test_format_no_data);

    /* Historical: ~1800 */
    RUN_TEST(test_1800_has_data);
    RUN_TEST(test_1800_section_count);
    RUN_TEST(test_1800_milestone_pre_billion);

    /* Historical: ~year 1 */
    RUN_TEST(test_year1_has_data);
    RUN_TEST(test_year1_section_count);
    RUN_TEST(test_year1_title_contains_population);
    RUN_TEST(test_year1_perspective_nonempty);

    return UNITY_END();
}
