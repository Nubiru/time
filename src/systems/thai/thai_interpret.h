/* thai_interpret.h — Thai calendar interpretation data
 *
 * Archetype data for the 12 Thai months and 3 major festivals.
 * The Thai solar calendar uses Buddhist Era dating (BE = CE + 543)
 * with Thai transliterations of Sanskrit month names.  Songkran
 * (April 13-15) is the Thai New Year water festival, one of the
 * world's most celebrated cultural events.
 *
 * Pure functions: no globals, no malloc, no side effects. */

#ifndef TIME_THAI_INTERPRET_H
#define TIME_THAI_INTERPRET_H

#include "../../ui/i18n.h"  /* i18n_locale_t */

/* Per-month interpretation (12 months) */
typedef struct {
    int month;                  /* 1-12 */
    const char *thai;           /* Thai transliteration */
    const char *origin;         /* Sanskrit origin */
    const char *season;         /* Thai seasonal quality */
    const char *brief;          /* One-sentence meaning */
} ti_thai_month_t;

/* Per-festival interpretation (3 festivals) */
typedef struct {
    int festival;               /* 1-3 */
    const char *name;           /* Festival name */
    const char *theme;          /* Core theme */
    const char *practice;       /* Traditional observance */
    const char *brief;          /* One-sentence meaning */
} ti_thai_festival_t;

/* Tiered interpretation result */
typedef struct {
    char glyph[16];             /* T0: month abbreviation (3 chars) */
    char glance[128];           /* T1: "BE 2569 Mesayon — Songkran" */
    char detail[512];           /* T3: Full narrative */
} thai_interp_t;

/* Return interpretation for a Thai month (1-12).
 * Invalid: returns struct with month=-1, all strings "?". */
ti_thai_month_t tti_month_data(int month);

/* Return interpretation for a Thai festival (1-3).
 * 1=Songkran, 2=Chakri Day, 3=Constitution Day.
 * Invalid: returns struct with festival=-1, all strings "?". */
ti_thai_festival_t tti_festival_data(int festival);

/* Compose tiered interpretation from Thai calendar components.
 * be_year: Buddhist Era year, month: 1-12, day: 1-31.
 * festival: 1-3 if active, 0 if none.
 * Invalid month: "?" strings in result. */
thai_interp_t tti_interpret(int be_year, int month, int day, int festival);

/* Returns 12 (number of months). */
int tti_month_count(void);

/* Returns 3 (number of festivals). */
int tti_festival_count(void);

/* Locale-aware interpretation. Same as tti_interpret but produces
 * translated output for the given locale (falls back to English).
 * Requires content_i18n module. */
thai_interp_t tti_interpret_locale(int be_year, int month, int day,
                                   int festival, i18n_locale_t locale);

#endif /* TIME_THAI_INTERPRET_H */
