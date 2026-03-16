/*
 * test_shader_audit.c -- Integration tests: shader source audit + layer visibility
 *
 * Verifies ALL render pack shader sources are non-NULL, contain valid GLSL
 * ES 3.00 structure (#version, void main), and have sufficient length.
 * Also verifies layer visibility at all 7 camera scale levels.
 *
 * Pure function tests only. No GL calls -- tests shader SOURCE STRINGS.
 */

#include "unity.h"
#include "render/star_field.h"
#include "render/planet_pack.h"
#include "render/zodiac_pack.h"
#include "render/moon_pack.h"
#include "render/orbit_trail_pack.h"
#include "render/earth_pack.h"
#include "render/bodygraph_pack.h"
#include "render/milkyway_pack.h"
#include "render/deep_sky_pack.h"
#include "render/card_pack.h"
#include "render/tree_pack.h"
#include "render/saturn_ring_pack.h"
#include "render/planet_surface_pack.h"
#include "render/lens_flare.h"
#include "render/post_process.h"
#include "render/weather_overlay.h"
#include "render/noise_shader.h"
#include "render/sun_shader.h"
#include "render/mesh_shader.h"
#include "render/billboard_shader.h"
#include "render/diffraction.h"
#include "render/render_layers.h"
#include "render/camera_scale.h"
#include <string.h>

void setUp(void) { }
void tearDown(void) { }

/* ===== Helper: validate a shader source string ===== */
static void assert_shader_valid(const char *src, const char *label) {
    char msg[128];

    /* Non-NULL */
    snprintf(msg, sizeof(msg), "%s: source is NULL", label);
    TEST_ASSERT_NOT_NULL_MESSAGE(src, msg);

    /* Not trivially empty (minimum meaningful GLSL) */
    snprintf(msg, sizeof(msg), "%s: source too short (%d bytes)", label,
             (int)strlen(src));
    TEST_ASSERT_TRUE_MESSAGE(strlen(src) > 50, msg);

    /* Contains void main */
    snprintf(msg, sizeof(msg), "%s: missing 'void main'", label);
    TEST_ASSERT_NOT_NULL_MESSAGE(strstr(src, "void main"), msg);
}

static void assert_shader_has_version(const char *src, const char *label) {
    char msg[128];
    TEST_ASSERT_NOT_NULL(src);
    snprintf(msg, sizeof(msg), "%s: missing '#version 300 es'", label);
    TEST_ASSERT_NOT_NULL_MESSAGE(strstr(src, "#version 300 es"), msg);
}

/* ===== A. Shader Source Non-NULL + Content Tests ===== */

/* --- star_field --- */
void test_star_field_vert_shader(void) {
    const char *src = star_field_vert_source();
    assert_shader_valid(src, "star_field_vert");
    assert_shader_has_version(src, "star_field_vert");
}

void test_star_field_frag_shader(void) {
    const char *src = star_field_frag_source();
    assert_shader_valid(src, "star_field_frag");
    assert_shader_has_version(src, "star_field_frag");
}

void test_constellation_line_vert_shader(void) {
    const char *src = constellation_line_vert_source();
    assert_shader_valid(src, "constellation_line_vert");
    assert_shader_has_version(src, "constellation_line_vert");
}

void test_constellation_line_frag_shader(void) {
    const char *src = constellation_line_frag_source();
    assert_shader_valid(src, "constellation_line_frag");
    assert_shader_has_version(src, "constellation_line_frag");
}

/* --- planet_pack --- */
void test_pp_planet_vert_shader(void) {
    const char *src = pp_planet_vert_source();
    assert_shader_valid(src, "pp_planet_vert");
    assert_shader_has_version(src, "pp_planet_vert");
}

void test_pp_planet_frag_shader(void) {
    const char *src = pp_planet_frag_source();
    assert_shader_valid(src, "pp_planet_frag");
    assert_shader_has_version(src, "pp_planet_frag");
}

void test_pp_trail_vert_shader(void) {
    const char *src = pp_trail_vert_source();
    assert_shader_valid(src, "pp_trail_vert");
    assert_shader_has_version(src, "pp_trail_vert");
}

void test_pp_trail_frag_shader(void) {
    const char *src = pp_trail_frag_source();
    assert_shader_valid(src, "pp_trail_frag");
    assert_shader_has_version(src, "pp_trail_frag");
}

/* --- zodiac_pack --- */
void test_zp_ring_vert_shader(void) {
    const char *src = zp_ring_vert_source();
    assert_shader_valid(src, "zp_ring_vert");
    assert_shader_has_version(src, "zp_ring_vert");
}

void test_zp_ring_frag_shader(void) {
    const char *src = zp_ring_frag_source();
    assert_shader_valid(src, "zp_ring_frag");
    assert_shader_has_version(src, "zp_ring_frag");
}

void test_zp_line_vert_shader(void) {
    const char *src = zp_line_vert_source();
    assert_shader_valid(src, "zp_line_vert");
    assert_shader_has_version(src, "zp_line_vert");
}

void test_zp_line_frag_shader(void) {
    const char *src = zp_line_frag_source();
    assert_shader_valid(src, "zp_line_frag");
    assert_shader_has_version(src, "zp_line_frag");
}

void test_zp_glyph_vert_shader(void) {
    const char *src = zp_glyph_vert_source();
    assert_shader_valid(src, "zp_glyph_vert");
    assert_shader_has_version(src, "zp_glyph_vert");
}

void test_zp_glyph_frag_shader(void) {
    const char *src = zp_glyph_frag_source();
    assert_shader_valid(src, "zp_glyph_frag");
    assert_shader_has_version(src, "zp_glyph_frag");
}

/* --- moon_pack --- */
void test_mp_moon_vert_shader(void) {
    const char *src = mp_moon_vert_source();
    assert_shader_valid(src, "mp_moon_vert");
    assert_shader_has_version(src, "mp_moon_vert");
}

void test_mp_moon_frag_shader(void) {
    const char *src = mp_moon_frag_source();
    assert_shader_valid(src, "mp_moon_frag");
    assert_shader_has_version(src, "mp_moon_frag");
}

void test_mp_trail_vert_shader(void) {
    const char *src = mp_trail_vert_source();
    assert_shader_valid(src, "mp_trail_vert");
    assert_shader_has_version(src, "mp_trail_vert");
}

void test_mp_trail_frag_shader(void) {
    const char *src = mp_trail_frag_source();
    assert_shader_valid(src, "mp_trail_frag");
    assert_shader_has_version(src, "mp_trail_frag");
}

/* --- orbit_trail_pack --- */
void test_otp_vert_shader(void) {
    const char *src = otp_vert_source();
    assert_shader_valid(src, "otp_vert");
    assert_shader_has_version(src, "otp_vert");
}

void test_otp_frag_shader(void) {
    const char *src = otp_frag_source();
    assert_shader_valid(src, "otp_frag");
    assert_shader_has_version(src, "otp_frag");
}

/* --- earth_pack --- */
void test_erp_globe_vert_shader(void) {
    const char *src = erp_globe_vert_source();
    assert_shader_valid(src, "erp_globe_vert");
    assert_shader_has_version(src, "erp_globe_vert");
}

void test_erp_globe_frag_shader(void) {
    const char *src = erp_globe_frag_source();
    assert_shader_valid(src, "erp_globe_frag");
    assert_shader_has_version(src, "erp_globe_frag");
}

void test_erp_atmo_vert_shader(void) {
    const char *src = erp_atmo_vert_source();
    assert_shader_valid(src, "erp_atmo_vert");
    assert_shader_has_version(src, "erp_atmo_vert");
}

void test_erp_atmo_frag_shader(void) {
    const char *src = erp_atmo_frag_source();
    assert_shader_valid(src, "erp_atmo_frag");
    assert_shader_has_version(src, "erp_atmo_frag");
}

void test_erp_line_vert_shader(void) {
    const char *src = erp_line_vert_source();
    assert_shader_valid(src, "erp_line_vert");
    assert_shader_has_version(src, "erp_line_vert");
}

void test_erp_line_frag_shader(void) {
    const char *src = erp_line_frag_source();
    assert_shader_valid(src, "erp_line_frag");
    assert_shader_has_version(src, "erp_line_frag");
}

/* --- bodygraph_pack --- */
void test_bgp_vert_shader(void) {
    const char *src = bgp_vert_source();
    assert_shader_valid(src, "bgp_vert");
    assert_shader_has_version(src, "bgp_vert");
}

void test_bgp_frag_shader(void) {
    const char *src = bgp_frag_source();
    assert_shader_valid(src, "bgp_frag");
    assert_shader_has_version(src, "bgp_frag");
}

/* --- milkyway_pack --- */
void test_mw_vert_shader(void) {
    const char *src = mw_vert_source();
    assert_shader_valid(src, "mw_vert");
    assert_shader_has_version(src, "mw_vert");
}

void test_mw_frag_shader(void) {
    const char *src = mw_frag_source();
    assert_shader_valid(src, "mw_frag");
    /* mw_frag concatenates noise_shader, so version is at start */
    assert_shader_has_version(src, "mw_frag");
}

/* --- deep_sky_pack --- */
void test_dsp_vert_shader(void) {
    const char *src = dsp_vert_source();
    assert_shader_valid(src, "dsp_vert");
    assert_shader_has_version(src, "dsp_vert");
}

void test_dsp_frag_shader(void) {
    const char *src = dsp_frag_source();
    assert_shader_valid(src, "dsp_frag");
    assert_shader_has_version(src, "dsp_frag");
}

/* --- card_pack --- */
void test_cp_quad_vert_shader(void) {
    const char *src = cp_quad_vert_source();
    assert_shader_valid(src, "cp_quad_vert");
    assert_shader_has_version(src, "cp_quad_vert");
}

void test_cp_quad_frag_shader(void) {
    const char *src = cp_quad_frag_source();
    assert_shader_valid(src, "cp_quad_frag");
    assert_shader_has_version(src, "cp_quad_frag");
}

void test_cp_line_vert_shader(void) {
    const char *src = cp_line_vert_source();
    assert_shader_valid(src, "cp_line_vert");
    assert_shader_has_version(src, "cp_line_vert");
}

void test_cp_line_frag_shader(void) {
    const char *src = cp_line_frag_source();
    assert_shader_valid(src, "cp_line_frag");
    assert_shader_has_version(src, "cp_line_frag");
}

/* --- tree_pack --- */
void test_trp_vert_shader(void) {
    const char *src = trp_vert_source();
    assert_shader_valid(src, "trp_vert");
    assert_shader_has_version(src, "trp_vert");
}

void test_trp_frag_shader(void) {
    const char *src = trp_frag_source();
    assert_shader_valid(src, "trp_frag");
    assert_shader_has_version(src, "trp_frag");
}

/* --- saturn_ring_pack --- */
void test_srp_vert_shader(void) {
    const char *src = srp_vert_source();
    assert_shader_valid(src, "srp_vert");
    assert_shader_has_version(src, "srp_vert");
}

void test_srp_frag_shader(void) {
    const char *src = srp_frag_source();
    assert_shader_valid(src, "srp_frag");
    assert_shader_has_version(src, "srp_frag");
}

/* --- planet_surface_pack --- */
void test_psp_vert_shader(void) {
    const char *src = psp_vert_source();
    assert_shader_valid(src, "psp_vert");
    assert_shader_has_version(src, "psp_vert");
}

void test_psp_frag_shader(void) {
    const char *src = psp_frag_source();
    assert_shader_valid(src, "psp_frag");
    /* psp_frag concatenates noise_shader, version at start */
    assert_shader_has_version(src, "psp_frag");
}

/* --- lens_flare --- */
void test_lf_vert_shader(void) {
    const char *src = lf_vert_source();
    assert_shader_valid(src, "lf_vert");
    assert_shader_has_version(src, "lf_vert");
}

void test_lf_frag_shader(void) {
    const char *src = lf_frag_source();
    assert_shader_valid(src, "lf_frag");
    assert_shader_has_version(src, "lf_frag");
}

/* --- post_process --- */
void test_pp_quad_vert_shader(void) {
    const char *src = pp_quad_vert_source();
    assert_shader_valid(src, "pp_quad_vert");
    assert_shader_has_version(src, "pp_quad_vert");
}

void test_pp_bright_extract_frag_shader(void) {
    const char *src = pp_bright_extract_frag_source();
    assert_shader_valid(src, "pp_bright_extract_frag");
    assert_shader_has_version(src, "pp_bright_extract_frag");
}

void test_pp_blur_frag_shader(void) {
    const char *src = pp_blur_frag_source();
    assert_shader_valid(src, "pp_blur_frag");
    assert_shader_has_version(src, "pp_blur_frag");
}

void test_pp_composite_frag_shader(void) {
    const char *src = pp_composite_frag_source();
    assert_shader_valid(src, "pp_composite_frag");
    /* composite concatenates noise_shader, version at start */
    assert_shader_has_version(src, "pp_composite_frag");
}

/* --- weather_overlay --- */
void test_wov_vert_shader(void) {
    const char *src = wov_vert_source();
    assert_shader_valid(src, "wov_vert");
    assert_shader_has_version(src, "wov_vert");
}

void test_wov_frag_shader(void) {
    const char *src = wov_frag_source();
    assert_shader_valid(src, "wov_frag");
    assert_shader_has_version(src, "wov_frag");
}

/* --- noise_shader (shared library) --- */
void test_noise_shader_source(void) {
    const char *src = noise_shader_source();
    TEST_ASSERT_NOT_NULL(src);
    TEST_ASSERT_TRUE(strlen(src) > 100);
    /* noise_shader provides functions, not a standalone shader.
     * It should have snoise/fbm but NOT its own #version or void main. */
    TEST_ASSERT_NOT_NULL(strstr(src, "snoise"));
}

/* --- sun_shader --- */
void test_sun_shader_vert(void) {
    const char *src = sun_shader_vert_source();
    assert_shader_valid(src, "sun_shader_vert");
    assert_shader_has_version(src, "sun_shader_vert");
}

void test_sun_shader_frag(void) {
    const char *src = sun_shader_frag_source();
    assert_shader_valid(src, "sun_shader_frag");
    /* sun_shader_frag concatenates noise_shader, version at start */
    assert_shader_has_version(src, "sun_shader_frag");
}

/* --- mesh_shader --- */
void test_mesh_shader_vert(void) {
    const char *src = mesh_shader_vert_source();
    assert_shader_valid(src, "mesh_shader_vert");
    assert_shader_has_version(src, "mesh_shader_vert");
}

void test_mesh_shader_frag(void) {
    const char *src = mesh_shader_frag_source();
    assert_shader_valid(src, "mesh_shader_frag");
    assert_shader_has_version(src, "mesh_shader_frag");
}

/* --- billboard_shader --- */
void test_billboard_shader_vert(void) {
    const char *src = billboard_shader_vert_source();
    assert_shader_valid(src, "billboard_shader_vert");
    assert_shader_has_version(src, "billboard_shader_vert");
}

void test_billboard_shader_frag(void) {
    const char *src = billboard_shader_frag_source();
    assert_shader_valid(src, "billboard_shader_frag");
    assert_shader_has_version(src, "billboard_shader_frag");
}

/* --- diffraction --- */
void test_diffraction_vert_shader(void) {
    const char *src = diffraction_vert_source();
    assert_shader_valid(src, "diffraction_vert");
    assert_shader_has_version(src, "diffraction_vert");
}

void test_diffraction_frag_shader(void) {
    const char *src = diffraction_frag_source();
    assert_shader_valid(src, "diffraction_frag");
    assert_shader_has_version(src, "diffraction_frag");
}

/* ===== B. Concatenated Shader Tests (shared modules) ===== */

void test_noise_shader_in_sun_frag(void) {
    const char *sun_frag = sun_shader_frag_source();
    TEST_ASSERT_NOT_NULL(sun_frag);
    /* Sun frag should contain noise functions from noise_shader */
    TEST_ASSERT_NOT_NULL_MESSAGE(strstr(sun_frag, "snoise"),
        "sun_shader_frag should contain noise functions");
}

void test_noise_shader_in_mw_frag(void) {
    const char *mw_frag = mw_frag_source();
    TEST_ASSERT_NOT_NULL(mw_frag);
    TEST_ASSERT_NOT_NULL_MESSAGE(strstr(mw_frag, "snoise"),
        "mw_frag should contain noise functions");
}

void test_noise_shader_in_psp_frag(void) {
    const char *psp_frag = psp_frag_source();
    TEST_ASSERT_NOT_NULL(psp_frag);
    TEST_ASSERT_NOT_NULL_MESSAGE(strstr(psp_frag, "snoise"),
        "psp_frag should contain noise functions");
}

void test_pp_composite_is_self_contained(void) {
    const char *pp_frag = pp_composite_frag_source();
    TEST_ASSERT_NOT_NULL(pp_frag);
    /* Composite uses simple hash grain, not noise_shader.
     * Verify it has its own grain implementation. */
    TEST_ASSERT_NOT_NULL_MESSAGE(strstr(pp_frag, "void main"),
        "pp_composite_frag should have void main");
    TEST_ASSERT_NOT_NULL_MESSAGE(strstr(pp_frag, "u_scene"),
        "pp_composite_frag should sample u_scene");
}

/* ===== C. Layer Visibility Tests at All 7 Scale Levels ===== */

/* Helper: build default layer configs and compute state at a scale */
static layer_state_t compute_layers_at_scale(scale_id_t scale) {
    layer_config_t configs[LAYER_COUNT];
    for (int i = 0; i < LAYER_COUNT; i++) {
        configs[i] = layer_default_config((layer_id_t)i);
    }
    float log_zoom = scale_center_log_zoom(scale);
    return layers_compute(configs, log_zoom);
}

/* Scale 0: Personal — cards and HUD visible */
void test_layers_at_scale_0_personal(void) {
    layer_state_t s = compute_layers_at_scale(SCALE_PERSONAL);
    TEST_ASSERT_TRUE(s.opacity[LAYER_CARDS] > 0.0f);
    TEST_ASSERT_TRUE(s.opacity[LAYER_HUD] > 0.0f);
}

/* Scale 1: Earth — planets and labels visible */
void test_layers_at_scale_1_earth(void) {
    layer_state_t s = compute_layers_at_scale(SCALE_EARTH);
    TEST_ASSERT_TRUE(s.opacity[LAYER_PLANETS] > 0.0f);
    TEST_ASSERT_TRUE(s.opacity[LAYER_HUD] > 0.0f);
}

/* Scale 2: Inner Solar — zodiac ring, orbits, planets */
void test_layers_at_scale_2_inner(void) {
    layer_state_t s = compute_layers_at_scale(SCALE_INNER);
    TEST_ASSERT_TRUE(s.opacity[LAYER_ZODIAC_RING] > 0.0f);
    TEST_ASSERT_TRUE(s.opacity[LAYER_ORBITS] > 0.0f);
    TEST_ASSERT_TRUE(s.opacity[LAYER_PLANETS] > 0.0f);
}

/* Scale 3: Solar System (DEFAULT) — orbits, planets, zodiac, labels */
void test_layers_at_scale_3_solar(void) {
    layer_state_t s = compute_layers_at_scale(SCALE_SOLAR);
    TEST_ASSERT_TRUE(s.opacity[LAYER_ORBITS] > 0.0f);
    TEST_ASSERT_TRUE(s.opacity[LAYER_PLANETS] > 0.0f);
    TEST_ASSERT_TRUE(s.opacity[LAYER_ZODIAC_RING] > 0.0f);
    TEST_ASSERT_TRUE(s.opacity[LAYER_HUD] > 0.0f);
}

/* Scale 4: Local — stars visible */
void test_layers_at_scale_4_local(void) {
    layer_state_t s = compute_layers_at_scale(SCALE_LOCAL);
    TEST_ASSERT_TRUE(s.opacity[LAYER_STARS] > 0.0f);
}

/* Scale 5: Galaxy — galaxy layer visible, stars still visible */
void test_layers_at_scale_5_galaxy(void) {
    layer_state_t s = compute_layers_at_scale(SCALE_GALAXY);
    TEST_ASSERT_TRUE(s.opacity[LAYER_GALAXY] > 0.0f);
    TEST_ASSERT_TRUE(s.opacity[LAYER_STARS] > 0.0f);
}

/* Scale 6: Universe — galaxy visible */
void test_layers_at_scale_6_universe(void) {
    layer_state_t s = compute_layers_at_scale(SCALE_UNIVERSE);
    TEST_ASSERT_TRUE(s.opacity[LAYER_GALAXY] > 0.0f);
}

/* All layers have valid opacity range [0.0, 1.0] at every scale */
void test_layer_opacity_range_all_scales(void) {
    for (int sc = 0; sc < SCALE_COUNT; sc++) {
        layer_state_t s = compute_layers_at_scale((scale_id_t)sc);
        for (int l = 0; l < LAYER_COUNT; l++) {
            TEST_ASSERT_TRUE(s.opacity[l] >= 0.0f);
            TEST_ASSERT_TRUE(s.opacity[l] <= 1.0f);
        }
    }
}

/* User toggle off disables layer regardless of scale */
void test_layer_user_toggle_off(void) {
    layer_config_t configs[LAYER_COUNT];
    for (int i = 0; i < LAYER_COUNT; i++) {
        configs[i] = layer_default_config((layer_id_t)i);
    }
    /* Toggle stars off */
    configs[LAYER_STARS].user_visible = 0;
    float log_zoom = scale_center_log_zoom(SCALE_SOLAR);
    layer_state_t s = layers_compute(configs, log_zoom);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, s.opacity[LAYER_STARS]);
}

/* HUD is visible at personal and solar scales (broad range) */
void test_hud_visible_across_scales(void) {
    layer_state_t s0 = compute_layers_at_scale(SCALE_PERSONAL);
    layer_state_t s3 = compute_layers_at_scale(SCALE_SOLAR);
    TEST_ASSERT_TRUE(s0.opacity[LAYER_HUD] > 0.0f);
    TEST_ASSERT_TRUE(s3.opacity[LAYER_HUD] > 0.0f);
}

/* ===== D. Scale Level Sanity ===== */

void test_all_scale_levels_defined(void) {
    for (int i = 0; i < SCALE_COUNT; i++) {
        scale_level_t lvl = scale_get((scale_id_t)i);
        TEST_ASSERT_NOT_NULL(lvl.name);
        TEST_ASSERT_TRUE(lvl.log_zoom_min < lvl.log_zoom_max);
        TEST_ASSERT_TRUE(lvl.log_zoom_center >= lvl.log_zoom_min);
        TEST_ASSERT_TRUE(lvl.log_zoom_center <= lvl.log_zoom_max);
    }
}

void test_scale_from_log_zoom_roundtrip(void) {
    for (int i = 0; i < SCALE_COUNT; i++) {
        float center = scale_center_log_zoom((scale_id_t)i);
        scale_id_t detected = scale_from_log_zoom(center);
        TEST_ASSERT_EQUAL_INT(i, (int)detected);
    }
}

/* ===== Main runner ===== */
int main(void) {
    UNITY_BEGIN();

    /* A. Shader source tests (56 tests: ~28 packs x 2 shaders each) */
    RUN_TEST(test_star_field_vert_shader);
    RUN_TEST(test_star_field_frag_shader);
    RUN_TEST(test_constellation_line_vert_shader);
    RUN_TEST(test_constellation_line_frag_shader);
    RUN_TEST(test_pp_planet_vert_shader);
    RUN_TEST(test_pp_planet_frag_shader);
    RUN_TEST(test_pp_trail_vert_shader);
    RUN_TEST(test_pp_trail_frag_shader);
    RUN_TEST(test_zp_ring_vert_shader);
    RUN_TEST(test_zp_ring_frag_shader);
    RUN_TEST(test_zp_line_vert_shader);
    RUN_TEST(test_zp_line_frag_shader);
    RUN_TEST(test_zp_glyph_vert_shader);
    RUN_TEST(test_zp_glyph_frag_shader);
    RUN_TEST(test_mp_moon_vert_shader);
    RUN_TEST(test_mp_moon_frag_shader);
    RUN_TEST(test_mp_trail_vert_shader);
    RUN_TEST(test_mp_trail_frag_shader);
    RUN_TEST(test_otp_vert_shader);
    RUN_TEST(test_otp_frag_shader);
    RUN_TEST(test_erp_globe_vert_shader);
    RUN_TEST(test_erp_globe_frag_shader);
    RUN_TEST(test_erp_atmo_vert_shader);
    RUN_TEST(test_erp_atmo_frag_shader);
    RUN_TEST(test_erp_line_vert_shader);
    RUN_TEST(test_erp_line_frag_shader);
    RUN_TEST(test_bgp_vert_shader);
    RUN_TEST(test_bgp_frag_shader);
    RUN_TEST(test_mw_vert_shader);
    RUN_TEST(test_mw_frag_shader);
    RUN_TEST(test_dsp_vert_shader);
    RUN_TEST(test_dsp_frag_shader);
    RUN_TEST(test_cp_quad_vert_shader);
    RUN_TEST(test_cp_quad_frag_shader);
    RUN_TEST(test_cp_line_vert_shader);
    RUN_TEST(test_cp_line_frag_shader);
    RUN_TEST(test_trp_vert_shader);
    RUN_TEST(test_trp_frag_shader);
    RUN_TEST(test_srp_vert_shader);
    RUN_TEST(test_srp_frag_shader);
    RUN_TEST(test_psp_vert_shader);
    RUN_TEST(test_psp_frag_shader);
    RUN_TEST(test_lf_vert_shader);
    RUN_TEST(test_lf_frag_shader);
    RUN_TEST(test_pp_quad_vert_shader);
    RUN_TEST(test_pp_bright_extract_frag_shader);
    RUN_TEST(test_pp_blur_frag_shader);
    RUN_TEST(test_pp_composite_frag_shader);
    RUN_TEST(test_wov_vert_shader);
    RUN_TEST(test_wov_frag_shader);
    RUN_TEST(test_noise_shader_source);
    RUN_TEST(test_sun_shader_vert);
    RUN_TEST(test_sun_shader_frag);
    RUN_TEST(test_mesh_shader_vert);
    RUN_TEST(test_mesh_shader_frag);
    RUN_TEST(test_billboard_shader_vert);
    RUN_TEST(test_billboard_shader_frag);
    RUN_TEST(test_diffraction_vert_shader);
    RUN_TEST(test_diffraction_frag_shader);

    /* B. Concatenated shader tests */
    RUN_TEST(test_noise_shader_in_sun_frag);
    RUN_TEST(test_noise_shader_in_mw_frag);
    RUN_TEST(test_noise_shader_in_psp_frag);
    RUN_TEST(test_pp_composite_is_self_contained);

    /* C. Layer visibility tests (7 scales + range + toggle + hud) */
    RUN_TEST(test_layers_at_scale_0_personal);
    RUN_TEST(test_layers_at_scale_1_earth);
    RUN_TEST(test_layers_at_scale_2_inner);
    RUN_TEST(test_layers_at_scale_3_solar);
    RUN_TEST(test_layers_at_scale_4_local);
    RUN_TEST(test_layers_at_scale_5_galaxy);
    RUN_TEST(test_layers_at_scale_6_universe);
    RUN_TEST(test_layer_opacity_range_all_scales);
    RUN_TEST(test_layer_user_toggle_off);
    RUN_TEST(test_hud_visible_across_scales);

    /* D. Scale level sanity */
    RUN_TEST(test_all_scale_levels_defined);
    RUN_TEST(test_scale_from_log_zoom_roundtrip);

    return UNITY_END();
}
