/* render_lod.c — Level-of-Detail management for render passes.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "render_lod.h"

/* ------------------------------------------------------------------ */
/* Preset quality tables                                               */
/* ------------------------------------------------------------------ */

static const lod_params_t LOD_PRESETS[LOD_TIER_COUNT] = {
    /* LOD_ULTRA */
    {
        .star_count_max    = 5000,
        .ring_segments     = 8,
        .atmosphere_subdivs = 32,
        .sky_dome_alt_steps = 24,
        .sky_dome_az_steps  = 48,
        .particle_density  = 1.0f,
        .shadow_quality    = 1.0f,
        .bloom_passes      = 5.0f,
        .text_max_glyphs   = 2048,
        .enable_post_process = 1
    },
    /* LOD_HIGH */
    {
        .star_count_max    = 3750,
        .ring_segments     = 6,
        .atmosphere_subdivs = 24,
        .sky_dome_alt_steps = 18,
        .sky_dome_az_steps  = 36,
        .particle_density  = 0.75f,
        .shadow_quality    = 0.75f,
        .bloom_passes      = 4.0f,
        .text_max_glyphs   = 1536,
        .enable_post_process = 1
    },
    /* LOD_MEDIUM */
    {
        .star_count_max    = 2500,
        .ring_segments     = 4,
        .atmosphere_subdivs = 16,
        .sky_dome_alt_steps = 12,
        .sky_dome_az_steps  = 24,
        .particle_density  = 0.5f,
        .shadow_quality    = 0.5f,
        .bloom_passes      = 3.0f,
        .text_max_glyphs   = 1024,
        .enable_post_process = 1
    },
    /* LOD_LOW */
    {
        .star_count_max    = 1250,
        .ring_segments     = 3,
        .atmosphere_subdivs = 8,
        .sky_dome_alt_steps = 8,
        .sky_dome_az_steps  = 16,
        .particle_density  = 0.35f,
        .shadow_quality    = 0.25f,
        .bloom_passes      = 2.0f,
        .text_max_glyphs   = 512,
        .enable_post_process = 0
    },
    /* LOD_MINIMAL */
    {
        .star_count_max    = 500,
        .ring_segments     = 2,
        .atmosphere_subdivs = 4,
        .sky_dome_alt_steps = 4,
        .sky_dome_az_steps  = 8,
        .particle_density  = 0.2f,
        .shadow_quality    = 0.0f,
        .bloom_passes      = 1.0f,
        .text_max_glyphs   = 256,
        .enable_post_process = 0
    }
};

static const char *const LOD_TIER_NAMES[LOD_TIER_COUNT] = {
    "Ultra",
    "High",
    "Medium",
    "Low",
    "Minimal"
};

/* Hysteresis: require at least this many samples before adjusting tier */
#define LOD_MIN_SAMPLES 30

/* Thresholds relative to target frame time */
#define LOD_DEGRADE_RATIO 1.5f  /* avg > target * 1.5 → lower quality */
#define LOD_UPGRADE_RATIO 0.7f  /* avg < target * 0.7 → higher quality */

/* ------------------------------------------------------------------ */
/* Implementation                                                      */
/* ------------------------------------------------------------------ */

lod_state_t lod_init(float target_fps) {
    lod_state_t s;
    for (int i = 0; i < LOD_FRAME_HISTORY; i++) {
        s.frame_times[i] = 0.0f;
    }
    s.write_index = 0;
    s.sample_count = 0;
    s.current_tier = LOD_ULTRA;
    s.target_frame_ms = (target_fps > 0.0f) ? (1000.0f / target_fps) : 16.67f;
    return s;
}

lod_state_t lod_record_frame(lod_state_t state, float frame_time_ms) {
    /* Write into ring buffer */
    state.frame_times[state.write_index] = frame_time_ms;
    state.write_index = (state.write_index + 1) % LOD_FRAME_HISTORY;
    if (state.sample_count < LOD_FRAME_HISTORY) {
        state.sample_count++;
    }

    /* Only adjust tier after enough samples for a stable average */
    if (state.sample_count >= LOD_MIN_SAMPLES) {
        float avg = lod_avg_frame_time(&state);
        float degrade_threshold = state.target_frame_ms * LOD_DEGRADE_RATIO;
        float upgrade_threshold = state.target_frame_ms * LOD_UPGRADE_RATIO;

        if (avg > degrade_threshold && state.current_tier < LOD_MINIMAL) {
            state.current_tier = (lod_tier_t)(state.current_tier + 1);
        } else if (avg < upgrade_threshold && state.current_tier > LOD_ULTRA) {
            state.current_tier = (lod_tier_t)(state.current_tier - 1);
        }
    }

    return state;
}

float lod_avg_frame_time(const lod_state_t *state) {
    if (state->sample_count == 0) {
        return 0.0f;
    }
    float sum = 0.0f;
    for (int i = 0; i < state->sample_count; i++) {
        sum += state->frame_times[i];
    }
    return sum / (float)state->sample_count;
}

lod_tier_t lod_current_tier(const lod_state_t *state) {
    return state->current_tier;
}

lod_params_t lod_params_for_tier(lod_tier_t tier) {
    if (tier < LOD_ULTRA || tier > LOD_MINIMAL) {
        return LOD_PRESETS[LOD_ULTRA];
    }
    return LOD_PRESETS[tier];
}

lod_params_t lod_current_params(const lod_state_t *state) {
    return lod_params_for_tier(state->current_tier);
}

lod_tier_t lod_from_distance(float camera_distance,
                             float close_distance,
                             float far_distance) {
    if (camera_distance <= close_distance) {
        return LOD_ULTRA;
    }
    if (camera_distance >= far_distance) {
        return LOD_MINIMAL;
    }

    /* Linear interpolation between close and far */
    float t = (camera_distance - close_distance) /
              (far_distance - close_distance);

    /* Map t (0..1) to tier (0..LOD_TIER_COUNT-1) */
    int tier = (int)(t * (float)(LOD_TIER_COUNT - 1) + 0.5f);
    if (tier < 0) tier = 0;
    if (tier > LOD_TIER_COUNT - 1) tier = LOD_TIER_COUNT - 1;
    return (lod_tier_t)tier;
}

lod_tier_t lod_combine(lod_tier_t distance_lod, lod_tier_t performance_lod) {
    return (distance_lod > performance_lod) ? distance_lod : performance_lod;
}

const char *lod_tier_name(lod_tier_t tier) {
    if (tier < LOD_ULTRA || tier > LOD_MINIMAL) {
        return "Unknown";
    }
    return LOD_TIER_NAMES[tier];
}

float lod_pass_budget_ms(lod_tier_t tier, int total_passes) {
    if (total_passes <= 0) {
        return 0.0f;
    }

    /* Target frame time depends on tier:
     * Higher tiers (lower quality) target a longer frame time (e.g. 30fps)
     * so passes get a more relaxed budget. */
    static const float TIER_TARGET_MS[LOD_TIER_COUNT] = {
        16.67f,  /* ULTRA: 60fps */
        16.67f,  /* HIGH: 60fps */
        22.22f,  /* MEDIUM: 45fps */
        33.33f,  /* LOW: 30fps */
        33.33f   /* MINIMAL: 30fps */
    };

    int clamped_tier = tier;
    if (clamped_tier < 0) clamped_tier = 0;
    if (clamped_tier > LOD_TIER_COUNT - 1) clamped_tier = LOD_TIER_COUNT - 1;

    return TIER_TARGET_MS[clamped_tier] / (float)total_passes;
}
