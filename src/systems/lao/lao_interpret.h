/* lao_interpret.h -- Lao calendar interpretation data
 *
 * Archetype data for 12 Lao months and major festivals.
 * The Lao calendar uses Buddhist Era dating (BE = CE + 543) with
 * Lao romanizations of Sanskrit month names.  Pi Mai Lao (April 14-16)
 * is the Lao New Year, celebrated with water, baci ceremonies, and
 * the Nang Sangkhan maiden procession (unique to Laos).
 *
 * Pure functions: no globals, no malloc, no side effects. */

#ifndef TIME_LAO_INTERPRET_H
#define TIME_LAO_INTERPRET_H

#include "../../ui/i18n.h"  /* i18n_locale_t */

/* Per-month interpretation (12 months) */
typedef struct {
    int month;                  /* 1-12 */
    const char *lao;            /* Lao romanization */
    const char *origin;         /* Sanskrit origin */
    const char *season;         /* Lao seasonal quality */
    const char *brief;          /* One-sentence meaning */
} li_lao_month_t;

/* Per-festival interpretation */
typedef struct {
    int festival;               /* 1-3 */
    const char *name;           /* Festival name */
    const char *theme;          /* Core theme */
    const char *practice;       /* Traditional observance */
    const char *brief;          /* One-sentence meaning */
} li_lao_festival_t;

/* Tiered interpretation result */
typedef struct {
    char glyph[16];             /* T0: month abbreviation (3 chars) */
    char glance[128];           /* T1: "BE 2569 Meesaa — Pi Mai" */
    char detail[512];           /* T3: Full narrative */
} lao_interp_t;

/* Return interpretation for a Lao month (1-12).
 * Invalid: returns struct with month=-1, all strings "?". */
li_lao_month_t lai_month_data(int month);

/* Return interpretation for a Lao festival (1-3).
 * 1=Pi Mai, 2=National Day, 3=Liberation Day.
 * Invalid: returns struct with festival=-1, all strings "?". */
li_lao_festival_t lai_festival_data(int festival);

/* Compose tiered interpretation from Lao calendar components.
 * be_year: Buddhist Era year, month: 1-12, day: 1-31.
 * festival: 1-3 if active, 0 if none.
 * Invalid month: "?" strings in result. */
lao_interp_t lai_interpret(int be_year, int month, int day, int festival);

/* Returns 12 (number of months). */
int lai_month_count(void);

/* Returns 3 (number of festivals). */
int lai_festival_count(void);

/* Locale-aware interpretation. Same as lai_interpret but produces
 * translated output for the given locale (falls back to English).
 * Requires content_i18n module. */
lao_interp_t lai_interpret_locale(int be_year, int month, int day,
                                  int festival, i18n_locale_t locale);

#endif /* TIME_LAO_INTERPRET_H */
