/* earth_transition.c — Multi-phase camera choreography for Space/Earth view transition.
 * Pure: no GL, no globals, no I/O, no malloc. */

#include "earth_transition.h"
#include "animation.h"
#include <math.h>

/* Total estimated duration (phi) for progress calculation */
static const float ET_TOTAL_DURATION = 1.618f;

camera_pose_t et_space_pose(void)
{
    return camera_pose_create(0.0f, 0.3f, 2.0f, vec3_create(0.0f, 0.0f, 0.0f));
}

camera_pose_t et_earth_pose(void)
{
    return camera_pose_create(0.0f, 0.8f, -1.0f, vec3_create(0.0f, 0.0f, 0.0f));
}

earth_transition_t et_create(void)
{
    earth_transition_t et;
    et.phase = ET_IDLE;
    et.direction = ET_DIR_TO_EARTH;
    et.path = camera_path_ease(
        camera_pose_create(0, 0, 0, vec3_create(0, 0, 0)),
        camera_pose_create(0, 0, 0, vec3_create(0, 0, 0)),
        0.001f, 0.0f);
    et.settle_az = spring_create(0.0f, 0.0f, 0.0f, 0.0f);
    et.settle_el = spring_create(0.0f, 0.0f, 0.0f, 0.0f);
    et.settle_zoom = spring_create(0.0f, 0.0f, 0.0f, 0.0f);
    et.origin = camera_pose_create(0, 0, 0, vec3_create(0, 0, 0));
    et.destination = camera_pose_create(0, 0, 0, vec3_create(0, 0, 0));
    et.elapsed = 0.0f;
    et.phase_elapsed = 0.0f;
    et.anticipate_start_zoom = 0.0f;
    return et;
}

earth_transition_t et_begin(earth_transition_t et, et_direction_e dir,
                             camera_pose_t current)
{
    et.phase = ET_ANTICIPATE;
    et.direction = dir;
    et.origin = current;
    et.anticipate_start_zoom = current.log_zoom;
    et.elapsed = 0.0f;
    et.phase_elapsed = 0.0f;

    if (dir == ET_DIR_TO_EARTH) {
        et.destination = et_earth_pose();
    } else {
        et.destination = et_space_pose();
    }

    return et;
}

/* Compute the counter-zoom offset for anticipation phase.
 * Going TO_EARTH (zooming in) -> briefly zoom OUT (+).
 * Going TO_SPACE (zooming out) -> briefly zoom IN (-). */
static float anticipate_zoom_offset(et_direction_e dir, float eased_t)
{
    float sign = (dir == ET_DIR_TO_EARTH) ? 1.0f : -1.0f;
    return sign * ET_ANTICIPATE_ZOOM * eased_t;
}

earth_transition_t et_tick(earth_transition_t et, float dt)
{
    if (dt <= 0.0f) {
        return et;
    }

    switch (et.phase) {
    case ET_IDLE:
        /* Nothing to do */
        break;

    case ET_ARRIVED:
        /* One tick in ARRIVED, then reset to IDLE */
        et.phase = ET_IDLE;
        break;

    case ET_ANTICIPATE: {
        et.elapsed += dt;
        et.phase_elapsed += dt;

        if (et.phase_elapsed >= ET_ANTICIPATE_DURATION) {
            /* Transition to FLY: create camera_path from counter-zoomed pose to destination */
            float ease_t = 1.0f; /* counter-zoom at full extent */
            float zoom_offset = anticipate_zoom_offset(et.direction, ease_t);
            camera_pose_t from_pose = et.origin;
            from_pose.log_zoom = et.anticipate_start_zoom + zoom_offset;

            et.path = camera_path_ease(from_pose, et.destination,
                                        ET_FLY_DURATION, ET_FLY_ARC_HEIGHT);
            et.phase = ET_FLYING;
            et.phase_elapsed = 0.0f;
        }
        break;
    }

    case ET_FLYING: {
        et.elapsed += dt;
        et.phase_elapsed += dt;

        et.path = camera_path_update(et.path, dt);

        if (camera_path_done(et.path)) {
            /* Transition to SETTLING: create settle springs from flight-end pose */
            camera_pose_t end_pose = camera_path_pose(et.path);

            et.settle_az = spring_create(end_pose.azimuth, et.destination.azimuth,
                                          ET_SETTLE_STIFFNESS, ET_SETTLE_DAMPING);
            et.settle_el = spring_create(end_pose.elevation, et.destination.elevation,
                                          ET_SETTLE_STIFFNESS, ET_SETTLE_DAMPING);
            et.settle_zoom = spring_create(end_pose.log_zoom, et.destination.log_zoom,
                                            ET_SETTLE_STIFFNESS, ET_SETTLE_DAMPING);
            et.phase = ET_SETTLING;
            et.phase_elapsed = 0.0f;
        }
        break;
    }

    case ET_SETTLING: {
        et.elapsed += dt;
        et.phase_elapsed += dt;

        et.settle_az = spring_update(et.settle_az, dt);
        et.settle_el = spring_update(et.settle_el, dt);
        et.settle_zoom = spring_update(et.settle_zoom, dt);

        if (spring_settled(et.settle_az, ET_SETTLE_EPSILON)
            && spring_settled(et.settle_el, ET_SETTLE_EPSILON)
            && spring_settled(et.settle_zoom, ET_SETTLE_EPSILON)) {
            et.phase = ET_ARRIVED;
        }
        break;
    }
    }

    return et;
}

earth_transition_t et_cancel(earth_transition_t et)
{
    et.phase = ET_IDLE;
    return et;
}

et_phase_e et_phase(earth_transition_t et)
{
    return et.phase;
}

camera_pose_t et_pose(earth_transition_t et)
{
    switch (et.phase) {
    case ET_ANTICIPATE: {
        /* Origin with counter-zoom applied via easing ramp */
        float t = (ET_ANTICIPATE_DURATION > 0.0f)
            ? et.phase_elapsed / ET_ANTICIPATE_DURATION
            : 1.0f;
        if (t > 1.0f) { t = 1.0f; }
        float eased_t = anim_apply_easing(EASE_TYPE_INOUT_QUAD, t);
        float zoom_offset = anticipate_zoom_offset(et.direction, eased_t);

        camera_pose_t pose = et.origin;
        pose.log_zoom = et.anticipate_start_zoom + zoom_offset;
        return pose;
    }

    case ET_FLYING:
        return camera_path_pose(et.path);

    case ET_SETTLING:
        return camera_pose_create(
            et.settle_az.position,
            et.settle_el.position,
            et.settle_zoom.position,
            et.destination.target);

    case ET_ARRIVED:
        return et.destination;

    case ET_IDLE:
    default:
        return et.origin;
    }
}

float et_progress(earth_transition_t et)
{
    if (et.phase == ET_IDLE) {
        return 0.0f;
    }
    if (et.phase == ET_ARRIVED) {
        return 1.0f;
    }
    float p = et.elapsed / ET_TOTAL_DURATION;
    if (p < 0.0f) { p = 0.0f; }
    if (p > 1.0f) { p = 1.0f; }
    return p;
}

int et_active(earth_transition_t et)
{
    return et.phase != ET_IDLE;
}

int et_just_arrived(earth_transition_t et)
{
    return et.phase == ET_ARRIVED;
}

et_direction_e et_direction(earth_transition_t et)
{
    return et.direction;
}
