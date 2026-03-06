#include "scale_hud.h"

/*
 * Scale-dependent HUD visibility matrix.
 *
 * Scale 0 (Personal):  All knowledge systems, astrology details, houses
 * Scale 1 (Earth):     Calendar systems, sun sign, Chinese, sidereal
 * Scale 2 (Inner):     Sun sign, aspects, astrology, Tzolkin
 * Scale 3 (Solar):     Core info: JD, sidereal, sun sign, Chinese
 * Scale 4 (Local):     Transitional: JD, basic info
 * Scale 5 (Galaxy):    Cosmic time, JD
 * Scale 6 (Universe):  Cosmic time, JD
 *
 * Always visible at all scales: gregorian, speed, scale_name
 */

static const char *MODE_NAMES[7] = {
    "Personal",
    "Earth",
    "Inner Solar",
    "Solar System",
    "Local",
    "Galaxy",
    "Universe"
};

hud_visibility_t hud_visibility_at_scale(int scale_id)
{
    /* Default to solar (3) for invalid */
    if (scale_id < 0 || scale_id > 6) scale_id = 3;

    hud_visibility_t v;

    /* Always visible */
    v.show_gregorian  = 1;
    v.show_speed      = 1;
    v.show_scale_name = 1;

    /* Scale-dependent */
    switch (scale_id) {
    case 0: /* Personal */
        v.show_jd        = 1;
        v.show_sidereal  = 1;
        v.show_sun_sign  = 1;
        v.show_tzolkin   = 1;
        v.show_iching    = 1;
        v.show_chinese   = 1;
        v.show_hd        = 1;
        v.show_astrology = 1;
        v.show_cosmic    = 0;
        v.show_aspects   = 1;
        v.show_houses    = 1;
        break;

    case 1: /* Earth */
        v.show_jd        = 1;
        v.show_sidereal  = 1;
        v.show_sun_sign  = 1;
        v.show_tzolkin   = 1;
        v.show_iching    = 1;
        v.show_chinese   = 1;
        v.show_hd        = 1;
        v.show_astrology = 1;
        v.show_cosmic    = 0;
        v.show_aspects   = 1;
        v.show_houses    = 1;
        break;

    case 2: /* Inner Solar */
        v.show_jd        = 1;
        v.show_sidereal  = 1;
        v.show_sun_sign  = 1;
        v.show_tzolkin   = 1;
        v.show_iching    = 0;
        v.show_chinese   = 1;
        v.show_hd        = 0;
        v.show_astrology = 1;
        v.show_cosmic    = 0;
        v.show_aspects   = 1;
        v.show_houses    = 0;
        break;

    case 3: /* Solar System (default) */
        v.show_jd        = 1;
        v.show_sidereal  = 1;
        v.show_sun_sign  = 1;
        v.show_tzolkin   = 1;
        v.show_iching    = 0;
        v.show_chinese   = 1;
        v.show_hd        = 0;
        v.show_astrology = 0;
        v.show_cosmic    = 0;
        v.show_aspects   = 0;
        v.show_houses    = 0;
        break;

    case 4: /* Local */
        v.show_jd        = 1;
        v.show_sidereal  = 0;
        v.show_sun_sign  = 0;
        v.show_tzolkin   = 0;
        v.show_iching    = 0;
        v.show_chinese   = 0;
        v.show_hd        = 0;
        v.show_astrology = 0;
        v.show_cosmic    = 0;
        v.show_aspects   = 0;
        v.show_houses    = 0;
        break;

    case 5: /* Galaxy */
        v.show_jd        = 1;
        v.show_sidereal  = 0;
        v.show_sun_sign  = 0;
        v.show_tzolkin   = 0;
        v.show_iching    = 0;
        v.show_chinese   = 0;
        v.show_hd        = 0;
        v.show_astrology = 0;
        v.show_cosmic    = 1;
        v.show_aspects   = 0;
        v.show_houses    = 0;
        break;

    case 6: /* Universe */
        v.show_jd        = 1;
        v.show_sidereal  = 0;
        v.show_sun_sign  = 0;
        v.show_tzolkin   = 0;
        v.show_iching    = 0;
        v.show_chinese   = 0;
        v.show_hd        = 0;
        v.show_astrology = 0;
        v.show_cosmic    = 1;
        v.show_aspects   = 0;
        v.show_houses    = 0;
        break;

    default:
        /* Should not reach here due to clamping above */
        v.show_jd        = 1;
        v.show_sidereal  = 1;
        v.show_sun_sign  = 1;
        v.show_tzolkin   = 0;
        v.show_iching    = 0;
        v.show_chinese   = 0;
        v.show_hd        = 0;
        v.show_astrology = 0;
        v.show_cosmic    = 0;
        v.show_aspects   = 0;
        v.show_houses    = 0;
        break;
    }

    return v;
}

int hud_visible_count(hud_visibility_t vis)
{
    return vis.show_gregorian + vis.show_jd + vis.show_sidereal +
           vis.show_speed + vis.show_scale_name + vis.show_sun_sign +
           vis.show_tzolkin + vis.show_iching + vis.show_chinese +
           vis.show_hd + vis.show_astrology + vis.show_cosmic +
           vis.show_aspects + vis.show_houses;
}

const char *hud_mode_name(int scale_id)
{
    if (scale_id < 0 || scale_id > 6) return "Unknown";
    return MODE_NAMES[scale_id];
}

hud_visibility_t hud_visibility_merge(hud_visibility_t a, hud_visibility_t b)
{
    hud_visibility_t m;
    m.show_gregorian  = a.show_gregorian  || b.show_gregorian;
    m.show_jd         = a.show_jd         || b.show_jd;
    m.show_sidereal   = a.show_sidereal   || b.show_sidereal;
    m.show_speed      = a.show_speed      || b.show_speed;
    m.show_scale_name = a.show_scale_name || b.show_scale_name;
    m.show_sun_sign   = a.show_sun_sign   || b.show_sun_sign;
    m.show_tzolkin    = a.show_tzolkin    || b.show_tzolkin;
    m.show_iching     = a.show_iching     || b.show_iching;
    m.show_chinese    = a.show_chinese    || b.show_chinese;
    m.show_hd         = a.show_hd         || b.show_hd;
    m.show_astrology  = a.show_astrology  || b.show_astrology;
    m.show_cosmic     = a.show_cosmic     || b.show_cosmic;
    m.show_aspects    = a.show_aspects    || b.show_aspects;
    m.show_houses     = a.show_houses     || b.show_houses;
    return m;
}

hud_visibility_t hud_visibility_default(void)
{
    return hud_visibility_at_scale(3);
}
