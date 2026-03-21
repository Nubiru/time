#ifndef TIME_KIN_TELEKTONON_LAYOUT_H
#define TIME_KIN_TELEKTONON_LAYOUT_H

/* Telektonon board layout — 28-day tube path with phases.
 *
 * The Telektonon game board: 28 days positioned as a tube path
 * through Earth Walk, Warrior's Cube, and Heaven Walk.
 *
 *   Earth Walk  (days 1-6):   bottom row, y=0.85
 *   Warrior's Cube (days 7-22): center 4x4 grid
 *   Heaven Walk (days 23-28): top row, y=0.15
 *
 * Tower days mark the 4 corners (1, 6, 23, 28).
 * Day 23 is the reunion day.
 *
 * Source: Arguelles, "Telektonon: El Juego de la Profecia" (1995).
 * Pure module: no GL, no malloc, no globals, no side effects.
 */

#define KIN_TK_DAYS     28
#define KIN_TK_CUBE_DAYS 16

typedef struct {
    int day;                /* 1-28 */
    int phase;              /* tk_phase_t value (1=Earth Walk, 2=Cube, 3=Heaven Walk) */
    int cube_position;      /* 0 or 1-16 */
    int is_tower;           /* tower day marker */
    int is_reunion;         /* reunion day (day 23) */
    int highlighted;        /* 1 if today */
    float x, y;             /* position in layout */
} kin_tk_day_t;

typedef struct {
    kin_tk_day_t days[KIN_TK_DAYS]; /* 28 days */
    int today_day;          /* 1-28 (0 if not in a moon) */
    int today_phase;        /* current phase */
    const char *phase_name; /* "Earth Walk", "Warrior's Cube", "Heaven Walk" */
    int cube_day;           /* 0 or 1-16 (position in cube if in cube phase) */
} kin_telektonon_layout_t;

/* Compute the Telektonon board layout for a Julian Day.
 * 28 days positioned as a tube path.
 * Today's day is highlighted based on thirteen_moon.
 * Returns zeroed struct for invalid JD or Day Out of Time. */
kin_telektonon_layout_t kin_telektonon_compute(double jd);

#endif /* TIME_KIN_TELEKTONON_LAYOUT_H */
