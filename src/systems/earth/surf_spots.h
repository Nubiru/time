/* surf_spots.h — World-class surf spot data: 50 iconic breaks with
 * coordinates, wave characteristics, tidal/swell quality functions. */

#ifndef TIME_SURF_SPOTS_H
#define TIME_SURF_SPOTS_H

#include <stdbool.h>

#define SURF_SPOT_COUNT 50

typedef enum {
    TIDE_LOW = 0,
    TIDE_MID,
    TIDE_HIGH,
    TIDE_ALL
} tide_pref_t;

typedef enum {
    SEASON_WINTER = 0,
    SEASON_SPRING,
    SEASON_SUMMER,
    SEASON_AUTUMN,
    SEASON_ALL
} season_pref_t;

typedef enum {
    WAVE_REEF = 0,
    WAVE_BEACH,
    WAVE_POINT,
    WAVE_RIVER_MOUTH
} wave_type_t;

typedef struct {
    int id;
    const char *name;
    const char *country;
    double lat;
    double lon;
    double beach_facing_deg;    /* compass degrees the beach faces (0=N, 90=E) */
    double best_swell_dir_deg;  /* optimal incoming swell direction (compass degrees) */
    tide_pref_t best_tide;
    season_pref_t best_season;
    wave_type_t wave_type;
    const char *description;
} surf_spot_t;

/* Get a surf spot by index (0..49). Returns id=-1 for invalid index. */
surf_spot_t surf_spot_get(int index);

/* Returns the total number of surf spots. */
int surf_spot_count(void);

/* Returns the index of the nearest spot to (lat, lon) via Haversine. */
int surf_spot_nearest(double lat, double lon);

/* Tide quality 0.0..1.0 for a given tidal_height (-1.0 low .. +1.0 high). */
double surf_spot_tide_quality(int spot_index, double tidal_height);

/* Swell direction quality 0.0..1.0. swell_dir_deg is incoming compass degrees. */
double surf_spot_swell_quality(int spot_index, double swell_dir_deg);

/* Returns the name string for a spot, or NULL if index is invalid. */
const char *surf_spot_name(int index);

/* Fills out_indices with spots matching wave type. Returns count written. */
int surf_spots_by_type(wave_type_t type, int *out_indices, int out_max);

/* Fills out_indices with spots within a lat/lon bounding box. Returns count. */
int surf_spots_in_region(double lat_min, double lat_max,
                         double lon_min, double lon_max,
                         int *out_indices, int out_max);

/* Human-readable enum names. */
const char *surf_tide_name(tide_pref_t tide);
const char *surf_season_name(season_pref_t season);
const char *surf_wave_type_name(wave_type_t type);

#endif /* TIME_SURF_SPOTS_H */
