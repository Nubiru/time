/* gregorian_interpret.h — Gregorian calendar interpretation data
 *
 * Archetype data for the 12 months, 4 seasons, and 7 days of the
 * Gregorian calendar.  While the most "ordinary" calendar in daily
 * use, each month carries ancient Roman roots and planetary day
 * names that connect our modern week to Babylonian astronomy.
 *
 * Pure functions: no globals, no malloc, no side effects. */

#ifndef TIME_GREGORIAN_INTERPRET_H
#define TIME_GREGORIAN_INTERPRET_H

/* Per-month interpretation (12 months) */
typedef struct {
    int month;                  /* 1-12 */
    const char *name;           /* "January", etc. */
    const char *origin;         /* Etymology / deity origin */
    const char *quality;        /* Seasonal quality */
    const char *brief;          /* One-sentence meaning */
} gi_month_t;

/* Per-season interpretation (4 seasons) */
typedef struct {
    int season;                 /* 0-3 (Spring, Summer, Autumn, Winter) */
    const char *name;           /* "Spring", etc. */
    const char *theme;          /* Seasonal archetype */
    const char *quality;        /* Essential quality */
    const char *brief;          /* One-sentence meaning */
} gi_season_t;

/* Per-day interpretation (7 days) */
typedef struct {
    int day;                    /* 0-6 (Monday=0 through Sunday=6) */
    const char *name;           /* "Monday", etc. */
    const char *planet;         /* Ruling planet */
    const char *origin;         /* Mythological origin */
    const char *brief;          /* One-sentence meaning */
} gi_day_t;

/* Tiered interpretation result */
typedef struct {
    char glyph[16];             /* T0: month abbreviation (3 chars) */
    char glance[128];           /* T1: "March 19 (Wednesday) — Spring" */
    char detail[512];           /* T3: Full narrative */
} gregorian_interp_t;

/* Return interpretation for a Gregorian month (1-12).
 * Invalid: returns struct with month=-1, all strings "?". */
gi_month_t gi_month_data(int month);

/* Return interpretation for a season (0-3).
 * 0=Spring, 1=Summer, 2=Autumn, 3=Winter.
 * Invalid: returns struct with season=-1, all strings "?". */
gi_season_t gi_season_data(int season);

/* Return interpretation for a day of week (0-6).
 * 0=Monday through 6=Sunday.
 * Invalid: returns struct with day=-1, all strings "?". */
gi_day_t gi_day_data(int day);

/* Compose tiered interpretation from Gregorian calendar components.
 * month: 1-12, day_of_month: 1-31, day_of_week: 0-6 (Mon-Sun).
 * Invalid month: "?" strings in result. */
gregorian_interp_t gi_interpret(int month, int day_of_month, int day_of_week);

/* Returns 12 (number of months). */
int gi_month_count(void);

/* Returns 4 (number of seasons). */
int gi_season_count(void);

/* Returns 7 (number of days). */
int gi_day_count(void);

#endif /* TIME_GREGORIAN_INTERPRET_H */
