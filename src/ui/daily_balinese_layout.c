/* daily_balinese_layout.c — Daily Balinese Pawukon card layout
 *
 * Converts JD to Pawukon 210-day cycle, computes urip, checks
 * for Tumpek/Kajeng Kliwon/ceremony, fills card slots.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "daily_balinese_layout.h"
#include "../systems/balinese/pawukon.h"
#include "../systems/balinese/pawukon_interpret.h"
#include <string.h>
#include <stdio.h>

/* Slot layout constants */
#define SLOT_X       0.10f
#define SLOT_W       0.80f
#define SLOT_H       0.10f

#define TITLE_Y      0.05f
#define WUKU_Y       0.19f
#define CYCLE_Y      0.33f
#define URIP_Y       0.47f
#define CEREMONY_Y   0.61f
#define SPECIAL_Y    0.75f

/* Card frame */
#define CARD_X       0.05f
#define CARD_Y       0.02f
#define CARD_W       0.90f
#define CARD_H       0.96f

static bali_slot_t make_slot(float y)
{
    bali_slot_t s;
    s.x = SLOT_X;
    s.y = y;
    s.w = SLOT_W;
    s.h = SLOT_H;
    return s;
}

daily_balinese_layout_t daily_balinese_compute(double jd)
{
    daily_balinese_layout_t layout;
    memset(&layout, 0, sizeof(layout));

    /* Pawukon from JD */
    layout.pawukon = pawukon_from_jd(jd);
    layout.wuku_name = pawukon_wuku_name(layout.pawukon.wuku);
    layout.urip = pawukon_urip(layout.pawukon);
    layout.is_tumpek = pawukon_is_tumpek(layout.pawukon);
    layout.is_kajeng_kliwon = pawukon_is_kajeng_kliwon(layout.pawukon);
    layout.ceremony = pawukon_ceremony(layout.pawukon);

    /* Interpretation */
    pawukon_interp_t interp = pwi_interpret(layout.pawukon.wuku,
                                            layout.pawukon.wuku_day + 1,
                                            layout.urip,
                                            layout.is_tumpek,
                                            layout.is_kajeng_kliwon);
    snprintf(layout.glance, sizeof(layout.glance), "%s", interp.glance);
    snprintf(layout.detail, sizeof(layout.detail), "%.511s", interp.detail);

    /* Layout slots */
    layout.title_slot    = make_slot(TITLE_Y);
    layout.wuku_slot     = make_slot(WUKU_Y);
    layout.cycle_slot    = make_slot(CYCLE_Y);
    layout.urip_slot     = make_slot(URIP_Y);
    layout.ceremony_slot = make_slot(CEREMONY_Y);
    layout.special_slot  = make_slot(SPECIAL_Y);

    /* Card frame */
    layout.card_x = CARD_X;
    layout.card_y = CARD_Y;
    layout.card_w = CARD_W;
    layout.card_h = CARD_H;

    return layout;
}

int daily_balinese_has_ceremony(const daily_balinese_layout_t *layout)
{
    if (!layout) {
        return 0;
    }
    return (layout->ceremony != NULL) ? 1 : 0;
}
