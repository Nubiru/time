#include "../../tests/unity/unity.h"
#include "../../src/ui/view_adapt.h"

#include <string.h>

#define EPSILON 0.01f

void setUp(void) {}
void tearDown(void) {}

/* ================================================================
 * TIER FROM WIDTH — boundary values
 * ================================================================ */

void test_tier_from_width_zero(void) {
    TEST_ASSERT_EQUAL_INT(VA_PHONE_PORT, va_tier_from_width(0));
}

void test_tier_from_width_one(void) {
    TEST_ASSERT_EQUAL_INT(VA_PHONE_PORT, va_tier_from_width(1));
}

void test_tier_from_width_480(void) {
    TEST_ASSERT_EQUAL_INT(VA_PHONE_PORT, va_tier_from_width(480));
}

void test_tier_from_width_481(void) {
    TEST_ASSERT_EQUAL_INT(VA_PHONE_LAND, va_tier_from_width(481));
}

void test_tier_from_width_767(void) {
    TEST_ASSERT_EQUAL_INT(VA_PHONE_LAND, va_tier_from_width(767));
}

void test_tier_from_width_768(void) {
    TEST_ASSERT_EQUAL_INT(VA_TABLET, va_tier_from_width(768));
}

void test_tier_from_width_1024(void) {
    TEST_ASSERT_EQUAL_INT(VA_TABLET, va_tier_from_width(1024));
}

void test_tier_from_width_1025(void) {
    TEST_ASSERT_EQUAL_INT(VA_DESKTOP, va_tier_from_width(1025));
}

void test_tier_from_width_10000(void) {
    TEST_ASSERT_EQUAL_INT(VA_DESKTOP, va_tier_from_width(10000));
}

/* ================================================================
 * TIER NAMES
 * ================================================================ */

void test_tier_name_desktop(void) {
    TEST_ASSERT_EQUAL_STRING("Desktop", va_tier_name(VA_DESKTOP));
}

void test_tier_name_tablet(void) {
    TEST_ASSERT_EQUAL_STRING("Tablet", va_tier_name(VA_TABLET));
}

void test_tier_name_phone_land(void) {
    TEST_ASSERT_EQUAL_STRING("Phone Landscape", va_tier_name(VA_PHONE_LAND));
}

void test_tier_name_phone_port(void) {
    TEST_ASSERT_EQUAL_STRING("Phone Portrait", va_tier_name(VA_PHONE_PORT));
}

void test_tier_name_invalid(void) {
    TEST_ASSERT_EQUAL_STRING("?", va_tier_name(VA_TIER_COUNT));
    TEST_ASSERT_EQUAL_STRING("?", va_tier_name((va_screen_tier_t)99));
}

/* ================================================================
 * LAYOUT NAMES
 * ================================================================ */

void test_layout_name_card(void) {
    TEST_ASSERT_EQUAL_STRING("Card", va_layout_name(VA_LAYOUT_CARD));
}

void test_layout_name_sheet(void) {
    TEST_ASSERT_EQUAL_STRING("Sheet", va_layout_name(VA_LAYOUT_SHEET));
}

void test_layout_name_fullscreen(void) {
    TEST_ASSERT_EQUAL_STRING("Fullscreen", va_layout_name(VA_LAYOUT_FULLSCREEN));
}

void test_layout_name_hidden(void) {
    TEST_ASSERT_EQUAL_STRING("Hidden", va_layout_name(VA_LAYOUT_HIDDEN));
}

void test_layout_name_invalid(void) {
    TEST_ASSERT_EQUAL_STRING("?", va_layout_name((va_layout_mode_t)99));
}

/* ================================================================
 * TIER COUNT AND BREAKPOINTS
 * ================================================================ */

void test_tier_count_is_4(void) {
    TEST_ASSERT_EQUAL_INT(4, va_tier_count());
}

void test_tier_breakpoint_desktop(void) {
    /* Desktop has no upper bound; expect a large sentinel */
    TEST_ASSERT_TRUE(va_tier_breakpoint(VA_DESKTOP) > 1024);
}

void test_tier_breakpoint_tablet(void) {
    TEST_ASSERT_EQUAL_INT(1024, va_tier_breakpoint(VA_TABLET));
}

void test_tier_breakpoint_phone_land(void) {
    TEST_ASSERT_EQUAL_INT(767, va_tier_breakpoint(VA_PHONE_LAND));
}

void test_tier_breakpoint_phone_port(void) {
    TEST_ASSERT_EQUAL_INT(480, va_tier_breakpoint(VA_PHONE_PORT));
}

/* ================================================================
 * DESKTOP TIER — all views share defaults
 * ================================================================ */

void test_desktop_space_panels_all_card(void) {
    va_adapt_t c = va_get(0, VA_DESKTOP);
    TEST_ASSERT_EQUAL_INT(VA_LAYOUT_CARD, c.help_layout);
    TEST_ASSERT_EQUAL_INT(VA_LAYOUT_CARD, c.layer_layout);
    TEST_ASSERT_EQUAL_INT(VA_LAYOUT_CARD, c.card_layout);
    TEST_ASSERT_EQUAL_INT(VA_LAYOUT_CARD, c.palette_layout);
}

void test_desktop_space_visibility(void) {
    va_adapt_t c = va_get(0, VA_DESKTOP);
    TEST_ASSERT_EQUAL_INT(1, c.show_hud);
    TEST_ASSERT_EQUAL_INT(0, c.show_time_bar);  /* edge proximity only */
    TEST_ASSERT_EQUAL_INT(1, c.show_corner_help);
    TEST_ASSERT_EQUAL_INT(1, c.show_labels);
}

void test_desktop_space_input(void) {
    va_adapt_t c = va_get(0, VA_DESKTOP);
    TEST_ASSERT_EQUAL_INT(0, c.touch_priority);  /* mouse first */
    TEST_ASSERT_EQUAL_INT(34, c.min_tap_target);
}

void test_desktop_space_scales(void) {
    va_adapt_t c = va_get(0, VA_DESKTOP);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, c.font_scale);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, c.hud_font_scale);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, c.spacing_scale);
}

void test_desktop_personal_same_as_space(void) {
    /* Desktop: no exceptions — all views identical */
    va_adapt_t space = va_get(0, VA_DESKTOP);
    va_adapt_t personal = va_get(3, VA_DESKTOP);
    TEST_ASSERT_EQUAL_INT(space.help_layout, personal.help_layout);
    TEST_ASSERT_EQUAL_INT(space.card_layout, personal.card_layout);
    TEST_ASSERT_EQUAL_INT(space.touch_priority, personal.touch_priority);
}

/* ================================================================
 * TABLET TIER
 * ================================================================ */

void test_tablet_space_panels_sheet(void) {
    va_adapt_t c = va_get(0, VA_TABLET);
    TEST_ASSERT_EQUAL_INT(VA_LAYOUT_SHEET, c.help_layout);
    TEST_ASSERT_EQUAL_INT(VA_LAYOUT_SHEET, c.layer_layout);
    TEST_ASSERT_EQUAL_INT(VA_LAYOUT_SHEET, c.card_layout);
    TEST_ASSERT_EQUAL_INT(VA_LAYOUT_SHEET, c.palette_layout);
}

void test_tablet_touch_first(void) {
    va_adapt_t c = va_get(0, VA_TABLET);
    TEST_ASSERT_EQUAL_INT(1, c.touch_priority);
    TEST_ASSERT_EQUAL_INT(44, c.min_tap_target);
}

void test_tablet_time_bar_always(void) {
    va_adapt_t c = va_get(0, VA_TABLET);
    TEST_ASSERT_EQUAL_INT(1, c.show_time_bar);
}

void test_tablet_personal_card_sheet(void) {
    /* Exception: Personal on tablet — card is sheet (primary content) */
    va_adapt_t c = va_get(3, VA_TABLET);
    TEST_ASSERT_EQUAL_INT(VA_LAYOUT_SHEET, c.card_layout);
}

void test_tablet_room_card_sheet(void) {
    /* Exception: Room on tablet — card is sheet (primary content) */
    va_adapt_t c = va_get(6, VA_TABLET);
    TEST_ASSERT_EQUAL_INT(VA_LAYOUT_SHEET, c.card_layout);
}

/* ================================================================
 * PHONE LANDSCAPE TIER
 * ================================================================ */

void test_phone_land_touch_only(void) {
    va_adapt_t c = va_get(0, VA_PHONE_LAND);
    TEST_ASSERT_EQUAL_INT(2, c.touch_priority);
    TEST_ASSERT_EQUAL_INT(44, c.min_tap_target);
}

void test_phone_land_condensed_hud(void) {
    va_adapt_t c = va_get(0, VA_PHONE_LAND);
    TEST_ASSERT_EQUAL_INT(0, c.show_hud);
}

void test_phone_land_no_labels(void) {
    va_adapt_t c = va_get(0, VA_PHONE_LAND);
    TEST_ASSERT_EQUAL_INT(0, c.show_labels);
}

void test_phone_land_font_scales(void) {
    va_adapt_t c = va_get(0, VA_PHONE_LAND);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.9f, c.font_scale);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.85f, c.hud_font_scale);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.85f, c.spacing_scale);
}

void test_phone_land_personal_card_fullscreen(void) {
    /* Exception: Personal on phone landscape — card fullscreen */
    va_adapt_t c = va_get(3, VA_PHONE_LAND);
    TEST_ASSERT_EQUAL_INT(VA_LAYOUT_FULLSCREEN, c.card_layout);
}

void test_phone_land_room_card_fullscreen(void) {
    /* Exception: Room on phone landscape — card fullscreen */
    va_adapt_t c = va_get(6, VA_PHONE_LAND);
    TEST_ASSERT_EQUAL_INT(VA_LAYOUT_FULLSCREEN, c.card_layout);
}

void test_phone_land_space_card_sheet(void) {
    /* Non-exception: Space on phone landscape — card stays sheet */
    va_adapt_t c = va_get(0, VA_PHONE_LAND);
    TEST_ASSERT_EQUAL_INT(VA_LAYOUT_SHEET, c.card_layout);
}

/* ================================================================
 * PHONE PORTRAIT TIER
 * ================================================================ */

void test_phone_port_touch_only(void) {
    va_adapt_t c = va_get(0, VA_PHONE_PORT);
    TEST_ASSERT_EQUAL_INT(2, c.touch_priority);
    TEST_ASSERT_EQUAL_INT(48, c.min_tap_target);
}

void test_phone_port_font_scales(void) {
    va_adapt_t c = va_get(0, VA_PHONE_PORT);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.85f, c.font_scale);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.75f, c.hud_font_scale);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.75f, c.spacing_scale);
}

void test_phone_port_help_fullscreen(void) {
    va_adapt_t c = va_get(0, VA_PHONE_PORT);
    TEST_ASSERT_EQUAL_INT(VA_LAYOUT_FULLSCREEN, c.help_layout);
}

void test_phone_port_layer_sheet(void) {
    va_adapt_t c = va_get(0, VA_PHONE_PORT);
    TEST_ASSERT_EQUAL_INT(VA_LAYOUT_SHEET, c.layer_layout);
}

void test_phone_port_palette_fullscreen(void) {
    va_adapt_t c = va_get(0, VA_PHONE_PORT);
    TEST_ASSERT_EQUAL_INT(VA_LAYOUT_FULLSCREEN, c.palette_layout);
}

void test_phone_port_space_card_fullscreen(void) {
    /* Phone portrait: Space/Galaxy/Earth card = fullscreen */
    va_adapt_t c = va_get(0, VA_PHONE_PORT);
    TEST_ASSERT_EQUAL_INT(VA_LAYOUT_FULLSCREEN, c.card_layout);
}

void test_phone_port_galaxy_card_fullscreen(void) {
    va_adapt_t c = va_get(2, VA_PHONE_PORT);
    TEST_ASSERT_EQUAL_INT(VA_LAYOUT_FULLSCREEN, c.card_layout);
}

void test_phone_port_earth_card_fullscreen(void) {
    va_adapt_t c = va_get(1, VA_PHONE_PORT);
    TEST_ASSERT_EQUAL_INT(VA_LAYOUT_FULLSCREEN, c.card_layout);
}

/* ================================================================
 * va_get — VIEW_SPACE across all 4 tiers
 * ================================================================ */

void test_space_all_tiers_view_id(void) {
    for (int t = 0; t < VA_TIER_COUNT; t++) {
        va_adapt_t c = va_get(0, (va_screen_tier_t)t);
        TEST_ASSERT_EQUAL_INT(0, c.view_id);
        TEST_ASSERT_EQUAL_INT(t, (int)c.tier);
    }
}

/* ================================================================
 * va_get — VIEW_PERSONAL across all 4 tiers (card-dominant)
 * ================================================================ */

void test_personal_all_tiers_view_id(void) {
    for (int t = 0; t < VA_TIER_COUNT; t++) {
        va_adapt_t c = va_get(3, (va_screen_tier_t)t);
        TEST_ASSERT_EQUAL_INT(3, c.view_id);
    }
}

/* ================================================================
 * OUT OF RANGE
 * ================================================================ */

void test_get_negative_view_returns_default(void) {
    va_adapt_t c = va_get(-1, VA_DESKTOP);
    TEST_ASSERT_EQUAL_INT(0, c.view_id);  /* defaults to Space */
    TEST_ASSERT_EQUAL_INT(VA_DESKTOP, c.tier);
}

void test_get_view_too_large_returns_default(void) {
    va_adapt_t c = va_get(7, VA_DESKTOP);
    TEST_ASSERT_EQUAL_INT(0, c.view_id);
}

void test_get_view_99_returns_default(void) {
    va_adapt_t c = va_get(99, VA_TABLET);
    TEST_ASSERT_EQUAL_INT(0, c.view_id);
    TEST_ASSERT_EQUAL_INT(VA_TABLET, c.tier);
}

void test_get_tier_out_of_range_clamps_desktop(void) {
    va_adapt_t c = va_get(0, (va_screen_tier_t)99);
    TEST_ASSERT_EQUAL_INT(VA_DESKTOP, c.tier);
}

void test_get_tier_negative_clamps_desktop(void) {
    va_adapt_t c = va_get(0, (va_screen_tier_t)-1);
    TEST_ASSERT_EQUAL_INT(VA_DESKTOP, c.tier);
}

/* ================================================================
 * va_get_for_width — convenience wrapper
 * ================================================================ */

void test_get_for_width_matches_manual(void) {
    va_adapt_t from_width = va_get_for_width(0, 800);
    va_screen_tier_t tier = va_tier_from_width(800);
    va_adapt_t from_tier = va_get(0, tier);
    TEST_ASSERT_EQUAL_INT(from_tier.view_id, from_width.view_id);
    TEST_ASSERT_EQUAL_INT(from_tier.tier, from_width.tier);
    TEST_ASSERT_EQUAL_INT(from_tier.touch_priority, from_width.touch_priority);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, from_tier.font_scale, from_width.font_scale);
}

void test_get_for_width_480(void) {
    va_adapt_t c = va_get_for_width(0, 480);
    TEST_ASSERT_EQUAL_INT(VA_PHONE_PORT, c.tier);
}

void test_get_for_width_1920(void) {
    va_adapt_t c = va_get_for_width(2, 1920);
    TEST_ASSERT_EQUAL_INT(VA_DESKTOP, c.tier);
}

/* ================================================================
 * PANEL AVAILABLE
 * ================================================================ */

void test_panel_available_card_layout(void) {
    va_adapt_t c = va_get(0, VA_DESKTOP);
    /* All panels CARD on desktop → all available */
    TEST_ASSERT_EQUAL_INT(1, va_panel_available(c, 0)); /* help */
    TEST_ASSERT_EQUAL_INT(1, va_panel_available(c, 1)); /* layer */
    TEST_ASSERT_EQUAL_INT(1, va_panel_available(c, 2)); /* card */
    TEST_ASSERT_EQUAL_INT(1, va_panel_available(c, 3)); /* palette */
}

void test_panel_available_hidden_returns_0(void) {
    /* Construct a config with hidden help */
    va_adapt_t c = va_get(0, VA_DESKTOP);
    c.help_layout = VA_LAYOUT_HIDDEN;
    TEST_ASSERT_EQUAL_INT(0, va_panel_available(c, 0));
}

void test_panel_available_invalid_type_returns_0(void) {
    va_adapt_t c = va_get(0, VA_DESKTOP);
    TEST_ASSERT_EQUAL_INT(0, va_panel_available(c, -1));
    TEST_ASSERT_EQUAL_INT(0, va_panel_available(c, 4));
}

/* ================================================================
 * PROGRESSIVE DEGRADATION — scales decrease with smaller tiers
 * ================================================================ */

void test_font_scale_decreases(void) {
    float desktop = va_get(0, VA_DESKTOP).font_scale;
    float tablet  = va_get(0, VA_TABLET).font_scale;
    float p_land  = va_get(0, VA_PHONE_LAND).font_scale;
    float p_port  = va_get(0, VA_PHONE_PORT).font_scale;
    TEST_ASSERT_TRUE(desktop >= tablet);
    TEST_ASSERT_TRUE(tablet >= p_land);
    TEST_ASSERT_TRUE(p_land >= p_port);
}

void test_hud_font_scale_decreases(void) {
    float desktop = va_get(0, VA_DESKTOP).hud_font_scale;
    float tablet  = va_get(0, VA_TABLET).hud_font_scale;
    float p_land  = va_get(0, VA_PHONE_LAND).hud_font_scale;
    float p_port  = va_get(0, VA_PHONE_PORT).hud_font_scale;
    TEST_ASSERT_TRUE(desktop >= tablet);
    TEST_ASSERT_TRUE(tablet >= p_land);
    TEST_ASSERT_TRUE(p_land >= p_port);
}

void test_spacing_scale_decreases(void) {
    float desktop = va_get(0, VA_DESKTOP).spacing_scale;
    float tablet  = va_get(0, VA_TABLET).spacing_scale;
    float p_land  = va_get(0, VA_PHONE_LAND).spacing_scale;
    float p_port  = va_get(0, VA_PHONE_PORT).spacing_scale;
    TEST_ASSERT_TRUE(desktop >= tablet);
    TEST_ASSERT_TRUE(tablet >= p_land);
    TEST_ASSERT_TRUE(p_land >= p_port);
}

void test_touch_priority_increases(void) {
    int desktop = va_get(0, VA_DESKTOP).touch_priority;
    int tablet  = va_get(0, VA_TABLET).touch_priority;
    int p_land  = va_get(0, VA_PHONE_LAND).touch_priority;
    int p_port  = va_get(0, VA_PHONE_PORT).touch_priority;
    TEST_ASSERT_TRUE(desktop <= tablet);
    TEST_ASSERT_TRUE(tablet <= p_land);
    TEST_ASSERT_TRUE(p_land <= p_port);
}

void test_min_tap_target_increases(void) {
    int desktop = va_get(0, VA_DESKTOP).min_tap_target;
    int tablet  = va_get(0, VA_TABLET).min_tap_target;
    int p_land  = va_get(0, VA_PHONE_LAND).min_tap_target;
    int p_port  = va_get(0, VA_PHONE_PORT).min_tap_target;
    TEST_ASSERT_TRUE(desktop <= tablet);
    TEST_ASSERT_TRUE(tablet <= p_land);
    TEST_ASSERT_TRUE(p_land <= p_port);
}

/* ================================================================
 * PURITY — same input, same output
 * ================================================================ */

void test_purity_same_result(void) {
    va_adapt_t a = va_get(2, VA_TABLET);
    va_adapt_t b = va_get(2, VA_TABLET);
    TEST_ASSERT_EQUAL_INT(a.view_id, b.view_id);
    TEST_ASSERT_EQUAL_INT(a.tier, b.tier);
    TEST_ASSERT_EQUAL_INT(a.touch_priority, b.touch_priority);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, a.font_scale, b.font_scale);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, a.spacing_scale, b.spacing_scale);
}

void test_purity_tier_from_width(void) {
    va_screen_tier_t a = va_tier_from_width(500);
    va_screen_tier_t b = va_tier_from_width(500);
    TEST_ASSERT_EQUAL_INT(a, b);
}

/* ================================================================
 * ALL VIEWS x ALL TIERS — structural integrity
 * ================================================================ */

void test_all_configs_have_valid_view_and_tier(void) {
    for (int v = 0; v < 7; v++) {
        for (int t = 0; t < VA_TIER_COUNT; t++) {
            va_adapt_t c = va_get(v, (va_screen_tier_t)t);
            TEST_ASSERT_EQUAL_INT(v, c.view_id);
            TEST_ASSERT_EQUAL_INT(t, (int)c.tier);
        }
    }
}

void test_all_configs_positive_font_scale(void) {
    for (int v = 0; v < 7; v++) {
        for (int t = 0; t < VA_TIER_COUNT; t++) {
            va_adapt_t c = va_get(v, (va_screen_tier_t)t);
            TEST_ASSERT_TRUE(c.font_scale > 0.0f);
            TEST_ASSERT_TRUE(c.hud_font_scale > 0.0f);
            TEST_ASSERT_TRUE(c.spacing_scale > 0.0f);
        }
    }
}

void test_all_configs_valid_touch_priority(void) {
    for (int v = 0; v < 7; v++) {
        for (int t = 0; t < VA_TIER_COUNT; t++) {
            va_adapt_t c = va_get(v, (va_screen_tier_t)t);
            TEST_ASSERT_TRUE(c.touch_priority >= 0);
            TEST_ASSERT_TRUE(c.touch_priority <= 2);
        }
    }
}

void test_all_configs_positive_tap_target(void) {
    for (int v = 0; v < 7; v++) {
        for (int t = 0; t < VA_TIER_COUNT; t++) {
            va_adapt_t c = va_get(v, (va_screen_tier_t)t);
            TEST_ASSERT_TRUE(c.min_tap_target > 0);
        }
    }
}

/* ================================================================
 * MAIN
 * ================================================================ */

int main(void) {
    UNITY_BEGIN();

    /* Tier from width — boundary values */
    RUN_TEST(test_tier_from_width_zero);
    RUN_TEST(test_tier_from_width_one);
    RUN_TEST(test_tier_from_width_480);
    RUN_TEST(test_tier_from_width_481);
    RUN_TEST(test_tier_from_width_767);
    RUN_TEST(test_tier_from_width_768);
    RUN_TEST(test_tier_from_width_1024);
    RUN_TEST(test_tier_from_width_1025);
    RUN_TEST(test_tier_from_width_10000);

    /* Tier names */
    RUN_TEST(test_tier_name_desktop);
    RUN_TEST(test_tier_name_tablet);
    RUN_TEST(test_tier_name_phone_land);
    RUN_TEST(test_tier_name_phone_port);
    RUN_TEST(test_tier_name_invalid);

    /* Layout names */
    RUN_TEST(test_layout_name_card);
    RUN_TEST(test_layout_name_sheet);
    RUN_TEST(test_layout_name_fullscreen);
    RUN_TEST(test_layout_name_hidden);
    RUN_TEST(test_layout_name_invalid);

    /* Tier count and breakpoints */
    RUN_TEST(test_tier_count_is_4);
    RUN_TEST(test_tier_breakpoint_desktop);
    RUN_TEST(test_tier_breakpoint_tablet);
    RUN_TEST(test_tier_breakpoint_phone_land);
    RUN_TEST(test_tier_breakpoint_phone_port);

    /* Desktop tier */
    RUN_TEST(test_desktop_space_panels_all_card);
    RUN_TEST(test_desktop_space_visibility);
    RUN_TEST(test_desktop_space_input);
    RUN_TEST(test_desktop_space_scales);
    RUN_TEST(test_desktop_personal_same_as_space);

    /* Tablet tier */
    RUN_TEST(test_tablet_space_panels_sheet);
    RUN_TEST(test_tablet_touch_first);
    RUN_TEST(test_tablet_time_bar_always);
    RUN_TEST(test_tablet_personal_card_sheet);
    RUN_TEST(test_tablet_room_card_sheet);

    /* Phone landscape tier */
    RUN_TEST(test_phone_land_touch_only);
    RUN_TEST(test_phone_land_condensed_hud);
    RUN_TEST(test_phone_land_no_labels);
    RUN_TEST(test_phone_land_font_scales);
    RUN_TEST(test_phone_land_personal_card_fullscreen);
    RUN_TEST(test_phone_land_room_card_fullscreen);
    RUN_TEST(test_phone_land_space_card_sheet);

    /* Phone portrait tier */
    RUN_TEST(test_phone_port_touch_only);
    RUN_TEST(test_phone_port_font_scales);
    RUN_TEST(test_phone_port_help_fullscreen);
    RUN_TEST(test_phone_port_layer_sheet);
    RUN_TEST(test_phone_port_palette_fullscreen);
    RUN_TEST(test_phone_port_space_card_fullscreen);
    RUN_TEST(test_phone_port_galaxy_card_fullscreen);
    RUN_TEST(test_phone_port_earth_card_fullscreen);

    /* All tiers for specific views */
    RUN_TEST(test_space_all_tiers_view_id);
    RUN_TEST(test_personal_all_tiers_view_id);

    /* Out of range */
    RUN_TEST(test_get_negative_view_returns_default);
    RUN_TEST(test_get_view_too_large_returns_default);
    RUN_TEST(test_get_view_99_returns_default);
    RUN_TEST(test_get_tier_out_of_range_clamps_desktop);
    RUN_TEST(test_get_tier_negative_clamps_desktop);

    /* va_get_for_width */
    RUN_TEST(test_get_for_width_matches_manual);
    RUN_TEST(test_get_for_width_480);
    RUN_TEST(test_get_for_width_1920);

    /* Panel available */
    RUN_TEST(test_panel_available_card_layout);
    RUN_TEST(test_panel_available_hidden_returns_0);
    RUN_TEST(test_panel_available_invalid_type_returns_0);

    /* Progressive degradation */
    RUN_TEST(test_font_scale_decreases);
    RUN_TEST(test_hud_font_scale_decreases);
    RUN_TEST(test_spacing_scale_decreases);
    RUN_TEST(test_touch_priority_increases);
    RUN_TEST(test_min_tap_target_increases);

    /* Purity */
    RUN_TEST(test_purity_same_result);
    RUN_TEST(test_purity_tier_from_width);

    /* Structural integrity */
    RUN_TEST(test_all_configs_have_valid_view_and_tier);
    RUN_TEST(test_all_configs_positive_font_scale);
    RUN_TEST(test_all_configs_valid_touch_priority);
    RUN_TEST(test_all_configs_positive_tap_target);

    return UNITY_END();
}
