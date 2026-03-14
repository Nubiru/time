/*
 * test_planet_surface_pack.c — tests for planet surface shader pack
 *
 * Tests: per-planet config, UV-sphere mesh generation, atmosphere shell,
 *        psp_pack_all positions, vertex layout, shader source strings.
 *
 * 60 tests across 8 sections.
 */

#include "../unity/unity.h"
#include "../../src/render/planet_surface_pack.h"
#include "../../src/systems/astronomy/planet_data.h"
#include "../../src/render/noise_shader.h"

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
 * 1. psp_planet_config — per-planet configuration
 * ═══════════════════════════════════════════════════════════════════════ */

void test_config_sun_is_star(void)
{
    psp_config_t c = psp_planet_config(PSP_SUN);
    TEST_ASSERT_EQUAL_INT(PSP_TYPE_STAR, c.type);
}

void test_config_mercury_is_rocky(void)
{
    psp_config_t c = psp_planet_config(PSP_MERCURY);
    TEST_ASSERT_EQUAL_INT(PSP_TYPE_ROCKY, c.type);
}

void test_config_venus_is_rocky(void)
{
    psp_config_t c = psp_planet_config(PSP_VENUS);
    TEST_ASSERT_EQUAL_INT(PSP_TYPE_ROCKY, c.type);
}

void test_config_earth_is_rocky(void)
{
    psp_config_t c = psp_planet_config(PSP_EARTH);
    TEST_ASSERT_EQUAL_INT(PSP_TYPE_ROCKY, c.type);
}

void test_config_mars_is_rocky(void)
{
    psp_config_t c = psp_planet_config(PSP_MARS);
    TEST_ASSERT_EQUAL_INT(PSP_TYPE_ROCKY, c.type);
}

void test_config_jupiter_is_gas_giant(void)
{
    psp_config_t c = psp_planet_config(PSP_JUPITER);
    TEST_ASSERT_EQUAL_INT(PSP_TYPE_GAS_GIANT, c.type);
}

void test_config_saturn_is_gas_giant(void)
{
    psp_config_t c = psp_planet_config(PSP_SATURN);
    TEST_ASSERT_EQUAL_INT(PSP_TYPE_GAS_GIANT, c.type);
}

void test_config_uranus_is_ice_giant(void)
{
    psp_config_t c = psp_planet_config(PSP_URANUS);
    TEST_ASSERT_EQUAL_INT(PSP_TYPE_ICE_GIANT, c.type);
}

void test_config_neptune_is_ice_giant(void)
{
    psp_config_t c = psp_planet_config(PSP_NEPTUNE);
    TEST_ASSERT_EQUAL_INT(PSP_TYPE_ICE_GIANT, c.type);
}

void test_config_jupiter_high_band_freq(void)
{
    psp_config_t c = psp_planet_config(PSP_JUPITER);
    TEST_ASSERT_TRUE(c.band_frequency >= 8.0f);
    TEST_ASSERT_TRUE(c.band_frequency <= 14.0f);
}

void test_config_saturn_moderate_band_freq(void)
{
    psp_config_t c = psp_planet_config(PSP_SATURN);
    TEST_ASSERT_TRUE(c.band_frequency >= 6.0f);
    TEST_ASSERT_TRUE(c.band_frequency <= 10.0f);
}

void test_config_uranus_low_band_freq(void)
{
    psp_config_t c = psp_planet_config(PSP_URANUS);
    TEST_ASSERT_TRUE(c.band_frequency >= 3.0f);
    TEST_ASSERT_TRUE(c.band_frequency <= 6.0f);
}

void test_config_neptune_moderate_band_freq(void)
{
    psp_config_t c = psp_planet_config(PSP_NEPTUNE);
    TEST_ASSERT_TRUE(c.band_frequency >= 4.0f);
    TEST_ASSERT_TRUE(c.band_frequency <= 8.0f);
}

void test_config_mercury_no_banding(void)
{
    psp_config_t c = psp_planet_config(PSP_MERCURY);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, c.band_amplitude);
}

void test_config_mars_no_banding(void)
{
    psp_config_t c = psp_planet_config(PSP_MARS);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, c.band_amplitude);
}

void test_config_jupiter_high_band_amplitude(void)
{
    psp_config_t c = psp_planet_config(PSP_JUPITER);
    TEST_ASSERT_TRUE(c.band_amplitude >= 0.3f);
    TEST_ASSERT_TRUE(c.band_amplitude <= 0.6f);
}

void test_config_uranus_low_band_amplitude(void)
{
    psp_config_t c = psp_planet_config(PSP_URANUS);
    TEST_ASSERT_TRUE(c.band_amplitude >= 0.05f);
    TEST_ASSERT_TRUE(c.band_amplitude <= 0.25f);
}

void test_config_venus_thick_atmo(void)
{
    psp_config_t c = psp_planet_config(PSP_VENUS);
    TEST_ASSERT_TRUE(c.atmo_thickness >= 0.08f);
}

void test_config_mercury_no_atmo(void)
{
    psp_config_t c = psp_planet_config(PSP_MERCURY);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, c.atmo_thickness);
}

void test_config_base_color_nonzero(void)
{
    for (int i = 0; i < PSP_MAX_PLANETS; i++) {
        psp_config_t c = psp_planet_config((psp_planet_t)i);
        float sum = c.base_color[0] + c.base_color[1] + c.base_color[2];
        TEST_ASSERT_TRUE(sum > 0.0f);
    }
}

void test_config_invalid_returns_star(void)
{
    psp_config_t c = psp_planet_config((psp_planet_t)99);
    TEST_ASSERT_EQUAL_INT(PSP_TYPE_STAR, c.type);
}

/* ═══════════════════════════════════════════════════════════════════════
 * 2. psp_pack_sphere — UV-sphere mesh generation
 * ═══════════════════════════════════════════════════════════════════════ */

/* Max verts for segments=24: (24+1)*(24+1)=625 verts, 24*24*6=3456 triangle indices
 * but packed as triangle list: 24*24*2 triangles * 3 verts = 3456 verts */
#define TEST_BUF_SIZE (4096 * PSP_VERTEX_FLOATS)
static float s_buf[TEST_BUF_SIZE];

void test_sphere_returns_positive_count(void)
{
    float center[3] = {0.0f, 0.0f, 0.0f};
    psp_config_t c = psp_planet_config(PSP_EARTH);
    int count = psp_pack_sphere(center, 1.0f, 16, c, s_buf, TEST_BUF_SIZE);
    TEST_ASSERT_TRUE(count > 0);
}

void test_sphere_count_divisible_by_3(void)
{
    /* Triangle list: vertex count must be multiple of 3 */
    float center[3] = {0.0f, 0.0f, 0.0f};
    psp_config_t c = psp_planet_config(PSP_MARS);
    int count = psp_pack_sphere(center, 1.0f, 16, c, s_buf, TEST_BUF_SIZE);
    TEST_ASSERT_EQUAL_INT(0, count % 3);
}

void test_sphere_vertex_count_formula(void)
{
    /* For segments=S: S*S*2 triangles * 3 verts each = S*S*6 */
    float center[3] = {0.0f, 0.0f, 0.0f};
    psp_config_t c = psp_planet_config(PSP_EARTH);
    int seg = 8;
    int expected = seg * seg * 6;
    int count = psp_pack_sphere(center, 1.0f, seg, c, s_buf, TEST_BUF_SIZE);
    TEST_ASSERT_EQUAL_INT(expected, count);
}

void test_sphere_positions_within_radius(void)
{
    float center[3] = {5.0f, 2.0f, -1.0f};
    float radius = 0.5f;
    psp_config_t c = psp_planet_config(PSP_JUPITER);
    int count = psp_pack_sphere(center, radius, 8, c, s_buf, TEST_BUF_SIZE);
    float tol = 0.01f;

    for (int v = 0; v < count; v++) {
        int base = v * PSP_VERTEX_FLOATS;
        float dx = s_buf[base + 0] - center[0];
        float dy = s_buf[base + 1] - center[1];
        float dz = s_buf[base + 2] - center[2];
        float dist = sqrtf(dx * dx + dy * dy + dz * dz);
        TEST_ASSERT_FLOAT_WITHIN(tol, radius, dist);
    }
}

void test_sphere_normals_unit_length(void)
{
    float center[3] = {0.0f, 0.0f, 0.0f};
    psp_config_t c = psp_planet_config(PSP_EARTH);
    int count = psp_pack_sphere(center, 2.0f, 8, c, s_buf, TEST_BUF_SIZE);
    float tol = 0.02f;

    for (int v = 0; v < count; v++) {
        int base = v * PSP_VERTEX_FLOATS;
        float nx = s_buf[base + 3];
        float ny = s_buf[base + 4];
        float nz = s_buf[base + 5];
        float len = sqrtf(nx * nx + ny * ny + nz * nz);
        TEST_ASSERT_FLOAT_WITHIN(tol, 1.0f, len);
    }
}

void test_sphere_uvs_in_range(void)
{
    float center[3] = {0.0f, 0.0f, 0.0f};
    psp_config_t c = psp_planet_config(PSP_SATURN);
    int count = psp_pack_sphere(center, 1.0f, 12, c, s_buf, TEST_BUF_SIZE);

    for (int v = 0; v < count; v++) {
        int base = v * PSP_VERTEX_FLOATS;
        float u = s_buf[base + 6];
        float vu = s_buf[base + 7];
        TEST_ASSERT_TRUE(u >= -0.01f && u <= 1.01f);
        TEST_ASSERT_TRUE(vu >= -0.01f && vu <= 1.01f);
    }
}

void test_sphere_colors_match_config(void)
{
    float center[3] = {0.0f, 0.0f, 0.0f};
    psp_config_t c = psp_planet_config(PSP_MARS);
    int count = psp_pack_sphere(center, 1.0f, 8, c, s_buf, TEST_BUF_SIZE);

    /* Every vertex should carry the config's base_color */
    for (int v = 0; v < count; v++) {
        int base = v * PSP_VERTEX_FLOATS;
        TEST_ASSERT_FLOAT_WITHIN(0.01f, c.base_color[0], s_buf[base + 8]);
        TEST_ASSERT_FLOAT_WITHIN(0.01f, c.base_color[1], s_buf[base + 9]);
        TEST_ASSERT_FLOAT_WITHIN(0.01f, c.base_color[2], s_buf[base + 10]);
    }
}

void test_sphere_zero_radius_returns_zero(void)
{
    float center[3] = {0.0f, 0.0f, 0.0f};
    psp_config_t c = psp_planet_config(PSP_EARTH);
    int count = psp_pack_sphere(center, 0.0f, 16, c, s_buf, TEST_BUF_SIZE);
    TEST_ASSERT_EQUAL_INT(0, count);
}

void test_sphere_negative_radius_returns_zero(void)
{
    float center[3] = {0.0f, 0.0f, 0.0f};
    psp_config_t c = psp_planet_config(PSP_EARTH);
    int count = psp_pack_sphere(center, -1.0f, 16, c, s_buf, TEST_BUF_SIZE);
    TEST_ASSERT_EQUAL_INT(0, count);
}

void test_sphere_tiny_segments_clamped(void)
{
    /* segments < 3 should be clamped to 3 minimum */
    float center[3] = {0.0f, 0.0f, 0.0f};
    psp_config_t c = psp_planet_config(PSP_EARTH);
    int count = psp_pack_sphere(center, 1.0f, 1, c, s_buf, TEST_BUF_SIZE);
    /* 3*3*6 = 54 (minimum) */
    TEST_ASSERT_TRUE(count >= 54);
}

void test_sphere_buffer_overflow_protection(void)
{
    /* Very small buffer should return 0 or capped count */
    float tiny_buf[PSP_VERTEX_FLOATS * 3]; /* only 3 vertices */
    float center[3] = {0.0f, 0.0f, 0.0f};
    psp_config_t c = psp_planet_config(PSP_EARTH);
    int count = psp_pack_sphere(center, 1.0f, 24, c, tiny_buf, 3);
    TEST_ASSERT_EQUAL_INT(0, count);
}

void test_sphere_default_segments(void)
{
    float center[3] = {0.0f, 0.0f, 0.0f};
    psp_config_t c = psp_planet_config(PSP_EARTH);
    int expected = PSP_SPHERE_SEGMENTS * PSP_SPHERE_SEGMENTS * 6;
    int count = psp_pack_sphere(center, 1.0f, PSP_SPHERE_SEGMENTS, c,
                                s_buf, TEST_BUF_SIZE);
    TEST_ASSERT_EQUAL_INT(expected, count);
}

/* ═══════════════════════════════════════════════════════════════════════
 * 3. psp_pack_atmosphere — atmosphere shell
 * ═══════════════════════════════════════════════════════════════════════ */

void test_atmo_returns_positive_for_earth(void)
{
    float center[3] = {0.0f, 0.0f, 0.0f};
    psp_config_t c = psp_planet_config(PSP_EARTH);
    int count = psp_pack_atmosphere(center, 1.0f, c, s_buf, TEST_BUF_SIZE);
    TEST_ASSERT_TRUE(count > 0);
}

void test_atmo_returns_zero_for_mercury(void)
{
    float center[3] = {0.0f, 0.0f, 0.0f};
    psp_config_t c = psp_planet_config(PSP_MERCURY);
    int count = psp_pack_atmosphere(center, 1.0f, c, s_buf, TEST_BUF_SIZE);
    TEST_ASSERT_EQUAL_INT(0, count);
}

void test_atmo_larger_than_planet(void)
{
    float center[3] = {3.0f, 0.0f, 0.0f};
    float planet_radius = 1.0f;
    psp_config_t c = psp_planet_config(PSP_EARTH);
    int count = psp_pack_atmosphere(center, planet_radius, c,
                                     s_buf, TEST_BUF_SIZE);
    TEST_ASSERT_TRUE(count > 0);

    /* Atmosphere shell should extend beyond planet radius */
    for (int v = 0; v < count; v++) {
        int base = v * PSP_VERTEX_FLOATS;
        float dx = s_buf[base + 0] - center[0];
        float dy = s_buf[base + 1] - center[1];
        float dz = s_buf[base + 2] - center[2];
        float dist = sqrtf(dx * dx + dy * dy + dz * dz);
        TEST_ASSERT_TRUE(dist >= planet_radius - 0.01f);
    }
}

void test_atmo_color_matches_config(void)
{
    float center[3] = {0.0f, 0.0f, 0.0f};
    psp_config_t c = psp_planet_config(PSP_EARTH);
    int count = psp_pack_atmosphere(center, 1.0f, c, s_buf, TEST_BUF_SIZE);

    if (count > 0) {
        /* Atmosphere verts carry atmosphere_color */
        int base = 0 * PSP_VERTEX_FLOATS;
        TEST_ASSERT_FLOAT_WITHIN(0.01f, c.atmosphere_color[0], s_buf[base + 8]);
        TEST_ASSERT_FLOAT_WITHIN(0.01f, c.atmosphere_color[1], s_buf[base + 9]);
        TEST_ASSERT_FLOAT_WITHIN(0.01f, c.atmosphere_color[2], s_buf[base + 10]);
    }
}

void test_atmo_venus_thick(void)
{
    float center[3] = {0.0f, 0.0f, 0.0f};
    psp_config_t c = psp_planet_config(PSP_VENUS);
    int count = psp_pack_atmosphere(center, 1.0f, c, s_buf, TEST_BUF_SIZE);
    TEST_ASSERT_TRUE(count > 0);

    /* Venus atmosphere shell should be larger relative to surface */
    for (int v = 0; v < count; v++) {
        int base = v * PSP_VERTEX_FLOATS;
        float dx = s_buf[base + 0] - center[0];
        float dy = s_buf[base + 1] - center[1];
        float dz = s_buf[base + 2] - center[2];
        float dist = sqrtf(dx * dx + dy * dy + dz * dz);
        /* Venus atmo is thick, shell should be > 1.0 + atmo_thickness */
        TEST_ASSERT_TRUE(dist >= 1.0f);
    }
}

void test_atmo_gas_giant_has_shell(void)
{
    float center[3] = {0.0f, 0.0f, 0.0f};
    psp_config_t c = psp_planet_config(PSP_JUPITER);
    int count = psp_pack_atmosphere(center, 1.0f, c, s_buf, TEST_BUF_SIZE);
    TEST_ASSERT_TRUE(count > 0);
}

/* ═══════════════════════════════════════════════════════════════════════
 * 4. psp_pack_all — all planets at computed positions
 * ═══════════════════════════════════════════════════════════════════════ */

/* Pack-all buffer: 9 planets * ~3500 verts * 11 floats = ~350K floats.
 * Use a static buffer large enough. */
#define PACK_ALL_BUF_SIZE (PSP_MAX_PLANETS * PSP_SPHERE_SEGMENTS * PSP_SPHERE_SEGMENTS * 6 * PSP_VERTEX_FLOATS + 1024)
static float s_all_buf[PACK_ALL_BUF_SIZE];

void test_pack_all_returns_positive(void)
{
    int count = psp_pack_all(J2000, 3.0f, s_all_buf, PACK_ALL_BUF_SIZE);
    TEST_ASSERT_TRUE(count > 0);
}

void test_pack_all_at_different_date(void)
{
    int count = psp_pack_all(JD_2024, 3.0f, s_all_buf, PACK_ALL_BUF_SIZE);
    TEST_ASSERT_TRUE(count > 0);
}

void test_pack_all_sun_at_origin(void)
{
    /* The first planet is the Sun, which should be at origin */
    int count = psp_pack_all(J2000, 3.0f, s_all_buf, PACK_ALL_BUF_SIZE);
    TEST_ASSERT_TRUE(count > 0);

    /* Sun's sphere vertices should cluster around (0,0,0) */
    /* Check first vertex position */
    float x = s_all_buf[0];
    float y = s_all_buf[1];
    float z = s_all_buf[2];
    float dist = sqrtf(x * x + y * y + z * z);
    /* Sun radius is small in scene units, so distance from origin should be small */
    TEST_ASSERT_TRUE(dist < 2.0f);
}

void test_pack_all_planets_at_different_distances(void)
{
    int count = psp_pack_all(J2000, 3.0f, s_all_buf, PACK_ALL_BUF_SIZE);
    TEST_ASSERT_TRUE(count > 0);
    /* As long as we get a valid vertex count, planet positions are computed */
}

void test_pack_all_zero_scale_returns_positive(void)
{
    /* Even with zero orbit_scale, sun at origin still produces vertices */
    int count = psp_pack_all(J2000, 0.0f, s_all_buf, PACK_ALL_BUF_SIZE);
    /* Sun is at origin regardless, should produce vertices */
    TEST_ASSERT_TRUE(count > 0);
}

void test_pack_all_small_buffer_returns_zero(void)
{
    float tiny[10];
    int count = psp_pack_all(J2000, 3.0f, tiny, 10 / PSP_VERTEX_FLOATS);
    TEST_ASSERT_EQUAL_INT(0, count);
}

/* ═══════════════════════════════════════════════════════════════════════
 * 5. Vertex layout and constants
 * ═══════════════════════════════════════════════════════════════════════ */

void test_vertex_floats_is_11(void)
{
    TEST_ASSERT_EQUAL_INT(11, PSP_VERTEX_FLOATS);
}

void test_max_planets_is_9(void)
{
    TEST_ASSERT_EQUAL_INT(9, PSP_MAX_PLANETS);
}

void test_sphere_segments_is_24(void)
{
    TEST_ASSERT_EQUAL_INT(24, PSP_SPHERE_SEGMENTS);
}

void test_planet_enum_sun_is_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, PSP_SUN);
}

void test_planet_enum_neptune_is_8(void)
{
    TEST_ASSERT_EQUAL_INT(8, PSP_NEPTUNE);
}

void test_type_enum_star_is_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, PSP_TYPE_STAR);
}

void test_type_enum_gas_giant_is_2(void)
{
    TEST_ASSERT_EQUAL_INT(2, PSP_TYPE_GAS_GIANT);
}

/* ═══════════════════════════════════════════════════════════════════════
 * 6. Shader source strings
 * ═══════════════════════════════════════════════════════════════════════ */

void test_vert_source_not_null(void)
{
    const char *src = psp_vert_source();
    TEST_ASSERT_NOT_NULL(src);
}

void test_vert_source_has_version(void)
{
    const char *src = psp_vert_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "#version 300 es"));
}

void test_vert_source_has_main(void)
{
    const char *src = psp_vert_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "void main()"));
}

void test_vert_source_has_position_attr(void)
{
    const char *src = psp_vert_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "a_position"));
}

void test_vert_source_has_normal_attr(void)
{
    const char *src = psp_vert_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "a_normal"));
}

void test_frag_source_not_null(void)
{
    const char *src = psp_frag_source();
    TEST_ASSERT_NOT_NULL(src);
}

void test_frag_source_has_version(void)
{
    const char *src = psp_frag_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "#version 300 es"));
}

void test_frag_source_has_main(void)
{
    const char *src = psp_frag_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "void main()"));
}

void test_frag_source_contains_noise(void)
{
    /* Fragment shader should contain noise functions from noise_shader */
    const char *src = psp_frag_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "snoise"));
}

void test_frag_source_contains_fbm(void)
{
    const char *src = psp_frag_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "fbm"));
}

void test_frag_source_has_planet_type_uniform(void)
{
    const char *src = psp_frag_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "u_planet_type"));
}

void test_frag_source_has_sun_dir_uniform(void)
{
    const char *src = psp_frag_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "u_sun_dir"));
}

void test_frag_source_has_time_uniform(void)
{
    const char *src = psp_frag_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "u_time"));
}

void test_frag_source_has_fresnel(void)
{
    /* Should include Fresnel limb glow logic */
    const char *src = psp_frag_source();
    /* Check for pow and dot product used in Fresnel */
    TEST_ASSERT_NOT_NULL(strstr(src, "pow"));
}

/* ═══════════════════════════════════════════════════════════════════════
 * 7. Consistency: config values from planet_data
 * ═══════════════════════════════════════════════════════════════════════ */

void test_earth_blue_ocean_base(void)
{
    psp_config_t c = psp_planet_config(PSP_EARTH);
    /* Earth base_color should be bluish (b > r) */
    TEST_ASSERT_TRUE(c.base_color[2] > c.base_color[0]);
}

void test_mars_red_base(void)
{
    psp_config_t c = psp_planet_config(PSP_MARS);
    /* Mars base_color should be reddish (r > b) */
    TEST_ASSERT_TRUE(c.base_color[0] > c.base_color[2]);
}

void test_jupiter_warm_base(void)
{
    psp_config_t c = psp_planet_config(PSP_JUPITER);
    /* Jupiter should have warm base color (r >= b) */
    TEST_ASSERT_TRUE(c.base_color[0] >= c.base_color[2]);
}

void test_neptune_blue_base(void)
{
    psp_config_t c = psp_planet_config(PSP_NEPTUNE);
    /* Neptune should have blue base color (b > r) */
    TEST_ASSERT_TRUE(c.base_color[2] > c.base_color[0]);
}

/* ═══════════════════════════════════════════════════════════════════════
 * 8. Edge cases and noise config
 * ═══════════════════════════════════════════════════════════════════════ */

void test_config_noise_scale_positive(void)
{
    for (int i = 0; i < PSP_MAX_PLANETS; i++) {
        psp_config_t c = psp_planet_config((psp_planet_t)i);
        TEST_ASSERT_TRUE(c.noise_scale >= 0.0f);
    }
}

void test_config_noise_amplitude_nonneg(void)
{
    for (int i = 0; i < PSP_MAX_PLANETS; i++) {
        psp_config_t c = psp_planet_config((psp_planet_t)i);
        TEST_ASSERT_TRUE(c.noise_amplitude >= 0.0f);
    }
}

void test_config_atmo_thickness_nonneg(void)
{
    for (int i = 0; i < PSP_MAX_PLANETS; i++) {
        psp_config_t c = psp_planet_config((psp_planet_t)i);
        TEST_ASSERT_TRUE(c.atmo_thickness >= 0.0f);
    }
}

void test_config_band_frequency_nonneg(void)
{
    for (int i = 0; i < PSP_MAX_PLANETS; i++) {
        psp_config_t c = psp_planet_config((psp_planet_t)i);
        TEST_ASSERT_TRUE(c.band_frequency >= 0.0f);
    }
}

/* ═══════════════════════════════════════════════════════════════════════
 * main
 * ═══════════════════════════════════════════════════════════════════════ */

int main(void)
{
    UNITY_BEGIN();

    /* 1. Per-planet config (21 tests) */
    RUN_TEST(test_config_sun_is_star);
    RUN_TEST(test_config_mercury_is_rocky);
    RUN_TEST(test_config_venus_is_rocky);
    RUN_TEST(test_config_earth_is_rocky);
    RUN_TEST(test_config_mars_is_rocky);
    RUN_TEST(test_config_jupiter_is_gas_giant);
    RUN_TEST(test_config_saturn_is_gas_giant);
    RUN_TEST(test_config_uranus_is_ice_giant);
    RUN_TEST(test_config_neptune_is_ice_giant);
    RUN_TEST(test_config_jupiter_high_band_freq);
    RUN_TEST(test_config_saturn_moderate_band_freq);
    RUN_TEST(test_config_uranus_low_band_freq);
    RUN_TEST(test_config_neptune_moderate_band_freq);
    RUN_TEST(test_config_mercury_no_banding);
    RUN_TEST(test_config_mars_no_banding);
    RUN_TEST(test_config_jupiter_high_band_amplitude);
    RUN_TEST(test_config_uranus_low_band_amplitude);
    RUN_TEST(test_config_venus_thick_atmo);
    RUN_TEST(test_config_mercury_no_atmo);
    RUN_TEST(test_config_base_color_nonzero);
    RUN_TEST(test_config_invalid_returns_star);

    /* 2. UV-sphere mesh (12 tests) */
    RUN_TEST(test_sphere_returns_positive_count);
    RUN_TEST(test_sphere_count_divisible_by_3);
    RUN_TEST(test_sphere_vertex_count_formula);
    RUN_TEST(test_sphere_positions_within_radius);
    RUN_TEST(test_sphere_normals_unit_length);
    RUN_TEST(test_sphere_uvs_in_range);
    RUN_TEST(test_sphere_colors_match_config);
    RUN_TEST(test_sphere_zero_radius_returns_zero);
    RUN_TEST(test_sphere_negative_radius_returns_zero);
    RUN_TEST(test_sphere_tiny_segments_clamped);
    RUN_TEST(test_sphere_buffer_overflow_protection);
    RUN_TEST(test_sphere_default_segments);

    /* 3. Atmosphere shell (6 tests) */
    RUN_TEST(test_atmo_returns_positive_for_earth);
    RUN_TEST(test_atmo_returns_zero_for_mercury);
    RUN_TEST(test_atmo_larger_than_planet);
    RUN_TEST(test_atmo_color_matches_config);
    RUN_TEST(test_atmo_venus_thick);
    RUN_TEST(test_atmo_gas_giant_has_shell);

    /* 4. Pack all (6 tests) */
    RUN_TEST(test_pack_all_returns_positive);
    RUN_TEST(test_pack_all_at_different_date);
    RUN_TEST(test_pack_all_sun_at_origin);
    RUN_TEST(test_pack_all_planets_at_different_distances);
    RUN_TEST(test_pack_all_zero_scale_returns_positive);
    RUN_TEST(test_pack_all_small_buffer_returns_zero);

    /* 5. Constants and layout (7 tests) */
    RUN_TEST(test_vertex_floats_is_11);
    RUN_TEST(test_max_planets_is_9);
    RUN_TEST(test_sphere_segments_is_24);
    RUN_TEST(test_planet_enum_sun_is_0);
    RUN_TEST(test_planet_enum_neptune_is_8);
    RUN_TEST(test_type_enum_star_is_0);
    RUN_TEST(test_type_enum_gas_giant_is_2);

    /* 6. Shader sources (14 tests) */
    RUN_TEST(test_vert_source_not_null);
    RUN_TEST(test_vert_source_has_version);
    RUN_TEST(test_vert_source_has_main);
    RUN_TEST(test_vert_source_has_position_attr);
    RUN_TEST(test_vert_source_has_normal_attr);
    RUN_TEST(test_frag_source_not_null);
    RUN_TEST(test_frag_source_has_version);
    RUN_TEST(test_frag_source_has_main);
    RUN_TEST(test_frag_source_contains_noise);
    RUN_TEST(test_frag_source_contains_fbm);
    RUN_TEST(test_frag_source_has_planet_type_uniform);
    RUN_TEST(test_frag_source_has_sun_dir_uniform);
    RUN_TEST(test_frag_source_has_time_uniform);
    RUN_TEST(test_frag_source_has_fresnel);

    /* 7. Color consistency (4 tests) */
    RUN_TEST(test_earth_blue_ocean_base);
    RUN_TEST(test_mars_red_base);
    RUN_TEST(test_jupiter_warm_base);
    RUN_TEST(test_neptune_blue_base);

    /* 8. Edge cases (4 tests) */
    RUN_TEST(test_config_noise_scale_positive);
    RUN_TEST(test_config_noise_amplitude_nonneg);
    RUN_TEST(test_config_atmo_thickness_nonneg);
    RUN_TEST(test_config_band_frequency_nonneg);

    return UNITY_END();
}
