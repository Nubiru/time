/* daily_tamil_layout.c — Daily Tamil calendar card layout
 *
 * Converts JD to Tamil sidereal solar date, looks up month,
 * Jovian year, festival, and interpretation, fills card slots.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "daily_tamil_layout.h"
#include "../systems/tamil/tamil_calendar.h"
#include "../systems/tamil/tamil_interpret.h"
#include <string.h>
#include <stdio.h>

/* Slot layout constants */
#define SLOT_X       0.10f
#define SLOT_W       0.80f
#define SLOT_H       0.10f

#define TITLE_Y      0.05f
#define DATE_Y       0.19f
#define MONTH_Y      0.33f
#define JOVIAN_Y     0.47f
#define FESTIVAL_Y   0.61f
#define INFO_Y       0.75f

/* Card frame */
#define CARD_X       0.05f
#define CARD_Y       0.02f
#define CARD_W       0.90f
#define CARD_H       0.96f

static tamil_slot_t make_slot(float y)
{
    tamil_slot_t s;
    s.x = SLOT_X;
    s.y = y;
    s.w = SLOT_W;
    s.h = SLOT_H;
    return s;
}

daily_tamil_layout_t daily_tamil_compute(double jd)
{
    daily_tamil_layout_t layout;
    memset(&layout, 0, sizeof(layout));

    /* Convert JD to Tamil date */
    layout.date = tamil_from_jd(jd);
    layout.month_name = tamil_month_name(layout.date.month);
    layout.jovian_name = tamil_jovian_name(layout.date.jovian);

    /* Festival */
    tamil_festival_t fest = tamil_festival(layout.date);
    layout.festival = (int)fest;
    if (fest != TAMIL_FEST_NONE) {
        layout.festival_name = tamil_festival_name(fest);
    } else {
        layout.festival_name = "";
    }

    /* Interpretation */
    tamil_interp_t interp = tmi_interpret(layout.date.month,
                                          layout.date.day,
                                          layout.date.jovian,
                                          (int)fest);
    snprintf(layout.glance, sizeof(layout.glance), "%s", interp.glance);
    snprintf(layout.detail, sizeof(layout.detail), "%.511s", interp.detail);

    /* Layout slots */
    layout.title_slot    = make_slot(TITLE_Y);
    layout.date_slot     = make_slot(DATE_Y);
    layout.month_slot    = make_slot(MONTH_Y);
    layout.jovian_slot   = make_slot(JOVIAN_Y);
    layout.festival_slot = make_slot(FESTIVAL_Y);
    layout.info_slot     = make_slot(INFO_Y);

    /* Card frame */
    layout.card_x = CARD_X;
    layout.card_y = CARD_Y;
    layout.card_w = CARD_W;
    layout.card_h = CARD_H;

    return layout;
}

int daily_tamil_has_festival(const daily_tamil_layout_t *layout)
{
    if (!layout) {
        return 0;
    }
    return (layout->festival != TAMIL_FEST_NONE) ? 1 : 0;
}
