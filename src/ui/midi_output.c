/* midi_output.c -- Pure MIDI data computation from audio parameters.
 *
 * Maps audio_params_t to MIDI messages: note numbers from frequencies,
 * velocity from amplitude, CC values from tension/warmth/consonance,
 * program changes from mood.
 *
 * MIDI note formula: note = 69 + 12 * log2(freq / 440)
 * A440 = MIDI note 69, each semitone = 1 MIDI note.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "midi_output.h"

#include <math.h>
#include <string.h>

/* log2(x) = log(x) / log(2) */
static const double LN2 = 0.693147180559945309417;

/* --- Helpers --- */

static int clamp_int(int v, int lo, int hi)
{
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

static float clampf(float v, float lo, float hi)
{
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

/* --- Public API --- */

int midi_note_from_freq(float freq_hz)
{
    if (freq_hz <= 0.0f) return -1;

    /* MIDI note = 69 + 12 * log2(freq / 440) */
    double semitones = 69.0 + 12.0 * log((double)freq_hz / 440.0) / LN2;
    int note = (int)round(semitones);

    if (note < 0) return -1;
    if (note > 127) return 127;
    return note;
}

int midi_float_to_value(float v)
{
    v = clampf(v, 0.0f, 1.0f);
    int val = (int)roundf(v * 127.0f);
    return clamp_int(val, 0, 127);
}

int midi_bipolar_to_value(float v)
{
    v = clampf(v, -1.0f, 1.0f);
    float mapped = (v + 1.0f) * 0.5f; /* -1..+1 -> 0..1 */
    int val = (int)roundf(mapped * 127.0f);
    return clamp_int(val, 0, 127);
}

midi_note_t midi_note_detailed(float freq_hz, float amplitude, int channel)
{
    midi_note_t result;
    memset(&result, 0, sizeof(result));
    result.channel = clamp_int(channel, 0, 15);

    if (freq_hz <= 0.0f) {
        result.note = -1;
        result.velocity = 0;
        return result;
    }

    double semitones = 69.0 + 12.0 * log((double)freq_hz / 440.0) / LN2;
    int note = (int)round(semitones);

    if (note < 0) note = 0;
    if (note > 127) note = 127;

    result.note = note;
    result.cents_off = (float)((semitones - (double)note) * 100.0);
    result.velocity = midi_float_to_value(amplitude);

    return result;
}

int midi_program_for_mood(const char *mood)
{
    if (mood == NULL) return 0;

    /* Map mood strings to General MIDI program numbers */
    if (strcmp(mood, "Vast Silence") == 0)          return 89;  /* Pad 2: Warm */
    if (strcmp(mood, "Cosmic Tension") == 0)         return 91;  /* Pad 4: Choir */
    if (strcmp(mood, "Earth Breath") == 0)            return 88;  /* Pad 1: New Age */
    if (strcmp(mood, "Convergence") == 0)             return 92;  /* Pad 5: Bowed */
    if (strcmp(mood, "Approaching Alignment") == 0)   return 90;  /* Pad 3: Polysynth */
    if (strcmp(mood, "Orbital Drift") == 0)           return 95;  /* Pad 8: Sweep */

    return 0; /* Acoustic Grand Piano */
}

midi_snapshot_t midi_from_params(const audio_params_t *params)
{
    midi_snapshot_t snap;
    memset(&snap, 0, sizeof(snap));

    if (params == NULL) {
        return snap;
    }

    /* Notes */
    int count = params->planet_count;
    if (count > MIDI_MAX_NOTES) count = MIDI_MAX_NOTES;
    snap.note_count = count;

    for (int i = 0; i < count; i++) {
        snap.notes[i] = midi_note_detailed(
            params->frequencies[i],
            params->amplitudes[i],
            MIDI_CHANNEL_DEFAULT);
    }

    /* CC mappings */
    snap.tension_cc.cc = 1;       /* Modulation Wheel */
    snap.tension_cc.value = midi_float_to_value(params->tension);
    snap.tension_cc.channel = MIDI_CHANNEL_DEFAULT;

    snap.warmth_cc.cc = 74;      /* Brightness */
    snap.warmth_cc.value = midi_bipolar_to_value(params->warmth);
    snap.warmth_cc.channel = MIDI_CHANNEL_DEFAULT;

    snap.consonance_cc.cc = 71;  /* Resonance / Timbre */
    snap.consonance_cc.value = midi_float_to_value(params->consonance);
    snap.consonance_cc.channel = MIDI_CHANNEL_DEFAULT;

    snap.moon_cc.cc = 11;       /* Expression */
    snap.moon_cc.value = midi_float_to_value(params->moon_factor);
    snap.moon_cc.channel = MIDI_CHANNEL_DEFAULT;

    snap.density_cc.cc = 91;    /* Reverb Send */
    snap.density_cc.value = midi_float_to_value(params->event_density);
    snap.density_cc.channel = MIDI_CHANNEL_DEFAULT;

    snap.surprise_cc.cc = 93;   /* Chorus Send */
    snap.surprise_cc.value = midi_float_to_value(params->surprise_factor);
    snap.surprise_cc.channel = MIDI_CHANNEL_DEFAULT;

    /* Program from mood */
    const char *mood = audio_score_mood(params->warmth, params->tension);
    snap.program = midi_program_for_mood(mood);

    /* Tempo */
    snap.tempo_bpm = params->tempo_bpm;

    return snap;
}
