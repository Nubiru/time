/* gregorian_today.h — Gregorian "today" page for the Time artwork
 *
 * Composes a full-page summary of the current Gregorian date:
 * date details, month mythology, day-of-week planet, season, and
 * year progress.  Pure functions: no globals, no malloc, no GL. */

#ifndef TIME_GREGORIAN_TODAY_H
#define TIME_GREGORIAN_TODAY_H

#define GT_SECTION_MAX 512
#define GT_TITLE_MAX   128

typedef struct {
    char page_title[GT_TITLE_MAX];          /* "Gregorian — March 20, 2026 (Friday)" */
    int  year, month, day;                  /* parsed from JD */
    int  day_of_week;                       /* 0=Mon..6=Sun */

    char date_section[GT_SECTION_MAX];      /* Full date, day-of-year, week, quarter */
    char month_section[GT_SECTION_MAX];     /* Month origin, quality */
    char day_section[GT_SECTION_MAX];       /* Day-of-week: planet, mythology */
    char season_section[GT_SECTION_MAX];    /* Season name, theme, quality */
    char calendar_section[GT_SECTION_MAX];  /* Year progress, days remaining */

    int  section_count;                     /* always 5 */
    int  has_data;
} gt_page_t;

/* Build a Gregorian "today" page from Julian Day. */
gt_page_t gt_today(double jd);

/* Format the page into a single text buffer.
 * Returns number of characters written (excluding NUL), or 0 on error. */
int gt_format(const gt_page_t *page, char *buf, int buf_size);

#endif /* TIME_GREGORIAN_TODAY_H */
