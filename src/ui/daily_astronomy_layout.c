/* daily_astronomy_layout.c — Daily astronomy card layout
 *
 * Computes lunar phase, illumination, zodiac sign, and
 * interpretation from Julian Day, then fills card slots.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "daily_astronomy_layout.h"
#include "../systems/astronomy/lunar.h"
#include "../systems/astronomy/astronomy_interpret.h"
#include "../math/julian.h"
#include <string.h>
#include <stdio.h>

/* Slot layout constants */
#define SLOT_X       0.10f
#define SLOT_W       0.80f
#define SLOT_H       0.10f

#define TITLE_Y      0.05f
#define PHASE_Y      0.19f
#define ILLUM_Y      0.33f
#define AGE_Y        0.47f
#define ZODIAC_Y     0.61f
#define DETAIL_Y     0.75f

/* Card frame */
#define CARD_X       0.05f
#define CARD_Y       0.02f
#define CARD_W       0.90f
#define CARD_H       0.96f

static astro_slot_t make_slot(float y)
{
    astro_slot_t s;
    s.x = SLOT_X;
    s.y = y;
    s.w = SLOT_W;
    s.h = SLOT_H;
    return s;
}

daily_astronomy_layout_t daily_astronomy_compute(double jd)
{
    daily_astronomy_layout_t layout;
    memset(&layout, 0, sizeof(layout));

    /* Lunar data */
    lunar_info_t moon = lunar_phase(jd);
    layout.moon_phase = (int)moon.phase;
    layout.moon_phase_name = lunar_phase_name(moon.phase);
    layout.illumination = moon.illumination;
    layout.moon_age_days = moon.age_days;
    layout.moon_longitude = moon.moon_longitude;
    layout.zodiac_sign = lunar_zodiac_sign(jd);

    /* Interpretation: Mercury(0), not retrograde, current moon phase */
    astronomy_interp_t interp = ai_astro_interpret(0, 0, (int)moon.phase);
    snprintf(layout.glance, sizeof(layout.glance), "%s", interp.glance);
    snprintf(layout.detail, sizeof(layout.detail), "%.511s", interp.detail);

    /* Layout slots */
    layout.title_slot        = make_slot(TITLE_Y);
    layout.phase_slot        = make_slot(PHASE_Y);
    layout.illumination_slot = make_slot(ILLUM_Y);
    layout.age_slot          = make_slot(AGE_Y);
    layout.zodiac_slot       = make_slot(ZODIAC_Y);
    layout.detail_slot       = make_slot(DETAIL_Y);

    /* Card frame */
    layout.card_x = CARD_X;
    layout.card_y = CARD_Y;
    layout.card_w = CARD_W;
    layout.card_h = CARD_H;

    return layout;
}
