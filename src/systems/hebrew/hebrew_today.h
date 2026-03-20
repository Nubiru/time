/* hebrew_today.h — Hebrew Calendar "Today" page
 *
 * Builds a rich daily page with date info, Sefer Yetzirah month meaning,
 * sabbatical/jubilee cycle position, and upcoming holidays.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_HEBREW_TODAY_H
#define TIME_HEBREW_TODAY_H

#define HT_SECTION_MAX 512
#define HT_TITLE_MAX   128

typedef struct {
    /* Page header */
    char page_title[HT_TITLE_MAX];      /* "Hebrew -- 22 Adar II 5786" */

    /* Sections */
    char date_section[HT_SECTION_MAX];       /* Full date, year type, leap, day count */
    char month_section[HT_SECTION_MAX];      /* Month meaning: letter, tribe, sense */
    char sabbatical_section[HT_SECTION_MAX]; /* Shemitah cycle position */
    char jubilee_section[HT_SECTION_MAX];    /* Jubilee cycle position */
    char holiday_section[HT_SECTION_MAX];    /* Upcoming holidays (next 3) */

    int section_count;     /* always 5 */
    int has_data;
} ht_page_t;

/* Build complete Hebrew "Today" page for a Julian Day. */
ht_page_t ht_today(double jd);

/* Format full page as multi-line display text.
 * Returns chars written (excluding NUL), 0 on error. */
int ht_format(const ht_page_t *page, char *buf, int buf_size);

#endif /* TIME_HEBREW_TODAY_H */
