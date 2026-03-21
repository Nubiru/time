#ifndef TIME_KIN_MOON_SCHEMATIC_H
#define TIME_KIN_MOON_SCHEMATIC_H

/* 13-Moon Wavespell Schematic — visual layout of all 13 moons.
 *
 * Arranges the 13 moons in a grid layout:
 *   Moons 1-4:  top row     (y=0.1)
 *   Moons 5-8:  second row  (y=0.35)
 *   Moons 9-12: third row   (y=0.6)
 *   Moon 13:    bottom center (0.5, 0.85)
 *   Day Out of Time: far right (0.9, 0.85)
 *
 * Current moon is highlighted.
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 *
 * Source: Arguelles, "13 Moon Calendar" system.
 */

#define KIN_SCHEMATIC_MOONS 13

typedef struct {
    int moon_number;              /* 1-13 */
    const char *name;             /* "Magnetic", "Lunar", etc. */
    int start_day_of_year;        /* 0-336 (day_of_year for day 1 of this moon) */
    float x, y;                   /* position in layout */
    int highlighted;              /* 1 if current moon */
} kin_schematic_moon_t;

typedef struct {
    kin_schematic_moon_t moons[KIN_SCHEMATIC_MOONS];
    int current_moon;             /* 1-13 (0 if Day Out of Time) */
    int current_day;              /* 1-28 (0 if Day Out of Time) */
    int year;                     /* 13-Moon year */
    float day_out_x, day_out_y;   /* position of Day Out of Time marker */
} kin_moon_schematic_t;

/* Compute the 13-moon schematic for a Julian Day.
 * Returns zeroed struct for invalid JD (< 0). */
kin_moon_schematic_t kin_moon_schematic_compute(double jd);

#endif /* TIME_KIN_MOON_SCHEMATIC_H */
