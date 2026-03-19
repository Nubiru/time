/* test_audio_pipeline.c -- Integration test for the full audio pipeline.
 *
 * Exercises the complete data flow:
 *   audio_score_compute() → pianist_score → midi_output → audio_vis_data → audio_a11y
 *
 * Verifies all audio modules produce consistent, non-degenerate output
 * when fed realistic Julian Day / view / zoom / time speed inputs. */

#include "unity.h"
#include "ui/audio_score.h"
#include "ui/pianist_score.h"
#include "ui/midi_output.h"
#include "ui/audio_vis_data.h"
#include "ui/audio_a11y.h"
#include "ui/audio_event.h"
#include "ui/audio_culture.h"
#include "ui/audio_meditation.h"
#include "ui/audio_queue.h"

#include <string.h>
#include <math.h>

void setUp(void) {}
void tearDown(void) {}

/* J2000.0 epoch for test dates */
static const double JD_J2000 = 2451545.0;

/* --- Full pipeline: score → pianist → midi → vis → a11y --- */

void test_pipeline_space_view(void)
{
    /* VIEW_SPACE=0, normal zoom, real-time speed */
    audio_params_t params = audio_score_compute(JD_J2000, 0, 0.0f, 1.0);

    /* Score should have planets */
    TEST_ASSERT_TRUE(params.planet_count > 0);
    TEST_ASSERT_TRUE(params.master_volume > 0.0f);

    /* Pianist score */
    ps_score_t ps = ps_from_params(&params);
    TEST_ASSERT_EQUAL_INT(params.planet_count, ps.note_count);
    TEST_ASSERT_NOT_NULL(ps.mood);
    TEST_ASSERT_NOT_NULL(ps.dynamics);
    TEST_ASSERT_NOT_NULL(ps.tempo_mark);
    TEST_ASSERT_TRUE(ps.root_name[0] != '\0');

    /* MIDI output */
    midi_snapshot_t midi = midi_from_params(&params);
    TEST_ASSERT_EQUAL_INT(params.planet_count, midi.note_count);
    for (int i = 0; i < midi.note_count; i++) {
        TEST_ASSERT_TRUE(midi.notes[i].note >= 0);
        TEST_ASSERT_TRUE(midi.notes[i].note <= 127);
        TEST_ASSERT_TRUE(midi.notes[i].velocity >= 0);
        TEST_ASSERT_TRUE(midi.notes[i].velocity <= 127);
    }
    TEST_ASSERT_EQUAL_INT(1, midi.tension_cc.cc);
    TEST_ASSERT_EQUAL_INT(74, midi.warmth_cc.cc);

    /* Audio visualization */
    av_snapshot_t vis = av_from_params(&params, 0.0);
    TEST_ASSERT_EQUAL_INT(params.planet_count, vis.ring_count);
    for (int i = 0; i < vis.ring_count; i++) {
        TEST_ASSERT_TRUE(vis.rings[i].scale >= 0.8f);
        TEST_ASSERT_TRUE(vis.rings[i].scale <= 1.2f);
        TEST_ASSERT_TRUE(vis.rings[i].opacity >= 0.0f);
        TEST_ASSERT_TRUE(vis.rings[i].opacity <= 1.0f);
    }

    /* Accessibility narrative */
    char buf[AA_MAX_LEN];
    int n = aa_chord_desc(&ps, buf, AA_MAX_LEN);
    TEST_ASSERT_TRUE(n > 0);
    n = aa_mood_desc(&ps, buf, AA_MAX_LEN);
    TEST_ASSERT_TRUE(n > 0);
}

void test_pipeline_earth_view(void)
{
    /* VIEW_EARTH=1, zoomed in, faster time */
    audio_params_t params = audio_score_compute(JD_J2000 + 100.0,
                                                 1, 2.0f, 3600.0);

    /* Warmer view should shift warmth positive */
    TEST_ASSERT_TRUE(params.warmth > 0.0f);

    /* Pianist should reflect warmth */
    ps_score_t ps = ps_from_params(&params);
    TEST_ASSERT_TRUE(ps.warmth > 0.0f);

    /* MIDI warmth CC should be above center (64) */
    midi_snapshot_t midi = midi_from_params(&params);
    TEST_ASSERT_TRUE(midi.warmth_cc.value > 64);

    /* Faster time = higher tempo */
    TEST_ASSERT_TRUE(params.tempo_bpm > 60.0f);
}

void test_pipeline_galaxy_view(void)
{
    /* VIEW_GALAXY=2, far zoom, paused */
    audio_params_t params = audio_score_compute(JD_J2000, 2, -5.0f, 0.0);

    /* Cold view */
    TEST_ASSERT_TRUE(params.warmth < 0.0f);
    /* Paused = 0 BPM */
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, params.tempo_bpm);

    ps_score_t ps = ps_from_params(&params);
    TEST_ASSERT_EQUAL_STRING("Tacet", ps.tempo_mark);

    /* MIDI warmth below center */
    midi_snapshot_t midi = midi_from_params(&params);
    TEST_ASSERT_TRUE(midi.warmth_cc.value < 64);
}

/* --- Cross-module consistency --- */

void test_pipeline_note_count_consistent(void)
{
    audio_params_t params = audio_score_compute(JD_J2000 + 500.0,
                                                 0, 0.0f, 1.0);
    ps_score_t ps = ps_from_params(&params);
    midi_snapshot_t midi = midi_from_params(&params);
    av_snapshot_t vis = av_from_params(&params, 1.0);

    /* All modules should agree on count */
    TEST_ASSERT_EQUAL_INT(params.planet_count, ps.note_count);
    TEST_ASSERT_EQUAL_INT(params.planet_count, midi.note_count);
    TEST_ASSERT_EQUAL_INT(params.planet_count, vis.ring_count);
}

void test_pipeline_tension_flows_through(void)
{
    audio_params_t params = audio_score_compute(JD_J2000, 2, 0.0f, 1.0);
    ps_score_t ps = ps_from_params(&params);
    midi_snapshot_t midi = midi_from_params(&params);

    /* Tension should be consistent across modules */
    TEST_ASSERT_FLOAT_WITHIN(0.01f, params.tension, ps.tension);
    /* MIDI tension CC value should correspond to params.tension */
    int expected_cc = (int)roundf(params.tension * 127.0f);
    TEST_ASSERT_INT_WITHIN(2, expected_cc, midi.tension_cc.value);
}

void test_pipeline_consonance_flows_through(void)
{
    audio_params_t params = audio_score_compute(JD_J2000, 2, 0.0f, 1.0);
    ps_score_t ps = ps_from_params(&params);
    av_snapshot_t vis = av_from_params(&params, 0.0);

    TEST_ASSERT_FLOAT_WITHIN(0.01f, params.consonance, ps.consonance);
    /* Aspect glow should scale with consonance */
    float expected_glow = params.consonance * params.consonance;
    TEST_ASSERT_FLOAT_WITHIN(0.02f, expected_glow, vis.aspect_line.glow);
}

/* --- Event pipeline --- */

void test_pipeline_events_to_queue(void)
{
    /* Scan for audio events, then queue them */
    audio_event_summary_t summary = audio_event_scan(JD_J2000, 8.0);

    aq_queue_t q;
    aq_init(&q);

    /* If there are aspects, queue note events */
    if (summary.aspect_count > 0) {
        aq_event_t evt = aq_note_on(0, 440.0f,
                                     summary.consonance, 0.0);
        TEST_ASSERT_EQUAL_INT(1, aq_push(&q, &evt));
        TEST_ASSERT_EQUAL_INT(1, aq_count(&q));
    }

    /* Queue a ramp based on event intensity */
    aq_event_t ramp = aq_ramp(0, AQ_TARGET_VOLUME,
                               summary.event_intensity,
                               0.5f, 0.0);
    aq_push(&q, &ramp);
    TEST_ASSERT_TRUE(aq_count(&q) >= 1);
}

/* --- A11y describes real state --- */

void test_pipeline_a11y_real_state(void)
{
    audio_params_t params = audio_score_compute(JD_J2000, 2, 0.0f, 1.0);
    ps_score_t ps = ps_from_params(&params);

    char chord_buf[AA_MAX_LEN];
    char mood_buf[AA_MAX_LEN];
    char tension_buf[AA_MAX_LEN];
    char moon_buf[AA_MAX_LEN];

    aa_chord_desc(&ps, chord_buf, AA_MAX_LEN);
    aa_mood_desc(&ps, mood_buf, AA_MAX_LEN);
    aa_tension_desc(params.tension, params.consonance,
                    tension_buf, AA_MAX_LEN);
    aa_moon_desc(params.moon_factor, moon_buf, AA_MAX_LEN);

    /* All descriptions should be non-empty */
    TEST_ASSERT_TRUE(strlen(chord_buf) > 0);
    TEST_ASSERT_TRUE(strlen(mood_buf) > 0);
    TEST_ASSERT_TRUE(strlen(tension_buf) > 0);
    TEST_ASSERT_TRUE(strlen(moon_buf) > 0);

    /* Mood description should contain the actual mood */
    TEST_ASSERT_NOT_NULL(strstr(mood_buf, ps.mood));
}

/* --- Meditation pipeline --- */

void test_pipeline_meditation(void)
{
    med_state_t med = med_compute(1.0, 6.0f);

    /* Meditation state should be valid */
    TEST_ASSERT_TRUE(med.breath_factor >= 0.3f);
    TEST_ASSERT_TRUE(med.breath_factor <= 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 6.0f, med.binaural_offset_hz);

    /* A11y description of meditation */
    char buf[AA_MAX_LEN];
    int phase_inhaling = (med.breath_phase == MED_PHASE_INHALE) ? 1 : 0;
    int n = aa_meditation_desc(1, phase_inhaling,
                                med_cycle_duration(), buf, AA_MAX_LEN);
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "active"));
}

/* --- Culture timbres available --- */

void test_pipeline_culture_timbres(void)
{
    /* Verify cultural timbres exist and have valid data */
    int count = audio_culture_count();
    TEST_ASSERT_TRUE(count >= 6); /* at least 6 cultural timbres */

    /* Each supported system should have non-zero partials */
    for (int sys = 0; sys < 16; sys++) {
        if (audio_culture_has_timbre(sys)) {
            audio_culture_t culture = audio_culture_get(sys);
            TEST_ASSERT_TRUE(culture.partial_count > 0);
            TEST_ASSERT_TRUE(culture.base_freq_hz > 0.0f);
            TEST_ASSERT_NOT_NULL(culture.name);
        }
    }
}

/* --- Change detection --- */

void test_pipeline_change_detection(void)
{
    audio_params_t prev = audio_score_compute(JD_J2000, 2, 0.0f, 1.0);
    audio_params_t curr = audio_score_compute(JD_J2000 + 15.0, 2, 0.0f, 1.0);

    char buf[AA_MAX_LEN];
    int n = aa_change_desc(&prev, &curr, buf, AA_MAX_LEN);
    TEST_ASSERT_TRUE(n > 0);
    /* Should produce some description (may be "No significant change"
     * if 15 days doesn't shift tension enough) */
    TEST_ASSERT_TRUE(strlen(buf) > 0);
}

int main(void)
{
    UNITY_BEGIN();

    /* Full pipeline */
    RUN_TEST(test_pipeline_space_view);
    RUN_TEST(test_pipeline_earth_view);
    RUN_TEST(test_pipeline_galaxy_view);

    /* Cross-module consistency */
    RUN_TEST(test_pipeline_note_count_consistent);
    RUN_TEST(test_pipeline_tension_flows_through);
    RUN_TEST(test_pipeline_consonance_flows_through);

    /* Event pipeline */
    RUN_TEST(test_pipeline_events_to_queue);

    /* A11y */
    RUN_TEST(test_pipeline_a11y_real_state);

    /* Meditation */
    RUN_TEST(test_pipeline_meditation);

    /* Culture */
    RUN_TEST(test_pipeline_culture_timbres);

    /* Change detection */
    RUN_TEST(test_pipeline_change_detection);

    return UNITY_END();
}
