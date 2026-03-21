#include "../unity/unity.h"
#include "../../src/ui/ui_state.h"

void setUp(void) {}
void tearDown(void) {}

/* ---- 1-4: ui_state_init defaults ---- */

void test_init_mode_is_default(void)
{
    ui_state_t s = ui_state_init();
    TEST_ASSERT_EQUAL_INT(UI_MODE_DEFAULT, s.mode);
}

void test_init_mode_progress_is_zero(void)
{
    ui_state_t s = ui_state_init();
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, s.mode_progress);
}

void test_init_hint_opacity_is_one(void)
{
    ui_state_t s = ui_state_init();
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, s.hint_opacity);
}

void test_init_all_proximities_zero(void)
{
    ui_state_t s = ui_state_init();
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, s.bottom_proximity);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, s.left_proximity);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, s.right_proximity);
}

/* ---- 5-6: ui_timing_default ---- */

void test_timing_default_panel_duration(void)
{
    ui_timing_t t = ui_timing_default();
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.382f, t.panel_duration);
}

void test_timing_default_hint_duration(void)
{
    ui_timing_t t = ui_timing_default();
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 5.0f, t.hint_duration);
}

/* ---- 7-8: toggle_mode from DEFAULT ---- */

void test_toggle_help_from_default_sets_mode(void)
{
    ui_state_t s = ui_state_init();
    s = ui_state_toggle_mode(s, UI_MODE_HELP);
    TEST_ASSERT_EQUAL_INT(UI_MODE_HELP, s.mode);
}

void test_toggle_help_from_default_sets_opening(void)
{
    ui_state_t s = ui_state_init();
    s = ui_state_toggle_mode(s, UI_MODE_HELP);
    TEST_ASSERT_EQUAL_INT(1, s.mode_opening);
}

/* ---- 9: toggle same mode starts closing ---- */

void test_toggle_help_when_help_starts_closing(void)
{
    ui_state_t s = ui_state_init();
    s = ui_state_toggle_mode(s, UI_MODE_HELP);
    s = ui_state_toggle_mode(s, UI_MODE_HELP);
    TEST_ASSERT_EQUAL_INT(0, s.mode_opening);
}

/* ---- 10: toggle different mode switches ---- */

void test_toggle_palette_when_help_switches(void)
{
    ui_state_t s = ui_state_init();
    s = ui_state_toggle_mode(s, UI_MODE_HELP);
    s = ui_state_toggle_mode(s, UI_MODE_PALETTE);
    TEST_ASSERT_EQUAL_INT(UI_MODE_PALETTE, s.mode);
    TEST_ASSERT_EQUAL_INT(1, s.mode_opening);
}

/* ---- 11-12: ui_state_close ---- */

void test_close_when_help_open(void)
{
    ui_state_t s = ui_state_init();
    s = ui_state_toggle_mode(s, UI_MODE_HELP);
    s = ui_state_close(s);
    TEST_ASSERT_EQUAL_INT(0, s.mode_opening);
}

void test_close_when_default_stays_default(void)
{
    ui_state_t s = ui_state_init();
    s = ui_state_close(s);
    TEST_ASSERT_EQUAL_INT(UI_MODE_DEFAULT, s.mode);
}

/* ---- 13-14: ui_state_is_mode ---- */

void test_is_mode_correct_for_active(void)
{
    ui_state_t s = ui_state_init();
    s = ui_state_toggle_mode(s, UI_MODE_HELP);
    TEST_ASSERT_EQUAL_INT(1, ui_state_is_mode(s, UI_MODE_HELP));
}

void test_is_mode_zero_for_inactive(void)
{
    ui_state_t s = ui_state_init();
    s = ui_state_toggle_mode(s, UI_MODE_HELP);
    TEST_ASSERT_EQUAL_INT(0, ui_state_is_mode(s, UI_MODE_PALETTE));
}

/* ---- 15-18: ui_state_update mode_progress ---- */

void test_update_progress_increases_when_opening(void)
{
    ui_state_t s = ui_state_init();
    ui_timing_t t = ui_timing_default();
    s = ui_state_toggle_mode(s, UI_MODE_HELP);
    s = ui_state_update(s, 0.1f, t);
    TEST_ASSERT_TRUE(s.mode_progress > 0.0f);
}

void test_update_progress_decreases_when_closing(void)
{
    ui_state_t s = ui_state_init();
    ui_timing_t t = ui_timing_default();
    s = ui_state_toggle_mode(s, UI_MODE_HELP);
    /* Advance to partially open */
    s = ui_state_update(s, 0.2f, t);
    float before = s.mode_progress;
    /* Start closing */
    s = ui_state_toggle_mode(s, UI_MODE_HELP);
    s = ui_state_update(s, 0.1f, t);
    TEST_ASSERT_TRUE(s.mode_progress < before);
}

void test_update_progress_clamps_to_one(void)
{
    ui_state_t s = ui_state_init();
    ui_timing_t t = ui_timing_default();
    s = ui_state_toggle_mode(s, UI_MODE_HELP);
    /* Advance well past panel_duration */
    s = ui_state_update(s, 10.0f, t);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, s.mode_progress);
}

void test_update_mode_becomes_default_when_closing_completes(void)
{
    ui_state_t s = ui_state_init();
    ui_timing_t t = ui_timing_default();
    s = ui_state_toggle_mode(s, UI_MODE_HELP);
    s = ui_state_update(s, 0.2f, t);
    /* Start closing */
    s = ui_state_toggle_mode(s, UI_MODE_HELP);
    /* Advance well past panel_duration to complete close */
    s = ui_state_update(s, 10.0f, t);
    TEST_ASSERT_EQUAL_INT(UI_MODE_DEFAULT, s.mode);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, s.mode_progress);
}

/* ---- 19-22: hint animation ---- */

void test_update_hint_timer_increases(void)
{
    ui_state_t s = ui_state_init();
    ui_timing_t t = ui_timing_default();
    s = ui_state_update(s, 1.0f, t);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, s.hint_timer);
}

void test_update_hint_opacity_stays_during_duration(void)
{
    ui_state_t s = ui_state_init();
    ui_timing_t t = ui_timing_default();
    /* 3 seconds into a 5-second duration: hint should still be fully visible */
    s = ui_state_update(s, 3.0f, t);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, s.hint_opacity);
}

void test_update_hint_opacity_decreases_after_duration(void)
{
    ui_state_t s = ui_state_init();
    ui_timing_t t = ui_timing_default();
    /* Past hint_duration (5.0) + some fade time */
    s = ui_state_update(s, 5.5f, t);
    TEST_ASSERT_TRUE(s.hint_opacity < 1.0f);
    TEST_ASSERT_TRUE(s.hint_opacity > 0.0f);
}

void test_update_hint_opacity_reaches_zero(void)
{
    ui_state_t s = ui_state_init();
    ui_timing_t t = ui_timing_default();
    /* Well past hint_duration + hint_fade (~5.0 + 1.618) */
    s = ui_state_update(s, 10.0f, t);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, s.hint_opacity);
}

/* ---- 23-26: edge proximity ---- */

void test_mouse_bottom_proximity_at_bottom(void)
{
    ui_state_t s = ui_state_init();
    ui_timing_t t = ui_timing_default();
    /* Mouse at very bottom of screen (y close to screen_h) */
    s = ui_state_update_mouse(s, 500.0f, 799.0f, 1000.0f, 800.0f, t);
    TEST_ASSERT_TRUE(s.bottom_proximity > 0.9f);
}

void test_mouse_bottom_proximity_zero_at_top(void)
{
    ui_state_t s = ui_state_init();
    ui_timing_t t = ui_timing_default();
    /* Mouse at top of screen (y=0), far from bottom */
    s = ui_state_update_mouse(s, 500.0f, 0.0f, 1000.0f, 800.0f, t);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, s.bottom_proximity);
}

void test_mouse_left_proximity_at_left_edge(void)
{
    ui_state_t s = ui_state_init();
    ui_timing_t t = ui_timing_default();
    /* Mouse at left edge (x=0) */
    s = ui_state_update_mouse(s, 0.0f, 400.0f, 1000.0f, 800.0f, t);
    TEST_ASSERT_TRUE(s.left_proximity > 0.9f);
}

void test_mouse_right_proximity_at_right_edge(void)
{
    ui_state_t s = ui_state_init();
    ui_timing_t t = ui_timing_default();
    /* Mouse at right edge (x=screen_w) */
    s = ui_state_update_mouse(s, 1000.0f, 400.0f, 1000.0f, 800.0f, t);
    TEST_ASSERT_TRUE(s.right_proximity > 0.9f);
}

/* ---- 27-28: panel opacity ---- */

void test_panel_opacity_zero_for_non_active(void)
{
    ui_state_t s = ui_state_init();
    s = ui_state_toggle_mode(s, UI_MODE_HELP);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, ui_state_panel_opacity(s, UI_MODE_PALETTE));
}

void test_panel_opacity_returns_progress_for_active(void)
{
    ui_state_t s = ui_state_init();
    ui_timing_t t = ui_timing_default();
    s = ui_state_toggle_mode(s, UI_MODE_HELP);
    s = ui_state_update(s, 0.1f, t);
    float expected = s.mode_progress;
    TEST_ASSERT_FLOAT_WITHIN(0.001f, expected, ui_state_panel_opacity(s, UI_MODE_HELP));
}

/* ---- 29: time bar opacity ---- */

void test_time_bar_opacity_returns_bottom_proximity(void)
{
    ui_state_t s = ui_state_init();
    ui_timing_t t = ui_timing_default();
    s = ui_state_update_mouse(s, 500.0f, 799.0f, 1000.0f, 800.0f, t);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, s.bottom_proximity, ui_state_time_bar_opacity(s));
}

/* ---- 30: layer panel opacity ---- */

void test_layer_panel_opacity_max_of_proximity_and_progress(void)
{
    ui_state_t s = ui_state_init();
    ui_timing_t t = ui_timing_default();
    /* Set left proximity via mouse at left edge */
    s = ui_state_update_mouse(s, 0.0f, 400.0f, 1000.0f, 800.0f, t);
    float prox = s.left_proximity;
    /* No layer mode active, so layer_progress = 0 */
    float result = ui_state_layer_panel_opacity(s);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, prox, result);

    /* Now open layer panel and advance */
    s = ui_state_toggle_mode(s, UI_MODE_LAYER_PANEL);
    s = ui_state_update(s, 10.0f, t);
    /* mode_progress should be 1.0, which may exceed proximity */
    result = ui_state_layer_panel_opacity(s);
    float expected = (s.left_proximity > s.mode_progress) ? s.left_proximity : s.mode_progress;
    TEST_ASSERT_FLOAT_WITHIN(0.001f, expected, result);
}

/* ---- 31: scene brightness ---- */

void test_scene_brightness_full_when_no_panel(void)
{
    ui_state_t s = ui_state_init();
    ui_timing_t t = ui_timing_default();
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, ui_state_scene_brightness(s, t));
}

void test_scene_brightness_reduced_when_panel_open(void)
{
    ui_state_t s = ui_state_init();
    ui_timing_t t = ui_timing_default();
    s = ui_state_toggle_mode(s, UI_MODE_HELP);
    /* Advance well past panel_duration so scene_dim catches up */
    s = ui_state_update(s, 10.0f, t);
    float brightness = ui_state_scene_brightness(s, t);
    TEST_ASSERT_TRUE(brightness < 1.0f);
    /* Should be approximately 1.0 - dim_amount = 1.0 - 0.382 = 0.618 */
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f - t.dim_amount, brightness);
}

/* ---- 32-33: ui_mode_name and ui_mode_count ---- */

void test_mode_name_non_null_for_all(void)
{
    for (int i = 0; i < UI_MODE_COUNT; i++) {
        const char *name = ui_mode_name((ui_mode_t)i);
        TEST_ASSERT_NOT_NULL(name);
        TEST_ASSERT_TRUE(name[0] != '\0');
    }
}

void test_mode_count_equals_enum(void)
{
    TEST_ASSERT_EQUAL_INT(UI_MODE_COUNT, ui_mode_count());
    TEST_ASSERT_EQUAL_INT(5, ui_mode_count());
}

/* ---- 34: full cycle: open -> fully open -> close -> fully closed ---- */

void test_full_open_close_cycle(void)
{
    ui_state_t s = ui_state_init();
    ui_timing_t t = ui_timing_default();

    /* Start in DEFAULT */
    TEST_ASSERT_EQUAL_INT(UI_MODE_DEFAULT, s.mode);

    /* Open HELP */
    s = ui_state_toggle_mode(s, UI_MODE_HELP);
    TEST_ASSERT_EQUAL_INT(UI_MODE_HELP, s.mode);
    TEST_ASSERT_EQUAL_INT(1, s.mode_opening);

    /* Advance to fully open */
    s = ui_state_update(s, 10.0f, t);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, s.mode_progress);
    TEST_ASSERT_EQUAL_INT(UI_MODE_HELP, s.mode);

    /* Close it */
    s = ui_state_toggle_mode(s, UI_MODE_HELP);
    TEST_ASSERT_EQUAL_INT(0, s.mode_opening);

    /* Advance to fully closed */
    s = ui_state_update(s, 10.0f, t);
    TEST_ASSERT_EQUAL_INT(UI_MODE_DEFAULT, s.mode);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, s.mode_progress);
}

/* ---- 35-40: Additional edge cases and coverage ---- */

void test_toggle_invalid_mode_no_change(void)
{
    ui_state_t s = ui_state_init();
    s = ui_state_toggle_mode(s, UI_MODE_DEFAULT);
    TEST_ASSERT_EQUAL_INT(UI_MODE_DEFAULT, s.mode);
    s = ui_state_toggle_mode(s, UI_MODE_COUNT);
    TEST_ASSERT_EQUAL_INT(UI_MODE_DEFAULT, s.mode);
}

void test_update_zero_dt_no_change(void)
{
    ui_state_t s = ui_state_init();
    ui_timing_t t = ui_timing_default();
    s = ui_state_toggle_mode(s, UI_MODE_HELP);
    ui_state_t s2 = ui_state_update(s, 0.0f, t);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, s.mode_progress, s2.mode_progress);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, s.hint_timer, s2.hint_timer);
}

void test_mouse_center_no_proximity(void)
{
    ui_state_t s = ui_state_init();
    ui_timing_t t = ui_timing_default();
    /* Mouse in center of screen */
    s = ui_state_update_mouse(s, 500.0f, 400.0f, 1000.0f, 800.0f, t);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, s.bottom_proximity);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, s.left_proximity);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, s.right_proximity);
}

void test_mouse_invalid_screen_no_crash(void)
{
    ui_state_t s = ui_state_init();
    ui_timing_t t = ui_timing_default();
    /* Zero-size screen should not crash */
    s = ui_state_update_mouse(s, 0.0f, 0.0f, 0.0f, 0.0f, t);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, s.bottom_proximity);
}

void test_mode_name_unknown_for_invalid(void)
{
    const char *name = ui_mode_name(UI_MODE_COUNT);
    TEST_ASSERT_EQUAL_STRING("Unknown", name);
}

void test_scene_dim_returns_to_full_after_close(void)
{
    ui_state_t s = ui_state_init();
    ui_timing_t t = ui_timing_default();

    /* Open panel and let dim settle */
    s = ui_state_toggle_mode(s, UI_MODE_HELP);
    s = ui_state_update(s, 10.0f, t);
    TEST_ASSERT_TRUE(s.scene_dim > 0.0f);

    /* Close panel and let dim settle */
    s = ui_state_toggle_mode(s, UI_MODE_HELP);
    s = ui_state_update(s, 10.0f, t);
    /* scene_dim should return to 0 */
    /* Note: mode becomes DEFAULT during the update, then scene_dim targets 0 */
    s = ui_state_update(s, 10.0f, t);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, s.scene_dim);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, ui_state_scene_brightness(s, t));
}

/* ---- Main runner ---- */

int main(void)
{
    UNITY_BEGIN();

    /* Init defaults: 1-4 */
    RUN_TEST(test_init_mode_is_default);
    RUN_TEST(test_init_mode_progress_is_zero);
    RUN_TEST(test_init_hint_opacity_is_one);
    RUN_TEST(test_init_all_proximities_zero);

    /* Timing defaults: 5-6 */
    RUN_TEST(test_timing_default_panel_duration);
    RUN_TEST(test_timing_default_hint_duration);

    /* Toggle mode: 7-10 */
    RUN_TEST(test_toggle_help_from_default_sets_mode);
    RUN_TEST(test_toggle_help_from_default_sets_opening);
    RUN_TEST(test_toggle_help_when_help_starts_closing);
    RUN_TEST(test_toggle_palette_when_help_switches);

    /* Close: 11-12 */
    RUN_TEST(test_close_when_help_open);
    RUN_TEST(test_close_when_default_stays_default);

    /* Is mode: 13-14 */
    RUN_TEST(test_is_mode_correct_for_active);
    RUN_TEST(test_is_mode_zero_for_inactive);

    /* Update mode_progress: 15-18 */
    RUN_TEST(test_update_progress_increases_when_opening);
    RUN_TEST(test_update_progress_decreases_when_closing);
    RUN_TEST(test_update_progress_clamps_to_one);
    RUN_TEST(test_update_mode_becomes_default_when_closing_completes);

    /* Hint animation: 19-22 */
    RUN_TEST(test_update_hint_timer_increases);
    RUN_TEST(test_update_hint_opacity_stays_during_duration);
    RUN_TEST(test_update_hint_opacity_decreases_after_duration);
    RUN_TEST(test_update_hint_opacity_reaches_zero);

    /* Edge proximity: 23-26 */
    RUN_TEST(test_mouse_bottom_proximity_at_bottom);
    RUN_TEST(test_mouse_bottom_proximity_zero_at_top);
    RUN_TEST(test_mouse_left_proximity_at_left_edge);
    RUN_TEST(test_mouse_right_proximity_at_right_edge);

    /* Panel opacity: 27-28 */
    RUN_TEST(test_panel_opacity_zero_for_non_active);
    RUN_TEST(test_panel_opacity_returns_progress_for_active);

    /* Time bar opacity: 29 */
    RUN_TEST(test_time_bar_opacity_returns_bottom_proximity);

    /* Layer panel opacity: 30 */
    RUN_TEST(test_layer_panel_opacity_max_of_proximity_and_progress);

    /* Scene brightness: 31 */
    RUN_TEST(test_scene_brightness_full_when_no_panel);
    RUN_TEST(test_scene_brightness_reduced_when_panel_open);

    /* Mode name/count: 32-33 */
    RUN_TEST(test_mode_name_non_null_for_all);
    RUN_TEST(test_mode_count_equals_enum);

    /* Full cycle: 34 */
    RUN_TEST(test_full_open_close_cycle);

    /* Edge cases: 35-40 */
    RUN_TEST(test_toggle_invalid_mode_no_change);
    RUN_TEST(test_update_zero_dt_no_change);
    RUN_TEST(test_mouse_center_no_proximity);
    RUN_TEST(test_mouse_invalid_screen_no_crash);
    RUN_TEST(test_mode_name_unknown_for_invalid);
    RUN_TEST(test_scene_dim_returns_to_full_after_close);

    return UNITY_END();
}
