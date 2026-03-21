#include "kin_color_compass.h"
#include <string.h>

/*
 * Kin Color Compass — four-directional color compass.
 *
 * Arguelles color-direction mapping:
 *   0 Red    → East  (90°),  "Initiates"
 *   1 White  → North (0°),   "Refines"
 *   2 Blue   → West  (270°), "Transforms"
 *   3 Yellow → South (180°), "Ripens"
 *
 * Seal distribution: color c gets seals {c, c+4, c+8, c+12, c+16}
 *
 * Pure: no GL, no malloc, no globals, no side effects.
 */

static const char *COLOR_NAMES[KIN_COMPASS_DIRECTIONS] = {
    "Red", "White", "Blue", "Yellow"
};

static const char *DIRECTION_NAMES[KIN_COMPASS_DIRECTIONS] = {
    "East", "North", "West", "South"
};

static const char *ACTION_NAMES[KIN_COMPASS_DIRECTIONS] = {
    "Initiates", "Refines", "Transforms", "Ripens"
};

/* Angles: East=90, North=0, West=270, South=180 */
static const float DIRECTION_ANGLES[KIN_COMPASS_DIRECTIONS] = {
    90.0f, 0.0f, 270.0f, 180.0f
};

/* Quadrant center positions around (0.5, 0.5) */
static const float QUADRANT_X[KIN_COMPASS_DIRECTIONS] = {
    0.85f, 0.50f, 0.15f, 0.50f
};

static const float QUADRANT_Y[KIN_COMPASS_DIRECTIONS] = {
    0.50f, 0.15f, 0.50f, 0.85f
};

kin_color_compass_t kin_compass_compute(int today_seal)
{
    kin_color_compass_t compass;
    memset(&compass, 0, sizeof(compass));

    if (today_seal < 0 || today_seal > 19)
        return compass;

    compass.today_seal = today_seal;
    compass.today_color = today_seal % 4;

    /* Populate 4 quadrants */
    for (int c = 0; c < KIN_COMPASS_DIRECTIONS; c++) {
        kin_compass_quadrant_t *q = &compass.quadrants[c];

        q->color = c;
        q->name = COLOR_NAMES[c];
        q->direction = DIRECTION_NAMES[c];
        q->action = ACTION_NAMES[c];
        q->angle = DIRECTION_ANGLES[c];
        q->x = QUADRANT_X[c];
        q->y = QUADRANT_Y[c];

        /* 5 seals per color: every 4th seal starting from color index */
        for (int s = 0; s < KIN_COMPASS_SEALS_PER; s++) {
            q->seals[s] = c + s * 4;
        }

        /* Mark which quadrant contains today's seal */
        q->contains_today = (c == compass.today_color) ? 1 : 0;
    }

    return compass;
}

kin_color_compass_t kin_compass_from_kin(int kin)
{
    kin_color_compass_t compass;
    memset(&compass, 0, sizeof(compass));

    if (kin < 1 || kin > 260)
        return compass;

    int seal = (kin - 1) % 20;
    return kin_compass_compute(seal);
}
