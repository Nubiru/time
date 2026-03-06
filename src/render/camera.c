#include "camera.h"
#include <math.h>

#define PI 3.14159265358979323846f

/* Clamp elevation to avoid flipping at poles */
#define ELEV_MAX ( PI / 2.0f - 0.01f)
#define ELEV_MIN (-PI / 2.0f + 0.01f)
#define LOG_ZOOM_MIN -4.605f   /* exp(-4.605) ≈ 0.01 */
#define LOG_ZOOM_MAX  10.820f  /* exp(10.82) ≈ 50000 */

camera_t camera_create(float distance, float fov, float aspect) {
    camera_t cam;
    cam.azimuth = 0.0f;
    cam.elevation = 0.3f;  /* slight upward angle */
    cam.log_zoom = logf(distance);
    cam.distance = distance;
    cam.target = vec3_create(0.0f, 0.0f, 0.0f);
    cam.fov = fov;
    cam.aspect = aspect;
    cam.near_plane = 0.01f;
    cam.far_plane = 100000.0f;
    return cam;
}

vec3_t camera_position(const camera_t *cam) {
    /* Spherical to Cartesian:
     * x = d * cos(elev) * sin(az)
     * y = d * sin(elev)
     * z = d * cos(elev) * cos(az) */
    float ce = cosf(cam->elevation);
    float se = sinf(cam->elevation);
    float ca = cosf(cam->azimuth);
    float sa = sinf(cam->azimuth);

    float x = cam->target.x + cam->distance * ce * sa;
    float y = cam->target.y + cam->distance * se;
    float z = cam->target.z + cam->distance * ce * ca;

    return vec3_create(x, y, z);
}

mat4_t camera_view_matrix(const camera_t *cam) {
    vec3_t eye = camera_position(cam);
    vec3_t up = vec3_create(0.0f, 1.0f, 0.0f);
    return mat4_look_at(eye, cam->target, up);
}

mat4_t camera_projection_matrix(const camera_t *cam) {
    return mat4_perspective(cam->fov, cam->aspect, cam->near_plane, cam->far_plane);
}

void camera_rotate(camera_t *cam, float delta_azimuth, float delta_elevation) {
    cam->azimuth += delta_azimuth;
    cam->elevation += delta_elevation;

    /* Clamp elevation */
    if (cam->elevation > ELEV_MAX) cam->elevation = ELEV_MAX;
    if (cam->elevation < ELEV_MIN) cam->elevation = ELEV_MIN;
}

void camera_zoom(camera_t *cam, float delta) {
    /* delta adjusts log_zoom linearly -> distance changes exponentially */
    cam->log_zoom += delta;
    if (cam->log_zoom < LOG_ZOOM_MIN) cam->log_zoom = LOG_ZOOM_MIN;
    if (cam->log_zoom > LOG_ZOOM_MAX) cam->log_zoom = LOG_ZOOM_MAX;
    cam->distance = expf(cam->log_zoom);
}
