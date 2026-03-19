#ifndef TIME_FOCUS_FLOW_H
#define TIME_FOCUS_FLOW_H

/* Click-to-Focus orchestration — state machine for the complete
 * focus flow: fly camera → dim background → arrive → reveal detail.
 *
 * Pure: no GL, no globals, no I/O. Returns by value. */

#include "../render/camera_path.h"
#include "../render/click_target.h"
#include "../render/render_layers.h"

/* Focus state machine */
typedef enum {
    FOCUS_IDLE,        /* nothing happening */
    FOCUS_FLYING,      /* camera in transit to target */
    FOCUS_ARRIVED,     /* camera settled, revealing detail */
    FOCUS_LOCKED,      /* fully focused, stable */
    FOCUS_RELEASING    /* returning to home position */
} focus_state_e;

/* Default animation parameters */
#define FOCUS_DIM_TARGET   0.3f   /* background dimmed to 30% opacity */
#define FOCUS_DIM_SPEED    2.5f   /* dim blend rate per second */
#define FOCUS_REVEAL_SPEED 3.0f   /* reveal rate per second */
#define FOCUS_FLY_STIFF    300.0f /* flight spring stiffness */
#define FOCUS_FLY_DAMP     22.0f  /* flight spring damping */
#define FOCUS_RET_STIFF    200.0f /* return spring stiffness */
#define FOCUS_RET_DAMP     24.0f  /* return spring damping */

typedef struct {
    focus_state_e state;
    camera_path_t path;          /* current camera animation */
    camera_pose_t home;          /* pose to return to on release */
    camera_pose_t destination;   /* focus target pose */
    ct_type_t target_type;       /* what kind of object is focused */
    int target_id;               /* which specific object */
    float dim_mix;               /* 0→1 background dimming progress */
    float reveal_mix;            /* 0→1 detail reveal progress */
    float dim_target;            /* opacity for dimmed layers (0.3) */
    float dim_speed;             /* dim blend rate (per second) */
    float reveal_speed;          /* reveal blend rate (per second) */
} focus_flow_t;

/* Create idle focus flow with default parameters. */
focus_flow_t focus_flow_create(void);

/* Begin focus: fly from current pose to destination.
 * Saves current as home (for release). Uses spring flight. */
focus_flow_t focus_flow_begin(focus_flow_t flow,
                              camera_pose_t current,
                              camera_pose_t destination,
                              ct_type_t type, int object_id);

/* Advance one frame. Updates camera path, dim, and reveal. */
focus_flow_t focus_flow_update(focus_flow_t flow, float dt);

/* Release focus: fly back to home pose. */
focus_flow_t focus_flow_release(focus_flow_t flow);

/* Cancel: snap to idle immediately. */
focus_flow_t focus_flow_cancel(focus_flow_t flow);

/* Retarget mid-flight: redirect to new destination. */
focus_flow_t focus_flow_retarget(focus_flow_t flow,
                                 camera_pose_t new_dest,
                                 ct_type_t type, int object_id);

/* Current state. */
focus_state_e focus_flow_state(focus_flow_t flow);

/* Current camera pose (from path, or home/destination if settled). */
camera_pose_t focus_flow_pose(focus_flow_t flow);

/* 1 if any focus action is in progress (not IDLE). */
int focus_flow_active(focus_flow_t flow);

/* Current background dim blend (0.0 = none, 1.0 = fully dimmed). */
float focus_flow_dim(focus_flow_t flow);

/* Current detail reveal blend (0.0 = hidden, 1.0 = fully revealed). */
float focus_flow_reveal(focus_flow_t flow);

/* Convenience: dim all layers except focused_layer and HUD.
 * Uses layer_blend_opacity with current dim_mix. */
layer_state_t focus_flow_apply_layers(focus_flow_t flow,
                                      layer_state_t base,
                                      layer_id_t focused_layer);

#endif
