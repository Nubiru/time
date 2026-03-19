#ifndef TIME_CAMERA_MOTION_H
#define TIME_CAMERA_MOTION_H

/* Inertial camera motion — adds momentum to rotation and zoom.
 * Pure: no GL, no globals, no I/O. Does not modify camera_t directly.
 * Produces per-frame delta values to apply to camera. */

typedef struct {
    /* Rotation inertia */
    float az_velocity;        /* radians/second */
    float el_velocity;        /* radians/second */
    float rotation_friction;  /* velocity multiplier per second (0.05 = 95% decay/sec) */
    int   dragging;           /* 1 while user is actively dragging */

    /* Zoom inertia */
    float zoom_velocity;      /* log_zoom units/second */
    float zoom_friction;      /* velocity multiplier per second */

    /* Per-frame output deltas (to apply to camera) */
    float az_out;             /* azimuth delta this frame */
    float el_out;             /* elevation delta this frame */
    float zoom_out;           /* log_zoom delta this frame */
} camera_motion_t;

/* Create with default friction values */
camera_motion_t camera_motion_create(void);

/* Create with custom friction (0.0 = instant stop, 1.0 = no friction) */
camera_motion_t camera_motion_create_custom(float rotation_friction,
                                             float zoom_friction);

/* During drag: accumulate velocity from drag delta, output direct delta */
camera_motion_t camera_motion_drag(camera_motion_t m,
                                    float delta_az, float delta_el, float dt);

/* Release drag: start coasting with accumulated velocity */
camera_motion_t camera_motion_release(camera_motion_t m);

/* Scroll wheel impulse: add to zoom velocity */
camera_motion_t camera_motion_scroll(camera_motion_t m, float scroll_delta);

/* Per-frame update: decay velocities, compute output deltas.
 * Call EVERY frame. After calling, read az_out/el_out/zoom_out. */
camera_motion_t camera_motion_update(camera_motion_t m, float dt);

/* Check if camera is still moving (any velocity above epsilon) */
int camera_motion_active(camera_motion_t m, float epsilon);

/* Emergency stop — zero all velocities */
camera_motion_t camera_motion_stop(camera_motion_t m);

#endif
