#include "../unity/unity.h"
#include "../../src/math/vec3.h"
#include <math.h>

#define FLOAT_EPSILON 1e-6f

void setUp(void) { }
void tearDown(void) { }

/* --- vec3_create --- */

void test_vec3_create(void) {
    vec3_t v = vec3_create(1.0f, 2.0f, 3.0f);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 1.0f, v.x);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 2.0f, v.y);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 3.0f, v.z);
}

/* --- vec3_add --- */

void test_vec3_add(void) {
    vec3_t a = vec3_create(1.0f, 2.0f, 3.0f);
    vec3_t b = vec3_create(4.0f, 5.0f, 6.0f);
    vec3_t r = vec3_add(a, b);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 5.0f, r.x);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 7.0f, r.y);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 9.0f, r.z);
}

/* --- vec3_sub --- */

void test_vec3_sub(void) {
    vec3_t a = vec3_create(5.0f, 7.0f, 9.0f);
    vec3_t b = vec3_create(1.0f, 2.0f, 3.0f);
    vec3_t r = vec3_sub(a, b);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 4.0f, r.x);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 5.0f, r.y);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 6.0f, r.z);
}

/* --- vec3_scale --- */

void test_vec3_scale(void) {
    vec3_t v = vec3_create(1.0f, 2.0f, 3.0f);
    vec3_t r = vec3_scale(v, 2.0f);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 2.0f, r.x);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 4.0f, r.y);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 6.0f, r.z);
}

/* --- vec3_dot --- */

void test_vec3_dot(void) {
    vec3_t a = vec3_create(1.0f, 2.0f, 3.0f);
    vec3_t b = vec3_create(4.0f, 5.0f, 6.0f);
    /* 1*4 + 2*5 + 3*6 = 32 */
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 32.0f, vec3_dot(a, b));
}

void test_vec3_dot_perpendicular(void) {
    vec3_t x = vec3_create(1.0f, 0.0f, 0.0f);
    vec3_t y = vec3_create(0.0f, 1.0f, 0.0f);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 0.0f, vec3_dot(x, y));
}

/* --- vec3_cross --- */

void test_vec3_cross_xy(void) {
    vec3_t x = vec3_create(1.0f, 0.0f, 0.0f);
    vec3_t y = vec3_create(0.0f, 1.0f, 0.0f);
    vec3_t r = vec3_cross(x, y);
    /* x cross y = z */
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 0.0f, r.x);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 0.0f, r.y);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 1.0f, r.z);
}

void test_vec3_cross_anticommutative(void) {
    vec3_t a = vec3_create(1.0f, 2.0f, 3.0f);
    vec3_t b = vec3_create(4.0f, 5.0f, 6.0f);
    vec3_t ab = vec3_cross(a, b);
    vec3_t ba = vec3_cross(b, a);
    /* a x b = -(b x a) */
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, -ba.x, ab.x);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, -ba.y, ab.y);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, -ba.z, ab.z);
}

/* --- vec3_length --- */

void test_vec3_length(void) {
    vec3_t v = vec3_create(3.0f, 4.0f, 0.0f);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 5.0f, vec3_length(v));
}

void test_vec3_length_unit(void) {
    vec3_t v = vec3_create(1.0f, 0.0f, 0.0f);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 1.0f, vec3_length(v));
}

/* --- vec3_normalize --- */

void test_vec3_normalize(void) {
    vec3_t v = vec3_create(3.0f, 4.0f, 0.0f);
    vec3_t n = vec3_normalize(v);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 0.6f, n.x);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 0.8f, n.y);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 0.0f, n.z);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 1.0f, vec3_length(n));
}

void test_vec3_normalize_zero(void) {
    vec3_t v = vec3_create(0.0f, 0.0f, 0.0f);
    vec3_t n = vec3_normalize(v);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 0.0f, n.x);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 0.0f, n.y);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_EPSILON, 0.0f, n.z);
}

/* --- Runner --- */

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_vec3_create);
    RUN_TEST(test_vec3_add);
    RUN_TEST(test_vec3_sub);
    RUN_TEST(test_vec3_scale);
    RUN_TEST(test_vec3_dot);
    RUN_TEST(test_vec3_dot_perpendicular);
    RUN_TEST(test_vec3_cross_xy);
    RUN_TEST(test_vec3_cross_anticommutative);
    RUN_TEST(test_vec3_length);
    RUN_TEST(test_vec3_length_unit);
    RUN_TEST(test_vec3_normalize);
    RUN_TEST(test_vec3_normalize_zero);
    return UNITY_END();
}
