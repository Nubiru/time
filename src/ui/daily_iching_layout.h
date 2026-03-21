#ifndef TIME_DAILY_ICHING_LAYOUT_H
#define TIME_DAILY_ICHING_LAYOUT_H

/* Daily I Ching card layout — today's hexagram with interpretation.
 *
 * Computes a card layout showing today's I Ching hexagram with its
 * name, judgment, image, keywords, line pattern, and trigram names.
 * All positions in normalized 0-1 coordinate space.
 *
 * Card bounds: x=0.05, y=0.02, w=0.90, h=0.96
 * Slot layout (vertical stack):
 *   Title:          y=0.03
 *   Number:         y=0.12
 *   Name:           y=0.22
 *   Lines (6):      y=0.30, h=0.30
 *   Upper trigram:  y=0.62
 *   Lower trigram:  y=0.70
 *   Judgment:       y=0.78
 *   Image:          y=0.88
 *
 * Sources: Wilhelm/Baynes, "I Ching" (King Wen sequence).
 * Pure module: no GL, no malloc, no globals, no side effects. */

typedef struct {
    float x, y, w, h;
} ic_slot_t;

typedef struct {
    /* Hexagram identity */
    int king_wen;
    const char *name;
    const char *judgment;
    const char *image;
    const char *keywords;

    /* Lines */
    int lines[6];            /* 0=yin, 1=yang, [0]=bottom [5]=top */

    /* Trigrams */
    int upper_trigram;
    int lower_trigram;
    const char *upper_name;
    const char *lower_name;
    const char *upper_symbol;
    const char *lower_symbol;

    /* Layout positions */
    ic_slot_t title_slot;     /* "I Ching" header */
    ic_slot_t number_slot;    /* hexagram number */
    ic_slot_t name_slot;      /* hexagram name */
    ic_slot_t lines_slot;     /* area for 6 lines */
    ic_slot_t upper_slot;     /* upper trigram label */
    ic_slot_t lower_slot;     /* lower trigram label */
    ic_slot_t judgment_slot;  /* judgment text */
    ic_slot_t image_slot;     /* image text */

    /* Card frame */
    float card_x, card_y, card_w, card_h;
} daily_iching_layout_t;

/* Compute the daily I Ching card layout from a Julian Day.
 * Uses iching_from_jd() to determine today's hexagram. */
daily_iching_layout_t daily_iching_compute(double jd);

/* Get the hexagram name from a computed layout.
 * Returns "?" for NULL layout or king_wen=0 (invalid). */
const char *daily_iching_hexagram_name(const daily_iching_layout_t *layout);

#endif /* TIME_DAILY_ICHING_LAYOUT_H */
