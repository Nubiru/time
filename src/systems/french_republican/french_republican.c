/* french_republican.c — French Republican (Revolutionary) calendar implementation
 *
 * 12 months x 30 days = 360 + Sansculottides (5 or 6 complementary days).
 * Epoch: 1 Vendemiaire An I = September 22, 1792 CE = JD 2375839.5.
 * Leap year: Romme rule — Gregorian rule applied to (year + 1).
 *
 * Sources: Reingold & Dershowitz, "Calendrical Calculations" (2018).
 *          Gilbert Romme (calendar designer, leap year proposal). */

#include "french_republican.h"
#include <math.h>

/* Epoch: JD of 1 Vendemiaire An I (September 22, 1792 at midnight UT) */
static const double FR_EPOCH_JD = 2375839.5;

/* Number of regular days in 12 months */
static const int REGULAR_DAYS = 360; /* 12 * 30 */

static const char *MONTH_NAMES[] = {
    "Sansculottides",  /* 0 */
    "Vendemiaire",     /* 1 */
    "Brumaire",        /* 2 */
    "Frimaire",        /* 3 */
    "Nivose",          /* 4 */
    "Pluviose",        /* 5 */
    "Ventose",         /* 6 */
    "Germinal",        /* 7 */
    "Floreal",         /* 8 */
    "Prairial",        /* 9 */
    "Messidor",        /* 10 */
    "Thermidor",       /* 11 */
    "Fructidor"        /* 12 */
};

static const char *MONTH_MEANINGS[] = {
    "",                /* 0 — Sansculottides has no seasonal meaning */
    "Grape Harvest",   /* 1 */
    "Fog",             /* 2 */
    "Frost",           /* 3 */
    "Snow",            /* 4 */
    "Rain",            /* 5 */
    "Wind",            /* 6 */
    "Germination",     /* 7 */
    "Flower",          /* 8 */
    "Meadow",          /* 9 */
    "Harvest",         /* 10 */
    "Heat",            /* 11 */
    "Fruit"            /* 12 */
};

static const char *SANSCULOTTIDE_NAMES[] = {
    "?",               /* 0 — invalid */
    "Vertu",           /* 1 — Virtue */
    "Genie",           /* 2 — Genius */
    "Travail",         /* 3 — Labour */
    "Opinion",         /* 4 — Opinion */
    "Recompenses",     /* 5 — Rewards */
    "Revolution"       /* 6 — Revolution (leap day only) */
};

static const char *DECADE_DAY_NAMES[] = {
    "?",               /* 0 — invalid */
    "Primidi",         /* 1 */
    "Duodi",           /* 2 */
    "Tridi",           /* 3 */
    "Quartidi",        /* 4 */
    "Quintidi",        /* 5 */
    "Sextidi",         /* 6 */
    "Septidi",         /* 7 */
    "Octidi",          /* 8 */
    "Nonidi",          /* 9 */
    "Decadi"           /* 10 */
};

/* Internal: Check if a Gregorian year is a leap year */
static bool gregorian_is_leap(int gy)
{
    return (gy % 4 == 0 && gy % 100 != 0) || (gy % 400 == 0);
}

bool fr_is_leap_year(int year)
{
    /* Romme rule: apply Gregorian leap year rule to (year + 1).
     * This matches historical equinox-based leap years:
     * An III (3+1=4), An VII (7+1=8), An XI (11+1=12), etc. */
    int gy = year + 1;
    return gregorian_is_leap(gy);
}

int fr_year_days(int year)
{
    return fr_is_leap_year(year) ? 366 : 365;
}

int fr_month_days(int year, int month)
{
    if (month == 0) {
        return fr_is_leap_year(year) ? 6 : 5;
    }
    return 30;
}

const char *fr_month_name(int month)
{
    if (month < 0 || month > 12) {
        return "?";
    }
    return MONTH_NAMES[month];
}

const char *fr_month_meaning(int month)
{
    if (month < 0 || month > 12) {
        return "?";
    }
    return MONTH_MEANINGS[month];
}

const char *fr_sansculottide_name(int day)
{
    if (day < 1 || day > 6) {
        return "?";
    }
    return SANSCULOTTIDE_NAMES[day];
}

const char *fr_decade_day_name(int day)
{
    if (day < 1 || day > 10) {
        return "?";
    }
    return DECADE_DAY_NAMES[day];
}

/* Internal: JD of 1 Vendemiaire for a given Republican year.
 * Sums year lengths from epoch. For efficiency, uses the known
 * epoch and accumulated day counts. */
static double year_start_jd(int year)
{
    /* For years close to epoch, direct summation is fast enough.
     * The calendar was only officially used for 14 years, but
     * we support arbitrary years for mathematical completeness. */
    double jd = FR_EPOCH_JD;
    if (year > 1) {
        for (int y = 1; y < year; y++) {
            jd += fr_year_days(y);
        }
    }
    return jd;
}

double fr_to_jd(fr_date_t date)
{
    double yr_start = year_start_jd(date.year);

    if (date.month == 0) {
        /* Sansculottides: after 360 regular days */
        return yr_start + REGULAR_DAYS + (date.day - 1);
    }
    /* Regular month: (month-1)*30 + (day-1) days from year start */
    return yr_start + (date.month - 1) * 30 + (date.day - 1);
}

fr_date_t fr_from_jd(double jd)
{
    fr_date_t result;

    /* Estimate year from days since epoch */
    double days_since = jd - FR_EPOCH_JD;
    int year = (int)floor(days_since / 365.2425) + 1;
    if (year < 1) {
        year = 1;
    }

    /* Adjust: ensure year_start_jd(year) <= jd */
    double yr_start = year_start_jd(year);
    if (jd < yr_start) {
        year--;
        yr_start = year_start_jd(year);
    }
    /* Check if we undershot */
    double yr_next = year_start_jd(year + 1);
    if (jd >= yr_next) {
        year++;
        yr_start = yr_next;
    }

    result.year = year;

    /* Day of year (0-indexed) */
    int doy = (int)floor(jd - yr_start + 0.5);

    if (doy >= REGULAR_DAYS) {
        /* Sansculottides */
        result.month = 0;
        result.day = doy - REGULAR_DAYS + 1;
    } else {
        /* Regular month */
        result.month = (doy / 30) + 1;
        result.day = (doy % 30) + 1;
    }

    return result;
}

int fr_decade_day(fr_date_t date)
{
    if (date.month == 0) {
        /* Sansculottides are outside the decade system */
        return 0;
    }
    return ((date.day - 1) % 10) + 1;
}

fr_decimal_time_t fr_decimal_time(double day_fraction)
{
    fr_decimal_time_t dt;

    /* Clamp to valid range */
    if (day_fraction < 0.0) {
        day_fraction = 0.0;
    }
    if (day_fraction >= 1.0) {
        day_fraction = 0.99999;
    }

    int total = (int)(day_fraction * 100000.0);

    dt.hours = total / 10000;
    dt.minutes = (total % 10000) / 100;
    dt.seconds = total % 100;

    return dt;
}

double fr_decimal_to_standard(fr_decimal_time_t dt)
{
    int total = dt.hours * 10000 + dt.minutes * 100 + dt.seconds;
    return (double)total / 100000.0;
}

bool fr_is_sansculottide(fr_date_t date)
{
    return date.month == 0;
}

int fr_ce_year_approx(int rep_year)
{
    return rep_year + 1791;
}

int fr_rep_year_approx(int ce_year)
{
    return ce_year - 1791;
}
