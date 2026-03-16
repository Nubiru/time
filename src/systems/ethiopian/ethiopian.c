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
#include "../unified/calendar_fixed.h"
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
    return cf_is_leap4(year) != 0;
}

double ethiopian_to_jd(ethiopian_date_t date)
{
    cf_date_t cf = { date.year, date.month, date.day };
    return cf_leap4_to_jd(cf, ETHIOPIAN_EPOCH_JD);
}

ethiopian_date_t ethiopian_from_jd(double jd)
{
    ethiopian_date_t result;
    cf_date_t cf = cf_leap4_from_jd(jd, ETHIOPIAN_EPOCH_JD);
    result.year = cf.year;
    result.month = cf.month;
    result.day = cf.day;
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
    return cf_month_days(year, month, 1);
}

int ethiopian_year_days(int year)
{
    return cf_year_days_leap4(year);
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
    return cf_day_of_year(date.month, date.day);
}
