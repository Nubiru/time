/* daily_gregorian_layout.h — Daily Gregorian calendar card layout
 *
 * Computes a card layout showing today's Gregorian date with month
 * origin/meaning, day's planetary ruler, season info, and day of
 * week etymology. All positions in normalized 0-1 coordinate space.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_DAILY_GREGORIAN_LAYOUT_H
#define TIME_DAILY_GREGORIAN_LAYOUT_H

#include "../systems/gregorian/gregorian.h"

/* Slot rectangle for positioning text elements. */
typedef struct {
    float x, y;
    float w, h;
} greg_slot_t;

typedef struct {
    /* Date */
    char date_str[20];          /* "2026-03-21" */
    int year, month, day;
    const char *day_name;       /* "Friday" */

    /* Month interpretation */
    const char *month_name;
    const char *month_origin;   /* etymology */
    const char *month_quality;  /* seasonal quality */

    /* Day interpretation */
    const char *day_planet;     /* ruling planet */
    const char *day_origin;     /* mythological origin */

    /* Season */
    int season;                 /* 0-3 */
    const char *season_name;
    const char *season_theme;

    /* Layout slots */
    greg_slot_t title_slot;     /* "Gregorian Calendar" */
    greg_slot_t date_slot;      /* full date */
    greg_slot_t day_slot;       /* day name + planet */
    greg_slot_t month_slot;     /* month name + origin */
    greg_slot_t season_slot;    /* season + theme */
    greg_slot_t quality_slot;   /* month quality */

    /* Card frame */
    float card_x, card_y, card_w, card_h;
} daily_gregorian_layout_t;

/* Compute the daily Gregorian card layout.
 * jd: Julian Day number. */
daily_gregorian_layout_t daily_gregorian_compute(double jd);

/* Get the ruling planet for the day. Returns "?" if NULL layout. */
const char *daily_gregorian_day_planet(const daily_gregorian_layout_t *layout);

#endif /* TIME_DAILY_GREGORIAN_LAYOUT_H */
