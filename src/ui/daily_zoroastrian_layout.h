/* daily_zoroastrian_layout.h — Daily Zoroastrian calendar card layout
 *
 * Computes a card layout showing today's Yazdgerdi date with month
 * and day names, active festival, and Gatha epagomenal status.
 * All positions in normalized 0-1 coordinate space.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_DAILY_ZOROASTRIAN_LAYOUT_H
#define TIME_DAILY_ZOROASTRIAN_LAYOUT_H

#include "../systems/zoroastrian/zoroastrian.h"

/* Slot rectangle for positioning text elements. */
typedef struct {
    float x, y;
    float w, h;
} zoro_slot_t;

typedef struct {
    /* Date */
    zoro_date_t date;               /* year, month (1-13), day */
    const char *month_name;         /* "Farvardin", etc. */
    const char *day_name;           /* Avestan day name (1-30) */
    int is_gatha;                   /* 1 if in Gatha epagomenal period */

    /* Festival */
    int festival;                   /* zoro_festival_t cast to int */
    const char *festival_name;      /* festival name or "" */

    /* Interpretation */
    char glance[128];               /* T1 one-liner */
    char detail[512];               /* T3 narrative */

    /* Layout slots */
    zoro_slot_t title_slot;         /* "Zoroastrian Calendar" */
    zoro_slot_t date_slot;          /* full date display */
    zoro_slot_t month_slot;         /* month name */
    zoro_slot_t day_name_slot;      /* Avestan day name */
    zoro_slot_t festival_slot;      /* festival (if active) */
    zoro_slot_t gatha_slot;         /* Gatha status */

    /* Card frame */
    float card_x, card_y, card_w, card_h;
} daily_zoroastrian_layout_t;

/* Compute the daily Zoroastrian card layout.
 * jd: Julian Day number. */
daily_zoroastrian_layout_t daily_zoroastrian_compute(double jd);

/* Check if a festival is active. Returns 1 if yes, 0 if none.
 * Returns 0 for NULL layout. */
int daily_zoroastrian_has_festival(const daily_zoroastrian_layout_t *layout);

#endif /* TIME_DAILY_ZOROASTRIAN_LAYOUT_H */
