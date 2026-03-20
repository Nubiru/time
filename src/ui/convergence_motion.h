/* convergence_motion.h — animated convergence motion parameters
 *
 * Converts static convergence intensity into animated motion:
 * pulse (breathing), glow (spring-smoothed), shimmer (fast oscillation),
 * warmth (atmospheric color temperature).
 *
 * Purity: P1 — no GL, no globals, no I/O, no malloc. */

#ifndef TIME_CONVERGENCE_MOTION_H
#define TIME_CONVERGENCE_MOTION_H

#include "../math/spring.h"

/* Pulse rhythm: full cycle period in seconds (slow breathing) */
#define CM_PULSE_PERIOD      3.0f
#define CM_PULSE_MIN         0.0f    /* pulse floor when intensity=0 */
#define CM_PULSE_MAX         1.0f    /* pulse ceiling when intensity=1 */

/* Shimmer: faster oscillation for connection lines */
#define CM_SHIMMER_PERIOD    1.5f
#define CM_SHIMMER_AMPLITUDE 0.3f    /* max shimmer deviation */

/* Spring parameters for smooth transitions */
#define CM_GLOW_STIFFNESS    120.0f  /* gentle, smooth */
#define CM_GLOW_DAMPING      18.0f   /* no bounce */
#define CM_WARMTH_STIFFNESS  80.0f   /* very gentle */
#define CM_WARMTH_DAMPING    14.0f   /* slow, atmospheric */

/* Activation threshold: below this intensity, animations fade out */
#define CM_ACTIVATION_THRESHOLD 0.05f

typedef struct {
    spring_t glow_spring;      /* smoothed glow level */
    spring_t warmth_spring;    /* smoothed atmosphere warmth */
    float intensity;           /* current target intensity (0-1) */
    float warmth_target;       /* current target warmth (0-1) */
    float phase;               /* accumulated phase for pulse/shimmer (radians) */
    float elapsed;             /* total time since creation */
} convergence_motion_t;

/* Create with all animations at rest (no convergence) */
convergence_motion_t cm_create(void);

/* Update each frame.
 * intensity: 0.0 (no convergence) to 1.0 (maximum convergence)
 * warmth_target: 0.0 (cool) to 1.0 (warm)
 * dt: frame delta time in seconds */
convergence_motion_t cm_update(convergence_motion_t cm,
                                float intensity, float warmth_target,
                                float dt);

/* Reset to idle (no convergence) */
convergence_motion_t cm_reset(convergence_motion_t cm);

/* --- Per-frame animated outputs --- */

/* Rhythmic pulse: 0.0 to 1.0, breathing sine wave modulated by intensity.
 * When intensity=0, pulse=0. When intensity=1, full sine oscillation. */
float cm_pulse(convergence_motion_t cm);

/* Smoothed glow level: spring-animated toward intensity target.
 * Use for ring glow, background glow, particle brightness. */
float cm_glow(convergence_motion_t cm);

/* Connection line shimmer: 0.0-1.0, faster oscillation offset.
 * Use as UV scroll or opacity modulation on connection lines. */
float cm_shimmer(convergence_motion_t cm);

/* Atmosphere warmth: spring-animated 0.0 (cool) to 1.0 (warm).
 * Use for background color temperature shift. */
float cm_warmth(convergence_motion_t cm);

/* 1 if convergence animations are active (intensity > threshold OR springs still moving) */
int cm_active(convergence_motion_t cm);

/* Per-ring pulse for a specific system: pulse offset by system_id for visual variety.
 * Each ring pulses slightly out of phase so they don't all breathe together. */
float cm_ring_pulse(convergence_motion_t cm, int system_id);

/* Connection shimmer for a specific link: offset by link_index.
 * Each connection line shimmers at slightly different phase. */
float cm_link_shimmer(convergence_motion_t cm, int link_index);

#endif /* TIME_CONVERGENCE_MOTION_H */
