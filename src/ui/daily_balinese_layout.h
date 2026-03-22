/* daily_balinese_layout.h — Daily Balinese Pawukon card layout
 *
 * Computes a card layout showing today's Pawukon 210-day cycle
 * position with wuku name, urip weight, and ceremony status.
 * All positions in normalized 0-1 coordinate space.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_DAILY_BALINESE_LAYOUT_H
#define TIME_DAILY_BALINESE_LAYOUT_H

#include "../systems/balinese/pawukon.h"

/* Slot rectangle for positioning text elements. */
typedef struct {
    float x, y;
    float w, h;
} bali_slot_t;

typedef struct {
    /* Pawukon data */
    pawukon_t pawukon;              /* full 10-cycle concurrent date */
    const char *wuku_name;          /* wuku name (0-29) */
    int urip;                       /* spiritual weight */
    int is_tumpek;                  /* 1 if Tumpek day */
    int is_kajeng_kliwon;           /* 1 if Kajeng Kliwon day */
    const char *ceremony;           /* ceremony name or NULL */

    /* Interpretation */
    char glance[128];               /* T1 one-liner */
    char detail[512];               /* T3 narrative */

    /* Layout slots */
    bali_slot_t title_slot;         /* "Balinese Pawukon" */
    bali_slot_t wuku_slot;          /* wuku name + day */
    bali_slot_t cycle_slot;         /* day in 210-day cycle */
    bali_slot_t urip_slot;          /* spiritual weight */
    bali_slot_t ceremony_slot;      /* ceremony (if active) */
    bali_slot_t special_slot;       /* Tumpek / Kajeng Kliwon */

    /* Card frame */
    float card_x, card_y, card_w, card_h;
} daily_balinese_layout_t;

/* Compute the daily Balinese Pawukon card layout.
 * jd: Julian Day number. */
daily_balinese_layout_t daily_balinese_compute(double jd);

/* Check if a ceremony is active. Returns 1 if yes, 0 if none.
 * Returns 0 for NULL layout. */
int daily_balinese_has_ceremony(const daily_balinese_layout_t *layout);

#endif /* TIME_DAILY_BALINESE_LAYOUT_H */
