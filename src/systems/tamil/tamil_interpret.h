/* tamil_interpret.h — Tamil calendar interpretation data
 *
 * Archetype data for the 12 sidereal solar months and 4 major
 * festivals of the Tamil calendar.  Each month corresponds to a
 * zodiac sign (rasi) entered by the sidereal Sun, with names
 * from the Sanskrit tradition.  The 60-year Jovian (Brihaspati)
 * cycle adds a layer of cosmic personality to each year.
 *
 * Pure functions: no globals, no malloc, no side effects. */

#ifndef TIME_TAMIL_INTERPRET_H
#define TIME_TAMIL_INTERPRET_H

/* Per-month interpretation (12 months) */
typedef struct {
    int month;                  /* 1-12 (Chittirai through Panguni) */
    const char *tamil;          /* Tamil month name */
    const char *rasi;           /* Zodiac sign (rasi) */
    const char *season;         /* Tamil seasonal quality */
    const char *brief;          /* One-sentence meaning */
} tmi_month_t;

/* Per-festival interpretation (4 festivals) */
typedef struct {
    int festival;               /* 1-4 */
    const char *name;           /* Festival name */
    const char *theme;          /* Spiritual theme */
    const char *practice;       /* Traditional observance */
    const char *brief;          /* One-sentence meaning */
} tmi_festival_t;

/* Tiered interpretation result */
typedef struct {
    char glyph[16];             /* T0: month abbreviation (3 chars) */
    char glance[128];           /* T1: "Chittirai 1 — Puthandu (Tamil New Year)" */
    char detail[512];           /* T3: Full narrative */
} tamil_interp_t;

/* Return interpretation for a Tamil month (1-12).
 * Invalid: returns struct with month=-1, all strings "?". */
tmi_month_t tmi_month_data(int month);

/* Return interpretation for a Tamil festival (1-4).
 * 1=Puthandu, 2=Pongal, 3=Chithirai Festival, 4=Deepavali.
 * Invalid: returns struct with festival=-1, all strings "?". */
tmi_festival_t tmi_festival_data(int festival);

/* Compose tiered interpretation from Tamil calendar components.
 * month: 1-12, day: 1-32, jovian_year: 1-60.
 * festival: 1-4 if active, 0 if none.
 * Invalid month: "?" strings in result. */
tamil_interp_t tmi_interpret(int month, int day, int jovian_year,
                             int festival);

/* Returns 12 (number of months). */
int tmi_month_count(void);

/* Returns 4 (number of major festivals). */
int tmi_festival_count(void);

#endif /* TIME_TAMIL_INTERPRET_H */
