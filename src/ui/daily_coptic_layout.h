/* daily_coptic_layout.h — Daily Coptic calendar card layout
 *
 * Computes a card layout showing today's Coptic (Anno Martyrum) date
 * with month origin, liturgical significance, and active feast.
 * All positions in normalized 0-1 coordinate space.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_DAILY_COPTIC_LAYOUT_H
#define TIME_DAILY_COPTIC_LAYOUT_H

#include "../systems/coptic/coptic.h"

/* Slot rectangle for positioning text elements. */
typedef struct {
    float x, y;
    float w, h;
} coptic_slot_t;

typedef struct {
    /* Date */
    coptic_date_t date;           /* year (AM), month (1-13), day */
    const char *month_name;       /* "Thout", "Paopi", etc. */
    int is_leap;                  /* 1 if leap year */

    /* Month interpretation */
    const char *origin;           /* Ancient Egyptian / Coptic heritage */
    const char *liturgy;          /* Christian liturgical significance */
    const char *brief;            /* One-sentence meaning */

    /* Feast (if active) */
    const char *feast_name;       /* Feast name or "" if none */
    int has_feast;                /* 1 if a feast is active today */

    /* Layout slots */
    coptic_slot_t title_slot;     /* "Coptic Calendar" header */
    coptic_slot_t date_slot;      /* full date display */
    coptic_slot_t month_slot;     /* month name + origin */
    coptic_slot_t liturgy_slot;   /* liturgical significance */
    coptic_slot_t feast_slot;     /* feast (if active) */
    coptic_slot_t brief_slot;     /* brief meaning */

    /* Card frame */
    float card_x, card_y, card_w, card_h;
} daily_coptic_layout_t;

/* Compute the daily Coptic card layout.
 * jd: Julian Day number. */
daily_coptic_layout_t daily_coptic_compute(double jd);

/* Get the month origin string. Returns "?" if NULL layout. */
const char *daily_coptic_month_origin(const daily_coptic_layout_t *layout);

/* Check if the given date falls on a Coptic feast.
 * Returns 1 if feast active, 0 otherwise. Returns 0 for NULL layout. */
int daily_coptic_has_feast(const daily_coptic_layout_t *layout);

#endif /* TIME_DAILY_COPTIC_LAYOUT_H */
