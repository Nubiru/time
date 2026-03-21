/* daily_persian_layout.h — Daily Persian/Solar Hijri calendar card layout
 *
 * Computes a card layout showing today's Persian date with Avestan
 * month origin, meaning, seasonal quality, and active festival.
 * All positions in normalized 0-1 coordinate space.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_DAILY_PERSIAN_LAYOUT_H
#define TIME_DAILY_PERSIAN_LAYOUT_H

#include "../systems/persian/persian.h"

/* Slot rectangle for positioning text elements. */
typedef struct {
    float x, y;
    float w, h;
} persian_slot_t;

typedef struct {
    /* Date */
    persian_date_t date;          /* year (SH), month (1-12), day */
    const char *month_name;       /* "Farvardin", etc. */
    const char *weekday_name;     /* "Shanbe", etc. */

    /* Month interpretation */
    const char *avestan;          /* Original Avestan deity/concept */
    const char *meaning;          /* What the name means */
    const char *season;           /* Seasonal quality */
    const char *brief;            /* One-sentence meaning */

    /* Festival (if active) */
    const char *festival_name;    /* Festival name or "" if none */
    int has_festival;             /* 1 if a festival is active today */

    /* Layout slots */
    persian_slot_t title_slot;    /* "Persian Calendar" header */
    persian_slot_t date_slot;     /* full date display */
    persian_slot_t month_slot;    /* month name + avestan origin */
    persian_slot_t meaning_slot;  /* meaning */
    persian_slot_t season_slot;   /* seasonal quality */
    persian_slot_t festival_slot; /* festival (if active) */
    persian_slot_t brief_slot;    /* brief meaning */

    /* Card frame */
    float card_x, card_y, card_w, card_h;
} daily_persian_layout_t;

/* Compute the daily Persian card layout.
 * jd: Julian Day number. */
daily_persian_layout_t daily_persian_compute(double jd);

/* Get the Avestan origin string. Returns "?" if NULL layout. */
const char *daily_persian_avestan(const daily_persian_layout_t *layout);

/* Check if the given date falls on a Persian festival.
 * Returns 1 if festival active, 0 otherwise. Returns 0 for NULL layout. */
int daily_persian_has_festival(const daily_persian_layout_t *layout);

#endif /* TIME_DAILY_PERSIAN_LAYOUT_H */
