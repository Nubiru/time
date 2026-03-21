/* bodygraph_chart_layout.c — Visual layout of the Human Design bodygraph.
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "bodygraph_chart_layout.h"
#include "../systems/human_design/bodygraph.h"
#include <string.h>

/* Layout bounds. */
#define BOUNDS_X 0.10f
#define BOUNDS_Y 0.05f
#define BOUNDS_W 0.80f
#define BOUNDS_H 0.90f

/* Center shape size. */
#define CENTER_W 0.12f
#define CENTER_H 0.08f

/* Scale a raw 0-1 position into the layout bounds. */
static float scale_x(float raw)
{
    return BOUNDS_X + raw * BOUNDS_W;
}

static float scale_y(float raw)
{
    return BOUNDS_Y + raw * BOUNDS_H;
}

bodygraph_chart_layout_t bodygraph_chart_compute(const int *active_gates)
{
    bodygraph_chart_layout_t layout;
    memset(&layout, 0, sizeof(layout));

    layout.bounds_x = BOUNDS_X;
    layout.bounds_y = BOUNDS_Y;
    layout.bounds_w = BOUNDS_W;
    layout.bounds_h = BOUNDS_H;
    layout.channel_count = BODYGRAPH_CHANNEL_COUNT;

    /* --- Centers --- */
    int defined_flags[BODYGRAPH_CENTER_COUNT];
    memset(defined_flags, 0, sizeof(defined_flags));

    /* Empty active_gates array for NULL input. */
    int empty_gates[65];
    memset(empty_gates, 0, sizeof(empty_gates));

    const int *gates = active_gates ? active_gates : empty_gates;

    /* Compute defined centers. */
    bodygraph_defined_centers(gates, defined_flags);

    layout.defined_count = 0;
    for (int i = 0; i < BG_CHART_CENTER_COUNT; i++) {
        bg_center_t src = bodygraph_center((bg_center_id_t)i);
        layout.centers[i].id = i;
        layout.centers[i].name = src.name;
        layout.centers[i].x = scale_x(src.position.x);
        layout.centers[i].y = scale_y(src.position.y);
        layout.centers[i].w = CENTER_W;
        layout.centers[i].h = CENTER_H;
        layout.centers[i].is_defined = defined_flags[i];
        layout.centers[i].is_motor = src.is_motor;
        layout.centers[i].is_awareness = src.is_awareness;
        if (defined_flags[i]) {
            layout.defined_count++;
        }
    }

    /* --- Channels --- */
    layout.active_channel_count = 0;
    for (int i = 0; i < BODYGRAPH_CHANNEL_COUNT; i++) {
        bg_channel_t ch = bodygraph_channel(i);

        layout.channels[i].channel_index = i;
        layout.channels[i].gate_a = ch.gate_a;
        layout.channels[i].gate_b = ch.gate_b;
        layout.channels[i].center_a = (int)ch.center_a;
        layout.channels[i].center_b = (int)ch.center_b;

        /* Line endpoints from center positions. */
        layout.channels[i].x0 = layout.centers[ch.center_a].x;
        layout.channels[i].y0 = layout.centers[ch.center_a].y;
        layout.channels[i].x1 = layout.centers[ch.center_b].x;
        layout.channels[i].y1 = layout.centers[ch.center_b].y;

        /* Active if both gates are active. */
        int active = (gates[ch.gate_a] && gates[ch.gate_b]) ? 1 : 0;
        layout.channels[i].is_active = active;
        if (active) {
            layout.active_channel_count++;
        }
    }

    /* --- HD type (simplified) --- */
    if (layout.defined_count == 0) {
        layout.type = 4;  /* Reflector */
        layout.type_name = "Reflector";
    } else if (defined_flags[BG_SACRAL]) {
        layout.type = 1;  /* Generator (simplified — no MG detection) */
        layout.type_name = "Generator";
    } else {
        layout.type = -1;
        layout.type_name = "Unknown";
    }

    return layout;
}

chart_center_t bodygraph_chart_center(const bodygraph_chart_layout_t *layout,
                                      int center_id)
{
    if (!layout || center_id < 0 || center_id >= BG_CHART_CENTER_COUNT) {
        chart_center_t invalid;
        memset(&invalid, 0, sizeof(invalid));
        invalid.id = -1;
        invalid.name = "?";
        return invalid;
    }
    return layout->centers[center_id];
}

int bodygraph_chart_is_defined(const bodygraph_chart_layout_t *layout,
                               int center_id)
{
    if (!layout || center_id < 0 || center_id >= BG_CHART_CENTER_COUNT) {
        return 0;
    }
    return layout->centers[center_id].is_defined;
}
