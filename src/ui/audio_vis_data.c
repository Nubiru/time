/* audio_vis_data.c -- Pure visual data from audio parameters.
 *
 * Computes visual modulation values so the user can SEE the sound:
 * orbital rings breathe at planet frequencies, aspect lines glow
 * with consonance, convergence events radiate aura.
 *
 * All modulation is subtle (ring scale +-10%, glow 0-0.6 alpha).
 * The audio visualization should enhance, never overpower, the
 * existing visual presentation.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "audio_vis_data.h"

#include <math.h>
#include <string.h>

#ifndef PI
#define PI 3.14159265358979323846
#endif

/* --- Helpers --- */

static float clampf(float v, float lo, float hi)
{
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

/* --- Public API --- */

float av_ring_breath(float freq_hz, float amp, double real_time_sec)
{
    if (freq_hz <= 0.0f || amp <= 0.0f) {
        return 1.0f;
    }

    /* Subtle cosine pulse at the planet's frequency
     * Max deviation: +-0.1 * amplitude (so quiet planets barely move) */
    double phase = 2.0 * PI * (double)freq_hz * real_time_sec;
    float deviation = 0.1f * amp * (float)cos(phase);

    return 1.0f + deviation;
}

float av_aspect_glow(float consonance)
{
    consonance = clampf(consonance, 0.0f, 1.0f);
    /* Squared curve: low consonance barely glows, high consonance shines */
    return consonance * consonance;
}

float av_convergence_alpha(float density, float intensity)
{
    density = clampf(density, 0.0f, 1.0f);
    intensity = clampf(intensity, 0.0f, 1.0f);

    /* Blend density and intensity, cap at 0.6 alpha */
    float raw = 0.5f * density + 0.5f * intensity;
    return raw * 0.6f;
}

av_snapshot_t av_from_params(const audio_params_t *params,
                             double real_time_sec)
{
    av_snapshot_t snap;
    memset(&snap, 0, sizeof(snap));

    if (params == NULL) {
        return snap;
    }

    /* Rings */
    int count = params->planet_count;
    if (count > AV_MAX_RINGS) count = AV_MAX_RINGS;
    snap.ring_count = count;

    for (int i = 0; i < count; i++) {
        av_ring_t *r = &snap.rings[i];
        float freq = params->frequencies[i];
        float amp = params->amplitudes[i];

        r->scale = av_ring_breath(freq, amp, real_time_sec);
        r->opacity = clampf(0.3f + 0.7f * amp, 0.0f, 1.0f);
        r->glow = clampf(amp * params->consonance, 0.0f, 1.0f);

        /* Warmth shifts hue: warm=shift toward gold, cold=shift toward blue */
        float w = clampf(params->warmth, -1.0f, 1.0f);
        r->hue_shift = (w + 1.0f) * 0.5f; /* 0.0=cold/blue, 1.0=warm/gold */
    }

    /* Aspect line glow */
    snap.aspect_line.glow = av_aspect_glow(params->consonance);
    snap.aspect_line.width = 1.0f + 2.0f * params->consonance;
    snap.aspect_line.alpha = clampf(params->consonance * 0.8f, 0.0f, 0.8f);

    /* Convergence aura */
    snap.convergence_alpha = av_convergence_alpha(
        params->event_density, params->event_intensity);
    snap.convergence_aura = clampf(params->event_density, 0.0f, 1.0f);

    /* Moon halo */
    snap.moon_halo = clampf(params->moon_factor, 0.0f, 1.0f);

    /* Beat flash: pulse_factor ranges 0.85-1.0, map deviation to brightness */
    float pulse_dev = 1.0f - params->pulse_factor; /* 0.0-0.15 */
    snap.beat_flash = clampf(pulse_dev, 0.0f, 0.15f);

    /* Surprise spark */
    snap.surprise_spark = clampf(params->surprise_factor, 0.0f, 1.0f);

    /* Scene dimming (no meditation state in audio_params — defaults to 0) */
    snap.scene_dim = 0.0f;

    return snap;
}
