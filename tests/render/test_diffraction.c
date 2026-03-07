/*
 * test_diffraction.c -- Tests for diffraction spike vertex packing
 *
 * Tests: spike length computation, alpha interpolation, vertex packing,
 *        info metadata, default config, shader source strings,
 *        edge cases (no bright stars, boundary magnitude), spike geometry.
 */

#include "../unity/unity.h"
#include "../../src/render/diffraction.h"

#include <math.h>
#include <string.h>

#define PI 3.14159265358979323846

void setUp(void) {}
void tearDown(void) {}

/* ===================================================================
 * Helper: build small test star arrays
 * =================================================================== */

/* 3 test stars: bright (mag 0.0), medium (mag 1.0), dim (mag 2.0) */
static const float test_positions[] = {
    1.0f, 0.0f, 0.0f,   /* star 0: on +x axis */
    0.0f, 1.0f, 0.0f,   /* star 1: on +y axis */
    0.0f, 0.0f, 1.0f    /* star 2: on +z axis */
};

static const float test_colors[] = {
    1.0f, 0.9f, 0.7f,   /* star 0: warm white */
    0.8f, 0.8f, 1.0f,   /* star 1: blue-white */
    1.0f, 0.5f, 0.3f    /* star 2: orange */
};

static const float test_magnitudes[] = {
    0.0f,   /* star 0: very bright */
    1.0f,   /* star 1: bright */
    2.0f    /* star 2: dim (above 1.5 threshold) */
};

/* ===================================================================
 * 1. diffraction_spike_length -- brightness to spike length
 * =================================================================== */

void test_spike_length_magnitude_zero(void)
{
    /* Magnitude 0 star should get maximum spike length */
    float len = diffraction_spike_length(0.0f, 3.0f, 1.5f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 3.0f, len);
}

void test_spike_length_at_threshold(void)
{
    /* Star exactly at min_magnitude threshold gets 0 length */
    float len = diffraction_spike_length(1.5f, 3.0f, 1.5f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, len);
}

void test_spike_length_above_threshold(void)
{
    /* Star dimmer than threshold gets 0 */
    float len = diffraction_spike_length(2.0f, 3.0f, 1.5f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, len);
}

void test_spike_length_mid_brightness(void)
{
    /* Magnitude 0.75 with threshold 1.5: half-brightness = half-length */
    float len = diffraction_spike_length(0.75f, 3.0f, 1.5f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.5f, len);
}

void test_spike_length_scales_with_base(void)
{
    /* Doubling base_length should double the result */
    float len1 = diffraction_spike_length(0.0f, 3.0f, 1.5f);
    float len2 = diffraction_spike_length(0.0f, 6.0f, 1.5f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, len1 * 2.0f, len2);
}

void test_spike_length_negative_magnitude(void)
{
    /* Negative magnitude (very bright, like Sirius at -1.46) should still work */
    float len = diffraction_spike_length(-1.0f, 3.0f, 1.5f);
    /* Should be > base_length since mag < 0 */
    TEST_ASSERT_TRUE(len > 3.0f);
}

/* ===================================================================
 * 2. diffraction_spike_alpha -- alpha along spike
 * =================================================================== */

void test_spike_alpha_at_center(void)
{
    float a = diffraction_spike_alpha(0.0f, 0.8f, 0.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.8f, a);
}

void test_spike_alpha_at_tip(void)
{
    float a = diffraction_spike_alpha(1.0f, 0.8f, 0.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, a);
}

void test_spike_alpha_midpoint(void)
{
    float a = diffraction_spike_alpha(0.5f, 0.8f, 0.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.4f, a);
}

void test_spike_alpha_nonzero_tip(void)
{
    /* Tip alpha = 0.2, center = 1.0 */
    float a = diffraction_spike_alpha(1.0f, 1.0f, 0.2f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.2f, a);
}

void test_spike_alpha_clamped_t_negative(void)
{
    /* t < 0 should be treated as 0 (center) */
    float a = diffraction_spike_alpha(-0.5f, 0.8f, 0.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.8f, a);
}

void test_spike_alpha_clamped_t_above_one(void)
{
    /* t > 1 should be treated as 1 (tip) */
    float a = diffraction_spike_alpha(2.0f, 0.8f, 0.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, a);
}

/* ===================================================================
 * 3. diffraction_default_config
 * =================================================================== */

void test_default_config_base_length(void)
{
    diffraction_config_t cfg = diffraction_default_config();
    TEST_ASSERT_TRUE(cfg.base_length > 0.0f);
}

void test_default_config_min_magnitude(void)
{
    diffraction_config_t cfg = diffraction_default_config();
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.5f, cfg.min_magnitude);
}

void test_default_config_center_alpha(void)
{
    diffraction_config_t cfg = diffraction_default_config();
    TEST_ASSERT_TRUE(cfg.center_alpha > 0.0f && cfg.center_alpha <= 1.0f);
}

void test_default_config_tip_alpha(void)
{
    diffraction_config_t cfg = diffraction_default_config();
    TEST_ASSERT_TRUE(cfg.tip_alpha >= 0.0f && cfg.tip_alpha < cfg.center_alpha);
}

void test_default_config_spike_count(void)
{
    diffraction_config_t cfg = diffraction_default_config();
    TEST_ASSERT_TRUE(cfg.spike_count == 4 || cfg.spike_count == 6);
}

/* ===================================================================
 * 4. diffraction_info -- metadata after packing
 * =================================================================== */

void test_info_single_star(void)
{
    diffraction_info_t info = diffraction_info(1, 4);
    TEST_ASSERT_EQUAL_INT(1, info.star_count);
    TEST_ASSERT_EQUAL_INT(4, info.line_count);
    TEST_ASSERT_EQUAL_INT(8, info.vertex_count);
    TEST_ASSERT_EQUAL_INT(56, info.float_count);
}

void test_info_multiple_stars(void)
{
    diffraction_info_t info = diffraction_info(5, 6);
    TEST_ASSERT_EQUAL_INT(5, info.star_count);
    TEST_ASSERT_EQUAL_INT(30, info.line_count);
    TEST_ASSERT_EQUAL_INT(60, info.vertex_count);
    TEST_ASSERT_EQUAL_INT(420, info.float_count);
}

void test_info_zero_stars(void)
{
    diffraction_info_t info = diffraction_info(0, 4);
    TEST_ASSERT_EQUAL_INT(0, info.star_count);
    TEST_ASSERT_EQUAL_INT(0, info.line_count);
    TEST_ASSERT_EQUAL_INT(0, info.vertex_count);
    TEST_ASSERT_EQUAL_INT(0, info.float_count);
}

/* ===================================================================
 * 5. diffraction_pack -- main vertex packing
 * =================================================================== */

void test_pack_returns_bright_star_count(void)
{
    diffraction_config_t cfg = diffraction_default_config();
    /* max buffer: 3 stars * 4 spikes * 2 verts * 7 floats = 168 */
    float out[168];
    memset(out, 0, sizeof(out));
    int count = diffraction_pack(test_positions, test_colors,
                                  test_magnitudes, 3, &cfg, out);
    /* Stars 0 (mag 0.0) and 1 (mag 1.0) are below 1.5 threshold */
    TEST_ASSERT_EQUAL_INT(2, count);
}

void test_pack_dim_star_excluded(void)
{
    diffraction_config_t cfg = diffraction_default_config();
    float out[168];
    memset(out, 0, sizeof(out));
    /* Only pass the dim star (mag 2.0) */
    float pos[] = { 0.0f, 0.0f, 1.0f };
    float col[] = { 1.0f, 0.5f, 0.3f };
    float mag[] = { 2.0f };
    int count = diffraction_pack(pos, col, mag, 1, &cfg, out);
    TEST_ASSERT_EQUAL_INT(0, count);
}

void test_pack_center_vertex_at_star_position(void)
{
    diffraction_config_t cfg = diffraction_default_config();
    float out[168];
    memset(out, 0, sizeof(out));
    diffraction_pack(test_positions, test_colors, test_magnitudes,
                     3, &cfg, out);
    /* First vertex of first spike should be at star 0 position (1,0,0) */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, out[0]);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, out[1]);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, out[2]);
}

void test_pack_center_vertex_has_star_color(void)
{
    diffraction_config_t cfg = diffraction_default_config();
    float out[168];
    memset(out, 0, sizeof(out));
    diffraction_pack(test_positions, test_colors, test_magnitudes,
                     3, &cfg, out);
    /* Color at center vertex: RGB should match star 0 color */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, out[3]);  /* r */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.9f, out[4]);  /* g */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.7f, out[5]);  /* b */
}

void test_pack_center_vertex_has_center_alpha(void)
{
    diffraction_config_t cfg = diffraction_default_config();
    float out[168];
    memset(out, 0, sizeof(out));
    diffraction_pack(test_positions, test_colors, test_magnitudes,
                     3, &cfg, out);
    /* Alpha at center vertex = center_alpha from config */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, cfg.center_alpha, out[6]);
}

void test_pack_tip_vertex_has_tip_alpha(void)
{
    diffraction_config_t cfg = diffraction_default_config();
    float out[168];
    memset(out, 0, sizeof(out));
    diffraction_pack(test_positions, test_colors, test_magnitudes,
                     3, &cfg, out);
    /* Tip vertex is 2nd vertex: index 7 (start of 2nd vertex in first spike) */
    int tip_base = DIFFRACTION_VERTEX_FLOATS; /* offset to 2nd vertex */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, cfg.tip_alpha, out[tip_base + 6]);
}

void test_pack_tip_vertex_offset_from_center(void)
{
    diffraction_config_t cfg = diffraction_default_config();
    float out[168];
    memset(out, 0, sizeof(out));
    diffraction_pack(test_positions, test_colors, test_magnitudes,
                     3, &cfg, out);
    /* Tip position should differ from center by spike_length in tangent plane */
    int tip_base = DIFFRACTION_VERTEX_FLOATS;
    float dx = out[tip_base + 0] - out[0];
    float dy = out[tip_base + 1] - out[1];
    float dz = out[tip_base + 2] - out[2];
    float dist = sqrtf(dx * dx + dy * dy + dz * dz);
    /* Should equal spike length for mag 0 star */
    float expected_len = diffraction_spike_length(0.0f, cfg.base_length,
                                                   cfg.min_magnitude);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, expected_len, dist);
}

void test_pack_spikes_evenly_spaced(void)
{
    /* For 4 spikes, angles between consecutive spike directions should be ~90 deg */
    diffraction_config_t cfg = diffraction_default_config();
    cfg.spike_count = 4;
    cfg.rotation_deg = 0.0f;
    float out[168];
    memset(out, 0, sizeof(out));
    diffraction_pack(test_positions, test_colors, test_magnitudes,
                     3, &cfg, out);

    /* Collect tip directions for star 0 (4 spikes, 2 verts each, 7 floats each) */
    float dirs[4][3];
    for (int s = 0; s < 4; s++) {
        int center_idx = s * 2 * DIFFRACTION_VERTEX_FLOATS;
        int tip_idx = center_idx + DIFFRACTION_VERTEX_FLOATS;
        for (int j = 0; j < 3; j++) {
            dirs[s][j] = out[tip_idx + j] - out[center_idx + j];
        }
    }
    /* Dot product of adjacent spike directions should be ~0 (perpendicular) */
    float dot01 = dirs[0][0] * dirs[1][0] + dirs[0][1] * dirs[1][1] +
                  dirs[0][2] * dirs[1][2];
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, dot01);
}

void test_pack_all_spikes_same_length_per_star(void)
{
    diffraction_config_t cfg = diffraction_default_config();
    float out[168];
    memset(out, 0, sizeof(out));
    diffraction_pack(test_positions, test_colors, test_magnitudes,
                     3, &cfg, out);

    float expected_len = diffraction_spike_length(0.0f, cfg.base_length,
                                                   cfg.min_magnitude);
    for (int s = 0; s < cfg.spike_count; s++) {
        int center_idx = s * 2 * DIFFRACTION_VERTEX_FLOATS;
        int tip_idx = center_idx + DIFFRACTION_VERTEX_FLOATS;
        float dx = out[tip_idx + 0] - out[center_idx + 0];
        float dy = out[tip_idx + 1] - out[center_idx + 1];
        float dz = out[tip_idx + 2] - out[center_idx + 2];
        float len = sqrtf(dx * dx + dy * dy + dz * dz);
        TEST_ASSERT_FLOAT_WITHIN(0.01f, expected_len, len);
    }
}

void test_pack_brighter_star_longer_spikes(void)
{
    diffraction_config_t cfg = diffraction_default_config();
    float out[168];
    memset(out, 0, sizeof(out));
    int count = diffraction_pack(test_positions, test_colors,
                                  test_magnitudes, 3, &cfg, out);
    TEST_ASSERT_EQUAL_INT(2, count);

    /* Star 0 (mag 0) spike length > Star 1 (mag 1) spike length */
    /* Star 0 first spike tip */
    int s0_tip = DIFFRACTION_VERTEX_FLOATS;
    float dx0 = out[s0_tip + 0] - out[0];
    float dy0 = out[s0_tip + 1] - out[1];
    float dz0 = out[s0_tip + 2] - out[2];
    float len0 = sqrtf(dx0 * dx0 + dy0 * dy0 + dz0 * dz0);

    /* Star 1 starts after star 0's spikes: 4 spikes * 2 verts * 7 floats */
    int s1_base = cfg.spike_count * 2 * DIFFRACTION_VERTEX_FLOATS;
    int s1_tip = s1_base + DIFFRACTION_VERTEX_FLOATS;
    float dx1 = out[s1_tip + 0] - out[s1_base + 0];
    float dy1 = out[s1_tip + 1] - out[s1_base + 1];
    float dz1 = out[s1_tip + 2] - out[s1_base + 2];
    float len1 = sqrtf(dx1 * dx1 + dy1 * dy1 + dz1 * dz1);

    TEST_ASSERT_TRUE(len0 > len1);
}

void test_pack_spike_perpendicular_to_star_direction(void)
{
    /* Spike directions should lie in the tangent plane at the star position,
     * meaning they should be perpendicular to the star position vector. */
    diffraction_config_t cfg = diffraction_default_config();
    float out[168];
    memset(out, 0, sizeof(out));
    diffraction_pack(test_positions, test_colors, test_magnitudes,
                     3, &cfg, out);

    /* Star 0 position = (1, 0, 0). First spike direction: */
    int tip_base = DIFFRACTION_VERTEX_FLOATS;
    float dx = out[tip_base + 0] - out[0];
    float dy = out[tip_base + 1] - out[1];
    float dz = out[tip_base + 2] - out[2];
    /* Dot product with star position (1, 0, 0) should be ~0 */
    float dot = dx * 1.0f + dy * 0.0f + dz * 0.0f;
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, dot);
}

void test_pack_null_out_returns_zero(void)
{
    diffraction_config_t cfg = diffraction_default_config();
    int count = diffraction_pack(test_positions, test_colors,
                                  test_magnitudes, 3, &cfg, NULL);
    TEST_ASSERT_EQUAL_INT(0, count);
}

void test_pack_zero_count(void)
{
    diffraction_config_t cfg = diffraction_default_config();
    float out[8];
    int count = diffraction_pack(test_positions, test_colors,
                                  test_magnitudes, 0, &cfg, out);
    TEST_ASSERT_EQUAL_INT(0, count);
}

void test_pack_six_spikes(void)
{
    diffraction_config_t cfg = diffraction_default_config();
    cfg.spike_count = 6;
    /* Need larger buffer: 3 stars * 6 spikes * 2 verts * 7 floats = 252 */
    float out[252];
    memset(out, 0, sizeof(out));
    int count = diffraction_pack(test_positions, test_colors,
                                  test_magnitudes, 3, &cfg, out);
    TEST_ASSERT_EQUAL_INT(2, count);

    /* Verify 6 spikes for star 0: check that 6th spike tip is nonzero */
    int spike5_tip = (5 * 2 + 1) * DIFFRACTION_VERTEX_FLOATS;
    float dx = out[spike5_tip + 0] - out[0];
    float dy = out[spike5_tip + 1] - out[1];
    float dz = out[spike5_tip + 2] - out[2];
    float len = sqrtf(dx * dx + dy * dy + dz * dz);
    TEST_ASSERT_TRUE(len > 0.0f);
}

void test_pack_spike_count_clamped_to_4(void)
{
    /* spike_count < 4 should be clamped to 4 */
    diffraction_config_t cfg = diffraction_default_config();
    cfg.spike_count = 2;
    float out[168];
    memset(out, 0, sizeof(out));
    diffraction_pack(test_positions, test_colors, test_magnitudes,
                     3, &cfg, out);
    /* The info should reflect clamped value = 4 */
    /* Verify that 4 spikes were generated for star 0: spike 3 tip exists */
    int spike3_tip = (3 * 2 + 1) * DIFFRACTION_VERTEX_FLOATS;
    float dx = out[spike3_tip + 0] - out[0];
    float dy = out[spike3_tip + 1] - out[1];
    float dz = out[spike3_tip + 2] - out[2];
    float len = sqrtf(dx * dx + dy * dy + dz * dz);
    TEST_ASSERT_TRUE(len > 0.0f);
}

void test_pack_spike_count_clamped_to_6(void)
{
    /* spike_count > 6 should be clamped to 6 */
    diffraction_config_t cfg = diffraction_default_config();
    cfg.spike_count = 10;
    float out[252];
    memset(out, 0, sizeof(out));
    int count = diffraction_pack(test_positions, test_colors,
                                  test_magnitudes, 3, &cfg, out);
    TEST_ASSERT_EQUAL_INT(2, count);
    /* Only 6 spikes worth of data per star should be generated.
     * After star 0 (6 spikes * 2 verts * 7 floats = 84 floats),
     * star 1's first center vertex should be at index 84. */
    int s1_base = 6 * 2 * DIFFRACTION_VERTEX_FLOATS;
    /* Star 1 center should be at its position (0, 1, 0) */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, out[s1_base + 0]);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, out[s1_base + 1]);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, out[s1_base + 2]);
}

void test_pack_rotation_offsets_spikes(void)
{
    /* Two packs with different rotation should produce different tip positions */
    diffraction_config_t cfg1 = diffraction_default_config();
    cfg1.rotation_deg = 0.0f;
    diffraction_config_t cfg2 = diffraction_default_config();
    cfg2.rotation_deg = 30.0f;
    float out1[168], out2[168];
    memset(out1, 0, sizeof(out1));
    memset(out2, 0, sizeof(out2));
    diffraction_pack(test_positions, test_colors, test_magnitudes,
                     3, &cfg1, out1);
    diffraction_pack(test_positions, test_colors, test_magnitudes,
                     3, &cfg2, out2);
    /* First spike tip positions should differ */
    int tip_base = DIFFRACTION_VERTEX_FLOATS;
    int differ = 0;
    for (int j = 0; j < 3; j++) {
        if (fabsf(out1[tip_base + j] - out2[tip_base + j]) > 0.001f) {
            differ = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(differ);
}

/* ===================================================================
 * 6. Shader source strings
 * =================================================================== */

void test_vert_source_not_null(void)
{
    TEST_ASSERT_NOT_NULL(diffraction_vert_source());
}

void test_frag_source_not_null(void)
{
    TEST_ASSERT_NOT_NULL(diffraction_frag_source());
}

void test_vert_has_version_300_es(void)
{
    TEST_ASSERT_NOT_NULL(strstr(diffraction_vert_source(), "#version 300 es"));
}

void test_frag_has_version_300_es(void)
{
    TEST_ASSERT_NOT_NULL(strstr(diffraction_frag_source(), "#version 300 es"));
}

void test_vert_has_mvp_uniform(void)
{
    TEST_ASSERT_NOT_NULL(strstr(diffraction_vert_source(), "u_mvp"));
}

void test_vert_has_position_attribute(void)
{
    TEST_ASSERT_NOT_NULL(strstr(diffraction_vert_source(), "a_position"));
}

void test_vert_has_color_attribute(void)
{
    TEST_ASSERT_NOT_NULL(strstr(diffraction_vert_source(), "a_color"));
}

void test_frag_has_frag_color_output(void)
{
    TEST_ASSERT_NOT_NULL(strstr(diffraction_frag_source(), "frag_color"));
}

/* ===================================================================
 * main
 * =================================================================== */

int main(void)
{
    UNITY_BEGIN();

    /* Spike length (6 tests) */
    RUN_TEST(test_spike_length_magnitude_zero);
    RUN_TEST(test_spike_length_at_threshold);
    RUN_TEST(test_spike_length_above_threshold);
    RUN_TEST(test_spike_length_mid_brightness);
    RUN_TEST(test_spike_length_scales_with_base);
    RUN_TEST(test_spike_length_negative_magnitude);

    /* Spike alpha (6 tests) */
    RUN_TEST(test_spike_alpha_at_center);
    RUN_TEST(test_spike_alpha_at_tip);
    RUN_TEST(test_spike_alpha_midpoint);
    RUN_TEST(test_spike_alpha_nonzero_tip);
    RUN_TEST(test_spike_alpha_clamped_t_negative);
    RUN_TEST(test_spike_alpha_clamped_t_above_one);

    /* Default config (5 tests) */
    RUN_TEST(test_default_config_base_length);
    RUN_TEST(test_default_config_min_magnitude);
    RUN_TEST(test_default_config_center_alpha);
    RUN_TEST(test_default_config_tip_alpha);
    RUN_TEST(test_default_config_spike_count);

    /* Info metadata (3 tests) */
    RUN_TEST(test_info_single_star);
    RUN_TEST(test_info_multiple_stars);
    RUN_TEST(test_info_zero_stars);

    /* Packing (14 tests) */
    RUN_TEST(test_pack_returns_bright_star_count);
    RUN_TEST(test_pack_dim_star_excluded);
    RUN_TEST(test_pack_center_vertex_at_star_position);
    RUN_TEST(test_pack_center_vertex_has_star_color);
    RUN_TEST(test_pack_center_vertex_has_center_alpha);
    RUN_TEST(test_pack_tip_vertex_has_tip_alpha);
    RUN_TEST(test_pack_tip_vertex_offset_from_center);
    RUN_TEST(test_pack_spikes_evenly_spaced);
    RUN_TEST(test_pack_all_spikes_same_length_per_star);
    RUN_TEST(test_pack_brighter_star_longer_spikes);
    RUN_TEST(test_pack_spike_perpendicular_to_star_direction);
    RUN_TEST(test_pack_null_out_returns_zero);
    RUN_TEST(test_pack_zero_count);
    RUN_TEST(test_pack_six_spikes);
    RUN_TEST(test_pack_spike_count_clamped_to_4);
    RUN_TEST(test_pack_spike_count_clamped_to_6);
    RUN_TEST(test_pack_rotation_offsets_spikes);

    /* Shader sources (8 tests) */
    RUN_TEST(test_vert_source_not_null);
    RUN_TEST(test_frag_source_not_null);
    RUN_TEST(test_vert_has_version_300_es);
    RUN_TEST(test_frag_has_version_300_es);
    RUN_TEST(test_vert_has_mvp_uniform);
    RUN_TEST(test_vert_has_position_attribute);
    RUN_TEST(test_vert_has_color_attribute);
    RUN_TEST(test_frag_has_frag_color_output);

    return UNITY_END();
}
