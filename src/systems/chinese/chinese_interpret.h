/* chinese_interpret.h — Chinese zodiac interpretation data
 *
 * Archetype data for the 12 animals and 5 elements of the Chinese
 * sexagenary cycle.  Composition function produces tiered interpretation
 * text (glyph / glance / detail).
 *
 * The Chinese calendar assigns personality through birth YEAR rather
 * than day (Tzolkin) or month (astrology) — a complementary perspective.
 *
 * Pure functions: no globals, no malloc, no side effects. */

#ifndef TIME_CHINESE_INTERPRET_H
#define TIME_CHINESE_INTERPRET_H

#include "../../ui/i18n.h"  /* i18n_locale_t */

/* Per-animal archetype (12 animals) */
typedef struct {
    int animal;                 /* 0-11 */
    const char *name;           /* "Rat", "Ox", etc. */
    const char *archetype;      /* The animal's essential nature */
    const char *strengths;      /* Key positive traits */
    const char *challenges;     /* Shadow traits */
    const char *compatibility;  /* Best compatible animals */
    const char *brief;          /* One-sentence meaning */
} ci_animal_t;

/* Per-element interpretation (5 elements) */
typedef struct {
    int element;                /* 0-4 (Wood, Fire, Earth, Metal, Water) */
    const char *name;
    const char *nature;         /* Essential nature */
    const char *season;         /* Associated season */
    const char *expression;     /* How it manifests */
} ci_element_t;

/* Tiered interpretation result */
typedef struct {
    char glyph[16];             /* T0: animal abbreviation (first 3 chars) */
    char glance[128];           /* T1: "Water Tiger (Yang) — ..." */
    char detail[512];           /* T3: Full narrative */
} chinese_interp_t;

/* Return archetype data for a Chinese zodiac animal (0-11).
 * Invalid index: returns struct with animal=-1, all strings "?". */
ci_animal_t ci_animal_data(int animal);

/* Return interpretation data for a Chinese element (0-4).
 * Invalid index: returns struct with element=-1, all strings "?". */
ci_element_t ci_element_data(int element);

/* Compose tiered interpretation from year components.
 * animal: 0-11, element: 0-4, polarity: 0=Yang / 1=Yin, cycle_year: 1-60.
 * Invalid animal or element: "?" strings in result. */
chinese_interp_t ci_interpret(int animal, int element,
                              int polarity, int cycle_year);

/* Locale-aware interpretation. Same as ci_interpret but produces
 * translated output for the given locale (falls back to English).
 * Requires content_i18n module. */
chinese_interp_t ci_interpret_locale(int animal, int element,
                                     int polarity, int cycle_year,
                                     i18n_locale_t locale);

/* Returns 12 (number of zodiac animals). */
int ci_animal_count(void);

/* Returns 5 (number of elements). */
int ci_element_count(void);

#endif /* TIME_CHINESE_INTERPRET_H */
