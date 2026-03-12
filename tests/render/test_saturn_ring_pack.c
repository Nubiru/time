/*
 * test_saturn_ring_pack.c — Tests for Saturn ring render pack module
 *
 * Tests: ring data accuracy (NASA/JPL), mesh generation, gap detection,
 * opacity/color interpolation, scene radius conversion, shader sources.
 *
 * 55 tests covering all public API functions.
 */

#include "../unity/unity.h"
#include "../../src/render/saturn_ring_pack.h"

#include <math.h>
#include <string.h>

#ifndef PI
#define PI 3.14159265358979323846
#endif

#define FTOL 0.001f
#define FCLOSE(exp, act) TEST_ASSERT_FLOAT_WITHIN(FTOL, (exp), (act))

void setUp(void) {}
void tearDown(void) {}

/* ======================================================================
 * 1. srp_default_config
 * ====================================================================== */

/* 1. Default config has positive saturn_scene_radius */
void test_default_config_scene_radius(void)
{
    srp_config_t cfg = srp_default_config();
    TEST_ASSERT_TRUE(cfg.saturn_scene_radius > 0.0f);
}

/* 2. Default config axial tilt is 26.73 degrees */
void test_default_config_axial_tilt(void)
{
    srp_config_t cfg = srp_default_config();
    FCLOSE(26.73f, cfg.axial_tilt_deg);
}

/* 3. Default config radial_segments >= 4 */
void test_default_config_radial_segments(void)
{
    srp_config_t cfg = srp_default_config();
    TEST_ASSERT_TRUE(cfg.radial_segments >= 4);
}

/* 4. Default config angular_segments >= 8 */
void test_default_config_angular_segments(void)
{
    srp_config_t cfg = srp_default_config();
    TEST_ASSERT_TRUE(cfg.angular_segments >= 8);
}

/* ======================================================================
 * 2. srp_ring_count / srp_ring_get — ring data
 * ====================================================================== */

/* 5. Ring count is 7 */
void test_ring_count_is_7(void)
{
    TEST_ASSERT_EQUAL_INT(7, srp_ring_count());
}

/* 6. Every ring has a non-null name */
void test_rings_have_names(void)
{
    for (int i = 0; i < srp_ring_count(); i++) {
        srp_ring_t r = srp_ring_get(i);
        TEST_ASSERT_NOT_NULL(r.name);
    }
}

/* 7. Every ring has inner < outer radius */
void test_rings_inner_less_than_outer(void)
{
    for (int i = 0; i < srp_ring_count(); i++) {
        srp_ring_t r = srp_ring_get(i);
        TEST_ASSERT_TRUE(r.inner_radius < r.outer_radius);
    }
}

/* 8. Every ring opacity in [0, 1] */
void test_rings_opacity_range(void)
{
    for (int i = 0; i < srp_ring_count(); i++) {
        srp_ring_t r = srp_ring_get(i);
        TEST_ASSERT_TRUE(r.opacity >= 0.0f && r.opacity <= 1.0f);
    }
}

/* 9. Every ring albedo in [0, 1] */
void test_rings_albedo_range(void)
{
    for (int i = 0; i < srp_ring_count(); i++) {
        srp_ring_t r = srp_ring_get(i);
        TEST_ASSERT_TRUE(r.albedo >= 0.0f && r.albedo <= 1.0f);
    }
}

/* 10. Ring radii are generally ordered: each ring's inner >= previous ring's
 * outer, EXCEPT the Encke Gap which is physically embedded within the A ring. */
void test_rings_radii_ordered(void)
{
    for (int i = 1; i < srp_ring_count(); i++) {
        srp_ring_t prev = srp_ring_get(i - 1);
        srp_ring_t curr = srp_ring_get(i);
        /* Skip Encke Gap (index 5) — it's a narrow gap inside the A ring */
        if (i == srp_find_ring("Encke Gap")) continue;
        /* Skip F ring vs Encke Gap comparison — F ring follows A ring */
        if (i == srp_find_ring("F ring")) {
            srp_ring_t a_ring = srp_ring_get(srp_find_ring("A ring"));
            TEST_ASSERT_TRUE(curr.inner_radius >= a_ring.outer_radius - 0.01f);
            continue;
        }
        TEST_ASSERT_TRUE(curr.inner_radius >= prev.outer_radius - 0.01f);
    }
}

/* 11. Specific named ring: "D ring" exists */
void test_ring_d_exists(void)
{
    int idx = srp_find_ring("D ring");
    TEST_ASSERT_TRUE(idx >= 0);
}

/* 12. Specific named ring: "C ring" exists */
void test_ring_c_exists(void)
{
    int idx = srp_find_ring("C ring");
    TEST_ASSERT_TRUE(idx >= 0);
}

/* 13. Specific named ring: "B ring" exists */
void test_ring_b_exists(void)
{
    int idx = srp_find_ring("B ring");
    TEST_ASSERT_TRUE(idx >= 0);
}

/* 14. Specific named ring: "Cassini Division" exists */
void test_ring_cassini_exists(void)
{
    int idx = srp_find_ring("Cassini Division");
    TEST_ASSERT_TRUE(idx >= 0);
}

/* 15. Specific named ring: "A ring" exists */
void test_ring_a_exists(void)
{
    int idx = srp_find_ring("A ring");
    TEST_ASSERT_TRUE(idx >= 0);
}

/* 16. Specific named ring: "Encke Gap" exists */
void test_ring_encke_exists(void)
{
    int idx = srp_find_ring("Encke Gap");
    TEST_ASSERT_TRUE(idx >= 0);
}

/* 17. Specific named ring: "F ring" exists */
void test_ring_f_exists(void)
{
    int idx = srp_find_ring("F ring");
    TEST_ASSERT_TRUE(idx >= 0);
}

/* 18. srp_find_ring returns -1 for nonexistent */
void test_find_ring_nonexistent(void)
{
    TEST_ASSERT_EQUAL_INT(-1, srp_find_ring("nonexistent"));
}

/* 19. srp_find_ring returns -1 for NULL */
void test_find_ring_null(void)
{
    TEST_ASSERT_EQUAL_INT(-1, srp_find_ring(NULL));
}

/* 20. B ring has highest opacity among all rings */
void test_b_ring_highest_opacity(void)
{
    int b_idx = srp_find_ring("B ring");
    srp_ring_t b = srp_ring_get(b_idx);
    for (int i = 0; i < srp_ring_count(); i++) {
        if (i == b_idx) continue;
        srp_ring_t r = srp_ring_get(i);
        TEST_ASSERT_TRUE(b.opacity >= r.opacity);
    }
}

/* 21. Cassini Division has low opacity (< 0.1) */
void test_cassini_low_opacity(void)
{
    int idx = srp_find_ring("Cassini Division");
    srp_ring_t r = srp_ring_get(idx);
    TEST_ASSERT_TRUE(r.opacity < 0.1f);
}

/* 22. Encke Gap has low opacity (< 0.1) */
void test_encke_low_opacity(void)
{
    int idx = srp_find_ring("Encke Gap");
    srp_ring_t r = srp_ring_get(idx);
    TEST_ASSERT_TRUE(r.opacity < 0.1f);
}

/* 23. Out-of-bounds ring get returns sentinel (name = NULL) */
void test_ring_get_out_of_bounds(void)
{
    srp_ring_t r = srp_ring_get(-1);
    TEST_ASSERT_NULL(r.name);
}

/* 24. Out-of-bounds ring get returns sentinel for index >= count */
void test_ring_get_above_bounds(void)
{
    srp_ring_t r = srp_ring_get(srp_ring_count());
    TEST_ASSERT_NULL(r.name);
}

/* ======================================================================
 * 3. srp_to_scene_radius
 * ====================================================================== */

/* 25. Scene radius conversion: 2.0 Rs * 5.0 scene = 10.0 */
void test_to_scene_radius_basic(void)
{
    float result = srp_to_scene_radius(2.0f, 5.0f);
    FCLOSE(10.0f, result);
}

/* 26. Scene radius conversion: 1.0 Rs * 1.0 scene = 1.0 */
void test_to_scene_radius_identity(void)
{
    float result = srp_to_scene_radius(1.0f, 1.0f);
    FCLOSE(1.0f, result);
}

/* 27. Scene radius conversion: 0.0 Rs = 0.0 */
void test_to_scene_radius_zero(void)
{
    float result = srp_to_scene_radius(0.0f, 5.0f);
    FCLOSE(0.0f, result);
}

/* ======================================================================
 * 4. srp_opacity_at_radius
 * ====================================================================== */

/* 28. Opacity in B ring range is high (>= 0.5) */
void test_opacity_in_b_ring(void)
{
    /* B ring: 1.527 - 1.951 Rs, opacity = 0.80 */
    float op = srp_opacity_at_radius(1.75f);
    TEST_ASSERT_TRUE(op >= 0.5f);
}

/* 29. Opacity in Cassini Division is low (< 0.1) */
void test_opacity_in_cassini(void)
{
    /* Cassini: 1.951 - 2.025 Rs */
    float op = srp_opacity_at_radius(1.99f);
    TEST_ASSERT_TRUE(op < 0.1f);
}

/* 30. Opacity outside all rings is 0 */
void test_opacity_outside_rings(void)
{
    float op = srp_opacity_at_radius(0.5f);
    FCLOSE(0.0f, op);
}

/* 31. Opacity beyond F ring is 0 */
void test_opacity_beyond_f_ring(void)
{
    float op = srp_opacity_at_radius(3.0f);
    FCLOSE(0.0f, op);
}

/* 32. Opacity in A ring range is moderate (0.3-0.7) */
void test_opacity_in_a_ring(void)
{
    /* A ring: 2.025 - 2.269 Rs, opacity = 0.50 */
    float op = srp_opacity_at_radius(2.15f);
    TEST_ASSERT_TRUE(op >= 0.3f && op <= 0.7f);
}

/* 32b. Opacity at Encke Gap center via interpolation returns gap opacity, not A ring */
void test_opacity_encke_gap_via_interpolation(void)
{
    /* Encke Gap: 2.214 - 2.222 Rs, opacity = 0.01
     * The Encke Gap is physically inside the A ring (2.025-2.269).
     * srp_opacity_at_radius must prefer the narrower band. */
    float op = srp_opacity_at_radius(2.218f);
    TEST_ASSERT_TRUE(op < 0.1f);
}

/* ======================================================================
 * 5. srp_color_at_radius
 * ====================================================================== */

/* 33. Color in B ring range: valid RGB [0,1] */
void test_color_b_ring_valid(void)
{
    float rgb[3];
    srp_color_at_radius(1.75f, rgb);
    for (int i = 0; i < 3; i++) {
        TEST_ASSERT_TRUE(rgb[i] >= 0.0f && rgb[i] <= 1.0f);
    }
}

/* 34. Color in C ring range: valid RGB [0,1] */
void test_color_c_ring_valid(void)
{
    float rgb[3];
    srp_color_at_radius(1.4f, rgb);
    for (int i = 0; i < 3; i++) {
        TEST_ASSERT_TRUE(rgb[i] >= 0.0f && rgb[i] <= 1.0f);
    }
}

/* 35. B ring is brighter than C ring (higher luminance) */
void test_b_ring_brighter_than_c(void)
{
    float b_rgb[3], c_rgb[3];
    srp_color_at_radius(1.75f, b_rgb);  /* B ring center */
    srp_color_at_radius(1.4f, c_rgb);   /* C ring center */
    float b_lum = 0.299f * b_rgb[0] + 0.587f * b_rgb[1] + 0.114f * b_rgb[2];
    float c_lum = 0.299f * c_rgb[0] + 0.587f * c_rgb[1] + 0.114f * c_rgb[2];
    TEST_ASSERT_TRUE(b_lum > c_lum);
}

/* 36. Color outside rings returns a default (zeros or neutral) */
void test_color_outside_rings(void)
{
    float rgb[3];
    srp_color_at_radius(0.5f, rgb);
    /* Outside rings: should get zeros */
    for (int i = 0; i < 3; i++) {
        FCLOSE(0.0f, rgb[i]);
    }
}

/* ======================================================================
 * 6. srp_is_gap
 * ====================================================================== */

/* 37. Cassini Division center is a gap */
void test_is_gap_cassini(void)
{
    /* Cassini: 1.951 - 2.025 Rs */
    TEST_ASSERT_EQUAL_INT(1, srp_is_gap(1.99f));
}

/* 38. Encke Gap center is a gap */
void test_is_gap_encke(void)
{
    /* Encke: 2.214 - 2.222 Rs */
    TEST_ASSERT_EQUAL_INT(1, srp_is_gap(2.218f));
}

/* 39. B ring center is not a gap */
void test_not_gap_b_ring(void)
{
    TEST_ASSERT_EQUAL_INT(0, srp_is_gap(1.75f));
}

/* 40. Outside all rings is not a gap */
void test_not_gap_outside(void)
{
    TEST_ASSERT_EQUAL_INT(0, srp_is_gap(0.5f));
}

/* ======================================================================
 * 7. srp_pack — mesh generation
 * ====================================================================== */

/* 41. Pack with default config: vertex count > 0 */
void test_pack_vertex_count_positive(void)
{
    srp_config_t cfg = srp_default_config();
    float verts[SRP_MAX_VERTICES * SRP_VERTEX_FLOATS];
    unsigned short indices[SRP_MAX_VERTICES * 6];

    srp_info_t info = srp_pack(cfg, verts, indices);
    TEST_ASSERT_TRUE(info.vertex_count > 0);
}

/* 42. Pack with default config: index count > 0 */
void test_pack_index_count_positive(void)
{
    srp_config_t cfg = srp_default_config();
    float verts[SRP_MAX_VERTICES * SRP_VERTEX_FLOATS];
    unsigned short indices[SRP_MAX_VERTICES * 6];

    srp_info_t info = srp_pack(cfg, verts, indices);
    TEST_ASSERT_TRUE(info.index_count > 0);
}

/* 43. Pack with default config: triangle count > 0 */
void test_pack_triangle_count_positive(void)
{
    srp_config_t cfg = srp_default_config();
    float verts[SRP_MAX_VERTICES * SRP_VERTEX_FLOATS];
    unsigned short indices[SRP_MAX_VERTICES * 6];

    srp_info_t info = srp_pack(cfg, verts, indices);
    TEST_ASSERT_TRUE(info.triangle_count > 0);
}

/* 44. Pack ring_count matches srp_ring_count() */
void test_pack_ring_count(void)
{
    srp_config_t cfg = srp_default_config();
    float verts[SRP_MAX_VERTICES * SRP_VERTEX_FLOATS];
    unsigned short indices[SRP_MAX_VERTICES * 6];

    srp_info_t info = srp_pack(cfg, verts, indices);
    TEST_ASSERT_EQUAL_INT(srp_ring_count(), info.ring_count);
}

/* 45. All indices < vertex_count */
void test_pack_indices_in_bounds(void)
{
    srp_config_t cfg = srp_default_config();
    float verts[SRP_MAX_VERTICES * SRP_VERTEX_FLOATS];
    unsigned short indices[SRP_MAX_VERTICES * 6];

    srp_info_t info = srp_pack(cfg, verts, indices);
    for (int i = 0; i < info.index_count; i++) {
        TEST_ASSERT_TRUE(indices[i] < (unsigned short)info.vertex_count);
    }
}

/* 46. Packed vertices: positions on tilted disc (y != 0 for default tilt) */
void test_pack_tilted_positions(void)
{
    srp_config_t cfg = srp_default_config();
    float verts[SRP_MAX_VERTICES * SRP_VERTEX_FLOATS];
    unsigned short indices[SRP_MAX_VERTICES * 6];

    srp_info_t info = srp_pack(cfg, verts, indices);

    /* At least some vertices should have non-zero y due to tilt */
    int has_nonzero_y = 0;
    for (int i = 0; i < info.vertex_count; i++) {
        float y = verts[i * SRP_VERTEX_FLOATS + 1];
        if (fabsf(y) > 0.001f) {
            has_nonzero_y = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(has_nonzero_y);
}

/* 47. Pack with zero tilt: all y = 0 */
void test_pack_zero_tilt_y_zero(void)
{
    srp_config_t cfg = srp_default_config();
    cfg.axial_tilt_deg = 0.0f;
    float verts[SRP_MAX_VERTICES * SRP_VERTEX_FLOATS];
    unsigned short indices[SRP_MAX_VERTICES * 6];

    srp_info_t info = srp_pack(cfg, verts, indices);

    for (int i = 0; i < info.vertex_count; i++) {
        float y = verts[i * SRP_VERTEX_FLOATS + 1];
        FCLOSE(0.0f, y);
    }
}

/* 48. Packed normals are consistent (all similar for flat disc) */
void test_pack_normals_consistent(void)
{
    srp_config_t cfg = srp_default_config();
    float verts[SRP_MAX_VERTICES * SRP_VERTEX_FLOATS];
    unsigned short indices[SRP_MAX_VERTICES * 6];

    srp_info_t info = srp_pack(cfg, verts, indices);

    /* All normals should be the same (tilted disc plane normal) */
    float nx0 = verts[3];
    float ny0 = verts[4];
    float nz0 = verts[5];
    for (int i = 1; i < info.vertex_count; i++) {
        int base = i * SRP_VERTEX_FLOATS;
        FCLOSE(nx0, verts[base + 3]);
        FCLOSE(ny0, verts[base + 4]);
        FCLOSE(nz0, verts[base + 5]);
    }
}

/* 49. Texcoords in [0, 1] */
void test_pack_texcoords_range(void)
{
    srp_config_t cfg = srp_default_config();
    float verts[SRP_MAX_VERTICES * SRP_VERTEX_FLOATS];
    unsigned short indices[SRP_MAX_VERTICES * 6];

    srp_info_t info = srp_pack(cfg, verts, indices);

    for (int i = 0; i < info.vertex_count; i++) {
        int base = i * SRP_VERTEX_FLOATS;
        float u = verts[base + 6];
        float v = verts[base + 7];
        TEST_ASSERT_TRUE(u >= -0.001f && u <= 1.001f);
        TEST_ASSERT_TRUE(v >= -0.001f && v <= 1.001f);
    }
}

/* 50. Pack with fewer segments: counts decrease */
void test_pack_fewer_segments_fewer_verts(void)
{
    srp_config_t cfg1 = srp_default_config();
    srp_config_t cfg2 = srp_default_config();
    cfg2.radial_segments = 4;
    cfg2.angular_segments = 12;

    float verts1[SRP_MAX_VERTICES * SRP_VERTEX_FLOATS];
    unsigned short indices1[SRP_MAX_VERTICES * 6];
    float verts2[SRP_MAX_VERTICES * SRP_VERTEX_FLOATS];
    unsigned short indices2[SRP_MAX_VERTICES * 6];

    srp_info_t info1 = srp_pack(cfg1, verts1, indices1);
    srp_info_t info2 = srp_pack(cfg2, verts2, indices2);

    TEST_ASSERT_TRUE(info2.vertex_count < info1.vertex_count);
}

/* 51. Index count = triangle_count * 3 */
void test_pack_index_triangle_relation(void)
{
    srp_config_t cfg = srp_default_config();
    float verts[SRP_MAX_VERTICES * SRP_VERTEX_FLOATS];
    unsigned short indices[SRP_MAX_VERTICES * 6];

    srp_info_t info = srp_pack(cfg, verts, indices);
    TEST_ASSERT_EQUAL_INT(info.triangle_count * 3, info.index_count);
}

/* ======================================================================
 * 8. Shader sources
 * ====================================================================== */

/* 52. Vertex shader source is non-null and non-empty */
void test_vert_source_exists(void)
{
    const char *src = srp_vert_source();
    TEST_ASSERT_NOT_NULL(src);
    TEST_ASSERT_TRUE(strlen(src) > 10);
}

/* 53. Fragment shader source is non-null and non-empty */
void test_frag_source_exists(void)
{
    const char *src = srp_frag_source();
    TEST_ASSERT_NOT_NULL(src);
    TEST_ASSERT_TRUE(strlen(src) > 10);
}

/* 54. Vert shader contains u_mvp */
void test_vert_has_u_mvp(void)
{
    TEST_ASSERT_NOT_NULL(strstr(srp_vert_source(), "u_mvp"));
}

/* 55. Frag shader contains u_sun_dir */
void test_frag_has_u_sun_dir(void)
{
    TEST_ASSERT_NOT_NULL(strstr(srp_frag_source(), "u_sun_dir"));
}

/* 56. Frag shader contains u_saturn_pos */
void test_frag_has_u_saturn_pos(void)
{
    TEST_ASSERT_NOT_NULL(strstr(srp_frag_source(), "u_saturn_pos"));
}

/* 57. Both shaders contain version 300 es */
void test_shaders_version(void)
{
    TEST_ASSERT_NOT_NULL(strstr(srp_vert_source(), "#version 300 es"));
    TEST_ASSERT_NOT_NULL(strstr(srp_frag_source(), "#version 300 es"));
}

/* 58. Both shaders contain void main() */
void test_shaders_have_main(void)
{
    TEST_ASSERT_NOT_NULL(strstr(srp_vert_source(), "void main()"));
    TEST_ASSERT_NOT_NULL(strstr(srp_frag_source(), "void main()"));
}

/* 59. Vert shader contains u_model */
void test_vert_has_u_model(void)
{
    TEST_ASSERT_NOT_NULL(strstr(srp_vert_source(), "u_model"));
}

/* 60. Frag shader contains discard (for transparent pixels) */
void test_frag_has_discard(void)
{
    TEST_ASSERT_NOT_NULL(strstr(srp_frag_source(), "discard"));
}

/* ======================================================================
 * Runner
 * ====================================================================== */

int main(void)
{
    UNITY_BEGIN();

    /* Default config (4 tests) */
    RUN_TEST(test_default_config_scene_radius);
    RUN_TEST(test_default_config_axial_tilt);
    RUN_TEST(test_default_config_radial_segments);
    RUN_TEST(test_default_config_angular_segments);

    /* Ring data (20 tests) */
    RUN_TEST(test_ring_count_is_7);
    RUN_TEST(test_rings_have_names);
    RUN_TEST(test_rings_inner_less_than_outer);
    RUN_TEST(test_rings_opacity_range);
    RUN_TEST(test_rings_albedo_range);
    RUN_TEST(test_rings_radii_ordered);
    RUN_TEST(test_ring_d_exists);
    RUN_TEST(test_ring_c_exists);
    RUN_TEST(test_ring_b_exists);
    RUN_TEST(test_ring_cassini_exists);
    RUN_TEST(test_ring_a_exists);
    RUN_TEST(test_ring_encke_exists);
    RUN_TEST(test_ring_f_exists);
    RUN_TEST(test_find_ring_nonexistent);
    RUN_TEST(test_find_ring_null);
    RUN_TEST(test_b_ring_highest_opacity);
    RUN_TEST(test_cassini_low_opacity);
    RUN_TEST(test_encke_low_opacity);
    RUN_TEST(test_ring_get_out_of_bounds);
    RUN_TEST(test_ring_get_above_bounds);

    /* Scene radius (3 tests) */
    RUN_TEST(test_to_scene_radius_basic);
    RUN_TEST(test_to_scene_radius_identity);
    RUN_TEST(test_to_scene_radius_zero);

    /* Opacity at radius (5 tests) */
    RUN_TEST(test_opacity_in_b_ring);
    RUN_TEST(test_opacity_in_cassini);
    RUN_TEST(test_opacity_outside_rings);
    RUN_TEST(test_opacity_beyond_f_ring);
    RUN_TEST(test_opacity_in_a_ring);
    RUN_TEST(test_opacity_encke_gap_via_interpolation);

    /* Color at radius (4 tests) */
    RUN_TEST(test_color_b_ring_valid);
    RUN_TEST(test_color_c_ring_valid);
    RUN_TEST(test_b_ring_brighter_than_c);
    RUN_TEST(test_color_outside_rings);

    /* Gap detection (4 tests) */
    RUN_TEST(test_is_gap_cassini);
    RUN_TEST(test_is_gap_encke);
    RUN_TEST(test_not_gap_b_ring);
    RUN_TEST(test_not_gap_outside);

    /* Mesh packing (11 tests) */
    RUN_TEST(test_pack_vertex_count_positive);
    RUN_TEST(test_pack_index_count_positive);
    RUN_TEST(test_pack_triangle_count_positive);
    RUN_TEST(test_pack_ring_count);
    RUN_TEST(test_pack_indices_in_bounds);
    RUN_TEST(test_pack_tilted_positions);
    RUN_TEST(test_pack_zero_tilt_y_zero);
    RUN_TEST(test_pack_normals_consistent);
    RUN_TEST(test_pack_texcoords_range);
    RUN_TEST(test_pack_fewer_segments_fewer_verts);
    RUN_TEST(test_pack_index_triangle_relation);

    /* Shaders (9 tests) */
    RUN_TEST(test_vert_source_exists);
    RUN_TEST(test_frag_source_exists);
    RUN_TEST(test_vert_has_u_mvp);
    RUN_TEST(test_frag_has_u_sun_dir);
    RUN_TEST(test_frag_has_u_saturn_pos);
    RUN_TEST(test_shaders_version);
    RUN_TEST(test_shaders_have_main);
    RUN_TEST(test_vert_has_u_model);
    RUN_TEST(test_frag_has_discard);

    return UNITY_END();
}
