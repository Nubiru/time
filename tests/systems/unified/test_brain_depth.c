/* test_brain_depth.c — Tests for layered depth content.
 *
 * Validates that each system produces content at every tier level,
 * with content length increasing with depth. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/brain_depth.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* 2024-03-20 spring equinox */
static const double TEST_JD = 2460389.5;

/* ===================================================================
 * System availability
 * =================================================================== */

void test_system_count_positive(void) {
    TEST_ASSERT_GREATER_THAN(0, br_depth_system_count());
}

void test_has_tzolkin(void) {
    TEST_ASSERT_EQUAL_INT(1, br_depth_has_system(TS_SYS_TZOLKIN));
}

void test_has_hebrew(void) {
    TEST_ASSERT_EQUAL_INT(1, br_depth_has_system(TS_SYS_HEBREW));
}

void test_has_chinese(void) {
    TEST_ASSERT_EQUAL_INT(1, br_depth_has_system(TS_SYS_CHINESE));
}

void test_has_gregorian(void) {
    TEST_ASSERT_EQUAL_INT(1, br_depth_has_system(TS_SYS_GREGORIAN));
}

/* ===================================================================
 * Max length per tier
 * =================================================================== */

void test_max_len_glyph(void) {
    TEST_ASSERT_EQUAL_INT(BR_DEPTH_GLYPH_MAX, br_depth_max_len(DEPTH_TIER_GLYPH));
}

void test_max_len_board(void) {
    TEST_ASSERT_EQUAL_INT(BR_DEPTH_BOARD_MAX, br_depth_max_len(DEPTH_TIER_BOARD));
}

void test_max_len_invalid(void) {
    TEST_ASSERT_EQUAL_INT(0, br_depth_max_len(99));
}

/* ===================================================================
 * br_depth_get — null safety
 * =================================================================== */

void test_get_null_out_returns_zero(void) {
    TEST_ASSERT_EQUAL_INT(0, br_depth_get(TEST_JD, TS_SYS_TZOLKIN,
                                           DEPTH_TIER_GLYPH, NULL));
}

void test_get_invalid_tier_returns_zero(void) {
    br_depth_content_t out;
    TEST_ASSERT_EQUAL_INT(0, br_depth_get(TEST_JD, TS_SYS_TZOLKIN, 99, &out));
}

/* ===================================================================
 * Tzolkin at each depth level
 * =================================================================== */

void test_tzolkin_glyph(void) {
    br_depth_content_t out;
    int ok = br_depth_get(TEST_JD, TS_SYS_TZOLKIN, DEPTH_TIER_GLYPH, &out);
    TEST_ASSERT_EQUAL_INT(1, ok);
    TEST_ASSERT_TRUE(out.content_len > 0);
    TEST_ASSERT_TRUE(out.content_len <= BR_DEPTH_GLYPH_MAX);
}

void test_tzolkin_glance(void) {
    br_depth_content_t out;
    int ok = br_depth_get(TEST_JD, TS_SYS_TZOLKIN, DEPTH_TIER_GLANCE, &out);
    TEST_ASSERT_EQUAL_INT(1, ok);
    TEST_ASSERT_TRUE(out.content_len > 0);
}

void test_tzolkin_card(void) {
    br_depth_content_t out;
    int ok = br_depth_get(TEST_JD, TS_SYS_TZOLKIN, DEPTH_TIER_CARD, &out);
    TEST_ASSERT_EQUAL_INT(1, ok);
    TEST_ASSERT_TRUE(out.content_len > 0);
}

void test_tzolkin_panel(void) {
    br_depth_content_t out;
    int ok = br_depth_get(TEST_JD, TS_SYS_TZOLKIN, DEPTH_TIER_PANEL, &out);
    TEST_ASSERT_EQUAL_INT(1, ok);
    TEST_ASSERT_NOT_NULL(strstr(out.content, "260"));
}

void test_tzolkin_board(void) {
    br_depth_content_t out;
    int ok = br_depth_get(TEST_JD, TS_SYS_TZOLKIN, DEPTH_TIER_BOARD, &out);
    TEST_ASSERT_EQUAL_INT(1, ok);
    TEST_ASSERT_NOT_NULL(strstr(out.content, "Calendar Round"));
}

/* ===================================================================
 * Content grows with depth
 * =================================================================== */

void test_tzolkin_depth_increases(void) {
    br_depth_content_t g, l, c, p, b;
    br_depth_get(TEST_JD, TS_SYS_TZOLKIN, DEPTH_TIER_GLYPH, &g);
    br_depth_get(TEST_JD, TS_SYS_TZOLKIN, DEPTH_TIER_GLANCE, &l);
    br_depth_get(TEST_JD, TS_SYS_TZOLKIN, DEPTH_TIER_CARD, &c);
    br_depth_get(TEST_JD, TS_SYS_TZOLKIN, DEPTH_TIER_PANEL, &p);
    br_depth_get(TEST_JD, TS_SYS_TZOLKIN, DEPTH_TIER_BOARD, &b);

    TEST_ASSERT_TRUE(l.content_len >= g.content_len);
    TEST_ASSERT_TRUE(c.content_len >= l.content_len);
    TEST_ASSERT_TRUE(p.content_len >= c.content_len);
    TEST_ASSERT_TRUE(b.content_len >= p.content_len);
}

/* ===================================================================
 * Hebrew depth
 * =================================================================== */

void test_hebrew_panel_mentions_metonic(void) {
    br_depth_content_t out;
    br_depth_get(TEST_JD, TS_SYS_HEBREW, DEPTH_TIER_PANEL, &out);
    TEST_ASSERT_NOT_NULL(strstr(out.content, "Metonic"));
}

void test_hebrew_board_mentions_jubilee(void) {
    br_depth_content_t out;
    br_depth_get(TEST_JD, TS_SYS_HEBREW, DEPTH_TIER_BOARD, &out);
    TEST_ASSERT_NOT_NULL(strstr(out.content, "Jubilee"));
}

/* ===================================================================
 * Output struct fields
 * =================================================================== */

void test_output_tier_set(void) {
    br_depth_content_t out;
    br_depth_get(TEST_JD, TS_SYS_CHINESE, DEPTH_TIER_CARD, &out);
    TEST_ASSERT_EQUAL_INT(DEPTH_TIER_CARD, out.tier);
}

void test_output_system_set(void) {
    br_depth_content_t out;
    br_depth_get(TEST_JD, TS_SYS_CHINESE, DEPTH_TIER_CARD, &out);
    TEST_ASSERT_EQUAL_INT(TS_SYS_CHINESE, out.system);
}

/* ===================================================================
 * I Ching depth
 * =================================================================== */

void test_iching_board_mentions_64(void) {
    br_depth_content_t out;
    br_depth_get(TEST_JD, TS_SYS_ICHING, DEPTH_TIER_BOARD, &out);
    TEST_ASSERT_NOT_NULL(strstr(out.content, "64"));
}

/* ===================================================================
 * Glyph refinement — system-specific symbol extraction
 * =================================================================== */

void test_tzolkin_glyph_is_kin_number(void) {
    br_depth_content_t out;
    br_depth_get(TEST_JD, TS_SYS_TZOLKIN, DEPTH_TIER_GLYPH, &out);
    /* Should be a kin number (1-260), not "Kin" */
    TEST_ASSERT_TRUE(out.content[0] >= '1' && out.content[0] <= '9');
}

void test_iching_glyph_is_number(void) {
    br_depth_content_t out;
    br_depth_get(TEST_JD, TS_SYS_ICHING, DEPTH_TIER_GLYPH, &out);
    /* Should be a hexagram number (1-64), not "64." */
    TEST_ASSERT_TRUE(out.content[0] >= '1' && out.content[0] <= '9');
    /* Should NOT contain a period */
    TEST_ASSERT_NULL(strstr(out.content, "."));
}

void test_astrology_glyph_is_sign_name(void) {
    br_depth_content_t out;
    br_depth_get(TEST_JD, TS_SYS_ASTROLOGY, DEPTH_TIER_GLYPH, &out);
    /* Should be a zodiac sign name, not "Sun" */
    TEST_ASSERT_TRUE(strlen(out.content) >= 3);
    /* "Sun" would mean the generic extraction failed */
    TEST_ASSERT_NULL(strstr(out.content, "Sun"));
}

void test_hebrew_glyph_is_month(void) {
    br_depth_content_t out;
    br_depth_get(TEST_JD, TS_SYS_HEBREW, DEPTH_TIER_GLYPH, &out);
    /* Should be a month name, not a day number */
    TEST_ASSERT_TRUE(out.content[0] >= 'A' && out.content[0] <= 'Z');
}

void test_islamic_glyph_is_month(void) {
    br_depth_content_t out;
    br_depth_get(TEST_JD, TS_SYS_ISLAMIC, DEPTH_TIER_GLYPH, &out);
    /* Should be a month name, not a day number */
    TEST_ASSERT_TRUE(strlen(out.content) >= 3);
}

/* ===================================================================
 * Main
 * =================================================================== */

int main(void) {
    UNITY_BEGIN();

    /* System availability */
    RUN_TEST(test_system_count_positive);
    RUN_TEST(test_has_tzolkin);
    RUN_TEST(test_has_hebrew);
    RUN_TEST(test_has_chinese);
    RUN_TEST(test_has_gregorian);

    /* Max length */
    RUN_TEST(test_max_len_glyph);
    RUN_TEST(test_max_len_board);
    RUN_TEST(test_max_len_invalid);

    /* Null safety */
    RUN_TEST(test_get_null_out_returns_zero);
    RUN_TEST(test_get_invalid_tier_returns_zero);

    /* Tzolkin at each tier */
    RUN_TEST(test_tzolkin_glyph);
    RUN_TEST(test_tzolkin_glance);
    RUN_TEST(test_tzolkin_card);
    RUN_TEST(test_tzolkin_panel);
    RUN_TEST(test_tzolkin_board);

    /* Depth increases */
    RUN_TEST(test_tzolkin_depth_increases);

    /* Hebrew depth */
    RUN_TEST(test_hebrew_panel_mentions_metonic);
    RUN_TEST(test_hebrew_board_mentions_jubilee);

    /* Output fields */
    RUN_TEST(test_output_tier_set);
    RUN_TEST(test_output_system_set);

    /* I Ching */
    RUN_TEST(test_iching_board_mentions_64);

    /* Glyph refinement */
    RUN_TEST(test_tzolkin_glyph_is_kin_number);
    RUN_TEST(test_iching_glyph_is_number);
    RUN_TEST(test_astrology_glyph_is_sign_name);
    RUN_TEST(test_hebrew_glyph_is_month);
    RUN_TEST(test_islamic_glyph_is_month);

    return UNITY_END();
}
