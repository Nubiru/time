/* persian.h — Persian Solar Hijri (Jalali) calendar
 * Pure functions for bidirectional conversion between Julian Day
 * and the Solar Hijri calendar.
 *
 * 6 months x 31 days + 5 months x 30 days + Esfand (29 or 30 in leap) = 365/366.
 * Leap year: 2820-year grand cycle with 683 leap years per cycle.
 * Epoch: JD 1948320.5 = 1 Farvardin 1 SH = March 22, 622 CE (Julian).
 *
 * Sources: Reingold & Dershowitz, "Calendrical Calculations" (2018). */

#ifndef TIME_PERSIAN_H
#define TIME_PERSIAN_H

#include <stdbool.h>

typedef struct {
    int year;     /* Solar Hijri year (can be negative for dates before epoch) */
    int month;    /* 1-12 */
    int day;      /* 1-31 */
} persian_date_t;

typedef enum {
    PERSIAN_FEST_NONE = 0,
    PERSIAN_FEST_NOWRUZ,              /* 1 Farvardin */
    PERSIAN_FEST_SIZDAH_BEDAR,        /* 13 Farvardin */
    PERSIAN_FEST_YALDA,               /* 30 Azar (month 9, day 30) */
    PERSIAN_FEST_CHAHARSHANBE_SURI,   /* Last Wednesday of Esfand */
    PERSIAN_FEST_COUNT
} persian_festival_t;

/* Convert Julian Day to Persian date. */
persian_date_t persian_from_jd(double jd);

/* Convert Persian date to Julian Day (midnight UT). */
double persian_to_jd(persian_date_t date);

/* Persian month name (1-12). Returns "?" for invalid. */
const char *persian_month_name(int month);

/* Days in a given month of a given year (handles leap year for month 12). */
int persian_month_days(int year, int month);

/* Weekday from JD (0=Shanbe/Saturday .. 6=Jom'e/Friday). */
int persian_weekday(double jd);

/* Weekday name (0=Shanbe .. 6=Jom'e). Returns "?" for invalid. */
const char *persian_weekday_name(int day);

/* Check if a Solar Hijri year is a leap year. */
bool persian_is_leap_year(int year);

/* Total days in a year (365 or 366). */
int persian_year_days(int year);

/* Detect festival for a given Persian date. */
persian_festival_t persian_festival(persian_date_t date);

/* Festival name string. Returns "" for NONE, "?" for invalid. */
const char *persian_festival_name(persian_festival_t fest);

/* Approximate CE year from SH year (not exact, off by up to 1). */
int persian_ce_year_approx(int sh_year);

/* Approximate SH year from CE year (not exact, off by up to 1). */
int persian_sh_year_approx(int ce_year);

#endif /* TIME_PERSIAN_H */
