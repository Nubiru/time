/* daily_tamil_layout.h — Daily Tamil calendar card layout
 *
 * Computes a card layout showing today's Tamil sidereal solar
 * date with month, Jovian cycle year, and festival status.
 * All positions in normalized 0-1 coordinate space.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_DAILY_TAMIL_LAYOUT_H
#define TIME_DAILY_TAMIL_LAYOUT_H

#include "../systems/tamil/tamil_calendar.h"

/* Slot rectangle for positioning text elements. */
typedef struct {
    float x, y;
    float w, h;
} tamil_slot_t;

typedef struct {
    /* Date */
    tamil_date_t date;              /* year (TV), month (1-12), day, jovian */
    const char *month_name;         /* "Chittirai", etc. */
    const char *jovian_name;        /* 60-year cycle name */

    /* Festival */
    int festival;                   /* tamil_festival_t cast to int */
    const char *festival_name;      /* festival name or "" */

    /* Interpretation */
    char glance[128];               /* T1 one-liner */
    char detail[512];               /* T3 narrative */

    /* Layout slots */
    tamil_slot_t title_slot;        /* "Tamil Calendar" */
    tamil_slot_t date_slot;         /* full date display */
    tamil_slot_t month_slot;        /* month name + rasi */
    tamil_slot_t jovian_slot;       /* Jovian cycle year */
    tamil_slot_t festival_slot;     /* festival (if active) */
    tamil_slot_t info_slot;         /* additional info */

    /* Card frame */
    float card_x, card_y, card_w, card_h;
} daily_tamil_layout_t;

/* Compute the daily Tamil card layout.
 * jd: Julian Day number. */
daily_tamil_layout_t daily_tamil_compute(double jd);

/* Check if a festival is active. Returns 1 if yes, 0 if none.
 * Returns 0 for NULL layout. */
int daily_tamil_has_festival(const daily_tamil_layout_t *layout);

#endif /* TIME_DAILY_TAMIL_LAYOUT_H */
