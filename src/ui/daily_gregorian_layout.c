/* daily_gregorian_layout.c — Daily Gregorian calendar card layout
 *
 * Converts JD to Gregorian date, looks up month/day/season interpretation,
 * and computes card slot positions for rendering.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "daily_gregorian_layout.h"
#include "../systems/gregorian/gregorian.h"
#include "../systems/gregorian/gregorian_interpret.h"
#include <string.h>
#include <stdlib.h>

/* Slot layout constants */
#define SLOT_X       0.10f
#define SLOT_W       0.80f
#define SLOT_H       0.10f

#define TITLE_Y      0.05f
#define DATE_Y       0.18f
#define DAY_Y        0.32f
#define MONTH_Y      0.46f
#define SEASON_Y     0.60f
#define QUALITY_Y    0.74f

/* Card frame */
#define CARD_X       0.05f
#define CARD_Y       0.02f
#define CARD_W       0.90f
#define CARD_H       0.96f

/* Season index from month (Northern Hemisphere approximation).
 * 3-5=Spring(0), 6-8=Summer(1), 9-11=Autumn(2), 12,1,2=Winter(3). */
static int season_from_month(int month)
{
    if (month >= 3 && month <= 5) return 0;
    if (month >= 6 && month <= 8) return 1;
    if (month >= 9 && month <= 11) return 2;
    return 3; /* Dec, Jan, Feb */
}

daily_gregorian_layout_t daily_gregorian_compute(double jd)
{
    daily_gregorian_layout_t layout;
    memset(&layout, 0, sizeof(layout));

    /* Format date string and extract components */
    gregorian_format_date(jd, layout.date_str, sizeof(layout.date_str));

    /* Parse "YYYY-MM-DD" to extract year, month, day */
    /* date_str is guaranteed to be "YYYY-MM-DD" format */
    char buf[20];
    memcpy(buf, layout.date_str, sizeof(buf));

    /* Year: chars 0-3 */
    buf[4] = '\0';
    layout.year = atoi(buf);

    /* Month: chars 5-6 */
    buf[7] = '\0';
    layout.month = atoi(buf + 5);

    /* Day: chars 8-9 */
    layout.day = atoi(layout.date_str + 8);

    /* Day of week */
    int dow = gregorian_day_of_week(jd);
    layout.day_name = gregorian_day_name(dow);

    /* Month interpretation */
    gi_month_t m = gi_month_data(layout.month);
    layout.month_name = m.name;
    layout.month_origin = m.origin;
    layout.month_quality = m.quality;

    /* Day interpretation */
    gi_day_t d = gi_day_data(dow);
    layout.day_planet = d.planet;
    layout.day_origin = d.origin;

    /* Season */
    layout.season = season_from_month(layout.month);
    gi_season_t s = gi_season_data(layout.season);
    layout.season_name = s.name;
    layout.season_theme = s.theme;

    /* Title slot */
    layout.title_slot.x = SLOT_X;
    layout.title_slot.y = TITLE_Y;
    layout.title_slot.w = SLOT_W;
    layout.title_slot.h = SLOT_H;

    /* Date slot */
    layout.date_slot.x = SLOT_X;
    layout.date_slot.y = DATE_Y;
    layout.date_slot.w = SLOT_W;
    layout.date_slot.h = SLOT_H;

    /* Day slot */
    layout.day_slot.x = SLOT_X;
    layout.day_slot.y = DAY_Y;
    layout.day_slot.w = SLOT_W;
    layout.day_slot.h = SLOT_H;

    /* Month slot */
    layout.month_slot.x = SLOT_X;
    layout.month_slot.y = MONTH_Y;
    layout.month_slot.w = SLOT_W;
    layout.month_slot.h = SLOT_H;

    /* Season slot */
    layout.season_slot.x = SLOT_X;
    layout.season_slot.y = SEASON_Y;
    layout.season_slot.w = SLOT_W;
    layout.season_slot.h = SLOT_H;

    /* Quality slot */
    layout.quality_slot.x = SLOT_X;
    layout.quality_slot.y = QUALITY_Y;
    layout.quality_slot.w = SLOT_W;
    layout.quality_slot.h = SLOT_H;

    /* Card frame */
    layout.card_x = CARD_X;
    layout.card_y = CARD_Y;
    layout.card_w = CARD_W;
    layout.card_h = CARD_H;

    return layout;
}

const char *daily_gregorian_day_planet(const daily_gregorian_layout_t *layout)
{
    if (!layout) {
        return "?";
    }
    return layout->day_planet;
}
