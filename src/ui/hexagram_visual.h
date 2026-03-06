#ifndef TIME_HEXAGRAM_VISUAL_H
#define TIME_HEXAGRAM_VISUAL_H

#include "../systems/iching/iching.h"

#define HEX_ART_LINE_MAX 12
#define HEX_ART_LINES     6
#define HEX_ART_BUF_SIZE  (HEX_ART_LINE_MAX * HEX_ART_LINES)

typedef struct {
    char lines[HEX_ART_LINES][HEX_ART_LINE_MAX]; /* [0]=top(line6) [5]=bottom(line1) */
} hexagram_art_t;

/* Nuclear hexagram: lines 2,3,4 form lower trigram, lines 3,4,5 form upper.
 * Returns the King Wen number of the nuclear hexagram, or -1 for invalid. */
int hexagram_nuclear(int king_wen);

/* Complementary hexagram: invert all lines (yang<->yin).
 * Returns King Wen number, or -1 for invalid. */
int hexagram_complement(int king_wen);

/* Reverse hexagram: flip lines upside down (line 1<->6, 2<->5, 3<->4).
 * Returns King Wen number, or -1 for invalid. */
int hexagram_reverse(int king_wen);

/* Generate ASCII art for a hexagram. Top row = line 6, bottom = line 1.
 * Yang: "---------"  Yin: "---   ---" (ASCII-safe). */
hexagram_art_t hexagram_ascii(int king_wen);

/* Generate single-line summary: "1. Qian — Heaven/Heaven"
 * Writes to buf, returns buf. max includes null terminator. */
char *hexagram_summary(int king_wen, char *buf, int max);

/* Get element associated with a trigram (0-7). */
const char *trigram_element(int trigram);

/* Check if two hexagrams are complementary (all lines inverted). */
int hexagram_is_complement(int kw_a, int kw_b);

/* Get Unicode hexagram symbol (U+4DC0-U+4DFF, King Wen order).
 * Returns UTF-8 string or "?" for invalid. */
const char *hexagram_unicode(int king_wen);

#endif
