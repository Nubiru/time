/* pianist_score.c -- Pianist Score View data computation.
 *
 * Translates audio_params_t into musician-readable data:
 * note names, intervals, dynamics, tempo markings, mood.
 *
 * Planet name lookup: audio_score_chord uses planet indices 1-8
 * (Mercury through Neptune), mapped to slots 0-7 in the params.
 * We use freq_planet() to get the planet name for each slot.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "pianist_score.h"
#include "../systems/unified/frequency.h"
#include "../systems/unified/audio_data.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

/* --- Helpers --- */

/* Find planet name for a given frequency by matching against known planets */
static const char *planet_for_slot(int slot)
{
    /* audio_score_chord fills slots 0-7 from planet indices 1-8 */
    int planet_idx = slot + 1;
    if (planet_idx < 0 || planet_idx >= FREQ_MAX_PLANETS) {
        return "?";
    }
    freq_planet_t p = freq_planet(planet_idx);
    return p.name;
}

/* --- Public API --- */

char *ps_format_note(float freq_hz, char *buf, int buf_size)
{
    if (buf == NULL || buf_size < 2) {
        return buf;
    }

    if (freq_hz <= 0.0f) {
        buf[0] = '?';
        buf[1] = '\0';
        return buf;
    }

    freq_note_t note = freq_to_note((double)freq_hz);
    snprintf(buf, (size_t)buf_size, "%s%d", note.name, note.octave);
    return buf;
}

const char *ps_dynamics_from_volume(float volume)
{
    if (volume < 0.15f) return "pp";
    if (volume < 0.35f) return "p";
    if (volume < 0.50f) return "mp";
    if (volume < 0.65f) return "mf";
    if (volume < 0.85f) return "f";
    return "ff";
}

const char *ps_tempo_marking(float bpm)
{
    if (bpm <= 0.0f)   return "Tacet";
    if (bpm < 60.0f)   return "Largo";
    if (bpm < 72.0f)   return "Adagio";
    if (bpm < 92.0f)   return "Andante";
    if (bpm < 120.0f)  return "Moderato";
    if (bpm < 152.0f)  return "Allegro";
    return "Presto";
}

ps_score_t ps_from_params(const audio_params_t *params)
{
    ps_score_t score;
    memset(&score, 0, sizeof(score));
    score.dynamics = "pp";
    score.tempo_mark = "Tacet";
    score.mood = "Silence";
    score.root_name[0] = '\0';

    if (params == NULL) {
        return score;
    }

    /* Copy scalar fields */
    score.tension = params->tension;
    score.warmth = params->warmth;
    score.consonance = params->consonance;
    score.density = params->event_density;
    score.surprise = params->surprise_factor;
    score.moon_factor = params->moon_factor;
    score.pulse_factor = params->pulse_factor;
    score.brain_systems = params->brain_system_count;
    score.tempo_bpm = params->tempo_bpm;

    /* Dynamics and tempo */
    score.dynamics = ps_dynamics_from_volume(params->master_volume);
    score.tempo_mark = ps_tempo_marking(params->tempo_bpm);

    /* Mood */
    score.mood = audio_score_mood(params->warmth, params->tension);

    /* Find root (lowest frequency) */
    int root_slot = -1;
    float root_freq = 1e9f;
    int count = params->planet_count;
    if (count > PS_MAX_NOTES) count = PS_MAX_NOTES;

    for (int i = 0; i < count; i++) {
        if (params->frequencies[i] > 0.0f &&
            params->frequencies[i] < root_freq) {
            root_freq = params->frequencies[i];
            root_slot = i;
        }
    }

    if (root_slot >= 0) {
        ps_format_note(root_freq, score.root_name, PS_NOTE_BUF);
    }

    /* Build notes */
    score.note_count = count;
    for (int i = 0; i < count; i++) {
        ps_note_t *n = &score.notes[i];
        n->frequency_hz = params->frequencies[i];
        n->amplitude = params->amplitudes[i];
        n->pan = params->pan_positions[i];
        n->planet = planet_for_slot(i);

        /* Format note name */
        ps_format_note(n->frequency_hz, n->name, PS_NOTE_BUF);

        /* Cents deviation from equal temperament */
        if (n->frequency_hz > 0.0f) {
            freq_note_t fn = freq_to_note((double)n->frequency_hz);
            n->cents_off = (float)fn.cents_off;
        }

        /* Interval from root */
        if (root_slot >= 0 && root_freq > 0.0f && n->frequency_hz > 0.0f) {
            freq_interval_t iv = freq_interval((double)root_freq,
                                               (double)n->frequency_hz);
            n->interval = iv.name;
        } else {
            n->interval = "?";
        }
    }

    return score;
}

const char *ps_description(const ps_score_t *score)
{
    static char buf[128];

    if (score == NULL || score->note_count == 0) {
        snprintf(buf, sizeof(buf), "Silence.");
        return buf;
    }

    snprintf(buf, sizeof(buf),
             "%s, %s at %s (%.0f BPM). Root: %s.",
             score->mood,
             score->dynamics,
             score->tempo_mark,
             (double)score->tempo_bpm,
             score->root_name[0] ? score->root_name : "?");

    return buf;
}
