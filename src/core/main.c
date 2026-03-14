#include <stdio.h>
#include <math.h>
#include "app_state.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#include <GLES3/gl3.h>
#include "input.h"
#include "hud.h"
#include "../render/gl_init.h"
#include "../render/camera.h"
#include "../render/render_frame.h"
#include "../render/passes/star_pass.h"
#include "../render/passes/planet_pass.h"
#include "../render/passes/zodiac_pass.h"
#include "../render/passes/diffraction_pass.h"
#include "../render/passes/constellation_pass.h"
#include "../render/passes/saturn_pass.h"
#include "../render/passes/milkyway_pass.h"
#include "../render/passes/moon_pass.h"
#include "../render/passes/earth_pass.h"
#include "../render/passes/deep_sky_pass.h"
#include "../render/passes/bodygraph_pass.h"
#include "../render/passes/hexagram_pass.h"
#include "../render/passes/tree_pass.h"
#include "../render/passes/card_pass.h"
#include "../render/passes/post_pass.h"
#endif

#ifdef __EMSCRIPTEN__

/* Single mutable state — the only global in the entire program. */
static app_state_t g_state;

#endif /* __EMSCRIPTEN__ */

void main_loop(void) {
#ifdef __EMSCRIPTEN__
    /* --- Time advancement --- */
    double now_ms = emscripten_get_now();
    double dt_sec = (now_ms - g_state.prev_time_ms) / 1000.0;
    g_state.prev_time_ms = now_ms;
    g_state.simulation_jd += g_state.time_speed * dt_sec;

    /* Advance scale transition (smooth camera zoom between levels) */
    if (g_state.scale_transition.active) {
        g_state.scale_transition = scale_transition_tick(
            g_state.scale_transition, (float)dt_sec);
        float new_log_zoom = scale_transition_value(g_state.scale_transition);
        g_state.camera.log_zoom = new_log_zoom;
        g_state.camera.distance = expf(new_log_zoom);
    }

    /* --- Compute layer visibility from current zoom --- */
    g_state.layer_state = layers_compute(g_state.layer_configs,
                                         g_state.camera.log_zoom);

    /* --- Build per-frame render data --- */
    mat4_t view = camera_view_matrix(&g_state.camera);
    mat4_t proj = camera_projection_matrix(&g_state.camera);
    render_frame_t frame = {
        .simulation_jd = g_state.simulation_jd,
        .time_sec      = (float)(now_ms / 1000.0),
        .view          = view,
        .proj          = proj,
        .view_proj     = mat4_multiply(proj, view),
        .layers        = g_state.layer_state,
        .show_trails   = g_state.show_trails,
        .observer_lat  = g_state.observer_lat,
        .observer_lon  = g_state.observer_lon,
    };

    /* --- Clear + draw (post-process wraps all passes) --- */
    post_pass_begin(&frame);

    star_pass_draw(&frame);
    constellation_pass_draw(&frame);
    deep_sky_pass_draw(&frame);
    milkyway_pass_draw(&frame);
    diffraction_pass_draw(&frame);
    planet_pass_draw(&frame);
    saturn_pass_draw(&frame);
    moon_pass_draw(&frame);
    zodiac_pass_draw(&frame);
    earth_pass_draw(&frame);
    bodygraph_pass_draw(&frame);
    hexagram_pass_draw(&frame);
    tree_pass_draw(&frame);
    card_pass_draw(&frame);

    post_pass_end(&frame);

    /* --- Time HUD overlay --- */
    if (g_state.show_hud &&
        layer_is_visible(g_state.layer_state, LAYER_HUD))
        hud_update(g_state.simulation_jd, g_state.time_speed,
                   g_state.observer_lat, g_state.observer_lon,
                   g_state.camera.log_zoom);
#endif
}

int main(void) {
    printf("Time - A Visual Study\n");

#ifdef __EMSCRIPTEN__
    if (gl_init() != 0) {
        printf("Failed to initialize WebGL\n");
        return 1;
    }

    /* Create state */
    double css_w, css_h;
    emscripten_get_element_css_size("#canvas", &css_w, &css_h);
    g_state = app_state_create((float)(css_w / css_h));

    /* Initialize render passes */
    if (star_pass_init() != 0) return 1;
    if (constellation_pass_init() != 0) return 1;
    if (planet_pass_init() != 0) return 1;
    if (saturn_pass_init() != 0) return 1;
    if (zodiac_pass_init() != 0) return 1;
    if (milkyway_pass_init() != 0) return 1;
    if (diffraction_pass_init() != 0) return 1;
    if (moon_pass_init() != 0) return 1;
    if (earth_pass_init() != 0) return 1;
    if (deep_sky_pass_init() != 0) return 1;
    if (bodygraph_pass_init() != 0) return 1;
    if (hexagram_pass_init() != 0) return 1;
    if (tree_pass_init() != 0) return 1;
    if (card_pass_init() != 0) return 1;
    if (post_pass_init((int)css_w, (int)css_h) != 0) return 1;

    /* Initialize timing */
    g_state.prev_time_ms = emscripten_get_now();

    /* Register input handlers */
    input_register(&g_state);

    printf("Controls: Space=pause, 1-5=speed, -=reverse, T=trails, H=hud, Shift+0-6=scale\n");

    emscripten_set_main_loop(main_loop, 0, 1);
#endif

    return 0;
}
