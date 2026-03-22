/* daily_myanmar_layout.c — Daily Myanmar calendar card layout
 *
 * Converts JD to Myanmar lunisolar date, checks Thingyan,
 * computes interpretation, then fills card slots.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "daily_myanmar_layout.h"
#include "../systems/myanmar/myanmar.h"
#include "../systems/myanmar/myanmar_interpret.h"
#include <string.h>
#include <stdio.h>

/* Slot layout constants */
#define SLOT_X       0.10f
#define SLOT_W       0.80f
#define SLOT_H       0.10f

#define TITLE_Y      0.05f
#define DATE_Y       0.19f
#define MONTH_Y      0.33f
#define YEAR_TYPE_Y  0.47f
#define THINGYAN_Y   0.61f
#define INFO_Y       0.75f

/* Card frame */
#define CARD_X       0.05f
#define CARD_Y       0.02f
#define CARD_W       0.90f
#define CARD_H       0.96f

static my_slot_t make_slot(float y)
{
    my_slot_t s;
    s.x = SLOT_X;
    s.y = y;
    s.w = SLOT_W;
    s.h = SLOT_H;
    return s;
}

daily_myanmar_layout_t daily_myanmar_compute(double jd)
{
    daily_myanmar_layout_t layout;
    memset(&layout, 0, sizeof(layout));

    /* Convert JD to Myanmar date */
    layout.date = myanmar_from_jd(jd);
    layout.month_name = myanmar_month_name(layout.date.month);

    /* Year type */
    myanmar_year_type_t yt = myanmar_year_type(layout.date.year);
    layout.year_type = (int)yt;
    layout.year_type_name = myanmar_year_type_name(yt);

    /* Thingyan check: see if JD falls between akyo and new year */
    myanmar_thingyan_t thingyan = myanmar_thingyan(layout.date.year);
    layout.is_thingyan = (jd >= thingyan.akyo_jd &&
                          jd < thingyan.new_year_jd) ? 1 : 0;

    /* Interpretation */
    myanmar_interp_t interp = mmi_interpret((int)layout.date.month,
                                            layout.date.day,
                                            layout.is_thingyan,
                                            layout.year_type);
    snprintf(layout.glance, sizeof(layout.glance), "%s", interp.glance);
    snprintf(layout.detail, sizeof(layout.detail), "%.511s", interp.detail);

    /* Layout slots */
    layout.title_slot     = make_slot(TITLE_Y);
    layout.date_slot      = make_slot(DATE_Y);
    layout.month_slot     = make_slot(MONTH_Y);
    layout.year_type_slot = make_slot(YEAR_TYPE_Y);
    layout.thingyan_slot  = make_slot(THINGYAN_Y);
    layout.info_slot      = make_slot(INFO_Y);

    /* Card frame */
    layout.card_x = CARD_X;
    layout.card_y = CARD_Y;
    layout.card_w = CARD_W;
    layout.card_h = CARD_H;

    return layout;
}

int daily_myanmar_is_thingyan(const daily_myanmar_layout_t *layout)
{
    if (!layout) {
        return 0;
    }
    return layout->is_thingyan;
}
