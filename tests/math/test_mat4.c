#include "../unity/unity.h"
#include "../../src/math/mat4.h"
#include "../../src/math/vec3.h"
#include <math.h>

#define FLOAT_EPSILON 1e-5f
#define PI 3.14159265358979323846f

void setUp(void) { }
void tearDown(void) { }

/* --- mat4_identity --- */

void test_mat4_identity(void) {
    mat4_t m = mat4_identity();
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 1.0f, m.m[0]);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 0.0f, m.m[1]);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 0.0f, m.m[4]);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 1.0f, m.m[5]);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 1.0f, m.m[10]);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 1.0f, m.m[15]);
}

/* --- mat4_multiply --- */

void test_mat4_multiply_identity(void) {
    mat4_t a = mat4_translate(3.0f, 4.0f, 5.0f);
    mat4_t i = mat4_identity();
    mat4_t r = mat4_multiply(a, i);
    for (int j = 0; j < 16; j++) {
        TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, a.m[j], r.m[j]);
    }
}

void test_mat4_multiply_translate(void) {
    /* Translating (1,0,0) by T(3,4,5) = (4,4,5) via matrix * vec */
    mat4_t t = mat4_translate(3.0f, 4.0f, 5.0f);
    /* Manually check column 3 (translation column) */
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 3.0f, t.m[12]);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 4.0f, t.m[13]);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 5.0f, t.m[14]);
}

/* --- mat4_scale --- */

void test_mat4_scale(void) {
    mat4_t s = mat4_scale(2.0f, 3.0f, 4.0f);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 2.0f, s.m[0]);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 3.0f, s.m[5]);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 4.0f, s.m[10]);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 1.0f, s.m[15]);
}

/* --- rotations --- */

void test_mat4_rotate_z_90(void) {
    /* 90° around Z: x-axis maps to y-axis */
    mat4_t r = mat4_rotate_z(PI / 2.0f);
    /* Column 0 (where x-axis goes): should be (0, 1, 0) */
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 0.0f, r.m[0]);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 1.0f, r.m[1]);
    /* Column 1 (where y-axis goes): should be (-1, 0, 0) */
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, -1.0f, r.m[4]);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON,  0.0f, r.m[5]);
}

void test_mat4_rotate_x_90(void) {
    /* 90° around X: y-axis maps to z-axis */
    mat4_t r = mat4_rotate_x(PI / 2.0f);
    /* Column 1 (where y-axis goes): should be (0, 0, 1) */
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 0.0f, r.m[5]);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 1.0f, r.m[6]);
}

void test_mat4_rotate_y_90(void) {
    /* 90° around Y: z-axis maps to x-axis */
    mat4_t r = mat4_rotate_y(PI / 2.0f);
    /* Column 2 (where z-axis goes): should be (1, 0, 0) */
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 1.0f, r.m[8]);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 0.0f, r.m[10]);
}

/* --- mat4_perspective --- */

void test_mat4_perspective(void) {
    mat4_t p = mat4_perspective(PI / 4.0f, 16.0f / 9.0f, 0.1f, 100.0f);
    /* m[11] should be -1 (perspective divide) */
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, -1.0f, p.m[11]);
    /* m[15] should be 0 (not affine) */
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 0.0f, p.m[15]);
    /* m[0] and m[5] should be positive (field of view scaling) */
    TEST_ASSERT_TRUE(p.m[0] > 0.0f);
    TEST_ASSERT_TRUE(p.m[5] > 0.0f);
}

/* --- mat4_look_at --- */

void test_mat4_look_at_origin(void) {
    /* Camera at (0,0,5) looking at origin, up is Y */
    vec3_t eye    = vec3_create(0.0f, 0.0f, 5.0f);
    vec3_t center = vec3_create(0.0f, 0.0f, 0.0f);
    vec3_t up     = vec3_create(0.0f, 1.0f, 0.0f);
    mat4_t v = mat4_look_at(eye, center, up);

    /* Forward is -Z, so camera looking down -Z from z=5.
     * The view matrix should translate z by -5 */
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, -5.0f, v.m[14]);
    /* X and Y axes should be unchanged (identity-like for top-left 2x2) */
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 1.0f, v.m[0]);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 1.0f, v.m[5]);
}

/* --- composition --- */

void test_mat4_compose_translate_scale(void) {
    /* Scale by 2, then translate by (10,0,0).
     * In column-major: result = T * S
     * Point (1,0,0) -> scale -> (2,0,0) -> translate -> (12,0,0) */
    mat4_t s = mat4_scale(2.0f, 2.0f, 2.0f);
    mat4_t t = mat4_translate(10.0f, 0.0f, 0.0f);
    mat4_t ts = mat4_multiply(t, s);

    /* Apply to point (1,0,0,1): result = ts * (1,0,0,1)
     * Col0*1 + Col3*1 = (2,0,0,0) + (10,0,0,1) = (12,0,0,1) */
    float x = ts.m[0] * 1.0f + ts.m[12] * 1.0f;
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 12.0f, x);
}

/* --- Runner --- */

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_mat4_identity);
    RUN_TEST(test_mat4_multiply_identity);
    RUN_TEST(test_mat4_multiply_translate);
    RUN_TEST(test_mat4_scale);
    RUN_TEST(test_mat4_rotate_z_90);
    RUN_TEST(test_mat4_rotate_x_90);
    RUN_TEST(test_mat4_rotate_y_90);
    RUN_TEST(test_mat4_perspective);
    RUN_TEST(test_mat4_look_at_origin);
    RUN_TEST(test_mat4_compose_translate_scale);
    return UNITY_END();
}
