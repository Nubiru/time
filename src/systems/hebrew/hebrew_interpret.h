/* hebrew_interpret.h — Hebrew month interpretation (Sefer Yetzirah tradition)
 *
 * Static data for each of the 13 Hebrew months: governing letter,
 * tribe of Israel, human sense/faculty, spiritual quality, and brief meaning.
 * Plus a composition function that produces tiered interpretation text.
 *
 * Pure functions: no globals, no malloc, no side effects. */

#ifndef TIME_HEBREW_INTERPRET_H
#define TIME_HEBREW_INTERPRET_H

#include "hebrew.h"
#include "../../ui/i18n.h"  /* i18n_locale_t */

/* Per-month interpretation data */
typedef struct {
    int month;                  /* 1-13 (Hebrew month number) */
    const char *hebrew_letter;  /* governing Hebrew letter */
    const char *tribe;          /* associated tribe of Israel */
    const char *sense;          /* associated human sense/faculty */
    const char *quality;        /* spiritual quality of the month */
    const char *brief;          /* one-sentence meaning */
} hi_month_t;

/* Tiered interpretation result */
typedef struct {
    char glyph[8];              /* T0: Hebrew letter (ASCII representation) */
    char glance[128];           /* T1: "5786 Tevet 15 — Transformation" */
    char detail[512];           /* T3: Full narrative paragraph */
} hebrew_interp_t;

/* Return interpretation data for a Hebrew month (1-13).
 * Invalid month: returns struct with month=-1, all strings "?". */
hi_month_t hi_month_data(int month);

/* Compose tiered interpretation from date, sabbatical position, leap status.
 * sabbatical_pos: 1-7 (year in Shemitah cycle), 0 if unknown.
 * is_leap_year: 1 if leap, 0 if not. */
hebrew_interp_t hi_interpret(hebrew_date_t date, int sabbatical_pos,
                             int is_leap_year);

/* Locale-aware interpretation. Same as hi_interpret but produces
 * translated output for the given locale (falls back to English).
 * Requires content_i18n module. */
hebrew_interp_t hi_interpret_locale(hebrew_date_t date, int sabbatical_pos,
                                    int is_leap_year,
                                    i18n_locale_t locale);

/* Returns 13 (total number of months including Adar II). */
int hi_month_count(void);

#endif /* TIME_HEBREW_INTERPRET_H */
