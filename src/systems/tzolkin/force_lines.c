#include "force_lines.h"
#include <string.h>

/* Lines of Telepathic Force data — 12 vertical pairings and 5 horizontal circuits.
 *
 * Source: Arguelles, "Telektonon: El Juego de la Profecia" (1995).
 */

/* --- 12 Vertical Force Lines (upper + lower = 29) --- */

static const force_line_vertical_t VERTICALS[FL_VERTICAL_COUNT] = {
    {   /* Line 0: days 1-28 */
        .upper_day = 1,
        .lower_day = 28,
        .meaning   = "Tower of Spirit / Tower of Babel"
    },
    {   /* Line 1: days 2-27 */
        .upper_day = 2,
        .lower_day = 27,
        .meaning   = "Telecosmic power of 9"
    },
    {   /* Line 2: days 3-26 */
        .upper_day = 3,
        .lower_day = 26,
        .meaning   = "Telecosmic power of 13"
    },
    {   /* Line 3: days 4-25 */
        .upper_day = 4,
        .lower_day = 25,
        .meaning   = "Telecosmic power of 5"
    },
    {   /* Line 4: days 5-24 */
        .upper_day = 5,
        .lower_day = 24,
        .meaning   = "Telecosmic power of 4"
    },
    {   /* Line 5: days 6-23 */
        .upper_day = 6,
        .lower_day = 23,
        .meaning   = "Tower of Navigation / Tower of Prophecy"
    },
    {   /* Line 6: days 7-22 */
        .upper_day = 7,
        .lower_day = 22,
        .meaning   = "Buddha — New Jerusalem"
    },
    {   /* Line 7: days 8-21 */
        .upper_day = 8,
        .lower_day = 21,
        .meaning   = "Christ — New Heaven, New Earth"
    },
    {   /* Line 8: days 9-20 */
        .upper_day = 9,
        .lower_day = 20,
        .meaning   = "Lords of Red and Black — Prophecy 7"
    },
    {   /* Line 9: days 10-19 */
        .upper_day = 10,
        .lower_day = 19,
        .meaning   = "Muhammad, Pacal Votan — Prophecy 6"
    },
    {   /* Line 10: days 11-18 */
        .upper_day = 11,
        .lower_day = 18,
        .meaning   = "Quetzalcoatl — Prophecy 5"
    },
    {   /* Line 11: days 12-17 */
        .upper_day = 12,
        .lower_day = 17,
        .meaning   = "Baktun 12 — Prophecy 4 (Stairway to Heaven)"
    }
};

/* --- 5 Horizontal Force Lines (telepathy circuits) --- */

static const force_line_horizontal_t HORIZONTALS[FL_HORIZONTAL_COUNT] = {
    {   /* Circuit 1: Pluto-Mercury */
        .circuit   = 1,
        .planet_a  = "Pluto",
        .planet_b  = "Mercury",
        .seal_codes = {0, 19, 9, 10},
        .powers    = "Illumination, autogeneration, purification, love"
    },
    {   /* Circuit 2: Neptune-Venus */
        .circuit   = 2,
        .planet_a  = "Neptune",
        .planet_b  = "Venus",
        .seal_codes = {1, 18, 8, 11},
        .powers    = "Memory, meditation, art, magic"
    },
    {   /* Circuit 3: Uranus-Earth */
        .circuit   = 3,
        .planet_a  = "Uranus",
        .planet_b  = "Earth",
        .seal_codes = {2, 17, 7, 12},
        .powers    = "Spirit, navigation, realization, free will"
    },
    {   /* Circuit 4: Saturn-Mars */
        .circuit   = 4,
        .planet_a  = "Saturn",
        .planet_b  = "Mars",
        .seal_codes = {3, 16, 6, 13},
        .powers    = "Abundance, intelligence, death, prophecy"
    },
    {   /* Circuit 5: Jupiter-Maldek */
        .circuit   = 5,
        .planet_a  = "Jupiter",
        .planet_b  = "Maldek",
        .seal_codes = {4, 15, 5, 14},
        .powers    = "Flowering, vision, sex, atemporality"
    }
};

/* --- Public functions --- */

int force_line_vertical_count(void)
{
    return FL_VERTICAL_COUNT;
}

force_line_vertical_t force_line_vertical_get(int index)
{
    if (index < 0 || index >= FL_VERTICAL_COUNT) {
        force_line_vertical_t empty;
        memset(&empty, 0, sizeof(empty));
        return empty;
    }
    return VERTICALS[index];
}

force_line_vertical_t force_line_vertical_for_day(int day)
{
    if (day < 1 || day > 28) {
        force_line_vertical_t empty;
        memset(&empty, 0, sizeof(empty));
        return empty;
    }
    /* Days 13-16: center of cube, no named vertical force line */
    if (day >= 13 && day <= 16) {
        force_line_vertical_t empty;
        memset(&empty, 0, sizeof(empty));
        return empty;
    }
    /* Days 1-12: upper day, index = day - 1 */
    if (day >= 1 && day <= 12) {
        return VERTICALS[day - 1];
    }
    /* Days 17-28: lower day, upper = 29 - day, index = upper - 1 */
    int upper = 29 - day;
    return VERTICALS[upper - 1];
}

int force_line_paired_day(int day)
{
    if (day < 1 || day > 28)
        return 0;
    return 29 - day;
}

int force_line_horizontal_count(void)
{
    return FL_HORIZONTAL_COUNT;
}

force_line_horizontal_t force_line_horizontal_get(int index)
{
    if (index < 0 || index >= FL_HORIZONTAL_COUNT) {
        force_line_horizontal_t empty;
        memset(&empty, 0, sizeof(empty));
        return empty;
    }
    return HORIZONTALS[index];
}
