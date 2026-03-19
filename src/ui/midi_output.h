/* midi_output.h -- Pure MIDI data computation from audio parameters.
 *
 * Maps audio_params_t to MIDI messages so a pianist's DAW can
 * receive a live feed of the planetary chord:
 *   - Planet frequencies -> MIDI note numbers (0-127)
 *   - Planet amplitudes  -> MIDI velocity (0-127)
 *   - Tension           -> CC1  (Modulation Wheel)
 *   - Warmth            -> CC74 (Brightness / Cutoff)
 *   - Consonance        -> CC71 (Resonance / Timbre)
 *   - Moon factor       -> CC11 (Expression)
 *   - Event density     -> CC91 (Reverb Send)
 *   - Mood              -> Program Change
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_MIDI_OUTPUT_H
#define TIME_MIDI_OUTPUT_H

#include "audio_score.h"

#define MIDI_MAX_NOTES AS_MAX_PLANETS
#define MIDI_CHANNEL_DEFAULT 0  /* MIDI channel 1 (0-indexed) */

/* MIDI note message */
typedef struct {
    int note;        /* MIDI note number (0-127) */
    int velocity;    /* MIDI velocity (0-127) */
    int channel;     /* MIDI channel (0-15) */
    float cents_off; /* deviation from exact MIDI note */
} midi_note_t;

/* MIDI CC (continuous controller) message */
typedef struct {
    int cc;      /* CC number (0-127) */
    int value;   /* CC value (0-127) */
    int channel; /* MIDI channel (0-15) */
} midi_cc_t;

/* Complete MIDI snapshot from audio parameters */
typedef struct {
    midi_note_t notes[MIDI_MAX_NOTES];
    int note_count;

    midi_cc_t tension_cc;     /* CC1:  tension as modulation */
    midi_cc_t warmth_cc;      /* CC74: warmth as brightness */
    midi_cc_t consonance_cc;  /* CC71: consonance as timbre */
    midi_cc_t moon_cc;        /* CC11: moon factor as expression */
    midi_cc_t density_cc;     /* CC91: event density as reverb */
    midi_cc_t surprise_cc;    /* CC93: surprise as chorus send */

    int program;              /* suggested GM program number (0-127) */
    float tempo_bpm;          /* for MIDI clock sync */
} midi_snapshot_t;

/* Compute complete MIDI snapshot from audio parameters.
 * If params is NULL, returns zeroed snapshot. */
midi_snapshot_t midi_from_params(const audio_params_t *params);

/* Convert a frequency (Hz) to MIDI note number (0-127).
 * Returns -1 for frequencies outside MIDI range (< ~8.18 Hz).
 * A440 = note 69. */
int midi_note_from_freq(float freq_hz);

/* Convert a frequency to MIDI note with cent deviation.
 * The note field is the nearest MIDI note, cents_off is
 * the deviation from exact (-50 to +50 cents). */
midi_note_t midi_note_detailed(float freq_hz, float amplitude,
                               int channel);

/* Map a 0.0-1.0 float to MIDI value (0-127). */
int midi_float_to_value(float v);

/* Map a -1.0 to +1.0 float to MIDI value (0-127).
 * -1.0 -> 0, 0.0 -> 64, +1.0 -> 127. */
int midi_bipolar_to_value(float v);

/* Get a suggested GM program number for the current mood.
 * "Vast Silence" -> 89 (Pad 2: Warm), "Earth Breath" -> 88 (Pad 1: New Age),
 * etc. Returns 0 (Acoustic Grand Piano) as default. */
int midi_program_for_mood(const char *mood);

#endif /* TIME_MIDI_OUTPUT_H */
