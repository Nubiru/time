/*
 * test_planet_pack.c — tests for planet render pack module
 *
 * Tests: planet vertex packing, orbit trail packing, byte sizes,
 *        shader source strings, position correctness, color validity.
 */

#include "../unity/unity.h"
#include "../../src/render/planet_pack.h"
#include "../../src/systems/astronomy/planets.h"
#include "../../src/systems/astronomy/planet_data.h"
#include "../../src/render/atmo_ring.h"

#include <math.h>
#include <string.h>

#ifndef PI
#define PI 3.14159265358979323846
#endif

/* J2000.0 epoch */
#define J2000 2451545.0
/* A different date: 2024-06-15 */
#define JD_2024 2460476.5

void setUp(void) {}
void tearDown(void) {}

/* ═══════════════════════════════════════════════════════════════════════
 * 1. pp_pack_planets — basic planet packing
 * ═══════════════════════════════════════════════════════════════════════ */

void test_pack_planets_returns_8(void)
{
    pp_planet_data_t data = pp_pack_planets(J2000, 10.0f, 0.01f);
    TEST_ASSERT_EQUAL_INT(8, data.planet_count);
}

void test_pack_planets_positions_nonzero(void)
{
    pp_planet_data_t data = pp_pack_planets(J2000, 10.0f, 0.01f);
    for (int i = 0; i < 8; i++) {
        int base = i * PP_PLANET_VERTEX_FLOATS;
        float x = data.vertices[base + 0];
        float y = data.vertices[base + 1];
        float z = data.vertices[base + 2];
        float dist = sqrtf(x * x + y * y + z * z);
        TEST_ASSERT_TRUE(dist > 0.0f);
    }
}

void test_pack_planets_mercury_closest(void)
{
    pp_planet_data_t data = pp_pack_planets(J2000, 10.0f, 0.01f);
    /* Mercury = index 0, distance should be smallest */
    float min_dist = 1e30f;
    int min_idx = -1;
    for (int i = 0; i < 8; i++) {
        int base = i * PP_PLANET_VERTEX_FLOATS;
        float x = data.vertices[base + 0];
        float y = data.vertices[base + 1];
        float z = data.vertices[base + 2];
        float dist = sqrtf(x * x + y * y + z * z);
        if (dist < min_dist) {
            min_dist = dist;
            min_idx = i;
        }
    }
    TEST_ASSERT_EQUAL_INT(0, min_idx); /* Mercury */
}

void test_pack_planets_neptune_farthest(void)
{
    pp_planet_data_t data = pp_pack_planets(J2000, 10.0f, 0.01f);
    /* Neptune = index 7, distance should be largest */
    float max_dist = 0.0f;
    int max_idx = -1;
    for (int i = 0; i < 8; i++) {
        int base = i * PP_PLANET_VERTEX_FLOATS;
        float x = data.vertices[base + 0];
        float y = data.vertices[base + 1];
        float z = data.vertices[base + 2];
        float dist = sqrtf(x * x + y * y + z * z);
        if (dist > max_dist) {
            max_dist = dist;
            max_idx = i;
        }
    }
    TEST_ASSERT_EQUAL_INT(7, max_idx); /* Neptune */
}

void test_pack_planets_colors_valid_range(void)
{
    pp_planet_data_t data = pp_pack_planets(J2000, 10.0f, 0.01f);
    for (int i = 0; i < 8; i++) {
        int base = i * PP_PLANET_VERTEX_FLOATS;
        float r = data.vertices[base + 3];
        float g = data.vertices[base + 4];
        float b = data.vertices[base + 5];
        TEST_ASSERT_TRUE(r >= 0.0f && r <= 1.0f);
        TEST_ASSERT_TRUE(g >= 0.0f && g <= 1.0f);
        TEST_ASSERT_TRUE(b >= 0.0f && b <= 1.0f);
    }
}

void test_pack_planets_radii_positive(void)
{
    pp_planet_data_t data = pp_pack_planets(J2000, 10.0f, 0.01f);
    for (int i = 0; i < 8; i++) {
        int base = i * PP_PLANET_VERTEX_FLOATS;
        float radius = data.vertices[base + 6];
        TEST_ASSERT_TRUE(radius > 0.0f);
    }
}

void test_pack_planets_earth_distance_about_1au(void)
{
    /* Earth at J2000 should be ~1 AU from Sun, so ~10 scene units at scale 10 */
    pp_planet_data_t data = pp_pack_planets(J2000, 10.0f, 0.01f);
    int base = 2 * PP_PLANET_VERTEX_FLOATS; /* Earth = index 2 */
    float x = data.vertices[base + 0];
    float y = data.vertices[base + 1];
    float z = data.vertices[base + 2];
    float dist = sqrtf(x * x + y * y + z * z);
    /* Should be roughly 10.0 scene units (1 AU * 10 scale) */
    TEST_ASSERT_FLOAT_WITHIN(2.0f, 10.0f, dist);
}

void test_pack_planets_different_jd_different_positions(void)
{
    pp_planet_data_t d1 = pp_pack_planets(J2000, 10.0f, 0.01f);
    pp_planet_data_t d2 = pp_pack_planets(JD_2024, 10.0f, 0.01f);
    /* At least one planet should have moved */
    int differ = 0;
    for (int i = 0; i < 8 * PP_PLANET_VERTEX_FLOATS; i++) {
        if (fabsf(d1.vertices[i] - d2.vertices[i]) > 0.001f) {
            differ = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(differ);
}

void test_pack_planets_scene_scale_affects_positions(void)
{
    pp_planet_data_t d1 = pp_pack_planets(J2000, 10.0f, 0.01f);
    pp_planet_data_t d2 = pp_pack_planets(J2000, 20.0f, 0.01f);
    /* Earth position at scale 20 should be ~2x farther from origin */
    int base = 2 * PP_PLANET_VERTEX_FLOATS;
    float dist1 = sqrtf(d1.vertices[base] * d1.vertices[base] +
                         d1.vertices[base + 1] * d1.vertices[base + 1] +
                         d1.vertices[base + 2] * d1.vertices[base + 2]);
    float dist2 = sqrtf(d2.vertices[base] * d2.vertices[base] +
                         d2.vertices[base + 1] * d2.vertices[base + 1] +
                         d2.vertices[base + 2] * d2.vertices[base + 2]);
    TEST_ASSERT_FLOAT_WITHIN(0.5f, dist1 * 2.0f, dist2);
}

void test_pack_planets_radius_scale_affects_radii(void)
{
    pp_planet_data_t d1 = pp_pack_planets(J2000, 10.0f, 0.01f);
    pp_planet_data_t d2 = pp_pack_planets(J2000, 10.0f, 0.02f);
    /* All radii at scale 0.02 should be 2x those at scale 0.01 */
    for (int i = 0; i < 8; i++) {
        int base = i * PP_PLANET_VERTEX_FLOATS;
        float r1 = d1.vertices[base + 6];
        float r2 = d2.vertices[base + 6];
        TEST_ASSERT_FLOAT_WITHIN(0.001f, r1 * 2.0f, r2);
    }
}

void test_pack_planets_colors_match_planet_data(void)
{
    pp_planet_data_t data = pp_pack_planets(J2000, 10.0f, 0.01f);
    for (int i = 0; i < 8; i++) {
        planet_color_t expected = planet_data_color(i);
        int base = i * PP_PLANET_VERTEX_FLOATS;
        TEST_ASSERT_FLOAT_WITHIN(0.001f, expected.r, data.vertices[base + 3]);
        TEST_ASSERT_FLOAT_WITHIN(0.001f, expected.g, data.vertices[base + 4]);
        TEST_ASSERT_FLOAT_WITHIN(0.001f, expected.b, data.vertices[base + 5]);
    }
}

void test_pack_planets_atmo_thickness_from_atmo_get(void)
{
    pp_planet_data_t data = pp_pack_planets(J2000, 10.0f, 0.01f);
    for (int i = 0; i < 8; i++) {
        atmo_properties_t atmo = atmo_get(i);
        int base = i * PP_PLANET_VERTEX_FLOATS;
        TEST_ASSERT_FLOAT_WITHIN(0.001f, atmo.thickness_ratio,
                                  data.vertices[base + 7]);
    }
}

void test_pack_planets_mercury_no_atmosphere(void)
{
    pp_planet_data_t data = pp_pack_planets(J2000, 10.0f, 0.01f);
    /* Mercury (index 0) has no atmosphere: thickness = 0 */
    float atmo = data.vertices[0 * PP_PLANET_VERTEX_FLOATS + 7];
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, atmo);
}

void test_pack_planets_earth_has_atmosphere(void)
{
    pp_planet_data_t data = pp_pack_planets(J2000, 10.0f, 0.01f);
    /* Earth (index 2) has atmosphere: thickness > 0 */
    float atmo = data.vertices[2 * PP_PLANET_VERTEX_FLOATS + 7];
    TEST_ASSERT_TRUE(atmo > 0.0f);
}

void test_pack_planets_jupiter_larger_radius_than_earth(void)
{
    pp_planet_data_t data = pp_pack_planets(J2000, 10.0f, 0.01f);
    float earth_r = data.vertices[2 * PP_PLANET_VERTEX_FLOATS + 6];
    float jupiter_r = data.vertices[4 * PP_PLANET_VERTEX_FLOATS + 6];
    TEST_ASSERT_TRUE(jupiter_r > earth_r);
}

/* ═══════════════════════════════════════════════════════════════════════
 * 2. pp_pack_trails — orbit trail packing
 * ═══════════════════════════════════════════════════════════════════════ */

void test_pack_trails_total_count(void)
{
    pp_trail_data_t data = pp_pack_trails(J2000, 10.0f, 36);
    TEST_ASSERT_EQUAL_INT(8 * 36, data.total_vertex_count);
}

void test_pack_trails_all_offsets_set(void)
{
    pp_trail_data_t data = pp_pack_trails(J2000, 10.0f, 36);
    for (int i = 0; i < 8; i++) {
        TEST_ASSERT_TRUE(data.trail_offsets[i] >= 0);
    }
}

void test_pack_trails_counts_sum_to_total(void)
{
    pp_trail_data_t data = pp_pack_trails(J2000, 10.0f, 36);
    int sum = 0;
    for (int i = 0; i < 8; i++) {
        sum += data.trail_counts[i];
    }
    TEST_ASSERT_EQUAL_INT(data.total_vertex_count, sum);
}

void test_pack_trails_positions_roughly_circular(void)
{
    /* Earth orbit: all trail points should be ~1 AU from origin (10 scene units) */
    pp_trail_data_t data = pp_pack_trails(J2000, 10.0f, 36);
    int earth_off = data.trail_offsets[2];
    int earth_cnt = data.trail_counts[2];
    for (int j = 0; j < earth_cnt; j++) {
        int base = (earth_off + j) * PP_TRAIL_VERTEX_FLOATS;
        float x = data.vertices[base + 0];
        float y = data.vertices[base + 1];
        float z = data.vertices[base + 2];
        float dist = sqrtf(x * x + y * y + z * z);
        /* Earth orbit is nearly circular, ~10 scene units +/- tolerance */
        TEST_ASSERT_FLOAT_WITHIN(2.0f, 10.0f, dist);
    }
}

void test_pack_trails_alpha_decreases(void)
{
    /* First vertex (current) should have higher alpha than last */
    pp_trail_data_t data = pp_pack_trails(J2000, 10.0f, 36);
    int off = data.trail_offsets[0]; /* Mercury */
    int cnt = data.trail_counts[0];
    int first_base = off * PP_TRAIL_VERTEX_FLOATS;
    int last_base = (off + cnt - 1) * PP_TRAIL_VERTEX_FLOATS;
    float first_alpha = data.vertices[first_base + 6];
    float last_alpha = data.vertices[last_base + 6];
    TEST_ASSERT_TRUE(first_alpha > last_alpha);
}

void test_pack_trails_first_alpha_near_1(void)
{
    pp_trail_data_t data = pp_pack_trails(J2000, 10.0f, 36);
    int off = data.trail_offsets[0];
    int first_base = off * PP_TRAIL_VERTEX_FLOATS;
    float alpha = data.vertices[first_base + 6];
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 1.0f, alpha);
}

void test_pack_trails_last_alpha_near_01(void)
{
    pp_trail_data_t data = pp_pack_trails(J2000, 10.0f, 36);
    int off = data.trail_offsets[0];
    int cnt = data.trail_counts[0];
    int last_base = (off + cnt - 1) * PP_TRAIL_VERTEX_FLOATS;
    float alpha = data.vertices[last_base + 6];
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 0.1f, alpha);
}

void test_pack_trails_different_samples(void)
{
    pp_trail_data_t d1 = pp_pack_trails(J2000, 10.0f, 36);
    pp_trail_data_t d2 = pp_pack_trails(J2000, 10.0f, 72);
    TEST_ASSERT_NOT_EQUAL(d1.total_vertex_count, d2.total_vertex_count);
}

void test_pack_trails_clamped_at_max(void)
{
    /* Requesting more than PP_TRAIL_MAX_SAMPLES should clamp */
    pp_trail_data_t data = pp_pack_trails(J2000, 10.0f, 9999);
    TEST_ASSERT_EQUAL_INT(8 * PP_TRAIL_MAX_SAMPLES, data.total_vertex_count);
}

void test_pack_trails_trail_colors_match_planet(void)
{
    pp_trail_data_t data = pp_pack_trails(J2000, 10.0f, 36);
    for (int i = 0; i < 8; i++) {
        planet_color_t pc = planet_data_color(i);
        int base = data.trail_offsets[i] * PP_TRAIL_VERTEX_FLOATS;
        /* First vertex RGB should match planet color */
        TEST_ASSERT_FLOAT_WITHIN(0.001f, pc.r, data.vertices[base + 3]);
        TEST_ASSERT_FLOAT_WITHIN(0.001f, pc.g, data.vertices[base + 4]);
        TEST_ASSERT_FLOAT_WITHIN(0.001f, pc.b, data.vertices[base + 5]);
    }
}

void test_pack_trails_offsets_sequential(void)
{
    pp_trail_data_t data = pp_pack_trails(J2000, 10.0f, 36);
    /* Offsets should be sequential: offset[i+1] = offset[i] + count[i] */
    for (int i = 0; i < 7; i++) {
        TEST_ASSERT_EQUAL_INT(data.trail_offsets[i] + data.trail_counts[i],
                              data.trail_offsets[i + 1]);
    }
}

void test_pack_trails_min_samples_1(void)
{
    pp_trail_data_t data = pp_pack_trails(J2000, 10.0f, 1);
    TEST_ASSERT_EQUAL_INT(8, data.total_vertex_count);
}

/* ═══════════════════════════════════════════════════════════════════════
 * 3. Byte size functions
 * ═══════════════════════════════════════════════════════════════════════ */

void test_planet_vertex_bytes(void)
{
    pp_planet_data_t data = pp_pack_planets(J2000, 10.0f, 0.01f);
    int expected = 8 * PP_PLANET_VERTEX_FLOATS * (int)sizeof(float);
    TEST_ASSERT_EQUAL_INT(expected, pp_planet_vertex_bytes(&data));
}

void test_trail_vertex_bytes(void)
{
    pp_trail_data_t data = pp_pack_trails(J2000, 10.0f, 36);
    int expected = data.total_vertex_count * PP_TRAIL_VERTEX_FLOATS *
                   (int)sizeof(float);
    TEST_ASSERT_EQUAL_INT(expected, pp_trail_vertex_bytes(&data));
}

void test_trail_vertex_bytes_different_samples(void)
{
    pp_trail_data_t data = pp_pack_trails(J2000, 10.0f, 72);
    int expected = (8 * 72) * PP_TRAIL_VERTEX_FLOATS * (int)sizeof(float);
    TEST_ASSERT_EQUAL_INT(expected, pp_trail_vertex_bytes(&data));
}

/* ═══════════════════════════════════════════════════════════════════════
 * 4. Shader source strings
 * ═══════════════════════════════════════════════════════════════════════ */

void test_planet_vert_source_not_null(void)
{
    TEST_ASSERT_NOT_NULL(pp_planet_vert_source());
}

void test_planet_frag_source_not_null(void)
{
    TEST_ASSERT_NOT_NULL(pp_planet_frag_source());
}

void test_trail_vert_source_not_null(void)
{
    TEST_ASSERT_NOT_NULL(pp_trail_vert_source());
}

void test_trail_frag_source_not_null(void)
{
    TEST_ASSERT_NOT_NULL(pp_trail_frag_source());
}

void test_planet_vert_has_version(void)
{
    TEST_ASSERT_NOT_NULL(strstr(pp_planet_vert_source(), "#version 300 es"));
}

void test_planet_frag_has_version(void)
{
    TEST_ASSERT_NOT_NULL(strstr(pp_planet_frag_source(), "#version 300 es"));
}

void test_trail_vert_has_version(void)
{
    TEST_ASSERT_NOT_NULL(strstr(pp_trail_vert_source(), "#version 300 es"));
}

void test_trail_frag_has_version(void)
{
    TEST_ASSERT_NOT_NULL(strstr(pp_trail_frag_source(), "#version 300 es"));
}

void test_planet_vert_has_point_size(void)
{
    TEST_ASSERT_NOT_NULL(strstr(pp_planet_vert_source(), "gl_PointSize"));
}

void test_trail_vert_has_mvp(void)
{
    TEST_ASSERT_NOT_NULL(strstr(pp_trail_vert_source(), "u_mvp"));
}

void test_planet_frag_has_point_coord(void)
{
    /* Planet frag should use gl_PointCoord for disc rendering */
    TEST_ASSERT_NOT_NULL(strstr(pp_planet_frag_source(), "gl_PointCoord"));
}

void test_planet_vert_has_mvp(void)
{
    TEST_ASSERT_NOT_NULL(strstr(pp_planet_vert_source(), "u_mvp"));
}

/* ═══════════════════════════════════════════════════════════════════════
 * main
 * ═══════════════════════════════════════════════════════════════════════ */

int main(void)
{
    UNITY_BEGIN();

    /* Planet packing (15 tests) */
    RUN_TEST(test_pack_planets_returns_8);
    RUN_TEST(test_pack_planets_positions_nonzero);
    RUN_TEST(test_pack_planets_mercury_closest);
    RUN_TEST(test_pack_planets_neptune_farthest);
    RUN_TEST(test_pack_planets_colors_valid_range);
    RUN_TEST(test_pack_planets_radii_positive);
    RUN_TEST(test_pack_planets_earth_distance_about_1au);
    RUN_TEST(test_pack_planets_different_jd_different_positions);
    RUN_TEST(test_pack_planets_scene_scale_affects_positions);
    RUN_TEST(test_pack_planets_radius_scale_affects_radii);
    RUN_TEST(test_pack_planets_colors_match_planet_data);
    RUN_TEST(test_pack_planets_atmo_thickness_from_atmo_get);
    RUN_TEST(test_pack_planets_mercury_no_atmosphere);
    RUN_TEST(test_pack_planets_earth_has_atmosphere);
    RUN_TEST(test_pack_planets_jupiter_larger_radius_than_earth);

    /* Trail packing (11 tests) */
    RUN_TEST(test_pack_trails_total_count);
    RUN_TEST(test_pack_trails_all_offsets_set);
    RUN_TEST(test_pack_trails_counts_sum_to_total);
    RUN_TEST(test_pack_trails_positions_roughly_circular);
    RUN_TEST(test_pack_trails_alpha_decreases);
    RUN_TEST(test_pack_trails_first_alpha_near_1);
    RUN_TEST(test_pack_trails_last_alpha_near_01);
    RUN_TEST(test_pack_trails_different_samples);
    RUN_TEST(test_pack_trails_clamped_at_max);
    RUN_TEST(test_pack_trails_trail_colors_match_planet);
    RUN_TEST(test_pack_trails_offsets_sequential);
    RUN_TEST(test_pack_trails_min_samples_1);

    /* Byte sizes (3 tests) */
    RUN_TEST(test_planet_vertex_bytes);
    RUN_TEST(test_trail_vertex_bytes);
    RUN_TEST(test_trail_vertex_bytes_different_samples);

    /* Shaders (12 tests) */
    RUN_TEST(test_planet_vert_source_not_null);
    RUN_TEST(test_planet_frag_source_not_null);
    RUN_TEST(test_trail_vert_source_not_null);
    RUN_TEST(test_trail_frag_source_not_null);
    RUN_TEST(test_planet_vert_has_version);
    RUN_TEST(test_planet_frag_has_version);
    RUN_TEST(test_trail_vert_has_version);
    RUN_TEST(test_trail_frag_has_version);
    RUN_TEST(test_planet_vert_has_point_size);
    RUN_TEST(test_trail_vert_has_mvp);
    RUN_TEST(test_planet_frag_has_point_coord);
    RUN_TEST(test_planet_vert_has_mvp);

    return UNITY_END();
}
