/* zoroastrian.c — Yazdgerdi (Zoroastrian) calendar implementation
 *
 * Pure arithmetic calendar: 12 x 30 + 5 Gatha = 365 days, no leap year.
 * Epoch: JD 1952063.5 = 16 June 632 CE (Julian) = Year 1, 1 Farvardin.
 *
 * Sources: S.H. Taqizadeh, "Various Eras and Calendars Used in the
 * Countries of Islam" (BSOAS, 1937-1940). */

#include "zoroastrian.h"
#include "../unified/calendar_fixed.h"
#include <math.h>

#define ZORO_EPOCH_JD 1952063.5
#define ZORO_YEAR_DAYS 365
#define ZORO_MONTH_DAYS_REGULAR 30
#define ZORO_MONTHS_REGULAR 12
#define ZORO_REGULAR_TOTAL 360  /* 12 * 30 */
#define ZORO_GATHA_DAYS 5

static const char *MONTH_NAMES[] = {
    "Farvardin", "Ordibehesht", "Khordad", "Tir",
    "Amordad", "Shahrivar", "Mehr", "Aban",
    "Azar", "Dey", "Bahman", "Esfand",
    "Gatha"
};

static const char *DAY_NAMES[] = {
    "Hormozd", "Bahman", "Ordibehesht", "Shahrivar", "Sepandarmaz",
    "Khordad", "Amordad", "Dey-be-Azar", "Azar", "Aban",
    "Khorshed", "Mah", "Tir", "Gosh", "Dey-be-Mehr",
    "Mehr", "Sorush", "Rashn", "Farvardin", "Bahram",
    "Ram", "Bad", "Dey-be-Din", "Din", "Ard",
    "Ashtad", "Asman", "Zamyad", "Mahraspand", "Aneran"
};

static const char *GATHA_NAMES[] = {
    "Ahunavad", "Ushtavad", "Spentamad", "Vohukhshathra", "Vahishtoisht"
};

static const char *FESTIVAL_NAMES[] = {
    "",                   /* ZORO_FEST_NONE */
    "Nowruz",             /* ZORO_FEST_NOWRUZ */
    "Maidyozarem",        /* ZORO_FEST_GAHANBAR_1 */
    "Maidyoshem",         /* ZORO_FEST_GAHANBAR_2 */
    "Paitishem",          /* ZORO_FEST_GAHANBAR_3 */
    "Ayathrem",           /* ZORO_FEST_GAHANBAR_4 */
    "Maidyarem",          /* ZORO_FEST_GAHANBAR_5 */
    "Hamaspathmaidyem",   /* ZORO_FEST_GAHANBAR_6 */
    "Yalda",              /* ZORO_FEST_YALDA */
    "Sadeh"               /* ZORO_FEST_SADEH */
};

zoro_date_t zoro_from_jd(double jd)
{
    zoro_date_t result;
    cf_date_t cf = cf_fixed_from_jd(jd, ZORO_EPOCH_JD);
    result.year = cf.year;
    result.month = cf.month;
    result.day = cf.day;
    return result;
}

double zoro_to_jd(zoro_date_t date)
{
    cf_date_t cf = { date.year, date.month, date.day };
    return cf_fixed_to_jd(cf, ZORO_EPOCH_JD);
}

const char *zoro_month_name(int month)
{
    if (month < 1 || month > 13) {
        return "?";
    }
    return MONTH_NAMES[month - 1];
}

const char *zoro_day_name(int day)
{
    if (day < 1 || day > 30) {
        return "?";
    }
    return DAY_NAMES[day - 1];
}

const char *zoro_gatha_name(int gatha_day)
{
    if (gatha_day < 1 || gatha_day > ZORO_GATHA_DAYS) {
        return "?";
    }
    return GATHA_NAMES[gatha_day - 1];
}

zoro_festival_t zoro_festival(zoro_date_t date)
{
    int m = date.month;
    int d = date.day;

    /* Hamaspathmaidyem: all 5 Gatha days */
    if (m == 13) {
        return ZORO_FEST_GAHANBAR_6;
    }

    /* Nowruz: 1 Farvardin */
    if (m == 1 && d == 1) {
        return ZORO_FEST_NOWRUZ;
    }

    /* Maidyozarem: 15 Ordibehesht */
    if (m == 2 && d == 15) {
        return ZORO_FEST_GAHANBAR_1;
    }

    /* Maidyoshem: 15 Tir */
    if (m == 4 && d == 15) {
        return ZORO_FEST_GAHANBAR_2;
    }

    /* Paitishem: 30 Shahrivar */
    if (m == 6 && d == 30) {
        return ZORO_FEST_GAHANBAR_3;
    }

    /* Ayathrem: 30 Mehr */
    if (m == 7 && d == 30) {
        return ZORO_FEST_GAHANBAR_4;
    }

    /* Maidyarem: 20 Dey */
    if (m == 10 && d == 20) {
        return ZORO_FEST_GAHANBAR_5;
    }

    /* Yalda: 1 Dey */
    if (m == 10 && d == 1) {
        return ZORO_FEST_YALDA;
    }

    /* Sadeh: 10 Bahman */
    if (m == 11 && d == 10) {
        return ZORO_FEST_SADEH;
    }

    return ZORO_FEST_NONE;
}

const char *zoro_festival_name(zoro_festival_t fest)
{
    if (fest < 0 || fest >= ZORO_FEST_COUNT) {
        return "?";
    }
    return FESTIVAL_NAMES[fest];
}

int zoro_month_days(int month)
{
    if (month >= 1 && month <= ZORO_MONTHS_REGULAR) {
        return ZORO_MONTH_DAYS_REGULAR;
    }
    if (month == 13) {
        return ZORO_GATHA_DAYS;
    }
    return 0;
}

int zoro_year(double jd)
{
    zoro_date_t d = zoro_from_jd(jd);
    return d.year;
}

bool zoro_is_gatha(zoro_date_t date)
{
    return date.month == 13;
}

int zoro_day_of_year(zoro_date_t date)
{
    return cf_day_of_year(date.month, date.day);
}
