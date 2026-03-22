/* src/ui/enter_zoom.c — "Enter Time" zoom-out choreography */

#include "enter_zoom.h"
#include "../math/easing.h"

static camera_pose_t make_start_pose(void)
{
    return camera_pose_create(EZ_START_AZ, EZ_START_EL,
                              EZ_START_LOG_ZOOM,
                              vec3_create(0.0f, 0.0f, 0.0f));
}

static camera_pose_t make_end_pose(void)
{
    return camera_pose_create(EZ_END_AZ, EZ_END_EL,
                              EZ_END_LOG_ZOOM,
                              vec3_create(0.0f, 0.0f, 0.0f));
}

static float clampf(float v, float lo, float hi)
{
    if (v < lo) { return lo; }
    if (v > hi) { return hi; }
    return v;
}

enter_zoom_t ez_create(void)
{
    enter_zoom_t ez;
    ez.phase           = EZ_IDLE;
    ez.start           = make_start_pose();
    ez.destination     = make_end_pose();
    ez.elapsed         = 0.0f;
    ez.phase_elapsed   = 0.0f;
    ez.hold_duration   = EZ_HOLD_DURATION;
    ez.reveal_progress = 0.0f;

    /* Zero-init path (inactive) */
    ez.path = camera_path_ease(ez.start, ez.destination, 1.0f, 0.0f);
    ez.path.active = 0;

    return ez;
}

enter_zoom_t ez_set_hold(enter_zoom_t ez, float hold_seconds)
{
    ez.hold_duration = hold_seconds > 0.0f ? hold_seconds : 0.0f;
    return ez;
}

enter_zoom_t ez_start(enter_zoom_t ez)
{
    ez.phase           = EZ_HOLDING;
    ez.elapsed         = 0.0f;
    ez.phase_elapsed   = 0.0f;
    ez.reveal_progress = 0.0f;
    return ez;
}

enter_zoom_t ez_tick(enter_zoom_t ez, float dt)
{
    if (dt <= 0.0f) {
        return ez;
    }

    switch (ez.phase) {
    case EZ_IDLE:
        /* no-op */
        break;

    case EZ_HOLDING:
        ez.elapsed       += dt;
        ez.phase_elapsed += dt;
        if (ez.phase_elapsed >= ez.hold_duration) {
            /* Transition to ZOOMING: create the camera flight */
            float overflow = ez.phase_elapsed - ez.hold_duration;
            ez.path = camera_path_ease(ez.start, ez.destination,
                                       EZ_ZOOM_DURATION,
                                       EZ_ZOOM_ARC_HEIGHT);
            ez.path.ease_fn = ease_out_quart;
            ez.phase         = EZ_ZOOMING;
            ez.phase_elapsed = 0.0f;
            /* Apply overflow time to the zoom */
            if (overflow > 0.0f) {
                ez.path          = camera_path_update(ez.path, overflow);
                ez.phase_elapsed = overflow;
            }
        }
        break;

    case EZ_ZOOMING:
        ez.elapsed       += dt;
        ez.phase_elapsed += dt;
        ez.path           = camera_path_update(ez.path, dt);
        if (camera_path_done(ez.path)) {
            /* Transition to REVEALING */
            float overflow = ez.phase_elapsed - EZ_ZOOM_DURATION;
            if (overflow < 0.0f) { overflow = 0.0f; }
            ez.phase           = EZ_REVEALING;
            ez.phase_elapsed   = overflow;
            ez.reveal_progress = clampf(overflow / EZ_REVEAL_DURATION,
                                        0.0f, 1.0f);
        }
        break;

    case EZ_REVEALING:
        ez.elapsed       += dt;
        ez.phase_elapsed += dt;
        ez.reveal_progress = clampf(ez.phase_elapsed / EZ_REVEAL_DURATION,
                                    0.0f, 1.0f);
        if (ez.reveal_progress >= 1.0f) {
            ez.phase           = EZ_DONE;
            ez.reveal_progress = 1.0f;
        }
        break;

    case EZ_DONE:
        /* Next tick resets to IDLE */
        ez.phase = EZ_IDLE;
        break;
    }

    return ez;
}

enter_zoom_t ez_skip(enter_zoom_t ez)
{
    float total = ez.hold_duration + EZ_ZOOM_DURATION + EZ_REVEAL_DURATION;
    ez.phase           = EZ_DONE;
    ez.elapsed         = total;
    ez.phase_elapsed   = EZ_REVEAL_DURATION;
    ez.reveal_progress = 1.0f;

    /* Snap path to end */
    ez.path = camera_path_ease(ez.start, ez.destination,
                               EZ_ZOOM_DURATION, EZ_ZOOM_ARC_HEIGHT);
    ez.path.ease_fn = ease_out_quart;
    ez.path.elapsed = ez.path.duration;
    ez.path.active  = 0;

    return ez;
}

ez_phase_e ez_phase(enter_zoom_t ez)
{
    return ez.phase;
}

camera_pose_t ez_pose(enter_zoom_t ez)
{
    switch (ez.phase) {
    case EZ_IDLE:
    case EZ_HOLDING:
        return ez.start;

    case EZ_ZOOMING:
        return camera_path_pose(ez.path);

    case EZ_REVEALING:
    case EZ_DONE:
        return ez.destination;
    }

    /* Unreachable, but silences compiler warning */
    return ez.start;
}

float ez_progress(enter_zoom_t ez)
{
    float total = ez.hold_duration + EZ_ZOOM_DURATION + EZ_REVEAL_DURATION;
    if (total <= 0.0f) {
        return 1.0f;
    }
    return clampf(ez.elapsed / total, 0.0f, 1.0f);
}

int ez_active(enter_zoom_t ez)
{
    return ez.phase == EZ_HOLDING
        || ez.phase == EZ_ZOOMING
        || ez.phase == EZ_REVEALING;
}

float ez_reveal_blend(enter_zoom_t ez)
{
    switch (ez.phase) {
    case EZ_IDLE:
    case EZ_HOLDING:
    case EZ_ZOOMING:
        return 0.0f;

    case EZ_REVEALING:
        return ez.reveal_progress;

    case EZ_DONE:
        return 1.0f;
    }

    return 0.0f;
}

int ez_just_done(enter_zoom_t ez)
{
    return ez.phase == EZ_DONE ? 1 : 0;
}
