/* daily_myanmar_layout.h — Daily Myanmar calendar card layout
 *
 * Computes a card layout showing today's Myanmar (Burmese)
 * lunisolar date with month, year type, and Thingyan status.
 * All positions in normalized 0-1 coordinate space.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_DAILY_MYANMAR_LAYOUT_H
#define TIME_DAILY_MYANMAR_LAYOUT_H

#include "../systems/myanmar/myanmar.h"

/* Slot rectangle for positioning text elements. */
typedef struct {
    float x, y;
    float w, h;
} my_slot_t;

typedef struct {
    /* Date */
    myanmar_date_t date;            /* year (ME), month, day */
    const char *month_name;         /* Myanmar month name */
    int year_type;                  /* myanmar_year_type_t cast to int */
    const char *year_type_name;     /* "Common", "Small Watat", etc. */

    /* Thingyan */
    int is_thingyan;                /* 1 if during Thingyan (water festival) */

    /* Interpretation */
    char glance[128];               /* T1 one-liner */
    char detail[512];               /* T3 narrative */

    /* Layout slots */
    my_slot_t title_slot;           /* "Myanmar Calendar" */
    my_slot_t date_slot;            /* full date display */
    my_slot_t month_slot;           /* month name */
    my_slot_t year_type_slot;       /* year type info */
    my_slot_t thingyan_slot;        /* Thingyan status */
    my_slot_t info_slot;            /* additional info */

    /* Card frame */
    float card_x, card_y, card_w, card_h;
} daily_myanmar_layout_t;

/* Compute the daily Myanmar card layout.
 * jd: Julian Day number. */
daily_myanmar_layout_t daily_myanmar_compute(double jd);

/* Check if Thingyan is active. Returns 1 if yes, 0 if no.
 * Returns 0 for NULL layout. */
int daily_myanmar_is_thingyan(const daily_myanmar_layout_t *layout);

#endif /* TIME_DAILY_MYANMAR_LAYOUT_H */
