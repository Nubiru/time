/* horizon_camera.c — Ground-level camera implementation.
 *
 * Positions camera on Earth's surface at observer lat/lon, builds local
 * tangent-plane basis (east, north), applies azimuth/elevation to compute
 * look direction. Produces view and projection matrices.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "horizon_camera.h"
#include <math.h>

#ifndef PI
#define PI 3.14159265358979323846
#endif

#define TWO_PI (2.0 * PI)

/* ---- Internal helpers ---- */

static void deg_to_rad(double deg_lat, double deg_lon,
                       double *rad_lat, double *rad_lon)
{
    *rad_lat = deg_lat * PI / 180.0;
    *rad_lon = deg_lon * PI / 180.0;
}

/* Compute zenith unit vector in Y-up coords (matching earth_globe.h) */
static void compute_zenith(double lat_deg, double lon_deg, double z[3])
{
    double lat_r, lon_r;
    deg_to_rad(lat_deg, lon_deg, &lat_r, &lon_r);
    z[0] = cos(lat_r) * cos(lon_r);
    z[1] = sin(lat_r);
    z[2] = cos(lat_r) * sin(lon_r);
}

/* Normalize a 3-vector in place. Returns length. */
static double normalize3(double v[3])
{
    double len = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    if (len > 1e-12) {
        v[0] /= len;
        v[1] /= len;
        v[2] /= len;
    }
    return len;
}

/* Cross product: out = a x b */
static void cross3(const double a[3], const double b[3], double out[3])
{
    out[0] = a[1] * b[2] - a[2] * b[1];
    out[1] = a[2] * b[0] - a[0] * b[2];
    out[2] = a[0] * b[1] - a[1] * b[0];
}

/* Build orthonormal tangent-plane basis at zenith.
 * east: geographic east, north_hz: geographic north along horizon.
 * Matches horizon_geometry.c build_horizon_basis(). */
static void build_basis(const double zenith[3],
                        double east[3], double north_hz[3])
{
    double up[3] = {0.0, 1.0, 0.0};
    cross3(zenith, up, east);
    double elen = normalize3(east);
    if (elen < 1e-9) {
        /* At a pole: zenith is parallel to Y_up, use X_right instead */
        double right[3] = {1.0, 0.0, 0.0};
        cross3(zenith, right, east);
        normalize3(east);
    }
    cross3(east, zenith, north_hz);
    normalize3(north_hz);
}

/* Compute look direction from azimuth/elevation in local tangent plane.
 * Matches horizon_geometry.c altaz_direction(). */
static void compute_look_dir(const double zenith[3],
                              const double north_hz[3],
                              const double east[3],
                              double az_rad, double el_rad,
                              double out[3])
{
    double ca = cos(el_rad);
    double sa = sin(el_rad);
    double caz = cos(az_rad);
    double saz = sin(az_rad);

    out[0] = sa * zenith[0] + ca * (caz * north_hz[0] + saz * east[0]);
    out[1] = sa * zenith[1] + ca * (caz * north_hz[1] + saz * east[1]);
    out[2] = sa * zenith[2] + ca * (caz * north_hz[2] + saz * east[2]);
}

/* ---- Public API ---- */

horizon_camera_t hcam_create(double lat_deg, double lon_deg, float globe_radius)
{
    horizon_camera_t cam;
    cam.observer_lat_deg = lat_deg;
    cam.observer_lon_deg = lon_deg;
    cam.globe_radius = globe_radius;
    cam.look_azimuth = 0.0f;
    cam.look_elevation = 0.0f;
    cam.fov = (float)(PI / 2.0);
    cam.aspect = 16.0f / 9.0f;
    cam.near_plane = 0.001f;
    cam.far_plane = 100.0f;
    return cam;
}

void hcam_position(const horizon_camera_t *cam, float xyz[3])
{
    double z[3];
    compute_zenith(cam->observer_lat_deg, cam->observer_lon_deg, z);
    double r = (double)cam->globe_radius;
    xyz[0] = (float)(z[0] * r);
    xyz[1] = (float)(z[1] * r);
    xyz[2] = (float)(z[2] * r);
}

void hcam_up_direction(const horizon_camera_t *cam, float up[3])
{
    double z[3];
    compute_zenith(cam->observer_lat_deg, cam->observer_lon_deg, z);
    /* zenith is already unit length */
    up[0] = (float)z[0];
    up[1] = (float)z[1];
    up[2] = (float)z[2];
}

void hcam_look_direction(const horizon_camera_t *cam, float dir[3])
{
    double zenith[3], east[3], north_hz[3];
    compute_zenith(cam->observer_lat_deg, cam->observer_lon_deg, zenith);
    build_basis(zenith, east, north_hz);

    double d[3];
    compute_look_dir(zenith, north_hz, east,
                     (double)cam->look_azimuth, (double)cam->look_elevation,
                     d);
    normalize3(d);
    dir[0] = (float)d[0];
    dir[1] = (float)d[1];
    dir[2] = (float)d[2];
}

mat4_t hcam_view_matrix(const horizon_camera_t *cam)
{
    float pos[3], dir[3], up[3];
    hcam_position(cam, pos);
    hcam_look_direction(cam, dir);
    hcam_up_direction(cam, up);

    vec3_t eye = vec3_create(pos[0], pos[1], pos[2]);
    vec3_t target = vec3_create(pos[0] + dir[0],
                                pos[1] + dir[1],
                                pos[2] + dir[2]);
    vec3_t up_vec = vec3_create(up[0], up[1], up[2]);

    return mat4_look_at(eye, target, up_vec);
}

mat4_t hcam_projection_matrix(const horizon_camera_t *cam)
{
    return mat4_perspective(cam->fov, cam->aspect,
                            cam->near_plane, cam->far_plane);
}

void hcam_rotate(horizon_camera_t *cam, float delta_azimuth, float delta_elevation)
{
    /* Update azimuth and wrap to [0, 2*PI) */
    cam->look_azimuth += delta_azimuth;
    double az = fmod((double)cam->look_azimuth, TWO_PI);
    if (az < 0.0) az += TWO_PI;
    cam->look_azimuth = (float)az;

    /* Update elevation and clamp to [-PI/4, PI/2] */
    cam->look_elevation += delta_elevation;
    float el_min = (float)(-PI / 4.0);
    float el_max = (float)(PI / 2.0);
    if (cam->look_elevation > el_max) cam->look_elevation = el_max;
    if (cam->look_elevation < el_min) cam->look_elevation = el_min;
}
