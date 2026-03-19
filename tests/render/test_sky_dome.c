#include "../unity/unity.h"
#include "../../src/render/sky_dome.h"

#include <math.h>

#define TOL 0.01f

void setUp(void) {}
void tearDown(void) {}

/* =========================================================
 *  Configuration (2 tests)
 * ========================================================= */

/* 1. Default config has sensible values */
void test_sky_dome_default_config(void)
{
    sky_dome_config_t cfg = sky_dome_default_config();
    TEST_ASSERT_TRUE(cfg.dome_radius > 0.0f);
    TEST_ASSERT_TRUE(cfg.sun_altitude_deg >= -90.0 && cfg.sun_altitude_deg <= 90.0);
    TEST_ASSERT_TRUE(cfg.sun_azimuth_deg >= 0.0 && cfg.sun_azimuth_deg <= 360.0);
}

/* =========================================================
 *  Counts (2 tests)
 * ========================================================= */

/* 2. Vertex count matches resolution constants */
void test_sky_dome_vertex_count(void)
{
    int expected = (SKY_DOME_ALT_STEPS + 1) * (SKY_DOME_AZ_STEPS + 1);
    TEST_ASSERT_EQUAL_INT(expected, sky_dome_vertex_count());
}

/* 3. Index count matches resolution constants */
void test_sky_dome_index_count(void)
{
    int expected = SKY_DOME_ALT_STEPS * SKY_DOME_AZ_STEPS * 6;
    TEST_ASSERT_EQUAL_INT(expected, sky_dome_index_count());
}

/* =========================================================
 *  Mesh generation (4 tests)
 * ========================================================= */

/* 4. Generate returns correct counts */
void test_sky_dome_generate_counts(void)
{
    sky_dome_config_t cfg = sky_dome_default_config();
    float verts[SKY_DOME_MAX_VERTICES * SKY_DOME_VERT_FLOATS];
    unsigned int idxs[SKY_DOME_MAX_INDICES];

    sky_dome_info_t info = sky_dome_generate(cfg, verts, idxs);

    TEST_ASSERT_EQUAL_INT(sky_dome_vertex_count(), info.vertex_count);
    TEST_ASSERT_EQUAL_INT(sky_dome_index_count(), info.index_count);
}

/* 5. All vertex positions have length ~dome_radius */
void test_sky_dome_generate_vertices_valid(void)
{
    sky_dome_config_t cfg = sky_dome_default_config();
    float verts[SKY_DOME_MAX_VERTICES * SKY_DOME_VERT_FLOATS];
    unsigned int idxs[SKY_DOME_MAX_INDICES];

    sky_dome_info_t info = sky_dome_generate(cfg, verts, idxs);

    /* Check first 20 vertices (skip horizon ring vertex 0 which is at alt=0) */
    for (int i = 1; i < 20 && i < info.vertex_count; i++) {
        float x = verts[i * SKY_DOME_VERT_FLOATS + 0];
        float y = verts[i * SKY_DOME_VERT_FLOATS + 1];
        float z = verts[i * SKY_DOME_VERT_FLOATS + 2];
        float r = sqrtf(x * x + y * y + z * z);
        TEST_ASSERT_FLOAT_WITHIN(0.1f, cfg.dome_radius, r);
    }
}

/* 6. All indices are in bounds */
void test_sky_dome_generate_indices_valid(void)
{
    sky_dome_config_t cfg = sky_dome_default_config();
    float verts[SKY_DOME_MAX_VERTICES * SKY_DOME_VERT_FLOATS];
    unsigned int idxs[SKY_DOME_MAX_INDICES];

    sky_dome_info_t info = sky_dome_generate(cfg, verts, idxs);

    for (int i = 0; i < info.index_count; i++) {
        TEST_ASSERT_TRUE(idxs[i] < (unsigned int)info.vertex_count);
    }
}

/* 7. Hemisphere coverage: vertices span from altitude ~0 to ~90 */
void test_sky_dome_hemisphere_coverage(void)
{
    sky_dome_config_t cfg = sky_dome_default_config();
    float verts[SKY_DOME_MAX_VERTICES * SKY_DOME_VERT_FLOATS];
    unsigned int idxs[SKY_DOME_MAX_INDICES];

    sky_dome_info_t info = sky_dome_generate(cfg, verts, idxs);

    /* Find min and max Y values (Y-up: 0=horizon, radius=zenith) */
    float min_y = 1e9f;
    float max_y = -1e9f;
    for (int i = 0; i < info.vertex_count; i++) {
        float y = verts[i * SKY_DOME_VERT_FLOATS + 1];
        if (y < min_y) min_y = y;
        if (y > max_y) max_y = y;
    }

    /* Minimum Y should be near 0 (horizon) */
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 0.0f, min_y);

    /* Maximum Y should be near dome_radius (zenith) */
    TEST_ASSERT_FLOAT_WITHIN(0.1f, cfg.dome_radius, max_y);
}

/* =========================================================
 *  Sky color at direction (5 tests)
 * ========================================================= */

/* 8. Zenith color during day: bluish (b > r, b > g) */
void test_sky_dome_color_zenith_day(void)
{
    float rgb[3];
    /* Sun high (altitude 60), looking at zenith (alt 90), far from sun (30 deg away) */
    sky_dome_color_at(90.0, 30.0, 60.0, rgb);

    TEST_ASSERT_TRUE(rgb[2] > rgb[0]); /* blue > red */
    TEST_ASSERT_TRUE(rgb[2] > rgb[1]); /* blue > green */
    TEST_ASSERT_TRUE(rgb[2] > 0.3f);   /* visibly blue */
}

/* 9. Horizon at sunset: warm (r > b) */
void test_sky_dome_color_horizon_sunset(void)
{
    float rgb[3];
    /* Sun at 2 degrees, looking at horizon near sun */
    sky_dome_color_at(5.0, 10.0, 2.0, rgb);

    TEST_ASSERT_TRUE(rgb[0] > rgb[2]); /* red > blue */
    TEST_ASSERT_TRUE(rgb[0] > 0.2f);   /* visibly warm */
}

/* 10. Night: dark (all channels < 0.1) */
void test_sky_dome_color_night(void)
{
    float rgb[3];
    /* Sun well below horizon (-30), looking anywhere */
    sky_dome_color_at(45.0, 120.0, -30.0, rgb);

    TEST_ASSERT_TRUE(rgb[0] < 0.1f);
    TEST_ASSERT_TRUE(rgb[1] < 0.1f);
    TEST_ASSERT_TRUE(rgb[2] < 0.1f);
}

/* 11. Color near sun is brighter than away from sun */
void test_sky_dome_color_near_sun(void)
{
    float near_rgb[3], far_rgb[3];

    /* Near sun: 5 degrees away */
    sky_dome_color_at(50.0, 5.0, 45.0, near_rgb);
    /* Far from sun: 120 degrees away */
    sky_dome_color_at(50.0, 120.0, 45.0, far_rgb);

    float near_lum = near_rgb[0] + near_rgb[1] + near_rgb[2];
    float far_lum  = far_rgb[0]  + far_rgb[1]  + far_rgb[2];

    TEST_ASSERT_TRUE(near_lum > far_lum);
}

/* 12. All color channels in [0,1] across various conditions */
void test_sky_dome_color_range(void)
{
    float rgb[3];
    double sun_alts[] = { -30.0, -10.0, 0.0, 5.0, 30.0, 60.0, 90.0 };
    int n = (int)(sizeof(sun_alts) / sizeof(sun_alts[0]));

    for (int i = 0; i < n; i++) {
        sky_dome_color_at(45.0, 45.0, sun_alts[i], rgb);
        TEST_ASSERT_TRUE(rgb[0] >= 0.0f && rgb[0] <= 1.0f);
        TEST_ASSERT_TRUE(rgb[1] >= 0.0f && rgb[1] <= 1.0f);
        TEST_ASSERT_TRUE(rgb[2] >= 0.0f && rgb[2] <= 1.0f);
    }
}

/* =========================================================
 *  Shader sources (2 tests)
 * ========================================================= */

/* 13. Vertex shader source is not NULL */
void test_sky_dome_vert_source_not_null(void)
{
    const char *src = sky_dome_vert_source();
    TEST_ASSERT_NOT_NULL(src);
    TEST_ASSERT_TRUE(src[0] != '\0');
}

/* 14. Fragment shader source is not NULL */
void test_sky_dome_frag_source_not_null(void)
{
    const char *src = sky_dome_frag_source();
    TEST_ASSERT_NOT_NULL(src);
    TEST_ASSERT_TRUE(src[0] != '\0');
}

/* ---- main ---- */

int main(void)
{
    UNITY_BEGIN();

    /* Configuration */
    RUN_TEST(test_sky_dome_default_config);

    /* Counts */
    RUN_TEST(test_sky_dome_vertex_count);
    RUN_TEST(test_sky_dome_index_count);

    /* Mesh generation */
    RUN_TEST(test_sky_dome_generate_counts);
    RUN_TEST(test_sky_dome_generate_vertices_valid);
    RUN_TEST(test_sky_dome_generate_indices_valid);
    RUN_TEST(test_sky_dome_hemisphere_coverage);

    /* Sky color */
    RUN_TEST(test_sky_dome_color_zenith_day);
    RUN_TEST(test_sky_dome_color_horizon_sunset);
    RUN_TEST(test_sky_dome_color_night);
    RUN_TEST(test_sky_dome_color_near_sun);
    RUN_TEST(test_sky_dome_color_range);

    /* Shaders */
    RUN_TEST(test_sky_dome_vert_source_not_null);
    RUN_TEST(test_sky_dome_frag_source_not_null);

    return UNITY_END();
}
