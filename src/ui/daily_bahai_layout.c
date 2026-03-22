/* daily_bahai_layout.c — Daily Baha'i (Badi') calendar card layout
 *
 * Converts JD to Baha'i date, looks up month, holy day, cycles,
 * and interpretation, then fills card slots.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "daily_bahai_layout.h"
#include "../systems/bahai/bahai.h"
#include "../systems/bahai/bahai_interpret.h"
#include <string.h>
#include <stdio.h>

/* Slot layout constants */
#define SLOT_X       0.10f
#define SLOT_W       0.80f
#define SLOT_H       0.10f

#define TITLE_Y      0.05f
#define DATE_Y       0.19f
#define MONTH_Y      0.33f
#define HOLY_DAY_Y   0.47f
#define CYCLE_Y      0.61f
#define AYYAM_Y      0.75f

/* Card frame */
#define CARD_X       0.05f
#define CARD_Y       0.02f
#define CARD_W       0.90f
#define CARD_H       0.96f

static bahai_slot_t make_slot(float y)
{
    bahai_slot_t s;
    s.x = SLOT_X;
    s.y = y;
    s.w = SLOT_W;
    s.h = SLOT_H;
    return s;
}

daily_bahai_layout_t daily_bahai_compute(double jd)
{
    daily_bahai_layout_t layout;
    memset(&layout, 0, sizeof(layout));

    /* Convert JD to Baha'i date */
    layout.date = bahai_from_jd(jd);
    layout.month_name = bahai_month_name(layout.date.month);
    layout.is_ayyam_i_ha = bahai_is_ayyam_i_ha(layout.date) ? 1 : 0;

    /* Holy day */
    bahai_holy_day_t hd = bahai_holy_day(layout.date);
    layout.holy_day = (int)hd;
    if (hd != BAHAI_HOLY_NONE) {
        layout.holy_day_name = bahai_holy_day_name(hd);
    } else {
        layout.holy_day_name = "";
    }

    /* Cycles */
    layout.vahid = bahai_vahid(layout.date.year);
    layout.kull_i_shay = bahai_kull_i_shay(layout.date.year);

    /* Interpretation */
    bahai_interp_t interp = bhi_interpret(layout.date.month,
                                          layout.date.day,
                                          (int)hd);
    snprintf(layout.glance, sizeof(layout.glance), "%s", interp.glance);
    snprintf(layout.detail, sizeof(layout.detail), "%.511s", interp.detail);

    /* Layout slots */
    layout.title_slot    = make_slot(TITLE_Y);
    layout.date_slot     = make_slot(DATE_Y);
    layout.month_slot    = make_slot(MONTH_Y);
    layout.holy_day_slot = make_slot(HOLY_DAY_Y);
    layout.cycle_slot    = make_slot(CYCLE_Y);
    layout.ayyam_slot    = make_slot(AYYAM_Y);

    /* Card frame */
    layout.card_x = CARD_X;
    layout.card_y = CARD_Y;
    layout.card_w = CARD_W;
    layout.card_h = CARD_H;

    return layout;
}

int daily_bahai_has_holy_day(const daily_bahai_layout_t *layout)
{
    if (!layout) {
        return 0;
    }
    return (layout->holy_day != BAHAI_HOLY_NONE) ? 1 : 0;
}
