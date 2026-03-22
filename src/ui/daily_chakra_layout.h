/* daily_chakra_layout.h — Daily chakra card layout
 *
 * Computes a card layout showing today's chakra based on day of
 * week mapping, with Sanskrit/English names, theme, question,
 * and interpretation. All positions in normalized 0-1 coordinate space.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_DAILY_CHAKRA_LAYOUT_H
#define TIME_DAILY_CHAKRA_LAYOUT_H

#include "../systems/chakra/chakra_interpret.h"

/* Slot rectangle for positioning text elements. */
typedef struct {
    float x, y;
    float w, h;
} chakra_slot_t;

typedef struct {
    /* Chakra data */
    int chakra_index;               /* 0-6 (Muladhara through Sahasrara) */
    const char *sanskrit_name;      /* Sanskrit name */
    const char *english_name;       /* English name */
    const char *theme;              /* core theme */
    const char *question;           /* existential question */
    int day_of_week;                /* 0=Mon..6=Sun */

    /* Interpretation */
    char glance[128];               /* T1 one-liner */
    char detail[512];               /* T3 narrative */

    /* Layout slots */
    chakra_slot_t title_slot;       /* "Chakra" */
    chakra_slot_t chakra_name_slot; /* chakra name display */
    chakra_slot_t theme_slot;       /* theme */
    chakra_slot_t question_slot;    /* existential question */
    chakra_slot_t energy_slot;      /* energy description */
    chakra_slot_t meditation_slot;  /* meditation guidance */

    /* Card frame */
    float card_x, card_y, card_w, card_h;
} daily_chakra_layout_t;

/* Compute the daily chakra card layout.
 * jd: Julian Day number. */
daily_chakra_layout_t daily_chakra_compute(double jd);

#endif /* TIME_DAILY_CHAKRA_LAYOUT_H */
