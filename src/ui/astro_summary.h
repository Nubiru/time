#ifndef TIME_ASTRO_SUMMARY_H
#define TIME_ASTRO_SUMMARY_H

#include <stddef.h>

#define ASTRO_SUMMARY_PLANETS     8
#define ASTRO_SUMMARY_MAX_ASPECTS 28

typedef struct {
    int planet_index;       /* 0-7 */
    int sign;               /* 0-11 */
    double degree_in_sign;  /* 0.0-30.0 */
    int dignity;            /* dignity_type_t value */
    char formatted[48];     /* "Sun Leo 15.3 deg" */
    char dignity_label[16]; /* "Domicile" */
} astro_planet_entry_t;

typedef struct {
    int planet_a;           /* 0-7 */
    int planet_b;           /* 0-7 */
    int type;               /* aspect_type_t value */
    double orb;             /* degrees */
    char formatted[64];     /* "sym_a asp sym_b orb" */
} astro_aspect_entry_t;

typedef struct {
    astro_planet_entry_t planets[ASTRO_SUMMARY_PLANETS];
    int planet_count;

    astro_aspect_entry_t aspects[ASTRO_SUMMARY_MAX_ASPECTS];
    int aspect_count;

    int ascendant_sign;
    double ascendant_degree;
    char ascendant_str[48]; /* "ASC Cancer 12.5 deg" */

    char element_str[64];   /* "Fire:3 Earth:2 Air:3 Water:2" */
    char modality_str[64];  /* "Cardinal:3 Fixed:4 Mutable:3" */

    int dignity_total;      /* sum of all planet dignity scores */
} astro_summary_t;

/* Build complete astrology summary from 8 geocentric longitudes + ascendant. */
astro_summary_t astro_summary_build(const double longitudes[8],
                                     double ascendant_lon,
                                     double max_orb);

/* Map planet index (0-7, geocentric) to dignity planet index (0-9).
 * Index 2 (Earth) maps to DIGNITY_SUN (8) in geocentric convention.
 * Returns -1 for out-of-range input. */
int astro_summary_dignity_index(int planet_index);

/* Format a single planet entry: "Earth Leo 15.3 deg" */
void astro_summary_fmt_planet(int planet_index, double longitude,
                               char *buf, size_t sz);

/* Format ascendant: "ASC Cancer 12.5 deg" */
void astro_summary_fmt_ascendant(double ascendant_lon, char *buf, size_t sz);

#endif
