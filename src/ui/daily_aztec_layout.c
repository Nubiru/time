/* daily_aztec_layout.c — Daily Aztec calendar card layout
 *
 * Converts JD to Tonalpohualli and Calendar Round positions,
 * computes interpretation, then fills card slots.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "daily_aztec_layout.h"
#include "../systems/aztec/aztec.h"
#include "../systems/aztec/aztec_interpret.h"
#include <string.h>
#include <stdio.h>

/* Slot layout constants */
#define SLOT_X       0.10f
#define SLOT_W       0.80f
#define SLOT_H       0.10f

#define TITLE_Y      0.05f
#define TONAL_Y      0.19f
#define SIGN_Y       0.33f
#define ROUND_Y      0.47f
#define BEARER_Y     0.61f
#define CYCLE_Y      0.75f

/* Card frame */
#define CARD_X       0.05f
#define CARD_Y       0.02f
#define CARD_W       0.90f
#define CARD_H       0.96f

static aztec_slot_t make_slot(float y)
{
    aztec_slot_t s;
    s.x = SLOT_X;
    s.y = y;
    s.w = SLOT_W;
    s.h = SLOT_H;
    return s;
}

daily_aztec_layout_t daily_aztec_compute(double jd)
{
    daily_aztec_layout_t layout;
    memset(&layout, 0, sizeof(layout));

    /* Tonalpohualli */
    layout.tonal = aztec_tonalpohualli(jd);
    layout.sign_name = aztec_day_sign_name(layout.tonal.day_sign);

    /* Calendar Round */
    layout.round = aztec_calendar_round(jd);
    layout.bearer_name = aztec_year_bearer_name(layout.round.year_bearer);

    /* Interpretation */
    aztec_interp_t interp = azi_interpret(layout.tonal.day_sign,
                                          layout.tonal.day_number,
                                          layout.round.year_bearer,
                                          layout.round.year_number);
    snprintf(layout.glance, sizeof(layout.glance), "%s", interp.glance);
    snprintf(layout.detail, sizeof(layout.detail), "%.511s", interp.detail);

    /* Layout slots */
    layout.title_slot  = make_slot(TITLE_Y);
    layout.tonal_slot  = make_slot(TONAL_Y);
    layout.sign_slot   = make_slot(SIGN_Y);
    layout.round_slot  = make_slot(ROUND_Y);
    layout.bearer_slot = make_slot(BEARER_Y);
    layout.cycle_slot  = make_slot(CYCLE_Y);

    /* Card frame */
    layout.card_x = CARD_X;
    layout.card_y = CARD_Y;
    layout.card_w = CARD_W;
    layout.card_h = CARD_H;

    return layout;
}

const char *daily_aztec_sign_name(const daily_aztec_layout_t *layout)
{
    if (!layout) {
        return "?";
    }
    return layout->sign_name;
}
