/* bahai_interpret.h — Baha'i calendar interpretation data
 *
 * Archetype data for the 19 months and 8 holy days of the
 * Baha'i (Badi) calendar.  Each month is named for a divine
 * attribute: Splendour, Glory, Beauty, Grandeur, Light, Mercy,
 * Words, Perfection, Names, Might, Will, Knowledge, Power,
 * Speech, Questions, Honour, Sovereignty, Dominion, Loftiness.
 * The 19-day structure (19 months x 19 days) reflects the
 * mystical significance of the number 19 in Baha'i theology.
 *
 * Pure functions: no globals, no malloc, no side effects. */

#ifndef TIME_BAHAI_INTERPRET_H
#define TIME_BAHAI_INTERPRET_H

#include "../../ui/i18n.h"  /* i18n_locale_t */

/* Per-month interpretation (19 months + intercalary) */
typedef struct {
    int month;                  /* 1-19 (or 0 for Ayyam-i-Ha) */
    const char *arabic;         /* Arabic name */
    const char *english;        /* English translation (divine attribute) */
    const char *quality;        /* Spiritual quality of this month */
    const char *brief;          /* One-sentence meaning */
} bhi_month_t;

/* Per-holy-day interpretation (8 holy days) */
typedef struct {
    int holy_day;               /* 1-8 */
    const char *name;           /* Holy day name */
    const char *theme;          /* Spiritual significance */
    const char *brief;          /* One-sentence meaning */
} bhi_holy_day_t;

/* Tiered interpretation result */
typedef struct {
    char glyph[16];             /* T0: month abbreviation */
    char glance[128];           /* T1: "Baha (Splendour) day 1 — Naw-Ruz" */
    char detail[512];           /* T3: Full narrative */
} bahai_interp_t;

/* Return interpretation for a Baha'i month (0-19).
 * 0=Ayyam-i-Ha, 1-19=regular months.
 * Invalid: returns struct with month=-1, all strings "?". */
bhi_month_t bhi_month_data(int month);

/* Return interpretation for a Baha'i holy day (1-8).
 * Invalid: returns struct with holy_day=-1, all strings "?". */
bhi_holy_day_t bhi_holy_day_data(int holy_day);

/* Compose tiered interpretation.
 * month: 0-19, day: 1-19, holy_day: 1-8 if active, 0 if none.
 * Invalid month (<0 or >19): "?" strings. */
bahai_interp_t bhi_interpret(int month, int day, int holy_day);

int bhi_month_count(void);     /* Returns 20 (19 + intercalary) */
int bhi_holy_day_count(void);  /* Returns 8 */

/* Locale-aware interpretation. Same as bhi_interpret but produces
 * translated output for the given locale (falls back to English).
 * Requires content_i18n module. */
bahai_interp_t bhi_interpret_locale(int month, int day, int holy_day,
                                    i18n_locale_t locale);

#endif /* TIME_BAHAI_INTERPRET_H */
