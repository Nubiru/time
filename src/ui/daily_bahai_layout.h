/* daily_bahai_layout.h — Daily Baha'i (Badi') calendar card layout
 *
 * Computes a card layout showing today's Baha'i date with month
 * name, holy day status, Vahid/Kull-i-Shay cycles, and Ayyam-i-Ha.
 * All positions in normalized 0-1 coordinate space.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_DAILY_BAHAI_LAYOUT_H
#define TIME_DAILY_BAHAI_LAYOUT_H

#include "../systems/bahai/bahai.h"

/* Slot rectangle for positioning text elements. */
typedef struct {
    float x, y;
    float w, h;
} bahai_slot_t;

typedef struct {
    /* Date */
    bahai_date_t date;              /* year, month (0-19), day */
    const char *month_name;         /* "Baha", "Jalal", etc. */
    int is_ayyam_i_ha;             /* 1 if in Ayyam-i-Ha */

    /* Holy day */
    int holy_day;                   /* bahai_holy_day_t cast to int */
    const char *holy_day_name;      /* holy day name or "" */

    /* Cycles */
    int vahid;                      /* Vahid number (19-year cycle) */
    int kull_i_shay;                /* Kull-i-Shay' number (361-year) */

    /* Interpretation */
    char glance[128];               /* T1 one-liner */
    char detail[512];               /* T3 narrative */

    /* Layout slots */
    bahai_slot_t title_slot;        /* "Baha'i Calendar" */
    bahai_slot_t date_slot;         /* full date display */
    bahai_slot_t month_slot;        /* month name + meaning */
    bahai_slot_t holy_day_slot;     /* holy day (if active) */
    bahai_slot_t cycle_slot;        /* Vahid / Kull-i-Shay' */
    bahai_slot_t ayyam_slot;        /* Ayyam-i-Ha status */

    /* Card frame */
    float card_x, card_y, card_w, card_h;
} daily_bahai_layout_t;

/* Compute the daily Baha'i card layout.
 * jd: Julian Day number. */
daily_bahai_layout_t daily_bahai_compute(double jd);

/* Check if a holy day is active. Returns 1 if yes, 0 if none.
 * Returns 0 for NULL layout. */
int daily_bahai_has_holy_day(const daily_bahai_layout_t *layout);

#endif /* TIME_DAILY_BAHAI_LAYOUT_H */
