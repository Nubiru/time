/* daily_thai_layout.c — Daily Thai calendar card layout
 *
 * Converts JD to Thai date with Buddhist Era year, looks up month
 * interpretation and active festival, computes card slot positions.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "daily_thai_layout.h"
#include "../systems/thai/thai_calendar.h"
#include "../systems/thai/thai_interpret.h"
#include <string.h>

/* Slot layout constants */
#define SLOT_X       0.10f
#define SLOT_W       0.80f
#define SLOT_H       0.10f

#define TITLE_Y      0.05f
#define DATE_Y       0.15f
#define MONTH_Y      0.28f
#define SEASON_Y     0.40f
#define FESTIVAL_Y   0.55f
#define BRIEF_Y      0.72f

/* Card frame */
#define CARD_X       0.05f
#define CARD_Y       0.02f
#define CARD_W       0.90f
#define CARD_H       0.96f

static thai_slot_t make_slot(float y)
{
    thai_slot_t s;
    s.x = SLOT_X;
    s.y = y;
    s.w = SLOT_W;
    s.h = SLOT_H;
    return s;
}

daily_thai_layout_t daily_thai_compute(double jd)
{
    daily_thai_layout_t layout;
    memset(&layout, 0, sizeof(layout));

    /* Convert JD to Thai date */
    layout.date = thai_from_jd(jd);
    layout.month_name = thai_month_name(layout.date.month);
    layout.month_abbr = thai_month_abbr(layout.date.month);
    layout.is_songkran = thai_is_songkran(layout.date) ? 1 : 0;

    /* Get month interpretation */
    ti_thai_month_t m = tti_month_data(layout.date.month);
    layout.origin = m.origin;
    layout.season = m.season;
    layout.brief = m.brief;

    /* Check for festival */
    thai_festival_t fest = thai_festival(layout.date);
    if (fest != THAI_FEST_NONE) {
        layout.has_festival = 1;
        layout.festival_name = thai_festival_name(fest);
    } else {
        layout.has_festival = 0;
        layout.festival_name = "";
    }

    /* Layout slots */
    layout.title_slot    = make_slot(TITLE_Y);
    layout.date_slot     = make_slot(DATE_Y);
    layout.month_slot    = make_slot(MONTH_Y);
    layout.season_slot   = make_slot(SEASON_Y);
    layout.festival_slot = make_slot(FESTIVAL_Y);
    layout.brief_slot    = make_slot(BRIEF_Y);

    /* Card frame */
    layout.card_x = CARD_X;
    layout.card_y = CARD_Y;
    layout.card_w = CARD_W;
    layout.card_h = CARD_H;

    return layout;
}

const char *daily_thai_month_origin(const daily_thai_layout_t *layout)
{
    if (!layout) {
        return "?";
    }
    return layout->origin;
}

int daily_thai_has_festival(const daily_thai_layout_t *layout)
{
    if (!layout) {
        return 0;
    }
    return layout->has_festival;
}
