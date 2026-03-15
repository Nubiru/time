/*
 * test_noise_shader.c — Tests for GLSL ES 3.00 noise function library
 *
 * Validates that the noise shader source string contains expected simplex noise,
 * fbm, and permutation functions with correct GLSL types.
 *
 * 12 tests.
 */

#include "../unity/unity.h"
#include "../../src/render/noise_shader.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ======================================================================
 * 1. Basic validity
 * ====================================================================== */

/* 1. Source is not NULL */
void test_noise_source_not_null(void)
{
    TEST_ASSERT_NOT_NULL(noise_shader_source());
}

/* 2. Source is not empty */
void test_noise_source_not_empty(void)
{
    TEST_ASSERT_TRUE(strlen(noise_shader_source()) > 0);
}

/* 3. Source length is bounded (< 8192 chars) */
void test_noise_source_length_bounded(void)
{
    TEST_ASSERT_TRUE(strlen(noise_shader_source()) < 8192);
}

/* ======================================================================
 * 2. Required noise functions
 * ====================================================================== */

/* 4. Contains snoise2 function */
void test_noise_has_snoise2(void)
{
    TEST_ASSERT_NOT_NULL(strstr(noise_shader_source(), "snoise2"));
}

/* 5. Contains snoise3 function */
void test_noise_has_snoise3(void)
{
    TEST_ASSERT_NOT_NULL(strstr(noise_shader_source(), "snoise3"));
}

/* 6. Contains fbm2 function */
void test_noise_has_fbm2(void)
{
    TEST_ASSERT_NOT_NULL(strstr(noise_shader_source(), "fbm2"));
}

/* 7. Contains fbm3 function */
void test_noise_has_fbm3(void)
{
    TEST_ASSERT_NOT_NULL(strstr(noise_shader_source(), "fbm3"));
}

/* 8. Contains permute function */
void test_noise_has_permute(void)
{
    TEST_ASSERT_NOT_NULL(strstr(noise_shader_source(), "permute"));
}

/* ======================================================================
 * 3. GLSL types present
 * ====================================================================== */

/* 9. Contains vec2 type */
void test_noise_has_vec2(void)
{
    TEST_ASSERT_NOT_NULL(strstr(noise_shader_source(), "vec2"));
}

/* 10. Contains vec3 type */
void test_noise_has_vec3(void)
{
    TEST_ASSERT_NOT_NULL(strstr(noise_shader_source(), "vec3"));
}

/* 11. Contains vec4 type */
void test_noise_has_vec4(void)
{
    TEST_ASSERT_NOT_NULL(strstr(noise_shader_source(), "vec4"));
}

/* ======================================================================
 * 4. Stability
 * ====================================================================== */

/* 12. Calling source twice returns same pointer */
void test_noise_source_stable(void)
{
    const char *a = noise_shader_source();
    const char *b = noise_shader_source();
    TEST_ASSERT_EQUAL_PTR(a, b);
}

/* ======================================================================
 * Runner
 * ====================================================================== */

int main(void)
{
    UNITY_BEGIN();

    /* Basic validity (3 tests) */
    RUN_TEST(test_noise_source_not_null);
    RUN_TEST(test_noise_source_not_empty);
    RUN_TEST(test_noise_source_length_bounded);

    /* Required functions (5 tests) */
    RUN_TEST(test_noise_has_snoise2);
    RUN_TEST(test_noise_has_snoise3);
    RUN_TEST(test_noise_has_fbm2);
    RUN_TEST(test_noise_has_fbm3);
    RUN_TEST(test_noise_has_permute);

    /* GLSL types (3 tests) */
    RUN_TEST(test_noise_has_vec2);
    RUN_TEST(test_noise_has_vec3);
    RUN_TEST(test_noise_has_vec4);

    /* Stability (1 test) */
    RUN_TEST(test_noise_source_stable);

    return UNITY_END();
}
