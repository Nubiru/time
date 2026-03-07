/* daylight.c — Daylight state, golden hour, blue hour, seasonal day length.
 * Wraps solar_events.h to provide observer-centric daylight data.
 * All functions are pure: no GL, no malloc, no globals, no side effects.
 * Algorithm: hour angle formula from Meeus "Astronomical Algorithms". */

#include "daylight.h"
#include "../astronomy/solar_events.h"

#include <math.h>

#define PI 3.14159265358979323846
#define DEG2RAD (PI / 180.0)
#define RAD2DEG (180.0 / PI)

/* Standard refraction correction for "is it daytime?" */
#define SUN_HORIZON_DEG (-0.833)

/* Golden hour upper boundary: sun altitude in degrees */
#define GOLDEN_ALTITUDE_DEG 6.0

/* Blue hour boundaries: sun altitude in degrees */
#define BLUE_UPPER_DEG (-4.0)
#define BLUE_LOWER_DEG (-6.0)

/* Minutes per day */
#define MINUTES_PER_DAY (24.0 * 60.0)

/* Reference year for solstice calculations: J2000.0 = Jan 1.5 2000 */
#define J2000_JD 2451545.0
#define JUNE_SOLSTICE_OFFSET 171.0
#define DEC_SOLSTICE_OFFSET 355.0

/* ---------- internal helpers ---------- */

/* Compute the hour angle (degrees) at which the sun reaches a given altitude.
 * Returns NAN if the sun never reaches that altitude at this lat/dec. */
static double hour_angle_for_alt(double lat_deg, double dec_deg, double alt_deg)
{
    double lat_r = lat_deg * DEG2RAD;
    double dec_r = dec_deg * DEG2RAD;
    double alt_r = alt_deg * DEG2RAD;

    double denom = cos(lat_r) * cos(dec_r);
    if (fabs(denom) < 1e-12) {
        return NAN;
    }

    double cos_omega = (sin(alt_r) - sin(lat_r) * sin(dec_r)) / denom;

    if (cos_omega > 1.0) return NAN;
    if (cos_omega < -1.0) return NAN;

    return acos(cos_omega) * RAD2DEG;
}

/* Build a daylight_window_t from two JD values. */
static daylight_window_t make_window(double start_jd, double end_jd, int valid)
{
    daylight_window_t w;
    w.start_jd = start_jd;
    w.end_jd = end_jd;
    w.valid = valid;
    return w;
}

/* Invalid window */
static daylight_window_t invalid_window(void)
{
    return make_window(0.0, 0.0, 0);
}

/* ---------- public API ---------- */

daylight_t daylight_at(double jd, double lat, double lon)
{
    daylight_t d;
    solar_day_t sd = se_solar_events(jd, lat, lon);

    d.sun_elevation_deg = se_solar_altitude(jd, lat, lon);
    d.is_day = (d.sun_elevation_deg > SUN_HORIZON_DEG) ? 1 : 0;
    d.day_length_hours = sd.day_length;
    d.polar_day = sd.always_day;
    d.polar_night = sd.always_night;

    /* minutes_to_sunrise */
    if (sd.always_night) {
        d.minutes_to_sunrise = -1.0;
    } else if (sd.always_day) {
        /* During midnight sun, no sunrise event */
        d.minutes_to_sunrise = -1.0;
    } else {
        if (jd < sd.sunrise_jd) {
            /* Before today's sunrise */
            d.minutes_to_sunrise = (sd.sunrise_jd - jd) * MINUTES_PER_DAY;
        } else {
            /* After today's sunrise: next sunrise is tomorrow */
            solar_day_t sd_next = se_solar_events(jd + 1.0, lat, lon);
            if (sd_next.always_night || sd_next.always_day) {
                d.minutes_to_sunrise = -1.0;
            } else {
                d.minutes_to_sunrise = (sd_next.sunrise_jd - jd) * MINUTES_PER_DAY;
            }
        }
    }

    /* minutes_to_sunset */
    if (sd.always_day) {
        d.minutes_to_sunset = -1.0;
    } else if (sd.always_night) {
        /* During polar night, no sunset event */
        d.minutes_to_sunset = -1.0;
    } else {
        if (jd < sd.sunset_jd) {
            /* Before today's sunset */
            d.minutes_to_sunset = (sd.sunset_jd - jd) * MINUTES_PER_DAY;
        } else {
            /* After today's sunset: next sunset is tomorrow */
            solar_day_t sd_next = se_solar_events(jd + 1.0, lat, lon);
            if (sd_next.always_day || sd_next.always_night) {
                d.minutes_to_sunset = -1.0;
            } else {
                d.minutes_to_sunset = (sd_next.sunset_jd - jd) * MINUTES_PER_DAY;
            }
        }
    }

    return d;
}

daylight_window_t daylight_golden_hour_morning(double jd, double lat, double lon)
{
    solar_day_t sd = se_solar_events(jd, lat, lon);

    if (sd.always_night || sd.always_day) {
        return invalid_window();
    }

    double noon = floor(jd - 0.5) + 0.5;
    double dec = se_solar_declination(noon);

    double omega_6 = hour_angle_for_alt(lat, dec, GOLDEN_ALTITUDE_DEG);
    if (isnan(omega_6)) {
        return invalid_window();
    }

    /* Morning golden hour: from sunrise to the moment sun reaches 6 degrees.
     * 6-degree moment is closer to noon than sunrise, so hour angle is smaller.
     * JD = solar_noon - omega/360 for morning. */
    double golden_end = sd.solar_noon_jd - omega_6 / 360.0;

    if (golden_end <= sd.sunrise_jd) {
        return invalid_window();
    }

    return make_window(sd.sunrise_jd, golden_end, 1);
}

daylight_window_t daylight_golden_hour_evening(double jd, double lat, double lon)
{
    solar_day_t sd = se_solar_events(jd, lat, lon);

    if (sd.always_night || sd.always_day) {
        return invalid_window();
    }

    double noon = floor(jd - 0.5) + 0.5;
    double dec = se_solar_declination(noon);

    double omega_6 = hour_angle_for_alt(lat, dec, GOLDEN_ALTITUDE_DEG);
    if (isnan(omega_6)) {
        return invalid_window();
    }

    /* Evening golden hour: from when sun drops to 6 degrees until sunset.
     * JD = solar_noon + omega/360 for evening. */
    double golden_start = sd.solar_noon_jd + omega_6 / 360.0;

    if (golden_start >= sd.sunset_jd) {
        return invalid_window();
    }

    return make_window(golden_start, sd.sunset_jd, 1);
}

daylight_window_t daylight_blue_hour_morning(double jd, double lat, double lon)
{
    solar_day_t sd = se_solar_events(jd, lat, lon);

    if (sd.always_night || sd.always_day) {
        return invalid_window();
    }

    double noon = floor(jd - 0.5) + 0.5;
    double dec = se_solar_declination(noon);

    double omega_m6 = hour_angle_for_alt(lat, dec, BLUE_LOWER_DEG);
    double omega_m4 = hour_angle_for_alt(lat, dec, BLUE_UPPER_DEG);

    if (isnan(omega_m6) || isnan(omega_m4)) {
        return invalid_window();
    }

    /* Morning blue hour: from -6 degrees to -4 degrees (before sunrise).
     * -6 deg is further from noon (larger hour angle), -4 deg is closer. */
    double blue_start = sd.solar_noon_jd - omega_m6 / 360.0;
    double blue_end = sd.solar_noon_jd - omega_m4 / 360.0;

    if (blue_end <= blue_start) {
        return invalid_window();
    }

    return make_window(blue_start, blue_end, 1);
}

daylight_window_t daylight_blue_hour_evening(double jd, double lat, double lon)
{
    solar_day_t sd = se_solar_events(jd, lat, lon);

    if (sd.always_night || sd.always_day) {
        return invalid_window();
    }

    double noon = floor(jd - 0.5) + 0.5;
    double dec = se_solar_declination(noon);

    double omega_m4 = hour_angle_for_alt(lat, dec, BLUE_UPPER_DEG);
    double omega_m6 = hour_angle_for_alt(lat, dec, BLUE_LOWER_DEG);

    if (isnan(omega_m4) || isnan(omega_m6)) {
        return invalid_window();
    }

    /* Evening blue hour: from -4 degrees to -6 degrees (after sunset).
     * -4 deg is closer to noon, -6 deg is further. */
    double blue_start = sd.solar_noon_jd + omega_m4 / 360.0;
    double blue_end = sd.solar_noon_jd + omega_m6 / 360.0;

    if (blue_end <= blue_start) {
        return invalid_window();
    }

    return make_window(blue_start, blue_end, 1);
}

double daylight_day_length(double jd, double lat, double lon)
{
    solar_day_t sd = se_solar_events(jd, lat, lon);
    return sd.day_length;
}

double daylight_longest_day(double lat)
{
    /* Northern hemisphere: longest day at June solstice.
     * Southern hemisphere: longest day at December solstice.
     * Use reference year (J2000) and longitude 0 for the calculation. */
    double solstice_jd;
    if (lat >= 0.0) {
        solstice_jd = J2000_JD + JUNE_SOLSTICE_OFFSET;
    } else {
        solstice_jd = J2000_JD + DEC_SOLSTICE_OFFSET;
    }

    solar_day_t sd = se_solar_events(solstice_jd, lat, 0.0);
    return sd.day_length;
}

double daylight_shortest_day(double lat)
{
    /* Northern hemisphere: shortest day at December solstice.
     * Southern hemisphere: shortest day at June solstice. */
    double solstice_jd;
    if (lat >= 0.0) {
        solstice_jd = J2000_JD + DEC_SOLSTICE_OFFSET;
    } else {
        solstice_jd = J2000_JD + JUNE_SOLSTICE_OFFSET;
    }

    solar_day_t sd = se_solar_events(solstice_jd, lat, 0.0);
    return sd.day_length;
}
