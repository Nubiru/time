#include "lunar.h"
#include <math.h>

#define PI 3.14159265358979323846

/* Synodic month (New Moon to New Moon) in days */
static const double SYNODIC_MONTH = 29.53058868;

/* Reference New Moon: 2000-01-06 18:14 UT */
static const double REF_NEW_MOON_JD = 2451550.26;

static double normalize_angle(double deg) {
    double r = fmod(deg, 360.0);
    if (r < 0.0) r += 360.0;
    return r;
}

static moon_phase_t phase_from_fraction(double f) {
    if (f < 0.0625 || f >= 0.9375) return MOON_NEW;
    if (f < 0.1875) return MOON_WAXING_CRESCENT;
    if (f < 0.3125) return MOON_FIRST_QUARTER;
    if (f < 0.4375) return MOON_WAXING_GIBBOUS;
    if (f < 0.5625) return MOON_FULL;
    if (f < 0.6875) return MOON_WANING_GIBBOUS;
    if (f < 0.8125) return MOON_LAST_QUARTER;
    return MOON_WANING_CRESCENT;
}

static double moon_longitude(double jd) {
    double t = (jd - 2451545.0) / 36525.0;
    double l = 218.3164477 + 481267.88123421 * t;
    double m = 134.9633964 + 477198.8675055 * t;
    double lon = l + 6.289 * sin(m * PI / 180.0);
    return normalize_angle(lon);
}

lunar_info_t lunar_phase(double jd) {
    double days_since = jd - REF_NEW_MOON_JD;
    double cycles = days_since / SYNODIC_MONTH;
    double frac = cycles - floor(cycles);
    if (frac < 0.0) frac += 1.0;

    double illum = (1.0 - cos(frac * 2.0 * PI)) / 2.0;
    double age = frac * SYNODIC_MONTH;

    lunar_info_t info;
    info.phase_fraction = frac;
    info.illumination = illum;
    info.moon_longitude = moon_longitude(jd);
    info.phase = phase_from_fraction(frac);
    info.age_days = age;
    return info;
}

const char *lunar_phase_name(moon_phase_t phase) {
    switch (phase) {
        case MOON_NEW:              return "New Moon";
        case MOON_WAXING_CRESCENT:  return "Waxing Crescent";
        case MOON_FIRST_QUARTER:    return "First Quarter";
        case MOON_WAXING_GIBBOUS:   return "Waxing Gibbous";
        case MOON_FULL:             return "Full Moon";
        case MOON_WANING_GIBBOUS:   return "Waning Gibbous";
        case MOON_LAST_QUARTER:     return "Last Quarter";
        case MOON_WANING_CRESCENT:  return "Waning Crescent";
    }
    return "?";
}

const char *lunar_phase_symbol(moon_phase_t phase) {
    static const char *symbols[] = {
        "\xf0\x9f\x8c\x91", /* U+1F311 New Moon */
        "\xf0\x9f\x8c\x92", /* U+1F312 Waxing Crescent */
        "\xf0\x9f\x8c\x93", /* U+1F313 First Quarter */
        "\xf0\x9f\x8c\x94", /* U+1F314 Waxing Gibbous */
        "\xf0\x9f\x8c\x95", /* U+1F315 Full Moon */
        "\xf0\x9f\x8c\x96", /* U+1F316 Waning Gibbous */
        "\xf0\x9f\x8c\x97", /* U+1F317 Last Quarter */
        "\xf0\x9f\x8c\x98"  /* U+1F318 Waning Crescent */
    };
    if (phase < 0 || phase > 7) return "?";
    return symbols[phase];
}

double lunar_next_new_moon(double jd) {
    double days_since = jd - REF_NEW_MOON_JD;
    double cycles = days_since / SYNODIC_MONTH;
    double next_cycle = floor(cycles) + 1.0;
    double next_jd = REF_NEW_MOON_JD + next_cycle * SYNODIC_MONTH;
    if (next_jd <= jd) {
        next_jd += SYNODIC_MONTH;
    }
    return next_jd;
}

double lunar_next_full_moon(double jd) {
    double days_since = jd - REF_NEW_MOON_JD;
    double cycles = days_since / SYNODIC_MONTH;
    double frac = cycles - floor(cycles);
    if (frac < 0.0) frac += 1.0;

    double days_to_full;
    if (frac < 0.5) {
        days_to_full = (0.5 - frac) * SYNODIC_MONTH;
    } else {
        days_to_full = (1.5 - frac) * SYNODIC_MONTH;
    }
    return jd + days_to_full;
}

int lunar_zodiac_sign(double jd) {
    double lon = moon_longitude(jd);
    return (int)(lon / 30.0);
}
