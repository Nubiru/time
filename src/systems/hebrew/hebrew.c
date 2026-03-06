/* hebrew.c — Hebrew (Jewish) lunisolar calendar implementation
 *
 * Algorithm: Dershowitz & Reingold "Calendrical Calculations"
 * All functions are pure (no globals, no malloc, no side effects). */

#include "hebrew.h"
#include <math.h>

/* R.D. (Rata Die) fixed day of 1 Tishrei, Year 1 */
static const int HEBREW_EPOCH_RD = -1373427;

/* R.D. to JD offset */
static const double RD_TO_JD = 1721424.5;

/* Chalakim constants */
static const long long CHALAKIM_PER_DAY   = 25920; /* 24 hours * 1080 parts */
static const long long CHALAKIM_LUNATION  = 13753; /* 12h*1080 + 793 parts */
static const long long CHALAKIM_MOLAD_INIT = 12084; /* BaHaRaD: 11h*1080 + 204 */

/* ===== Core calendar functions ===== */

bool hebrew_is_leap(int year)
{
    return ((7 * year + 1) % 19) < 7;
}

int hebrew_months_in_year(int year)
{
    return hebrew_is_leap(year) ? 13 : 12;
}

/* Elapsed days from epoch to 1 Tishrei of given year.
 * Uses long long to avoid 32-bit overflow on WASM. */
static int hebrew_elapsed_days(int year)
{
    long long months = (235LL * year - 234) / 19;
    long long parts = CHALAKIM_MOLAD_INIT + CHALAKIM_LUNATION * months;
    long long day = 29LL * months + parts / CHALAKIM_PER_DAY;

    /* Combined dechiyyot (all 4 postponement rules in one test) */
    if ((3 * (day + 1)) % 7 < 3) {
        day += 1;
    }

    return (int)day;
}

int hebrew_new_year(int year)
{
    return HEBREW_EPOCH_RD + hebrew_elapsed_days(year);
}

int hebrew_year_days(int year)
{
    return hebrew_new_year(year + 1) - hebrew_new_year(year);
}

hebrew_year_type_t hebrew_year_type(int year)
{
    int days = hebrew_year_days(year);
    /* Common: 353/354/355. Leap: 383/384/385.
     * days % 10 gives 3/4/5, + 350 gives 353/354/355. */
    return (hebrew_year_type_t)(days % 10 + 350);
}

int hebrew_month_days(int year, int month)
{
    if (month < 1 || month > 13) {
        return 0;
    }

    /* Month 13 only exists in leap years */
    if (month == 13 && !hebrew_is_leap(year)) {
        return 0;
    }

    hebrew_year_type_t type = hebrew_year_type(year);

    switch (month) {
    case 1:  return 30; /* Nisan */
    case 2:  return 29; /* Iyar */
    case 3:  return 30; /* Sivan */
    case 4:  return 29; /* Tammuz */
    case 5:  return 30; /* Av */
    case 6:  return 29; /* Elul */
    case 7:  return 30; /* Tishrei */
    case 8:             /* Cheshvan: 30 if abundant, else 29 */
        return (type == HEBREW_ABUNDANT) ? 30 : 29;
    case 9:             /* Kislev: 29 if deficient, else 30 */
        return (type == HEBREW_DEFICIENT) ? 29 : 30;
    case 10: return 29; /* Tevet */
    case 11: return 30; /* Shevat */
    case 12:            /* Adar (common=29) / Adar I (leap=30) */
        return hebrew_is_leap(year) ? 30 : 29;
    case 13: return 29; /* Adar II (leap only) */
    default: return 0;
    }
}

const char *hebrew_month_name(int month, bool is_leap_year)
{
    switch (month) {
    case 1:  return "Nisan";
    case 2:  return "Iyar";
    case 3:  return "Sivan";
    case 4:  return "Tammuz";
    case 5:  return "Av";
    case 6:  return "Elul";
    case 7:  return "Tishrei";
    case 8:  return "Cheshvan";
    case 9:  return "Kislev";
    case 10: return "Tevet";
    case 11: return "Shevat";
    case 12: return is_leap_year ? "Adar I" : "Adar";
    case 13: return "Adar II";
    default: return "";
    }
}

double hebrew_molad_tishrei(int year)
{
    long long months = (235LL * year - 234) / 19;
    return (double)months * (29.0 + (double)CHALAKIM_LUNATION / (double)CHALAKIM_PER_DAY)
         + (double)CHALAKIM_MOLAD_INIT / (double)CHALAKIM_PER_DAY;
}

/* ===== Bidirectional JD conversion ===== */

/* Civil month walk order: Tishrei(7), Cheshvan(8), ..., Elul(6).
 * In leap years, month 13 (Adar II) comes after month 12 (Adar I). */
static const int CIVIL_ORDER_COMMON[] = { 7, 8, 9, 10, 11, 12, 1, 2, 3, 4, 5, 6 };
static const int CIVIL_ORDER_LEAP[]   = { 7, 8, 9, 10, 11, 12, 13, 1, 2, 3, 4, 5, 6 };

hebrew_date_t hebrew_from_jd(double jd)
{
    int rd = (int)floor(jd - RD_TO_JD);

    /* Estimate year */
    int year = (int)((double)(rd - HEBREW_EPOCH_RD) * 19.0 / 6940.0) + 1;

    /* Adjust: increment until next year's new year exceeds rd */
    while (hebrew_new_year(year + 1) <= rd) {
        year++;
    }

    /* Day offset from 1 Tishrei (0-based) */
    int day_offset = rd - hebrew_new_year(year);

    /* Walk civil months to find which month contains day_offset */
    bool leap = hebrew_is_leap(year);
    int n_months = leap ? 13 : 12;
    const int *order = leap ? CIVIL_ORDER_LEAP : CIVIL_ORDER_COMMON;

    int month = order[0];
    for (int i = 0; i < n_months; i++) {
        int m = order[i];
        int mdays = hebrew_month_days(year, m);
        if (day_offset < mdays) {
            month = m;
            break;
        }
        day_offset -= mdays;
    }

    hebrew_date_t result = {
        .year  = year,
        .month = month,
        .day   = day_offset + 1
    };
    return result;
}

double hebrew_to_jd(hebrew_date_t date)
{
    int rd = hebrew_new_year(date.year);

    /* Sum days of all civil months from Tishrei up to (not including) date.month */
    bool leap = hebrew_is_leap(date.year);
    int n_months = leap ? 13 : 12;
    const int *order = leap ? CIVIL_ORDER_LEAP : CIVIL_ORDER_COMMON;

    for (int i = 0; i < n_months; i++) {
        int m = order[i];
        if (m == date.month) {
            break;
        }
        rd += hebrew_month_days(date.year, m);
    }

    rd += date.day - 1;

    return (double)rd + RD_TO_JD;
}
