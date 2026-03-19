/* horizon_camera.h — Ground-level camera on Earth's surface.
 *
 * Observer at lat/lon looks in direction defined by azimuth (compass bearing)
 * and elevation (tilt up/down from horizon). Produces view and projection
 * matrices for rendering the horizon view.
 *
 * Pure module: no GL calls, no malloc, no globals, no side effects.
 *
 * Coordinate system: Y-up right-handed, matching earth_globe.h.
 *   X = cos(lat) * cos(lon) * R
 *   Y = sin(lat) * R
 *   Z = cos(lat) * sin(lon) * R
 */

#ifndef TIME_HORIZON_CAMERA_H
#define TIME_HORIZON_CAMERA_H

#include "../math/mat4.h"
#include "../math/vec3.h"

typedef struct {
    double observer_lat_deg;   /* -90 to 90 */
    double observer_lon_deg;   /* -180 to 180 */
    float  globe_radius;       /* Earth globe radius in scene units */
    float  look_azimuth;       /* compass bearing (radians): 0=N, PI/2=E, PI=S */
    float  look_elevation;     /* vertical tilt (radians): 0=horizon, PI/2=zenith */
    float  fov;                /* field of view (radians) */
    float  aspect;             /* viewport width / height */
    float  near_plane;
    float  far_plane;
} horizon_camera_t;

/* Create camera at lat/lon on globe surface.
 * Defaults: looking North at horizon, 90-degree FOV, 16:9 aspect. */
horizon_camera_t hcam_create(double lat_deg, double lon_deg, float globe_radius);

/* Observer position on globe surface (XYZ in Y-up coords). */
void hcam_position(const horizon_camera_t *cam, float xyz[3]);

/* Unit look direction vector in world space. */
void hcam_look_direction(const horizon_camera_t *cam, float dir[3]);

/* Local up (zenith) direction — radially outward from globe center. */
void hcam_up_direction(const horizon_camera_t *cam, float up[3]);

/* View matrix (camera transform). */
mat4_t hcam_view_matrix(const horizon_camera_t *cam);

/* Projection matrix. */
mat4_t hcam_projection_matrix(const horizon_camera_t *cam);

/* Rotate look direction by deltas. Azimuth wraps, elevation clamps. */
void hcam_rotate(horizon_camera_t *cam, float delta_azimuth, float delta_elevation);

#endif /* TIME_HORIZON_CAMERA_H */
