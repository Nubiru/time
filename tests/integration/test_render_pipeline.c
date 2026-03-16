/*
 * test_render_pipeline.c — Integration tests for render data packs
 *
 * Verifies that all render data packs produce valid output for known inputs.
 * Tests pack functions end-to-end: camera -> render_frame -> packs -> vertex data.
 *
 * Pure function tests only. No GL calls.
 */

#include "unity.h"
#include "math/vec3.h"
#include "math/mat4.h"
#include "render/camera.h"
#include "render/camera_scale.h"
#include "render/render_layers.h"
#include "render/render_frame.h"
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
#include <math.h>
#include <string.h>

#define PI 3.14159265358979323846
#define J2000 2451545.0

void setUp(void) { }
void tearDown(void) { }

/* ===== Helper: check a float buffer for NAN/INF ===== */
static int buffer_has_nan_inf(const float *buf, int count) {
    for (int i = 0; i < count; i++) {
        if (isnan(buf[i]) || isinf(buf[i])) return 1;
    }
    return 0;
}

/* ===== Test: render_frame_t construction ===== */
void test_render_frame_construction(void) {
    camera_t cam = camera_create(50.0f, PI / 4.0f, 16.0f / 9.0f);
    mat4_t view = camera_view_matrix(&cam);
    mat4_t proj = camera_projection_matrix(&cam);
    mat4_t vp = mat4_multiply(proj, view);

    layer_config_t configs[LAYER_COUNT];
    for (int i = 0; i < LAYER_COUNT; i++) {
        configs[i] = layer_default_config((layer_id_t)i);
    }
    layer_state_t layers = layers_compute(configs, cam.log_zoom);

    render_frame_t frame;
    frame.simulation_jd = J2000;
    frame.time_sec = 0.0f;
    frame.view = view;
    frame.proj = proj;
    frame.view_proj = vp;
    frame.layers = layers;
    frame.show_trails = 1;
    frame.observer_lat = 40.0;
    frame.observer_lon = -74.0;

    /* Verify matrices have no NAN/INF */
    TEST_ASSERT_FALSE(buffer_has_nan_inf(frame.view.m, 16));
    TEST_ASSERT_FALSE(buffer_has_nan_inf(frame.proj.m, 16));
    TEST_ASSERT_FALSE(buffer_has_nan_inf(frame.view_proj.m, 16));
    /* JD should be valid */
    TEST_ASSERT_FLOAT_WITHIN(0.001, J2000, (float)frame.simulation_jd);
}

/* ===== Test: camera + render_frame work together ===== */
void test_camera_render_frame_integration(void) {
    camera_t cam = camera_create(10.0f, PI / 3.0f, 1.0f);
    camera_rotate(&cam, 0.5f, 0.3f);
    camera_zoom(&cam, -0.2f);

    mat4_t view = camera_view_matrix(&cam);
    mat4_t proj = camera_projection_matrix(&cam);
    mat4_t vp = mat4_multiply(proj, view);

    TEST_ASSERT_FALSE(buffer_has_nan_inf(view.m, 16));
    TEST_ASSERT_FALSE(buffer_has_nan_inf(proj.m, 16));
    TEST_ASSERT_FALSE(buffer_has_nan_inf(vp.m, 16));

    /* Camera position should be valid */
    vec3_t pos = camera_position(&cam);
    TEST_ASSERT_FALSE(isnan(pos.x) || isnan(pos.y) || isnan(pos.z));
    TEST_ASSERT_TRUE(vec3_length(pos) > 0.0f);
}

/* ===== Test: star_field pack produces valid output ===== */
void test_star_field_pack_j2000(void) {
    float buf[600 * STAR_VERTEX_FLOATS];
    int count = star_field_pack(buf, 600, 8.0f, 100.0f);

    TEST_ASSERT_TRUE(count > 0);
    TEST_ASSERT_FALSE(buffer_has_nan_inf(buf, count * STAR_VERTEX_FLOATS));

    star_field_info_t info = star_field_info(count);
    TEST_ASSERT_EQUAL(count, info.vertex_count);
    TEST_ASSERT_TRUE(info.float_count > 0);
}

/* ===== Test: planet_pack produces valid output at J2000 ===== */
void test_planet_pack_j2000(void) {
    pp_planet_data_t data = pp_pack_planets(J2000, 10.0f, 0.5f);

    TEST_ASSERT_EQUAL(PP_PLANET_COUNT, data.planet_count);
    TEST_ASSERT_FALSE(buffer_has_nan_inf(data.vertices,
                      data.planet_count * PP_PLANET_VERTEX_FLOATS));
}

/* ===== Test: planet trail pack produces valid output ===== */
void test_planet_trail_pack_j2000(void) {
    pp_trail_data_t trails = pp_pack_trails(J2000, 10.0f, 60);

    TEST_ASSERT_TRUE(trails.total_vertex_count > 0);
    TEST_ASSERT_FALSE(buffer_has_nan_inf(trails.vertices,
                      trails.total_vertex_count * PP_TRAIL_VERTEX_FLOATS));
}

/* ===== Test: zodiac ring pack produces valid output ===== */
void test_zodiac_ring_pack(void) {
    zp_ring_data_t ring = zp_pack_ring(8.0f, 10.0f, 8, NULL);

    TEST_ASSERT_TRUE(ring.vertex_count > 0);
    TEST_ASSERT_TRUE(ring.index_count > 0);
    TEST_ASSERT_FALSE(buffer_has_nan_inf(ring.vertices,
                      ring.vertex_count * ZP_RING_VERTEX_FLOATS));
}

/* ===== Test: moon_pack produces valid output at J2000 ===== */
void test_moon_pack_j2000(void) {
    mp_moon_data_t moons = mp_pack_moons(J2000, 10.0f, 5.0f, 0.3f);

    TEST_ASSERT_TRUE(moons.moon_count > 0);
    TEST_ASSERT_TRUE(moons.moon_count <= MP_MAX_MOONS);
    TEST_ASSERT_FALSE(buffer_has_nan_inf(moons.vertices,
                      moons.moon_count * MP_VERTEX_FLOATS));
}

/* ===== Test: orbit_trail_pack produces valid output ===== */
void test_orbit_trail_pack_j2000(void) {
    float verts[OTP_MAX_VERTICES * OTP_VERTEX_FLOATS];
    otp_config_t cfg = otp_default_config();
    otp_info_t info = otp_pack(J2000, cfg, verts);

    TEST_ASSERT_TRUE(info.vertex_count > 0);
    TEST_ASSERT_EQUAL(8, info.orbit_count);
    TEST_ASSERT_FALSE(buffer_has_nan_inf(verts,
                      info.vertex_count * OTP_VERTEX_FLOATS));
}

/* ===== Test: earth_pack produces valid output ===== */
void test_earth_pack_globe(void) {
    erp_config_t cfg = erp_default_config();
    erp_info_t info = erp_info(&cfg);

    TEST_ASSERT_TRUE(info.globe_verts > 0);
    TEST_ASSERT_TRUE(info.globe_indices > 0);

    /* Pack globe mesh */
    float globe_verts[info.globe_verts * ERP_GLOBE_VERTEX_FLOATS];
    unsigned int globe_indices[info.globe_indices];
    int count = erp_pack_globe(&cfg, globe_verts, globe_indices);

    TEST_ASSERT_TRUE(count > 0);
    TEST_ASSERT_FALSE(buffer_has_nan_inf(globe_verts,
                      count * ERP_GLOBE_VERTEX_FLOATS));
}

/* ===== Test: bodygraph_pack produces valid output ===== */
void test_bodygraph_pack_output(void) {
    int defined[9] = {1, 0, 1, 0, 1, 0, 1, 0, 1};
    int gates[65];
    memset(gates, 0, sizeof(gates));
    gates[1] = 1; gates[13] = 1; gates[33] = 1;

    bgp_config_t cfg = bgp_default_config();
    float verts[BGP_MAX_VERTICES * BGP_VERTEX_FLOATS];
    int total = bgp_pack(defined, gates, &cfg, verts);

    TEST_ASSERT_TRUE(total > 0);
    TEST_ASSERT_FALSE(buffer_has_nan_inf(verts, total * BGP_VERTEX_FLOATS));

    bgp_info_t info = bgp_info(defined, gates);
    TEST_ASSERT_TRUE(info.total_verts > 0);
    TEST_ASSERT_EQUAL(5, info.defined_centers);
}

/* ===== Test: milkyway_pack produces valid output ===== */
void test_milkyway_pack_output(void) {
    mw_config_t cfg = mw_default_config();
    float verts[MW_MAX_VERTICES * MW_VERTEX_FLOATS];
    unsigned short indices[MW_MAX_VERTICES * 6];

    mw_info_t info = mw_pack(cfg, verts, indices);

    TEST_ASSERT_TRUE(info.vertex_count > 0);
    TEST_ASSERT_TRUE(info.index_count > 0);
    TEST_ASSERT_TRUE(info.triangle_count > 0);
    TEST_ASSERT_FALSE(buffer_has_nan_inf(verts,
                      info.vertex_count * MW_VERTEX_FLOATS));
}

/* ===== Test: saturn_ring_pack produces valid output ===== */
void test_saturn_ring_pack_output(void) {
    srp_config_t cfg = srp_default_config();
    float verts[SRP_MAX_VERTICES * SRP_VERTEX_FLOATS];
    unsigned short indices[SRP_MAX_VERTICES * 6];

    srp_info_t info = srp_pack(cfg, verts, indices);

    TEST_ASSERT_TRUE(info.vertex_count > 0);
    TEST_ASSERT_TRUE(info.index_count > 0);
    TEST_ASSERT_EQUAL(srp_ring_count(), info.ring_count);
    TEST_ASSERT_FALSE(buffer_has_nan_inf(verts,
                      info.vertex_count * SRP_VERTEX_FLOATS));
}

/* ===== Test: tree_pack (Kabbalah) produces valid output ===== */
void test_tree_pack_output(void) {
    int active_nodes[11] = {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0};
    int active_paths[22];
    memset(active_paths, 0, sizeof(active_paths));
    active_paths[0] = 1; active_paths[5] = 1; active_paths[10] = 1;

    trp_config_t cfg = trp_default_config();
    float verts[TRP_MAX_VERTICES * TRP_VERTEX_FLOATS];
    int total = trp_pack(active_nodes, active_paths, &cfg, verts);

    TEST_ASSERT_TRUE(total > 0);
    TEST_ASSERT_FALSE(buffer_has_nan_inf(verts, total * TRP_VERTEX_FLOATS));
}

/* ===== Test: lens_flare pack produces valid output ===== */
void test_lens_flare_pack_output(void) {
    lf_config_t cfg = lf_sun_config();
    float source_ndc[2] = {0.2f, 0.3f};
    float verts[LF_MAX_VERTICES * LF_VERTEX_FLOATS];

    int count = lf_pack(cfg, source_ndc, 16.0f / 9.0f, verts, LF_MAX_VERTICES);

    TEST_ASSERT_TRUE(count > 0);
    TEST_ASSERT_FALSE(buffer_has_nan_inf(verts, count * LF_VERTEX_FLOATS));
}

/* ===== Test: post_process quad produces valid output ===== */
void test_post_process_quad_output(void) {
    float verts[PP_QUAD_VERTEX_COUNT * PP_QUAD_VERTEX_FLOATS];
    int count = pp_pack_quad(verts);

    TEST_ASSERT_EQUAL(PP_QUAD_VERTEX_COUNT, count);
    TEST_ASSERT_FALSE(buffer_has_nan_inf(verts,
                      count * PP_QUAD_VERTEX_FLOATS));
}

/* ===== Test: weather_overlay produces valid output ===== */
void test_weather_overlay_pack_output(void) {
    wov_config_t cfg = wov_default_config();
    float verts[WOV_MAX_VERTICES * WOV_VERTEX_FLOATS];

    wov_info_t info = wov_pack(cfg, verts);

    TEST_ASSERT_TRUE(info.total_verts > 0);
    TEST_ASSERT_TRUE(info.wind_arrow_verts >= 0);
    TEST_ASSERT_FALSE(buffer_has_nan_inf(verts,
                      info.total_verts * WOV_VERTEX_FLOATS));
}

/* ===== Test: layer visibility across scales ===== */
void test_layer_visibility_at_scales(void) {
    layer_config_t configs[LAYER_COUNT];
    for (int i = 0; i < LAYER_COUNT; i++) {
        configs[i] = layer_default_config((layer_id_t)i);
    }

    /* Test at each scale center */
    for (int s = 0; s < SCALE_COUNT; s++) {
        float log_zoom = scale_center_log_zoom((scale_id_t)s);
        layer_state_t state = layers_compute(configs, log_zoom);

        /* All opacities must be in valid range */
        for (int l = 0; l < LAYER_COUNT; l++) {
            TEST_ASSERT_TRUE(state.opacity[l] >= 0.0f);
            TEST_ASSERT_TRUE(state.opacity[l] <= 1.0f);
            TEST_ASSERT_FALSE(isnan(state.opacity[l]));
        }
    }
}

/* ===== Test: all shader sources are non-NULL ===== */
void test_all_shader_sources_non_null(void) {
    /* Star field shaders */
    TEST_ASSERT_NOT_NULL(star_field_vert_source());
    TEST_ASSERT_NOT_NULL(star_field_frag_source());

    /* Planet pack shaders */
    TEST_ASSERT_NOT_NULL(pp_planet_vert_source());
    TEST_ASSERT_NOT_NULL(pp_planet_frag_source());

    /* Zodiac pack shaders */
    TEST_ASSERT_NOT_NULL(zp_ring_vert_source());
    TEST_ASSERT_NOT_NULL(zp_ring_frag_source());

    /* Moon pack shaders */
    TEST_ASSERT_NOT_NULL(mp_moon_vert_source());
    TEST_ASSERT_NOT_NULL(mp_moon_frag_source());

    /* Orbit trail pack shaders */
    TEST_ASSERT_NOT_NULL(otp_vert_source());
    TEST_ASSERT_NOT_NULL(otp_frag_source());

    /* Earth pack shaders */
    TEST_ASSERT_NOT_NULL(erp_globe_vert_source());
    TEST_ASSERT_NOT_NULL(erp_globe_frag_source());

    /* Bodygraph pack shaders */
    TEST_ASSERT_NOT_NULL(bgp_vert_source());
    TEST_ASSERT_NOT_NULL(bgp_frag_source());

    /* Milky Way pack shaders */
    TEST_ASSERT_NOT_NULL(mw_vert_source());
    TEST_ASSERT_NOT_NULL(mw_frag_source());

    /* Saturn ring pack shaders */
    TEST_ASSERT_NOT_NULL(srp_vert_source());
    TEST_ASSERT_NOT_NULL(srp_frag_source());

    /* Lens flare shaders */
    TEST_ASSERT_NOT_NULL(lf_vert_source());
    TEST_ASSERT_NOT_NULL(lf_frag_source());

    /* Post-process shaders */
    TEST_ASSERT_NOT_NULL(pp_quad_vert_source());
    TEST_ASSERT_NOT_NULL(pp_bright_extract_frag_source());
    TEST_ASSERT_NOT_NULL(pp_blur_frag_source());
    TEST_ASSERT_NOT_NULL(pp_composite_frag_source());

    /* Weather overlay shaders */
    TEST_ASSERT_NOT_NULL(wov_vert_source());
    TEST_ASSERT_NOT_NULL(wov_frag_source());

    /* Tree pack shaders */
    TEST_ASSERT_NOT_NULL(trp_vert_source());
    TEST_ASSERT_NOT_NULL(trp_frag_source());
}

/* ===== Test: post_process info is consistent ===== */
void test_post_process_info_consistent(void) {
    pp_config_t cfg = pp_default_config();
    cfg.screen_width = 1920;
    cfg.screen_height = 1080;

    pp_info_t info = pp_info(cfg);
    TEST_ASSERT_TRUE(info.fbo_count >= 0);
    TEST_ASSERT_TRUE(info.texture_count >= 0);
    TEST_ASSERT_TRUE(info.total_vram_bytes >= 0);

    int vram = pp_estimate_vram(1920, 1080, cfg);
    TEST_ASSERT_TRUE(vram > 0);
}

/* ===== Test: constellation lines pack ===== */
void test_constellation_lines_pack(void) {
    float buf[200 * 2 * CLINE_VERTEX_FLOATS];
    int count = constellation_lines_pack(buf, 200, 0.25f, 0.4f, 100.0f);

    TEST_ASSERT_TRUE(count > 0);
    TEST_ASSERT_FALSE(buffer_has_nan_inf(buf, count * 2 * CLINE_VERTEX_FLOATS));

    constellation_line_info_t info = constellation_lines_info(count);
    TEST_ASSERT_EQUAL(count, info.line_count);
    TEST_ASSERT_TRUE(info.zodiac_line_count >= 0);
    TEST_ASSERT_TRUE(info.zodiac_line_count <= count);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_render_frame_construction);
    RUN_TEST(test_camera_render_frame_integration);
    RUN_TEST(test_star_field_pack_j2000);
    RUN_TEST(test_planet_pack_j2000);
    RUN_TEST(test_planet_trail_pack_j2000);
    RUN_TEST(test_zodiac_ring_pack);
    RUN_TEST(test_moon_pack_j2000);
    RUN_TEST(test_orbit_trail_pack_j2000);
    RUN_TEST(test_earth_pack_globe);
    RUN_TEST(test_bodygraph_pack_output);
    RUN_TEST(test_milkyway_pack_output);
    RUN_TEST(test_saturn_ring_pack_output);
    RUN_TEST(test_tree_pack_output);
    RUN_TEST(test_lens_flare_pack_output);
    RUN_TEST(test_post_process_quad_output);
    RUN_TEST(test_weather_overlay_pack_output);
    RUN_TEST(test_layer_visibility_at_scales);
    RUN_TEST(test_all_shader_sources_non_null);
    RUN_TEST(test_post_process_info_consistent);
    RUN_TEST(test_constellation_lines_pack);

    return UNITY_END();
}
