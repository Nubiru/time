/* test_audio_vis_data.c -- Tests for audio visualization data module. */

#include "unity.h"
#include "ui/audio_vis_data.h"

#include <string.h>
#include <math.h>

void setUp(void) {}
void tearDown(void) {}

/* --- av_ring_breath tests --- */

void test_av_ring_breath_returns_around_one(void)
{
    float b = av_ring_breath(440.0f, 0.5f, 0.0);
    TEST_ASSERT_TRUE(b >= 0.8f && b <= 1.2f);
}

void test_av_ring_breath_zero_freq(void)
{
    float b = av_ring_breath(0.0f, 0.5f, 1.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, b);
}

void test_av_ring_breath_zero_amp(void)
{
    float b = av_ring_breath(440.0f, 0.0f, 1.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, b);
}

void test_av_ring_breath_varies_with_time(void)
{
    float b1 = av_ring_breath(1.0f, 1.0f, 0.0);
    float b2 = av_ring_breath(1.0f, 1.0f, 0.25);
    /* At 1 Hz, 0.25s is quarter cycle — values should differ */
    TEST_ASSERT_TRUE(fabsf(b1 - b2) > 0.01f);
}

void test_av_ring_breath_louder_means_more_motion(void)
{
    /* At peak time (t=0 for cosine), amplitude should scale the deviation */
    float b_quiet = av_ring_breath(1.0f, 0.2f, 0.0);
    float b_loud = av_ring_breath(1.0f, 0.8f, 0.0);
    float dev_quiet = fabsf(b_quiet - 1.0f);
    float dev_loud = fabsf(b_loud - 1.0f);
    TEST_ASSERT_TRUE(dev_loud >= dev_quiet);
}

/* --- av_aspect_glow tests --- */

void test_av_aspect_glow_zero(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, av_aspect_glow(0.0f));
}

void test_av_aspect_glow_full(void)
{
    float g = av_aspect_glow(1.0f);
    TEST_ASSERT_TRUE(g > 0.5f);
}

void test_av_aspect_glow_monotonic(void)
{
    float g1 = av_aspect_glow(0.3f);
    float g2 = av_aspect_glow(0.7f);
    TEST_ASSERT_TRUE(g2 >= g1);
}

/* --- av_convergence_alpha tests --- */

void test_av_convergence_alpha_zero(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, av_convergence_alpha(0.0f, 0.0f));
}

void test_av_convergence_alpha_max(void)
{
    float a = av_convergence_alpha(1.0f, 1.0f);
    TEST_ASSERT_TRUE(a > 0.0f && a <= 0.6f);
}

void test_av_convergence_alpha_scales_with_density(void)
{
    float a1 = av_convergence_alpha(0.2f, 0.5f);
    float a2 = av_convergence_alpha(0.8f, 0.5f);
    TEST_ASSERT_TRUE(a2 >= a1);
}

/* --- av_from_params tests --- */

void test_av_from_params_null(void)
{
    av_snapshot_t snap = av_from_params(NULL, 0.0);
    TEST_ASSERT_EQUAL_INT(0, snap.ring_count);
}

void test_av_from_params_ring_count(void)
{
    audio_params_t params;
    memset(&params, 0, sizeof(params));
    params.planet_count = 4;
    params.frequencies[0] = 200.0f;
    params.frequencies[1] = 300.0f;
    params.frequencies[2] = 400.0f;
    params.frequencies[3] = 500.0f;
    params.amplitudes[0] = 0.5f;
    params.amplitudes[1] = 0.4f;
    params.amplitudes[2] = 0.3f;
    params.amplitudes[3] = 0.2f;

    av_snapshot_t snap = av_from_params(&params, 0.0);
    TEST_ASSERT_EQUAL_INT(4, snap.ring_count);
}

void test_av_from_params_ring_opacity_range(void)
{
    audio_params_t params;
    memset(&params, 0, sizeof(params));
    params.planet_count = 1;
    params.frequencies[0] = 440.0f;
    params.amplitudes[0] = 0.6f;

    av_snapshot_t snap = av_from_params(&params, 0.0);
    TEST_ASSERT_TRUE(snap.rings[0].opacity >= 0.0f &&
                     snap.rings[0].opacity <= 1.0f);
}

void test_av_from_params_moon_halo(void)
{
    audio_params_t params;
    memset(&params, 0, sizeof(params));
    params.moon_factor = 0.8f;

    av_snapshot_t snap = av_from_params(&params, 0.0);
    TEST_ASSERT_TRUE(snap.moon_halo > 0.0f);
}

void test_av_from_params_beat_flash(void)
{
    audio_params_t params;
    memset(&params, 0, sizeof(params));
    params.pulse_factor = 0.9f;

    av_snapshot_t snap = av_from_params(&params, 0.0);
    TEST_ASSERT_TRUE(snap.beat_flash >= 0.0f && snap.beat_flash <= 0.15f);
}

void test_av_from_params_surprise_spark(void)
{
    audio_params_t params;
    memset(&params, 0, sizeof(params));
    params.surprise_factor = 0.7f;

    av_snapshot_t snap = av_from_params(&params, 0.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.7f, snap.surprise_spark);
}

void test_av_from_params_convergence(void)
{
    audio_params_t params;
    memset(&params, 0, sizeof(params));
    params.event_density = 0.6f;
    params.event_intensity = 0.8f;

    av_snapshot_t snap = av_from_params(&params, 0.0);
    TEST_ASSERT_TRUE(snap.convergence_aura > 0.0f);
    TEST_ASSERT_TRUE(snap.convergence_alpha > 0.0f);
}

void test_av_from_params_warmth_hue_shift(void)
{
    audio_params_t params;
    memset(&params, 0, sizeof(params));
    params.planet_count = 1;
    params.frequencies[0] = 440.0f;
    params.amplitudes[0] = 0.5f;
    params.warmth = 0.8f;

    av_snapshot_t snap = av_from_params(&params, 0.0);
    TEST_ASSERT_TRUE(snap.rings[0].hue_shift > 0.0f);
}

int main(void)
{
    UNITY_BEGIN();

    /* av_ring_breath */
    RUN_TEST(test_av_ring_breath_returns_around_one);
    RUN_TEST(test_av_ring_breath_zero_freq);
    RUN_TEST(test_av_ring_breath_zero_amp);
    RUN_TEST(test_av_ring_breath_varies_with_time);
    RUN_TEST(test_av_ring_breath_louder_means_more_motion);

    /* av_aspect_glow */
    RUN_TEST(test_av_aspect_glow_zero);
    RUN_TEST(test_av_aspect_glow_full);
    RUN_TEST(test_av_aspect_glow_monotonic);

    /* av_convergence_alpha */
    RUN_TEST(test_av_convergence_alpha_zero);
    RUN_TEST(test_av_convergence_alpha_max);
    RUN_TEST(test_av_convergence_alpha_scales_with_density);

    /* av_from_params */
    RUN_TEST(test_av_from_params_null);
    RUN_TEST(test_av_from_params_ring_count);
    RUN_TEST(test_av_from_params_ring_opacity_range);
    RUN_TEST(test_av_from_params_moon_halo);
    RUN_TEST(test_av_from_params_beat_flash);
    RUN_TEST(test_av_from_params_surprise_spark);
    RUN_TEST(test_av_from_params_convergence);
    RUN_TEST(test_av_from_params_warmth_hue_shift);

    return UNITY_END();
}
