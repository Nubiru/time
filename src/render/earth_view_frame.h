/* earth_view_frame.h — Earth View per-frame data aggregator.
 *
 * Computes all data needed to render one frame of the Earth View:
 *   1. Camera matrices (from horizon_camera)
 *   2. Sun position in sky (altitude/azimuth from solar_events)
 *   3. Sky dome config (from sky_dome using sun position)
 *   4. Horizon cardinals (from horizon_geometry)
 *   5. Visible celestial objects (Sun for v1)
 *
 * The render pass calls ev_compute_frame() once, then reads the result.
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 * Uses stack-allocated buffers for geometry data. */

#ifndef TIME_EARTH_VIEW_FRAME_H
#define TIME_EARTH_VIEW_FRAME_H

#include "horizon_camera.h"
#include "horizon_geometry.h"
#include "sky_dome.h"
#include "../math/mat4.h"
#include "../math/equatorial_horizontal.h"

/* Maximum visible celestial objects in ground view */
#define EV_MAX_OBJECTS 16

/* A celestial object projected onto the sky */
typedef struct {
    float altitude_deg;      /* degrees above horizon */
    float azimuth_deg;       /* compass bearing (0=N, 90=E) */
    float brightness;        /* relative brightness 0-1 */
    int   is_visible;        /* above horizon? */
    const char *name;        /* "Sun", "Moon", "Venus", etc. */
} ev_sky_object_t;

/* Complete Earth View frame data */
typedef struct {
    /* Camera */
    mat4_t view_matrix;
    mat4_t proj_matrix;
    float camera_pos[3];
    float camera_up[3];
    float camera_look[3];

    /* Sun */
    float sun_altitude_deg;
    float sun_azimuth_deg;
    int   is_daytime;         /* sun altitude > 0 */

    /* Sky dome config (pass to sky_dome_generate) */
    sky_dome_config_t sky_config;

    /* Horizon (cardinal count always 8) */
    hzg_cardinal_t cardinals[8];
    int cardinal_count;

    /* Visible objects */
    ev_sky_object_t objects[EV_MAX_OBJECTS];
    int object_count;
} ev_frame_t;

/* Compute Earth View frame data.
 * jd: Julian Day (determines sun position, time).
 * observer_lat: degrees (-90 to 90).
 * observer_lon: degrees (-180 to 180).
 * look_azimuth: camera compass bearing (radians).
 * look_elevation: camera vertical tilt (radians).
 * fov: field of view (radians).
 * aspect: viewport width/height ratio.
 * globe_radius: Earth globe radius in scene units.
 * Returns complete frame data. */
ev_frame_t ev_compute_frame(double jd,
                             double observer_lat, double observer_lon,
                             float look_azimuth, float look_elevation,
                             float fov, float aspect, float globe_radius);

/* Simplified: compute frame with default FOV/aspect/radius. */
ev_frame_t ev_compute_simple(double jd,
                              double observer_lat, double observer_lon,
                              float look_azimuth, float look_elevation);

/* Is it currently daytime at this location? */
int ev_is_daytime(const ev_frame_t *frame);

/* Get the sun's sky position. */
void ev_sun_position(const ev_frame_t *frame, float *alt_deg, float *az_deg);

#endif /* TIME_EARTH_VIEW_FRAME_H */
