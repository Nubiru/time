#ifndef TIME_APP_STATE_H
#define TIME_APP_STATE_H

#include "../math/vec3.h"
#include "../math/mat4.h"
#include "../render/camera.h"
#include "../render/camera_scale.h"
#include "../math/spring.h"
#include "../render/render_layers.h"
#include "../render/view_state.h"
#include "../render/render_lod.h"
#include "../ui/auto_theme.h"
#include "../render/birth_sky.h"
#include "../ui/enter_zoom.h"
#include "../ui/zoom_depth.h"
#include "../ui/motion_prefs.h"
#include "../ui/earth_transition.h"
#include "../ui/birth_flight.h"
#include "../ui/convergence_motion.h"
#include "../ui/transition_fx.h"
#include "../ui/card_flight.h"
#include "../ui/focus_flow.h"
#include "../ui/text_reveal.h"
#include "../systems/unified/brain_visual_bridge.h"
#include "../systems/unified/birth_profile.h"

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
    spring_t zoom_spring;         /* scroll/pinch zoom with momentum */
    scale_transition_t scale_transition;

    /* Camera orbit inertia — velocity-decay on drag release */
    float orbit_vel_az;           /* azimuth angular velocity (rad/s) */
    float orbit_vel_el;           /* elevation angular velocity (rad/s) */

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
    int prev_focus_mode;          /* previous focus_mode for edge detection */
    text_reveal_t focus_text;     /* per-word text reveal on focus card appearance */

    /* Performance LOD */
    lod_state_t lod;

    /* Auto-theme (time-of-day Cosmos↔Dawn) */
    auto_theme_state_t auto_theme;
    int auto_theme_enabled;    /* 1=auto, 0=manual */

    /* Birth sky (natal chart view) */
    birth_sky_t birth_sky;   /* pre-computed natal data */
    double saved_jd;         /* real simulation_jd to restore when exiting birth view */

    /* Motion choreography */
    enter_zoom_t enter_zoom;
    int enter_zoom_active;        /* 1 until sequence completes or user skips */
    earth_transition_t earth_trans;
    birth_flight_t birth_flight;
    zoom_depth_t zoom_depth;
    motion_prefs_t motion_prefs;
    convergence_motion_t convergence;
    transition_fx_t fx;
    card_flight_t cards;
    focus_flow_t focus;

    /* Visual toggles */
    int show_trails;       /* orbit trail lines */
    int show_hud;          /* time HUD overlay */
    int meditation_active; /* 1=meditation mode on */

    /* Locale (persisted via localStorage) */
    int locale;                  /* i18n_locale_t value */

    /* Birth profile (entered by user, persisted via localStorage) */
    int birth_year;
    int birth_month;
    int birth_day;
    int birth_entered;           /* 1 if user has entered birth date */
    birth_profile_t birth_profile; /* computed from birth date */

    /* Brain scan cache (refreshed once per simulated day) */
    double headline_jd;          /* floor(jd) of last brain scan */
    char   headline[128];        /* cached daily narrative headline */
    char   wisdom_text[256];     /* cached daily wisdom quote */
    char   wisdom_author[64];    /* cached wisdom quote author */
    char   percentile_text[64];  /* "Day #47 of 365 — top 12.9%" */
    char   red_thread[256];      /* convergence narrative (truncated from 512) */
    char   grand_cycle[128];     /* "This configuration won't recur for X years" */
    char   last_similar[96];     /* "Last similar convergence: X days ago" */
    char   entropy_text[64];     /* "Time entropy: 0.87 — complex" */
    char   top_resonance[96];    /* "Strongest resonance: Hebrew ↔ Islamic" */
    char   author_line[128];     /* "Gabriel — Creator of Time — 283 contributors" */
    float  convergence_strength; /* 0.0-1.0 from brain scan */
    br_visual_frame_t brain_visual; /* cached visual encoding for convergence_pass + ring glow */

    /* Timing */
    double prev_time_ms;
} app_state_t;

/* Initialize with sensible defaults.
 * Does NOT create GPU resources — call after GL init. */
app_state_t app_state_create(float aspect_ratio);

#endif
