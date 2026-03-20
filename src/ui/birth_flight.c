/* birth_flight.c — Camera choreography for birth moment transition.
 * Pure: no GL, no globals, no I/O, no malloc. */

#include "birth_flight.h"
#include "../math/vec_interp.h"

/* Total duration for progress calculation */
static const float BF_TOTAL_DURATION =
    BF_PREPARE_DURATION + BF_TRAVEL_DURATION + BF_ARRIVE_DURATION;

/* Clamp float to [0, 1] */
static float clamp01(float x)
{
    if (x < 0.0f) { return 0.0f; }
    if (x > 1.0f) { return 1.0f; }
    return x;
}

birth_flight_t bf_create(void)
{
    birth_flight_t bf;
    bf.phase = BF_IDLE;
    bf.home = camera_pose_create(0.0f, 0.0f, 0.0f, vec3_create(0, 0, 0));
    bf.travel_pose = bf.home;
    bf.return_spring = spring_create(0.0f, 0.0f, 0.0f, 0.0f);
    bf.elapsed = 0.0f;
    bf.phase_elapsed = 0.0f;
    bf.dim_blend = 0.0f;
    bf.scrub_progress = 0.0f;
    bf.reveal_blend = 0.0f;
    return bf;
}

birth_flight_t bf_begin(birth_flight_t bf, camera_pose_t current)
{
    bf.home = current;

    /* Travel pose: same as home but zoomed out */
    bf.travel_pose = current;
    bf.travel_pose.log_zoom += BF_TRAVEL_ZOOM_OFFSET;

    bf.phase = BF_PREPARE;
    bf.elapsed = 0.0f;
    bf.phase_elapsed = 0.0f;
    bf.dim_blend = 0.0f;
    bf.scrub_progress = 0.0f;
    bf.reveal_blend = 0.0f;
    bf.return_spring = spring_create(0.0f, 0.0f, 0.0f, 0.0f);
    return bf;
}

birth_flight_t bf_tick(birth_flight_t bf, float dt)
{
    if (dt <= 0.0f) {
        return bf;
    }

    switch (bf.phase) {
    case BF_IDLE:
        /* Nothing to do */
        break;

    case BF_DONE:
        /* One frame in DONE, then reset to IDLE */
        bf.phase = BF_IDLE;
        break;

    case BF_PREPARE: {
        bf.elapsed += dt;
        bf.phase_elapsed += dt;

        float t = clamp01(bf.phase_elapsed / BF_PREPARE_DURATION);
        bf.dim_blend = t;

        if (bf.phase_elapsed >= BF_PREPARE_DURATION) {
            bf.dim_blend = 1.0f;
            bf.phase = BF_TRAVELING;
            bf.phase_elapsed = 0.0f;
        }
        break;
    }

    case BF_TRAVELING: {
        bf.elapsed += dt;
        bf.phase_elapsed += dt;

        bf.dim_blend = 1.0f;
        float t = clamp01(bf.phase_elapsed / BF_TRAVEL_DURATION);
        bf.scrub_progress = t;

        if (bf.phase_elapsed >= BF_TRAVEL_DURATION) {
            bf.scrub_progress = 1.0f;
            bf.phase = BF_ARRIVING;
            bf.phase_elapsed = 0.0f;

            /* Create spring from travel zoom back to home zoom */
            bf.return_spring = spring_create(
                bf.travel_pose.log_zoom,
                bf.home.log_zoom,
                BF_SETTLE_STIFFNESS,
                BF_SETTLE_DAMPING);
        }
        break;
    }

    case BF_ARRIVING: {
        bf.elapsed += dt;
        bf.phase_elapsed += dt;

        float t = clamp01(bf.phase_elapsed / BF_ARRIVE_DURATION);
        bf.dim_blend = 1.0f - t;
        bf.reveal_blend = t;

        /* Spring drives the zoom-back */
        bf.return_spring = spring_update(bf.return_spring, dt);

        if (bf.phase_elapsed >= BF_ARRIVE_DURATION) {
            bf.dim_blend = 0.0f;
            bf.reveal_blend = 1.0f;
            bf.phase = BF_DONE;
        }
        break;
    }
    }

    return bf;
}

birth_flight_t bf_cancel(birth_flight_t bf)
{
    bf.phase = BF_IDLE;
    bf.dim_blend = 0.0f;
    bf.scrub_progress = 0.0f;
    bf.reveal_blend = 0.0f;
    return bf;
}

birth_flight_t bf_skip(birth_flight_t bf)
{
    bf.phase = BF_DONE;
    bf.scrub_progress = 1.0f;
    bf.reveal_blend = 1.0f;
    bf.dim_blend = 0.0f;
    return bf;
}

bf_phase_e bf_phase(birth_flight_t bf)
{
    return bf.phase;
}

camera_pose_t bf_pose(birth_flight_t bf)
{
    switch (bf.phase) {
    case BF_PREPARE: {
        /* Interpolate home -> travel_pose using dim_blend as t */
        camera_pose_t pose;
        pose.azimuth   = lerp_f(bf.home.azimuth,   bf.travel_pose.azimuth,   bf.dim_blend);
        pose.elevation = lerp_f(bf.home.elevation,  bf.travel_pose.elevation, bf.dim_blend);
        pose.log_zoom  = lerp_f(bf.home.log_zoom,   bf.travel_pose.log_zoom,  bf.dim_blend);
        pose.target    = lerp_vec3(bf.home.target,   bf.travel_pose.target,    bf.dim_blend);
        return pose;
    }

    case BF_TRAVELING:
        return bf.travel_pose;

    case BF_ARRIVING: {
        /* Home pose with log_zoom from spring */
        camera_pose_t pose = bf.home;
        pose.log_zoom = bf.return_spring.position;
        return pose;
    }

    case BF_DONE:
        return bf.home;

    case BF_IDLE:
    default:
        return bf.home;
    }
}

float bf_progress(birth_flight_t bf)
{
    if (bf.phase == BF_IDLE) {
        return 0.0f;
    }
    if (bf.phase == BF_DONE) {
        return 1.0f;
    }
    float p = bf.elapsed / BF_TOTAL_DURATION;
    if (p < 0.0f) { p = 0.0f; }
    if (p > 1.0f) { p = 1.0f; }
    return p;
}

int bf_active(birth_flight_t bf)
{
    return bf.phase != BF_IDLE;
}

float bf_dim(birth_flight_t bf)
{
    return bf.dim_blend;
}

float bf_scrub(birth_flight_t bf)
{
    return bf.scrub_progress;
}

float bf_reveal(birth_flight_t bf)
{
    return bf.reveal_blend;
}

int bf_just_done(birth_flight_t bf)
{
    return bf.phase == BF_DONE;
}
