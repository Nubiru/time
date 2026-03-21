#include "thirteen_moon.h"
#include "../../math/julian.h"
#include <math.h>
#include <string.h>

/* Moon names — each named after a galactic tone (Arguelles). */
static const char *MOON_NAMES[] = {
    "Magnetic",       /* Moon 1 */
    "Lunar",          /* Moon 2 */
    "Electric",       /* Moon 3 */
    "Self-Existing",  /* Moon 4 */
    "Overtone",       /* Moon 5 */
    "Rhythmic",       /* Moon 6 */
    "Resonant",       /* Moon 7 */
    "Galactic",       /* Moon 8 */
    "Solar",          /* Moon 9 */
    "Planetary",      /* Moon 10 */
    "Spectral",       /* Moon 11 */
    "Crystal",        /* Moon 12 */
    "Cosmic"          /* Moon 13 */
};

/* 7 Radial Plasmas — one per day of the heptad. */
static const char *PLASMA_NAMES[] = {
    "Dali", "Seli", "Gamma", "Kali", "Alpha", "Limi", "Silio"
};

static const char *PLASMA_CHAKRAS[] = {
    "Crown", "Root", "Third Eye", "Sacral", "Throat", "Solar Plexus", "Heart"
};

static const char *PLASMA_ACTIONS[] = {
    "Target", "Flow", "Pacify", "Establish", "Release", "Purify", "Discharge"
};

thirteen_moon_t thirteen_moon_from_jd(double jd)
{
    thirteen_moon_t result;
    memset(&result, 0, sizeof(result));

    if (jd < 0.0)
        return result;

    /* Step 1: Convert JD to Gregorian */
    int greg_year, greg_month;
    double day_frac = jd_to_gregorian(jd, &greg_year, &greg_month);
    int greg_day = (int)day_frac;

    /* Step 2: Determine 13-Moon year.
     * If date >= July 26, tm_year = greg_year.
     * If date < July 26, tm_year = greg_year - 1. */
    int tm_year;
    if (greg_month > 7 || (greg_month == 7 && greg_day >= 26)) {
        tm_year = greg_year;
    } else {
        tm_year = greg_year - 1;
    }

    /* Step 3: Reference JD = July 26 of the 13-Moon year */
    double ref_jd = gregorian_to_jd(tm_year, 7, 26.0);

    /* Step 4: Day of year within the 13-Moon calendar */
    int day_of_year = (int)floor(jd - ref_jd);

    /* Step 5: Handle overflow into next 13-Moon year.
     * A normal year has days 0-364 (365 days).
     * If day_of_year >= 365, recalculate for next year. */
    if (day_of_year >= TM_YEAR_DAYS) {
        tm_year++;
        ref_jd = gregorian_to_jd(tm_year, 7, 26.0);
        day_of_year = (int)floor(jd - ref_jd);
    }

    result.year = tm_year;
    result.day_of_year = day_of_year;

    /* Step 6: Day Out of Time = day 364 (July 25 of next Gregorian year) */
    if (day_of_year == 364) {
        result.is_day_out = 1;
        result.moon = 0;
        result.day = 0;
        result.heptad = 0;
        result.day_of_week = 0;
        result.plasma = 0;
        return result;
    }

    /* Step 7: Regular days 0-363 */
    result.moon = day_of_year / TM_MONTH_DAYS + 1;
    result.day = day_of_year % TM_MONTH_DAYS + 1;

    /* Step 8: Heptad and day of week */
    result.heptad = (result.day - 1) / TM_HEPTAD_DAYS + 1;
    result.day_of_week = (result.day - 1) % TM_HEPTAD_DAYS + 1;

    /* Step 9: Plasma (cycles daily within heptad) */
    result.plasma = (result.day_of_week - 1) % 7;

    return result;
}

const char *thirteen_moon_name(int moon)
{
    if (moon < 1 || moon > TM_MONTH_COUNT)
        return "?";
    return MOON_NAMES[moon - 1];
}

const char *thirteen_moon_plasma_name(int plasma)
{
    if (plasma < 0 || plasma > 6)
        return "?";
    return PLASMA_NAMES[plasma];
}

const char *thirteen_moon_plasma_chakra(int plasma)
{
    if (plasma < 0 || plasma > 6)
        return "?";
    return PLASMA_CHAKRAS[plasma];
}

const char *thirteen_moon_plasma_action(int plasma)
{
    if (plasma < 0 || plasma > 6)
        return "?";
    return PLASMA_ACTIONS[plasma];
}
