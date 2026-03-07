#include "../../tests/unity/unity.h"
#include "../../src/ui/scroll_layers.h"

#include <math.h>
#include <string.h>

#define EPSILON 0.001f

#define ASSERT_FLOAT_NEAR(expected, actual, eps) \
    TEST_ASSERT_TRUE(fabsf((actual) - (expected)) < (eps))

void setUp(void) {}
void tearDown(void) {}

/* ===================================================================
 * scroll_layer_name — string lookup
 * =================================================================== */

void test_layer_name_stars(void) {
    TEST_ASSERT_EQUAL_STRING("STARS", scroll_layer_name(SCROLL_LAYER_STARS));
}

void test_layer_name_orbits(void) {
    TEST_ASSERT_EQUAL_STRING("ORBITS", scroll_layer_name(SCROLL_LAYER_ORBITS));
}

void test_layer_name_planets(void) {
    TEST_ASSERT_EQUAL_STRING("PLANETS", scroll_layer_name(SCROLL_LAYER_PLANETS));
}

void test_layer_name_zodiac(void) {
    TEST_ASSERT_EQUAL_STRING("ZODIAC", scroll_layer_name(SCROLL_LAYER_ZODIAC));
}

void test_layer_name_labels(void) {
    TEST_ASSERT_EQUAL_STRING("LABELS", scroll_layer_name(SCROLL_LAYER_LABELS));
}

void test_layer_name_aspects(void) {
    TEST_ASSERT_EQUAL_STRING("ASPECTS", scroll_layer_name(SCROLL_LAYER_ASPECTS));
}

void test_layer_name_houses(void) {
    TEST_ASSERT_EQUAL_STRING("HOUSES", scroll_layer_name(SCROLL_LAYER_HOUSES));
}

void test_layer_name_cards(void) {
    TEST_ASSERT_EQUAL_STRING("CARDS", scroll_layer_name(SCROLL_LAYER_CARDS));
}

void test_layer_name_hud(void) {
    TEST_ASSERT_EQUAL_STRING("HUD", scroll_layer_name(SCROLL_LAYER_HUD));
}

void test_layer_name_galaxy(void) {
    TEST_ASSERT_EQUAL_STRING("GALAXY", scroll_layer_name(SCROLL_LAYER_GALAXY));
}

void test_layer_name_earth(void) {
    TEST_ASSERT_EQUAL_STRING("EARTH", scroll_layer_name(SCROLL_LAYER_EARTH));
}

void test_layer_name_invalid_returns_unknown(void) {
    TEST_ASSERT_EQUAL_STRING("UNKNOWN", scroll_layer_name(SCROLL_LAYER_COUNT));
    TEST_ASSERT_EQUAL_STRING("UNKNOWN", scroll_layer_name((scroll_layer_t)99));
}

/* ===================================================================
 * scroll_depth_name — string lookup
 * =================================================================== */

void test_depth_name_background(void) {
    TEST_ASSERT_EQUAL_STRING("BACKGROUND", scroll_depth_name(SCROLL_DEPTH_BACKGROUND));
}

void test_depth_name_mid(void) {
    TEST_ASSERT_EQUAL_STRING("MID", scroll_depth_name(SCROLL_DEPTH_MID));
}

void test_depth_name_foreground(void) {
    TEST_ASSERT_EQUAL_STRING("FOREGROUND", scroll_depth_name(SCROLL_DEPTH_FOREGROUND));
}

void test_depth_name_overlay(void) {
    TEST_ASSERT_EQUAL_STRING("OVERLAY", scroll_depth_name(SCROLL_DEPTH_OVERLAY));
}

void test_depth_name_invalid_returns_unknown(void) {
    TEST_ASSERT_EQUAL_STRING("UNKNOWN", scroll_depth_name(SCROLL_DEPTH_COUNT));
}

/* ===================================================================
 * scroll_layer_rule — layer configuration
 * =================================================================== */

void test_rule_stars_zoom_range(void) {
    scroll_layer_rule_t r = scroll_layer_rule(SCROLL_LAYER_STARS);
    ASSERT_FLOAT_NEAR(0.0f, r.zoom_min, EPSILON);
    ASSERT_FLOAT_NEAR(4.0f, r.zoom_max, EPSILON);
}

void test_rule_stars_depth(void) {
    scroll_layer_rule_t r = scroll_layer_rule(SCROLL_LAYER_STARS);
    TEST_ASSERT_EQUAL_INT(SCROLL_DEPTH_BACKGROUND, r.depth);
}

void test_rule_orbits_zoom_range(void) {
    scroll_layer_rule_t r = scroll_layer_rule(SCROLL_LAYER_ORBITS);
    ASSERT_FLOAT_NEAR(0.5f, r.zoom_min, EPSILON);
    ASSERT_FLOAT_NEAR(3.5f, r.zoom_max, EPSILON);
}

void test_rule_orbits_depth(void) {
    scroll_layer_rule_t r = scroll_layer_rule(SCROLL_LAYER_ORBITS);
    TEST_ASSERT_EQUAL_INT(SCROLL_DEPTH_MID, r.depth);
}

void test_rule_planets_zoom_range(void) {
    scroll_layer_rule_t r = scroll_layer_rule(SCROLL_LAYER_PLANETS);
    ASSERT_FLOAT_NEAR(1.0f, r.zoom_min, EPSILON);
    ASSERT_FLOAT_NEAR(3.0f, r.zoom_max, EPSILON);
}

void test_rule_planets_depth(void) {
    scroll_layer_rule_t r = scroll_layer_rule(SCROLL_LAYER_PLANETS);
    TEST_ASSERT_EQUAL_INT(SCROLL_DEPTH_FOREGROUND, r.depth);
}

void test_rule_zodiac_zoom_range(void) {
    scroll_layer_rule_t r = scroll_layer_rule(SCROLL_LAYER_ZODIAC);
    ASSERT_FLOAT_NEAR(1.5f, r.zoom_min, EPSILON);
    ASSERT_FLOAT_NEAR(3.0f, r.zoom_max, EPSILON);
}

void test_rule_labels_zoom_range(void) {
    scroll_layer_rule_t r = scroll_layer_rule(SCROLL_LAYER_LABELS);
    ASSERT_FLOAT_NEAR(1.0f, r.zoom_min, EPSILON);
    ASSERT_FLOAT_NEAR(3.5f, r.zoom_max, EPSILON);
}

void test_rule_labels_depth(void) {
    scroll_layer_rule_t r = scroll_layer_rule(SCROLL_LAYER_LABELS);
    TEST_ASSERT_EQUAL_INT(SCROLL_DEPTH_FOREGROUND, r.depth);
}

void test_rule_aspects_zoom_range(void) {
    scroll_layer_rule_t r = scroll_layer_rule(SCROLL_LAYER_ASPECTS);
    ASSERT_FLOAT_NEAR(1.5f, r.zoom_min, EPSILON);
    ASSERT_FLOAT_NEAR(3.0f, r.zoom_max, EPSILON);
}

void test_rule_houses_zoom_range(void) {
    scroll_layer_rule_t r = scroll_layer_rule(SCROLL_LAYER_HOUSES);
    ASSERT_FLOAT_NEAR(1.5f, r.zoom_min, EPSILON);
    ASSERT_FLOAT_NEAR(3.0f, r.zoom_max, EPSILON);
}

void test_rule_cards_zoom_range(void) {
    scroll_layer_rule_t r = scroll_layer_rule(SCROLL_LAYER_CARDS);
    ASSERT_FLOAT_NEAR(1.0f, r.zoom_min, EPSILON);
    ASSERT_FLOAT_NEAR(4.0f, r.zoom_max, EPSILON);
}

void test_rule_cards_depth(void) {
    scroll_layer_rule_t r = scroll_layer_rule(SCROLL_LAYER_CARDS);
    TEST_ASSERT_EQUAL_INT(SCROLL_DEPTH_OVERLAY, r.depth);
}

void test_rule_hud_zoom_range(void) {
    scroll_layer_rule_t r = scroll_layer_rule(SCROLL_LAYER_HUD);
    ASSERT_FLOAT_NEAR(0.0f, r.zoom_min, EPSILON);
    ASSERT_FLOAT_NEAR(6.0f, r.zoom_max, EPSILON);
}

void test_rule_hud_depth(void) {
    scroll_layer_rule_t r = scroll_layer_rule(SCROLL_LAYER_HUD);
    TEST_ASSERT_EQUAL_INT(SCROLL_DEPTH_OVERLAY, r.depth);
}

void test_rule_galaxy_zoom_range(void) {
    scroll_layer_rule_t r = scroll_layer_rule(SCROLL_LAYER_GALAXY);
    ASSERT_FLOAT_NEAR(4.0f, r.zoom_min, EPSILON);
    ASSERT_FLOAT_NEAR(6.0f, r.zoom_max, EPSILON);
}

void test_rule_galaxy_depth(void) {
    scroll_layer_rule_t r = scroll_layer_rule(SCROLL_LAYER_GALAXY);
    TEST_ASSERT_EQUAL_INT(SCROLL_DEPTH_BACKGROUND, r.depth);
}

void test_rule_earth_zoom_range(void) {
    scroll_layer_rule_t r = scroll_layer_rule(SCROLL_LAYER_EARTH);
    ASSERT_FLOAT_NEAR(0.0f, r.zoom_min, EPSILON);
    ASSERT_FLOAT_NEAR(2.0f, r.zoom_max, EPSILON);
}

void test_rule_earth_depth(void) {
    scroll_layer_rule_t r = scroll_layer_rule(SCROLL_LAYER_EARTH);
    TEST_ASSERT_EQUAL_INT(SCROLL_DEPTH_FOREGROUND, r.depth);
}

void test_rule_fade_widths_default(void) {
    /* All layers should have default fade_in_width and fade_out_width of 0.5 */
    for (int i = 0; i < SCROLL_LAYER_COUNT; i++) {
        scroll_layer_rule_t r = scroll_layer_rule((scroll_layer_t)i);
        ASSERT_FLOAT_NEAR(0.5f, r.fade_in_width, EPSILON);
        ASSERT_FLOAT_NEAR(0.5f, r.fade_out_width, EPSILON);
    }
}

void test_rule_layer_field_matches_index(void) {
    for (int i = 0; i < SCROLL_LAYER_COUNT; i++) {
        scroll_layer_rule_t r = scroll_layer_rule((scroll_layer_t)i);
        TEST_ASSERT_EQUAL_INT(i, r.layer);
    }
}

/* ===================================================================
 * scroll_layer_opacity — fade logic
 * =================================================================== */

void test_opacity_below_zoom_min_is_zero(void) {
    /* PLANETS: zoom_min=1.0, below that should be 0 */
    ASSERT_FLOAT_NEAR(0.0f, scroll_layer_opacity(SCROLL_LAYER_PLANETS, 0.0f), EPSILON);
    ASSERT_FLOAT_NEAR(0.0f, scroll_layer_opacity(SCROLL_LAYER_PLANETS, 0.5f), EPSILON);
}

void test_opacity_above_zoom_max_is_zero(void) {
    /* PLANETS: zoom_max=3.0, above that should be 0 */
    ASSERT_FLOAT_NEAR(0.0f, scroll_layer_opacity(SCROLL_LAYER_PLANETS, 3.5f), EPSILON);
    ASSERT_FLOAT_NEAR(0.0f, scroll_layer_opacity(SCROLL_LAYER_PLANETS, 10.0f), EPSILON);
}

void test_opacity_in_full_region_is_one(void) {
    /* PLANETS: zoom_min=1.0, fade_in=0.5, fade_out=0.5, zoom_max=3.0
     * Full opacity region: [1.5, 2.5] */
    ASSERT_FLOAT_NEAR(1.0f, scroll_layer_opacity(SCROLL_LAYER_PLANETS, 2.0f), EPSILON);
    ASSERT_FLOAT_NEAR(1.0f, scroll_layer_opacity(SCROLL_LAYER_PLANETS, 1.5f), EPSILON);
    ASSERT_FLOAT_NEAR(1.0f, scroll_layer_opacity(SCROLL_LAYER_PLANETS, 2.5f), EPSILON);
}

void test_opacity_fade_in_midpoint(void) {
    /* PLANETS: zoom_min=1.0, fade_in=0.5
     * At zoom=1.25 (halfway through fade-in), opacity should be 0.5 */
    ASSERT_FLOAT_NEAR(0.5f, scroll_layer_opacity(SCROLL_LAYER_PLANETS, 1.25f), EPSILON);
}

void test_opacity_fade_out_midpoint(void) {
    /* PLANETS: zoom_max=3.0, fade_out=0.5
     * At zoom=2.75 (halfway through fade-out), opacity should be 0.5 */
    ASSERT_FLOAT_NEAR(0.5f, scroll_layer_opacity(SCROLL_LAYER_PLANETS, 2.75f), EPSILON);
}

void test_opacity_fade_in_start_is_zero(void) {
    /* PLANETS: at zoom_min=1.0, opacity starts at 0 */
    ASSERT_FLOAT_NEAR(0.0f, scroll_layer_opacity(SCROLL_LAYER_PLANETS, 1.0f), EPSILON);
}

void test_opacity_fade_out_end_is_zero(void) {
    /* PLANETS: at zoom_max=3.0, opacity is 0 */
    ASSERT_FLOAT_NEAR(0.0f, scroll_layer_opacity(SCROLL_LAYER_PLANETS, 3.0f), EPSILON);
}

void test_opacity_stars_at_zero_starts_fading_in(void) {
    /* STARS: zoom_min=0.0, fade_in=0.5. At zoom=0, that's start of fade-in */
    ASSERT_FLOAT_NEAR(0.0f, scroll_layer_opacity(SCROLL_LAYER_STARS, 0.0f), EPSILON);
}

void test_opacity_stars_fully_visible(void) {
    /* STARS: zoom_min=0.0, fade_in=0.5, fade_out=0.5, zoom_max=4.0
     * Full region: [0.5, 3.5] */
    ASSERT_FLOAT_NEAR(1.0f, scroll_layer_opacity(SCROLL_LAYER_STARS, 2.0f), EPSILON);
}

void test_opacity_hud_wide_range(void) {
    /* HUD: zoom_min=0.0, zoom_max=6.0, fade_in=0.5, fade_out=0.5
     * Full region: [0.5, 5.5] */
    ASSERT_FLOAT_NEAR(1.0f, scroll_layer_opacity(SCROLL_LAYER_HUD, 3.0f), EPSILON);
}

void test_opacity_galaxy_only_at_high_zoom(void) {
    /* GALAXY: zoom_min=4.0, zoom_max=6.0 */
    ASSERT_FLOAT_NEAR(0.0f, scroll_layer_opacity(SCROLL_LAYER_GALAXY, 3.0f), EPSILON);
    ASSERT_FLOAT_NEAR(1.0f, scroll_layer_opacity(SCROLL_LAYER_GALAXY, 5.0f), EPSILON);
}

void test_opacity_clamped_zero_to_one(void) {
    /* opacity should never exceed [0, 1] */
    for (int i = 0; i < SCROLL_LAYER_COUNT; i++) {
        for (float z = -1.0f; z <= 8.0f; z += 0.25f) {
            float o = scroll_layer_opacity((scroll_layer_t)i, z);
            TEST_ASSERT_TRUE(o >= 0.0f);
            TEST_ASSERT_TRUE(o <= 1.0f);
        }
    }
}

void test_opacity_linear_ramp_quarter(void) {
    /* ORBITS: zoom_min=0.5, fade_in=0.5. At zoom=0.625 (25% through): 0.25 */
    ASSERT_FLOAT_NEAR(0.25f, scroll_layer_opacity(SCROLL_LAYER_ORBITS, 0.625f), EPSILON);
}

void test_opacity_linear_ramp_three_quarter(void) {
    /* ORBITS: zoom_min=0.5, fade_in=0.5. At zoom=0.875 (75% through): 0.75 */
    ASSERT_FLOAT_NEAR(0.75f, scroll_layer_opacity(SCROLL_LAYER_ORBITS, 0.875f), EPSILON);
}

/* ===================================================================
 * scroll_to_zoom / zoom_to_scroll — phi-logarithmic mapping
 * =================================================================== */

void test_scroll_to_zoom_at_zero(void) {
    /* scroll=0 -> zoom = phi^0 = 1.0 */
    ASSERT_FLOAT_NEAR(1.0f, scroll_to_zoom(0.0f), EPSILON);
}

void test_scroll_to_zoom_at_one(void) {
    /* scroll=1 -> zoom = phi^1 = 1.618 */
    float phi = 1.6180339887f;
    ASSERT_FLOAT_NEAR(phi, scroll_to_zoom(1.0f), EPSILON);
}

void test_scroll_to_zoom_at_two(void) {
    /* scroll=2 -> zoom = phi^2 */
    float phi_sq = 1.6180339887f * 1.6180339887f;
    ASSERT_FLOAT_NEAR(phi_sq, scroll_to_zoom(2.0f), EPSILON);
}

void test_scroll_to_zoom_negative(void) {
    /* scroll=-1 -> zoom = phi^-1 = 0.618 */
    float phi_inv = 0.6180339887f;
    ASSERT_FLOAT_NEAR(phi_inv, scroll_to_zoom(-1.0f), EPSILON);
}

void test_zoom_to_scroll_at_one(void) {
    /* zoom=1 -> scroll = 0 */
    ASSERT_FLOAT_NEAR(0.0f, zoom_to_scroll(1.0f), EPSILON);
}

void test_zoom_to_scroll_at_phi(void) {
    /* zoom=phi -> scroll = 1 */
    float phi = 1.6180339887f;
    ASSERT_FLOAT_NEAR(1.0f, zoom_to_scroll(phi), EPSILON);
}

void test_zoom_to_scroll_at_phi_inv(void) {
    /* zoom=phi^-1 -> scroll = -1 */
    float phi_inv = 0.6180339887f;
    ASSERT_FLOAT_NEAR(-1.0f, zoom_to_scroll(phi_inv), EPSILON);
}

void test_scroll_zoom_roundtrip(void) {
    /* scroll -> zoom -> scroll should return original */
    for (float s = -3.0f; s <= 3.0f; s += 0.5f) {
        float z = scroll_to_zoom(s);
        float s2 = zoom_to_scroll(z);
        ASSERT_FLOAT_NEAR(s, s2, EPSILON);
    }
}

void test_zoom_scroll_roundtrip(void) {
    /* zoom -> scroll -> zoom should return original (for positive zoom) */
    for (float z = 0.1f; z <= 5.0f; z += 0.5f) {
        float s = zoom_to_scroll(z);
        float z2 = scroll_to_zoom(s);
        ASSERT_FLOAT_NEAR(z, z2, EPSILON);
    }
}

void test_scroll_to_zoom_always_positive(void) {
    /* phi^x is always positive */
    for (float s = -10.0f; s <= 10.0f; s += 1.0f) {
        TEST_ASSERT_TRUE(scroll_to_zoom(s) > 0.0f);
    }
}

/* ===================================================================
 * scroll_perspective — camera interpolation
 * =================================================================== */

void test_perspective_at_zoom_zero(void) {
    scroll_perspective_t p = scroll_perspective(0.0f);
    ASSERT_FLOAT_NEAR(0.3f, p.pitch, EPSILON);
    ASSERT_FLOAT_NEAR(1.0f, p.distance, EPSILON);
    ASSERT_FLOAT_NEAR((float)PI / 3.0f, p.fov, EPSILON);
}

void test_perspective_at_zoom_three(void) {
    scroll_perspective_t p = scroll_perspective(3.0f);
    ASSERT_FLOAT_NEAR(0.6f, p.pitch, EPSILON);
    ASSERT_FLOAT_NEAR(10.0f, p.distance, EPSILON);
    ASSERT_FLOAT_NEAR((float)PI / 4.0f, p.fov, EPSILON);
}

void test_perspective_at_zoom_six(void) {
    scroll_perspective_t p = scroll_perspective(6.0f);
    ASSERT_FLOAT_NEAR(1.0f, p.pitch, EPSILON);
    ASSERT_FLOAT_NEAR(100.0f, p.distance, EPSILON);
    ASSERT_FLOAT_NEAR((float)PI / 6.0f, p.fov, EPSILON);
}

void test_perspective_interpolation_midpoint(void) {
    /* At zoom=1.5, midpoint between keyframe 0 (zoom=0) and 1 (zoom=3) */
    scroll_perspective_t p = scroll_perspective(1.5f);
    float expected_pitch = 0.3f + (0.6f - 0.3f) * 0.5f;
    float expected_dist = 1.0f + (10.0f - 1.0f) * 0.5f;
    float expected_fov = (float)PI / 3.0f + ((float)PI / 4.0f - (float)PI / 3.0f) * 0.5f;
    ASSERT_FLOAT_NEAR(expected_pitch, p.pitch, EPSILON);
    ASSERT_FLOAT_NEAR(expected_dist, p.distance, EPSILON);
    ASSERT_FLOAT_NEAR(expected_fov, p.fov, EPSILON);
}

void test_perspective_below_zero_clamps(void) {
    /* Below zoom 0, should clamp to keyframe 0 */
    scroll_perspective_t p = scroll_perspective(-1.0f);
    ASSERT_FLOAT_NEAR(0.3f, p.pitch, EPSILON);
    ASSERT_FLOAT_NEAR(1.0f, p.distance, EPSILON);
}

void test_perspective_above_six_clamps(void) {
    /* Above zoom 6, should clamp to keyframe 2 */
    scroll_perspective_t p = scroll_perspective(10.0f);
    ASSERT_FLOAT_NEAR(1.0f, p.pitch, EPSILON);
    ASSERT_FLOAT_NEAR(100.0f, p.distance, EPSILON);
}

void test_perspective_at_zoom_4_5(void) {
    /* zoom=4.5, midpoint between keyframe 1 (zoom=3) and 2 (zoom=6) */
    scroll_perspective_t p = scroll_perspective(4.5f);
    float expected_pitch = 0.6f + (1.0f - 0.6f) * 0.5f;
    float expected_dist = 10.0f + (100.0f - 10.0f) * 0.5f;
    ASSERT_FLOAT_NEAR(expected_pitch, p.pitch, EPSILON);
    ASSERT_FLOAT_NEAR(expected_dist, p.distance, EPSILON);
}

/* ===================================================================
 * scroll_depth_parallax — phi-based depth factors
 * =================================================================== */

void test_depth_parallax_background(void) {
    /* phi^-2 = 0.3820 */
    float phi_inv2 = 1.0f / (1.6180339887f * 1.6180339887f);
    ASSERT_FLOAT_NEAR(phi_inv2, scroll_depth_parallax(SCROLL_DEPTH_BACKGROUND), EPSILON);
}

void test_depth_parallax_mid(void) {
    /* phi^-1 = 0.6180 */
    float phi_inv = 1.0f / 1.6180339887f;
    ASSERT_FLOAT_NEAR(phi_inv, scroll_depth_parallax(SCROLL_DEPTH_MID), EPSILON);
}

void test_depth_parallax_foreground(void) {
    ASSERT_FLOAT_NEAR(1.0f, scroll_depth_parallax(SCROLL_DEPTH_FOREGROUND), EPSILON);
}

void test_depth_parallax_overlay(void) {
    ASSERT_FLOAT_NEAR(0.0f, scroll_depth_parallax(SCROLL_DEPTH_OVERLAY), EPSILON);
}

void test_depth_parallax_invalid(void) {
    /* Out-of-range should return 0.0 */
    ASSERT_FLOAT_NEAR(0.0f, scroll_depth_parallax(SCROLL_DEPTH_COUNT), EPSILON);
}

/* ===================================================================
 * scroll_parallax_offset — scroll delta scaled by layer depth
 * =================================================================== */

void test_parallax_offset_foreground(void) {
    /* PLANETS (FOREGROUND): factor=1.0, offset = delta * 1.0 */
    float offset = scroll_parallax_offset(SCROLL_LAYER_PLANETS, 5.0f);
    ASSERT_FLOAT_NEAR(5.0f, offset, EPSILON);
}

void test_parallax_offset_background(void) {
    /* STARS (BACKGROUND): factor=phi^-2, offset = delta * phi^-2 */
    float phi_inv2 = 1.0f / (1.6180339887f * 1.6180339887f);
    float offset = scroll_parallax_offset(SCROLL_LAYER_STARS, 5.0f);
    ASSERT_FLOAT_NEAR(5.0f * phi_inv2, offset, EPSILON);
}

void test_parallax_offset_mid(void) {
    /* ORBITS (MID): factor=phi^-1, offset = delta * phi^-1 */
    float phi_inv = 1.0f / 1.6180339887f;
    float offset = scroll_parallax_offset(SCROLL_LAYER_ORBITS, 5.0f);
    ASSERT_FLOAT_NEAR(5.0f * phi_inv, offset, EPSILON);
}

void test_parallax_offset_overlay_is_zero(void) {
    /* CARDS (OVERLAY): factor=0.0, offset = 0 */
    float offset = scroll_parallax_offset(SCROLL_LAYER_CARDS, 5.0f);
    ASSERT_FLOAT_NEAR(0.0f, offset, EPSILON);
}

void test_parallax_offset_negative_delta(void) {
    float phi_inv = 1.0f / 1.6180339887f;
    float offset = scroll_parallax_offset(SCROLL_LAYER_ORBITS, -3.0f);
    ASSERT_FLOAT_NEAR(-3.0f * phi_inv, offset, EPSILON);
}

void test_parallax_offset_zero_delta(void) {
    float offset = scroll_parallax_offset(SCROLL_LAYER_PLANETS, 0.0f);
    ASSERT_FLOAT_NEAR(0.0f, offset, EPSILON);
}

/* ===================================================================
 * scroll_is_visible / scroll_visible_count
 * =================================================================== */

void test_is_visible_stars_at_zoom_2(void) {
    TEST_ASSERT_EQUAL_INT(1, scroll_is_visible(SCROLL_LAYER_STARS, 2.0f));
}

void test_is_visible_galaxy_at_zoom_2(void) {
    /* GALAXY: zoom_min=4.0. Not visible at 2.0 */
    TEST_ASSERT_EQUAL_INT(0, scroll_is_visible(SCROLL_LAYER_GALAXY, 2.0f));
}

void test_is_visible_galaxy_at_zoom_5(void) {
    TEST_ASSERT_EQUAL_INT(1, scroll_is_visible(SCROLL_LAYER_GALAXY, 5.0f));
}

void test_is_visible_at_boundary_is_not_visible(void) {
    /* At exactly zoom_min, opacity=0 so not visible */
    TEST_ASSERT_EQUAL_INT(0, scroll_is_visible(SCROLL_LAYER_PLANETS, 1.0f));
}

void test_is_visible_just_inside_boundary(void) {
    /* Just past zoom_min, should be visible (opacity>0) */
    TEST_ASSERT_EQUAL_INT(1, scroll_is_visible(SCROLL_LAYER_PLANETS, 1.01f));
}

void test_visible_count_at_zoom_2(void) {
    /* At zoom=2.0 these should be visible:
     * STARS (0-4), ORBITS (0.5-3.5), PLANETS (1.0-3.0),
     * ZODIAC (1.5-3.0), LABELS (1.0-3.5), ASPECTS (1.5-3.0),
     * HOUSES (1.5-3.0), CARDS (1.0-4.0), HUD (0-6), EARTH (0-2) */
    int count = scroll_visible_count(2.0f);
    /* Need to verify: at zoom=2.0, which have opacity > 0?
     * STARS: fade-in done (0.5), fade-out starts at 3.5 -> yes
     * ORBITS: zoom_min=0.5, well past fade-in -> yes
     * PLANETS: zoom_min=1.0, past fade-in -> yes
     * ZODIAC: zoom_min=1.5, at 2.0 we're past fade-in (1.5+0.5=2.0). Opacity at 2.0:
     *   fade-in region is [1.5, 2.0], fade-out region is [2.5, 3.0].
     *   At zoom=2.0: we are at end of fade-in, opacity = 1.0. Actually wait:
     *   Between zoom_min(1.5) and zoom_min+fade_in(2.0): linear ramp.
     *   At zoom=2.0 = zoom_min + fade_in_width -> opacity = (2.0-1.5)/0.5 = 1.0. Yes, visible.
     * LABELS: yes
     * ASPECTS: yes (same as ZODIAC logic)
     * HOUSES: yes (same)
     * CARDS: yes
     * HUD: yes
     * GALAXY: zoom_min=4.0, not visible at 2.0 -> no
     * EARTH: zoom_min=0.0, zoom_max=2.0, fade_out=0.5.
     *   fade-out region: [1.5, 2.0]. At zoom=2.0: opacity = (2.0-2.0)/0.5 = 0 -> no
     * So: 9 visible */
    TEST_ASSERT_EQUAL_INT(9, count);
}

void test_visible_count_at_zoom_0(void) {
    /* At zoom=0.0:
     * STARS: opacity at zoom_min=0 -> 0 (at boundary) -> no
     * ORBITS: below zoom_min=0.5 -> no
     * PLANETS: below 1.0 -> no
     * ... all others below their minimums -> no
     * HUD: zoom_min=0.0 -> 0 (at boundary) -> no
     * EARTH: zoom_min=0.0 -> 0 (at boundary) -> no
     * GALAXY: below 4.0 -> no
     * Count = 0 */
    TEST_ASSERT_EQUAL_INT(0, scroll_visible_count(0.0f));
}

void test_visible_count_at_zoom_5(void) {
    /* At zoom=5.0:
     * STARS: zoom_max=4.0, above -> no
     * ORBITS: zoom_max=3.5, above -> no
     * PLANETS: above 3.0 -> no
     * ZODIAC: above 3.0 -> no
     * LABELS: above 3.5 -> no
     * ASPECTS: above 3.0 -> no
     * HOUSES: above 3.0 -> no
     * CARDS: above 4.0 -> no
     * HUD: zoom_max=6.0, in fade-out region? [5.5, 6.0]. At 5.0: opacity=1.0 -> yes
     * GALAXY: zoom_min=4.0, zoom_max=6.0, fade-in [4.0,4.5]. At 5.0: past fade-in -> yes
     * EARTH: above 2.0 -> no
     * Count = 2 */
    TEST_ASSERT_EQUAL_INT(2, scroll_visible_count(5.0f));
}

/* ===================================================================
 * scroll_layer_rule parallax_factor consistency
 * =================================================================== */

void test_rule_parallax_factor_matches_depth(void) {
    /* Each rule's parallax_factor should match scroll_depth_parallax(rule.depth) */
    for (int i = 0; i < SCROLL_LAYER_COUNT; i++) {
        scroll_layer_rule_t r = scroll_layer_rule((scroll_layer_t)i);
        float expected = scroll_depth_parallax(r.depth);
        ASSERT_FLOAT_NEAR(expected, r.parallax_factor, EPSILON);
    }
}

/* ===================================================================
 * Edge cases
 * =================================================================== */

void test_opacity_negative_zoom(void) {
    /* Negative zoom: all layers should have 0 opacity */
    for (int i = 0; i < SCROLL_LAYER_COUNT; i++) {
        float o = scroll_layer_opacity((scroll_layer_t)i, -1.0f);
        ASSERT_FLOAT_NEAR(0.0f, o, EPSILON);
    }
}

void test_opacity_very_large_zoom(void) {
    /* Very large zoom: only HUD might be near limit, all others 0 */
    ASSERT_FLOAT_NEAR(0.0f, scroll_layer_opacity(SCROLL_LAYER_STARS, 100.0f), EPSILON);
    ASSERT_FLOAT_NEAR(0.0f, scroll_layer_opacity(SCROLL_LAYER_HUD, 100.0f), EPSILON);
}

void test_zoom_to_scroll_very_small_positive(void) {
    /* Very small zoom should give large negative scroll */
    float s = zoom_to_scroll(0.01f);
    TEST_ASSERT_TRUE(s < -5.0f);
}

void test_perspective_monotonic_pitch(void) {
    /* Pitch should increase with zoom */
    float prev_pitch = scroll_perspective(0.0f).pitch;
    for (float z = 0.5f; z <= 6.0f; z += 0.5f) {
        float pitch = scroll_perspective(z).pitch;
        TEST_ASSERT_TRUE(pitch >= prev_pitch - EPSILON);
        prev_pitch = pitch;
    }
}

void test_perspective_monotonic_distance(void) {
    /* Distance should increase with zoom */
    float prev_dist = scroll_perspective(0.0f).distance;
    for (float z = 0.5f; z <= 6.0f; z += 0.5f) {
        float dist = scroll_perspective(z).distance;
        TEST_ASSERT_TRUE(dist >= prev_dist - EPSILON);
        prev_dist = dist;
    }
}

void test_perspective_fov_decreases(void) {
    /* FOV should decrease with zoom (narrowing field) */
    float prev_fov = scroll_perspective(0.0f).fov;
    for (float z = 0.5f; z <= 6.0f; z += 0.5f) {
        float fov = scroll_perspective(z).fov;
        TEST_ASSERT_TRUE(fov <= prev_fov + EPSILON);
        prev_fov = fov;
    }
}

/* ===================================================================
 * main
 * =================================================================== */

int main(void) {
    UNITY_BEGIN();

    /* Layer name tests */
    RUN_TEST(test_layer_name_stars);
    RUN_TEST(test_layer_name_orbits);
    RUN_TEST(test_layer_name_planets);
    RUN_TEST(test_layer_name_zodiac);
    RUN_TEST(test_layer_name_labels);
    RUN_TEST(test_layer_name_aspects);
    RUN_TEST(test_layer_name_houses);
    RUN_TEST(test_layer_name_cards);
    RUN_TEST(test_layer_name_hud);
    RUN_TEST(test_layer_name_galaxy);
    RUN_TEST(test_layer_name_earth);
    RUN_TEST(test_layer_name_invalid_returns_unknown);

    /* Depth name tests */
    RUN_TEST(test_depth_name_background);
    RUN_TEST(test_depth_name_mid);
    RUN_TEST(test_depth_name_foreground);
    RUN_TEST(test_depth_name_overlay);
    RUN_TEST(test_depth_name_invalid_returns_unknown);

    /* Layer rule tests */
    RUN_TEST(test_rule_stars_zoom_range);
    RUN_TEST(test_rule_stars_depth);
    RUN_TEST(test_rule_orbits_zoom_range);
    RUN_TEST(test_rule_orbits_depth);
    RUN_TEST(test_rule_planets_zoom_range);
    RUN_TEST(test_rule_planets_depth);
    RUN_TEST(test_rule_zodiac_zoom_range);
    RUN_TEST(test_rule_labels_zoom_range);
    RUN_TEST(test_rule_labels_depth);
    RUN_TEST(test_rule_aspects_zoom_range);
    RUN_TEST(test_rule_houses_zoom_range);
    RUN_TEST(test_rule_cards_zoom_range);
    RUN_TEST(test_rule_cards_depth);
    RUN_TEST(test_rule_hud_zoom_range);
    RUN_TEST(test_rule_hud_depth);
    RUN_TEST(test_rule_galaxy_zoom_range);
    RUN_TEST(test_rule_galaxy_depth);
    RUN_TEST(test_rule_earth_zoom_range);
    RUN_TEST(test_rule_earth_depth);
    RUN_TEST(test_rule_fade_widths_default);
    RUN_TEST(test_rule_layer_field_matches_index);

    /* Opacity tests */
    RUN_TEST(test_opacity_below_zoom_min_is_zero);
    RUN_TEST(test_opacity_above_zoom_max_is_zero);
    RUN_TEST(test_opacity_in_full_region_is_one);
    RUN_TEST(test_opacity_fade_in_midpoint);
    RUN_TEST(test_opacity_fade_out_midpoint);
    RUN_TEST(test_opacity_fade_in_start_is_zero);
    RUN_TEST(test_opacity_fade_out_end_is_zero);
    RUN_TEST(test_opacity_stars_at_zero_starts_fading_in);
    RUN_TEST(test_opacity_stars_fully_visible);
    RUN_TEST(test_opacity_hud_wide_range);
    RUN_TEST(test_opacity_galaxy_only_at_high_zoom);
    RUN_TEST(test_opacity_clamped_zero_to_one);
    RUN_TEST(test_opacity_linear_ramp_quarter);
    RUN_TEST(test_opacity_linear_ramp_three_quarter);

    /* Scroll-to-zoom tests */
    RUN_TEST(test_scroll_to_zoom_at_zero);
    RUN_TEST(test_scroll_to_zoom_at_one);
    RUN_TEST(test_scroll_to_zoom_at_two);
    RUN_TEST(test_scroll_to_zoom_negative);
    RUN_TEST(test_zoom_to_scroll_at_one);
    RUN_TEST(test_zoom_to_scroll_at_phi);
    RUN_TEST(test_zoom_to_scroll_at_phi_inv);
    RUN_TEST(test_scroll_zoom_roundtrip);
    RUN_TEST(test_zoom_scroll_roundtrip);
    RUN_TEST(test_scroll_to_zoom_always_positive);

    /* Perspective tests */
    RUN_TEST(test_perspective_at_zoom_zero);
    RUN_TEST(test_perspective_at_zoom_three);
    RUN_TEST(test_perspective_at_zoom_six);
    RUN_TEST(test_perspective_interpolation_midpoint);
    RUN_TEST(test_perspective_below_zero_clamps);
    RUN_TEST(test_perspective_above_six_clamps);
    RUN_TEST(test_perspective_at_zoom_4_5);

    /* Depth parallax tests */
    RUN_TEST(test_depth_parallax_background);
    RUN_TEST(test_depth_parallax_mid);
    RUN_TEST(test_depth_parallax_foreground);
    RUN_TEST(test_depth_parallax_overlay);
    RUN_TEST(test_depth_parallax_invalid);

    /* Parallax offset tests */
    RUN_TEST(test_parallax_offset_foreground);
    RUN_TEST(test_parallax_offset_background);
    RUN_TEST(test_parallax_offset_mid);
    RUN_TEST(test_parallax_offset_overlay_is_zero);
    RUN_TEST(test_parallax_offset_negative_delta);
    RUN_TEST(test_parallax_offset_zero_delta);

    /* Visibility tests */
    RUN_TEST(test_is_visible_stars_at_zoom_2);
    RUN_TEST(test_is_visible_galaxy_at_zoom_2);
    RUN_TEST(test_is_visible_galaxy_at_zoom_5);
    RUN_TEST(test_is_visible_at_boundary_is_not_visible);
    RUN_TEST(test_is_visible_just_inside_boundary);
    RUN_TEST(test_visible_count_at_zoom_2);
    RUN_TEST(test_visible_count_at_zoom_0);
    RUN_TEST(test_visible_count_at_zoom_5);

    /* Consistency tests */
    RUN_TEST(test_rule_parallax_factor_matches_depth);

    /* Edge cases */
    RUN_TEST(test_opacity_negative_zoom);
    RUN_TEST(test_opacity_very_large_zoom);
    RUN_TEST(test_zoom_to_scroll_very_small_positive);
    RUN_TEST(test_perspective_monotonic_pitch);
    RUN_TEST(test_perspective_monotonic_distance);
    RUN_TEST(test_perspective_fov_decreases);

    return UNITY_END();
}
