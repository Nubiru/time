#ifndef TIME_SPRING_H
#define TIME_SPRING_H

/* Damped harmonic oscillator for physics-based animation.
 * Pure: no GL, no globals, no I/O. Returns by value. */

typedef struct {
    float position;  /* current value */
    float velocity;  /* current velocity (units/second) */
    float target;    /* equilibrium target */
    float stiffness; /* spring constant k (higher = faster) */
    float damping;   /* damping coefficient d (higher = less bounce) */
} spring_t;

/* Presets: usage: spring_create(0.0f, 1.0f, SPRING_GENTLE) */
#define SPRING_GENTLE 170.0f, 26.0f
#define SPRING_SNAPPY 400.0f, 28.0f
#define SPRING_BOUNCY 300.0f, 10.0f

/* Create a spring at initial position, heading toward target */
spring_t spring_create(float initial, float target,
                       float stiffness, float damping);

/* Advance by dt seconds. Uses semi-implicit Euler integration.
 * Subdivides internally if dt > 1/120s for numerical stability. */
spring_t spring_update(spring_t s, float dt);

/* Change target while preserving current position and velocity
 * (interruptible) */
spring_t spring_set_target(spring_t s, float target);

/* True when |position - target| < epsilon AND |velocity| < epsilon */
int spring_settled(spring_t s, float epsilon);

/* Reset to new position with zero velocity */
spring_t spring_reset(spring_t s, float position);

#endif
