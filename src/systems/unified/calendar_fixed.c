/* calendar_fixed.c — Shared helpers for fixed-year (12x30 + epagomenal) calendars
 *
 * Two variants:
 *   Non-leap (365 days always): Egyptian, Zoroastrian
 *   Leap-4 (leap when year%4==3): Coptic, Ethiopian
 *
 * Sources: Dershowitz & Reingold, "Calendrical Calculations" (2018). */

#include "calendar_fixed.h"
#include <math.h>

#define CF_YEAR_DAYS    365
#define CF_MONTH_DAYS   30
#define CF_MONTHS       12
#define CF_REGULAR_TOTAL 360  /* 12 * 30 */
#define CF_CYCLE_DAYS   1461  /* 4 * 365 + 1 */

/* === Non-leap fixed calendars (365 days always) === */

cf_date_t cf_fixed_from_jd(double jd, double epoch_jd)
{
    cf_date_t result;
    int days_since = (int)floor(jd - epoch_jd);

    int year;
    int day_of_year;

    if (days_since >= 0) {
        year = days_since / CF_YEAR_DAYS + 1;
        day_of_year = days_since % CF_YEAR_DAYS + 1;
    } else {
        /* For negative days_since, C integer division truncates toward zero.
         * We need floor division. */
        year = (days_since + 1) / CF_YEAR_DAYS;
        day_of_year = days_since - (year - 1) * CF_YEAR_DAYS + 1;
        if (day_of_year <= 0) {
            year--;
            day_of_year += CF_YEAR_DAYS;
        }
    }

    result.year = year;

    if (day_of_year <= CF_REGULAR_TOTAL) {
        result.month = (day_of_year - 1) / CF_MONTH_DAYS + 1;
        result.day = (day_of_year - 1) % CF_MONTH_DAYS + 1;
    } else {
        result.month = 13;
        result.day = day_of_year - CF_REGULAR_TOTAL;
    }

    return result;
}

double cf_fixed_to_jd(cf_date_t date, double epoch_jd)
{
    int day_of_year;

    if (date.month <= CF_MONTHS) {
        day_of_year = (date.month - 1) * CF_MONTH_DAYS + date.day;
    } else {
        day_of_year = CF_REGULAR_TOTAL + date.day;
    }

    return epoch_jd + (double)(date.year - 1) * CF_YEAR_DAYS
           + (double)(day_of_year - 1);
}

/* === Leap-4 fixed calendars (leap when year % 4 == 3) === */

cf_date_t cf_leap4_from_jd(double jd, double epoch_jd)
{
    cf_date_t result;
    int days = (int)floor(jd - epoch_jd);

    /* Year approximation accounting for leap pattern offset.
     * Leap years at Y%4==3, so use +1463 constant (Reingold/Dershowitz). */
    int year = (4 * days + 1463) / CF_CYCLE_DAYS;

    int day_in_year = days - (365 * (year - 1) + year / 4) + 1;

    if (day_in_year <= CF_REGULAR_TOTAL) {
        result.month = (day_in_year - 1) / CF_MONTH_DAYS + 1;
        result.day = day_in_year - (result.month - 1) * CF_MONTH_DAYS;
    } else {
        result.month = 13;
        result.day = day_in_year - CF_REGULAR_TOTAL;
    }

    result.year = year;
    return result;
}

double cf_leap4_to_jd(cf_date_t date, double epoch_jd)
{
    return epoch_jd
           + 365 * (date.year - 1)
           + date.year / 4
           + 30 * (date.month - 1)
           + date.day - 1;
}

/* === Shared utilities === */

int cf_day_of_year(int month, int day)
{
    if (month <= CF_MONTHS) {
        return (month - 1) * CF_MONTH_DAYS + day;
    }
    return CF_REGULAR_TOTAL + day;
}

int cf_is_leap4(int year)
{
    return (year % 4) == 3;
}

int cf_year_days_leap4(int year)
{
    return cf_is_leap4(year) ? 366 : 365;
}

int cf_month_days(int year, int month, int has_leap)
{
    if (month >= 1 && month <= CF_MONTHS) {
        return CF_MONTH_DAYS;
    }
    if (month == 13) {
        if (has_leap) {
            return cf_is_leap4(year) ? 6 : 5;
        }
        return 5;  /* Non-leap calendars always have 5 epagomenal days */
    }
    return 0;
}
