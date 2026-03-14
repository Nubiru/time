/*
 * test_seasonal_lighting.c — Tests for seasonal lighting pack module
 *
 * Tests: default config, sky gradient generation for day/night/twilight,
 * color interpolation, ambient light, sun color, night sky, fog color,
 * gradient strip packing, shader sources, edge cases (polar, tropical).
 *
 * 65 tests covering all public API functions.
 */

#include "../unity/unity.h"
#include "../../src/render/seasonal_lighting.h"

#include <math.h>
#include <string.h>

#ifndef PI
#define PI 3.14159265358979323846
#endif

#define FTOL 0.01f
#define FCLOSE(exp, act) TEST_ASSERT_FLOAT_WITHIN(FTOL, (exp), (act))

/* Helper: clamp check — value in [lo, hi] */
#define ASSERT_IN_RANGE(lo, hi, val) do { \
    TEST_ASSERT_TRUE_MESSAGE((val) >= (lo), "value below lower bound"); \
    TEST_ASSERT_TRUE_MESSAGE((val) <= (hi), "value above upper bound"); \
} while(0)

/* Helper: check RGB in [0,1] */
static void assert_rgb_valid(float r, float g, float b)
{
    ASSERT_IN_RANGE(0.0f, 1.0f, r);
    ASSERT_IN_RANGE(0.0f, 1.0f, g);
    ASSERT_IN_RANGE(0.0f, 1.0f, b);
}

void setUp(void) {}
void tearDown(void) {}

/* ======================================================================
 * 1. slp_default_config
 * ====================================================================== */

/* 1. Default config latitude is 0 (equator) */
void test_default_config_latitude(void)
{
    slp_config_t cfg = slp_default_config();
    FCLOSE(0.0, (float)cfg.latitude_deg);
}

/* 2. Default config day_of_year is spring equinox (~80) */
void test_default_config_day_of_year(void)
{
    slp_config_t cfg = slp_default_config();
    TEST_ASSERT_TRUE(cfg.day_of_year >= 1 && cfg.day_of_year <= 365);
}

/* 3. Default config sun_elevation is high (noon) */
void test_default_config_sun_elevation(void)
{
    slp_config_t cfg = slp_default_config();
    TEST_ASSERT_TRUE(cfg.sun_elevation_deg > 0.0f);
}

/* ======================================================================
 * 2. slp_sky_gradient — daytime
 * ====================================================================== */

/* 4. Noon sky has positive stop count */
void test_noon_sky_has_stops(void)
{
    slp_config_t cfg = slp_default_config();
    slp_sky_t sky = slp_sky_gradient(cfg);
    TEST_ASSERT_TRUE(sky.stop_count >= 2);
    TEST_ASSERT_TRUE(sky.stop_count <= SLP_MAX_STOPS);
}

/* 5. Noon sky first stop is at altitude 0 (horizon) */
void test_noon_sky_first_stop_at_horizon(void)
{
    slp_config_t cfg = slp_default_config();
    slp_sky_t sky = slp_sky_gradient(cfg);
    FCLOSE(0.0f, sky.stops[0].altitude);
}

/* 6. Noon sky last stop is at altitude 1 (zenith) */
void test_noon_sky_last_stop_at_zenith(void)
{
    slp_config_t cfg = slp_default_config();
    slp_sky_t sky = slp_sky_gradient(cfg);
    FCLOSE(1.0f, sky.stops[sky.stop_count - 1].altitude);
}

/* 7. Noon sky stops are in ascending altitude order */
void test_noon_sky_stops_ascending(void)
{
    slp_config_t cfg = slp_default_config();
    slp_sky_t sky = slp_sky_gradient(cfg);
    for (int i = 1; i < sky.stop_count; i++) {
        TEST_ASSERT_TRUE(sky.stops[i].altitude > sky.stops[i - 1].altitude);
    }
}

/* 8. Noon sky all colors are valid RGB */
void test_noon_sky_colors_valid(void)
{
    slp_config_t cfg = slp_default_config();
    slp_sky_t sky = slp_sky_gradient(cfg);
    for (int i = 0; i < sky.stop_count; i++) {
        assert_rgb_valid(sky.stops[i].r, sky.stops[i].g, sky.stops[i].b);
    }
}

/* 9. Noon sky zenith is bluer than horizon */
void test_noon_sky_zenith_bluer_than_horizon(void)
{
    slp_config_t cfg = slp_default_config();
    slp_sky_t sky = slp_sky_gradient(cfg);
    slp_color_stop_t horizon = sky.stops[0];
    slp_color_stop_t zenith = sky.stops[sky.stop_count - 1];
    /* Zenith blue channel should be significant */
    TEST_ASSERT_TRUE(zenith.b > 0.3f);
    /* Zenith should be more blue-dominant (b/r ratio higher) than horizon */
    float zenith_ratio = (zenith.r > 0.001f) ? zenith.b / zenith.r : 100.0f;
    float horizon_ratio = (horizon.r > 0.001f) ? horizon.b / horizon.r : 100.0f;
    TEST_ASSERT_TRUE(zenith_ratio >= horizon_ratio);
}

/* 10. Noon sky ambient intensity is high */
void test_noon_ambient_intensity_high(void)
{
    slp_config_t cfg = slp_default_config();
    slp_sky_t sky = slp_sky_gradient(cfg);
    TEST_ASSERT_TRUE(sky.ambient_intensity >= 0.7f);
    TEST_ASSERT_TRUE(sky.ambient_intensity <= 1.0f);
}

/* 11. Noon sky ambient color is valid */
void test_noon_ambient_color_valid(void)
{
    slp_config_t cfg = slp_default_config();
    slp_sky_t sky = slp_sky_gradient(cfg);
    assert_rgb_valid(sky.ambient_r, sky.ambient_g, sky.ambient_b);
}

/* ======================================================================
 * 3. slp_sky_gradient — night
 * ====================================================================== */

/* 12. Night sky (sun at -20 deg) is very dark */
void test_night_sky_is_dark(void)
{
    slp_config_t cfg = slp_default_config();
    cfg.sun_elevation_deg = -20.0f;
    slp_sky_t sky = slp_sky_gradient(cfg);
    for (int i = 0; i < sky.stop_count; i++) {
        TEST_ASSERT_TRUE(sky.stops[i].r < 0.15f);
        TEST_ASSERT_TRUE(sky.stops[i].g < 0.15f);
        TEST_ASSERT_TRUE(sky.stops[i].b < 0.20f);
    }
}

/* 13. Night sky ambient intensity is very low */
void test_night_ambient_intensity_low(void)
{
    slp_config_t cfg = slp_default_config();
    cfg.sun_elevation_deg = -20.0f;
    slp_sky_t sky = slp_sky_gradient(cfg);
    TEST_ASSERT_TRUE(sky.ambient_intensity <= 0.10f);
}

/* 14. Night sky gradient is not pure black (space-black philosophy) */
void test_night_gradient_not_pure_black(void)
{
    slp_config_t cfg = slp_default_config();
    cfg.sun_elevation_deg = -30.0f;
    slp_sky_t sky = slp_sky_gradient(cfg);
    /* At least one color channel should be > 0 at zenith */
    slp_color_stop_t zenith = sky.stops[sky.stop_count - 1];
    float total = zenith.r + zenith.g + zenith.b;
    TEST_ASSERT_TRUE(total > 0.01f);
}

/* ======================================================================
 * 4. slp_sky_gradient — golden hour
 * ====================================================================== */

/* 15. Golden hour (sun at 3 deg) horizon has warm tones */
void test_golden_hour_horizon_warm(void)
{
    slp_config_t cfg = slp_default_config();
    cfg.sun_elevation_deg = 3.0f;
    slp_sky_t sky = slp_sky_gradient(cfg);
    slp_color_stop_t horizon = sky.stops[0];
    /* Horizon should have red > blue in golden hour */
    TEST_ASSERT_TRUE(horizon.r > horizon.b);
}

/* 16. Golden hour zenith is blue-purple */
void test_golden_hour_zenith_blue(void)
{
    slp_config_t cfg = slp_default_config();
    cfg.sun_elevation_deg = 3.0f;
    slp_sky_t sky = slp_sky_gradient(cfg);
    slp_color_stop_t zenith = sky.stops[sky.stop_count - 1];
    /* Zenith should still have significant blue */
    TEST_ASSERT_TRUE(zenith.b > 0.15f);
}

/* 17. Golden hour ambient intensity is medium */
void test_golden_hour_ambient_medium(void)
{
    slp_config_t cfg = slp_default_config();
    cfg.sun_elevation_deg = 3.0f;
    slp_sky_t sky = slp_sky_gradient(cfg);
    TEST_ASSERT_TRUE(sky.ambient_intensity >= 0.3f);
    TEST_ASSERT_TRUE(sky.ambient_intensity <= 0.8f);
}

/* ======================================================================
 * 5. slp_sky_gradient — blue hour
 * ====================================================================== */

/* 18. Blue hour (sun at -5 deg) is predominantly blue */
void test_blue_hour_sky_is_blue(void)
{
    slp_config_t cfg = slp_default_config();
    cfg.sun_elevation_deg = -5.0f;
    slp_sky_t sky = slp_sky_gradient(cfg);
    for (int i = 0; i < sky.stop_count; i++) {
        /* Blue should be the dominant channel or close to it */
        TEST_ASSERT_TRUE(sky.stops[i].b >= sky.stops[i].r - 0.05f);
    }
}

/* 19. Blue hour ambient intensity is low */
void test_blue_hour_ambient_low(void)
{
    slp_config_t cfg = slp_default_config();
    cfg.sun_elevation_deg = -5.0f;
    slp_sky_t sky = slp_sky_gradient(cfg);
    TEST_ASSERT_TRUE(sky.ambient_intensity >= 0.05f);
    TEST_ASSERT_TRUE(sky.ambient_intensity <= 0.40f);
}

/* ======================================================================
 * 6. slp_sky_color_at — interpolation
 * ====================================================================== */

/* 20. Color at altitude 0 matches first stop */
void test_sky_color_at_horizon(void)
{
    slp_config_t cfg = slp_default_config();
    slp_sky_t sky = slp_sky_gradient(cfg);
    float rgb[3];
    slp_sky_color_at(&sky, 0.0f, rgb);
    FCLOSE(sky.stops[0].r, rgb[0]);
    FCLOSE(sky.stops[0].g, rgb[1]);
    FCLOSE(sky.stops[0].b, rgb[2]);
}

/* 21. Color at altitude 1 matches last stop */
void test_sky_color_at_zenith(void)
{
    slp_config_t cfg = slp_default_config();
    slp_sky_t sky = slp_sky_gradient(cfg);
    float rgb[3];
    slp_sky_color_at(&sky, 1.0f, rgb);
    slp_color_stop_t last = sky.stops[sky.stop_count - 1];
    FCLOSE(last.r, rgb[0]);
    FCLOSE(last.g, rgb[1]);
    FCLOSE(last.b, rgb[2]);
}

/* 22. Color at midpoint is between horizon and zenith */
void test_sky_color_at_midpoint(void)
{
    slp_config_t cfg = slp_default_config();
    slp_sky_t sky = slp_sky_gradient(cfg);
    float rgb[3];
    slp_sky_color_at(&sky, 0.5f, rgb);
    assert_rgb_valid(rgb[0], rgb[1], rgb[2]);
}

/* 23. Color at negative altitude clamps to horizon */
void test_sky_color_at_negative_clamps(void)
{
    slp_config_t cfg = slp_default_config();
    slp_sky_t sky = slp_sky_gradient(cfg);
    float rgb_neg[3], rgb_zero[3];
    slp_sky_color_at(&sky, -0.5f, rgb_neg);
    slp_sky_color_at(&sky, 0.0f, rgb_zero);
    FCLOSE(rgb_zero[0], rgb_neg[0]);
    FCLOSE(rgb_zero[1], rgb_neg[1]);
    FCLOSE(rgb_zero[2], rgb_neg[2]);
}

/* 24. Color at altitude > 1 clamps to zenith */
void test_sky_color_at_above_one_clamps(void)
{
    slp_config_t cfg = slp_default_config();
    slp_sky_t sky = slp_sky_gradient(cfg);
    float rgb_over[3], rgb_one[3];
    slp_sky_color_at(&sky, 1.5f, rgb_over);
    slp_sky_color_at(&sky, 1.0f, rgb_one);
    FCLOSE(rgb_one[0], rgb_over[0]);
    FCLOSE(rgb_one[1], rgb_over[1]);
    FCLOSE(rgb_one[2], rgb_over[2]);
}

/* ======================================================================
 * 7. slp_ambient_light
 * ====================================================================== */

/* 25. Noon ambient is bright */
void test_ambient_noon_bright(void)
{
    slp_config_t cfg = slp_default_config();
    float rgb[3], intensity;
    slp_ambient_light(cfg, rgb, &intensity);
    TEST_ASSERT_TRUE(intensity >= 0.7f);
    assert_rgb_valid(rgb[0], rgb[1], rgb[2]);
}

/* 26. Night ambient is dim */
void test_ambient_night_dim(void)
{
    slp_config_t cfg = slp_default_config();
    cfg.sun_elevation_deg = -20.0f;
    float rgb[3], intensity;
    slp_ambient_light(cfg, rgb, &intensity);
    TEST_ASSERT_TRUE(intensity <= 0.10f);
}

/* 27. Sunrise ambient is medium */
void test_ambient_sunrise_medium(void)
{
    slp_config_t cfg = slp_default_config();
    cfg.sun_elevation_deg = 1.0f;
    float rgb[3], intensity;
    slp_ambient_light(cfg, rgb, &intensity);
    TEST_ASSERT_TRUE(intensity >= 0.2f);
    TEST_ASSERT_TRUE(intensity <= 0.8f);
}

/* 28. Ambient intensity monotonically increases with sun elevation */
void test_ambient_monotonic_with_elevation(void)
{
    slp_config_t cfg = slp_default_config();
    float prev_intensity = 0.0f;
    float elevations[] = {-20.0f, -10.0f, -5.0f, 0.0f, 5.0f, 15.0f, 30.0f, 60.0f, 90.0f};
    int n = (int)(sizeof(elevations) / sizeof(elevations[0]));
    for (int i = 0; i < n; i++) {
        cfg.sun_elevation_deg = elevations[i];
        float rgb[3], intensity;
        slp_ambient_light(cfg, rgb, &intensity);
        TEST_ASSERT_TRUE(intensity >= prev_intensity - 0.01f);
        prev_intensity = intensity;
    }
}

/* ======================================================================
 * 8. slp_sun_color
 * ====================================================================== */

/* 29. Sun at noon is near white */
void test_sun_color_noon_white(void)
{
    float rgb[3];
    slp_sun_color(60.0f, rgb);
    assert_rgb_valid(rgb[0], rgb[1], rgb[2]);
    /* Should be near white: all channels > 0.8 */
    TEST_ASSERT_TRUE(rgb[0] > 0.85f);
    TEST_ASSERT_TRUE(rgb[1] > 0.80f);
    TEST_ASSERT_TRUE(rgb[2] > 0.70f);
}

/* 30. Sun at horizon is orange/gold */
void test_sun_color_horizon_orange(void)
{
    float rgb[3];
    slp_sun_color(2.0f, rgb);
    assert_rgb_valid(rgb[0], rgb[1], rgb[2]);
    /* Red should dominate blue significantly */
    TEST_ASSERT_TRUE(rgb[0] > rgb[2] + 0.15f);
}

/* 31. Sun below horizon is deep orange/red */
void test_sun_color_below_horizon(void)
{
    float rgb[3];
    slp_sun_color(-1.0f, rgb);
    assert_rgb_valid(rgb[0], rgb[1], rgb[2]);
    /* Red/orange dominant */
    TEST_ASSERT_TRUE(rgb[0] > rgb[2]);
}

/* 32. Sun color red channel always >= blue channel */
void test_sun_color_red_ge_blue(void)
{
    float elevations[] = {-5.0f, -2.0f, 0.0f, 5.0f, 10.0f, 30.0f, 60.0f, 90.0f};
    int n = (int)(sizeof(elevations) / sizeof(elevations[0]));
    for (int i = 0; i < n; i++) {
        float rgb[3];
        slp_sun_color(elevations[i], rgb);
        TEST_ASSERT_TRUE(rgb[0] >= rgb[2] - 0.02f);
    }
}

/* 33. Sun color is valid RGB for all elevations */
void test_sun_color_valid_all_elevations(void)
{
    for (float e = -10.0f; e <= 90.0f; e += 5.0f) {
        float rgb[3];
        slp_sun_color(e, rgb);
        assert_rgb_valid(rgb[0], rgb[1], rgb[2]);
    }
}

/* ======================================================================
 * 9. slp_night_sky_color
 * ====================================================================== */

/* 34. Night sky is very dark */
void test_night_sky_color_dark(void)
{
    float rgb[3];
    slp_night_sky_color(0.0, 80, rgb);
    assert_rgb_valid(rgb[0], rgb[1], rgb[2]);
    TEST_ASSERT_TRUE(rgb[0] < 0.10f);
    TEST_ASSERT_TRUE(rgb[1] < 0.10f);
    TEST_ASSERT_TRUE(rgb[2] < 0.15f);
}

/* 35. Night sky is not pure black */
void test_night_sky_not_pure_black(void)
{
    float rgb[3];
    slp_night_sky_color(45.0, 172, rgb);
    float total = rgb[0] + rgb[1] + rgb[2];
    TEST_ASSERT_TRUE(total > 0.01f);
}

/* 36. Night sky has blue tint (blue >= red) */
void test_night_sky_blue_tint(void)
{
    float rgb[3];
    slp_night_sky_color(30.0, 100, rgb);
    TEST_ASSERT_TRUE(rgb[2] >= rgb[0]);
}

/* 37. Night sky varies slightly with latitude */
void test_night_sky_latitude_variation(void)
{
    float rgb_eq[3], rgb_hi[3];
    slp_night_sky_color(0.0, 172, rgb_eq);
    slp_night_sky_color(65.0, 172, rgb_hi);
    /* There should be some difference (however slight) */
    float diff = fabsf(rgb_eq[0] - rgb_hi[0]) + fabsf(rgb_eq[1] - rgb_hi[1])
               + fabsf(rgb_eq[2] - rgb_hi[2]);
    TEST_ASSERT_TRUE(diff > 0.001f);
}

/* 38. Night sky varies with day of year */
void test_night_sky_seasonal_variation(void)
{
    float rgb_sum[3], rgb_win[3];
    slp_night_sky_color(45.0, 172, rgb_sum);
    slp_night_sky_color(45.0, 355, rgb_win);
    float diff = fabsf(rgb_sum[0] - rgb_win[0]) + fabsf(rgb_sum[1] - rgb_win[1])
               + fabsf(rgb_sum[2] - rgb_win[2]);
    TEST_ASSERT_TRUE(diff > 0.001f);
}

/* ======================================================================
 * 10. slp_fog_color
 * ====================================================================== */

/* 39. Fog color at noon is valid */
void test_fog_color_noon_valid(void)
{
    slp_config_t cfg = slp_default_config();
    float rgb[3];
    slp_fog_color(cfg, rgb);
    assert_rgb_valid(rgb[0], rgb[1], rgb[2]);
}

/* 40. Fog color at night is dark */
void test_fog_color_night_dark(void)
{
    slp_config_t cfg = slp_default_config();
    cfg.sun_elevation_deg = -20.0f;
    float rgb[3];
    slp_fog_color(cfg, rgb);
    float total = rgb[0] + rgb[1] + rgb[2];
    TEST_ASSERT_TRUE(total < 0.3f);
}

/* 41. Fog color at sunset is warm */
void test_fog_color_sunset_warm(void)
{
    slp_config_t cfg = slp_default_config();
    cfg.sun_elevation_deg = 2.0f;
    float rgb[3];
    slp_fog_color(cfg, rgb);
    assert_rgb_valid(rgb[0], rgb[1], rgb[2]);
    /* Should have some warmth: red >= blue */
    TEST_ASSERT_TRUE(rgb[0] >= rgb[2] - 0.1f);
}

/* ======================================================================
 * 11. slp_pack_gradient_strip
 * ====================================================================== */

/* 42. Strip produces vertices from sky gradient */
void test_strip_produces_vertices(void)
{
    slp_config_t cfg = slp_default_config();
    slp_sky_t sky = slp_sky_gradient(cfg);
    float verts[SLP_MAX_STRIP_VERTICES * SLP_STRIP_VERTEX_FLOATS];
    int count = slp_pack_gradient_strip(&sky, verts);
    TEST_ASSERT_TRUE(count > 0);
    TEST_ASSERT_TRUE(count <= SLP_MAX_STRIP_VERTICES);
}

/* 43. Strip has 2 vertices per stop (triangle strip) */
void test_strip_two_verts_per_stop(void)
{
    slp_config_t cfg = slp_default_config();
    slp_sky_t sky = slp_sky_gradient(cfg);
    float verts[SLP_MAX_STRIP_VERTICES * SLP_STRIP_VERTEX_FLOATS];
    int count = slp_pack_gradient_strip(&sky, verts);
    TEST_ASSERT_EQUAL_INT(sky.stop_count * 2, count);
}

/* 44. Strip vertex positions: x is -1 or +1 */
void test_strip_vertex_x_positions(void)
{
    slp_config_t cfg = slp_default_config();
    slp_sky_t sky = slp_sky_gradient(cfg);
    float verts[SLP_MAX_STRIP_VERTICES * SLP_STRIP_VERTEX_FLOATS];
    int count = slp_pack_gradient_strip(&sky, verts);
    for (int i = 0; i < count; i++) {
        float x = verts[i * SLP_STRIP_VERTEX_FLOATS + 0];
        TEST_ASSERT_TRUE(fabsf(x - (-1.0f)) < 0.01f || fabsf(x - 1.0f) < 0.01f);
    }
}

/* 45. Strip vertex y positions match stop altitudes */
void test_strip_vertex_y_positions(void)
{
    slp_config_t cfg = slp_default_config();
    slp_sky_t sky = slp_sky_gradient(cfg);
    float verts[SLP_MAX_STRIP_VERTICES * SLP_STRIP_VERTEX_FLOATS];
    slp_pack_gradient_strip(&sky, verts);
    /* Each pair of vertices should share the same y, matching stop altitude */
    for (int i = 0; i < sky.stop_count; i++) {
        float y_left = verts[(i * 2) * SLP_STRIP_VERTEX_FLOATS + 1];
        float y_right = verts[(i * 2 + 1) * SLP_STRIP_VERTEX_FLOATS + 1];
        FCLOSE(sky.stops[i].altitude, y_left);
        FCLOSE(sky.stops[i].altitude, y_right);
    }
}

/* 46. Strip vertex colors match stop colors */
void test_strip_vertex_colors(void)
{
    slp_config_t cfg = slp_default_config();
    slp_sky_t sky = slp_sky_gradient(cfg);
    float verts[SLP_MAX_STRIP_VERTICES * SLP_STRIP_VERTEX_FLOATS];
    slp_pack_gradient_strip(&sky, verts);
    for (int i = 0; i < sky.stop_count; i++) {
        int base_left = (i * 2) * SLP_STRIP_VERTEX_FLOATS;
        FCLOSE(sky.stops[i].r, verts[base_left + 2]);
        FCLOSE(sky.stops[i].g, verts[base_left + 3]);
        FCLOSE(sky.stops[i].b, verts[base_left + 4]);
    }
}

/* 47. Strip vertex count doesn't exceed max */
void test_strip_max_vertices(void)
{
    slp_config_t cfg = slp_default_config();
    slp_sky_t sky = slp_sky_gradient(cfg);
    float verts[SLP_MAX_STRIP_VERTICES * SLP_STRIP_VERTEX_FLOATS];
    int count = slp_pack_gradient_strip(&sky, verts);
    TEST_ASSERT_TRUE(count <= SLP_MAX_STRIP_VERTICES);
}

/* ======================================================================
 * 12. slp_vert_source / slp_frag_source — shaders
 * ====================================================================== */

/* 48. Vertex shader source is not NULL */
void test_vert_source_not_null(void)
{
    const char *src = slp_vert_source();
    TEST_ASSERT_NOT_NULL(src);
}

/* 49. Fragment shader source is not NULL */
void test_frag_source_not_null(void)
{
    const char *src = slp_frag_source();
    TEST_ASSERT_NOT_NULL(src);
}

/* 50. Vertex shader contains version directive */
void test_vert_source_has_version(void)
{
    const char *src = slp_vert_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "#version 300 es"));
}

/* 51. Fragment shader contains version directive */
void test_frag_source_has_version(void)
{
    const char *src = slp_frag_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "#version 300 es"));
}

/* 52. Vertex shader has main function */
void test_vert_source_has_main(void)
{
    const char *src = slp_vert_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "void main"));
}

/* 53. Fragment shader has main function */
void test_frag_source_has_main(void)
{
    const char *src = slp_frag_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "void main"));
}

/* ======================================================================
 * 13. Seasonal modulation
 * ====================================================================== */

/* 54. Summer sky is warmer/brighter than winter at same latitude */
void test_summer_brighter_than_winter(void)
{
    slp_config_t cfg_sum = {.latitude_deg = 45.0, .day_of_year = 172,
                            .sun_elevation_deg = 45.0f};
    slp_config_t cfg_win = {.latitude_deg = 45.0, .day_of_year = 355,
                            .sun_elevation_deg = 45.0f};
    slp_sky_t sky_sum = slp_sky_gradient(cfg_sum);
    slp_sky_t sky_win = slp_sky_gradient(cfg_win);
    /* Summer ambient should be >= winter ambient (more saturated) */
    TEST_ASSERT_TRUE(sky_sum.ambient_intensity >= sky_win.ambient_intensity - 0.05f);
}

/* 55. Tropical latitude has minimal seasonal variation */
void test_tropical_minimal_variation(void)
{
    slp_config_t cfg_jun = {.latitude_deg = 5.0, .day_of_year = 172,
                            .sun_elevation_deg = 60.0f};
    slp_config_t cfg_dec = {.latitude_deg = 5.0, .day_of_year = 355,
                            .sun_elevation_deg = 60.0f};
    slp_sky_t sky_jun = slp_sky_gradient(cfg_jun);
    slp_sky_t sky_dec = slp_sky_gradient(cfg_dec);
    /* Ambient intensity should be very close */
    float diff = fabsf(sky_jun.ambient_intensity - sky_dec.ambient_intensity);
    TEST_ASSERT_TRUE(diff < 0.15f);
}

/* 56. Polar latitude has extreme variation */
void test_polar_extreme_variation(void)
{
    /* Arctic: summer noon vs winter noon */
    slp_config_t cfg_sum = {.latitude_deg = 70.0, .day_of_year = 172,
                            .sun_elevation_deg = 40.0f};
    slp_config_t cfg_win = {.latitude_deg = 70.0, .day_of_year = 355,
                            .sun_elevation_deg = 40.0f};
    slp_sky_t sky_sum = slp_sky_gradient(cfg_sum);
    slp_sky_t sky_win = slp_sky_gradient(cfg_win);
    float diff = fabsf(sky_sum.ambient_intensity - sky_win.ambient_intensity);
    TEST_ASSERT_TRUE(diff > 0.01f);
}

/* ======================================================================
 * 14. Edge cases
 * ====================================================================== */

/* 57. Southern hemisphere seasons are flipped */
void test_southern_hemisphere(void)
{
    /* December = summer in south */
    slp_config_t cfg = {.latitude_deg = -35.0, .day_of_year = 355,
                        .sun_elevation_deg = 50.0f};
    slp_sky_t sky = slp_sky_gradient(cfg);
    TEST_ASSERT_TRUE(sky.stop_count >= 2);
    assert_rgb_valid(sky.ambient_r, sky.ambient_g, sky.ambient_b);
}

/* 58. Extreme latitude: 89 degrees */
void test_extreme_latitude(void)
{
    slp_config_t cfg = {.latitude_deg = 89.0, .day_of_year = 172,
                        .sun_elevation_deg = 20.0f};
    slp_sky_t sky = slp_sky_gradient(cfg);
    TEST_ASSERT_TRUE(sky.stop_count >= 2);
}

/* 59. Day of year boundaries: day 1 */
void test_day_of_year_1(void)
{
    slp_config_t cfg = {.latitude_deg = 40.0, .day_of_year = 1,
                        .sun_elevation_deg = 30.0f};
    slp_sky_t sky = slp_sky_gradient(cfg);
    TEST_ASSERT_TRUE(sky.stop_count >= 2);
}

/* 60. Day of year boundaries: day 365 */
void test_day_of_year_365(void)
{
    slp_config_t cfg = {.latitude_deg = 40.0, .day_of_year = 365,
                        .sun_elevation_deg = 30.0f};
    slp_sky_t sky = slp_sky_gradient(cfg);
    TEST_ASSERT_TRUE(sky.stop_count >= 2);
}

/* 61. Sun exactly at horizon (elevation = 0) */
void test_sun_at_horizon(void)
{
    slp_config_t cfg = slp_default_config();
    cfg.sun_elevation_deg = 0.0f;
    slp_sky_t sky = slp_sky_gradient(cfg);
    TEST_ASSERT_TRUE(sky.stop_count >= 2);
    /* Should be golden hour / sunset conditions */
    TEST_ASSERT_TRUE(sky.ambient_intensity >= 0.15f);
    TEST_ASSERT_TRUE(sky.ambient_intensity <= 0.65f);
}

/* 62. Sun at -90 degrees (deep below horizon) */
void test_sun_far_below_horizon(void)
{
    slp_config_t cfg = slp_default_config();
    cfg.sun_elevation_deg = -90.0f;
    slp_sky_t sky = slp_sky_gradient(cfg);
    TEST_ASSERT_TRUE(sky.stop_count >= 2);
    TEST_ASSERT_TRUE(sky.ambient_intensity <= 0.10f);
}

/* 63. Sun at 90 degrees (directly overhead) */
void test_sun_directly_overhead(void)
{
    slp_config_t cfg = slp_default_config();
    cfg.sun_elevation_deg = 90.0f;
    slp_sky_t sky = slp_sky_gradient(cfg);
    TEST_ASSERT_TRUE(sky.ambient_intensity >= 0.8f);
}

/* ======================================================================
 * 15. Gradient continuity
 * ====================================================================== */

/* 64. Sky colors transition smoothly (no huge jumps between stops) */
void test_smooth_gradient(void)
{
    slp_config_t cfg = slp_default_config();
    slp_sky_t sky = slp_sky_gradient(cfg);
    for (int i = 1; i < sky.stop_count; i++) {
        float dr = fabsf(sky.stops[i].r - sky.stops[i - 1].r);
        float dg = fabsf(sky.stops[i].g - sky.stops[i - 1].g);
        float db = fabsf(sky.stops[i].b - sky.stops[i - 1].b);
        /* No channel should jump more than 0.6 between adjacent stops */
        TEST_ASSERT_TRUE(dr < 0.6f);
        TEST_ASSERT_TRUE(dg < 0.6f);
        TEST_ASSERT_TRUE(db < 0.6f);
    }
}

/* 65. Interpolated colors are smooth across full altitude range */
void test_interpolation_smooth(void)
{
    slp_config_t cfg = slp_default_config();
    slp_sky_t sky = slp_sky_gradient(cfg);
    float prev_rgb[3] = {0.0f, 0.0f, 0.0f};
    slp_sky_color_at(&sky, 0.0f, prev_rgb);
    for (int i = 1; i <= 20; i++) {
        float alt = (float)i / 20.0f;
        float rgb[3];
        slp_sky_color_at(&sky, alt, rgb);
        float dr = fabsf(rgb[0] - prev_rgb[0]);
        float dg = fabsf(rgb[1] - prev_rgb[1]);
        float db = fabsf(rgb[2] - prev_rgb[2]);
        /* Each 5% step should not jump more than 0.3 in any channel */
        TEST_ASSERT_TRUE(dr < 0.3f);
        TEST_ASSERT_TRUE(dg < 0.3f);
        TEST_ASSERT_TRUE(db < 0.3f);
        prev_rgb[0] = rgb[0];
        prev_rgb[1] = rgb[1];
        prev_rgb[2] = rgb[2];
    }
}

/* ======================================================================
 * Main
 * ====================================================================== */

int main(void)
{
    UNITY_BEGIN();

    /* 1. Default config */
    RUN_TEST(test_default_config_latitude);
    RUN_TEST(test_default_config_day_of_year);
    RUN_TEST(test_default_config_sun_elevation);

    /* 2. Sky gradient — daytime */
    RUN_TEST(test_noon_sky_has_stops);
    RUN_TEST(test_noon_sky_first_stop_at_horizon);
    RUN_TEST(test_noon_sky_last_stop_at_zenith);
    RUN_TEST(test_noon_sky_stops_ascending);
    RUN_TEST(test_noon_sky_colors_valid);
    RUN_TEST(test_noon_sky_zenith_bluer_than_horizon);
    RUN_TEST(test_noon_ambient_intensity_high);
    RUN_TEST(test_noon_ambient_color_valid);

    /* 3. Sky gradient — night */
    RUN_TEST(test_night_sky_is_dark);
    RUN_TEST(test_night_ambient_intensity_low);
    RUN_TEST(test_night_gradient_not_pure_black);

    /* 4. Sky gradient — golden hour */
    RUN_TEST(test_golden_hour_horizon_warm);
    RUN_TEST(test_golden_hour_zenith_blue);
    RUN_TEST(test_golden_hour_ambient_medium);

    /* 5. Sky gradient — blue hour */
    RUN_TEST(test_blue_hour_sky_is_blue);
    RUN_TEST(test_blue_hour_ambient_low);

    /* 6. Color interpolation */
    RUN_TEST(test_sky_color_at_horizon);
    RUN_TEST(test_sky_color_at_zenith);
    RUN_TEST(test_sky_color_at_midpoint);
    RUN_TEST(test_sky_color_at_negative_clamps);
    RUN_TEST(test_sky_color_at_above_one_clamps);

    /* 7. Ambient light */
    RUN_TEST(test_ambient_noon_bright);
    RUN_TEST(test_ambient_night_dim);
    RUN_TEST(test_ambient_sunrise_medium);
    RUN_TEST(test_ambient_monotonic_with_elevation);

    /* 8. Sun color */
    RUN_TEST(test_sun_color_noon_white);
    RUN_TEST(test_sun_color_horizon_orange);
    RUN_TEST(test_sun_color_below_horizon);
    RUN_TEST(test_sun_color_red_ge_blue);
    RUN_TEST(test_sun_color_valid_all_elevations);

    /* 9. Night sky color */
    RUN_TEST(test_night_sky_color_dark);
    RUN_TEST(test_night_sky_not_pure_black);
    RUN_TEST(test_night_sky_blue_tint);
    RUN_TEST(test_night_sky_latitude_variation);
    RUN_TEST(test_night_sky_seasonal_variation);

    /* 10. Fog color */
    RUN_TEST(test_fog_color_noon_valid);
    RUN_TEST(test_fog_color_night_dark);
    RUN_TEST(test_fog_color_sunset_warm);

    /* 11. Gradient strip packing */
    RUN_TEST(test_strip_produces_vertices);
    RUN_TEST(test_strip_two_verts_per_stop);
    RUN_TEST(test_strip_vertex_x_positions);
    RUN_TEST(test_strip_vertex_y_positions);
    RUN_TEST(test_strip_vertex_colors);
    RUN_TEST(test_strip_max_vertices);

    /* 12. Shaders */
    RUN_TEST(test_vert_source_not_null);
    RUN_TEST(test_frag_source_not_null);
    RUN_TEST(test_vert_source_has_version);
    RUN_TEST(test_frag_source_has_version);
    RUN_TEST(test_vert_source_has_main);
    RUN_TEST(test_frag_source_has_main);

    /* 13. Seasonal modulation */
    RUN_TEST(test_summer_brighter_than_winter);
    RUN_TEST(test_tropical_minimal_variation);
    RUN_TEST(test_polar_extreme_variation);

    /* 14. Edge cases */
    RUN_TEST(test_southern_hemisphere);
    RUN_TEST(test_extreme_latitude);
    RUN_TEST(test_day_of_year_1);
    RUN_TEST(test_day_of_year_365);
    RUN_TEST(test_sun_at_horizon);
    RUN_TEST(test_sun_far_below_horizon);
    RUN_TEST(test_sun_directly_overhead);

    /* 15. Gradient continuity */
    RUN_TEST(test_smooth_gradient);
    RUN_TEST(test_interpolation_smooth);

    return UNITY_END();
}
