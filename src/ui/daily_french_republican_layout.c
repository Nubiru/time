/* daily_french_republican_layout.c — Daily French Republican card layout
 *
 * Converts JD to French Republican date, looks up month and day
 * names, computes interpretation, then fills card slots.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "daily_french_republican_layout.h"
#include "../systems/french_republican/french_republican.h"
#include "../systems/french_republican/french_republican_interpret.h"
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
#define SEASON_Y     0.61f
#define INFO_Y       0.75f

/* Card frame */
#define CARD_X       0.05f
#define CARD_Y       0.02f
#define CARD_W       0.90f
#define CARD_H       0.96f

static fr_slot_t make_slot(float y)
{
    fr_slot_t s;
    s.x = SLOT_X;
    s.y = y;
    s.w = SLOT_W;
    s.h = SLOT_H;
    return s;
}

daily_french_republican_layout_t daily_french_republican_compute(double jd)
{
    daily_french_republican_layout_t layout;
    memset(&layout, 0, sizeof(layout));

    /* Convert JD to French Republican date */
    layout.date = fr_from_jd(jd);
    layout.month_name = fr_month_name(layout.date.month);
    layout.is_sansculottide = fr_is_sansculottide(layout.date) ? 1 : 0;

    /* Day name: decade day or Sansculottide name */
    if (layout.is_sansculottide) {
        layout.day_name = fr_sansculottide_name(layout.date.day);
    } else {
        int dd = fr_decade_day(layout.date);
        layout.day_name = fr_decade_day_name(dd);
    }

    /* Interpretation */
    french_republican_interp_t interp = fri_interpret(layout.date.year,
                                                      layout.date.month,
                                                      layout.date.day);
    snprintf(layout.glance, sizeof(layout.glance), "%s", interp.glance);
    snprintf(layout.detail, sizeof(layout.detail), "%.511s", interp.detail);

    /* Layout slots */
    layout.title_slot    = make_slot(TITLE_Y);
    layout.date_slot     = make_slot(DATE_Y);
    layout.month_slot    = make_slot(MONTH_Y);
    layout.day_name_slot = make_slot(DAY_NAME_Y);
    layout.season_slot   = make_slot(SEASON_Y);
    layout.info_slot     = make_slot(INFO_Y);

    /* Card frame */
    layout.card_x = CARD_X;
    layout.card_y = CARD_Y;
    layout.card_w = CARD_W;
    layout.card_h = CARD_H;

    return layout;
}

int daily_french_republican_is_sansculottide(
    const daily_french_republican_layout_t *layout)
{
    if (!layout) {
        return 0;
    }
    return layout->is_sansculottide;
}
