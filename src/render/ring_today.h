/* ring_today.h — Today's active segment index per knowledge system.
 *
 * Computes which ring segment is "active today" for each of the
 * TS_SYS_COUNT knowledge systems, given a Julian Day and planetary
 * longitudes. Feeds into concentric ring visualization.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_RING_TODAY_H
#define TIME_RING_TODAY_H

#include "../systems/unified/today_summary.h"  /* ts_system_t, TS_SYS_COUNT */

#define RT_MAX_SYSTEMS TS_SYS_COUNT

typedef struct {
    int indices[RT_MAX_SYSTEMS];  /* today index per ts_system_t (-1 = unknown) */
} ring_today_t;

/* Compute today indices for all systems.
 * jd: Julian Day number
 * sun_lon_deg: Sun ecliptic longitude in degrees (for zodiac, HD)
 * moon_lon_deg: Moon ecliptic longitude in degrees (for nakshatra) */
ring_today_t ring_today_compute(double jd, double sun_lon_deg,
                                 double moon_lon_deg);

/* Get today index for a specific system. Returns -1 if unknown. */
int ring_today_index(const ring_today_t *rt, int system_id);

/* Build a today_indices array for sr_depth_layout().
 * Extracts the 6 depth-representative system indices using sr_depth_representative().
 * out: array of 6 ints. */
void ring_today_for_depth(const ring_today_t *rt, int out[6]);

#endif /* TIME_RING_TODAY_H */
