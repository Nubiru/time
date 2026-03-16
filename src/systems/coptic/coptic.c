/* coptic.c — Coptic (Anno Martyrum) calendar implementation
 *
 * Pure arithmetic calendar with leap year support.
 * 12 months x 30 days + Nasie (5 or 6 epagomenal days).
 * Leap year when (year % 4) == 3.
 * Epoch: JD 1825029.5 = 29 Aug 284 CE (Julian) = AM 1, 1 Thout.
 *
 * Sources: Dershowitz & Reingold, "Calendrical Calculations" (2018).
 *          O.F.A. Meinardus, "Two Thousand Years of Coptic Christianity" (1999). */

#include "coptic.h"
#include "../unified/calendar_fixed.h"
#include <math.h>

#define COPTIC_EPOCH_JD      1825029.5
#define COPTIC_REGULAR_DAYS  30
#define COPTIC_REGULAR_TOTAL 360  /* 12 * 30 */
#define COPTIC_MONTHS        12
#define COPTIC_CYCLE_DAYS    1461 /* 4 * 365 + 1 */

static const char *MONTH_NAMES[] = {
    "Thout", "Paopi", "Hathor", "Koiak", "Tobi", "Meshir",
    "Paremhat", "Parmouti", "Pashons", "Paoni", "Epip", "Mesori",
    "Nasie"
};

static const char *FEAST_NAMES[] = {
    "",                     /* COPTIC_FEAST_NONE */
    "Nayrouz",              /* COPTIC_FEAST_NAYROUZ */
    "Feast of the Cross",   /* COPTIC_FEAST_CROSS */
    "Christmas",            /* COPTIC_FEAST_CHRISTMAS */
    "Epiphany",             /* COPTIC_FEAST_EPIPHANY */
    "Annunciation",         /* COPTIC_FEAST_ANNUNCIATION */
    "Easter"                /* COPTIC_FEAST_EASTER */
};

bool coptic_is_leap(int year)
{
    return cf_is_leap4(year) != 0;
}

double coptic_to_jd(coptic_date_t date)
{
    cf_date_t cf = { date.year, date.month, date.day };
    return cf_leap4_to_jd(cf, COPTIC_EPOCH_JD);
}

coptic_date_t coptic_from_jd(double jd)
{
    coptic_date_t result;
    cf_date_t cf = cf_leap4_from_jd(jd, COPTIC_EPOCH_JD);
    result.year = cf.year;
    result.month = cf.month;
    result.day = cf.day;
    return result;
}

const char *coptic_month_name(int month)
{
    if (month < 1 || month > 13) {
        return "?";
    }
    return MONTH_NAMES[month - 1];
}

int coptic_month_days(int year, int month)
{
    return cf_month_days(year, month, 1);
}

int coptic_year_days(int year)
{
    return cf_year_days_leap4(year);
}

int coptic_year(double jd)
{
    coptic_date_t d = coptic_from_jd(jd);
    return d.year;
}

coptic_feast_t coptic_feast(coptic_date_t date)
{
    int m = date.month;
    int d = date.day;

    /* Nayrouz: 1 Thout (Coptic New Year) */
    if (m == 1 && d == 1) {
        return COPTIC_FEAST_NAYROUZ;
    }

    /* Feast of the Cross: 17 Thout */
    if (m == 1 && d == 17) {
        return COPTIC_FEAST_CROSS;
    }

    /* Christmas: 29 Koiak */
    if (m == 4 && d == 29) {
        return COPTIC_FEAST_CHRISTMAS;
    }

    /* Epiphany/Theophany: 11 Tobi */
    if (m == 5 && d == 11) {
        return COPTIC_FEAST_EPIPHANY;
    }

    /* Annunciation: 29 Paremhat */
    if (m == 7 && d == 29) {
        return COPTIC_FEAST_ANNUNCIATION;
    }

    /* Easter is movable (Alexandrian computus) — not implemented */

    return COPTIC_FEAST_NONE;
}

const char *coptic_feast_name(coptic_feast_t feast)
{
    if (feast < 0 || feast >= COPTIC_FEAST_COUNT) {
        return "?";
    }
    return FEAST_NAMES[feast];
}

int coptic_day_of_year(coptic_date_t date)
{
    return cf_day_of_year(date.month, date.day);
}
