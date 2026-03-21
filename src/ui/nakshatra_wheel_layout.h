/* nakshatra_wheel_layout.h — 27 Nakshatra wheel layout
 *
 * 27 Hindu Nakshatras arranged in a circle, with the current Moon's
 * Nakshatra highlighted. Each shows name, deity, quality, and symbol.
 * All positions in normalized 0-1 coordinate space.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_NAKSHATRA_WHEEL_LAYOUT_H
#define TIME_NAKSHATRA_WHEEL_LAYOUT_H

#include "../systems/hindu/nakshatra.h"

typedef struct {
    int id;                /* 0-26 */
    const char *name;      /* Sanskrit name */
    const char *deity;     /* presiding deity */
    const char *quality;   /* Deva/Manushya/Rakshasa */
    const char *symbol;    /* traditional symbol */
    float x, y;            /* position on circle */
    float angle_deg;       /* angle from top (0=Ashwini at top) */
    float arc_start_deg;   /* start of this nakshatra's arc */
    float arc_end_deg;     /* end of arc */
    int highlighted;       /* 1 if Moon is in this nakshatra */
} nw_nakshatra_t;

#define NW_NAKSHATRA_COUNT 27

typedef struct {
    nw_nakshatra_t nakshatras[NW_NAKSHATRA_COUNT];
    float center_x, center_y;
    float radius;
    int moon_nakshatra;    /* index of highlighted nakshatra, or -1 */
} nakshatra_wheel_layout_t;

/* Compute Nakshatra wheel layout.
 * moon_tropical_lon: Moon's tropical ecliptic longitude in degrees.
 * jd: for computing ayanamsa. Pass 0.0 for no highlighting. */
nakshatra_wheel_layout_t nakshatra_wheel_compute(double moon_tropical_lon,
                                                  double jd);

/* Get nakshatra data at index (0-26).
 * Returns nakshatra with id=-1 for invalid index or NULL layout. */
nw_nakshatra_t nakshatra_wheel_at(const nakshatra_wheel_layout_t *layout,
                                   int index);

#endif /* TIME_NAKSHATRA_WHEEL_LAYOUT_H */
