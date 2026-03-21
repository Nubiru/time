/* daily_hebrew_layout.h — Daily Hebrew calendar card layout
 *
 * Computes a card layout showing today's Hebrew calendar date with
 * month meaning, governing letter, tribe, and spiritual quality.
 * All positions in normalized 0-1 coordinate space.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_DAILY_HEBREW_LAYOUT_H
#define TIME_DAILY_HEBREW_LAYOUT_H

#include "../systems/hebrew/hebrew.h"

/* Slot rectangle for positioning text elements. */
typedef struct {
    float x, y;
    float w, h;
} hebrew_slot_t;

typedef struct {
    /* Date */
    hebrew_date_t date;           /* year, month, day */
    const char *month_name;       /* "Tevet", "Nisan", etc. */
    int is_leap;                  /* 1 if leap year */

    /* Month interpretation */
    const char *hebrew_letter;    /* governing letter for the month */
    const char *tribe;            /* associated tribe */
    const char *sense;            /* associated sense/faculty */
    const char *quality;          /* spiritual quality */
    const char *brief;            /* one-sentence meaning */

    /* Layout slots */
    hebrew_slot_t title_slot;     /* "Hebrew Calendar" header */
    hebrew_slot_t date_slot;      /* full date display */
    hebrew_slot_t month_slot;     /* month name + meaning */
    hebrew_slot_t letter_slot;    /* governing letter */
    hebrew_slot_t tribe_slot;     /* tribe of Israel */
    hebrew_slot_t quality_slot;   /* spiritual quality */
    hebrew_slot_t brief_slot;     /* brief meaning */

    /* Card frame */
    float card_x, card_y, card_w, card_h;
} daily_hebrew_layout_t;

/* Compute the daily Hebrew card layout.
 * jd: Julian Day number. */
daily_hebrew_layout_t daily_hebrew_compute(double jd);

/* Get the spiritual quality string. Returns "?" if NULL layout. */
const char *daily_hebrew_month_quality(const daily_hebrew_layout_t *layout);

/* Check if the given JD falls on Shabbat (Saturday).
 * Returns 1 if Saturday, 0 otherwise. Returns 0 for NULL layout. */
int daily_hebrew_is_shabbat(const daily_hebrew_layout_t *layout);

#endif /* TIME_DAILY_HEBREW_LAYOUT_H */
