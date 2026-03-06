#ifndef TIME_CHINESE_FMT_H
#define TIME_CHINESE_FMT_H

#include <stddef.h>

/* Chinese calendar display data — all strings ready for rendering. */
typedef struct {
    char title[48];           /* "🐎 Year of the Fire Horse" */
    char stem_branch[32];     /* "Bing-Wu" */
    char element_line[32];    /* "Fire · Yang" */
    char cycle_line[32];      /* "Year 43 of 60" */
    char animal_line[32];     /* "🐎 Horse" */
    char summary[64];         /* "🐎 Fire Horse · Bing-Wu · Yang" */
} chinese_display_t;

/* Compute all display strings from Gregorian year. */
chinese_display_t chinese_display_from_year(int year);

/* Compute all display strings from Julian Day. */
chinese_display_t chinese_display_from_jd(double jd);

/* Format title: "🐎 Year of the Fire Horse" */
void chinese_fmt_title(int animal, int element, char *buf, size_t sz);

/* Format stem-branch pair: "Bing-Wu" */
void chinese_fmt_stem_branch(int stem, int branch, char *buf, size_t sz);

/* Format element + polarity: "Fire · Yang" */
void chinese_fmt_element(int element, int polarity, char *buf, size_t sz);

/* Format cycle position: "Year 43 of 60" */
void chinese_fmt_cycle(int cycle_year, char *buf, size_t sz);

/* Format animal line: "🐎 Horse" */
void chinese_fmt_animal(int animal, char *buf, size_t sz);

#endif
