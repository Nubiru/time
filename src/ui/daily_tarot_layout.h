/* daily_tarot_layout.h — Daily Tarot card layout
 *
 * Computes a card layout showing today's deterministic Major Arcanum
 * with Hebrew letter, Tree of Life path, and interpretation.
 * All positions in normalized 0-1 coordinate space.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_DAILY_TAROT_LAYOUT_H
#define TIME_DAILY_TAROT_LAYOUT_H

#include "../systems/tarot/tarot.h"

/* Slot rectangle for positioning text elements. */
typedef struct {
    float x, y;
    float w, h;
} tarot_slot_t;

typedef struct {
    /* Card data */
    int card_number;                /* 0-21 Major Arcanum */
    const char *card_name;          /* Thoth deck name */
    const char *hebrew_letter;      /* Hebrew letter name */
    int tree_path;                  /* Tree of Life path (0-21) */

    /* Interpretation */
    char glance[128];               /* T1 one-liner */
    char detail[1024];              /* T3 narrative */

    /* Layout slots */
    tarot_slot_t title_slot;        /* "Tarot" */
    tarot_slot_t card_slot;         /* card name display */
    tarot_slot_t arcana_slot;       /* arcanum number */
    tarot_slot_t hebrew_slot;       /* Hebrew letter */
    tarot_slot_t path_slot;         /* Tree of Life path */
    tarot_slot_t meaning_slot;      /* interpretation glance */

    /* Card frame */
    float card_x, card_y, card_w, card_h;
} daily_tarot_layout_t;

/* Compute the daily Tarot card layout.
 * jd: Julian Day number. */
daily_tarot_layout_t daily_tarot_compute(double jd);

#endif /* TIME_DAILY_TAROT_LAYOUT_H */
