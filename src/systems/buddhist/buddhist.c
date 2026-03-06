/* buddhist.c — Buddhist calendar data module
 * Pure functions: no GL, no malloc, no globals, no side effects.
 * Depends on: lunar.h (moon phase computation). */

#include "buddhist.h"
#include "../astronomy/lunar.h"
#include <math.h>

#define PI 3.14159265358979323846

/* Theravada Buddhist Era offset */
static const int BE_OFFSET = 543;

/* Synodic month in days (same as lunar.c) */
static const double SYNODIC_MONTH = 29.53058868;

/* Tolerance for quarter-phase detection in phase_fraction units.
 * ~1 day out of ~29.53 day cycle = ~0.034 */
static const double PHASE_TOL = 0.034;

/* 12 Theravada Pali month names */
static const char *MONTH_NAMES[12] = {
    "Citta",       /* 1  — March-April */
    "Vesakha",     /* 2  — April-May */
    "Jettha",      /* 3  — May-June */
    "Asalha",      /* 4  — June-July */
    "Savana",      /* 5  — July-August */
    "Potthapadha", /* 6  — August-September */
    "Assayuja",    /* 7  — September-October */
    "Kattika",     /* 8  — October-November */
    "Maggasira",   /* 9  — November-December */
    "Phussa",      /* 10 — December-January */
    "Magha",       /* 11 — January-February */
    "Phalguna"     /* 12 — February-March */
};

/* Uposatha descriptions indexed by uposatha_type_t */
static const char *UPOSATHA_DESCRIPTIONS[5] = {
    "Not an observance day",
    "Amavasya — new moon observance",
    "Purnima — full moon observance, Patimokkha recitation",
    "Waxing half-moon observance",
    "Waning half-moon observance"
};

/* Approximate solar longitude from Julian Day.
 * Simplified: mean longitude + equation of center.
 * Accuracy: ~1 degree, sufficient for month-level detection. */
static double approx_sun_longitude(double jd)
{
    double t = (jd - 2451545.0) / 36525.0;
    double l0 = 280.46646 + 36000.76983 * t;
    double m = 357.52911 + 35999.05029 * t;
    double m_rad = m * PI / 180.0;
    double c = 1.9146 * sin(m_rad) + 0.02 * sin(2.0 * m_rad);
    double lon = l0 + c;
    lon = fmod(lon, 360.0);
    if (lon < 0.0) lon += 360.0;
    return lon;
}

/* ===== Buddhist Era ===== */

int buddhist_era(int ce_year)
{
    return ce_year + BE_OFFSET;
}

int buddhist_to_ce(int be_year)
{
    return be_year - BE_OFFSET;
}

/* ===== Uposatha detection ===== */

/* Classify phase_fraction into quarter phase type.
 * Quarter phases: 0.0=new, 0.25=first quarter, 0.5=full, 0.75=last quarter.
 * Returns UPOSATHA_NONE if not within tolerance of any quarter. */
static uposatha_type_t classify_phase(double frac)
{
    /* New Moon: near 0.0 or 1.0 */
    if (frac < PHASE_TOL || (1.0 - frac) < PHASE_TOL) {
        return UPOSATHA_NEW_MOON;
    }
    /* First Quarter: near 0.25 */
    if (fabs(frac - 0.25) < PHASE_TOL) {
        return UPOSATHA_FIRST_QUARTER;
    }
    /* Full Moon: near 0.5 */
    if (fabs(frac - 0.5) < PHASE_TOL) {
        return UPOSATHA_FULL_MOON;
    }
    /* Last Quarter: near 0.75 */
    if (fabs(frac - 0.75) < PHASE_TOL) {
        return UPOSATHA_LAST_QUARTER;
    }
    return UPOSATHA_NONE;
}

uposatha_t buddhist_uposatha(double jd)
{
    lunar_info_t lunar = lunar_phase(jd);
    uposatha_type_t type = classify_phase(lunar.phase_fraction);

    uposatha_t u;
    u.type = type;
    u.name = buddhist_uposatha_name(type);
    u.description = UPOSATHA_DESCRIPTIONS[type];
    u.moon_age_days = lunar.age_days;
    u.illumination = lunar.illumination;
    return u;
}

const char *buddhist_uposatha_name(uposatha_type_t type)
{
    switch (type) {
        case UPOSATHA_NONE:          return "None";
        case UPOSATHA_NEW_MOON:      return "Amavasya";
        case UPOSATHA_FULL_MOON:     return "Purnima";
        case UPOSATHA_FIRST_QUARTER: return "First Quarter";
        case UPOSATHA_LAST_QUARTER:  return "Last Quarter";
    }
    return "?";
}

/* ===== Next Uposatha ===== */

double buddhist_next_uposatha(double jd)
{
    lunar_info_t lunar = lunar_phase(jd);
    double frac = lunar.phase_fraction;

    /* Quarter boundaries at 0.0, 0.25, 0.5, 0.75 in phase_fraction.
     * Find the next boundary after current fraction. */
    static const double quarters[4] = { 0.25, 0.5, 0.75, 1.0 };

    for (int i = 0; i < 4; i++) {
        double diff = quarters[i] - frac;
        if (diff > PHASE_TOL) {
            return jd + diff * SYNODIC_MONTH;
        }
    }

    /* Past last quarter (very near 1.0/0.0), next is first quarter of next cycle */
    double diff = (1.0 - frac) + 0.25;
    return jd + diff * SYNODIC_MONTH;
}

/* ===== Buddhist month ===== */

const char *buddhist_month_name(int month)
{
    if (month < 1 || month > 12) return "?";
    return MONTH_NAMES[month - 1];
}

int buddhist_month_count(void)
{
    return 12;
}

buddhist_month_t buddhist_month(double jd)
{
    /* Approximate: Citta (month 1) starts near March new moon.
     * Use solar longitude to determine approximate month.
     * Citta (1) ~ sun_lon 330-360/0 (March-April)
     * Vesakha (2) ~ 0-30 (April-May)
     * ...
     * Shift: (sun_lon + 30) mod 360, then divide by 30, +1 */
    double sun_lon = approx_sun_longitude(jd);
    double shifted = fmod(sun_lon + 30.0, 360.0);
    if (shifted < 0.0) shifted += 360.0;
    int m = (int)(shifted / 30.0) + 1;

    if (m < 1) m = 1;
    if (m > 12) m = 12;

    buddhist_month_t result;
    result.month = m;
    result.name = MONTH_NAMES[m - 1];
    /* Vassa: months 5 (Savana) through 7 (Assayuja) */
    result.is_vassa = (m >= 5 && m <= 7);
    return result;
}

/* ===== Vassa ===== */

bool buddhist_is_vassa(double jd)
{
    /* Vassa runs approximately from Asalha full moon (~July, sun lon ~100)
     * to Pavarana full moon (~October, sun lon ~200).
     * Simplified: sun longitude between 100 and 200 degrees. */
    double sun_lon = approx_sun_longitude(jd);
    return (sun_lon >= 100.0 && sun_lon < 200.0);
}

/* ===== Vesak ===== */

double buddhist_vesak_jd(int ce_year)
{
    /* Vesak: first full moon when sun is in Taurus (lon >= 30 degrees).
     * Strategy: estimate when sun enters Taurus (~April 20), then find
     * the next full moon on or after that date.
     *
     * J2000.0 = 2000-01-01.5 (JD 2451545.0). Approximate Jan 1 of year,
     * then add ~120 days (late April) to ensure we skip the April full moon
     * and land on the May one. */
    double y = (double)ce_year;
    double days_from_j2000 = 365.25 * (y - 2000.0);
    /* ~April 30 = day 120 of the year (safely past Aries) */
    double search_start = 2451545.0 + days_from_j2000 + 120.0;

    /* Find next full moon on or after ~April 30 */
    return lunar_next_full_moon(search_start);
}
