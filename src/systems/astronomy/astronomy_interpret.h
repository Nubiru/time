/* astronomy_interpret.h — Astronomical interpretation data
 *
 * Archetype data for the 8 planets and 8 lunar phases.  Each
 * planet carries mythological and psychological significance
 * drawn from Babylonian, Greek, and Roman tradition — the same
 * associations that gave us the days of the week and the roots
 * of Western astrology.
 *
 * Pure functions: no globals, no malloc, no side effects. */

#ifndef TIME_ASTRONOMY_INTERPRET_H
#define TIME_ASTRONOMY_INTERPRET_H

#include "../../ui/i18n.h"  /* i18n_locale_t */

/* Per-planet interpretation (8 planets) */
typedef struct {
    int planet;                 /* 0-7 (Mercury through Neptune) */
    const char *name;           /* "Mercury", etc. */
    const char *deity;          /* Mythological deity */
    const char *domain;         /* What the planet governs symbolically */
    const char *archetype;      /* Jungian/psychological archetype */
    const char *brief;          /* One-sentence meaning */
} ai_planet_t;

/* Per-moon-phase interpretation (8 phases) */
typedef struct {
    int phase;                  /* 0-7 */
    const char *name;           /* "New Moon", etc. */
    const char *theme;          /* Spiritual/psychological theme */
    const char *guidance;       /* What this phase supports */
    const char *brief;          /* One-sentence meaning */
} ai_phase_t;

/* Tiered interpretation result */
typedef struct {
    char glyph[16];             /* T0: planet abbreviation (3 chars) */
    char glance[128];           /* T1: "Mars (retrograde) — The Warrior turns inward" */
    char detail[512];           /* T3: Full narrative */
} astronomy_interp_t;

/* Return interpretation for a planet (0-7).
 * 0=Mercury, 1=Venus, 2=Earth, 3=Mars, 4=Jupiter, 5=Saturn,
 * 6=Uranus, 7=Neptune.
 * Invalid: returns struct with planet=-1, all strings "?". */
ai_planet_t ai_planet_data(int planet);

/* Return interpretation for a lunar phase (0-7).
 * 0=New, 1=Waxing Crescent, 2=First Quarter, 3=Waxing Gibbous,
 * 4=Full, 5=Waning Gibbous, 6=Last Quarter, 7=Waning Crescent.
 * Invalid: returns struct with phase=-1, all strings "?". */
ai_phase_t ai_phase_data(int phase);

/* Compose tiered interpretation from astronomical components.
 * planet: 0-7, is_retrograde: 0 or 1, moon_phase: 0-7.
 * Invalid planet: "?" strings in result. */
astronomy_interp_t ai_astro_interpret(int planet, int is_retrograde,
                                      int moon_phase);

/* Locale-aware interpretation. Same as ai_astro_interpret but produces
 * translated output for the given locale. */
astronomy_interp_t ai_astro_interpret_locale(int planet, int is_retrograde,
                                             int moon_phase,
                                             i18n_locale_t locale);

/* Returns 8 (number of planets). */
int ai_planet_count(void);

/* Returns 8 (number of lunar phases). */
int ai_phase_count(void);

#endif /* TIME_ASTRONOMY_INTERPRET_H */
