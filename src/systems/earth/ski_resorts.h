/* ski_resorts.h — 50 world ski/mountain resorts with coordinates,
 * elevation data, season windows, and quality scoring. */

#ifndef TIME_SKI_RESORTS_H
#define TIME_SKI_RESORTS_H

#include <stdbool.h>

#define SKI_RESORT_COUNT 50

typedef struct {
    int id;
    const char *name;
    const char *country;
    double lat;
    double lon;
    int base_elev_m;        /* base elevation in meters */
    int summit_elev_m;      /* summit elevation in meters */
    int season_start;       /* month 1-12 (start of ski season) */
    int season_end;         /* month 1-12 (end of ski season) */
    const char *description;
} ski_resort_t;

/* Get resort by index (0..49). Returns id=-1 for invalid. */
ski_resort_t ski_resort_get(int index);

/* Total number of resorts. */
int ski_resort_count(void);

/* Index of nearest resort to lat/lon (Haversine). */
int ski_resort_nearest(double lat, double lon);

/* Snow quality 0.0-1.0 for a resort in a given month (1-12).
 * Peak months = 1.0, shoulder = partial, off-season = 0.0. */
double ski_resort_snow_quality(int resort_index, int month);

/* Resort name by index, or NULL if invalid. */
const char *ski_resort_name(int index);

/* Vertical drop in meters (summit - base). Returns 0 for invalid. */
int ski_resort_vertical(int index);

/* Find resorts in a lat/lon bounding box. Returns count written. */
int ski_resorts_in_region(double lat_min, double lat_max,
                          double lon_min, double lon_max,
                          int *out_indices, int out_max);

/* Is the resort in season for the given month? Handles wrap-around. */
bool ski_resort_in_season(int index, int month);

#endif /* TIME_SKI_RESORTS_H */
