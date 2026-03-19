#include "../../tests/unity/unity.h"
#include "../../src/ui/input_motion.h"

#include <math.h>

#define EPSILON 0.001f

#define ASSERT_FLOAT_NEAR(expected, actual, eps) \
    TEST_ASSERT_TRUE(fabsf((actual) - (expected)) < (eps))

void setUp(void) {}
void tearDown(void) {}

/* ===================================================================
 * Creation
 * =================================================================== */

void test_create_initial_speed(void)
{
    input_motion_t im = input_motion_create(1.0f, 4);
    ASSERT_FLOAT_NEAR(1.0f, input_motion_speed(im), EPSILON);
}

void test_create_layers_at_one(void)
{
    input_motion_t im = input_motion_create(1.0f, 4);
    for (int i = 0; i < 4; i++) {
        ASSERT_FLOAT_NEAR(1.0f, input_motion_layer_opacity(im, i), EPSILON);
    }
}

/* ===================================================================
 * Speed spring
 * =================================================================== */

void test_set_speed_changes_target(void)
{
    input_motion_t im = input_motion_create(1.0f, 0);
    im = input_motion_set_speed(im, 3600.0f);

    /* Speed hasn't jumped yet — still near initial */
    ASSERT_FLOAT_NEAR(1.0f, input_motion_speed(im), EPSILON);

    /* But target is set */
    ASSERT_FLOAT_NEAR(3600.0f, im.speed_spring.target, EPSILON);
}

void test_speed_converges(void)
{
    input_motion_t im = input_motion_create(1.0f, 0);
    im = input_motion_set_speed(im, 100.0f);

    /* Simulate 2 seconds at 60fps */
    for (int i = 0; i < 120; i++) {
        im = input_motion_update(im, 1.0f / 60.0f);
    }
    ASSERT_FLOAT_NEAR(100.0f, input_motion_speed(im), 0.5f);
}

void test_speed_smooth_transition(void)
{
    input_motion_t im = input_motion_create(0.0f, 0);
    im = input_motion_set_speed(im, 100.0f);

    /* Single frame: should be between 0 and 100 */
    im = input_motion_update(im, 1.0f / 60.0f);
    float speed = input_motion_speed(im);
    TEST_ASSERT_TRUE(speed > 0.0f);
    TEST_ASSERT_TRUE(speed < 100.0f);
}

/* ===================================================================
 * Layer springs
 * =================================================================== */

void test_toggle_layer_off(void)
{
    input_motion_t im = input_motion_create(1.0f, 4);
    im = input_motion_toggle_layer(im, 2);

    /* Target should now be 0.0 */
    ASSERT_FLOAT_NEAR(0.0f, im.layer_springs[2].target, EPSILON);
}

void test_toggle_layer_on(void)
{
    input_motion_t im = input_motion_create(1.0f, 4);
    /* Toggle off first */
    im = input_motion_toggle_layer(im, 1);
    /* Simulate to settle */
    for (int i = 0; i < 120; i++) {
        im = input_motion_update(im, 1.0f / 60.0f);
    }
    /* Toggle back on */
    im = input_motion_toggle_layer(im, 1);
    ASSERT_FLOAT_NEAR(1.0f, im.layer_springs[1].target, EPSILON);
}

void test_layer_opacity_converges(void)
{
    input_motion_t im = input_motion_create(1.0f, 4);
    im = input_motion_toggle_layer(im, 0);

    /* Simulate 2 seconds */
    for (int i = 0; i < 120; i++) {
        im = input_motion_update(im, 1.0f / 60.0f);
    }
    ASSERT_FLOAT_NEAR(0.0f, input_motion_layer_opacity(im, 0), 0.01f);
}

void test_layer_opacity_clamped(void)
{
    input_motion_t im = input_motion_create(1.0f, 2);
    /* The spring might overshoot, but the getter must clamp */
    /* Force a spring state with position > 1 for testing */
    im.layer_springs[0].position = 1.5f;
    TEST_ASSERT_TRUE(input_motion_layer_opacity(im, 0) <= 1.0f);

    im.layer_springs[0].position = -0.3f;
    TEST_ASSERT_TRUE(input_motion_layer_opacity(im, 0) >= 0.0f);
}

void test_set_layer_explicit(void)
{
    input_motion_t im = input_motion_create(1.0f, 4);
    im = input_motion_set_layer(im, 3, 0.5f);
    ASSERT_FLOAT_NEAR(0.5f, im.layer_springs[3].target, EPSILON);
}

/* ===================================================================
 * Pulse
 * =================================================================== */

void test_pulse_starts_at_one(void)
{
    input_motion_t im = input_motion_create(1.0f, 0);
    im = input_motion_pulse(im);
    ASSERT_FLOAT_NEAR(1.0f, input_motion_pulse_value(im), EPSILON);
}

void test_pulse_decays(void)
{
    input_motion_t im = input_motion_create(1.0f, 0);
    im = input_motion_pulse(im);

    /* After a few frames, pulse should decrease */
    for (int i = 0; i < 10; i++) {
        im = input_motion_update(im, 1.0f / 60.0f);
    }
    float pv = input_motion_pulse_value(im);
    TEST_ASSERT_TRUE(pv < 1.0f);
    TEST_ASSERT_TRUE(pv > 0.0f);
}

void test_pulse_reaches_zero(void)
{
    input_motion_t im = input_motion_create(1.0f, 0);
    im = input_motion_pulse(im);

    /* Simulate 2 seconds — pulse should be gone */
    for (int i = 0; i < 120; i++) {
        im = input_motion_update(im, 1.0f / 60.0f);
    }
    ASSERT_FLOAT_NEAR(0.0f, input_motion_pulse_value(im), EPSILON);
}

/* ===================================================================
 * Active state
 * =================================================================== */

void test_active_during_transition(void)
{
    input_motion_t im = input_motion_create(1.0f, 0);
    im = input_motion_set_speed(im, 100.0f);

    /* One frame in — should be active */
    im = input_motion_update(im, 1.0f / 60.0f);
    TEST_ASSERT_EQUAL_INT(1, input_motion_active(im));
}

void test_not_active_at_rest(void)
{
    input_motion_t im = input_motion_create(1.0f, 2);

    /* Everything starts at rest */
    TEST_ASSERT_EQUAL_INT(0, input_motion_active(im));
}

/* ===================================================================
 * Edge cases
 * =================================================================== */

void test_invalid_layer_id(void)
{
    input_motion_t im = input_motion_create(1.0f, 4);

    /* Negative layer */
    ASSERT_FLOAT_NEAR(0.0f, input_motion_layer_opacity(im, -1), EPSILON);

    /* Out of range */
    ASSERT_FLOAT_NEAR(0.0f, input_motion_layer_opacity(im, 99), EPSILON);

    /* Toggle out of range — should not crash, return unchanged */
    input_motion_t im2 = input_motion_toggle_layer(im, -1);
    (void)im2;
    input_motion_t im3 = input_motion_toggle_layer(im, 99);
    (void)im3;

    /* set_layer out of range */
    input_motion_t im4 = input_motion_set_layer(im, -1, 0.5f);
    (void)im4;
    input_motion_t im5 = input_motion_set_layer(im, 99, 0.5f);
    (void)im5;
}

/* ===================================================================
 * Runner
 * =================================================================== */

int main(void)
{
    UNITY_BEGIN();

    /* Creation */
    RUN_TEST(test_create_initial_speed);
    RUN_TEST(test_create_layers_at_one);

    /* Speed spring */
    RUN_TEST(test_set_speed_changes_target);
    RUN_TEST(test_speed_converges);
    RUN_TEST(test_speed_smooth_transition);

    /* Layer springs */
    RUN_TEST(test_toggle_layer_off);
    RUN_TEST(test_toggle_layer_on);
    RUN_TEST(test_layer_opacity_converges);
    RUN_TEST(test_layer_opacity_clamped);
    RUN_TEST(test_set_layer_explicit);

    /* Pulse */
    RUN_TEST(test_pulse_starts_at_one);
    RUN_TEST(test_pulse_decays);
    RUN_TEST(test_pulse_reaches_zero);

    /* Active state */
    RUN_TEST(test_active_during_transition);
    RUN_TEST(test_not_active_at_rest);

    /* Edge cases */
    RUN_TEST(test_invalid_layer_id);

    return UNITY_END();
}
