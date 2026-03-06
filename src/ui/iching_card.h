#ifndef TIME_ICHING_CARD_H
#define TIME_ICHING_CARD_H

#include <stddef.h>

#define ICHING_ART_LINES 6
#define ICHING_ART_MAX   12

/* I Ching display data — all strings ready for card rendering. */
typedef struct {
    int  king_wen;                             /* 1-64 */
    char title[48];                            /* "Hexagram 42 · Yi" */
    char trigrams[48];                         /* "☴ Wind / ☳ Thunder" */
    char nuclear_line[48];                     /* "Nuclear: 23 · Bo" */
    char unicode_sym[8];                       /* Unicode hexagram char */
    char ascii_art[ICHING_ART_LINES][ICHING_ART_MAX]; /* [0]=top [5]=bottom */
    char summary[64];                          /* "☷ 42 Yi · Wind/Thunder" */
} iching_display_t;

/* Compute all display strings from Julian Day. */
iching_display_t iching_display_from_jd(double jd);

/* Compute all display strings from King Wen number (1-64). */
iching_display_t iching_display_from_number(int king_wen);

/* Format title: "Hexagram 42 · Yi" */
void iching_fmt_title(int king_wen, char *buf, size_t sz);

/* Format trigrams: "☴ Wind / ☳ Thunder" */
void iching_fmt_trigrams(int lower, int upper, char *buf, size_t sz);

#endif
