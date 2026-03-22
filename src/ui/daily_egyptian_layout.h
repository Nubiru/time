/* daily_egyptian_layout.h — Daily Egyptian calendar card layout
 *
 * Computes a card layout showing today's Egyptian civil date
 * with season, month, Sothic cycle position, and epagomenal status.
 * All positions in normalized 0-1 coordinate space.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_DAILY_EGYPTIAN_LAYOUT_H
#define TIME_DAILY_EGYPTIAN_LAYOUT_H

#include "../systems/egyptian/egyptian.h"

/* Slot rectangle for positioning text elements. */
typedef struct {
    float x, y;
    float w, h;
} egypt_slot_t;

typedef struct {
    /* Date */
    egypt_date_t date;              /* year, month (1-13), day */
    const char *month_name;         /* "Thoth", etc. */
    int is_epagomenal;              /* 1 if in epagomenal period */

    /* Season */
    egypt_season_t season;          /* Akhet/Peret/Shemu/Epag */
    const char *season_name;        /* "Akhet", etc. */

    /* Sothic cycle */
    egypt_sothic_t sothic;          /* cycle year, progress, drift */

    /* Interpretation */
    char glance[128];               /* T1 one-liner */
    char detail[512];               /* T3 narrative */

    /* Layout slots */
    egypt_slot_t title_slot;        /* "Egyptian Calendar" */
    egypt_slot_t date_slot;         /* full date display */
    egypt_slot_t month_slot;        /* month name */
    egypt_slot_t season_slot;       /* season info */
    egypt_slot_t sothic_slot;       /* Sothic cycle position */
    egypt_slot_t epag_slot;         /* epagomenal status */

    /* Card frame */
    float card_x, card_y, card_w, card_h;
} daily_egyptian_layout_t;

/* Compute the daily Egyptian card layout.
 * jd: Julian Day number. */
daily_egyptian_layout_t daily_egyptian_compute(double jd);

/* Check if the date is epagomenal. Returns 1 if yes, 0 if no.
 * Returns 0 for NULL layout. */
int daily_egyptian_is_epagomenal(const daily_egyptian_layout_t *layout);

#endif /* TIME_DAILY_EGYPTIAN_LAYOUT_H */
