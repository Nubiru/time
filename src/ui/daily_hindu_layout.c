/* daily_hindu_layout.c — Daily Hindu Panchanga card layout
 *
 * Computes Panchanga from JD + Sun/Moon longitudes, extracts
 * names for all 5 limbs, and lays out card slot positions.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "daily_hindu_layout.h"
#include "../systems/hindu/panchanga.h"
#include "../systems/hindu/nakshatra.h"
#include <string.h>

/* Slot layout constants */
#define SLOT_X       0.10f
#define SLOT_W       0.80f
#define SLOT_H       0.10f

#define TITLE_Y      0.05f
#define TITHI_Y      0.18f
#define VARA_Y       0.32f
#define NAKSHATRA_Y  0.46f
#define YOGA_Y       0.60f
#define KARANA_Y     0.74f

/* Card frame */
#define CARD_X       0.05f
#define CARD_Y       0.02f
#define CARD_W       0.90f
#define CARD_H       0.96f

daily_hindu_layout_t daily_hindu_compute(double jd, double sun_lon,
                                          double moon_lon)
{
    daily_hindu_layout_t layout;
    memset(&layout, 0, sizeof(layout));

    /* Compute full Panchanga */
    panchanga_t p = panchanga_compute(jd, sun_lon, moon_lon);

    /* Tithi */
    layout.tithi_number = p.tithi.number;
    layout.tithi_name   = p.tithi.name;
    layout.paksha       = p.tithi.paksha;

    /* Vara */
    layout.vara_number = p.vara.number;
    layout.vara_name   = p.vara.name;
    layout.vara_deity  = p.vara.deity;

    /* Nakshatra */
    layout.nakshatra_index = p.nakshatra_index;

    /* Yoga */
    layout.yoga_number  = p.yoga.number;
    layout.yoga_name    = p.yoga.name;
    layout.yoga_quality = p.yoga.quality;

    /* Karana */
    layout.karana_number = p.karana.number;
    layout.karana_name   = p.karana.name;

    /* Title slot */
    layout.title_slot.x = SLOT_X;
    layout.title_slot.y = TITLE_Y;
    layout.title_slot.w = SLOT_W;
    layout.title_slot.h = SLOT_H;

    /* Tithi slot */
    layout.tithi_slot.x = SLOT_X;
    layout.tithi_slot.y = TITHI_Y;
    layout.tithi_slot.w = SLOT_W;
    layout.tithi_slot.h = SLOT_H;

    /* Vara slot */
    layout.vara_slot.x = SLOT_X;
    layout.vara_slot.y = VARA_Y;
    layout.vara_slot.w = SLOT_W;
    layout.vara_slot.h = SLOT_H;

    /* Nakshatra slot */
    layout.nakshatra_slot.x = SLOT_X;
    layout.nakshatra_slot.y = NAKSHATRA_Y;
    layout.nakshatra_slot.w = SLOT_W;
    layout.nakshatra_slot.h = SLOT_H;

    /* Yoga slot */
    layout.yoga_slot.x = SLOT_X;
    layout.yoga_slot.y = YOGA_Y;
    layout.yoga_slot.w = SLOT_W;
    layout.yoga_slot.h = SLOT_H;

    /* Karana slot */
    layout.karana_slot.x = SLOT_X;
    layout.karana_slot.y = KARANA_Y;
    layout.karana_slot.w = SLOT_W;
    layout.karana_slot.h = SLOT_H;

    /* Card frame */
    layout.card_x = CARD_X;
    layout.card_y = CARD_Y;
    layout.card_w = CARD_W;
    layout.card_h = CARD_H;

    return layout;
}

const char *daily_hindu_tithi_name(const daily_hindu_layout_t *layout)
{
    if (!layout) {
        return "?";
    }
    return layout->tithi_name;
}
