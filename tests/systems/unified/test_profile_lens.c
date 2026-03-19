/* test_profile_lens.c — Tests for multi-lens profile presentation.
 * TDD: tests written first, implementation follows. */

#include "unity.h"
#include "systems/unified/profile_lens.h"
#include "systems/unified/cosmic_fingerprint.h"
#include "systems/unified/birth_profile.h"

#include <string.h>

static birth_profile_t s_profile;
static cf_fingerprint_t s_fp;

void setUp(void) {
    s_profile = bp_compute(1990, 6, 15);
    s_fp = cf_compute(&s_profile);
}

void tearDown(void) {}

/* ===== Lens Names ===== */

void test_lens_name_pattern(void) {
    TEST_ASSERT_EQUAL_STRING("Your Pattern", pl_lens_name(PL_LENS_PATTERN));
}

void test_lens_name_chart(void) {
    TEST_ASSERT_EQUAL_STRING("Your Chart", pl_lens_name(PL_LENS_CHART));
}

void test_lens_name_calendar(void) {
    TEST_ASSERT_EQUAL_STRING("Your Calendar", pl_lens_name(PL_LENS_CALENDAR));
}

void test_lens_name_energy(void) {
    TEST_ASSERT_EQUAL_STRING("Your Energy", pl_lens_name(PL_LENS_ENERGY));
}

void test_lens_name_invalid(void) {
    TEST_ASSERT_EQUAL_STRING("?", pl_lens_name(PL_LENS_COUNT));
    TEST_ASSERT_EQUAL_STRING("?", pl_lens_name(-1));
}

/* ===== Lens Subtitles ===== */

void test_lens_subtitle_pattern(void) {
    const char *s = pl_lens_subtitle(PL_LENS_PATTERN);
    TEST_ASSERT_NOT_NULL(s);
    TEST_ASSERT_TRUE(strlen(s) > 0);
}

void test_lens_subtitle_invalid(void) {
    TEST_ASSERT_EQUAL_STRING("?", pl_lens_subtitle(PL_LENS_COUNT));
}

/* ===== Lens Count ===== */

void test_lens_count(void) {
    TEST_ASSERT_EQUAL_INT(PL_LENS_COUNT, pl_lens_count());
}

/* ===== Compose: Pattern Lens ===== */

void test_compose_pattern_has_title(void) {
    pl_view_t v = pl_compose(&s_fp, PL_LENS_PATTERN);
    TEST_ASSERT_EQUAL_STRING("Your Pattern", v.title);
}

void test_compose_pattern_has_sections(void) {
    pl_view_t v = pl_compose(&s_fp, PL_LENS_PATTERN);
    TEST_ASSERT_TRUE(v.section_count > 0);
    TEST_ASSERT_TRUE(v.section_count <= PL_MAX_SECTIONS);
}

void test_compose_pattern_sections_have_titles(void) {
    pl_view_t v = pl_compose(&s_fp, PL_LENS_PATTERN);
    for (int i = 0; i < v.section_count; i++) {
        TEST_ASSERT_TRUE(strlen(v.sections[i].title) > 0);
    }
}

void test_compose_pattern_sections_have_body(void) {
    pl_view_t v = pl_compose(&s_fp, PL_LENS_PATTERN);
    for (int i = 0; i < v.section_count; i++) {
        TEST_ASSERT_TRUE(strlen(v.sections[i].body) > 0);
    }
}

/* ===== Compose: Chart Lens ===== */

void test_compose_chart_has_title(void) {
    pl_view_t v = pl_compose(&s_fp, PL_LENS_CHART);
    TEST_ASSERT_EQUAL_STRING("Your Chart", v.title);
}

void test_compose_chart_has_sections(void) {
    pl_view_t v = pl_compose(&s_fp, PL_LENS_CHART);
    TEST_ASSERT_TRUE(v.section_count > 0);
}

/* ===== Compose: Calendar Lens ===== */

void test_compose_calendar_has_title(void) {
    pl_view_t v = pl_compose(&s_fp, PL_LENS_CALENDAR);
    TEST_ASSERT_EQUAL_STRING("Your Calendar", v.title);
}

void test_compose_calendar_has_sections(void) {
    pl_view_t v = pl_compose(&s_fp, PL_LENS_CALENDAR);
    TEST_ASSERT_TRUE(v.section_count > 0);
}

/* ===== Compose: Energy Lens ===== */

void test_compose_energy_has_title(void) {
    pl_view_t v = pl_compose(&s_fp, PL_LENS_ENERGY);
    TEST_ASSERT_EQUAL_STRING("Your Energy", v.title);
}

void test_compose_energy_has_sections(void) {
    pl_view_t v = pl_compose(&s_fp, PL_LENS_ENERGY);
    TEST_ASSERT_TRUE(v.section_count > 0);
}

/* ===== Null Safety ===== */

void test_compose_null_fingerprint(void) {
    pl_view_t v = pl_compose(NULL, PL_LENS_PATTERN);
    TEST_ASSERT_EQUAL_INT(0, v.section_count);
    TEST_ASSERT_EQUAL_STRING("", v.title);
}

void test_compose_invalid_lens(void) {
    pl_view_t v = pl_compose(&s_fp, PL_LENS_COUNT);
    TEST_ASSERT_EQUAL_INT(0, v.section_count);
}

/* ===== Different Lenses Produce Different Output ===== */

void test_lenses_differ(void) {
    pl_view_t pattern = pl_compose(&s_fp, PL_LENS_PATTERN);
    pl_view_t chart = pl_compose(&s_fp, PL_LENS_CHART);
    pl_view_t calendar = pl_compose(&s_fp, PL_LENS_CALENDAR);
    pl_view_t energy = pl_compose(&s_fp, PL_LENS_ENERGY);

    /* Titles must differ */
    TEST_ASSERT_TRUE(strcmp(pattern.title, chart.title) != 0);
    TEST_ASSERT_TRUE(strcmp(chart.title, calendar.title) != 0);
    TEST_ASSERT_TRUE(strcmp(calendar.title, energy.title) != 0);
}

/* ===== Format View ===== */

void test_format_view_basic(void) {
    pl_view_t v = pl_compose(&s_fp, PL_LENS_PATTERN);
    char buf[2048];
    int n = pl_format_view(&v, buf, sizeof(buf));
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_TRUE(strlen(buf) > 0);
    /* Should contain the title */
    TEST_ASSERT_NOT_NULL(strstr(buf, "Your Pattern"));
}

void test_format_view_null_view(void) {
    char buf[64];
    int n = pl_format_view(NULL, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_INT(0, n);
    TEST_ASSERT_EQUAL_STRING("", buf);
}

void test_format_view_null_buf(void) {
    pl_view_t v = pl_compose(&s_fp, PL_LENS_PATTERN);
    int n = pl_format_view(&v, NULL, 0);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_format_view_tiny_buf(void) {
    pl_view_t v = pl_compose(&s_fp, PL_LENS_PATTERN);
    char buf[4];
    int n = pl_format_view(&v, buf, sizeof(buf));
    TEST_ASSERT_TRUE(n <= 3);
}

/* ===== Sections contain birth-specific data ===== */

void test_pattern_contains_birth_data(void) {
    pl_view_t v = pl_compose(&s_fp, PL_LENS_PATTERN);
    char buf[2048];
    pl_format_view(&v, buf, sizeof(buf));
    /* Born 1990-06-15: Gemini sun sign. Pattern lens should reference this. */
    TEST_ASSERT_NOT_NULL(strstr(buf, "Gemini"));
}

void test_calendar_contains_birth_data(void) {
    pl_view_t v = pl_compose(&s_fp, PL_LENS_CALENDAR);
    char buf[2048];
    pl_format_view(&v, buf, sizeof(buf));
    /* Calendar lens should reference Gregorian date */
    TEST_ASSERT_NOT_NULL(strstr(buf, "1990"));
}

/* ===== Each lens's section count ===== */

void test_pattern_section_count(void) {
    pl_view_t v = pl_compose(&s_fp, PL_LENS_PATTERN);
    /* Pattern: astrology, tzolkin, i ching — personality-focused systems */
    TEST_ASSERT_TRUE(v.section_count >= 2);
    TEST_ASSERT_TRUE(v.section_count <= PL_MAX_SECTIONS);
}

void test_chart_section_count(void) {
    pl_view_t v = pl_compose(&s_fp, PL_LENS_CHART);
    /* Chart: astrology, human design — astronomical position systems */
    TEST_ASSERT_TRUE(v.section_count >= 2);
}

void test_calendar_section_count(void) {
    pl_view_t v = pl_compose(&s_fp, PL_LENS_CALENDAR);
    /* Calendar: gregorian, chinese, hebrew, islamic, buddhist */
    TEST_ASSERT_TRUE(v.section_count >= 3);
}

void test_energy_section_count(void) {
    pl_view_t v = pl_compose(&s_fp, PL_LENS_ENERGY);
    /* Energy: human design, tzolkin, i ching — spiritual/esoteric systems */
    TEST_ASSERT_TRUE(v.section_count >= 2);
}

int main(void) {
    UNITY_BEGIN();

    /* Lens names */
    RUN_TEST(test_lens_name_pattern);
    RUN_TEST(test_lens_name_chart);
    RUN_TEST(test_lens_name_calendar);
    RUN_TEST(test_lens_name_energy);
    RUN_TEST(test_lens_name_invalid);

    /* Subtitles */
    RUN_TEST(test_lens_subtitle_pattern);
    RUN_TEST(test_lens_subtitle_invalid);

    /* Count */
    RUN_TEST(test_lens_count);

    /* Compose: each lens */
    RUN_TEST(test_compose_pattern_has_title);
    RUN_TEST(test_compose_pattern_has_sections);
    RUN_TEST(test_compose_pattern_sections_have_titles);
    RUN_TEST(test_compose_pattern_sections_have_body);
    RUN_TEST(test_compose_chart_has_title);
    RUN_TEST(test_compose_chart_has_sections);
    RUN_TEST(test_compose_calendar_has_title);
    RUN_TEST(test_compose_calendar_has_sections);
    RUN_TEST(test_compose_energy_has_title);
    RUN_TEST(test_compose_energy_has_sections);

    /* Null safety */
    RUN_TEST(test_compose_null_fingerprint);
    RUN_TEST(test_compose_invalid_lens);

    /* Lenses differ */
    RUN_TEST(test_lenses_differ);

    /* Format */
    RUN_TEST(test_format_view_basic);
    RUN_TEST(test_format_view_null_view);
    RUN_TEST(test_format_view_null_buf);
    RUN_TEST(test_format_view_tiny_buf);

    /* Birth-specific content */
    RUN_TEST(test_pattern_contains_birth_data);
    RUN_TEST(test_calendar_contains_birth_data);

    /* Section counts */
    RUN_TEST(test_pattern_section_count);
    RUN_TEST(test_chart_section_count);
    RUN_TEST(test_calendar_section_count);
    RUN_TEST(test_energy_section_count);

    return UNITY_END();
}
