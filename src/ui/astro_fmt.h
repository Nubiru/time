#ifndef TIME_ASTRO_FMT_H
#define TIME_ASTRO_FMT_H

#include <stddef.h>

/* Format ecliptic position: "15°42' Aries" */
void astro_fmt_position(double ecliptic_lon, char *buf, size_t sz);

/* Format planet position with symbol prefix: "☉ 15°42' ♈" */
void astro_fmt_planet_position(const char *planet_symbol, double ecliptic_lon,
                               char *buf, size_t sz);

/* Format aspect: "☉ □ ☽ 2.3°" */
void astro_fmt_aspect(const char *sym_a, const char *aspect_sym,
                      const char *sym_b, double orb, char *buf, size_t sz);

/* Zodiac element for sign index (0-11). Returns "Fire"/"Earth"/"Air"/"Water" */
const char *astro_sign_element(int sign);

/* Zodiac modality for sign (0-11). Returns "Cardinal"/"Fixed"/"Mutable" */
const char *astro_sign_modality(int sign);

/* Zodiac polarity for sign (0-11). Returns "Yang"/"Yin" */
const char *astro_sign_polarity(int sign);

/* Element balance: count placements in each element.
 * planet_signs[0..9] = sign index (0-11). */
void astro_element_balance(const int planet_signs[10],
                           int *fire, int *earth, int *air, int *water);

/* Modality balance: count placements in each modality. */
void astro_modality_balance(const int planet_signs[10],
                            int *cardinal, int *fixed, int *mutable_count);

/* Format element balance: "Fire:3 Earth:2 Air:3 Water:2" */
void astro_fmt_element_balance(const int planet_signs[10], char *buf, size_t sz);

/* Format modality balance: "Cardinal:3 Fixed:4 Mutable:3" */
void astro_fmt_modality_balance(const int planet_signs[10], char *buf, size_t sz);

#endif
