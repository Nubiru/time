/* test_system_theme.c — Tests for per-system CSS custom properties
 *
 * Pure tests: no GL, no malloc, no globals. */

#include "../../tests/unity/unity.h"
#include "../../src/ui/system_theme.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ================================================================
 * st_system_slug — all 13 valid systems
 * ================================================================ */

void test_slug_astronomy(void) {
    TEST_ASSERT_EQUAL_STRING("astronomy", st_system_slug(CT_SYSTEM_ASTRONOMY));
}

void test_slug_astrology(void) {
    TEST_ASSERT_EQUAL_STRING("astrology", st_system_slug(CT_SYSTEM_ASTROLOGY));
}

void test_slug_tzolkin(void) {
    TEST_ASSERT_EQUAL_STRING("tzolkin", st_system_slug(CT_SYSTEM_TZOLKIN));
}

void test_slug_iching(void) {
    TEST_ASSERT_EQUAL_STRING("iching", st_system_slug(CT_SYSTEM_ICHING));
}

void test_slug_chinese(void) {
    TEST_ASSERT_EQUAL_STRING("chinese", st_system_slug(CT_SYSTEM_CHINESE));
}

void test_slug_human_design(void) {
    TEST_ASSERT_EQUAL_STRING("human-design", st_system_slug(CT_SYSTEM_HUMAN_DESIGN));
}

void test_slug_gregorian(void) {
    TEST_ASSERT_EQUAL_STRING("gregorian", st_system_slug(CT_SYSTEM_GREGORIAN));
}

void test_slug_kabbalah(void) {
    TEST_ASSERT_EQUAL_STRING("kabbalah", st_system_slug(CT_SYSTEM_KABBALAH));
}

void test_slug_hindu(void) {
    TEST_ASSERT_EQUAL_STRING("hindu", st_system_slug(CT_SYSTEM_HINDU));
}

void test_slug_buddhist(void) {
    TEST_ASSERT_EQUAL_STRING("buddhist", st_system_slug(CT_SYSTEM_BUDDHIST));
}

void test_slug_hebrew(void) {
    TEST_ASSERT_EQUAL_STRING("hebrew", st_system_slug(CT_SYSTEM_HEBREW));
}

void test_slug_islamic(void) {
    TEST_ASSERT_EQUAL_STRING("islamic", st_system_slug(CT_SYSTEM_ISLAMIC));
}

void test_slug_geology(void) {
    TEST_ASSERT_EQUAL_STRING("geology", st_system_slug(CT_SYSTEM_GEOLOGY));
}

/* ================================================================
 * st_system_slug — invalid indices
 * ================================================================ */

void test_slug_negative_returns_unknown(void) {
    TEST_ASSERT_EQUAL_STRING("unknown", st_system_slug(-1));
}

void test_slug_too_large_returns_unknown(void) {
    TEST_ASSERT_EQUAL_STRING("unknown", st_system_slug(CT_SYSTEM_COUNT));
}

void test_slug_way_too_large_returns_unknown(void) {
    TEST_ASSERT_EQUAL_STRING("unknown", st_system_slug(999));
}

/* ================================================================
 * st_system_count
 * ================================================================ */

void test_system_count_equals_ct_system_count(void) {
    TEST_ASSERT_EQUAL_INT(CT_SYSTEM_COUNT, st_system_count());
}

void test_system_count_is_19(void) {
    TEST_ASSERT_EQUAL_INT(19, st_system_count());
}

/* ================================================================
 * st_system_colors — non-zero values for valid systems
 * ================================================================ */

void test_astronomy_colors_nonzero(void) {
    st_system_colors_t c = st_system_colors(CT_SYSTEM_ASTRONOMY, THEME_COSMOS);
    /* At least one channel of primary must be nonzero */
    TEST_ASSERT_TRUE(c.primary.r > 0 || c.primary.g > 0 || c.primary.b > 0);
    TEST_ASSERT_EQUAL_STRING("astronomy", c.slug);
}

void test_tzolkin_colors_nonzero(void) {
    st_system_colors_t c = st_system_colors(CT_SYSTEM_TZOLKIN, THEME_COSMOS);
    TEST_ASSERT_TRUE(c.primary.r > 0 || c.primary.g > 0 || c.primary.b > 0);
    TEST_ASSERT_TRUE(c.secondary.r > 0 || c.secondary.g > 0 || c.secondary.b > 0);
    TEST_ASSERT_TRUE(c.accent.r > 0 || c.accent.g > 0 || c.accent.b > 0);
}

void test_geology_colors_nonzero(void) {
    st_system_colors_t c = st_system_colors(CT_SYSTEM_GEOLOGY, THEME_COSMOS);
    TEST_ASSERT_TRUE(c.primary.r > 0 || c.primary.g > 0 || c.primary.b > 0);
}

/* ================================================================
 * st_system_colors — invalid system returns all zeros
 * ================================================================ */

void test_invalid_system_returns_zeros(void) {
    st_system_colors_t c = st_system_colors(-1, THEME_COSMOS);
    TEST_ASSERT_EQUAL_INT(0, c.primary.r);
    TEST_ASSERT_EQUAL_INT(0, c.primary.g);
    TEST_ASSERT_EQUAL_INT(0, c.primary.b);
    TEST_ASSERT_EQUAL_INT(0, c.secondary.r);
    TEST_ASSERT_EQUAL_INT(0, c.accent.r);
    TEST_ASSERT_EQUAL_INT(0, c.surface.r);
    TEST_ASSERT_NULL(c.slug);
}

void test_invalid_system_too_large_returns_zeros(void) {
    st_system_colors_t c = st_system_colors(CT_SYSTEM_COUNT, THEME_COSMOS);
    TEST_ASSERT_EQUAL_INT(0, c.primary.r);
    TEST_ASSERT_NULL(c.slug);
}

/* ================================================================
 * st_system_colors — RGB values in 0-255 range
 * ================================================================ */

void test_rgb_values_in_range(void) {
    for (int i = 0; i < CT_SYSTEM_COUNT; i++) {
        st_system_colors_t c = st_system_colors(i, THEME_COSMOS);
        TEST_ASSERT_TRUE(c.primary.r >= 0 && c.primary.r <= 255);
        TEST_ASSERT_TRUE(c.primary.g >= 0 && c.primary.g <= 255);
        TEST_ASSERT_TRUE(c.primary.b >= 0 && c.primary.b <= 255);
        TEST_ASSERT_TRUE(c.secondary.r >= 0 && c.secondary.r <= 255);
        TEST_ASSERT_TRUE(c.secondary.g >= 0 && c.secondary.g <= 255);
        TEST_ASSERT_TRUE(c.secondary.b >= 0 && c.secondary.b <= 255);
        TEST_ASSERT_TRUE(c.accent.r >= 0 && c.accent.r <= 255);
        TEST_ASSERT_TRUE(c.accent.g >= 0 && c.accent.g <= 255);
        TEST_ASSERT_TRUE(c.accent.b >= 0 && c.accent.b <= 255);
        TEST_ASSERT_TRUE(c.surface.r >= 0 && c.surface.r <= 255);
        TEST_ASSERT_TRUE(c.surface.g >= 0 && c.surface.g <= 255);
        TEST_ASSERT_TRUE(c.surface.b >= 0 && c.surface.b <= 255);
    }
}

/* ================================================================
 * st_system_colors — Cosmos vs Dawn produces different surface
 * ================================================================ */

void test_cosmos_vs_dawn_different_surface(void) {
    st_system_colors_t cosmos = st_system_colors(CT_SYSTEM_ASTRONOMY, THEME_COSMOS);
    st_system_colors_t dawn = st_system_colors(CT_SYSTEM_ASTRONOMY, THEME_DAWN);
    /* Surface colors must differ between themes */
    int same = (cosmos.surface.r == dawn.surface.r &&
                cosmos.surface.g == dawn.surface.g &&
                cosmos.surface.b == dawn.surface.b);
    TEST_ASSERT_FALSE(same);
}

/* ================================================================
 * st_css_vars — output non-empty
 * ================================================================ */

void test_css_vars_nonempty(void) {
    char buf[ST_CSS_BUF_SIZE];
    int n = st_css_vars(THEME_COSMOS, buf, (int)sizeof(buf));
    TEST_ASSERT_GREATER_THAN(0, n);
    TEST_ASSERT_GREATER_THAN(0, (int)strlen(buf));
}

/* ================================================================
 * st_css_vars — contains all 13 system slugs
 * ================================================================ */

void test_css_vars_contains_all_slugs(void) {
    char buf[ST_CSS_BUF_SIZE];
    st_css_vars(THEME_COSMOS, buf, (int)sizeof(buf));

    TEST_ASSERT_NOT_NULL(strstr(buf, "--sys-astronomy-"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "--sys-astrology-"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "--sys-tzolkin-"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "--sys-iching-"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "--sys-chinese-"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "--sys-human-design-"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "--sys-gregorian-"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "--sys-kabbalah-"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "--sys-hindu-"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "--sys-buddhist-"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "--sys-hebrew-"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "--sys-islamic-"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "--sys-geology-"));
}

/* ================================================================
 * st_css_vars — each system has 4 properties
 * ================================================================ */

void test_css_vars_has_four_properties_per_system(void) {
    char buf[ST_CSS_BUF_SIZE];
    st_css_vars(THEME_COSMOS, buf, (int)sizeof(buf));

    /* Spot-check astronomy for all 4 */
    TEST_ASSERT_NOT_NULL(strstr(buf, "--sys-astronomy-primary:"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "--sys-astronomy-secondary:"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "--sys-astronomy-accent:"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "--sys-astronomy-surface:"));

    /* Spot-check geology for all 4 */
    TEST_ASSERT_NOT_NULL(strstr(buf, "--sys-geology-primary:"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "--sys-geology-secondary:"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "--sys-geology-accent:"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "--sys-geology-surface:"));
}

/* ================================================================
 * st_css_vars — no decimal points (integer RGB)
 * ================================================================ */

void test_css_vars_no_decimals(void) {
    char buf[ST_CSS_BUF_SIZE];
    st_css_vars(THEME_COSMOS, buf, (int)sizeof(buf));
    /* Integer output — no decimal points */
    TEST_ASSERT_NULL(strchr(buf, '.'));
}

/* ================================================================
 * st_css_vars — buffer too small truncates gracefully
 * ================================================================ */

void test_css_vars_small_buffer_truncates(void) {
    char buf[64];
    int n = st_css_vars(THEME_COSMOS, buf, (int)sizeof(buf));
    TEST_ASSERT_GREATER_THAN(0, n);
    TEST_ASSERT_TRUE(n < (int)sizeof(buf));
    TEST_ASSERT_EQUAL_CHAR('\0', buf[n]);
}

/* ================================================================
 * st_css_vars — NULL buffer returns 0
 * ================================================================ */

void test_css_vars_null_buffer_returns_zero(void) {
    int n = st_css_vars(THEME_COSMOS, NULL, ST_CSS_BUF_SIZE);
    TEST_ASSERT_EQUAL_INT(0, n);
}

/* ================================================================
 * st_css_vars — zero buffer size returns 0
 * ================================================================ */

void test_css_vars_zero_bufsize_returns_zero(void) {
    char buf[64];
    int n = st_css_vars(THEME_COSMOS, buf, 0);
    TEST_ASSERT_EQUAL_INT(0, n);
}

/* ================================================================
 * st_css_vars — Dawn produces different output from Cosmos
 * ================================================================ */

void test_css_vars_dawn_differs(void) {
    char cosmos[ST_CSS_BUF_SIZE], dawn[ST_CSS_BUF_SIZE];
    st_css_vars(THEME_COSMOS, cosmos, (int)sizeof(cosmos));
    st_css_vars(THEME_DAWN, dawn, (int)sizeof(dawn));
    TEST_ASSERT_TRUE(strcmp(cosmos, dawn) != 0);
}

/* ================================================================
 * st_css_vars — correct total property count (13 systems x 4 = 52)
 * ================================================================ */

void test_css_vars_52_properties(void) {
    char buf[ST_CSS_BUF_SIZE];
    st_css_vars(THEME_COSMOS, buf, (int)sizeof(buf));

    /* Count newlines — each property ends with \n */
    int count = 0;
    for (int i = 0; buf[i] != '\0'; i++) {
        if (buf[i] == '\n') count++;
    }
    TEST_ASSERT_EQUAL_INT(76, count);  /* 19 systems * 4 properties */
}

/* ================================================================
 * RUNNER
 * ================================================================ */

int main(void) {
    UNITY_BEGIN();

    /* st_system_slug — all 13 valid */
    RUN_TEST(test_slug_astronomy);
    RUN_TEST(test_slug_astrology);
    RUN_TEST(test_slug_tzolkin);
    RUN_TEST(test_slug_iching);
    RUN_TEST(test_slug_chinese);
    RUN_TEST(test_slug_human_design);
    RUN_TEST(test_slug_gregorian);
    RUN_TEST(test_slug_kabbalah);
    RUN_TEST(test_slug_hindu);
    RUN_TEST(test_slug_buddhist);
    RUN_TEST(test_slug_hebrew);
    RUN_TEST(test_slug_islamic);
    RUN_TEST(test_slug_geology);

    /* st_system_slug — invalid */
    RUN_TEST(test_slug_negative_returns_unknown);
    RUN_TEST(test_slug_too_large_returns_unknown);
    RUN_TEST(test_slug_way_too_large_returns_unknown);

    /* st_system_count */
    RUN_TEST(test_system_count_equals_ct_system_count);
    RUN_TEST(test_system_count_is_19);

    /* st_system_colors — valid */
    RUN_TEST(test_astronomy_colors_nonzero);
    RUN_TEST(test_tzolkin_colors_nonzero);
    RUN_TEST(test_geology_colors_nonzero);

    /* st_system_colors — invalid */
    RUN_TEST(test_invalid_system_returns_zeros);
    RUN_TEST(test_invalid_system_too_large_returns_zeros);

    /* st_system_colors — range */
    RUN_TEST(test_rgb_values_in_range);

    /* st_system_colors — theme difference */
    RUN_TEST(test_cosmos_vs_dawn_different_surface);

    /* st_css_vars — basic */
    RUN_TEST(test_css_vars_nonempty);
    RUN_TEST(test_css_vars_contains_all_slugs);
    RUN_TEST(test_css_vars_has_four_properties_per_system);
    RUN_TEST(test_css_vars_no_decimals);

    /* st_css_vars — edge cases */
    RUN_TEST(test_css_vars_small_buffer_truncates);
    RUN_TEST(test_css_vars_null_buffer_returns_zero);
    RUN_TEST(test_css_vars_zero_bufsize_returns_zero);

    /* st_css_vars — theme variation */
    RUN_TEST(test_css_vars_dawn_differs);

    /* st_css_vars — property count */
    RUN_TEST(test_css_vars_52_properties);

    return UNITY_END();
}
