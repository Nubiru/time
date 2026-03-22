/* daily_french_republican_layout.h — Daily French Republican card layout
 *
 * Computes a card layout showing today's French Republican date
 * with month name, decade day, and Sansculottide status.
 * All positions in normalized 0-1 coordinate space.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_DAILY_FRENCH_REPUBLICAN_LAYOUT_H
#define TIME_DAILY_FRENCH_REPUBLICAN_LAYOUT_H

#include "../systems/french_republican/french_republican.h"

/* Slot rectangle for positioning text elements. */
typedef struct {
    float x, y;
    float w, h;
} fr_slot_t;

typedef struct {
    /* Date */
    fr_date_t date;                 /* year, month (0-12), day */
    const char *month_name;         /* "Vendemiaire", etc. */
    const char *day_name;           /* decade day name or Sansculottide name */
    int is_sansculottide;           /* 1 if in Sansculottides */

    /* Interpretation */
    char glance[128];               /* T1 one-liner */
    char detail[512];               /* T3 narrative */

    /* Layout slots */
    fr_slot_t title_slot;           /* "French Republican Calendar" */
    fr_slot_t date_slot;            /* full date display */
    fr_slot_t month_slot;           /* month name */
    fr_slot_t day_name_slot;        /* decade day or Sansculottide */
    fr_slot_t season_slot;          /* seasonal quality */
    fr_slot_t info_slot;            /* additional info */

    /* Card frame */
    float card_x, card_y, card_w, card_h;
} daily_french_republican_layout_t;

/* Compute the daily French Republican card layout.
 * jd: Julian Day number. */
daily_french_republican_layout_t daily_french_republican_compute(double jd);

/* Check if the date is a Sansculottide. Returns 1 if yes, 0 if no.
 * Returns 0 for NULL layout. */
int daily_french_republican_is_sansculottide(
    const daily_french_republican_layout_t *layout);

#endif /* TIME_DAILY_FRENCH_REPUBLICAN_LAYOUT_H */
