#include "vec_interp.h"

#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static float clamp01(float t) {
    if (t < 0.0f) {
        return 0.0f;
    }
    if (t > 1.0f) {
        return 1.0f;
    }
    return t;
}

vec3_t lerp_vec3(vec3_t a, vec3_t b, float t) {
    return (vec3_t){
        a.x + (b.x - a.x) * t,
        a.y + (b.y - a.y) * t,
        a.z + (b.z - a.z) * t,
    };
}

vec3_t lerp_vec3_clamped(vec3_t a, vec3_t b, float t) {
    return lerp_vec3(a, b, clamp01(t));
}

vec3_t slerp_vec3(vec3_t a, vec3_t b, float t) {
    vec3_t an = vec3_normalize(a);
    vec3_t bn = vec3_normalize(b);

    float dot = vec3_dot(an, bn);

    /* Clamp dot to avoid NaN from acos */
    if (dot > 1.0f) {
        dot = 1.0f;
    }
    if (dot < -1.0f) {
        dot = -1.0f;
    }

    /* Nearly parallel: fall back to lerp + normalize */
    if (dot > 0.9995f) {
        vec3_t result = lerp_vec3(an, bn, t);
        return vec3_normalize(result);
    }

    /* Nearly anti-parallel: interpolate via perpendicular midpoint */
    if (dot < -0.9995f) {
        /* Pick an arbitrary perpendicular vector */
        vec3_t perp;
        if (fabsf(an.x) < 0.9f) {
            perp = vec3_normalize(vec3_cross(an, (vec3_t){1.0f, 0.0f, 0.0f}));
        } else {
            perp = vec3_normalize(vec3_cross(an, (vec3_t){0.0f, 1.0f, 0.0f}));
        }

        /* First half: slerp from a to perp; second half: perp to b */
        if (t < 0.5f) {
            float omega = (float)M_PI * 0.5f;
            float s0 = sinf((1.0f - 2.0f * t) * omega) / sinf(omega);
            float s1 = sinf(2.0f * t * omega) / sinf(omega);
            return vec3_normalize((vec3_t){
                an.x * s0 + perp.x * s1,
                an.y * s0 + perp.y * s1,
                an.z * s0 + perp.z * s1,
            });
        } else {
            float omega = (float)M_PI * 0.5f;
            float tt = (t - 0.5f) * 2.0f;
            float s0 = sinf((1.0f - tt) * omega) / sinf(omega);
            float s1 = sinf(tt * omega) / sinf(omega);
            return vec3_normalize((vec3_t){
                perp.x * s0 + bn.x * s1,
                perp.y * s0 + bn.y * s1,
                perp.z * s0 + bn.z * s1,
            });
        }
    }

    /* Normal case: standard slerp */
    float omega = acosf(dot);
    float sin_omega = sinf(omega);
    float s0 = sinf((1.0f - t) * omega) / sin_omega;
    float s1 = sinf(t * omega) / sin_omega;

    return (vec3_t){
        an.x * s0 + bn.x * s1,
        an.y * s0 + bn.y * s1,
        an.z * s0 + bn.z * s1,
    };
}

float lerp_f(float a, float b, float t) {
    return a + (b - a) * t;
}

float lerp_f_clamped(float a, float b, float t) {
    return lerp_f(a, b, clamp01(t));
}

float lerp_angle(float a_rad, float b_rad, float t) {
    float diff = b_rad - a_rad;

    /* Normalize difference to [-PI, PI] */
    while (diff > (float)M_PI) {
        diff -= 2.0f * (float)M_PI;
    }
    while (diff < -(float)M_PI) {
        diff += 2.0f * (float)M_PI;
    }

    return a_rad + diff * t;
}

spring_vec3_t spring_vec3_create(vec3_t initial, vec3_t target,
                                 float stiffness, float damping) {
    return (spring_vec3_t){
        .x = spring_create(initial.x, target.x, stiffness, damping),
        .y = spring_create(initial.y, target.y, stiffness, damping),
        .z = spring_create(initial.z, target.z, stiffness, damping),
    };
}

spring_vec3_t spring_vec3_update(spring_vec3_t s, float dt) {
    s.x = spring_update(s.x, dt);
    s.y = spring_update(s.y, dt);
    s.z = spring_update(s.z, dt);
    return s;
}

spring_vec3_t spring_vec3_set_target(spring_vec3_t s, vec3_t target) {
    s.x = spring_set_target(s.x, target.x);
    s.y = spring_set_target(s.y, target.y);
    s.z = spring_set_target(s.z, target.z);
    return s;
}

int spring_vec3_settled(spring_vec3_t s, float epsilon) {
    return spring_settled(s.x, epsilon)
        && spring_settled(s.y, epsilon)
        && spring_settled(s.z, epsilon);
}

vec3_t spring_vec3_position(spring_vec3_t s) {
    return vec3_create(s.x.position, s.y.position, s.z.position);
}

vec3_t spring_vec3_velocity(spring_vec3_t s) {
    return vec3_create(s.x.velocity, s.y.velocity, s.z.velocity);
}
