#include "../../tests/unity/unity.h"
#include "../../src/ui/golden_layout.h"

#include <math.h>

#define EPSILON 0.001f
#define PHI_F 1.6180339887f
#define PHI_INV_F 0.6180339887f

void setUp(void) {}
void tearDown(void) {}

/* --- Typography Scale --- */

void test_type_scale_body_is_base(void) {
    gl_type_scale_t s = gl_type_scale(14.0f);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 14.0f, s.sizes[3]);
}

void test_type_scale_h2_is_body_times_phi(void) {
    gl_type_scale_t s = gl_type_scale(14.0f);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 14.0f * PHI_F, s.sizes[2]);
}

void test_type_scale_h1_is_h2_times_phi(void) {
    gl_type_scale_t s = gl_type_scale(14.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 14.0f * PHI_F * PHI_F, s.sizes[1]);
}

void test_type_scale_display_is_h1_times_phi(void) {
    gl_type_scale_t s = gl_type_scale(14.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 14.0f * PHI_F * PHI_F * PHI_F, s.sizes[0]);
}

void test_type_scale_small_is_body_times_phi_inv(void) {
    gl_type_scale_t s = gl_type_scale(14.0f);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 14.0f * PHI_INV_F, s.sizes[4]);
}

void test_type_scale_decreasing(void) {
    gl_type_scale_t s = gl_type_scale(16.0f);
    for (int i = 0; i < PHI_TYPE_LEVELS - 1; i++) {
        TEST_ASSERT_TRUE(s.sizes[i] > s.sizes[i + 1]);
    }
}

void test_type_scale_line_height_is_phi(void) {
    gl_type_scale_t s = gl_type_scale(14.0f);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, PHI_F, s.line_height);
}

void test_type_scale_paragraph_is_phi_sq(void) {
    gl_type_scale_t s = gl_type_scale(14.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, PHI_F * PHI_F, s.paragraph_spacing);
}

void test_font_size_clamped(void) {
    gl_type_scale_t s = gl_type_scale(14.0f);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, s.sizes[0], gl_font_size(s, -1));
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, s.sizes[7], gl_font_size(s, 99));
}

/* --- Spacing Scale --- */

void test_spacing_base_at_level_4(void) {
    gl_spacing_t s = gl_spacing_scale(8.0f);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 8.0f, s.values[4]);
}

void test_spacing_level_5_is_base_times_phi(void) {
    gl_spacing_t s = gl_spacing_scale(8.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 8.0f * PHI_F, s.values[5]);
}

void test_spacing_level_3_is_base_times_phi_inv(void) {
    gl_spacing_t s = gl_spacing_scale(8.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 8.0f * PHI_INV_F, s.values[3]);
}

void test_spacing_monotonic_increasing(void) {
    gl_spacing_t s = gl_spacing_scale(8.0f);
    for (int i = 0; i < PHI_SPACE_LEVELS - 1; i++) {
        TEST_ASSERT_TRUE(s.values[i] < s.values[i + 1]);
    }
}

void test_space_clamped(void) {
    gl_spacing_t s = gl_spacing_scale(8.0f);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, s.values[0], gl_space(s, -5));
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, s.values[9], gl_space(s, 20));
}

/* --- Screen Layout --- */

void test_screen_layout_margin_positive(void) {
    gl_screen_t l = gl_screen_layout(16.0f / 9.0f);
    TEST_ASSERT_TRUE(l.margin > 0.0f);
    TEST_ASSERT_TRUE(l.margin < 0.2f);
}

void test_screen_main_wider_than_sidebar(void) {
    gl_screen_t l = gl_screen_layout(16.0f / 9.0f);
    TEST_ASSERT_TRUE(l.main_area.w > l.sidebar.w);
}

void test_screen_main_sidebar_ratio_is_phi(void) {
    gl_screen_t l = gl_screen_layout(16.0f / 9.0f);
    float total = l.main_area.w + l.sidebar.w;
    float ratio = total / l.main_area.w;
    TEST_ASSERT_FLOAT_WITHIN(0.01f, PHI_F, ratio);
}

void test_screen_hud_positions_within_bounds(void) {
    gl_screen_t l = gl_screen_layout(16.0f / 9.0f);
    TEST_ASSERT_TRUE(l.hud_top_left.x >= 0.0f);
    TEST_ASSERT_TRUE(l.hud_top_left.y >= 0.0f);
    TEST_ASSERT_TRUE(l.hud_top_right.x + l.hud_top_right.w <= 1.01f);
    TEST_ASSERT_TRUE(l.hud_bottom.y + l.hud_bottom.h <= 1.01f);
}

/* --- Rect Operations --- */

void test_rect_major_golden_ratio(void) {
    gl_rect_t r = {0.0f, 0.0f, 1.0f, 1.0f};
    gl_rect_t major = gl_rect_major(r, 1);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, PHI_INV_F, major.w);
}

void test_rect_minor_plus_major_equals_whole(void) {
    gl_rect_t r = {0.1f, 0.1f, 0.8f, 0.8f};
    gl_rect_t major = gl_rect_major(r, 1);
    gl_rect_t minor = gl_rect_minor(r, 1);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, r.w, major.w + minor.w);
}

void test_rect_golden_aspect_ratio(void) {
    gl_rect_t g = gl_rect_golden(0.5f, 0.5f, 0.8f);
    float ratio = g.w / g.h;
    TEST_ASSERT_FLOAT_WITHIN(0.01f, PHI_F, ratio);
}

/* --- Card Dimensions --- */

void test_card_golden_aspect(void) {
    gl_card_t c = gl_card_dimensions(200.0f, 8.0f);
    float ratio = c.width / c.height;
    TEST_ASSERT_FLOAT_WITHIN(0.01f, PHI_F, ratio);
}

void test_card_sections_sum_to_inner_height(void) {
    gl_card_t c = gl_card_dimensions(200.0f, 8.0f);
    float inner = c.height - 2.0f * c.padding;
    float sections = c.header_height + c.body_height + c.footer_height;
    TEST_ASSERT_FLOAT_WITHIN(0.1f, inner, sections);
}

void test_card_body_is_largest_section(void) {
    gl_card_t c = gl_card_dimensions(200.0f, 8.0f);
    TEST_ASSERT_TRUE(c.body_height > c.header_height);
    TEST_ASSERT_TRUE(c.body_height > c.footer_height);
}

/* --- Animation Timing --- */

void test_timing_base_preserved(void) {
    gl_timing_t t = gl_timing_scale(1.0f);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, t.base);
}

void test_timing_dramatic_is_base_times_phi(void) {
    gl_timing_t t = gl_timing_scale(1.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, PHI_F, t.dramatic);
}

void test_timing_slow_is_base_times_phi_inv(void) {
    gl_timing_t t = gl_timing_scale(1.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, PHI_INV_F, t.slow);
}

void test_timing_hierarchy(void) {
    gl_timing_t t = gl_timing_scale(1.0f);
    TEST_ASSERT_TRUE(t.micro < t.fast);
    TEST_ASSERT_TRUE(t.fast < t.normal);
    TEST_ASSERT_TRUE(t.normal < t.medium);
    TEST_ASSERT_TRUE(t.medium < t.slow);
    TEST_ASSERT_TRUE(t.slow < t.base);
    TEST_ASSERT_TRUE(t.base < t.dramatic);
    TEST_ASSERT_TRUE(t.dramatic < t.epic);
}

void test_stagger_delay_first_is_zero(void) {
    gl_timing_t t = gl_timing_scale(1.0f);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, gl_stagger_delay(t, 0, 5));
}

void test_stagger_delay_increasing(void) {
    gl_timing_t t = gl_timing_scale(1.0f);
    float d1 = gl_stagger_delay(t, 1, 5);
    float d2 = gl_stagger_delay(t, 2, 5);
    float d3 = gl_stagger_delay(t, 3, 5);
    TEST_ASSERT_TRUE(d1 > 0.0f);
    TEST_ASSERT_TRUE(d2 > d1);
    TEST_ASSERT_TRUE(d3 > d2);
}

/* --- Opacity Cascade --- */

void test_opacity_level_0_is_1(void) {
    gl_opacity_t o = gl_opacity_cascade();
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, o.values[0]);
}

void test_opacity_level_1_is_phi_inv(void) {
    gl_opacity_t o = gl_opacity_cascade();
    TEST_ASSERT_FLOAT_WITHIN(0.01f, PHI_INV_F, o.values[1]);
}

void test_opacity_decreasing(void) {
    gl_opacity_t o = gl_opacity_cascade();
    for (int i = 0; i < PHI_OPACITY_LEVELS - 1; i++) {
        TEST_ASSERT_TRUE(o.values[i] > o.values[i + 1]);
    }
}

void test_opacity_at_matches_cascade(void) {
    gl_opacity_t o = gl_opacity_cascade();
    for (int i = 0; i < PHI_OPACITY_LEVELS; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.001f, o.values[i], gl_opacity_at(i));
    }
}

/* --- Ring Proportions --- */

void test_ring_outer_is_inner_times_phi(void) {
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 10.0f * PHI_F, gl_ring_outer(10.0f));
}

void test_ring_inner_is_outer_times_phi_inv(void) {
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 10.0f * PHI_INV_F, gl_ring_inner(10.0f));
}

void test_ring_roundtrip(void) {
    float inner = 5.0f;
    float outer = gl_ring_outer(inner);
    float back = gl_ring_inner(outer);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, inner, back);
}

void test_concentric_radii_phi_spaced(void) {
    float radii[4];
    gl_concentric_radii(1.0f, 4, radii);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, radii[0]);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, PHI_F, radii[1]);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, PHI_F * PHI_F, radii[2]);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, PHI_F * PHI_F * PHI_F, radii[3]);
}

/* --- Scale System --- */

void test_zoom_factor_0_is_1(void) {
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, gl_zoom_factor(0));
}

void test_zoom_factor_1_is_phi(void) {
    TEST_ASSERT_FLOAT_WITHIN(0.01f, PHI_F, gl_zoom_factor(1));
}

void test_zoom_factor_2_is_phi_sq(void) {
    TEST_ASSERT_FLOAT_WITHIN(0.01f, PHI_F * PHI_F, gl_zoom_factor(2));
}

void test_aspect_ratio_is_phi(void) {
    TEST_ASSERT_FLOAT_WITHIN(0.01f, PHI_F, gl_aspect_ratio());
}

void test_scale_fade_center_is_max(void) {
    /* At the center of level 3 (position ~0.5), should be near 1.0 */
    float at_center = gl_scale_fade(3.0f / 6.0f, 3, 7);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, at_center);
}

void test_distribute_single_at_golden_point(void) {
    float pos;
    gl_distribute(0.0f, 100.0f, 1, &pos);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 100.0f * PHI_INV_F, pos);
}

/* --- Purity --- */

void test_purity_no_side_effects(void) {
    gl_type_scale_t s1 = gl_type_scale(14.0f);
    gl_type_scale_t s2 = gl_type_scale(14.0f);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, s1.sizes[3], s2.sizes[3]);
}

int main(void) {
    UNITY_BEGIN();

    /* Typography */
    RUN_TEST(test_type_scale_body_is_base);
    RUN_TEST(test_type_scale_h2_is_body_times_phi);
    RUN_TEST(test_type_scale_h1_is_h2_times_phi);
    RUN_TEST(test_type_scale_display_is_h1_times_phi);
    RUN_TEST(test_type_scale_small_is_body_times_phi_inv);
    RUN_TEST(test_type_scale_decreasing);
    RUN_TEST(test_type_scale_line_height_is_phi);
    RUN_TEST(test_type_scale_paragraph_is_phi_sq);
    RUN_TEST(test_font_size_clamped);

    /* Spacing */
    RUN_TEST(test_spacing_base_at_level_4);
    RUN_TEST(test_spacing_level_5_is_base_times_phi);
    RUN_TEST(test_spacing_level_3_is_base_times_phi_inv);
    RUN_TEST(test_spacing_monotonic_increasing);
    RUN_TEST(test_space_clamped);

    /* Screen Layout */
    RUN_TEST(test_screen_layout_margin_positive);
    RUN_TEST(test_screen_main_wider_than_sidebar);
    RUN_TEST(test_screen_main_sidebar_ratio_is_phi);
    RUN_TEST(test_screen_hud_positions_within_bounds);

    /* Rects */
    RUN_TEST(test_rect_major_golden_ratio);
    RUN_TEST(test_rect_minor_plus_major_equals_whole);
    RUN_TEST(test_rect_golden_aspect_ratio);

    /* Cards */
    RUN_TEST(test_card_golden_aspect);
    RUN_TEST(test_card_sections_sum_to_inner_height);
    RUN_TEST(test_card_body_is_largest_section);

    /* Timing */
    RUN_TEST(test_timing_base_preserved);
    RUN_TEST(test_timing_dramatic_is_base_times_phi);
    RUN_TEST(test_timing_slow_is_base_times_phi_inv);
    RUN_TEST(test_timing_hierarchy);
    RUN_TEST(test_stagger_delay_first_is_zero);
    RUN_TEST(test_stagger_delay_increasing);

    /* Opacity */
    RUN_TEST(test_opacity_level_0_is_1);
    RUN_TEST(test_opacity_level_1_is_phi_inv);
    RUN_TEST(test_opacity_decreasing);
    RUN_TEST(test_opacity_at_matches_cascade);

    /* Rings */
    RUN_TEST(test_ring_outer_is_inner_times_phi);
    RUN_TEST(test_ring_inner_is_outer_times_phi_inv);
    RUN_TEST(test_ring_roundtrip);
    RUN_TEST(test_concentric_radii_phi_spaced);

    /* Scale */
    RUN_TEST(test_zoom_factor_0_is_1);
    RUN_TEST(test_zoom_factor_1_is_phi);
    RUN_TEST(test_zoom_factor_2_is_phi_sq);
    RUN_TEST(test_aspect_ratio_is_phi);
    RUN_TEST(test_scale_fade_center_is_max);
    RUN_TEST(test_distribute_single_at_golden_point);

    /* Purity */
    RUN_TEST(test_purity_no_side_effects);

    return UNITY_END();
}
