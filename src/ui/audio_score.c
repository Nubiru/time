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
#include "audio_culture.h"
#include "../systems/unified/audio_data.h"
#include "../systems/unified/brain_scan.h"
#include "../systems/unified/brain_audio_bridge.h"
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
    result.surprise_factor = 0.0f;
    result.brain_system_count = 0;
    result.focused_system = -1;

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

    /* Brain scan (used by event detection and cultural timbre) */
    br_result_t br;
    br_scan(jd, &br);

    /* Event detection: brain intelligence + aspects (L2.1+L2.2+L2.4) */
    {
        br_audio_event_t brain_evt;
        br_audio_from_result(&br, &brain_evt);

        /* Also get aspect-level consonance from audio_event for immediacy */
        audio_event_summary_t aspect_events = audio_event_scan(jd, 8.0);

        /* Blend brain and aspect consonance (brain=deeper, aspects=immediate) */
        result.consonance = 0.4f * brain_evt.consonance
                          + 0.6f * aspect_events.consonance;

        /* Use brain's convergence density (more comprehensive than
         * cd_significance alone) */
        result.event_density = brain_evt.convergence_density;

        /* Combined intensity from brain */
        result.event_intensity = brain_evt.event_intensity;

        /* Brain-specific fields */
        result.surprise_factor = brain_evt.surprise_factor;
        result.brain_system_count = brain_evt.system_count;
    }

    /* Populate timbre data from audio_data profiles (base layer).
     * Must happen BEFORE convergence modulation so convergence can
     * add richness on top of the base harmonic profile. */
    for (int i = 0; i < result.planet_count; i++) {
        int planet_idx = i + 1; /* audio_score_chord uses planets 1-8, mapping to slots 0-7 */
        audio_planet_profile_t prof = audio_planet_profile(planet_idx);
        result.waveform_types[i] = (int)prof.waveform;
        result.harmonic_counts[i] = prof.harmonic_count;
        for (int h = 0; h < prof.harmonic_count && h < AS_MAX_HARMONICS; h++) {
            result.harmonic_amps[i][h] = (float)prof.harmonics[h].amplitude;
        }
    }

    /* Convergence modulation (L2.1+L2.4 + sound design quality):
     * Inspired by ITPRA (digest 079 Sweet Anticipation):
     *   Tension builds as convergence approaches, then resolves.
     *   More converging systems = richer harmonic content.
     *
     * 1. Volume: gentle boost (up to +30%), surprise spike (+20%)
     * 2. Tension detuning: slight frequency sharpening that relaxes
     *    at peak convergence (appoggiatura → resolution pattern)
     * 3. Harmonic richness: more partials activated during convergence
     * 4. Reverb: spacious during convergence (resonant moment) */
    {
        float intensity = result.event_intensity;
        float surprise = result.surprise_factor;
        float consonance = result.consonance;
        int sys_count = result.brain_system_count;

        /* Volume boost: gentler than before (+30% max, not +40%)
         * to leave headroom for harmonic richness to do the work */
        float convergence_boost = 1.0f + 0.3f * intensity;
        convergence_boost += 0.20f * surprise;
        result.master_volume *= convergence_boost;
        if (result.master_volume > 1.0f)
            result.master_volume = 1.0f;

        /* Tension detuning: when intensity is building (< 0.7),
         * slightly sharpen frequencies (up to +1.5% = ~25 cents).
         * At peak (>0.7), resolve back to natural pitch.
         * Creates the "almost there... NOW" feeling. */
        {
            float detune;
            if (intensity < 0.7f) {
                /* Building: sharpen proportional to intensity */
                detune = intensity * 0.015f / 0.7f; /* 0 → 1.5% */
            } else {
                /* Resolving: relax back toward natural pitch */
                float resolve_t = (intensity - 0.7f) / 0.3f; /* 0→1 */
                detune = 0.015f * (1.0f - resolve_t * resolve_t);
            }
            /* Apply detuning: add frequency offset to create tension */
            if (detune > 0.001f) {
                for (int i = 0; i < result.planet_count; i++) {
                    result.frequencies[i] *= (1.0f + detune);
                }
            }
        }

        /* Reverb: spacious during convergence (resonant moment) */
        result.reverb_wet += 0.15f * intensity;

        /* Harmonic richness: convergence activates more partials.
         * Quiet = sparse (base harmonics). Convergence = full series.
         * More systems converging = richer, more complex spectrum. */
        if (intensity > 0.2f) {
            float richness = (intensity - 0.2f) / 0.8f; /* 0→1 */
            /* System count bonus: more systems = more partials added */
            int extra_partials = (int)(richness * 2.0f);
            if (sys_count > 3) extra_partials++;
            if (sys_count > 5) extra_partials++;
            if (extra_partials > 0) {
                for (int i = 0; i < result.planet_count; i++) {
                    int new_count = result.harmonic_counts[i] + extra_partials;
                    if (new_count > AS_MAX_HARMONICS)
                        new_count = AS_MAX_HARMONICS;
                    /* Fill newly activated partials with decaying amplitudes */
                    for (int h = result.harmonic_counts[i]; h < new_count; h++) {
                        result.harmonic_amps[i][h] = richness * 0.3f
                            / (float)(h + 1);
                    }
                    result.harmonic_counts[i] = new_count;
                }
            }
        }

        /* Consonance-driven brightness at resolution:
         * High consonance (aspects aligning) → existing partials get
         * a slight amplitude boost, making the chord "glow" */
        if (consonance > 0.5f) {
            float glow = (consonance - 0.5f) * 0.3f; /* up to +0.15 */
            for (int i = 0; i < result.planet_count; i++) {
                for (int h = 1; h < result.harmonic_counts[i]; h++) {
                    result.harmonic_amps[i][h] += glow / (float)(h + 1);
                    if (result.harmonic_amps[i][h] > 1.0f)
                        result.harmonic_amps[i][h] = 1.0f;
                }
            }
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

    /* Zoom-level audio modulation (sound design quality):
     * Maps camera zoom to three audio dimensions:
     *   Far (log_zoom < -2): cosmic drone — frequencies down, vast reverb
     *   Mid  (-2 to 5):      planetary chord — standard frequencies
     *   Close (log_zoom > 5): intimate — focus timbre prominent
     *
     * Inspired by digest 080 (Brain on Music): spatial audio changes
     * must match visual scale shifts for coherent experience. */
    {
        /* 1. Zoom proximity: zoomed in = louder overall */
        float zoom_factor = 1.0f + log_zoom * 0.02f;
        if (zoom_factor < 0.6f) zoom_factor = 0.6f;
        if (zoom_factor > 1.4f) zoom_factor = 1.4f;
        for (int i = 0; i < result.planet_count; i++) {
            result.amplitudes[i] *= zoom_factor;
        }

        /* 2. Octave shifting at extreme zoom-out:
         * log_zoom < -2 → start shifting down (cosmic drone effect)
         * log_zoom < -4 → full octave down (frequencies × 0.5)
         * Creates the "deep cosmic drone" from far away without
         * needing extra oscillators. */
        if (log_zoom < -2.0f) {
            /* Ramp: -2 → 1.0 (no shift), -4.6 → 0.5 (one octave down) */
            float shift_t = (log_zoom + 2.0f) / -2.6f; /* 0 at -2, 1 at -4.6 */
            if (shift_t > 1.0f) shift_t = 1.0f;
            float oct_factor = 1.0f - 0.5f * shift_t; /* 1.0 → 0.5 */
            for (int i = 0; i < result.planet_count; i++) {
                result.frequencies[i] *= oct_factor;
            }
        }

        /* 3. Zoom-dependent amplitude morphing:
         * Far zoom-out: outer planets (Jupiter, Saturn, Uranus, Neptune)
         * become dominant — they ARE the cosmos at that scale.
         * Close zoom-in: inner planets become louder — personal scale. */
        if (log_zoom < -1.0f) {
            /* Fade inner planets (slots 0-3) as we zoom out */
            float inner_fade = 1.0f + (log_zoom + 1.0f) * 0.15f;
            if (inner_fade < 0.15f) inner_fade = 0.15f;
            if (inner_fade > 1.0f) inner_fade = 1.0f;
            for (int i = 0; i < 4 && i < result.planet_count; i++) {
                result.amplitudes[i] *= inner_fade;
            }
        } else if (log_zoom > 6.0f) {
            /* Fade outer planets (slots 4-7) as we zoom in close */
            float outer_fade = 1.0f - (log_zoom - 6.0f) * 0.12f;
            if (outer_fade < 0.2f) outer_fade = 0.2f;
            if (outer_fade > 1.0f) outer_fade = 1.0f;
            for (int i = 4; i < 8 && i < result.planet_count; i++) {
                result.amplitudes[i] *= outer_fade;
            }
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

    /* Convergence chord (L2.3 + S111):
     * Scan ALL brain insights, collect converging systems with timbres.
     * Blend their frequencies (weighted average) and merge harmonic
     * partials (take richest spectrum). More convergence = richer sound.
     * Single-system convergence sounds like that system's instrument.
     * Multi-system convergence sounds like a blended composite. */
    {
        float blend_freq = 0.0f;
        float blend_amp = 0.0f;
        float blend_partials[AS_MAX_HARMONICS];
        int blend_max_partials = 0;
        float weight_sum = 0.0f;
        int system_count = 0;
        int primary_sys = -1;

        for (int h = 0; h < AS_MAX_HARMONICS; h++)
            blend_partials[h] = 0.0f;

        /* Iterate all insights, collect timbres from converging systems */
        for (int i = 0; i < br.insight_count && i < BR_MAX_INSIGHTS; i++) {
            const br_insight_t *ins = &br.insights[i];
            if (ins->score < 0.2) continue;

            for (int s = 0; s < ins->system_count && s < BR_MAX_SYSTEMS; s++) {
                int sys = ins->systems[s];
                if (sys < 0 || sys >= (int)CD_SYS_COUNT) continue;
                if (!audio_culture_has_timbre((cd_system_t)sys)) continue;

                audio_culture_t culture = audio_culture_get((cd_system_t)sys);
                float w = (float)ins->score;

                blend_freq += culture.base_freq_hz * w;
                blend_amp += culture.base_amplitude * w;
                weight_sum += w;

                /* Merge partials: take max at each position for richest spectrum */
                for (int h = 0; h < culture.partial_count && h < AS_MAX_HARMONICS; h++) {
                    if (culture.partials[h] > blend_partials[h])
                        blend_partials[h] = culture.partials[h];
                }
                if (culture.partial_count > blend_max_partials)
                    blend_max_partials = culture.partial_count;

                if (primary_sys < 0 || w > 0.5f)
                    primary_sys = sys;
                system_count++;
            }
        }

        /* Apply blended timbre to oscillator slot 8 */
        if (system_count > 0 && weight_sum > 0.0f) {
            int slot = result.planet_count;
            if (slot < AS_MAX_PLANETS) {
                result.focused_system = primary_sys;
                result.frequencies[slot] = blend_freq / weight_sum;
                /* Amplitude scales with convergence: more systems = louder */
                float sys_boost = 1.0f + 0.15f * (float)(system_count - 1);
                result.amplitudes[slot] = (blend_amp / weight_sum) * 0.3f
                                        * (0.5f + 0.5f * result.event_intensity)
                                        * sys_boost;
                result.amplitudes[slot] = clampf(result.amplitudes[slot],
                                                 0.0f, 1.0f);
                result.waveform_types[slot] = 0; /* sine for composite */
                result.harmonic_counts[slot] = blend_max_partials;
                for (int h = 0; h < blend_max_partials && h < AS_MAX_HARMONICS; h++)
                    result.harmonic_amps[slot][h] = blend_partials[h];
                result.pan_positions[slot] = 0.0f;
                result.planet_count = slot + 1;
            }
        }
    }

    /* Reverb: combines warmth (view) and zoom level for spatial depth.
     * Cold views + far zoom = vast cathedral reverb (you're in the void).
     * Warm views + close zoom = intimate, dry (you're in the room).
     * Zoom dominates: even warm views get reverb when zoomed way out. */
    {
        float w = result.warmth;
        if (w < -1.0f) w = -1.0f;
        if (w > 1.0f) w = 1.0f;
        float cold_factor = (-w + 1.0f) * 0.5f; /* 0.0 at warmth=+1, 1.0 at warmth=-1 */

        /* Zoom reverb: far zoom adds reverb on top of warmth-based reverb */
        float zoom_reverb = 0.0f;
        if (log_zoom < 0.0f) {
            zoom_reverb = -log_zoom * 0.06f; /* up to +0.27 at log_zoom=-4.6 */
            if (zoom_reverb > 0.30f) zoom_reverb = 0.30f;
        } else if (log_zoom > 7.0f) {
            /* Close zoom: reduce reverb for intimacy */
            zoom_reverb = -(log_zoom - 7.0f) * 0.04f; /* up to -0.15 */
            if (zoom_reverb < -0.15f) zoom_reverb = -0.15f;
        }

        result.reverb_wet = 0.15f + 0.45f * cold_factor + zoom_reverb;
        if (result.reverb_wet < 0.05f) result.reverb_wet = 0.05f;
        if (result.reverb_wet > 0.75f) result.reverb_wet = 0.75f;

        /* Decay time: far = longer tail (up to 3.5s), close = shorter (0.8s) */
        float zoom_decay = 0.0f;
        if (log_zoom < -1.0f) {
            zoom_decay = -(log_zoom + 1.0f) * 0.4f;
            if (zoom_decay > 1.5f) zoom_decay = 1.5f;
        }
        result.reverb_decay_s = 1.0f + 1.0f * cold_factor + zoom_decay;
        if (result.reverb_decay_s > 3.5f) result.reverb_decay_s = 3.5f;
    }

    return result;
}
