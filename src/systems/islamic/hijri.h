/* hijri.h — Tabular Islamic (Hijri) calendar
 * Pure arithmetic calendar (no moon observation).
 * Algorithm: Dershowitz & Reingold "Calendrical Calculations" */

#ifndef TIME_HIJRI_H
#define TIME_HIJRI_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
    int year;    /* Hijri year (e.g., 1446) */
    int month;   /* 1-12 (1=Muharram, 12=Dhul Hijjah) */
    int day;     /* 1-30 */
} hijri_date_t;

/* Convert Julian Day to Hijri date (tabular/arithmetic calendar). */
hijri_date_t hijri_from_jd(double jd);

/* Convert Hijri date to Julian Day (midnight UT). */
double hijri_to_jd(hijri_date_t date);

/* Month name string (1-12). Returns "?" for invalid. */
const char *hijri_month_name(int month);

/* Is this a leap year in the 30-year cycle?
 * Leap years: 2, 5, 7, 10, 13, 16, 18, 21, 24, 26, 29. */
bool hijri_is_leap(int year);

/* Number of days in a Hijri month (29 or 30).
 * Returns 0 for invalid month. */
int hijri_month_days(int year, int month);

/* Total days in a Hijri year (354 or 355). */
int hijri_year_days(int year);

/* Day of year (1-354/355). */
int hijri_day_of_year(hijri_date_t date);

/* Format: "15 Ramadan 1446". */
void hijri_fmt(hijri_date_t date, char *buf, size_t sz);

#endif /* TIME_HIJRI_H */
