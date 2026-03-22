/* daily_celtic_layout.h — Daily Celtic Tree calendar card layout
 *
 * Computes a card layout showing today's Celtic tree month,
 * Ogham letter, and nearest Wheel of the Year festival.
 * All positions in normalized 0-1 coordinate space.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_DAILY_CELTIC_LAYOUT_H
#define TIME_DAILY_CELTIC_LAYOUT_H

#include "../systems/celtic/celtic_tree.h"

/* Slot rectangle for positioning text elements. */
typedef struct {
    float x, y;
    float w, h;
} celtic_slot_t;

typedef struct {
    /* Date */
    celtic_tree_date_t date;        /* month (0-13), day */
    const char *tree_name;          /* tree name for the month */
    const char *ogham_name;         /* Ogham name */
    int is_intercalary;             /* 1 if Yew intercalary day */

    /* Festival */
    int festival;                   /* 0-7 if active, -1 if none */

    /* Interpretation */
    char glance[128];               /* T1 one-liner */
    char detail[512];               /* T3 narrative */

    /* Layout slots */
    celtic_slot_t title_slot;       /* "Celtic Tree Calendar" */
    celtic_slot_t date_slot;        /* date display */
    celtic_slot_t tree_slot;        /* tree name */
    celtic_slot_t ogham_slot;       /* Ogham letter/name */
    celtic_slot_t festival_slot;    /* Wheel of Year festival */
    celtic_slot_t meaning_slot;     /* tree meaning */

    /* Card frame */
    float card_x, card_y, card_w, card_h;
} daily_celtic_layout_t;

/* Compute the daily Celtic card layout.
 * jd: Julian Day number. */
daily_celtic_layout_t daily_celtic_compute(double jd);

/* Check if a Wheel of the Year festival is active.
 * Returns 1 if yes, 0 if none. Returns 0 for NULL layout. */
int daily_celtic_has_festival(const daily_celtic_layout_t *layout);

#endif /* TIME_DAILY_CELTIC_LAYOUT_H */
