/* daily_astronomy_layout.h — Daily astronomy card layout
 *
 * Computes a card layout showing today's lunar phase, illumination,
 * age, zodiac sign, and astronomical interpretation.
 * All positions in normalized 0-1 coordinate space.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_DAILY_ASTRONOMY_LAYOUT_H
#define TIME_DAILY_ASTRONOMY_LAYOUT_H

#include "../systems/astronomy/lunar.h"
#include "../systems/astronomy/astronomy_interpret.h"

/* Slot rectangle for positioning text elements. */
typedef struct {
    float x, y;
    float w, h;
} astro_slot_t;

typedef struct {
    /* Moon data */
    int moon_phase;                 /* 0-7 (moon_phase_t) */
    const char *moon_phase_name;    /* "New Moon", etc. */
    double illumination;            /* 0.0-1.0 */
    double moon_age_days;           /* days since last new moon */
    double moon_longitude;          /* ecliptic longitude, degrees */
    int zodiac_sign;                /* 0-11 from lunar_zodiac_sign */

    /* Interpretation */
    char glance[128];               /* T1 one-liner */
    char detail[512];               /* T3 narrative */

    /* Layout slots */
    astro_slot_t title_slot;        /* "Astronomy" */
    astro_slot_t phase_slot;        /* moon phase name */
    astro_slot_t illumination_slot; /* illumination percentage */
    astro_slot_t age_slot;          /* moon age in days */
    astro_slot_t zodiac_slot;       /* zodiac sign */
    astro_slot_t detail_slot;       /* interpretation detail */

    /* Card frame */
    float card_x, card_y, card_w, card_h;
} daily_astronomy_layout_t;

/* Compute the daily astronomy card layout.
 * jd: Julian Day number. */
daily_astronomy_layout_t daily_astronomy_compute(double jd);

#endif /* TIME_DAILY_ASTRONOMY_LAYOUT_H */
