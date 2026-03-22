#include "../../tests/unity/unity.h"
#include "../../src/ui/theme.h"

#include <math.h>
#include <string.h>

#define EPSILON 0.01f
#define PHI_INV_F 0.6180339887f

void setUp(void) {}
void tearDown(void) {}

/* ================================================================
 * THEME CONSTRUCTION
 * ================================================================ */

void test_default_is_cosmos(void) {
    theme_t t = theme_default();
    TEST_ASSERT_EQUAL_INT(THEME_COSMOS, t.id);
}

void test_get_cosmos_by_id(void) {
    theme_t t = theme_get(THEME_COSMOS);
    TEST_ASSERT_EQUAL_INT(THEME_COSMOS, t.id);
    TEST_ASSERT_EQUAL_STRING("Cosmos", t.name);
}

void test_get_dawn_by_id(void) {
    theme_t t = theme_get(THEME_DAWN);
    TEST_ASSERT_EQUAL_INT(THEME_DAWN, t.id);
    TEST_ASSERT_EQUAL_STRING("Dawn", t.name);
}

void test_theme_name_cosmos(void) {
    TEST_ASSERT_EQUAL_STRING("Cosmos", theme_name(THEME_COSMOS));
}

void test_theme_name_dawn(void) {
    TEST_ASSERT_EQUAL_STRING("Dawn", theme_name(THEME_DAWN));
}

void test_theme_name_invalid(void) {
    TEST_ASSERT_EQUAL_STRING("?", theme_name(THEME_COUNT));
    TEST_ASSERT_EQUAL_STRING("?", theme_name((theme_id_t)99));
}

/* ================================================================
 * COSMOS THEME COLORS
 * ================================================================ */

void test_cosmos_bg_space_is_dark(void) {
    theme_t t = theme_get(THEME_COSMOS);
    /* Deep space background should be very dark */
    TEST_ASSERT_TRUE(t.bg_space.r < 0.1f);
    TEST_ASSERT_TRUE(t.bg_space.g < 0.1f);
    TEST_ASSERT_TRUE(t.bg_space.b < 0.1f);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, t.bg_space.a);
}

void test_cosmos_text_primary_is_bright(void) {
    theme_t t = theme_get(THEME_COSMOS);
    /* Primary text should be bright on dark bg */
    TEST_ASSERT_TRUE(t.text_primary.r > 0.5f);
    TEST_ASSERT_TRUE(t.text_primary.g > 0.5f);
    TEST_ASSERT_TRUE(t.text_primary.b > 0.5f);
}

void test_cosmos_brand_primary_is_solar_gold(void) {
    theme_t t = theme_get(THEME_COSMOS);
    /* Solar gold: {1.0, 0.85, 0.55, 1.0} */
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, t.brand_primary.r);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.85f, t.brand_primary.g);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.55f, t.brand_primary.b);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, t.brand_primary.a);
}

void test_cosmos_brand_secondary_is_celestial_teal(void) {
    theme_t t = theme_get(THEME_COSMOS);
    /* Celestial teal: {0.2, 0.75, 0.8, 1.0} */
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.2f, t.brand_secondary.r);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.75f, t.brand_secondary.g);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.8f, t.brand_secondary.b);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, t.brand_secondary.a);
}

void test_cosmos_text_secondary_dimmer_than_primary(void) {
    theme_t t = theme_get(THEME_COSMOS);
    TEST_ASSERT_TRUE(t.text_secondary.a < t.text_primary.a);
}

void test_cosmos_text_muted_dimmer_than_secondary(void) {
    theme_t t = theme_get(THEME_COSMOS);
    TEST_ASSERT_TRUE(t.text_muted.a < t.text_secondary.a);
}

void test_cosmos_all_alpha_valid(void) {
    theme_t t = theme_get(THEME_COSMOS);
    TEST_ASSERT_TRUE(t.bg_space.a >= 0.0f && t.bg_space.a <= 1.0f);
    TEST_ASSERT_TRUE(t.text_primary.a >= 0.0f && t.text_primary.a <= 1.0f);
    TEST_ASSERT_TRUE(t.accent.a >= 0.0f && t.accent.a <= 1.0f);
    TEST_ASSERT_TRUE(t.danger.a >= 0.0f && t.danger.a <= 1.0f);
    TEST_ASSERT_TRUE(t.success.a >= 0.0f && t.success.a <= 1.0f);
    TEST_ASSERT_TRUE(t.warning.a >= 0.0f && t.warning.a <= 1.0f);
    TEST_ASSERT_TRUE(t.border.a >= 0.0f && t.border.a <= 1.0f);
    TEST_ASSERT_TRUE(t.star_glow.a >= 0.0f && t.star_glow.a <= 1.0f);
}

/* ================================================================
 * DAWN THEME COLORS
 * ================================================================ */

void test_dawn_bg_space_is_dark(void) {
    theme_t t = theme_get(THEME_DAWN);
    /* Principle 1: bg_space is ALWAYS dark — cosmos scope constant.
     * Dawn changes UI surfaces, not the sky. */
    TEST_ASSERT_TRUE(t.bg_space.r < 0.1f);
    TEST_ASSERT_TRUE(t.bg_space.g < 0.1f);
    TEST_ASSERT_TRUE(t.bg_space.b < 0.1f);
}

void test_dawn_text_primary_is_dark(void) {
    theme_t t = theme_get(THEME_DAWN);
    /* Primary text should be dark on light bg */
    TEST_ASSERT_TRUE(t.text_primary.r < 0.3f);
    TEST_ASSERT_TRUE(t.text_primary.g < 0.3f);
    TEST_ASSERT_TRUE(t.text_primary.b < 0.3f);
}

void test_dawn_brand_primary_matches_cosmos(void) {
    theme_t cosmos = theme_get(THEME_COSMOS);
    theme_t dawn = theme_get(THEME_DAWN);
    /* Brand colors should be the same across themes */
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, cosmos.brand_primary.r, dawn.brand_primary.r);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, cosmos.brand_primary.g, dawn.brand_primary.g);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, cosmos.brand_primary.b, dawn.brand_primary.b);
}

/* ================================================================
 * LAYOUT FIELDS
 * ================================================================ */

void test_typography_populated(void) {
    theme_t t = theme_get(THEME_COSMOS);
    /* Body text (level 3) should be ~13px */
    TEST_ASSERT_FLOAT_WITHIN(0.5f, 13.0f, t.typography.sizes[3]);
}

void test_spacing_populated(void) {
    theme_t t = theme_get(THEME_COSMOS);
    /* Spacing values should be positive */
    for (int i = 0; i < PHI_SPACE_LEVELS; i++) {
        TEST_ASSERT_TRUE(t.spacing.values[i] > 0.0f);
    }
}

void test_timing_populated(void) {
    theme_t t = theme_get(THEME_COSMOS);
    TEST_ASSERT_TRUE(t.timing.micro > 0.0f);
    TEST_ASSERT_TRUE(t.timing.base > 0.0f);
    TEST_ASSERT_TRUE(t.timing.epic > 0.0f);
}

void test_opacity_populated(void) {
    theme_t t = theme_get(THEME_COSMOS);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, t.opacity.values[0]);
    TEST_ASSERT_TRUE(t.opacity.values[1] < 1.0f);
}

void test_derived_metrics_positive(void) {
    theme_t t = theme_get(THEME_COSMOS);
    TEST_ASSERT_TRUE(t.corner_radius > 0.0f);
    TEST_ASSERT_TRUE(t.border_width > 0.0f);
    TEST_ASSERT_TRUE(t.shadow_blur > 0.0f);
    TEST_ASSERT_TRUE(t.icon_size > 0.0f);
}

/* ================================================================
 * USER PREFERENCES
 * ================================================================ */

void test_default_prefs_neutral(void) {
    theme_prefs_t p = theme_default_prefs();
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, p.font_scale);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, p.opacity_scale);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, p.animation_speed);
    TEST_ASSERT_EQUAL_INT(0, p.use_custom_accent);
}

void test_validate_prefs_clamps_font_scale(void) {
    theme_prefs_t p = theme_default_prefs();
    p.font_scale = 5.0f;
    theme_prefs_t v = theme_validate_prefs(p);
    TEST_ASSERT_TRUE(v.font_scale <= 2.0f);

    p.font_scale = 0.1f;
    v = theme_validate_prefs(p);
    TEST_ASSERT_TRUE(v.font_scale >= 0.5f);
}

void test_validate_prefs_clamps_opacity(void) {
    theme_prefs_t p = theme_default_prefs();
    p.opacity_scale = 3.0f;
    theme_prefs_t v = theme_validate_prefs(p);
    TEST_ASSERT_TRUE(v.opacity_scale <= 1.0f);

    p.opacity_scale = 0.01f;
    v = theme_validate_prefs(p);
    TEST_ASSERT_TRUE(v.opacity_scale >= 0.5f);
}

void test_validate_prefs_clamps_animation_speed(void) {
    theme_prefs_t p = theme_default_prefs();
    p.animation_speed = 10.0f;
    theme_prefs_t v = theme_validate_prefs(p);
    TEST_ASSERT_TRUE(v.animation_speed <= 2.0f);
}

void test_apply_prefs_font_scale(void) {
    theme_t base = theme_default();
    theme_prefs_t p = theme_default_prefs();
    p.font_scale = 1.5f;
    theme_t scaled = theme_apply_prefs(base, p);
    /* Font sizes should be larger */
    TEST_ASSERT_TRUE(scaled.typography.sizes[3] > base.typography.sizes[3]);
}

void test_apply_prefs_opacity_scale(void) {
    theme_t base = theme_default();
    theme_prefs_t p = theme_default_prefs();
    p.opacity_scale = 0.7f;
    theme_t scaled = theme_apply_prefs(base, p);
    /* Opacity values should be scaled down */
    TEST_ASSERT_TRUE(scaled.opacity.values[1] < base.opacity.values[1]);
}

void test_apply_prefs_animation_speed(void) {
    theme_t base = theme_default();
    theme_prefs_t p = theme_default_prefs();
    p.animation_speed = 2.0f;
    theme_t scaled = theme_apply_prefs(base, p);
    /* Timing should be slower (multiplied by 2.0) */
    TEST_ASSERT_FLOAT_WITHIN(0.01f, base.timing.base * 2.0f, scaled.timing.base);
}

void test_apply_prefs_custom_accent(void) {
    theme_t base = theme_default();
    theme_prefs_t p = theme_default_prefs();
    p.use_custom_accent = 1;
    p.accent_hue = (color_hsl_t){120.0f, 0.8f, 0.5f};
    theme_t custom = theme_apply_prefs(base, p);
    /* Accent should differ from default (gold accent) */
    TEST_ASSERT_TRUE(fabsf(custom.accent.r - base.accent.r) > 0.05f ||
                     fabsf(custom.accent.g - base.accent.g) > 0.05f ||
                     fabsf(custom.accent.b - base.accent.b) > 0.05f);
}

void test_apply_prefs_default_preserves_theme(void) {
    theme_t base = theme_default();
    theme_prefs_t p = theme_default_prefs();
    theme_t same = theme_apply_prefs(base, p);
    /* Default prefs should not change the theme significantly */
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, base.bg_space.r, same.bg_space.r);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, base.text_primary.r, same.text_primary.r);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, base.typography.sizes[3], same.typography.sizes[3]);
}

/* ================================================================
 * SYSTEM-AWARE COLORS
 * ================================================================ */

void test_system_accent_no_system(void) {
    theme_t t = theme_get(THEME_COSMOS);
    color_rgba_t accent = theme_system_accent(&t, CT_SYSTEM_COUNT);
    /* Should return theme accent when no system focused */
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, t.accent.r, accent.r);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, t.accent.g, accent.g);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, t.accent.b, accent.b);
}

void test_system_accent_astronomy(void) {
    theme_t t = theme_get(THEME_COSMOS);
    color_rgba_t accent = theme_system_accent(&t, CT_SYSTEM_ASTRONOMY);
    /* Should return astronomy system primary as RGBA */
    color_rgb_t sys_primary = ct_system_primary(CT_SYSTEM_ASTRONOMY);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, sys_primary.r, accent.r);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, sys_primary.g, accent.g);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, sys_primary.b, accent.b);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, accent.a);
}

void test_system_surface_no_system(void) {
    theme_t t = theme_get(THEME_COSMOS);
    color_rgba_t surface = theme_system_surface(&t, CT_SYSTEM_COUNT);
    /* Should return theme surface when no system focused */
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, t.bg_surface.r, surface.r);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, t.bg_surface.g, surface.g);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, t.bg_surface.b, surface.b);
}

void test_system_surface_tinted(void) {
    theme_t t = theme_get(THEME_COSMOS);
    color_rgba_t surface = theme_system_surface(&t, CT_SYSTEM_TZOLKIN);
    /* Surface should be subtly different from base surface */
    float dr = fabsf(surface.r - t.bg_surface.r);
    float dg = fabsf(surface.g - t.bg_surface.g);
    float db = fabsf(surface.b - t.bg_surface.b);
    /* At least one channel should differ (subtle tint) */
    TEST_ASSERT_TRUE(dr > 0.001f || dg > 0.001f || db > 0.001f);
}

/* ================================================================
 * THEME INTERPOLATION
 * ================================================================ */

void test_lerp_at_zero_is_a(void) {
    theme_t cosmos = theme_get(THEME_COSMOS);
    theme_t dawn = theme_get(THEME_DAWN);
    theme_t lerped = theme_lerp(cosmos, dawn, 0.0f);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, cosmos.bg_space.r, lerped.bg_space.r);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, cosmos.bg_space.g, lerped.bg_space.g);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, cosmos.bg_space.b, lerped.bg_space.b);
}

void test_lerp_at_one_is_b(void) {
    theme_t cosmos = theme_get(THEME_COSMOS);
    theme_t dawn = theme_get(THEME_DAWN);
    theme_t lerped = theme_lerp(cosmos, dawn, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, dawn.bg_space.r, lerped.bg_space.r);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, dawn.bg_space.g, lerped.bg_space.g);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, dawn.bg_space.b, lerped.bg_space.b);
}

void test_lerp_at_half_is_midpoint(void) {
    theme_t cosmos = theme_get(THEME_COSMOS);
    theme_t dawn = theme_get(THEME_DAWN);
    theme_t mid = theme_lerp(cosmos, dawn, 0.5f);
    float expected_r = (cosmos.bg_space.r + dawn.bg_space.r) * 0.5f;
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, expected_r, mid.bg_space.r);
}

void test_lerp_interpolates_text(void) {
    theme_t cosmos = theme_get(THEME_COSMOS);
    theme_t dawn = theme_get(THEME_DAWN);
    theme_t mid = theme_lerp(cosmos, dawn, 0.5f);
    /* Text primary should be between cosmos and dawn values */
    float min_r = fminf(cosmos.text_primary.r, dawn.text_primary.r);
    float max_r = fmaxf(cosmos.text_primary.r, dawn.text_primary.r);
    TEST_ASSERT_TRUE(mid.text_primary.r >= min_r - EPSILON);
    TEST_ASSERT_TRUE(mid.text_primary.r <= max_r + EPSILON);
}

void test_lerp_interpolates_derived_metrics(void) {
    theme_t cosmos = theme_get(THEME_COSMOS);
    theme_t dawn = theme_get(THEME_DAWN);
    theme_t mid = theme_lerp(cosmos, dawn, 0.5f);
    float expected_cr = (cosmos.corner_radius + dawn.corner_radius) * 0.5f;
    TEST_ASSERT_FLOAT_WITHIN(0.1f, expected_cr, mid.corner_radius);
}

/* ================================================================
 * CSS EXPORT
 * ================================================================ */

void test_css_export_not_empty(void) {
    theme_t t = theme_get(THEME_COSMOS);
    char buf[4096];
    int len = theme_to_css(&t, buf, sizeof(buf));
    TEST_ASSERT_TRUE(len > 0);
}

void test_css_export_contains_bg_space(void) {
    theme_t t = theme_get(THEME_COSMOS);
    char buf[4096];
    theme_to_css(&t, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "--bg-space:"));
}

void test_css_export_contains_text_primary(void) {
    theme_t t = theme_get(THEME_COSMOS);
    char buf[4096];
    theme_to_css(&t, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "--text-primary:"));
}

void test_css_export_small_buffer_truncates(void) {
    theme_t t = theme_get(THEME_COSMOS);
    char buf[32];
    int len = theme_to_css(&t, buf, sizeof(buf));
    /* Should write something but be truncated */
    TEST_ASSERT_TRUE(len >= 0);
    TEST_ASSERT_TRUE(len < 32);
}

void test_css_export_contains_brand(void) {
    theme_t t = theme_get(THEME_COSMOS);
    char buf[4096];
    theme_to_css(&t, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "--brand-primary:"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "--brand-secondary:"));
}

/* ================================================================
 * COSMOS-SCOPE CONSTANTS
 * ================================================================ */

void test_cosmos_constant_bg_always_dark(void) {
    theme_cosmos_t c = theme_cosmos_constant();
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.024f, c.bg_space.r);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.027f, c.bg_space.g);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.036f, c.bg_space.b);
}

void test_cosmos_constant_star_warm(void) {
    theme_cosmos_t c = theme_cosmos_constant();
    TEST_ASSERT_TRUE(c.star_color.r > 0.9f);
    TEST_ASSERT_TRUE(c.star_color.g > 0.9f);
    TEST_ASSERT_TRUE(c.star_color.b > 0.7f);
}

void test_cosmos_constant_brand_matches_theme(void) {
    theme_cosmos_t c = theme_cosmos_constant();
    theme_t t = theme_get(THEME_COSMOS);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, t.brand_primary.r, c.brand_primary.r);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, t.brand_primary.g, c.brand_primary.g);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, t.brand_secondary.r, c.brand_secondary.r);
}

void test_cosmos_constant_matches_both_themes_bg(void) {
    theme_cosmos_t c = theme_cosmos_constant();
    theme_t cosmos = theme_get(THEME_COSMOS);
    theme_t dawn = theme_get(THEME_DAWN);
    /* bg_space must be identical across cosmos constant AND both themes */
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, c.bg_space.r, cosmos.bg_space.r);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, c.bg_space.r, dawn.bg_space.r);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, c.bg_space.g, cosmos.bg_space.g);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, c.bg_space.g, dawn.bg_space.g);
}

void test_cosmos_constant_purity(void) {
    theme_cosmos_t a = theme_cosmos_constant();
    theme_cosmos_t b = theme_cosmos_constant();
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, a.bg_space.r, b.bg_space.r);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, a.star_color.r, b.star_color.r);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, a.orbit_line.r, b.orbit_line.r);
}

/* ================================================================
 * PURITY
 * ================================================================ */

void test_purity_get_same_result(void) {
    theme_t a = theme_get(THEME_COSMOS);
    theme_t b = theme_get(THEME_COSMOS);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, a.bg_space.r, b.bg_space.r);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, a.text_primary.r, b.text_primary.r);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, a.brand_primary.r, b.brand_primary.r);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, a.corner_radius, b.corner_radius);
}

void test_purity_default_prefs_same_result(void) {
    theme_prefs_t a = theme_default_prefs();
    theme_prefs_t b = theme_default_prefs();
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, a.font_scale, b.font_scale);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, a.opacity_scale, b.opacity_scale);
}

/* ================================================================
 * MAIN
 * ================================================================ */

int main(void) {
    UNITY_BEGIN();

    /* Theme construction */
    RUN_TEST(test_default_is_cosmos);
    RUN_TEST(test_get_cosmos_by_id);
    RUN_TEST(test_get_dawn_by_id);
    RUN_TEST(test_theme_name_cosmos);
    RUN_TEST(test_theme_name_dawn);
    RUN_TEST(test_theme_name_invalid);

    /* Cosmos theme colors */
    RUN_TEST(test_cosmos_bg_space_is_dark);
    RUN_TEST(test_cosmos_text_primary_is_bright);
    RUN_TEST(test_cosmos_brand_primary_is_solar_gold);
    RUN_TEST(test_cosmos_brand_secondary_is_celestial_teal);
    RUN_TEST(test_cosmos_text_secondary_dimmer_than_primary);
    RUN_TEST(test_cosmos_text_muted_dimmer_than_secondary);
    RUN_TEST(test_cosmos_all_alpha_valid);

    /* Dawn theme colors */
    RUN_TEST(test_dawn_bg_space_is_dark);
    RUN_TEST(test_dawn_text_primary_is_dark);
    RUN_TEST(test_dawn_brand_primary_matches_cosmos);

    /* Layout fields */
    RUN_TEST(test_typography_populated);
    RUN_TEST(test_spacing_populated);
    RUN_TEST(test_timing_populated);
    RUN_TEST(test_opacity_populated);
    RUN_TEST(test_derived_metrics_positive);

    /* User preferences */
    RUN_TEST(test_default_prefs_neutral);
    RUN_TEST(test_validate_prefs_clamps_font_scale);
    RUN_TEST(test_validate_prefs_clamps_opacity);
    RUN_TEST(test_validate_prefs_clamps_animation_speed);
    RUN_TEST(test_apply_prefs_font_scale);
    RUN_TEST(test_apply_prefs_opacity_scale);
    RUN_TEST(test_apply_prefs_animation_speed);
    RUN_TEST(test_apply_prefs_custom_accent);
    RUN_TEST(test_apply_prefs_default_preserves_theme);

    /* System-aware colors */
    RUN_TEST(test_system_accent_no_system);
    RUN_TEST(test_system_accent_astronomy);
    RUN_TEST(test_system_surface_no_system);
    RUN_TEST(test_system_surface_tinted);

    /* Theme interpolation */
    RUN_TEST(test_lerp_at_zero_is_a);
    RUN_TEST(test_lerp_at_one_is_b);
    RUN_TEST(test_lerp_at_half_is_midpoint);
    RUN_TEST(test_lerp_interpolates_text);
    RUN_TEST(test_lerp_interpolates_derived_metrics);

    /* CSS export */
    RUN_TEST(test_css_export_not_empty);
    RUN_TEST(test_css_export_contains_bg_space);
    RUN_TEST(test_css_export_contains_text_primary);
    RUN_TEST(test_css_export_small_buffer_truncates);
    RUN_TEST(test_css_export_contains_brand);

    /* Cosmos-scope constants */
    RUN_TEST(test_cosmos_constant_bg_always_dark);
    RUN_TEST(test_cosmos_constant_star_warm);
    RUN_TEST(test_cosmos_constant_brand_matches_theme);
    RUN_TEST(test_cosmos_constant_matches_both_themes_bg);
    RUN_TEST(test_cosmos_constant_purity);

    /* Purity */
    RUN_TEST(test_purity_get_same_result);
    RUN_TEST(test_purity_default_prefs_same_result);

    return UNITY_END();
}
