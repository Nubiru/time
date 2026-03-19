/* audio_a11y.h -- Audio accessibility narrative module.
 *
 * Generates human-readable descriptions of audio state changes
 * for screen readers: chord shifts, aspect formations, tension
 * changes, convergence events. Extends the ARIA system with
 * audio-specific descriptions so blind users experience Time
 * through sound descriptions alone.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_AUDIO_A11Y_H
#define TIME_AUDIO_A11Y_H

#include "audio_score.h"
#include "pianist_score.h"

#define AA_MAX_LEN 256

/* Describe the current chord for screen readers.
 * Output: "Planetary chord: A4 (Mercury), C#3 (Venus), E5 (Mars). 3 notes."
 * Returns chars written (excluding null). */
int aa_chord_desc(const ps_score_t *score, char *out, int max_len);

/* Describe the current mood and dynamics.
 * Output: "Mood: Orbital Drift. Dynamics: mf (moderately loud). Tempo: Andante at 80 BPM."
 * Returns chars written. */
int aa_mood_desc(const ps_score_t *score, char *out, int max_len);

/* Describe tension and consonance levels.
 * Output: "Tension is low. Consonance is high — the chord feels resolved."
 * Returns chars written. */
int aa_tension_desc(float tension, float consonance, char *out, int max_len);

/* Describe a chord change between two snapshots.
 * Output: "The chord has shifted. Tension is increasing. A new aspect forms between Jupiter and Saturn."
 * If prev is NULL, describes current state only.
 * Returns chars written. */
int aa_change_desc(const audio_params_t *prev, const audio_params_t *curr,
                   char *out, int max_len);

/* Describe the moon's influence on the sound.
 * Output: "Full moon — the chord is at peak volume."
 * Returns chars written. */
int aa_moon_desc(float moon_factor, char *out, int max_len);

/* Describe meditation mode state.
 * Output: "Meditation mode active. Breathing in. Phi-timed breathing at 4.2 second cycles."
 * Returns chars written. */
int aa_meditation_desc(int active, int inhaling, float cycle_sec,
                       char *out, int max_len);

/* Get a one-word tension level: "minimal", "low", "moderate", "high", "extreme". */
const char *aa_tension_level(float tension);

/* Get a one-word consonance level: "dissonant", "tense", "neutral", "consonant", "resolved". */
const char *aa_consonance_level(float consonance);

/* Get a one-word moon phase description: "new", "crescent", "half", "gibbous", "full". */
const char *aa_moon_phase_word(float moon_factor);

#endif /* TIME_AUDIO_A11Y_H */
