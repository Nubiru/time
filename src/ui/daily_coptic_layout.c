/* daily_coptic_layout.c — Daily Coptic calendar card layout
 *
 * Converts JD to Coptic date, looks up month interpretation and
 * active feast, computes card slot positions for rendering.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "daily_coptic_layout.h"
#include "../systems/coptic/coptic.h"
#include "../systems/coptic/coptic_interpret.h"
#include <string.h>

/* Slot layout constants */
#define SLOT_X       0.10f
#define SLOT_W       0.80f
#define SLOT_H       0.10f

#define TITLE_Y      0.05f
#define DATE_Y       0.15f
#define MONTH_Y      0.28f
#define LITURGY_Y    0.40f
#define FEAST_Y      0.55f
#define BRIEF_Y      0.72f

/* Card frame */
#define CARD_X       0.05f
#define CARD_Y       0.02f
#define CARD_W       0.90f
#define CARD_H       0.96f

static coptic_slot_t make_slot(float y)
{
    coptic_slot_t s;
    s.x = SLOT_X;
    s.y = y;
    s.w = SLOT_W;
    s.h = SLOT_H;
    return s;
}

daily_coptic_layout_t daily_coptic_compute(double jd)
{
    daily_coptic_layout_t layout;
    memset(&layout, 0, sizeof(layout));

    /* Convert JD to Coptic date */
    layout.date = coptic_from_jd(jd);
    layout.month_name = coptic_month_name(layout.date.month);
    layout.is_leap = coptic_is_leap(layout.date.year) ? 1 : 0;

    /* Get month interpretation */
    ci_coptic_month_t m = cci_month_data(layout.date.month);
    layout.origin = m.origin;
    layout.liturgy = m.liturgy;
    layout.brief = m.brief;

    /* Check for feast */
    coptic_feast_t feast = coptic_feast(layout.date);
    if (feast != COPTIC_FEAST_NONE) {
        layout.has_feast = 1;
        layout.feast_name = coptic_feast_name(feast);
    } else {
        layout.has_feast = 0;
        layout.feast_name = "";
    }

    /* Layout slots */
    layout.title_slot   = make_slot(TITLE_Y);
    layout.date_slot    = make_slot(DATE_Y);
    layout.month_slot   = make_slot(MONTH_Y);
    layout.liturgy_slot = make_slot(LITURGY_Y);
    layout.feast_slot   = make_slot(FEAST_Y);
    layout.brief_slot   = make_slot(BRIEF_Y);

    /* Card frame */
    layout.card_x = CARD_X;
    layout.card_y = CARD_Y;
    layout.card_w = CARD_W;
    layout.card_h = CARD_H;

    return layout;
}

const char *daily_coptic_month_origin(const daily_coptic_layout_t *layout)
{
    if (!layout) {
        return "?";
    }
    return layout->origin;
}

int daily_coptic_has_feast(const daily_coptic_layout_t *layout)
{
    if (!layout) {
        return 0;
    }
    return layout->has_feast;
}
