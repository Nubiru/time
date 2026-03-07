#include "../unity/unity.h"
#include "../../src/render/text_render.h"

#define EPSILON 1e-4f

void setUp(void) { }
void tearDown(void) { }

/* ===== SDF Alpha Tests ===== */

void test_sdf_alpha_below_edge_fully_opaque(void) {
    /* Distance well below edge => fully inside glyph => alpha ~1.0 */
    float a = sdf_alpha(0.3f, 0.5f, 0.05f);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, a);
}

void test_sdf_alpha_above_edge_fully_transparent(void) {
    /* Distance well above edge => fully outside glyph => alpha ~0.0 */
    float a = sdf_alpha(0.8f, 0.5f, 0.05f);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, a);
}

void test_sdf_alpha_at_edge_half(void) {
    /* Distance exactly at edge => alpha ~0.5 */
    float a = sdf_alpha(0.5f, 0.5f, 0.05f);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.5f, a);
}

void test_sdf_alpha_zero_smoothing(void) {
    /* Hard edge: below = 1, at = 0.5, above = 0 */
    float below = sdf_alpha(0.49f, 0.5f, 0.0f);
    float above = sdf_alpha(0.51f, 0.5f, 0.0f);
    /* With zero smoothing, should still clamp reasonably */
    TEST_ASSERT_TRUE(below >= 0.0f && below <= 1.0f);
    TEST_ASSERT_TRUE(above >= 0.0f && above <= 1.0f);
}

void test_sdf_alpha_clamped_range(void) {
    /* Result must always be in [0, 1] */
    for (float d = 0.0f; d <= 1.0f; d += 0.1f) {
        float a = sdf_alpha(d, 0.5f, 0.1f);
        TEST_ASSERT_TRUE(a >= -EPSILON && a <= 1.0f + EPSILON);
    }
}

void test_sdf_alpha_wider_smoothing(void) {
    /* Wider smoothing => more gradual transition */
    float narrow = sdf_alpha(0.52f, 0.5f, 0.01f);
    float wide = sdf_alpha(0.52f, 0.5f, 0.2f);
    /* Wider smoothing keeps more alpha at same distance */
    TEST_ASSERT_TRUE(wide > narrow);
}

/* ===== SDF Outline Alpha Tests ===== */

void test_sdf_outline_alpha_inside_glyph(void) {
    /* Well inside the glyph (distance << edge): outline = 0 */
    float a = sdf_outline_alpha(0.2f, 0.5f, 0.1f, 0.02f);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, a);
}

void test_sdf_outline_alpha_at_edge(void) {
    /* At the glyph edge: should be in outline zone */
    float a = sdf_outline_alpha(0.5f, 0.5f, 0.1f, 0.02f);
    TEST_ASSERT_TRUE(a > 0.0f);
}

void test_sdf_outline_alpha_outside_outline(void) {
    /* Well outside outline range: alpha = 0 */
    float a = sdf_outline_alpha(0.8f, 0.5f, 0.1f, 0.02f);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, a);
}

void test_sdf_outline_zero_width(void) {
    /* Zero outline width => always 0 */
    float a = sdf_outline_alpha(0.5f, 0.5f, 0.0f, 0.02f);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, a);
}

/* ===== SDF Params Default/Factory Tests ===== */

void test_sdf_params_default_edge(void) {
    sdf_params_t p = sdf_params_default();
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.5f, p.edge_threshold);
}

void test_sdf_params_default_smoothing(void) {
    sdf_params_t p = sdf_params_default();
    TEST_ASSERT_TRUE(p.smoothing > 0.0f && p.smoothing <= 0.2f);
}

void test_sdf_params_default_no_outline(void) {
    sdf_params_t p = sdf_params_default();
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, p.outline_width);
}

void test_sdf_params_default_no_glow(void) {
    sdf_params_t p = sdf_params_default();
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, p.glow_radius);
}

void test_sdf_params_outlined_width(void) {
    sdf_params_t p = sdf_params_outlined(0.05f, 1.0f, 0.0f, 0.0f);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.05f, p.outline_width);
}

void test_sdf_params_outlined_color(void) {
    sdf_params_t p = sdf_params_outlined(0.05f, 0.2f, 0.3f, 0.4f);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.2f, p.outline_r);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.3f, p.outline_g);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.4f, p.outline_b);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, p.outline_a);
}

void test_sdf_params_shadowed_offsets(void) {
    sdf_params_t p = sdf_params_shadowed(2.0f, 3.0f, 0.5f);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 2.0f, p.shadow_offset_x);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 3.0f, p.shadow_offset_y);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.5f, p.shadow_softness);
}

void test_sdf_params_glowing_radius(void) {
    sdf_params_t p = sdf_params_glowing(0.15f, 1.0f, 0.8f, 0.2f);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.15f, p.glow_radius);
}

void test_sdf_params_glowing_color(void) {
    sdf_params_t p = sdf_params_glowing(0.15f, 0.9f, 0.7f, 0.1f);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.9f, p.glow_r);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.7f, p.glow_g);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.1f, p.glow_b);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, p.glow_a);
}

/* ===== Text Size Scale Tests ===== */

void test_text_size_scale_tiny(void) {
    float s = text_size_scale(TEXT_SIZE_TINY);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.5f, s);
}

void test_text_size_scale_small(void) {
    float s = text_size_scale(TEXT_SIZE_SMALL);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.7f, s);
}

void test_text_size_scale_body(void) {
    float s = text_size_scale(TEXT_SIZE_BODY);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, s);
}

void test_text_size_scale_large(void) {
    float s = text_size_scale(TEXT_SIZE_LARGE);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.4f, s);
}

void test_text_size_scale_title(void) {
    float s = text_size_scale(TEXT_SIZE_TITLE);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 2.0f, s);
}

void test_text_size_scale_display(void) {
    float s = text_size_scale(TEXT_SIZE_DISPLAY);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 3.0f, s);
}

void test_text_size_scale_invalid_returns_body(void) {
    float s = text_size_scale(TEXT_SIZE_COUNT);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, s);
}

/* ===== Text Style Factory Tests ===== */

void test_text_style_label_size(void) {
    text_style_t s = text_style_label();
    TEST_ASSERT_EQUAL_INT(TEXT_SIZE_SMALL, s.size);
}

void test_text_style_body_size(void) {
    text_style_t s = text_style_body();
    TEST_ASSERT_EQUAL_INT(TEXT_SIZE_BODY, s.size);
}

void test_text_style_title_size(void) {
    text_style_t s = text_style_title();
    TEST_ASSERT_EQUAL_INT(TEXT_SIZE_TITLE, s.size);
}

void test_text_style_subtitle_size(void) {
    text_style_t s = text_style_subtitle();
    TEST_ASSERT_EQUAL_INT(TEXT_SIZE_LARGE, s.size);
}

void test_text_style_annotation_size(void) {
    text_style_t s = text_style_annotation();
    TEST_ASSERT_EQUAL_INT(TEXT_SIZE_TINY, s.size);
}

void test_text_style_data_alignment(void) {
    text_style_t s = text_style_data();
    TEST_ASSERT_EQUAL_INT(TEXT_ALIGN_RIGHT, s.align);
}

void test_text_style_accent_has_glow(void) {
    text_style_t s = text_style_accent();
    TEST_ASSERT_TRUE(s.sdf.glow_radius > 0.0f);
}

void test_text_style_hud_alignment(void) {
    text_style_t s = text_style_hud();
    TEST_ASSERT_EQUAL_INT(TEXT_ALIGN_LEFT, s.align);
}

void test_text_style_opacity_valid(void) {
    text_style_t s = text_style_body();
    TEST_ASSERT_TRUE(s.opacity > 0.0f && s.opacity <= 1.0f);
}

void test_text_style_line_spacing_positive(void) {
    text_style_t s = text_style_body();
    TEST_ASSERT_TRUE(s.line_spacing > 0.0f);
}

/* ===== Animation Create Tests ===== */

void test_anim_create_type(void) {
    text_anim_t a = text_anim_create(TEXT_ANIM_FADE_IN, 0.5f);
    TEST_ASSERT_EQUAL_INT(TEXT_ANIM_FADE_IN, a.type);
}

void test_anim_create_duration(void) {
    text_anim_t a = text_anim_create(TEXT_ANIM_SLIDE_UP, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, a.duration);
}

void test_anim_create_initial_state(void) {
    text_anim_t a = text_anim_create(TEXT_ANIM_FADE_IN, 0.5f);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, a.elapsed);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, a.delay);
    TEST_ASSERT_EQUAL_INT(1, a.active);
    TEST_ASSERT_EQUAL_INT(0, a.finished);
}

void test_anim_none_always_finished(void) {
    text_anim_t a = text_anim_create(TEXT_ANIM_NONE, 1.0f);
    /* NONE type should be immediately finished */
    TEST_ASSERT_EQUAL_INT(1, a.finished);
}

void test_anim_typewriter_creates(void) {
    text_anim_t a = text_anim_typewriter(0.05f, 10);
    TEST_ASSERT_EQUAL_INT(TEXT_ANIM_TYPEWRITER, a.type);
    TEST_ASSERT_EQUAL_INT(10, a.char_count);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.05f, a.char_stagger);
}

void test_anim_typewriter_duration(void) {
    /* Duration = char_stagger * char_count */
    text_anim_t a = text_anim_typewriter(0.05f, 10);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.5f, a.duration);
}

void test_anim_delayed_adds_delay(void) {
    text_anim_t a = text_anim_create(TEXT_ANIM_FADE_IN, 0.5f);
    text_anim_t d = text_anim_delayed(a, 0.3f);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.3f, d.delay);
}

/* ===== Animation Update Tests ===== */

void test_anim_update_advances_time(void) {
    text_anim_t a = text_anim_create(TEXT_ANIM_FADE_IN, 1.0f);
    text_anim_t b = text_anim_update(a, 0.25f);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.25f, b.elapsed);
}

void test_anim_update_value_semantics(void) {
    text_anim_t a = text_anim_create(TEXT_ANIM_FADE_IN, 1.0f);
    text_anim_t b = text_anim_update(a, 0.5f);
    /* Original unchanged */
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, a.elapsed);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.5f, b.elapsed);
}

void test_anim_update_finishes(void) {
    text_anim_t a = text_anim_create(TEXT_ANIM_FADE_IN, 1.0f);
    text_anim_t b = text_anim_update(a, 1.5f);
    TEST_ASSERT_EQUAL_INT(1, b.finished);
}

void test_anim_update_not_finished_mid(void) {
    text_anim_t a = text_anim_create(TEXT_ANIM_FADE_IN, 1.0f);
    text_anim_t b = text_anim_update(a, 0.5f);
    TEST_ASSERT_EQUAL_INT(0, b.finished);
}

/* ===== Animation Progress Tests ===== */

void test_anim_progress_zero_at_start(void) {
    text_anim_t a = text_anim_create(TEXT_ANIM_FADE_IN, 1.0f);
    float p = text_anim_progress(a);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, p);
}

void test_anim_progress_half(void) {
    text_anim_t a = text_anim_create(TEXT_ANIM_FADE_IN, 1.0f);
    a = text_anim_update(a, 0.5f);
    float p = text_anim_progress(a);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.5f, p);
}

void test_anim_progress_one_at_end(void) {
    text_anim_t a = text_anim_create(TEXT_ANIM_FADE_IN, 1.0f);
    a = text_anim_update(a, 2.0f);
    float p = text_anim_progress(a);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, p);
}

void test_anim_progress_respects_delay(void) {
    text_anim_t a = text_anim_create(TEXT_ANIM_FADE_IN, 1.0f);
    a = text_anim_delayed(a, 0.5f);
    a = text_anim_update(a, 0.5f);
    /* elapsed=0.5, delay=0.5 => progress = (0.5-0.5)/1.0 = 0.0 */
    float p = text_anim_progress(a);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, p);
}

void test_anim_progress_after_delay(void) {
    text_anim_t a = text_anim_create(TEXT_ANIM_FADE_IN, 1.0f);
    a = text_anim_delayed(a, 0.5f);
    a = text_anim_update(a, 1.0f);
    /* elapsed=1.0, delay=0.5 => progress = (1.0-0.5)/1.0 = 0.5 */
    float p = text_anim_progress(a);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.5f, p);
}

/* ===== Animation Opacity Tests ===== */

void test_anim_opacity_fade_in_start(void) {
    text_anim_t a = text_anim_create(TEXT_ANIM_FADE_IN, 1.0f);
    float o = text_anim_opacity(a);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, o);
}

void test_anim_opacity_fade_in_end(void) {
    text_anim_t a = text_anim_create(TEXT_ANIM_FADE_IN, 1.0f);
    a = text_anim_update(a, 1.0f);
    float o = text_anim_opacity(a);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, o);
}

void test_anim_opacity_fade_out_start(void) {
    text_anim_t a = text_anim_create(TEXT_ANIM_FADE_OUT, 1.0f);
    float o = text_anim_opacity(a);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, o);
}

void test_anim_opacity_fade_out_end(void) {
    text_anim_t a = text_anim_create(TEXT_ANIM_FADE_OUT, 1.0f);
    a = text_anim_update(a, 1.0f);
    float o = text_anim_opacity(a);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, o);
}

void test_anim_opacity_none_always_one(void) {
    text_anim_t a = text_anim_create(TEXT_ANIM_NONE, 1.0f);
    float o = text_anim_opacity(a);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, o);
}

void test_anim_opacity_slide_always_one(void) {
    /* Slide animations affect position, not opacity */
    text_anim_t a = text_anim_create(TEXT_ANIM_SLIDE_UP, 1.0f);
    a = text_anim_update(a, 0.5f);
    float o = text_anim_opacity(a);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, o);
}

/* ===== Animation Offset Tests ===== */

void test_anim_offset_none_zero(void) {
    text_anim_t a = text_anim_create(TEXT_ANIM_NONE, 1.0f);
    float dx = 999.0f, dy = 999.0f;
    text_anim_offset(a, &dx, &dy);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, dx);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, dy);
}

void test_anim_offset_slide_up_start(void) {
    text_anim_t a = text_anim_create(TEXT_ANIM_SLIDE_UP, 1.0f);
    float dx = 0.0f, dy = 0.0f;
    text_anim_offset(a, &dx, &dy);
    /* At start: offset = (1-0)*30 = 30 upward (negative y) */
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, dx);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, -30.0f, dy);
}

void test_anim_offset_slide_up_end(void) {
    text_anim_t a = text_anim_create(TEXT_ANIM_SLIDE_UP, 1.0f);
    a = text_anim_update(a, 1.0f);
    float dx = 0.0f, dy = 0.0f;
    text_anim_offset(a, &dx, &dy);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, dx);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, dy);
}

void test_anim_offset_slide_down_start(void) {
    text_anim_t a = text_anim_create(TEXT_ANIM_SLIDE_DOWN, 1.0f);
    float dx = 0.0f, dy = 0.0f;
    text_anim_offset(a, &dx, &dy);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, dx);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 30.0f, dy);
}

void test_anim_offset_slide_left_start(void) {
    text_anim_t a = text_anim_create(TEXT_ANIM_SLIDE_LEFT, 1.0f);
    float dx = 0.0f, dy = 0.0f;
    text_anim_offset(a, &dx, &dy);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, -30.0f, dx);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, dy);
}

void test_anim_offset_slide_right_start(void) {
    text_anim_t a = text_anim_create(TEXT_ANIM_SLIDE_RIGHT, 1.0f);
    float dx = 0.0f, dy = 0.0f;
    text_anim_offset(a, &dx, &dy);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 30.0f, dx);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, dy);
}

void test_anim_offset_fade_is_zero(void) {
    text_anim_t a = text_anim_create(TEXT_ANIM_FADE_IN, 1.0f);
    float dx = 999.0f, dy = 999.0f;
    text_anim_offset(a, &dx, &dy);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, dx);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, dy);
}

/* ===== Animation Scale Tests ===== */

void test_anim_scale_none_one(void) {
    text_anim_t a = text_anim_create(TEXT_ANIM_NONE, 1.0f);
    float s = text_anim_scale(a);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, s);
}

void test_anim_scale_up_start(void) {
    text_anim_t a = text_anim_create(TEXT_ANIM_SCALE_UP, 1.0f);
    float s = text_anim_scale(a);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, s);
}

void test_anim_scale_up_end(void) {
    text_anim_t a = text_anim_create(TEXT_ANIM_SCALE_UP, 1.0f);
    a = text_anim_update(a, 1.0f);
    float s = text_anim_scale(a);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, s);
}

void test_anim_scale_down_start(void) {
    text_anim_t a = text_anim_create(TEXT_ANIM_SCALE_DOWN, 1.0f);
    float s = text_anim_scale(a);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, s);
}

void test_anim_scale_down_end(void) {
    text_anim_t a = text_anim_create(TEXT_ANIM_SCALE_DOWN, 1.0f);
    a = text_anim_update(a, 1.0f);
    float s = text_anim_scale(a);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, s);
}

void test_anim_scale_fade_is_one(void) {
    /* Fade animations don't affect scale */
    text_anim_t a = text_anim_create(TEXT_ANIM_FADE_IN, 1.0f);
    a = text_anim_update(a, 0.5f);
    float s = text_anim_scale(a);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, s);
}

/* ===== Typewriter Char Opacity Tests ===== */

void test_typewriter_char_0_visible_immediately(void) {
    text_anim_t a = text_anim_typewriter(0.1f, 5);
    a = text_anim_update(a, 0.05f);
    float o = text_anim_char_opacity(a, 0);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, o);
}

void test_typewriter_char_4_not_visible_early(void) {
    text_anim_t a = text_anim_typewriter(0.1f, 5);
    a = text_anim_update(a, 0.2f);
    /* char 4 needs elapsed >= 4*0.1 = 0.4 */
    float o = text_anim_char_opacity(a, 4);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, o);
}

void test_typewriter_char_4_visible_at_end(void) {
    text_anim_t a = text_anim_typewriter(0.1f, 5);
    a = text_anim_update(a, 0.5f);
    float o = text_anim_char_opacity(a, 4);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, o);
}

void test_typewriter_respects_delay(void) {
    text_anim_t a = text_anim_typewriter(0.1f, 5);
    a = text_anim_delayed(a, 1.0f);
    a = text_anim_update(a, 0.5f);
    /* elapsed=0.5 < delay=1.0 => no chars visible */
    float o = text_anim_char_opacity(a, 0);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, o);
}

void test_non_typewriter_char_opacity_uniform(void) {
    /* For non-typewriter anims, char opacity = overall opacity */
    text_anim_t a = text_anim_create(TEXT_ANIM_FADE_IN, 1.0f);
    a = text_anim_update(a, 0.5f);
    float o0 = text_anim_char_opacity(a, 0);
    float o5 = text_anim_char_opacity(a, 5);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, o0, o5);
}

/* ===== Script Range Tests ===== */

void test_script_range_count_at_least_8(void) {
    int c = script_range_count();
    TEST_ASSERT_TRUE(c >= 8);
}

void test_script_range_basic_latin(void) {
    script_range_t r = script_range_get(0);
    TEST_ASSERT_EQUAL_INT(0x0020, r.start_codepoint);
    TEST_ASSERT_EQUAL_INT(0x007E, r.end_codepoint);
    TEST_ASSERT_EQUAL_INT(0, r.rtl);
    TEST_ASSERT_EQUAL_INT(0, r.atlas_index);
    TEST_ASSERT_NOT_NULL(r.name);
}

void test_script_range_hebrew_rtl(void) {
    /* Find Hebrew range and verify it's RTL */
    script_range_t r = script_range_get(2);
    TEST_ASSERT_EQUAL_INT(1, r.rtl);
    TEST_ASSERT_EQUAL_INT(0x0590, r.start_codepoint);
}

void test_script_range_arabic_rtl(void) {
    script_range_t r = script_range_get(3);
    TEST_ASSERT_EQUAL_INT(1, r.rtl);
    TEST_ASSERT_EQUAL_INT(0x0600, r.start_codepoint);
}

void test_script_range_find_ascii(void) {
    int idx = script_range_find('A');
    TEST_ASSERT_EQUAL_INT(0, idx);
}

void test_script_range_find_hebrew(void) {
    int idx = script_range_find(0x05D0); /* Aleph */
    TEST_ASSERT_EQUAL_INT(2, idx);
}

void test_script_range_find_unknown(void) {
    /* Codepoint that doesn't match any range */
    int idx = script_range_find(0xFFFF);
    TEST_ASSERT_EQUAL_INT(-1, idx);
}

void test_script_is_rtl_hebrew(void) {
    int r = script_is_rtl(0x05D0);
    TEST_ASSERT_EQUAL_INT(1, r);
}

void test_script_is_rtl_latin(void) {
    int r = script_is_rtl('A');
    TEST_ASSERT_EQUAL_INT(0, r);
}

void test_script_is_rtl_unknown(void) {
    int r = script_is_rtl(0xFFFF);
    TEST_ASSERT_EQUAL_INT(0, r);
}

void test_script_atlas_for_latin(void) {
    int a = script_atlas_for('A');
    TEST_ASSERT_EQUAL_INT(0, a);
}

void test_script_atlas_for_cjk(void) {
    int a = script_atlas_for(0x4E00);
    TEST_ASSERT_EQUAL_INT(4, a);
}

void test_script_atlas_for_zodiac(void) {
    int a = script_atlas_for(0x2648); /* Aries */
    TEST_ASSERT_EQUAL_INT(5, a);
}

void test_script_atlas_for_unknown(void) {
    int a = script_atlas_for(0xFFFF);
    TEST_ASSERT_EQUAL_INT(-1, a);
}

/* ===== Purity Tests ===== */

void test_purity_sdf_alpha_deterministic(void) {
    float a1 = sdf_alpha(0.45f, 0.5f, 0.1f);
    float a2 = sdf_alpha(0.45f, 0.5f, 0.1f);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, a1, a2);
}

void test_purity_anim_update_no_mutation(void) {
    text_anim_t a = text_anim_create(TEXT_ANIM_FADE_IN, 1.0f);
    text_anim_t b = text_anim_update(a, 0.3f);
    text_anim_t c = text_anim_update(a, 0.3f);
    /* Same input => same output */
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, b.elapsed, c.elapsed);
    TEST_ASSERT_EQUAL_INT(b.finished, c.finished);
}

void test_purity_style_deterministic(void) {
    text_style_t a = text_style_title();
    text_style_t b = text_style_title();
    TEST_ASSERT_EQUAL_INT(a.size, b.size);
    TEST_ASSERT_EQUAL_INT(a.align, b.align);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, a.opacity, b.opacity);
}

/* ===== Runner ===== */

int main(void) {
    UNITY_BEGIN();

    /* SDF Alpha */
    RUN_TEST(test_sdf_alpha_below_edge_fully_opaque);
    RUN_TEST(test_sdf_alpha_above_edge_fully_transparent);
    RUN_TEST(test_sdf_alpha_at_edge_half);
    RUN_TEST(test_sdf_alpha_zero_smoothing);
    RUN_TEST(test_sdf_alpha_clamped_range);
    RUN_TEST(test_sdf_alpha_wider_smoothing);

    /* SDF Outline */
    RUN_TEST(test_sdf_outline_alpha_inside_glyph);
    RUN_TEST(test_sdf_outline_alpha_at_edge);
    RUN_TEST(test_sdf_outline_alpha_outside_outline);
    RUN_TEST(test_sdf_outline_zero_width);

    /* SDF Params */
    RUN_TEST(test_sdf_params_default_edge);
    RUN_TEST(test_sdf_params_default_smoothing);
    RUN_TEST(test_sdf_params_default_no_outline);
    RUN_TEST(test_sdf_params_default_no_glow);
    RUN_TEST(test_sdf_params_outlined_width);
    RUN_TEST(test_sdf_params_outlined_color);
    RUN_TEST(test_sdf_params_shadowed_offsets);
    RUN_TEST(test_sdf_params_glowing_radius);
    RUN_TEST(test_sdf_params_glowing_color);

    /* Text Size Scale */
    RUN_TEST(test_text_size_scale_tiny);
    RUN_TEST(test_text_size_scale_small);
    RUN_TEST(test_text_size_scale_body);
    RUN_TEST(test_text_size_scale_large);
    RUN_TEST(test_text_size_scale_title);
    RUN_TEST(test_text_size_scale_display);
    RUN_TEST(test_text_size_scale_invalid_returns_body);

    /* Text Styles */
    RUN_TEST(test_text_style_label_size);
    RUN_TEST(test_text_style_body_size);
    RUN_TEST(test_text_style_title_size);
    RUN_TEST(test_text_style_subtitle_size);
    RUN_TEST(test_text_style_annotation_size);
    RUN_TEST(test_text_style_data_alignment);
    RUN_TEST(test_text_style_accent_has_glow);
    RUN_TEST(test_text_style_hud_alignment);
    RUN_TEST(test_text_style_opacity_valid);
    RUN_TEST(test_text_style_line_spacing_positive);

    /* Animation Create */
    RUN_TEST(test_anim_create_type);
    RUN_TEST(test_anim_create_duration);
    RUN_TEST(test_anim_create_initial_state);
    RUN_TEST(test_anim_none_always_finished);
    RUN_TEST(test_anim_typewriter_creates);
    RUN_TEST(test_anim_typewriter_duration);
    RUN_TEST(test_anim_delayed_adds_delay);

    /* Animation Update */
    RUN_TEST(test_anim_update_advances_time);
    RUN_TEST(test_anim_update_value_semantics);
    RUN_TEST(test_anim_update_finishes);
    RUN_TEST(test_anim_update_not_finished_mid);

    /* Animation Progress */
    RUN_TEST(test_anim_progress_zero_at_start);
    RUN_TEST(test_anim_progress_half);
    RUN_TEST(test_anim_progress_one_at_end);
    RUN_TEST(test_anim_progress_respects_delay);
    RUN_TEST(test_anim_progress_after_delay);

    /* Animation Opacity */
    RUN_TEST(test_anim_opacity_fade_in_start);
    RUN_TEST(test_anim_opacity_fade_in_end);
    RUN_TEST(test_anim_opacity_fade_out_start);
    RUN_TEST(test_anim_opacity_fade_out_end);
    RUN_TEST(test_anim_opacity_none_always_one);
    RUN_TEST(test_anim_opacity_slide_always_one);

    /* Animation Offset */
    RUN_TEST(test_anim_offset_none_zero);
    RUN_TEST(test_anim_offset_slide_up_start);
    RUN_TEST(test_anim_offset_slide_up_end);
    RUN_TEST(test_anim_offset_slide_down_start);
    RUN_TEST(test_anim_offset_slide_left_start);
    RUN_TEST(test_anim_offset_slide_right_start);
    RUN_TEST(test_anim_offset_fade_is_zero);

    /* Animation Scale */
    RUN_TEST(test_anim_scale_none_one);
    RUN_TEST(test_anim_scale_up_start);
    RUN_TEST(test_anim_scale_up_end);
    RUN_TEST(test_anim_scale_down_start);
    RUN_TEST(test_anim_scale_down_end);
    RUN_TEST(test_anim_scale_fade_is_one);

    /* Typewriter */
    RUN_TEST(test_typewriter_char_0_visible_immediately);
    RUN_TEST(test_typewriter_char_4_not_visible_early);
    RUN_TEST(test_typewriter_char_4_visible_at_end);
    RUN_TEST(test_typewriter_respects_delay);
    RUN_TEST(test_non_typewriter_char_opacity_uniform);

    /* Script Ranges */
    RUN_TEST(test_script_range_count_at_least_8);
    RUN_TEST(test_script_range_basic_latin);
    RUN_TEST(test_script_range_hebrew_rtl);
    RUN_TEST(test_script_range_arabic_rtl);
    RUN_TEST(test_script_range_find_ascii);
    RUN_TEST(test_script_range_find_hebrew);
    RUN_TEST(test_script_range_find_unknown);
    RUN_TEST(test_script_is_rtl_hebrew);
    RUN_TEST(test_script_is_rtl_latin);
    RUN_TEST(test_script_is_rtl_unknown);
    RUN_TEST(test_script_atlas_for_latin);
    RUN_TEST(test_script_atlas_for_cjk);
    RUN_TEST(test_script_atlas_for_zodiac);
    RUN_TEST(test_script_atlas_for_unknown);

    /* Purity */
    RUN_TEST(test_purity_sdf_alpha_deterministic);
    RUN_TEST(test_purity_anim_update_no_mutation);
    RUN_TEST(test_purity_style_deterministic);

    return UNITY_END();
}
