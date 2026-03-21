/* daily_haab_layout.c — Daily Haab solar calendar card layout
 *
 * Converts JD to Haab date, looks up vinal wisdom and Calendar
 * Round position, computes card slot positions for rendering.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "daily_haab_layout.h"
#include "../systems/tzolkin/haab.h"
#include "../systems/tzolkin/haab_vinal.h"
#include <string.h>

/* Slot layout constants */
#define SLOT_X       0.10f
#define SLOT_W       0.80f
#define SLOT_H       0.09f

#define TITLE_Y      0.04f
#define DATE_Y       0.14f
#define MEANING_Y    0.26f
#define WISDOM_Y     0.40f
#define ROUND_Y      0.56f
#define PROGRESS_Y   0.70f

/* Card frame */
#define CARD_X       0.05f
#define CARD_Y       0.02f
#define CARD_W       0.90f
#define CARD_H       0.96f

static haab_slot_t make_slot(float y)
{
    haab_slot_t s;
    s.x = SLOT_X;
    s.y = y;
    s.w = SLOT_W;
    s.h = SLOT_H;
    return s;
}

daily_haab_layout_t daily_haab_compute(double jd)
{
    daily_haab_layout_t layout;
    memset(&layout, 0, sizeof(layout));

    /* Convert JD to Haab date */
    layout.date = haab_from_jd(jd);
    layout.month_name = haab_month_name(layout.date.month);
    layout.month_meaning = haab_month_meaning(layout.date.month);
    layout.day_of_year = haab_day_of_year(layout.date);

    /* Vinal wisdom */
    layout.vinal_number = hv_vinal_for_day_of_year(layout.day_of_year);
    if (layout.vinal_number > 0) {
        layout.vinal_wisdom = hv_vinal_meaning(layout.vinal_number);
        layout.is_wayeb = hv_is_uayeb(layout.vinal_number);
    } else {
        layout.vinal_wisdom = "?";
        layout.is_wayeb = 0;
    }

    /* Calendar Round */
    layout.cal_round = calendar_round_from_jd(jd);
    calendar_round_fmt(layout.cal_round, layout.round_fmt,
                       sizeof(layout.round_fmt));

    /* Layout slots */
    layout.title_slot    = make_slot(TITLE_Y);
    layout.date_slot     = make_slot(DATE_Y);
    layout.meaning_slot  = make_slot(MEANING_Y);
    layout.wisdom_slot   = make_slot(WISDOM_Y);
    layout.round_slot    = make_slot(ROUND_Y);
    layout.progress_slot = make_slot(PROGRESS_Y);

    /* Card frame */
    layout.card_x = CARD_X;
    layout.card_y = CARD_Y;
    layout.card_w = CARD_W;
    layout.card_h = CARD_H;

    return layout;
}

const char *daily_haab_wisdom(const daily_haab_layout_t *layout)
{
    if (!layout) {
        return "?";
    }
    return layout->vinal_wisdom;
}

int daily_haab_is_wayeb(const daily_haab_layout_t *layout)
{
    if (!layout) {
        return 0;
    }
    return layout->is_wayeb;
}
