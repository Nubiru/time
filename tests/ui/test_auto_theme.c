#include "../../tests/unity/unity.h"
#include "../../src/ui/auto_theme.h"

#include <math.h>

#define EPSILON 0.001f

void setUp(void) {}
void tearDown(void) {}

/* ================================================================
 * SUN ELEVATION -> THEME STATE
 * ================================================================ */

void test_at_full_day(void) {
    auto_theme_state_t s = at_from_sun_elevation(30.0);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, s.blend_factor);
    TEST_ASSERT_EQUAL_INT(THEME_DAWN, s.active_theme);
    TEST_ASSERT_EQUAL_INT(0, s.is_transitioning);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 30.0, s.sun_elevation_deg);
}

void test_at_full_night(void) {
    auto_theme_state_t s = at_from_sun_elevation(-20.0);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, s.blend_factor);
    TEST_ASSERT_EQUAL_INT(THEME_COSMOS, s.active_theme);
    TEST_ASSERT_EQUAL_INT(0, s.is_transitioning);
    TEST_ASSERT_FLOAT_WITHIN(0.01, -20.0, s.sun_elevation_deg);
}

void test_at_sunrise_start(void) {
    /* At exactly -6 degrees: bottom of twilight zone */
    auto_theme_state_t s = at_from_sun_elevation(-6.0);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, s.blend_factor);
    TEST_ASSERT_EQUAL_INT(THEME_COSMOS, s.active_theme);
    TEST_ASSERT_EQUAL_INT(0, s.is_transitioning);
}

void test_at_sunrise_mid(void) {
    /* At 0 degrees: midpoint of twilight zone */
    auto_theme_state_t s = at_from_sun_elevation(0.0);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.5f, s.blend_factor);
    TEST_ASSERT_EQUAL_INT(1, s.is_transitioning);
}

void test_at_sunrise_end(void) {
    /* At exactly +6 degrees: top of twilight zone */
    auto_theme_state_t s = at_from_sun_elevation(6.0);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, s.blend_factor);
    TEST_ASSERT_EQUAL_INT(THEME_DAWN, s.active_theme);
    TEST_ASSERT_EQUAL_INT(0, s.is_transitioning);
}

void test_at_sunset_mid(void) {
    /* Same as sunrise mid: sun at 0 degrees */
    auto_theme_state_t s = at_from_sun_elevation(0.0);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.5f, s.blend_factor);
    TEST_ASSERT_EQUAL_INT(1, s.is_transitioning);
}

void test_at_blend_monotonic(void) {
    /* As sun rises from -6 to +6, blend_factor must increase monotonically */
    float prev = -1.0f;
    for (double elev = -6.0; elev <= 6.0; elev += 0.5) {
        auto_theme_state_t s = at_from_sun_elevation(elev);
        TEST_ASSERT_TRUE(s.blend_factor >= prev);
        prev = s.blend_factor;
    }
}

/* ================================================================
 * BLENDED THEME
 * ================================================================ */

void test_at_blended_theme_day(void) {
    auto_theme_state_t s = at_from_sun_elevation(30.0);
    theme_t t = at_blended_theme(s);
    /* blend_factor=1.0 means fully Dawn */
    theme_t dawn = theme_get(THEME_DAWN);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, dawn.bg_space.r, t.bg_space.r);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, dawn.bg_space.g, t.bg_space.g);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, dawn.bg_space.b, t.bg_space.b);
}

void test_at_blended_theme_night(void) {
    auto_theme_state_t s = at_from_sun_elevation(-20.0);
    theme_t t = at_blended_theme(s);
    /* blend_factor=0.0 means fully Cosmos */
    theme_t cosmos = theme_get(THEME_COSMOS);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, cosmos.bg_space.r, t.bg_space.r);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, cosmos.bg_space.g, t.bg_space.g);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, cosmos.bg_space.b, t.bg_space.b);
}

void test_at_blended_theme_twilight(void) {
    /* At 0 degrees, blend_factor=0.5: should be between Cosmos and Dawn */
    auto_theme_state_t s = at_from_sun_elevation(0.0);
    theme_t t = at_blended_theme(s);
    theme_t cosmos = theme_get(THEME_COSMOS);
    theme_t dawn = theme_get(THEME_DAWN);
    /* bg_space should be midpoint of cosmos and dawn bg_space */
    float expected_r = (cosmos.bg_space.r + dawn.bg_space.r) * 0.5f;
    float expected_g = (cosmos.bg_space.g + dawn.bg_space.g) * 0.5f;
    float expected_b = (cosmos.bg_space.b + dawn.bg_space.b) * 0.5f;
    TEST_ASSERT_FLOAT_WITHIN(0.01f, expected_r, t.bg_space.r);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, expected_g, t.bg_space.g);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, expected_b, t.bg_space.b);
}

/* ================================================================
 * EFFECTIVE THEME (manual override)
 * ================================================================ */

void test_at_effective_manual(void) {
    /* When auto_enabled=0, returns the manual theme regardless of sun */
    auto_theme_state_t s = at_from_sun_elevation(30.0); /* full day */
    theme_t t = at_effective_theme(s, 0, THEME_COSMOS);
    /* Should return Cosmos even though sun says Dawn */
    theme_t cosmos = theme_get(THEME_COSMOS);
    TEST_ASSERT_EQUAL_INT(THEME_COSMOS, t.id);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, cosmos.bg_space.r, t.bg_space.r);
}

void test_at_effective_auto(void) {
    /* When auto_enabled=1, returns auto-selected theme */
    auto_theme_state_t s = at_from_sun_elevation(30.0); /* full day */
    theme_t t = at_effective_theme(s, 1, THEME_COSMOS);
    /* Should return Dawn-blended theme (auto overrides manual) */
    theme_t dawn = theme_get(THEME_DAWN);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, dawn.bg_space.r, t.bg_space.r);
}

/* ================================================================
 * SMOOTHING
 * ================================================================ */

void test_at_smooth_converges(void) {
    /* Start at Cosmos (0), target Dawn (1). After many steps, should approach 1 */
    auto_theme_state_t prev = at_from_sun_elevation(-20.0);
    auto_theme_state_t target = at_from_sun_elevation(30.0);

    for (int i = 0; i < 100; i++) {
        prev = at_smooth(prev, target, 0.016f, 2.0f);
    }
    /* After 100 frames at 60fps (~1.6s) with rate 2.0, should be very close */
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 1.0f, prev.blend_factor);
}

void test_at_smooth_no_overshoot(void) {
    /* Smoothed blend should never exceed [0, 1] */
    auto_theme_state_t prev = at_from_sun_elevation(-20.0);
    auto_theme_state_t target = at_from_sun_elevation(30.0);

    for (int i = 0; i < 200; i++) {
        prev = at_smooth(prev, target, 0.016f, 5.0f);
        TEST_ASSERT_TRUE(prev.blend_factor >= 0.0f);
        TEST_ASSERT_TRUE(prev.blend_factor <= 1.0f);
    }

    /* Also test reverse direction */
    prev = at_from_sun_elevation(30.0);
    target = at_from_sun_elevation(-20.0);
    for (int i = 0; i < 200; i++) {
        prev = at_smooth(prev, target, 0.016f, 5.0f);
        TEST_ASSERT_TRUE(prev.blend_factor >= 0.0f);
        TEST_ASSERT_TRUE(prev.blend_factor <= 1.0f);
    }
}

/* ================================================================
 * CONVENIENCE: from location
 * ================================================================ */

void test_at_from_location(void) {
    /* at_from_location returns valid state with populated sun_elevation.
     * We test two points: one known-daytime and one known-nighttime.
     * JD+0.5 offset at lon=0 gives local noon-ish from daylight_at. */
    double jd_day = 2451545.5;   /* known: sun_elevation ~27 deg at lat=40, lon=0 */
    double jd_night = 2451545.0; /* known: sun_elevation ~-73 deg at lat=40, lon=0 */

    auto_theme_state_t day = at_from_location(jd_day, 40.0, 0.0);
    auto_theme_state_t night = at_from_location(jd_night, 40.0, 0.0);

    /* Day state should have higher blend_factor than night state */
    TEST_ASSERT_TRUE(day.blend_factor > night.blend_factor);
    /* Both should have valid blend_factor in [0,1] */
    TEST_ASSERT_TRUE(day.blend_factor >= 0.0f);
    TEST_ASSERT_TRUE(day.blend_factor <= 1.0f);
    TEST_ASSERT_TRUE(night.blend_factor >= 0.0f);
    TEST_ASSERT_TRUE(night.blend_factor <= 1.0f);
    /* sun_elevation should be populated */
    TEST_ASSERT_TRUE(day.sun_elevation_deg > night.sun_elevation_deg);
}

/* ================================================================
 * PURITY: same input gives same output
 * ================================================================ */

void test_at_purity_deterministic(void) {
    auto_theme_state_t s1 = at_from_sun_elevation(3.0);
    auto_theme_state_t s2 = at_from_sun_elevation(3.0);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, s1.blend_factor, s2.blend_factor);
    TEST_ASSERT_EQUAL_INT(s1.active_theme, s2.active_theme);
    TEST_ASSERT_EQUAL_INT(s1.is_transitioning, s2.is_transitioning);
}

/* ================================================================
 * MAIN
 * ================================================================ */

int main(void) {
    UNITY_BEGIN();

    /* Sun elevation -> theme state */
    RUN_TEST(test_at_full_day);
    RUN_TEST(test_at_full_night);
    RUN_TEST(test_at_sunrise_start);
    RUN_TEST(test_at_sunrise_mid);
    RUN_TEST(test_at_sunrise_end);
    RUN_TEST(test_at_sunset_mid);
    RUN_TEST(test_at_blend_monotonic);

    /* Blended theme */
    RUN_TEST(test_at_blended_theme_day);
    RUN_TEST(test_at_blended_theme_night);
    RUN_TEST(test_at_blended_theme_twilight);

    /* Effective theme (manual override) */
    RUN_TEST(test_at_effective_manual);
    RUN_TEST(test_at_effective_auto);

    /* Smoothing */
    RUN_TEST(test_at_smooth_converges);
    RUN_TEST(test_at_smooth_no_overshoot);

    /* Convenience */
    RUN_TEST(test_at_from_location);

    /* Purity */
    RUN_TEST(test_at_purity_deterministic);

    return UNITY_END();
}
