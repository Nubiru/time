#ifndef TIME_UI_STATE_H
#define TIME_UI_STATE_H

/* UI state machine for panel visibility, edge proximity, and animation.
 * Pure module: no GL, no malloc, no globals, no side effects.
 * All functions take state by value and return new state by value. */

/* UI modes -- mutually exclusive panel states */
typedef enum {
    UI_MODE_DEFAULT = 0,     /* No panel open -- artwork only */
    UI_MODE_HELP,            /* Help overlay visible (? key) */
    UI_MODE_PALETTE,         /* Command palette visible (/ key) */
    UI_MODE_LAYER_PANEL,     /* Layer toggle panel visible */
    UI_MODE_SETTINGS,        /* Settings panel visible (, key) */
    UI_MODE_COUNT
} ui_mode_t;

/* Edge proximity zones -- which screen edge is being approached */
typedef enum {
    UI_EDGE_NONE = 0,
    UI_EDGE_BOTTOM,          /* Time control bar */
    UI_EDGE_LEFT,            /* Layer panel */
    UI_EDGE_RIGHT,           /* Detail panel (future) */
    UI_EDGE_COUNT
} ui_edge_t;

/* Complete UI state -- pass by value, return by value (functional style) */
typedef struct {
    /* Mode state */
    ui_mode_t mode;           /* Currently active mode */
    ui_mode_t prev_mode;      /* Previous mode (for transition direction) */
    float mode_progress;      /* 0.0=fully closed, 1.0=fully open */
    int mode_opening;         /* 1=transitioning open, 0=transitioning closed */

    /* Edge proximity state */
    float bottom_proximity;   /* 0.0=far, 1.0=at edge */
    float left_proximity;     /* 0.0=far, 1.0=at edge */
    float right_proximity;    /* 0.0=far, 1.0=at edge */

    /* Onboarding hint */
    float hint_opacity;       /* "Press ? for controls" -- fades from 1.0 to 0.0 */
    float hint_timer;         /* Seconds since first display */

    /* Scene dimming */
    float scene_dim;          /* 0.0=full brightness, 1.0=fully dimmed */
} ui_state_t;

/* Animation timing configuration (phi-derived values) */
typedef struct {
    float panel_duration;     /* How long panel open/close takes (seconds) */
    float hint_duration;      /* How long onboarding hint stays visible */
    float hint_fade;          /* How long hint fade-out takes */
    float proximity_threshold; /* Distance from edge to trigger (0.0-1.0 of screen) */
    float dim_amount;         /* How much to dim scene when panel open (0.0-1.0) */
} ui_timing_t;

/* Initialize a default UI state (all panels closed, hint visible) */
ui_state_t ui_state_init(void);

/* Get default timing configuration (phi-derived values) */
ui_timing_t ui_timing_default(void);

/* --- Mode Transitions --- */

/* Toggle a mode on/off. If the mode is already active, close it.
 * If a different mode is active, switch to the new one. */
ui_state_t ui_state_toggle_mode(ui_state_t state, ui_mode_t mode);

/* Close whatever mode is currently open (return to DEFAULT) */
ui_state_t ui_state_close(ui_state_t state);

/* Check if a specific mode is active */
int ui_state_is_mode(ui_state_t state, ui_mode_t mode);

/* --- Time Update --- */

/* Advance all animations by dt seconds.
 * Updates mode_progress, hint_opacity, scene_dim.
 * timing: animation parameters. */
ui_state_t ui_state_update(ui_state_t state, float dt, ui_timing_t timing);

/* --- Edge Proximity --- */

/* Update proximity values based on mouse position.
 * mouse_x, mouse_y: pixel coordinates.
 * screen_w, screen_h: viewport dimensions.
 * timing: contains proximity_threshold. */
ui_state_t ui_state_update_mouse(ui_state_t state, float mouse_x, float mouse_y,
                                  float screen_w, float screen_h, ui_timing_t timing);

/* --- Query Computed Values --- */

/* Get effective opacity for a panel (combines mode_progress + mode match).
 * Returns 0.0 if this mode isn't active, mode_progress if it is. */
float ui_state_panel_opacity(ui_state_t state, ui_mode_t mode);

/* Get effective opacity for the time control bar (edge-proximity driven).
 * Returns bottom_proximity value. */
float ui_state_time_bar_opacity(ui_state_t state);

/* Get effective opacity for the layer panel (edge-proximity OR mode-driven).
 * Returns max(left_proximity, mode==LAYER_PANEL ? mode_progress : 0). */
float ui_state_layer_panel_opacity(ui_state_t state);

/* Get scene dim factor (0.0=normal, dim_amount when panel open).
 * Returns 1.0 - scene_dim (brightness multiplier). */
float ui_state_scene_brightness(ui_state_t state, ui_timing_t timing);

/* Get the name string for a UI mode. Returns static string. */
const char *ui_mode_name(ui_mode_t mode);

/* Number of modes (equals UI_MODE_COUNT). */
int ui_mode_count(void);

#endif /* TIME_UI_STATE_H */
