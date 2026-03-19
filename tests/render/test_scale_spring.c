#include "../unity/unity.h"
#include "../../src/render/scale_spring.h"

#include <math.h>

#define DT (1.0f / 60.0f)

void setUp(void)  {}
void tearDown(void) {}

/* Helper: tick for n frames at 60fps */
static scale_spring_t run_frames(scale_spring_t ss, int n)
{
    for (int i = 0; i < n; i++) {
        ss = scale_spring_tick(ss, DT);
    }
    return ss;
}

/* 1. Create: phase is SS_IDLE, done, not active */
static void test_create_idle(void)
{
    scale_spring_t ss = scale_spring_create(3.0f);
    TEST_ASSERT_EQUAL_INT(SS_IDLE, scale_spring_phase(ss));
    TEST_ASSERT_TRUE(scale_spring_done(ss));
    TEST_ASSERT_FALSE(scale_spring_active(ss));
}

/* 2. Create: returns initial log_zoom */
static void test_create_value(void)
{
    scale_spring_t ss = scale_spring_create(3.401f);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, 3.401f, scale_spring_value(ss));
}

/* 3. Start: enters SS_ANTICIPATE phase */
static void test_start_enters_anticipate(void)
{
    scale_spring_t ss = scale_spring_create(3.401f);
    ss = scale_spring_start(ss, 3.401f, SCALE_UNIVERSE, 0.05f, 0.1f);
    TEST_ASSERT_EQUAL_INT(SS_ANTICIPATE, scale_spring_phase(ss));
    TEST_ASSERT_TRUE(scale_spring_active(ss));
    TEST_ASSERT_FALSE(scale_spring_done(ss));
}

/* 4. Anticipation: value moves AWAY from target */
static void test_anticipation_moves_opposite(void)
{
    float start = scale_center_log_zoom(SCALE_SOLAR);   /* 3.401 */
    float target_center = scale_center_log_zoom(SCALE_UNIVERSE); /* 9.903 */

    scale_spring_t ss = scale_spring_create(start);
    ss = scale_spring_start(ss, start, SCALE_UNIVERSE, 0.05f, 0.1f);

    /* Target is above start, so anticipation should move BELOW start */
    ss = run_frames(ss, 3);
    float val = scale_spring_value(ss);

    /* Value should have moved away from target (below start) */
    TEST_ASSERT_TRUE(val < start);
    (void)target_center; /* used for reasoning, not assertion */
}

/* 5. Anticipation ends: after anticipation_dur, phase transitions to SS_SPRING */
static void test_anticipation_ends(void)
{
    scale_spring_t ss = scale_spring_create(3.401f);
    ss = scale_spring_start(ss, 3.401f, SCALE_UNIVERSE, 0.05f, 0.1f);

    /* Run for 0.1s (6 frames at 60fps) + a bit more to ensure transition */
    ss = run_frames(ss, 8);
    TEST_ASSERT_EQUAL_INT(SS_SPRING, scale_spring_phase(ss));
}

/* 6. Spring phase: value approaches target */
static void test_spring_phase_approaches_target(void)
{
    float start = scale_center_log_zoom(SCALE_SOLAR);
    float target_center = scale_center_log_zoom(SCALE_UNIVERSE);

    scale_spring_t ss = scale_spring_create(start);
    ss = scale_spring_start(ss, start, SCALE_UNIVERSE, 0.05f, 0.1f);

    /* Run past anticipation into spring phase */
    ss = run_frames(ss, 30);
    TEST_ASSERT_EQUAL_INT(SS_SPRING, scale_spring_phase(ss));

    float val = scale_spring_value(ss);
    /* Should have moved toward target (above start) */
    TEST_ASSERT_TRUE(val > start);
    /* Should be closer to target than to start */
    float dist_to_start  = fabsf(val - start);
    float dist_to_target = fabsf(val - target_center);
    TEST_ASSERT_TRUE(dist_to_start > dist_to_target);
}

/* 7. Spring overshoots: SPRING_SNAPPY has slight overshoot */
static void test_spring_overshoots(void)
{
    float start = scale_center_log_zoom(SCALE_SOLAR);
    float target_center = scale_center_log_zoom(SCALE_UNIVERSE);

    scale_spring_t ss = scale_spring_create(start);
    /* No anticipation to isolate the spring overshoot behavior */
    ss = scale_spring_start(ss, start, SCALE_UNIVERSE, 0.0f, 0.0f);

    int overshot = 0;
    for (int i = 0; i < 300; i++) {
        ss = scale_spring_tick(ss, DT);
        if (scale_spring_value(ss) > target_center + 0.001f) {
            overshot = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(overshot);
}

/* 8. Spring settles: eventually reaches SS_SETTLED */
static void test_spring_settles(void)
{
    scale_spring_t ss = scale_spring_create(3.401f);
    ss = scale_spring_start(ss, 3.401f, SCALE_UNIVERSE, 0.05f, 0.1f);

    /* Run for 5 seconds (300 frames) */
    ss = run_frames(ss, 300);
    TEST_ASSERT_EQUAL_INT(SS_SETTLED, scale_spring_phase(ss));
}

/* 9. Settled value: equals target center log_zoom */
static void test_settled_value(void)
{
    float target_center = scale_center_log_zoom(SCALE_UNIVERSE);

    scale_spring_t ss = scale_spring_create(3.401f);
    ss = scale_spring_start(ss, 3.401f, SCALE_UNIVERSE, 0.05f, 0.1f);
    ss = run_frames(ss, 300);

    TEST_ASSERT_FLOAT_WITHIN(0.01f, target_center, scale_spring_value(ss));
}

/* 10. Done when settled */
static void test_done_when_settled(void)
{
    scale_spring_t ss = scale_spring_create(3.401f);
    ss = scale_spring_start(ss, 3.401f, SCALE_PERSONAL, 0.05f, 0.1f);
    ss = run_frames(ss, 300);

    TEST_ASSERT_EQUAL_INT(SS_SETTLED, scale_spring_phase(ss));
    TEST_ASSERT_TRUE(scale_spring_done(ss));
}

/* 11. Not done during spring */
static void test_not_done_during_spring(void)
{
    scale_spring_t ss = scale_spring_create(3.401f);
    ss = scale_spring_start(ss, 3.401f, SCALE_UNIVERSE, 0.05f, 0.1f);

    /* Run past anticipation but not long enough to settle */
    ss = run_frames(ss, 10);
    TEST_ASSERT_FALSE(scale_spring_done(ss));
}

/* 12. Active during transition (both anticipate and spring) */
static void test_active_during_transition(void)
{
    scale_spring_t ss = scale_spring_create(3.401f);
    ss = scale_spring_start(ss, 3.401f, SCALE_UNIVERSE, 0.05f, 0.1f);

    /* During anticipation */
    TEST_ASSERT_TRUE(scale_spring_active(ss));

    /* During spring */
    ss = run_frames(ss, 10);
    TEST_ASSERT_EQUAL_INT(SS_SPRING, scale_spring_phase(ss));
    TEST_ASSERT_TRUE(scale_spring_active(ss));
}

/* 13. Not active when idle */
static void test_not_active_when_idle(void)
{
    scale_spring_t ss = scale_spring_create(3.401f);
    TEST_ASSERT_FALSE(scale_spring_active(ss));
}

/* 14. Cancel: stops transition, preserves position */
static void test_cancel_stops(void)
{
    scale_spring_t ss = scale_spring_create(3.401f);
    ss = scale_spring_start(ss, 3.401f, SCALE_UNIVERSE, 0.05f, 0.1f);
    ss = run_frames(ss, 20);

    float pos_before = scale_spring_value(ss);
    ss = scale_spring_cancel(ss);

    TEST_ASSERT_EQUAL_INT(SS_IDLE, scale_spring_phase(ss));
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, pos_before, scale_spring_value(ss));
    TEST_ASSERT_FALSE(scale_spring_active(ss));
}

/* 15. Start default: uses 5% and 0.1s anticipation */
static void test_start_default_anticipation(void)
{
    float start = scale_center_log_zoom(SCALE_SOLAR);

    scale_spring_t ss = scale_spring_create(start);
    ss = scale_spring_start_default(ss, start, SCALE_UNIVERSE);

    TEST_ASSERT_EQUAL_INT(SS_ANTICIPATE, scale_spring_phase(ss));
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, 0.1f, ss.anticipation_duration);

    float distance = scale_center_log_zoom(SCALE_UNIVERSE) - start;
    float expected_amount = distance * 0.05f;
    TEST_ASSERT_FLOAT_WITHIN(0.001f, expected_amount, ss.anticipation_amount);
}

/* 16. Zero anticipation: skips straight to SS_SPRING */
static void test_zero_anticipation(void)
{
    scale_spring_t ss = scale_spring_create(3.401f);
    ss = scale_spring_start(ss, 3.401f, SCALE_UNIVERSE, 0.0f, 0.0f);

    TEST_ASSERT_EQUAL_INT(SS_SPRING, scale_spring_phase(ss));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_create_idle);
    RUN_TEST(test_create_value);
    RUN_TEST(test_start_enters_anticipate);
    RUN_TEST(test_anticipation_moves_opposite);
    RUN_TEST(test_anticipation_ends);
    RUN_TEST(test_spring_phase_approaches_target);
    RUN_TEST(test_spring_overshoots);
    RUN_TEST(test_spring_settles);
    RUN_TEST(test_settled_value);
    RUN_TEST(test_done_when_settled);
    RUN_TEST(test_not_done_during_spring);
    RUN_TEST(test_active_during_transition);
    RUN_TEST(test_not_active_when_idle);
    RUN_TEST(test_cancel_stops);
    RUN_TEST(test_start_default_anticipation);
    RUN_TEST(test_zero_anticipation);
    return UNITY_END();
}
