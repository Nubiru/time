/* daily_geology_layout.c — Daily geological time card layout
 *
 * Looks up current eon interpretation, rotates through earth_fraction
 * facts by day, and computes card slot positions for rendering.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "daily_geology_layout.h"
#include "../systems/geology/geology_interpret.h"
#include "../systems/earth/earth_fraction.h"
#include <string.h>
#include <math.h>

/* Slot layout constants */
#define SLOT_X       0.10f
#define SLOT_W       0.80f
#define SLOT_H       0.10f

#define TITLE_Y      0.05f
#define EON_Y        0.18f
#define MEANING_Y    0.32f
#define FACT_Y       0.46f
#define ANALOGY_Y    0.60f
#define FRACTION_Y   0.74f

/* Card frame */
#define CARD_X       0.05f
#define CARD_Y       0.02f
#define CARD_W       0.90f
#define CARD_H       0.96f

/* J2000.0 epoch for day counting */
#define J2000_JD     2451545.0

daily_geology_layout_t daily_geology_compute(double jd)
{
    daily_geology_layout_t layout;
    memset(&layout, 0, sizeof(layout));

    /* Current eon: Phanerozoic (index 3) for present day */
    gli_eon_t eon = gli_eon_data(3);
    layout.eon_name = eon.name;
    layout.eon_meaning = eon.meaning;
    layout.eon_brief = eon.brief;

    /* Rotate through earth_fraction facts based on day number */
    int fact_count = ef_fact_count();
    int day_number = (int)(jd - J2000_JD);
    /* Ensure positive modulo */
    int fact_index = day_number % fact_count;
    if (fact_index < 0) {
        fact_index += fact_count;
    }

    earth_fraction_t fact = ef_fact_get(fact_index);
    layout.fact_name = fact.fact_name;
    layout.fact_description = fact.description;
    layout.fact_analogy = fact.visual_analogy;

    /* Human fraction of Earth's age */
    layout.human_fraction_pct = ef_human_fraction();

    /* Title slot */
    layout.title_slot.x = SLOT_X;
    layout.title_slot.y = TITLE_Y;
    layout.title_slot.w = SLOT_W;
    layout.title_slot.h = SLOT_H;

    /* Eon slot */
    layout.eon_slot.x = SLOT_X;
    layout.eon_slot.y = EON_Y;
    layout.eon_slot.w = SLOT_W;
    layout.eon_slot.h = SLOT_H;

    /* Meaning slot */
    layout.meaning_slot.x = SLOT_X;
    layout.meaning_slot.y = MEANING_Y;
    layout.meaning_slot.w = SLOT_W;
    layout.meaning_slot.h = SLOT_H;

    /* Fact slot */
    layout.fact_slot.x = SLOT_X;
    layout.fact_slot.y = FACT_Y;
    layout.fact_slot.w = SLOT_W;
    layout.fact_slot.h = SLOT_H;

    /* Analogy slot */
    layout.analogy_slot.x = SLOT_X;
    layout.analogy_slot.y = ANALOGY_Y;
    layout.analogy_slot.w = SLOT_W;
    layout.analogy_slot.h = SLOT_H;

    /* Fraction slot */
    layout.fraction_slot.x = SLOT_X;
    layout.fraction_slot.y = FRACTION_Y;
    layout.fraction_slot.w = SLOT_W;
    layout.fraction_slot.h = SLOT_H;

    /* Card frame */
    layout.card_x = CARD_X;
    layout.card_y = CARD_Y;
    layout.card_w = CARD_W;
    layout.card_h = CARD_H;

    return layout;
}

const char *daily_geology_eon_name(const daily_geology_layout_t *layout)
{
    if (!layout) {
        return "?";
    }
    return layout->eon_name;
}
