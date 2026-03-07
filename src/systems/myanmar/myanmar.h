/* myanmar.h — Myanmar (Burmese) lunisolar calendar
 * Pure functions for bidirectional conversion between Julian Day
 * and the Myanmar calendar.
 *
 * Myanmar Era (ME) epoch: 22 March 638 CE (Julian), JD ~1954168.050623
 * Solar year (Surya Siddhanta) = 1577917828 / 4320000 ~= 365.2587565 days
 *
 * Three year types:
 *   Common    = 354 days (12 months)
 *   Small watat = 384 days (13 months, intercalary Second Waso)
 *   Big watat   = 385 days (13 months + extra day in Nayon)
 *
 * 19-year Metonic-like cycle for intercalation.
 * Watat positions in cycle (0-indexed): 2, 5, 7, 10, 13, 15, 18
 * Big watat positions: 7, 18
 *
 * Source: Yan Naing Aye, Myanmar Calendar Algorithm */

#ifndef TIME_MYANMAR_H
#define TIME_MYANMAR_H

#include <stdbool.h>

/* Myanmar year type */
typedef enum {
    MY_YEAR_COMMON = 0,      /* 354 days (12 months) */
    MY_YEAR_SMALL_WATAT,     /* 384 days (13 months, intercalary Waso) */
    MY_YEAR_BIG_WATAT,       /* 385 days (13 months + extra day in Nayon) */
    MY_YEAR_TYPE_COUNT
} myanmar_year_type_t;

/* Myanmar months */
typedef enum {
    MY_TAGU = 0,        /* Month 1 */
    MY_KASON,           /* Month 2 */
    MY_NAYON,           /* Month 3 */
    MY_WASO,            /* Month 4 (first Waso) */
    MY_SECOND_WASO,     /* Intercalary month (watat years only) */
    MY_WAGAUNG,         /* Month 5 */
    MY_TAWTHALIN,       /* Month 6 */
    MY_THADINGYUT,      /* Month 7 */
    MY_TAZAUNGMON,      /* Month 8 */
    MY_NADAW,           /* Month 9 */
    MY_PYATHO,          /* Month 10 */
    MY_TABODWE,         /* Month 11 */
    MY_TABAUNG,         /* Month 12 */
    MY_MONTH_COUNT
} myanmar_month_t;

/* Myanmar date */
typedef struct {
    int year;            /* Myanmar Era year (ME) */
    myanmar_month_t month;
    int day;             /* 1-29 or 1-30 */
    bool is_watat_year;  /* True if year has intercalary month */
} myanmar_date_t;

/* Thingyan (water festival) dates */
typedef struct {
    double akyo_jd;      /* Thingyan start (akyo day) */
    double akya_jd;      /* Thingyan end (akya day) */
    double new_year_jd;  /* Myanmar New Year (1 Tagu) */
    int num_days;        /* Total festival days */
} myanmar_thingyan_t;

/* Convert Julian Day to Myanmar date. */
myanmar_date_t myanmar_from_jd(double jd);

/* Convert Myanmar date to Julian Day. */
double myanmar_to_jd(myanmar_date_t date);

/* Year type (common, small watat, big watat). */
myanmar_year_type_t myanmar_year_type(int me_year);

/* Is this a watat (intercalary) year? */
bool myanmar_is_watat(int me_year);

/* Month name string. Returns "?" for invalid. */
const char *myanmar_month_name(myanmar_month_t month);

/* Year type name string. */
const char *myanmar_year_type_name(myanmar_year_type_t type);

/* Number of days in a given month for a given year. */
int myanmar_month_days(int me_year, myanmar_month_t month);

/* Total days in a Myanmar year. */
int myanmar_year_days(int me_year);

/* Myanmar year from JD (convenience). */
int myanmar_year(double jd);

/* Thingyan (water festival) dates for a given ME year. */
myanmar_thingyan_t myanmar_thingyan(int me_year);

/* Day of year (1-354/384/385). */
int myanmar_day_of_year(myanmar_date_t date);

/* Convert Myanmar Era year to Gregorian approximate year. */
int myanmar_to_gregorian_year(int me_year);

#endif /* TIME_MYANMAR_H */
