/* convergence_motion.c — animated convergence motion parameters
 *
 * Purity: P1 — no GL, no globals, no I/O, no malloc. */

#include "convergence_motion.h"

#include <math.h>

#define CM_PI 3.14159265358979323846f
#define CM_TWO_PI (2.0f * CM_PI)

/* Phase wrapping threshold to prevent float overflow */
#define CM_PHASE_WRAP 100.0f

static float clamp01(float x)
{
    if (x < 0.0f) { return 0.0f; }
    if (x > 1.0f) { return 1.0f; }
    return x;
}

convergence_motion_t cm_create(void)
{
    convergence_motion_t cm;
    cm.glow_spring   = spring_create(0.0f, 0.0f, CM_GLOW_STIFFNESS, CM_GLOW_DAMPING);
    cm.warmth_spring = spring_create(0.0f, 0.0f, CM_WARMTH_STIFFNESS, CM_WARMTH_DAMPING);
    cm.intensity     = 0.0f;
    cm.warmth_target = 0.0f;
    cm.phase         = 0.0f;
    cm.elapsed       = 0.0f;
    return cm;
}

convergence_motion_t cm_update(convergence_motion_t cm,
                                float intensity, float warmth_target,
                                float dt)
{
    cm.intensity     = intensity;
    cm.warmth_target = warmth_target;

    /* Advance phase */
    cm.phase += (CM_TWO_PI / CM_PULSE_PERIOD) * dt;
    if (cm.phase > CM_PHASE_WRAP) {
        cm.phase = fmodf(cm.phase, CM_TWO_PI);
    }

    /* Update glow spring toward intensity */
    cm.glow_spring = spring_set_target(cm.glow_spring, intensity);
    cm.glow_spring = spring_update(cm.glow_spring, dt);

    /* Update warmth spring toward warmth_target */
    cm.warmth_spring = spring_set_target(cm.warmth_spring, warmth_target);
    cm.warmth_spring = spring_update(cm.warmth_spring, dt);

    cm.elapsed += dt;

    return cm;
}

convergence_motion_t cm_reset(convergence_motion_t cm)
{
    cm.intensity     = 0.0f;
    cm.warmth_target = 0.0f;
    cm.glow_spring   = spring_set_target(cm.glow_spring, 0.0f);
    cm.warmth_spring = spring_set_target(cm.warmth_spring, 0.0f);
    return cm;
}

float cm_pulse(convergence_motion_t cm)
{
    float base = (sinf(cm.phase) + 1.0f) * 0.5f; /* 0..1 */
    return base * cm.intensity;
}

float cm_glow(convergence_motion_t cm)
{
    return clamp01(cm.glow_spring.position);
}

float cm_shimmer(convergence_motion_t cm)
{
    float wave = sinf(cm.phase * 2.0f);
    return 0.5f + CM_SHIMMER_AMPLITUDE * wave * cm.intensity;
}

float cm_warmth(convergence_motion_t cm)
{
    return clamp01(cm.warmth_spring.position);
}

int cm_active(convergence_motion_t cm)
{
    if (cm.intensity > CM_ACTIVATION_THRESHOLD) {
        return 1;
    }
    if (!spring_settled(cm.glow_spring, 0.01f)) {
        return 1;
    }
    if (!spring_settled(cm.warmth_spring, 0.01f)) {
        return 1;
    }
    return 0;
}

float cm_ring_pulse(convergence_motion_t cm, int system_id)
{
    float offset = (float)system_id * 0.618f;
    float base = (sinf(cm.phase + offset) + 1.0f) * 0.5f;
    return base * cm.intensity;
}

float cm_link_shimmer(convergence_motion_t cm, int link_index)
{
    float offset = (float)link_index * 1.047f; /* ~60 degrees */
    float wave = sinf(cm.phase * 2.0f + offset);
    return 0.5f + CM_SHIMMER_AMPLITUDE * wave * cm.intensity;
}
