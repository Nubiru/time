#ifndef TIME_TELEKTONON_BOARD_H
#define TIME_TELEKTONON_BOARD_H

/* Telektonon Board — Game of Prophecy board layout.
 *
 * The Telektonon game board maps the solar system as a telepathy
 * cosmology: 10 planetary orbits connected by 5 telepathy circuits,
 * a 28-day tube path with three phases, and a 16-day Warrior's Cube.
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 *
 * Source: Arguelles, "Telektonon: El Juego de la Profecia" (1995).
 */

#define TK_CIRCUIT_COUNT     5
#define TK_TUBE_DAY_COUNT   28
#define TK_TOTAL_UNITS     140
#define TK_CUBE_SIZE        16

/* --- Enums --- */

typedef enum {
    TK_PHASE_NONE          = 0,
    TK_PHASE_EARTH_WALK    = 1,
    TK_PHASE_WARRIORS_CUBE = 2,
    TK_PHASE_HEAVEN_WALK   = 3
} tk_phase_t;

/* --- Structs --- */

typedef struct {
    int         number;      /* circuit 1-5 */
    const char *planet_a;    /* outer planet */
    const char *planet_b;    /* inner planet */
    const char *name;        /* circuit name */
    int         units;       /* coordinate units */
} tk_circuit_t;

typedef struct {
    int         day;             /* 1-28 */
    tk_phase_t  phase;           /* which phase this day belongs to */
    int         cube_position;   /* 0 if not in cube, 1-16 if in cube */
    int         is_tower_day;    /* 1 for days 1,6,23,28 */
    int         is_reunion_day;  /* 1 for day 23 only */
    int         is_stairway;     /* 1 for days 13-16 */
} tk_day_info_t;

/* --- Functions --- */

/* Number of telepathy circuits (always 5). */
int tk_circuit_count(void);

/* Get circuit data by number (1-5). Returns zeroed struct for invalid. */
tk_circuit_t tk_circuit_get(int num);

/* Total coordinate units across all circuits (always 140). */
int tk_total_units(void);

/* Full day info for a tube day (1-28). Returns zeroed struct for invalid. */
tk_day_info_t tk_day_info(int day);

/* Phase name string. Returns "None" for TK_PHASE_NONE. */
const char *tk_phase_name(tk_phase_t phase);

/* Phase for a given day (1-28). Returns TK_PHASE_NONE for invalid. */
tk_phase_t tk_phase_for_day(int day);

/* Warrior's Cube position for a day.
 * Returns 1-16 for days 7-22, 0 otherwise. */
int tk_cube_position(int day);

/* Is this a tower day? (days 1, 6, 23, 28). */
int tk_is_tower_day(int day);

/* Is this the reunion day? (day 23 only). */
int tk_is_reunion_day(int day);

/* Number of days in the tube (always 28). */
int tk_tube_day_count(void);

#endif /* TIME_TELEKTONON_BOARD_H */
