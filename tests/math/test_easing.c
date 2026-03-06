#include "../unity/unity.h"
#include "../../src/math/easing.h"
#include <math.h>

#define EPSILON 1e-4f
#define PI 3.14159265358979323846

void setUp(void) { }
void tearDown(void) { }

/* --- Boundary tests: f(0)=0, f(1)=1 --- */

void test_all_easing_at_zero(void) {
    ease_fn_t fns[] = {
        ease_linear, ease_in_quad, ease_out_quad, ease_in_out_quad,
        ease_in_cubic, ease_out_cubic, ease_in_out_cubic,
        ease_in_sine, ease_out_sine, ease_in_out_sine,
        ease_in_expo, ease_out_expo,
        ease_in_elastic, ease_out_elastic,
        ease_in_back, ease_out_back
    };
    int count = (int)(sizeof(fns) / sizeof(fns[0]));
    for (int i = 0; i < count; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, (float)fns[i](0.0));
    }
}

void test_all_easing_at_one(void) {
    ease_fn_t fns[] = {
        ease_linear, ease_in_quad, ease_out_quad, ease_in_out_quad,
        ease_in_cubic, ease_out_cubic, ease_in_out_cubic,
        ease_in_sine, ease_out_sine, ease_in_out_sine,
        ease_in_expo, ease_out_expo,
        ease_in_elastic, ease_out_elastic,
        ease_in_back, ease_out_back
    };
    int count = (int)(sizeof(fns) / sizeof(fns[0]));
    for (int i = 0; i < count; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, (float)fns[i](1.0));
    }
}

/* --- Specific value tests --- */

void test_linear_midpoint(void) {
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.5f, (float)ease_linear(0.5));
}

void test_in_quad_midpoint(void) {
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.25f, (float)ease_in_quad(0.5));
}

void test_out_quad_midpoint(void) {
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.75f, (float)ease_out_quad(0.5));
}

void test_in_cubic_midpoint(void) {
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.125f, (float)ease_in_cubic(0.5));
}

void test_in_sine_midpoint(void) {
    float expected = (float)(1.0 - cos(PI / 4.0));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, expected, (float)ease_in_sine(0.5));
}

void test_out_sine_midpoint(void) {
    float expected = (float)sin(PI / 4.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, expected, (float)ease_out_sine(0.5));
}

/* --- Symmetry tests --- */

void test_in_out_quad_midpoint(void) {
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.5f, (float)ease_in_out_quad(0.5));
}

void test_in_out_cubic_midpoint(void) {
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.5f, (float)ease_in_out_cubic(0.5));
}

void test_in_out_sine_midpoint(void) {
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.5f, (float)ease_in_out_sine(0.5));
}

/* --- Overshoot tests --- */

void test_in_back_pullback(void) {
    int found_negative = 0;
    for (int i = 1; i < 50; i++) {
        double t = (double)i / 100.0;
        if (ease_in_back(t) < 0.0) {
            found_negative = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found_negative);
}

void test_out_back_overshoot(void) {
    int found_overshoot = 0;
    for (int i = 51; i < 100; i++) {
        double t = (double)i / 100.0;
        if (ease_out_back(t) > 1.0) {
            found_overshoot = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found_overshoot);
}

void test_out_elastic_overshoot(void) {
    int found_overshoot = 0;
    for (int i = 1; i < 100; i++) {
        double t = (double)i / 100.0;
        if (ease_out_elastic(t) > 1.0) {
            found_overshoot = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found_overshoot);
}

/* --- Monotonicity tests --- */

void test_in_quad_monotonic(void) {
    double prev = ease_in_quad(0.0);
    for (int i = 1; i <= 10; i++) {
        double t = (double)i / 10.0;
        double val = ease_in_quad(t);
        TEST_ASSERT_TRUE(val >= prev);
        prev = val;
    }
}

void test_out_quad_monotonic(void) {
    double prev = ease_out_quad(0.0);
    for (int i = 1; i <= 10; i++) {
        double t = (double)i / 10.0;
        double val = ease_out_quad(t);
        TEST_ASSERT_TRUE(val >= prev);
        prev = val;
    }
}

/* --- Animation helper tests --- */

void test_anim_value_at_start(void) {
    anim_t a = { .start = 10.0, .end = 20.0, .duration = 2.0 };
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 10.0f, (float)anim_value(a, 0.0, ease_linear));
}

void test_anim_value_at_end(void) {
    anim_t a = { .start = 10.0, .end = 20.0, .duration = 2.0 };
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 20.0f, (float)anim_value(a, 2.0, ease_linear));
}

void test_anim_value_midpoint_linear(void) {
    anim_t a = { .start = 10.0, .end = 20.0, .duration = 2.0 };
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 15.0f, (float)anim_value(a, 1.0, ease_linear));
}

void test_anim_done(void) {
    anim_t a = { .start = 0.0, .end = 1.0, .duration = 1.0 };
    TEST_ASSERT_EQUAL_INT(0, anim_done(a, 0.5));
    TEST_ASSERT_EQUAL_INT(1, anim_done(a, 1.0));
    TEST_ASSERT_EQUAL_INT(1, anim_done(a, 2.0));
}

/* --- Angle tests --- */

void test_angle_lerp_wraparound(void) {
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, (float)angle_lerp(350.0, 10.0, 0.5));
}

void test_angle_normalize_negative(void) {
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 270.0f, (float)angle_normalize(-90.0));
}

void test_angle_distance_clockwise(void) {
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, -20.0f, (float)angle_distance(10.0, 350.0));
}

void test_angle_distance_counterclockwise(void) {
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 20.0f, (float)angle_distance(350.0, 10.0));
}

/* --- Purity test --- */

void test_purity_same_input_same_output(void) {
    float a = (float)ease_in_out_cubic(0.3);
    float b = (float)ease_in_out_cubic(0.3);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, a, b);

    float c = (float)angle_lerp(350.0, 10.0, 0.25);
    float d = (float)angle_lerp(350.0, 10.0, 0.25);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, c, d);
}

/* --- Runner --- */

int main(void) {
    UNITY_BEGIN();

    /* Boundary */
    RUN_TEST(test_all_easing_at_zero);
    RUN_TEST(test_all_easing_at_one);

    /* Specific values */
    RUN_TEST(test_linear_midpoint);
    RUN_TEST(test_in_quad_midpoint);
    RUN_TEST(test_out_quad_midpoint);
    RUN_TEST(test_in_cubic_midpoint);
    RUN_TEST(test_in_sine_midpoint);
    RUN_TEST(test_out_sine_midpoint);

    /* Symmetry */
    RUN_TEST(test_in_out_quad_midpoint);
    RUN_TEST(test_in_out_cubic_midpoint);
    RUN_TEST(test_in_out_sine_midpoint);

    /* Overshoot */
    RUN_TEST(test_in_back_pullback);
    RUN_TEST(test_out_back_overshoot);
    RUN_TEST(test_out_elastic_overshoot);

    /* Monotonicity */
    RUN_TEST(test_in_quad_monotonic);
    RUN_TEST(test_out_quad_monotonic);

    /* Animation helpers */
    RUN_TEST(test_anim_value_at_start);
    RUN_TEST(test_anim_value_at_end);
    RUN_TEST(test_anim_value_midpoint_linear);
    RUN_TEST(test_anim_done);

    /* Angles */
    RUN_TEST(test_angle_lerp_wraparound);
    RUN_TEST(test_angle_normalize_negative);
    RUN_TEST(test_angle_distance_clockwise);
    RUN_TEST(test_angle_distance_counterclockwise);

    /* Purity */
    RUN_TEST(test_purity_same_input_same_output);

    return UNITY_END();
}
