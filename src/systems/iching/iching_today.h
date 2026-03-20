/* iching_today.h — I Ching "Today" page
 *
 * Builds a complete daily I Ching page: hexagram of the day with trigram
 * breakdown, judgment, image, keywords, element, and ASCII line art.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_ICHING_TODAY_H
#define TIME_ICHING_TODAY_H

#define IC_SECTION_MAX 512
#define IC_TITLE_MAX   128
#define IC_LINE_MAX     32

typedef struct {
    /* Page header */
    char page_title[IC_TITLE_MAX];          /* "I Ching -- Hexagram 42 . Yi (Increase)" */
    int  king_wen;                          /* 1-64 */

    /* Sections */
    char hexagram_section[IC_SECTION_MAX];  /* Number, name, element, keywords */
    char trigram_section[IC_SECTION_MAX];   /* Upper/lower trigrams with symbols */
    char judgment_section[IC_SECTION_MAX];  /* The Judgment text */
    char image_section[IC_SECTION_MAX];     /* The Image text */
    char lines_section[IC_SECTION_MAX];     /* ASCII visualization of 6 lines */

    int  section_count;                     /* always 5 */
    int  has_data;
} ic_page_t;

/* Build I Ching "Today" page for a Julian Day. */
ic_page_t ic_today(double jd);

/* Format page as multi-line text. Returns chars written, 0 on error. */
int ic_format(const ic_page_t *page, char *buf, int buf_size);

#endif /* TIME_ICHING_TODAY_H */
