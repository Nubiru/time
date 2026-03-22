/* daily_buddhist_layout.c — Daily Buddhist calendar card layout
 *
 * Converts JD to Buddhist Era, computes Uposatha, lunar month,
 * and Vassa status, then lays out card slot positions.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "daily_buddhist_layout.h"
#include "../systems/buddhist/buddhist.h"
#include "../systems/buddhist/buddhist_interpret.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

/* Slot layout constants */
#define SLOT_X       0.10f
#define SLOT_W       0.80f
#define SLOT_H       0.10f

#define TITLE_Y      0.05f
#define ERA_Y        0.18f
#define MONTH_Y      0.32f
#define UPOSATHA_Y   0.46f
#define MOON_Y       0.60f
#define VASSA_Y      0.74f

/* Card frame */
#define CARD_X       0.05f
#define CARD_Y       0.02f
#define CARD_W       0.90f
#define CARD_H       0.96f

daily_buddhist_layout_t daily_buddhist_compute(double jd)
{
    daily_buddhist_layout_t layout;
    memset(&layout, 0, sizeof(layout));

    /* Extract approximate CE year from JD */
    int ce_year = (int)((jd - 2451545.0) / 365.25 + 2000.0);

    /* Buddhist Era = CE + 543 */
    layout.be_year = buddhist_era(ce_year);

    /* Uposatha observance */
    uposatha_t upo = buddhist_uposatha(jd);
    layout.uposatha_type = (int)upo.type;
    layout.uposatha_name = upo.name;
    layout.uposatha_desc = upo.description;
    layout.moon_age_days = upo.moon_age_days;
    layout.illumination  = upo.illumination;

    /* Lunar month */
    buddhist_month_t bm = buddhist_month(jd);
    layout.month_number = bm.month;
    layout.month_name   = bm.name;

    /* Vassa */
    layout.is_vassa = buddhist_is_vassa(jd) ? 1 : 0;

    /* Interpretation */
    buddhist_interp_t interp = bi_interpret(layout.be_year,
                                            layout.month_number,
                                            (uposatha_type_t)layout.uposatha_type,
                                            KALPA_PHASE_EXISTENCE);
    snprintf(layout.glance, sizeof(layout.glance), "%s", interp.glance);
    snprintf(layout.detail, sizeof(layout.detail), "%.511s", interp.detail);

    /* Title slot */
    layout.title_slot.x = SLOT_X;
    layout.title_slot.y = TITLE_Y;
    layout.title_slot.w = SLOT_W;
    layout.title_slot.h = SLOT_H;

    /* Era slot */
    layout.era_slot.x = SLOT_X;
    layout.era_slot.y = ERA_Y;
    layout.era_slot.w = SLOT_W;
    layout.era_slot.h = SLOT_H;

    /* Month slot */
    layout.month_slot.x = SLOT_X;
    layout.month_slot.y = MONTH_Y;
    layout.month_slot.w = SLOT_W;
    layout.month_slot.h = SLOT_H;

    /* Uposatha slot */
    layout.uposatha_slot.x = SLOT_X;
    layout.uposatha_slot.y = UPOSATHA_Y;
    layout.uposatha_slot.w = SLOT_W;
    layout.uposatha_slot.h = SLOT_H;

    /* Moon slot */
    layout.moon_slot.x = SLOT_X;
    layout.moon_slot.y = MOON_Y;
    layout.moon_slot.w = SLOT_W;
    layout.moon_slot.h = SLOT_H;

    /* Vassa slot */
    layout.vassa_slot.x = SLOT_X;
    layout.vassa_slot.y = VASSA_Y;
    layout.vassa_slot.w = SLOT_W;
    layout.vassa_slot.h = SLOT_H;

    /* Card frame */
    layout.card_x = CARD_X;
    layout.card_y = CARD_Y;
    layout.card_w = CARD_W;
    layout.card_h = CARD_H;

    return layout;
}

int daily_buddhist_is_observance(const daily_buddhist_layout_t *layout)
{
    if (!layout) {
        return 0;
    }
    return (layout->uposatha_type != UPOSATHA_NONE) ? 1 : 0;
}
