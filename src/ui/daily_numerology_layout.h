/* daily_numerology_layout.h — Daily numerology card layout
 *
 * Computes a card layout showing today's universal day number,
 * master number status, universal year, and interpretation.
 * All positions in normalized 0-1 coordinate space.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_DAILY_NUMEROLOGY_LAYOUT_H
#define TIME_DAILY_NUMEROLOGY_LAYOUT_H

/* Slot rectangle for positioning text elements. */
typedef struct {
    float x, y;
    float w, h;
} num_slot_t;

typedef struct {
    /* Numerology data */
    int daily_number;               /* reduced root: 1-9 or 11/22/33 */
    int is_master;                  /* 1 if master number */
    int universal_year;             /* reduced year root */
    int year;                       /* Gregorian year */
    int month;                      /* Gregorian month (1-12) */
    int day;                        /* Gregorian day (1-31) */
    const char *number_meaning;     /* keyword for daily number */

    /* Interpretation */
    char glance[128];               /* T1 one-liner */
    char detail[512];               /* T3 narrative */

    /* Layout slots */
    num_slot_t title_slot;          /* "Numerology" */
    num_slot_t number_slot;         /* daily number display */
    num_slot_t meaning_slot;        /* number meaning */
    num_slot_t universal_slot;      /* universal year */
    num_slot_t year_energy_slot;    /* year energy */
    num_slot_t detail_slot;         /* interpretation detail */

    /* Card frame */
    float card_x, card_y, card_w, card_h;
} daily_numerology_layout_t;

/* Compute the daily numerology card layout.
 * jd: Julian Day number. */
daily_numerology_layout_t daily_numerology_compute(double jd);

#endif /* TIME_DAILY_NUMEROLOGY_LAYOUT_H */
