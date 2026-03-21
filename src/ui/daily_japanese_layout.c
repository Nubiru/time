/* daily_japanese_layout.c — Daily Japanese calendar card layout
 *
 * Converts JD to Japanese date with imperial era, looks up rokuyo,
 * sekki, and zodiac animal, computes card slot positions for rendering.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "daily_japanese_layout.h"
#include "../systems/japanese/japanese.h"
#include "../systems/japanese/japanese_interpret.h"
#include <string.h>

/* Slot layout constants */
#define SLOT_X       0.10f
#define SLOT_W       0.80f
#define SLOT_H       0.10f

#define TITLE_Y      0.05f
#define DATE_Y       0.18f
#define ROKUYO_Y     0.34f
#define SEKKI_Y      0.50f
#define ANIMAL_Y     0.66f

/* Card frame */
#define CARD_X       0.05f
#define CARD_Y       0.02f
#define CARD_W       0.90f
#define CARD_H       0.96f

static japanese_slot_t make_slot(float y)
{
    japanese_slot_t s;
    s.x = SLOT_X;
    s.y = y;
    s.w = SLOT_W;
    s.h = SLOT_H;
    return s;
}

daily_japanese_layout_t daily_japanese_compute(double jd,
                                               double solar_longitude_deg)
{
    daily_japanese_layout_t layout;
    memset(&layout, 0, sizeof(layout));

    /* Convert JD to Japanese date */
    layout.date = japanese_from_jd(jd);
    layout.era_name = japanese_era_name(layout.date.era_index);
    layout.era_kanji = japanese_era_kanji(layout.date.era_index);

    /* Rokuyo */
    layout.rokuyo = (int)japanese_rokuyo(jd);
    layout.rokuyo_name = japanese_rokuyo_name((jp_rokuyo_t)layout.rokuyo);
    layout.rokuyo_meaning = japanese_rokuyo_meaning((jp_rokuyo_t)layout.rokuyo);

    /* Sekki */
    layout.sekki = japanese_sekki(solar_longitude_deg);
    layout.sekki_name = japanese_sekki_name(layout.sekki);
    layout.sekki_kanji = japanese_sekki_kanji(layout.sekki);

    /* Zodiac animal */
    layout.animal = japanese_zodiac_animal(layout.date.greg_year);
    layout.animal_name = japanese_zodiac_name(layout.animal);
    layout.animal_kanji = japanese_zodiac_kanji(layout.animal);

    /* Layout slots */
    layout.title_slot  = make_slot(TITLE_Y);
    layout.date_slot   = make_slot(DATE_Y);
    layout.rokuyo_slot = make_slot(ROKUYO_Y);
    layout.sekki_slot  = make_slot(SEKKI_Y);
    layout.animal_slot = make_slot(ANIMAL_Y);

    /* Card frame */
    layout.card_x = CARD_X;
    layout.card_y = CARD_Y;
    layout.card_w = CARD_W;
    layout.card_h = CARD_H;

    return layout;
}

const char *daily_japanese_rokuyo(const daily_japanese_layout_t *layout)
{
    if (!layout) {
        return "?";
    }
    return layout->rokuyo_meaning;
}

const char *daily_japanese_sekki(const daily_japanese_layout_t *layout)
{
    if (!layout) {
        return "?";
    }
    return layout->sekki_name;
}
