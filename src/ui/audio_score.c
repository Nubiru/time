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
#include "audio_event.h"
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

float audio_score_pulse(float tempo_bpm, double real_time_sec)
{
    if (tempo_bpm <= 0.0f || real_time_sec < 0.0) {
        return 1.0f;
    }

    /* Beat period in seconds */
    double beat_period = 60.0 / (double)tempo_bpm;

    /* Phase within current beat (0.0 to 1.0) */
    double phase = fmod(real_time_sec / beat_period, 1.0);
    if (phase < 0.0) phase += 1.0;

    /* Smooth cosine pulse: 0.85 at trough, 1.0 at peak */
    float pulse = 0.85f + 0.15f * (float)(0.5 + 0.5 * cos(2.0 * PI * phase));

    return pulse;
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
        result.waveform_types[i] = 0;
        result.harmonic_counts[i] = 0;
        for (int h = 0; h < AS_MAX_HARMONICS; h++) {
            result.harmonic_amps[i][h] = 0.0f;
        }
    }
    result.reverb_wet = 0.0f;
    result.reverb_decay_s = 0.0f;
    result.moon_factor = 0.0f;
    for (int j = 0; j < AS_MAX_PLANETS; j++) {
        result.pan_positions[j] = 0.0f;
    }
    result.pulse_factor = 1.0f;
    result.consonance = 0.0f;
    result.event_density = 0.0f;
    result.event_intensity = 0.0f;

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

    /* Event detection: aspects + convergences (L2.1+L2.2) */
    {
        audio_event_summary_t events = audio_event_scan(jd, 8.0);
        result.consonance = events.consonance;
        result.event_density = events.convergence_density;
        result.event_intensity = events.event_intensity;
    }

    /* Populate timbre data from audio_data profiles */
    for (int i = 0; i < result.planet_count; i++) {
        int planet_idx = i + 1; /* audio_score_chord uses planets 1-8, mapping to slots 0-7 */
        audio_planet_profile_t prof = audio_planet_profile(planet_idx);
        result.waveform_types[i] = (int)prof.waveform;
        result.harmonic_counts[i] = prof.harmonic_count;
        for (int h = 0; h < prof.harmonic_count && h < AS_MAX_HARMONICS; h++) {
            result.harmonic_amps[i][h] = (float)prof.harmonics[h].amplitude;
        }
    }

    /* Moon envelope: modulate all amplitudes by lunar phase (L1.3) */
    {
        double phase = lunar_phase_approx(jd);
        float moon = (float)audio_moon_envelope(phase);
        result.moon_factor = moon;
        for (int i = 0; i < result.planet_count; i++) {
            result.amplitudes[i] *= moon;
        }
    }

    /* Warmth-based amplitude scaling (L1.1):
     * warmth > 0 (warm): boost inner planets, soften outer
     * warmth < 0 (cold): boost outer planets, soften inner */
    {
        float w = result.warmth;
        for (int i = 0; i < result.planet_count; i++) {
            float mix_factor;
            if (i < 4) {
                /* Inner planet: boost when warm */
                mix_factor = 1.0f + 0.3f * w;
            } else {
                /* Outer planet: boost when cold */
                mix_factor = 1.0f - 0.3f * w;
            }
            if (mix_factor < 0.3f) mix_factor = 0.3f;
            if (mix_factor > 1.5f) mix_factor = 1.5f;
            result.amplitudes[i] *= mix_factor;
        }
    }

    /* Zoom proximity: zoomed in = louder, zoomed out = softer (L1.1) */
    {
        float zoom_factor = 1.0f + log_zoom * 0.02f;
        if (zoom_factor < 0.6f) zoom_factor = 0.6f;
        if (zoom_factor > 1.4f) zoom_factor = 1.4f;
        for (int i = 0; i < result.planet_count; i++) {
            result.amplitudes[i] *= zoom_factor;
        }
    }

    /* Spatial panning: spread planets across stereo field (L1.2)
     * Simple fixed positions for now -- future: compute from ecliptic longitude */
    {
        static const float PAN_SPREAD[8] = {
            -0.7f,  /* Mercury: left */
            -0.4f,  /* Venus: slight left */
            -0.1f,  /* Earth year: near center-left */
             0.2f,  /* Mars: slight right */
             0.5f,  /* Jupiter: right */
             0.8f,  /* Saturn: far right */
            -0.5f,  /* Uranus: left */
             0.0f   /* Neptune: center */
        };
        for (int i = 0; i < result.planet_count && i < 8; i++) {
            result.pan_positions[i] = PAN_SPREAD[i];
        }
    }

    /* Final amplitude clamp to [0, 1] */
    for (int i = 0; i < result.planet_count; i++) {
        result.amplitudes[i] = clampf(result.amplitudes[i], 0.0f, 1.0f);
    }

    /* Reverb: cold views get more reverb (vast/spacious), warm views less (intimate) */
    {
        float w = result.warmth;
        if (w < -1.0f) w = -1.0f;
        if (w > 1.0f) w = 1.0f;
        float cold_factor = (-w + 1.0f) * 0.5f; /* 0.0 at warmth=+1, 1.0 at warmth=-1 */
        result.reverb_wet = 0.15f + 0.45f * cold_factor;   /* range: 0.15 to 0.60 */
        result.reverb_decay_s = 1.0f + 1.0f * cold_factor; /* range: 1.0 to 2.0 */
    }

    return result;
}
