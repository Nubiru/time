/* bahai.c — Baha'i (Badi') calendar implementation
 *
 * 19 months x 19 days = 361 + Ayyam-i-Ha (4 or 5 intercalary days).
 * Epoch: 1 Baha 1 BE = March 20, 1844 CE = JD 2394645.5.
 * Leap year follows Gregorian rule on (year + 1844). */

#include "bahai.h"
#include <math.h>

/* Epoch: JD of 1 Baha 1 BE (March 20, 1844 at midnight UT, Meeus) */
static const double BAHAI_EPOCH_JD = 2394645.5;

/* Number of regular days in 18 months (before Ayyam-i-Ha) */
static const int REGULAR_DAYS_18 = 18 * 19; /* 342 */

static const char *MONTH_NAMES[] = {
    "Ayyam-i-Ha",   /* 0 */
    "Baha",          /* 1 */
    "Jalal",         /* 2 */
    "Jamal",         /* 3 */
    "Azamat",        /* 4 */
    "Nur",           /* 5 */
    "Rahmat",        /* 6 */
    "Kalimat",       /* 7 */
    "Kamal",         /* 8 */
    "Asma",          /* 9 */
    "Izzat",         /* 10 */
    "Mashiyyat",     /* 11 */
    "Ilm",           /* 12 */
    "Qudrat",        /* 13 */
    "Qawl",          /* 14 */
    "Masail",        /* 15 */
    "Sharaf",        /* 16 */
    "Sultan",        /* 17 */
    "Mulk",          /* 18 */
    "Ala"            /* 19 */
};

static const char *MONTH_MEANINGS[] = {
    "",              /* 0 — Ayyam-i-Ha has no attribute meaning */
    "Splendour",     /* 1 */
    "Glory",         /* 2 */
    "Beauty",        /* 3 */
    "Grandeur",      /* 4 */
    "Light",         /* 5 */
    "Mercy",         /* 6 */
    "Words",         /* 7 */
    "Perfection",    /* 8 */
    "Names",         /* 9 */
    "Might",         /* 10 */
    "Will",          /* 11 */
    "Knowledge",     /* 12 */
    "Power",         /* 13 */
    "Speech",        /* 14 */
    "Questions",     /* 15 */
    "Honour",        /* 16 */
    "Sovereignty",   /* 17 */
    "Dominion",      /* 18 */
    "Loftiness"      /* 19 */
};

static const char *HOLY_DAY_NAMES[] = {
    "",                          /* BAHAI_HOLY_NONE */
    "Naw-Ruz",                   /* BAHAI_HOLY_NAW_RUZ */
    "First Day of Ridvan",       /* BAHAI_HOLY_RIDVAN_1 */
    "Ninth Day of Ridvan",       /* BAHAI_HOLY_RIDVAN_9 */
    "Twelfth Day of Ridvan",     /* BAHAI_HOLY_RIDVAN_12 */
    "Declaration of the Bab",    /* BAHAI_HOLY_BAB_DECLARATION */
    "Ascension of Bahaullah",    /* BAHAI_HOLY_BAHAULLAH_ASCENSION */
    "Martyrdom of the Bab"       /* BAHAI_HOLY_BAB_MARTYRDOM */
};

/* Internal: Gregorian to JD (Meeus algorithm, midnight UT).
 * Standard algorithm from "Astronomical Algorithms". */
static double gregorian_to_jd(int year, int month, int day)
{
    int a = (14 - month) / 12;
    int y = year + 4800 - a;
    int m = month + 12 * a - 3;
    int jdn = day + (153 * m + 2) / 5 + 365 * y + y / 4 - y / 100 + y / 400 - 32045;
    return (double)jdn - 0.5;
}

/* Internal: Check if a Gregorian year is a leap year */
static bool gregorian_is_leap(int gy)
{
    return (gy % 4 == 0 && gy % 100 != 0) || (gy % 400 == 0);
}

/* Internal: JD of Naw-Ruz (1 Baha) for a given Baha'i year */
static double naw_ruz_jd(int year)
{
    int gy = year + 1843;
    return gregorian_to_jd(gy, 3, 20);
}

bool bahai_is_leap_year(int year)
{
    /* Ayyam-i-Ha falls in the Gregorian year AFTER Naw-Ruz.
     * Naw-Ruz of year Y is March 20 of GY = Y+1843.
     * Ayyam-i-Ha falls in Feb/March of GY+1 = Y+1844.
     * So check if Y+1844 is a Gregorian leap year. */
    int gy = year + 1844;
    return gregorian_is_leap(gy);
}

int bahai_ayyam_i_ha_days(int year)
{
    return bahai_is_leap_year(year) ? 5 : 4;
}

int bahai_year_days(int year)
{
    return bahai_is_leap_year(year) ? 366 : 365;
}

int bahai_month_days(int year, int month)
{
    if (month == 0) {
        return bahai_ayyam_i_ha_days(year);
    }
    return 19;
}

const char *bahai_month_name(int month)
{
    if (month < 0 || month > 19) {
        return "?";
    }
    return MONTH_NAMES[month];
}

const char *bahai_month_meaning(int month)
{
    if (month < 0 || month > 19) {
        return "?";
    }
    return MONTH_MEANINGS[month];
}

double bahai_to_jd(bahai_date_t date)
{
    double nr = naw_ruz_jd(date.year);

    if (date.month == 0) {
        /* Ayyam-i-Ha: starts after month 18 */
        return nr + REGULAR_DAYS_18 + (date.day - 1);
    }
    if (date.month >= 1 && date.month <= 18) {
        return nr + (date.month - 1) * 19 + (date.day - 1);
    }
    /* month == 19: after Ayyam-i-Ha */
    int ayyam = bahai_ayyam_i_ha_days(date.year);
    return nr + REGULAR_DAYS_18 + ayyam + (date.day - 1);
}

bahai_date_t bahai_from_jd(double jd)
{
    bahai_date_t result;

    /* Estimate year */
    int year = (int)floor((jd - BAHAI_EPOCH_JD) / 365.2422) + 1;
    if (year < 1) {
        year = 1;
    }

    /* Adjust: ensure naw_ruz_jd(year) <= jd */
    double nr = naw_ruz_jd(year);
    if (jd < nr) {
        year--;
        nr = naw_ruz_jd(year);
    }
    /* Also check if we undershot */
    double nr_next = naw_ruz_jd(year + 1);
    if (jd >= nr_next) {
        year++;
        nr = nr_next;
    }

    result.year = year;

    /* Day of year (1-indexed) */
    int doy = (int)floor(jd - nr + 0.5) + 1;
    int ayyam = bahai_ayyam_i_ha_days(year);

    if (doy <= REGULAR_DAYS_18) {
        /* Months 1-18 */
        result.month = ((doy - 1) / 19) + 1;
        result.day = ((doy - 1) % 19) + 1;
    } else if (doy <= REGULAR_DAYS_18 + ayyam) {
        /* Ayyam-i-Ha */
        result.month = 0;
        result.day = doy - REGULAR_DAYS_18;
    } else {
        /* Month 19 (Ala) */
        result.month = 19;
        result.day = doy - REGULAR_DAYS_18 - ayyam;
    }

    return result;
}

int bahai_vahid(int year)
{
    return ((year - 1) / 19) + 1;
}

int bahai_kull_i_shay(int year)
{
    return ((year - 1) / 361) + 1;
}

int bahai_year_in_vahid(int year)
{
    return ((year - 1) % 19) + 1;
}

bahai_holy_day_t bahai_holy_day(bahai_date_t date)
{
    if (date.month == 1 && date.day == 1) {
        return BAHAI_HOLY_NAW_RUZ;
    }
    if (date.month == 2 && date.day == 13) {
        return BAHAI_HOLY_RIDVAN_1;
    }
    if (date.month == 4 && date.day == 2) {
        return BAHAI_HOLY_RIDVAN_9;
    }
    if (date.month == 4 && date.day == 5) {
        return BAHAI_HOLY_RIDVAN_12;
    }
    if (date.month == 4 && date.day == 8) {
        return BAHAI_HOLY_BAB_DECLARATION;
    }
    if (date.month == 4 && date.day == 13) {
        return BAHAI_HOLY_BAHAULLAH_ASCENSION;
    }
    if (date.month == 6 && date.day == 17) {
        return BAHAI_HOLY_BAB_MARTYRDOM;
    }
    return BAHAI_HOLY_NONE;
}

const char *bahai_holy_day_name(bahai_holy_day_t day)
{
    if (day < 0 || day >= BAHAI_HOLY_COUNT) {
        return "?";
    }
    return HOLY_DAY_NAMES[day];
}

bool bahai_is_ayyam_i_ha(bahai_date_t date)
{
    return date.month == 0;
}
