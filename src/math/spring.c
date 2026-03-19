#include "spring.h"

#include <math.h>

static const float MAX_SUBSTEP = 1.0f / 120.0f;

spring_t spring_create(float initial, float target,
                       float stiffness, float damping)
{
    return (spring_t){
        .position  = initial,
        .velocity  = 0.0f,
        .target    = target,
        .stiffness = stiffness,
        .damping   = damping
    };
}

static spring_t spring_step(spring_t s, float dt)
{
    float force = -s.stiffness * (s.position - s.target)
                  - s.damping * s.velocity;
    s.velocity += force * dt;
    s.position += s.velocity * dt;
    return s;
}

spring_t spring_update(spring_t s, float dt)
{
    if (dt <= 0.0f) {
        return s;
    }

    /* Subdivide for stability */
    while (dt > MAX_SUBSTEP) {
        s   = spring_step(s, MAX_SUBSTEP);
        dt -= MAX_SUBSTEP;
    }
    if (dt > 0.0f) {
        s = spring_step(s, dt);
    }
    return s;
}

spring_t spring_set_target(spring_t s, float target)
{
    s.target = target;
    return s;
}

int spring_settled(spring_t s, float epsilon)
{
    return fabsf(s.position - s.target) < epsilon
        && fabsf(s.velocity) < epsilon;
}

spring_t spring_reset(spring_t s, float position)
{
    s.position = position;
    s.velocity = 0.0f;
    return s;
}
