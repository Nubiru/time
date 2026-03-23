/*
 * hud.c — Time HUD overlay (DOM-based)
 *
 * Extracted from main.c. Formats time data from multiple systems
 * and pushes to HTML DOM overlay.
 */

#ifdef __EMSCRIPTEN__

#include "hud.h"
#include <stdio.h>
#include <math.h>
#include <emscripten.h>
#include "../systems/gregorian/gregorian.h"
#include "../systems/astrology/zodiac.h"
#include "../systems/astrology/observer.h"
#include "../systems/astrology/aspects.h"
#include "../systems/astrology/houses.h"
#include "../systems/astronomy/planets.h"
#include "../systems/tzolkin/tzolkin.h"
#include "../systems/chinese/chinese.h"
#include "../math/sidereal.h"
#include "../math/ecliptic.h"
#include "../render/camera_scale.h"

static void degrees_to_hms_buf(double deg, char *buf, size_t sz) {
    deg = fmod(deg, 360.0);
    if (deg < 0.0) deg += 360.0;
    double hours = deg / 15.0;
    int h = (int)hours;
    int m = (int)((hours - h) * 60.0);
    int s = (int)((hours - h - m / 60.0) * 3600.0 + 0.5);
    if (s >= 60) { s -= 60; m++; }
    if (m >= 60) { m -= 60; h++; }
    if (h >= 24) h -= 24;
    snprintf(buf, sz, "%02dh %02dm %02ds", h, m, s);
}

void hud_update(double jd, double time_speed,
                double obs_lat, double obs_lon,
                float log_zoom) {
    /* Gregorian */
    char date[16], time_str[16];
    gregorian_format_date(jd, date, sizeof(date));
    gregorian_format_time(jd, time_str, sizeof(time_str));
    int dow = gregorian_day_of_week(jd);
    const char *day_name = gregorian_day_name(dow);

    /* JD */
    char jd_str[24];
    snprintf(jd_str, sizeof(jd_str), "%.4f", jd);

    /* Sidereal — GMST and LST */
    double jd_0h = floor(jd - 0.5) + 0.5;
    double gmst = gmst_degrees(jd_0h);
    char gmst_str[16];
    degrees_to_hms_buf(gmst, gmst_str, sizeof(gmst_str));

    double lst = lst_degrees(gmst, obs_lon);
    char lst_str[16];
    degrees_to_hms_buf(lst, lst_str, sizeof(lst_str));

    /* Ascendant */
    double obliquity = mean_obliquity(jd);
    double asc = ascendant_longitude(lst, obliquity, obs_lat);
    int asc_sign = zodiac_sign(asc);
    double asc_deg = zodiac_degree(asc);
    char asc_str[32];
    snprintf(asc_str, sizeof(asc_str), "ASC %s %s %.0f",
             zodiac_sign_symbol(asc_sign), zodiac_sign_name(asc_sign), asc_deg);

    /* Houses (Whole Sign) */
    house_system_t houses = houses_whole_sign(asc);
    int mc_sign = houses.signs[9];
    char house_str[48];
    snprintf(house_str, sizeof(house_str), "MC %s %s",
             zodiac_sign_symbol(mc_sign), zodiac_sign_name(mc_sign));

    /* Tzolkin */
    tzolkin_day_t kin = tzolkin_from_jd(jd);
    char tzolkin_str[64];
    snprintf(tzolkin_str, sizeof(tzolkin_str), "%d %s (Kin %d)",
             kin.tone, tzolkin_seal_name(kin.seal), kin.kin);

    /* Chinese calendar */
    chinese_year_t cy = chinese_year_from_jd(jd);
    char chinese_str[64];
    snprintf(chinese_str, sizeof(chinese_str), "%s %s %s %s",
             chinese_animal_symbol(cy.animal),
             chinese_element_name(cy.element),
             chinese_animal_name(cy.animal),
             chinese_polarity_name(cy.polarity));

    /* Sun sign (geocentric = Earth heliocentric + 180) */
    solar_system_t sys = planets_at(jd);
    double earth_lon = sys.positions[PLANET_EARTH].longitude;
    double sun_lon = fmod(earth_lon + 180.0, 360.0);
    int sign = zodiac_sign(sun_lon);
    double deg_in_sign = zodiac_degree(sun_lon);
    char sun_str[32];
    snprintf(sun_str, sizeof(sun_str), "%s %s %.1fd",
             zodiac_sign_symbol(sign), zodiac_sign_name(sign), deg_in_sign);

    /* Aspects — approximate geocentric longitudes */
    double geo_lons[8];
    for (int p = 0; p < 8; p++) {
        if (p == PLANET_EARTH) {
            geo_lons[p] = sun_lon;
        } else {
            double gl = fmod(sys.positions[p].longitude - earth_lon + 180.0, 360.0);
            if (gl < 0.0) gl += 360.0;
            geo_lons[p] = gl;
        }
    }
    aspect_list_t aspects = aspects_find_all(geo_lons, 8.0);
    char aspect_str[128];
    aspect_str[0] = '\0';
    int apos = 0;
    int max_show = 3;
    for (int a = 0; a < aspects.count && a < max_show; a++) {
        aspect_t asp = aspects.aspects[a];
        const char *pa = planet_symbol(asp.planet_a);
        const char *pb = planet_symbol(asp.planet_b);
        const char *sym = aspect_symbol(asp.type);
        int written = snprintf(aspect_str + apos, sizeof(aspect_str) - (size_t)apos,
                               "%s%s%s%s", a > 0 ? " " : "", pa, sym, pb);
        if (written > 0) apos += written;
    }
    if (aspects.count == 0) {
        snprintf(aspect_str, sizeof(aspect_str), "no major aspects");
    }

    /* Speed */
    char speed_str[24];
    if (time_speed == 0.0)
        snprintf(speed_str, sizeof(speed_str), "PAUSED");
    else
        snprintf(speed_str, sizeof(speed_str), "%.0fx", time_speed);

    /* Scale level */
    scale_id_t current_scale = scale_from_log_zoom(log_zoom);
    const char *scale_str = scale_name(current_scale);

    /* Build HTML and push to DOM */
    char html[1024];
    snprintf(html, sizeof(html),
        "%s %s %s<br>"
        "JD %s<br>"
        "GMST %s &middot; LST %s<br>"
        "%s &middot; %s<br>"
        "%s<br>"
        "%s &middot; %s<br>"
        "Aspects: %s<br>"
        "%s &middot; Speed: %s",
        date, time_str, day_name,
        jd_str,
        gmst_str, lst_str,
        asc_str, house_str,
        tzolkin_str,
        sun_str, chinese_str,
        aspect_str,
        scale_str, speed_str);

    EM_ASM({
        var hud = document.getElementById('time-hud');
        if (hud) hud.innerHTML = (function(p){var e=p;while(HEAPU8[e])e++;return new TextDecoder().decode(HEAPU8.subarray(p,e))})($0);
    }, html);
}

#endif /* __EMSCRIPTEN__ */
