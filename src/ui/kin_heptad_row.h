#ifndef TIME_KIN_HEPTAD_ROW_H
#define TIME_KIN_HEPTAD_ROW_H

/* Kin Heptad Row — one 7-day week of a 13-Moon month.
 *
 * Shows a single heptad (week) with Tzolkin kin data, plasma info,
 * and layout geometry. Used as a building block for the full moon
 * page and for the "this week" view.
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 */

#define KIN_HEPTAD_DAYS 7

typedef struct {
    int kin;                   /* 1-260 */
    int seal;                  /* 0-19 */
    int tone;                  /* 1-13 */
    int color;                 /* 0-3 (Red/White/Blue/Yellow) */
    int plasma;                /* 0-6 */
    const char *plasma_name;   /* "Dali", "Seli", etc. */
    const char *plasma_chakra; /* "Crown", "Root", etc. */
    int day_in_moon;           /* 1-28 */
    int highlighted;           /* 1 if today */
    float x, y, w, h;         /* position in row */
} kin_heptad_day_t;

typedef struct {
    kin_heptad_day_t days[KIN_HEPTAD_DAYS]; /* 7 days */
    int heptad_number;         /* 1-4 (which week in the moon) */
    int moon_number;           /* 1-13 */
    int today_index;           /* 0-6: which day is today (-1 if not this week) */
} kin_heptad_row_t;

/* Compute a heptad row for a Julian Day.
 * Shows the 7-day week containing the given JD.
 * Days arranged horizontally:
 *   day[0] at x=0.0, day[6] at x=6/7
 *   all y=0.0, w=1/7, h=1.0
 * Returns zeroed struct for Day Out of Time or invalid JD. */
kin_heptad_row_t kin_heptad_compute(double jd);

#endif /* TIME_KIN_HEPTAD_ROW_H */
