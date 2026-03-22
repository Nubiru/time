/* daily_lao_layout.h — Daily Lao Buddhist calendar card layout
 *
 * Computes a card layout showing today's Lao date with Buddhist
 * Era year, month, festival, and zodiac animal.
 * All positions in normalized 0-1 coordinate space.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_DAILY_LAO_LAYOUT_H
#define TIME_DAILY_LAO_LAYOUT_H

#include "../systems/lao/lao_calendar.h"

/* Slot rectangle for positioning text elements. */
typedef struct {
    float x, y;
    float w, h;
} lao_slot_t;

typedef struct {
    /* Date */
    lao_date_t date;                /* be_year, month, day */
    const char *month_name;         /* Lao month name */
    const char *day_name;           /* Lao day name */

    /* Festival */
    int festival;                   /* lao_festival_t cast to int */
    const char *festival_name;      /* festival name or "" */

    /* Zodiac */
    const char *zodiac_animal;      /* 12-year cycle animal */

    /* Interpretation */
    char glance[128];               /* T1 one-liner */
    char detail[512];               /* T3 narrative */

    /* Layout slots */
    lao_slot_t title_slot;          /* "Lao Calendar" */
    lao_slot_t date_slot;           /* full date display */
    lao_slot_t month_slot;          /* month name */
    lao_slot_t festival_slot;       /* festival (if active) */
    lao_slot_t zodiac_slot;         /* zodiac animal */
    lao_slot_t day_name_slot;       /* day of week name */

    /* Card frame */
    float card_x, card_y, card_w, card_h;
} daily_lao_layout_t;

/* Compute the daily Lao card layout.
 * jd: Julian Day number. */
daily_lao_layout_t daily_lao_compute(double jd);

/* Check if a festival is active. Returns 1 if yes, 0 if none.
 * Returns 0 for NULL layout. */
int daily_lao_has_festival(const daily_lao_layout_t *layout);

#endif /* TIME_DAILY_LAO_LAYOUT_H */
