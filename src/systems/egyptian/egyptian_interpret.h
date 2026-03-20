/* egyptian_interpret.h — Egyptian calendar interpretation data
 *
 * Archetype data for the 3 seasons, 12 months, and 5 epagomenal
 * days of the ancient Egyptian civil calendar.  The 5 epagomenal
 * days are each dedicated to a deity (Osiris, Horus, Set, Isis,
 * Nephthys).  The Sothic cycle (1461-year drift) adds a cosmic
 * dimension unique to Egyptian timekeeping.
 *
 * Pure functions: no globals, no malloc, no side effects. */

#ifndef TIME_EGYPTIAN_INTERPRET_H
#define TIME_EGYPTIAN_INTERPRET_H

#include "../../ui/i18n.h"  /* i18n_locale_t */

/* Per-season interpretation (3 seasons + epagomenal) */
typedef struct {
    int season;                 /* 0-3 (Akhet, Peret, Shemu, Epagomenal) */
    const char *name;           /* "Akhet", "Peret", etc. */
    const char *meaning;        /* "Inundation", "Emergence", etc. */
    const char *theme;          /* Spiritual/agricultural theme */
    const char *reflection;     /* Inner meaning */
    const char *brief;          /* One-sentence meaning */
} ei_season_t;

/* Per-month interpretation (12 months) */
typedef struct {
    int month;                  /* 1-12 */
    const char *name;           /* "Thoth", "Phaophi", etc. */
    const char *deity;          /* Presiding deity */
    const char *domain;         /* What the deity governs */
    const char *brief;          /* One-sentence meaning */
} ei_month_t;

/* Per-epagomenal day interpretation (5 days) */
typedef struct {
    int day;                    /* 1-5 */
    const char *deity;          /* Osiris, Horus, Set, Isis, Nephthys */
    const char *archetype;      /* Essential nature */
    const char *theme;          /* What the day invokes */
    const char *brief;          /* One-sentence meaning */
} ei_epag_t;

/* Tiered interpretation result */
typedef struct {
    char glyph[16];             /* T0: month abbreviation (3 chars) */
    char glance[128];           /* T1: "Thoth day 14 — Akhet (Inundation)" */
    char detail[1024];          /* T3: Full narrative */
} egyptian_interp_t;

/* Return interpretation for an Egyptian season (0-3).
 * 0=Akhet, 1=Peret, 2=Shemu, 3=Epagomenal.
 * Invalid: returns struct with season=-1, all strings "?". */
ei_season_t ei_season_data(int season);

/* Return interpretation for an Egyptian month (1-12).
 * Invalid: returns struct with month=-1, all strings "?". */
ei_month_t ei_month_data(int month);

/* Return interpretation for an epagomenal day (1-5).
 * Invalid: returns struct with day=-1, all strings "?". */
ei_epag_t ei_epag_data(int day);

/* Compose tiered interpretation from Egyptian calendar components.
 * month: 1-12 for regular months, 13 for epagomenal.
 * day: 1-30 for months 1-12, 1-5 for month 13.
 * sothic_year: 0-1460 (year within Sothic cycle, from egypt_sothic_t).
 * Invalid month: "?" strings in result. */
egyptian_interp_t ei_interpret(int month, int day, int sothic_year);

/* Locale-aware interpretation. Same as ei_interpret but produces
 * translated output for the given locale (falls back to English).
 * Requires content_i18n module. */
egyptian_interp_t ei_interpret_locale(int month, int day, int sothic_year,
                                      i18n_locale_t locale);

/* Returns 4 (3 seasons + epagomenal). */
int ei_season_count(void);

/* Returns 12 (number of regular months). */
int ei_month_count(void);

/* Returns 5 (number of epagomenal days). */
int ei_epag_count(void);

#endif /* TIME_EGYPTIAN_INTERPRET_H */
