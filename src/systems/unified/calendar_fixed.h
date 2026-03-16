/* calendar_fixed.h — Shared helpers for fixed-year (12x30 + epagomenal) calendars
 *
 * Two variants:
 *   Non-leap (365 days always): Egyptian, Zoroastrian
 *   Leap-4 (leap when year%4==3): Coptic, Ethiopian
 *
 * Sources: Dershowitz & Reingold, "Calendrical Calculations" (2018). */

#ifndef TIME_CALENDAR_FIXED_H
#define TIME_CALENDAR_FIXED_H

/* Generic date for fixed-year calendars (12 x 30 + epagomenal). */
typedef struct {
    int year;
    int month;   /* 1-12 regular, 13 = epagomenal */
    int day;     /* 1-30 for months 1-12, 1-5/6 for month 13 */
} cf_date_t;

/* === Non-leap fixed calendars (365 days always) ===
 * Used by: Egyptian, Zoroastrian */

/* Convert JD to date in a simple 365-day fixed calendar. */
cf_date_t cf_fixed_from_jd(double jd, double epoch_jd);

/* Convert date back to JD. */
double cf_fixed_to_jd(cf_date_t date, double epoch_jd);

/* === Leap-4 fixed calendars (leap when year % 4 == 3) ===
 * Used by: Coptic, Ethiopian */

/* Convert JD to date with leap year support. */
cf_date_t cf_leap4_from_jd(double jd, double epoch_jd);

/* Convert date back to JD with leap year support. */
double cf_leap4_to_jd(cf_date_t date, double epoch_jd);

/* === Shared utilities === */

/* Day of year from month/day (1-365 or 1-366). */
int cf_day_of_year(int month, int day);

/* Is this a leap year? (year % 4 == 3) */
int cf_is_leap4(int year);

/* Year days (365 or 366 for leap). */
int cf_year_days_leap4(int year);

/* Month days: 30 for months 1-12, 5 or 6 for month 13. */
int cf_month_days(int year, int month, int has_leap);

#endif /* TIME_CALENDAR_FIXED_H */
