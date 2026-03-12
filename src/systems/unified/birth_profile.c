/* birth_profile.c -- Birth Profile Computer implementation
 *
 * Aggregates 7 cultural/astronomical systems into a single birth profile.
 * Pure functions: no GL, no malloc, no globals, no side effects
 * (except static buffers in bp_summary). */

#include "birth_profile.h"
#include "../../math/julian.h"
#include "../tzolkin/tzolkin.h"
#include "../tzolkin/dreamspell.h"
#include "../chinese/chinese.h"
#include "../hebrew/hebrew.h"
#include "../islamic/hijri.h"
#include "../buddhist/buddhist.h"
#include "../iching/iching.h"
#include "../astrology/zodiac.h"
#include "../human_design/human_design.h"
#include "../astronomy/planets.h"

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define EARTH_AGE_YEARS 4540000000.0
#define BP_SYSTEM_COUNT 7
#define BP_SUMMARY_BUF  256

static const char *SYSTEM_NAMES[BP_SYSTEM_COUNT] = {
    "Tzolkin", "Chinese", "Hebrew", "Islamic",
    "Buddhist", "I Ching", "Astrology"
};

/* ---- helpers ---- */

static int bp_is_leap(int y)
{
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

static int bp_day_of_year(int year, int month, int day)
{
    static const int cum[] = {0,31,59,90,120,151,181,212,243,273,304,334};
    if (month < 1 || month > 12) return 0;
    int doy = cum[month - 1] + day;
    if (month > 2 && bp_is_leap(year))
        doy++;
    return doy;
}

static int bp_strcasecmp(const char *a, const char *b)
{
    while (*a && *b) {
        int ca = tolower((unsigned char)*a);
        int cb = tolower((unsigned char)*b);
        if (ca != cb) return ca - cb;
        a++;
        b++;
    }
    return tolower((unsigned char)*a) - tolower((unsigned char)*b);
}

/* ---- sub-system builders ---- */

static bp_tzolkin_t build_tzolkin(double jd)
{
    bp_tzolkin_t t;
    tzolkin_day_t tz = tzolkin_from_jd(jd);
    t.kin  = tz.kin;
    t.tone = tz.tone;
    t.seal = tz.seal;
    t.seal_name = tzolkin_seal_name(tz.seal);
    t.tone_name = tzolkin_tone_name(tz.tone);

    dreamspell_oracle_t oracle = dreamspell_oracle(tz.kin);
    t.guide_kin   = oracle.guide;
    t.analog_kin  = oracle.analog;
    t.antipode_kin = oracle.antipode;
    t.occult_kin  = oracle.occult;

    dreamspell_wavespell_t ws = dreamspell_wavespell(tz.kin);
    t.wavespell = ws.number;

    dreamspell_castle_t cs = dreamspell_castle(tz.kin);
    t.castle = cs.number;

    return t;
}

static bp_chinese_t build_chinese(double jd)
{
    bp_chinese_t c;
    chinese_year_t cy = chinese_year_from_jd(jd);
    c.animal   = cy.animal;
    c.element  = cy.element;
    c.stem     = cy.stem;
    c.branch   = cy.branch;
    c.polarity = cy.polarity;
    c.animal_name  = chinese_animal_name(cy.animal);
    c.element_name = chinese_element_name(cy.element);
    return c;
}

static bp_hebrew_t build_hebrew(double jd)
{
    bp_hebrew_t h;
    hebrew_date_t hd = hebrew_from_jd(jd);
    h.year  = hd.year;
    h.month = hd.month;
    h.day   = hd.day;
    h.month_name = hebrew_month_name(hd.month, hebrew_is_leap(hd.year));
    return h;
}

static bp_islamic_t build_islamic(double jd)
{
    bp_islamic_t i;
    hijri_date_t hi = hijri_from_jd(jd);
    i.year  = hi.year;
    i.month = hi.month;
    i.day   = hi.day;
    i.month_name = hijri_month_name(hi.month);
    return i;
}

static bp_buddhist_t build_buddhist(int ce_year)
{
    bp_buddhist_t b;
    b.era_year = buddhist_era(ce_year);
    return b;
}

static bp_iching_t build_iching(double jd)
{
    bp_iching_t ic;
    hexagram_t hex = iching_from_jd(jd);
    ic.king_wen = hex.king_wen;
    ic.name = iching_hexagram_name(hex.king_wen);
    return ic;
}

static bp_astrology_t build_astrology(double jd)
{
    bp_astrology_t a;

    /* Get Earth's heliocentric longitude, then geocentric Sun = Earth + 180 */
    solar_system_t sys = planets_at(jd);
    double earth_lon = sys.positions[PLANET_EARTH].longitude;
    double sun_lon = fmod(earth_lon + 180.0, 360.0);
    if (sun_lon < 0.0) sun_lon += 360.0;

    a.sun_longitude = sun_lon;
    a.sun_sign = zodiac_sign(sun_lon);
    a.sun_sign_name = zodiac_sign_name(a.sun_sign);

    hd_incarnation_t inc = hd_incarnation(sun_lon);
    a.hd_sun_gate   = inc.sun.gate;
    a.hd_earth_gate = inc.earth.gate;
    a.hd_sun_gate_name = hd_gate_name(inc.sun.gate);

    return a;
}

/* ---- public API ---- */

birth_profile_t bp_compute(int year, int month, int day)
{
    birth_profile_t p;
    memset(&p, 0, sizeof(p));

    p.year  = year;
    p.month = month;
    p.day   = day;
    p.jd    = gregorian_to_jd(year, month, (double)day);
    p.has_location = false;
    p.latitude  = 0.0;
    p.longitude = 0.0;

    p.tzolkin  = build_tzolkin(p.jd);
    p.chinese  = build_chinese(p.jd);
    p.hebrew   = build_hebrew(p.jd);
    p.islamic  = build_islamic(p.jd);
    p.buddhist = build_buddhist(year);
    p.iching   = build_iching(p.jd);
    p.astrology = build_astrology(p.jd);

    p.earth_age_ratio = (double)year / EARTH_AGE_YEARS;
    p.day_of_year = bp_day_of_year(year, month, day);
    p.western_zodiac = p.astrology.sun_sign_name;

    return p;
}

birth_profile_t bp_compute_with_location(int year, int month, int day,
                                          double latitude, double longitude)
{
    birth_profile_t p = bp_compute(year, month, day);
    p.has_location = true;
    p.latitude  = latitude;
    p.longitude = longitude;
    return p;
}

const char *bp_summary(const birth_profile_t *profile, const char *system)
{
    static char buf[BP_SUMMARY_BUF];

    if (!profile || !system) return NULL;

    if (bp_strcasecmp(system, "tzolkin") == 0) {
        snprintf(buf, sizeof(buf), "Kin %d %s %s",
                 profile->tzolkin.kin,
                 profile->tzolkin.tone_name,
                 profile->tzolkin.seal_name);
        return buf;
    }

    if (bp_strcasecmp(system, "chinese") == 0) {
        const char *pol = profile->chinese.polarity == 0 ? "Yang" : "Yin";
        snprintf(buf, sizeof(buf), "%s %s (%s)",
                 profile->chinese.element_name,
                 profile->chinese.animal_name,
                 pol);
        return buf;
    }

    if (bp_strcasecmp(system, "hebrew") == 0) {
        snprintf(buf, sizeof(buf), "%d %s %d",
                 profile->hebrew.year,
                 profile->hebrew.month_name,
                 profile->hebrew.day);
        return buf;
    }

    if (bp_strcasecmp(system, "islamic") == 0) {
        snprintf(buf, sizeof(buf), "%d %s %d AH",
                 profile->islamic.day,
                 profile->islamic.month_name,
                 profile->islamic.year);
        return buf;
    }

    if (bp_strcasecmp(system, "buddhist") == 0) {
        snprintf(buf, sizeof(buf), "BE %d", profile->buddhist.era_year);
        return buf;
    }

    if (bp_strcasecmp(system, "iching") == 0) {
        snprintf(buf, sizeof(buf), "Hexagram %d: %s",
                 profile->iching.king_wen,
                 profile->iching.name);
        return buf;
    }

    if (bp_strcasecmp(system, "astrology") == 0) {
        snprintf(buf, sizeof(buf), "Sun in %s (%.1f%s)",
                 profile->astrology.sun_sign_name,
                 profile->astrology.sun_longitude,
                 "\xc2\xb0"); /* degree sign UTF-8 */
        return buf;
    }

    return NULL;
}

int bp_system_count(void)
{
    return BP_SYSTEM_COUNT;
}

const char *bp_system_name(int index)
{
    if (index < 0 || index >= BP_SYSTEM_COUNT) return NULL;
    return SYSTEM_NAMES[index];
}
