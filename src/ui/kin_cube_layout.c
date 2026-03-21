#include "kin_cube_layout.h"
#include "../systems/tzolkin/thirteen_moon.h"
#include <string.h>

/* 16 Warrior powers — one per cube position. */
static const char *WARRIOR_POWERS[KIN_CUBE_POSITIONS] = {
    "Memory",          /* Position 1  */
    "Breath",          /* Position 2  */
    "Instinct",        /* Position 3  */
    "Endurance",       /* Position 4  */
    "Sex",             /* Position 5  */
    "Death",           /* Position 6  */
    "Accomplishment",  /* Position 7  */
    "Art",             /* Position 8  */
    "Purification",    /* Position 9  */
    "Love",            /* Position 10 */
    "Magic",           /* Position 11 */
    "Free Will",       /* Position 12 */
    "Prophecy",        /* Position 13 */
    "Timelessness",    /* Position 14 */
    "Vision",          /* Position 15 */
    "Intelligence"     /* Position 16 */
};

kin_cube_layout_t kin_cube_compute(double jd)
{
    kin_cube_layout_t result;
    memset(&result, 0, sizeof(result));

    if (jd < 0.0) {
        result.today_position = -1;
        return result;
    }

    thirteen_moon_t tm = thirteen_moon_from_jd(jd);

    /* Determine if today is in the Warrior's Cube phase (days 7-22) */
    int in_cube = (tm.day >= 7 && tm.day <= 22 && !tm.is_day_out);

    result.is_active = in_cube ? 1 : 0;
    result.today_position = in_cube ? (tm.day - 7) : -1;

    /* Build all 16 positions regardless of active state */
    for (int i = 0; i < KIN_CUBE_POSITIONS; i++) {
        kin_cube_cell_t *cell = &result.cells[i];

        cell->position = i + 1;
        cell->moon_day = i + 7;
        cell->codon_number = i + 1;
        cell->power = WARRIOR_POWERS[i];
        cell->x = (float)(i % KIN_CUBE_SIZE) * 0.25f;
        cell->y = (float)(i / KIN_CUBE_SIZE) * 0.25f;

        /* Highlight only today's cube position */
        cell->highlighted = (in_cube && tm.day == cell->moon_day) ? 1 : 0;
    }

    return result;
}
