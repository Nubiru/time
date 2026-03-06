#ifndef TIME_ICHING_H
#define TIME_ICHING_H

typedef struct {
    int lines[6];        /* 0=yin, 1=yang, [0]=bottom [5]=top */
    int lower_trigram;    /* 0-7 */
    int upper_trigram;    /* 0-7 */
    int king_wen;         /* 1-64 */
} hexagram_t;

/* Get hexagram by King Wen number (1-64). Returns zeroed struct for invalid. */
hexagram_t    iching_hexagram(int king_wen_number);

/* Get hexagram for a Julian Day (cycles through 64 hexagrams). */
hexagram_t    iching_from_jd(double jd);

/* Get name of hexagram by King Wen number. Returns "?" for invalid. */
const char   *iching_hexagram_name(int king_wen);

/* Get trigram name by index (0-7). Returns "?" for invalid. */
const char   *iching_trigram_name(int trigram);

/* Get trigram Unicode symbol by index (0-7). Returns "?" for invalid. */
const char   *iching_trigram_symbol(int trigram);

/* Convert 6 lines to King Wen number. Returns -1 for invalid. */
int           iching_lines_to_king_wen(const int lines[6]);

#endif
