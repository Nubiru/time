/* iching_interpret.h — I Ching hexagram interpretation data
 *
 * Static interpretation data for all 64 hexagrams: judgment, image,
 * keywords, and element. Plus a composition function producing tiered
 * interpretation text (glyph, glance, detail).
 *
 * Pure functions: no globals, no malloc, no side effects. */

#ifndef TIME_ICHING_INTERPRET_H
#define TIME_ICHING_INTERPRET_H

/* Per-hexagram interpretation data */
typedef struct {
    int king_wen;               /* 1-64 */
    const char *name;           /* English name */
    const char *judgment;       /* Core judgment (brief, 1 sentence) */
    const char *image;          /* The Image text (1 sentence) */
    const char *keywords;       /* 2-3 keywords separated by commas */
    const char *element;        /* Primary element association */
} ii_hexagram_t;

/* Tiered interpretation result */
typedef struct {
    char glyph[8];              /* T0: hexagram number as string "23" */
    char glance[128];           /* T1: "Hexagram 23 Splitting Apart — Mountain over Earth" */
    char detail[512];           /* T3: Full narrative with judgment + image + trigrams */
} iching_interp_t;

/* Return interpretation data for a hexagram by King Wen number (1-64).
 * Invalid: returns struct with king_wen=-1, all strings "?". */
ii_hexagram_t ii_hexagram_data(int king_wen);

/* Compose tiered interpretation from hexagram number and trigram names.
 * king_wen: 1-64, upper_tri/lower_tri: trigram name strings.
 * NULL trigram names are replaced with "?". */
iching_interp_t ii_interpret(int king_wen, const char *upper_tri,
                             const char *lower_tri);

/* Returns 64 (total number of hexagrams). */
int ii_hexagram_count(void);

#endif /* TIME_ICHING_INTERPRET_H */
