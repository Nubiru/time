/* daily_lao_layout.c — Daily Lao Buddhist calendar card layout
 *
 * Converts JD to Lao date, looks up month, festival, zodiac,
 * and interpretation, then fills card slots.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "daily_lao_layout.h"
#include "../systems/lao/lao_calendar.h"
#include "../systems/lao/lao_interpret.h"
#include <string.h>
#include <stdio.h>

/* Slot layout constants */
#define SLOT_X       0.10f
#define SLOT_W       0.80f
#define SLOT_H       0.10f

#define TITLE_Y      0.05f
#define DATE_Y       0.19f
#define MONTH_Y      0.33f
#define FESTIVAL_Y   0.47f
#define ZODIAC_Y     0.61f
#define DAY_NAME_Y   0.75f

/* Card frame */
#define CARD_X       0.05f
#define CARD_Y       0.02f
#define CARD_W       0.90f
#define CARD_H       0.96f

static lao_slot_t make_slot(float y)
{
    lao_slot_t s;
    s.x = SLOT_X;
    s.y = y;
    s.w = SLOT_W;
    s.h = SLOT_H;
    return s;
}

daily_lao_layout_t daily_lao_compute(double jd)
{
    daily_lao_layout_t layout;
    memset(&layout, 0, sizeof(layout));

    /* Convert JD to Lao date */
    layout.date = lao_from_jd(jd);
    layout.month_name = lao_month_name(layout.date.month);

    /* Day of week */
    int weekday = lao_day_of_week(layout.date);
    layout.day_name = lao_day_name(weekday);

    /* Festival */
    lao_festival_t fest = lao_festival(layout.date);
    layout.festival = (int)fest;
    if (fest != LAO_FEST_NONE) {
        layout.festival_name = lao_festival_name(fest);
    } else {
        layout.festival_name = "";
    }

    /* Zodiac */
    layout.zodiac_animal = lao_zodiac_animal(layout.date.be_year);

    /* Interpretation */
    lao_interp_t interp = lai_interpret(layout.date.be_year,
                                        layout.date.month,
                                        layout.date.day,
                                        (int)fest);
    snprintf(layout.glance, sizeof(layout.glance), "%s", interp.glance);
    snprintf(layout.detail, sizeof(layout.detail), "%.511s", interp.detail);

    /* Layout slots */
    layout.title_slot    = make_slot(TITLE_Y);
    layout.date_slot     = make_slot(DATE_Y);
    layout.month_slot    = make_slot(MONTH_Y);
    layout.festival_slot = make_slot(FESTIVAL_Y);
    layout.zodiac_slot   = make_slot(ZODIAC_Y);
    layout.day_name_slot = make_slot(DAY_NAME_Y);

    /* Card frame */
    layout.card_x = CARD_X;
    layout.card_y = CARD_Y;
    layout.card_w = CARD_W;
    layout.card_h = CARD_H;

    return layout;
}

int daily_lao_has_festival(const daily_lao_layout_t *layout)
{
    if (!layout) {
        return 0;
    }
    return (layout->festival != LAO_FEST_NONE) ? 1 : 0;
}
