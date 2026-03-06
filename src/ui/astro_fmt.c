#include "astro_fmt.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

/* Zodiac sign names */
static const char *SIGN_NAMES[12] = {
    "Aries", "Taurus", "Gemini", "Cancer",
    "Leo", "Virgo", "Libra", "Scorpio",
    "Sagittarius", "Capricorn", "Aquarius", "Pisces"
};

/* Zodiac sign Unicode symbols */
static const char *SIGN_SYMBOLS[12] = {
    "\xe2\x99\x88", "\xe2\x99\x89", "\xe2\x99\x8a", "\xe2\x99\x8b",
    "\xe2\x99\x8c", "\xe2\x99\x8d", "\xe2\x99\x8e", "\xe2\x99\x8f",
    "\xe2\x99\x90", "\xe2\x99\x91", "\xe2\x99\x92", "\xe2\x99\x93"
};

/* Elements: Fire(0,4,8), Earth(1,5,9), Air(2,6,10), Water(3,7,11) */
static const char *ELEMENTS[4] = { "Fire", "Earth", "Air", "Water" };

/* Modalities: Cardinal(0,3,6,9), Fixed(1,4,7,10), Mutable(2,5,8,11) */
static const char *MODALITIES[3] = { "Cardinal", "Fixed", "Mutable" };

static double normalize_lon(double lon)
{
    lon = fmod(lon, 360.0);
    if (lon < 0.0) lon += 360.0;
    return lon;
}

void astro_fmt_position(double ecliptic_lon, char *buf, size_t sz)
{
    if (!buf || sz == 0) return;

    double lon = normalize_lon(ecliptic_lon);
    int sign = (int)(lon / 30.0) % 12;
    double deg_in_sign = fmod(lon, 30.0);
    int deg = (int)deg_in_sign;
    int min = (int)((deg_in_sign - deg) * 60.0);

    snprintf(buf, sz, "%d\xc2\xb0%02d' %s", deg, min, SIGN_NAMES[sign]);
}

void astro_fmt_planet_position(const char *planet_symbol, double ecliptic_lon,
                               char *buf, size_t sz)
{
    if (!buf || sz == 0) return;

    double lon = normalize_lon(ecliptic_lon);
    int sign = (int)(lon / 30.0) % 12;
    double deg_in_sign = fmod(lon, 30.0);
    int deg = (int)deg_in_sign;
    int min = (int)((deg_in_sign - deg) * 60.0);

    snprintf(buf, sz, "%s %d\xc2\xb0%02d' %s",
             planet_symbol ? planet_symbol : "?",
             deg, min, SIGN_SYMBOLS[sign]);
}

void astro_fmt_aspect(const char *sym_a, const char *aspect_sym,
                      const char *sym_b, double orb, char *buf, size_t sz)
{
    if (!buf || sz == 0) return;

    snprintf(buf, sz, "%s %s %s %.1f\xc2\xb0",
             sym_a ? sym_a : "?",
             aspect_sym ? aspect_sym : "?",
             sym_b ? sym_b : "?",
             orb);
}

const char *astro_sign_element(int sign)
{
    if (sign < 0 || sign > 11) return "?";
    return ELEMENTS[sign % 4];
}

const char *astro_sign_modality(int sign)
{
    if (sign < 0 || sign > 11) return "?";
    return MODALITIES[sign % 3];
}

const char *astro_sign_polarity(int sign)
{
    if (sign < 0 || sign > 11) return "?";
    return (sign % 2 == 0) ? "Yang" : "Yin";
}

void astro_element_balance(const int planet_signs[10],
                           int *fire, int *earth, int *air, int *water)
{
    *fire = 0; *earth = 0; *air = 0; *water = 0;
    for (int i = 0; i < 10; i++) {
        int s = planet_signs[i];
        if (s < 0 || s > 11) continue;
        switch (s % 4) {
        case 0: (*fire)++;  break;
        case 1: (*earth)++; break;
        case 2: (*air)++;   break;
        case 3: (*water)++; break;
        }
    }
}

void astro_modality_balance(const int planet_signs[10],
                            int *cardinal, int *fixed, int *mutable_count)
{
    *cardinal = 0; *fixed = 0; *mutable_count = 0;
    for (int i = 0; i < 10; i++) {
        int s = planet_signs[i];
        if (s < 0 || s > 11) continue;
        switch (s % 3) {
        case 0: (*cardinal)++;     break;
        case 1: (*fixed)++;        break;
        case 2: (*mutable_count)++; break;
        }
    }
}

void astro_fmt_element_balance(const int planet_signs[10], char *buf, size_t sz)
{
    if (!buf || sz == 0) return;
    int f, e, a, w;
    astro_element_balance(planet_signs, &f, &e, &a, &w);
    snprintf(buf, sz, "Fire:%d Earth:%d Air:%d Water:%d", f, e, a, w);
}

void astro_fmt_modality_balance(const int planet_signs[10], char *buf, size_t sz)
{
    if (!buf || sz == 0) return;
    int c, f, m;
    astro_modality_balance(planet_signs, &c, &f, &m);
    snprintf(buf, sz, "Cardinal:%d Fixed:%d Mutable:%d", c, f, m);
}
