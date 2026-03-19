/* brain_depth.h — Layered depth content for knowledge systems (Track 26, L2.3)
 *
 * Provides content at each depth tier for each knowledge system.
 * Uses depth_tier_t from depth_tier.h to determine detail level:
 *   T0 GLYPH  — symbol/number (one word)
 *   T1 GLANCE — one-line summary
 *   T2 CARD   — title + 3 lines
 *   T3 PANEL  — full narrative paragraph
 *   T4 BOARD  — system overview with cycle data
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_BRAIN_DEPTH_H
#define TIME_BRAIN_DEPTH_H

#include "brain_types.h"
#include "depth_tier.h"
#include "today_summary.h"

/* Maximum content length per tier */
#define BR_DEPTH_GLYPH_MAX   16
#define BR_DEPTH_GLANCE_MAX  64
#define BR_DEPTH_CARD_MAX   256
#define BR_DEPTH_PANEL_MAX  512
#define BR_DEPTH_BOARD_MAX  512

/* Content at a specific depth tier */
typedef struct {
    depth_tier_t  tier;
    ts_system_t   system;
    char          content[BR_DEPTH_BOARD_MAX];
    int           content_len;
} br_depth_content_t;

/* Get content for a system at a specific depth tier and Julian Day.
 * Fills out->content with tier-appropriate text.
 * Returns 1 on success, 0 if system or tier is invalid. */
int br_depth_get(double jd, ts_system_t system, depth_tier_t tier,
                 br_depth_content_t *out);

/* Get content length limit for a given tier. */
int br_depth_max_len(depth_tier_t tier);

/* Check if a system has depth content implemented.
 * Returns 1 if content is available at all tiers. */
int br_depth_has_system(ts_system_t system);

/* Count of systems with depth content implemented. */
int br_depth_system_count(void);

#endif /* TIME_BRAIN_DEPTH_H */
