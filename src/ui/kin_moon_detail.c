#include "kin_moon_detail.h"
#include "kin_cell.h"
#include "../systems/tzolkin/thirteen_moon.h"
#include "../systems/tzolkin/tzolkin.h"
#include "../systems/tzolkin/tzolkin_board.h"
#include "../systems/tzolkin/dreamspell.h"
#include "../math/julian.h"
#include <string.h>
#include <stdbool.h>

kin_moon_detail_t kin_moon_detail_compute(double jd)
{
    kin_moon_detail_t result;
    memset(&result, 0, sizeof(result));

    /* Get current 13-Moon date */
    thirteen_moon_t tm = thirteen_moon_from_jd(jd);

    /* Invalid or Day Out of Time: return zeroed */
    if (tm.is_day_out || (tm.moon == 0 && tm.day == 0 && tm.year == 0))
        return result;

    result.moon_number = tm.moon;
    result.moon_name = thirteen_moon_name(tm.moon);
    result.today_day = tm.day;
    result.year = tm.year;

    /* Find JD of moon day 1 */
    double moon_start_jd = jd - (tm.day - 1);

    int portal_count = 0;

    /* Fill 28 days */
    for (int i = 0; i < KIN_MOON_DETAIL_DAYS; i++) {
        double day_jd = moon_start_jd + i;
        kin_moon_detail_day_t *d = &result.days[i];

        d->day = i + 1;

        /* Tzolkin data */
        tzolkin_day_t tz = tzolkin_from_jd(day_jd);
        d->kin = tz.kin;
        d->seal = tz.seal;
        d->tone = tz.tone;
        d->color = tz.seal % 4;

        /* 13-Moon data for this day */
        thirteen_moon_t day_tm = thirteen_moon_from_jd(day_jd);
        d->plasma = day_tm.plasma;
        d->plasma_name = thirteen_moon_plasma_name(day_tm.plasma);

        /* Portal day */
        d->is_portal = tzolkin_board_is_portal(tz.kin) ? 1 : 0;
        if (d->is_portal)
            portal_count++;

        /* Wavespell data */
        d->wavespell_pos = dreamspell_wavespell_position(tz.kin);
        dreamspell_wavespell_t ws = dreamspell_wavespell(tz.kin);
        d->wavespell_number = ws.number;

        /* Gregorian date */
        int greg_year_unused, greg_month;
        double greg_day_frac = jd_to_gregorian(day_jd, &greg_year_unused, &greg_month);
        (void)greg_year_unused;
        d->greg_month = greg_month;
        d->greg_day = (int)greg_day_frac;

        /* Highlight today */
        d->highlighted = (i + 1 == tm.day) ? 1 : 0;

        /* Grid position: 4 rows x 7 columns */
        int col = i % 7;
        int row = i / 7;
        d->x = col * (1.0f / 7.0f);
        d->y = row * (1.0f / 4.0f);
    }

    result.portal_count = portal_count;

    return result;
}
