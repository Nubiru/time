#include "kin_moon_page.h"
#include "../systems/tzolkin/thirteen_moon.h"
#include "../systems/tzolkin/tzolkin.h"
#include "../systems/tzolkin/tzolkin_board.h"
#include <string.h>

kin_moon_page_t kin_moon_page_compute(double jd)
{
    kin_moon_page_t page;
    memset(&page, 0, sizeof(page));

    /* Get current 13-Moon date */
    thirteen_moon_t tm = thirteen_moon_from_jd(jd);

    /* Invalid or Day Out of Time: return zeroed */
    if (tm.is_day_out || (tm.moon == 0 && tm.day == 0 && tm.year == 0))
        return page;

    page.moon_number = tm.moon;
    page.moon_name = thirteen_moon_name(tm.moon);
    page.today_day = tm.day;
    page.year = tm.year;

    /* Find JD of moon day 1 */
    double moon_start_jd = jd - (tm.day - 1);

    /* Fill 28 days */
    for (int i = 0; i < KIN_MOON_TOTAL_DAYS; i++) {
        double day_jd = moon_start_jd + i;

        /* Tzolkin data */
        tzolkin_day_t tz = tzolkin_from_jd(day_jd);
        page.days[i].kin = tz.kin;
        page.days[i].seal = tz.seal;
        page.days[i].tone = tz.tone;
        page.days[i].color = tz.seal % 4;

        /* 13-Moon data for this day */
        thirteen_moon_t day_tm = thirteen_moon_from_jd(day_jd);
        page.days[i].plasma = day_tm.plasma;
        page.days[i].day = i + 1;

        /* Portal day */
        page.days[i].is_portal = tzolkin_board_is_portal(tz.kin) ? 1 : 0;

        /* Highlight today */
        page.days[i].highlighted = (i + 1 == tm.day) ? 1 : 0;

        /* Grid position: 4 rows x 7 columns */
        int col = i % KIN_MOON_DAYS_PER_HEPTAD;
        int row = i / KIN_MOON_DAYS_PER_HEPTAD;
        page.days[i].x = col * (1.0f / 7.0f);
        page.days[i].y = row * (1.0f / 4.0f);
    }

    return page;
}
