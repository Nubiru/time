/* myanmar.c — Myanmar (Burmese) lunisolar calendar implementation
 *
 * Pure arithmetic calendar with lunisolar intercalation.
 * Myanmar Era (ME) epoch: 22 March 638 CE (Julian), JD ~1954168.050623
 * Solar year from Surya Siddhanta: 1577917828 / 4320000 days
 * 19-year Metonic-like cycle for watat (intercalary) year detection.
 *
 * Sources: Yan Naing Aye, "Myanmar Calendar Algorithm"
 *          Surya Siddhanta (traditional solar year constant) */

#include "myanmar.h"
#include <math.h>

/* Myanmar Era epoch: JD of 22 March 638 CE (Julian) */
static const double MYANMAR_EPOCH_JD = 1954168.050623;

/* Surya Siddhanta solar year: 1577917828 / 4320000 days */
static const double MYANMAR_SOLAR_YEAR = 1577917828.0 / 4320000.0;

/* Gregorian year offset for ME 0 */
static const int MYANMAR_GREGORIAN_OFFSET = 638;

/* Thingyan festival duration in days */
static const int THINGYAN_DAYS = 4;

/* Month names (indexed by myanmar_month_t) */
static const char *MONTH_NAMES[] = {
    "Tagu",         /* MY_TAGU */
    "Kason",        /* MY_KASON */
    "Nayon",        /* MY_NAYON */
    "Waso",         /* MY_WASO */
    "Second Waso",  /* MY_SECOND_WASO */
    "Wagaung",      /* MY_WAGAUNG */
    "Tawthalin",    /* MY_TAWTHALIN */
    "Thadingyut",   /* MY_THADINGYUT */
    "Tazaungmon",   /* MY_TAZAUNGMON */
    "Nadaw",        /* MY_NADAW */
    "Pyatho",       /* MY_PYATHO */
    "Tabodwe",      /* MY_TABODWE */
    "Tabaung"       /* MY_TABAUNG */
};

/* Year type names */
static const char *YEAR_TYPE_NAMES[] = {
    "Common",       /* MY_YEAR_COMMON */
    "Small Watat",  /* MY_YEAR_SMALL_WATAT */
    "Big Watat"     /* MY_YEAR_BIG_WATAT */
};

/* Base month lengths (common year):
 * Tagu=29, Kason=30, Nayon=29, Waso=30, SecondWaso=0,
 * Wagaung=29, Tawthalin=30, Thadingyut=29, Tazaungmon=30,
 * Nadaw=29, Pyatho=30, Tabodwe=29, Tabaung=30 */
static const int BASE_MONTH_DAYS[] = {
    29, 30, 29, 30, 0,
    29, 30, 29, 30,
    29, 30, 29, 30
};

/* Watat positions in a 19-year cycle (0-indexed) */
static const int WATAT_POSITIONS[] = { 2, 5, 7, 10, 13, 15, 18 };
static const int WATAT_COUNT = 7;

/* Big watat positions within the cycle */
static const int BIG_WATAT_POSITIONS[] = { 7, 18 };
static const int BIG_WATAT_COUNT = 2;

/* Check if a cycle position is in a given set */
static bool position_in_set(int pos, const int *set, int set_size)
{
    for (int i = 0; i < set_size; i++) {
        if (set[i] == pos) {
            return true;
        }
    }
    return false;
}

/* New Year JD for a given ME year */
static double new_year_jd(int me_year)
{
    return MYANMAR_EPOCH_JD + MYANMAR_SOLAR_YEAR * me_year;
}

myanmar_year_type_t myanmar_year_type(int me_year)
{
    int pos = me_year % 19;
    if (pos < 0) {
        pos += 19;
    }

    if (!position_in_set(pos, WATAT_POSITIONS, WATAT_COUNT)) {
        return MY_YEAR_COMMON;
    }

    if (position_in_set(pos, BIG_WATAT_POSITIONS, BIG_WATAT_COUNT)) {
        return MY_YEAR_BIG_WATAT;
    }

    return MY_YEAR_SMALL_WATAT;
}

bool myanmar_is_watat(int me_year)
{
    return myanmar_year_type(me_year) != MY_YEAR_COMMON;
}

const char *myanmar_month_name(myanmar_month_t month)
{
    if (month < 0 || month >= MY_MONTH_COUNT) {
        return "?";
    }
    return MONTH_NAMES[month];
}

const char *myanmar_year_type_name(myanmar_year_type_t type)
{
    if (type < 0 || type >= MY_YEAR_TYPE_COUNT) {
        return "?";
    }
    return YEAR_TYPE_NAMES[type];
}

int myanmar_month_days(int me_year, myanmar_month_t month)
{
    if (month < 0 || month >= MY_MONTH_COUNT) {
        return 0;
    }

    myanmar_year_type_t yt = myanmar_year_type(me_year);

    /* Second Waso: 30 days in watat years, 0 (skipped) otherwise */
    if (month == MY_SECOND_WASO) {
        return (yt != MY_YEAR_COMMON) ? 30 : 0;
    }

    /* Nayon: 30 in big watat, 29 otherwise */
    if (month == MY_NAYON) {
        return (yt == MY_YEAR_BIG_WATAT) ? 30 : 29;
    }

    return BASE_MONTH_DAYS[month];
}

int myanmar_year_days(int me_year)
{
    myanmar_year_type_t yt = myanmar_year_type(me_year);
    switch (yt) {
        case MY_YEAR_BIG_WATAT:   return 385;
        case MY_YEAR_SMALL_WATAT: return 384;
        default:                  return 354;
    }
}

int myanmar_to_gregorian_year(int me_year)
{
    return me_year + MYANMAR_GREGORIAN_OFFSET;
}

myanmar_thingyan_t myanmar_thingyan(int me_year)
{
    myanmar_thingyan_t result;
    result.new_year_jd = new_year_jd(me_year);
    result.num_days = THINGYAN_DAYS;
    result.akyo_jd = result.new_year_jd - THINGYAN_DAYS;
    result.akya_jd = result.new_year_jd - 1.0;
    return result;
}

int myanmar_day_of_year(myanmar_date_t date)
{
    int doy = 0;
    for (int m = MY_TAGU; m < (int)date.month; m++) {
        doy += myanmar_month_days(date.year, (myanmar_month_t)m);
    }
    doy += date.day;
    return doy;
}

double myanmar_to_jd(myanmar_date_t date)
{
    double ny_jd = new_year_jd(date.year);
    int doy = myanmar_day_of_year(date);
    return ny_jd + (double)(doy - 1);
}

myanmar_date_t myanmar_from_jd(double jd)
{
    myanmar_date_t result;

    /* Approximate year */
    int approx_year = (int)floor((jd - MYANMAR_EPOCH_JD) / MYANMAR_SOLAR_YEAR);

    /* Adjust: check if jd falls before this year's New Year */
    double ny = new_year_jd(approx_year);
    if (jd < ny) {
        approx_year--;
        ny = new_year_jd(approx_year);
    }

    /* Also check if jd falls in the next year */
    double ny_next = new_year_jd(approx_year + 1);
    if (jd >= ny_next) {
        approx_year++;
        ny = ny_next;
    }

    result.year = approx_year;
    result.is_watat_year = myanmar_is_watat(approx_year);

    /* Day offset from New Year (0-based) */
    int day_offset = (int)floor(jd - ny);

    /* Walk through months to find month and day */
    int remaining = day_offset;
    result.month = MY_TAGU;
    result.day = 1;

    for (int m = MY_TAGU; m < MY_MONTH_COUNT; m++) {
        int mdays = myanmar_month_days(approx_year, (myanmar_month_t)m);
        if (mdays == 0) {
            continue; /* Skip Second Waso in non-watat years */
        }
        if (remaining < mdays) {
            result.month = (myanmar_month_t)m;
            result.day = remaining + 1;
            return result;
        }
        remaining -= mdays;
    }

    /* Fallback: last day of last month */
    result.month = MY_TABAUNG;
    result.day = 30;
    return result;
}

int myanmar_year(double jd)
{
    myanmar_date_t d = myanmar_from_jd(jd);
    return d.year;
}
