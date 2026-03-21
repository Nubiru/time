#include "kin_heptad_row.h"
#include "../systems/tzolkin/thirteen_moon.h"
#include "../systems/tzolkin/tzolkin.h"
#include <string.h>

kin_heptad_row_t kin_heptad_compute(double jd)
{
    kin_heptad_row_t row;
    memset(&row, 0, sizeof(row));

    /* Get current 13-Moon date */
    thirteen_moon_t tm = thirteen_moon_from_jd(jd);

    /* Invalid or Day Out of Time: return zeroed */
    if (tm.is_day_out || tm.moon < 1)
        return row;

    row.heptad_number = tm.heptad;
    row.moon_number = tm.moon;
    row.today_index = tm.day_of_week - 1;

    /* Find JD of heptad start (day_of_week=1 of this heptad) */
    double heptad_start_jd = jd - (tm.day_of_week - 1);

    /* Fill 7 days */
    for (int i = 0; i < KIN_HEPTAD_DAYS; i++) {
        double day_jd = heptad_start_jd + i;

        /* Tzolkin data */
        tzolkin_day_t tz = tzolkin_from_jd(day_jd);
        row.days[i].kin = tz.kin;
        row.days[i].seal = tz.seal;
        row.days[i].tone = tz.tone;
        row.days[i].color = tz.seal % 4;

        /* 13-Moon data */
        thirteen_moon_t day_tm = thirteen_moon_from_jd(day_jd);
        row.days[i].plasma = day_tm.plasma;
        row.days[i].plasma_name = thirteen_moon_plasma_name(day_tm.plasma);
        row.days[i].plasma_chakra = thirteen_moon_plasma_chakra(day_tm.plasma);
        row.days[i].day_in_moon = day_tm.day;

        /* Highlight today */
        row.days[i].highlighted = (i == tm.day_of_week - 1) ? 1 : 0;

        /* Horizontal layout */
        row.days[i].x = (float)i / 7.0f;
        row.days[i].y = 0.0f;
        row.days[i].w = 1.0f / 7.0f;
        row.days[i].h = 1.0f;
    }

    return row;
}
