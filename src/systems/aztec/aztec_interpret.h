/* aztec_interpret.h — Aztec calendar interpretation data
 *
 * Archetype data for the 20 day signs (veintenas) of the
 * Tonalpohualli (260-day sacred count) and the 4 year bearers
 * of the Calendar Round.  Each day sign carries a patron deity,
 * directional association, and personality quality from
 * Mesoamerican cosmology.
 *
 * Pure functions: no globals, no malloc, no side effects. */

#ifndef TIME_AZTEC_INTERPRET_H
#define TIME_AZTEC_INTERPRET_H

#include "../../ui/i18n.h"  /* i18n_locale_t */

/* Per-day-sign interpretation (20 signs) */
typedef struct {
    int sign;                   /* 0-19 */
    const char *nahuatl;        /* Nahuatl name */
    const char *english;        /* English translation */
    const char *deity;          /* Patron deity */
    const char *direction;      /* Cardinal direction */
    const char *quality;        /* Personality quality */
    const char *brief;          /* One-sentence meaning */
} azi_sign_t;

/* Per-year-bearer interpretation (4 year bearers) */
typedef struct {
    int bearer;                 /* 0-3 */
    const char *nahuatl;        /* Nahuatl name */
    const char *english;        /* English translation */
    const char *direction;      /* Cardinal direction */
    const char *quality;        /* Year quality */
    const char *brief;          /* One-sentence meaning */
} azi_bearer_t;

/* Tiered interpretation result */
typedef struct {
    char glyph[16];             /* T0: day sign abbreviation */
    char glance[128];           /* T1: "4 Jaguar — The warrior's heart" */
    char detail[512];           /* T3: Full narrative */
} aztec_interp_t;

/* Return interpretation for a Tonalpohualli day sign (0-19).
 * Invalid: returns struct with sign=-1, all strings "?". */
azi_sign_t azi_sign_data(int sign);

/* Return interpretation for a year bearer (0-3).
 * 0=Tochtli, 1=Acatl, 2=Tecpatl, 3=Calli.
 * Invalid: returns struct with bearer=-1, all strings "?". */
azi_bearer_t azi_bearer_data(int bearer);

/* Compose tiered interpretation from Aztec calendar components.
 * day_sign: 0-19, day_number: 1-13, year_bearer: 0-3, year_number: 1-13.
 * Invalid day_sign: "?" strings in result. */
aztec_interp_t azi_interpret(int day_sign, int day_number,
                             int year_bearer, int year_number);

/* Returns 20 (number of day signs). */
int azi_sign_count(void);

/* Returns 4 (number of year bearers). */
int azi_bearer_count(void);

/* Locale-aware interpretation. Same as azi_interpret but produces
 * translated output for the given locale (falls back to English).
 * Requires content_i18n module. */
aztec_interp_t azi_interpret_locale(int day_sign, int day_number,
                                    int year_bearer, int year_number,
                                    i18n_locale_t locale);

#endif /* TIME_AZTEC_INTERPRET_H */
