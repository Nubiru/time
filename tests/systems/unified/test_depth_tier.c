/* test_depth_tier.c — Tests for depth tier mapping module
 * Maps camera scale level + system identity -> display detail tier (T0-T4).
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/depth_tier.h"

#include <string.h>

void setUp(void) { }
void tearDown(void) { }

/* ========== depth_tier_count ========== */

static void test_count_returns_5(void)
{
    TEST_ASSERT_EQUAL_INT(5, depth_tier_count());
}

/* ========== depth_tier_default_for_scale — all 7 scale levels ========== */

static void test_default_scale_0_personal_is_panel(void)
{
    TEST_ASSERT_EQUAL_INT(DEPTH_TIER_PANEL, depth_tier_default_for_scale(0));
}

static void test_default_scale_1_earth_is_card(void)
{
    TEST_ASSERT_EQUAL_INT(DEPTH_TIER_CARD, depth_tier_default_for_scale(1));
}

static void test_default_scale_2_inner_is_card(void)
{
    TEST_ASSERT_EQUAL_INT(DEPTH_TIER_CARD, depth_tier_default_for_scale(2));
}

static void test_default_scale_3_solar_is_glance(void)
{
    TEST_ASSERT_EQUAL_INT(DEPTH_TIER_GLANCE, depth_tier_default_for_scale(3));
}

static void test_default_scale_4_local_is_glyph(void)
{
    TEST_ASSERT_EQUAL_INT(DEPTH_TIER_GLYPH, depth_tier_default_for_scale(4));
}

static void test_default_scale_5_galaxy_is_glyph(void)
{
    TEST_ASSERT_EQUAL_INT(DEPTH_TIER_GLYPH, depth_tier_default_for_scale(5));
}

static void test_default_scale_6_universe_is_glyph(void)
{
    TEST_ASSERT_EQUAL_INT(DEPTH_TIER_GLYPH, depth_tier_default_for_scale(6));
}

/* ========== depth_tier_for_scale — no system override, no user override ========== */

static void test_for_scale_default_no_overrides(void)
{
    /* system_id -1 (out of range) + user_override -1 (no override) = default */
    TEST_ASSERT_EQUAL_INT(DEPTH_TIER_PANEL, depth_tier_for_scale(0, -1, -1));
    TEST_ASSERT_EQUAL_INT(DEPTH_TIER_CARD, depth_tier_for_scale(1, -1, -1));
    TEST_ASSERT_EQUAL_INT(DEPTH_TIER_GLYPH, depth_tier_for_scale(4, -1, -1));
}

/* ========== System-specific promotions ========== */

static void test_geological_promoted_at_galaxy_scale(void)
{
    /* TS_SYS_GEOLOGICAL at scale 5 (galaxy): default GLYPH(0) + 1 = GLANCE(1) */
    TEST_ASSERT_EQUAL_INT(DEPTH_TIER_GLANCE,
                          depth_tier_for_scale(5, TS_SYS_GEOLOGICAL, -1));
}

static void test_geological_promoted_at_universe_scale(void)
{
    /* TS_SYS_GEOLOGICAL at scale 6 (universe): default GLYPH(0) + 1 = GLANCE(1) */
    TEST_ASSERT_EQUAL_INT(DEPTH_TIER_GLANCE,
                          depth_tier_for_scale(6, TS_SYS_GEOLOGICAL, -1));
}

static void test_geological_not_promoted_at_solar_scale(void)
{
    /* TS_SYS_GEOLOGICAL at scale 3 (solar): no promotion, stays GLANCE(1) */
    TEST_ASSERT_EQUAL_INT(DEPTH_TIER_GLANCE,
                          depth_tier_for_scale(3, TS_SYS_GEOLOGICAL, -1));
}

static void test_cosmic_promoted_at_galaxy_scale(void)
{
    /* TS_SYS_COSMIC at scale 5: default GLYPH(0) + 1 = GLANCE(1) */
    TEST_ASSERT_EQUAL_INT(DEPTH_TIER_GLANCE,
                          depth_tier_for_scale(5, TS_SYS_COSMIC, -1));
}

static void test_cosmic_promoted_at_universe_scale(void)
{
    /* TS_SYS_COSMIC at scale 6: default GLYPH(0) + 1 = GLANCE(1) */
    TEST_ASSERT_EQUAL_INT(DEPTH_TIER_GLANCE,
                          depth_tier_for_scale(6, TS_SYS_COSMIC, -1));
}

static void test_buddhist_promoted_at_galaxy_scale(void)
{
    /* TS_SYS_BUDDHIST at scale 5 (galaxy): default GLYPH(0) + 1 = GLANCE(1) */
    TEST_ASSERT_EQUAL_INT(DEPTH_TIER_GLANCE,
                          depth_tier_for_scale(5, TS_SYS_BUDDHIST, -1));
}

static void test_buddhist_not_promoted_at_solar_scale(void)
{
    /* TS_SYS_BUDDHIST at scale 3 (solar): no promotion, stays GLANCE(1) */
    TEST_ASSERT_EQUAL_INT(DEPTH_TIER_GLANCE,
                          depth_tier_for_scale(3, TS_SYS_BUDDHIST, -1));
}

static void test_islamic_promoted_at_personal_scale(void)
{
    /* TS_SYS_ISLAMIC at scale 0 (personal): default PANEL(3) + 1 = BOARD(4) */
    TEST_ASSERT_EQUAL_INT(DEPTH_TIER_BOARD,
                          depth_tier_for_scale(0, TS_SYS_ISLAMIC, -1));
}

static void test_islamic_promoted_at_earth_scale(void)
{
    /* TS_SYS_ISLAMIC at scale 1 (earth): default CARD(2) + 1 = PANEL(3) */
    TEST_ASSERT_EQUAL_INT(DEPTH_TIER_PANEL,
                          depth_tier_for_scale(1, TS_SYS_ISLAMIC, -1));
}

static void test_islamic_not_promoted_at_solar_scale(void)
{
    /* TS_SYS_ISLAMIC at scale 3 (solar): no promotion, stays GLANCE(1) */
    TEST_ASSERT_EQUAL_INT(DEPTH_TIER_GLANCE,
                          depth_tier_for_scale(3, TS_SYS_ISLAMIC, -1));
}

/* ========== User override ========== */

static void test_user_override_forces_glyph(void)
{
    /* Override 0 forces GLYPH regardless of scale/system */
    TEST_ASSERT_EQUAL_INT(DEPTH_TIER_GLYPH,
                          depth_tier_for_scale(0, TS_SYS_GREGORIAN, 0));
}

static void test_user_override_forces_board(void)
{
    /* Override 4 forces BOARD regardless of scale */
    TEST_ASSERT_EQUAL_INT(DEPTH_TIER_BOARD,
                          depth_tier_for_scale(6, TS_SYS_GREGORIAN, 4));
}

static void test_user_override_forces_card(void)
{
    /* Override 2 forces CARD */
    TEST_ASSERT_EQUAL_INT(DEPTH_TIER_CARD,
                          depth_tier_for_scale(3, TS_SYS_ISLAMIC, 2));
}

static void test_user_override_negative_one_means_computed(void)
{
    /* -1 means no override — use computed tier */
    TEST_ASSERT_EQUAL_INT(DEPTH_TIER_PANEL,
                          depth_tier_for_scale(0, TS_SYS_GREGORIAN, -1));
}

/* ========== Out-of-range scale values (clamping) ========== */

static void test_scale_negative_clamps_to_zero(void)
{
    /* Negative scale clamps to 0 -> PANEL */
    TEST_ASSERT_EQUAL_INT(DEPTH_TIER_PANEL,
                          depth_tier_for_scale(-5, -1, -1));
}

static void test_scale_too_large_clamps_to_six(void)
{
    /* Scale 99 clamps to 6 -> GLYPH */
    TEST_ASSERT_EQUAL_INT(DEPTH_TIER_GLYPH,
                          depth_tier_for_scale(99, -1, -1));
}

/* ========== Out-of-range system_id ========== */

static void test_invalid_system_id_uses_default(void)
{
    /* system_id 999 is out of range -> default mapping */
    TEST_ASSERT_EQUAL_INT(DEPTH_TIER_CARD,
                          depth_tier_for_scale(2, 999, -1));
}

static void test_negative_system_id_uses_default(void)
{
    /* system_id -10 is out of range -> default mapping */
    TEST_ASSERT_EQUAL_INT(DEPTH_TIER_GLANCE,
                          depth_tier_for_scale(3, -10, -1));
}

/* ========== Clamping: promotion should not exceed BOARD ========== */

static void test_promotion_clamps_at_board(void)
{
    /* Islamic at PERSONAL scale: PANEL(3) + 1 = BOARD(4), which is max.
     * Verify it doesn't go to 5. */
    depth_tier_t tier = depth_tier_for_scale(0, TS_SYS_ISLAMIC, -1);
    TEST_ASSERT_EQUAL_INT(DEPTH_TIER_BOARD, tier);
    TEST_ASSERT_TRUE(tier <= DEPTH_TIER_BOARD);
}

/* ========== User override out-of-range clamping ========== */

static void test_user_override_too_large_clamps_to_board(void)
{
    /* Override 99 should clamp to BOARD(4) */
    TEST_ASSERT_EQUAL_INT(DEPTH_TIER_BOARD,
                          depth_tier_for_scale(3, -1, 99));
}

static void test_user_override_negative_two_means_computed(void)
{
    /* Override -2 (any negative) should be treated same as -1: no override */
    TEST_ASSERT_EQUAL_INT(DEPTH_TIER_PANEL,
                          depth_tier_for_scale(0, -1, -2));
}

/* ========== depth_tier_name ========== */

static void test_name_glyph(void)
{
    TEST_ASSERT_EQUAL_STRING("Glyph", depth_tier_name(DEPTH_TIER_GLYPH));
}

static void test_name_glance(void)
{
    TEST_ASSERT_EQUAL_STRING("Glance", depth_tier_name(DEPTH_TIER_GLANCE));
}

static void test_name_card(void)
{
    TEST_ASSERT_EQUAL_STRING("Card", depth_tier_name(DEPTH_TIER_CARD));
}

static void test_name_panel(void)
{
    TEST_ASSERT_EQUAL_STRING("Panel", depth_tier_name(DEPTH_TIER_PANEL));
}

static void test_name_board(void)
{
    TEST_ASSERT_EQUAL_STRING("Board", depth_tier_name(DEPTH_TIER_BOARD));
}

static void test_name_invalid_returns_question_mark(void)
{
    TEST_ASSERT_EQUAL_STRING("?", depth_tier_name((depth_tier_t)99));
}

static void test_name_negative_returns_question_mark(void)
{
    TEST_ASSERT_EQUAL_STRING("?", depth_tier_name((depth_tier_t)-1));
}

/* ========== Non-promoted system at various scales ========== */

static void test_gregorian_at_personal_is_panel(void)
{
    TEST_ASSERT_EQUAL_INT(DEPTH_TIER_PANEL,
                          depth_tier_for_scale(0, TS_SYS_GREGORIAN, -1));
}

static void test_gregorian_at_galaxy_is_glyph(void)
{
    TEST_ASSERT_EQUAL_INT(DEPTH_TIER_GLYPH,
                          depth_tier_for_scale(5, TS_SYS_GREGORIAN, -1));
}

/* ========== Runner ========== */

int main(void)
{
    UNITY_BEGIN();

    /* count */
    RUN_TEST(test_count_returns_5);

    /* default for scale — all 7 levels */
    RUN_TEST(test_default_scale_0_personal_is_panel);
    RUN_TEST(test_default_scale_1_earth_is_card);
    RUN_TEST(test_default_scale_2_inner_is_card);
    RUN_TEST(test_default_scale_3_solar_is_glance);
    RUN_TEST(test_default_scale_4_local_is_glyph);
    RUN_TEST(test_default_scale_5_galaxy_is_glyph);
    RUN_TEST(test_default_scale_6_universe_is_glyph);

    /* no overrides */
    RUN_TEST(test_for_scale_default_no_overrides);

    /* system-specific promotions */
    RUN_TEST(test_geological_promoted_at_galaxy_scale);
    RUN_TEST(test_geological_promoted_at_universe_scale);
    RUN_TEST(test_geological_not_promoted_at_solar_scale);
    RUN_TEST(test_cosmic_promoted_at_galaxy_scale);
    RUN_TEST(test_cosmic_promoted_at_universe_scale);
    RUN_TEST(test_buddhist_promoted_at_galaxy_scale);
    RUN_TEST(test_buddhist_not_promoted_at_solar_scale);
    RUN_TEST(test_islamic_promoted_at_personal_scale);
    RUN_TEST(test_islamic_promoted_at_earth_scale);
    RUN_TEST(test_islamic_not_promoted_at_solar_scale);

    /* user overrides */
    RUN_TEST(test_user_override_forces_glyph);
    RUN_TEST(test_user_override_forces_board);
    RUN_TEST(test_user_override_forces_card);
    RUN_TEST(test_user_override_negative_one_means_computed);

    /* out-of-range scale */
    RUN_TEST(test_scale_negative_clamps_to_zero);
    RUN_TEST(test_scale_too_large_clamps_to_six);

    /* out-of-range system_id */
    RUN_TEST(test_invalid_system_id_uses_default);
    RUN_TEST(test_negative_system_id_uses_default);

    /* clamping */
    RUN_TEST(test_promotion_clamps_at_board);
    RUN_TEST(test_user_override_too_large_clamps_to_board);
    RUN_TEST(test_user_override_negative_two_means_computed);

    /* tier names */
    RUN_TEST(test_name_glyph);
    RUN_TEST(test_name_glance);
    RUN_TEST(test_name_card);
    RUN_TEST(test_name_panel);
    RUN_TEST(test_name_board);
    RUN_TEST(test_name_invalid_returns_question_mark);
    RUN_TEST(test_name_negative_returns_question_mark);

    /* non-promoted system */
    RUN_TEST(test_gregorian_at_personal_is_panel);
    RUN_TEST(test_gregorian_at_galaxy_is_glyph);

    return UNITY_END();
}
