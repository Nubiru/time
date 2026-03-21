/* daily_geology_layout.h — Daily geological time card layout
 *
 * Computes a card layout showing Earth's current position in geological
 * time: current eon, a rotating "fact of the day" from the earth_fraction
 * module, and the human fraction of Earth's history.
 * All positions in normalized 0-1 coordinate space.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_DAILY_GEOLOGY_LAYOUT_H
#define TIME_DAILY_GEOLOGY_LAYOUT_H

/* Slot rectangle for positioning text elements. */
typedef struct {
    float x, y;
    float w, h;
} geo_slot_t;

typedef struct {
    /* Current eon (always Phanerozoic for present day) */
    const char *eon_name;
    const char *eon_meaning;
    const char *eon_brief;

    /* Earth fraction fact (rotating based on JD) */
    const char *fact_name;
    const char *fact_description;
    const char *fact_analogy;

    /* Human fraction */
    double human_fraction_pct;  /* ~0.0066% */

    /* Layout slots */
    geo_slot_t title_slot;
    geo_slot_t eon_slot;
    geo_slot_t meaning_slot;
    geo_slot_t fact_slot;
    geo_slot_t analogy_slot;
    geo_slot_t fraction_slot;

    /* Card frame */
    float card_x, card_y, card_w, card_h;
} daily_geology_layout_t;

/* Compute the daily geology card layout.
 * jd: Julian Day number. */
daily_geology_layout_t daily_geology_compute(double jd);

/* Get the current eon name. Returns "?" if NULL layout. */
const char *daily_geology_eon_name(const daily_geology_layout_t *layout);

#endif /* TIME_DAILY_GEOLOGY_LAYOUT_H */
