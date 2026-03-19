/* test_audio_meditation.c — Tests for meditation mode computation.
 *
 * Validates phi-timed breathing, binaural offset, and full state compute. */

#include "../unity/unity.h"
#include "../../src/ui/audio_meditation.h"
#include <math.h>

void setUp(void) {}
void tearDown(void) {}

/* ---- Duration constants ---- */

void test_cycle_duration_is_phi_cubed(void)
{
    float dur = med_cycle_duration();
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 4.236f, dur);
}

void test_inhale_duration_is_phi(void)
{
    float dur = med_inhale_duration();
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.618f, dur);
}

void test_exhale_duration_is_phi_squared(void)
{
    float dur = med_exhale_duration();
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 2.618f, dur);
}

void test_inhale_plus_exhale_equals_cycle(void)
{
    float sum = med_inhale_duration() + med_exhale_duration();
    TEST_ASSERT_FLOAT_WITHIN(0.001f, med_cycle_duration(), sum);
}

/* ---- Breath factor ---- */

void test_breath_factor_at_start(void)
{
    /* t=0: start of inhale, should be near BREATH_MIN (0.3) */
    float f = med_breath_factor(0.0);
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 0.3f, f);
}

void test_breath_factor_at_inhale_peak(void)
{
    /* t=inhale_dur: end of inhale, should be near BREATH_MAX (1.0) */
    float f = med_breath_factor((double)med_inhale_duration() - 0.001);
    TEST_ASSERT_TRUE(f > 0.9f);
}

void test_breath_factor_at_exhale_end(void)
{
    /* t=cycle_dur: end of exhale = start of next cycle, near BREATH_MIN */
    float f = med_breath_factor((double)med_cycle_duration() - 0.001);
    TEST_ASSERT_TRUE(f < 0.4f);
}

void test_breath_factor_range(void)
{
    /* Over a full cycle, factor should stay in [0.3, 1.0] */
    for (int i = 0; i < 100; i++) {
        double t = (double)i * (double)med_cycle_duration() / 100.0;
        float f = med_breath_factor(t);
        TEST_ASSERT_TRUE(f >= 0.29f);
        TEST_ASSERT_TRUE(f <= 1.01f);
    }
}

void test_breath_factor_periodic(void)
{
    /* Same value at t and t + cycle_duration */
    float f1 = med_breath_factor(1.0);
    float f2 = med_breath_factor(1.0 + (double)med_cycle_duration());
    TEST_ASSERT_FLOAT_WITHIN(0.01f, f1, f2);
}

void test_breath_factor_inhale_rises(void)
{
    /* During inhale, factor should generally increase */
    float f_start = med_breath_factor(0.1);
    float f_mid = med_breath_factor((double)med_inhale_duration() * 0.5);
    float f_end = med_breath_factor((double)med_inhale_duration() - 0.1);
    TEST_ASSERT_TRUE(f_mid > f_start);
    TEST_ASSERT_TRUE(f_end > f_mid);
}

void test_breath_factor_exhale_falls(void)
{
    /* During exhale, factor should generally decrease */
    double exhale_start = (double)med_inhale_duration() + 0.1;
    double exhale_mid =
        (double)med_inhale_duration() + (double)med_exhale_duration() * 0.5;
    double exhale_end = (double)med_cycle_duration() - 0.1;
    float f_start = med_breath_factor(exhale_start);
    float f_mid = med_breath_factor(exhale_mid);
    float f_end = med_breath_factor(exhale_end);
    TEST_ASSERT_TRUE(f_start > f_mid);
    TEST_ASSERT_TRUE(f_mid > f_end);
}

/* ---- Breath phase ---- */

void test_phase_inhale_at_start(void)
{
    med_phase_t p = med_breath_phase(0.0);
    TEST_ASSERT_EQUAL_INT(MED_PHASE_INHALE, p);
}

void test_phase_exhale_after_inhale(void)
{
    double t = (double)med_inhale_duration() + 0.1;
    med_phase_t p = med_breath_phase(t);
    TEST_ASSERT_EQUAL_INT(MED_PHASE_EXHALE, p);
}

void test_phase_inhale_next_cycle(void)
{
    double t = (double)med_cycle_duration() + 0.1;
    med_phase_t p = med_breath_phase(t);
    TEST_ASSERT_EQUAL_INT(MED_PHASE_INHALE, p);
}

/* ---- Breath progress ---- */

void test_progress_start_is_zero(void)
{
    float p = med_breath_progress(0.001);
    TEST_ASSERT_TRUE(p < 0.01f);
}

void test_progress_end_of_inhale_near_one(void)
{
    float p = med_breath_progress((double)med_inhale_duration() - 0.01);
    TEST_ASSERT_TRUE(p > 0.99f);
}

void test_progress_range(void)
{
    for (int i = 0; i < 100; i++) {
        double t = (double)i * (double)med_cycle_duration() / 100.0;
        float p = med_breath_progress(t);
        TEST_ASSERT_TRUE(p >= 0.0f);
        TEST_ASSERT_TRUE(p <= 1.001f);
    }
}

/* ---- Full compute ---- */

void test_compute_active(void)
{
    med_state_t s = med_compute(1.0, 6.0f);
    TEST_ASSERT_EQUAL_INT(1, s.active);
}

void test_compute_binaural_offset(void)
{
    med_state_t s = med_compute(1.0, 6.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 6.0f, s.binaural_offset_hz);
}

void test_compute_binaural_default(void)
{
    /* If binaural_hz <= 0, defaults to 6.0 */
    med_state_t s = med_compute(1.0, 0.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 6.0f, s.binaural_offset_hz);
}

void test_compute_reverb_boost(void)
{
    med_state_t s = med_compute(1.0, 6.0f);
    TEST_ASSERT_TRUE(s.reverb_boost > 0.0f);
    TEST_ASSERT_TRUE(s.reverb_boost <= 0.5f);
}

void test_compute_dimming(void)
{
    med_state_t s = med_compute(1.0, 6.0f);
    TEST_ASSERT_TRUE(s.dimming > 0.5f);
    TEST_ASSERT_TRUE(s.dimming <= 1.0f);
}

void test_compute_breath_factor_matches(void)
{
    /* med_compute breath_factor should match standalone med_breath_factor */
    med_state_t s = med_compute(2.5, 6.0f);
    float expected = med_breath_factor(2.5);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, expected, s.breath_factor);
}

void test_compute_breath_phase_matches(void)
{
    med_state_t s = med_compute(2.5, 6.0f);
    med_phase_t expected = med_breath_phase(2.5);
    TEST_ASSERT_EQUAL_INT(expected, s.breath_phase);
}

void test_compute_breath_progress_matches(void)
{
    med_state_t s = med_compute(2.5, 6.0f);
    float expected = med_breath_progress(2.5);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, expected, s.breath_progress);
}

void test_compute_negative_binaural_defaults(void)
{
    /* Negative binaural_hz should also default to 6.0 */
    med_state_t s = med_compute(1.0, -3.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 6.0f, s.binaural_offset_hz);
}

void test_breath_factor_negative_time(void)
{
    /* Negative time should wrap correctly */
    float f = med_breath_factor(-1.0);
    TEST_ASSERT_TRUE(f >= 0.29f);
    TEST_ASSERT_TRUE(f <= 1.01f);
}

int main(void)
{
    UNITY_BEGIN();
    /* durations */
    RUN_TEST(test_cycle_duration_is_phi_cubed);
    RUN_TEST(test_inhale_duration_is_phi);
    RUN_TEST(test_exhale_duration_is_phi_squared);
    RUN_TEST(test_inhale_plus_exhale_equals_cycle);
    /* breath factor */
    RUN_TEST(test_breath_factor_at_start);
    RUN_TEST(test_breath_factor_at_inhale_peak);
    RUN_TEST(test_breath_factor_at_exhale_end);
    RUN_TEST(test_breath_factor_range);
    RUN_TEST(test_breath_factor_periodic);
    RUN_TEST(test_breath_factor_inhale_rises);
    RUN_TEST(test_breath_factor_exhale_falls);
    /* phase */
    RUN_TEST(test_phase_inhale_at_start);
    RUN_TEST(test_phase_exhale_after_inhale);
    RUN_TEST(test_phase_inhale_next_cycle);
    /* progress */
    RUN_TEST(test_progress_start_is_zero);
    RUN_TEST(test_progress_end_of_inhale_near_one);
    RUN_TEST(test_progress_range);
    /* compute */
    RUN_TEST(test_compute_active);
    RUN_TEST(test_compute_binaural_offset);
    RUN_TEST(test_compute_binaural_default);
    RUN_TEST(test_compute_reverb_boost);
    RUN_TEST(test_compute_dimming);
    RUN_TEST(test_compute_breath_factor_matches);
    RUN_TEST(test_compute_breath_phase_matches);
    RUN_TEST(test_compute_breath_progress_matches);
    RUN_TEST(test_compute_negative_binaural_defaults);
    RUN_TEST(test_breath_factor_negative_time);
    return UNITY_END();
}
