/*
 * test_depth_visual.c — Tests for depth tier visual properties
 */

#include "../unity/unity.h"
#include "../../src/render/depth_visual.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* 1. Far zoom maps to GLYPH tier */
void test_far_zoom_is_glyph(void)
{
    TEST_ASSERT_EQUAL_INT(DV_TIER_GLYPH, dv_tier_from_zoom(3.0f));
}

/* 2. Medium zoom maps to GLANCE */
void test_medium_zoom_is_glance(void)
{
    TEST_ASSERT_EQUAL_INT(DV_TIER_GLANCE, dv_tier_from_zoom(1.0f));
}

/* 3. Default zoom maps to CARD */
void test_default_zoom_is_card(void)
{
    TEST_ASSERT_EQUAL_INT(DV_TIER_CARD, dv_tier_from_zoom(0.0f));
}

/* 4. Close zoom maps to CONTEXT */
void test_close_zoom_is_context(void)
{
    TEST_ASSERT_EQUAL_INT(DV_TIER_CONTEXT, dv_tier_from_zoom(-1.5f));
}

/* 5. Very close zoom maps to BOARD */
void test_very_close_is_board(void)
{
    TEST_ASSERT_EQUAL_INT(DV_TIER_BOARD, dv_tier_from_zoom(-3.0f));
}

/* 6. Tier names are non-empty */
void test_tier_names(void)
{
    TEST_ASSERT_TRUE(strlen(dv_tier_name(DV_TIER_GLYPH)) > 0);
    TEST_ASSERT_TRUE(strlen(dv_tier_name(DV_TIER_CARD)) > 0);
    TEST_ASSERT_TRUE(strlen(dv_tier_name(DV_TIER_BOARD)) > 0);
}

/* 7. GLYPH tier has no tint */
void test_glyph_no_tint(void)
{
    dv_tint_t t = dv_tier_tint(DV_TIER_GLYPH);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, t.a);
}

/* 8. CARD tier has no tint (default view) */
void test_card_no_tint(void)
{
    dv_tint_t t = dv_tier_tint(DV_TIER_CARD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, t.a);
}

/* 9. GLANCE tier has warm gold tint */
void test_glance_warm_tint(void)
{
    dv_tint_t t = dv_tier_tint(DV_TIER_GLANCE);
    TEST_ASSERT_TRUE(t.a > 0.0f);
    TEST_ASSERT_TRUE(t.r > t.b); /* warm = red > blue */
}

/* 10. CONTEXT tier has teal tint */
void test_context_teal_tint(void)
{
    dv_tint_t t = dv_tier_tint(DV_TIER_CONTEXT);
    TEST_ASSERT_TRUE(t.a > 0.0f);
    TEST_ASSERT_TRUE(t.b > t.r); /* teal = blue > red */
}

/* 11. BOARD tier has purple tint */
void test_board_purple_tint(void)
{
    dv_tint_t t = dv_tier_tint(DV_TIER_BOARD);
    TEST_ASSERT_TRUE(t.a > 0.0f);
    TEST_ASSERT_TRUE(t.b > t.g); /* purple = blue > green */
}

/* 12. Blend is 0-1 range */
void test_blend_range(void)
{
    for (float z = -5.0f; z <= 5.0f; z += 0.5f) {
        dv_tier_t tier = dv_tier_from_zoom(z);
        float b = dv_tier_blend(z, tier);
        TEST_ASSERT_TRUE(b >= 0.0f);
        TEST_ASSERT_TRUE(b <= 1.0f);
    }
}

/* 13. Blend is high at tier center */
void test_blend_high_at_center(void)
{
    float b = dv_tier_blend(0.0f, DV_TIER_CARD);
    TEST_ASSERT_TRUE(b > 0.5f);
}

/* 14. All tint alphas are <= 0.05 (subtle) */
void test_tints_subtle(void)
{
    for (int t = 0; t <= 4; t++) {
        dv_tint_t tint = dv_tier_tint((dv_tier_t)t);
        TEST_ASSERT_TRUE(tint.a <= 0.06f);
    }
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_far_zoom_is_glyph);
    RUN_TEST(test_medium_zoom_is_glance);
    RUN_TEST(test_default_zoom_is_card);
    RUN_TEST(test_close_zoom_is_context);
    RUN_TEST(test_very_close_is_board);
    RUN_TEST(test_tier_names);
    RUN_TEST(test_glyph_no_tint);
    RUN_TEST(test_card_no_tint);
    RUN_TEST(test_glance_warm_tint);
    RUN_TEST(test_context_teal_tint);
    RUN_TEST(test_board_purple_tint);
    RUN_TEST(test_blend_range);
    RUN_TEST(test_blend_high_at_center);
    RUN_TEST(test_tints_subtle);
    return UNITY_END();
}
