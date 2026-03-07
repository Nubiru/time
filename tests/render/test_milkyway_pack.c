/*
 * test_milkyway_pack.c -- Tests for Milky Way band vertex packing
 *
 * RED phase: 42 tests covering all public API functions.
 */

#include "../unity/unity.h"
#include "../../src/render/milkyway_pack.h"
#include "../../src/render/galaxy_geometry.h"

#include <math.h>
#include <string.h>

#define PI 3.14159265358979323846

#define FTOL 0.01f
#define FCLOSE(exp, act) TEST_ASSERT_FLOAT_WITHIN(FTOL, (exp), (act))

void setUp(void) {}
void tearDown(void) {}

/* ======================================================================
 * mwp_default_config
 * ====================================================================== */

/* 1. Default config returns positive sphere_radius */
void test_default_config_sphere_radius(void)
{
    mwp_config_t cfg = mwp_default_config();
    TEST_ASSERT_TRUE(cfg.sphere_radius > 0.0f);
}

/* 2. Default config has positive band_width */
void test_default_config_band_width(void)
{
    mwp_config_t cfg = mwp_default_config();
    TEST_ASSERT_TRUE(cfg.band_width > 0.0f);
}

/* 3. Default config core_brightness > edge_brightness */
void test_default_config_brightness_order(void)
{
    mwp_config_t cfg = mwp_default_config();
    TEST_ASSERT_TRUE(cfg.core_brightness > cfg.edge_brightness);
}

/* 4. Default config base_alpha in (0,1] */
void test_default_config_base_alpha(void)
{
    mwp_config_t cfg = mwp_default_config();
    TEST_ASSERT_TRUE(cfg.base_alpha > 0.0f);
    TEST_ASSERT_TRUE(cfg.base_alpha <= 1.0f);
}

/* 5. Default config dark_lane_factor in (0,1) */
void test_default_config_dark_lane_factor(void)
{
    mwp_config_t cfg = mwp_default_config();
    TEST_ASSERT_TRUE(cfg.dark_lane_factor > 0.0f);
    TEST_ASSERT_TRUE(cfg.dark_lane_factor < 1.0f);
}

/* 6. Default config galaxy_radius positive */
void test_default_config_galaxy_radius(void)
{
    mwp_config_t cfg = mwp_default_config();
    TEST_ASSERT_TRUE(cfg.galaxy_radius > 0.0f);
}

/* 7. Default config points_per_arm > 0 and <= 128 */
void test_default_config_points_per_arm(void)
{
    mwp_config_t cfg = mwp_default_config();
    TEST_ASSERT_TRUE(cfg.points_per_arm > 0);
    TEST_ASSERT_TRUE(cfg.points_per_arm <= GALAXY_MAX_ARM_POINTS);
}

/* ======================================================================
 * mwp_galactic_to_sphere
 * ====================================================================== */

/* 8. Origin galactic point at lat=0 maps to sphere surface */
void test_galactic_to_sphere_origin_on_sphere(void)
{
    float out[3];
    mwp_galactic_to_sphere(1.0f, 0.0f, 0.0f, 100.0f, out);
    float mag = sqrtf(out[0] * out[0] + out[1] * out[1] + out[2] * out[2]);
    FCLOSE(100.0f, mag);
}

/* 9. Positive galactic latitude lifts Y */
void test_galactic_to_sphere_positive_lat(void)
{
    float out_zero[3], out_pos[3];
    mwp_galactic_to_sphere(50.0f, 0.0f, 0.0f, 100.0f, out_zero);
    mwp_galactic_to_sphere(50.0f, 0.0f, 20.0f, 100.0f, out_pos);
    /* Positive lat should elevate the Y component compared to zero lat */
    TEST_ASSERT_TRUE(out_pos[1] > out_zero[1]);
}

/* 10. Negative galactic latitude lowers Y */
void test_galactic_to_sphere_negative_lat(void)
{
    float out_zero[3], out_neg[3];
    mwp_galactic_to_sphere(50.0f, 0.0f, 0.0f, 100.0f, out_zero);
    mwp_galactic_to_sphere(50.0f, 0.0f, -20.0f, 100.0f, out_neg);
    TEST_ASSERT_TRUE(out_neg[1] < out_zero[1]);
}

/* 11. All results lie on the sphere regardless of input angle */
void test_galactic_to_sphere_always_on_sphere(void)
{
    float out[3];
    float radius = 50.0f;
    for (int i = 0; i < 36; i++) {
        float angle = (float)i * 10.0f;
        float gx = cosf(angle * (float)PI / 180.0f) * 30.0f;
        float gz = sinf(angle * (float)PI / 180.0f) * 30.0f;
        mwp_galactic_to_sphere(gx, gz, 5.0f, radius, out);
        float mag = sqrtf(out[0] * out[0] + out[1] * out[1] + out[2] * out[2]);
        FCLOSE(radius, mag);
    }
}

/* 12. Radius scaling */
void test_galactic_to_sphere_radius_scaling(void)
{
    float out1[3], out2[3];
    mwp_galactic_to_sphere(30.0f, 20.0f, 10.0f, 1.0f, out1);
    mwp_galactic_to_sphere(30.0f, 20.0f, 10.0f, 50.0f, out2);
    FCLOSE(out1[0] * 50.0f, out2[0]);
    FCLOSE(out1[1] * 50.0f, out2[1]);
    FCLOSE(out1[2] * 50.0f, out2[2]);
}

/* 13. Galactic point at origin maps to a definite direction */
void test_galactic_to_sphere_zero_zero(void)
{
    float out[3];
    mwp_galactic_to_sphere(0.0f, 0.0f, 0.0f, 100.0f, out);
    /* gal_x=0, gal_z=0 => lon=atan2(0,0)=0, lat=0 => x=r, y=0, z=0 */
    /* However atan2(0,0) is implementation defined; just check on sphere */
    float mag = sqrtf(out[0] * out[0] + out[1] * out[1] + out[2] * out[2]);
    FCLOSE(100.0f, mag);
}

/* ======================================================================
 * mwp_arm_brightness
 * ====================================================================== */

/* 14. t=0 (center) returns core_brightness */
void test_arm_brightness_at_center(void)
{
    float b = mwp_arm_brightness(0.0f, 0.8f, 0.15f);
    FCLOSE(0.8f, b);
}

/* 15. t=1 (edge) returns edge_brightness */
void test_arm_brightness_at_edge(void)
{
    float b = mwp_arm_brightness(1.0f, 0.8f, 0.15f);
    FCLOSE(0.15f, b);
}

/* 16. t=0.5 (midway) returns value between core and edge */
void test_arm_brightness_midway(void)
{
    float b = mwp_arm_brightness(0.5f, 0.8f, 0.15f);
    TEST_ASSERT_TRUE(b > 0.15f);
    TEST_ASSERT_TRUE(b < 0.8f);
}

/* 17. Brightness monotonically decreases from center to edge */
void test_arm_brightness_monotonic(void)
{
    float prev = mwp_arm_brightness(0.0f, 1.0f, 0.0f);
    for (int i = 1; i <= 10; i++) {
        float t = (float)i / 10.0f;
        float cur = mwp_arm_brightness(t, 1.0f, 0.0f);
        TEST_ASSERT_TRUE(cur <= prev);
        prev = cur;
    }
}

/* 18. t clamped below 0 */
void test_arm_brightness_clamp_negative(void)
{
    float b = mwp_arm_brightness(-0.5f, 0.8f, 0.15f);
    FCLOSE(0.8f, b);
}

/* 19. t clamped above 1 */
void test_arm_brightness_clamp_above_one(void)
{
    float b = mwp_arm_brightness(1.5f, 0.8f, 0.15f);
    FCLOSE(0.15f, b);
}

/* ======================================================================
 * mwp_dark_lane_factor
 * ====================================================================== */

/* 20. Longitude 180 (far from rift) returns 1.0 (no reduction) */
void test_dark_lane_far_from_rift(void)
{
    float f = mwp_dark_lane_factor(180.0f, 0.3f);
    FCLOSE(1.0f, f);
}

/* 21. Longitude 0 (in rift center) returns dark_lane_factor */
void test_dark_lane_at_center(void)
{
    float f = mwp_dark_lane_factor(0.0f, 0.3f);
    TEST_ASSERT_TRUE(f < 1.0f);
    TEST_ASSERT_TRUE(f >= 0.3f);
}

/* 22. Longitude 330 (in rift zone) returns reduced factor */
void test_dark_lane_in_rift_zone(void)
{
    float f = mwp_dark_lane_factor(330.0f, 0.3f);
    TEST_ASSERT_TRUE(f < 1.0f);
}

/* 23. Longitude 15 (in rift zone) also reduced */
void test_dark_lane_at_15_deg(void)
{
    float f = mwp_dark_lane_factor(15.0f, 0.3f);
    TEST_ASSERT_TRUE(f < 1.0f);
}

/* 24. Factor always >= dark_lane_factor and <= 1.0 */
void test_dark_lane_bounds(void)
{
    for (int i = 0; i < 360; i++) {
        float f = mwp_dark_lane_factor((float)i, 0.3f);
        TEST_ASSERT_TRUE(f >= 0.3f - 0.001f);
        TEST_ASSERT_TRUE(f <= 1.001f);
    }
}

/* 25. dark_lane_factor=1.0 means no reduction anywhere */
void test_dark_lane_factor_one(void)
{
    float f = mwp_dark_lane_factor(0.0f, 1.0f);
    FCLOSE(1.0f, f);
}

/* ======================================================================
 * mwp_pack
 * ====================================================================== */

/* 26. Pack returns positive vertex count */
void test_pack_returns_positive_count(void)
{
    mwp_config_t cfg = mwp_default_config();
    float buf[MWP_MAX_VERTICES * MWP_VERTEX_FLOATS];
    int count = mwp_pack(&cfg, buf);
    TEST_ASSERT_TRUE(count > 0);
}

/* 27. Pack vertex count does not exceed maximum */
void test_pack_within_max_vertices(void)
{
    mwp_config_t cfg = mwp_default_config();
    float buf[MWP_MAX_VERTICES * MWP_VERTEX_FLOATS];
    int count = mwp_pack(&cfg, buf);
    TEST_ASSERT_TRUE(count <= MWP_MAX_VERTICES);
}

/* 28. All packed positions lie on the celestial sphere */
void test_pack_positions_on_sphere(void)
{
    mwp_config_t cfg = mwp_default_config();
    float buf[MWP_MAX_VERTICES * MWP_VERTEX_FLOATS];
    int count = mwp_pack(&cfg, buf);

    for (int i = 0; i < count; i++) {
        int base = i * MWP_VERTEX_FLOATS;
        float x = buf[base + 0];
        float y = buf[base + 1];
        float z = buf[base + 2];
        float mag = sqrtf(x * x + y * y + z * z);
        /* All vertices should be on the sphere within tolerance */
        TEST_ASSERT_FLOAT_WITHIN(1.0f, cfg.sphere_radius, mag);
    }
}

/* 29. Color channels are in [0,1] range */
void test_pack_colors_in_range(void)
{
    mwp_config_t cfg = mwp_default_config();
    float buf[MWP_MAX_VERTICES * MWP_VERTEX_FLOATS];
    int count = mwp_pack(&cfg, buf);

    for (int i = 0; i < count; i++) {
        int base = i * MWP_VERTEX_FLOATS;
        for (int c = 5; c <= 8; c++) {
            TEST_ASSERT_TRUE(buf[base + c] >= 0.0f);
            TEST_ASSERT_TRUE(buf[base + c] <= 1.0f);
        }
    }
}

/* 30. UV coordinates are in [0,1] range */
void test_pack_uv_in_range(void)
{
    mwp_config_t cfg = mwp_default_config();
    float buf[MWP_MAX_VERTICES * MWP_VERTEX_FLOATS];
    int count = mwp_pack(&cfg, buf);

    for (int i = 0; i < count; i++) {
        int base = i * MWP_VERTEX_FLOATS;
        float u = buf[base + 3];
        float v = buf[base + 4];
        TEST_ASSERT_TRUE(u >= 0.0f && u <= 1.0f);
        TEST_ASSERT_TRUE(v >= 0.0f && v <= 1.0f);
    }
}

/* 31. Vertex count is divisible by 6 (triangle quads) */
void test_pack_vertex_count_divisible_by_6(void)
{
    mwp_config_t cfg = mwp_default_config();
    float buf[MWP_MAX_VERTICES * MWP_VERTEX_FLOATS];
    int count = mwp_pack(&cfg, buf);
    TEST_ASSERT_EQUAL_INT(0, count % 6);
}

/* 32. Pack with 1 point per arm produces no vertices (need >= 2 for segments) */
void test_pack_one_point_per_arm(void)
{
    mwp_config_t cfg = mwp_default_config();
    cfg.points_per_arm = 1;
    float buf[MWP_MAX_VERTICES * MWP_VERTEX_FLOATS];
    int count = mwp_pack(&cfg, buf);
    TEST_ASSERT_EQUAL_INT(0, count);
}

/* 33. Pack uses all 4 arms */
void test_pack_uses_four_arms(void)
{
    mwp_config_t cfg = mwp_default_config();
    cfg.points_per_arm = 8;
    float buf[MWP_MAX_VERTICES * MWP_VERTEX_FLOATS];
    int count = mwp_pack(&cfg, buf);
    /* 4 arms * (8-1) segments * 6 verts per quad = 168 */
    TEST_ASSERT_EQUAL_INT(4 * 7 * 6, count);
}

/* 34. Pack produces non-zero vertex data */
void test_pack_nonzero_data(void)
{
    mwp_config_t cfg = mwp_default_config();
    float buf[MWP_MAX_VERTICES * MWP_VERTEX_FLOATS];
    memset(buf, 0, sizeof(buf));
    int count = mwp_pack(&cfg, buf);
    int non_zero = 0;
    for (int i = 0; i < count * MWP_VERTEX_FLOATS; i++) {
        if (buf[i] != 0.0f) non_zero++;
    }
    TEST_ASSERT_TRUE(non_zero > 0);
}

/* 35. Different sphere radius changes vertex positions */
void test_pack_radius_affects_positions(void)
{
    mwp_config_t cfg1 = mwp_default_config();
    mwp_config_t cfg2 = mwp_default_config();
    cfg1.sphere_radius = 100.0f;
    cfg2.sphere_radius = 200.0f;
    cfg1.points_per_arm = 4;
    cfg2.points_per_arm = 4;
    float buf1[MWP_MAX_VERTICES * MWP_VERTEX_FLOATS];
    float buf2[MWP_MAX_VERTICES * MWP_VERTEX_FLOATS];
    mwp_pack(&cfg1, buf1);
    mwp_pack(&cfg2, buf2);
    /* First vertex position should differ */
    float mag1 = sqrtf(buf1[0] * buf1[0] + buf1[1] * buf1[1] + buf1[2] * buf1[2]);
    float mag2 = sqrtf(buf2[0] * buf2[0] + buf2[1] * buf2[1] + buf2[2] * buf2[2]);
    TEST_ASSERT_TRUE(mag2 > mag1);
}

/* ======================================================================
 * mwp_info
 * ====================================================================== */

/* 36. Info vertex_count matches input */
void test_info_vertex_count(void)
{
    mwp_info_t info = mwp_info(168);
    TEST_ASSERT_EQUAL_INT(168, info.vertex_count);
}

/* 37. Info float_count = vertex_count * 9 */
void test_info_float_count(void)
{
    mwp_info_t info = mwp_info(168);
    TEST_ASSERT_EQUAL_INT(168 * MWP_VERTEX_FLOATS, info.float_count);
}

/* 38. Info arm_count is 4 */
void test_info_arm_count(void)
{
    mwp_info_t info = mwp_info(168);
    TEST_ASSERT_EQUAL_INT(GALAXY_ARM_COUNT, info.arm_count);
}

/* 39. Info segment_count consistent with vertex_count */
void test_info_segment_count(void)
{
    /* 168 vertices / 6 verts per quad = 28 segments */
    mwp_info_t info = mwp_info(168);
    TEST_ASSERT_EQUAL_INT(28, info.segment_count);
}

/* ======================================================================
 * Shader sources
 * ====================================================================== */

/* 40. Vertex shader starts with #version 300 es */
void test_vert_source_version(void)
{
    const char *src = mwp_vert_source();
    TEST_ASSERT_NOT_NULL(src);
    TEST_ASSERT_EQUAL_INT(0, strncmp(src, "#version 300 es", 15));
}

/* 41. Fragment shader starts with #version 300 es */
void test_frag_source_version(void)
{
    const char *src = mwp_frag_source();
    TEST_ASSERT_NOT_NULL(src);
    TEST_ASSERT_EQUAL_INT(0, strncmp(src, "#version 300 es", 15));
}

/* 42. Vertex shader contains u_mvp */
void test_vert_source_has_mvp(void)
{
    const char *src = mwp_vert_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "u_mvp"));
}

/* 43. Fragment shader has lateral falloff */
void test_frag_source_has_falloff(void)
{
    const char *src = mwp_frag_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "exp("));
}

/* 44. Vertex shader has a_position */
void test_vert_source_has_position(void)
{
    const char *src = mwp_vert_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "a_position"));
}

/* 45. Vertex shader has a_uv */
void test_vert_source_has_uv(void)
{
    const char *src = mwp_vert_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "a_uv"));
}

/* 46. Vertex shader has a_color */
void test_vert_source_has_color(void)
{
    const char *src = mwp_vert_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "a_color"));
}

/* 47. Fragment shader has frag_color output */
void test_frag_source_has_output(void)
{
    const char *src = mwp_frag_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "frag_color"));
}

/* ======================================================================
 * Constants
 * ====================================================================== */

/* 48. MWP_VERTEX_FLOATS is 9 */
void test_vertex_floats_is_9(void)
{
    TEST_ASSERT_EQUAL_INT(9, MWP_VERTEX_FLOATS);
}

/* 49. MWP_VERTEX_STRIDE is 36 */
void test_vertex_stride_is_36(void)
{
    TEST_ASSERT_EQUAL_INT(36, MWP_VERTEX_STRIDE);
}

/* 50. MWP_MAX_VERTICES is 4*128*2*6 = 6144 */
void test_max_vertices(void)
{
    TEST_ASSERT_EQUAL_INT(6144, MWP_MAX_VERTICES);
}

/* ======================================================================
 * main
 * ====================================================================== */

int main(void)
{
    UNITY_BEGIN();

    /* Config */
    RUN_TEST(test_default_config_sphere_radius);
    RUN_TEST(test_default_config_band_width);
    RUN_TEST(test_default_config_brightness_order);
    RUN_TEST(test_default_config_base_alpha);
    RUN_TEST(test_default_config_dark_lane_factor);
    RUN_TEST(test_default_config_galaxy_radius);
    RUN_TEST(test_default_config_points_per_arm);

    /* Galactic to sphere */
    RUN_TEST(test_galactic_to_sphere_origin_on_sphere);
    RUN_TEST(test_galactic_to_sphere_positive_lat);
    RUN_TEST(test_galactic_to_sphere_negative_lat);
    RUN_TEST(test_galactic_to_sphere_always_on_sphere);
    RUN_TEST(test_galactic_to_sphere_radius_scaling);
    RUN_TEST(test_galactic_to_sphere_zero_zero);

    /* Arm brightness */
    RUN_TEST(test_arm_brightness_at_center);
    RUN_TEST(test_arm_brightness_at_edge);
    RUN_TEST(test_arm_brightness_midway);
    RUN_TEST(test_arm_brightness_monotonic);
    RUN_TEST(test_arm_brightness_clamp_negative);
    RUN_TEST(test_arm_brightness_clamp_above_one);

    /* Dark lane factor */
    RUN_TEST(test_dark_lane_far_from_rift);
    RUN_TEST(test_dark_lane_at_center);
    RUN_TEST(test_dark_lane_in_rift_zone);
    RUN_TEST(test_dark_lane_at_15_deg);
    RUN_TEST(test_dark_lane_bounds);
    RUN_TEST(test_dark_lane_factor_one);

    /* Pack */
    RUN_TEST(test_pack_returns_positive_count);
    RUN_TEST(test_pack_within_max_vertices);
    RUN_TEST(test_pack_positions_on_sphere);
    RUN_TEST(test_pack_colors_in_range);
    RUN_TEST(test_pack_uv_in_range);
    RUN_TEST(test_pack_vertex_count_divisible_by_6);
    RUN_TEST(test_pack_one_point_per_arm);
    RUN_TEST(test_pack_uses_four_arms);
    RUN_TEST(test_pack_nonzero_data);
    RUN_TEST(test_pack_radius_affects_positions);

    /* Info */
    RUN_TEST(test_info_vertex_count);
    RUN_TEST(test_info_float_count);
    RUN_TEST(test_info_arm_count);
    RUN_TEST(test_info_segment_count);

    /* Shaders */
    RUN_TEST(test_vert_source_version);
    RUN_TEST(test_frag_source_version);
    RUN_TEST(test_vert_source_has_mvp);
    RUN_TEST(test_frag_source_has_falloff);
    RUN_TEST(test_vert_source_has_position);
    RUN_TEST(test_vert_source_has_uv);
    RUN_TEST(test_vert_source_has_color);
    RUN_TEST(test_frag_source_has_output);

    /* Constants */
    RUN_TEST(test_vertex_floats_is_9);
    RUN_TEST(test_vertex_stride_is_36);
    RUN_TEST(test_max_vertices);

    return UNITY_END();
}
