/* daily_hindu_layout.h — Daily Hindu Panchanga card layout
 *
 * Computes a card layout showing the five limbs of the Panchanga:
 * Tithi, Vara, Nakshatra, Yoga, and Karana.
 * All positions in normalized 0-1 coordinate space.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_DAILY_HINDU_LAYOUT_H
#define TIME_DAILY_HINDU_LAYOUT_H

#include "../systems/hindu/panchanga.h"
#include "../systems/hindu/nakshatra.h"

/* Slot rectangle for positioning text elements. */
typedef struct {
    float x, y;
    float w, h;
} hin_slot_t;

typedef struct {
    /* Panchanga 5 limbs */
    int tithi_number;               /* 1-30 */
    const char *tithi_name;         /* Sanskrit tithi name */
    const char *paksha;             /* "Shukla" or "Krishna" */

    int vara_number;                /* 0-6 (Sun=0..Sat=6) */
    const char *vara_name;          /* Sanskrit day name */
    const char *vara_deity;         /* presiding deity */

    int nakshatra_index;            /* 0-26 */

    int yoga_number;                /* 0-26 */
    const char *yoga_name;          /* yoga name */
    const char *yoga_quality;       /* "Auspicious"/"Inauspicious"/"Neutral" */

    int karana_number;              /* 0-10 */
    const char *karana_name;        /* karana name */

    /* Layout slots */
    hin_slot_t title_slot;          /* "Hindu Panchanga" */
    hin_slot_t tithi_slot;          /* tithi + paksha */
    hin_slot_t vara_slot;           /* weekday + deity */
    hin_slot_t nakshatra_slot;      /* lunar mansion */
    hin_slot_t yoga_slot;           /* yoga + quality */
    hin_slot_t karana_slot;         /* half-tithi */

    /* Card frame */
    float card_x, card_y, card_w, card_h;
} daily_hindu_layout_t;

/* Compute the daily Hindu Panchanga card layout.
 * jd: Julian Day number.
 * sun_lon: tropical ecliptic longitude of Sun in degrees.
 * moon_lon: tropical ecliptic longitude of Moon in degrees. */
daily_hindu_layout_t daily_hindu_compute(double jd, double sun_lon,
                                          double moon_lon);

/* Get the tithi name string. Returns "?" if NULL layout. */
const char *daily_hindu_tithi_name(const daily_hindu_layout_t *layout);

#endif /* TIME_DAILY_HINDU_LAYOUT_H */
