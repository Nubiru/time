#include "astro_summary.h"
#include "astro_fmt.h"
#include "../systems/astrology/zodiac.h"
#include "../systems/astrology/dignity.h"
#include "../systems/astrology/aspects.h"
#include "../systems/astronomy/planets.h"

#include <stdio.h>
#include <string.h>

/* Dignity index mapping: geocentric planet index (0-7) -> dignity planet index.
 * Index 2 (Earth slot) maps to DIGNITY_SUN (8) in geocentric convention. */
static const int DIGNITY_MAP[8] = {
    0,  /* 0: Mercury -> DIGNITY_MERCURY */
    1,  /* 1: Venus   -> DIGNITY_VENUS   */
    8,  /* 2: Earth/Sun -> DIGNITY_SUN   */
    3,  /* 3: Mars    -> DIGNITY_MARS    */
    4,  /* 4: Jupiter -> DIGNITY_JUPITER */
    5,  /* 5: Saturn  -> DIGNITY_SATURN  */
    6,  /* 6: Uranus  -> DIGNITY_URANUS  */
    7   /* 7: Neptune -> DIGNITY_NEPTUNE */
};

int astro_summary_dignity_index(int planet_index)
{
    if (planet_index < 0 || planet_index > 7) return -1;
    return DIGNITY_MAP[planet_index];
}

void astro_summary_fmt_planet(int planet_index, double longitude,
                               char *buf, size_t sz)
{
    if (!buf || sz == 0) return;

    const char *name = planet_name(planet_index);
    int sign = zodiac_sign(longitude);
    double deg = zodiac_degree(longitude);
    const char *sym = zodiac_sign_symbol(sign);
    const char *sign_name = zodiac_sign_name(sign);

    snprintf(buf, sz, "%s %s %s %.1f\xc2\xb0", name, sym, sign_name, deg);
}

void astro_summary_fmt_ascendant(double ascendant_lon, char *buf, size_t sz)
{
    if (!buf || sz == 0) return;

    int sign = zodiac_sign(ascendant_lon);
    double deg = zodiac_degree(ascendant_lon);
    const char *sym = zodiac_sign_symbol(sign);
    const char *sign_name = zodiac_sign_name(sign);

    snprintf(buf, sz, "ASC %s %s %.1f\xc2\xb0", sym, sign_name, deg);
}

astro_summary_t astro_summary_build(const double longitudes[8],
                                     double ascendant_lon,
                                     double max_orb)
{
    astro_summary_t s;
    memset(&s, 0, sizeof(s));

    /* --- Planet entries --- */
    s.planet_count = ASTRO_SUMMARY_PLANETS;
    for (int i = 0; i < ASTRO_SUMMARY_PLANETS; i++) {
        astro_planet_entry_t *p = &s.planets[i];
        p->planet_index = i;
        p->sign = zodiac_sign(longitudes[i]);
        p->degree_in_sign = zodiac_degree(longitudes[i]);

        /* Dignity lookup using mapped index */
        int dig_idx = astro_summary_dignity_index(i);
        dignity_result_t dr = planet_dignity(dig_idx, p->sign);
        p->dignity = (int)dr.dignity;
        snprintf(p->dignity_label, sizeof(p->dignity_label),
                 "%s", dignity_name(dr.dignity));

        /* Formatted string */
        astro_summary_fmt_planet(i, longitudes[i],
                                  p->formatted, sizeof(p->formatted));
    }

    /* --- Aspects --- */
    aspect_list_t alist = aspects_find_all(longitudes, max_orb);
    s.aspect_count = alist.count;
    for (int i = 0; i < alist.count; i++) {
        astro_aspect_entry_t *ae = &s.aspects[i];
        ae->planet_a = alist.aspects[i].planet_a;
        ae->planet_b = alist.aspects[i].planet_b;
        ae->type = (int)alist.aspects[i].type;
        ae->orb = alist.aspects[i].orb;

        astro_fmt_aspect(
            planet_symbol(ae->planet_a),
            aspect_symbol(alist.aspects[i].type),
            planet_symbol(ae->planet_b),
            ae->orb,
            ae->formatted, sizeof(ae->formatted));
    }

    /* --- Ascendant --- */
    s.ascendant_sign = zodiac_sign(ascendant_lon);
    s.ascendant_degree = zodiac_degree(ascendant_lon);
    astro_summary_fmt_ascendant(ascendant_lon,
                                 s.ascendant_str, sizeof(s.ascendant_str));

    /* --- Balance strings ---
     * Build a 10-element planet_signs array for the balance functions.
     * Indices 0-7 match dignity system indices 0-7.
     * Index 8 = Sun, Index 9 = Moon.
     * We map our geocentric data into this array:
     *   [0] Mercury sign, [1] Venus sign, [2] Earth = -1 (no astro significance)
     *   [3] Mars sign, [4] Jupiter sign, [5] Saturn sign,
     *   [6] Uranus sign, [7] Neptune sign,
     *   [8] Sun sign (from longitudes[2]), [9] Moon = -1 (not available) */
    int planet_signs[10];
    for (int i = 0; i < 8; i++) {
        planet_signs[i] = zodiac_sign(longitudes[i]);
    }
    /* Override index 2 (Earth) to -1: no astrological dignity for Earth */
    planet_signs[2] = -1;
    /* Index 8 = Sun: use the sign from longitudes[2] (geocentric Sun) */
    planet_signs[8] = zodiac_sign(longitudes[2]);
    /* Index 9 = Moon: not available in 8-planet input */
    planet_signs[9] = -1;

    astro_fmt_element_balance(planet_signs,
                               s.element_str, sizeof(s.element_str));
    astro_fmt_modality_balance(planet_signs,
                                s.modality_str, sizeof(s.modality_str));

    /* --- Dignity total ---
     * Sum dignity scores for all 8 geocentric planets using mapped indices. */
    s.dignity_total = 0;
    for (int i = 0; i < ASTRO_SUMMARY_PLANETS; i++) {
        int dig_idx = astro_summary_dignity_index(i);
        dignity_result_t dr = planet_dignity(dig_idx, s.planets[i].sign);
        s.dignity_total += dr.score;
    }

    return s;
}
