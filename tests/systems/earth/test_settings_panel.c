/* test_settings_panel.c -- Tests for settings panel data layer.
 * TDD RED phase: all tests written before implementation.
 * PERSONA stream: Settings UI data model. */

#include "unity.h"
#include "systems/earth/settings_panel.h"

#include <string.h>

void setUp(void) { }
void tearDown(void) { }

/* ===== sp_build — section structure ===== */

void test_build_returns_four_sections(void) {
    up_prefs_t p = up_default();
    sp_panel_t panel = sp_build(&p);
    TEST_ASSERT_EQUAL_INT(4, panel.section_count);
}

void test_build_section_0_is_appearance(void) {
    up_prefs_t p = up_default();
    sp_panel_t panel = sp_build(&p);
    TEST_ASSERT_EQUAL_STRING("Appearance", panel.sections[0].name);
}

void test_build_section_1_is_time(void) {
    up_prefs_t p = up_default();
    sp_panel_t panel = sp_build(&p);
    TEST_ASSERT_EQUAL_STRING("Time", panel.sections[1].name);
}

void test_build_section_2_is_social(void) {
    up_prefs_t p = up_default();
    sp_panel_t panel = sp_build(&p);
    TEST_ASSERT_EQUAL_STRING("Social", panel.sections[2].name);
}

void test_build_section_3_is_location(void) {
    up_prefs_t p = up_default();
    sp_panel_t panel = sp_build(&p);
    TEST_ASSERT_EQUAL_STRING("Location", panel.sections[3].name);
}

/* ===== sp_build — option counts ===== */

void test_appearance_has_four_options(void) {
    up_prefs_t p = up_default();
    sp_panel_t panel = sp_build(&p);
    TEST_ASSERT_EQUAL_INT(4, panel.sections[0].option_count);
}

void test_time_has_two_options(void) {
    up_prefs_t p = up_default();
    sp_panel_t panel = sp_build(&p);
    TEST_ASSERT_EQUAL_INT(2, panel.sections[1].option_count);
}

void test_social_has_three_options(void) {
    up_prefs_t p = up_default();
    sp_panel_t panel = sp_build(&p);
    TEST_ASSERT_EQUAL_INT(3, panel.sections[2].option_count);
}

void test_location_has_one_option(void) {
    up_prefs_t p = up_default();
    sp_panel_t panel = sp_build(&p);
    TEST_ASSERT_EQUAL_INT(1, panel.sections[3].option_count);
}

/* ===== sp_build — Appearance options ===== */

void test_appearance_theme_label(void) {
    up_prefs_t p = up_default();
    sp_panel_t panel = sp_build(&p);
    TEST_ASSERT_EQUAL_STRING("Theme", panel.sections[0].options[0].label);
}

void test_appearance_theme_type_is_choice(void) {
    up_prefs_t p = up_default();
    sp_panel_t panel = sp_build(&p);
    TEST_ASSERT_EQUAL_INT(SP_OPT_CHOICE, panel.sections[0].options[0].type);
}

void test_appearance_theme_choices(void) {
    up_prefs_t p = up_default();
    sp_panel_t panel = sp_build(&p);
    sp_option_t *opt = &panel.sections[0].options[0];
    TEST_ASSERT_EQUAL_INT(2, opt->choice_count);
    TEST_ASSERT_EQUAL_STRING("Cosmos", opt->choices[0]);
    TEST_ASSERT_EQUAL_STRING("Dawn", opt->choices[1]);
}

void test_appearance_theme_default_cosmos(void) {
    up_prefs_t p = up_default();
    sp_panel_t panel = sp_build(&p);
    TEST_ASSERT_EQUAL_INT(0, panel.sections[0].options[0].choice_selected);
}

void test_appearance_font_scale_label(void) {
    up_prefs_t p = up_default();
    sp_panel_t panel = sp_build(&p);
    TEST_ASSERT_EQUAL_STRING("Font Scale", panel.sections[0].options[1].label);
}

void test_appearance_font_scale_type_is_slider(void) {
    up_prefs_t p = up_default();
    sp_panel_t panel = sp_build(&p);
    TEST_ASSERT_EQUAL_INT(SP_OPT_SLIDER, panel.sections[0].options[1].type);
}

void test_appearance_font_scale_range(void) {
    up_prefs_t p = up_default();
    sp_panel_t panel = sp_build(&p);
    sp_option_t *opt = &panel.sections[0].options[1];
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.5f, opt->slider_min);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 2.0f, opt->slider_max);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.1f, opt->slider_step);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, opt->slider_value);
}

void test_appearance_animation_speed_label(void) {
    up_prefs_t p = up_default();
    sp_panel_t panel = sp_build(&p);
    TEST_ASSERT_EQUAL_STRING("Animation Speed", panel.sections[0].options[2].label);
}

void test_appearance_opacity_label(void) {
    up_prefs_t p = up_default();
    sp_panel_t panel = sp_build(&p);
    TEST_ASSERT_EQUAL_STRING("Opacity", panel.sections[0].options[3].label);
}

void test_appearance_opacity_range(void) {
    up_prefs_t p = up_default();
    sp_panel_t panel = sp_build(&p);
    sp_option_t *opt = &panel.sections[0].options[3];
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.5f, opt->slider_min);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, opt->slider_max);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, opt->slider_step);
}

/* ===== sp_build — Time options ===== */

void test_time_24h_label(void) {
    up_prefs_t p = up_default();
    sp_panel_t panel = sp_build(&p);
    TEST_ASSERT_EQUAL_STRING("24-Hour Time", panel.sections[1].options[0].label);
}

void test_time_24h_type_is_toggle(void) {
    up_prefs_t p = up_default();
    sp_panel_t panel = sp_build(&p);
    TEST_ASSERT_EQUAL_INT(SP_OPT_TOGGLE, panel.sections[1].options[0].type);
}

void test_time_24h_default_off(void) {
    up_prefs_t p = up_default();
    sp_panel_t panel = sp_build(&p);
    TEST_ASSERT_EQUAL_INT(0, panel.sections[1].options[0].toggle_value);
}

void test_time_week_start_label(void) {
    up_prefs_t p = up_default();
    sp_panel_t panel = sp_build(&p);
    TEST_ASSERT_EQUAL_STRING("Week Starts On", panel.sections[1].options[1].label);
}

void test_time_week_start_choices(void) {
    up_prefs_t p = up_default();
    sp_panel_t panel = sp_build(&p);
    sp_option_t *opt = &panel.sections[1].options[1];
    TEST_ASSERT_EQUAL_INT(SP_OPT_CHOICE, opt->type);
    TEST_ASSERT_EQUAL_INT(2, opt->choice_count);
    TEST_ASSERT_EQUAL_STRING("Sunday", opt->choices[0]);
    TEST_ASSERT_EQUAL_STRING("Monday", opt->choices[1]);
}

/* ===== sp_build — Social options ===== */

void test_social_activity_label(void) {
    up_prefs_t p = up_default();
    sp_panel_t panel = sp_build(&p);
    TEST_ASSERT_EQUAL_STRING("Activity Level", panel.sections[2].options[0].label);
}

void test_social_activity_choices(void) {
    up_prefs_t p = up_default();
    sp_panel_t panel = sp_build(&p);
    sp_option_t *opt = &panel.sections[2].options[0];
    TEST_ASSERT_EQUAL_INT(SP_OPT_CHOICE, opt->type);
    TEST_ASSERT_EQUAL_INT(3, opt->choice_count);
    TEST_ASSERT_EQUAL_STRING("Introvert", opt->choices[0]);
    TEST_ASSERT_EQUAL_STRING("Balanced", opt->choices[1]);
    TEST_ASSERT_EQUAL_STRING("Extrovert", opt->choices[2]);
}

void test_social_notifications_choices(void) {
    up_prefs_t p = up_default();
    sp_panel_t panel = sp_build(&p);
    sp_option_t *opt = &panel.sections[2].options[1];
    TEST_ASSERT_EQUAL_STRING("Notifications", opt->label);
    TEST_ASSERT_EQUAL_INT(SP_OPT_CHOICE, opt->type);
    TEST_ASSERT_EQUAL_INT(4, opt->choice_count);
    TEST_ASSERT_EQUAL_STRING("Off", opt->choices[0]);
    TEST_ASSERT_EQUAL_STRING("Low", opt->choices[1]);
    TEST_ASSERT_EQUAL_STRING("Medium", opt->choices[2]);
    TEST_ASSERT_EQUAL_STRING("High", opt->choices[3]);
}

void test_social_roulette_label(void) {
    up_prefs_t p = up_default();
    sp_panel_t panel = sp_build(&p);
    TEST_ASSERT_EQUAL_STRING("Neighbor Roulette", panel.sections[2].options[2].label);
}

void test_social_roulette_type_is_toggle(void) {
    up_prefs_t p = up_default();
    sp_panel_t panel = sp_build(&p);
    TEST_ASSERT_EQUAL_INT(SP_OPT_TOGGLE, panel.sections[2].options[2].type);
}

/* ===== sp_build — Location options ===== */

void test_location_radius_label(void) {
    up_prefs_t p = up_default();
    sp_panel_t panel = sp_build(&p);
    TEST_ASSERT_EQUAL_STRING("Discovery Radius", panel.sections[3].options[0].label);
}

void test_location_radius_range(void) {
    up_prefs_t p = up_default();
    sp_panel_t panel = sp_build(&p);
    sp_option_t *opt = &panel.sections[3].options[0];
    TEST_ASSERT_EQUAL_INT(SP_OPT_SLIDER, opt->type);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, opt->slider_min);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 500.0f, opt->slider_max);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 10.0f, opt->slider_step);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 50.0f, opt->slider_value);
}

/* ===== sp_build — non-default prefs ===== */

void test_build_reflects_dawn_theme(void) {
    up_prefs_t p = up_default();
    p.visual.theme = THEME_DAWN;
    sp_panel_t panel = sp_build(&p);
    TEST_ASSERT_EQUAL_INT(1, panel.sections[0].options[0].choice_selected);
}

void test_build_reflects_24h_on(void) {
    up_prefs_t p = up_default();
    p.temporal.use_24h_time = 1;
    sp_panel_t panel = sp_build(&p);
    TEST_ASSERT_EQUAL_INT(1, panel.sections[1].options[0].toggle_value);
}

/* ===== sp_apply_toggle ===== */

void test_apply_toggle_24h(void) {
    up_prefs_t p = up_default();
    up_prefs_t updated = sp_apply_toggle(&p, 1, 0, 1);
    TEST_ASSERT_EQUAL_INT(1, updated.temporal.use_24h_time);
}

void test_apply_toggle_roulette(void) {
    up_prefs_t p = up_default();
    up_prefs_t updated = sp_apply_toggle(&p, 2, 2, 1);
    TEST_ASSERT_EQUAL_INT(1, updated.social.roulette_opt_in);
}

void test_apply_toggle_invalid_section(void) {
    up_prefs_t p = up_default();
    up_prefs_t updated = sp_apply_toggle(&p, 99, 0, 1);
    TEST_ASSERT_EQUAL_INT(0, up_diff(&p, &updated));
}

void test_apply_toggle_invalid_option(void) {
    up_prefs_t p = up_default();
    up_prefs_t updated = sp_apply_toggle(&p, 1, 99, 1);
    TEST_ASSERT_EQUAL_INT(0, up_diff(&p, &updated));
}

/* ===== sp_apply_choice ===== */

void test_apply_choice_theme_dawn(void) {
    up_prefs_t p = up_default();
    up_prefs_t updated = sp_apply_choice(&p, 0, 0, 1);
    TEST_ASSERT_EQUAL_INT(THEME_DAWN, updated.visual.theme);
}

void test_apply_choice_week_start_monday(void) {
    up_prefs_t p = up_default();
    up_prefs_t updated = sp_apply_choice(&p, 1, 1, 1);
    TEST_ASSERT_EQUAL_INT(1, updated.temporal.week_start_day);
}

void test_apply_choice_activity_extrovert(void) {
    up_prefs_t p = up_default();
    up_prefs_t updated = sp_apply_choice(&p, 2, 0, 2);
    TEST_ASSERT_EQUAL_INT(IP_ACT_EXTROVERT, updated.social.activity_level);
}

void test_apply_choice_notifications_high(void) {
    up_prefs_t p = up_default();
    up_prefs_t updated = sp_apply_choice(&p, 2, 1, 3);
    TEST_ASSERT_EQUAL_INT(3, updated.social.notification_frequency);
}

void test_apply_choice_invalid_section(void) {
    up_prefs_t p = up_default();
    up_prefs_t updated = sp_apply_choice(&p, -1, 0, 1);
    TEST_ASSERT_EQUAL_INT(0, up_diff(&p, &updated));
}

/* ===== sp_apply_slider ===== */

void test_apply_slider_font_scale(void) {
    up_prefs_t p = up_default();
    up_prefs_t updated = sp_apply_slider(&p, 0, 1, 1.5f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.5f, updated.visual.font_scale);
}

void test_apply_slider_opacity(void) {
    up_prefs_t p = up_default();
    up_prefs_t updated = sp_apply_slider(&p, 0, 3, 0.75f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.75f, updated.visual.opacity_scale);
}

void test_apply_slider_discovery_radius(void) {
    up_prefs_t p = up_default();
    up_prefs_t updated = sp_apply_slider(&p, 3, 0, 200.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.1, 200.0, (float)updated.location.exploration_radius_km);
}

void test_apply_slider_clamps_above_max(void) {
    up_prefs_t p = up_default();
    up_prefs_t updated = sp_apply_slider(&p, 0, 1, 999.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 2.0f, updated.visual.font_scale);
}

void test_apply_slider_clamps_below_min(void) {
    up_prefs_t p = up_default();
    up_prefs_t updated = sp_apply_slider(&p, 0, 1, -5.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.5f, updated.visual.font_scale);
}

void test_apply_slider_invalid_section(void) {
    up_prefs_t p = up_default();
    up_prefs_t updated = sp_apply_slider(&p, 99, 0, 1.0f);
    TEST_ASSERT_EQUAL_INT(0, up_diff(&p, &updated));
}

/* ===== sp_section_count / sp_option_count ===== */

void test_section_count(void) {
    up_prefs_t p = up_default();
    sp_panel_t panel = sp_build(&p);
    TEST_ASSERT_EQUAL_INT(4, sp_section_count(&panel));
}

void test_option_count_appearance(void) {
    up_prefs_t p = up_default();
    sp_panel_t panel = sp_build(&p);
    TEST_ASSERT_EQUAL_INT(4, sp_option_count(&panel, 0));
}

void test_option_count_invalid_section(void) {
    up_prefs_t p = up_default();
    sp_panel_t panel = sp_build(&p);
    TEST_ASSERT_EQUAL_INT(0, sp_option_count(&panel, 99));
}

/* ===== sp_option_label ===== */

void test_option_label_theme(void) {
    up_prefs_t p = up_default();
    sp_panel_t panel = sp_build(&p);
    TEST_ASSERT_EQUAL_STRING("Theme", sp_option_label(&panel, 0, 0));
}

void test_option_label_invalid_section(void) {
    up_prefs_t p = up_default();
    sp_panel_t panel = sp_build(&p);
    TEST_ASSERT_NULL(sp_option_label(&panel, 99, 0));
}

void test_option_label_invalid_option(void) {
    up_prefs_t p = up_default();
    sp_panel_t panel = sp_build(&p);
    TEST_ASSERT_NULL(sp_option_label(&panel, 0, 99));
}

/* ===== Collapsible Sections ===== */

void test_build_all_expanded_by_default(void) {
    up_prefs_t p = up_default();
    sp_panel_t panel = sp_build(&p);
    TEST_ASSERT_EQUAL_UINT32(0, panel.collapsed_mask);
}

void test_toggle_section_collapses(void) {
    up_prefs_t p = up_default();
    sp_panel_t panel = sp_build(&p);
    panel = sp_toggle_section(panel, 0);
    TEST_ASSERT_EQUAL_INT(1, sp_is_collapsed(&panel, 0));
}

void test_toggle_section_twice_expands(void) {
    up_prefs_t p = up_default();
    sp_panel_t panel = sp_build(&p);
    panel = sp_toggle_section(panel, 1);
    panel = sp_toggle_section(panel, 1);
    TEST_ASSERT_EQUAL_INT(0, sp_is_collapsed(&panel, 1));
}

void test_is_collapsed_false_by_default(void) {
    up_prefs_t p = up_default();
    sp_panel_t panel = sp_build(&p);
    for (int i = 0; i < panel.section_count; i++) {
        TEST_ASSERT_EQUAL_INT(0, sp_is_collapsed(&panel, i));
    }
}

void test_is_collapsed_after_toggle(void) {
    up_prefs_t p = up_default();
    sp_panel_t panel = sp_build(&p);
    panel = sp_toggle_section(panel, 2);
    TEST_ASSERT_EQUAL_INT(0, sp_is_collapsed(&panel, 0));
    TEST_ASSERT_EQUAL_INT(0, sp_is_collapsed(&panel, 1));
    TEST_ASSERT_EQUAL_INT(1, sp_is_collapsed(&panel, 2));
    TEST_ASSERT_EQUAL_INT(0, sp_is_collapsed(&panel, 3));
}

void test_is_collapsed_invalid_section(void) {
    up_prefs_t p = up_default();
    sp_panel_t panel = sp_build(&p);
    TEST_ASSERT_EQUAL_INT(0, sp_is_collapsed(&panel, -1));
    TEST_ASSERT_EQUAL_INT(0, sp_is_collapsed(&panel, 99));
}

void test_collapse_mask_default_zero(void) {
    up_prefs_t p = up_default();
    sp_panel_t panel = sp_build(&p);
    TEST_ASSERT_EQUAL_UINT32(0, sp_collapse_mask(&panel));
}

void test_collapse_mask_after_toggles(void) {
    up_prefs_t p = up_default();
    sp_panel_t panel = sp_build(&p);
    panel = sp_toggle_section(panel, 0);
    panel = sp_toggle_section(panel, 2);
    /* bits 0 and 2 set = 0b0101 = 5 */
    TEST_ASSERT_EQUAL_UINT32(5, sp_collapse_mask(&panel));
}

void test_set_collapse_mask(void) {
    up_prefs_t p = up_default();
    sp_panel_t panel = sp_build(&p);
    panel = sp_set_collapse_mask(panel, 0x0A); /* bits 1 and 3 */
    TEST_ASSERT_EQUAL_INT(0, sp_is_collapsed(&panel, 0));
    TEST_ASSERT_EQUAL_INT(1, sp_is_collapsed(&panel, 1));
    TEST_ASSERT_EQUAL_INT(0, sp_is_collapsed(&panel, 2));
    TEST_ASSERT_EQUAL_INT(1, sp_is_collapsed(&panel, 3));
}

void test_set_collapse_mask_roundtrip(void) {
    up_prefs_t p = up_default();
    sp_panel_t panel = sp_build(&p);
    panel = sp_toggle_section(panel, 1);
    panel = sp_toggle_section(panel, 3);
    uint32_t mask = sp_collapse_mask(&panel);

    /* Rebuild panel and restore mask */
    sp_panel_t panel2 = sp_build(&p);
    panel2 = sp_set_collapse_mask(panel2, mask);
    TEST_ASSERT_EQUAL_INT(0, sp_is_collapsed(&panel2, 0));
    TEST_ASSERT_EQUAL_INT(1, sp_is_collapsed(&panel2, 1));
    TEST_ASSERT_EQUAL_INT(0, sp_is_collapsed(&panel2, 2));
    TEST_ASSERT_EQUAL_INT(1, sp_is_collapsed(&panel2, 3));
}

void test_expanded_count_all_expanded(void) {
    up_prefs_t p = up_default();
    sp_panel_t panel = sp_build(&p);
    TEST_ASSERT_EQUAL_INT(4, sp_expanded_count(&panel));
}

void test_expanded_count_one_collapsed(void) {
    up_prefs_t p = up_default();
    sp_panel_t panel = sp_build(&p);
    panel = sp_toggle_section(panel, 2);
    TEST_ASSERT_EQUAL_INT(3, sp_expanded_count(&panel));
}

void test_expanded_count_all_collapsed(void) {
    up_prefs_t p = up_default();
    sp_panel_t panel = sp_build(&p);
    for (int i = 0; i < panel.section_count; i++) {
        panel = sp_toggle_section(panel, i);
    }
    TEST_ASSERT_EQUAL_INT(0, sp_expanded_count(&panel));
}

void test_toggle_invalid_section_no_change(void) {
    up_prefs_t p = up_default();
    sp_panel_t panel = sp_build(&p);
    uint32_t before = sp_collapse_mask(&panel);
    panel = sp_toggle_section(panel, -1);
    TEST_ASSERT_EQUAL_UINT32(before, sp_collapse_mask(&panel));
    panel = sp_toggle_section(panel, 99);
    TEST_ASSERT_EQUAL_UINT32(before, sp_collapse_mask(&panel));
}

/* ===== main ===== */

int main(void) {
    UNITY_BEGIN();

    /* sp_build — sections */
    RUN_TEST(test_build_returns_four_sections);
    RUN_TEST(test_build_section_0_is_appearance);
    RUN_TEST(test_build_section_1_is_time);
    RUN_TEST(test_build_section_2_is_social);
    RUN_TEST(test_build_section_3_is_location);

    /* sp_build — option counts */
    RUN_TEST(test_appearance_has_four_options);
    RUN_TEST(test_time_has_two_options);
    RUN_TEST(test_social_has_three_options);
    RUN_TEST(test_location_has_one_option);

    /* sp_build — Appearance options */
    RUN_TEST(test_appearance_theme_label);
    RUN_TEST(test_appearance_theme_type_is_choice);
    RUN_TEST(test_appearance_theme_choices);
    RUN_TEST(test_appearance_theme_default_cosmos);
    RUN_TEST(test_appearance_font_scale_label);
    RUN_TEST(test_appearance_font_scale_type_is_slider);
    RUN_TEST(test_appearance_font_scale_range);
    RUN_TEST(test_appearance_animation_speed_label);
    RUN_TEST(test_appearance_opacity_label);
    RUN_TEST(test_appearance_opacity_range);

    /* sp_build — Time options */
    RUN_TEST(test_time_24h_label);
    RUN_TEST(test_time_24h_type_is_toggle);
    RUN_TEST(test_time_24h_default_off);
    RUN_TEST(test_time_week_start_label);
    RUN_TEST(test_time_week_start_choices);

    /* sp_build — Social options */
    RUN_TEST(test_social_activity_label);
    RUN_TEST(test_social_activity_choices);
    RUN_TEST(test_social_notifications_choices);
    RUN_TEST(test_social_roulette_label);
    RUN_TEST(test_social_roulette_type_is_toggle);

    /* sp_build — Location options */
    RUN_TEST(test_location_radius_label);
    RUN_TEST(test_location_radius_range);

    /* sp_build — non-default prefs */
    RUN_TEST(test_build_reflects_dawn_theme);
    RUN_TEST(test_build_reflects_24h_on);

    /* sp_apply_toggle */
    RUN_TEST(test_apply_toggle_24h);
    RUN_TEST(test_apply_toggle_roulette);
    RUN_TEST(test_apply_toggle_invalid_section);
    RUN_TEST(test_apply_toggle_invalid_option);

    /* sp_apply_choice */
    RUN_TEST(test_apply_choice_theme_dawn);
    RUN_TEST(test_apply_choice_week_start_monday);
    RUN_TEST(test_apply_choice_activity_extrovert);
    RUN_TEST(test_apply_choice_notifications_high);
    RUN_TEST(test_apply_choice_invalid_section);

    /* sp_apply_slider */
    RUN_TEST(test_apply_slider_font_scale);
    RUN_TEST(test_apply_slider_opacity);
    RUN_TEST(test_apply_slider_discovery_radius);
    RUN_TEST(test_apply_slider_clamps_above_max);
    RUN_TEST(test_apply_slider_clamps_below_min);
    RUN_TEST(test_apply_slider_invalid_section);

    /* sp_section_count / sp_option_count */
    RUN_TEST(test_section_count);
    RUN_TEST(test_option_count_appearance);
    RUN_TEST(test_option_count_invalid_section);

    /* sp_option_label */
    RUN_TEST(test_option_label_theme);
    RUN_TEST(test_option_label_invalid_section);
    RUN_TEST(test_option_label_invalid_option);

    /* Collapsible Sections (14) */
    RUN_TEST(test_build_all_expanded_by_default);
    RUN_TEST(test_toggle_section_collapses);
    RUN_TEST(test_toggle_section_twice_expands);
    RUN_TEST(test_is_collapsed_false_by_default);
    RUN_TEST(test_is_collapsed_after_toggle);
    RUN_TEST(test_is_collapsed_invalid_section);
    RUN_TEST(test_collapse_mask_default_zero);
    RUN_TEST(test_collapse_mask_after_toggles);
    RUN_TEST(test_set_collapse_mask);
    RUN_TEST(test_set_collapse_mask_roundtrip);
    RUN_TEST(test_expanded_count_all_expanded);
    RUN_TEST(test_expanded_count_one_collapsed);
    RUN_TEST(test_expanded_count_all_collapsed);
    RUN_TEST(test_toggle_invalid_section_no_change);

    return UNITY_END();
}
