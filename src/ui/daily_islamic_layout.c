/* daily_islamic_layout.c — Daily Islamic calendar card layout
 *
 * Converts JD to Hijri date, looks up month interpretation, and
 * computes card slot positions for rendering.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "daily_islamic_layout.h"
#include "../systems/islamic/hijri.h"
#include "../systems/islamic/islamic_interpret.h"
#include <string.h>

/* Slot layout constants */
#define SLOT_X       0.10f
#define SLOT_W       0.80f
#define SLOT_H       0.10f

#define TITLE_Y      0.05f
#define DATE_Y       0.18f
#define MONTH_Y      0.32f
#define SACRED_Y     0.45f
#define SIGNIF_Y     0.58f
#define EVENT_Y      0.72f

/* Card frame */
#define CARD_X       0.05f
#define CARD_Y       0.02f
#define CARD_W       0.90f
#define CARD_H       0.96f

daily_islamic_layout_t daily_islamic_compute(double jd)
{
    daily_islamic_layout_t layout;
    memset(&layout, 0, sizeof(layout));

    /* Convert JD to Hijri date */
    layout.date = hijri_from_jd(jd);

    /* Month name and leap status */
    layout.month_name = hijri_month_name(layout.date.month);
    layout.is_leap = hijri_is_leap(layout.date.year) ? 1 : 0;

    /* Get month interpretation */
    isi_month_t m = isi_month_data(layout.date.month);
    layout.is_sacred_month = m.is_sacred;
    layout.significance = m.significance;
    layout.event = m.event;
    layout.brief = m.brief;

    /* Title slot */
    layout.title_slot.x = SLOT_X;
    layout.title_slot.y = TITLE_Y;
    layout.title_slot.w = SLOT_W;
    layout.title_slot.h = SLOT_H;

    /* Date slot */
    layout.date_slot.x = SLOT_X;
    layout.date_slot.y = DATE_Y;
    layout.date_slot.w = SLOT_W;
    layout.date_slot.h = SLOT_H;

    /* Month slot */
    layout.month_slot.x = SLOT_X;
    layout.month_slot.y = MONTH_Y;
    layout.month_slot.w = SLOT_W;
    layout.month_slot.h = SLOT_H;

    /* Sacred slot */
    layout.sacred_slot.x = SLOT_X;
    layout.sacred_slot.y = SACRED_Y;
    layout.sacred_slot.w = SLOT_W;
    layout.sacred_slot.h = SLOT_H;

    /* Significance slot */
    layout.significance_slot.x = SLOT_X;
    layout.significance_slot.y = SIGNIF_Y;
    layout.significance_slot.w = SLOT_W;
    layout.significance_slot.h = SLOT_H;

    /* Event slot */
    layout.event_slot.x = SLOT_X;
    layout.event_slot.y = EVENT_Y;
    layout.event_slot.w = SLOT_W;
    layout.event_slot.h = SLOT_H;

    /* Card frame */
    layout.card_x = CARD_X;
    layout.card_y = CARD_Y;
    layout.card_w = CARD_W;
    layout.card_h = CARD_H;

    return layout;
}

int daily_islamic_is_sacred(const daily_islamic_layout_t *layout)
{
    if (!layout) {
        return 0;
    }
    return layout->is_sacred_month;
}
