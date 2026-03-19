/* tarot_interpret.h — Tarot interpretation data (Thoth/Crowley tradition)
 *
 * Archetype data for the 22 Major Arcana and 4 elemental suits.
 * Composition function produces tiered interpretation text
 * (glyph / glance / detail).
 *
 * Uses the Thoth deck tradition (Crowley/Harris): psychological
 * archetypes informed by Qabalah, astrology, and alchemy.
 * Not fortune-telling — each card is a mirror showing a facet
 * of the psyche's current relationship with cosmic forces.
 *
 * Pure functions: no globals, no malloc, no side effects. */

#ifndef TIME_TAROT_INTERPRET_H
#define TIME_TAROT_INTERPRET_H

/* Per-Major Arcanum interpretation (22 cards, 0-21) */
typedef struct {
    int number;                 /* 0-21 (Fool through Universe) */
    const char *name;           /* Thoth name */
    const char *archetype;      /* Jungian/psychological archetype */
    const char *keyword;        /* Single word essence */
    const char *light;          /* Upright/integrated expression */
    const char *shadow;         /* Reversed/shadow expression */
    const char *brief;          /* One-sentence meaning */
} trt_major_t;

/* Per-suit interpretation (4 suits) */
typedef struct {
    int suit;                   /* 0-3 (Wands, Cups, Swords, Disks) */
    const char *name;           /* "Wands", "Cups", etc. */
    const char *element;        /* "Fire", "Water", "Air", "Earth" */
    const char *domain;         /* Life domain this suit governs */
    const char *quality;        /* Essential quality */
    const char *brief;          /* One-sentence meaning */
} trt_suit_t;

/* Tiered interpretation result */
typedef struct {
    char glyph[16];             /* T0: Roman numeral or card abbreviation */
    char glance[128];           /* T1: "The Magus — Skill and communication" */
    char detail[1024];          /* T3: Full narrative */
} tarot_interp_t;

/* Return interpretation data for a Major Arcanum (0-21).
 * Invalid number: returns struct with number=-1, all strings "?". */
trt_major_t trt_major_data(int number);

/* Return interpretation data for a Tarot suit (0-3).
 * 0=Wands, 1=Cups, 2=Swords, 3=Disks.
 * Invalid index: returns struct with suit=-1, all strings "?". */
trt_suit_t trt_suit_data(int suit);

/* Compose tiered interpretation from Tarot components.
 * major_number: 0-21 (Major Arcanum from zodiac/planet attribution).
 * decan_suit: 0-3 (suit of the decan card), or -1 if no decan.
 * decan_rank: 2-10 (rank of the decan card), or -1 if no decan.
 * Invalid major_number: "?" strings in result. */
tarot_interp_t trt_interpret(int major_number, int decan_suit, int decan_rank);

/* Returns 22 (number of Major Arcana). */
int trt_major_count(void);

/* Returns 4 (number of suits). */
int trt_suit_count(void);

#endif /* TIME_TAROT_INTERPRET_H */
