/* tests/ui/test_convergence_motion.c — convergence_motion TDD tests */

#include "../../tests/unity/unity.h"
#include "../../src/ui/convergence_motion.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void setUp(void) {}
void tearDown(void) {}

/* ---- Creation ---- */

void test_cm_create_idle(void)
{
    convergence_motion_t cm = cm_create();
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, cm.intensity);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, cm.warmth_target);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, cm.phase);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, cm.elapsed);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, cm.glow_spring.position);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, cm.warmth_spring.position);
    TEST_ASSERT_EQUAL_INT(0, cm_active(cm));
}

/* ---- Update ---- */

void test_cm_update_with_intensity(void)
{
    convergence_motion_t cm = cm_create();
    cm = cm_update(cm, 0.8f, 0.0f, 1.0f / 60.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.8f, cm.intensity);
    /* glow spring should be targeting 0.8 */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.8f, cm.glow_spring.target);
}

void test_cm_update_advances_phase(void)
{
    convergence_motion_t cm = cm_create();
    float dt = 1.0f / 60.0f;
    cm = cm_update(cm, 0.5f, 0.0f, dt);
    float expected_phase = (2.0f * (float)M_PI / CM_PULSE_PERIOD) * dt;
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, expected_phase, cm.phase);
}

/* ---- Pulse ---- */

void test_cm_pulse_zero_when_idle(void)
{
    convergence_motion_t cm = cm_create();
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, cm_pulse(cm));
}

void test_cm_pulse_oscillates(void)
{
    convergence_motion_t cm = cm_create();
    float dt = 1.0f / 60.0f;

    /* Run for a bit with full intensity */
    float values[60];
    for (int i = 0; i < 60; i++) {
        cm = cm_update(cm, 1.0f, 0.0f, dt);
        values[i] = cm_pulse(cm);
    }

    /* Verify not all values are the same (oscillation) */
    int different = 0;
    for (int i = 1; i < 60; i++) {
        if (fabsf(values[i] - values[0]) > 0.001f) {
            different = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(different);
}

void test_cm_pulse_range(void)
{
    convergence_motion_t cm = cm_create();
    float dt = 1.0f / 60.0f;

    /* Run for several full periods */
    for (int i = 0; i < 300; i++) {
        cm = cm_update(cm, 1.0f, 0.0f, dt);
        float p = cm_pulse(cm);
        TEST_ASSERT_TRUE(p >= -0.001f);
        TEST_ASSERT_TRUE(p <= 1.001f);
    }
}

/* ---- Glow ---- */

void test_cm_glow_tracks_intensity(void)
{
    convergence_motion_t cm = cm_create();
    float dt = 1.0f / 60.0f;

    /* Run spring for 2 seconds toward intensity=0.7 */
    for (int i = 0; i < 120; i++) {
        cm = cm_update(cm, 0.7f, 0.0f, dt);
    }

    /* After 2s with stiffness=120, damping=18, should be close to target */
    float glow = cm_glow(cm);
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 0.7f, glow);
}

void test_cm_glow_smooth(void)
{
    convergence_motion_t cm = cm_create();
    float dt = 1.0f / 60.0f;

    /* First frame: glow should NOT jump to target */
    cm = cm_update(cm, 1.0f, 0.0f, dt);
    float glow = cm_glow(cm);
    TEST_ASSERT_TRUE(glow < 0.5f); /* spring can't reach 1.0 in one frame */
}

void test_cm_glow_zero_when_idle(void)
{
    convergence_motion_t cm = cm_create();
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, cm_glow(cm));
}

/* ---- Shimmer ---- */

void test_cm_shimmer_centered(void)
{
    convergence_motion_t cm = cm_create();
    float dt = 1.0f / 60.0f;
    float sum = 0.0f;
    int n = 300;

    for (int i = 0; i < n; i++) {
        cm = cm_update(cm, 1.0f, 0.0f, dt);
        sum += cm_shimmer(cm);
    }

    /* Average should be near 0.5 (centered oscillation) */
    float avg = sum / (float)n;
    TEST_ASSERT_FLOAT_WITHIN(0.15f, 0.5f, avg);
}

void test_cm_shimmer_amplitude(void)
{
    convergence_motion_t cm = cm_create();
    float dt = 1.0f / 60.0f;

    for (int i = 0; i < 300; i++) {
        cm = cm_update(cm, 1.0f, 0.0f, dt);
        float s = cm_shimmer(cm);
        /* Should stay within 0.5 +/- CM_SHIMMER_AMPLITUDE */
        TEST_ASSERT_TRUE(s >= 0.5f - CM_SHIMMER_AMPLITUDE - 0.01f);
        TEST_ASSERT_TRUE(s <= 0.5f + CM_SHIMMER_AMPLITUDE + 0.01f);
    }
}

/* ---- Warmth ---- */

void test_cm_warmth_tracks_target(void)
{
    convergence_motion_t cm = cm_create();
    float dt = 1.0f / 60.0f;

    for (int i = 0; i < 180; i++) {
        cm = cm_update(cm, 0.5f, 0.6f, dt);
    }

    float w = cm_warmth(cm);
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 0.6f, w);
}

void test_cm_warmth_smooth(void)
{
    convergence_motion_t cm = cm_create();
    float dt = 1.0f / 60.0f;

    cm = cm_update(cm, 0.5f, 1.0f, dt);
    float w = cm_warmth(cm);
    TEST_ASSERT_TRUE(w < 0.5f); /* spring can't jump to 1.0 in one frame */
}

void test_cm_warmth_zero_when_idle(void)
{
    convergence_motion_t cm = cm_create();
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, cm_warmth(cm));
}

/* ---- Active state ---- */

void test_cm_active_when_converging(void)
{
    convergence_motion_t cm = cm_create();
    cm = cm_update(cm, 0.5f, 0.0f, 1.0f / 60.0f);
    TEST_ASSERT_EQUAL_INT(1, cm_active(cm));
}

void test_cm_active_during_decay(void)
{
    convergence_motion_t cm = cm_create();
    float dt = 1.0f / 60.0f;

    /* Build up some glow */
    for (int i = 0; i < 60; i++) {
        cm = cm_update(cm, 0.8f, 0.0f, dt);
    }

    /* Drop intensity to 0 */
    cm = cm_update(cm, 0.0f, 0.0f, dt);

    /* Should still be active (spring is decaying) */
    TEST_ASSERT_EQUAL_INT(1, cm_active(cm));
}

void test_cm_not_active_idle(void)
{
    convergence_motion_t cm = cm_create();
    TEST_ASSERT_EQUAL_INT(0, cm_active(cm));
}

/* ---- Ring pulse ---- */

void test_cm_ring_pulse_per_system(void)
{
    convergence_motion_t cm = cm_create();
    float dt = 1.0f / 60.0f;

    /* Advance a bit with intensity */
    for (int i = 0; i < 30; i++) {
        cm = cm_update(cm, 1.0f, 0.0f, dt);
    }

    float p0 = cm_ring_pulse(cm, 0);
    float p1 = cm_ring_pulse(cm, 1);
    float p2 = cm_ring_pulse(cm, 2);

    /* Different system_ids should give different values (not all the same) */
    int some_differ = (fabsf(p0 - p1) > 0.001f) ||
                      (fabsf(p1 - p2) > 0.001f) ||
                      (fabsf(p0 - p2) > 0.001f);
    TEST_ASSERT_TRUE(some_differ);
}

void test_cm_ring_pulse_golden_offset(void)
{
    convergence_motion_t cm = cm_create();
    float dt = 1.0f / 60.0f;

    for (int i = 0; i < 30; i++) {
        cm = cm_update(cm, 1.0f, 0.0f, dt);
    }

    /* Ring pulse for system 0 uses phase, system 1 uses phase + 0.618.
     * Verify values are in [0,1] range */
    for (int sys = 0; sys < 10; sys++) {
        float p = cm_ring_pulse(cm, sys);
        TEST_ASSERT_TRUE(p >= -0.001f);
        TEST_ASSERT_TRUE(p <= 1.001f);
    }
}

/* ---- Link shimmer ---- */

void test_cm_link_shimmer_per_link(void)
{
    convergence_motion_t cm = cm_create();
    float dt = 1.0f / 60.0f;

    for (int i = 0; i < 30; i++) {
        cm = cm_update(cm, 1.0f, 0.0f, dt);
    }

    float s0 = cm_link_shimmer(cm, 0);
    float s1 = cm_link_shimmer(cm, 1);
    float s2 = cm_link_shimmer(cm, 2);

    int some_differ = (fabsf(s0 - s1) > 0.001f) ||
                      (fabsf(s1 - s2) > 0.001f) ||
                      (fabsf(s0 - s2) > 0.001f);
    TEST_ASSERT_TRUE(some_differ);
}

/* ---- Reset ---- */

void test_cm_reset_returns_idle(void)
{
    convergence_motion_t cm = cm_create();
    float dt = 1.0f / 60.0f;

    /* Build up state */
    for (int i = 0; i < 60; i++) {
        cm = cm_update(cm, 0.9f, 0.7f, dt);
    }

    /* Reset */
    cm = cm_reset(cm);

    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, cm.intensity);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, cm.warmth_target);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, cm.glow_spring.target);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, cm.warmth_spring.target);
}

/* ---- Phase wrapping ---- */

void test_cm_phase_wraps(void)
{
    convergence_motion_t cm = cm_create();

    /* Simulate a long time to make phase large */
    float dt = 1.0f;
    for (int i = 0; i < 200; i++) {
        cm = cm_update(cm, 0.5f, 0.0f, dt);
    }

    /* Phase should have been wrapped (not growing unbounded past 100) */
    TEST_ASSERT_TRUE(cm.phase < 200.0f);
}

/* ---- Main ---- */

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_cm_create_idle);
    RUN_TEST(test_cm_update_with_intensity);
    RUN_TEST(test_cm_update_advances_phase);
    RUN_TEST(test_cm_pulse_zero_when_idle);
    RUN_TEST(test_cm_pulse_oscillates);
    RUN_TEST(test_cm_pulse_range);
    RUN_TEST(test_cm_glow_tracks_intensity);
    RUN_TEST(test_cm_glow_smooth);
    RUN_TEST(test_cm_glow_zero_when_idle);
    RUN_TEST(test_cm_shimmer_centered);
    RUN_TEST(test_cm_shimmer_amplitude);
    RUN_TEST(test_cm_warmth_tracks_target);
    RUN_TEST(test_cm_warmth_smooth);
    RUN_TEST(test_cm_warmth_zero_when_idle);
    RUN_TEST(test_cm_active_when_converging);
    RUN_TEST(test_cm_active_during_decay);
    RUN_TEST(test_cm_not_active_idle);
    RUN_TEST(test_cm_ring_pulse_per_system);
    RUN_TEST(test_cm_ring_pulse_golden_offset);
    RUN_TEST(test_cm_link_shimmer_per_link);
    RUN_TEST(test_cm_reset_returns_idle);
    RUN_TEST(test_cm_phase_wraps);
    return UNITY_END();
}
