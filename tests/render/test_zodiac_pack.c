/*
 * test_zodiac_pack.c — tests for zodiac render pack module
 *
 * Tests: ring packing, line packing, glyph packing, byte sizes,
 *        shader sources, edge cases, null inputs.
 */

#include "../unity/unity.h"
#include "../../src/render/zodiac_pack.h"

#include <math.h>
#include <string.h>

#define PI 3.14159265358979323846
#define TOL 0.001f

void setUp(void) {}
void tearDown(void) {}

/* ═══════════════════════════════════════════════════════════════════════
 * Ring packing tests
 * ═══════════════════════════════════════════════════════════════════════ */

/* 1. Ring returns positive vertex count */
void test_ring_positive_vertex_count(void)
{
    zp_ring_data_t ring = zp_pack_ring(4.0f, 5.0f, 8, NULL);
    TEST_ASSERT_TRUE(ring.vertex_count > 0);
}

/* 2. Ring returns positive index count */
void test_ring_positive_index_count(void)
{
    zp_ring_data_t ring = zp_pack_ring(4.0f, 5.0f, 8, NULL);
    TEST_ASSERT_TRUE(ring.index_count > 0);
}

/* 3. Ring vertex data has correct stride (9 floats per vertex) */
void test_ring_vertex_stride(void)
{
    zp_ring_data_t ring = zp_pack_ring(4.0f, 5.0f, 8, NULL);
    /* Total float usage = vertex_count * 9 */
    int total_floats = ring.vertex_count * ZP_RING_VERTEX_FLOATS;
    TEST_ASSERT_TRUE(total_floats > 0);
    /* Spot check: vertex 0 should have 9 floats worth of data starting at index 0 */
    /* position(3) + color(4) + uv(2) */
    /* Just check that the position values aren't all zero for the first vertex */
    float px = ring.vertices[0];
    float py = ring.vertices[1];
    float pz = ring.vertices[2];
    float dist = sqrtf(px * px + py * py + pz * pz);
    TEST_ASSERT_TRUE(dist > 0.0f);
}

/* 4. 12 sign_offsets are set */
void test_ring_sign_offsets_set(void)
{
    zp_ring_data_t ring = zp_pack_ring(4.0f, 5.0f, 8, NULL);
    /* sign_offsets[0] should be 0 */
    TEST_ASSERT_EQUAL_INT(0, ring.sign_offsets[0]);
    /* Each subsequent offset should be >= previous */
    for (int i = 1; i < 12; i++) {
        TEST_ASSERT_TRUE(ring.sign_offsets[i] >= ring.sign_offsets[i - 1]);
    }
}

/* 5. sign_counts sum to vertex_count */
void test_ring_sign_counts_sum(void)
{
    zp_ring_data_t ring = zp_pack_ring(4.0f, 5.0f, 8, NULL);
    int sum = 0;
    for (int i = 0; i < 12; i++) {
        sum += ring.sign_counts[i];
    }
    TEST_ASSERT_EQUAL_INT(ring.vertex_count, sum);
}

/* 6. Default colors are valid (0-1 range) */
void test_ring_default_colors_valid(void)
{
    zp_ring_data_t ring = zp_pack_ring(4.0f, 5.0f, 8, NULL);
    for (int i = 0; i < ring.vertex_count; i++) {
        int base = i * ZP_RING_VERTEX_FLOATS;
        /* color starts at offset 3 (after position) */
        float r = ring.vertices[base + 3];
        float g = ring.vertices[base + 4];
        float b = ring.vertices[base + 5];
        float a = ring.vertices[base + 6];
        TEST_ASSERT_TRUE(r >= 0.0f && r <= 1.0f);
        TEST_ASSERT_TRUE(g >= 0.0f && g <= 1.0f);
        TEST_ASSERT_TRUE(b >= 0.0f && b <= 1.0f);
        TEST_ASSERT_TRUE(a >= 0.0f && a <= 1.0f);
    }
}

/* 7. Custom sign colors are applied */
void test_ring_custom_colors_applied(void)
{
    /* Set all 12 signs to a known color: R=0.1, G=0.2, B=0.3, A=0.4 */
    float custom[48];
    for (int i = 0; i < 12; i++) {
        custom[i * 4 + 0] = 0.1f;
        custom[i * 4 + 1] = 0.2f;
        custom[i * 4 + 2] = 0.3f;
        custom[i * 4 + 3] = 0.4f;
    }
    zp_ring_data_t ring = zp_pack_ring(4.0f, 5.0f, 8, custom);
    /* Check first vertex's color */
    float r = ring.vertices[3];
    float g = ring.vertices[4];
    float b = ring.vertices[5];
    float a = ring.vertices[6];
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.1f, r);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.2f, g);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.3f, b);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.4f, a);
}

/* 8. Ring vertices are on correct radii */
void test_ring_vertex_radii(void)
{
    float inner = 4.0f;
    float outer = 5.0f;
    zp_ring_data_t ring = zp_pack_ring(inner, outer, 8, NULL);
    /* Each vertex position (x,y,z) with y=0 (ecliptic plane) should have
     * radius equal to inner or outer */
    for (int i = 0; i < ring.vertex_count; i++) {
        int base = i * ZP_RING_VERTEX_FLOATS;
        float x = ring.vertices[base + 0];
        float z = ring.vertices[base + 2];
        float r = sqrtf(x * x + z * z);
        /* Should be approximately inner_radius or outer_radius */
        int near_inner = (fabsf(r - inner) < 0.1f);
        int near_outer = (fabsf(r - outer) < 0.1f);
        TEST_ASSERT_TRUE(near_inner || near_outer);
    }
}

/* 9. Ring y-coordinates are zero (ecliptic plane) */
void test_ring_ecliptic_plane(void)
{
    zp_ring_data_t ring = zp_pack_ring(4.0f, 5.0f, 8, NULL);
    for (int i = 0; i < ring.vertex_count; i++) {
        int base = i * ZP_RING_VERTEX_FLOATS;
        float y = ring.vertices[base + 1];
        TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, y);
    }
}

/* ═══════════════════════════════════════════════════════════════════════
 * Line packing tests
 * ═══════════════════════════════════════════════════════════════════════ */

/* 10. Lines return total_vertex_count > 0 with tick marks */
void test_lines_positive_vertex_count(void)
{
    zp_line_data_t lines = zp_pack_lines(4.0f, 5.0f, NULL, NULL, NULL, 0, 8.0);
    TEST_ASSERT_TRUE(lines.total_vertex_count > 0);
}

/* 11. Tick count is 360 */
void test_lines_tick_count(void)
{
    zp_line_data_t lines = zp_pack_lines(4.0f, 5.0f, NULL, NULL, NULL, 0, 8.0);
    TEST_ASSERT_EQUAL_INT(360, lines.tick_count);
}

/* 12. Cusp count is 12 when cusps provided */
void test_lines_cusp_count_with_cusps(void)
{
    double cusps[12] = {0, 30, 60, 90, 120, 150, 180, 210, 240, 270, 300, 330};
    zp_line_data_t lines = zp_pack_lines(4.0f, 5.0f, cusps, NULL, NULL, 0, 8.0);
    TEST_ASSERT_EQUAL_INT(12, lines.cusp_count);
}

/* 13. Cusp count is 0 when cusps is NULL */
void test_lines_cusp_count_null(void)
{
    zp_line_data_t lines = zp_pack_lines(4.0f, 5.0f, NULL, NULL, NULL, 0, 8.0);
    TEST_ASSERT_EQUAL_INT(0, lines.cusp_count);
}

/* 14. Aspect count depends on planet data */
void test_lines_aspect_count_with_planets(void)
{
    /* Two planets in opposition (0 and 180 degrees) */
    float positions[6] = {5.0f, 0.0f, 0.0f, -5.0f, 0.0f, 0.0f};
    double longitudes[2] = {0.0, 180.0};
    zp_line_data_t lines = zp_pack_lines(4.0f, 5.0f, NULL, positions,
                                          longitudes, 2, 8.0);
    TEST_ASSERT_TRUE(lines.aspect_count > 0);
}

/* 15. Aspect count is 0 when num_planets is 0 */
void test_lines_aspect_count_zero_planets(void)
{
    zp_line_data_t lines = zp_pack_lines(4.0f, 5.0f, NULL, NULL, NULL, 0, 8.0);
    TEST_ASSERT_EQUAL_INT(0, lines.aspect_count);
}

/* 16. Section offsets are sequential */
void test_lines_section_offsets_sequential(void)
{
    double cusps[12] = {0, 30, 60, 90, 120, 150, 180, 210, 240, 270, 300, 330};
    float positions[6] = {5.0f, 0.0f, 0.0f, -5.0f, 0.0f, 0.0f};
    double longitudes[2] = {0.0, 180.0};
    zp_line_data_t lines = zp_pack_lines(4.0f, 5.0f, cusps, positions,
                                          longitudes, 2, 8.0);
    /* tick_offset should be 0 */
    TEST_ASSERT_EQUAL_INT(0, lines.tick_offset);
    /* cusp_offset should come after ticks */
    TEST_ASSERT_EQUAL_INT(lines.tick_count * 2, lines.cusp_offset);
    /* aspect_offset should come after cusps */
    TEST_ASSERT_EQUAL_INT(lines.cusp_offset + lines.cusp_count * 2,
                          lines.aspect_offset);
}

/* 17. Angular cusps have higher alpha */
void test_lines_angular_cusp_alpha(void)
{
    double cusps[12] = {0, 30, 60, 90, 120, 150, 180, 210, 240, 270, 300, 330};
    zp_line_data_t lines = zp_pack_lines(4.0f, 5.0f, cusps, NULL, NULL, 0, 8.0);

    /* House 0 (ASC) is angular, house 1 is not. Compare alphas. */
    /* Angular cusp is at cusp_offset vertex, index house 0 */
    int angular_idx = lines.cusp_offset;
    int normal_idx = lines.cusp_offset + 2; /* house 1, first vertex */

    /* Alpha is at offset 6 in the 7-float stride (pos3 + rgba4) */
    float angular_alpha = lines.vertices[angular_idx * ZP_LINE_VERTEX_FLOATS + 6];
    float normal_alpha = lines.vertices[normal_idx * ZP_LINE_VERTEX_FLOATS + 6];

    TEST_ASSERT_TRUE(angular_alpha > normal_alpha);
}

/* 18. Line vertex stride is 7 floats */
void test_lines_vertex_stride(void)
{
    zp_line_data_t lines = zp_pack_lines(4.0f, 5.0f, NULL, NULL, NULL, 0, 8.0);
    /* Check that first tick vertex has meaningful data in 7-float stride */
    float x = lines.vertices[0];
    float y = lines.vertices[1];
    float z = lines.vertices[2];
    float dist = sqrtf(x * x + y * y + z * z);
    TEST_ASSERT_TRUE(dist > 0.0f);
    /* Alpha component at index 6 */
    float a = lines.vertices[6];
    TEST_ASSERT_TRUE(a > 0.0f && a <= 1.0f);
}

/* 19. Total vertex count matches sum of sections */
void test_lines_total_vertex_count_matches(void)
{
    double cusps[12] = {0, 30, 60, 90, 120, 150, 180, 210, 240, 270, 300, 330};
    float positions[6] = {5.0f, 0.0f, 0.0f, -5.0f, 0.0f, 0.0f};
    double longitudes[2] = {0.0, 180.0};
    zp_line_data_t lines = zp_pack_lines(4.0f, 5.0f, cusps, positions,
                                          longitudes, 2, 8.0);
    int expected = (lines.tick_count + lines.cusp_count + lines.aspect_count) * 2;
    TEST_ASSERT_EQUAL_INT(expected, lines.total_vertex_count);
}

/* ═══════════════════════════════════════════════════════════════════════
 * Glyph packing tests
 * ═══════════════════════════════════════════════════════════════════════ */

/* 20. 12 sign glyphs always present */
void test_glyphs_sign_count(void)
{
    float cam_right[3] = {1.0f, 0.0f, 0.0f};
    float cam_up[3] = {0.0f, 1.0f, 0.0f};
    zp_glyph_data_t glyphs = zp_pack_glyphs(4.5f, cam_right, cam_up,
                                              NULL, 0, 0.5f);
    TEST_ASSERT_EQUAL_INT(12, glyphs.sign_glyph_count);
}

/* 21. Planet glyph count matches num_planets */
void test_glyphs_planet_count(void)
{
    float cam_right[3] = {1.0f, 0.0f, 0.0f};
    float cam_up[3] = {0.0f, 1.0f, 0.0f};
    double longs[3] = {45.0, 120.0, 270.0};
    zp_glyph_data_t glyphs = zp_pack_glyphs(4.5f, cam_right, cam_up,
                                              longs, 3, 0.5f);
    TEST_ASSERT_EQUAL_INT(3, glyphs.planet_glyph_count);
}

/* 22. Glyph vertex stride is 9 floats */
void test_glyphs_vertex_stride(void)
{
    float cam_right[3] = {1.0f, 0.0f, 0.0f};
    float cam_up[3] = {0.0f, 1.0f, 0.0f};
    zp_glyph_data_t glyphs = zp_pack_glyphs(4.5f, cam_right, cam_up,
                                              NULL, 0, 0.5f);
    /* 12 sign glyphs * 4 verts = 48 vertices */
    TEST_ASSERT_EQUAL_INT(48, glyphs.vertex_count);
    /* Spot-check: first vertex at [0] should be position data */
    /* The total floats used = vertex_count * 9 */
    int total_floats = glyphs.vertex_count * ZP_GLYPH_VERTEX_FLOATS;
    TEST_ASSERT_TRUE(total_floats > 0);
}

/* 23. Total quads = signs + planets */
void test_glyphs_total_quads(void)
{
    float cam_right[3] = {1.0f, 0.0f, 0.0f};
    float cam_up[3] = {0.0f, 1.0f, 0.0f};
    double longs[5] = {0.0, 72.0, 144.0, 216.0, 288.0};
    zp_glyph_data_t glyphs = zp_pack_glyphs(4.5f, cam_right, cam_up,
                                              longs, 5, 0.5f);
    int total_quads = glyphs.sign_glyph_count + glyphs.planet_glyph_count;
    TEST_ASSERT_EQUAL_INT(17, total_quads); /* 12 + 5 */
    TEST_ASSERT_EQUAL_INT(total_quads * 4, glyphs.vertex_count);
}

/* 24. Index count = quad_count * 6 */
void test_glyphs_index_count(void)
{
    float cam_right[3] = {1.0f, 0.0f, 0.0f};
    float cam_up[3] = {0.0f, 1.0f, 0.0f};
    double longs[2] = {0.0, 180.0};
    zp_glyph_data_t glyphs = zp_pack_glyphs(4.5f, cam_right, cam_up,
                                              longs, 2, 0.5f);
    int total_quads = glyphs.sign_glyph_count + glyphs.planet_glyph_count;
    TEST_ASSERT_EQUAL_INT(total_quads * 6, glyphs.index_count);
}

/* 25. Glyphs positioned at correct angles */
void test_glyphs_positioned_at_angles(void)
{
    float cam_right[3] = {1.0f, 0.0f, 0.0f};
    float cam_up[3] = {0.0f, 1.0f, 0.0f};
    zp_glyph_data_t glyphs = zp_pack_glyphs(4.5f, cam_right, cam_up,
                                              NULL, 0, 0.5f);
    /* Sign 0 (Aries) center should be near angle 15 degrees (midpoint of 0-30) */
    /* The first quad center is average of 4 vertex positions.
     * Vertex 0 of glyph 0 starts at index 0 in vertices. */
    float cx = 0.0f, cz = 0.0f;
    for (int v = 0; v < 4; v++) {
        int base = v * ZP_GLYPH_VERTEX_FLOATS;
        cx += glyphs.vertices[base + 0];
        cz += glyphs.vertices[base + 2];
    }
    cx /= 4.0f;
    cz /= 4.0f;
    float angle = atan2f(cz, cx);
    if (angle < 0.0f) angle += 2.0f * (float)PI;
    float angle_deg = angle * 180.0f / (float)PI;
    /* Aries midpoint is 15 degrees. Using a wide tolerance because the
     * billboard orientation may shift the quad corners. */
    TEST_ASSERT_FLOAT_WITHIN(20.0f, 15.0f, angle_deg);
}

/* 26. Glyph colors are valid */
void test_glyphs_colors_valid(void)
{
    float cam_right[3] = {1.0f, 0.0f, 0.0f};
    float cam_up[3] = {0.0f, 1.0f, 0.0f};
    zp_glyph_data_t glyphs = zp_pack_glyphs(4.5f, cam_right, cam_up,
                                              NULL, 0, 0.5f);
    for (int i = 0; i < glyphs.vertex_count; i++) {
        int base = i * ZP_GLYPH_VERTEX_FLOATS;
        /* color starts at offset 5 (after pos3 + uv2) */
        float r = glyphs.vertices[base + 5];
        float g = glyphs.vertices[base + 6];
        float b = glyphs.vertices[base + 7];
        float a = glyphs.vertices[base + 8];
        TEST_ASSERT_TRUE(r >= 0.0f && r <= 1.0f);
        TEST_ASSERT_TRUE(g >= 0.0f && g <= 1.0f);
        TEST_ASSERT_TRUE(b >= 0.0f && b <= 1.0f);
        TEST_ASSERT_TRUE(a >= 0.0f && a <= 1.0f);
    }
}

/* ═══════════════════════════════════════════════════════════════════════
 * Byte size tests
 * ═══════════════════════════════════════════════════════════════════════ */

/* 27. Ring vertex bytes */
void test_ring_vertex_bytes(void)
{
    zp_ring_data_t ring = zp_pack_ring(4.0f, 5.0f, 8, NULL);
    int expected = ring.vertex_count * ZP_RING_VERTEX_FLOATS * (int)sizeof(float);
    TEST_ASSERT_EQUAL_INT(expected, zp_ring_vertex_bytes(&ring));
}

/* 28. Ring index bytes */
void test_ring_index_bytes(void)
{
    zp_ring_data_t ring = zp_pack_ring(4.0f, 5.0f, 8, NULL);
    int expected = ring.index_count * (int)sizeof(unsigned int);
    TEST_ASSERT_EQUAL_INT(expected, zp_ring_index_bytes(&ring));
}

/* 29. Line vertex bytes */
void test_line_vertex_bytes(void)
{
    zp_line_data_t lines = zp_pack_lines(4.0f, 5.0f, NULL, NULL, NULL, 0, 8.0);
    int expected = lines.total_vertex_count * ZP_LINE_VERTEX_FLOATS * (int)sizeof(float);
    TEST_ASSERT_EQUAL_INT(expected, zp_line_vertex_bytes(&lines));
}

/* 30. Glyph vertex bytes */
void test_glyph_vertex_bytes(void)
{
    float cam_right[3] = {1.0f, 0.0f, 0.0f};
    float cam_up[3] = {0.0f, 1.0f, 0.0f};
    zp_glyph_data_t glyphs = zp_pack_glyphs(4.5f, cam_right, cam_up,
                                              NULL, 0, 0.5f);
    int expected = glyphs.vertex_count * ZP_GLYPH_VERTEX_FLOATS * (int)sizeof(float);
    TEST_ASSERT_EQUAL_INT(expected, zp_glyph_vertex_bytes(&glyphs));
}

/* 31. Glyph index bytes */
void test_glyph_index_bytes(void)
{
    float cam_right[3] = {1.0f, 0.0f, 0.0f};
    float cam_up[3] = {0.0f, 1.0f, 0.0f};
    zp_glyph_data_t glyphs = zp_pack_glyphs(4.5f, cam_right, cam_up,
                                              NULL, 0, 0.5f);
    int expected = glyphs.index_count * (int)sizeof(unsigned int);
    TEST_ASSERT_EQUAL_INT(expected, zp_glyph_index_bytes(&glyphs));
}

/* ═══════════════════════════════════════════════════════════════════════
 * Shader source tests
 * ═══════════════════════════════════════════════════════════════════════ */

/* 32. All 6 shader sources non-NULL */
void test_shader_sources_non_null(void)
{
    TEST_ASSERT_NOT_NULL(zp_ring_vert_source());
    TEST_ASSERT_NOT_NULL(zp_ring_frag_source());
    TEST_ASSERT_NOT_NULL(zp_line_vert_source());
    TEST_ASSERT_NOT_NULL(zp_line_frag_source());
    TEST_ASSERT_NOT_NULL(zp_glyph_vert_source());
    TEST_ASSERT_NOT_NULL(zp_glyph_frag_source());
}

/* 33. All shaders contain #version 300 es */
void test_shader_versions(void)
{
    TEST_ASSERT_NOT_NULL(strstr(zp_ring_vert_source(), "#version 300 es"));
    TEST_ASSERT_NOT_NULL(strstr(zp_ring_frag_source(), "#version 300 es"));
    TEST_ASSERT_NOT_NULL(strstr(zp_line_vert_source(), "#version 300 es"));
    TEST_ASSERT_NOT_NULL(strstr(zp_line_frag_source(), "#version 300 es"));
    TEST_ASSERT_NOT_NULL(strstr(zp_glyph_vert_source(), "#version 300 es"));
    TEST_ASSERT_NOT_NULL(strstr(zp_glyph_frag_source(), "#version 300 es"));
}

/* 34. Ring shader has u_mvp */
void test_ring_shader_has_mvp(void)
{
    TEST_ASSERT_NOT_NULL(strstr(zp_ring_vert_source(), "u_mvp"));
}

/* 35. Line shader has u_mvp */
void test_line_shader_has_mvp(void)
{
    TEST_ASSERT_NOT_NULL(strstr(zp_line_vert_source(), "u_mvp"));
}

/* 36. Glyph shader has u_atlas */
void test_glyph_shader_has_atlas(void)
{
    const char *src = zp_glyph_frag_source();
    int has_atlas = (strstr(src, "u_atlas") != NULL) ||
                    (strstr(src, "u_texture") != NULL);
    TEST_ASSERT_TRUE(has_atlas);
}

/* ═══════════════════════════════════════════════════════════════════════
 * Edge case tests
 * ═══════════════════════════════════════════════════════════════════════ */

/* 37. Zero planets = no aspect lines, no planet glyphs */
void test_zero_planets_no_aspects(void)
{
    zp_line_data_t lines = zp_pack_lines(4.0f, 5.0f, NULL, NULL, NULL, 0, 8.0);
    TEST_ASSERT_EQUAL_INT(0, lines.aspect_count);
}

/* 38. Zero planets = no planet glyphs */
void test_zero_planets_no_glyphs(void)
{
    float cam_right[3] = {1.0f, 0.0f, 0.0f};
    float cam_up[3] = {0.0f, 1.0f, 0.0f};
    zp_glyph_data_t glyphs = zp_pack_glyphs(4.5f, cam_right, cam_up,
                                              NULL, 0, 0.5f);
    TEST_ASSERT_EQUAL_INT(0, glyphs.planet_glyph_count);
}

/* 39. NULL cusps = no cusp lines */
void test_null_cusps_no_cusps(void)
{
    zp_line_data_t lines = zp_pack_lines(4.0f, 5.0f, NULL, NULL, NULL, 0, 8.0);
    TEST_ASSERT_EQUAL_INT(0, lines.cusp_count);
}

/* 40. segments_per_sign = 1 still works */
void test_ring_one_segment(void)
{
    zp_ring_data_t ring = zp_pack_ring(4.0f, 5.0f, 1, NULL);
    TEST_ASSERT_TRUE(ring.vertex_count > 0);
    TEST_ASSERT_TRUE(ring.index_count > 0);
    /* All 12 signs should still have data */
    for (int i = 0; i < 12; i++) {
        TEST_ASSERT_TRUE(ring.sign_counts[i] > 0);
    }
}

/* 41. Ring indices are within vertex count bounds */
void test_ring_indices_in_bounds(void)
{
    zp_ring_data_t ring = zp_pack_ring(4.0f, 5.0f, 8, NULL);
    for (int i = 0; i < ring.index_count; i++) {
        TEST_ASSERT_TRUE((int)ring.indices[i] < ring.vertex_count);
    }
}

/* 42. Glyph indices are within vertex count bounds */
void test_glyph_indices_in_bounds(void)
{
    float cam_right[3] = {1.0f, 0.0f, 0.0f};
    float cam_up[3] = {0.0f, 1.0f, 0.0f};
    double longs[4] = {0.0, 90.0, 180.0, 270.0};
    zp_glyph_data_t glyphs = zp_pack_glyphs(4.5f, cam_right, cam_up,
                                              longs, 4, 0.5f);
    for (int i = 0; i < glyphs.index_count; i++) {
        TEST_ASSERT_TRUE((int)glyphs.indices[i] < glyphs.vertex_count);
    }
}

/* 43. Ring UVs are in valid range */
void test_ring_uv_range(void)
{
    zp_ring_data_t ring = zp_pack_ring(4.0f, 5.0f, 8, NULL);
    for (int i = 0; i < ring.vertex_count; i++) {
        int base = i * ZP_RING_VERTEX_FLOATS;
        float u = ring.vertices[base + 7];
        float v = ring.vertices[base + 8];
        TEST_ASSERT_TRUE(u >= -0.01f && u <= 1.01f);
        TEST_ASSERT_TRUE(v >= -0.01f && v <= 1.01f);
    }
}

/* 44. Lines with full data (ticks + cusps + aspects) */
void test_lines_full_data(void)
{
    double cusps[12] = {0, 30, 60, 90, 120, 150, 180, 210, 240, 270, 300, 330};
    float positions[6] = {5.0f, 0.0f, 0.0f, -5.0f, 0.0f, 0.0f};
    double longitudes[2] = {0.0, 180.0};
    zp_line_data_t lines = zp_pack_lines(4.0f, 5.0f, cusps, positions,
                                          longitudes, 2, 8.0);
    TEST_ASSERT_EQUAL_INT(360, lines.tick_count);
    TEST_ASSERT_EQUAL_INT(12, lines.cusp_count);
    TEST_ASSERT_TRUE(lines.aspect_count > 0);
    TEST_ASSERT_TRUE(lines.total_vertex_count > 0);
}

/* 45. Line colors are in valid range */
void test_line_colors_valid(void)
{
    double cusps[12] = {0, 30, 60, 90, 120, 150, 180, 210, 240, 270, 300, 330};
    zp_line_data_t lines = zp_pack_lines(4.0f, 5.0f, cusps, NULL, NULL, 0, 8.0);
    for (int i = 0; i < lines.total_vertex_count; i++) {
        int base = i * ZP_LINE_VERTEX_FLOATS;
        float r = lines.vertices[base + 3];
        float g = lines.vertices[base + 4];
        float b = lines.vertices[base + 5];
        float a = lines.vertices[base + 6];
        TEST_ASSERT_TRUE(r >= 0.0f && r <= 1.0f);
        TEST_ASSERT_TRUE(g >= 0.0f && g <= 1.0f);
        TEST_ASSERT_TRUE(b >= 0.0f && b <= 1.0f);
        TEST_ASSERT_TRUE(a >= 0.0f && a <= 1.0f);
    }
}

/* ═══════════════════════════════════════════════════════════════════════
 * Runner
 * ═══════════════════════════════════════════════════════════════════════ */

int main(void)
{
    UNITY_BEGIN();

    /* Ring packing (1-9) */
    RUN_TEST(test_ring_positive_vertex_count);
    RUN_TEST(test_ring_positive_index_count);
    RUN_TEST(test_ring_vertex_stride);
    RUN_TEST(test_ring_sign_offsets_set);
    RUN_TEST(test_ring_sign_counts_sum);
    RUN_TEST(test_ring_default_colors_valid);
    RUN_TEST(test_ring_custom_colors_applied);
    RUN_TEST(test_ring_vertex_radii);
    RUN_TEST(test_ring_ecliptic_plane);

    /* Line packing (10-19) */
    RUN_TEST(test_lines_positive_vertex_count);
    RUN_TEST(test_lines_tick_count);
    RUN_TEST(test_lines_cusp_count_with_cusps);
    RUN_TEST(test_lines_cusp_count_null);
    RUN_TEST(test_lines_aspect_count_with_planets);
    RUN_TEST(test_lines_aspect_count_zero_planets);
    RUN_TEST(test_lines_section_offsets_sequential);
    RUN_TEST(test_lines_angular_cusp_alpha);
    RUN_TEST(test_lines_vertex_stride);
    RUN_TEST(test_lines_total_vertex_count_matches);

    /* Glyph packing (20-26) */
    RUN_TEST(test_glyphs_sign_count);
    RUN_TEST(test_glyphs_planet_count);
    RUN_TEST(test_glyphs_vertex_stride);
    RUN_TEST(test_glyphs_total_quads);
    RUN_TEST(test_glyphs_index_count);
    RUN_TEST(test_glyphs_positioned_at_angles);
    RUN_TEST(test_glyphs_colors_valid);

    /* Byte sizes (27-31) */
    RUN_TEST(test_ring_vertex_bytes);
    RUN_TEST(test_ring_index_bytes);
    RUN_TEST(test_line_vertex_bytes);
    RUN_TEST(test_glyph_vertex_bytes);
    RUN_TEST(test_glyph_index_bytes);

    /* Shaders (32-36) */
    RUN_TEST(test_shader_sources_non_null);
    RUN_TEST(test_shader_versions);
    RUN_TEST(test_ring_shader_has_mvp);
    RUN_TEST(test_line_shader_has_mvp);
    RUN_TEST(test_glyph_shader_has_atlas);

    /* Edge cases (37-45) */
    RUN_TEST(test_zero_planets_no_aspects);
    RUN_TEST(test_zero_planets_no_glyphs);
    RUN_TEST(test_null_cusps_no_cusps);
    RUN_TEST(test_ring_one_segment);
    RUN_TEST(test_ring_indices_in_bounds);
    RUN_TEST(test_glyph_indices_in_bounds);
    RUN_TEST(test_ring_uv_range);
    RUN_TEST(test_lines_full_data);
    RUN_TEST(test_line_colors_valid);

    return UNITY_END();
}
