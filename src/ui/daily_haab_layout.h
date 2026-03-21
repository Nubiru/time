/* daily_haab_layout.h — Daily Haab solar calendar card layout
 *
 * Computes a card layout showing today's Haab date with vinal
 * name, wisdom meaning, Wayeb detection, and Calendar Round.
 * All positions in normalized 0-1 coordinate space.
 *
 * The Haab is the 365-day Mayan solar calendar: 18 vinales of
 * 20 days each + 5-day Wayeb closing period.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_DAILY_HAAB_LAYOUT_H
#define TIME_DAILY_HAAB_LAYOUT_H

#include "../systems/tzolkin/haab.h"

/* Slot rectangle for positioning text elements. */
typedef struct {
    float x, y;
    float w, h;
} haab_slot_t;

typedef struct {
    /* Date */
    haab_date_t date;             /* month (0-18), day (0-19/0-4) */
    const char *month_name;       /* "Pop", "Uo", ..., "Wayeb" */
    const char *month_meaning;    /* Month association */
    int day_of_year;              /* 0-364 position in Haab cycle */

    /* Vinal wisdom */
    int vinal_number;             /* 1-19 (19 = Uayeb) */
    const char *vinal_wisdom;     /* Wisdom text for this period */
    int is_wayeb;                 /* 1 if in 5-day Wayeb period */

    /* Calendar Round (Tzolkin + Haab) */
    calendar_round_t cal_round;   /* Combined Tzolkin-Haab date */
    char round_fmt[64];           /* Formatted: "4 Sun 8 Kumk'u" */

    /* Layout slots */
    haab_slot_t title_slot;       /* "Haab Calendar" header */
    haab_slot_t date_slot;        /* full date display */
    haab_slot_t meaning_slot;     /* month meaning */
    haab_slot_t wisdom_slot;      /* vinal wisdom text */
    haab_slot_t round_slot;       /* Calendar Round */
    haab_slot_t progress_slot;    /* day-of-year progress */

    /* Card frame */
    float card_x, card_y, card_w, card_h;
} daily_haab_layout_t;

/* Compute the daily Haab card layout.
 * jd: Julian Day number. */
daily_haab_layout_t daily_haab_compute(double jd);

/* Get the vinal wisdom string. Returns "?" if NULL layout. */
const char *daily_haab_wisdom(const daily_haab_layout_t *layout);

/* Check if the given date falls in Wayeb.
 * Returns 1 if Wayeb, 0 otherwise. Returns 0 for NULL layout. */
int daily_haab_is_wayeb(const daily_haab_layout_t *layout);

#endif /* TIME_DAILY_HAAB_LAYOUT_H */
