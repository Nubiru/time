/* daily_hd_layout.h — Daily Human Design transit card layout.
 *
 * Computes a card layout showing today's HD transit information:
 * Sun gate, Earth gate, incarnation cross, and gate names/keywords.
 * All positions in normalized 0-1 coordinate space.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_DAILY_HD_LAYOUT_H
#define TIME_DAILY_HD_LAYOUT_H

/* Slot rectangle for positioning text elements. */
typedef struct {
    float x, y;
    float w, h;
} hd_slot_t;

/* A single gate display with position and descriptive text. */
typedef struct {
    int gate;                /* 1-64 */
    int line;                /* 1-6 */
    const char *gate_name;   /* e.g., "The Creative" */
    const char *keyword;     /* e.g., "Self-Expression" */
    hd_slot_t slot;          /* position in card */
} hd_gate_display_t;

typedef struct {
    /* Sun and Earth gates (incarnation cross) */
    hd_gate_display_t sun_gate;
    hd_gate_display_t earth_gate;

    /* Gate count (currently 2: sun + earth) */
    int gate_count;

    /* Text positions */
    hd_slot_t title_slot;    /* "Human Design" header */
    hd_slot_t cross_slot;    /* incarnation cross label */
    hd_slot_t sun_label;     /* "Sun Gate" label */
    hd_slot_t earth_label;   /* "Earth Gate" label */

    /* Card frame */
    float card_x, card_y, card_w, card_h;
} daily_hd_layout_t;

/* Compute the daily HD card layout.
 * sun_longitude: ecliptic longitude of the Sun in degrees (0-360).
 * Out-of-range values are normalized to 0-360. */
daily_hd_layout_t daily_hd_compute(double sun_longitude);

/* Get the Sun gate name from the layout. Returns "?" if NULL layout. */
const char *daily_hd_sun_gate_name(const daily_hd_layout_t *layout);

/* Get the Earth gate name from the layout. Returns "?" if NULL layout. */
const char *daily_hd_earth_gate_name(const daily_hd_layout_t *layout);

#endif /* TIME_DAILY_HD_LAYOUT_H */
