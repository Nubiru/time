/* daily_zoroastrian_layout.c — Daily Zoroastrian calendar card layout
 *
 * Converts JD to Yazdgerdi date, looks up day/month names,
 * festival status, and interpretation, then fills card slots.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "daily_zoroastrian_layout.h"
#include "../systems/zoroastrian/zoroastrian.h"
#include "../systems/zoroastrian/zoroastrian_interpret.h"
#include <string.h>
#include <stdio.h>

/* Slot layout constants */
#define SLOT_X       0.10f
#define SLOT_W       0.80f
#define SLOT_H       0.10f

#define TITLE_Y      0.05f
#define DATE_Y       0.19f
#define MONTH_Y      0.33f
#define DAY_NAME_Y   0.47f
#define FESTIVAL_Y   0.61f
#define GATHA_Y      0.75f

/* Card frame */
#define CARD_X       0.05f
#define CARD_Y       0.02f
#define CARD_W       0.90f
#define CARD_H       0.96f

static zoro_slot_t make_slot(float y)
{
    zoro_slot_t s;
    s.x = SLOT_X;
    s.y = y;
    s.w = SLOT_W;
    s.h = SLOT_H;
    return s;
}

daily_zoroastrian_layout_t daily_zoroastrian_compute(double jd)
{
    daily_zoroastrian_layout_t layout;
    memset(&layout, 0, sizeof(layout));

    /* Convert JD to Zoroastrian date */
    layout.date = zoro_from_jd(jd);
    layout.month_name = zoro_month_name(layout.date.month);
    layout.day_name = zoro_day_name(layout.date.day);
    layout.is_gatha = zoro_is_gatha(layout.date) ? 1 : 0;

    /* Festival */
    zoro_festival_t fest = zoro_festival(layout.date);
    layout.festival = (int)fest;
    if (fest != ZORO_FEST_NONE) {
        layout.festival_name = zoro_festival_name(fest);
    } else {
        layout.festival_name = "";
    }

    /* Interpretation */
    zoroastrian_interp_t interp = zi_interpret(layout.date.month,
                                               layout.date.day,
                                               (int)fest);
    snprintf(layout.glance, sizeof(layout.glance), "%s", interp.glance);
    snprintf(layout.detail, sizeof(layout.detail), "%.511s", interp.detail);

    /* Layout slots */
    layout.title_slot    = make_slot(TITLE_Y);
    layout.date_slot     = make_slot(DATE_Y);
    layout.month_slot    = make_slot(MONTH_Y);
    layout.day_name_slot = make_slot(DAY_NAME_Y);
    layout.festival_slot = make_slot(FESTIVAL_Y);
    layout.gatha_slot    = make_slot(GATHA_Y);

    /* Card frame */
    layout.card_x = CARD_X;
    layout.card_y = CARD_Y;
    layout.card_w = CARD_W;
    layout.card_h = CARD_H;

    return layout;
}

int daily_zoroastrian_has_festival(const daily_zoroastrian_layout_t *layout)
{
    if (!layout) {
        return 0;
    }
    return (layout->festival != ZORO_FEST_NONE) ? 1 : 0;
}
