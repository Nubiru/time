/* daily_chakra_layout.c — Daily chakra card layout
 *
 * Maps day of week to chakra (Mon=Root..Sun=Crown), looks up
 * chakra data and interpretation, then fills card slots.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "daily_chakra_layout.h"
#include "../systems/chakra/chakra_interpret.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

/* Slot layout constants */
#define SLOT_X       0.10f
#define SLOT_W       0.80f
#define SLOT_H       0.10f

#define TITLE_Y      0.05f
#define CHAKRA_NAME_Y 0.19f
#define THEME_Y      0.33f
#define QUESTION_Y   0.47f
#define ENERGY_Y     0.61f
#define MEDITATION_Y 0.75f

/* Card frame */
#define CARD_X       0.05f
#define CARD_Y       0.02f
#define CARD_W       0.90f
#define CARD_H       0.96f

static chakra_slot_t make_slot(float y)
{
    chakra_slot_t s;
    s.x = SLOT_X;
    s.y = y;
    s.w = SLOT_W;
    s.h = SLOT_H;
    return s;
}

daily_chakra_layout_t daily_chakra_compute(double jd)
{
    daily_chakra_layout_t layout;
    memset(&layout, 0, sizeof(layout));

    /* Day of week from JD: JD 0 = Monday.
     * (jd+0.5) day number % 7: 0=Mon..6=Sun */
    long day_num = (long)floor(jd + 0.5);
    int dow = (int)(day_num % 7);
    if (dow < 0) dow += 7;

    /* Map: Mon(0)=Root(0), Tue(1)=Sacral(1), Wed(2)=Solar Plexus(2),
     * Thu(3)=Heart(3), Fri(4)=Throat(4), Sat(5)=Third Eye(5),
     * Sun(6)=Crown(6) */
    int chakra = dow;
    layout.chakra_index = chakra;
    layout.day_of_week = dow;

    /* Chakra data */
    chi_chakra_t cd = chi_chakra_data(chakra);
    layout.sanskrit_name = cd.sanskrit;
    layout.english_name = cd.english;
    layout.theme = cd.theme;
    layout.question = cd.question;

    /* Interpretation */
    chakra_interp_t interp = chi_interpret(chakra);
    snprintf(layout.glance, sizeof(layout.glance), "%s", interp.glance);
    snprintf(layout.detail, sizeof(layout.detail), "%.511s", interp.detail);

    /* Layout slots */
    layout.title_slot       = make_slot(TITLE_Y);
    layout.chakra_name_slot = make_slot(CHAKRA_NAME_Y);
    layout.theme_slot       = make_slot(THEME_Y);
    layout.question_slot    = make_slot(QUESTION_Y);
    layout.energy_slot      = make_slot(ENERGY_Y);
    layout.meditation_slot  = make_slot(MEDITATION_Y);

    /* Card frame */
    layout.card_x = CARD_X;
    layout.card_y = CARD_Y;
    layout.card_w = CARD_W;
    layout.card_h = CARD_H;

    return layout;
}
