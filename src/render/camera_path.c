#include "camera_path.h"
#include "../math/easing.h"
#include <math.h>
#include <stddef.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static const float SETTLE_EPSILON = 0.001f;

/* Shortest-path unwrap: returns a target angle near 'from' that
 * differs by at most pi (avoids the long way around). */
static float unwrap_angle(float from, float to)
{
    float diff = fmodf(to - from + (float)M_PI, 2.0f * (float)M_PI) - (float)M_PI;
    if (diff < -(float)M_PI) {
        diff += 2.0f * (float)M_PI;
    }
    return from + diff;
}

camera_pose_t camera_pose_create(float azimuth, float elevation,
                                  float log_zoom, vec3_t target)
{
    camera_pose_t p;
    p.azimuth   = azimuth;
    p.elevation = elevation;
    p.log_zoom  = log_zoom;
    p.target    = target;
    return p;
}

camera_path_t camera_path_ease(camera_pose_t from, camera_pose_t to,
                                float duration, float arc_height)
{
    camera_path_t path;
    path.start      = from;
    path.end        = to;
    path.mode       = FLIGHT_EASE;
    path.duration   = duration > 0.0f ? duration : 0.001f;
    path.elapsed    = 0.0f;
    path.arc_height = arc_height;
    path.ease_fn    = ease_in_out_cubic;
    path.active     = 1;

    /* Zero-init spring fields (unused in ease mode) */
    path.az_spring     = spring_create(0.0f, 0.0f, 0.0f, 0.0f);
    path.el_spring     = spring_create(0.0f, 0.0f, 0.0f, 0.0f);
    path.zoom_spring   = spring_create(0.0f, 0.0f, 0.0f, 0.0f);
    path.target_spring = spring_vec3_create(vec3_create(0, 0, 0),
                                             vec3_create(0, 0, 0), 0.0f, 0.0f);
    return path;
}

camera_path_t camera_path_spring(camera_pose_t from, camera_pose_t to,
                                  float stiffness, float damping)
{
    camera_path_t path;
    path.start    = from;
    path.end      = to;
    path.mode     = FLIGHT_SPRING;
    path.duration   = 0.0f;
    path.elapsed    = 0.0f;
    path.arc_height = 0.0f;
    path.ease_fn    = NULL;
    path.active     = 1;

    /* Unwrap azimuth for shortest path */
    float az_target = unwrap_angle(from.azimuth, to.azimuth);

    path.az_spring     = spring_create(from.azimuth, az_target,
                                        stiffness, damping);
    path.el_spring     = spring_create(from.elevation, to.elevation,
                                        stiffness, damping);
    path.zoom_spring   = spring_create(from.log_zoom, to.log_zoom,
                                        stiffness, damping);
    path.target_spring = spring_vec3_create(from.target, to.target,
                                             stiffness, damping);
    return path;
}

camera_path_t camera_path_update(camera_path_t path, float dt)
{
    if (!path.active || dt <= 0.0f) {
        return path;
    }

    if (path.mode == FLIGHT_EASE) {
        path.elapsed += dt;
        if (path.elapsed >= path.duration) {
            path.elapsed = path.duration;
            path.active  = 0;
        }
    } else {
        path.az_spring     = spring_update(path.az_spring, dt);
        path.el_spring     = spring_update(path.el_spring, dt);
        path.zoom_spring   = spring_update(path.zoom_spring, dt);
        path.target_spring = spring_vec3_update(path.target_spring, dt);

        if (spring_settled(path.az_spring, SETTLE_EPSILON)
            && spring_settled(path.el_spring, SETTLE_EPSILON)
            && spring_settled(path.zoom_spring, SETTLE_EPSILON)
            && spring_vec3_settled(path.target_spring, SETTLE_EPSILON)) {
            path.active = 0;
        }
        path.elapsed += dt;
    }

    return path;
}

camera_pose_t camera_path_pose(camera_path_t path)
{
    camera_pose_t pose;

    if (path.mode == FLIGHT_EASE) {
        float t = path.elapsed / path.duration;
        if (t > 1.0f) { t = 1.0f; }

        /* Apply configured easing curve (default: ease_in_out_cubic) */
        ease_fn_t fn = path.ease_fn ? path.ease_fn : ease_in_out_cubic;
        float eased = (float)fn((double)t);

        pose.azimuth   = lerp_angle(path.start.azimuth, path.end.azimuth, eased);
        pose.elevation = lerp_f(path.start.elevation, path.end.elevation, eased);
        pose.log_zoom  = lerp_f(path.start.log_zoom, path.end.log_zoom, eased);
        pose.target    = lerp_vec3(path.start.target, path.end.target, eased);

        /* Arc height: sinusoidal bump peaking at midpoint */
        if (path.arc_height != 0.0f) {
            pose.log_zoom += path.arc_height * sinf((float)M_PI * eased);
        }
    } else {
        pose.azimuth   = path.az_spring.position;
        pose.elevation = path.el_spring.position;
        pose.log_zoom  = path.zoom_spring.position;
        pose.target    = spring_vec3_position(path.target_spring);
    }

    return pose;
}

camera_path_t camera_path_retarget(camera_path_t path, camera_pose_t new_dest)
{
    if (path.mode != FLIGHT_SPRING) {
        return path;
    }

    path.end = new_dest;
    path.active = 1;

    float az_target = unwrap_angle(path.az_spring.position, new_dest.azimuth);
    path.az_spring   = spring_set_target(path.az_spring, az_target);
    path.el_spring   = spring_set_target(path.el_spring, new_dest.elevation);
    path.zoom_spring = spring_set_target(path.zoom_spring, new_dest.log_zoom);
    path.target_spring = spring_vec3_set_target(path.target_spring, new_dest.target);

    return path;
}

int camera_path_done(camera_path_t path)
{
    return !path.active;
}

float camera_path_progress(camera_path_t path)
{
    if (path.mode == FLIGHT_EASE) {
        if (path.duration <= 0.0f) { return 1.0f; }
        float p = path.elapsed / path.duration;
        return p > 1.0f ? 1.0f : p;
    }

    /* Spring mode: approximate progress from position convergence.
     * Average of per-component distance ratios. */
    float az_total = fabsf(path.end.azimuth - path.start.azimuth);
    float el_total = fabsf(path.end.elevation - path.start.elevation);
    float zm_total = fabsf(path.end.log_zoom - path.start.log_zoom);

    float az_remain = fabsf(path.az_spring.position - path.az_spring.target);
    float el_remain = fabsf(path.el_spring.position - path.el_spring.target);
    float zm_remain = fabsf(path.zoom_spring.position - path.zoom_spring.target);

    int count = 0;
    float sum = 0.0f;

    if (az_total > SETTLE_EPSILON) {
        sum += 1.0f - (az_remain / az_total);
        count++;
    }
    if (el_total > SETTLE_EPSILON) {
        sum += 1.0f - (el_remain / el_total);
        count++;
    }
    if (zm_total > SETTLE_EPSILON) {
        sum += 1.0f - (zm_remain / zm_total);
        count++;
    }

    if (count == 0) { return path.active ? 0.0f : 1.0f; }

    float p = sum / (float)count;
    if (p < 0.0f) { p = 0.0f; }
    if (p > 1.0f) { p = 1.0f; }
    return p;
}
