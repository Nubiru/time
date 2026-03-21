/* depth_ring.c — Concentric timescale ring assignment for knowledge systems
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 *
 * Sources: Knowledge system timescales, mandala visual design. */

#include "depth_ring.h"

/* Static mapping: ts_system_t (0-20) -> depth_ring_t
 *
 * RING_COSMIC (0): TS_SYS_GEOLOGICAL, TS_SYS_COSMIC
 * RING_AGE    (1): TS_SYS_BUDDHIST, TS_SYS_HINDU
 * RING_ERA    (2): TS_SYS_KABBALAH, TS_SYS_HUMAN_DESIGN
 * RING_YEAR   (3): TS_SYS_ASTROLOGY, TS_SYS_HEBREW, TS_SYS_ISLAMIC,
 *                  TS_SYS_COPTIC, TS_SYS_ETHIOPIAN, TS_SYS_PERSIAN,
 *                  TS_SYS_JAPANESE, TS_SYS_KOREAN, TS_SYS_THAI
 * RING_CYCLE  (4): TS_SYS_CHINESE, TS_SYS_ICHING
 * RING_TODAY  (5): TS_SYS_GREGORIAN, TS_SYS_TZOLKIN, TS_SYS_HAAB
 */
static const depth_ring_t s_ring_map[TS_SYS_COUNT] = {
    /* [TS_SYS_GREGORIAN    =  0] */ DEPTH_RING_TODAY,
    /* [TS_SYS_TZOLKIN      =  1] */ DEPTH_RING_TODAY,
    /* [TS_SYS_HAAB         =  2] */ DEPTH_RING_TODAY,
    /* [TS_SYS_CHINESE      =  3] */ DEPTH_RING_CYCLE,
    /* [TS_SYS_HEBREW       =  4] */ DEPTH_RING_YEAR,
    /* [TS_SYS_ISLAMIC      =  5] */ DEPTH_RING_YEAR,
    /* [TS_SYS_BUDDHIST     =  6] */ DEPTH_RING_AGE,
    /* [TS_SYS_HINDU        =  7] */ DEPTH_RING_AGE,
    /* [TS_SYS_ICHING       =  8] */ DEPTH_RING_CYCLE,
    /* [TS_SYS_ASTROLOGY    =  9] */ DEPTH_RING_YEAR,
    /* [TS_SYS_HUMAN_DESIGN = 10] */ DEPTH_RING_ERA,
    /* [TS_SYS_KABBALAH     = 11] */ DEPTH_RING_ERA,
    /* [TS_SYS_COPTIC       = 12] */ DEPTH_RING_YEAR,
    /* [TS_SYS_ETHIOPIAN    = 13] */ DEPTH_RING_YEAR,
    /* [TS_SYS_PERSIAN      = 14] */ DEPTH_RING_YEAR,
    /* [TS_SYS_JAPANESE     = 15] */ DEPTH_RING_YEAR,
    /* [TS_SYS_KOREAN       = 16] */ DEPTH_RING_YEAR,
    /* [TS_SYS_THAI         = 17] */ DEPTH_RING_YEAR,
    /* [TS_SYS_GEOLOGICAL   = 18] */ DEPTH_RING_COSMIC,
    /* [TS_SYS_COSMIC       = 19] */ DEPTH_RING_COSMIC,
    /* [TS_SYS_EARTH        = 20] */ DEPTH_RING_TODAY
};

depth_ring_t depth_ring_for_system(int system_id)
{
    if (system_id < 0 || system_id >= TS_SYS_COUNT) {
        return DEPTH_RING_TODAY;
    }
    return s_ring_map[system_id];
}

int depth_ring_systems(depth_ring_t ring, int *out_ids, int max_out)
{
    if (ring < DEPTH_RING_COSMIC || ring >= DEPTH_RING_COUNT) {
        return 0;
    }

    int count = 0;
    for (int s = 0; s < TS_SYS_COUNT; s++) {
        if (s_ring_map[s] == ring) {
            if (out_ids && count < max_out) {
                out_ids[count] = s;
            }
            count++;
        }
    }
    return count;
}

const char *depth_ring_name(depth_ring_t ring)
{
    switch (ring) {
    case DEPTH_RING_COSMIC: return "Cosmic";
    case DEPTH_RING_AGE:    return "Age";
    case DEPTH_RING_ERA:    return "Era";
    case DEPTH_RING_YEAR:   return "Year";
    case DEPTH_RING_CYCLE:  return "Cycle";
    case DEPTH_RING_TODAY:  return "Today";
    default:                return "?";
    }
}

int depth_ring_count(void)
{
    return DEPTH_RING_COUNT;
}

const char *depth_ring_timescale(depth_ring_t ring)
{
    switch (ring) {
    case DEPTH_RING_COSMIC: return "Billions of years";
    case DEPTH_RING_AGE:    return "Hundreds of thousands to millions of years";
    case DEPTH_RING_ERA:    return "Decades to centuries";
    case DEPTH_RING_YEAR:   return "Annual cycles";
    case DEPTH_RING_CYCLE:  return "Monthly and weekly cycles";
    case DEPTH_RING_TODAY:  return "Today";
    default:                return "?";
    }
}
