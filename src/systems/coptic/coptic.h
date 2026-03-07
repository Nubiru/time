/* coptic.h — Coptic (Anno Martyrum) calendar
 * Pure functions for bidirectional conversion between Julian Day
 * and the Coptic calendar.
 *
 * 12 months x 30 days + Nasie (5 or 6 epagomenal days) = 365 or 366 days.
 * Leap year when (year % 4) == 3.
 * Epoch: JD 1825029.5 = 29 Aug 284 CE (Julian) = AM 1, 1 Thout.
 *
 * Knowledge gap: Easter (COPTIC_FEAST_EASTER) is a movable feast computed
 * via the Alexandrian computus. Not implemented as a fixed date; the feast
 * detection function will never return COPTIC_FEAST_EASTER. */

#ifndef TIME_COPTIC_H
#define TIME_COPTIC_H

#include <stdbool.h>

typedef struct {
    int year;    /* Anno Martyrum (year 1 = 284 CE) */
    int month;   /* 1-13 (Thout=1 through Nasie=13) */
    int day;     /* 1-30 for months 1-12, 1-5 or 1-6 for month 13 */
} coptic_date_t;

typedef enum {
    COPTIC_FEAST_NONE = 0,
    COPTIC_FEAST_NAYROUZ,
    COPTIC_FEAST_CROSS,
    COPTIC_FEAST_CHRISTMAS,
    COPTIC_FEAST_EPIPHANY,
    COPTIC_FEAST_ANNUNCIATION,
    COPTIC_FEAST_EASTER,
    COPTIC_FEAST_COUNT
} coptic_feast_t;

/* Convert Julian Day to Coptic date. */
coptic_date_t coptic_from_jd(double jd);

/* Convert Coptic date to Julian Day (midnight UT). */
double coptic_to_jd(coptic_date_t date);

/* Month name string (1-13). Returns "?" for invalid. */
const char *coptic_month_name(int month);

/* True if Coptic year is a leap year (year % 4 == 3). */
bool coptic_is_leap(int year);

/* Number of days in a month (30 for 1-12, 5 or 6 for 13, 0 for invalid).
 * Month 13 depends on whether the year is a leap year. */
int coptic_month_days(int year, int month);

/* Total days in a Coptic year (365 or 366). */
int coptic_year_days(int year);

/* Convenience: extract Anno Martyrum year from a Julian Day. */
int coptic_year(double jd);

/* Check if a given date falls on a fixed feast. */
coptic_feast_t coptic_feast(coptic_date_t date);

/* Feast name string. Returns "" for NONE, "?" for invalid. */
const char *coptic_feast_name(coptic_feast_t feast);

/* Day of year (1-365 or 1-366) for a given Coptic date. */
int coptic_day_of_year(coptic_date_t date);

#endif /* TIME_COPTIC_H */
