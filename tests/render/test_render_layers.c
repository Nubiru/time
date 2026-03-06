#include "../unity/unity.h"
#include "../../src/render/render_layers.h"
#include <math.h>

void setUp(void) {}
void tearDown(void) {}

/* --- default configs --- */

static void test_default_stars(void) {
    layer_config_t c = layer_default_config(LAYER_STARS);
    TEST_ASSERT_EQUAL_INT(LAYER_STARS, c.id);
    TEST_ASSERT_EQUAL_INT(SCALE_LOCAL, c.scale_min);
    TEST_ASSERT_EQUAL_INT(SCALE_UNIVERSE, c.scale_max);
    TEST_ASSERT_EQUAL_INT(1, c.user_visible);
}

static void test_default_hud(void) {
    layer_config_t c = layer_default_config(LAYER_HUD);
    TEST_ASSERT_EQUAL_INT(SCALE_PERSONAL, c.scale_min);
    TEST_ASSERT_EQUAL_INT(SCALE_UNIVERSE, c.scale_max);
}

static void test_default_invalid(void) {
    layer_config_t c = layer_default_config(99);
    TEST_ASSERT_EQUAL_INT(LAYER_HUD, c.id); /* fallback */
}

/* --- layer_opacity --- */

static void test_opacity_hud_always_visible(void) {
    layer_config_t c = layer_default_config(LAYER_HUD);
    /* HUD spans all scales — should be visible at solar system center */
    float o = layer_opacity(c, scale_center_log_zoom(SCALE_SOLAR));
    TEST_ASSERT_TRUE(o > 0.5f);
}

static void test_opacity_stars_at_universe(void) {
    layer_config_t c = layer_default_config(LAYER_STARS);
    float o = layer_opacity(c, scale_center_log_zoom(SCALE_UNIVERSE));
    TEST_ASSERT_TRUE(o > 0.0f);
}

static void test_opacity_stars_at_personal(void) {
    layer_config_t c = layer_default_config(LAYER_STARS);
    /* Stars not visible when zoomed in to personal chart */
    float o = layer_opacity(c, scale_center_log_zoom(SCALE_PERSONAL));
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, o);
}

static void test_opacity_planets_at_solar(void) {
    layer_config_t c = layer_default_config(LAYER_PLANETS);
    float o = layer_opacity(c, scale_center_log_zoom(SCALE_SOLAR));
    TEST_ASSERT_TRUE(o > 0.5f);
}

static void test_opacity_cards_at_personal(void) {
    layer_config_t c = layer_default_config(LAYER_CARDS);
    float o = layer_opacity(c, scale_center_log_zoom(SCALE_PERSONAL));
    TEST_ASSERT_TRUE(o > 0.0f);
}

static void test_opacity_cards_at_galaxy(void) {
    layer_config_t c = layer_default_config(LAYER_CARDS);
    float o = layer_opacity(c, scale_center_log_zoom(SCALE_GALAXY));
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, o);
}

static void test_opacity_user_toggled_off(void) {
    layer_config_t c = layer_default_config(LAYER_PLANETS);
    c.user_visible = 0;
    float o = layer_opacity(c, scale_center_log_zoom(SCALE_SOLAR));
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, o);
}

static void test_opacity_fade_in(void) {
    /* At the very edge of a layer's range, opacity should be low */
    layer_config_t c = layer_default_config(LAYER_STARS);
    scale_level_t smin = scale_get(c.scale_min);
    float just_inside = smin.log_zoom_min + 0.01f;
    float deeper = smin.log_zoom_min + 2.0f;
    float o_edge = layer_opacity(c, just_inside);
    float o_deep = layer_opacity(c, deeper);
    TEST_ASSERT_TRUE(o_edge < o_deep);
}

static void test_opacity_galaxy_at_inner(void) {
    layer_config_t c = layer_default_config(LAYER_GALAXY);
    float o = layer_opacity(c, scale_center_log_zoom(SCALE_INNER));
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, o);
}

/* --- layers_compute --- */

static void test_compute_all_at_solar(void) {
    layer_config_t configs[LAYER_COUNT];
    for (int i = 0; i < LAYER_COUNT; i++) {
        configs[i] = layer_default_config(i);
    }
    layer_state_t state = layers_compute(configs, scale_center_log_zoom(SCALE_SOLAR));

    /* At solar system scale: planets, orbits, zodiac, labels, HUD visible */
    TEST_ASSERT_TRUE(state.opacity[LAYER_PLANETS] > 0.0f);
    TEST_ASSERT_TRUE(state.opacity[LAYER_ORBITS] > 0.0f);
    TEST_ASSERT_TRUE(state.opacity[LAYER_ZODIAC_RING] > 0.0f);
    TEST_ASSERT_TRUE(state.opacity[LAYER_HUD] > 0.0f);

    /* Galaxy and cards NOT visible at solar scale */
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, state.opacity[LAYER_GALAXY]);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, state.opacity[LAYER_CARDS]);
}

/* --- layer_name --- */

static void test_name_planets(void) {
    TEST_ASSERT_EQUAL_STRING("Planets", layer_name(LAYER_PLANETS));
}

static void test_name_invalid(void) {
    TEST_ASSERT_EQUAL_STRING("Unknown", layer_name(99));
}

/* --- layer_is_visible --- */

static void test_is_visible_true(void) {
    layer_state_t state;
    state.opacity[LAYER_PLANETS] = 0.5f;
    TEST_ASSERT_EQUAL_INT(1, layer_is_visible(state, LAYER_PLANETS));
}

static void test_is_visible_false(void) {
    layer_state_t state;
    state.opacity[LAYER_PLANETS] = 0.0f;
    TEST_ASSERT_EQUAL_INT(0, layer_is_visible(state, LAYER_PLANETS));
}

static void test_is_visible_invalid(void) {
    layer_state_t state = {0};
    TEST_ASSERT_EQUAL_INT(0, layer_is_visible(state, 99));
}

/* --- purity --- */

static void test_purity(void) {
    layer_config_t c = layer_default_config(LAYER_STARS);
    float o1 = layer_opacity(c, 5.0f);
    float o2 = layer_opacity(c, 5.0f);
    TEST_ASSERT_EQUAL_FLOAT(o1, o2);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_default_stars);
    RUN_TEST(test_default_hud);
    RUN_TEST(test_default_invalid);
    RUN_TEST(test_opacity_hud_always_visible);
    RUN_TEST(test_opacity_stars_at_universe);
    RUN_TEST(test_opacity_stars_at_personal);
    RUN_TEST(test_opacity_planets_at_solar);
    RUN_TEST(test_opacity_cards_at_personal);
    RUN_TEST(test_opacity_cards_at_galaxy);
    RUN_TEST(test_opacity_user_toggled_off);
    RUN_TEST(test_opacity_fade_in);
    RUN_TEST(test_opacity_galaxy_at_inner);
    RUN_TEST(test_compute_all_at_solar);
    RUN_TEST(test_name_planets);
    RUN_TEST(test_name_invalid);
    RUN_TEST(test_is_visible_true);
    RUN_TEST(test_is_visible_false);
    RUN_TEST(test_is_visible_invalid);
    RUN_TEST(test_purity);
    return UNITY_END();
}
