/* daily_hd_layout.c — Daily Human Design transit card layout.
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "daily_hd_layout.h"
#include "../systems/human_design/human_design.h"
#include <string.h>

/* Card frame. */
#define CARD_X 0.05f
#define CARD_Y 0.02f
#define CARD_W 0.90f
#define CARD_H 0.96f

/* Slot widths. */
#define LABEL_W 0.80f
#define LABEL_H 0.06f
#define GATE_W  0.80f
#define GATE_H  0.18f

/* Horizontal margin from card left. */
#define SLOT_X  0.10f

daily_hd_layout_t daily_hd_compute(double sun_longitude)
{
    daily_hd_layout_t layout;
    memset(&layout, 0, sizeof(layout));

    /* Card frame. */
    layout.card_x = CARD_X;
    layout.card_y = CARD_Y;
    layout.card_w = CARD_W;
    layout.card_h = CARD_H;

    /* Gate count. */
    layout.gate_count = 2;

    /* Title slot. */
    layout.title_slot.x = SLOT_X;
    layout.title_slot.y = 0.05f;
    layout.title_slot.w = LABEL_W;
    layout.title_slot.h = LABEL_H;

    /* Cross label. */
    layout.cross_slot.x = SLOT_X;
    layout.cross_slot.y = 0.14f;
    layout.cross_slot.w = LABEL_W;
    layout.cross_slot.h = LABEL_H;

    /* Sun label. */
    layout.sun_label.x = SLOT_X;
    layout.sun_label.y = 0.18f;
    layout.sun_label.w = LABEL_W;
    layout.sun_label.h = LABEL_H;

    /* Earth label. */
    layout.earth_label.x = SLOT_X;
    layout.earth_label.y = 0.42f;
    layout.earth_label.w = LABEL_W;
    layout.earth_label.h = LABEL_H;

    /* Compute incarnation cross from Sun longitude. */
    hd_incarnation_t ic = hd_incarnation(sun_longitude);

    /* Sun gate display. */
    layout.sun_gate.gate = ic.sun.gate;
    layout.sun_gate.line = ic.sun.line;
    layout.sun_gate.gate_name = hd_gate_name(ic.sun.gate);
    layout.sun_gate.keyword = hd_gate_keyword(ic.sun.gate);
    layout.sun_gate.slot.x = SLOT_X;
    layout.sun_gate.slot.y = 0.22f;
    layout.sun_gate.slot.w = GATE_W;
    layout.sun_gate.slot.h = GATE_H;

    /* Earth gate display. */
    layout.earth_gate.gate = ic.earth.gate;
    layout.earth_gate.line = ic.earth.line;
    layout.earth_gate.gate_name = hd_gate_name(ic.earth.gate);
    layout.earth_gate.keyword = hd_gate_keyword(ic.earth.gate);
    layout.earth_gate.slot.x = SLOT_X;
    layout.earth_gate.slot.y = 0.48f;
    layout.earth_gate.slot.w = GATE_W;
    layout.earth_gate.slot.h = GATE_H;

    return layout;
}

const char *daily_hd_sun_gate_name(const daily_hd_layout_t *layout)
{
    if (!layout) {
        return "?";
    }
    return layout->sun_gate.gate_name;
}

const char *daily_hd_earth_gate_name(const daily_hd_layout_t *layout)
{
    if (!layout) {
        return "?";
    }
    return layout->earth_gate.gate_name;
}
