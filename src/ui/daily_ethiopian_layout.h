/* daily_ethiopian_layout.h — Daily Ethiopian calendar card layout
 *
 * Computes a card layout showing today's Ethiopian date with
 * Ge'ez month meaning, seasonal quality, and active feast.
 * All positions in normalized 0-1 coordinate space.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_DAILY_ETHIOPIAN_LAYOUT_H
#define TIME_DAILY_ETHIOPIAN_LAYOUT_H

#include "../systems/ethiopian/ethiopian.h"

/* Slot rectangle for positioning text elements. */
typedef struct {
    float x, y;
    float w, h;
} ethiopian_slot_t;

typedef struct {
    /* Date */
    ethiopian_date_t date;        /* year, month (1-13), day */
    const char *month_name;       /* Ge'ez name */
    int is_leap;                  /* 1 if leap year */

    /* Month interpretation */
    const char *meaning;          /* What the month name signifies */
    const char *season;           /* Ethiopian seasonal quality */
    const char *brief;            /* One-sentence meaning */

    /* Feast (if active) */
    const char *feast_name;       /* Feast name or "" if none */
    int has_feast;                /* 1 if a feast is active today */

    /* Layout slots */
    ethiopian_slot_t title_slot;  /* "Ethiopian Calendar" header */
    ethiopian_slot_t date_slot;   /* full date display */
    ethiopian_slot_t month_slot;  /* month name + meaning */
    ethiopian_slot_t season_slot; /* seasonal quality */
    ethiopian_slot_t feast_slot;  /* feast (if active) */
    ethiopian_slot_t brief_slot;  /* brief meaning */

    /* Card frame */
    float card_x, card_y, card_w, card_h;
} daily_ethiopian_layout_t;

/* Compute the daily Ethiopian card layout.
 * jd: Julian Day number. */
daily_ethiopian_layout_t daily_ethiopian_compute(double jd);

/* Get the month meaning string. Returns "?" if NULL layout. */
const char *daily_ethiopian_month_meaning(const daily_ethiopian_layout_t *layout);

/* Check if the given date falls on an Ethiopian feast.
 * Returns 1 if feast active, 0 otherwise. Returns 0 for NULL layout. */
int daily_ethiopian_has_feast(const daily_ethiopian_layout_t *layout);

#endif /* TIME_DAILY_ETHIOPIAN_LAYOUT_H */
