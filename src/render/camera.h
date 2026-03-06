#ifndef TIME_CAMERA_H
#define TIME_CAMERA_H

#include "../math/vec3.h"
#include "../math/mat4.h"

/* Orbital camera that rotates around a target point.
 * Uses spherical coordinates (azimuth, elevation, distance).
 * Distance is controlled via log_zoom for smooth multi-scale navigation:
 *   distance = exp(log_zoom) */
typedef struct {
    float azimuth;     /* horizontal angle (radians), 0 = +Z axis */
    float elevation;   /* vertical angle (radians), clamped to avoid gimbal lock */
    float distance;    /* distance from target (derived from log_zoom) */
    float log_zoom;    /* logarithmic zoom level: distance = exp(log_zoom) */
    vec3_t target;     /* point camera orbits around */
    float fov;         /* field of view (radians) */
    float aspect;      /* viewport width / height */
    float near_plane;
    float far_plane;
} camera_t;

camera_t camera_create(float distance, float fov, float aspect);
vec3_t   camera_position(const camera_t *cam);
mat4_t   camera_view_matrix(const camera_t *cam);
mat4_t   camera_projection_matrix(const camera_t *cam);

/* Rotate camera by delta angles (from mouse drag). */
void camera_rotate(camera_t *cam, float delta_azimuth, float delta_elevation);

/* Zoom camera (change distance). */
void camera_zoom(camera_t *cam, float delta);

#endif
