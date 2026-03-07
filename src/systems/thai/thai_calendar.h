/* thai_calendar.h -- Thai Solar Calendar
 * Buddhist Era (BE = CE + 543) year numbering with Gregorian structure.
 * Thai month names, Songkran detection, 1941 calendar reform awareness.
 * Self-contained: implements its own Gregorian JD math internally. */

#ifndef TIME_THAI_CALENDAR_H
#define TIME_THAI_CALENDAR_H

#include <stdbool.h>

typedef struct {
    int be_year;    /* Buddhist Era year */
    int month;      /* 1-12 (January=1 through December=12) */
    int day;        /* 1-31 */
} thai_date_t;

typedef enum {
    THAI_FEST_NONE = 0,
    THAI_FEST_SONGKRAN,       /* April 13-15: Thai New Year water festival */
    THAI_FEST_CHAKRI_DAY,     /* April 6: Chakri Memorial Day */
    THAI_FEST_CONSTITUTION,   /* December 10: Constitution Day */
    THAI_FEST_COUNT
} thai_festival_t;

/* Convert Julian Day to Thai date (BE year, Gregorian month/day). */
thai_date_t thai_from_jd(double jd);

/* Convert Thai date to Julian Day (midnight UT). */
double thai_to_jd(thai_date_t date);

/* Thai month name (1-12). Returns "?" for invalid. */
const char *thai_month_name(int month);

/* Thai month abbreviation (1-12). Returns "?" for invalid. */
const char *thai_month_abbr(int month);

/* BE year from CE year. */
int thai_be_year(int ce_year);

/* CE year from BE year. */
int thai_ce_year(int be_year);

/* Check if a date falls on Songkran (April 13-15). */
bool thai_is_songkran(thai_date_t date);

/* Check festival for a given Thai date (fixed-date festivals only). */
thai_festival_t thai_festival(thai_date_t date);

/* Festival name string. Returns "" for NONE, "?" for invalid. */
const char *thai_festival_name(thai_festival_t fest);

/* Is a given CE year before the 1941 calendar reform?
 * Before reform: Thai year started April 1. After: January 1. */
bool thai_is_pre_reform(int ce_year);

/* Number of days in a Thai month (same as Gregorian). */
int thai_month_days(int be_year, int month);

/* Is this a leap year in the Thai calendar (same as Gregorian)? */
bool thai_is_leap_year(int be_year);

#endif /* TIME_THAI_CALENDAR_H */
