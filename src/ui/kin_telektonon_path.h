#ifndef TIME_KIN_TELEKTONON_PATH_H
#define TIME_KIN_TELEKTONON_PATH_H

/* Telektonon 28-Day Path with Turtles — detailed path diagram.
 *
 * Shows all 28 days with three turtle piece positions (Yellow/Pacal Votan,
 * White/Bolon Ik, Green/Warrior), tower markers, phase labels, and
 * stairway indicators. Layout follows an S-curve through three phases:
 *
 *   Earth Walk  (days 1-6):   bottom row, left to right,  y=0.90
 *   Warrior's Cube (days 7-22): center zigzag, 4 rows of 4
 *     Row 0 (days 7-10):  y=0.70, left->right
 *     Row 1 (days 11-14): y=0.55, right->left (boustrophedon)
 *     Row 2 (days 15-18): y=0.40, left->right
 *     Row 3 (days 19-22): y=0.25, right->left
 *   Heaven Walk (days 23-28): top row, left to right, y=0.10
 *
 * Source: Arguelles, "Telektonon: El Juego de la Profecia" (1995).
 * Pure module: no GL, no malloc, no globals, no side effects.
 */

#define KIN_TK_PATH_DAYS 28

typedef struct {
    int day;                 /* 1-28 */
    int phase;               /* 1=Earth Walk, 2=Cube, 3=Heaven Walk */
    const char *phase_name;  /* from tk_journey */
    int yellow_pos;          /* Pacal Votan tube position */
    int white_pos;           /* Bolon Ik tube position */
    int green_pos;           /* Warrior cube position (0=resting) */
    int is_tower;            /* tower day marker */
    int is_reunion;          /* day 23 reunion */
    int lovers_together;     /* days 23-28 */
    int cube_position;       /* 1-16 for cube days, 0 otherwise */
    int highlighted;         /* today */
    float x, y;              /* position on path */
} kin_tk_path_day_t;

typedef struct {
    kin_tk_path_day_t days[KIN_TK_PATH_DAYS];
    int today_day;           /* 1-28 (0 if not in moon) */
    int today_phase;         /* current phase */
} kin_tk_path_layout_t;

/* Compute the full Telektonon path for a Julian Day.
 * 28 days with turtle positions from tk_journey_for_day().
 * Layout: S-curve path through 3 phases:
 *   Earth Walk (1-6): bottom, left to right, y=0.90
 *   Warrior's Cube (7-22): center zigzag, 4 rows of 4
 *     Row 0 (7-10):  y=0.70, left->right
 *     Row 1 (11-14): y=0.55, right->left (boustrophedon)
 *     Row 2 (15-18): y=0.40, left->right
 *     Row 3 (19-22): y=0.25, right->left
 *   Heaven Walk (23-28): top, left to right, y=0.10
 * X positions: 6 per row for walks, 4 per row for cube.
 * Returns zeroed struct for invalid JD / Day Out of Time. */
kin_tk_path_layout_t kin_tk_path_compute(double jd);

#endif /* TIME_KIN_TELEKTONON_PATH_H */
