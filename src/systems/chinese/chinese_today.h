/* chinese_today.h — Chinese Calendar "Today" page
 *
 * Builds a rich yearly page with year animal, element, stem-branch
 * designation, sexagenary cycle position, polarity, animal archetype
 * with strengths/challenges/compatibility, and element interpretation.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_CHINESE_TODAY_H
#define TIME_CHINESE_TODAY_H

#define CT_SECTION_MAX 512
#define CT_TITLE_MAX   128

typedef struct {
    /* Page header */
    char page_title[CT_TITLE_MAX];        /* "{emoji} Year of the Fire Horse" */

    /* Sections */
    char year_section[CT_SECTION_MAX];    /* Stem-branch, animal, element, polarity, cycle */
    char animal_section[CT_SECTION_MAX];  /* Archetype, strengths, challenges */
    char element_section[CT_SECTION_MAX]; /* Element nature, season, expression */
    char compat_section[CT_SECTION_MAX];  /* Compatible animals */
    char cycle_section[CT_SECTION_MAX];   /* Sexagenary cycle position, remaining years */

    int section_count;  /* always 5 */
    int has_data;
} ct_page_t;

/* Build complete Chinese Calendar "Today" page for a Julian Day. */
ct_page_t ct_today(double jd);

/* Format full page as multi-line display text.
 * Returns chars written (excluding NUL), 0 on error. */
int ct_format(const ct_page_t *page, char *buf, int buf_size);

#endif /* TIME_CHINESE_TODAY_H */
