/* chakra.h — 7-chakra knowledge system data module
 *
 * Pure data module for the traditional Hindu/yogic 7-chakra system.
 * Each chakra carries Sanskrit name, English name, body location,
 * bija mantra, element, color (normalized RGB), Solfeggio frequency,
 * lotus petal count, and planetary correspondence.
 *
 * All functions are pure: no globals, no malloc, no side effects.
 *
 * Sources: Traditional Hindu/yogic chakra system;
 * Solfeggio frequencies from Gregorian chant tradition. */

#ifndef TIME_CHAKRA_H
#define TIME_CHAKRA_H

#include <stdbool.h>

/* The 7 chakras, root to crown */
typedef enum {
    CHAKRA_MULADHARA = 0,   /* Root */
    CHAKRA_SVADHISTHANA,    /* Sacral */
    CHAKRA_MANIPURA,        /* Solar Plexus */
    CHAKRA_ANAHATA,         /* Heart */
    CHAKRA_VISHUDDHA,       /* Throat */
    CHAKRA_AJNA,            /* Third Eye */
    CHAKRA_SAHASRARA,       /* Crown */
    CHAKRA_COUNT
} chakra_t;

/* Element association */
typedef enum {
    CHAKRA_ELEM_EARTH = 0,
    CHAKRA_ELEM_WATER,
    CHAKRA_ELEM_FIRE,
    CHAKRA_ELEM_AIR,
    CHAKRA_ELEM_ETHER,
    CHAKRA_ELEM_LIGHT,
    CHAKRA_ELEM_THOUGHT,
    CHAKRA_ELEM_COUNT
} chakra_element_t;

/* Color as normalized RGB */
typedef struct {
    float r, g, b;
} chakra_color_t;

/* Chakra information */
typedef struct {
    chakra_t id;
    const char *sanskrit_name;    /* e.g. "Muladhara" */
    const char *english_name;     /* e.g. "Root" */
    const char *location;         /* e.g. "Base of spine" */
    const char *bija_mantra;      /* Seed syllable, e.g. "LAM" */
    chakra_element_t element;
    chakra_color_t color;         /* Traditional color */
    double frequency_hz;          /* Solfeggio frequency */
    int petal_count;              /* Lotus petal count */
    const char *planet;           /* Associated planet */
} chakra_info_t;

/* Get full info for a chakra. Returns zeroed struct for invalid. */
chakra_info_t chakra_info(chakra_t chakra);

/* Sanskrit name. Returns "?" for invalid. */
const char *chakra_sanskrit_name(chakra_t chakra);

/* English name. Returns "?" for invalid. */
const char *chakra_english_name(chakra_t chakra);

/* Body location. Returns "?" for invalid. */
const char *chakra_location(chakra_t chakra);

/* Bija mantra. Returns "?" for invalid. */
const char *chakra_bija(chakra_t chakra);

/* Element for a chakra. Returns CHAKRA_ELEM_COUNT for invalid. */
chakra_element_t chakra_element(chakra_t chakra);

/* Element name string. Returns "?" for invalid. */
const char *chakra_element_name(chakra_element_t element);

/* Traditional color as normalized RGB. Returns {0,0,0} for invalid. */
chakra_color_t chakra_color(chakra_t chakra);

/* Solfeggio frequency in Hz. Returns 0.0 for invalid. */
double chakra_frequency(chakra_t chakra);

/* Lotus petal count. Returns 0 for invalid. */
int chakra_petals(chakra_t chakra);

/* Associated planet name. Returns "?" for invalid. */
const char *chakra_planet(chakra_t chakra);

/* Number of chakras (always 7). */
int chakra_count(void);

/* Find chakra by frequency (nearest match within tolerance_hz).
 * Returns -1 if no chakra frequency is within tolerance. */
int chakra_from_frequency(double freq_hz, double tolerance_hz);

/* Find chakra by color (nearest match by Euclidean distance in RGB space). */
chakra_t chakra_from_color(float r, float g, float b);

#endif /* TIME_CHAKRA_H */
