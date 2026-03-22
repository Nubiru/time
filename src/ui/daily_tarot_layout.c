/* daily_tarot_layout.c — Daily Tarot card layout
 *
 * Deterministically selects a Major Arcanum from the Julian Day,
 * looks up card data and interpretation, then fills card slots.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "daily_tarot_layout.h"
#include "../systems/tarot/tarot.h"
#include "../systems/tarot/tarot_interpret.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

/* Slot layout constants */
#define SLOT_X       0.10f
#define SLOT_W       0.80f
#define SLOT_H       0.10f

#define TITLE_Y      0.05f
#define CARD_NAME_Y  0.19f
#define ARCANA_Y     0.33f
#define HEBREW_Y     0.47f
#define PATH_Y       0.61f
#define MEANING_Y    0.75f

/* Card frame */
#define CARD_X       0.05f
#define CARD_Y       0.02f
#define CARD_W       0.90f
#define CARD_H       0.96f

static tarot_slot_t make_slot(float y)
{
    tarot_slot_t s;
    s.x = SLOT_X;
    s.y = y;
    s.w = SLOT_W;
    s.h = SLOT_H;
    return s;
}

daily_tarot_layout_t daily_tarot_compute(double jd)
{
    daily_tarot_layout_t layout;
    memset(&layout, 0, sizeof(layout));

    /* Deterministic daily card from JD */
    unsigned int day_num = (unsigned int)floor(jd + 0.5);
    unsigned int hash = day_num * 2654435761u;
    int major = (int)(hash % 22);

    /* Card data */
    tarot_major_t card = tarot_major_get(major);
    layout.card_number = card.number;
    layout.card_name = card.name;
    layout.hebrew_letter = card.hebrew_letter;
    layout.tree_path = card.tree_path;

    /* Interpretation: major arcanum only, no decan */
    tarot_interp_t interp = trt_interpret(major, -1, -1);
    snprintf(layout.glance, sizeof(layout.glance), "%s", interp.glance);
    snprintf(layout.detail, sizeof(layout.detail), "%.1023s", interp.detail);

    /* Layout slots */
    layout.title_slot   = make_slot(TITLE_Y);
    layout.card_slot    = make_slot(CARD_NAME_Y);
    layout.arcana_slot  = make_slot(ARCANA_Y);
    layout.hebrew_slot  = make_slot(HEBREW_Y);
    layout.path_slot    = make_slot(PATH_Y);
    layout.meaning_slot = make_slot(MEANING_Y);

    /* Card frame */
    layout.card_x = CARD_X;
    layout.card_y = CARD_Y;
    layout.card_w = CARD_W;
    layout.card_h = CARD_H;

    return layout;
}
