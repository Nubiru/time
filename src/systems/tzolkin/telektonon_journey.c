#include "telektonon_journey.h"
#include <string.h>

/* Telektonon Journey — Three turtle piece movement rules.
 *
 * Source: Arguelles, "Telektonon: El Juego de la Profecia" (1995).
 *
 * Movement rules (all computed from day number):
 *   Yellow (Pacal Votan): always at tube position = day
 *   White  (Bolon Ik):    29 - day  for days 1-6  (descending 28->23)
 *                          23        for days 7-22 (waiting at position 23)
 *                          day       for days 23-28 (walking with Yellow)
 *   Green  (Warrior):     day - 6   for days 7-22 (cube positions 1-16)
 *                          0         otherwise     (resting)
 */

/* --- Phase name strings --- */

static const char *PHASE_EARTH_WALK    = "Earth Walk";
static const char *PHASE_WARRIORS_CUBE = "Warrior's Cube";
static const char *PHASE_HEAVEN_WALK   = "Heaven Walk";

/* --- Validity check --- */

static int is_valid_day(int day)
{
    return (day >= 1 && day <= 28) ? 1 : 0;
}

/* --- Phase name for a day --- */

static const char *phase_name_for_day(int day)
{
    if (day >= 1 && day <= 6)   return PHASE_EARTH_WALK;
    if (day >= 7 && day <= 22)  return PHASE_WARRIORS_CUBE;
    if (day >= 23 && day <= 28) return PHASE_HEAVEN_WALK;
    return NULL;
}

/* --- Public functions --- */

int tk_yellow_position(int day)
{
    if (!is_valid_day(day))
        return 0;
    return day;
}

int tk_white_position(int day)
{
    if (!is_valid_day(day))
        return 0;
    if (day >= 1 && day <= 6)
        return 29 - day;
    if (day >= 7 && day <= 22)
        return 23;
    /* days 23-28 */
    return day;
}

int tk_green_position(int day)
{
    if (day >= 7 && day <= 22)
        return day - 6;
    return 0;
}

int tk_lovers_together(int day)
{
    return (day >= 23 && day <= 28) ? 1 : 0;
}

tk_journey_t tk_journey_for_day(int day)
{
    if (!is_valid_day(day)) {
        tk_journey_t empty;
        memset(&empty, 0, sizeof(empty));
        return empty;
    }

    tk_journey_t j;
    j.day                      = day;
    j.yellow_tube_pos          = tk_yellow_position(day);
    j.white_tube_pos           = tk_white_position(day);
    j.green_cube_pos           = tk_green_position(day);
    j.is_reunion               = (day == 23) ? 1 : 0;
    j.lovers_walking_together  = tk_lovers_together(day);
    j.phase_name               = phase_name_for_day(day);

    return j;
}
