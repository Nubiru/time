/* daily_celtic_layout.c — Daily Celtic Tree calendar card layout
 *
 * Converts JD to Celtic tree month, looks up Ogham data and
 * nearest Wheel of the Year festival, fills card slots.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "daily_celtic_layout.h"
#include "../systems/celtic/celtic_tree.h"
#include "../systems/celtic/celtic_interpret.h"
#include <string.h>
#include <stdio.h>

/* Slot layout constants */
#define SLOT_X       0.10f
#define SLOT_W       0.80f
#define SLOT_H       0.10f

#define TITLE_Y      0.05f
#define DATE_Y       0.19f
#define TREE_Y       0.33f
#define OGHAM_Y      0.47f
#define FESTIVAL_Y   0.61f
#define MEANING_Y    0.75f

/* Card frame */
#define CARD_X       0.05f
#define CARD_Y       0.02f
#define CARD_W       0.90f
#define CARD_H       0.96f

static celtic_slot_t make_slot(float y)
{
    celtic_slot_t s;
    s.x = SLOT_X;
    s.y = y;
    s.w = SLOT_W;
    s.h = SLOT_H;
    return s;
}

daily_celtic_layout_t daily_celtic_compute(double jd)
{
    daily_celtic_layout_t layout;
    memset(&layout, 0, sizeof(layout));

    /* Convert JD to Celtic tree date */
    layout.date = celtic_tree_from_jd(jd);
    layout.tree_name = celtic_tree_name(layout.date.month);
    layout.ogham_name = celtic_tree_ogham(layout.date.month);
    layout.is_intercalary = celtic_tree_is_intercalary(layout.date) ? 1 : 0;

    /* Festival: check if any Wheel of the Year festival falls in
     * this tree month.  celtic_tree_for_festival(fest_idx) returns the
     * tree month containing festival fest_idx. */
    layout.festival = -1;
    for (int fi = 0; fi < 8; fi++) {
        if (celtic_tree_for_festival(fi) == layout.date.month) {
            layout.festival = fi;
            break;
        }
    }

    /* Interpretation */
    celtic_interp_t interp = cti_interpret(layout.date.month,
                                           layout.date.day,
                                           layout.festival);
    snprintf(layout.glance, sizeof(layout.glance), "%s", interp.glance);
    snprintf(layout.detail, sizeof(layout.detail), "%.511s", interp.detail);

    /* Layout slots */
    layout.title_slot    = make_slot(TITLE_Y);
    layout.date_slot     = make_slot(DATE_Y);
    layout.tree_slot     = make_slot(TREE_Y);
    layout.ogham_slot    = make_slot(OGHAM_Y);
    layout.festival_slot = make_slot(FESTIVAL_Y);
    layout.meaning_slot  = make_slot(MEANING_Y);

    /* Card frame */
    layout.card_x = CARD_X;
    layout.card_y = CARD_Y;
    layout.card_w = CARD_W;
    layout.card_h = CARD_H;

    return layout;
}

int daily_celtic_has_festival(const daily_celtic_layout_t *layout)
{
    if (!layout) {
        return 0;
    }
    return (layout->festival >= 0) ? 1 : 0;
}
