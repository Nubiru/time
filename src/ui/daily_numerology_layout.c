/* daily_numerology_layout.c — Daily numerology card layout
 *
 * Converts JD to Gregorian date, computes universal day number
 * by reducing date digits, looks up meaning and interpretation,
 * then fills card slots.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "daily_numerology_layout.h"
#include "../systems/numerology/numerology.h"
#include "../systems/numerology/numerology_interpret.h"
#include "../math/julian.h"
#include <string.h>
#include <stdio.h>

/* Slot layout constants */
#define SLOT_X       0.10f
#define SLOT_W       0.80f
#define SLOT_H       0.10f

#define TITLE_Y      0.05f
#define NUMBER_Y     0.19f
#define MEANING_Y    0.33f
#define UNIVERSAL_Y  0.47f
#define YEAR_ENERGY_Y 0.61f
#define DETAIL_Y     0.75f

/* Card frame */
#define CARD_X       0.05f
#define CARD_Y       0.02f
#define CARD_W       0.90f
#define CARD_H       0.96f

static num_slot_t make_slot(float y)
{
    num_slot_t s;
    s.x = SLOT_X;
    s.y = y;
    s.w = SLOT_W;
    s.h = SLOT_H;
    return s;
}

daily_numerology_layout_t daily_numerology_compute(double jd)
{
    daily_numerology_layout_t layout;
    memset(&layout, 0, sizeof(layout));

    /* Convert JD to Gregorian date */
    int year, month;
    double day_frac = jd_to_gregorian(jd, &year, &month);
    int day = (int)day_frac;

    layout.year = year;
    layout.month = month;
    layout.day = day;

    /* Universal day: sum all digits of year + month + day, then reduce */
    int sum = 0;
    int y = year > 0 ? year : -year;
    while (y > 0) { sum += y % 10; y /= 10; }
    sum += month / 10 + month % 10;
    sum += day / 10 + day % 10;

    num_root_t daily = num_reduce(sum);
    layout.daily_number = daily.root;
    layout.is_master = daily.is_master;

    /* Universal year */
    num_root_t uy = num_universal_year(year);
    layout.universal_year = uy.root;

    /* Meaning keyword */
    layout.number_meaning = num_meaning(daily.root);

    /* Interpretation */
    numerology_interp_t interp = ni_interpret(daily.root, uy.root);
    snprintf(layout.glance, sizeof(layout.glance), "%s", interp.glance);
    snprintf(layout.detail, sizeof(layout.detail), "%.511s", interp.detail);

    /* Layout slots */
    layout.title_slot       = make_slot(TITLE_Y);
    layout.number_slot      = make_slot(NUMBER_Y);
    layout.meaning_slot     = make_slot(MEANING_Y);
    layout.universal_slot   = make_slot(UNIVERSAL_Y);
    layout.year_energy_slot = make_slot(YEAR_ENERGY_Y);
    layout.detail_slot      = make_slot(DETAIL_Y);

    /* Card frame */
    layout.card_x = CARD_X;
    layout.card_y = CARD_Y;
    layout.card_w = CARD_W;
    layout.card_h = CARD_H;

    return layout;
}
