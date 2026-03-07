/* persian.c — Persian Solar Hijri (Jalali) calendar implementation
 *
 * Pure arithmetic calendar with 2820-year leap cycle.
 * 6 months x 31 days + 5 months x 30 days + Esfand (29 or 30 in leap).
 * Epoch: JD 1948320.5 = 1 Farvardin 1 SH = March 22, 622 CE (Julian).
 *
 * Sources: Reingold & Dershowitz, "Calendrical Calculations" (2018).
 *          The 2820-year cycle algorithm via Birashk/Borkowski. */

#include "persian.h"
#include <math.h>

/* Persian epoch: 1 Farvardin 1 SH at midnight UT */
static const double PERSIAN_EPOCH_JD = 1948320.5;

/* Grand cycle constants */
static const int GRAND_CYCLE_YEARS = 2820;
static const int GRAND_CYCLE_DAYS  = 1029983;

/* Reference year for the algorithm (Reingold/Dershowitz canonical form) */
static const int REFERENCE_YEAR = 474;

static const char *MONTH_NAMES[] = {
    "Farvardin", "Ordibehesht", "Khordad",
    "Tir", "Mordad", "Shahrivar",
    "Mehr", "Aban", "Azar",
    "Dey", "Bahman", "Esfand"
};

static const char *WEEKDAY_NAMES[] = {
    "Shanbe", "Yekshanbe", "Doshanbe", "Seshanbe",
    "Chaharshanbe", "Panjshanbe", "Jom'e"
};

static const char *FESTIVAL_NAMES[] = {
    "",                    /* PERSIAN_FEST_NONE */
    "Nowruz",              /* PERSIAN_FEST_NOWRUZ */
    "Sizdah Bedar",        /* PERSIAN_FEST_SIZDAH_BEDAR */
    "Yalda",               /* PERSIAN_FEST_YALDA */
    "Chaharshanbe Suri"    /* PERSIAN_FEST_CHAHARSHANBE_SURI */
};

/* Floor division that works correctly for negative dividends.
 * C's % operator truncates toward zero; we need floor semantics. */
static long floor_div(long a, long b)
{
    return (a >= 0) ? a / b : ((a - b + 1) / b);
}

/* Floor modulus that always returns a non-negative result. */
static long floor_mod(long a, long b)
{
    long r = a % b;
    if (r < 0) {
        r += b;
    }
    return r;
}

bool persian_is_leap_year(int year)
{
    /* Leap year check using the 2820-year cycle.
     * A year Y is leap if:
     * ((Y - (Y > 0 ? 474 : 473)) % 2820 + 474 + 38) * 682 % 2816 < 682 */
    long y = (long)year - (year > 0 ? REFERENCE_YEAR : REFERENCE_YEAR - 1);
    long ky = floor_mod(y, GRAND_CYCLE_YEARS) + REFERENCE_YEAR;
    return ((ky + 38) * 682) % 2816 < 682;
}

double persian_to_jd(persian_date_t date)
{
    /* Reingold/Dershowitz algorithm for Persian -> JD.
     *
     * 1. Normalize year relative to reference year 474.
     * 2. Compute position within the 2820-year grand cycle.
     * 3. Accumulate days from completed years, months, and current day. */
    long y = (long)date.year - (date.year > 0 ? REFERENCE_YEAR : REFERENCE_YEAR - 1);
    long ky = floor_mod(y, GRAND_CYCLE_YEARS) + REFERENCE_YEAR;

    double jd = PERSIAN_EPOCH_JD - 1.0
        + (double)GRAND_CYCLE_DAYS * floor_div(y, GRAND_CYCLE_YEARS)
        + 365.0 * (ky - 1)
        + floor((682.0 * ky - 110.0) / 2816.0);

    /* Add month days */
    if (date.month <= 6) {
        jd += 31.0 * (date.month - 1);
    } else {
        jd += 186.0 + 30.0 * (date.month - 7);
    }

    /* Add day of month */
    jd += date.day;

    return jd;
}

persian_date_t persian_from_jd(double jd)
{
    persian_date_t result;

    /* Total days from epoch */
    double d_from_epoch = jd - persian_to_jd((persian_date_t){ .year = 475, .month = 1, .day = 1 }) + 1.0;
    long day_count = (long)floor(d_from_epoch);

    /* Number of complete grand cycles */
    long cycle = floor_div(day_count - 1, GRAND_CYCLE_DAYS);
    long cyc_day = day_count - 1 - cycle * GRAND_CYCLE_DAYS;

    /* Year within cycle: find how many complete years fit in cyc_day days.
     * Use the formula: if cyc_day == 0, year_in_cycle = 0.
     * Otherwise, solve: year_in_cycle from accumulated days. */
    long y_cycle;
    if (cyc_day == 0) {
        y_cycle = 0;
    } else {
        /* The average year length in the cycle is 1029983/2820 = 365.24219858...
         * Use a binary-style approximation via the leap formula. */
        y_cycle = (long)floor((2816.0 * cyc_day + 1031337.0) / 1028522.0);

        /* Verify by checking if day falls within this year */
        long days_in_years = 365 * y_cycle + (long)floor((682.0 * (y_cycle + REFERENCE_YEAR) - 110.0) / 2816.0)
                           - (long)floor((682.0 * REFERENCE_YEAR - 110.0) / 2816.0);

        if (cyc_day < days_in_years) {
            y_cycle--;
        }
    }

    int year = (int)(2820 * cycle + y_cycle + 475);
    if (year <= 0) {
        year--;  /* No year zero: skip from -1 to 1 */
    }

    /* Day of year */
    persian_date_t jan1 = { .year = year, .month = 1, .day = 1 };
    long doy = (long)floor(jd - persian_to_jd(jan1)) + 1;

    /* Month from day of year */
    int month;
    if (doy <= 186) {
        /* First 6 months (31 days each): total = 186 */
        month = (int)((doy - 1) / 31) + 1;
    } else {
        /* Last 6 months (30 days each + Esfand 29/30) */
        month = (int)((doy - 187) / 30) + 7;
        if (month > 12) {
            month = 12;
        }
    }

    /* Day of month */
    int day;
    if (month <= 6) {
        day = (int)(doy - 31 * (month - 1));
    } else {
        day = (int)(doy - 186 - 30 * (month - 7));
    }

    result.year = year;
    result.month = month;
    result.day = day;
    return result;
}

const char *persian_month_name(int month)
{
    if (month < 1 || month > 12) {
        return "?";
    }
    return MONTH_NAMES[month - 1];
}

int persian_month_days(int year, int month)
{
    if (month < 1 || month > 12) {
        return 0;
    }
    if (month <= 6) {
        return 31;
    }
    if (month <= 11) {
        return 30;
    }
    /* Month 12 (Esfand): 30 in leap, 29 otherwise */
    return persian_is_leap_year(year) ? 30 : 29;
}

int persian_weekday(double jd)
{
    /* JD 0.5 (Monday Jan 1, 4713 BCE Julian) has a known weekday.
     * JD 2451545.5 = Jan 1, 2000 CE = Saturday.
     * Saturday = 0 in Persian weekday system.
     * The standard formula: weekday = (floor(jd + 0.5) + 1) % 7
     * gives 0=Monday. We need 0=Saturday.
     * Saturday offset: (floor(jd + 0.5) + 1 + 2) % 7 doesn't work.
     *
     * Let's derive directly:
     * JD 2451545.5 => floor(jd + 0.5) = 2451546.
     * 2451546 % 7 = 2451546 - 350220*7 = 2451546 - 2451540 = 6.
     * So (floor(jd+0.5)) % 7 = 6 for Saturday.
     * We want Saturday = 0, so: (floor(jd+0.5) + 1) % 7.
     * (2451546 + 1) % 7 = 2451547 % 7 = 0. Yes! Saturday = 0. */
    long jdn = (long)floor(jd + 0.5);
    int wd = (int)(floor_mod(jdn + 1, 7));
    return wd;
}

const char *persian_weekday_name(int day)
{
    if (day < 0 || day > 6) {
        return "?";
    }
    return WEEKDAY_NAMES[day];
}

int persian_year_days(int year)
{
    return persian_is_leap_year(year) ? 366 : 365;
}

persian_festival_t persian_festival(persian_date_t date)
{
    int m = date.month;
    int d = date.day;

    /* Nowruz: 1 Farvardin */
    if (m == 1 && d == 1) {
        return PERSIAN_FEST_NOWRUZ;
    }

    /* Sizdah Bedar: 13 Farvardin */
    if (m == 1 && d == 13) {
        return PERSIAN_FEST_SIZDAH_BEDAR;
    }

    /* Yalda: 30 Azar (month 9, day 30) */
    if (m == 9 && d == 30) {
        return PERSIAN_FEST_YALDA;
    }

    /* Chaharshanbe Suri: last Wednesday of Esfand (month 12).
     * Wednesday = 4 in our weekday system.
     * Find the last day of Esfand, then walk backwards to find
     * the last Wednesday. */
    if (m == 12) {
        int last_day = persian_month_days(date.year, 12);
        /* Find weekday of the last day of month */
        persian_date_t last = { .year = date.year, .month = 12, .day = last_day };
        double jd_last = persian_to_jd(last);
        int wd_last = persian_weekday(jd_last);
        /* How many days back from last_day to get to Wednesday (4)? */
        int days_back = (wd_last - 4 + 7) % 7;
        int wed_day = last_day - days_back;
        if (d == wed_day) {
            return PERSIAN_FEST_CHAHARSHANBE_SURI;
        }
    }

    return PERSIAN_FEST_NONE;
}

const char *persian_festival_name(persian_festival_t fest)
{
    if (fest < 0 || fest >= PERSIAN_FEST_COUNT) {
        return "?";
    }
    return FESTIVAL_NAMES[fest];
}

int persian_ce_year_approx(int sh_year)
{
    /* The Persian epoch is 622 CE. A Solar Hijri year is approximately
     * the same length as a Gregorian year (~365.2422 days), so
     * CE ~ SH + 621 (or 622, depending on which part of the year). */
    return sh_year + 621;
}

int persian_sh_year_approx(int ce_year)
{
    return ce_year - 621;
}
