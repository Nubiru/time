/* text_lod.h — Text Level of Detail (LANGUAGE L1.2)
 *
 * Maps depth_tier_t + per-system data → text at the correct verbosity.
 * Provides transition state for smooth fading between depth levels.
 *
 * Tier mapping:
 *   T0 Glyph  — symbol only (glyph from depth_interp)
 *   T1 Glance — one-line summary (glance from depth_interp)
 *   T2 Card   — date + note (entry data + glance)
 *   T3 Panel  — full detail paragraph (depth_interp detail)
 *   T4 Board  — all available text combined
 *
 * Transition timing uses phi (1.618s total: fade out + fade in).
 *
 * Pure functions: no GL, no malloc, no globals, no side effects.
 *
 * Sources: depth_tier.h (tiers), depth_interpret.h (per-system interp),
 * today_summary.h (ts_entry_t), golden_layout.h (phi timing). */

#ifndef TIME_TEXT_LOD_H
#define TIME_TEXT_LOD_H

#include "depth_tier.h"
#include "depth_interpret.h"
#include "today_summary.h"
#include <string.h>
#include <stdio.h>

/* ===================================================================
 * Constants
 * =================================================================== */

#define TEXT_LOD_MAX 512

/* Phi-timed transition: total duration = phi * base */
#define TEXT_LOD_FADE_BASE     1.0
#define TEXT_LOD_FADE_DURATION 1.618 /* phi seconds total */

/* ===================================================================
 * Text LOD result — text at a specific tier
 * =================================================================== */

typedef struct {
    char          text[TEXT_LOD_MAX];  /* composed text at this tier */
    depth_tier_t  tier;               /* which tier this was produced for */
    int           system_id;          /* ts_system_t */
} text_lod_t;

/* ===================================================================
 * Transition state — smooth fade between depth levels
 * =================================================================== */

typedef struct {
    depth_tier_t  current_tier;   /* tier being displayed */
    depth_tier_t  target_tier;    /* tier transitioning to */
    double        opacity;        /* 0.0 (invisible) to 1.0 (fully visible) */
    double        elapsed;        /* seconds into transition */
    int           transitioning;  /* 1 if actively transitioning */
    int           phase;          /* 0 = fading out old, 1 = fading in new */
} text_lod_transition_t;

/* ===================================================================
 * API
 * =================================================================== */

/* Produce text for a system at a given depth tier.
 * entry: from today_summary (may be NULL → empty result).
 * interp: from depth_interpret (may be NULL → uses entry data only).
 * tier: desired depth tier (T0-T4).
 * Returns text_lod_t with composed text. */
text_lod_t text_lod_for_system(const ts_entry_t *entry,
                                const depth_interp_t *interp,
                                depth_tier_t tier);

/* Initialize a transition state at a given tier. */
void text_lod_transition_init(text_lod_transition_t *tr,
                               depth_tier_t initial_tier);

/* Set a new target tier. Starts transition if different from current. */
void text_lod_transition_set_target(text_lod_transition_t *tr,
                                     depth_tier_t target);

/* Update transition state with elapsed time (seconds).
 * Call each frame. Updates opacity and switches tier at midpoint. */
void text_lod_transition_update(text_lod_transition_t *tr, double dt);

#endif /* TIME_TEXT_LOD_H */
