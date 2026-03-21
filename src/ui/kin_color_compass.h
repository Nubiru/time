#ifndef TIME_KIN_COLOR_COMPASS_H
#define TIME_KIN_COLOR_COMPASS_H

/* Kin Color Compass — Four-directional color compass.
 *
 * Visual diagram showing the 4 Arguelles directional colors mapped
 * to cardinal directions, with the 20 seals distributed among them.
 *
 * Color-direction mapping (Arguelles):
 *   Red   → East  (angle  90), "Initiates"
 *   White → North (angle   0), "Refines"
 *   Blue  → West  (angle 270), "Transforms"
 *   Yellow→ South (angle 180), "Ripens"
 *
 * Each color has 5 seals (every 4th seal starting from color index):
 *   Red:    0(Dragon), 4(Serpent), 8(Moon), 12(Skywalker), 16(Earth)
 *   White:  1(Wind), 5(World-Bridger), 9(Dog), 13(Wizard), 17(Mirror)
 *   Blue:   2(Night), 6(Hand), 10(Monkey), 14(Eagle), 18(Storm)
 *   Yellow: 3(Seed), 7(Star), 11(Human), 15(Warrior), 19(Sun)
 *
 * Source: Arguelles, "Dreamspell: The Journey of Timeship Earth 2013" (1990).
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 */

#define KIN_COMPASS_DIRECTIONS 4
#define KIN_COMPASS_SEALS_PER 5  /* 20 seals / 4 colors */

typedef struct {
    int color;              /* 0=Red, 1=White, 2=Blue, 3=Yellow */
    const char *name;       /* "Red", "White", "Blue", "Yellow" */
    const char *direction;  /* "East", "North", "West", "South" */
    const char *action;     /* "Initiates", "Refines", "Transforms", "Ripens" */
    float angle;            /* direction angle: East=90, North=0, West=270, South=180 */
    float x, y;             /* center position of this quadrant */
    int seals[KIN_COMPASS_SEALS_PER]; /* 5 seal indices in this color */
    int contains_today;     /* 1 if today's seal is this color */
} kin_compass_quadrant_t;

typedef struct {
    kin_compass_quadrant_t quadrants[KIN_COMPASS_DIRECTIONS];
    int today_seal;         /* 0-19 */
    int today_color;        /* 0-3 */
} kin_color_compass_t;

/* Compute the color compass for a seal (0-19).
 * 4 quadrants at cardinal positions around center (0.5, 0.5):
 *   Red/East: (0.85, 0.5)   — angle 90
 *   White/North: (0.5, 0.15) — angle 0
 *   Blue/West: (0.15, 0.5)  — angle 270
 *   Yellow/South: (0.5, 0.85) — angle 180
 * Each quadrant lists its 5 seals (every 4th seal: Red=0,4,8,12,16).
 * The quadrant matching today's seal is marked contains_today=1.
 * Returns zeroed struct for invalid seal (outside 0-19). */
kin_color_compass_t kin_compass_compute(int today_seal);

/* Get compass from kin number. Extracts seal = (kin-1)%20. */
kin_color_compass_t kin_compass_from_kin(int kin);

#endif /* TIME_KIN_COLOR_COMPASS_H */
