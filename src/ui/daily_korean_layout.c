/* daily_korean_layout.c — Daily Korean calendar card layout
 *
 * Converts JD to Korean date, looks up zodiac animal, element,
 * and active festival, computes card slot positions for rendering.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "daily_korean_layout.h"
#include "../systems/korean/korean_calendar.h"
#include "../systems/korean/korean_interpret.h"
#include <string.h>

/* Slot layout constants */
#define SLOT_X       0.10f
#define SLOT_W       0.80f
#define SLOT_H       0.10f

#define TITLE_Y      0.05f
#define DATE_Y       0.18f
#define ANIMAL_Y     0.34f
#define CYCLE_Y      0.50f
#define FESTIVAL_Y   0.66f

/* Card frame */
#define CARD_X       0.05f
#define CARD_Y       0.02f
#define CARD_W       0.90f
#define CARD_H       0.96f

static korean_slot_t make_slot(float y)
{
    korean_slot_t s;
    s.x = SLOT_X;
    s.y = y;
    s.w = SLOT_W;
    s.h = SLOT_H;
    return s;
}

daily_korean_layout_t daily_korean_compute(double jd)
{
    daily_korean_layout_t layout;
    memset(&layout, 0, sizeof(layout));

    /* Convert JD to Korean date */
    layout.date = korean_from_jd(jd);

    /* Zodiac interpretation */
    layout.animal_name = korean_animal_name(layout.date.animal);
    layout.element_name = korean_element_name(layout.date.element);
    layout.polarity_name = korean_polarity_name(layout.date.polarity);
    layout.sexagenary = korean_sexagenary_name(layout.date.cycle_year);

    /* Check for festival */
    const korean_festival_t *fest = korean_festival(layout.date.lunar_month,
                                                    layout.date.lunar_day);
    if (fest) {
        layout.has_festival = 1;
        layout.festival_name = fest->name;
        layout.festival_korean = fest->korean;
    } else {
        layout.has_festival = 0;
        layout.festival_name = "";
        layout.festival_korean = "";
    }

    /* Layout slots */
    layout.title_slot    = make_slot(TITLE_Y);
    layout.date_slot     = make_slot(DATE_Y);
    layout.animal_slot   = make_slot(ANIMAL_Y);
    layout.cycle_slot    = make_slot(CYCLE_Y);
    layout.festival_slot = make_slot(FESTIVAL_Y);

    /* Card frame */
    layout.card_x = CARD_X;
    layout.card_y = CARD_Y;
    layout.card_w = CARD_W;
    layout.card_h = CARD_H;

    return layout;
}

const char *daily_korean_animal(const daily_korean_layout_t *layout)
{
    if (!layout) {
        return "?";
    }
    return layout->animal_name;
}

int daily_korean_has_festival(const daily_korean_layout_t *layout)
{
    if (!layout) {
        return 0;
    }
    return layout->has_festival;
}
