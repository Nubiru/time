/* motion_prefs.h — global motion preference framework for accessibility
 * Purity: P1 (pure function, no side effects) */

#ifndef TIME_MOTION_PREFS_H
#define TIME_MOTION_PREFS_H

/* Default bounds */
#define MP_SPEED_MIN       0.1f
#define MP_SPEED_MAX      10.0f
#define MP_DURATION_MIN    0.05f   /* 50ms floor */
#define MP_DURATION_MAX    5.0f    /* 5s ceiling */
#define MP_DEFAULT_SPEED   1.0f

typedef struct {
    float speed_scale;       /* global animation speed multiplier (1.0 = normal) */
    int   reduced_motion;    /* 1 = user prefers reduced motion */
    float min_duration;      /* minimum animation duration (seconds) */
    float max_duration;      /* maximum animation duration cap (seconds) */
} motion_prefs_t;

/* Create with defaults: speed=1.0, reduced=0, min=0.05, max=5.0 */
motion_prefs_t mp_create(void);

/* Set speed scale (clamped to MP_SPEED_MIN..MP_SPEED_MAX) */
motion_prefs_t mp_set_speed(motion_prefs_t p, float scale);

/* Set reduced motion flag (0 or 1) */
motion_prefs_t mp_set_reduced(motion_prefs_t p, int reduced);

/* Set duration bounds (min_dur must be >= 0, max_dur >= min_dur) */
motion_prefs_t mp_set_bounds(motion_prefs_t p, float min_dur, float max_dur);

/* Apply to duration: base / speed_scale, clamped to [min,max].
 * Returns 0.0f if reduced_motion is active. */
float mp_apply_duration(motion_prefs_t p, float base_duration);

/* Apply to spring stiffness: base * speed_scale^2 (frequency scales linearly with speed) */
float mp_apply_stiffness(motion_prefs_t p, float base_stiffness);

/* Apply to spring damping: base * speed_scale */
float mp_apply_damping(motion_prefs_t p, float base_damping);

/* 1 if animations should play (reduced_motion == 0) */
int mp_should_animate(motion_prefs_t p);

/* Get effective speed scale */
float mp_speed(motion_prefs_t p);

/* Apply to delay: base / speed_scale. Returns 0.0f if reduced. */
float mp_apply_delay(motion_prefs_t p, float base_delay);

#endif /* TIME_MOTION_PREFS_H */
