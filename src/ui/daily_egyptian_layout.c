/* daily_egyptian_layout.c — Daily Egyptian calendar card layout
 *
 * Converts JD to Egyptian civil date, looks up season, month,
 * Sothic cycle, and interpretation, then fills card slots.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "daily_egyptian_layout.h"
#include "../systems/egyptian/egyptian.h"
#include "../systems/egyptian/egyptian_interpret.h"
#include <string.h>
#include <stdio.h>

/* Slot layout constants */
#define SLOT_X       0.10f
#define SLOT_W       0.80f
#define SLOT_H       0.10f

#define TITLE_Y      0.05f
#define DATE_Y       0.19f
#define MONTH_Y      0.33f
#define SEASON_Y     0.47f
#define SOTHIC_Y     0.61f
#define EPAG_Y       0.75f

/* Card frame */
#define CARD_X       0.05f
#define CARD_Y       0.02f
#define CARD_W       0.90f
#define CARD_H       0.96f

static egypt_slot_t make_slot(float y)
{
    egypt_slot_t s;
    s.x = SLOT_X;
    s.y = y;
    s.w = SLOT_W;
    s.h = SLOT_H;
    return s;
}

daily_egyptian_layout_t daily_egyptian_compute(double jd)
{
    daily_egyptian_layout_t layout;
    memset(&layout, 0, sizeof(layout));

    /* Convert JD to Egyptian date */
    layout.date = egypt_from_jd(jd);
    layout.month_name = egypt_month_name(layout.date.month);
    layout.is_epagomenal = egypt_is_epagomenal(layout.date) ? 1 : 0;

    /* Season */
    layout.season = egypt_season(layout.date.month);
    layout.season_name = egypt_season_name(layout.season);

    /* Sothic cycle */
    layout.sothic = egypt_sothic_position(jd);

    /* Interpretation */
    egyptian_interp_t interp = ei_interpret(layout.date.month,
                                            layout.date.day,
                                            layout.sothic.cycle_year);
    snprintf(layout.glance, sizeof(layout.glance), "%s", interp.glance);
    snprintf(layout.detail, sizeof(layout.detail), "%.511s", interp.detail);

    /* Layout slots */
    layout.title_slot  = make_slot(TITLE_Y);
    layout.date_slot   = make_slot(DATE_Y);
    layout.month_slot  = make_slot(MONTH_Y);
    layout.season_slot = make_slot(SEASON_Y);
    layout.sothic_slot = make_slot(SOTHIC_Y);
    layout.epag_slot   = make_slot(EPAG_Y);

    /* Card frame */
    layout.card_x = CARD_X;
    layout.card_y = CARD_Y;
    layout.card_w = CARD_W;
    layout.card_h = CARD_H;

    return layout;
}

int daily_egyptian_is_epagomenal(const daily_egyptian_layout_t *layout)
{
    if (!layout) {
        return 0;
    }
    return layout->is_epagomenal;
}
