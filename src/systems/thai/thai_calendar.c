/* thai_calendar.c -- Thai Solar Calendar implementation
 * Self-contained Gregorian JD math (Meeus, "Astronomical Algorithms" Ch.7).
 * Buddhist Era offset: BE = CE + 543. */

#include "thai_calendar.h"
#include <math.h>

/* ===== Buddhist Era offset ===== */

static const int BE_OFFSET = 543;

/* ===== Internal Gregorian <-> JD (Meeus Ch.7) ===== */

static double gregorian_to_jd_internal(int year, int month, double day)
{
    int y = year;
    int m = month;

    if (m <= 2) {
        y = y - 1;
        m = m + 12;
    }

    int a = (int)floor((double)y / 100.0);
    double b = 2.0 - a + (int)floor((double)a / 4.0);

    return floor(365.25 * (y + 4716))
         + floor(30.6001 * (m + 1))
         + day + b - 1524.5;
}

static void jd_to_gregorian_internal(double jd, int *year, int *month, int *day)
{
    double z_d = floor(jd + 0.5);
    double f = (jd + 0.5) - z_d;
    int z = (int)z_d;

    int a;
    if (z < 2299161) {
        a = z;
    } else {
        int alpha = (int)floor((z - 1867216.25) / 36524.25);
        a = z + 1 + alpha - (int)floor((double)alpha / 4.0);
    }

    int b = a + 1524;
    int c = (int)floor((b - 122.1) / 365.25);
    int d = (int)floor(365.25 * c);
    int e = (int)floor((double)(b - d) / 30.6001);

    double day_frac = (b - d) - (int)floor(30.6001 * e) + f;

    int m;
    if (e < 14) {
        m = e - 1;
    } else {
        m = e - 13;
    }

    int y;
    if (m > 2) {
        y = c - 4716;
    } else {
        y = c - 4715;
    }

    *year = y;
    *month = m;
    *day = (int)day_frac;
}

/* ===== Leap year (Gregorian rules on CE year) ===== */

static bool is_gregorian_leap(int ce_year)
{
    if (ce_year % 4 != 0) return false;
    if (ce_year % 100 != 0) return true;
    return (ce_year % 400 == 0);
}

/* ===== Month names ===== */

static const char *THAI_MONTH_NAMES[] = {
    "Mokarakhom",       /* 1  January */
    "Kumphaphan",       /* 2  February */
    "Minakhom",         /* 3  March */
    "Mesayon",          /* 4  April */
    "Phruetsaphakhom",  /* 5  May */
    "Mithunayon",       /* 6  June */
    "Karakadakhom",     /* 7  July */
    "Singhakhom",       /* 8  August */
    "Kanyayon",         /* 9  September */
    "Tulakhom",         /* 10 October */
    "Phruetsachikayon", /* 11 November */
    "Thanwakhom"        /* 12 December */
};

static const char *THAI_MONTH_ABBRS[] = {
    "Mok",  /* 1  */
    "Kum",  /* 2  */
    "Min",  /* 3  */
    "Mes",  /* 4  */
    "Phr",  /* 5  */
    "Mit",  /* 6  */
    "Kar",  /* 7  */
    "Sin",  /* 8  */
    "Kan",  /* 9  */
    "Tul",  /* 10 */
    "Pch",  /* 11 */
    "Thw"   /* 12 */
};

/* ===== Days per month (Gregorian) ===== */

static const int DAYS_IN_MONTH[] = {
    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

/* ===== Festival names ===== */

static const char *FESTIVAL_NAMES[] = {
    "",                     /* THAI_FEST_NONE */
    "Songkran",             /* THAI_FEST_SONGKRAN */
    "Chakri Memorial Day",  /* THAI_FEST_CHAKRI_DAY */
    "Constitution Day"      /* THAI_FEST_CONSTITUTION */
};

/* ===== Public API ===== */

int thai_be_year(int ce_year)
{
    return ce_year + BE_OFFSET;
}

int thai_ce_year(int be_year)
{
    return be_year - BE_OFFSET;
}

thai_date_t thai_from_jd(double jd)
{
    int ce_year, month, day;
    jd_to_gregorian_internal(jd, &ce_year, &month, &day);

    thai_date_t result;
    result.be_year = ce_year + BE_OFFSET;
    result.month = month;
    result.day = day;
    return result;
}

double thai_to_jd(thai_date_t date)
{
    int ce_year = date.be_year - BE_OFFSET;
    return gregorian_to_jd_internal(ce_year, date.month, (double)date.day);
}

const char *thai_month_name(int month)
{
    if (month < 1 || month > 12) return "?";
    return THAI_MONTH_NAMES[month - 1];
}

const char *thai_month_abbr(int month)
{
    if (month < 1 || month > 12) return "?";
    return THAI_MONTH_ABBRS[month - 1];
}

bool thai_is_songkran(thai_date_t date)
{
    return (date.month == 4 && date.day >= 13 && date.day <= 15);
}

thai_festival_t thai_festival(thai_date_t date)
{
    /* Songkran: April 13-15 */
    if (date.month == 4 && date.day >= 13 && date.day <= 15) {
        return THAI_FEST_SONGKRAN;
    }
    /* Chakri Memorial Day: April 6 */
    if (date.month == 4 && date.day == 6) {
        return THAI_FEST_CHAKRI_DAY;
    }
    /* Constitution Day: December 10 */
    if (date.month == 12 && date.day == 10) {
        return THAI_FEST_CONSTITUTION;
    }
    return THAI_FEST_NONE;
}

const char *thai_festival_name(thai_festival_t fest)
{
    if (fest < 0 || fest >= THAI_FEST_COUNT) return "?";
    return FESTIVAL_NAMES[fest];
}

bool thai_is_pre_reform(int ce_year)
{
    return (ce_year < 1941);
}

int thai_month_days(int be_year, int month)
{
    if (month < 1 || month > 12) return 0;
    int ce_year = be_year - BE_OFFSET;
    if (month == 2 && is_gregorian_leap(ce_year)) {
        return 29;
    }
    return DAYS_IN_MONTH[month - 1];
}

bool thai_is_leap_year(int be_year)
{
    int ce_year = be_year - BE_OFFSET;
    return is_gregorian_leap(ce_year);
}
