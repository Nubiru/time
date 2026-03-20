/* pawukon_interpret.h — Balinese Pawukon calendar interpretation data
 *
 * Archetype data for the 30 wuku (weeks) and key ceremonial days
 * of the 210-day Balinese Pawukon cycle.  The Pawukon is unique:
 * 10 concurrent cycles of 1-10 days each create a complex
 * auspiciousness matrix.  Tumpek days and Kajeng Kliwon are
 * the most significant recurring ceremonies.
 *
 * Pure functions: no globals, no malloc, no side effects. */

#ifndef TIME_PAWUKON_INTERPRET_H
#define TIME_PAWUKON_INTERPRET_H

#include "../../ui/i18n.h"  /* i18n_locale_t */

/* Per-wuku interpretation (30 wuku) */
typedef struct {
    int wuku;                   /* 0-29 */
    const char *name;           /* Balinese name */
    const char *deity;          /* Presiding deity */
    const char *quality;        /* Spiritual quality */
    const char *brief;          /* One-sentence meaning */
} pwi_wuku_t;

/* Tiered interpretation result */
typedef struct {
    char glyph[16];             /* T0: wuku abbreviation */
    char glance[128];           /* T1: "Sinta day 3 — Kajeng Kliwon" */
    char detail[512];           /* T3: Full narrative */
} pawukon_interp_t;

/* Return interpretation for a wuku (0-29).
 * Invalid: returns struct with wuku=-1, all strings "?". */
pwi_wuku_t pwi_wuku_data(int wuku);

/* Compose tiered interpretation.
 * wuku: 0-29, wuku_day: 1-7, urip: spiritual weight sum,
 * is_tumpek: 1 if Tumpek day, is_kajeng_kliwon: 1 if Kajeng Kliwon.
 * Invalid wuku: "?" strings. */
pawukon_interp_t pwi_interpret(int wuku, int wuku_day, int urip,
                               int is_tumpek, int is_kajeng_kliwon);

int pwi_wuku_count(void);      /* Returns 30 */

/* Locale-aware interpretation. Same as pwi_interpret but produces
 * translated output for the given locale (falls back to English).
 * Requires content_i18n module. */
pawukon_interp_t pwi_interpret_locale(int wuku, int wuku_day, int urip,
                                      int is_tumpek, int is_kajeng_kliwon,
                                      i18n_locale_t locale);

#endif /* TIME_PAWUKON_INTERPRET_H */
