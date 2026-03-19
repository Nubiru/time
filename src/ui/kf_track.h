#ifndef TIME_KF_TRACK_H
#define TIME_KF_TRACK_H

#include "animation.h"

#define KF_MAX_KEYS 32

/* A keyframe: at `time` seconds, the value should be `value`.
 * `easing` controls how we ease INTO this key from the previous one.
 * The first keyframe's easing is ignored (it's the starting point). */
typedef struct {
    float time;         /* seconds from track start */
    float value;        /* target value at this time */
    ease_type_e easing; /* easing function for segment ending at this key */
} kf_key_t;

typedef struct {
    kf_key_t keys[KF_MAX_KEYS];
    int count;
    float elapsed;
    int started;
    int done;
} kf_track_t;

/* Create empty track */
kf_track_t kf_track_create(void);

/* Add keyframe. Keys MUST be added in chronological order (ascending time).
 * If count >= KF_MAX_KEYS, the key is ignored. */
kf_track_t kf_track_add(kf_track_t tr, float time, float value, ease_type_e easing);

/* Start the track (begins ticking from elapsed=0) */
kf_track_t kf_track_start(kf_track_t tr);

/* Advance by dt seconds */
kf_track_t kf_track_tick(kf_track_t tr, float dt);

/* Current interpolated value.
 * Before first key time: returns first key's value.
 * After last key time: returns last key's value.
 * Between keys: interpolated with segment easing. */
float kf_track_value(kf_track_t tr);

/* Duration: time of last keyframe (0 if empty) */
float kf_track_duration(kf_track_t tr);

/* Is the track complete? (elapsed >= last key time) */
int kf_track_done(kf_track_t tr);

/* Overall progress 0.0 to 1.0 */
float kf_track_progress(kf_track_t tr);

/* Which segment is active (0 = before/at first key, count-2 = last segment).
 * Returns -1 if not started or empty. */
int kf_track_segment(kf_track_t tr);

#endif
