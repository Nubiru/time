#include "../unity/unity.h"
#include "../../src/math/spring.h"

#include <math.h>

#define EPSILON 0.01f
#define DT (1.0f / 60.0f)

void setUp(void)  {}
void tearDown(void) {}

/* Helper: run spring for n steps of dt */
static spring_t run_steps(spring_t s, float dt, int n)
{
    for (int i = 0; i < n; i++) {
        s = spring_update(s, dt);
    }
    return s;
}

/* 1. Create: fields match constructor args */
void test_spring_create_initial_values(void)
{
    spring_t s = spring_create(2.0f, 5.0f, 170.0f, 26.0f);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, 2.0f, s.position);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, 0.0f, s.velocity);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, 5.0f, s.target);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, 170.0f, s.stiffness);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, 26.0f, s.damping);
}

/* 2. Created at target: already settled */
void test_spring_at_target_settled(void)
{
    spring_t s = spring_create(1.0f, 1.0f, SPRING_GENTLE);
    TEST_ASSERT_TRUE(spring_settled(s, EPSILON));
}

/* 3. SPRING_GENTLE converges within 3 seconds */
void test_spring_converges_gentle(void)
{
    spring_t s = spring_create(0.0f, 1.0f, SPRING_GENTLE);
    /* 3 seconds at 60 fps = 180 steps */
    s = run_steps(s, DT, 180);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, s.position);
}

/* 4. SPRING_SNAPPY converges within 1 second */
void test_spring_converges_snappy(void)
{
    spring_t s = spring_create(0.0f, 1.0f, SPRING_SNAPPY);
    /* 1 second at 60 fps = 60 steps */
    s = run_steps(s, DT, 60);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, s.position);
}

/* 5. Critically damped: no overshoot (position never exceeds target) */
void test_spring_critically_damped_no_overshoot(void)
{
    /* Critical damping: d = 2 * sqrt(k) */
    float k = 100.0f;
    float d = 2.0f * sqrtf(k);
    spring_t s = spring_create(0.0f, 1.0f, k, d);
    float max_pos = 0.0f;
    for (int i = 0; i < 300; i++) {
        s = spring_update(s, DT);
        if (s.position > max_pos) {
            max_pos = s.position;
        }
    }
    /* Should never overshoot target of 1.0 (allow tiny float error) */
    TEST_ASSERT_TRUE(max_pos < 1.0f + 0.001f);
}

/* 6. Underdamped (SPRING_BOUNCY): overshoots target */
void test_spring_underdamped_overshoots(void)
{
    spring_t s = spring_create(0.0f, 1.0f, SPRING_BOUNCY);
    int overshot = 0;
    for (int i = 0; i < 300; i++) {
        s = spring_update(s, DT);
        if (s.position > 1.0f + 0.01f) {
            overshot = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(overshot);
}

/* 7. Overdamped: no overshoot */
void test_spring_overdamped_no_overshoot(void)
{
    /* Very high damping */
    spring_t s = spring_create(0.0f, 1.0f, 100.0f, 100.0f);
    float max_pos = 0.0f;
    for (int i = 0; i < 600; i++) {
        s = spring_update(s, DT);
        if (s.position > max_pos) {
            max_pos = s.position;
        }
    }
    TEST_ASSERT_TRUE(max_pos < 1.0f + 0.001f);
}

/* 8. Retarget preserves velocity */
void test_spring_retarget_preserves_velocity(void)
{
    spring_t s = spring_create(0.0f, 1.0f, SPRING_GENTLE);
    /* Run a bit to build up velocity */
    s = run_steps(s, DT, 5);
    float vel_before = s.velocity;
    s = spring_set_target(s, 2.0f);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, vel_before, s.velocity);
}

/* 9. Retarget mid-flight: converges to new target */
void test_spring_retarget_mid_flight(void)
{
    spring_t s = spring_create(0.0f, 1.0f, SPRING_SNAPPY);
    /* Run halfway */
    s = run_steps(s, DT, 15);
    /* Retarget to opposite side */
    s = spring_set_target(s, -1.0f);
    /* Run long enough to converge */
    s = run_steps(s, DT, 300);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, -1.0f, s.position);
}

/* 10. Reset zeroes velocity */
void test_spring_reset_zeroes_velocity(void)
{
    spring_t s = spring_create(0.0f, 1.0f, SPRING_GENTLE);
    s = run_steps(s, DT, 10);
    /* Verify there is velocity before reset */
    TEST_ASSERT_TRUE(fabsf(s.velocity) > 0.001f);
    s = spring_reset(s, 5.0f);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, 5.0f, s.position);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, 0.0f, s.velocity);
}

/* 11. Zero dt: no change */
void test_spring_zero_dt_no_change(void)
{
    spring_t s = spring_create(0.0f, 1.0f, SPRING_GENTLE);
    s = run_steps(s, DT, 10);
    spring_t before = s;
    spring_t after  = spring_update(s, 0.0f);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, before.position, after.position);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, before.velocity, after.velocity);
}

/* 12. Large dt doesn't explode */
void test_spring_large_dt_stable(void)
{
    spring_t s = spring_create(0.0f, 1.0f, SPRING_GENTLE);
    s = spring_update(s, 1.0f);
    /* Must not be NaN or Inf */
    TEST_ASSERT_TRUE(isfinite(s.position));
    TEST_ASSERT_TRUE(isfinite(s.velocity));
    /* After 1 full second, should be reasonably close to target */
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 1.0f, s.position);
}

/* 13. Negative values work */
void test_spring_negative_values(void)
{
    spring_t s = spring_create(-5.0f, -10.0f, SPRING_SNAPPY);
    s = run_steps(s, DT, 120);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, -10.0f, s.position);
}

/* 14. Settled returns true at rest */
void test_spring_settled_true(void)
{
    spring_t s = spring_create(3.0f, 3.0f, SPRING_GENTLE);
    TEST_ASSERT_TRUE(spring_settled(s, EPSILON));
}

/* 15. Settled returns false when moving */
void test_spring_settled_false_moving(void)
{
    spring_t s = spring_create(0.0f, 1.0f, SPRING_GENTLE);
    s = run_steps(s, DT, 5);
    /* Has velocity */
    TEST_ASSERT_FALSE(spring_settled(s, EPSILON));
}

/* 16. Settled returns false when displaced */
void test_spring_settled_false_displaced(void)
{
    spring_t s = spring_create(0.0f, 1.0f, SPRING_GENTLE);
    /* Not at target, no velocity yet either but displaced */
    TEST_ASSERT_FALSE(spring_settled(s, EPSILON));
}

/* 17. SPRING_GENTLE preset eventually settles */
void test_spring_preset_gentle_settles(void)
{
    spring_t s = spring_create(0.0f, 1.0f, SPRING_GENTLE);
    /* Run for 5 seconds */
    s = run_steps(s, DT, 300);
    TEST_ASSERT_TRUE(spring_settled(s, EPSILON));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_spring_create_initial_values);
    RUN_TEST(test_spring_at_target_settled);
    RUN_TEST(test_spring_converges_gentle);
    RUN_TEST(test_spring_converges_snappy);
    RUN_TEST(test_spring_critically_damped_no_overshoot);
    RUN_TEST(test_spring_underdamped_overshoots);
    RUN_TEST(test_spring_overdamped_no_overshoot);
    RUN_TEST(test_spring_retarget_preserves_velocity);
    RUN_TEST(test_spring_retarget_mid_flight);
    RUN_TEST(test_spring_reset_zeroes_velocity);
    RUN_TEST(test_spring_zero_dt_no_change);
    RUN_TEST(test_spring_large_dt_stable);
    RUN_TEST(test_spring_negative_values);
    RUN_TEST(test_spring_settled_true);
    RUN_TEST(test_spring_settled_false_moving);
    RUN_TEST(test_spring_settled_false_displaced);
    RUN_TEST(test_spring_preset_gentle_settles);
    return UNITY_END();
}
