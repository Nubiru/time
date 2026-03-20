/* test_chinese_today.c — Tests for Chinese Calendar "Today" page module.
 * TDD RED phase: tests written before implementation.
 *
 * Test dates:
 *   JD 2460400.5 => Gregorian 2024 => Jia-Chen, Wood Dragon, Yang, cycle 41
 *   JD 2461131.5 => Gregorian 2026 => Bing-Wu, Fire Horse, Yang, cycle 43
 */

#include "../../unity/unity.h"
#include "../../../src/systems/chinese/chinese_today.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* === Known JD constants === */

/* Gregorian 2024 — Wood Dragon (Yang), Jia-Chen, cycle 41 */
static const double JD_2024 = 2460400.5;

/* Gregorian 2026 — Fire Horse (Yang), Bing-Wu, cycle 43 */
static const double JD_2026 = 2461131.5;

/* ===== ct_today — has_data and section_count ===== */

void test_today_has_data(void) {
    ct_page_t p = ct_today(JD_2024);
    TEST_ASSERT_EQUAL_INT(1, p.has_data);
}

void test_today_section_count(void) {
    ct_page_t p = ct_today(JD_2024);
    TEST_ASSERT_EQUAL_INT(5, p.section_count);
}

/* ===== page_title ===== */

void test_title_contains_year_of_the(void) {
    ct_page_t p = ct_today(JD_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.page_title, "Year of the"));
}

void test_title_contains_element_and_animal_2024(void) {
    ct_page_t p = ct_today(JD_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.page_title, "Wood"));
    TEST_ASSERT_NOT_NULL(strstr(p.page_title, "Dragon"));
}

void test_title_contains_element_and_animal_2026(void) {
    ct_page_t p = ct_today(JD_2026);
    TEST_ASSERT_NOT_NULL(strstr(p.page_title, "Fire"));
    TEST_ASSERT_NOT_NULL(strstr(p.page_title, "Horse"));
}

void test_title_contains_emoji(void) {
    ct_page_t p = ct_today(JD_2024);
    /* Dragon emoji is multi-byte UTF-8; title should have more than ASCII */
    TEST_ASSERT_TRUE(strlen(p.page_title) > 0);
}

/* ===== year_section ===== */

void test_year_section_contains_stem_branch(void) {
    ct_page_t p = ct_today(JD_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.year_section, "Stem-Branch:"));
}

void test_year_section_contains_jia_chen(void) {
    ct_page_t p = ct_today(JD_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.year_section, "Jia"));
    TEST_ASSERT_NOT_NULL(strstr(p.year_section, "Chen"));
}

void test_year_section_contains_sexagenary(void) {
    ct_page_t p = ct_today(JD_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.year_section, "Sexagenary Cycle:"));
}

void test_year_section_contains_cycle_41(void) {
    ct_page_t p = ct_today(JD_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.year_section, "Year 41 of 60"));
}

void test_year_section_contains_polarity_yang(void) {
    ct_page_t p = ct_today(JD_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.year_section, "Yang"));
}

void test_year_section_contains_element(void) {
    ct_page_t p = ct_today(JD_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.year_section, "Wood"));
}

void test_year_section_2026_bing_wu(void) {
    ct_page_t p = ct_today(JD_2026);
    TEST_ASSERT_NOT_NULL(strstr(p.year_section, "Bing"));
    TEST_ASSERT_NOT_NULL(strstr(p.year_section, "Wu"));
}

void test_year_section_2026_cycle_43(void) {
    ct_page_t p = ct_today(JD_2026);
    TEST_ASSERT_NOT_NULL(strstr(p.year_section, "Year 43 of 60"));
}

/* ===== animal_section ===== */

void test_animal_section_contains_animal_label(void) {
    ct_page_t p = ct_today(JD_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.animal_section, "Animal:"));
}

void test_animal_section_contains_archetype(void) {
    ct_page_t p = ct_today(JD_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.animal_section, "Archetype:"));
}

void test_animal_section_contains_strengths(void) {
    ct_page_t p = ct_today(JD_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.animal_section, "Strengths:"));
}

void test_animal_section_contains_challenges(void) {
    ct_page_t p = ct_today(JD_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.animal_section, "Challenges:"));
}

void test_animal_section_dragon_archetype(void) {
    ct_page_t p = ct_today(JD_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.animal_section, "The Emperor"));
}

void test_animal_section_horse_archetype(void) {
    ct_page_t p = ct_today(JD_2026);
    TEST_ASSERT_NOT_NULL(strstr(p.animal_section, "The Free Spirit"));
}

/* ===== element_section ===== */

void test_element_section_contains_element_label(void) {
    ct_page_t p = ct_today(JD_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.element_section, "Element:"));
}

void test_element_section_contains_nature(void) {
    ct_page_t p = ct_today(JD_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.element_section, "Nature:"));
}

void test_element_section_contains_season(void) {
    ct_page_t p = ct_today(JD_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.element_section, "Season:"));
}

void test_element_section_contains_expression(void) {
    ct_page_t p = ct_today(JD_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.element_section, "Expression:"));
}

void test_element_section_wood_season(void) {
    ct_page_t p = ct_today(JD_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.element_section, "Spring"));
}

void test_element_section_fire_season(void) {
    ct_page_t p = ct_today(JD_2026);
    TEST_ASSERT_NOT_NULL(strstr(p.element_section, "Summer"));
}

/* ===== compat_section ===== */

void test_compat_section_contains_compatible(void) {
    ct_page_t p = ct_today(JD_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.compat_section, "Compatible Animals:"));
}

void test_compat_section_dragon_compat(void) {
    /* Dragon compatible: Rat, Monkey, Rooster */
    ct_page_t p = ct_today(JD_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.compat_section, "Rat"));
    TEST_ASSERT_NOT_NULL(strstr(p.compat_section, "Monkey"));
    TEST_ASSERT_NOT_NULL(strstr(p.compat_section, "Rooster"));
}

void test_compat_section_horse_compat(void) {
    /* Horse compatible: Tiger, Goat, Dog */
    ct_page_t p = ct_today(JD_2026);
    TEST_ASSERT_NOT_NULL(strstr(p.compat_section, "Tiger"));
    TEST_ASSERT_NOT_NULL(strstr(p.compat_section, "Goat"));
    TEST_ASSERT_NOT_NULL(strstr(p.compat_section, "Dog"));
}

/* ===== cycle_section ===== */

void test_cycle_section_contains_cycle_position(void) {
    ct_page_t p = ct_today(JD_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.cycle_section, "Cycle Position:"));
}

void test_cycle_section_contains_of_60(void) {
    ct_page_t p = ct_today(JD_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.cycle_section, "of 60"));
}

void test_cycle_section_contains_years_until(void) {
    ct_page_t p = ct_today(JD_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.cycle_section, "Years until next cycle:"));
}

void test_cycle_section_2024_remaining_19(void) {
    /* cycle_year=41, remaining = 60-41 = 19 */
    ct_page_t p = ct_today(JD_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.cycle_section, "19"));
}

void test_cycle_section_2026_remaining_17(void) {
    /* cycle_year=43, remaining = 60-43 = 17 */
    ct_page_t p = ct_today(JD_2026);
    TEST_ASSERT_NOT_NULL(strstr(p.cycle_section, "17"));
}

void test_cycle_section_contains_phase(void) {
    ct_page_t p = ct_today(JD_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.cycle_section, "Current phase:"));
}

void test_cycle_section_2024_phase_late(void) {
    /* cycle_year=41, 31-45 => Late */
    ct_page_t p = ct_today(JD_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.cycle_section, "Late"));
}

void test_cycle_section_2026_phase_late(void) {
    /* cycle_year=43, 31-45 => Late */
    ct_page_t p = ct_today(JD_2026);
    TEST_ASSERT_NOT_NULL(strstr(p.cycle_section, "Late"));
}

/* ===== ct_format ===== */

void test_format_produces_output(void) {
    ct_page_t p = ct_today(JD_2024);
    char buf[4096];
    int len = ct_format(&p, buf, (int)sizeof(buf));
    TEST_ASSERT_TRUE(len > 0);
    TEST_ASSERT_TRUE(strlen(buf) > 0);
}

void test_format_contains_title(void) {
    ct_page_t p = ct_today(JD_2024);
    char buf[4096];
    ct_format(&p, buf, (int)sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Year of the"));
}

void test_format_contains_year_section(void) {
    ct_page_t p = ct_today(JD_2024);
    char buf[4096];
    ct_format(&p, buf, (int)sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Stem-Branch:"));
}

void test_format_contains_animal_section(void) {
    ct_page_t p = ct_today(JD_2024);
    char buf[4096];
    ct_format(&p, buf, (int)sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Archetype:"));
}

void test_format_contains_element_section(void) {
    ct_page_t p = ct_today(JD_2024);
    char buf[4096];
    ct_format(&p, buf, (int)sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Nature:"));
}

void test_format_contains_compat_section(void) {
    ct_page_t p = ct_today(JD_2024);
    char buf[4096];
    ct_format(&p, buf, (int)sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Compatible Animals:"));
}

void test_format_contains_cycle_section(void) {
    ct_page_t p = ct_today(JD_2024);
    char buf[4096];
    ct_format(&p, buf, (int)sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Cycle Position:"));
}

void test_format_null_page(void) {
    char buf[256];
    int len = ct_format(NULL, buf, (int)sizeof(buf));
    TEST_ASSERT_EQUAL_INT(0, len);
}

void test_format_null_buf(void) {
    ct_page_t p = ct_today(JD_2024);
    int len = ct_format(&p, NULL, 256);
    TEST_ASSERT_EQUAL_INT(0, len);
}

void test_format_zero_buf_size(void) {
    ct_page_t p = ct_today(JD_2024);
    char buf[1] = {'X'};
    int len = ct_format(&p, buf, 0);
    TEST_ASSERT_EQUAL_INT(0, len);
}

void test_format_small_buffer(void) {
    ct_page_t p = ct_today(JD_2024);
    char buf[32];
    int len = ct_format(&p, buf, (int)sizeof(buf));
    TEST_ASSERT_TRUE(len > 0);
    TEST_ASSERT_TRUE(len < 32);
    TEST_ASSERT_TRUE(strlen(buf) < 32);
}

/* ===== Different years produce different content ===== */

void test_different_years_different_animals(void) {
    ct_page_t p1 = ct_today(JD_2024);
    ct_page_t p2 = ct_today(JD_2026);
    /* 2024=Dragon, 2026=Horse — titles should differ */
    TEST_ASSERT_TRUE(strcmp(p1.page_title, p2.page_title) != 0);
}

void test_different_years_different_elements(void) {
    ct_page_t p1 = ct_today(JD_2024);
    ct_page_t p2 = ct_today(JD_2026);
    /* 2024=Wood, 2026=Fire — element sections should differ */
    TEST_ASSERT_TRUE(strcmp(p1.element_section, p2.element_section) != 0);
}

/* ===== All fields non-empty ===== */

void test_all_fields_nonempty_2024(void) {
    ct_page_t p = ct_today(JD_2024);
    TEST_ASSERT_TRUE(strlen(p.page_title) > 0);
    TEST_ASSERT_TRUE(strlen(p.year_section) > 0);
    TEST_ASSERT_TRUE(strlen(p.animal_section) > 0);
    TEST_ASSERT_TRUE(strlen(p.element_section) > 0);
    TEST_ASSERT_TRUE(strlen(p.compat_section) > 0);
    TEST_ASSERT_TRUE(strlen(p.cycle_section) > 0);
}

void test_all_fields_nonempty_2026(void) {
    ct_page_t p = ct_today(JD_2026);
    TEST_ASSERT_TRUE(strlen(p.page_title) > 0);
    TEST_ASSERT_TRUE(strlen(p.year_section) > 0);
    TEST_ASSERT_TRUE(strlen(p.animal_section) > 0);
    TEST_ASSERT_TRUE(strlen(p.element_section) > 0);
    TEST_ASSERT_TRUE(strlen(p.compat_section) > 0);
    TEST_ASSERT_TRUE(strlen(p.cycle_section) > 0);
}

/* ===== main ===== */

int main(void) {
    UNITY_BEGIN();

    /* has_data / section_count */
    RUN_TEST(test_today_has_data);
    RUN_TEST(test_today_section_count);

    /* page_title */
    RUN_TEST(test_title_contains_year_of_the);
    RUN_TEST(test_title_contains_element_and_animal_2024);
    RUN_TEST(test_title_contains_element_and_animal_2026);
    RUN_TEST(test_title_contains_emoji);

    /* year_section */
    RUN_TEST(test_year_section_contains_stem_branch);
    RUN_TEST(test_year_section_contains_jia_chen);
    RUN_TEST(test_year_section_contains_sexagenary);
    RUN_TEST(test_year_section_contains_cycle_41);
    RUN_TEST(test_year_section_contains_polarity_yang);
    RUN_TEST(test_year_section_contains_element);
    RUN_TEST(test_year_section_2026_bing_wu);
    RUN_TEST(test_year_section_2026_cycle_43);

    /* animal_section */
    RUN_TEST(test_animal_section_contains_animal_label);
    RUN_TEST(test_animal_section_contains_archetype);
    RUN_TEST(test_animal_section_contains_strengths);
    RUN_TEST(test_animal_section_contains_challenges);
    RUN_TEST(test_animal_section_dragon_archetype);
    RUN_TEST(test_animal_section_horse_archetype);

    /* element_section */
    RUN_TEST(test_element_section_contains_element_label);
    RUN_TEST(test_element_section_contains_nature);
    RUN_TEST(test_element_section_contains_season);
    RUN_TEST(test_element_section_contains_expression);
    RUN_TEST(test_element_section_wood_season);
    RUN_TEST(test_element_section_fire_season);

    /* compat_section */
    RUN_TEST(test_compat_section_contains_compatible);
    RUN_TEST(test_compat_section_dragon_compat);
    RUN_TEST(test_compat_section_horse_compat);

    /* cycle_section */
    RUN_TEST(test_cycle_section_contains_cycle_position);
    RUN_TEST(test_cycle_section_contains_of_60);
    RUN_TEST(test_cycle_section_contains_years_until);
    RUN_TEST(test_cycle_section_2024_remaining_19);
    RUN_TEST(test_cycle_section_2026_remaining_17);
    RUN_TEST(test_cycle_section_contains_phase);
    RUN_TEST(test_cycle_section_2024_phase_late);
    RUN_TEST(test_cycle_section_2026_phase_late);

    /* ct_format */
    RUN_TEST(test_format_produces_output);
    RUN_TEST(test_format_contains_title);
    RUN_TEST(test_format_contains_year_section);
    RUN_TEST(test_format_contains_animal_section);
    RUN_TEST(test_format_contains_element_section);
    RUN_TEST(test_format_contains_compat_section);
    RUN_TEST(test_format_contains_cycle_section);
    RUN_TEST(test_format_null_page);
    RUN_TEST(test_format_null_buf);
    RUN_TEST(test_format_zero_buf_size);
    RUN_TEST(test_format_small_buffer);

    /* Different years */
    RUN_TEST(test_different_years_different_animals);
    RUN_TEST(test_different_years_different_elements);

    /* All fields non-empty */
    RUN_TEST(test_all_fields_nonempty_2024);
    RUN_TEST(test_all_fields_nonempty_2026);

    return UNITY_END();
}
