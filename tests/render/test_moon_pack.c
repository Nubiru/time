/*
 * test_moon_pack.c -- Tests for moon render pack module
 *
 * Tests: moon vertex packing, orbit trail packing, byte sizes,
 *        shader source strings, position correctness, albedo-based colors.
 */

#include "../unity/unity.h"
#include "../../src/render/moon_pack.h"
#include "../../src/systems/astronomy/moon_data.h"
#include "../../src/systems/astronomy/planets.h"

#include <math.h>
#include <string.h>

#define PI 3.14159265358979323846

/* J2000.0 epoch */
#define J2000 2451545.0
/* A different date: 2024-06-15 */
#define JD_2024 2460476.5

/* Standard test parameters */
#define SCENE_SCALE 10.0f
#define MOON_SCALE  5.0f
#define RADIUS_SCALE 0.01f
#define TRAIL_SAMPLES 30

void setUp(void) {}
void tearDown(void) {}

/* ===================================================================
 * 1. mp_pack_moons -- basic moon packing
 * =================================================================== */

void test_pack_moons_returns_9(void)
{
    mp_moon_data_t data = mp_pack_moons(J2000, SCENE_SCALE, MOON_SCALE,
                                         RADIUS_SCALE);
    TEST_ASSERT_EQUAL_INT(9, data.moon_count);
}

void test_pack_moons_positions_nonzero(void)
{
    mp_moon_data_t data = mp_pack_moons(J2000, SCENE_SCALE, MOON_SCALE,
                                         RADIUS_SCALE);
    for (int i = 0; i < data.moon_count; i++) {
        int base = i * MP_VERTEX_FLOATS;
        float x = data.vertices[base + 0];
        float y = data.vertices[base + 1];
        float z = data.vertices[base + 2];
        float dist = sqrtf(x * x + y * y + z * z);
        TEST_ASSERT_TRUE(dist > 0.0f);
    }
}

void test_pack_moons_parent_earth(void)
{
    /* First moon (index 0) should be Earth's Moon, parent = 2 */
    mp_moon_data_t data = mp_pack_moons(J2000, SCENE_SCALE, MOON_SCALE,
                                         RADIUS_SCALE);
    TEST_ASSERT_EQUAL_INT(2, data.moon_parent[0]);
}

void test_pack_moons_parent_mars(void)
{
    /* Moons 1-2 should be Mars moons, parent = 3 */
    mp_moon_data_t data = mp_pack_moons(J2000, SCENE_SCALE, MOON_SCALE,
                                         RADIUS_SCALE);
    TEST_ASSERT_EQUAL_INT(3, data.moon_parent[1]);
    TEST_ASSERT_EQUAL_INT(3, data.moon_parent[2]);
}

void test_pack_moons_parent_jupiter(void)
{
    /* Moons 3-6 should be Jupiter Galilean moons, parent = 4 */
    mp_moon_data_t data = mp_pack_moons(J2000, SCENE_SCALE, MOON_SCALE,
                                         RADIUS_SCALE);
    TEST_ASSERT_EQUAL_INT(4, data.moon_parent[3]);
    TEST_ASSERT_EQUAL_INT(4, data.moon_parent[4]);
    TEST_ASSERT_EQUAL_INT(4, data.moon_parent[5]);
    TEST_ASSERT_EQUAL_INT(4, data.moon_parent[6]);
}

void test_pack_moons_parent_saturn(void)
{
    /* Moons 7-8 should be Saturn moons, parent = 5 */
    mp_moon_data_t data = mp_pack_moons(J2000, SCENE_SCALE, MOON_SCALE,
                                         RADIUS_SCALE);
    TEST_ASSERT_EQUAL_INT(5, data.moon_parent[7]);
    TEST_ASSERT_EQUAL_INT(5, data.moon_parent[8]);
}

void test_pack_moons_positions_differ_by_planet(void)
{
    /* Earth moon should be near Earth, Jupiter moon should be near Jupiter */
    mp_moon_data_t data = mp_pack_moons(J2000, SCENE_SCALE, MOON_SCALE,
                                         RADIUS_SCALE);
    /* Earth moon (index 0) position */
    float ex = data.vertices[0 * MP_VERTEX_FLOATS + 0];
    float ey = data.vertices[0 * MP_VERTEX_FLOATS + 1];
    float ez = data.vertices[0 * MP_VERTEX_FLOATS + 2];
    float earth_dist = sqrtf(ex * ex + ey * ey + ez * ez);

    /* Io (Jupiter moon, index 3) position */
    float jx = data.vertices[3 * MP_VERTEX_FLOATS + 0];
    float jy = data.vertices[3 * MP_VERTEX_FLOATS + 1];
    float jz = data.vertices[3 * MP_VERTEX_FLOATS + 2];
    float jup_dist = sqrtf(jx * jx + jy * jy + jz * jz);

    /* Jupiter is farther from Sun than Earth */
    TEST_ASSERT_TRUE(jup_dist > earth_dist);
}

void test_pack_moons_colors_proportional_to_albedo(void)
{
    /* Enceladus (albedo 0.99) should be brighter than Callisto (albedo 0.17) */
    mp_moon_data_t data = mp_pack_moons(J2000, SCENE_SCALE, MOON_SCALE,
                                         RADIUS_SCALE);
    /* Enceladus = moon index 8, Callisto = moon index 6 */
    float enceladus_r = data.vertices[8 * MP_VERTEX_FLOATS + 3];
    float callisto_r = data.vertices[6 * MP_VERTEX_FLOATS + 3];
    TEST_ASSERT_TRUE(enceladus_r > callisto_r);
}

void test_pack_moons_colors_valid_range(void)
{
    mp_moon_data_t data = mp_pack_moons(J2000, SCENE_SCALE, MOON_SCALE,
                                         RADIUS_SCALE);
    for (int i = 0; i < data.moon_count; i++) {
        int base = i * MP_VERTEX_FLOATS;
        float r = data.vertices[base + 3];
        float g = data.vertices[base + 4];
        float b = data.vertices[base + 5];
        TEST_ASSERT_TRUE(r >= 0.0f && r <= 1.0f);
        TEST_ASSERT_TRUE(g >= 0.0f && g <= 1.0f);
        TEST_ASSERT_TRUE(b >= 0.0f && b <= 1.0f);
    }
}

void test_pack_moons_radii_positive(void)
{
    mp_moon_data_t data = mp_pack_moons(J2000, SCENE_SCALE, MOON_SCALE,
                                         RADIUS_SCALE);
    for (int i = 0; i < data.moon_count; i++) {
        int base = i * MP_VERTEX_FLOATS;
        float radius = data.vertices[base + 6];
        TEST_ASSERT_TRUE(radius > 0.0f);
    }
}

void test_pack_moons_different_jd_different_positions(void)
{
    mp_moon_data_t d1 = mp_pack_moons(J2000, SCENE_SCALE, MOON_SCALE,
                                        RADIUS_SCALE);
    mp_moon_data_t d2 = mp_pack_moons(JD_2024, SCENE_SCALE, MOON_SCALE,
                                        RADIUS_SCALE);
    int differ = 0;
    for (int i = 0; i < 9 * MP_VERTEX_FLOATS; i++) {
        if (fabsf(d1.vertices[i] - d2.vertices[i]) > 0.001f) {
            differ = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(differ);
}

void test_pack_moons_all_nonidentical_positions(void)
{
    /* No two moons should be at the exact same position */
    mp_moon_data_t data = mp_pack_moons(J2000, SCENE_SCALE, MOON_SCALE,
                                         RADIUS_SCALE);
    for (int i = 0; i < data.moon_count; i++) {
        for (int j = i + 1; j < data.moon_count; j++) {
            int bi = i * MP_VERTEX_FLOATS;
            int bj = j * MP_VERTEX_FLOATS;
            float dx = data.vertices[bi + 0] - data.vertices[bj + 0];
            float dy = data.vertices[bi + 1] - data.vertices[bj + 1];
            float dz = data.vertices[bi + 2] - data.vertices[bj + 2];
            float dist = sqrtf(dx * dx + dy * dy + dz * dz);
            TEST_ASSERT_TRUE(dist > 0.0001f);
        }
    }
}

void test_pack_moons_galilean_different_angles(void)
{
    /* Galilean moons (indices 3-6) should have different positions
     * relative to Jupiter */
    mp_moon_data_t data = mp_pack_moons(J2000, SCENE_SCALE, MOON_SCALE,
                                         RADIUS_SCALE);
    /* Check pairwise differences among Galilean moon positions */
    for (int i = 3; i <= 6; i++) {
        for (int j = i + 1; j <= 6; j++) {
            int bi = i * MP_VERTEX_FLOATS;
            int bj = j * MP_VERTEX_FLOATS;
            float dx = data.vertices[bi + 0] - data.vertices[bj + 0];
            float dy = data.vertices[bi + 1] - data.vertices[bj + 1];
            float dz = data.vertices[bi + 2] - data.vertices[bj + 2];
            float dist = sqrtf(dx * dx + dy * dy + dz * dz);
            TEST_ASSERT_TRUE(dist > 0.0001f);
        }
    }
}

void test_pack_moons_scene_scale_affects_positions(void)
{
    mp_moon_data_t d1 = mp_pack_moons(J2000, 10.0f, MOON_SCALE, RADIUS_SCALE);
    mp_moon_data_t d2 = mp_pack_moons(J2000, 20.0f, MOON_SCALE, RADIUS_SCALE);
    /* Earth Moon (index 0) at scale 20 should be ~2x farther from origin */
    float dist1 = sqrtf(d1.vertices[0] * d1.vertices[0] +
                          d1.vertices[1] * d1.vertices[1] +
                          d1.vertices[2] * d1.vertices[2]);
    float dist2 = sqrtf(d2.vertices[0] * d2.vertices[0] +
                          d2.vertices[1] * d2.vertices[1] +
                          d2.vertices[2] * d2.vertices[2]);
    /* Parent position scales linearly, moon offset also scales, so ratio ~2x */
    TEST_ASSERT_FLOAT_WITHIN(dist1 * 0.5f, dist1 * 2.0f, dist2);
}

void test_pack_moons_moon_scale_affects_offset(void)
{
    /* Increasing moon_scale should change moon positions relative to parent */
    mp_moon_data_t d1 = mp_pack_moons(J2000, SCENE_SCALE, 5.0f, RADIUS_SCALE);
    mp_moon_data_t d2 = mp_pack_moons(J2000, SCENE_SCALE, 10.0f, RADIUS_SCALE);
    /* Positions should differ */
    int differ = 0;
    for (int i = 0; i < 9 * MP_VERTEX_FLOATS; i++) {
        if (fabsf(d1.vertices[i] - d2.vertices[i]) > 0.0001f) {
            differ = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(differ);
}

void test_pack_moons_radius_scale_affects_radii(void)
{
    mp_moon_data_t d1 = mp_pack_moons(J2000, SCENE_SCALE, MOON_SCALE, 0.01f);
    mp_moon_data_t d2 = mp_pack_moons(J2000, SCENE_SCALE, MOON_SCALE, 0.02f);
    /* All radii at scale 0.02 should be 2x those at scale 0.01 */
    for (int i = 0; i < 9; i++) {
        int base = i * MP_VERTEX_FLOATS;
        float r1 = d1.vertices[base + 6];
        float r2 = d2.vertices[base + 6];
        TEST_ASSERT_FLOAT_WITHIN(0.001f, r1 * 2.0f, r2);
    }
}

/* ===================================================================
 * 2. mp_pack_trails -- orbit trail packing
 * =================================================================== */

void test_pack_trails_for_all_9_moons(void)
{
    mp_trail_data_t data = mp_pack_trails(J2000, SCENE_SCALE, MOON_SCALE,
                                           TRAIL_SAMPLES);
    TEST_ASSERT_EQUAL_INT(9 * TRAIL_SAMPLES, data.total_vertex_count);
}

void test_pack_trails_offsets_sequential(void)
{
    mp_trail_data_t data = mp_pack_trails(J2000, SCENE_SCALE, MOON_SCALE,
                                           TRAIL_SAMPLES);
    for (int i = 0; i < 8; i++) {
        TEST_ASSERT_EQUAL_INT(data.trail_offsets[i] + data.trail_counts[i],
                              data.trail_offsets[i + 1]);
    }
}

void test_pack_trails_counts_match_samples(void)
{
    mp_trail_data_t data = mp_pack_trails(J2000, SCENE_SCALE, MOON_SCALE,
                                           TRAIL_SAMPLES);
    for (int i = 0; i < 9; i++) {
        TEST_ASSERT_EQUAL_INT(TRAIL_SAMPLES, data.trail_counts[i]);
    }
}

void test_pack_trails_counts_sum_to_total(void)
{
    mp_trail_data_t data = mp_pack_trails(J2000, SCENE_SCALE, MOON_SCALE,
                                           TRAIL_SAMPLES);
    int sum = 0;
    for (int i = 0; i < 9; i++) {
        sum += data.trail_counts[i];
    }
    TEST_ASSERT_EQUAL_INT(data.total_vertex_count, sum);
}

void test_pack_trails_alpha_fades(void)
{
    /* First vertex (current) should have higher alpha than last */
    mp_trail_data_t data = mp_pack_trails(J2000, SCENE_SCALE, MOON_SCALE,
                                           TRAIL_SAMPLES);
    int off = data.trail_offsets[0];
    int cnt = data.trail_counts[0];
    int first_base = off * MP_TRAIL_VERTEX_FLOATS;
    int last_base = (off + cnt - 1) * MP_TRAIL_VERTEX_FLOATS;
    float first_alpha = data.vertices[first_base + 6];
    float last_alpha = data.vertices[last_base + 6];
    TEST_ASSERT_TRUE(first_alpha > last_alpha);
}

void test_pack_trails_first_alpha_near_1(void)
{
    mp_trail_data_t data = mp_pack_trails(J2000, SCENE_SCALE, MOON_SCALE,
                                           TRAIL_SAMPLES);
    int off = data.trail_offsets[0];
    int first_base = off * MP_TRAIL_VERTEX_FLOATS;
    float alpha = data.vertices[first_base + 6];
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 1.0f, alpha);
}

void test_pack_trails_last_alpha_near_01(void)
{
    mp_trail_data_t data = mp_pack_trails(J2000, SCENE_SCALE, MOON_SCALE,
                                           TRAIL_SAMPLES);
    int off = data.trail_offsets[0];
    int cnt = data.trail_counts[0];
    int last_base = (off + cnt - 1) * MP_TRAIL_VERTEX_FLOATS;
    float alpha = data.vertices[last_base + 6];
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 0.1f, alpha);
}

void test_pack_trails_samples_clamped_at_max(void)
{
    mp_trail_data_t data = mp_pack_trails(J2000, SCENE_SCALE, MOON_SCALE, 9999);
    TEST_ASSERT_EQUAL_INT(9 * MP_TRAIL_MAX_SAMPLES, data.total_vertex_count);
}

void test_pack_trails_samples_minimum_1(void)
{
    mp_trail_data_t data = mp_pack_trails(J2000, SCENE_SCALE, MOON_SCALE, 0);
    TEST_ASSERT_EQUAL_INT(9, data.total_vertex_count);
}

void test_pack_trails_trail_colors_grayscale(void)
{
    /* Trail colors should be grayscale (R == G == B, from albedo) */
    mp_trail_data_t data = mp_pack_trails(J2000, SCENE_SCALE, MOON_SCALE,
                                           TRAIL_SAMPLES);
    for (int i = 0; i < 9; i++) {
        int base = data.trail_offsets[i] * MP_TRAIL_VERTEX_FLOATS;
        float r = data.vertices[base + 3];
        float g = data.vertices[base + 4];
        float b = data.vertices[base + 5];
        TEST_ASSERT_FLOAT_WITHIN(0.001f, r, g);
        TEST_ASSERT_FLOAT_WITHIN(0.001f, r, b);
    }
}

void test_pack_trails_different_samples_different_count(void)
{
    mp_trail_data_t d1 = mp_pack_trails(J2000, SCENE_SCALE, MOON_SCALE, 20);
    mp_trail_data_t d2 = mp_pack_trails(J2000, SCENE_SCALE, MOON_SCALE, 40);
    TEST_ASSERT_NOT_EQUAL(d1.total_vertex_count, d2.total_vertex_count);
}

/* ===================================================================
 * 3. Byte size functions
 * =================================================================== */

void test_moon_vertex_bytes_correct(void)
{
    mp_moon_data_t data = mp_pack_moons(J2000, SCENE_SCALE, MOON_SCALE,
                                         RADIUS_SCALE);
    int expected = 9 * MP_VERTEX_FLOATS * (int)sizeof(float);
    TEST_ASSERT_EQUAL_INT(expected, mp_moon_vertex_bytes(&data));
}

void test_trail_vertex_bytes_correct(void)
{
    mp_trail_data_t data = mp_pack_trails(J2000, SCENE_SCALE, MOON_SCALE,
                                           TRAIL_SAMPLES);
    int expected = data.total_vertex_count * MP_TRAIL_VERTEX_FLOATS *
                   (int)sizeof(float);
    TEST_ASSERT_EQUAL_INT(expected, mp_trail_vertex_bytes(&data));
}

void test_trail_vertex_bytes_different_samples(void)
{
    mp_trail_data_t data = mp_pack_trails(J2000, SCENE_SCALE, MOON_SCALE, 50);
    int expected = (9 * 50) * MP_TRAIL_VERTEX_FLOATS * (int)sizeof(float);
    TEST_ASSERT_EQUAL_INT(expected, mp_trail_vertex_bytes(&data));
}

/* ===================================================================
 * 4. Shader source strings
 * =================================================================== */

void test_moon_vert_source_not_null(void)
{
    TEST_ASSERT_NOT_NULL(mp_moon_vert_source());
}

void test_moon_frag_source_not_null(void)
{
    TEST_ASSERT_NOT_NULL(mp_moon_frag_source());
}

void test_trail_vert_source_not_null(void)
{
    TEST_ASSERT_NOT_NULL(mp_trail_vert_source());
}

void test_trail_frag_source_not_null(void)
{
    TEST_ASSERT_NOT_NULL(mp_trail_frag_source());
}

void test_moon_vert_has_version(void)
{
    TEST_ASSERT_NOT_NULL(strstr(mp_moon_vert_source(), "#version 300 es"));
}

void test_moon_frag_has_version(void)
{
    TEST_ASSERT_NOT_NULL(strstr(mp_moon_frag_source(), "#version 300 es"));
}

void test_trail_vert_has_version(void)
{
    TEST_ASSERT_NOT_NULL(strstr(mp_trail_vert_source(), "#version 300 es"));
}

void test_trail_frag_has_version(void)
{
    TEST_ASSERT_NOT_NULL(strstr(mp_trail_frag_source(), "#version 300 es"));
}

void test_moon_vert_has_point_size(void)
{
    TEST_ASSERT_NOT_NULL(strstr(mp_moon_vert_source(), "gl_PointSize"));
}

void test_moon_vert_has_mvp(void)
{
    TEST_ASSERT_NOT_NULL(strstr(mp_moon_vert_source(), "u_mvp"));
}

void test_moon_frag_has_point_coord(void)
{
    TEST_ASSERT_NOT_NULL(strstr(mp_moon_frag_source(), "gl_PointCoord"));
}

void test_trail_vert_has_mvp(void)
{
    TEST_ASSERT_NOT_NULL(strstr(mp_trail_vert_source(), "u_mvp"));
}

/* ===================================================================
 * main
 * =================================================================== */

int main(void)
{
    UNITY_BEGIN();

    /* Moon packing (16 tests) */
    RUN_TEST(test_pack_moons_returns_9);
    RUN_TEST(test_pack_moons_positions_nonzero);
    RUN_TEST(test_pack_moons_parent_earth);
    RUN_TEST(test_pack_moons_parent_mars);
    RUN_TEST(test_pack_moons_parent_jupiter);
    RUN_TEST(test_pack_moons_parent_saturn);
    RUN_TEST(test_pack_moons_positions_differ_by_planet);
    RUN_TEST(test_pack_moons_colors_proportional_to_albedo);
    RUN_TEST(test_pack_moons_colors_valid_range);
    RUN_TEST(test_pack_moons_radii_positive);
    RUN_TEST(test_pack_moons_different_jd_different_positions);
    RUN_TEST(test_pack_moons_all_nonidentical_positions);
    RUN_TEST(test_pack_moons_galilean_different_angles);
    RUN_TEST(test_pack_moons_scene_scale_affects_positions);
    RUN_TEST(test_pack_moons_moon_scale_affects_offset);
    RUN_TEST(test_pack_moons_radius_scale_affects_radii);

    /* Trail packing (11 tests) */
    RUN_TEST(test_pack_trails_for_all_9_moons);
    RUN_TEST(test_pack_trails_offsets_sequential);
    RUN_TEST(test_pack_trails_counts_match_samples);
    RUN_TEST(test_pack_trails_counts_sum_to_total);
    RUN_TEST(test_pack_trails_alpha_fades);
    RUN_TEST(test_pack_trails_first_alpha_near_1);
    RUN_TEST(test_pack_trails_last_alpha_near_01);
    RUN_TEST(test_pack_trails_samples_clamped_at_max);
    RUN_TEST(test_pack_trails_samples_minimum_1);
    RUN_TEST(test_pack_trails_trail_colors_grayscale);
    RUN_TEST(test_pack_trails_different_samples_different_count);

    /* Byte sizes (3 tests) */
    RUN_TEST(test_moon_vertex_bytes_correct);
    RUN_TEST(test_trail_vertex_bytes_correct);
    RUN_TEST(test_trail_vertex_bytes_different_samples);

    /* Shaders (12 tests) */
    RUN_TEST(test_moon_vert_source_not_null);
    RUN_TEST(test_moon_frag_source_not_null);
    RUN_TEST(test_trail_vert_source_not_null);
    RUN_TEST(test_trail_frag_source_not_null);
    RUN_TEST(test_moon_vert_has_version);
    RUN_TEST(test_moon_frag_has_version);
    RUN_TEST(test_trail_vert_has_version);
    RUN_TEST(test_trail_frag_has_version);
    RUN_TEST(test_moon_vert_has_point_size);
    RUN_TEST(test_moon_vert_has_mvp);
    RUN_TEST(test_moon_frag_has_point_coord);
    RUN_TEST(test_trail_vert_has_mvp);

    return UNITY_END();
}
