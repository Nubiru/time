/* hijri.c — Tabular Islamic (Hijri) calendar implementation
 * Algorithm: Dershowitz & Reingold "Calendrical Calculations"
 * Pure functions, no side effects, no dynamic allocation. */

#include "hijri.h"

#include <math.h>
#include <stdio.h>

/* Julian Day of 1 Muharram 1 AH (July 16, 622 CE Julian) */
static const double HIJRI_EPOCH_JD = 1948439.5;

/* Month names (Arabic transliteration) */
static const char *MONTH_NAMES[12] = {
    "Muharram",
    "Safar",
    "Rabi' al-Awwal",
    "Rabi' al-Thani",
    "Jumada al-Ula",
    "Jumada al-Thani",
    "Rajab",
    "Sha'ban",
    "Ramadan",
    "Shawwal",
    "Dhul Qi'dah",
    "Dhul Hijjah"
};

bool hijri_is_leap(int year)
{
    /* Leap years in each 30-year cycle: 2,5,7,10,13,16,18,21,24,26,29
     * Formula: (11*year + 14) % 30 < 11 */
    return ((11 * year + 14) % 30) < 11;
}

int hijri_month_days(int year, int month)
{
    if (month < 1 || month > 12) {
        return 0;
    }
    /* Odd months = 30, even months = 29, except month 12 in leap years = 30 */
    if (month == 12) {
        return hijri_is_leap(year) ? 30 : 29;
    }
    return (month % 2 == 1) ? 30 : 29;
}

int hijri_year_days(int year)
{
    return hijri_is_leap(year) ? 355 : 354;
}

int hijri_day_of_year(hijri_date_t date)
{
    int days = 0;
    int m;
    for (m = 1; m < date.month; m++) {
        days += hijri_month_days(date.year, m);
    }
    return days + date.day;
}

const char *hijri_month_name(int month)
{
    if (month < 1 || month > 12) {
        return "?";
    }
    return MONTH_NAMES[month - 1];
}

double hijri_to_jd(hijri_date_t date)
{
    int y = date.year;
    int m = date.month;
    int d = date.day;

    /* Days from complete years (1 to y-1):
     * Each common year = 354, leap day count = floor((3 + 11*y) / 30) */
    int year_days = (y - 1) * 354 + (3 + 11 * y) / 30;

    /* Days from complete months (1 to m-1):
     * Odd months=30, even=29 => ceil(29.5*(m-1)) = 29*(m-1) + (m-1)/2
     * Which equals 29*(m-1) + floor(m/2) */
    int month_days = 29 * (m - 1) + m / 2;

    return HIJRI_EPOCH_JD + (double)year_days + (double)month_days
         + (double)(d - 1);
}

hijri_date_t hijri_from_jd(double jd)
{
    /* Days since epoch */
    int k = (int)floor(jd - HIJRI_EPOCH_JD);

    /* Estimate year from 30-year cycle (each cycle = 10631 days) */
    int y = (int)((30.0 * (double)k + 15.0) / 10631.0) + 1;

    /* Adjust: ensure we don't overshoot */
    while (hijri_to_jd((hijri_date_t){ y + 1, 1, 1 }) <= jd) {
        y++;
    }
    /* Also check if we've gone too far (shouldn't happen, but safety) */
    while (y > 1 && hijri_to_jd((hijri_date_t){ y, 1, 1 }) > jd) {
        y--;
    }

    /* Find month */
    int m = 1;
    while (m < 12 && hijri_to_jd((hijri_date_t){ y, m + 1, 1 }) <= jd) {
        m++;
    }

    /* Day */
    int d = (int)floor(jd - hijri_to_jd((hijri_date_t){ y, m, 1 })) + 1;

    return (hijri_date_t){ .year = y, .month = m, .day = d };
}

void hijri_fmt(hijri_date_t date, char *buf, size_t sz)
{
    if (sz == 0) {
        return;
    }
    snprintf(buf, sz, "%d %s %d", date.day, hijri_month_name(date.month),
             date.year);
}
