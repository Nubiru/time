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
#include "../render/passes/orbit_trail_pass.h"
#include "../render/passes/card_pass.h"
#include "../render/passes/text_pass.h"
#include "../render/passes/ring_pass.h"
#include "../render/passes/convergence_pass.h"
#include "../render/passes/post_pass.h"
#include "../render/earth_view_frame.h"
#include "../ui/ui_bridge.h"
#include "../ui/view_registry.h"
#include "../ui/audio_score.h"
#include "../ui/audio_engine.h"
#include "../ui/audio_meditation.h"
#include "../systems/unified/brain_scan.h"
#include "../systems/unified/brain_narrative.h"
#include <string.h>
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
    /* In birth sky mode, hold time at birth JD (don't advance) */
    if (!g_state.birth_sky.active)
        g_state.simulation_jd += g_state.time_speed * dt_sec;

    /* Advance scale transition (smooth camera zoom between levels) */
    if (g_state.scale_transition.active) {
        g_state.scale_transition = scale_transition_tick(
            g_state.scale_transition, (float)dt_sec);
        float new_log_zoom = scale_transition_value(g_state.scale_transition);
        g_state.camera.log_zoom = new_log_zoom;
        g_state.camera.distance = expf(new_log_zoom);
    }

    /* --- Brain scan: refresh once per simulated day --- */
    {
        double day_now = floor(g_state.simulation_jd);
        if (day_now != g_state.headline_jd) {
            g_state.headline_jd = day_now;
            br_result_t br;
            br_scan(g_state.simulation_jd, &br);
            g_state.convergence_strength = (float)br.convergence_strength;
            br_narrative_t story;
            if (br_narrative_compose(&br, &story)) {
                memcpy(g_state.headline, story.headline,
                       sizeof(g_state.headline));
            } else {
                g_state.headline[0] = '\0';
            }
        }
    }

    /* --- Motion choreographies: tick all, apply highest-priority pose --- */
    g_state.enter_zoom  = ez_tick(g_state.enter_zoom, (float)dt_sec);
    g_state.earth_trans = et_tick(g_state.earth_trans, (float)dt_sec);
    g_state.birth_flight = bf_tick(g_state.birth_flight, (float)dt_sec);

    {
        camera_pose_t motion_pose;
        int motion_active = 0;

        if (g_state.enter_zoom_active && ez_active(g_state.enter_zoom)) {
            motion_pose = ez_pose(g_state.enter_zoom);
            motion_active = 1;
            if (ez_just_done(g_state.enter_zoom))
                g_state.enter_zoom_active = 0;
        } else if (et_active(g_state.earth_trans)) {
            motion_pose = et_pose(g_state.earth_trans);
            motion_active = 1;
        } else if (bf_active(g_state.birth_flight)) {
            motion_pose = bf_pose(g_state.birth_flight);
            motion_active = 1;
        }

        if (motion_active) {
            g_state.camera.azimuth   = motion_pose.azimuth;
            g_state.camera.elevation = motion_pose.elevation;
            g_state.camera.log_zoom  = motion_pose.log_zoom;
            g_state.camera.distance  = expf(motion_pose.log_zoom);
            g_state.camera.target    = motion_pose.target;
        }
    }

    /* --- Zoom-depth: track camera zoom → depth tier mapping --- */
    g_state.zoom_depth = zoom_depth_update(g_state.zoom_depth,
                                            g_state.camera.log_zoom,
                                            (float)dt_sec);

    /* --- Convergence motion: animate from brain scan intensity --- */
    g_state.convergence = cm_update(g_state.convergence,
                                     g_state.convergence_strength, 0.0f,
                                     (float)dt_sec);

    /* --- Card flight: spring-animated depth navigation --- */
    g_state.cards = cf_tick(g_state.cards, (float)dt_sec);

    /* --- Focus flow: click-to-focus camera (idle until triggered) --- */
    g_state.focus = focus_flow_update(g_state.focus, (float)dt_sec);

    /* --- Transition FX: drive post-processing from active choreography --- */
    if (g_state.enter_zoom_active && ez_active(g_state.enter_zoom)) {
        g_state.fx = tfx_activate(g_state.fx, TFX_ENTER_ZOOM, 1.0f);
        g_state.fx = tfx_set_progress(g_state.fx, ez_progress(g_state.enter_zoom));
    } else if (et_active(g_state.earth_trans)) {
        g_state.fx = tfx_activate(g_state.fx, TFX_EARTH_VIEW, 1.0f);
        g_state.fx = tfx_set_progress(g_state.fx, et_progress(g_state.earth_trans));
    } else if (bf_active(g_state.birth_flight)) {
        g_state.fx = tfx_activate(g_state.fx, TFX_BIRTH_TRAVEL, 1.0f);
        g_state.fx = tfx_set_progress(g_state.fx, bf_progress(g_state.birth_flight));
    } else if (cm_active(g_state.convergence)) {
        g_state.fx = tfx_activate(g_state.fx, TFX_CONVERGENCE,
                                   cm_glow(g_state.convergence));
    } else {
        g_state.fx = tfx_deactivate(g_state.fx);
    }
    g_state.fx = tfx_tick(g_state.fx, (float)dt_sec);

    /* --- Performance LOD: record frame time, adapt quality --- */
    g_state.lod = lod_record_frame(g_state.lod, (float)(dt_sec * 1000.0));
    g_state.view = vs_set_lod(g_state.view, (int)lod_current_tier(&g_state.lod));

    /* --- Auto-theme: sun elevation → Cosmos/Dawn blend --- */
    if (g_state.auto_theme_enabled) {
        auto_theme_state_t target = at_from_location(
            g_state.simulation_jd, g_state.observer_lat, g_state.observer_lon);
        g_state.auto_theme = at_smooth(g_state.auto_theme, target,
                                        (float)dt_sec, 2.0f);
        /* Export key theme colors to DOM as CSS custom properties */
        theme_t theme = at_blended_theme(g_state.auto_theme);
        EM_ASM({
            var s = document.documentElement.style;
            s.setProperty("--bg-space",
                $0.toFixed(3) + ", " + $1.toFixed(3) + ", " + $2.toFixed(3));
            s.setProperty("--text-primary",
                $3.toFixed(3) + ", " + $4.toFixed(3) + ", " + $5.toFixed(3));
        }, theme.bg_space.r, theme.bg_space.g, theme.bg_space.b,
           theme.text_primary.r, theme.text_primary.g, theme.text_primary.b);
    }

    /* --- Advance view state (transitions, focus) --- */
    g_state.view = vs_tick(g_state.view, (float)dt_sec);

    /* --- Compute layer visibility from current zoom --- */
    g_state.layer_state = layers_compute(g_state.layer_configs,
                                         g_state.camera.log_zoom);

    /* --- Build per-frame render data --- */
    mat4_t view, proj;
    int effective_view = vs_effective_view(&g_state.view);
    if (effective_view == 1) { /* VIEW_EARTH: ground-level horizon camera */
        ev_frame_t ev = ev_compute_simple(
            g_state.simulation_jd,
            g_state.observer_lat, g_state.observer_lon,
            g_state.camera.azimuth, g_state.camera.elevation);
        view = ev.view_matrix;
        proj = ev.proj_matrix;
    } else { /* Space View (and others): orbital camera */
        view = camera_view_matrix(&g_state.camera);
        proj = camera_projection_matrix(&g_state.camera);
    }
    render_frame_t frame = {
        .simulation_jd = g_state.simulation_jd,
        .time_sec      = (float)(now_ms / 1000.0),
        .view          = view,
        .proj          = proj,
        .view_proj     = mat4_multiply(proj, view),
        .layers        = g_state.layer_state,
        .show_trails   = g_state.show_trails,
        .log_zoom      = g_state.camera.log_zoom,
        .observer_lat  = g_state.observer_lat,
        .observer_lon  = g_state.observer_lon,
        .theme_id      = (int)g_state.auto_theme.active_theme,
        .focus_mode    = g_state.view.focus_mode,
    };
    memcpy(frame.headline, g_state.headline, sizeof(frame.headline));

    /* --- Get pass schedule from view state --- */
    pass_schedule_t sched = vs_blended_schedule(&g_state.view);

    /* --- Clear + draw (post-process wraps all passes) --- */
    post_pass_begin(&frame);

    if (ps_is_enabled(&sched, PASS_STARS))         star_pass_draw(&frame);
    if (ps_is_enabled(&sched, PASS_CONSTELLATION)) constellation_pass_draw(&frame);
    if (ps_is_enabled(&sched, PASS_DEEP_SKY))      deep_sky_pass_draw(&frame);
    if (ps_is_enabled(&sched, PASS_MILKYWAY))       milkyway_pass_draw(&frame);
    if (ps_is_enabled(&sched, PASS_DIFFRACTION))    diffraction_pass_draw(&frame);
    if (ps_is_enabled(&sched, PASS_ORBIT_TRAIL))    orbit_trail_pass_draw(&frame);
    if (ps_is_enabled(&sched, PASS_PLANET))         planet_pass_draw(&frame);
    if (ps_is_enabled(&sched, PASS_SATURN))         saturn_pass_draw(&frame);
    if (ps_is_enabled(&sched, PASS_MOON))           moon_pass_draw(&frame);
    if (ps_is_enabled(&sched, PASS_ZODIAC))         zodiac_pass_draw(&frame);
    ring_pass_draw(&frame);  /* concentric knowledge system rings */
    convergence_pass_draw(&frame);  /* brain convergence connection lines */
    if (ps_is_enabled(&sched, PASS_EARTH))          earth_pass_draw(&frame);
    if (ps_is_enabled(&sched, PASS_BODYGRAPH))      bodygraph_pass_draw(&frame);
    if (ps_is_enabled(&sched, PASS_HEXAGRAM))       hexagram_pass_draw(&frame);
    if (ps_is_enabled(&sched, PASS_TREE))           tree_pass_draw(&frame);
    if (ps_is_enabled(&sched, PASS_CARD))           card_pass_draw(&frame);
    if (ps_is_enabled(&sched, PASS_TEXT))           text_pass_draw(&frame);

    post_pass_end(&frame);

    /* --- Time HUD overlay --- */
    if (g_state.show_hud &&
        layer_is_visible(g_state.layer_state, LAYER_HUD))
        hud_update(g_state.simulation_jd, g_state.time_speed,
                   g_state.observer_lat, g_state.observer_lon,
                   g_state.camera.log_zoom);

    /* --- Update time bar display --- */
    ui_bridge_update_time(g_state.simulation_jd, g_state.time_speed);

    /* --- Audio: compute score + pulse, feed engine --- */
    {
        audio_params_t audio = audio_score_compute(
            g_state.simulation_jd,
            g_state.view.current_view,
            g_state.camera.log_zoom,
            g_state.time_speed);
        audio.pulse_factor = audio_score_pulse(audio.tempo_bpm,
                                               now_ms / 1000.0);

        /* Meditation mode modulation */
        if (g_state.meditation_active) {
            med_state_t med = med_compute(now_ms / 1000.0, 6.0f);
            /* Override pulse factor with meditation breathing */
            audio.pulse_factor = med.breath_factor;
            /* Boost reverb for spaciousness */
            audio.reverb_wet += med.reverb_boost;
            if (audio.reverb_wet > 0.85f)
                audio.reverb_wet = 0.85f;
        }

        audio_engine_update(&audio);
    }
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

    /* Check prefers-reduced-motion accessibility preference */
    {
        int reduced = EM_ASM_INT({
            return window.matchMedia('(prefers-reduced-motion: reduce)').matches ? 1 : 0;
        });
        g_state.motion_prefs = mp_set_reduced(g_state.motion_prefs, reduced);
        if (reduced) {
            g_state.enter_zoom = ez_skip(g_state.enter_zoom);
            g_state.enter_zoom_active = 0;
        }
    }

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
    if (orbit_trail_pass_init() != 0) return 1;
    if (ring_pass_init() != 0) return 1;
    if (convergence_pass_init() != 0) return 1;
    if (card_pass_init() != 0) return 1;
    if (text_pass_init() != 0) return 1;
    if (post_pass_init((int)css_w, (int)css_h) != 0) return 1;

    /* Initialize timing — start at the actual current date/time */
    g_state.prev_time_ms = emscripten_get_now();
    {
        double ms_since_epoch = emscripten_date_now();
        g_state.simulation_jd = 2440587.5 + (ms_since_epoch / 86400000.0);
    }

    /* Register input handlers */
    input_register(&g_state);

    /* Initialize UI bridge — populate panels with data from pure modules */
    ui_bridge_init();

    /* Initialize audio engine (WebAudio oscillators + reverb) */
    audio_engine_init();

    printf("Controls: Space=pause, 1-5=speed, -=reverse, T=trails, H=hud, Shift+T=theme, Shift+M=meditate, Shift+0-6=scale\n");

    /* Show init success in HUD */
    EM_ASM({
        var hud = document.getElementById('time-hud');
        if (hud) hud.innerHTML = 'Time initialized — starting render loop...';
    });

    emscripten_set_main_loop(main_loop, 0, 1);
#endif

    return 0;
}

/* =========================================================================
 * UI Bridge — EMSCRIPTEN_KEEPALIVE functions for DOM UI interaction
 * Called from JavaScript (web/index.html) via Module._ui_*()
 * ========================================================================= */
#ifdef __EMSCRIPTEN__

#include <emscripten.h>

EMSCRIPTEN_KEEPALIVE void ui_toggle_pause(void) {
    if (g_state.time_speed == 0.0)
        g_state.time_speed = g_state.prev_speed;
    else {
        g_state.prev_speed = g_state.time_speed;
        g_state.time_speed = 0.0;
    }
}

EMSCRIPTEN_KEEPALIVE void ui_set_speed(double speed) {
    g_state.time_speed = speed;
}

EMSCRIPTEN_KEEPALIVE double ui_get_speed(void) {
    return g_state.time_speed;
}

EMSCRIPTEN_KEEPALIVE void ui_jump_to_now(void) {
    /* Reset to current real time.
     * emscripten_date_now() = ms since Unix epoch (1970-01-01 00:00 UTC).
     * Unix epoch in JD = 2440587.5 */
    double ms_since_epoch = emscripten_date_now();
    g_state.simulation_jd = 2440587.5 + (ms_since_epoch / 86400000.0);
    g_state.time_speed = 1.0;
}

EMSCRIPTEN_KEEPALIVE double ui_get_jd(void) {
    return g_state.simulation_jd;
}

EMSCRIPTEN_KEEPALIVE void ui_toggle_trails(void) {
    g_state.show_trails = !g_state.show_trails;
}

EMSCRIPTEN_KEEPALIVE void ui_toggle_hud(void) {
    g_state.show_hud = !g_state.show_hud;
}

EMSCRIPTEN_KEEPALIVE void ui_set_scale(int level) {
    if (level < 0 || level > 6) return;
    g_state.scale_transition = scale_transition_create(
        g_state.camera.log_zoom, (scale_id_t)level, 1.2f);
}

EMSCRIPTEN_KEEPALIVE float ui_get_log_zoom(void) {
    return g_state.camera.log_zoom;
}

#endif /* __EMSCRIPTEN__ */
