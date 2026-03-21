#include "telektonon_board.h"
#include <string.h>

/* Telektonon Board data — 5 telepathy circuits and 28-day tube path.
 *
 * Source: Arguelles, "Telektonon: El Juego de la Profecia" (1995).
 */

/* --- Telepathy Circuits --- */

static const tk_circuit_t CIRCUITS[TK_CIRCUIT_COUNT] = {
    {   /* Circuit 1 */
        .number   = 1,
        .planet_a = "Pluto",
        .planet_b = "Mercury",
        .name     = "Alpha-Omega Recharge",
        .units    = 36
    },
    {   /* Circuit 2 */
        .number   = 2,
        .planet_a = "Neptune",
        .planet_b = "Venus",
        .name     = "Allied Memory-Instinct",
        .units    = 32
    },
    {   /* Circuit 3 */
        .number   = 3,
        .planet_a = "Uranus",
        .planet_b = "Earth",
        .name     = "Telektonon",
        .units    = 28
    },
    {   /* Circuit 4 */
        .number   = 4,
        .planet_a = "Saturn",
        .planet_b = "Mars",
        .name     = "Externalized Intelligence",
        .units    = 24
    },
    {   /* Circuit 5 */
        .number   = 5,
        .planet_a = "Jupiter",
        .planet_b = "Maldek",
        .name     = "Internalized Intelligence",
        .units    = 20
    }
};

/* --- Phase names --- */

static const char *PHASE_NAMES[] = {
    "None",
    "Earth Walk",
    "Warrior's Cube",
    "Heaven Walk"
};

/* --- Helper: build day info --- */

static tk_phase_t phase_for_day(int day)
{
    if (day >= 1 && day <= 6)   return TK_PHASE_EARTH_WALK;
    if (day >= 7 && day <= 22)  return TK_PHASE_WARRIORS_CUBE;
    if (day >= 23 && day <= 28) return TK_PHASE_HEAVEN_WALK;
    return TK_PHASE_NONE;
}

static int cube_pos_for_day(int day)
{
    if (day >= 7 && day <= 22)
        return day - 6;
    return 0;
}

static int is_tower(int day)
{
    return (day == 1 || day == 6 || day == 23 || day == 28) ? 1 : 0;
}

static int is_reunion(int day)
{
    return (day == 23) ? 1 : 0;
}

static int is_stairway(int day)
{
    return (day >= 13 && day <= 16) ? 1 : 0;
}

/* --- Public functions --- */

int tk_circuit_count(void)
{
    return TK_CIRCUIT_COUNT;
}

tk_circuit_t tk_circuit_get(int num)
{
    if (num < 1 || num > TK_CIRCUIT_COUNT) {
        tk_circuit_t empty;
        memset(&empty, 0, sizeof(empty));
        return empty;
    }
    return CIRCUITS[num - 1];
}

int tk_total_units(void)
{
    return TK_TOTAL_UNITS;
}

tk_day_info_t tk_day_info(int day)
{
    if (day < 1 || day > TK_TUBE_DAY_COUNT) {
        tk_day_info_t empty;
        memset(&empty, 0, sizeof(empty));
        return empty;
    }
    tk_day_info_t info;
    info.day            = day;
    info.phase          = phase_for_day(day);
    info.cube_position  = cube_pos_for_day(day);
    info.is_tower_day   = is_tower(day);
    info.is_reunion_day = is_reunion(day);
    info.is_stairway    = is_stairway(day);
    return info;
}

const char *tk_phase_name(tk_phase_t phase)
{
    if ((int)phase < (int)TK_PHASE_NONE || (int)phase > (int)TK_PHASE_HEAVEN_WALK)
        return "None";
    return PHASE_NAMES[phase];
}

tk_phase_t tk_phase_for_day(int day)
{
    return phase_for_day(day);
}

int tk_cube_position(int day)
{
    return cube_pos_for_day(day);
}

int tk_is_tower_day(int day)
{
    return is_tower(day);
}

int tk_is_reunion_day(int day)
{
    return is_reunion(day);
}

int tk_tube_day_count(void)
{
    return TK_TUBE_DAY_COUNT;
}
