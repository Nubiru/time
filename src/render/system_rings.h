/* system_rings.h — Knowledge system concentric ring definitions.
 *
 * Maps each knowledge system to a ring configuration:
 *   - Segment count: the system's natural cycle length
 *   - Color: from color_theory.h system palettes
 *   - Today highlight: which segment is "today"
 *
 * Produces cr_layout_t ready for cr_generate().
 *
 * Ring layout uses depth_ring.h timescale ordering:
 *   Ring 0 (center): Cosmic systems (geological, cosmic)
 *   Ring 1: Age-scale systems (Buddhist, Hindu)
 *   Ring 2: Era-scale (Kabbalah, Human Design)
 *   Ring 3: Year-scale (Zodiac, Chinese, etc.)
 *   Ring 4: Cycle-scale (I Ching, Chinese 60-year)
 *   Ring 5 (outer): Today-scale (Gregorian, Tzolkin, Haab)
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_SYSTEM_RINGS_H
#define TIME_SYSTEM_RINGS_H

#include "concentric_ring.h"

#define SR_MAX_SYSTEMS 8  /* max systems representable as rings */

/* System ring definition */
typedef struct {
    int system_id;          /* ts_system_t value */
    int segment_count;      /* natural cycle length */
    int today_index;        /* which segment is active today (-1 = unknown) */
    float color[3];         /* primary system color (RGB, normalized) */
    const char *label;      /* system name for display */
} sr_system_def_t;

/* Get the natural cycle length (segment count) for a system.
 * Returns: 12 (zodiac), 260 (tzolkin), 64 (iching), 60 (chinese), etc.
 * Unknown systems return 12 as default. */
int sr_cycle_length(int system_id);

/* Get system ring definition (static data, no "today" computation).
 * system_id: ts_system_t value.
 * Returns definition with segment_count and color, today_index = -1. */
sr_system_def_t sr_system_def(int system_id);

/* Build a complete concentric ring layout for a set of systems.
 * system_ids: array of ts_system_t values to display.
 * today_indices: per-system "today" index (-1 if unknown), or NULL.
 * count: number of systems (clamped to SR_MAX_SYSTEMS).
 * innermost_radius: inner edge of first ring.
 * ring_gap: gap between rings.
 * Returns cr_layout_t ready for cr_generate(). */
cr_layout_t sr_build_layout(const int *system_ids, const int *today_indices,
                             int count, float innermost_radius, float ring_gap);

/* Build the standard 6-ring depth layout (Cosmic -> Today).
 * Uses one representative system per depth ring.
 * today_indices: 6 "today" values (or NULL for no highlighting).
 * Returns cr_layout_t with 6 rings. */
cr_layout_t sr_depth_layout(const int *today_indices,
                             float innermost_radius, float ring_gap);

/* Get the default representative system for each depth ring.
 * ring: depth_ring_t value (0-5).
 * Returns ts_system_t value of the representative system. */
int sr_depth_representative(int ring);

/* Total number of known system cycle lengths. */
int sr_known_system_count(void);

#endif /* TIME_SYSTEM_RINGS_H */
