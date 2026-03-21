#ifndef TIME_KIN_MOON_DETAIL_H
#define TIME_KIN_MOON_DETAIL_H

/* Kin Moon Detail — enhanced 13-Moon monthly page (L20).
 *
 * 28-day page with wavespell overlay, GAP portal markers,
 * Gregorian dates, and plasma names alongside each day.
 * Extends kin_moon_page with wavespell position, wavespell number,
 * Gregorian month/day, and portal count per moon.
 *
 * Grid layout: 4 rows x 7 columns.
 *   col = (day-1) % 7, row = (day-1) / 7
 *   x = col / 7.0,     y = row / 4.0
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 */

#define KIN_MOON_DETAIL_DAYS 28

typedef struct {
    int day;              /* 1-28 (day within this moon) */
    int kin;              /* 1-260 */
    int seal;             /* 0-19 */
    int tone;             /* 1-13 */
    int color;            /* 0-3 */
    int plasma;           /* 0-6 */
    const char *plasma_name; /* "Dali"..."Silio" */
    int is_portal;        /* Galactic Activation Portal */
    int wavespell_pos;    /* 1-13 within current wavespell */
    int wavespell_number; /* 1-20 */
    int greg_month;       /* Gregorian month */
    int greg_day;         /* Gregorian day */
    int highlighted;      /* 1 if today */
    float x, y;           /* grid position */
} kin_moon_detail_day_t;

typedef struct {
    kin_moon_detail_day_t days[KIN_MOON_DETAIL_DAYS];
    int moon_number;        /* 1-13 */
    const char *moon_name;  /* "Magnetic", etc. */
    int today_day;          /* 1-28 (0 if not this moon) */
    int portal_count;       /* number of GAP days in this moon */
    int year;               /* 13-Moon year */
} kin_moon_detail_t;

/* Compute detailed moon page for a Julian Day.
 * 28 days in 4x7 grid with full data: Tzolkin, 13-Moon, Gregorian,
 * wavespell position, GAP status. Counts portals per moon.
 * Grid: x = col * (1/7), y = row * (1/4), col = (day-1)%7, row = (day-1)/7.
 * Returns zeroed struct for Day Out of Time or invalid JD. */
kin_moon_detail_t kin_moon_detail_compute(double jd);

#endif /* TIME_KIN_MOON_DETAIL_H */
