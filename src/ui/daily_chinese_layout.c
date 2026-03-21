/* daily_chinese_layout.c — Daily Chinese calendar card layout
 *
 * Converts JD to Chinese sexagenary year, looks up names, and
 * computes card slot positions for rendering.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "daily_chinese_layout.h"
#include "../systems/chinese/chinese.h"
#include <string.h>

/* Slot layout constants */
#define SLOT_X       0.10f
#define SLOT_W       0.80f
#define SLOT_H       0.10f

#define TITLE_Y      0.05f
#define ANIMAL_Y     0.18f
#define ELEMENT_Y    0.32f
#define CYCLE_Y      0.46f
#define STEM_Y       0.60f
#define BRANCH_Y     0.74f

/* Card frame */
#define CARD_X       0.05f
#define CARD_Y       0.02f
#define CARD_W       0.90f
#define CARD_H       0.96f

daily_chinese_layout_t daily_chinese_compute(double jd)
{
    daily_chinese_layout_t layout;
    memset(&layout, 0, sizeof(layout));

    /* Compute Chinese year from JD */
    chinese_year_t cy = chinese_year_from_jd(jd);
    layout.stem       = cy.stem;
    layout.branch     = cy.branch;
    layout.animal     = cy.animal;
    layout.element    = cy.element;
    layout.polarity   = cy.polarity;
    layout.cycle_year = cy.cycle_year;

    /* Look up name strings */
    layout.animal_name   = chinese_animal_name(cy.animal);
    layout.element_name  = chinese_element_name(cy.element);
    layout.polarity_name = chinese_polarity_name(cy.polarity);
    layout.stem_name     = chinese_stem_name(cy.stem);
    layout.branch_name   = chinese_branch_name(cy.branch);
    layout.animal_symbol = chinese_animal_symbol(cy.animal);

    /* Title slot */
    layout.title_slot.x = SLOT_X;
    layout.title_slot.y = TITLE_Y;
    layout.title_slot.w = SLOT_W;
    layout.title_slot.h = SLOT_H;

    /* Animal slot */
    layout.animal_slot.x = SLOT_X;
    layout.animal_slot.y = ANIMAL_Y;
    layout.animal_slot.w = SLOT_W;
    layout.animal_slot.h = SLOT_H;

    /* Element slot */
    layout.element_slot.x = SLOT_X;
    layout.element_slot.y = ELEMENT_Y;
    layout.element_slot.w = SLOT_W;
    layout.element_slot.h = SLOT_H;

    /* Cycle slot */
    layout.cycle_slot.x = SLOT_X;
    layout.cycle_slot.y = CYCLE_Y;
    layout.cycle_slot.w = SLOT_W;
    layout.cycle_slot.h = SLOT_H;

    /* Stem slot */
    layout.stem_slot.x = SLOT_X;
    layout.stem_slot.y = STEM_Y;
    layout.stem_slot.w = SLOT_W;
    layout.stem_slot.h = SLOT_H;

    /* Branch slot */
    layout.branch_slot.x = SLOT_X;
    layout.branch_slot.y = BRANCH_Y;
    layout.branch_slot.w = SLOT_W;
    layout.branch_slot.h = SLOT_H;

    /* Card frame */
    layout.card_x = CARD_X;
    layout.card_y = CARD_Y;
    layout.card_w = CARD_W;
    layout.card_h = CARD_H;

    return layout;
}

const char *daily_chinese_animal_name(const daily_chinese_layout_t *layout)
{
    if (!layout) {
        return "?";
    }
    return layout->animal_name;
}
