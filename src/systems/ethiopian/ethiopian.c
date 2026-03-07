/* ethiopian.c — Ethiopian calendar implementation
 *
 * Pure arithmetic calendar with leap year support.
 * 12 months x 30 days + Pagume (5 or 6 epagomenal days).
 * Leap year when (year % 4) == 3.
 * Epoch: JD 1724220.5 = 29 Aug 8 CE (Julian) = Year 1, 1 Meskerem.
 *
 * Structurally identical to the Coptic calendar but with different epoch
 * (Coptic: JD 1825029.5) and Ge'ez month names.
 *
 * Sources: Dershowitz & Reingold, "Calendrical Calculations" (2018). */

#include "ethiopian.h"
#include <math.h>

#define ETHIOPIAN_EPOCH_JD      1724220.5
#define ETHIOPIAN_REGULAR_DAYS  30
#define ETHIOPIAN_REGULAR_TOTAL 360  /* 12 * 30 */
#define ETHIOPIAN_MONTHS        12
#define ETHIOPIAN_CYCLE_DAYS    1461 /* 4 * 365 + 1 */

static const char *MONTH_NAMES[] = {
    "Meskerem", "Tikimt", "Hidar", "Tahsas", "Tir", "Yekatit",
    "Megabit", "Miyazia", "Ginbot", "Sene", "Hamle", "Nehase",
    "Pagume"
};

static const char *FEAST_NAMES[] = {
    "",                     /* ETH_FEAST_NONE */
    "Enkutatash",           /* ETH_FEAST_ENKUTATASH */
    "Meskel",               /* ETH_FEAST_MESKEL */
    "Genna",                /* ETH_FEAST_GENNA */
    "Timkat",               /* ETH_FEAST_TIMKAT */
    "Fasika"                /* ETH_FEAST_FASIKA */
};

bool ethiopian_is_leap(int year)
{
    return (year % 4) == 3;
}

double ethiopian_to_jd(ethiopian_date_t date)
{
    /* Leap years are Y%4==3 (years 3,7,11...).
     * Leap days before year Y = Y/4 (integer division). */
    return ETHIOPIAN_EPOCH_JD
           + 365 * (date.year - 1)
           + date.year / 4
           + 30 * (date.month - 1)
           + date.day - 1;
}

ethiopian_date_t ethiopian_from_jd(double jd)
{
    ethiopian_date_t result;
    int days = (int)floor(jd - ETHIOPIAN_EPOCH_JD);

    /* Year approximation accounting for leap pattern offset.
     * Leap years at Y%4==3, so use +1463 constant (Reingold/Dershowitz). */
    int year = (4 * days + 1463) / ETHIOPIAN_CYCLE_DAYS;

    int day_in_year = days - (365 * (year - 1) + year / 4) + 1;

    if (day_in_year <= ETHIOPIAN_REGULAR_TOTAL) {
        result.month = (day_in_year - 1) / ETHIOPIAN_REGULAR_DAYS + 1;
        result.day = day_in_year - (result.month - 1) * ETHIOPIAN_REGULAR_DAYS;
    } else {
        result.month = 13;
        result.day = day_in_year - ETHIOPIAN_REGULAR_TOTAL;
    }

    result.year = year;
    return result;
}

const char *ethiopian_month_name(int month)
{
    if (month < 1 || month > 13) {
        return "?";
    }
    return MONTH_NAMES[month - 1];
}

int ethiopian_month_days(int year, int month)
{
    if (month >= 1 && month <= ETHIOPIAN_MONTHS) {
        return ETHIOPIAN_REGULAR_DAYS;
    }
    if (month == 13) {
        return ethiopian_is_leap(year) ? 6 : 5;
    }
    return 0;
}

int ethiopian_year_days(int year)
{
    return ethiopian_is_leap(year) ? 366 : 365;
}

int ethiopian_year(double jd)
{
    ethiopian_date_t d = ethiopian_from_jd(jd);
    return d.year;
}

ethiopian_feast_t ethiopian_feast(ethiopian_date_t date)
{
    int m = date.month;
    int d = date.day;

    /* Enkutatash (New Year): 1 Meskerem */
    if (m == 1 && d == 1) {
        return ETH_FEAST_ENKUTATASH;
    }

    /* Meskel (Finding of True Cross): 17 Meskerem */
    if (m == 1 && d == 17) {
        return ETH_FEAST_MESKEL;
    }

    /* Genna (Christmas): 29 Tahsas (month 4) */
    if (m == 4 && d == 29) {
        return ETH_FEAST_GENNA;
    }

    /* Timkat (Epiphany): 11 Tir (month 5) */
    if (m == 5 && d == 11) {
        return ETH_FEAST_TIMKAT;
    }

    /* Fasika (Easter) is movable (Alexandrian computus) — not implemented */

    return ETH_FEAST_NONE;
}

const char *ethiopian_feast_name(ethiopian_feast_t feast)
{
    if (feast < 0 || feast >= ETH_FEAST_COUNT) {
        return "?";
    }
    return FEAST_NAMES[feast];
}

int ethiopian_day_of_year(ethiopian_date_t date)
{
    if (date.month <= ETHIOPIAN_MONTHS) {
        return (date.month - 1) * ETHIOPIAN_REGULAR_DAYS + date.day;
    }
    return ETHIOPIAN_REGULAR_TOTAL + date.day;
}
