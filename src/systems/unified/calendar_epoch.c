/* calendar_epoch.c -- How one religion's festival reshaped global timekeeping.
 *
 * Easter algorithm: Anonymous Gregorian Computus (Meeus, "Astronomical
 * Algorithms", Chapter 8). Integer-only computation.
 *
 * Julian/Gregorian drift: the Julian calendar gains ~3 days every 400 years
 * because it lacks the Gregorian century-skip rule. By 1582 (Gregorian reform),
 * 10 days had accumulated.
 *
 * Sources:
 *   Jean Meeus, "Astronomical Algorithms" (1991), Chapter 8
 *   Dionysius Exiguus, "Liber de Paschate" (525 CE)
 *   Pope Gregory XIII, "Inter gravissimas" (1582) */

#include "calendar_epoch.h"
#include <math.h>

/* ===== Easter computation (Computus) ===== */

ce_easter_t ce_easter_date(int year)
{
    /* Anonymous Gregorian algorithm — Meeus, Ch. 8 */
    int a = year % 19;
    int b = year / 100;
    int c = year % 100;
    int d = b / 4;
    int e = b % 4;
    int f = (b + 8) / 25;
    int g = (b - f + 1) / 3;
    int h = (19 * a + b - d - g + 15) % 30;
    int i = c / 4;
    int k = c % 4;
    int l = (32 + 2 * e + 2 * i - h - k) % 7;
    int m = (a + 11 * h + 22 * l) / 451;
    int month = (h + l - 7 * m + 114) / 31;
    int day = ((h + l - 7 * m + 114) % 31) + 1;

    ce_easter_t result;
    result.month = month;
    result.day = day;
    result.year = year;
    return result;
}

double ce_easter_jd(int year)
{
    ce_easter_t e = ce_easter_date(year);

    /* Meeus JD formula for Gregorian dates (Chapter 7) */
    int y = e.year;
    int m = e.month;
    double day_d = (double)e.day;

    if (m <= 2) {
        y = y - 1;
        m = m + 12;
    }

    int a = y / 100;
    double b = 2.0 - a + (a / 4);

    double jd = floor(365.25 * (y + 4716))
              + floor(30.6001 * (m + 1))
              + day_d + b - 1524.5;

    return jd;
}

/* ===== Epoch offset ===== */

int ce_epoch_offset_min(void)
{
    return CE_EPOCH_ERROR_YEARS_MIN;
}

int ce_epoch_offset_max(void)
{
    return CE_EPOCH_ERROR_YEARS_MAX;
}

/* ===== Julian/Gregorian drift ===== */

static int compute_drift(int year)
{
    /* The drift equals the number of century years that are NOT
     * divisible by 400, counted from century 2 (200 CE) onward.
     * Formula: (year/100) - (year/400) - 2
     * This gives the accumulated days the Julian calendar has
     * fallen behind the Gregorian. */
    int century = year / 100;
    int quad_century = year / 400;
    int drift = century - quad_century - 2;
    if (drift < 0) {
        drift = 0;
    }
    return drift;
}

ce_drift_t ce_julian_drift(int year)
{
    ce_drift_t result;
    result.drift_days = compute_drift(year);
    result.year = year;

    if (year < 200) {
        result.note = "Before significant drift accumulated";
    } else if (year <= 1582) {
        result.note = "Pre-reform: drift existed but was not yet corrected";
    } else if (year <= 1752) {
        result.note = "Post-reform: Catholic nations adopted Gregorian calendar";
    } else if (year <= 1923) {
        result.note = "Many Protestant and Orthodox nations still on Julian";
    } else {
        result.note = "Most nations Gregorian; some Orthodox churches retain Julian for Easter";
    }

    return result;
}

/* ===== Epoch facts ===== */

static const ce_epoch_fact_t EPOCH_FACTS[] = {
    {
        .year = -45,
        .title = "Julian Calendar",
        .who = "Julius Caesar / Sosigenes of Alexandria",
        .what = "Replaced the Roman calendar with a 365.25-day solar calendar",
        .impact = "First standardized solar calendar; leap year every 4 years"
    },
    {
        .year = 325,
        .title = "Council of Nicaea",
        .who = "Emperor Constantine / 318 bishops",
        .what = "Standardized Easter computation across Christendom",
        .impact = "Easter tied to spring equinox; drove need for calendar accuracy"
    },
    {
        .year = 525,
        .title = "Anno Domini epoch",
        .who = "Dionysius Exiguus",
        .what = "Created AD/BC dating system, placing Christ's birth at 1 AD",
        .impact = "Global year-numbering standard; birth date off by 4-7 years"
    },
    {
        .year = 1582,
        .title = "Gregorian Reform",
        .who = "Pope Gregory XIII / Aloysius Lilius / Christopher Clavius",
        .what = "Deleted 10 days (Oct 4 -> Oct 15); added century leap year rule",
        .impact = "Modern civil calendar; Easter stays near spring equinox"
    },
    {
        .year = 1752,
        .title = "British Calendar Act",
        .who = "British Parliament / Lord Chesterfield",
        .what = "Britain and colonies adopted Gregorian calendar; 11 days deleted",
        .impact = "English-speaking world synchronized with Catholic Europe"
    },
    {
        .year = 1917,
        .title = "Russian adoption",
        .who = "Soviet government / Vladimir Lenin",
        .what = "Russia adopted Gregorian calendar; 13 days deleted",
        .impact = "October Revolution actually occurred in November (Gregorian)"
    },
    {
        .year = 1923,
        .title = "Revised Julian calendar",
        .who = "Ecumenical Patriarch Meletius IV",
        .what = "Greek and other Orthodox churches adopted revised calendar",
        .impact = "Fixed dates align with Gregorian; Easter still computed by Julian"
    },
    {
        .year = 2023,
        .title = "Orthodox Easter divergence",
        .who = "Eastern Orthodox churches",
        .what = "Orthodox churches still compute Easter using Julian calendar",
        .impact = "Two Easters in most years; calendar unification remains incomplete"
    }
};

static const int EPOCH_FACTS_COUNT =
    (int)(sizeof(EPOCH_FACTS) / sizeof(EPOCH_FACTS[0]));

static const ce_epoch_fact_t EMPTY_FACT = {
    .year = 0,
    .title = "",
    .who = "",
    .what = "",
    .impact = ""
};

int ce_epoch_fact_count(void)
{
    return EPOCH_FACTS_COUNT;
}

ce_epoch_fact_t ce_epoch_fact_get(int index)
{
    if (index < 0 || index >= EPOCH_FACTS_COUNT) {
        return EMPTY_FACT;
    }
    return EPOCH_FACTS[index];
}

/* ===== System name ===== */

const char *ce_system_name(ce_system_t system)
{
    switch (system) {
        case CE_SYSTEM_JULIAN:    return "Julian";
        case CE_SYSTEM_GREGORIAN: return "Gregorian";
        default:                  return "?";
    }
}

/* ===== Leap years ===== */

int ce_is_julian_leap(int year)
{
    return (year % 4 == 0) ? 1 : 0;
}

int ce_is_gregorian_leap(int year)
{
    if (year % 400 == 0) return 1;
    if (year % 100 == 0) return 0;
    if (year % 4 == 0)   return 1;
    return 0;
}

/* ===== Gregorian-Julian difference ===== */

int ce_gregorian_julian_diff(int year)
{
    return compute_drift(year);
}
