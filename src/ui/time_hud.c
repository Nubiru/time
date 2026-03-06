#include "time_hud.h"
#include "../math/julian.h"
#include "../math/sidereal.h"
#include "../systems/gregorian/gregorian.h"
#include "../systems/tzolkin/tzolkin.h"
#include "../systems/astrology/zodiac.h"
#include "../systems/astronomy/planets.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

/* Sidereal rate: degrees per solar day (Meeus). */
#define SIDEREAL_RATE 360.98564736629

static double normalize_deg(double deg) {
    deg = fmod(deg, 360.0);
    if (deg < 0.0) deg += 360.0;
    return deg;
}

void degrees_to_hms(double degrees, char *buf, size_t buf_size) {
    double norm = normalize_deg(degrees);
    double total_hours = norm / 15.0;
    int h = (int)total_hours;
    double min_frac = (total_hours - h) * 60.0;
    int m = (int)min_frac;
    double sec_frac = (min_frac - m) * 60.0;
    int s = (int)(sec_frac + 0.5);

    if (s >= 60) { s -= 60; m++; }
    if (m >= 60) { m -= 60; h++; }
    if (h >= 24) { h -= 24; }

    snprintf(buf, buf_size, "%02dh %02dm %02ds", h, m, s);
}

time_hud_t time_hud_compute(double jd, double observer_lon, double time_speed) {
    time_hud_t hud;
    memset(&hud, 0, sizeof(hud));

    /* 1. Gregorian date */
    gregorian_format_date(jd, hud.gregorian_date, sizeof(hud.gregorian_date));

    /* 2. Gregorian time */
    gregorian_format_time(jd, hud.gregorian_time, sizeof(hud.gregorian_time));

    /* 3. Day of week */
    int dow = gregorian_day_of_week(jd);
    snprintf(hud.day_name, sizeof(hud.day_name), "%s", gregorian_day_name(dow));

    /* 4. Julian Day string (4 decimal places) */
    snprintf(hud.jd_str, sizeof(hud.jd_str), "%.4f", jd);

    /* 5. GMST: compute at 0h UT then add elapsed sidereal time */
    double jd_0h = floor(jd - 0.5) + 0.5;
    double gmst_0h_deg = gmst_degrees(jd_0h);
    double gmst_deg = normalize_deg(gmst_0h_deg + (jd - jd_0h) * SIDEREAL_RATE);
    degrees_to_hms(gmst_deg, hud.gmst_str, sizeof(hud.gmst_str));

    /* 6. GAST (handles arbitrary JD internally) */
    double gast_deg = gast_degrees(jd);
    degrees_to_hms(gast_deg, hud.gast_str, sizeof(hud.gast_str));

    /* 7. LST = GAST + observer longitude */
    double lst_deg = lst_degrees(gast_deg, observer_lon);
    degrees_to_hms(lst_deg, hud.lst_str, sizeof(hud.lst_str));

    /* 8. Tzolkin */
    tzolkin_day_t tz = tzolkin_from_jd(jd);
    const char *seal = tzolkin_seal_name(tz.seal);
    snprintf(hud.tzolkin_str, sizeof(hud.tzolkin_str),
             "%d %s (Kin %d)", tz.tone, seal, tz.kin);

    /* 9. Sun sign: geocentric Sun longitude = Earth heliocentric + 180 */
    solar_system_t sys = planets_at(jd);
    double earth_lon = sys.positions[PLANET_EARTH].longitude;
    double sun_lon = normalize_deg(earth_lon + 180.0);
    int sign = zodiac_sign(sun_lon);
    double deg_in_sign = zodiac_degree(sun_lon);
    snprintf(hud.sun_sign_str, sizeof(hud.sun_sign_str),
             "%s %.1fd", zodiac_sign_name(sign), deg_in_sign);

    /* 10. Speed label */
    if (time_speed == 0.0) {
        snprintf(hud.speed_str, sizeof(hud.speed_str), "PAUSED");
    } else if (time_speed == floor(time_speed) && fabs(time_speed) < 1e9) {
        snprintf(hud.speed_str, sizeof(hud.speed_str), "%ldx", (long)time_speed);
    } else {
        snprintf(hud.speed_str, sizeof(hud.speed_str), "%.1fx", time_speed);
    }

    return hud;
}
