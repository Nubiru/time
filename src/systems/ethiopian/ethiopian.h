/* ethiopian.h — Ethiopian calendar
 * Pure functions for bidirectional conversion between Julian Day
 * and the Ethiopian calendar.
 *
 * 12 months x 30 days + Pagume (5 or 6 epagomenal days) = 365 or 366 days.
 * Leap year when (year % 4) == 3.
 * Epoch: JD 1724220.5 = 29 Aug 8 CE (Julian) = Year 1, 1 Meskerem.
 *
 * Structurally identical to the Coptic calendar (same leap rule, same
 * 13-month structure) but with different epoch and Ge'ez month names.
 *
 * Knowledge gap: Fasika (Ethiopian Easter) is a movable feast computed
 * via the Alexandrian computus. Not implemented as a fixed date; the feast
 * detection function will never return ETH_FEAST_FASIKA. */

#ifndef TIME_ETHIOPIAN_H
#define TIME_ETHIOPIAN_H

#include <stdbool.h>

typedef struct {
    int year;    /* Ethiopian year (year 1 = 8 CE) */
    int month;   /* 1-13 (Meskerem=1 through Pagume=13) */
    int day;     /* 1-30 for months 1-12, 1-5 or 1-6 for month 13 */
} ethiopian_date_t;

typedef enum {
    ETH_FEAST_NONE = 0,
    ETH_FEAST_ENKUTATASH,    /* 1 Meskerem — New Year */
    ETH_FEAST_MESKEL,        /* 17 Meskerem — Finding of True Cross */
    ETH_FEAST_GENNA,         /* 29 Tahsas (month 4) — Christmas */
    ETH_FEAST_TIMKAT,        /* 11 Tir (month 5) — Epiphany */
    ETH_FEAST_FASIKA,        /* Movable — Easter (not implemented) */
    ETH_FEAST_COUNT
} ethiopian_feast_t;

/* Convert Julian Day to Ethiopian date. */
ethiopian_date_t ethiopian_from_jd(double jd);

/* Convert Ethiopian date to Julian Day (midnight UT). */
double ethiopian_to_jd(ethiopian_date_t date);

/* Month name string (1-13). Returns "?" for invalid. */
const char *ethiopian_month_name(int month);

/* True if Ethiopian year is a leap year (year % 4 == 3). */
bool ethiopian_is_leap(int year);

/* Number of days in a month (30 for 1-12, 5 or 6 for 13, 0 for invalid).
 * Month 13 depends on whether the year is a leap year. */
int ethiopian_month_days(int year, int month);

/* Total days in an Ethiopian year (365 or 366). */
int ethiopian_year_days(int year);

/* Convenience: extract Ethiopian year from a Julian Day. */
int ethiopian_year(double jd);

/* Check if a given date falls on a fixed feast. */
ethiopian_feast_t ethiopian_feast(ethiopian_date_t date);

/* Feast name string. Returns "" for NONE, "?" for invalid. */
const char *ethiopian_feast_name(ethiopian_feast_t feast);

/* Day of year (1-365 or 1-366) for a given Ethiopian date. */
int ethiopian_day_of_year(ethiopian_date_t date);

#endif /* TIME_ETHIOPIAN_H */
