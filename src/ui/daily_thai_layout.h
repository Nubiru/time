/* daily_thai_layout.h — Daily Thai calendar card layout
 *
 * Computes a card layout showing today's Thai date with Buddhist
 * Era year, Thai month name, and active festival (Songkran etc.).
 * All positions in normalized 0-1 coordinate space.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_DAILY_THAI_LAYOUT_H
#define TIME_DAILY_THAI_LAYOUT_H

#include "../systems/thai/thai_calendar.h"

/* Slot rectangle for positioning text elements. */
typedef struct {
    float x, y;
    float w, h;
} thai_slot_t;

typedef struct {
    /* Date */
    thai_date_t date;             /* be_year, month (1-12), day */
    const char *month_name;       /* Thai transliteration */
    const char *month_abbr;       /* Thai month abbreviation */
    int is_songkran;              /* 1 if Songkran (April 13-15) */

    /* Month interpretation */
    const char *origin;           /* Sanskrit origin */
    const char *season;           /* Thai seasonal quality */
    const char *brief;            /* One-sentence meaning */

    /* Festival (if active) */
    const char *festival_name;    /* Festival name or "" if none */
    int has_festival;             /* 1 if a festival is active today */

    /* Layout slots */
    thai_slot_t title_slot;       /* "Thai Calendar" header */
    thai_slot_t date_slot;        /* BE year + date */
    thai_slot_t month_slot;       /* month name + origin */
    thai_slot_t season_slot;      /* seasonal quality */
    thai_slot_t festival_slot;    /* festival (if active) */
    thai_slot_t brief_slot;       /* brief meaning */

    /* Card frame */
    float card_x, card_y, card_w, card_h;
} daily_thai_layout_t;

/* Compute the daily Thai card layout.
 * jd: Julian Day number. */
daily_thai_layout_t daily_thai_compute(double jd);

/* Get the month origin string. Returns "?" if NULL layout. */
const char *daily_thai_month_origin(const daily_thai_layout_t *layout);

/* Check if the given date falls on a Thai festival.
 * Returns 1 if festival active, 0 otherwise. Returns 0 for NULL layout. */
int daily_thai_has_festival(const daily_thai_layout_t *layout);

#endif /* TIME_DAILY_THAI_LAYOUT_H */
