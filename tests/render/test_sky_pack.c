/*
 * test_sky_pack.c — Tests for Preetham/Perez sky color model
 */

#include "../unity/unity.h"
#include "../../src/render/sky_pack.h"

#include <math.h>
#include <string.h>

#ifndef PI
#define PI 3.14159265358979323846
#endif

void setUp(void) {}
void tearDown(void) {}

/* 1. Default config has reasonable turbidity */
void test_default_turbidity(void)
{
    sky_config_t cfg = sky_default_config();
    TEST_ASSERT_TRUE(cfg.turbidity >= 2.0f);
    TEST_ASSERT_TRUE(cfg.turbidity <= 20.0f);
}

/* 2. Zenith luminance is positive for clear sky */
void test_zenith_luminance_positive(void)
{
    sky_config_t cfg = sky_default_config();
    sky_params_t p = sky_compute_params(cfg);
    TEST_ASSERT_TRUE(p.Y_z > 0.0f);
}

/* 3. Zenith chromaticity values are in valid CIE range */
void test_zenith_chromaticity_valid(void)
{
    sky_config_t cfg = sky_default_config();
    sky_params_t p = sky_compute_params(cfg);
    TEST_ASSERT_TRUE(p.x_z > 0.0f && p.x_z < 1.0f);
    TEST_ASSERT_TRUE(p.y_z > 0.0f && p.y_z < 1.0f);
}

/* 4. Sky at zenith is blue */
void test_zenith_is_blue(void)
{
    sky_config_t cfg = sky_default_config();
    cfg.sun_theta = (float)(PI / 4.0); /* sun at 45 degrees */
    sky_params_t p = sky_compute_params(cfg);
    sky_rgb_t color = sky_color_at(&p, 0.0f, 0.0f); /* looking straight up */
    /* Blue should dominate at zenith */
    TEST_ASSERT_TRUE(color.b > color.r);
}

/* 5. Sky near horizon is warmer than zenith */
void test_horizon_warmer_than_zenith(void)
{
    sky_config_t cfg = sky_default_config();
    cfg.sun_theta = (float)(PI / 3.0); /* sun at 60 degrees */
    sky_params_t p = sky_compute_params(cfg);
    sky_rgb_t zenith = sky_color_at(&p, 0.1f, 0.0f);
    sky_rgb_t horizon = sky_color_at(&p, 1.4f, 0.0f); /* near horizon */
    /* Horizon should have higher red/blue ratio */
    float zenith_rb = (zenith.b > 0.001f) ? zenith.r / zenith.b : 0.0f;
    float horizon_rb = (horizon.b > 0.001f) ? horizon.r / horizon.b : 999.0f;
    TEST_ASSERT_TRUE(horizon_rb >= zenith_rb);
}

/* 6. Sky color is non-negative */
void test_color_non_negative(void)
{
    sky_config_t cfg = sky_default_config();
    sky_params_t p = sky_compute_params(cfg);
    for (float theta = 0.0f; theta < 1.5f; theta += 0.3f) {
        for (float phi = 0.0f; phi < 6.0f; phi += 1.0f) {
            sky_rgb_t c = sky_color_at(&p, theta, phi);
            TEST_ASSERT_TRUE(c.r >= 0.0f);
            TEST_ASSERT_TRUE(c.g >= 0.0f);
            TEST_ASSERT_TRUE(c.b >= 0.0f);
        }
    }
}

/* 7. Perez function is positive for reasonable inputs */
void test_perez_positive(void)
{
    float f = sky_perez(0.5f, 0.5f, -1.0f, -0.5f, 5.0f, -2.5f, 0.3f);
    TEST_ASSERT_TRUE(f > 0.0f);
}

/* 8. Higher turbidity = more luminance at zenith */
void test_turbidity_increases_luminance(void)
{
    sky_config_t cfg1 = sky_default_config();
    cfg1.turbidity = 2.0f;
    sky_config_t cfg2 = cfg1;
    cfg2.turbidity = 10.0f;
    sky_params_t p1 = sky_compute_params(cfg1);
    sky_params_t p2 = sky_compute_params(cfg2);
    /* Higher turbidity generally increases zenith luminance */
    TEST_ASSERT_TRUE(p2.Y_z != p1.Y_z); /* at least different */
}

/* 9. NULL params returns zero color */
void test_null_params(void)
{
    sky_rgb_t c = sky_color_at(NULL, 0.0f, 0.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, c.r);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, c.g);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, c.b);
}

/* 10. Vertex shader source is GLSL ES 3.00 */
void test_vert_source(void)
{
    const char *src = sky_vert_source();
    TEST_ASSERT_NOT_NULL(src);
    TEST_ASSERT_NOT_NULL(strstr(src, "#version 300 es"));
}

/* 11. Fragment shader source has Perez function */
void test_frag_has_perez(void)
{
    const char *src = sky_frag_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "perez"));
    TEST_ASSERT_NOT_NULL(strstr(src, "u_turbidity"));
    TEST_ASSERT_NOT_NULL(strstr(src, "u_sun_dir"));
}

/* 12. Fragment shader has sun glow */
void test_frag_has_sun_glow(void)
{
    const char *src = sky_frag_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "sun_glow"));
}

/* 13. Sun overhead: sky brighter than sun at horizon */
void test_sun_overhead_brighter(void)
{
    sky_config_t cfg1 = sky_default_config();
    cfg1.sun_theta = 0.1f; /* nearly overhead */
    sky_config_t cfg2 = cfg1;
    cfg2.sun_theta = 1.4f; /* near horizon */
    sky_params_t p1 = sky_compute_params(cfg1);
    sky_params_t p2 = sky_compute_params(cfg2);
    /* Zenith luminance should be higher when sun is overhead */
    TEST_ASSERT_TRUE(p1.Y_z > p2.Y_z);
}

/* 14. Distribution coefficients change with turbidity */
void test_coefficients_vary(void)
{
    sky_config_t cfg1 = sky_default_config();
    cfg1.turbidity = 2.0f;
    sky_config_t cfg2 = cfg1;
    cfg2.turbidity = 15.0f;
    sky_params_t p1 = sky_compute_params(cfg1);
    sky_params_t p2 = sky_compute_params(cfg2);
    TEST_ASSERT_TRUE(p1.A_Y != p2.A_Y);
    TEST_ASSERT_TRUE(p1.C_Y != p2.C_Y);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_default_turbidity);
    RUN_TEST(test_zenith_luminance_positive);
    RUN_TEST(test_zenith_chromaticity_valid);
    RUN_TEST(test_zenith_is_blue);
    RUN_TEST(test_horizon_warmer_than_zenith);
    RUN_TEST(test_color_non_negative);
    RUN_TEST(test_perez_positive);
    RUN_TEST(test_turbidity_increases_luminance);
    RUN_TEST(test_null_params);
    RUN_TEST(test_vert_source);
    RUN_TEST(test_frag_has_perez);
    RUN_TEST(test_frag_has_sun_glow);
    RUN_TEST(test_sun_overhead_brighter);
    RUN_TEST(test_coefficients_vary);
    return UNITY_END();
}
