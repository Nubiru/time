#include "unity.h"
#include "math/vec_interp.h"

#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define EPSILON 0.001f
#define DT (1.0f / 60.0f)

void setUp(void) {}
void tearDown(void) {}

/* Helper: run spring_vec3 for n steps of dt */
static spring_vec3_t run_vec3_steps(spring_vec3_t s, float dt, int n) {
    for (int i = 0; i < n; i++) {
        s = spring_vec3_update(s, dt);
    }
    return s;
}

/* ===== lerp_vec3 ===== */

void test_lerp_vec3_t0(void) {
    vec3_t a = vec3_create(1.0f, 2.0f, 3.0f);
    vec3_t b = vec3_create(5.0f, 6.0f, 7.0f);
    vec3_t r = lerp_vec3(a, b, 0.0f);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, r.x);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 2.0f, r.y);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 3.0f, r.z);
}

void test_lerp_vec3_t1(void) {
    vec3_t a = vec3_create(1.0f, 2.0f, 3.0f);
    vec3_t b = vec3_create(5.0f, 6.0f, 7.0f);
    vec3_t r = lerp_vec3(a, b, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 5.0f, r.x);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 6.0f, r.y);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 7.0f, r.z);
}

void test_lerp_vec3_midpoint(void) {
    vec3_t a = vec3_create(0.0f, 0.0f, 0.0f);
    vec3_t b = vec3_create(10.0f, 20.0f, 30.0f);
    vec3_t r = lerp_vec3(a, b, 0.5f);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 5.0f, r.x);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 10.0f, r.y);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 15.0f, r.z);
}

void test_lerp_vec3_extrapolate(void) {
    vec3_t a = vec3_create(0.0f, 0.0f, 0.0f);
    vec3_t b = vec3_create(1.0f, 2.0f, 3.0f);
    vec3_t r = lerp_vec3(a, b, 2.0f);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 2.0f, r.x);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 4.0f, r.y);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 6.0f, r.z);
}

/* ===== lerp_vec3_clamped ===== */

void test_lerp_vec3_clamped_over(void) {
    vec3_t a = vec3_create(0.0f, 0.0f, 0.0f);
    vec3_t b = vec3_create(1.0f, 2.0f, 3.0f);
    vec3_t r = lerp_vec3_clamped(a, b, 1.5f);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, r.x);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 2.0f, r.y);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 3.0f, r.z);
}

void test_lerp_vec3_clamped_under(void) {
    vec3_t a = vec3_create(1.0f, 2.0f, 3.0f);
    vec3_t b = vec3_create(5.0f, 6.0f, 7.0f);
    vec3_t r = lerp_vec3_clamped(a, b, -0.5f);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, r.x);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 2.0f, r.y);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 3.0f, r.z);
}

/* ===== slerp_vec3 ===== */

void test_slerp_vec3_t0(void) {
    vec3_t a = vec3_create(1.0f, 0.0f, 0.0f);
    vec3_t b = vec3_create(0.0f, 1.0f, 0.0f);
    vec3_t r = slerp_vec3(a, b, 0.0f);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, r.x);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, r.y);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, r.z);
}

void test_slerp_vec3_t1(void) {
    vec3_t a = vec3_create(1.0f, 0.0f, 0.0f);
    vec3_t b = vec3_create(0.0f, 1.0f, 0.0f);
    vec3_t r = slerp_vec3(a, b, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, r.x);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, r.y);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, r.z);
}

void test_slerp_vec3_midpoint_unit_length(void) {
    vec3_t a = vec3_create(1.0f, 0.0f, 0.0f);
    vec3_t b = vec3_create(0.0f, 1.0f, 0.0f);
    vec3_t r = slerp_vec3(a, b, 0.5f);
    float len = vec3_length(r);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, len);
}

void test_slerp_vec3_parallel(void) {
    vec3_t a = vec3_create(1.0f, 0.0f, 0.0f);
    vec3_t b = vec3_create(1.0f, 0.0001f, 0.0f); /* nearly identical */
    vec3_t r = slerp_vec3(a, b, 0.5f);
    float len = vec3_length(r);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, len);
    /* Should be very close to a */
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, r.x);
}

/* ===== lerp_f ===== */

void test_lerp_f_basic(void) {
    float r = lerp_f(0.0f, 1.0f, 0.5f);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.5f, r);
}

void test_lerp_f_clamped(void) {
    float r = lerp_f_clamped(0.0f, 1.0f, 2.0f);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, r);
}

/* ===== lerp_angle ===== */

void test_lerp_angle_same(void) {
    float r = lerp_angle(1.0f, 1.0f, 0.5f);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, r);
}

void test_lerp_angle_shortest(void) {
    /* 350 degrees to 10 degrees (in radians) */
    float a = 350.0f * (float)M_PI / 180.0f; /* ~6.1087 rad */
    float b = 10.0f * (float)M_PI / 180.0f;  /* ~0.1745 rad */
    float r = lerp_angle(a, b, 0.5f);
    /* Midpoint should be at 0 degrees (or 360, i.e. 2*PI) */
    /* Normalize result to [0, 2PI] for comparison */
    float expected = 0.0f; /* 0 degrees */
    float diff = r - expected;
    while (diff > (float)M_PI) {
        diff -= 2.0f * (float)M_PI;
    }
    while (diff < -(float)M_PI) {
        diff += 2.0f * (float)M_PI;
    }
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, diff);
}

void test_lerp_angle_wrap(void) {
    /* From just below 2PI to just above 0 */
    float a = 6.0f;                        /* ~344 degrees */
    float b = 0.5f;                        /* ~28.6 degrees */
    float r = lerp_angle(a, b, 0.5f);
    /* Should go through the short way (crossing 2PI/0) */
    /* Midpoint: a + (b - a normalized) * 0.5 */
    /* diff = 0.5 - 6.0 = -5.5, normalized: -5.5 + 2PI ~ 0.783 */
    /* result = 6.0 + 0.783 * 0.5 = 6.3916 */
    /* Which wraps to 6.3916 - 2PI ~ 0.1084 from above */
    /* The key point: it doesn't go the long way round */
    float expected_diff = b - a;
    while (expected_diff > (float)M_PI) {
        expected_diff -= 2.0f * (float)M_PI;
    }
    while (expected_diff < -(float)M_PI) {
        expected_diff += 2.0f * (float)M_PI;
    }
    float expected = a + expected_diff * 0.5f;
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, expected, r);
}

/* ===== spring_vec3 ===== */

void test_spring_vec3_converges(void) {
    vec3_t initial = vec3_create(0.0f, 0.0f, 0.0f);
    vec3_t target = vec3_create(1.0f, 2.0f, 3.0f);
    spring_vec3_t s = spring_vec3_create(initial, target, SPRING_GENTLE);
    /* Run for 5 seconds at 60fps = 300 steps */
    s = run_vec3_steps(s, DT, 300);
    vec3_t pos = spring_vec3_position(s);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, pos.x);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 2.0f, pos.y);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 3.0f, pos.z);
}

void test_spring_vec3_settled(void) {
    vec3_t initial = vec3_create(0.0f, 0.0f, 0.0f);
    vec3_t target = vec3_create(1.0f, 1.0f, 1.0f);
    spring_vec3_t s = spring_vec3_create(initial, target, SPRING_SNAPPY);
    /* Run for 3 seconds = 180 steps */
    s = run_vec3_steps(s, DT, 180);
    TEST_ASSERT_TRUE(spring_vec3_settled(s, 0.01f));
}

void test_spring_vec3_position(void) {
    vec3_t initial = vec3_create(3.0f, 4.0f, 5.0f);
    vec3_t target = vec3_create(3.0f, 4.0f, 5.0f);
    spring_vec3_t s = spring_vec3_create(initial, target, SPRING_GENTLE);
    vec3_t pos = spring_vec3_position(s);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 3.0f, pos.x);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 4.0f, pos.y);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 5.0f, pos.z);
}

int main(void) {
    UNITY_BEGIN();

    /* lerp_vec3 */
    RUN_TEST(test_lerp_vec3_t0);
    RUN_TEST(test_lerp_vec3_t1);
    RUN_TEST(test_lerp_vec3_midpoint);
    RUN_TEST(test_lerp_vec3_extrapolate);

    /* lerp_vec3_clamped */
    RUN_TEST(test_lerp_vec3_clamped_over);
    RUN_TEST(test_lerp_vec3_clamped_under);

    /* slerp_vec3 */
    RUN_TEST(test_slerp_vec3_t0);
    RUN_TEST(test_slerp_vec3_t1);
    RUN_TEST(test_slerp_vec3_midpoint_unit_length);
    RUN_TEST(test_slerp_vec3_parallel);

    /* lerp_f */
    RUN_TEST(test_lerp_f_basic);
    RUN_TEST(test_lerp_f_clamped);

    /* lerp_angle */
    RUN_TEST(test_lerp_angle_same);
    RUN_TEST(test_lerp_angle_shortest);
    RUN_TEST(test_lerp_angle_wrap);

    /* spring_vec3 */
    RUN_TEST(test_spring_vec3_converges);
    RUN_TEST(test_spring_vec3_settled);
    RUN_TEST(test_spring_vec3_position);

    return UNITY_END();
}
