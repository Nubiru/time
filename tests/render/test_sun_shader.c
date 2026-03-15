/*
 * test_sun_shader.c — Tests for procedural animated Sun shader source strings
 *
 * Validates that sun_shader vertex and fragment sources are well-formed
 * GLSL ES 3.00 strings. The fragment shader concatenates the noise library
 * at runtime via shader_builder, so it must contain noise functions.
 *
 * 14 tests.
 */

#include "../unity/unity.h"
#include "../../src/render/sun_shader.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ======================================================================
 * 1. Vertex shader source
 * ====================================================================== */

/* 1. Vertex source is not NULL */
void test_sun_vert_source_not_null(void)
{
    TEST_ASSERT_NOT_NULL(sun_shader_vert_source());
}

/* 2. Vertex source is not empty */
void test_sun_vert_source_not_empty(void)
{
    TEST_ASSERT_TRUE(strlen(sun_shader_vert_source()) > 0);
}

/* 3. Vertex source contains #version 300 es */
void test_sun_vert_has_version(void)
{
    TEST_ASSERT_NOT_NULL(strstr(sun_shader_vert_source(), "#version 300 es"));
}

/* 4. Vertex source contains void main() */
void test_sun_vert_has_main(void)
{
    TEST_ASSERT_NOT_NULL(strstr(sun_shader_vert_source(), "void main()"));
}

/* 5. Vertex source contains gl_Position */
void test_sun_vert_has_gl_position(void)
{
    TEST_ASSERT_NOT_NULL(strstr(sun_shader_vert_source(), "gl_Position"));
}

/* 6. Vertex source contains u_sun_pos uniform */
void test_sun_vert_has_sun_pos(void)
{
    TEST_ASSERT_NOT_NULL(strstr(sun_shader_vert_source(), "u_sun_pos"));
}

/* ======================================================================
 * 2. Fragment shader source
 * ====================================================================== */

/* 7. Fragment source is not NULL */
void test_sun_frag_source_not_null(void)
{
    TEST_ASSERT_NOT_NULL(sun_shader_frag_source());
}

/* 8. Fragment source is not empty */
void test_sun_frag_source_not_empty(void)
{
    TEST_ASSERT_TRUE(strlen(sun_shader_frag_source()) > 0);
}

/* 9. Fragment source contains #version 300 es */
void test_sun_frag_has_version(void)
{
    TEST_ASSERT_NOT_NULL(strstr(sun_shader_frag_source(), "#version 300 es"));
}

/* 10. Fragment source contains void main() */
void test_sun_frag_has_main(void)
{
    TEST_ASSERT_NOT_NULL(strstr(sun_shader_frag_source(), "void main()"));
}

/* 11. Fragment source contains u_time uniform */
void test_sun_frag_has_u_time(void)
{
    TEST_ASSERT_NOT_NULL(strstr(sun_shader_frag_source(), "u_time"));
}

/* 12. Fragment source contains noise functions (concatenated noise lib) */
void test_sun_frag_has_noise_functions(void)
{
    const char *src = sun_shader_frag_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "snoise2"));
    TEST_ASSERT_NOT_NULL(strstr(src, "snoise3"));
    TEST_ASSERT_NOT_NULL(strstr(src, "fbm3"));
}

/* 13. Fragment source contains frag_color output */
void test_sun_frag_has_frag_color(void)
{
    TEST_ASSERT_NOT_NULL(strstr(sun_shader_frag_source(), "frag_color"));
}

/* 14. Fragment source length is bounded (< 16384 chars — shader_builder capacity) */
void test_sun_frag_length_bounded(void)
{
    TEST_ASSERT_TRUE(strlen(sun_shader_frag_source()) < 16384);
}

/* ======================================================================
 * Runner
 * ====================================================================== */

int main(void)
{
    UNITY_BEGIN();

    /* Vertex shader (6 tests) */
    RUN_TEST(test_sun_vert_source_not_null);
    RUN_TEST(test_sun_vert_source_not_empty);
    RUN_TEST(test_sun_vert_has_version);
    RUN_TEST(test_sun_vert_has_main);
    RUN_TEST(test_sun_vert_has_gl_position);
    RUN_TEST(test_sun_vert_has_sun_pos);

    /* Fragment shader (8 tests) */
    RUN_TEST(test_sun_frag_source_not_null);
    RUN_TEST(test_sun_frag_source_not_empty);
    RUN_TEST(test_sun_frag_has_version);
    RUN_TEST(test_sun_frag_has_main);
    RUN_TEST(test_sun_frag_has_u_time);
    RUN_TEST(test_sun_frag_has_noise_functions);
    RUN_TEST(test_sun_frag_has_frag_color);
    RUN_TEST(test_sun_frag_length_bounded);

    return UNITY_END();
}
