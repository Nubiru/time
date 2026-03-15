/*
 * test_billboard_shader.c — Tests for GPU billboard shader module
 *
 * Tests: shader source strings, quad geometry packing, NULL safety,
 *        CCW winding, UV mapping, static pointer stability.
 */

#include "../unity/unity.h"
#include "../../src/render/billboard_shader.h"

#include <string.h>
#include <math.h>

void setUp(void) {}
void tearDown(void) {}

/* ═══════════════════════════════════════════════════════════════════════
 * 1. Vertex shader source
 * ═══════════════════════════════════════════════════════════════════════ */

void test_vert_source_non_null(void)
{
    const char *src = billboard_shader_vert_source();
    TEST_ASSERT_NOT_NULL(src);
    TEST_ASSERT_TRUE(strlen(src) > 0);
}

void test_vert_source_contains_version(void)
{
    const char *src = billboard_shader_vert_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "#version 300 es"));
}

void test_vert_source_contains_gl_position(void)
{
    const char *src = billboard_shader_vert_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "gl_Position"));
}

void test_vert_source_contains_view_proj_uniforms(void)
{
    const char *src = billboard_shader_vert_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "u_view"));
    TEST_ASSERT_NOT_NULL(strstr(src, "u_proj"));
}

void test_vert_source_contains_attributes(void)
{
    const char *src = billboard_shader_vert_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "a_center"));
    TEST_ASSERT_NOT_NULL(strstr(src, "a_offset"));
    TEST_ASSERT_NOT_NULL(strstr(src, "a_size"));
}

void test_vert_source_extracts_camera_axes(void)
{
    const char *src = billboard_shader_vert_source();
    /* Should use view matrix columns for right/up extraction */
    TEST_ASSERT_NOT_NULL(strstr(src, "u_view[0][0]"));
    TEST_ASSERT_NOT_NULL(strstr(src, "u_view[1][0]"));
    TEST_ASSERT_NOT_NULL(strstr(src, "u_view[0][1]"));
    TEST_ASSERT_NOT_NULL(strstr(src, "u_view[1][1]"));
}

/* ═══════════════════════════════════════════════════════════════════════
 * 2. Fragment shader source
 * ═══════════════════════════════════════════════════════════════════════ */

void test_frag_source_non_null(void)
{
    const char *src = billboard_shader_frag_source();
    TEST_ASSERT_NOT_NULL(src);
    TEST_ASSERT_TRUE(strlen(src) > 0);
}

void test_frag_source_contains_version(void)
{
    const char *src = billboard_shader_frag_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "#version 300 es"));
}

void test_frag_source_contains_uniforms(void)
{
    const char *src = billboard_shader_frag_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "u_texture"));
    TEST_ASSERT_NOT_NULL(strstr(src, "u_color"));
}

/* ═══════════════════════════════════════════════════════════════════════
 * 3. pack_offsets
 * ═══════════════════════════════════════════════════════════════════════ */

void test_pack_offsets_returns_4(void)
{
    float buf[8];
    int n = billboard_shader_pack_offsets(buf);
    TEST_ASSERT_EQUAL_INT(4, n);
}

void test_pack_offsets_correct_values(void)
{
    float buf[8];
    billboard_shader_pack_offsets(buf);

    /* BL: (-1,-1) */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, -1.0f, buf[0]);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, -1.0f, buf[1]);
    /* BR: (1,-1) */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, buf[2]);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, -1.0f, buf[3]);
    /* TR: (1,1) */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, buf[4]);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, buf[5]);
    /* TL: (-1,1) */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, -1.0f, buf[6]);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, buf[7]);
}

void test_pack_offsets_null_returns_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, billboard_shader_pack_offsets(NULL));
}

void test_offsets_are_unit_quad(void)
{
    float buf[8];
    billboard_shader_pack_offsets(buf);

    /* All values should be -1 or 1 */
    for (int i = 0; i < 8; i++) {
        TEST_ASSERT_TRUE(fabsf(buf[i]) == 1.0f);
    }
}

/* ═══════════════════════════════════════════════════════════════════════
 * 4. pack_uvs
 * ═══════════════════════════════════════════════════════════════════════ */

void test_pack_uvs_returns_4(void)
{
    float buf[8];
    int n = billboard_shader_pack_uvs(buf);
    TEST_ASSERT_EQUAL_INT(4, n);
}

void test_pack_uvs_correct_values(void)
{
    float buf[8];
    billboard_shader_pack_uvs(buf);

    /* BL: (0,0), BR: (1,0), TR: (1,1), TL: (0,1) */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, buf[0]);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, buf[1]);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, buf[2]);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, buf[3]);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, buf[4]);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, buf[5]);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, buf[6]);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, buf[7]);
}

void test_pack_uvs_null_returns_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, billboard_shader_pack_uvs(NULL));
}

void test_uvs_map_0_to_1(void)
{
    float buf[8];
    billboard_shader_pack_uvs(buf);

    for (int i = 0; i < 8; i++) {
        TEST_ASSERT_TRUE(buf[i] >= 0.0f && buf[i] <= 1.0f);
    }
}

/* ═══════════════════════════════════════════════════════════════════════
 * 5. pack_indices
 * ═══════════════════════════════════════════════════════════════════════ */

void test_pack_indices_returns_6(void)
{
    unsigned int buf[6];
    int n = billboard_shader_pack_indices(buf);
    TEST_ASSERT_EQUAL_INT(6, n);
}

void test_pack_indices_null_returns_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, billboard_shader_pack_indices(NULL));
}

void test_indices_form_two_triangles_ccw(void)
{
    unsigned int idx[6];
    billboard_shader_pack_indices(idx);

    /* Triangle 1: BL(0), BR(1), TR(2) — CCW */
    TEST_ASSERT_EQUAL_UINT(0, idx[0]);
    TEST_ASSERT_EQUAL_UINT(1, idx[1]);
    TEST_ASSERT_EQUAL_UINT(2, idx[2]);

    /* Triangle 2: BL(0), TR(2), TL(3) — CCW */
    TEST_ASSERT_EQUAL_UINT(0, idx[3]);
    TEST_ASSERT_EQUAL_UINT(2, idx[4]);
    TEST_ASSERT_EQUAL_UINT(3, idx[5]);
}

/* ═══════════════════════════════════════════════════════════════════════
 * 6. Static pointer stability
 * ═══════════════════════════════════════════════════════════════════════ */

void test_shader_sources_same_pointer(void)
{
    const char *v1 = billboard_shader_vert_source();
    const char *v2 = billboard_shader_vert_source();
    TEST_ASSERT_EQUAL_PTR(v1, v2);

    const char *f1 = billboard_shader_frag_source();
    const char *f2 = billboard_shader_frag_source();
    TEST_ASSERT_EQUAL_PTR(f1, f2);
}

/* ═══════════════════════════════════════════════════════════════════════
 * Runner
 * ═══════════════════════════════════════════════════════════════════════ */

int main(void)
{
    UNITY_BEGIN();

    /* Vertex shader */
    RUN_TEST(test_vert_source_non_null);
    RUN_TEST(test_vert_source_contains_version);
    RUN_TEST(test_vert_source_contains_gl_position);
    RUN_TEST(test_vert_source_contains_view_proj_uniforms);
    RUN_TEST(test_vert_source_contains_attributes);
    RUN_TEST(test_vert_source_extracts_camera_axes);

    /* Fragment shader */
    RUN_TEST(test_frag_source_non_null);
    RUN_TEST(test_frag_source_contains_version);
    RUN_TEST(test_frag_source_contains_uniforms);

    /* pack_offsets */
    RUN_TEST(test_pack_offsets_returns_4);
    RUN_TEST(test_pack_offsets_correct_values);
    RUN_TEST(test_pack_offsets_null_returns_0);
    RUN_TEST(test_offsets_are_unit_quad);

    /* pack_uvs */
    RUN_TEST(test_pack_uvs_returns_4);
    RUN_TEST(test_pack_uvs_correct_values);
    RUN_TEST(test_pack_uvs_null_returns_0);
    RUN_TEST(test_uvs_map_0_to_1);

    /* pack_indices */
    RUN_TEST(test_pack_indices_returns_6);
    RUN_TEST(test_pack_indices_null_returns_0);
    RUN_TEST(test_indices_form_two_triangles_ccw);

    /* Static pointer */
    RUN_TEST(test_shader_sources_same_pointer);

    return UNITY_END();
}
