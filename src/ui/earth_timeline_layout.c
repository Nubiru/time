/*
 * earth_timeline_layout.c — Earth History timeline card positions (E6)
 *
 * Logarithmic time scale: log10(ma + 1) / log10(4500 + 1)
 * This naturally compresses ancient events and spreads recent ones.
 *
 * Pure module: no GL, no malloc, no globals.
 */

#include "earth_timeline_layout.h"

#include <math.h>

/* Total Earth age in Ma */
#define TOTAL_MA 4500.0

/* Timeline margins (normalized screen coords) */
#define TL_MARGIN_LEFT   0.05f
#define TL_MARGIN_RIGHT  0.95f
#define TL_AXIS_Y        0.50f

/* Card dimensions (normalized) */
#define TL_CARD_W        0.12f
#define TL_CARD_H        0.15f
#define TL_CARD_GAP_Y    0.02f

float etl_ma_to_x(double ma, double total_ma)
{
    if (ma <= 0.0) return TL_MARGIN_RIGHT;
    if (ma >= total_ma) return TL_MARGIN_LEFT;

    /* Logarithmic compression: recent events spread, ancient compressed */
    double log_total = log10(total_ma + 1.0);
    double log_ma = log10(ma + 1.0);
    double frac = log_ma / log_total; /* 0 = now, 1 = oldest */

    /* Map to screen: right = now, left = oldest */
    float x = TL_MARGIN_RIGHT - (float)frac * (TL_MARGIN_RIGHT - TL_MARGIN_LEFT);
    return x;
}

etl_layout_t etl_compute(float scroll_offset, float visible_span_ma)
{
    (void)scroll_offset;   /* future: horizontal scrolling */
    (void)visible_span_ma; /* future: zoom into timeline */

    etl_layout_t layout;
    layout.count = 0;
    layout.timeline_y = TL_AXIS_Y;
    layout.timeline_x_min = TL_MARGIN_LEFT;
    layout.timeline_x_max = TL_MARGIN_RIGHT;

    int n = es_moment_count();
    if (n > ETL_MAX_CARDS) n = ETL_MAX_CARDS;

    for (int i = 0; i < n; i++) {
        es_moment_t m = es_moment_get(i);
        if (m.title == 0) continue;

        etl_card_t *c = &layout.cards[layout.count];
        c->moment_index = i;
        c->title = m.title;
        c->ma = m.ma;
        c->drama = (int)m.drama;

        /* X position from logarithmic time scale */
        c->x = etl_ma_to_x(m.ma, TOTAL_MA);

        /* Alternate cards above/below the timeline axis */
        float offset = (layout.count % 2 == 0) ? -1.0f : 1.0f;
        c->y = TL_AXIS_Y + offset * (TL_CARD_H * 0.5f + TL_CARD_GAP_Y);

        /* Size and opacity from drama level */
        float drama_frac = (float)m.drama / (float)ES_DRAMA_MAX;
        c->width = TL_CARD_W * (0.8f + drama_frac * 0.4f);  /* 80-120% base */
        c->height = TL_CARD_H * (0.8f + drama_frac * 0.4f);
        c->opacity = 0.5f + drama_frac * 0.5f;  /* 50-100% */
        c->scale = 0.8f + drama_frac * 0.4f;    /* 80-120% */

        layout.count++;
    }

    return layout;
}

int etl_nearest_card(const etl_layout_t *layout, float screen_x)
{
    if (!layout || layout->count == 0) return -1;

    int best = -1;
    float best_dist = 999.0f;

    for (int i = 0; i < layout->count; i++) {
        float dx = screen_x - layout->cards[i].x;
        if (dx < 0.0f) dx = -dx;
        if (dx < best_dist) {
            best_dist = dx;
            best = i;
        }
    }

    /* Only return if within reasonable proximity */
    if (best_dist > 0.1f) return -1;
    return best;
}
