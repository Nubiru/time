/*
 * test_lens_flare.c -- Tests for procedural screen-space lens flare geometry
 *
 * Tests: sun/star configs, screen position projection, flare axis,
 *        edge fade, vertex packing, shader source strings,
 *        null safety, aspect ratio handling, element placement.
 */

#include "../unity/unity.h"
#include "../../src/render/lens_flare.h"

#include <math.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===================================================================
 * Helper: identity matrix (column-major 4x4)
 * =================================================================== */

static void identity_matrix(float m[16])
{
    memset(m, 0, 16 * sizeof(float));
    m[0] = 1.0f;
    m[5] = 1.0f;
    m[10] = 1.0f;
    m[15] = 1.0f;
}

/* Helper: perspective-like matrix that puts z=-1 at NDC origin */
static void simple_proj_matrix(float m[16])
{
    memset(m, 0, 16 * sizeof(float));
    m[0] = 1.0f;   /* scale x */
    m[5] = 1.0f;   /* scale y */
    m[10] = -1.0f;  /* map z */
    m[11] = -1.0f;  /* perspective divide: w = -z */
    m[14] = -1.0f;  /* z offset */
}

/* ===================================================================
 * 1. lf_sun_config — Sun flare configuration
 * =================================================================== */

void test_sun_config_has_elements(void)
{
    lf_config_t cfg = lf_sun_config();
    TEST_ASSERT_GREATER_THAN(0, cfg.element_count);
    TEST_ASSERT_LESS_OR_EQUAL(LF_MAX_ELEMENTS, cfg.element_count);
}

void test_sun_config_has_starburst(void)
{
    lf_config_t cfg = lf_sun_config();
    int found = 0;
    for (int i = 0; i < cfg.element_count; i++) {
        if (cfg.elements[i].type == LF_STARBURST) {
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);
}

void test_sun_config_has_halo(void)
{
    lf_config_t cfg = lf_sun_config();
    int found = 0;
    for (int i = 0; i < cfg.element_count; i++) {
        if (cfg.elements[i].type == LF_HALO) {
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);
}

void test_sun_config_has_ghosts(void)
{
    lf_config_t cfg = lf_sun_config();
    int count = 0;
    for (int i = 0; i < cfg.element_count; i++) {
        if (cfg.elements[i].type == LF_GHOST) {
            count++;
        }
    }
    TEST_ASSERT_GREATER_THAN(0, count);
}

void test_sun_config_global_intensity(void)
{
    lf_config_t cfg = lf_sun_config();
    TEST_ASSERT_FLOAT_WITHIN(0.5f, 0.75f, cfg.global_intensity);
}

void test_sun_config_warm_colors(void)
{
    /* Sun colors should be warm: R > G > B */
    lf_config_t cfg = lf_sun_config();
    for (int i = 0; i < cfg.element_count; i++) {
        float r = cfg.elements[i].color[0];
        float g = cfg.elements[i].color[1];
        float b = cfg.elements[i].color[2];
        TEST_ASSERT_TRUE_MESSAGE(r >= g, "Sun element: R should >= G");
        TEST_ASSERT_TRUE_MESSAGE(g >= b, "Sun element: G should >= B");
    }
}

/* ===================================================================
 * 2. lf_star_config — Star flare configuration
 * =================================================================== */

void test_star_config_has_elements(void)
{
    lf_config_t cfg = lf_star_config();
    TEST_ASSERT_GREATER_THAN(0, cfg.element_count);
}

void test_star_config_fewer_than_sun(void)
{
    lf_config_t sun = lf_sun_config();
    lf_config_t star = lf_star_config();
    TEST_ASSERT_TRUE(star.element_count < sun.element_count);
}

void test_star_config_cool_colors(void)
{
    /* Star colors should be cool: B >= R for at least some elements */
    lf_config_t cfg = lf_star_config();
    int has_cool = 0;
    for (int i = 0; i < cfg.element_count; i++) {
        if (cfg.elements[i].color[2] >= cfg.elements[i].color[0]) {
            has_cool = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(has_cool);
}

/* ===================================================================
 * 3. lf_screen_position — World to NDC projection
 * =================================================================== */

void test_screen_pos_origin_identity(void)
{
    /* Point at origin with identity matrix → NDC near (0,0) */
    float mat[16];
    identity_matrix(mat);
    float world[3] = { 0.0f, 0.0f, 0.0f };
    float ndc[2] = { 99.0f, 99.0f };
    int vis = lf_screen_position(world, mat, ndc);
    TEST_ASSERT_EQUAL(1, vis);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, ndc[0]);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, ndc[1]);
}

void test_screen_pos_behind_camera(void)
{
    /* Point behind camera: w <= 0 after projection.
     * Use a perspective-like matrix where w = -z. Point at z=+1 gives w=-1. */
    float mat[16];
    simple_proj_matrix(mat);
    float world[3] = { 0.0f, 0.0f, 1.0f }; /* positive z = behind in this setup */
    float ndc[2];
    int vis = lf_screen_position(world, mat, ndc);
    TEST_ASSERT_EQUAL(0, vis);
}

void test_screen_pos_off_screen(void)
{
    /* Point far off to the side with identity matrix */
    float mat[16];
    identity_matrix(mat);
    float world[3] = { 5.0f, 0.0f, 0.0f };
    float ndc[2];
    int vis = lf_screen_position(world, mat, ndc);
    TEST_ASSERT_EQUAL(0, vis);
}

void test_screen_pos_null_output(void)
{
    /* NULL screen_pos_out should not crash */
    float mat[16];
    identity_matrix(mat);
    float world[3] = { 0.0f, 0.0f, 0.0f };
    int vis = lf_screen_position(world, mat, NULL);
    TEST_ASSERT_EQUAL(1, vis);
}

void test_screen_pos_null_view_proj(void)
{
    float world[3] = { 0.0f, 0.0f, 0.0f };
    float ndc[2];
    int vis = lf_screen_position(world, NULL, ndc);
    TEST_ASSERT_EQUAL(0, vis);
}

void test_screen_pos_null_world_pos(void)
{
    float mat[16];
    identity_matrix(mat);
    float ndc[2];
    int vis = lf_screen_position(NULL, mat, ndc);
    TEST_ASSERT_EQUAL(0, vis);
}

/* ===================================================================
 * 4. lf_flare_axis — Axis from source through center
 * =================================================================== */

void test_flare_axis_direction(void)
{
    /* Source at (0.5, 0.5) → axis toward (-0.5, -0.5) normalized */
    float src[2] = { 0.5f, 0.5f };
    float axis[2];
    float len = lf_flare_axis(src, axis);

    /* Axis should point toward negative (toward center from source) */
    TEST_ASSERT_TRUE(axis[0] < 0.0f);
    TEST_ASSERT_TRUE(axis[1] < 0.0f);

    /* Should be normalized */
    float mag = sqrtf(axis[0] * axis[0] + axis[1] * axis[1]);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, mag);

    /* Length is distance from source to center */
    float expected_len = sqrtf(0.5f * 0.5f + 0.5f * 0.5f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, expected_len, len);
}

void test_flare_axis_at_center(void)
{
    /* Source at center (0,0) → axis length 0 */
    float src[2] = { 0.0f, 0.0f };
    float axis[2];
    float len = lf_flare_axis(src, axis);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, len);
    /* Axis direction should be zero when source is at center */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, axis[0]);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, axis[1]);
}

void test_flare_axis_null_source(void)
{
    float axis[2] = { 99.0f, 99.0f };
    float len = lf_flare_axis(NULL, axis);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, len);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, axis[0]);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, axis[1]);
}

void test_flare_axis_null_output(void)
{
    float src[2] = { 0.5f, 0.5f };
    float len = lf_flare_axis(src, NULL);
    float expected = sqrtf(0.5f * 0.5f + 0.5f * 0.5f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, expected, len);
}

void test_flare_axis_length(void)
{
    /* Source at (0.8, 0.0) → axis length is 0.8 */
    float src[2] = { 0.8f, 0.0f };
    float axis[2];
    float len = lf_flare_axis(src, axis);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.8f, len);
}

/* ===================================================================
 * 5. lf_edge_fade — Screen edge proximity fade
 * =================================================================== */

void test_edge_fade_center(void)
{
    /* Source at center → full intensity */
    float src[2] = { 0.0f, 0.0f };
    float fade = lf_edge_fade(src, 0.7f, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, fade);
}

void test_edge_fade_edge(void)
{
    /* Source near edge → reduced intensity */
    float src[2] = { 0.85f, 0.0f };
    float fade = lf_edge_fade(src, 0.7f, 1.0f);
    TEST_ASSERT_TRUE(fade > 0.0f);
    TEST_ASSERT_TRUE(fade < 1.0f);
}

void test_edge_fade_outside(void)
{
    /* Source far outside → 0 intensity */
    float src[2] = { 1.5f, 0.0f };
    float fade = lf_edge_fade(src, 0.7f, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, fade);
}

void test_edge_fade_inverted_range(void)
{
    /* fade_start > fade_end → should still work (swap internally) */
    float src[2] = { 0.85f, 0.0f };
    float fade = lf_edge_fade(src, 1.0f, 0.7f);
    TEST_ASSERT_TRUE(fade >= 0.0f);
    TEST_ASSERT_TRUE(fade <= 1.0f);
}

void test_edge_fade_null_source(void)
{
    float fade = lf_edge_fade(NULL, 0.7f, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, fade);
}

/* ===================================================================
 * 6. lf_pack — Vertex packing
 * =================================================================== */

void test_pack_sun_returns_vertices(void)
{
    lf_config_t cfg = lf_sun_config();
    float src[2] = { 0.3f, 0.2f };
    float verts[LF_MAX_VERTICES * LF_VERTEX_FLOATS];
    int count = lf_pack(cfg, src, 1.0f, verts, LF_MAX_VERTICES);
    TEST_ASSERT_GREATER_THAN(0, count);
}

void test_pack_vertex_count_multiple_of_6(void)
{
    lf_config_t cfg = lf_sun_config();
    float src[2] = { 0.3f, 0.2f };
    float verts[LF_MAX_VERTICES * LF_VERTEX_FLOATS];
    int count = lf_pack(cfg, src, 1.0f, verts, LF_MAX_VERTICES);
    TEST_ASSERT_EQUAL(0, count % 6);
}

void test_pack_respects_max_verts(void)
{
    lf_config_t cfg = lf_sun_config();
    float src[2] = { 0.3f, 0.2f };
    float verts[LF_MAX_VERTICES * LF_VERTEX_FLOATS];
    int count = lf_pack(cfg, src, 1.0f, verts, LF_MAX_VERTICES);
    TEST_ASSERT_TRUE(count <= LF_MAX_VERTICES);
}

void test_pack_null_output(void)
{
    lf_config_t cfg = lf_sun_config();
    float src[2] = { 0.3f, 0.2f };
    int count = lf_pack(cfg, src, 1.0f, NULL, LF_MAX_VERTICES);
    TEST_ASSERT_EQUAL(0, count);
}

void test_pack_max_verts_too_small(void)
{
    /* max_verts = 3 (less than 6 for one quad) → returns 0 */
    lf_config_t cfg = lf_sun_config();
    float src[2] = { 0.3f, 0.2f };
    float verts[6 * LF_VERTEX_FLOATS];
    int count = lf_pack(cfg, src, 1.0f, verts, 3);
    TEST_ASSERT_EQUAL(0, count);
}

void test_pack_limited_capacity(void)
{
    /* max_verts = 6 → should only fit 1 quad */
    lf_config_t cfg = lf_sun_config();
    float src[2] = { 0.3f, 0.2f };
    float verts[6 * LF_VERTEX_FLOATS];
    int count = lf_pack(cfg, src, 1.0f, verts, 6);
    TEST_ASSERT_EQUAL(6, count);
}

void test_pack_vertices_in_ndc_range(void)
{
    lf_config_t cfg = lf_sun_config();
    float src[2] = { 0.0f, 0.0f };
    float verts[LF_MAX_VERTICES * LF_VERTEX_FLOATS];
    int count = lf_pack(cfg, src, 1.0f, verts, LF_MAX_VERTICES);
    for (int i = 0; i < count; i++) {
        int base = i * LF_VERTEX_FLOATS;
        float px = verts[base + 0];
        float py = verts[base + 1];
        /* Positions should be reasonable NDC range (allow slight overshoot for large elements) */
        TEST_ASSERT_TRUE(px >= -3.0f && px <= 3.0f);
        TEST_ASSERT_TRUE(py >= -3.0f && py <= 3.0f);
    }
}

void test_pack_uvs_in_range(void)
{
    lf_config_t cfg = lf_sun_config();
    float src[2] = { 0.0f, 0.0f };
    float verts[LF_MAX_VERTICES * LF_VERTEX_FLOATS];
    int count = lf_pack(cfg, src, 1.0f, verts, LF_MAX_VERTICES);
    for (int i = 0; i < count; i++) {
        int base = i * LF_VERTEX_FLOATS;
        float u = verts[base + 2];
        float v = verts[base + 3];
        TEST_ASSERT_TRUE(u >= 0.0f && u <= 1.0f);
        TEST_ASSERT_TRUE(v >= 0.0f && v <= 1.0f);
    }
}

void test_pack_colors_have_alpha(void)
{
    lf_config_t cfg = lf_sun_config();
    float src[2] = { 0.3f, 0.2f };
    float verts[LF_MAX_VERTICES * LF_VERTEX_FLOATS];
    int count = lf_pack(cfg, src, 1.0f, verts, LF_MAX_VERTICES);
    for (int i = 0; i < count; i++) {
        int base = i * LF_VERTEX_FLOATS;
        float a = verts[base + 7]; /* alpha is the 8th float */
        TEST_ASSERT_TRUE(a >= 0.0f);
    }
}

void test_pack_aspect_1_symmetric(void)
{
    /* With aspect_ratio = 1.0 and source at center, quads should be symmetric */
    lf_config_t cfg;
    cfg.element_count = 1;
    cfg.global_intensity = 1.0f;
    cfg.fade_start = 2.0f;
    cfg.fade_end = 3.0f;
    cfg.elements[0].type = LF_HALO;
    cfg.elements[0].offset = 0.0f;
    cfg.elements[0].size = 0.1f;
    cfg.elements[0].intensity = 1.0f;
    cfg.elements[0].color[0] = 1.0f;
    cfg.elements[0].color[1] = 1.0f;
    cfg.elements[0].color[2] = 1.0f;
    cfg.elements[0].color[3] = 1.0f;
    cfg.elements[0].ray_count = 0;
    cfg.elements[0].rotation = 0.0f;

    float src[2] = { 0.0f, 0.0f };
    float verts[6 * LF_VERTEX_FLOATS];
    int count = lf_pack(cfg, src, 1.0f, verts, 6);
    TEST_ASSERT_EQUAL(6, count);

    /* With aspect 1.0, half_w == half_h = 0.1 */
    /* Check first vertex (bottom-left corner): px should be -0.1 */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, -0.1f, verts[0]);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, -0.1f, verts[1]);
}

void test_pack_aspect_1_5_adjusted(void)
{
    /* With aspect_ratio = 1.5, half_h should be 1.5x half_w */
    lf_config_t cfg;
    cfg.element_count = 1;
    cfg.global_intensity = 1.0f;
    cfg.fade_start = 2.0f;
    cfg.fade_end = 3.0f;
    cfg.elements[0].type = LF_HALO;
    cfg.elements[0].offset = 0.0f;
    cfg.elements[0].size = 0.1f;
    cfg.elements[0].intensity = 1.0f;
    cfg.elements[0].color[0] = 1.0f;
    cfg.elements[0].color[1] = 1.0f;
    cfg.elements[0].color[2] = 1.0f;
    cfg.elements[0].color[3] = 1.0f;
    cfg.elements[0].ray_count = 0;
    cfg.elements[0].rotation = 0.0f;

    float src[2] = { 0.0f, 0.0f };
    float verts[6 * LF_VERTEX_FLOATS];
    int count = lf_pack(cfg, src, 1.5f, verts, 6);
    TEST_ASSERT_EQUAL(6, count);

    /* half_w = 0.1, half_h = 0.1 * 1.5 = 0.15 */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, -0.1f, verts[0]);   /* px */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, -0.15f, verts[1]);   /* py */
}

void test_pack_ghost_along_axis(void)
{
    /* Ghost with offset > 0 should be positioned away from source along axis */
    lf_config_t cfg;
    cfg.element_count = 1;
    cfg.global_intensity = 1.0f;
    cfg.fade_start = 2.0f;
    cfg.fade_end = 3.0f;
    cfg.elements[0].type = LF_GHOST;
    cfg.elements[0].offset = 1.0f; /* at center */
    cfg.elements[0].size = 0.05f;
    cfg.elements[0].intensity = 1.0f;
    cfg.elements[0].color[0] = 1.0f;
    cfg.elements[0].color[1] = 1.0f;
    cfg.elements[0].color[2] = 1.0f;
    cfg.elements[0].color[3] = 1.0f;
    cfg.elements[0].ray_count = 0;
    cfg.elements[0].rotation = 0.0f;

    /* Source off-center at (0.5, 0.0) */
    float src[2] = { 0.5f, 0.0f };
    float verts[6 * LF_VERTEX_FLOATS];
    int count = lf_pack(cfg, src, 1.0f, verts, 6);
    TEST_ASSERT_EQUAL(6, count);

    /* Ghost center should be near origin (axis from source to center, offset=1.0) */
    /* The quad vertices surround the center, so average x should be near 0 */
    float avg_x = 0.0f;
    for (int i = 0; i < 6; i++) {
        avg_x += verts[i * LF_VERTEX_FLOATS + 0];
    }
    avg_x /= 6.0f;
    /* With source at 0.5, axis goes toward 0.0, offset 1.0 * axis_len 0.5 = 0.5 travel */
    /* So ghost center = 0.5 + (-1.0) * 1.0 * 0.5 = 0.0 */
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, avg_x);
}

void test_pack_source_at_center(void)
{
    /* Source at center → starburst/halo still pack, ghosts degenerate to center */
    lf_config_t cfg = lf_sun_config();
    float src[2] = { 0.0f, 0.0f };
    float verts[LF_MAX_VERTICES * LF_VERTEX_FLOATS];
    int count = lf_pack(cfg, src, 1.0f, verts, LF_MAX_VERTICES);
    /* Should still produce vertices (starburst, halo at offset=0 still valid) */
    TEST_ASSERT_GREATER_THAN(0, count);
}

void test_pack_null_source(void)
{
    lf_config_t cfg = lf_sun_config();
    float verts[LF_MAX_VERTICES * LF_VERTEX_FLOATS];
    int count = lf_pack(cfg, NULL, 1.0f, verts, LF_MAX_VERTICES);
    TEST_ASSERT_EQUAL(0, count);
}

/* ===================================================================
 * 7. Shader sources
 * =================================================================== */

void test_vert_source_non_null(void)
{
    const char *src = lf_vert_source();
    TEST_ASSERT_NOT_NULL(src);
}

void test_vert_source_has_gl_position(void)
{
    const char *src = lf_vert_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "gl_Position"));
}

void test_vert_source_has_version(void)
{
    const char *src = lf_vert_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "#version 300 es"));
}

void test_frag_source_non_null(void)
{
    const char *src = lf_frag_source();
    TEST_ASSERT_NOT_NULL(src);
}

void test_frag_source_has_frag_color(void)
{
    const char *src = lf_frag_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "frag_color"));
}

void test_frag_source_has_version(void)
{
    const char *src = lf_frag_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "#version 300 es"));
}

void test_frag_source_has_u_type(void)
{
    const char *src = lf_frag_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "u_type"));
}

/* ===================================================================
 * 8. Additional edge cases
 * =================================================================== */

void test_sun_config_starburst_has_rays(void)
{
    lf_config_t cfg = lf_sun_config();
    for (int i = 0; i < cfg.element_count; i++) {
        if (cfg.elements[i].type == LF_STARBURST) {
            TEST_ASSERT_GREATER_THAN(0, cfg.elements[i].ray_count);
            return;
        }
    }
    TEST_FAIL_MESSAGE("No starburst element found in sun config");
}

void test_star_config_starburst_has_rays(void)
{
    lf_config_t cfg = lf_star_config();
    for (int i = 0; i < cfg.element_count; i++) {
        if (cfg.elements[i].type == LF_STARBURST) {
            TEST_ASSERT_GREATER_THAN(0, cfg.elements[i].ray_count);
            return;
        }
    }
    TEST_FAIL_MESSAGE("No starburst element found in star config");
}

void test_edge_fade_at_fade_start(void)
{
    /* At exactly fade_start → should be 1.0 */
    float src[2] = { 0.7f, 0.0f };
    float fade = lf_edge_fade(src, 0.7f, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, fade);
}

void test_edge_fade_at_fade_end(void)
{
    /* At exactly fade_end → should be 0.0 */
    float src[2] = { 1.0f, 0.0f };
    float fade = lf_edge_fade(src, 0.7f, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, fade);
}

void test_pack_star_config(void)
{
    lf_config_t cfg = lf_star_config();
    float src[2] = { 0.2f, -0.3f };
    float verts[LF_MAX_VERTICES * LF_VERTEX_FLOATS];
    int count = lf_pack(cfg, src, 1.0f, verts, LF_MAX_VERTICES);
    TEST_ASSERT_GREATER_THAN(0, count);
    TEST_ASSERT_EQUAL(0, count % 6);
}

void test_flare_axis_negative_source(void)
{
    /* Source at (-0.5, -0.5) → axis toward (0.5, 0.5) */
    float src[2] = { -0.5f, -0.5f };
    float axis[2];
    lf_flare_axis(src, axis);
    TEST_ASSERT_TRUE(axis[0] > 0.0f);
    TEST_ASSERT_TRUE(axis[1] > 0.0f);
}

/* ===================================================================
 * Runner
 * =================================================================== */

int main(void)
{
    UNITY_BEGIN();

    /* Sun config */
    RUN_TEST(test_sun_config_has_elements);
    RUN_TEST(test_sun_config_has_starburst);
    RUN_TEST(test_sun_config_has_halo);
    RUN_TEST(test_sun_config_has_ghosts);
    RUN_TEST(test_sun_config_global_intensity);
    RUN_TEST(test_sun_config_warm_colors);
    RUN_TEST(test_sun_config_starburst_has_rays);

    /* Star config */
    RUN_TEST(test_star_config_has_elements);
    RUN_TEST(test_star_config_fewer_than_sun);
    RUN_TEST(test_star_config_cool_colors);
    RUN_TEST(test_star_config_starburst_has_rays);

    /* Screen position */
    RUN_TEST(test_screen_pos_origin_identity);
    RUN_TEST(test_screen_pos_behind_camera);
    RUN_TEST(test_screen_pos_off_screen);
    RUN_TEST(test_screen_pos_null_output);
    RUN_TEST(test_screen_pos_null_view_proj);
    RUN_TEST(test_screen_pos_null_world_pos);

    /* Flare axis */
    RUN_TEST(test_flare_axis_direction);
    RUN_TEST(test_flare_axis_at_center);
    RUN_TEST(test_flare_axis_null_source);
    RUN_TEST(test_flare_axis_null_output);
    RUN_TEST(test_flare_axis_length);
    RUN_TEST(test_flare_axis_negative_source);

    /* Edge fade */
    RUN_TEST(test_edge_fade_center);
    RUN_TEST(test_edge_fade_edge);
    RUN_TEST(test_edge_fade_outside);
    RUN_TEST(test_edge_fade_inverted_range);
    RUN_TEST(test_edge_fade_null_source);
    RUN_TEST(test_edge_fade_at_fade_start);
    RUN_TEST(test_edge_fade_at_fade_end);

    /* Packing */
    RUN_TEST(test_pack_sun_returns_vertices);
    RUN_TEST(test_pack_vertex_count_multiple_of_6);
    RUN_TEST(test_pack_respects_max_verts);
    RUN_TEST(test_pack_null_output);
    RUN_TEST(test_pack_max_verts_too_small);
    RUN_TEST(test_pack_limited_capacity);
    RUN_TEST(test_pack_vertices_in_ndc_range);
    RUN_TEST(test_pack_uvs_in_range);
    RUN_TEST(test_pack_colors_have_alpha);
    RUN_TEST(test_pack_aspect_1_symmetric);
    RUN_TEST(test_pack_aspect_1_5_adjusted);
    RUN_TEST(test_pack_ghost_along_axis);
    RUN_TEST(test_pack_source_at_center);
    RUN_TEST(test_pack_null_source);
    RUN_TEST(test_pack_star_config);

    /* Shader sources */
    RUN_TEST(test_vert_source_non_null);
    RUN_TEST(test_vert_source_has_gl_position);
    RUN_TEST(test_vert_source_has_version);
    RUN_TEST(test_frag_source_non_null);
    RUN_TEST(test_frag_source_has_frag_color);
    RUN_TEST(test_frag_source_has_version);
    RUN_TEST(test_frag_source_has_u_type);

    return UNITY_END();
}
