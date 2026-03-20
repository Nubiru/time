/* persian_interpret.h — Persian/Solar Hijri calendar interpretation data
 *
 * Archetype data for the 12 months and 4 major festivals of the
 * Persian calendar.  Each month name derives from ancient Avestan
 * (Zoroastrian) yazatas — the same names used for 3000+ years.
 * The Persian calendar is the most astronomically precise solar
 * calendar in use, with its 2820-year grand cycle.
 *
 * Pure functions: no globals, no malloc, no side effects. */

#ifndef TIME_PERSIAN_INTERPRET_H
#define TIME_PERSIAN_INTERPRET_H

#include "../../ui/i18n.h"  /* i18n_locale_t */

/* Per-month interpretation (12 months) */
typedef struct {
    int month;                  /* 1-12 */
    const char *name;           /* "Farvardin", etc. */
    const char *avestan;        /* Original Avestan deity/concept */
    const char *meaning;        /* What the name means */
    const char *season;         /* Seasonal quality */
    const char *brief;          /* One-sentence meaning */
} pi_month_t;

/* Per-festival interpretation (4 festivals) */
typedef struct {
    int festival;               /* 1-4 */
    const char *name;           /* "Nowruz", etc. */
    const char *theme;          /* Core spiritual/cultural theme */
    const char *practice;       /* Traditional observance */
    const char *brief;          /* One-sentence meaning */
} pi_festival_t;

/* Tiered interpretation result */
typedef struct {
    char glyph[16];             /* T0: month abbreviation (3 chars) */
    char glance[128];           /* T1: "Farvardin 1 — Nowruz (New Day)" */
    char detail[512];           /* T3: Full narrative */
} persian_interp_t;

/* Return interpretation for a Persian month (1-12).
 * Invalid: returns struct with month=-1, all strings "?". */
pi_month_t pi_month_data(int month);

/* Return interpretation for a Persian festival (1-4).
 * 1=Nowruz, 2=Sizdah Bedar, 3=Yalda, 4=Chaharshanbe Suri.
 * Invalid: returns struct with festival=-1, all strings "?". */
pi_festival_t pi_festival_data(int festival);

/* Compose tiered interpretation from Persian calendar components.
 * month: 1-12, day: 1-31, festival: 1-4 if active, 0 if none.
 * Invalid month: "?" strings in result. */
persian_interp_t pi_interpret(int month, int day, int festival);

/* Returns 12 (number of months). */
int pi_month_count(void);

/* Returns 4 (number of major festivals). */
int pi_festival_count(void);

/* Locale-aware interpretation. Same as pi_interpret but produces
 * translated output for the given locale (falls back to English).
 * Requires content_i18n module. */
persian_interp_t pi_interpret_locale(int month, int day, int festival,
                                     i18n_locale_t locale);

#endif /* TIME_PERSIAN_INTERPRET_H */
