/* depth_ring.h — Concentric timescale ring assignment for knowledge systems
 *
 * Assigns each knowledge system to a concentric ring (0-5) based on its
 * natural timescale. Ring 0 (center) = cosmic billions-of-years.
 * Ring 5 (outermost) = today's state. Used by the VISUALS stream for
 * mandala layout and by BRAIN for depth-level output.
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 *
 * Sources: Knowledge system timescales, mandala visual design. */

#ifndef TIME_DEPTH_RING_H
#define TIME_DEPTH_RING_H

#include "today_summary.h"  /* ts_system_t */

typedef enum {
    DEPTH_RING_COSMIC = 0,  /* center: billions of years (geological, cosmic, maha-kalpa) */
    DEPTH_RING_AGE    = 1,  /* hundred-thousands to millions (yuga, precession, sothic) */
    DEPTH_RING_ERA    = 2,  /* decades to centuries (calendar round, jubilee, sexagenary) */
    DEPTH_RING_YEAR   = 3,  /* annual cycles (zodiac, hebrew month, chinese animal, seasons) */
    DEPTH_RING_CYCLE  = 4,  /* monthly/weekly (lunar phase, uposatha, tithi, prayer times, wavespell) */
    DEPTH_RING_TODAY  = 5,  /* outermost: today's state in all systems */
    DEPTH_RING_COUNT  = 6
} depth_ring_t;

/* Primary ring for a knowledge system.
 * system_id: ts_system_t value (0-19).
 * Out of range: returns DEPTH_RING_TODAY as default. */
depth_ring_t depth_ring_for_system(int system_id);

/* List all systems in a given ring.
 * Fills out_ids with ts_system_t values. Returns total count in ring.
 * If ring is invalid, returns 0.
 * If out_ids is NULL, returns count without writing. */
int depth_ring_systems(depth_ring_t ring, int *out_ids, int max_out);

/* Human-readable ring name.
 * Returns: "Cosmic", "Age", "Era", "Year", "Cycle", "Today", or "?" for invalid. */
const char *depth_ring_name(depth_ring_t ring);

/* Number of ring levels (always DEPTH_RING_COUNT = 6). */
int depth_ring_count(void);

/* Human-readable timescale description for a ring.
 * Returns descriptive string or "?" for invalid. */
const char *depth_ring_timescale(depth_ring_t ring);

#endif /* TIME_DEPTH_RING_H */
