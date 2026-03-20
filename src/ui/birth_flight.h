/* birth_flight.h — Camera choreography for birth moment transition.
 * Four-phase sequence: PREPARE -> TRAVEL -> ARRIVE -> DONE.
 * Pure: no GL, no globals, no I/O, no malloc. Returns by value. */

#ifndef TIME_BIRTH_FLIGHT_H
#define TIME_BIRTH_FLIGHT_H

#include "../render/camera_path.h"
#include "../math/spring.h"

/* Phase durations */
#define BF_PREPARE_DURATION   0.3f     /* dim + zoom out */
#define BF_TRAVEL_DURATION    1.0f     /* time scrub */
#define BF_ARRIVE_DURATION    0.618f   /* phi^-1, zoom in + reveal */

/* Camera offsets during travel */
#define BF_TRAVEL_ZOOM_OFFSET 0.3f     /* how much to zoom out during travel */

/* Settle spring parameters */
#define BF_SETTLE_STIFFNESS   300.0f
#define BF_SETTLE_DAMPING     24.0f

typedef enum {
    BF_IDLE,       /* not active */
    BF_PREPARE,    /* dimming + slight zoom out */
    BF_TRAVELING,  /* time scrub in progress */
    BF_ARRIVING,   /* zooming back in + revealing birth sky */
    BF_DONE        /* just completed (resets to IDLE on next tick) */
} bf_phase_e;

typedef struct {
    bf_phase_e phase;
    camera_pose_t home;         /* camera pose before transition */
    camera_pose_t travel_pose;  /* zoomed-out pose during travel */
    spring_t return_spring;     /* spring for zoom-back during ARRIVE */
    float elapsed;              /* total time */
    float phase_elapsed;        /* time in current phase */
    float dim_blend;            /* 0=normal, 1=fully dimmed (for background layers) */
    float scrub_progress;       /* 0=current time, 1=birth time */
    float reveal_blend;         /* 0=today display, 1=birth display fully revealed */
} birth_flight_t;

/* Create idle */
birth_flight_t bf_create(void);

/* Begin transition from current camera pose */
birth_flight_t bf_begin(birth_flight_t bf, camera_pose_t current);

/* Advance by dt */
birth_flight_t bf_tick(birth_flight_t bf, float dt);

/* Cancel and return to home */
birth_flight_t bf_cancel(birth_flight_t bf);

/* Skip to end (instant) */
birth_flight_t bf_skip(birth_flight_t bf);

/* Current phase */
bf_phase_e bf_phase(birth_flight_t bf);

/* Current camera pose */
camera_pose_t bf_pose(birth_flight_t bf);

/* Overall progress 0-1 */
float bf_progress(birth_flight_t bf);

/* Is active (not IDLE) */
int bf_active(birth_flight_t bf);

/* Background dim: 0=normal, ramps to 1 during PREPARE, stays 1 during TRAVEL,
 * ramps back to 0 during ARRIVE */
float bf_dim(birth_flight_t bf);

/* Time scrub progress: 0=current time, ramps 0->1 during TRAVEL.
 * Caller uses this to interpolate Julian Day or reconfigure rings. */
float bf_scrub(birth_flight_t bf);

/* Birth data reveal: 0=today, ramps 0->1 during ARRIVE.
 * Caller uses this to crossfade card content. */
float bf_reveal(birth_flight_t bf);

/* 1 if just completed this frame */
int bf_just_done(birth_flight_t bf);

#endif /* TIME_BIRTH_FLIGHT_H */
