#include "../../tests/unity/unity.h"
#include "../../src/render/color_theory.h"
#include "../../src/math/color.h"

#include <math.h>

#define EPSILON 0.01f
#define GOLDEN_ANGLE_F 137.508f

void setUp(void) {}
void tearDown(void) {}

/* --- sRGB / Linear Conversion --- */

void test_srgb_black_stays_black(void) {
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, ct_srgb_to_linear(0.0f));
}

void test_srgb_white_stays_white(void) {
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, ct_srgb_to_linear(1.0f));
}

void test_srgb_roundtrip(void) {
    float original = 0.5f;
    float linear = ct_srgb_to_linear(original);
    float back = ct_linear_to_srgb(linear);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, original, back);
}

void test_srgb_midpoint_is_darker_linear(void) {
    /* sRGB 0.5 maps to ~0.214 in linear space */
    float linear = ct_srgb_to_linear(0.5f);
    TEST_ASSERT_TRUE(linear < 0.5f);
    TEST_ASSERT_TRUE(linear > 0.1f);
}

void test_to_linear_full_color(void) {
    color_rgb_t srgb = {0.5f, 0.5f, 0.5f};
    color_rgb_t lin = ct_to_linear(srgb);
    TEST_ASSERT_TRUE(lin.r < 0.5f);
    TEST_ASSERT_TRUE(lin.g < 0.5f);
    TEST_ASSERT_TRUE(lin.b < 0.5f);
}

void test_to_srgb_full_color(void) {
    color_rgb_t srgb = {0.5f, 0.5f, 0.5f};
    color_rgb_t lin = ct_to_linear(srgb);
    color_rgb_t back = ct_to_srgb(lin);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, srgb.r, back.r);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, srgb.g, back.g);
}

/* --- Golden Ratio Color Harmony --- */

void test_golden_palette_first_is_base(void) {
    color_hsl_t base = {180.0f, 0.7f, 0.5f};
    color_hsl_t out[3];
    ct_golden_palette(base, 3, out);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 180.0f, out[0].h);
}

void test_golden_palette_second_is_base_plus_golden_angle(void) {
    color_hsl_t base = {0.0f, 0.7f, 0.5f};
    color_hsl_t out[3];
    ct_golden_palette(base, 3, out);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, GOLDEN_ANGLE_F, out[1].h);
}

void test_golden_palette_preserves_sat_light(void) {
    color_hsl_t base = {90.0f, 0.65f, 0.45f};
    color_hsl_t out[5];
    ct_golden_palette(base, 5, out);
    for (int i = 0; i < 5; i++) {
        TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.65f, out[i].s);
        TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.45f, out[i].l);
    }
}

void test_golden_complement_offset(void) {
    color_hsl_t base = {50.0f, 0.7f, 0.5f};
    color_hsl_t gc = ct_golden_complement(base);
    float expected = fmodf(50.0f + GOLDEN_ANGLE_F, 360.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, expected, gc.h);
}

void test_golden_triad_three_colors(void) {
    color_hsl_t base = {0.0f, 0.8f, 0.5f};
    color_hsl_t out[3];
    ct_golden_triad(base, out);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, out[0].h);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, GOLDEN_ANGLE_F, out[1].h);
    float expected2 = fmodf(2.0f * GOLDEN_ANGLE_F, 360.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, expected2, out[2].h);
}

/* --- Color Psychology --- */

void test_mood_energy_is_red(void) {
    color_hsl_t c = ct_mood_color(CT_MOOD_ENERGY);
    TEST_ASSERT_TRUE(c.h < 30.0f || c.h > 340.0f); /* red zone */
}

void test_mood_trust_is_blue(void) {
    color_hsl_t c = ct_mood_color(CT_MOOD_TRUST);
    TEST_ASSERT_TRUE(c.h > 190.0f && c.h < 240.0f);
}

void test_mood_harmony_is_green(void) {
    color_hsl_t c = ct_mood_color(CT_MOOD_HARMONY);
    TEST_ASSERT_TRUE(c.h > 100.0f && c.h < 160.0f);
}

void test_mood_neutral_low_saturation(void) {
    color_hsl_t c = ct_mood_color(CT_MOOD_NEUTRAL);
    TEST_ASSERT_TRUE(c.s < 0.1f);
}

void test_mood_name_valid(void) {
    for (int i = 0; i < CT_MOOD_COUNT; i++) {
        const char *name = ct_mood_name((ct_mood_t)i);
        TEST_ASSERT_NOT_NULL(name);
        TEST_ASSERT_TRUE(name[0] != '?');
    }
}

void test_color_mood_roundtrip(void) {
    ct_mood_t mood = CT_MOOD_TRUST;
    color_hsl_t c = ct_mood_color(mood);
    ct_mood_t detected = ct_color_mood(c);
    TEST_ASSERT_EQUAL_INT(mood, detected);
}

/* --- Semantic Roles --- */

void test_role_background_is_dark(void) {
    color_rgba_t bg = ct_role_color(CT_ROLE_BACKGROUND);
    /* In linear space, should be very dark */
    TEST_ASSERT_TRUE(bg.r < 0.01f);
    TEST_ASSERT_TRUE(bg.g < 0.01f);
    TEST_ASSERT_TRUE(bg.b < 0.02f);
}

void test_role_text_primary_is_bright(void) {
    color_rgba_t txt = ct_role_color(CT_ROLE_TEXT_PRIMARY);
    TEST_ASSERT_TRUE(txt.r > 0.5f);
    TEST_ASSERT_TRUE(txt.g > 0.5f);
}

void test_role_danger_is_red(void) {
    color_rgba_t d = ct_role_color(CT_ROLE_DANGER);
    TEST_ASSERT_TRUE(d.r > d.g);
    TEST_ASSERT_TRUE(d.r > d.b);
}

void test_role_name_valid(void) {
    for (int i = 0; i < CT_ROLE_COUNT; i++) {
        const char *name = ct_role_name((ct_role_t)i);
        TEST_ASSERT_NOT_NULL(name);
        TEST_ASSERT_TRUE(name[0] != '?');
    }
}

void test_role_alpha_is_1(void) {
    for (int i = 0; i < CT_ROLE_COUNT; i++) {
        color_rgba_t c = ct_role_color((ct_role_t)i);
        TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, c.a);
    }
}

/* --- System Palettes --- */

void test_system_astronomy_blue(void) {
    color_rgb_t c = ct_system_primary(CT_SYSTEM_ASTRONOMY);
    /* In linear space, blue channel should be dominant */
    TEST_ASSERT_TRUE(c.b > c.r);
}

void test_system_tzolkin_red(void) {
    color_rgb_t c = ct_system_primary(CT_SYSTEM_TZOLKIN);
    TEST_ASSERT_TRUE(c.r > c.g);
    TEST_ASSERT_TRUE(c.r > c.b);
}

void test_system_islamic_green(void) {
    color_rgb_t c = ct_system_primary(CT_SYSTEM_ISLAMIC);
    TEST_ASSERT_TRUE(c.g > c.r);
    TEST_ASSERT_TRUE(c.g > c.b);
}

void test_system_name_valid(void) {
    for (int i = 0; i < CT_SYSTEM_COUNT; i++) {
        const char *name = ct_system_name((ct_system_t)i);
        TEST_ASSERT_NOT_NULL(name);
        TEST_ASSERT_TRUE(name[0] != '?');
    }
}

void test_system_invalid_returns_gray(void) {
    color_rgb_t c = ct_system_primary(-1);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, c.r, c.g);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, c.g, c.b);
}

void test_system_three_colors_distinct(void) {
    color_rgb_t p = ct_system_primary(CT_SYSTEM_KABBALAH);
    color_rgb_t s = ct_system_secondary(CT_SYSTEM_KABBALAH);
    color_rgb_t a = ct_system_accent(CT_SYSTEM_KABBALAH);
    /* At least one channel must differ significantly */
    float diff_ps = fabsf(p.r - s.r) + fabsf(p.g - s.g) + fabsf(p.b - s.b);
    float diff_pa = fabsf(p.r - a.r) + fabsf(p.g - a.g) + fabsf(p.b - a.b);
    TEST_ASSERT_TRUE(diff_ps > 0.05f);
    TEST_ASSERT_TRUE(diff_pa > 0.05f);
}

/* --- Depth & Atmosphere --- */

void test_space_black_not_pure_black(void) {
    color_rgb_t sb = ct_space_black();
    /* Should have slight color, not 0,0,0 */
    TEST_ASSERT_TRUE(sb.r > 0.0f || sb.g > 0.0f || sb.b > 0.0f);
    /* But still very dark */
    TEST_ASSERT_TRUE(sb.r < 0.01f);
}

void test_depth_fog_near_vs_far(void) {
    color_rgb_t near = ct_depth_fog(0.0f);
    color_rgb_t far = ct_depth_fog(1.0f);
    /* Near should be slightly brighter than far */
    float near_lum = near.r + near.g + near.b;
    float far_lum = far.r + far.g + far.b;
    TEST_ASSERT_TRUE(near_lum > far_lum);
}

void test_star_glow_warm(void) {
    color_rgb_t glow = ct_star_glow();
    /* Should be warm white: r > g > b */
    TEST_ASSERT_TRUE(glow.r >= glow.g);
    TEST_ASSERT_TRUE(glow.g >= glow.b);
}

/* --- Accessibility --- */

void test_contrast_white_on_black(void) {
    color_rgb_t white = {1.0f, 1.0f, 1.0f};
    color_rgb_t black = {0.0f, 0.0f, 0.0f};
    float ratio = ct_contrast_ratio(white, black);
    TEST_ASSERT_FLOAT_WITHIN(0.5f, 21.0f, ratio);
}

void test_contrast_same_color_is_1(void) {
    color_rgb_t c = {0.5f, 0.5f, 0.5f};
    float ratio = ct_contrast_ratio(c, c);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 1.0f, ratio);
}

void test_meets_aa_white_on_black(void) {
    color_rgb_t white = {1.0f, 1.0f, 1.0f};
    color_rgb_t black = {0.0f, 0.0f, 0.0f};
    TEST_ASSERT_TRUE(ct_meets_aa(white, black));
}

void test_ensure_contrast_adjusts(void) {
    color_rgb_t dark_text = {0.1f, 0.1f, 0.1f};
    color_rgb_t dark_bg = {0.05f, 0.05f, 0.05f};
    color_rgb_t adjusted = ct_ensure_contrast(dark_text, dark_bg, 4.5f);
    /* Should have been lightened */
    TEST_ASSERT_TRUE(adjusted.r > dark_text.r);
}

/* --- Phi-Based Saturation & Lightness --- */

void test_phi_saturations_base_preserved(void) {
    float out[4];
    ct_phi_saturations(0.8f, 4, out);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.8f, out[0]);
}

void test_phi_saturations_decreasing(void) {
    float out[5];
    ct_phi_saturations(0.9f, 5, out);
    for (int i = 0; i < 4; i++) {
        TEST_ASSERT_TRUE(out[i] > out[i + 1]);
    }
}

void test_phi_saturations_ratio_is_phi_inv(void) {
    float out[3];
    ct_phi_saturations(1.0f, 3, out);
    float ratio = out[1] / out[0];
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.618f, ratio);
}

void test_phi_lightness_decreasing(void) {
    float out[5];
    ct_phi_lightness(0.8f, 5, out);
    for (int i = 0; i < 4; i++) {
        TEST_ASSERT_TRUE(out[i] > out[i + 1]);
    }
}

/* --- Purity --- */

void test_purity_no_side_effects(void) {
    color_rgb_t a = ct_system_primary(CT_SYSTEM_ASTRONOMY);
    color_rgb_t b = ct_system_primary(CT_SYSTEM_ASTRONOMY);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, a.r, b.r);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, a.g, b.g);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, a.b, b.b);
}

int main(void) {
    UNITY_BEGIN();

    /* sRGB/Linear */
    RUN_TEST(test_srgb_black_stays_black);
    RUN_TEST(test_srgb_white_stays_white);
    RUN_TEST(test_srgb_roundtrip);
    RUN_TEST(test_srgb_midpoint_is_darker_linear);
    RUN_TEST(test_to_linear_full_color);
    RUN_TEST(test_to_srgb_full_color);

    /* Golden Harmony */
    RUN_TEST(test_golden_palette_first_is_base);
    RUN_TEST(test_golden_palette_second_is_base_plus_golden_angle);
    RUN_TEST(test_golden_palette_preserves_sat_light);
    RUN_TEST(test_golden_complement_offset);
    RUN_TEST(test_golden_triad_three_colors);

    /* Psychology */
    RUN_TEST(test_mood_energy_is_red);
    RUN_TEST(test_mood_trust_is_blue);
    RUN_TEST(test_mood_harmony_is_green);
    RUN_TEST(test_mood_neutral_low_saturation);
    RUN_TEST(test_mood_name_valid);
    RUN_TEST(test_color_mood_roundtrip);

    /* Semantic Roles */
    RUN_TEST(test_role_background_is_dark);
    RUN_TEST(test_role_text_primary_is_bright);
    RUN_TEST(test_role_danger_is_red);
    RUN_TEST(test_role_name_valid);
    RUN_TEST(test_role_alpha_is_1);

    /* System Palettes */
    RUN_TEST(test_system_astronomy_blue);
    RUN_TEST(test_system_tzolkin_red);
    RUN_TEST(test_system_islamic_green);
    RUN_TEST(test_system_name_valid);
    RUN_TEST(test_system_invalid_returns_gray);
    RUN_TEST(test_system_three_colors_distinct);

    /* Depth */
    RUN_TEST(test_space_black_not_pure_black);
    RUN_TEST(test_depth_fog_near_vs_far);
    RUN_TEST(test_star_glow_warm);

    /* Accessibility */
    RUN_TEST(test_contrast_white_on_black);
    RUN_TEST(test_contrast_same_color_is_1);
    RUN_TEST(test_meets_aa_white_on_black);
    RUN_TEST(test_ensure_contrast_adjusts);

    /* Phi Saturation/Lightness */
    RUN_TEST(test_phi_saturations_base_preserved);
    RUN_TEST(test_phi_saturations_decreasing);
    RUN_TEST(test_phi_saturations_ratio_is_phi_inv);
    RUN_TEST(test_phi_lightness_decreasing);

    /* Purity */
    RUN_TEST(test_purity_no_side_effects);

    return UNITY_END();
}
