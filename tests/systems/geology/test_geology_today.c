/* test_geology_today.c — Tests for the geology_today page module.
 * Validates page composition from geo_time + geology_interpret data. */

#include "../../unity/unity.h"
#include "../../../src/systems/geology/geology_today.h"
#include <string.h>

/* JD 2460394.5 ≈ 2024-03-06, modern era. geo_jd_to_ma returns ~-0.000024,
 * clamped to 0.0 in gl_today — geologically "now". */
static const double TEST_JD = 2460394.5;

void setUp(void) {}
void tearDown(void) {}

/* --- gl_today basic --- */

void test_today_has_data(void) {
    gl_page_t p = gl_today(TEST_JD);
    TEST_ASSERT_EQUAL_INT(1, p.has_data);
}

void test_today_section_count(void) {
    gl_page_t p = gl_today(TEST_JD);
    TEST_ASSERT_EQUAL_INT(5, p.section_count);
}

/* --- page_title --- */

void test_title_contains_geology(void) {
    gl_page_t p = gl_today(TEST_JD);
    TEST_ASSERT_NOT_NULL(strstr(p.page_title, "Geology"));
}

void test_title_contains_phanerozoic(void) {
    gl_page_t p = gl_today(TEST_JD);
    TEST_ASSERT_NOT_NULL(strstr(p.page_title, "Phanerozoic"));
}

/* --- hierarchy_section --- */

void test_hierarchy_has_eon_label(void) {
    gl_page_t p = gl_today(TEST_JD);
    TEST_ASSERT_NOT_NULL(strstr(p.hierarchy_section, "Eon:"));
}

void test_hierarchy_has_era_label(void) {
    gl_page_t p = gl_today(TEST_JD);
    TEST_ASSERT_NOT_NULL(strstr(p.hierarchy_section, "Era:"));
}

void test_hierarchy_has_period_label(void) {
    gl_page_t p = gl_today(TEST_JD);
    TEST_ASSERT_NOT_NULL(strstr(p.hierarchy_section, "Period:"));
}

void test_hierarchy_has_epoch_label(void) {
    gl_page_t p = gl_today(TEST_JD);
    TEST_ASSERT_NOT_NULL(strstr(p.hierarchy_section, "Epoch:"));
}

void test_hierarchy_contains_phanerozoic(void) {
    gl_page_t p = gl_today(TEST_JD);
    TEST_ASSERT_NOT_NULL(strstr(p.hierarchy_section, "Phanerozoic"));
}

void test_hierarchy_contains_cenozoic(void) {
    gl_page_t p = gl_today(TEST_JD);
    TEST_ASSERT_NOT_NULL(strstr(p.hierarchy_section, "Cenozoic"));
}

void test_hierarchy_contains_quaternary(void) {
    gl_page_t p = gl_today(TEST_JD);
    TEST_ASSERT_NOT_NULL(strstr(p.hierarchy_section, "Quaternary"));
}

void test_hierarchy_contains_anthropocene(void) {
    /* At ma=0.0, geo_epoch_at returns Anthropocene (end_ma=0.0) */
    gl_page_t p = gl_today(TEST_JD);
    TEST_ASSERT_NOT_NULL(strstr(p.hierarchy_section, "Anthropocene"));
}

/* --- eon_section --- */

void test_eon_section_has_label(void) {
    gl_page_t p = gl_today(TEST_JD);
    TEST_ASSERT_NOT_NULL(strstr(p.eon_section, "Eon:"));
}

void test_eon_section_has_meaning(void) {
    gl_page_t p = gl_today(TEST_JD);
    /* gli_eon_data(3).meaning contains "visible" */
    TEST_ASSERT_NOT_NULL(strstr(p.eon_section, "visible"));
}

void test_eon_section_has_character(void) {
    gl_page_t p = gl_today(TEST_JD);
    TEST_ASSERT_NOT_NULL(strstr(p.eon_section, "Character:"));
}

void test_eon_section_has_life(void) {
    gl_page_t p = gl_today(TEST_JD);
    TEST_ASSERT_NOT_NULL(strstr(p.eon_section, "Life:"));
}

/* --- extinction_section --- */

void test_extinction_has_label(void) {
    gl_page_t p = gl_today(TEST_JD);
    TEST_ASSERT_NOT_NULL(strstr(p.extinction_section, "Extinction:"));
}

void test_extinction_has_time_unit(void) {
    gl_page_t p = gl_today(TEST_JD);
    TEST_ASSERT_NOT_NULL(strstr(p.extinction_section, "million years"));
}

void test_extinction_has_cretaceous(void) {
    gl_page_t p = gl_today(TEST_JD);
    /* K-Pg is index 4, name = "End-Cretaceous" */
    TEST_ASSERT_NOT_NULL(strstr(p.extinction_section, "Cretaceous"));
}

void test_extinction_has_aftermath(void) {
    gl_page_t p = gl_today(TEST_JD);
    TEST_ASSERT_NOT_NULL(strstr(p.extinction_section, "Aftermath:"));
}

/* --- deep_time_section --- */

void test_deep_time_earth_age(void) {
    gl_page_t p = gl_today(TEST_JD);
    TEST_ASSERT_NOT_NULL(strstr(p.deep_time_section, "Earth Age:"));
}

void test_deep_time_4540(void) {
    gl_page_t p = gl_today(TEST_JD);
    TEST_ASSERT_NOT_NULL(strstr(p.deep_time_section, "4,540"));
}

void test_deep_time_24_hours(void) {
    gl_page_t p = gl_today(TEST_JD);
    TEST_ASSERT_NOT_NULL(strstr(p.deep_time_section, "24 hours"));
}

void test_deep_time_last_extinction(void) {
    gl_page_t p = gl_today(TEST_JD);
    TEST_ASSERT_NOT_NULL(strstr(p.deep_time_section, "Last Extinction"));
}

/* --- perspective_section --- */

void test_perspective_nonempty(void) {
    gl_page_t p = gl_today(TEST_JD);
    TEST_ASSERT_TRUE(strlen(p.perspective_section) > 0);
}

void test_perspective_has_content(void) {
    gl_page_t p = gl_today(TEST_JD);
    /* gli_interpret(3, 4, -1).detail contains "Phanerozoic" */
    TEST_ASSERT_NOT_NULL(strstr(p.perspective_section, "Phanerozoic"));
}

/* --- gl_format --- */

void test_format_basic(void) {
    gl_page_t p = gl_today(TEST_JD);
    char buf[4096];
    int n = gl_format(&p, buf, (int)sizeof(buf));
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "Geology"));
}

void test_format_null_buf(void) {
    gl_page_t p = gl_today(TEST_JD);
    int n = gl_format(&p, NULL, 0);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_format_small_buf(void) {
    gl_page_t p = gl_today(TEST_JD);
    char buf[16];
    int n = gl_format(&p, buf, (int)sizeof(buf));
    /* Should write something but be truncated */
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_TRUE(n < 16);
}

void test_format_null_page(void) {
    char buf[256];
    int n = gl_format(NULL, buf, (int)sizeof(buf));
    TEST_ASSERT_EQUAL_INT(0, n);
}

/* --- Different JDs same geological result --- */

void test_different_jd_same_result(void) {
    gl_page_t p1 = gl_today(2460394.5);
    gl_page_t p2 = gl_today(2460000.0);
    /* Both are "now" geologically — same hierarchy */
    TEST_ASSERT_EQUAL_STRING(p1.page_title, p2.page_title);
    TEST_ASSERT_EQUAL_INT(p1.section_count, p2.section_count);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_today_has_data);
    RUN_TEST(test_today_section_count);
    RUN_TEST(test_title_contains_geology);
    RUN_TEST(test_title_contains_phanerozoic);
    RUN_TEST(test_hierarchy_has_eon_label);
    RUN_TEST(test_hierarchy_has_era_label);
    RUN_TEST(test_hierarchy_has_period_label);
    RUN_TEST(test_hierarchy_has_epoch_label);
    RUN_TEST(test_hierarchy_contains_phanerozoic);
    RUN_TEST(test_hierarchy_contains_cenozoic);
    RUN_TEST(test_hierarchy_contains_quaternary);
    RUN_TEST(test_hierarchy_contains_anthropocene);
    RUN_TEST(test_eon_section_has_label);
    RUN_TEST(test_eon_section_has_meaning);
    RUN_TEST(test_eon_section_has_character);
    RUN_TEST(test_eon_section_has_life);
    RUN_TEST(test_extinction_has_label);
    RUN_TEST(test_extinction_has_time_unit);
    RUN_TEST(test_extinction_has_cretaceous);
    RUN_TEST(test_extinction_has_aftermath);
    RUN_TEST(test_deep_time_earth_age);
    RUN_TEST(test_deep_time_4540);
    RUN_TEST(test_deep_time_24_hours);
    RUN_TEST(test_deep_time_last_extinction);
    RUN_TEST(test_perspective_nonempty);
    RUN_TEST(test_perspective_has_content);
    RUN_TEST(test_format_basic);
    RUN_TEST(test_format_null_buf);
    RUN_TEST(test_format_small_buf);
    RUN_TEST(test_format_null_page);
    RUN_TEST(test_different_jd_same_result);
    return UNITY_END();
}
