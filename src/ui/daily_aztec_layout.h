/* daily_aztec_layout.h — Daily Aztec calendar card layout
 *
 * Computes a card layout showing today's Tonalpohualli (260-day),
 * Xiuhpohualli (365-day), and Calendar Round positions.
 * All positions in normalized 0-1 coordinate space.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_DAILY_AZTEC_LAYOUT_H
#define TIME_DAILY_AZTEC_LAYOUT_H

#include "../systems/aztec/aztec.h"

/* Slot rectangle for positioning text elements. */
typedef struct {
    float x, y;
    float w, h;
} aztec_slot_t;

typedef struct {
    /* Tonalpohualli (260-day sacred count) */
    aztec_tonalpohualli_t tonal;    /* day_sign, day_number, day_index */
    const char *sign_name;          /* Nahuatl day sign name */

    /* Calendar Round (52-year cycle) */
    aztec_calendar_round_t round;   /* year_bearer, year_number, round_year */
    const char *bearer_name;        /* year bearer name */

    /* Interpretation */
    char glance[128];               /* T1 one-liner */
    char detail[512];               /* T3 narrative */

    /* Layout slots */
    aztec_slot_t title_slot;        /* "Aztec Calendar" */
    aztec_slot_t tonal_slot;        /* Tonalpohualli position */
    aztec_slot_t sign_slot;         /* day sign name + meaning */
    aztec_slot_t round_slot;        /* Calendar Round year */
    aztec_slot_t bearer_slot;       /* year bearer info */
    aztec_slot_t cycle_slot;        /* cycle position info */

    /* Card frame */
    float card_x, card_y, card_w, card_h;
} daily_aztec_layout_t;

/* Compute the daily Aztec card layout.
 * jd: Julian Day number. */
daily_aztec_layout_t daily_aztec_compute(double jd);

/* Get the day sign name. Returns "?" for NULL layout. */
const char *daily_aztec_sign_name(const daily_aztec_layout_t *layout);

#endif /* TIME_DAILY_AZTEC_LAYOUT_H */
