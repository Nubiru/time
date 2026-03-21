#include "daily_transit_layout.h"
#include "../systems/astrology/zodiac.h"
#include "../systems/astrology/aspects.h"
#include "../systems/astrology/dignity.h"
#include <math.h>
#include <string.h>
#include <stdio.h>

/* Card frame. */
#define CARD_X  0.05f
#define CARD_Y  0.05f
#define CARD_W  0.90f
#define CARD_H  0.90f

/* Slot dimensions. */
#define SLOT_W  0.80f
#define SLOT_H  0.06f
#define SLOT_X  0.10f

/* Vertical positions. */
#define SUN_SLOT_Y     0.08f
#define MOON_SLOT_Y    0.18f
#define PLANET_START_Y 0.30f
#define PLANET_STEP_Y  0.07f
#define ASPECT_START_Y 0.68f
#define ASPECT_STEP_Y  0.06f

/* Header slot dimensions (slightly larger). */
#define HEADER_SLOT_H  0.08f

static transit_planet_info_t make_planet_info(int index, double lon)
{
    transit_planet_info_t info;
    memset(&info, 0, sizeof(info));
    info.planet_index = index;
    info.sign = zodiac_sign(lon);
    info.degree = (float)zodiac_degree(lon);
    info.sign_name = zodiac_sign_name(info.sign);
    info.sign_symbol = zodiac_sign_symbol(info.sign);
    info.is_retrograde = 0;

    dignity_result_t dr = planet_dignity(index, info.sign);
    info.dignity = dr.score;

    return info;
}

static transit_slot_t make_slot(float x, float y, float w, float h)
{
    transit_slot_t s;
    s.x = x;
    s.y = y;
    s.w = w;
    s.h = h;
    return s;
}

daily_transit_layout_t daily_transit_compute(const double lons[10],
                                             double max_orb)
{
    daily_transit_layout_t layout;
    memset(&layout, 0, sizeof(layout));

    layout.card_x = CARD_X;
    layout.card_y = CARD_Y;
    layout.card_w = CARD_W;
    layout.card_h = CARD_H;

    if (!lons) {
        return layout;
    }

    /* Sun (index 8) and Moon (index 9) header slots. */
    layout.sun_slot  = make_slot(SLOT_X, SUN_SLOT_Y,  SLOT_W, HEADER_SLOT_H);
    layout.moon_slot = make_slot(SLOT_X, MOON_SLOT_Y, SLOT_W, HEADER_SLOT_H);
    layout.sun  = make_planet_info(DIGNITY_SUN,  lons[8]);
    layout.moon = make_planet_info(DIGNITY_MOON, lons[9]);

    /* All 10 planet slots stacked vertically. */
    layout.planet_count = TRANSIT_MAX_PLANETS;
    for (int i = 0; i < TRANSIT_MAX_PLANETS; i++) {
        float y = PLANET_START_Y + (float)i * PLANET_STEP_Y;
        layout.planet_slots[i] = make_slot(SLOT_X, y, SLOT_W, SLOT_H);
        layout.planets[i] = make_planet_info(i, lons[i]);
    }

    /* Aspects among first 8 planets (0-7). */
    aspect_list_t alist = aspects_find_all(lons, max_orb);
    layout.aspect_count = alist.count;
    for (int i = 0; i < alist.count; i++) {
        float y = ASPECT_START_Y + (float)i * ASPECT_STEP_Y;
        layout.aspect_slots[i] = make_slot(SLOT_X, y, SLOT_W, SLOT_H);

        layout.aspects[i].planet_a = alist.aspects[i].planet_a;
        layout.aspects[i].planet_b = alist.aspects[i].planet_b;
        layout.aspects[i].type = (int)alist.aspects[i].type;
        layout.aspects[i].orb = (float)alist.aspects[i].orb;
        layout.aspects[i].aspect_name = aspect_name(alist.aspects[i].type);
        layout.aspects[i].aspect_symbol = aspect_symbol(alist.aspects[i].type);
    }

    return layout;
}

const char *daily_transit_summary(const daily_transit_layout_t *layout,
                                  char *buf, int buf_size)
{
    if (!layout || !buf || buf_size < 1) return NULL;

    snprintf(buf, (size_t)buf_size, "Sun in %s, Moon in %s",
             layout->sun.sign_name ? layout->sun.sign_name : "?",
             layout->moon.sign_name ? layout->moon.sign_name : "?");
    return buf;
}

int daily_transit_aspect_count(const daily_transit_layout_t *layout)
{
    if (!layout) return 0;
    return layout->aspect_count;
}
