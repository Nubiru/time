/*
 * earth_timeline_layout.h — Earth History timeline card positions (E6)
 *
 * Maps 20 earth_story moments to screen positions along a horizontal
 * timeline. Uses logarithmic time compression (4.5 Ga → now) so recent
 * events are spaced apart and ancient events are compressed.
 *
 * Each moment produces a positioned card with title, era, and drama level.
 * Drama level controls visual prominence (opacity, size, glow).
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 */

#ifndef TIME_EARTH_TIMELINE_LAYOUT_H
#define TIME_EARTH_TIMELINE_LAYOUT_H

#include "../systems/geology/earth_story.h"

#define ETL_MAX_CARDS 20

/* A single era card on the timeline */
typedef struct {
    int moment_index;       /* earth_story moment index */
    const char *title;      /* moment title */
    float x;                /* normalized screen x (0=left, 1=right) */
    float y;                /* normalized screen y (0.5 = center) */
    float width;            /* card width (normalized) */
    float height;           /* card height (normalized) */
    float opacity;          /* 0.0-1.0 (from drama level) */
    float scale;            /* 1.0 = normal, larger for dramatic moments */
    double ma;              /* millions of years ago */
    int drama;              /* 1-5 drama level */
} etl_card_t;

/* Timeline layout result */
typedef struct {
    etl_card_t cards[ETL_MAX_CARDS];
    int count;
    float timeline_y;       /* y position of the timeline axis */
    float timeline_x_min;   /* left edge of timeline (normalized) */
    float timeline_x_max;   /* right edge of timeline (normalized) */
} etl_layout_t;

/* Compute timeline layout for a given viewport.
 * scroll_offset: 0.0 = show most recent, 1.0 = show oldest.
 * visible_span_ma: how many Ma are visible at once (default: 500). */
etl_layout_t etl_compute(float scroll_offset, float visible_span_ma);

/* Convert Ma (millions of years ago) to normalized x position.
 * Uses logarithmic compression: log10(ma + 1). */
float etl_ma_to_x(double ma, double total_ma);

/* Get the card nearest to a screen x position.
 * Returns card index, or -1 if no cards are near. */
int etl_nearest_card(const etl_layout_t *layout, float screen_x);

#endif /* TIME_EARTH_TIMELINE_LAYOUT_H */
