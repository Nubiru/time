/* test_pianist_score.c -- Tests for pianist score view data module. */

#include "unity.h"
#include "ui/pianist_score.h"
#include "systems/unified/audio_data.h"
#include "systems/unified/frequency.h"

#include <string.h>
#include <math.h>

void setUp(void) {}
void tearDown(void) {}

/* --- ps_format_note tests --- */

void test_ps_format_note_a440(void)
{
    char buf[PS_NOTE_BUF];
    ps_format_note(440.0f, buf, PS_NOTE_BUF);
    TEST_ASSERT_EQUAL_STRING("A4", buf);
}

void test_ps_format_note_middle_c(void)
{
    char buf[PS_NOTE_BUF];
    ps_format_note(261.63f, buf, PS_NOTE_BUF);
    TEST_ASSERT_EQUAL_STRING("C4", buf);
}

void test_ps_format_note_low_freq(void)
{
    char buf[PS_NOTE_BUF];
    /* ~65.41 Hz = C2 */
    ps_format_note(65.41f, buf, PS_NOTE_BUF);
    TEST_ASSERT_EQUAL_STRING("C2", buf);
}

void test_ps_format_note_high_freq(void)
{
    char buf[PS_NOTE_BUF];
    /* ~1760 Hz = A6 */
    ps_format_note(1760.0f, buf, PS_NOTE_BUF);
    TEST_ASSERT_EQUAL_STRING("A6", buf);
}

void test_ps_format_note_zero(void)
{
    char buf[PS_NOTE_BUF];
    ps_format_note(0.0f, buf, PS_NOTE_BUF);
    TEST_ASSERT_EQUAL_STRING("?", buf);
}

/* --- ps_dynamics_from_volume tests --- */

void test_ps_dynamics_pp(void)
{
    TEST_ASSERT_EQUAL_STRING("pp", ps_dynamics_from_volume(0.1f));
}

void test_ps_dynamics_p(void)
{
    TEST_ASSERT_EQUAL_STRING("p", ps_dynamics_from_volume(0.25f));
}

void test_ps_dynamics_mp(void)
{
    TEST_ASSERT_EQUAL_STRING("mp", ps_dynamics_from_volume(0.4f));
}

void test_ps_dynamics_mf(void)
{
    TEST_ASSERT_EQUAL_STRING("mf", ps_dynamics_from_volume(0.55f));
}

void test_ps_dynamics_f(void)
{
    TEST_ASSERT_EQUAL_STRING("f", ps_dynamics_from_volume(0.75f));
}

void test_ps_dynamics_ff(void)
{
    TEST_ASSERT_EQUAL_STRING("ff", ps_dynamics_from_volume(0.95f));
}

/* --- ps_tempo_marking tests --- */

void test_ps_tempo_tacet(void)
{
    TEST_ASSERT_EQUAL_STRING("Tacet", ps_tempo_marking(0.0f));
}

void test_ps_tempo_largo(void)
{
    TEST_ASSERT_EQUAL_STRING("Largo", ps_tempo_marking(50.0f));
}

void test_ps_tempo_adagio(void)
{
    TEST_ASSERT_EQUAL_STRING("Adagio", ps_tempo_marking(66.0f));
}

void test_ps_tempo_andante(void)
{
    TEST_ASSERT_EQUAL_STRING("Andante", ps_tempo_marking(80.0f));
}

void test_ps_tempo_moderato(void)
{
    TEST_ASSERT_EQUAL_STRING("Moderato", ps_tempo_marking(110.0f));
}

void test_ps_tempo_allegro(void)
{
    TEST_ASSERT_EQUAL_STRING("Allegro", ps_tempo_marking(130.0f));
}

void test_ps_tempo_presto(void)
{
    TEST_ASSERT_EQUAL_STRING("Presto", ps_tempo_marking(160.0f));
}

/* --- ps_from_params tests --- */

void test_ps_from_params_null(void)
{
    ps_score_t score = ps_from_params(NULL);
    TEST_ASSERT_EQUAL_INT(0, score.note_count);
}

void test_ps_from_params_note_count(void)
{
    audio_params_t params;
    memset(&params, 0, sizeof(params));

    /* Set up 3 planets with frequencies */
    params.planet_count = 3;
    params.frequencies[0] = 440.0f;   /* A4 */
    params.frequencies[1] = 523.25f;  /* C5 */
    params.frequencies[2] = 659.25f;  /* E5 */
    params.amplitudes[0] = 0.5f;
    params.amplitudes[1] = 0.4f;
    params.amplitudes[2] = 0.3f;
    params.master_volume = 0.7f;

    ps_score_t score = ps_from_params(&params);
    TEST_ASSERT_EQUAL_INT(3, score.note_count);
}

void test_ps_from_params_note_names(void)
{
    audio_params_t params;
    memset(&params, 0, sizeof(params));

    params.planet_count = 1;
    params.frequencies[0] = 440.0f;
    params.amplitudes[0] = 0.5f;
    params.master_volume = 0.7f;

    ps_score_t score = ps_from_params(&params);
    TEST_ASSERT_EQUAL_STRING("A4", score.notes[0].name);
}

void test_ps_from_params_root_note(void)
{
    audio_params_t params;
    memset(&params, 0, sizeof(params));

    /* Frequencies not in order -- root should be lowest */
    params.planet_count = 3;
    params.frequencies[0] = 523.25f;  /* C5 */
    params.frequencies[1] = 440.0f;   /* A4 -- lowest */
    params.frequencies[2] = 659.25f;  /* E5 */
    params.amplitudes[0] = 0.5f;
    params.amplitudes[1] = 0.5f;
    params.amplitudes[2] = 0.5f;
    params.master_volume = 0.7f;

    ps_score_t score = ps_from_params(&params);
    TEST_ASSERT_EQUAL_STRING("A4", score.root_name);
}

void test_ps_from_params_dynamics(void)
{
    audio_params_t params;
    memset(&params, 0, sizeof(params));
    params.master_volume = 0.7f;

    ps_score_t score = ps_from_params(&params);
    TEST_ASSERT_EQUAL_STRING("f", score.dynamics);
}

void test_ps_from_params_tempo(void)
{
    audio_params_t params;
    memset(&params, 0, sizeof(params));
    params.tempo_bpm = 80.0f;

    ps_score_t score = ps_from_params(&params);
    TEST_ASSERT_EQUAL_STRING("Andante", score.tempo_mark);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 80.0f, score.tempo_bpm);
}

void test_ps_from_params_mood(void)
{
    audio_params_t params;
    memset(&params, 0, sizeof(params));
    params.warmth = 0.5f;
    params.tension = 0.1f;

    ps_score_t score = ps_from_params(&params);
    TEST_ASSERT_EQUAL_STRING("Earth Breath", score.mood);
}

void test_ps_from_params_tension_warmth(void)
{
    audio_params_t params;
    memset(&params, 0, sizeof(params));
    params.tension = 0.6f;
    params.warmth = -0.5f;
    params.consonance = 0.8f;
    params.event_density = 0.3f;

    ps_score_t score = ps_from_params(&params);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.6f, score.tension);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, -0.5f, score.warmth);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.8f, score.consonance);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.3f, score.density);
}

void test_ps_from_params_planet_names(void)
{
    /* Use real audio_params from audio_score_compute to verify planet mapping */
    audio_params_t params;
    memset(&params, 0, sizeof(params));

    /* Planet indices 1-8 map to slots 0-7 in audio_score_chord */
    params.planet_count = 2;
    params.frequencies[0] = (float)audio_planet_freq(1);  /* Mercury */
    params.frequencies[1] = (float)audio_planet_freq(2);  /* Venus */
    params.amplitudes[0] = 0.5f;
    params.amplitudes[1] = 0.4f;
    params.master_volume = 0.7f;

    ps_score_t score = ps_from_params(&params);

    /* Planet names come from freq_planet() which maps index to name */
    TEST_ASSERT_NOT_NULL(score.notes[0].planet);
    TEST_ASSERT_NOT_NULL(score.notes[1].planet);
}

/* --- ps_description tests --- */

void test_ps_description_not_null(void)
{
    audio_params_t params;
    memset(&params, 0, sizeof(params));
    params.planet_count = 2;
    params.frequencies[0] = 440.0f;
    params.frequencies[1] = 660.0f;
    params.amplitudes[0] = 0.5f;
    params.amplitudes[1] = 0.3f;
    params.master_volume = 0.5f;
    params.tempo_bpm = 72.0f;

    ps_score_t score = ps_from_params(&params);
    const char *desc = ps_description(&score);
    TEST_ASSERT_NOT_NULL(desc);
    TEST_ASSERT_TRUE(strlen(desc) > 0);
}

void test_ps_description_null_score(void)
{
    const char *desc = ps_description(NULL);
    TEST_ASSERT_NOT_NULL(desc);
}

int main(void)
{
    UNITY_BEGIN();

    /* ps_format_note */
    RUN_TEST(test_ps_format_note_a440);
    RUN_TEST(test_ps_format_note_middle_c);
    RUN_TEST(test_ps_format_note_low_freq);
    RUN_TEST(test_ps_format_note_high_freq);
    RUN_TEST(test_ps_format_note_zero);

    /* ps_dynamics_from_volume */
    RUN_TEST(test_ps_dynamics_pp);
    RUN_TEST(test_ps_dynamics_p);
    RUN_TEST(test_ps_dynamics_mp);
    RUN_TEST(test_ps_dynamics_mf);
    RUN_TEST(test_ps_dynamics_f);
    RUN_TEST(test_ps_dynamics_ff);

    /* ps_tempo_marking */
    RUN_TEST(test_ps_tempo_tacet);
    RUN_TEST(test_ps_tempo_largo);
    RUN_TEST(test_ps_tempo_adagio);
    RUN_TEST(test_ps_tempo_andante);
    RUN_TEST(test_ps_tempo_moderato);
    RUN_TEST(test_ps_tempo_allegro);
    RUN_TEST(test_ps_tempo_presto);

    /* ps_from_params */
    RUN_TEST(test_ps_from_params_null);
    RUN_TEST(test_ps_from_params_note_count);
    RUN_TEST(test_ps_from_params_note_names);
    RUN_TEST(test_ps_from_params_root_note);
    RUN_TEST(test_ps_from_params_dynamics);
    RUN_TEST(test_ps_from_params_tempo);
    RUN_TEST(test_ps_from_params_mood);
    RUN_TEST(test_ps_from_params_tension_warmth);
    RUN_TEST(test_ps_from_params_planet_names);

    /* ps_description */
    RUN_TEST(test_ps_description_not_null);
    RUN_TEST(test_ps_description_null_score);

    return UNITY_END();
}
