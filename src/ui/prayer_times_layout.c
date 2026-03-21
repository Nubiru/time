/* prayer_times_layout.c — Islamic prayer times vertical timeline layout
 *
 * Computes prayer times via prayer_times_compute(), maps them to a
 * vertical card layout, and marks the next upcoming prayer.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "prayer_times_layout.h"
#include "../systems/islamic/prayer_times.h"
#include <math.h>
#include <string.h>

/* Static prayer names in order. */
static const char *s_prayer_names[PT_PRAYER_COUNT] = {
    "Fajr", "Sunrise", "Dhuhr", "Asr", "Maghrib", "Isha"
};

/* Slot layout constants */
#define SLOT_X       0.10f
#define SLOT_W       0.80f
#define SLOT_H       0.10f
#define SLOT_Y_START 0.18f
#define SLOT_Y_STEP  0.12f

/* Title and method slots */
#define TITLE_Y      0.03f
#define METHOD_Y     0.10f

/* Card frame */
#define CARD_X       0.05f
#define CARD_Y       0.02f
#define CARD_W       0.90f
#define CARD_H       0.96f

/* JD tolerance for matching next prayer. */
#define JD_TOLERANCE 0.001

prayer_times_layout_t prayer_times_layout_compute(
    double jd_noon, double lat, double lon,
    int method, double current_jd)
{
    prayer_times_layout_t layout;
    memset(&layout, 0, sizeof(layout));

    layout.prayer_count = PT_PRAYER_COUNT;

    /* Clamp method to valid range */
    prayer_method_t pm = (prayer_method_t)method;
    if (method < 0 || method >= PRAYER_METHOD_COUNT) {
        pm = PRAYER_METHOD_MWL;
    }

    layout.method_name = prayer_method_name(pm);

    /* Compute prayer times */
    prayer_times_t times = prayer_times_compute(jd_noon, lat, lon,
                                                 pm, ASR_SHAFII);

    /* Map prayer_times_t fields to array */
    double jds[PT_PRAYER_COUNT];
    jds[0] = times.fajr;
    jds[1] = times.sunrise;
    jds[2] = times.dhuhr;
    jds[3] = times.asr;
    jds[4] = times.maghrib;
    jds[5] = times.isha;

    /* Determine next prayer */
    next_prayer_t next = prayer_next(times, current_jd);

    /* Fill prayer slots */
    for (int i = 0; i < PT_PRAYER_COUNT; i++) {
        layout.prayers[i].index = i;
        layout.prayers[i].name  = s_prayer_names[i];
        layout.prayers[i].jd    = jds[i];

        /* Slot position: stacked vertically */
        layout.prayers[i].slot.x = SLOT_X;
        layout.prayers[i].slot.y = SLOT_Y_START + (float)i * SLOT_Y_STEP;
        layout.prayers[i].slot.w = SLOT_W;
        layout.prayers[i].slot.h = SLOT_H;

        /* Mark next prayer by matching JD within tolerance */
        layout.prayers[i].is_next =
            (fabs(jds[i] - next.jd) < JD_TOLERANCE) ? 1 : 0;
    }

    /* Title slot */
    layout.title_slot.x = SLOT_X;
    layout.title_slot.y = TITLE_Y;
    layout.title_slot.w = SLOT_W;
    layout.title_slot.h = SLOT_H;

    /* Method slot */
    layout.method_slot.x = SLOT_X;
    layout.method_slot.y = METHOD_Y;
    layout.method_slot.w = SLOT_W;
    layout.method_slot.h = SLOT_H;

    /* Card frame */
    layout.card_x = CARD_X;
    layout.card_y = CARD_Y;
    layout.card_w = CARD_W;
    layout.card_h = CARD_H;

    return layout;
}

const char *prayer_times_next_name(const prayer_times_layout_t *layout)
{
    if (!layout) {
        return "?";
    }

    for (int i = 0; i < PT_PRAYER_COUNT; i++) {
        if (layout->prayers[i].is_next) {
            return layout->prayers[i].name;
        }
    }

    return "?";
}
