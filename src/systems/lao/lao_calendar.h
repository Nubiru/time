/* lao_calendar.h -- Lao Buddhist Calendar
 * Buddhist Era (BE = CE + 543) year numbering with Gregorian structure.
 * Lao month names (Sanskrit-derived, romanized from Lao script).
 * Pi Mai detection (April 14-16), Nang Sangkhan maiden lookup.
 * Self-contained: implements its own Gregorian JD math internally.
 *
 * Pure functions: no globals, no malloc, no side effects. */

#ifndef TIME_LAO_CALENDAR_H
#define TIME_LAO_CALENDAR_H

#include <stdbool.h>

typedef struct {
    int be_year;    /* Buddhist Era year */
    int month;      /* 1-12 (January=1 through December=12) */
    int day;        /* 1-31 */
} lao_date_t;

typedef enum {
    LAO_FEST_NONE = 0,
    LAO_FEST_PI_MAI,          /* April 14-16: Lao New Year (Songkhan) */
    LAO_FEST_NATIONAL_DAY,    /* December 2: Founding of Lao PDR (1975) */
    LAO_FEST_LIBERATION_DAY,  /* October 12: Freedom from France (1945) */
    LAO_FEST_COUNT
} lao_festival_t;

/* Nang Sangkhan: seven celestial maidens, one per weekday.
 * The maiden who presides over Pi Mai is determined by which
 * day of the week April 14 falls on. */
typedef struct {
    int weekday;           /* 0=Sunday .. 6=Saturday */
    const char *name;      /* Maiden name (romanized Lao) */
    const char *mount;     /* Animal mount */
    const char *offering;  /* Items she carries */
} lao_sangkhan_t;

/* Convert Julian Day to Lao date (BE year, Gregorian month/day). */
lao_date_t lao_from_jd(double jd);

/* Convert Lao date to Julian Day (midnight UT). */
double lao_to_jd(lao_date_t date);

/* Lao month name (1-12, romanized). Returns "?" for invalid. */
const char *lao_month_name(int month);

/* Lao month abbreviation (1-12). Returns "?" for invalid. */
const char *lao_month_abbr(int month);

/* BE year from CE year. */
int lao_be_year(int ce_year);

/* CE year from BE year. */
int lao_ce_year(int be_year);

/* Check if a date falls on Pi Mai (April 14-16). */
bool lao_is_pi_mai(lao_date_t date);

/* Check festival for a given Lao date (fixed-date festivals only). */
lao_festival_t lao_festival(lao_date_t date);

/* Festival name string. Returns "" for NONE, "?" for invalid. */
const char *lao_festival_name(lao_festival_t fest);

/* Number of days in a Lao month (same as Gregorian). */
int lao_month_days(int be_year, int month);

/* Is this a leap year in the Lao calendar (same as Gregorian)? */
bool lao_is_leap_year(int be_year);

/* Day of week for a Lao date: 0=Sunday .. 6=Saturday. */
int lao_day_of_week(lao_date_t date);

/* Lao day name (0=Sunday .. 6=Saturday, romanized). Returns "?" for invalid. */
const char *lao_day_name(int weekday);

/* Nang Sangkhan maiden for a given BE year.
 * Determined by weekday of April 14 that year. */
lao_sangkhan_t lao_sangkhan(int be_year);

/* Lao zodiac animal name for a BE year (12-year cycle).
 * Note: Lao year starts in April, so Jan-Mar belong to previous animal.
 * Returns "?" for convenience if accessing the raw name. */
const char *lao_zodiac_animal(int be_year);

#endif /* TIME_LAO_CALENDAR_H */
