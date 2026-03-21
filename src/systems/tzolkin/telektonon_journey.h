#ifndef TIME_TELEKTONON_JOURNEY_H
#define TIME_TELEKTONON_JOURNEY_H

/* Telektonon Journey — Three turtle movements across the 28-day tube.
 *
 * The Telektonon game is played daily over 28-day moon cycles. Three
 * turtle pieces traverse the board:
 *   - Yellow Turtle (Pacal Votan): walks tube positions 1-28
 *   - White Turtle (Bolon Ik): descends 28->23, holds at 23, then
 *     reunites and walks 23->28 with Yellow
 *   - Green Turtle (Warrior): enters the 16-day Cube on day 7,
 *     exits on day 22
 *
 * Phase 1 — Earth Walk (days 1-6):   Yellow ascends, White descends
 * Phase 2 — Warrior's Cube (days 7-22): Yellow ascends, White holds,
 *                                        Green walks cube 1-16
 * Phase 3 — Heaven Walk (days 23-28): Yellow+White walk together,
 *                                      Green rests
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 *
 * Source: Arguelles, "Telektonon: El Juego de la Profecia" (1995).
 */

typedef struct {
    int         day;                      /* 1-28 (moon day) */
    int         yellow_tube_pos;          /* 1-28, Pacal Votan's position */
    int         white_tube_pos;           /* 23-28, Bolon Ik's position */
    int         green_cube_pos;           /* 0-16, 0=resting, 1-16=in cube */
    int         is_reunion;               /* 1 only on day 23 */
    int         lovers_walking_together;  /* 1 for days 23-28 */
    const char *phase_name;              /* "Earth Walk", "Warrior's Cube",
                                           * "Heaven Walk" */
} tk_journey_t;

/* Full journey state for a moon day (1-28).
 * Returns zeroed struct for invalid day. */
tk_journey_t tk_journey_for_day(int day);

/* Yellow Turtle (Pacal Votan) tube position for a day.
 * Returns day itself for valid (1-28), 0 for invalid. */
int tk_yellow_position(int day);

/* White Turtle (Bolon Ik) tube position for a day.
 * Days 1-6: 29-day (descending 28->23).
 * Days 7-22: 23 (fixed, waiting).
 * Days 23-28: day (walking with Yellow).
 * Returns 0 for invalid. */
int tk_white_position(int day);

/* Green Turtle (Warrior) cube position for a day.
 * Days 7-22: day-6 (cube positions 1-16).
 * All other days: 0 (resting).
 * Returns 0 for invalid. */
int tk_green_position(int day);

/* Are the lovers walking together? (days 23-28).
 * Returns 1 for days 23-28, 0 otherwise. */
int tk_lovers_together(int day);

#endif /* TIME_TELEKTONON_JOURNEY_H */
