/* daily_persian_layout.c — Daily Persian/Solar Hijri calendar card layout
 *
 * Converts JD to Persian date, looks up Avestan month interpretation
 * and active festival, computes card slot positions for rendering.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "daily_persian_layout.h"
#include "../systems/persian/persian.h"
#include "../systems/persian/persian_interpret.h"
#include <string.h>

/* Slot layout constants */
#define SLOT_X       0.10f
#define SLOT_W       0.80f
#define SLOT_H       0.08f

#define TITLE_Y      0.04f
#define DATE_Y       0.13f
#define MONTH_Y      0.24f
#define MEANING_Y    0.35f
#define SEASON_Y     0.47f
#define FESTIVAL_Y   0.60f
#define BRIEF_Y      0.74f

/* Card frame */
#define CARD_X       0.05f
#define CARD_Y       0.02f
#define CARD_W       0.90f
#define CARD_H       0.96f

static persian_slot_t make_slot(float y)
{
    persian_slot_t s;
    s.x = SLOT_X;
    s.y = y;
    s.w = SLOT_W;
    s.h = SLOT_H;
    return s;
}

daily_persian_layout_t daily_persian_compute(double jd)
{
    daily_persian_layout_t layout;
    memset(&layout, 0, sizeof(layout));

    /* Convert JD to Persian date */
    layout.date = persian_from_jd(jd);
    layout.month_name = persian_month_name(layout.date.month);
    layout.weekday_name = persian_weekday_name(persian_weekday(jd));

    /* Get month interpretation */
    pi_month_t m = pi_month_data(layout.date.month);
    layout.avestan = m.avestan;
    layout.meaning = m.meaning;
    layout.season = m.season;
    layout.brief = m.brief;

    /* Check for festival */
    persian_festival_t fest = persian_festival(layout.date);
    if (fest != PERSIAN_FEST_NONE) {
        layout.has_festival = 1;
        layout.festival_name = persian_festival_name(fest);
    } else {
        layout.has_festival = 0;
        layout.festival_name = "";
    }

    /* Layout slots */
    layout.title_slot    = make_slot(TITLE_Y);
    layout.date_slot     = make_slot(DATE_Y);
    layout.month_slot    = make_slot(MONTH_Y);
    layout.meaning_slot  = make_slot(MEANING_Y);
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

const char *daily_persian_avestan(const daily_persian_layout_t *layout)
{
    if (!layout) {
        return "?";
    }
    return layout->avestan;
}

int daily_persian_has_festival(const daily_persian_layout_t *layout)
{
    if (!layout) {
        return 0;
    }
    return layout->has_festival;
}
