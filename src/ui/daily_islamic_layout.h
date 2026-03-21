/* daily_islamic_layout.h — Daily Islamic calendar card layout
 *
 * Computes a card layout showing today's Hijri date with
 * month significance, sacred month status, and events.
 * All positions in normalized 0-1 coordinate space.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_DAILY_ISLAMIC_LAYOUT_H
#define TIME_DAILY_ISLAMIC_LAYOUT_H

#include "../systems/islamic/hijri.h"
#include "../systems/islamic/islamic_interpret.h"

/* Slot rectangle for positioning text elements. */
typedef struct {
    float x, y;
    float w, h;
} isl_slot_t;

typedef struct {
    hijri_date_t date;              /* Hijri year, month, day */
    const char *month_name;         /* "Muharram", "Ramadan", etc. */
    int is_leap;                    /* 1 if leap year */
    int is_sacred_month;            /* 1 if one of 4 sacred months */
    const char *significance;       /* key significance of the month */
    const char *event;              /* major event or "" */
    const char *brief;              /* one-sentence meaning */

    /* Layout slots */
    isl_slot_t title_slot;          /* "Islamic Calendar" */
    isl_slot_t date_slot;           /* "day month year" */
    isl_slot_t month_slot;          /* month name */
    isl_slot_t sacred_slot;         /* sacred month indicator */
    isl_slot_t significance_slot;   /* significance text */
    isl_slot_t event_slot;          /* event if any */

    /* Card frame */
    float card_x, card_y, card_w, card_h;
} daily_islamic_layout_t;

/* Compute the daily Islamic card layout.
 * jd: Julian Day number. */
daily_islamic_layout_t daily_islamic_compute(double jd);

/* Check if current month is sacred. Returns 1 if sacred, 0 otherwise.
 * Returns 0 for NULL layout. */
int daily_islamic_is_sacred(const daily_islamic_layout_t *layout);

#endif /* TIME_DAILY_ISLAMIC_LAYOUT_H */
