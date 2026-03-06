/* moon_data.h -- Orbital elements and position calculations for 9 major moons.
 * Data source: NASA/JPL Planetary Satellite Physical Parameters (public domain).
 * All functions are pure (no side effects, no global mutation). */

#ifndef TIME_MOON_DATA_H
#define TIME_MOON_DATA_H

#define MOON_DATA_EARTH_COUNT   1
#define MOON_DATA_MARS_COUNT    2
#define MOON_DATA_JUPITER_COUNT 4
#define MOON_DATA_SATURN_COUNT  2

typedef struct {
    double a_km;           /* semi-major axis in km */
    double e;              /* eccentricity */
    double i_deg;          /* inclination to parent equator (degrees) */
    double period_days;    /* orbital period in days */
    double mean_lon_epoch; /* mean longitude at J2000.0 (degrees) */
    double radius_km;      /* moon's physical radius in km */
    double albedo;         /* geometric albedo */
    int parent_planet;     /* index into planets.h (0-7) */
} moon_orbit_t;

typedef struct {
    double x_km;        /* position relative to planet (km, orbital plane) */
    double y_km;
    double z_km;
    double distance_km; /* distance from planet center */
    double angle_deg;   /* orbital angle (degrees, 0-360) */
} moon_position_t;

typedef struct {
    const char *name;
    moon_orbit_t orbit;
} moon_entry_t;

/* Get number of cataloged moons for a planet (0 for planets without major moons listed) */
int moon_data_count(int planet_index);

/* Get moon entry by planet and moon index */
moon_entry_t moon_data_get(int planet_index, int moon_index);

/* Get moon name */
const char *moon_data_name(int planet_index, int moon_index);

/* Compute moon position relative to parent planet at Julian Day */
moon_position_t moon_data_position(int planet_index, int moon_index, double jd);

/* Get orbital period in days */
double moon_data_period(int planet_index, int moon_index);

/* Get moon radius in km */
double moon_data_radius(int planet_index, int moon_index);

/* Scene-space distance from parent planet (for rendering).
 * base_scale converts km to scene units. */
double moon_data_scene_distance(int planet_index, int moon_index, double base_scale);

/* Scene-space radius (for rendering) */
double moon_data_scene_radius(int planet_index, int moon_index, double base_scale);

/* Total number of moons across all planets in catalog */
int moon_data_total_count(void);

#endif /* TIME_MOON_DATA_H */
