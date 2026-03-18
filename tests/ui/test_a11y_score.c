#include "../../tests/unity/unity.h"
#include "../../src/ui/a11y_score.h"

#include <math.h>
#include <string.h>

#define EPSILON 0.01f

void setUp(void) {}
void tearDown(void) {}

/* ================================================================
 * UTILITY FUNCTIONS
 * ================================================================ */

void test_category_count_is_seven(void) {
    TEST_ASSERT_EQUAL_INT(7, a11y_category_count());
    TEST_ASSERT_EQUAL_INT(A11Y_CAT_COUNT, a11y_category_count());
}

void test_category_name_contrast(void) {
    TEST_ASSERT_EQUAL_STRING("Contrast", a11y_category_name(A11Y_CAT_CONTRAST));
}

void test_category_name_text_size(void) {
    TEST_ASSERT_EQUAL_STRING("Text Size", a11y_category_name(A11Y_CAT_TEXT_SIZE));
}

void test_category_name_target_size(void) {
    TEST_ASSERT_EQUAL_STRING("Target Size", a11y_category_name(A11Y_CAT_TARGET_SIZE));
}

void test_category_name_motion(void) {
    TEST_ASSERT_EQUAL_STRING("Motion", a11y_category_name(A11Y_CAT_MOTION));
}

void test_category_name_reflow(void) {
    TEST_ASSERT_EQUAL_STRING("Reflow", a11y_category_name(A11Y_CAT_REFLOW));
}

void test_category_name_spacing(void) {
    TEST_ASSERT_EQUAL_STRING("Spacing", a11y_category_name(A11Y_CAT_SPACING));
}

void test_category_name_focus(void) {
    TEST_ASSERT_EQUAL_STRING("Focus", a11y_category_name(A11Y_CAT_FOCUS));
}

void test_category_name_invalid(void) {
    TEST_ASSERT_EQUAL_STRING("?", a11y_category_name(A11Y_CAT_COUNT));
    TEST_ASSERT_EQUAL_STRING("?", a11y_category_name((a11y_category_t)99));
}

void test_level_name_fail(void) {
    TEST_ASSERT_EQUAL_STRING("Fail", a11y_level_name(A11Y_FAIL));
}

void test_level_name_a(void) {
    TEST_ASSERT_EQUAL_STRING("A", a11y_level_name(A11Y_A));
}

void test_level_name_aa(void) {
    TEST_ASSERT_EQUAL_STRING("AA", a11y_level_name(A11Y_AA));
}

void test_level_name_aaa(void) {
    TEST_ASSERT_EQUAL_STRING("AAA", a11y_level_name(A11Y_AAA));
}

void test_level_name_invalid(void) {
    TEST_ASSERT_EQUAL_STRING("?", a11y_level_name((a11y_level_t)99));
}

/* ================================================================
 * CONTRAST (1.4.3 / 1.4.6)
 * ================================================================ */

void test_contrast_fail_below_3(void) {
    a11y_input_t inp = a11y_input_default();
    inp.text_contrast_ratio = 2.9f;
    a11y_result_t r = a11y_score_category(inp, A11Y_CAT_CONTRAST);
    TEST_ASSERT_EQUAL_INT(A11Y_FAIL, r.level);
    TEST_ASSERT_EQUAL_STRING("1.4.3", r.guideline);
}

void test_contrast_a_at_3(void) {
    a11y_input_t inp = a11y_input_default();
    inp.text_contrast_ratio = 3.0f;
    a11y_result_t r = a11y_score_category(inp, A11Y_CAT_CONTRAST);
    TEST_ASSERT_EQUAL_INT(A11Y_A, r.level);
}

void test_contrast_aa_at_4_5(void) {
    a11y_input_t inp = a11y_input_default();
    inp.text_contrast_ratio = 4.5f;
    a11y_result_t r = a11y_score_category(inp, A11Y_CAT_CONTRAST);
    TEST_ASSERT_EQUAL_INT(A11Y_AA, r.level);
}

void test_contrast_aaa_at_7(void) {
    a11y_input_t inp = a11y_input_default();
    inp.text_contrast_ratio = 7.0f;
    a11y_result_t r = a11y_score_category(inp, A11Y_CAT_CONTRAST);
    TEST_ASSERT_EQUAL_INT(A11Y_AAA, r.level);
}

void test_contrast_score_clamped(void) {
    a11y_input_t inp = a11y_input_default();
    inp.text_contrast_ratio = 15.0f;
    a11y_result_t r = a11y_score_category(inp, A11Y_CAT_CONTRAST);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, r.score);
}

void test_contrast_score_at_zero(void) {
    a11y_input_t inp = a11y_input_default();
    inp.text_contrast_ratio = 0.0f;
    a11y_result_t r = a11y_score_category(inp, A11Y_CAT_CONTRAST);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, r.score);
}

void test_contrast_score_proportional(void) {
    a11y_input_t inp = a11y_input_default();
    inp.text_contrast_ratio = 3.5f;
    a11y_result_t r = a11y_score_category(inp, A11Y_CAT_CONTRAST);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 3.5f / 7.0f, r.score);
}

/* ================================================================
 * TEXT SIZE (1.4.4)
 * ================================================================ */

void test_text_fail_below_10(void) {
    a11y_input_t inp = a11y_input_default();
    inp.base_font_px = 9.0f;
    a11y_result_t r = a11y_score_category(inp, A11Y_CAT_TEXT_SIZE);
    TEST_ASSERT_EQUAL_INT(A11Y_FAIL, r.level);
    TEST_ASSERT_EQUAL_STRING("1.4.4", r.guideline);
}

void test_text_a_at_10(void) {
    a11y_input_t inp = a11y_input_default();
    inp.base_font_px = 10.0f;
    inp.min_font_px = 8.0f;
    a11y_result_t r = a11y_score_category(inp, A11Y_CAT_TEXT_SIZE);
    TEST_ASSERT_EQUAL_INT(A11Y_A, r.level);
}

void test_text_aa_requires_min_font(void) {
    a11y_input_t inp = a11y_input_default();
    inp.base_font_px = 14.0f;
    inp.min_font_px = 10.0f;
    a11y_result_t r = a11y_score_category(inp, A11Y_CAT_TEXT_SIZE);
    TEST_ASSERT_EQUAL_INT(A11Y_AA, r.level);
}

void test_text_aa_fails_if_min_font_low(void) {
    a11y_input_t inp = a11y_input_default();
    inp.base_font_px = 14.0f;
    inp.min_font_px = 9.0f;
    a11y_result_t r = a11y_score_category(inp, A11Y_CAT_TEXT_SIZE);
    TEST_ASSERT_EQUAL_INT(A11Y_A, r.level);
}

void test_text_aaa(void) {
    a11y_input_t inp = a11y_input_default();
    inp.base_font_px = 16.0f;
    inp.min_font_px = 12.0f;
    a11y_result_t r = a11y_score_category(inp, A11Y_CAT_TEXT_SIZE);
    TEST_ASSERT_EQUAL_INT(A11Y_AAA, r.level);
}

void test_text_score_clamped(void) {
    a11y_input_t inp = a11y_input_default();
    inp.base_font_px = 32.0f;
    a11y_result_t r = a11y_score_category(inp, A11Y_CAT_TEXT_SIZE);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, r.score);
}

/* ================================================================
 * TARGET SIZE (2.5.5)
 * ================================================================ */

void test_target_touch_fail(void) {
    a11y_input_t inp = a11y_input_default();
    inp.pointer_type = 1;
    inp.min_tap_target_px = 20;
    a11y_result_t r = a11y_score_category(inp, A11Y_CAT_TARGET_SIZE);
    TEST_ASSERT_EQUAL_INT(A11Y_FAIL, r.level);
    TEST_ASSERT_EQUAL_STRING("2.5.5", r.guideline);
}

void test_target_touch_a(void) {
    a11y_input_t inp = a11y_input_default();
    inp.pointer_type = 1;
    inp.min_tap_target_px = 24;
    a11y_result_t r = a11y_score_category(inp, A11Y_CAT_TARGET_SIZE);
    TEST_ASSERT_EQUAL_INT(A11Y_A, r.level);
}

void test_target_touch_aa(void) {
    a11y_input_t inp = a11y_input_default();
    inp.pointer_type = 1;
    inp.min_tap_target_px = 44;
    a11y_result_t r = a11y_score_category(inp, A11Y_CAT_TARGET_SIZE);
    TEST_ASSERT_EQUAL_INT(A11Y_AA, r.level);
}

void test_target_touch_aaa(void) {
    a11y_input_t inp = a11y_input_default();
    inp.pointer_type = 1;
    inp.min_tap_target_px = 48;
    a11y_result_t r = a11y_score_category(inp, A11Y_CAT_TARGET_SIZE);
    TEST_ASSERT_EQUAL_INT(A11Y_AAA, r.level);
}

void test_target_mouse_fail(void) {
    a11y_input_t inp = a11y_input_default();
    inp.pointer_type = 0;
    inp.min_tap_target_px = 10;
    a11y_result_t r = a11y_score_category(inp, A11Y_CAT_TARGET_SIZE);
    TEST_ASSERT_EQUAL_INT(A11Y_FAIL, r.level);
}

void test_target_mouse_a(void) {
    a11y_input_t inp = a11y_input_default();
    inp.pointer_type = 0;
    inp.min_tap_target_px = 16;
    a11y_result_t r = a11y_score_category(inp, A11Y_CAT_TARGET_SIZE);
    TEST_ASSERT_EQUAL_INT(A11Y_A, r.level);
}

void test_target_mouse_aa(void) {
    a11y_input_t inp = a11y_input_default();
    inp.pointer_type = 0;
    inp.min_tap_target_px = 24;
    a11y_result_t r = a11y_score_category(inp, A11Y_CAT_TARGET_SIZE);
    TEST_ASSERT_EQUAL_INT(A11Y_AA, r.level);
}

void test_target_mouse_aaa(void) {
    a11y_input_t inp = a11y_input_default();
    inp.pointer_type = 0;
    inp.min_tap_target_px = 44;
    a11y_result_t r = a11y_score_category(inp, A11Y_CAT_TARGET_SIZE);
    TEST_ASSERT_EQUAL_INT(A11Y_AAA, r.level);
}

void test_target_touch_score_clamped(void) {
    a11y_input_t inp = a11y_input_default();
    inp.pointer_type = 1;
    inp.min_tap_target_px = 100;
    a11y_result_t r = a11y_score_category(inp, A11Y_CAT_TARGET_SIZE);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, r.score);
}

/* ================================================================
 * MOTION (2.3.3)
 * ================================================================ */

void test_motion_disabled_is_aaa(void) {
    a11y_input_t inp = a11y_input_default();
    inp.animations_enabled = 0;
    a11y_result_t r = a11y_score_category(inp, A11Y_CAT_MOTION);
    TEST_ASSERT_EQUAL_INT(A11Y_AAA, r.level);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, r.score);
    TEST_ASSERT_EQUAL_STRING("2.3.3", r.guideline);
}

void test_motion_short_is_aaa(void) {
    a11y_input_t inp = a11y_input_default();
    inp.animations_enabled = 1;
    inp.motion_duration_ms = 300;
    a11y_result_t r = a11y_score_category(inp, A11Y_CAT_MOTION);
    TEST_ASSERT_EQUAL_INT(A11Y_AAA, r.level);
}

void test_motion_medium_is_aa(void) {
    a11y_input_t inp = a11y_input_default();
    inp.animations_enabled = 1;
    inp.motion_duration_ms = 3000;
    a11y_result_t r = a11y_score_category(inp, A11Y_CAT_MOTION);
    TEST_ASSERT_EQUAL_INT(A11Y_AA, r.level);
}

void test_motion_long_is_a(void) {
    a11y_input_t inp = a11y_input_default();
    inp.animations_enabled = 1;
    inp.motion_duration_ms = 7000;
    a11y_result_t r = a11y_score_category(inp, A11Y_CAT_MOTION);
    TEST_ASSERT_EQUAL_INT(A11Y_A, r.level);
}

void test_motion_excessive_is_fail(void) {
    a11y_input_t inp = a11y_input_default();
    inp.animations_enabled = 1;
    inp.motion_duration_ms = 15000;
    a11y_result_t r = a11y_score_category(inp, A11Y_CAT_MOTION);
    TEST_ASSERT_EQUAL_INT(A11Y_FAIL, r.level);
}

void test_motion_score_proportional(void) {
    a11y_input_t inp = a11y_input_default();
    inp.animations_enabled = 1;
    inp.motion_duration_ms = 5000;
    a11y_result_t r = a11y_score_category(inp, A11Y_CAT_MOTION);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.5f, r.score);
}

/* ================================================================
 * REFLOW (1.4.10)
 * ================================================================ */

void test_reflow_no_reflow_is_fail(void) {
    a11y_input_t inp = a11y_input_default();
    inp.content_reflows = 0;
    a11y_result_t r = a11y_score_category(inp, A11Y_CAT_REFLOW);
    TEST_ASSERT_EQUAL_INT(A11Y_FAIL, r.level);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, r.score);
    TEST_ASSERT_EQUAL_STRING("1.4.10", r.guideline);
}

void test_reflow_wide_screen_is_aa(void) {
    a11y_input_t inp = a11y_input_default();
    inp.content_reflows = 1;
    inp.screen_width_px = 1920;
    a11y_result_t r = a11y_score_category(inp, A11Y_CAT_REFLOW);
    TEST_ASSERT_EQUAL_INT(A11Y_AA, r.level);
}

void test_reflow_320_is_aa(void) {
    a11y_input_t inp = a11y_input_default();
    inp.content_reflows = 1;
    inp.screen_width_px = 320;
    a11y_result_t r = a11y_score_category(inp, A11Y_CAT_REFLOW);
    TEST_ASSERT_EQUAL_INT(A11Y_AA, r.level);
}

void test_reflow_256_is_aaa(void) {
    a11y_input_t inp = a11y_input_default();
    inp.content_reflows = 1;
    inp.screen_width_px = 256;
    a11y_result_t r = a11y_score_category(inp, A11Y_CAT_REFLOW);
    TEST_ASSERT_EQUAL_INT(A11Y_AAA, r.level);
}

void test_reflow_score_with_reflow(void) {
    a11y_input_t inp = a11y_input_default();
    inp.content_reflows = 1;
    a11y_result_t r = a11y_score_category(inp, A11Y_CAT_REFLOW);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, r.score);
}

/* ================================================================
 * SPACING (1.4.12)
 * ================================================================ */

void test_spacing_fail_below_1(void) {
    a11y_input_t inp = a11y_input_default();
    inp.line_height_ratio = 0.9f;
    a11y_result_t r = a11y_score_category(inp, A11Y_CAT_SPACING);
    TEST_ASSERT_EQUAL_INT(A11Y_FAIL, r.level);
    TEST_ASSERT_EQUAL_STRING("1.4.12", r.guideline);
}

void test_spacing_a_at_1_2(void) {
    a11y_input_t inp = a11y_input_default();
    inp.line_height_ratio = 1.2f;
    inp.paragraph_spacing_em = 1.0f;
    a11y_result_t r = a11y_score_category(inp, A11Y_CAT_SPACING);
    TEST_ASSERT_EQUAL_INT(A11Y_A, r.level);
}

void test_spacing_aa(void) {
    a11y_input_t inp = a11y_input_default();
    inp.line_height_ratio = 1.5f;
    inp.paragraph_spacing_em = 1.5f;
    a11y_result_t r = a11y_score_category(inp, A11Y_CAT_SPACING);
    TEST_ASSERT_EQUAL_INT(A11Y_AA, r.level);
}

void test_spacing_aa_fails_without_paragraph(void) {
    a11y_input_t inp = a11y_input_default();
    inp.line_height_ratio = 1.5f;
    inp.paragraph_spacing_em = 1.0f;
    a11y_result_t r = a11y_score_category(inp, A11Y_CAT_SPACING);
    TEST_ASSERT_EQUAL_INT(A11Y_A, r.level);
}

void test_spacing_aaa(void) {
    a11y_input_t inp = a11y_input_default();
    inp.line_height_ratio = 1.618f;
    inp.paragraph_spacing_em = 2.0f;
    a11y_result_t r = a11y_score_category(inp, A11Y_CAT_SPACING);
    TEST_ASSERT_EQUAL_INT(A11Y_AAA, r.level);
}

void test_spacing_score_clamped(void) {
    a11y_input_t inp = a11y_input_default();
    inp.line_height_ratio = 3.0f;
    a11y_result_t r = a11y_score_category(inp, A11Y_CAT_SPACING);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, r.score);
}

/* ================================================================
 * FOCUS (2.4.7)
 * ================================================================ */

void test_focus_not_visible_is_fail(void) {
    a11y_input_t inp = a11y_input_default();
    inp.focus_indicator_visible = 0;
    a11y_result_t r = a11y_score_category(inp, A11Y_CAT_FOCUS);
    TEST_ASSERT_EQUAL_INT(A11Y_FAIL, r.level);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, r.score);
    TEST_ASSERT_EQUAL_STRING("2.4.7", r.guideline);
}

void test_focus_visible_low_contrast_is_a(void) {
    a11y_input_t inp = a11y_input_default();
    inp.focus_indicator_visible = 1;
    inp.focus_contrast_ratio = 2;
    a11y_result_t r = a11y_score_category(inp, A11Y_CAT_FOCUS);
    TEST_ASSERT_EQUAL_INT(A11Y_A, r.level);
}

void test_focus_aa(void) {
    a11y_input_t inp = a11y_input_default();
    inp.focus_indicator_visible = 1;
    inp.focus_contrast_ratio = 3;
    a11y_result_t r = a11y_score_category(inp, A11Y_CAT_FOCUS);
    TEST_ASSERT_EQUAL_INT(A11Y_AA, r.level);
}

void test_focus_aaa(void) {
    a11y_input_t inp = a11y_input_default();
    inp.focus_indicator_visible = 1;
    inp.focus_contrast_ratio = 5;
    a11y_result_t r = a11y_score_category(inp, A11Y_CAT_FOCUS);
    TEST_ASSERT_EQUAL_INT(A11Y_AAA, r.level);
}

void test_focus_score_proportional(void) {
    a11y_input_t inp = a11y_input_default();
    inp.focus_indicator_visible = 1;
    inp.focus_contrast_ratio = 3;
    a11y_result_t r = a11y_score_category(inp, A11Y_CAT_FOCUS);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 3.0f / 5.0f, r.score);
}

/* ================================================================
 * DEFAULT AND WORST INPUTS
 * ================================================================ */

void test_default_input_overall_aa(void) {
    a11y_input_t inp = a11y_input_default();
    a11y_audit_t audit = a11y_audit(inp);
    TEST_ASSERT_EQUAL_INT(A11Y_AA, audit.overall);
}

void test_worst_input_overall_fail(void) {
    a11y_input_t inp = a11y_input_worst();
    a11y_audit_t audit = a11y_audit(inp);
    TEST_ASSERT_EQUAL_INT(A11Y_FAIL, audit.overall);
}

/* ================================================================
 * FULL AUDIT
 * ================================================================ */

void test_audit_has_all_categories(void) {
    a11y_input_t inp = a11y_input_default();
    a11y_audit_t audit = a11y_audit(inp);
    for (int i = 0; i < A11Y_CAT_COUNT; i++) {
        TEST_ASSERT_EQUAL_INT(i, audit.results[i].category);
    }
}

void test_audit_overall_is_minimum(void) {
    a11y_input_t inp = a11y_input_default();
    /* Force one category to A */
    inp.focus_indicator_visible = 1;
    inp.focus_contrast_ratio = 2;
    a11y_audit_t audit = a11y_audit(inp);
    TEST_ASSERT_EQUAL_INT(A11Y_A, audit.overall);
}

void test_audit_overall_score_is_average(void) {
    a11y_input_t inp = a11y_input_default();
    a11y_audit_t audit = a11y_audit(inp);
    float sum = 0.0f;
    for (int i = 0; i < A11Y_CAT_COUNT; i++) {
        sum += audit.results[i].score;
    }
    float expected = sum / (float)A11Y_CAT_COUNT;
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, expected, audit.overall_score);
}

void test_audit_pass_count(void) {
    a11y_input_t inp = a11y_input_default();
    a11y_audit_t audit = a11y_audit(inp);
    /* Default input: all categories should be AA or above */
    TEST_ASSERT_EQUAL_INT(A11Y_CAT_COUNT, audit.pass_count);
    TEST_ASSERT_EQUAL_INT(0, audit.fail_count);
}

void test_audit_fail_count(void) {
    a11y_input_t inp = a11y_input_worst();
    a11y_audit_t audit = a11y_audit(inp);
    TEST_ASSERT_TRUE(audit.fail_count > 0);
}

void test_audit_summaries_not_null(void) {
    a11y_input_t inp = a11y_input_default();
    a11y_audit_t audit = a11y_audit(inp);
    for (int i = 0; i < A11Y_CAT_COUNT; i++) {
        TEST_ASSERT_NOT_NULL(audit.results[i].summary);
        TEST_ASSERT_NOT_NULL(audit.results[i].guideline);
    }
}

/* ================================================================
 * SCORE CLAMPING
 * ================================================================ */

void test_score_never_exceeds_one(void) {
    a11y_input_t inp = a11y_input_default();
    inp.text_contrast_ratio = 21.0f;
    inp.base_font_px = 100.0f;
    inp.min_tap_target_px = 200;
    inp.line_height_ratio = 5.0f;
    inp.focus_contrast_ratio = 20;
    a11y_audit_t audit = a11y_audit(inp);
    for (int i = 0; i < A11Y_CAT_COUNT; i++) {
        TEST_ASSERT_TRUE(audit.results[i].score <= 1.0f);
    }
}

void test_score_never_below_zero(void) {
    a11y_input_t inp = a11y_input_worst();
    a11y_audit_t audit = a11y_audit(inp);
    for (int i = 0; i < A11Y_CAT_COUNT; i++) {
        TEST_ASSERT_TRUE(audit.results[i].score >= 0.0f);
    }
}

/* ================================================================
 * MAIN
 * ================================================================ */

int main(void) {
    UNITY_BEGIN();

    /* Utility */
    RUN_TEST(test_category_count_is_seven);
    RUN_TEST(test_category_name_contrast);
    RUN_TEST(test_category_name_text_size);
    RUN_TEST(test_category_name_target_size);
    RUN_TEST(test_category_name_motion);
    RUN_TEST(test_category_name_reflow);
    RUN_TEST(test_category_name_spacing);
    RUN_TEST(test_category_name_focus);
    RUN_TEST(test_category_name_invalid);
    RUN_TEST(test_level_name_fail);
    RUN_TEST(test_level_name_a);
    RUN_TEST(test_level_name_aa);
    RUN_TEST(test_level_name_aaa);
    RUN_TEST(test_level_name_invalid);

    /* Contrast */
    RUN_TEST(test_contrast_fail_below_3);
    RUN_TEST(test_contrast_a_at_3);
    RUN_TEST(test_contrast_aa_at_4_5);
    RUN_TEST(test_contrast_aaa_at_7);
    RUN_TEST(test_contrast_score_clamped);
    RUN_TEST(test_contrast_score_at_zero);
    RUN_TEST(test_contrast_score_proportional);

    /* Text Size */
    RUN_TEST(test_text_fail_below_10);
    RUN_TEST(test_text_a_at_10);
    RUN_TEST(test_text_aa_requires_min_font);
    RUN_TEST(test_text_aa_fails_if_min_font_low);
    RUN_TEST(test_text_aaa);
    RUN_TEST(test_text_score_clamped);

    /* Target Size */
    RUN_TEST(test_target_touch_fail);
    RUN_TEST(test_target_touch_a);
    RUN_TEST(test_target_touch_aa);
    RUN_TEST(test_target_touch_aaa);
    RUN_TEST(test_target_mouse_fail);
    RUN_TEST(test_target_mouse_a);
    RUN_TEST(test_target_mouse_aa);
    RUN_TEST(test_target_mouse_aaa);
    RUN_TEST(test_target_touch_score_clamped);

    /* Motion */
    RUN_TEST(test_motion_disabled_is_aaa);
    RUN_TEST(test_motion_short_is_aaa);
    RUN_TEST(test_motion_medium_is_aa);
    RUN_TEST(test_motion_long_is_a);
    RUN_TEST(test_motion_excessive_is_fail);
    RUN_TEST(test_motion_score_proportional);

    /* Reflow */
    RUN_TEST(test_reflow_no_reflow_is_fail);
    RUN_TEST(test_reflow_wide_screen_is_aa);
    RUN_TEST(test_reflow_320_is_aa);
    RUN_TEST(test_reflow_256_is_aaa);
    RUN_TEST(test_reflow_score_with_reflow);

    /* Spacing */
    RUN_TEST(test_spacing_fail_below_1);
    RUN_TEST(test_spacing_a_at_1_2);
    RUN_TEST(test_spacing_aa);
    RUN_TEST(test_spacing_aa_fails_without_paragraph);
    RUN_TEST(test_spacing_aaa);
    RUN_TEST(test_spacing_score_clamped);

    /* Focus */
    RUN_TEST(test_focus_not_visible_is_fail);
    RUN_TEST(test_focus_visible_low_contrast_is_a);
    RUN_TEST(test_focus_aa);
    RUN_TEST(test_focus_aaa);
    RUN_TEST(test_focus_score_proportional);

    /* Default / Worst */
    RUN_TEST(test_default_input_overall_aa);
    RUN_TEST(test_worst_input_overall_fail);

    /* Full Audit */
    RUN_TEST(test_audit_has_all_categories);
    RUN_TEST(test_audit_overall_is_minimum);
    RUN_TEST(test_audit_overall_score_is_average);
    RUN_TEST(test_audit_pass_count);
    RUN_TEST(test_audit_fail_count);
    RUN_TEST(test_audit_summaries_not_null);

    /* Score Clamping */
    RUN_TEST(test_score_never_exceeds_one);
    RUN_TEST(test_score_never_below_zero);

    return UNITY_END();
}
