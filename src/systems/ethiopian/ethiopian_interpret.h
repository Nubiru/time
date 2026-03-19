/* ethiopian_interpret.h — Ethiopian calendar interpretation data
 *
 * Archetype data for the 13 Ge'ez months and 4 fixed feasts of
 * the Ethiopian calendar.  Structurally identical to the Coptic
 * calendar but with Ge'ez month names and a different epoch
 * (8 CE).  Ethiopia's calendar is 7-8 years behind the Gregorian,
 * keeping its own count of time since creation.
 *
 * Pure functions: no globals, no malloc, no side effects. */

#ifndef TIME_ETHIOPIAN_INTERPRET_H
#define TIME_ETHIOPIAN_INTERPRET_H

/* Per-month interpretation (13 months) */
typedef struct {
    int month;                  /* 1-13 (Meskerem through Pagume) */
    const char *name;           /* Ge'ez name */
    const char *meaning;        /* What the name signifies */
    const char *season;         /* Ethiopian seasonal quality */
    const char *brief;          /* One-sentence meaning */
} eti_month_t;

/* Per-feast interpretation (4 fixed feasts) */
typedef struct {
    int feast;                  /* 1-4 */
    const char *name;           /* Feast name */
    const char *theme;          /* Spiritual theme */
    const char *practice;       /* Traditional observance */
    const char *brief;          /* One-sentence meaning */
} eti_feast_t;

/* Tiered interpretation result */
typedef struct {
    char glyph[16];             /* T0: month abbreviation (3 chars) */
    char glance[128];           /* T1: "Meskerem 1 — Enkutatash (New Year)" */
    char detail[512];           /* T3: Full narrative */
} ethiopian_interp_t;

/* Return interpretation for an Ethiopian month (1-13).
 * Invalid: returns struct with month=-1, all strings "?". */
eti_month_t eti_month_data(int month);

/* Return interpretation for an Ethiopian feast (1-4).
 * 1=Enkutatash, 2=Meskel, 3=Genna, 4=Timkat.
 * Invalid: returns struct with feast=-1, all strings "?". */
eti_feast_t eti_feast_data(int feast);

/* Compose tiered interpretation from Ethiopian calendar components.
 * month: 1-13, day: 1-30 (or 1-6 for Pagume).
 * feast: 1-4 if active, 0 if none.
 * Invalid month: "?" strings in result. */
ethiopian_interp_t eti_interpret(int month, int day, int feast);

/* Returns 13 (12 months + Pagume). */
int eti_month_count(void);

/* Returns 4 (number of fixed feasts). */
int eti_feast_count(void);

#endif /* TIME_ETHIOPIAN_INTERPRET_H */
