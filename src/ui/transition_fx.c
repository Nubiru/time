/* src/ui/transition_fx.c — transition post-processing effect deltas
 *
 * Pure: no GL, no globals, no I/O, no malloc. */

#include "transition_fx.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* --- Helpers --- */

static float clamp01(float x)
{
    if (x < 0.0f) return 0.0f;
    if (x > 1.0f) return 1.0f;
    return x;
}

/* Bell curve: sin(progress * PI). Peaks at 0.5, zero at 0 and 1. */
static float envelope_bell(float progress)
{
    return sinf(clamp01(progress) * (float)M_PI);
}

/* Ramp: linear rise from start to end, clamped [0,1]. */
static float envelope_ramp(float progress, float start, float end)
{
    if (fabsf(end - start) < 1e-6f) return 1.0f;
    return clamp01((progress - start) / (end - start));
}

/* Plateau: ramp up to rise_end, hold, ramp down from fall_start. */
static float envelope_plateau(float progress, float rise_end, float fall_start)
{
    if (progress < rise_end) {
        return envelope_ramp(progress, 0.0f, rise_end);
    }
    if (progress > fall_start) {
        return 1.0f - envelope_ramp(progress, fall_start, 1.0f);
    }
    return 1.0f;
}

/* --- Preset profiles --- */

static tfx_adjustments_t compute_fly_to(float intensity, float progress)
{
    float env = envelope_bell(progress);
    float scale = intensity * env;
    tfx_adjustments_t adj = {0};
    adj.bloom_boost     =  0.3f  * scale;
    adj.bloom_threshold = -0.15f * scale;
    adj.vignette_boost  =  0.2f  * scale;
    adj.vignette_radius = -0.15f * scale;
    return adj;
}

static tfx_adjustments_t compute_earth_view(float intensity, float progress)
{
    /* Ramp up to 0.6, then hold at peak until 1.0 */
    float env = envelope_ramp(progress, 0.0f, 0.6f);
    float scale = intensity * env;
    tfx_adjustments_t adj = {0};
    adj.bloom_boost    = 0.4f  * scale;
    adj.exposure_boost = 0.2f  * scale;
    adj.vignette_boost = 0.15f * scale;
    return adj;
}

static tfx_adjustments_t compute_birth_travel(float intensity, float progress)
{
    /* Strongest during TRAVEL phase (0.15-0.65) */
    float env = envelope_plateau(progress, 0.15f, 0.65f);
    float scale = intensity * env;
    tfx_adjustments_t adj = {0};
    adj.grain_boost    = 0.04f * scale;
    adj.warmth_shift   = 0.6f  * scale;
    adj.bloom_boost    = 0.2f  * scale;
    adj.vignette_boost = 0.25f * scale;
    return adj;
}

static tfx_adjustments_t compute_convergence(float intensity,
                                              float progress)
{
    /* Steady glow — no envelope, just intensity */
    (void)progress;
    tfx_adjustments_t adj = {0};
    adj.bloom_boost     =  0.35f * intensity;
    adj.bloom_threshold = -0.2f  * intensity;
    adj.warmth_shift    =  0.4f  * intensity;
    return adj;
}

static tfx_adjustments_t compute_enter_zoom(float intensity, float progress)
{
    /* Ramp up during reveal phase (progress > 0.7) */
    float env = envelope_ramp(progress, 0.7f, 1.0f);
    float scale = intensity * env;
    tfx_adjustments_t adj = {0};
    adj.bloom_boost    =  0.5f  * scale;
    adj.exposure_boost =  0.15f * scale;
    adj.vignette_boost = -0.1f  * scale;
    return adj;
}

/* --- Public API --- */

transition_fx_t tfx_create(void)
{
    transition_fx_t fx;
    fx.preset    = TFX_NONE;
    fx.intensity = 0.0f;
    fx.progress  = 0.0f;
    fx.elapsed   = 0.0f;
    return fx;
}

transition_fx_t tfx_activate(transition_fx_t fx, tfx_preset_e preset,
                              float intensity)
{
    fx.preset    = preset;
    fx.intensity = clamp01(intensity);
    fx.progress  = 0.0f;
    fx.elapsed   = 0.0f;
    return fx;
}

transition_fx_t tfx_set_progress(transition_fx_t fx, float progress)
{
    fx.progress = clamp01(progress);
    return fx;
}

transition_fx_t tfx_set_intensity(transition_fx_t fx, float intensity)
{
    fx.intensity = clamp01(intensity);
    return fx;
}

transition_fx_t tfx_tick(transition_fx_t fx, float dt)
{
    if (dt > 0.0f) {
        fx.elapsed += dt;
    }
    return fx;
}

transition_fx_t tfx_deactivate(transition_fx_t fx)
{
    fx.preset    = TFX_NONE;
    fx.intensity = 0.0f;
    fx.progress  = 0.0f;
    fx.elapsed   = 0.0f;
    return fx;
}

tfx_adjustments_t tfx_compute(tfx_preset_e preset, float intensity,
                               float progress)
{
    switch (preset) {
    case TFX_FLY_TO:       return compute_fly_to(intensity, progress);
    case TFX_EARTH_VIEW:   return compute_earth_view(intensity, progress);
    case TFX_BIRTH_TRAVEL: return compute_birth_travel(intensity, progress);
    case TFX_CONVERGENCE:  return compute_convergence(intensity, progress);
    case TFX_ENTER_ZOOM:   return compute_enter_zoom(intensity, progress);
    case TFX_NONE:         /* fall through */
    case TFX_PRESET_COUNT: /* fall through */
    default:               return tfx_zero();
    }
}

tfx_adjustments_t tfx_adjustments(transition_fx_t fx)
{
    return tfx_compute(fx.preset, fx.intensity, fx.progress);
}

tfx_preset_e tfx_preset(transition_fx_t fx)
{
    return fx.preset;
}

int tfx_active(transition_fx_t fx)
{
    return fx.preset != TFX_NONE;
}

float tfx_intensity(transition_fx_t fx)
{
    return fx.intensity;
}

tfx_adjustments_t tfx_zero(void)
{
    tfx_adjustments_t adj = {0};
    return adj;
}

tfx_adjustments_t tfx_combine(tfx_adjustments_t a, tfx_adjustments_t b)
{
    tfx_adjustments_t c;
    c.bloom_boost     = a.bloom_boost     + b.bloom_boost;
    c.bloom_threshold = a.bloom_threshold + b.bloom_threshold;
    c.vignette_boost  = a.vignette_boost  + b.vignette_boost;
    c.vignette_radius = a.vignette_radius + b.vignette_radius;
    c.exposure_boost  = a.exposure_boost  + b.exposure_boost;
    c.grain_boost     = a.grain_boost     + b.grain_boost;
    c.warmth_shift    = a.warmth_shift    + b.warmth_shift;
    return c;
}
