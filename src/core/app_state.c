#include "app_state.h"
#include "../math/julian.h"

app_state_t app_state_create(float aspect_ratio) {
    app_state_t state = {0};

    /* Start at current date (2026 March 6, noon) */
    state.simulation_jd = gregorian_to_jd(2026, 3, 6.5);
    state.time_speed = 1.0; /* real-time: 1 day per day */

    /* Camera: 25 units away, 45° FOV — sees full solar system (sqrt scale) */
    state.camera = camera_create(25.0f, 0.785f, aspect_ratio);

    /* Observer: Greenwich by default */
    state.observer_lat = 51.4769;  /* Greenwich Observatory */
    state.observer_lon = -0.0005;

    /* Render layers: init default configs */
    for (int i = 0; i < LAYER_COUNT; i++) {
        state.layer_configs[i] = layer_default_config((layer_id_t)i);
    }

    /* Toggles */
    state.show_trails = 1; /* on by default */
    state.show_hud = 1;    /* on by default */

    return state;
}
