/* bahai.h — Baha'i (Badi') calendar
 *
 * Pure functions for bidirectional conversion between Julian Day
 * and the Baha'i calendar. 19 months of 19 days + Ayyam-i-Ha
 * intercalary period. Epoch: Naw-Ruz 1 BE = March 20, 1844 CE.
 *
 * Month numbering: 1-19 for regular months, 0 for Ayyam-i-Ha.
 * Leap year follows Gregorian rule applied to (year + 1844). */

#ifndef TIME_BAHAI_H
#define TIME_BAHAI_H

#include <stdbool.h>

/* Baha'i date. month=0 means Ayyam-i-Ha intercalary days. */
typedef struct {
    int year;      /* Baha'i Era year (1 = 1844 CE) */
    int month;     /* 1-19 for regular months, 0 for Ayyam-i-Ha */
    int day;       /* 1-19 for months, 1-4/5 for Ayyam-i-Ha */
} bahai_date_t;

typedef enum {
    BAHAI_HOLY_NONE = 0,
    BAHAI_HOLY_NAW_RUZ,
    BAHAI_HOLY_RIDVAN_1,
    BAHAI_HOLY_RIDVAN_9,
    BAHAI_HOLY_RIDVAN_12,
    BAHAI_HOLY_BAB_DECLARATION,
    BAHAI_HOLY_BAHAULLAH_ASCENSION,
    BAHAI_HOLY_BAB_MARTYRDOM,
    BAHAI_HOLY_COUNT
} bahai_holy_day_t;

/* Convert Julian Day to Baha'i date. */
bahai_date_t bahai_from_jd(double jd);

/* Convert Baha'i date to Julian Day (midnight UT). */
double bahai_to_jd(bahai_date_t date);

/* Month name (1-19). Returns "Ayyam-i-Ha" for 0, "?" for invalid. */
const char *bahai_month_name(int month);

/* Month meaning/translation (1-19). Returns "" for 0, "?" for invalid. */
const char *bahai_month_meaning(int month);

/* Days in a given month. Always 19 for months 1-19.
 * 4 or 5 for month 0 (Ayyam-i-Ha) depending on leap year. */
int bahai_month_days(int year, int month);

/* Check if a Baha'i year is a leap year. */
bool bahai_is_leap_year(int year);

/* Total days in a year (365 or 366). */
int bahai_year_days(int year);

/* Number of Ayyam-i-Ha days in a year (4 or 5). */
int bahai_ayyam_i_ha_days(int year);

/* Vahid number (1-indexed, each Vahid = 19 years). */
int bahai_vahid(int year);

/* Kull-i-Shay' number (1-indexed, each = 361 years). */
int bahai_kull_i_shay(int year);

/* Year within current Vahid (1-19). */
int bahai_year_in_vahid(int year);

/* Detect holy day for a given Baha'i date. */
bahai_holy_day_t bahai_holy_day(bahai_date_t date);

/* Holy day name string. Returns "" for NONE, "?" for invalid. */
const char *bahai_holy_day_name(bahai_holy_day_t day);

/* Check if a date falls in Ayyam-i-Ha. */
bool bahai_is_ayyam_i_ha(bahai_date_t date);

#endif /* TIME_BAHAI_H */
