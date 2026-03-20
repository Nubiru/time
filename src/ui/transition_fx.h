/* src/ui/transition_fx.h — transition post-processing effect deltas
 *
 * Maps motion transition states to post-processing parameter adjustments.
 * Outputs DELTA values that the caller ADDS to the base pp_config_t.
 * This avoids coupling — transition_fx never needs to know pp_config_t.
 *
 * Pure: no GL, no globals, no I/O, no malloc. Returns by value. */

#ifndef TIME_TRANSITION_FX_H
#define TIME_TRANSITION_FX_H

/* Effect presets — which transition type is active */
typedef enum {
    TFX_NONE,          /* no effects */
    TFX_FLY_TO,        /* camera flight (click-to-focus, fly-to) */
    TFX_EARTH_VIEW,    /* space <-> earth transition */
    TFX_BIRTH_TRAVEL,  /* birth moment time-travel */
    TFX_CONVERGENCE,   /* convergence event active */
    TFX_ENTER_ZOOM,    /* initial "Enter Time" zoom-out */
    TFX_PRESET_COUNT
} tfx_preset_e;

/* Post-processing DELTA adjustments.
 * All values are ADDITIVE — caller adds these to base config.
 * Values are 0.0 when no effect is active. */
typedef struct {
    float bloom_boost;        /* add to bloom_intensity (+0.0 to +0.5) */
    float bloom_threshold;    /* add to bloom_threshold (-0.3 to 0.0, lower = more bloom) */
    float vignette_boost;     /* add to vignette_strength (+0.0 to +0.3) */
    float vignette_radius;    /* add to vignette_radius (-0.2 to 0.0, tighter) */
    float exposure_boost;     /* add to exposure (+0.0 to +0.3) */
    float grain_boost;        /* add to grain_intensity (+0.0 to +0.05) */
    float warmth_shift;       /* 0.0=no change, +1.0=full warm shift (for color grading) */
} tfx_adjustments_t;

/* State for transition effects */
typedef struct {
    tfx_preset_e preset;     /* which effect is active */
    float intensity;          /* 0.0 to 1.0 — how strong the effect is */
    float progress;           /* 0.0 to 1.0 — transition progress (from motion module) */
    float elapsed;            /* time since effect started */
} transition_fx_t;

/* Create idle (no effects) */
transition_fx_t tfx_create(void);

/* Activate a preset with given intensity (0-1).
 * Intensity modulates all delta values. */
transition_fx_t tfx_activate(transition_fx_t fx, tfx_preset_e preset, float intensity);

/* Update progress (from the motion module driving this transition).
 * progress: 0.0 = start of transition, 1.0 = end */
transition_fx_t tfx_set_progress(transition_fx_t fx, float progress);

/* Update intensity (for dynamic effects like convergence that change over time) */
transition_fx_t tfx_set_intensity(transition_fx_t fx, float intensity);

/* Advance time */
transition_fx_t tfx_tick(transition_fx_t fx, float dt);

/* Deactivate (returns to idle) */
transition_fx_t tfx_deactivate(transition_fx_t fx);

/* Get current adjustments — the DELTA values to add to base pp_config */
tfx_adjustments_t tfx_adjustments(transition_fx_t fx);

/* Query */
tfx_preset_e tfx_preset(transition_fx_t fx);
int tfx_active(transition_fx_t fx);
float tfx_intensity(transition_fx_t fx);

/* Get adjustments for a specific preset at given intensity and progress.
 * Stateless convenience — doesn't need a transition_fx_t.
 * Useful for testing or one-shot queries. */
tfx_adjustments_t tfx_compute(tfx_preset_e preset, float intensity, float progress);

/* Zero adjustments (identity) */
tfx_adjustments_t tfx_zero(void);

/* Combine two adjustment sets (add values) */
tfx_adjustments_t tfx_combine(tfx_adjustments_t a, tfx_adjustments_t b);

#endif /* TIME_TRANSITION_FX_H */
