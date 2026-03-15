/*
 * test_mesh_shader.c — Tests for lit 3D mesh shader source strings
 *
 * Validates that mesh_shader vertex and fragment sources are well-formed
 * GLSL ES 3.00 strings containing expected attributes, uniforms, and tokens.
 *
 * 12 tests.
 */

#include "../unity/unity.h"
#include "../../src/render/mesh_shader.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ======================================================================
 * 1. Vertex shader source
 * ====================================================================== */

/* 1. Vertex source is not NULL */
void test_mesh_vert_source_not_null(void)
{
    TEST_ASSERT_NOT_NULL(mesh_shader_vert_source());
}

/* 2. Vertex source is not empty */
void test_mesh_vert_source_not_empty(void)
{
    TEST_ASSERT_TRUE(strlen(mesh_shader_vert_source()) > 0);
}

/* 3. Vertex source contains #version 300 es */
void test_mesh_vert_has_version(void)
{
    TEST_ASSERT_NOT_NULL(strstr(mesh_shader_vert_source(), "#version 300 es"));
}

/* 4. Vertex source contains void main() */
void test_mesh_vert_has_main(void)
{
    TEST_ASSERT_NOT_NULL(strstr(mesh_shader_vert_source(), "void main()"));
}

/* 5. Vertex source contains a_position attribute */
void test_mesh_vert_has_position_attr(void)
{
    TEST_ASSERT_NOT_NULL(strstr(mesh_shader_vert_source(), "a_position"));
}

/* 6. Vertex source contains model/view/proj uniforms */
void test_mesh_vert_has_mvp_uniforms(void)
{
    const char *src = mesh_shader_vert_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "u_model"));
    TEST_ASSERT_NOT_NULL(strstr(src, "u_view"));
    TEST_ASSERT_NOT_NULL(strstr(src, "u_proj"));
}

/* 7. Vertex source contains gl_Position assignment */
void test_mesh_vert_has_gl_position(void)
{
    TEST_ASSERT_NOT_NULL(strstr(mesh_shader_vert_source(), "gl_Position"));
}

/* ======================================================================
 * 2. Fragment shader source
 * ====================================================================== */

/* 8. Fragment source is not NULL */
void test_mesh_frag_source_not_null(void)
{
    TEST_ASSERT_NOT_NULL(mesh_shader_frag_source());
}

/* 9. Fragment source is not empty */
void test_mesh_frag_source_not_empty(void)
{
    TEST_ASSERT_TRUE(strlen(mesh_shader_frag_source()) > 0);
}

/* 10. Fragment source contains #version 300 es */
void test_mesh_frag_has_version(void)
{
    TEST_ASSERT_NOT_NULL(strstr(mesh_shader_frag_source(), "#version 300 es"));
}

/* 11. Fragment source contains frag_color output */
void test_mesh_frag_has_frag_color(void)
{
    TEST_ASSERT_NOT_NULL(strstr(mesh_shader_frag_source(), "frag_color"));
}

/* 12. Fragment source contains u_color and u_light_dir uniforms */
void test_mesh_frag_has_lighting_uniforms(void)
{
    const char *src = mesh_shader_frag_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "u_color"));
    TEST_ASSERT_NOT_NULL(strstr(src, "u_light_dir"));
}

/* ======================================================================
 * 3. Stability
 * ====================================================================== */

/* (bonus) Calling vert source twice returns same pointer */
void test_mesh_vert_source_stable(void)
{
    const char *a = mesh_shader_vert_source();
    const char *b = mesh_shader_vert_source();
    TEST_ASSERT_EQUAL_PTR(a, b);
}

/* (bonus) Calling frag source twice returns same pointer */
void test_mesh_frag_source_stable(void)
{
    const char *a = mesh_shader_frag_source();
    const char *b = mesh_shader_frag_source();
    TEST_ASSERT_EQUAL_PTR(a, b);
}

/* ======================================================================
 * Runner
 * ====================================================================== */

int main(void)
{
    UNITY_BEGIN();

    /* Vertex shader (7 tests) */
    RUN_TEST(test_mesh_vert_source_not_null);
    RUN_TEST(test_mesh_vert_source_not_empty);
    RUN_TEST(test_mesh_vert_has_version);
    RUN_TEST(test_mesh_vert_has_main);
    RUN_TEST(test_mesh_vert_has_position_attr);
    RUN_TEST(test_mesh_vert_has_mvp_uniforms);
    RUN_TEST(test_mesh_vert_has_gl_position);

    /* Fragment shader (5 tests) */
    RUN_TEST(test_mesh_frag_source_not_null);
    RUN_TEST(test_mesh_frag_source_not_empty);
    RUN_TEST(test_mesh_frag_has_version);
    RUN_TEST(test_mesh_frag_has_frag_color);
    RUN_TEST(test_mesh_frag_has_lighting_uniforms);

    /* Stability (2 tests) */
    RUN_TEST(test_mesh_vert_source_stable);
    RUN_TEST(test_mesh_frag_source_stable);

    return UNITY_END();
}
