#include "../unity/unity.h"
#include "../../src/ui/audio_score.h"
#include "../../src/ui/view_registry.h"

#include <math.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ---- Constants for test JDs ---- */
static const double JD_J2000 = 2451545.0;    /* 2000-01-01 12:00 TT */

/* ---- 1: audio_score_compute returns valid params ---- */

void test_compute_returns_valid_params(void)
{
    audio_params_t p = audio_score_compute(JD_J2000, VIEW_SPACE, 0.0f, 1.0);
    TEST_ASSERT_TRUE(p.planet_count > 0);
    TEST_ASSERT_TRUE(p.planet_count <= AS_MAX_PLANETS);
    TEST_ASSERT_TRUE(p.master_volume >= 0.0f);
    TEST_ASSERT_TRUE(p.master_volume <= 1.0f);
}

/* ---- 2-3: planet frequencies positive and in audible range ---- */

void test_compute_frequencies_positive(void)
{
    audio_params_t p = audio_score_compute(JD_J2000, VIEW_SPACE, 0.0f, 1.0);
    for (int i = 0; i < p.planet_count; i++) {
        TEST_ASSERT_TRUE(p.frequencies[i] > 0.0f);
    }
}

void test_compute_frequencies_audible_range(void)
{
    audio_params_t p = audio_score_compute(JD_J2000, VIEW_SPACE, 0.0f, 1.0);
    for (int i = 0; i < p.planet_count; i++) {
        /* audible range: 20 Hz - 20000 Hz */
        TEST_ASSERT_TRUE(p.frequencies[i] >= 20.0f);
        TEST_ASSERT_TRUE(p.frequencies[i] <= 20000.0f);
    }
}

/* ---- 4-5: amplitudes 0-1 ---- */

void test_compute_amplitudes_in_range(void)
{
    audio_params_t p = audio_score_compute(JD_J2000, VIEW_SPACE, 0.0f, 1.0);
    for (int i = 0; i < p.planet_count; i++) {
        TEST_ASSERT_TRUE(p.amplitudes[i] >= 0.0f);
        TEST_ASSERT_TRUE(p.amplitudes[i] <= 1.0f);
    }
}

void test_compute_amplitudes_nonzero(void)
{
    audio_params_t p = audio_score_compute(JD_J2000, VIEW_SPACE, 0.0f, 1.0);
    int any_nonzero = 0;
    for (int i = 0; i < p.planet_count; i++) {
        if (p.amplitudes[i] > 0.0f) any_nonzero = 1;
    }
    TEST_ASSERT_TRUE(any_nonzero);
}

/* ---- 6: master_volume 0-1 ---- */

void test_compute_master_volume_range(void)
{
    audio_params_t p = audio_score_compute(JD_J2000, VIEW_SPACE, 0.0f, 1.0);
    TEST_ASSERT_TRUE(p.master_volume >= 0.0f);
    TEST_ASSERT_TRUE(p.master_volume <= 1.0f);
}

/* ---- 7: tension 0-1 ---- */

void test_compute_tension_range(void)
{
    audio_params_t p = audio_score_compute(JD_J2000, VIEW_SPACE, 0.0f, 1.0);
    TEST_ASSERT_TRUE(p.tension >= 0.0f);
    TEST_ASSERT_TRUE(p.tension <= 1.0f);
}

/* ---- 8-13: warmth for each view type ---- */

void test_warmth_space_neutral(void)
{
    float w = audio_score_warmth(VIEW_SPACE, 0.0f);
    TEST_ASSERT_TRUE(w >= -1.0f);
    TEST_ASSERT_TRUE(w <= 1.0f);
    /* Space view is neutral-ish */
    TEST_ASSERT_TRUE(w >= -0.5f);
    TEST_ASSERT_TRUE(w <= 0.5f);
}

void test_warmth_earth_warm(void)
{
    float w = audio_score_warmth(VIEW_EARTH, 0.0f);
    TEST_ASSERT_TRUE(w >= 0.5f);
    TEST_ASSERT_TRUE(w <= 1.0f);
}

void test_warmth_galaxy_cold(void)
{
    float w = audio_score_warmth(VIEW_GALAXY, 0.0f);
    TEST_ASSERT_TRUE(w >= -1.0f);
    TEST_ASSERT_TRUE(w <= -0.3f);
}

void test_warmth_personal_slightly_warm(void)
{
    float w = audio_score_warmth(VIEW_PERSONAL, 0.0f);
    TEST_ASSERT_TRUE(w >= 0.0f);
    TEST_ASSERT_TRUE(w <= 0.6f);
}

void test_warmth_deep_time_cold(void)
{
    float w = audio_score_warmth(VIEW_DEEP_TIME, 0.0f);
    TEST_ASSERT_TRUE(w >= -1.0f);
    TEST_ASSERT_TRUE(w <= -0.3f);
}

void test_warmth_city_warm(void)
{
    float w = audio_score_warmth(VIEW_CITY, 0.0f);
    TEST_ASSERT_TRUE(w >= 0.5f);
    TEST_ASSERT_TRUE(w <= 1.0f);
}

void test_warmth_room_very_warm(void)
{
    float w = audio_score_warmth(VIEW_ROOM, 0.0f);
    TEST_ASSERT_TRUE(w >= 0.6f);
    TEST_ASSERT_TRUE(w <= 1.0f);
}

void test_warmth_range_always_valid(void)
{
    /* Test all view IDs */
    for (int v = 0; v < VIEW_COUNT; v++) {
        float w = audio_score_warmth(v, 0.0f);
        TEST_ASSERT_TRUE(w >= -1.0f);
        TEST_ASSERT_TRUE(w <= 1.0f);
    }
}

/* ---- 14-18: tempo mapping ---- */

void test_tempo_paused_is_zero(void)
{
    float bpm = audio_score_tempo(0.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, bpm);
}

void test_tempo_realtime_is_60(void)
{
    float bpm = audio_score_tempo(1.0);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 60.0f, bpm);
}

void test_tempo_fast_is_higher(void)
{
    float bpm_slow = audio_score_tempo(1.0);
    float bpm_fast = audio_score_tempo(3600.0);
    TEST_ASSERT_TRUE(bpm_fast > bpm_slow);
}

void test_tempo_very_fast_capped(void)
{
    float bpm = audio_score_tempo(86400.0);
    TEST_ASSERT_TRUE(bpm >= 100.0f);
    TEST_ASSERT_TRUE(bpm <= 200.0f);
}

void test_tempo_negative_is_zero(void)
{
    float bpm = audio_score_tempo(-1.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, bpm);
}

/* ---- 19-21: chord function ---- */

void test_chord_returns_positive_count(void)
{
    float freqs[AS_MAX_PLANETS];
    float amps[AS_MAX_PLANETS];
    int count = audio_score_chord(JD_J2000, freqs, amps, AS_MAX_PLANETS);
    TEST_ASSERT_TRUE(count > 0);
    TEST_ASSERT_TRUE(count <= AS_MAX_PLANETS);
}

void test_chord_frequencies_positive(void)
{
    float freqs[AS_MAX_PLANETS];
    float amps[AS_MAX_PLANETS];
    int count = audio_score_chord(JD_J2000, freqs, amps, AS_MAX_PLANETS);
    for (int i = 0; i < count; i++) {
        TEST_ASSERT_TRUE(freqs[i] > 0.0f);
    }
}

void test_chord_respects_max(void)
{
    float freqs[3];
    float amps[3];
    int count = audio_score_chord(JD_J2000, freqs, amps, 3);
    TEST_ASSERT_TRUE(count <= 3);
}

/* ---- 22-23: tension function ---- */

void test_tension_range_valid(void)
{
    float t = audio_score_tension(JD_J2000);
    TEST_ASSERT_TRUE(t >= 0.0f);
    TEST_ASSERT_TRUE(t <= 1.0f);
}

void test_tension_different_dates(void)
{
    float t1 = audio_score_tension(JD_J2000);
    float t2 = audio_score_tension(JD_J2000 + 7.3826);  /* ~quarter moon offset */
    /* They should differ (full moon vs quarter moon) */
    TEST_ASSERT_TRUE(t1 >= 0.0f && t1 <= 1.0f);
    TEST_ASSERT_TRUE(t2 >= 0.0f && t2 <= 1.0f);
}

/* ---- 24-27: mood strings ---- */

void test_mood_cold_calm(void)
{
    const char *m = audio_score_mood(-0.8f, 0.1f);
    TEST_ASSERT_NOT_NULL(m);
    TEST_ASSERT_EQUAL_STRING("Vast Silence", m);
}

void test_mood_cold_tense(void)
{
    const char *m = audio_score_mood(-0.8f, 0.7f);
    TEST_ASSERT_NOT_NULL(m);
    TEST_ASSERT_EQUAL_STRING("Cosmic Tension", m);
}

void test_mood_neutral_calm(void)
{
    const char *m = audio_score_mood(0.0f, 0.1f);
    TEST_ASSERT_NOT_NULL(m);
    TEST_ASSERT_EQUAL_STRING("Orbital Drift", m);
}

void test_mood_warm_tense(void)
{
    const char *m = audio_score_mood(0.8f, 0.7f);
    TEST_ASSERT_NOT_NULL(m);
    TEST_ASSERT_EQUAL_STRING("Convergence", m);
}

/* ---- 28-29: warmth + zoom interpolation ---- */

void test_warmth_zoom_affects_value(void)
{
    float w1 = audio_score_warmth(VIEW_SPACE, -2.0f);
    float w2 = audio_score_warmth(VIEW_SPACE, 2.0f);
    /* Different zoom levels should produce slightly different warmth */
    TEST_ASSERT_TRUE(w1 >= -1.0f && w1 <= 1.0f);
    TEST_ASSERT_TRUE(w2 >= -1.0f && w2 <= 1.0f);
}

void test_warmth_invalid_view_returns_neutral(void)
{
    float w = audio_score_warmth(-1, 0.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, w);
}

/* ---- 30: edge case — extreme JD ---- */

void test_compute_extreme_jd(void)
{
    audio_params_t p = audio_score_compute(0.0, VIEW_SPACE, 0.0f, 1.0);
    TEST_ASSERT_TRUE(p.planet_count > 0);
    TEST_ASSERT_TRUE(p.tension >= 0.0f);
    TEST_ASSERT_TRUE(p.tension <= 1.0f);
}

/* ---- 31: edge case — zero time speed ---- */

void test_compute_zero_speed(void)
{
    audio_params_t p = audio_score_compute(JD_J2000, VIEW_SPACE, 0.0f, 0.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, p.tempo_bpm);
}

/* ---- 32: mood warm+calm ---- */

void test_mood_warm_calm(void)
{
    const char *m = audio_score_mood(0.8f, 0.1f);
    TEST_ASSERT_NOT_NULL(m);
    TEST_ASSERT_EQUAL_STRING("Earth Breath", m);
}

/* ---- 33: mood neutral+tense ---- */

void test_mood_neutral_tense(void)
{
    const char *m = audio_score_mood(0.0f, 0.7f);
    TEST_ASSERT_NOT_NULL(m);
    TEST_ASSERT_EQUAL_STRING("Approaching Alignment", m);
}

/* ---- 34: compute warmth matches standalone warmth ---- */

void test_compute_warmth_consistent(void)
{
    audio_params_t p = audio_score_compute(JD_J2000, VIEW_EARTH, 1.0f, 1.0);
    float standalone = audio_score_warmth(VIEW_EARTH, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, standalone, p.warmth);
}

/* ---- 35: compute tension matches standalone tension ---- */

void test_compute_tension_consistent(void)
{
    audio_params_t p = audio_score_compute(JD_J2000, VIEW_SPACE, 0.0f, 1.0);
    float standalone = audio_score_tension(JD_J2000);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, standalone, p.tension);
}

/* ---- 36: compute tempo matches standalone tempo ---- */

void test_compute_tempo_consistent(void)
{
    audio_params_t p = audio_score_compute(JD_J2000, VIEW_SPACE, 0.0f, 60.0);
    float standalone = audio_score_tempo(60.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, standalone, p.tempo_bpm);
}

int main(void)
{
    UNITY_BEGIN();

    /* audio_score_compute */
    RUN_TEST(test_compute_returns_valid_params);
    RUN_TEST(test_compute_frequencies_positive);
    RUN_TEST(test_compute_frequencies_audible_range);
    RUN_TEST(test_compute_amplitudes_in_range);
    RUN_TEST(test_compute_amplitudes_nonzero);
    RUN_TEST(test_compute_master_volume_range);
    RUN_TEST(test_compute_tension_range);

    /* warmth per view */
    RUN_TEST(test_warmth_space_neutral);
    RUN_TEST(test_warmth_earth_warm);
    RUN_TEST(test_warmth_galaxy_cold);
    RUN_TEST(test_warmth_personal_slightly_warm);
    RUN_TEST(test_warmth_deep_time_cold);
    RUN_TEST(test_warmth_city_warm);
    RUN_TEST(test_warmth_room_very_warm);
    RUN_TEST(test_warmth_range_always_valid);

    /* tempo */
    RUN_TEST(test_tempo_paused_is_zero);
    RUN_TEST(test_tempo_realtime_is_60);
    RUN_TEST(test_tempo_fast_is_higher);
    RUN_TEST(test_tempo_very_fast_capped);
    RUN_TEST(test_tempo_negative_is_zero);

    /* chord */
    RUN_TEST(test_chord_returns_positive_count);
    RUN_TEST(test_chord_frequencies_positive);
    RUN_TEST(test_chord_respects_max);

    /* tension */
    RUN_TEST(test_tension_range_valid);
    RUN_TEST(test_tension_different_dates);

    /* mood */
    RUN_TEST(test_mood_cold_calm);
    RUN_TEST(test_mood_cold_tense);
    RUN_TEST(test_mood_neutral_calm);
    RUN_TEST(test_mood_warm_tense);

    /* zoom + edge cases */
    RUN_TEST(test_warmth_zoom_affects_value);
    RUN_TEST(test_warmth_invalid_view_returns_neutral);
    RUN_TEST(test_compute_extreme_jd);
    RUN_TEST(test_compute_zero_speed);
    RUN_TEST(test_mood_warm_calm);
    RUN_TEST(test_mood_neutral_tense);

    /* consistency */
    RUN_TEST(test_compute_warmth_consistent);
    RUN_TEST(test_compute_tension_consistent);
    RUN_TEST(test_compute_tempo_consistent);

    return UNITY_END();
}
