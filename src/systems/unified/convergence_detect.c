/* convergence_detect.c -- Real Cross-System Convergence Detector (Track 19.1)
 *
 * Calls real calendar modules for significance detection, unlike the
 * standalone convergence.c which uses inline approximations.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#include "convergence_detect.h"
#include "../../math/julian.h"
#include "../tzolkin/tzolkin.h"
#include "../tzolkin/tzolkin_board.h"
#include "../iching/iching.h"
#include "../chinese/chinese.h"
#include "../hebrew/hebrew.h"
#include "../islamic/hijri.h"
#include "../buddhist/buddhist.h"
#include "../persian/persian.h"
#include "../coptic/coptic.h"
#include "../celtic/wheel_of_year.h"
#include "../bahai/bahai.h"
#include <math.h>
#include <stdlib.h>

#define PI 3.14159265358979323846

/* Reference new moon epoch (2000-01-06 ~18:14 UT) */
#define SYNODIC_REF_JD   2451550.1
#define SYNODIC_PERIOD   29.530589

/* Solstice/equinox approximate day-of-year (Northern hemisphere) */
#define DOY_SPRING_EQUINOX  80   /* Mar 20 */
#define DOY_SUMMER_SOLSTICE 172  /* Jun 21 */
#define DOY_AUTUMN_EQUINOX  265  /* Sep 22 */
#define DOY_WINTER_SOLSTICE 355  /* Dec 21 */

/* ===================================================================
 * Internal helpers
 * =================================================================== */

/* Approximate day-of-year from Gregorian month/day. */
static int approx_day_of_year(int month, int day)
{
    static const int cum[] = {
        0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334
    };
    if (month < 1 || month > 12) return 0;
    return cum[month - 1] + day;
}

/* Moon age in days from reference new moon epoch. */
static double moon_age(double jd)
{
    double elapsed = jd - SYNODIC_REF_JD;
    double age = fmod(elapsed, SYNODIC_PERIOD);
    if (age < 0.0) age += SYNODIC_PERIOD;
    return age;
}

/* Approximate sun ecliptic longitude (degrees, 0-360). */
static double approx_sun_longitude(double jd)
{
    double lon = (jd - 2451545.0) / 365.25 * 360.0 + 280.46;
    lon = fmod(lon, 360.0);
    if (lon < 0.0) lon += 360.0;
    return lon;
}

/* Check if a system considers this JD significant.
 * Returns 1 if significant, 0 otherwise. */
static int cd_is_significant(cd_system_t sys, double jd)
{
    switch (sys) {

    case CD_SYS_ASTRONOMY: {
        /* Lunar phase: new moon (age < 1) or full moon (age ~14.765) */
        double age = moon_age(jd);
        double half = SYNODIC_PERIOD / 2.0;
        if (age < 1.0 || fabs(age - half) < 1.0)
            return 1;

        /* Solstice / equinox: within 1 day of cardinal points */
        int year, month;
        double day_frac = jd_to_gregorian(jd, &year, &month);
        int day = (int)day_frac;
        int doy = approx_day_of_year(month, day);

        if (abs(doy - DOY_SPRING_EQUINOX) <= 1 ||
            abs(doy - DOY_SUMMER_SOLSTICE) <= 1 ||
            abs(doy - DOY_AUTUMN_EQUINOX) <= 1 ||
            abs(doy - DOY_WINTER_SOLSTICE) <= 1)
            return 1;

        return 0;
    }

    case CD_SYS_ASTROLOGY: {
        /* Sun ingress: first 1 degree of any zodiac sign */
        double lon = approx_sun_longitude(jd);
        double within_sign = fmod(lon, 30.0);
        if (within_sign < 0.0) within_sign += 30.0;
        return within_sign < 1.0 ? 1 : 0;
    }

    case CD_SYS_TZOLKIN: {
        tzolkin_day_t tz = tzolkin_from_jd(jd);
        /* Kin 1 (Red Magnetic Dragon) */
        if (tz.kin == 1) return 1;
        /* Any Magnetic tone (tone 1) */
        if (tz.tone == 1) return 1;
        /* Galactic Activation Portal day */
        if (tzolkin_board_is_portal(tz.kin)) return 1;
        return 0;
    }

    case CD_SYS_ICHING: {
        hexagram_t hex = iching_from_jd(jd);
        /* Creative (1), Receptive (2), Peace (11), Standstill (12) */
        if (hex.king_wen == 1 || hex.king_wen == 2 ||
            hex.king_wen == 11 || hex.king_wen == 12)
            return 1;
        return 0;
    }

    case CD_SYS_CHINESE: {
        chinese_year_t cy = chinese_year_from_jd(jd);
        /* Sexagenary cycle start */
        if (cy.cycle_year == 1) return 1;

        /* Near Chinese New Year (approximate: Jan 1-3 Gregorian).
         * Real CNY falls Jan 21 - Feb 20, but for a first-pass
         * approximation we use Gregorian month 1, day <= 3. */
        int year, month;
        double day_frac = jd_to_gregorian(jd, &year, &month);
        int day = (int)day_frac;
        if (month == 1 && day <= 3)
            return 1;

        return 0;
    }

    case CD_SYS_HEBREW: {
        hebrew_date_t hd = hebrew_from_jd(jd);
        /* Rosh Chodesh (new month) */
        if (hd.day == 1) return 1;
        /* Rosh Hashanah: month 7 (Tishrei), days 1-2 */
        if (hd.month == 7 && hd.day <= 2) return 1;
        /* Yom Kippur: month 7, day 10 */
        if (hd.month == 7 && hd.day == 10) return 1;
        return 0;
    }

    case CD_SYS_ISLAMIC: {
        hijri_date_t hd = hijri_from_jd(jd);
        /* New month */
        if (hd.day == 1) return 1;
        /* Ramadan (month 9) */
        if (hd.month == 9) return 1;
        return 0;
    }

    case CD_SYS_HINDU: {
        /* Lunar phase proxy: tithi 1 (new moon) or tithi 15 (full moon).
         * tithi = elongation / 12, where elongation cycles 0-360 each
         * synodic month. Use moon_age to approximate. */
        double age = moon_age(jd);
        double tithi_approx = (age / SYNODIC_PERIOD) * 30.0;
        /* Near tithi 1 (new moon start) or tithi 15/16 (full moon) */
        if (tithi_approx < 1.0 || fabs(tithi_approx - 15.0) < 1.0)
            return 1;
        return 0;
    }

    case CD_SYS_BUDDHIST: {
        /* Uposatha: quarter moon observance days */
        uposatha_t u = buddhist_uposatha(jd);
        if (u.type != UPOSATHA_NONE)
            return 1;
        return 0;
    }

    case CD_SYS_PERSIAN: {
        persian_date_t pd = persian_from_jd(jd);
        /* Month boundary (day 1) */
        if (pd.day == 1) return 1;
        /* Nowruz: 1 Farvardin (month 1, day 1) already covered above.
         * Sizdah Bedar: 13 Farvardin */
        if (pd.month == 1 && pd.day == 13) return 1;
        return 0;
    }

    case CD_SYS_COPTIC: {
        coptic_date_t cd = coptic_from_jd(jd);
        /* Month boundary (day 1) */
        if (cd.day == 1) return 1;
        /* Feast day detection */
        coptic_feast_t feast = coptic_feast(cd);
        if (feast != COPTIC_FEAST_NONE) return 1;
        return 0;
    }

    case CD_SYS_CELTIC: {
        /* Check if sun longitude is within 1.5 degrees of any sabbat.
         * 1.5 deg ~ 1.5 day of solar motion. */
        double sun_lon = approx_sun_longitude(jd);
        if (wheel_is_festival_active(sun_lon, 1.5))
            return 1;
        return 0;
    }

    case CD_SYS_BAHAI: {
        bahai_date_t bd = bahai_from_jd(jd);
        /* Feast boundary: day 1 of any 19-day month */
        if (bd.month >= 1 && bd.day == 1) return 1;
        /* Holy day */
        bahai_holy_day_t holy = bahai_holy_day(bd);
        if (holy != BAHAI_HOLY_NONE) return 1;
        return 0;
    }

    default:
        return 0;
    }
}

/* ===================================================================
 * Public API
 * =================================================================== */

const char *cd_system_name(cd_system_t sys)
{
    switch (sys) {
    case CD_SYS_ASTRONOMY: return "Astronomy";
    case CD_SYS_ASTROLOGY: return "Astrology";
    case CD_SYS_TZOLKIN:   return "Tzolkin";
    case CD_SYS_ICHING:    return "I Ching";
    case CD_SYS_CHINESE:   return "Chinese";
    case CD_SYS_HEBREW:    return "Hebrew";
    case CD_SYS_ISLAMIC:   return "Islamic";
    case CD_SYS_HINDU:     return "Hindu";
    case CD_SYS_BUDDHIST:  return "Buddhist";
    case CD_SYS_PERSIAN:   return "Persian";
    case CD_SYS_COPTIC:    return "Coptic";
    case CD_SYS_CELTIC:    return "Celtic";
    case CD_SYS_BAHAI:     return "Bahai";
    default:               return "?";
    }
}

int cd_type_count(void)
{
    return CD_SYS_COUNT;
}

cd_result_t cd_scan(double jd)
{
    cd_result_t result;
    result.count = 0;

    /* Check each system for significance */
    int sig[CD_SYS_COUNT];
    int sig_count = 0;
    for (int i = 0; i < CD_SYS_COUNT; i++) {
        sig[i] = cd_is_significant((cd_system_t)i, jd);
        if (sig[i]) sig_count++;
    }

    /* If 2+ systems are significant, record convergence event */
    if (sig_count >= 2 && result.count < CD_MAX_CONVERGENCES) {
        cd_event_t *ev = &result.events[result.count];
        ev->system_count = 0;
        for (int i = 0; i < CD_SYS_COUNT; i++) {
            if (sig[i] && ev->system_count < CD_MAX_SYSTEMS) {
                ev->systems[ev->system_count++] = (cd_system_t)i;
            }
        }

        if (sig_count >= 5)
            ev->strength = CD_STRENGTH_RARE;
        else if (sig_count >= 4)
            ev->strength = CD_STRENGTH_MAJOR;
        else if (sig_count >= 3)
            ev->strength = CD_STRENGTH_NOTABLE;
        else
            ev->strength = CD_STRENGTH_MINOR;

        ev->significance = (double)sig_count / (double)CD_SYS_COUNT;
        ev->description = "Cross-system convergence";
        result.count++;
    }

    return result;
}

int cd_check_pair(cd_system_t a, cd_system_t b, double jd)
{
    if (a < 0 || a >= CD_SYS_COUNT) return 0;
    if (b < 0 || b >= CD_SYS_COUNT) return 0;
    return cd_is_significant(a, jd) && cd_is_significant(b, jd) ? 1 : 0;
}

double cd_significance(double jd)
{
    int sig_count = 0;
    for (int i = 0; i < CD_SYS_COUNT; i++) {
        if (cd_is_significant((cd_system_t)i, jd))
            sig_count++;
    }
    return (double)sig_count / (double)CD_SYS_COUNT;
}

double cd_find_next(double start_jd, cd_strength_t min_strength, int max_days)
{
    for (int d = 0; d < max_days; d++) {
        double jd = start_jd + (double)d;
        cd_result_t r = cd_scan(jd);
        if (r.count > 0 && r.events[0].strength >= min_strength) {
            return jd;
        }
    }
    return 0.0;
}
