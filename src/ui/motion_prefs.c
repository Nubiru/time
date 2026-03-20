/* motion_prefs.c — global motion preference framework for accessibility
 * Purity: P1 (pure function, no side effects) */

#include "motion_prefs.h"

motion_prefs_t mp_create(void)
{
    motion_prefs_t p;
    p.speed_scale    = MP_DEFAULT_SPEED;
    p.reduced_motion = 0;
    p.min_duration   = MP_DURATION_MIN;
    p.max_duration   = MP_DURATION_MAX;
    return p;
}

motion_prefs_t mp_set_speed(motion_prefs_t p, float scale)
{
    if (scale < MP_SPEED_MIN) {
        scale = MP_SPEED_MIN;
    } else if (scale > MP_SPEED_MAX) {
        scale = MP_SPEED_MAX;
    }
    p.speed_scale = scale;
    return p;
}

motion_prefs_t mp_set_reduced(motion_prefs_t p, int reduced)
{
    p.reduced_motion = !!reduced;
    return p;
}

motion_prefs_t mp_set_bounds(motion_prefs_t p, float min_dur, float max_dur)
{
    if (min_dur < 0.0f) {
        min_dur = 0.0f;
    }
    if (max_dur < min_dur) {
        max_dur = min_dur;
    }
    p.min_duration = min_dur;
    p.max_duration = max_dur;
    return p;
}

float mp_apply_duration(motion_prefs_t p, float base_duration)
{
    if (p.reduced_motion) {
        return 0.0f;
    }
    if (base_duration <= 0.0f) {
        return 0.0f;
    }
    float result = base_duration / p.speed_scale;
    if (result < p.min_duration) {
        result = p.min_duration;
    } else if (result > p.max_duration) {
        result = p.max_duration;
    }
    return result;
}

float mp_apply_stiffness(motion_prefs_t p, float base_stiffness)
{
    return base_stiffness * p.speed_scale * p.speed_scale;
}

float mp_apply_damping(motion_prefs_t p, float base_damping)
{
    return base_damping * p.speed_scale;
}

int mp_should_animate(motion_prefs_t p)
{
    return !p.reduced_motion;
}

float mp_speed(motion_prefs_t p)
{
    return p.speed_scale;
}

float mp_apply_delay(motion_prefs_t p, float base_delay)
{
    if (p.reduced_motion) {
        return 0.0f;
    }
    if (base_delay <= 0.0f) {
        return 0.0f;
    }
    return base_delay / p.speed_scale;
}
