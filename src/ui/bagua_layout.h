/* bagua_layout.h — Bagua (8 trigrams) wheel layout
 *
 * Computes positions for the 8 trigrams in the Later Heaven (King Wen)
 * octagonal arrangement. Each trigram has a position, label, direction,
 * element, and line data.
 *
 * All positions in normalized 0-1 coordinate space.
 * Center at (0.50, 0.50), radius 0.35.
 *
 * Later Heaven arrangement (clockwise from South):
 *   S=Li, SW=Kun, W=Dui, NW=Qian, N=Kan, NE=Gen, E=Zhen, SE=Xun
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_BAGUA_LAYOUT_H
#define TIME_BAGUA_LAYOUT_H

#define BAGUA_TRIGRAM_COUNT 8

/* Single trigram position in the bagua wheel */
typedef struct {
    int trigram_index;     /* 0-7 (iching.h convention) */
    const char *name;      /* English name, e.g., "Heaven" */
    const char *symbol;    /* Unicode trigram symbol */
    float x, y;            /* center position (normalized 0-1) */
    float angle_deg;       /* angle from center in degrees */
    const char *direction; /* "S", "SW", "W", "NW", "N", "NE", "E", "SE" */
    const char *element;   /* "Fire", "Earth", "Lake", "Heaven", "Water",
                              "Mountain", "Thunder", "Wind" */
    int lines[3];          /* 3 lines bottom to top, 0=yin 1=yang */
} bagua_trigram_t;

typedef struct {
    bagua_trigram_t trigrams[BAGUA_TRIGRAM_COUNT];
    float center_x, center_y;   /* center of wheel */
    float radius;                /* radius to trigram positions */
    int highlighted;             /* index of highlighted trigram, or -1 */
} bagua_layout_t;

/* Compute the bagua wheel layout.
 * highlight_trigram: 0-7 to highlight a trigram, -1 for none.
 * Invalid highlight values are treated as -1. */
bagua_layout_t bagua_layout_compute(int highlight_trigram);

/* Get a trigram at a wheel position (0-7, clockwise from S).
 * Returns zeroed struct with trigram_index=-1 for invalid. */
bagua_trigram_t bagua_trigram_at(const bagua_layout_t *layout, int index);

/* Get direction name for a wheel position (0-7).
 * Returns "?" for invalid position. */
const char *bagua_direction_name(int position);

#endif /* TIME_BAGUA_LAYOUT_H */
