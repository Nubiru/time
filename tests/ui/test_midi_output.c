/* test_midi_output.c -- Tests for MIDI output data module. */

#include "unity.h"
#include "ui/midi_output.h"
#include "ui/audio_score.h"

#include <string.h>
#include <math.h>

void setUp(void) {}
void tearDown(void) {}

/* --- midi_note_from_freq tests --- */

void test_midi_note_a440(void)
{
    TEST_ASSERT_EQUAL_INT(69, midi_note_from_freq(440.0f));
}

void test_midi_note_middle_c(void)
{
    /* C4 = ~261.63 Hz = MIDI 60 */
    TEST_ASSERT_EQUAL_INT(60, midi_note_from_freq(261.63f));
}

void test_midi_note_c3(void)
{
    /* C3 = ~130.81 Hz = MIDI 48 */
    TEST_ASSERT_EQUAL_INT(48, midi_note_from_freq(130.81f));
}

void test_midi_note_low_limit(void)
{
    /* MIDI 0 = C-1 = ~8.18 Hz */
    TEST_ASSERT_EQUAL_INT(0, midi_note_from_freq(8.18f));
}

void test_midi_note_too_low(void)
{
    TEST_ASSERT_EQUAL_INT(-1, midi_note_from_freq(1.0f));
}

void test_midi_note_zero(void)
{
    TEST_ASSERT_EQUAL_INT(-1, midi_note_from_freq(0.0f));
}

void test_midi_note_high(void)
{
    /* G9 = ~12544 Hz = MIDI 127 */
    TEST_ASSERT_EQUAL_INT(127, midi_note_from_freq(12544.0f));
}

/* --- midi_float_to_value tests --- */

void test_midi_float_zero(void)
{
    TEST_ASSERT_EQUAL_INT(0, midi_float_to_value(0.0f));
}

void test_midi_float_half(void)
{
    TEST_ASSERT_EQUAL_INT(64, midi_float_to_value(0.5f));
}

void test_midi_float_one(void)
{
    TEST_ASSERT_EQUAL_INT(127, midi_float_to_value(1.0f));
}

void test_midi_float_clamp_low(void)
{
    TEST_ASSERT_EQUAL_INT(0, midi_float_to_value(-0.5f));
}

void test_midi_float_clamp_high(void)
{
    TEST_ASSERT_EQUAL_INT(127, midi_float_to_value(1.5f));
}

/* --- midi_bipolar_to_value tests --- */

void test_midi_bipolar_center(void)
{
    TEST_ASSERT_EQUAL_INT(64, midi_bipolar_to_value(0.0f));
}

void test_midi_bipolar_min(void)
{
    TEST_ASSERT_EQUAL_INT(0, midi_bipolar_to_value(-1.0f));
}

void test_midi_bipolar_max(void)
{
    TEST_ASSERT_EQUAL_INT(127, midi_bipolar_to_value(1.0f));
}

/* --- midi_note_detailed tests --- */

void test_midi_note_detailed_a440(void)
{
    midi_note_t n = midi_note_detailed(440.0f, 0.8f, 0);
    TEST_ASSERT_EQUAL_INT(69, n.note);
    TEST_ASSERT_EQUAL_INT(102, n.velocity); /* 0.8 * 127 = 101.6 -> 102 */
    TEST_ASSERT_EQUAL_INT(0, n.channel);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 0.0f, n.cents_off);
}

void test_midi_note_detailed_zero_amp(void)
{
    midi_note_t n = midi_note_detailed(440.0f, 0.0f, 5);
    TEST_ASSERT_EQUAL_INT(69, n.note);
    TEST_ASSERT_EQUAL_INT(0, n.velocity);
    TEST_ASSERT_EQUAL_INT(5, n.channel);
}

/* --- midi_program_for_mood tests --- */

void test_midi_program_default(void)
{
    TEST_ASSERT_EQUAL_INT(0, midi_program_for_mood("Unknown"));
}

void test_midi_program_vast_silence(void)
{
    int prog = midi_program_for_mood("Vast Silence");
    TEST_ASSERT_TRUE(prog >= 0 && prog <= 127);
}

void test_midi_program_earth_breath(void)
{
    int prog = midi_program_for_mood("Earth Breath");
    TEST_ASSERT_TRUE(prog >= 0 && prog <= 127);
}

/* --- midi_from_params tests --- */

void test_midi_from_params_null(void)
{
    midi_snapshot_t snap = midi_from_params(NULL);
    TEST_ASSERT_EQUAL_INT(0, snap.note_count);
}

void test_midi_from_params_note_count(void)
{
    audio_params_t params;
    memset(&params, 0, sizeof(params));
    params.planet_count = 3;
    params.frequencies[0] = 440.0f;
    params.frequencies[1] = 523.25f;
    params.frequencies[2] = 659.25f;
    params.amplitudes[0] = 0.5f;
    params.amplitudes[1] = 0.4f;
    params.amplitudes[2] = 0.3f;

    midi_snapshot_t snap = midi_from_params(&params);
    TEST_ASSERT_EQUAL_INT(3, snap.note_count);
}

void test_midi_from_params_note_values(void)
{
    audio_params_t params;
    memset(&params, 0, sizeof(params));
    params.planet_count = 1;
    params.frequencies[0] = 440.0f;
    params.amplitudes[0] = 0.7f;

    midi_snapshot_t snap = midi_from_params(&params);
    TEST_ASSERT_EQUAL_INT(69, snap.notes[0].note);
    TEST_ASSERT_EQUAL_INT(89, snap.notes[0].velocity); /* 0.7 * 127 = 88.9 -> 89 */
}

void test_midi_from_params_tension_cc(void)
{
    audio_params_t params;
    memset(&params, 0, sizeof(params));
    params.tension = 0.5f;

    midi_snapshot_t snap = midi_from_params(&params);
    TEST_ASSERT_EQUAL_INT(1, snap.tension_cc.cc);
    TEST_ASSERT_EQUAL_INT(64, snap.tension_cc.value);
}

void test_midi_from_params_warmth_cc(void)
{
    audio_params_t params;
    memset(&params, 0, sizeof(params));
    params.warmth = -0.5f;

    midi_snapshot_t snap = midi_from_params(&params);
    TEST_ASSERT_EQUAL_INT(74, snap.warmth_cc.cc);
    TEST_ASSERT_EQUAL_INT(32, snap.warmth_cc.value); /* bipolar: (-0.5+1)/2*127 = 32 */
}

void test_midi_from_params_tempo(void)
{
    audio_params_t params;
    memset(&params, 0, sizeof(params));
    params.tempo_bpm = 120.0f;

    midi_snapshot_t snap = midi_from_params(&params);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 120.0f, snap.tempo_bpm);
}

int main(void)
{
    UNITY_BEGIN();

    /* midi_note_from_freq */
    RUN_TEST(test_midi_note_a440);
    RUN_TEST(test_midi_note_middle_c);
    RUN_TEST(test_midi_note_c3);
    RUN_TEST(test_midi_note_low_limit);
    RUN_TEST(test_midi_note_too_low);
    RUN_TEST(test_midi_note_zero);
    RUN_TEST(test_midi_note_high);

    /* midi_float_to_value / midi_bipolar_to_value */
    RUN_TEST(test_midi_float_zero);
    RUN_TEST(test_midi_float_half);
    RUN_TEST(test_midi_float_one);
    RUN_TEST(test_midi_float_clamp_low);
    RUN_TEST(test_midi_float_clamp_high);
    RUN_TEST(test_midi_bipolar_center);
    RUN_TEST(test_midi_bipolar_min);
    RUN_TEST(test_midi_bipolar_max);

    /* midi_note_detailed */
    RUN_TEST(test_midi_note_detailed_a440);
    RUN_TEST(test_midi_note_detailed_zero_amp);

    /* midi_program_for_mood */
    RUN_TEST(test_midi_program_default);
    RUN_TEST(test_midi_program_vast_silence);
    RUN_TEST(test_midi_program_earth_breath);

    /* midi_from_params */
    RUN_TEST(test_midi_from_params_null);
    RUN_TEST(test_midi_from_params_note_count);
    RUN_TEST(test_midi_from_params_note_values);
    RUN_TEST(test_midi_from_params_tension_cc);
    RUN_TEST(test_midi_from_params_warmth_cc);
    RUN_TEST(test_midi_from_params_tempo);

    return UNITY_END();
}
