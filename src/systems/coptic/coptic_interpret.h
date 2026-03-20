/* coptic_interpret.h — Coptic calendar interpretation data
 *
 * Archetype data for the 13 months and 6 major feasts of the
 * Coptic (Alexandrian) calendar.  The Coptic calendar preserves
 * the ancient Egyptian month structure (12 x 30 + epagomenal)
 * but adds a Christian liturgical layer.  The Anno Martyrum era
 * commemorates Diocletian's persecution (284 CE).
 *
 * Pure functions: no globals, no malloc, no side effects. */

#ifndef TIME_COPTIC_INTERPRET_H
#define TIME_COPTIC_INTERPRET_H

#include "../../ui/i18n.h"  /* i18n_locale_t */

/* Per-month interpretation (13 months: 12 regular + Nasie) */
typedef struct {
    int month;                  /* 1-13 (Thout=1 through Nasie=13) */
    const char *name;           /* "Thout", "Paopi", etc. */
    const char *origin;         /* Ancient Egyptian / Coptic heritage */
    const char *liturgy;        /* Christian liturgical significance */
    const char *brief;          /* One-sentence meaning */
} ci_coptic_month_t;

/* Per-feast interpretation (6 feasts) */
typedef struct {
    int feast;                  /* 1-6 */
    const char *name;           /* "Nayrouz", "Christmas", etc. */
    const char *theme;          /* Spiritual theme */
    const char *practice;       /* Traditional observance */
    const char *brief;          /* One-sentence meaning */
} ci_coptic_feast_t;

/* Tiered interpretation result */
typedef struct {
    char glyph[16];             /* T0: month abbreviation (3 chars) */
    char glance[128];           /* T1: "Thout 1 — Nayrouz (Coptic New Year)" */
    char detail[512];           /* T3: Full narrative */
} coptic_interp_t;

/* Return interpretation for a Coptic month (1-13).
 * 1-12 = regular months, 13 = Nasie (epagomenal).
 * Invalid: returns struct with month=-1, all strings "?". */
ci_coptic_month_t cci_month_data(int month);

/* Return interpretation for a Coptic feast (1-6).
 * Invalid: returns struct with feast=-1, all strings "?". */
ci_coptic_feast_t cci_feast_data(int feast);

/* Compose tiered interpretation from Coptic calendar components.
 * month: 1-13, day: 1-30 (or 1-6 for Nasie).
 * feast: 1-6 if a feast is active, 0 if none.
 * Invalid month: "?" strings in result. */
coptic_interp_t cci_interpret(int month, int day, int feast);

/* Returns 13 (12 months + Nasie). */
int cci_month_count(void);

/* Returns 6 (number of major feasts). */
int cci_feast_count(void);

/* Locale-aware interpretation. Same as cci_interpret but produces
 * translated output for the given locale (falls back to English).
 * Requires content_i18n module. */
coptic_interp_t cci_interpret_locale(int month, int day, int feast,
                                     i18n_locale_t locale);

#endif /* TIME_COPTIC_INTERPRET_H */
