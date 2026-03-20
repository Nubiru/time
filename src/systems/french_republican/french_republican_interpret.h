/* french_republican_interpret.h — French Republican calendar interpretation
 *
 * Archetype data for the 12 months and 5-6 Sansculottides of
 * the French Republican calendar (1793-1805).  Each month is
 * named for the season and natural world: grapes, fog, frost,
 * snow, rain, wind, seeds, flowers, meadows, harvest, heat,
 * and fruit.  The Sansculottides honored revolutionary virtues.
 *
 * Pure functions: no globals, no malloc, no side effects. */

#ifndef TIME_FRENCH_REPUBLICAN_INTERPRET_H
#define TIME_FRENCH_REPUBLICAN_INTERPRET_H

#include "../../ui/i18n.h"  /* i18n_locale_t */

/* Per-month interpretation (12 months) */
typedef struct {
    int month;                  /* 1-12 */
    const char *french;         /* French name */
    const char *english;        /* English translation */
    const char *season;         /* Seasonal quality */
    const char *nature;         /* Natural phenomenon */
    const char *brief;          /* One-sentence meaning */
} fri_month_t;

/* Per-Sansculottide interpretation (5 or 6 complementary days) */
typedef struct {
    int day;                    /* 1-6 */
    const char *french;         /* French name */
    const char *virtue;         /* Republican virtue honored */
    const char *brief;          /* One-sentence meaning */
} fri_sans_t;

/* Tiered interpretation result */
typedef struct {
    char glyph[16];             /* T0: month abbreviation (3 chars) */
    char glance[128];           /* T1: "An 234 Vendemiaire — Month of the Grape Harvest" */
    char detail[512];           /* T3: Full narrative */
} french_republican_interp_t;

/* Return interpretation for a Republican month (1-12).
 * Invalid: returns struct with month=-1, all strings "?". */
fri_month_t fri_month_data(int month);

/* Return interpretation for a Sansculottide day (1-6).
 * Invalid: returns struct with day=-1, all strings "?". */
fri_sans_t fri_sans_data(int day);

/* Compose tiered interpretation.
 * rep_year: Republican year, month: 1-12 (0=Sansculottides),
 * day: 1-30 (or 1-6 for Sansculottides).
 * Invalid month: "?" strings in result. */
french_republican_interp_t fri_interpret(int rep_year, int month, int day);

/* Returns 12 (number of months). */
int fri_month_count(void);

/* Returns 6 (max Sansculottide days). */
int fri_sans_count(void);

/* Locale-aware interpretation. Same as fri_interpret but produces
 * translated output for the given locale (falls back to English).
 * Requires content_i18n module. */
french_republican_interp_t fri_interpret_locale(int rep_year, int month,
                                                int day,
                                                i18n_locale_t locale);

#endif /* TIME_FRENCH_REPUBLICAN_INTERPRET_H */
