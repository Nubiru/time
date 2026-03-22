/* daily_earth_layout.h — Daily Earth surface card layout
 *
 * Computes a card layout showing Earth-surface information for a
 * given moment and location: daylight hours, sun elevation, season,
 * season progress, and polar day/night status.
 * All positions in normalized 0-1 coordinate space.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_DAILY_EARTH_LAYOUT_H
#define TIME_DAILY_EARTH_LAYOUT_H

/* Slot rectangle for positioning text elements. */
typedef struct {
    float x, y;
    float w, h;
} earth_slot_t;

typedef struct {
    /* Daylight */
    int is_day;
    double sun_elevation_deg;
    double day_length_hours;
    int polar_day;
    int polar_night;

    /* Season */
    int season;                 /* 0-3 */
    const char *season_name;
    double season_progress;     /* 0.0-1.0 */

    /* Interpretation (composed from daylight + season data) */
    char glance[128];           /* "Spring · 12.3 hours daylight" */
    char detail[512];           /* Full narrative */

    /* Layout slots */
    earth_slot_t title_slot;
    earth_slot_t daylight_slot;  /* "Daylight: 12.3 hours" */
    earth_slot_t sun_slot;       /* "Sun: 45.2 above horizon" */
    earth_slot_t season_slot;    /* "Spring (42% complete)" */
    earth_slot_t polar_slot;     /* polar day/night or empty */
    earth_slot_t length_slot;    /* day length detail */

    /* Card frame */
    float card_x, card_y, card_w, card_h;
} daily_earth_layout_t;

/* Compute the daily Earth card layout.
 * jd: current Julian Day.
 * lat: latitude in degrees (-90 to 90).
 * lon: longitude in degrees (-180 to 180). */
daily_earth_layout_t daily_earth_compute(double jd, double lat, double lon);

/* Check if it is currently daytime. Returns 0 if NULL layout. */
int daily_earth_is_day(const daily_earth_layout_t *layout);

#endif /* TIME_DAILY_EARTH_LAYOUT_H */
