#ifndef TIME_DAILY_TRANSIT_LAYOUT_H
#define TIME_DAILY_TRANSIT_LAYOUT_H

/* Daily transit card layout — today's astrological snapshot.
 *
 * Shows Sun sign, Moon sign, planet positions, active aspects,
 * and retrograde status in a card layout with normalized 0-1 positions.
 *
 * Planet index convention (matches dignity.h):
 *   0=Mercury, 1=Venus, 2=Earth, 3=Mars, 4=Jupiter,
 *   5=Saturn, 6=Uranus, 7=Neptune, 8=Sun, 9=Moon
 *
 * Aspects are computed among the first 8 planets (0-7) only,
 * matching aspects_find_all() convention.
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 */

#define TRANSIT_MAX_PLANETS 10
#define TRANSIT_MAX_ASPECTS 28

typedef struct {
    float x, y;           /* position (normalized 0-1) */
    float w, h;           /* size */
} transit_slot_t;

typedef struct {
    int planet_index;
    int sign;
    float degree;
    const char *sign_name;
    const char *sign_symbol;
    int dignity;
    int is_retrograde;        /* 1 if retrograde (placeholder — always 0) */
} transit_planet_info_t;

typedef struct {
    int planet_a;
    int planet_b;
    int type;                 /* aspect_type_t */
    float orb;
    const char *aspect_name;
    const char *aspect_symbol;
} transit_aspect_info_t;

typedef struct {
    /* Header: Sun and Moon info prominently */
    transit_slot_t sun_slot;
    transit_slot_t moon_slot;
    transit_planet_info_t sun;
    transit_planet_info_t moon;

    /* All planet positions */
    transit_slot_t planet_slots[TRANSIT_MAX_PLANETS];
    transit_planet_info_t planets[TRANSIT_MAX_PLANETS];
    int planet_count;

    /* Active aspects */
    transit_slot_t aspect_slots[TRANSIT_MAX_ASPECTS];
    transit_aspect_info_t aspects[TRANSIT_MAX_ASPECTS];
    int aspect_count;

    /* Card frame */
    float card_x, card_y, card_w, card_h;
} daily_transit_layout_t;

/* Compute the daily transit card layout.
 * lons: array of 10 ecliptic longitudes (0-7=planets, 8=Sun, 9=Moon).
 * max_orb: aspect tolerance in degrees.
 * Returns zeroed layout for NULL lons. */
daily_transit_layout_t daily_transit_compute(
    const double lons[10],
    double max_orb
);

/* Write a one-line summary into buf (e.g. "Sun in Aries, Moon in Cancer").
 * Returns buf, or NULL if layout or buf is NULL. */
const char *daily_transit_summary(
    const daily_transit_layout_t *layout,
    char *buf,
    int buf_size
);

/* Number of active aspects in this transit layout. */
int daily_transit_aspect_count(const daily_transit_layout_t *layout);

#endif /* TIME_DAILY_TRANSIT_LAYOUT_H */
