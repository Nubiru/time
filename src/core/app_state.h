#ifndef TIME_APP_STATE_H
#define TIME_APP_STATE_H

#include "../math/vec3.h"
#include "../math/mat4.h"
#include "../render/camera.h"
#include "../render/camera_scale.h"
#include "../render/render_layers.h"
#include "../render/view_state.h"
#include "../render/render_lod.h"
#include "../ui/auto_theme.h"
#include "../render/birth_sky.h"

/* All mutable application state in one place.
 * Pure functions never touch this — they receive data and return data.
 * Only main.c and input handlers mutate this struct.
 *
 * GL-free: GPU resource handles live in render pass modules
 * (src/render/passes/) as module-static s_ variables. */
typedef struct {
    /* Time */
    double simulation_jd;    /* current Julian Day in simulation */
    double time_speed;       /* days per real second */
    double prev_speed;       /* speed before last pause (for resume) */

    /* Camera */
    camera_t camera;
    scale_transition_t scale_transition;

    /* Mouse */
    int mouse_down;
    double mouse_x;
    double mouse_y;

    /* Observer location */
    double observer_lat;     /* degrees, positive north */
    double observer_lon;     /* degrees, positive east */

    /* Render layers */
    layer_config_t layer_configs[LAYER_COUNT];
    layer_state_t layer_state;

    /* View state (view mode, focus, transitions) */
    view_state_t view;

    /* Performance LOD */
    lod_state_t lod;

    /* Auto-theme (time-of-day Cosmos↔Dawn) */
    auto_theme_state_t auto_theme;
    int auto_theme_enabled;    /* 1=auto, 0=manual */

    /* Birth sky (natal chart view) */
    birth_sky_t birth_sky;   /* pre-computed natal data */
    double saved_jd;         /* real simulation_jd to restore when exiting birth view */

    /* Visual toggles */
    int show_trails;       /* orbit trail lines */
    int show_hud;          /* time HUD overlay */
    int meditation_active; /* 1=meditation mode on */

    /* Timing */
    double prev_time_ms;
} app_state_t;

/* Initialize with sensible defaults.
 * Does NOT create GPU resources — call after GL init. */
app_state_t app_state_create(float aspect_ratio);

#endif
