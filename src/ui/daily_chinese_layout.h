/* daily_chinese_layout.h — Daily Chinese calendar card layout
 *
 * Computes a card layout showing today's Chinese sexagenary cycle data:
 * zodiac animal, element, polarity, heavenly stem, earthly branch.
 * All positions in normalized 0-1 coordinate space.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_DAILY_CHINESE_LAYOUT_H
#define TIME_DAILY_CHINESE_LAYOUT_H

#include "../systems/chinese/chinese.h"

/* Slot rectangle for positioning text elements. */
typedef struct {
    float x, y;
    float w, h;
} cn_slot_t;

typedef struct {
    /* Year data */
    int stem;           /* 0-9 Heavenly Stem index */
    int branch;         /* 0-11 Earthly Branch index */
    int animal;         /* 0-11 zodiac animal index */
    int element;        /* 0-4 element index */
    int polarity;       /* 0=Yang, 1=Yin */
    int cycle_year;     /* 1-60 within sexagenary cycle */
    const char *animal_name;    /* "Rat", "Snake", etc. */
    const char *element_name;   /* "Wood", "Fire", etc. */
    const char *polarity_name;  /* "Yang" or "Yin" */
    const char *stem_name;      /* "Jia", "Yi", etc. */
    const char *branch_name;    /* "Zi", "Chou", etc. */
    const char *animal_symbol;  /* Unicode emoji */

    /* Interpretation (from chinese_interpret) */
    char glance[128];           /* "Water Tiger (Yang) — ..." */
    char detail[512];           /* Full narrative */

    /* Layout slots */
    cn_slot_t title_slot;       /* "Chinese Calendar" */
    cn_slot_t animal_slot;      /* animal emoji + name */
    cn_slot_t element_slot;     /* element + polarity */
    cn_slot_t cycle_slot;       /* "Year X of 60" */
    cn_slot_t stem_slot;        /* Heavenly Stem */
    cn_slot_t branch_slot;      /* Earthly Branch */

    /* Card frame */
    float card_x, card_y, card_w, card_h;
} daily_chinese_layout_t;

/* Compute the daily Chinese card layout.
 * jd: Julian Day number. */
daily_chinese_layout_t daily_chinese_compute(double jd);

/* Get the animal name string. Returns "?" if NULL layout. */
const char *daily_chinese_animal_name(const daily_chinese_layout_t *layout);

#endif /* TIME_DAILY_CHINESE_LAYOUT_H */
