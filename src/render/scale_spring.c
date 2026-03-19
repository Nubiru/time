#include "scale_spring.h"

static const float SETTLE_EPSILON   = 0.001f;
static const float DEFAULT_ANTIC_FRAC = 0.05f;
static const float DEFAULT_ANTIC_DUR  = 0.1f;

scale_spring_t scale_spring_create(float current_log_zoom)
{
    scale_spring_t ss;
    ss.spring = spring_create(current_log_zoom, current_log_zoom,
                              SPRING_SNAPPY);
    ss.phase                = SS_IDLE;
    ss.anticipation_timer   = 0.0f;
    ss.anticipation_amount  = 0.0f;
    ss.anticipation_duration = 0.0f;
    ss.final_target         = current_log_zoom;
    ss.target_scale         = SCALE_SOLAR;
    return ss;
}

scale_spring_t scale_spring_start(scale_spring_t ss,
                                  float current_log_zoom,
                                  scale_id_t target,
                                  float anticipation_frac,
                                  float anticipation_dur)
{
    float dest     = scale_center_log_zoom(target);
    float distance = dest - current_log_zoom;

    ss.target_scale = target;
    ss.final_target = dest;

    if (anticipation_frac > 0.0f && anticipation_dur > 0.0f) {
        /* Anticipation: move opposite to the intended direction */
        ss.anticipation_amount   = distance * anticipation_frac;
        ss.anticipation_duration = anticipation_dur;
        ss.anticipation_timer    = anticipation_dur;
        ss.phase = SS_ANTICIPATE;

        /* Spring targets the counter-move position */
        float counter_target = current_log_zoom - ss.anticipation_amount;
        ss.spring = spring_create(current_log_zoom, counter_target,
                                  SPRING_SNAPPY);
    } else {
        /* Skip anticipation, go straight to spring phase */
        ss.anticipation_amount   = 0.0f;
        ss.anticipation_duration = 0.0f;
        ss.anticipation_timer    = 0.0f;
        ss.phase = SS_SPRING;

        ss.spring = spring_create(current_log_zoom, dest, SPRING_SNAPPY);
    }

    return ss;
}

scale_spring_t scale_spring_start_default(scale_spring_t ss,
                                          float current_log_zoom,
                                          scale_id_t target)
{
    return scale_spring_start(ss, current_log_zoom, target,
                              DEFAULT_ANTIC_FRAC, DEFAULT_ANTIC_DUR);
}

scale_spring_t scale_spring_tick(scale_spring_t ss, float dt)
{
    if (dt <= 0.0f) {
        return ss;
    }

    switch (ss.phase) {
    case SS_IDLE:
    case SS_SETTLED:
        /* Nothing to do */
        break;

    case SS_ANTICIPATE:
        ss.spring = spring_update(ss.spring, dt);
        ss.anticipation_timer -= dt;
        if (ss.anticipation_timer <= 0.0f) {
            /* Transition to main spring: retarget to final destination,
             * preserving current position and velocity for smooth handoff */
            ss.spring = spring_set_target(ss.spring, ss.final_target);
            ss.phase  = SS_SPRING;
        }
        break;

    case SS_SPRING:
        ss.spring = spring_update(ss.spring, dt);
        if (spring_settled(ss.spring, SETTLE_EPSILON)) {
            ss.spring.position = ss.final_target;
            ss.spring.velocity = 0.0f;
            ss.phase = SS_SETTLED;
        }
        break;
    }

    return ss;
}

float scale_spring_value(scale_spring_t ss)
{
    return ss.spring.position;
}

int scale_spring_done(scale_spring_t ss)
{
    return ss.phase == SS_SETTLED || ss.phase == SS_IDLE;
}

int scale_spring_active(scale_spring_t ss)
{
    return ss.phase == SS_ANTICIPATE || ss.phase == SS_SPRING;
}

scale_spring_phase_e scale_spring_phase(scale_spring_t ss)
{
    return ss.phase;
}

scale_spring_t scale_spring_cancel(scale_spring_t ss)
{
    /* Freeze at current position */
    float current = ss.spring.position;
    ss.spring = spring_create(current, current, SPRING_SNAPPY);
    ss.phase  = SS_IDLE;
    return ss;
}
