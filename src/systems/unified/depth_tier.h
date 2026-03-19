/* depth_tier.h — Camera scale -> display detail tier mapping
 *
 * Maps camera scale level (0-6) + system identity -> display detail
 * tier (T0-T4). The depth controller's core mapping function.
 *
 * Tiers control how much information is rendered for each knowledge system:
 *   T0 Glyph  — one symbol/color
 *   T1 Glance — one-line summary
 *   T2 Card   — card with title + 3 lines
 *   T3 Panel  — detail panel, full narrative
 *   T4 Board  — full-screen system view
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 *
 * Sources: Visual design spec (VISUAL_DESIGN.md), golden layout system. */

#ifndef TIME_DEPTH_TIER_H
#define TIME_DEPTH_TIER_H

#include "today_summary.h"  /* ts_system_t */

typedef enum {
    DEPTH_TIER_GLYPH  = 0,  /* T0: one symbol/color */
    DEPTH_TIER_GLANCE = 1,  /* T1: one-line summary */
    DEPTH_TIER_CARD   = 2,  /* T2: card with title + 3 lines */
    DEPTH_TIER_PANEL  = 3,  /* T3: detail panel, full narrative */
    DEPTH_TIER_BOARD  = 4,  /* T4: full-screen system view */
    DEPTH_TIER_COUNT  = 5
} depth_tier_t;

/* Compute display tier for a camera scale + system combination.
 *
 * camera_scale: 0-6 (PERSONAL..UNIVERSE). Clamped if out of range.
 * system_id:    ts_system_t value (0-19). Out of range uses default.
 * user_override: -1 = no override (use computed), 0-4 forces specific tier.
 *                Negative values other than -1 treated as no override.
 *                Values > BOARD clamped to BOARD.
 *
 * Returns: the appropriate depth_tier_t. */
depth_tier_t depth_tier_for_scale(int camera_scale, int system_id,
                                  int user_override);

/* Default tier for a camera scale with no system override.
 * Equivalent to depth_tier_for_scale(scale, -1, -1). */
depth_tier_t depth_tier_default_for_scale(int camera_scale);

/* Human-readable tier name.
 * Returns: "Glyph", "Glance", "Card", "Panel", "Board", or "?" for invalid. */
const char *depth_tier_name(depth_tier_t tier);

/* Number of tier levels (always DEPTH_TIER_COUNT = 5). */
int depth_tier_count(void);

#endif /* TIME_DEPTH_TIER_H */
