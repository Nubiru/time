/* audio_score.c -- Pure audio score computation.
 *
 * Computes audio parameters (frequencies, amplitudes, tension, warmth,
 * tempo) from Julian Day, view, zoom, and time speed.
 *
 * Planet chord: indices 1-8 from audio_data.h (Mercury through Neptune).
 * Skips index 0 (Earth day) and index 9 (Moon synodic).
 *
 * Tension: simplified lunar-phase model. New/full moon proximity
 * drives a 0-1 tension value (lunar cycle ~29.53 days).
 *
 * Warmth: maps view_id to a base warmth, then interpolates with log_zoom.
 *
 * Tempo: logarithmic mapping from simulation speed to BPM.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "audio_score.h"
#include "../systems/unified/audio_data.h"
#include "view_registry.h"

#include <math.h>
#include <stddef.h>

#define PI 3.14159265358979323846

/* Synodic month in days */
static const double SYNODIC_MONTH = 29.530588853;

/* Known new moon epoch (JD of a known new moon: 2000-01-06 18:14 UTC) */
static const double NEW_MOON_EPOCH = 2451550.26;

/* --- Internal helpers --- */

/* Compute lunar phase (0.0 = new, 0.5 = full, 1.0 = next new) */
static double lunar_phase_approx(double jd)
{
    double elapsed = jd - NEW_MOON_EPOCH;
    double cycles = elapsed / SYNODIC_MONTH;
    double phase = cycles - floor(cycles);
    if (phase < 0.0) phase += 1.0;
    return phase;
}

/* Clamp float to [lo, hi] */
static float clampf(float v, float lo, float hi)
{
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

/* --- Public API --- */

int audio_score_chord(double jd, float *frequencies, float *amplitudes,
                      int max)
{
    (void)jd;  /* frequencies are orbital, not JD-dependent */

    if (frequencies == NULL || amplitudes == NULL || max <= 0) {
        return 0;
    }

    int count = 0;
    /* Planets: indices 1 (Mercury) through 8 (Neptune), skip 0 and 9 */
    for (int i = 1; i <= 8 && count < max; i++) {
        double freq = audio_planet_freq(i);
        double amp = audio_planet_amplitude(i);
        if (freq > 0.0) {
            frequencies[count] = (float)freq;
            amplitudes[count] = (float)amp;
            count++;
        }
    }

    return count;
}

float audio_score_tension(double jd)
{
    double phase = lunar_phase_approx(jd);

    /* Tension peaks at new moon (phase=0) and full moon (phase=0.5).
     * Use cos(4*PI*phase) which peaks at 0, 0.5, 1.0.
     * Scale: 0.0 (quarter moons) to ~0.3 (new/full moons). */
    double raw = 0.5 + 0.5 * cos(4.0 * PI * phase);

    /* Scale to a subtle range: base 0.05 + 0.3 at peak */
    float tension = (float)(0.05 + 0.30 * raw);

    return clampf(tension, 0.0f, 1.0f);
}

float audio_score_warmth(int view_id, float log_zoom)
{
    /* Base warmth per view type */
    float base;
    float zoom_sensitivity;

    switch (view_id) {
    case VIEW_GALAXY:
        base = -0.7f;
        zoom_sensitivity = 0.05f;
        break;
    case VIEW_DEEP_TIME:
        base = -0.6f;
        zoom_sensitivity = 0.05f;
        break;
    case VIEW_SPACE:
        base = 0.0f;
        zoom_sensitivity = 0.1f;
        break;
    case VIEW_PERSONAL:
        base = 0.3f;
        zoom_sensitivity = 0.05f;
        break;
    case VIEW_EARTH:
        base = 0.7f;
        zoom_sensitivity = 0.05f;
        break;
    case VIEW_CITY:
        base = 0.8f;
        zoom_sensitivity = 0.03f;
        break;
    case VIEW_ROOM:
        base = 0.8f;
        zoom_sensitivity = 0.02f;
        break;
    default:
        return 0.0f;
    }

    /* log_zoom > 0 means zoomed in (warmer), < 0 means zoomed out (colder) */
    float warmth = base + log_zoom * zoom_sensitivity;

    return clampf(warmth, -1.0f, 1.0f);
}

float audio_score_tempo(double time_speed)
{
    if (time_speed <= 0.0) {
        return 0.0f;
    }

    /* Logarithmic mapping:
     * speed  1.0      -> 60 BPM (resting heartbeat)
     * speed  60.0     -> ~72 BPM (walking)
     * speed  3600.0   -> ~90 BPM (active)
     * speed  86400.0  -> ~120 BPM (energetic)
     *
     * Formula: bpm = 60 + 60 * log10(speed) / log10(86400)
     * log10(86400) ~ 4.937
     * At speed=1:     60 + 0     = 60
     * At speed=86400: 60 + 60    = 120
     */
    static const double LOG_MAX = 4.936513743;  /* log10(86400) */
    double log_speed = log10(time_speed);

    /* Clamp log_speed to [0, LOG_MAX] for BPM range [60, 120] */
    if (log_speed < 0.0) log_speed = 0.0;
    if (log_speed > LOG_MAX) log_speed = LOG_MAX;

    float bpm = (float)(60.0 + 60.0 * log_speed / LOG_MAX);

    return clampf(bpm, 0.0f, 200.0f);
}

const char *audio_score_mood(float warmth, float tension)
{
    /* Thresholds */
    int cold = warmth < -0.3f;
    int warm = warmth > 0.3f;
    int tense = tension > 0.4f;

    if (cold && !tense) return "Vast Silence";
    if (cold && tense)  return "Cosmic Tension";
    if (warm && !tense) return "Earth Breath";
    if (warm && tense)  return "Convergence";
    if (!cold && !warm && tense)  return "Approaching Alignment";

    /* neutral + calm */
    return "Orbital Drift";
}

audio_params_t audio_score_compute(double jd, int view_id, float log_zoom,
                                   double time_speed)
{
    audio_params_t result;

    /* Zero-initialize */
    for (int i = 0; i < AS_MAX_PLANETS; i++) {
        result.frequencies[i] = 0.0f;
        result.amplitudes[i] = 0.0f;
    }

    /* Chord */
    result.planet_count = audio_score_chord(jd, result.frequencies,
                                            result.amplitudes,
                                            AS_MAX_PLANETS);

    /* Master volume: base 0.7 */
    result.master_volume = 0.7f;

    /* Tension */
    result.tension = audio_score_tension(jd);

    /* Warmth */
    result.warmth = audio_score_warmth(view_id, log_zoom);

    /* Tempo */
    result.tempo_bpm = audio_score_tempo(time_speed);

    return result;
}
