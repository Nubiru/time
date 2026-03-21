#include "app_state.h"
#include "../math/julian.h"
#include "../systems/unified/birth_profile.h"

app_state_t app_state_create(float aspect_ratio) {
    app_state_t state = {0};

    /* Fallback date — overridden by main.c with emscripten_date_now() */
    state.simulation_jd = gregorian_to_jd(2026, 3, 16.5);
    state.time_speed = 1.0;  /* 1 day per real second */
    state.prev_speed = 1.0;

    /* Camera: 25 units away, 45° FOV — sees full solar system (sqrt scale) */
    state.camera = camera_create(25.0f, 0.785f, aspect_ratio);

    /* Observer: Greenwich by default */
    state.observer_lat = 51.4769;  /* Greenwich Observatory */
    state.observer_lon = -0.0005;

    /* Render layers: init default configs */
    for (int i = 0; i < LAYER_COUNT; i++) {
        state.layer_configs[i] = layer_default_config((layer_id_t)i);
    }

    /* View state: Space View, no focus, LOD ULTRA */
    state.view = vs_init();

    /* Performance LOD: target 60fps */
    state.lod = lod_init(60.0f);

    /* Auto-theme: disabled — Cosmos (dark) only until Dawn theme is refined */
    state.auto_theme = at_from_sun_elevation(-10.0); /* force night = Cosmos */
    state.auto_theme_enabled = 0;

    /* Birth sky: default to Jan 1, 2000 — user replaces via birth entry UI */
    {
        birth_profile_t bp = bp_compute(2000, 1, 1);
        state.birth_sky = birth_sky_from_profile(&bp);
        state.birth_sky.active = false; /* not viewing birth by default */
        state.saved_jd = 0.0;
    }

    /* Motion choreography */
    state.enter_zoom = ez_create();
    state.enter_zoom = ez_start(state.enter_zoom);
    state.enter_zoom_active = 1;
    state.earth_trans = et_create();
    state.birth_flight = bf_create();
    state.zoom_depth = zoom_depth_create(state.camera.log_zoom, -1);
    state.motion_prefs = mp_create();
    state.convergence = cm_create();
    state.fx = tfx_create();
    state.cards = cf_create(CF_DEPTH_TODAY);
    state.focus = focus_flow_create();

    /* Toggles */
    state.show_trails = 1; /* on by default */
    state.show_hud = 1;    /* on by default */

    return state;
}
