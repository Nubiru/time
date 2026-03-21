/* daily_iching_layout.c — Daily I Ching card layout.
 *
 * Converts JD to hexagram, looks up interpretation data, and computes
 * card slot positions for rendering.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "daily_iching_layout.h"
#include "../systems/iching/iching.h"
#include "../systems/iching/iching_interpret.h"
#include <string.h>

/* Slot layout constants */
#define SLOT_X       0.10f
#define SLOT_W       0.80f
#define SLOT_H       0.08f

#define TITLE_Y      0.03f
#define NUMBER_Y     0.12f
#define NAME_Y       0.22f
#define LINES_Y      0.30f
#define LINES_H      0.30f
#define UPPER_Y      0.62f
#define LOWER_Y      0.70f
#define JUDGMENT_Y   0.78f
#define IMAGE_Y      0.88f

/* Card frame */
#define CARD_X       0.05f
#define CARD_Y       0.02f
#define CARD_W       0.90f
#define CARD_H       0.96f

static ic_slot_t make_slot(float y, float h)
{
    ic_slot_t slot;
    slot.x = SLOT_X;
    slot.y = y;
    slot.w = SLOT_W;
    slot.h = h;
    return slot;
}

daily_iching_layout_t daily_iching_compute(double jd)
{
    daily_iching_layout_t layout;
    memset(&layout, 0, sizeof(layout));

    /* Get hexagram from JD. */
    hexagram_t hex = iching_from_jd(jd);
    ii_hexagram_t data = ii_hexagram_data(hex.king_wen);

    /* Identity. */
    layout.king_wen = hex.king_wen;
    layout.name     = data.name;
    layout.judgment  = data.judgment;
    layout.image     = data.image;
    layout.keywords  = data.keywords;

    /* Lines. */
    for (int i = 0; i < 6; i++) {
        layout.lines[i] = hex.lines[i];
    }

    /* Trigrams. */
    layout.upper_trigram = hex.upper_trigram;
    layout.lower_trigram = hex.lower_trigram;
    layout.upper_name   = iching_trigram_name(hex.upper_trigram);
    layout.lower_name   = iching_trigram_name(hex.lower_trigram);
    layout.upper_symbol = iching_trigram_symbol(hex.upper_trigram);
    layout.lower_symbol = iching_trigram_symbol(hex.lower_trigram);

    /* Layout slots. */
    layout.title_slot    = make_slot(TITLE_Y,    SLOT_H);
    layout.number_slot   = make_slot(NUMBER_Y,   SLOT_H);
    layout.name_slot     = make_slot(NAME_Y,     SLOT_H);
    layout.lines_slot    = make_slot(LINES_Y,    LINES_H);
    layout.upper_slot    = make_slot(UPPER_Y,    SLOT_H);
    layout.lower_slot    = make_slot(LOWER_Y,    SLOT_H);
    layout.judgment_slot = make_slot(JUDGMENT_Y,  SLOT_H);
    layout.image_slot    = make_slot(IMAGE_Y,    SLOT_H);

    /* Card frame. */
    layout.card_x = CARD_X;
    layout.card_y = CARD_Y;
    layout.card_w = CARD_W;
    layout.card_h = CARD_H;

    return layout;
}

const char *daily_iching_hexagram_name(const daily_iching_layout_t *layout)
{
    if (!layout || layout->king_wen == 0) {
        return "?";
    }
    return layout->name;
}
