/* pianist_score.h -- Pure data for the Pianist Score View.
 *
 * Translates audio_params_t into musician-readable data:
 * note names with octave, intervals from root, dynamics marking,
 * tempo marking, mood description, tension/consonance bars.
 *
 * Designed for Gabriel's pianist friend: everything needed to
 * improvise along with the planetary chord at a glance.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_PIANIST_SCORE_H
#define TIME_PIANIST_SCORE_H

#include "audio_score.h"

#define PS_MAX_NOTES AS_MAX_PLANETS
#define PS_NOTE_BUF  8   /* "C#4\0" fits easily */

/* A single note in the pianist's chord */
typedef struct {
    char name[PS_NOTE_BUF];   /* e.g. "A4", "C#3", "Eb5" */
    float frequency_hz;       /* actual frequency */
    float amplitude;          /* 0.0-1.0 */
    float pan;                /* -1.0 left, +1.0 right */
    float cents_off;          /* deviation from equal temperament */
    const char *planet;       /* "Mercury", "Venus", etc. */
    const char *interval;     /* interval from root: "unison", "minor third", etc. */
} ps_note_t;

/* Complete pianist score snapshot */
typedef struct {
    ps_note_t notes[PS_MAX_NOTES];
    int note_count;

    /* Root note (lowest frequency) */
    char root_name[PS_NOTE_BUF];

    /* Dynamics and tempo */
    const char *dynamics;     /* "pp", "p", "mp", "mf", "f", "ff" */
    const char *tempo_mark;   /* "Largo", "Adagio", "Andante", etc. */
    float tempo_bpm;

    /* Character */
    const char *mood;         /* from audio_score_mood */
    float tension;            /* 0.0-1.0 */
    float warmth;             /* -1.0 to +1.0 */
    float consonance;         /* 0.0-1.0 */
    float density;            /* 0.0-1.0 event density */
    float surprise;           /* 0.0-1.0 */
    float moon_factor;        /* 0.0-1.0 */
    float pulse_factor;       /* 0.85-1.0 */
    int   brain_systems;      /* number of systems in brain insight */
} ps_score_t;

/* Compute pianist-readable score from audio parameters.
 * If params is NULL, returns a zeroed score with note_count=0. */
ps_score_t ps_from_params(const audio_params_t *params);

/* Format a frequency as a note name with octave (e.g., 440.0 -> "A4").
 * Writes into buf (must be >= PS_NOTE_BUF bytes). Returns buf. */
char *ps_format_note(float freq_hz, char *buf, int buf_size);

/* Get standard dynamics marking from master volume (0.0-1.0).
 *   0.0-0.15 -> "pp"    0.15-0.35 -> "p"     0.35-0.5 -> "mp"
 *   0.5-0.65 -> "mf"    0.65-0.85 -> "f"     0.85-1.0 -> "ff" */
const char *ps_dynamics_from_volume(float volume);

/* Get tempo marking from BPM.
 *   0       -> "Tacet"    <60 -> "Largo"    60-72 -> "Adagio"
 *   72-92   -> "Andante"  92-120 -> "Moderato"  120-152 -> "Allegro"
 *   >152    -> "Presto" */
const char *ps_tempo_marking(float bpm);

/* One-sentence description of the current score for the pianist.
 * Uses a static buffer (not thread-safe). */
const char *ps_description(const ps_score_t *score);

#endif /* TIME_PIANIST_SCORE_H */
