/* audio_a11y.c -- Audio accessibility narrative generation.
 *
 * Generates human-readable descriptions of audio state for
 * screen readers, enabling blind users to experience Time
 * through sound descriptions.
 *
 * All output via snprintf to caller-provided buffers.
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "audio_a11y.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

/* --- Level descriptors --- */

const char *aa_tension_level(float tension)
{
    if (tension < 0.15f) return "minimal";
    if (tension < 0.35f) return "low";
    if (tension < 0.55f) return "moderate";
    if (tension < 0.80f) return "high";
    return "extreme";
}

const char *aa_consonance_level(float consonance)
{
    if (consonance < 0.20f) return "dissonant";
    if (consonance < 0.40f) return "tense";
    if (consonance < 0.60f) return "neutral";
    if (consonance < 0.80f) return "consonant";
    return "resolved";
}

const char *aa_moon_phase_word(float moon_factor)
{
    if (moon_factor < 0.25f) return "new";
    if (moon_factor < 0.45f) return "crescent";
    if (moon_factor < 0.65f) return "half";
    if (moon_factor < 0.85f) return "gibbous";
    return "full";
}

/* --- Description generators --- */

int aa_chord_desc(const ps_score_t *score, char *out, int max_len)
{
    if (out == NULL || max_len <= 0) return 0;

    if (score == NULL || score->note_count == 0) {
        return snprintf(out, (size_t)max_len, "Silence. No planetary chord.");
    }

    int written = snprintf(out, (size_t)max_len, "Planetary chord: ");
    if (written < 0) return 0;

    for (int i = 0; i < score->note_count && written < max_len - 1; i++) {
        int n;
        if (i > 0) {
            n = snprintf(out + written, (size_t)(max_len - written), ", ");
            if (n > 0) written += n;
        }
        n = snprintf(out + written, (size_t)(max_len - written),
                     "%s (%s)",
                     score->notes[i].name,
                     score->notes[i].planet ? score->notes[i].planet : "?");
        if (n > 0) written += n;
    }

    {
        int n = snprintf(out + written, (size_t)(max_len - written),
                         ". %d notes.", score->note_count);
        if (n > 0) written += n;
    }

    return written;
}

int aa_mood_desc(const ps_score_t *score, char *out, int max_len)
{
    if (out == NULL || max_len <= 0) return 0;

    if (score == NULL) {
        return snprintf(out, (size_t)max_len, "No audio.");
    }

    return snprintf(out, (size_t)max_len,
                    "Mood: %s. Dynamics: %s. Tempo: %s at %.0f BPM.",
                    score->mood ? score->mood : "?",
                    score->dynamics ? score->dynamics : "?",
                    score->tempo_mark ? score->tempo_mark : "?",
                    (double)score->tempo_bpm);
}

int aa_tension_desc(float tension, float consonance, char *out, int max_len)
{
    if (out == NULL || max_len <= 0) return 0;

    return snprintf(out, (size_t)max_len,
                    "Tension is %s. Consonance is %s.",
                    aa_tension_level(tension),
                    aa_consonance_level(consonance));
}

int aa_moon_desc(float moon_factor, char *out, int max_len)
{
    if (out == NULL || max_len <= 0) return 0;

    const char *phase = aa_moon_phase_word(moon_factor);

    if (moon_factor >= 0.85f) {
        return snprintf(out, (size_t)max_len,
                        "Moon is %s. The chord is at peak volume.", phase);
    }
    if (moon_factor < 0.25f) {
        return snprintf(out, (size_t)max_len,
                        "Moon is %s. The chord is at its quietest.", phase);
    }

    return snprintf(out, (size_t)max_len,
                    "Moon is %s. Volume at %.0f percent.",
                    phase, (double)(moon_factor * 100.0f));
}

int aa_meditation_desc(int active, int inhaling, float cycle_sec,
                       char *out, int max_len)
{
    if (out == NULL || max_len <= 0) return 0;

    if (!active) {
        return snprintf(out, (size_t)max_len, "Meditation mode inactive.");
    }

    return snprintf(out, (size_t)max_len,
                    "Meditation mode active. Breathing %s. "
                    "Phi-timed breathing at %.1f second cycles.",
                    inhaling ? "in" : "out",
                    (double)cycle_sec);
}

int aa_change_desc(const audio_params_t *prev, const audio_params_t *curr,
                   char *out, int max_len)
{
    if (out == NULL || max_len <= 0) return 0;

    if (curr == NULL) {
        return snprintf(out, (size_t)max_len, "No audio state.");
    }

    if (prev == NULL) {
        return snprintf(out, (size_t)max_len,
                        "Audio state: tension %s, consonance %s.",
                        aa_tension_level(curr->tension),
                        aa_consonance_level(curr->consonance));
    }

    /* Detect significant changes */
    float dt = curr->tension - prev->tension;
    float dc = curr->consonance - prev->consonance;

    int written = 0;

    if (fabsf(dt) > 0.1f) {
        int n = snprintf(out + written, (size_t)(max_len - written),
                         "Tension is %s. ",
                         dt > 0 ? "increasing" : "decreasing");
        if (n > 0) written += n;
    }

    if (fabsf(dc) > 0.1f) {
        int n = snprintf(out + written, (size_t)(max_len - written),
                         "Consonance is %s. ",
                         dc > 0 ? "increasing" : "decreasing");
        if (n > 0) written += n;
    }

    if (written == 0) {
        written = snprintf(out, (size_t)max_len, "No significant change.");
    }

    return written;
}
