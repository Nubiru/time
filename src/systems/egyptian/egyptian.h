/* egyptian.h — Egyptian civil calendar
 * Pure functions for bidirectional conversion between Julian Day
 * and the Egyptian civil calendar (Nabonassar era).
 *
 * 12 months x 30 days + 5 epagomenal days = 365 days, no leap year.
 * Epoch: JD 1448638.0 = 26 Feb 747 BCE (Julian) = Year 1, 1 Thoth.
 *
 * Three seasons of 4 months each:
 *   Akhet (Inundation): months 1-4
 *   Peret (Emergence):  months 5-8
 *   Shemu (Harvest):    months 9-12
 *
 * Sothic cycle: 1461 civil years for heliacal rising of Sirius
 * to drift through the entire calendar. */

#ifndef TIME_EGYPTIAN_H
#define TIME_EGYPTIAN_H

#include <stdbool.h>

typedef struct {
    int year;    /* Nabonassar year */
    int month;   /* 1-12 (Thoth=1 through Mesore=12), 13=Epagomenal days */
    int day;     /* 1-30 for months 1-12, 1-5 for month 13 */
} egypt_date_t;

typedef enum {
    EGYPT_SEASON_AKHET = 0,  /* Inundation (months 1-4) */
    EGYPT_SEASON_PERET,      /* Emergence/Growth (months 5-8) */
    EGYPT_SEASON_SHEMU,      /* Harvest (months 9-12) */
    EGYPT_SEASON_EPAG,       /* Epagomenal days (month 13) */
    EGYPT_SEASON_COUNT
} egypt_season_t;

#define EGYPT_SOTHIC_CYCLE_YEARS 1461

typedef struct {
    int cycle_year;       /* year within current Sothic cycle (0-1460) */
    double progress;      /* 0.0 to 1.0 progress through cycle */
    int drift_days;       /* approximate days Sirius has drifted from anchor */
} egypt_sothic_t;

/* Convert Julian Day to Egyptian date. */
egypt_date_t egypt_from_jd(double jd);

/* Convert Egyptian date to Julian Day (midnight UT). */
double egypt_to_jd(egypt_date_t date);

/* Month name string (1-12 or 13 for Epagomenal). Returns "?" for invalid. */
const char *egypt_month_name(int month);

/* Season for a given month number (1-13). */
egypt_season_t egypt_season(int month);

/* Season name string. Returns "?" for invalid. */
const char *egypt_season_name(egypt_season_t season);

/* Number of days in a month (30 for 1-12, 5 for 13, 0 for invalid). */
int egypt_month_days(int month);

/* Day of year (1-365) for a given Egyptian date. */
int egypt_day_of_year(egypt_date_t date);

/* True if date is in the epagomenal period (month 13). */
bool egypt_is_epagomenal(egypt_date_t date);

/* Sothic cycle position for a given Julian Day. */
egypt_sothic_t egypt_sothic_position(double jd);

/* Month number within its season (1-4), or 0 for epagomenal/invalid. */
int egypt_season_month(int month);

/* Convenience: extract Nabonassar year from a Julian Day. */
int egypt_year(double jd);

#endif /* TIME_EGYPTIAN_H */
