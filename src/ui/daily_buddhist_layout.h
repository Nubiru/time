/* daily_buddhist_layout.h — Daily Buddhist calendar card layout
 *
 * Computes a card layout showing today's Buddhist Era year,
 * Uposatha observance status, lunar month, and Vassa retreat.
 * All positions in normalized 0-1 coordinate space.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_DAILY_BUDDHIST_LAYOUT_H
#define TIME_DAILY_BUDDHIST_LAYOUT_H

#include "../systems/buddhist/buddhist.h"

/* Slot rectangle for positioning text elements. */
typedef struct {
    float x, y;
    float w, h;
} bud_slot_t;

typedef struct {
    int be_year;                    /* Buddhist Era year */

    /* Uposatha */
    int uposatha_type;              /* uposatha_type_t cast to int */
    const char *uposatha_name;      /* "Purnima", "Amavasya", etc. */
    const char *uposatha_desc;      /* brief description */
    double moon_age_days;           /* days since last new moon */
    double illumination;            /* 0.0-1.0 */

    /* Month */
    int month_number;               /* 1-12 */
    const char *month_name;         /* Pali month name */
    int is_vassa;                   /* 1 if within Rains Retreat */

    /* Layout slots */
    bud_slot_t title_slot;          /* "Buddhist Calendar" */
    bud_slot_t era_slot;            /* "BE 2568" */
    bud_slot_t month_slot;          /* month name */
    bud_slot_t uposatha_slot;       /* observance status */
    bud_slot_t moon_slot;           /* moon age + illumination */
    bud_slot_t vassa_slot;          /* vassa status */

    /* Card frame */
    float card_x, card_y, card_w, card_h;
} daily_buddhist_layout_t;

/* Compute the daily Buddhist card layout.
 * jd: Julian Day number. */
daily_buddhist_layout_t daily_buddhist_compute(double jd);

/* Check if today is an Uposatha observance day.
 * Returns 1 if any observance, 0 if UPOSATHA_NONE.
 * Returns 0 for NULL layout. */
int daily_buddhist_is_observance(const daily_buddhist_layout_t *layout);

#endif /* TIME_DAILY_BUDDHIST_LAYOUT_H */
