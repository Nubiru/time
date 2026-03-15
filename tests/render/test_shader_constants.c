/*
 * test_shader_constants.c — Tests for GLSL shader constants preamble
 *
 * Tests: preamble content, named constant lookup, count, mathematical
 *        accuracy, brand colors, luminance coefficients.
 */

#include "../unity/unity.h"
#include "../../src/render/shader_constants.h"

#include <string.h>
#include <math.h>

void setUp(void) {}
void tearDown(void) {}

/* ═══════════════════════════════════════════════════════════════════════
 * 1. Preamble basics
 * ═══════════════════════════════════════════════════════════════════════ */

void test_preamble_non_null(void)
{
    const char *p = shader_constants_preamble();
    TEST_ASSERT_NOT_NULL(p);
    TEST_ASSERT_TRUE(strlen(p) > 0);
}

void test_preamble_starts_with_comment(void)
{
    const char *p = shader_constants_preamble();
    /* Should start with a comment or #define */
    TEST_ASSERT_TRUE(p[0] == '/' || p[0] == '#');
}

/* ═══════════════════════════════════════════════════════════════════════
 * 2. Mathematical constants
 * ═══════════════════════════════════════════════════════════════════════ */

void test_preamble_contains_pi(void)
{
    const char *p = shader_constants_preamble();
    TEST_ASSERT_NOT_NULL(strstr(p, "#define PI"));
    TEST_ASSERT_NOT_NULL(strstr(p, "3.14159"));
}

void test_preamble_contains_tau(void)
{
    const char *p = shader_constants_preamble();
    TEST_ASSERT_NOT_NULL(strstr(p, "#define TAU"));
}

void test_preamble_contains_phi(void)
{
    const char *p = shader_constants_preamble();
    TEST_ASSERT_NOT_NULL(strstr(p, "#define PHI"));
    TEST_ASSERT_NOT_NULL(strstr(p, "1.61803"));
}

/* ═══════════════════════════════════════════════════════════════════════
 * 3. Rendering constants
 * ═══════════════════════════════════════════════════════════════════════ */

void test_preamble_contains_bloom_threshold(void)
{
    const char *p = shader_constants_preamble();
    TEST_ASSERT_NOT_NULL(strstr(p, "#define BLOOM_THRESHOLD"));
}

/* ═══════════════════════════════════════════════════════════════════════
 * 4. Brand colors
 * ═══════════════════════════════════════════════════════════════════════ */

void test_preamble_contains_solar_gold(void)
{
    const char *p = shader_constants_preamble();
    TEST_ASSERT_NOT_NULL(strstr(p, "SOLAR_GOLD_R"));
    TEST_ASSERT_NOT_NULL(strstr(p, "SOLAR_GOLD_G"));
    TEST_ASSERT_NOT_NULL(strstr(p, "SOLAR_GOLD_B"));
}

/* ═══════════════════════════════════════════════════════════════════════
 * 5. Luminance coefficients
 * ═══════════════════════════════════════════════════════════════════════ */

void test_preamble_contains_luminance(void)
{
    const char *p = shader_constants_preamble();
    TEST_ASSERT_NOT_NULL(strstr(p, "LUMINANCE_R"));
    TEST_ASSERT_NOT_NULL(strstr(p, "0.2126"));
    TEST_ASSERT_NOT_NULL(strstr(p, "LUMINANCE_G"));
    TEST_ASSERT_NOT_NULL(strstr(p, "0.7152"));
    TEST_ASSERT_NOT_NULL(strstr(p, "LUMINANCE_B"));
    TEST_ASSERT_NOT_NULL(strstr(p, "0.0722"));
}

/* ═══════════════════════════════════════════════════════════════════════
 * 6. shader_constant_value
 * ═══════════════════════════════════════════════════════════════════════ */

void test_value_pi(void)
{
    float pi = shader_constant_value("PI");
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 3.14159f, pi);
}

void test_value_phi(void)
{
    float phi = shader_constant_value("PHI");
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.618f, phi);
}

void test_value_nonexistent(void)
{
    float val = shader_constant_value("NONEXISTENT");
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.0f, val);
}

void test_value_null_name(void)
{
    float val = shader_constant_value(NULL);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.0f, val);
}

/* ═══════════════════════════════════════════════════════════════════════
 * 7. shader_constants_count
 * ═══════════════════════════════════════════════════════════════════════ */

void test_count_positive(void)
{
    int n = shader_constants_count();
    TEST_ASSERT_TRUE(n > 0);
}

void test_count_matches_expected(void)
{
    /* We defined 18 constants: PI, TAU, PHI, PHI_INV (4),
     * BLOOM_THRESHOLD, AMBIENT_LEVEL (2),
     * SOLAR_GOLD_R/G/B (3), CELESTIAL_TEAL_R/G/B (3),
     * SPACE_BLACK_R/G/B (3), LUMINANCE_R/G/B (3) = 18 */
    int n = shader_constants_count();
    TEST_ASSERT_EQUAL_INT(18, n);
}

/* ═══════════════════════════════════════════════════════════════════════
 * Runner
 * ═══════════════════════════════════════════════════════════════════════ */

int main(void)
{
    UNITY_BEGIN();

    /* Preamble basics */
    RUN_TEST(test_preamble_non_null);
    RUN_TEST(test_preamble_starts_with_comment);

    /* Mathematical constants */
    RUN_TEST(test_preamble_contains_pi);
    RUN_TEST(test_preamble_contains_tau);
    RUN_TEST(test_preamble_contains_phi);

    /* Rendering */
    RUN_TEST(test_preamble_contains_bloom_threshold);

    /* Brand colors */
    RUN_TEST(test_preamble_contains_solar_gold);

    /* Luminance */
    RUN_TEST(test_preamble_contains_luminance);

    /* Value lookup */
    RUN_TEST(test_value_pi);
    RUN_TEST(test_value_phi);
    RUN_TEST(test_value_nonexistent);
    RUN_TEST(test_value_null_name);

    /* Count */
    RUN_TEST(test_count_positive);
    RUN_TEST(test_count_matches_expected);

    return UNITY_END();
}
