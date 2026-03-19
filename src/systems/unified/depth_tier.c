/* depth_tier.c — Camera scale -> display detail tier mapping
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 *
 * Sources: Visual design spec, golden layout system. */

#include "depth_tier.h"

/* Scale levels: 0=PERSONAL, 1=EARTH, 2=INNER, 3=SOLAR,
 *               4=LOCAL,    5=GALAXY, 6=UNIVERSE */
#define SCALE_MIN 0
#define SCALE_MAX 6
#define SCALE_COUNT 7

/* Default scale -> tier mapping table */
static const depth_tier_t s_default_tier[SCALE_COUNT] = {
    DEPTH_TIER_PANEL,   /* 0: PERSONAL */
    DEPTH_TIER_CARD,    /* 1: EARTH */
    DEPTH_TIER_CARD,    /* 2: INNER */
    DEPTH_TIER_GLANCE,  /* 3: SOLAR */
    DEPTH_TIER_GLYPH,   /* 4: LOCAL */
    DEPTH_TIER_GLYPH,   /* 5: GALAXY */
    DEPTH_TIER_GLYPH    /* 6: UNIVERSE */
};

/* Clamp integer to [lo, hi]. */
static int clamp_int(int val, int lo, int hi)
{
    if (val < lo) return lo;
    if (val > hi) return hi;
    return val;
}

/* Check if a system gets +1 promotion at the given scale.
 * Returns 1 if promoted, 0 otherwise. */
static int is_promoted(int system_id, int scale)
{
    /* Geological / Cosmic: promoted at galaxy (5) and universe (6) */
    if (system_id == TS_SYS_GEOLOGICAL || system_id == TS_SYS_COSMIC) {
        return (scale >= 5) ? 1 : 0;
    }
    /* Buddhist (kalpa): promoted at galaxy (5) and universe (6) */
    if (system_id == TS_SYS_BUDDHIST) {
        return (scale >= 5) ? 1 : 0;
    }
    /* Islamic (prayer times): promoted at personal (0) and earth (1) */
    if (system_id == TS_SYS_ISLAMIC) {
        return (scale <= 1) ? 1 : 0;
    }
    return 0;
}

depth_tier_t depth_tier_for_scale(int camera_scale, int system_id,
                                  int user_override)
{
    /* Handle user override first */
    if (user_override >= 0) {
        return (depth_tier_t)clamp_int(user_override,
                                       DEPTH_TIER_GLYPH, DEPTH_TIER_BOARD);
    }

    /* Clamp scale to valid range */
    int scale = clamp_int(camera_scale, SCALE_MIN, SCALE_MAX);

    /* Start with default tier for this scale */
    int tier = (int)s_default_tier[scale];

    /* Apply system-specific promotion if system_id is valid */
    if (system_id >= 0 && system_id < TS_SYS_COUNT) {
        if (is_promoted(system_id, scale)) {
            tier += 1;
        }
    }

    /* Clamp result to valid tier range */
    tier = clamp_int(tier, DEPTH_TIER_GLYPH, DEPTH_TIER_BOARD);

    return (depth_tier_t)tier;
}

depth_tier_t depth_tier_default_for_scale(int camera_scale)
{
    int scale = clamp_int(camera_scale, SCALE_MIN, SCALE_MAX);
    return s_default_tier[scale];
}

const char *depth_tier_name(depth_tier_t tier)
{
    switch (tier) {
    case DEPTH_TIER_GLYPH:  return "Glyph";
    case DEPTH_TIER_GLANCE: return "Glance";
    case DEPTH_TIER_CARD:   return "Card";
    case DEPTH_TIER_PANEL:  return "Panel";
    case DEPTH_TIER_BOARD:  return "Board";
    default:                return "?";
    }
}

int depth_tier_count(void)
{
    return DEPTH_TIER_COUNT;
}
