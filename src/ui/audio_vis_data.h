/* audio_vis_data.h -- Pure visual data from audio parameters.
 *
 * Computes visual modulation values so the render pass can make
 * the user SEE the sound:
 *   - Ring pulse: orbital rings breathe at planet frequencies
 *   - Aspect glow: aspect lines glow with consonance
 *   - Convergence aura: radiating glow at convergence events
 *   - Moon halo: halo intensity from lunar envelope
 *   - Beat flash: global brightness pulse at tempo
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_AUDIO_VIS_DATA_H
#define TIME_AUDIO_VIS_DATA_H

#include "audio_score.h"

#define AV_MAX_RINGS AS_MAX_PLANETS

/* Per-ring visual modulation */
typedef struct {
    float opacity;     /* 0.0-1.0 base ring opacity */
    float scale;       /* 0.8-1.2 ring scale factor (breathing) */
    float glow;        /* 0.0-1.0 additional glow intensity */
    float hue_shift;   /* 0.0-1.0 color shift from warmth */
} av_ring_t;

/* Aspect line visual data */
typedef struct {
    float glow;        /* 0.0-1.0 line glow intensity */
    float width;       /* 1.0-3.0 line width multiplier */
    float alpha;       /* 0.0-1.0 line alpha */
} av_aspect_t;

/* Complete audio visualization snapshot */
typedef struct {
    av_ring_t rings[AV_MAX_RINGS];
    int ring_count;

    av_aspect_t aspect_line;    /* shared aspect glow (from consonance) */

    float convergence_aura;     /* 0.0-1.0 convergence glow radius factor */
    float convergence_alpha;    /* 0.0-0.6 convergence aura alpha */

    float moon_halo;            /* 0.0-1.0 moon halo intensity */

    float beat_flash;           /* 0.0-0.15 global brightness pulse at tempo */

    float surprise_spark;       /* 0.0-1.0 spark effect from brain surprise */

    float scene_dim;            /* 0.0-1.0 overall scene dimming (meditation) */
} av_snapshot_t;

/* Compute full audio visualization snapshot from audio parameters
 * and a real-time clock (for animation). If params is NULL, returns
 * a zeroed/default snapshot. */
av_snapshot_t av_from_params(const audio_params_t *params,
                             double real_time_sec);

/* Compute single ring breathing factor from frequency and time.
 * Returns a scale factor around 1.0 that pulses at freq_hz.
 * Amplitude of pulse scales with amp (quieter = less breathing). */
float av_ring_breath(float freq_hz, float amp, double real_time_sec);

/* Compute aspect line glow from consonance (0-1).
 * Higher consonance = brighter glow. */
float av_aspect_glow(float consonance);

/* Compute convergence aura alpha from event density and intensity. */
float av_convergence_alpha(float density, float intensity);

#endif /* TIME_AUDIO_VIS_DATA_H */
