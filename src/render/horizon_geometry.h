/* horizon_geometry.h — Pure geometry for horizon line rendering.
 *
 * Computes the geometric horizon (great circle 90 degrees from zenith),
 * altitude/azimuth grid, cardinal directions, and alt/az to XYZ conversion
 * for an observer on the Earth globe. Packs results as GPU-ready line strip
 * vertex data. Includes GLSL ES 3.00 shader source strings.
 *
 * Pure module: no GL calls, no malloc, no globals, no side effects.
 *
 * Coordinate system: Y-up right-handed, matching earth_globe.h.
 *   X = cos(lat) * cos(lon)
 *   Y = sin(lat)
 *   Z = cos(lat) * sin(lon)
 */

#ifndef TIME_HORIZON_GEOMETRY_H
#define TIME_HORIZON_GEOMETRY_H

#ifndef PI
#define PI 3.14159265358979323846
#endif

/* Horizon line configuration */
typedef struct {
    double observer_lat_deg;    /* observer latitude in degrees (-90 to 90) */
    double observer_lon_deg;    /* observer longitude in degrees (-180 to 180) */
    float  globe_radius;        /* Earth globe radius in scene units */
    int    horizon_segments;    /* line segments around the horizon (default: 72) */
    int    altitude_steps;      /* altitude grid circles (default: 5, at 15/30/45/60/75 deg) */
    int    azimuth_lines;       /* azimuth grid lines from zenith (default: 12, every 30 deg) */
} hzg_config_t;

/* Default configuration */
hzg_config_t hzg_default_config(void);

/* Horizon vertex layout: position(vec3) + color(vec4) = 7 floats */
#define HZG_VERTEX_FLOATS 7
#define HZG_MAX_VERTICES 4000

/* Cardinal/intercardinal point on the horizon */
typedef struct {
    float x, y, z;              /* position on globe surface */
    float azimuth_deg;          /* compass bearing (0=N, 90=E, 180=S, 270=W) */
    const char *label;          /* "N", "NE", "E", "SE", "S", "SW", "W", "NW" */
} hzg_cardinal_t;

/* Pack result */
typedef struct {
    int horizon_verts;          /* horizon great circle line strip vertex count */
    int altitude_verts;         /* altitude grid circle vertices */
    int azimuth_verts;          /* azimuth grid line vertices */
    int cardinal_count;         /* number of cardinal points (8) */
    int total_verts;            /* total vertices in buffer */
} hzg_info_t;

/* Compute observer zenith direction (unit vector pointing straight up from lat/lon).
 * Returns xyz on unit sphere in Y-up coords. */
void hzg_zenith(double lat_deg, double lon_deg, float xyz[3]);

/* Compute the horizon great circle + altitude/azimuth grid for an observer.
 * out_verts: HZG_MAX_VERTICES * HZG_VERTEX_FLOATS floats
 * Returns hzg_info_t with vertex counts. */
hzg_info_t hzg_pack(hzg_config_t config, float *out_verts);

/* Get the 8 cardinal/intercardinal points on the horizon.
 * out: array of 8 hzg_cardinal_t.
 * Returns number of points (always 8). */
int hzg_cardinals(hzg_config_t config, hzg_cardinal_t out[8]);

/* Convert altitude/azimuth at observer location to XYZ on globe.
 * altitude_deg: 0 = horizon, 90 = zenith
 * azimuth_deg: 0 = north, 90 = east (compass convention)
 * Returns xyz position on sphere of given globe_radius. */
void hzg_altaz_to_xyz(double lat_deg, double lon_deg, float globe_radius,
                       double altitude_deg, double azimuth_deg, float xyz[3]);

/* Compute angular distance between two points on sphere (Haversine).
 * All in degrees. Returns angular distance in degrees (0-180). */
float hzg_angular_distance(double lat1, double lon1, double lat2, double lon2);

/* Check if a point (lat/lon) is above the geometric horizon from observer.
 * Returns 1 if visible (angular distance < 90), 0 otherwise. */
int hzg_is_visible(double obs_lat, double obs_lon, double pt_lat, double pt_lon);

/* Horizon line vertex shader source (GLSL ES 3.00).
 * Uniforms: u_mvp (mat4).
 * Attributes: a_position (vec3, loc 0), a_color (vec4, loc 1). */
const char *hzg_vert_source(void);

/* Horizon line fragment shader source (GLSL ES 3.00).
 * Passthrough color. */
const char *hzg_frag_source(void);

#endif /* TIME_HORIZON_GEOMETRY_H */
