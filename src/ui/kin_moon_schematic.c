#include "kin_moon_schematic.h"
#include "../systems/tzolkin/thirteen_moon.h"
#include <string.h>

kin_moon_schematic_t kin_moon_schematic_compute(double jd)
{
    kin_moon_schematic_t s;
    memset(&s, 0, sizeof(s));

    if (jd < 0.0)
        return s;

    /* Get current 13-Moon date */
    thirteen_moon_t tm = thirteen_moon_from_jd(jd);
    s.current_moon = tm.moon;
    s.current_day = tm.day;
    s.year = tm.year;

    /* Day Out of Time marker position */
    s.day_out_x = 0.9f;
    s.day_out_y = 0.85f;

    /* Fill all 13 moons */
    for (int i = 0; i < KIN_SCHEMATIC_MOONS; i++) {
        kin_schematic_moon_t *m = &s.moons[i];
        m->moon_number = i + 1;
        m->name = thirteen_moon_name(i + 1);
        m->start_day_of_year = i * 28;

        /* Layout: 4 columns x 3 rows + moon 13 at bottom center */
        if (i < 12) {
            int row = i / 4;
            int col = i % 4;
            m->x = 0.1f + (float)col * 0.267f;
            m->y = 0.1f + (float)row * 0.25f;
        } else {
            /* Moon 13: bottom center */
            m->x = 0.5f;
            m->y = 0.85f;
        }

        /* Highlight current moon */
        m->highlighted = (m->moon_number == s.current_moon) ? 1 : 0;
    }

    return s;
}
