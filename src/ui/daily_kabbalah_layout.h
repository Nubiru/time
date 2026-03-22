/* daily_kabbalah_layout.h — Daily Kabbalah Tree of Life card layout
 *
 * Computes a card layout showing today's Sefirah meditation and
 * path journey. Cycles through 10 Sefirot and 22 paths on a
 * daily basis. Includes pillar, triad, Hebrew letter, and Tarot.
 * All positions in normalized 0-1 coordinate space.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_DAILY_KABBALAH_LAYOUT_H
#define TIME_DAILY_KABBALAH_LAYOUT_H

#include "../systems/kabbalah/sefirot.h"
#include "../systems/kabbalah/tree_geometry.h"

/* Slot rectangle for positioning text elements. */
typedef struct {
    float x, y;
    float w, h;
} kabbalah_slot_t;

typedef struct {
    /* Today's Sefirah (cycles through 10) */
    int sefirah_index;            /* 0-9 */
    const char *sefirah_name;     /* "Keter", "Chokmah", etc. */
    const char *sefirah_meaning;  /* "Crown", "Wisdom", etc. */
    const char *pillar_name;      /* "Balance", "Mercy", "Severity" */
    const char *triad_name;       /* "Supernal", "Moral", "Material", "Kingdom" */

    /* Sefirah interpretation */
    const char *meditation;       /* Contemplative focus */
    const char *question;         /* The question this Sefirah asks */
    const char *brief;            /* One-sentence essence */

    /* Today's path (cycles through 22) */
    int path_index;               /* 0-21 */
    const char *hebrew_letter;    /* "Aleph", "Bet", etc. */
    const char *tarot;            /* Major Arcana name */
    const char *path_journey;     /* What traveling this path means */

    /* Layout slots */
    kabbalah_slot_t title_slot;   /* "Kabbalah" header */
    kabbalah_slot_t sefirah_slot; /* Sefirah name + meaning */
    kabbalah_slot_t pillar_slot;  /* Pillar + triad */
    kabbalah_slot_t meditate_slot;/* Meditation focus */
    kabbalah_slot_t question_slot;/* Contemplative question */
    kabbalah_slot_t path_slot;    /* Today's path + Hebrew letter */
    kabbalah_slot_t journey_slot; /* Path journey text */

    /* Card frame */
    float card_x, card_y, card_w, card_h;
} daily_kabbalah_layout_t;

/* Compute the daily Kabbalah card layout.
 * jd: Julian Day number. */
daily_kabbalah_layout_t daily_kabbalah_compute(double jd);

/* Get the meditation focus string. Returns "?" if NULL layout. */
const char *daily_kabbalah_meditation(const daily_kabbalah_layout_t *layout);

/* Get the Hebrew letter for today's path. Returns "?" if NULL layout. */
const char *daily_kabbalah_letter(const daily_kabbalah_layout_t *layout);

#endif /* TIME_DAILY_KABBALAH_LAYOUT_H */
