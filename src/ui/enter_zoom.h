/* src/ui/enter_zoom.h — "Enter Time" zoom-out choreography
 *
 * Three-phase cinematic sequence:
 *   HOLD    — brief pause on greeting screen
 *   ZOOM    — camera flies out via camera_path_ease with arc
 *   REVEAL  — final settle, UI layers fade in
 *
 * Pure: no GL, no globals, no I/O, no malloc. Returns by value. */

#ifndef TIME_ENTER_ZOOM_H
#define TIME_ENTER_ZOOM_H

#include "../render/camera_path.h"

/* Phase durations */
#define EZ_HOLD_DURATION    0.5f    /* pause on greeting (seconds) */
#define EZ_ZOOM_DURATION    2.0f    /* zoom-out flight (seconds) */
#define EZ_REVEAL_DURATION  0.618f  /* final reveal (phi^-1 seconds) */
#define EZ_ZOOM_ARC_HEIGHT  0.6f    /* cinematic arc at flight midpoint */

/* Start pose: tight close-up (zoomed in, slightly below center) */
#define EZ_START_AZ         0.0f
#define EZ_START_EL         0.1f
#define EZ_START_LOG_ZOOM  -0.5f    /* very close */

/* End pose: solar system overview (standard space view) */
#define EZ_END_AZ           0.0f
#define EZ_END_EL           0.3f
#define EZ_END_LOG_ZOOM     2.0f    /* pulled back to see everything */

typedef enum {
    EZ_IDLE,      /* not started or completed */
    EZ_HOLDING,   /* paused on greeting screen */
    EZ_ZOOMING,   /* camera flying out */
    EZ_REVEALING, /* final settle + layer fade */
    EZ_DONE       /* sequence complete (resets to IDLE on next tick) */
} ez_phase_e;

typedef struct {
    ez_phase_e phase;
    camera_path_t path;       /* zoom-out flight */
    camera_pose_t start;      /* tight close-up pose */
    camera_pose_t destination;/* solar system overview pose */
    float elapsed;            /* total time since start */
    float phase_elapsed;      /* time in current phase */
    float hold_duration;      /* configurable hold time */
    float reveal_progress;    /* 0.0 to 1.0 during REVEALING */
} enter_zoom_t;

/* Create with default timings */
enter_zoom_t ez_create(void);

/* Set custom hold duration (pause before zoom) */
enter_zoom_t ez_set_hold(enter_zoom_t ez, float hold_seconds);

/* Start the sequence */
enter_zoom_t ez_start(enter_zoom_t ez);

/* Advance by dt seconds */
enter_zoom_t ez_tick(enter_zoom_t ez, float dt);

/* Skip to end immediately */
enter_zoom_t ez_skip(enter_zoom_t ez);

/* Current phase */
ez_phase_e ez_phase(enter_zoom_t ez);

/* Current camera pose */
camera_pose_t ez_pose(enter_zoom_t ez);

/* Overall progress 0.0 to 1.0 */
float ez_progress(enter_zoom_t ez);

/* 1 if sequence is active (not IDLE and not after DONE cleared) */
int ez_active(enter_zoom_t ez);

/* Reveal blend: 0.0 during HOLD+ZOOM, ramps 0->1 during REVEALING.
 * Caller uses this to fade in UI layers. */
float ez_reveal_blend(enter_zoom_t ez);

/* 1 if just completed this frame (EZ_DONE phase) */
int ez_just_done(enter_zoom_t ez);

/* Scene brightness: dim stars during HOLD, ramp during ZOOM, full after.
 * Multiplier for 3D scene elements (stars, milkyway, constellations).
 * Returns 1.0 when enter_zoom is inactive. */
float ez_scene_brightness(enter_zoom_t ez);

#endif /* TIME_ENTER_ZOOM_H */
