/* render_lod.h — Level-of-Detail management for render passes.
 *
 * Computes mesh resolution, particle counts, and quality settings
 * based on camera distance, viewport size, and frame time history.
 * Helps maintain 60fps by automatically reducing quality when needed.
 *
 * LOD Tiers:
 *   LOD_ULTRA  (0): Maximum quality, no reduction
 *   LOD_HIGH   (1): Slight reduction (~75% detail)
 *   LOD_MEDIUM (2): Moderate reduction (~50% detail)
 *   LOD_LOW    (3): Significant reduction (~25% detail)
 *   LOD_MINIMAL(4): Minimum viable quality
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_RENDER_LOD_H
#define TIME_RENDER_LOD_H

#define LOD_TIER_COUNT    5
#define LOD_FRAME_HISTORY 60  /* track last 60 frames for averaging */

typedef enum {
    LOD_ULTRA   = 0,
    LOD_HIGH    = 1,
    LOD_MEDIUM  = 2,
    LOD_LOW     = 3,
    LOD_MINIMAL = 4
} lod_tier_t;

/* Per-pass quality parameters at a given LOD tier */
typedef struct {
    int   star_count_max;      /* max visible stars */
    int   ring_segments;       /* segments per ring arc */
    int   atmosphere_subdivs;  /* atmosphere mesh resolution */
    int   sky_dome_alt_steps;  /* sky dome altitude divisions */
    int   sky_dome_az_steps;   /* sky dome azimuth divisions */
    float particle_density;    /* multiplier on particle counts (0-1) */
    float shadow_quality;      /* shadow map resolution multiplier */
    float bloom_passes;        /* number of bloom blur passes (1-5) */
    int   text_max_glyphs;     /* max text glyphs per frame */
    int   enable_post_process; /* 0=skip post-processing entirely */
} lod_params_t;

/* Frame time tracking state */
typedef struct {
    float      frame_times[LOD_FRAME_HISTORY]; /* ring buffer of frame times (ms) */
    int        write_index;                     /* next write position */
    int        sample_count;                    /* how many samples recorded */
    lod_tier_t current_tier;                    /* current LOD tier */
    float      target_frame_ms;                 /* target frame time (16.67 for 60fps) */
} lod_state_t;

/* Initialize LOD state (target 60fps, LOD_ULTRA). */
lod_state_t lod_init(float target_fps);

/* Record a frame time and potentially adjust LOD tier. */
lod_state_t lod_record_frame(lod_state_t state, float frame_time_ms);

/* Get average frame time over recent history. */
float lod_avg_frame_time(const lod_state_t *state);

/* Get current LOD tier. */
lod_tier_t lod_current_tier(const lod_state_t *state);

/* Get quality parameters for a specific tier. */
lod_params_t lod_params_for_tier(lod_tier_t tier);

/* Get quality parameters for current state. */
lod_params_t lod_current_params(const lod_state_t *state);

/* Compute LOD tier from camera distance (zoom-based LOD).
 * Far away = lower detail (things are smaller).
 * close_distance: distance below which LOD_ULTRA is used.
 * far_distance: distance above which LOD_MINIMAL is used. */
lod_tier_t lod_from_distance(float camera_distance,
                             float close_distance,
                             float far_distance);

/* Combine distance LOD and performance LOD (take the worse one). */
lod_tier_t lod_combine(lod_tier_t distance_lod, lod_tier_t performance_lod);

/* Human-readable tier name. */
const char *lod_tier_name(lod_tier_t tier);

/* Per-pass time budget in milliseconds for a given tier.
 * total_passes: number of render passes sharing the frame budget.
 * Returns recommended max time in ms for each pass. */
float lod_pass_budget_ms(lod_tier_t tier, int total_passes);

#endif /* TIME_RENDER_LOD_H */
