#ifndef TIME_KIN_MOON_PAGE_H
#define TIME_KIN_MOON_PAGE_H

/* Kin Moon Page — monthly page for the 13-Moon/28-Day calendar.
 *
 * Displays one 13-Moon month as a 4x7 grid (4 heptads x 7 days).
 * Each cell holds Tzolkin kin data, plasma, and grid position.
 * Today's date is highlighted.
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 */

#define KIN_MOON_HEPTADS 4
#define KIN_MOON_DAYS_PER_HEPTAD 7
#define KIN_MOON_TOTAL_DAYS 28

typedef struct {
    int kin;            /* 1-260 (Tzolkin kin for this day) */
    int seal;           /* 0-19 */
    int tone;           /* 1-13 */
    int color;          /* 0-3 */
    int plasma;         /* 0-6 */
    int day;            /* 1-28 (day within this moon) */
    int is_portal;      /* Galactic Activation Portal */
    int highlighted;    /* 1 if this is today */
    float x, y;         /* position in grid */
} kin_moon_day_t;

typedef struct {
    kin_moon_day_t days[KIN_MOON_TOTAL_DAYS]; /* 28 days */
    int moon_number;           /* 1-13 */
    const char *moon_name;     /* "Magnetic", "Lunar", etc. */
    int today_day;             /* 1-28 (which day is today, 0 if not in this moon) */
    int year;                  /* 13-Moon year */
} kin_moon_page_t;

/* Compute a 13-Moon calendar page for a Julian Day.
 * Shows the full month (28 days) that contains the given JD.
 * Days arranged in 4x7 grid:
 *   x = col * (1.0/7.0), y = row * (1.0/4.0)
 *   where col = (day-1) % 7, row = (day-1) / 7
 * Each day has Tzolkin kin, seal, tone, color, plasma.
 * Today's day is highlighted.
 * Returns zeroed struct for Day Out of Time or invalid JD. */
kin_moon_page_t kin_moon_page_compute(double jd);

#endif /* TIME_KIN_MOON_PAGE_H */
