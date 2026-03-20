/* earth_transition.h — Multi-phase camera choreography for Space/Earth view transition.
 * Pure: no GL, no globals, no I/O, no malloc. Returns by value. */

#ifndef TIME_EARTH_TRANSITION_H
#define TIME_EARTH_TRANSITION_H

#include "../render/camera_path.h"
#include "../math/spring.h"

/* Phase durations (sum ~ phi = 1.618s) */
#define ET_ANTICIPATE_DURATION  0.1f
#define ET_FLY_DURATION         1.218f
#define ET_SETTLE_EPSILON       0.01f

/* Anticipation zoom offset (slight counter-zoom before flight) */
#define ET_ANTICIPATE_ZOOM      0.15f

/* Settle spring parameters */
#define ET_SETTLE_STIFFNESS     350.0f
#define ET_SETTLE_DAMPING       25.0f

/* Arc height for cinematic zoom-out at flight midpoint */
#define ET_FLY_ARC_HEIGHT       0.4f

typedef enum {
    ET_IDLE,        /* no transition */
    ET_ANTICIPATE,  /* brief counter-zoom */
    ET_FLYING,      /* camera in flight */
    ET_SETTLING,    /* spring settle at destination */
    ET_ARRIVED      /* just arrived (resets to IDLE on next tick) */
} et_phase_e;

typedef enum {
    ET_DIR_TO_EARTH,  /* Space -> Earth */
    ET_DIR_TO_SPACE   /* Earth -> Space */
} et_direction_e;

typedef struct {
    et_phase_e phase;
    et_direction_e direction;
    camera_path_t path;         /* camera flight (FLY phase) */
    spring_t settle_az;         /* settle springs for final damping */
    spring_t settle_el;
    spring_t settle_zoom;
    camera_pose_t origin;       /* where transition started */
    camera_pose_t destination;  /* where transition ends */
    float elapsed;              /* total time since begin */
    float phase_elapsed;        /* time in current phase */
    float anticipate_start_zoom; /* original log_zoom at begin */
} earth_transition_t;

/* Canonical camera poses for each view */
camera_pose_t et_space_pose(void);   /* orbital: az=0, el=0.3, log_zoom=2.0, target=origin */
camera_pose_t et_earth_pose(void);   /* surface: az=0, el=0.8, log_zoom=-1.0, target=origin */

/* Create idle transition */
earth_transition_t et_create(void);

/* Begin transition. dir=direction, current=current camera pose.
 * If already active, restarts from current pose. */
earth_transition_t et_begin(earth_transition_t et, et_direction_e dir,
                             camera_pose_t current);

/* Advance one frame by dt seconds */
earth_transition_t et_tick(earth_transition_t et, float dt);

/* Cancel mid-transition. Returns to IDLE immediately. */
earth_transition_t et_cancel(earth_transition_t et);

/* Current phase */
et_phase_e et_phase(earth_transition_t et);

/* Current camera pose (blended during transition, destination when arrived) */
camera_pose_t et_pose(earth_transition_t et);

/* Overall progress 0.0 to 1.0 */
float et_progress(earth_transition_t et);

/* 1 if transition is active (not IDLE) */
int et_active(earth_transition_t et);

/* 1 if just arrived this frame (ET_ARRIVED state) */
int et_just_arrived(earth_transition_t et);

/* Current direction */
et_direction_e et_direction(earth_transition_t et);

#endif /* TIME_EARTH_TRANSITION_H */
