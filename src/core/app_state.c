#include "app_state.h"
#include "../math/julian.h"

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

    /* Auto-theme: enabled by default */
    state.auto_theme = at_from_sun_elevation(0.0);
    state.auto_theme_enabled = 1;

    /* Toggles */
    state.show_trails = 1; /* on by default */
    state.show_hud = 1;    /* on by default */

    return state;
}
