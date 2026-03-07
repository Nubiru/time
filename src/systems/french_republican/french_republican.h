/* french_republican.h — French Republican (Revolutionary) calendar
 *
 * Pure functions for bidirectional conversion between Julian Day
 * and the French Republican calendar. 12 months x 30 days + 5/6
 * complementary days (Sansculottides). Decimal time support.
 *
 * Month numbering: 1-12 for regular months, 0 for Sansculottides.
 * Epoch: 1 Vendemiaire An I = September 22, 1792 CE = JD 2375839.5.
 * Leap year: Romme rule — Gregorian rule applied to (year + 1).
 * Historical leap years: An III, VII, XI, XV, ...
 *
 * Sources: Reingold & Dershowitz, "Calendrical Calculations" (2018).
 *          Gilbert Romme (calendar designer, leap year proposal). */

#ifndef TIME_FRENCH_REPUBLICAN_H
#define TIME_FRENCH_REPUBLICAN_H

#include <stdbool.h>

/* French Republican date. month=0 means Sansculottides (complementary days). */
typedef struct {
    int year;    /* Republican year (An I = 1) */
    int month;   /* 1-12 for regular months, 0 for Sansculottides */
    int day;     /* 1-30 for months, 1-5/6 for Sansculottides */
} fr_date_t;

/* Decimal time (10-hour day, 100-minute hour, 100-second minute). */
typedef struct {
    int hours;    /* 0-9 (decimal hours) */
    int minutes;  /* 0-99 (decimal minutes) */
    int seconds;  /* 0-99 (decimal seconds) */
} fr_decimal_time_t;

/* Convert Julian Day to French Republican date. */
fr_date_t fr_from_jd(double jd);

/* Convert French Republican date to Julian Day (midnight UT). */
double fr_to_jd(fr_date_t date);

/* Month name (1-12). Returns "Sansculottides" for 0, "?" for invalid. */
const char *fr_month_name(int month);

/* Month meaning/translation (1-12). Returns "" for 0, "?" for invalid. */
const char *fr_month_meaning(int month);

/* Days in a given month. Always 30 for months 1-12.
 * 5 or 6 for month 0 (Sansculottides) depending on leap year. */
int fr_month_days(int year, int month);

/* Check if a Republican year is a leap year (sextile).
 * Romme rule: Gregorian leap rule applied to (year + 1). */
bool fr_is_leap_year(int year);

/* Total days in a year (365 or 366). */
int fr_year_days(int year);

/* Sansculottide day name (1-6). Returns "?" for invalid.
 * Day 6 (Revolution) exists only in leap years. */
const char *fr_sansculottide_name(int day);

/* Decade day name (1-10). Returns "?" for invalid.
 * Primidi(1) through Decadi(10). */
const char *fr_decade_day_name(int day);

/* Position within the decade (10-day week): 1-10.
 * Returns 0 for Sansculottide dates (outside decade system). */
int fr_decade_day(fr_date_t date);

/* Convert standard day fraction (0.0-1.0) to decimal time.
 * Clamps input to [0.0, 1.0). */
fr_decimal_time_t fr_decimal_time(double day_fraction);

/* Convert decimal time back to standard day fraction (0.0-1.0). */
double fr_decimal_to_standard(fr_decimal_time_t dt);

/* Check if a date falls in the Sansculottides period. */
bool fr_is_sansculottide(fr_date_t date);

/* Approximate CE year from Republican year (year + 1791). */
int fr_ce_year_approx(int rep_year);

/* Approximate Republican year from CE year (ce_year - 1791). */
int fr_rep_year_approx(int ce_year);

#endif /* TIME_FRENCH_REPUBLICAN_H */
