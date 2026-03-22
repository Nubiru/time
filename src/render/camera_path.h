#ifndef TIME_CAMERA_PATH_H
#define TIME_CAMERA_PATH_H

/* Smooth camera flight between orbital positions.
 * Two modes: eased (fixed duration, cinematic) or spring (interruptible).
 * Pure: no GL, no globals, no I/O. Returns by value. */

#include "../math/vec3.h"
#include "../math/spring.h"
#include "../math/vec_interp.h"
#include "../math/easing.h"

/* Snapshot of orbital camera state */
typedef struct {
    float azimuth;     /* radians */
    float elevation;   /* radians */
    float log_zoom;    /* log(distance) */
    vec3_t target;     /* orbit center */
} camera_pose_t;

/* Flight timing mode */
typedef enum {
    FLIGHT_EASE,       /* eased interpolation, fixed duration */
    FLIGHT_SPRING      /* spring-based, natural duration, interruptible */
} flight_mode_e;

/* Camera flight state */
typedef struct {
    camera_pose_t start;
    camera_pose_t end;
    flight_mode_e mode;

    /* Ease mode */
    float duration;    /* total seconds */
    float elapsed;     /* current time */
    float arc_height;  /* extra log_zoom at midpoint (0 = direct path) */
    ease_fn_t ease_fn; /* easing curve (NULL = ease_in_out_cubic) */

    /* Spring mode */
    spring_t az_spring;
    spring_t el_spring;
    spring_t zoom_spring;
    spring_vec3_t target_spring;

    int active;        /* 1 while flight in progress */
} camera_path_t;

/* Create pose from components */
camera_pose_t camera_pose_create(float azimuth, float elevation,
                                  float log_zoom, vec3_t target);

/* Eased flight: smooth in-out, fixed duration.
 * arc_height > 0 adds a cinematic zoom-out at midpoint. */
camera_path_t camera_path_ease(camera_pose_t from, camera_pose_t to,
                                float duration, float arc_height);

/* Spring flight: physics-based, interruptible. */
camera_path_t camera_path_spring(camera_pose_t from, camera_pose_t to,
                                  float stiffness, float damping);

/* Advance by dt seconds */
camera_path_t camera_path_update(camera_path_t path, float dt);

/* Current interpolated pose */
camera_pose_t camera_path_pose(camera_path_t path);

/* Retarget mid-flight (spring mode only, preserves momentum) */
camera_path_t camera_path_retarget(camera_path_t path, camera_pose_t new_dest);

/* 1 when flight complete */
int camera_path_done(camera_path_t path);

/* Progress 0.0-1.0 (ease: elapsed/duration, spring: approximate) */
float camera_path_progress(camera_path_t path);

#endif
