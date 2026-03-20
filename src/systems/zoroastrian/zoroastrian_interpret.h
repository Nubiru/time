/* zoroastrian_interpret.h — Zoroastrian calendar interpretation data
 *
 * Archetype data for the 7 Amesha Spentas, 5 Gatha epagomenal days,
 * and 9 festivals of the Zoroastrian calendar.  Each of the 30
 * days in a month is named for a yazata (divine being), and the
 * first 7 correspond to the Amesha Spentas — the "Holy Immortals"
 * who embody Ahura Mazda's aspects of creation.
 *
 * The Zoroastrian worldview sees time as a 12,000-year cosmic
 * battle between Asha (Truth) and Druj (Lie), ending in the
 * Frashokereti (renovation of the world).
 *
 * Pure functions: no globals, no malloc, no side effects. */

#ifndef TIME_ZOROASTRIAN_INTERPRET_H
#define TIME_ZOROASTRIAN_INTERPRET_H

#include "../../ui/i18n.h"  /* i18n_locale_t */

/* Per-Amesha Spenta interpretation (7 Holy Immortals) */
typedef struct {
    int index;                  /* 0-6 */
    const char *avestan;        /* Avestan name */
    const char *meaning;        /* Translation */
    const char *domain;         /* What they protect */
    const char *element;        /* Associated element of creation */
    const char *virtue;         /* Ethical quality */
    const char *brief;          /* One-sentence meaning */
} zi_amesha_t;

/* Per-Gatha day interpretation (5 epagomenal days) */
typedef struct {
    int day;                    /* 1-5 */
    const char *name;           /* Gatha name */
    const char *theme;          /* Spiritual theme */
    const char *practice;       /* Devotional focus */
    const char *brief;          /* One-sentence meaning */
} zi_gatha_t;

/* Per-festival interpretation (9 festivals, matches zoro_festival_t 1-9) */
typedef struct {
    int festival;               /* 1-9 (Nowruz through Sadeh) */
    const char *name;           /* Festival name */
    const char *theme;          /* Core spiritual theme */
    const char *practice;       /* Traditional observance */
    const char *brief;          /* One-sentence meaning */
} zi_festival_t;

/* Tiered interpretation result */
typedef struct {
    char glyph[16];             /* T0: day name abbreviation (3 chars) */
    char glance[128];           /* T1: "Farvardin day 1 (Hormozd) — Nowruz" */
    char detail[1024];          /* T3: Full narrative */
} zoroastrian_interp_t;

/* Return interpretation for an Amesha Spenta (0-6).
 * Invalid: returns struct with index=-1, all strings "?". */
zi_amesha_t zi_amesha_data(int index);

/* Return interpretation for a Gatha epagomenal day (1-5).
 * Invalid: returns struct with day=-1, all strings "?". */
zi_gatha_t zi_gatha_data(int day);

/* Return interpretation for a Zoroastrian festival (1-9).
 * Matches ZORO_FEST_NOWRUZ through ZORO_FEST_SADEH.
 * Invalid: returns struct with festival=-1, all strings "?". */
zi_festival_t zi_festival_data(int festival);

/* Compose tiered interpretation from Zoroastrian calendar components.
 * month: 1-12 for regular months, 13 for Gatha days.
 * day: 1-30 for months 1-12, 1-5 for month 13.
 * festival: 1-9 if a festival is active, 0 if none.
 * Invalid month: "?" strings in result. */
zoroastrian_interp_t zi_interpret(int month, int day, int festival);

/* Locale-aware interpretation. Same as zi_interpret but produces
 * translated output for the given locale (falls back to English).
 * Requires content_i18n module. */
zoroastrian_interp_t zi_interpret_locale(int month, int day, int festival,
                                         i18n_locale_t locale);

/* Returns 7 (number of Amesha Spentas). */
int zi_amesha_count(void);

/* Returns 5 (number of Gatha days). */
int zi_gatha_count(void);

/* Returns 9 (number of festivals). */
int zi_festival_count(void);

#endif /* TIME_ZOROASTRIAN_INTERPRET_H */
