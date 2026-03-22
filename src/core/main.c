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
#include "../render/passes/natal_chart_pass.h"
#include "../render/passes/tree_of_life_pass.h"
#include "../render/passes/bagua_pass.h"
#include "../render/passes/gates_mandala_pass.h"
#include "../render/earth_view_frame.h"
#include "../systems/astronomy/planets.h"
#include "../ui/ui_bridge.h"
#include "../ui/view_registry.h"
#include "../ui/audio_score.h"
#include "../ui/audio_engine.h"
#include "../ui/audio_meditation.h"
#include "../ui/audio_culture.h"
#include "../systems/tzolkin/tzolkin.h"
#include "../systems/tzolkin/dreamspell.h"
#include "../systems/unified/audio_data.h"
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
            br_visual_from_result(&br, &g_state.brain_visual);
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

    /* --- Theme: export CSS custom properties --- */
    {
        theme_t theme;
        if (g_state.auto_theme_enabled) {
            auto_theme_state_t target = at_from_location(
                g_state.simulation_jd, g_state.observer_lat, g_state.observer_lon);
            g_state.auto_theme = at_smooth(g_state.auto_theme, target,
                                            (float)dt_sec, 2.0f);
            theme = at_blended_theme(g_state.auto_theme);
        } else {
            /* Forced Cosmos (dark) — always export CSS for correct UI styling */
            theme = theme_get(THEME_COSMOS);
        }
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
        .theme_id      = 0, /* THEME_COSMOS — always dark, Dawn disabled */
        .focus_mode    = g_state.view.focus_mode,
    };
    memcpy(frame.headline, g_state.headline, sizeof(frame.headline));
    frame.brain_visual = g_state.brain_visual;

    /* Planet data for natal chart pass */
    {
        solar_system_t ss = planets_at(frame.simulation_jd);
        frame.sun_lon = fmod(ss.positions[PLANET_EARTH].longitude + 180.0, 360.0);
        if (frame.sun_lon < 0.0) frame.sun_lon += 360.0;
        for (int p = 0; p < 8; p++)
            frame.planet_lon[p] = ss.positions[p].longitude;
        GLint vp[4];
        glGetIntegerv(GL_VIEWPORT, vp);
        frame.viewport_width  = vp[2] > 0 ? vp[2] : 1920;
        frame.viewport_height = vp[3] > 0 ? vp[3] : 1080;
    }

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
    /* Zodiac ring + knowledge rings: only in Astrology focus or overview */
    if (ps_is_enabled(&sched, PASS_ZODIAC) &&
        (frame.focus_mode == 0 || frame.focus_mode == 1))
        zodiac_pass_draw(&frame);
    if (frame.focus_mode == 0)
        ring_pass_draw(&frame);  /* concentric knowledge system rings — overview only */
    convergence_pass_draw(&frame);  /* brain convergence connection lines */
    if (ps_is_enabled(&sched, PASS_EARTH))          earth_pass_draw(&frame);

    /* System-specific geometric passes — only draw when focused (BUG 5 fix) */
    if (frame.focus_mode == 5) { /* FOCUS_MODE_HD */
        if (ps_is_enabled(&sched, PASS_BODYGRAPH))  bodygraph_pass_draw(&frame);
    }
    if (frame.focus_mode == 3) { /* FOCUS_MODE_ICHING */
        if (ps_is_enabled(&sched, PASS_HEXAGRAM))   hexagram_pass_draw(&frame);
        bagua_pass_draw(&frame);
    }
    if (frame.focus_mode == 6) { /* FOCUS_MODE_KABBALAH */
        tree_of_life_pass_draw(&frame);
        gates_mandala_pass_draw(&frame);
    }
    if (frame.focus_mode == 1) natal_chart_pass_draw(&frame); /* Astrology */
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

        /* Focus-mode audio override (S108):
         * K key: seal-specific planetary tone from Dreamspell correspondence.
         * A/I/C keys: cultural timbre from audio_culture. */
        {
            int fm = g_state.view.focus_mode;
            if (fm > 0 && fm < 6) {
                int slot = 8; /* dedicated cultural timbre slot */

                if (fm == 2) {
                    /* FOCUS_MODE_KIN — Dreamspell seal-specific planetary tone */
                    tzolkin_day_t tz = tzolkin_from_jd(g_state.simulation_jd);
                    dreamspell_planet_t dp = dreamspell_planet(tz.seal);
                    if (dp.freq_planet_index >= 0) {
                        audio_planet_profile_t prof = audio_planet_profile(dp.freq_planet_index);
                        audio.focused_system = CD_SYS_TZOLKIN;
                        audio.frequencies[slot] = (float)(prof.fundamental_hz * (double)dp.freq_multiplier);
                        audio.amplitudes[slot] = (float)prof.base_amplitude * 0.5f;
                        audio.waveform_types[slot] = (int)prof.waveform;
                        audio.harmonic_counts[slot] = prof.harmonic_count;
                        for (int h = 0; h < prof.harmonic_count && h < AS_MAX_HARMONICS; h++)
                            audio.harmonic_amps[slot][h] = (float)prof.harmonics[h].amplitude;
                        audio.pan_positions[slot] = 0.0f;
                        if (audio.planet_count <= slot)
                            audio.planet_count = slot + 1;
                    }
                } else {
                    /* A/I/C/D — cultural timbre */
                    static const int FOCUS_TO_CD[] = {
                        -1, CD_SYS_ASTROLOGY, -1, CD_SYS_ICHING, CD_SYS_CHINESE, -1
                    };
                    int cd_sys = FOCUS_TO_CD[fm];
                    if (cd_sys >= 0 && audio_culture_has_timbre((cd_system_t)cd_sys)) {
                        audio_culture_t culture = audio_culture_get((cd_system_t)cd_sys);
                        audio.focused_system = cd_sys;
                        audio.frequencies[slot] = culture.base_freq_hz;
                        audio.amplitudes[slot] = culture.base_amplitude * 0.5f;
                        audio.waveform_types[slot] = 0;
                        audio.harmonic_counts[slot] = culture.partial_count;
                        for (int h = 0; h < culture.partial_count && h < AS_MAX_HARMONICS; h++)
                            audio.harmonic_amps[slot][h] = culture.partials[h];
                        audio.pan_positions[slot] = 0.0f;
                        if (audio.planet_count <= slot)
                            audio.planet_count = slot + 1;
                    }
                }
            }
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
    natal_chart_pass_init();
    tree_of_life_pass_init();
    bagua_pass_init();
    gates_mandala_pass_init();
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

EMSCRIPTEN_KEEPALIVE void ui_set_birth_date(int year, int month, int day) {
    if (year < 1 || year > 9999 || month < 1 || month > 12 ||
        day < 1 || day > 31)
        return;
    g_state.birth_year = year;
    g_state.birth_month = month;
    g_state.birth_day = day;
    g_state.birth_entered = 1;
    g_state.birth_profile = bp_compute(year, month, day);
    g_state.birth_sky = birth_sky_from_profile(&g_state.birth_profile);
    g_state.birth_sky.active = false;
}

EMSCRIPTEN_KEEPALIVE int ui_get_birth_entered(void) {
    return g_state.birth_entered;
}

EMSCRIPTEN_KEEPALIVE int ui_get_birth_kin(void) {
    return g_state.birth_entered ? g_state.birth_profile.tzolkin.kin : 0;
}

EMSCRIPTEN_KEEPALIVE int ui_get_birth_year(void) {
    return g_state.birth_year;
}

EMSCRIPTEN_KEEPALIVE int ui_get_birth_month(void) {
    return g_state.birth_month;
}

EMSCRIPTEN_KEEPALIVE int ui_get_birth_day(void) {
    return g_state.birth_day;
}

EMSCRIPTEN_KEEPALIVE void ui_show_birth_profile(void) {
    if (!g_state.birth_entered) return;
    const birth_profile_t *p = &g_state.birth_profile;

    /* Build HTML string in C to avoid EM_ASM parameter limits */
    static char html[1024];
    snprintf(html, sizeof(html),
        "<b>Your Cosmic Identity</b><br>"
        "Kin %d — %s %s<br>"
        "%s %s (%s)<br>"
        "Hebrew: %d %s %d<br>"
        "Islamic: %d %s %d<br>"
        "Buddhist Era: %d<br>"
        "I Ching: Hexagram %d — %s<br>"
        "%s | HD Gate %d.%d",
        p->tzolkin.kin,
        p->tzolkin.tone_name ? p->tzolkin.tone_name : "?",
        p->tzolkin.seal_name ? p->tzolkin.seal_name : "?",
        p->chinese.element_name ? p->chinese.element_name : "?",
        p->chinese.animal_name ? p->chinese.animal_name : "?",
        p->western_zodiac ? p->western_zodiac : "?",
        p->hebrew.year,
        p->hebrew.month_name ? p->hebrew.month_name : "?",
        p->hebrew.day,
        p->islamic.year,
        p->islamic.month_name ? p->islamic.month_name : "?",
        p->islamic.day,
        p->buddhist.era_year,
        p->iching.king_wen,
        p->iching.name ? p->iching.name : "?",
        p->astrology.sun_sign_name ? p->astrology.sun_sign_name : "?",
        p->astrology.hd_sun_gate, p->astrology.hd_earth_gate
    );

    EM_ASM({
        var el = document.getElementById('birth-result');
        if (el) el.innerHTML = UTF8ToString($0);
    }, html);
}

EMSCRIPTEN_KEEPALIVE void ui_set_locale(int locale_id) {
    if (locale_id < 0 || locale_id >= 20) return;
    g_state.locale = locale_id;
}

EMSCRIPTEN_KEEPALIVE int ui_get_locale(void) {
    return g_state.locale;
}

EMSCRIPTEN_KEEPALIVE void ui_set_view(int view_id) {
    if (view_id < 0 || view_id > 1) return;
    g_state.view = vs_set_view(g_state.view, view_id);
}

EMSCRIPTEN_KEEPALIVE int ui_get_view(void) {
    return g_state.view.current_view;
}

EMSCRIPTEN_KEEPALIVE void ui_on_resize(int width, int height) {
    if (width < 1) width = 1;
    if (height < 1) height = 1;
    emscripten_set_canvas_element_size("#canvas", width, height);
    glViewport(0, 0, width, height);
    g_state.camera.aspect = (float)width / (float)height;
    post_pass_resize(width, height);
}

EMSCRIPTEN_KEEPALIVE const char *ui_get_headline(void) {
    return g_state.headline;
}

EMSCRIPTEN_KEEPALIVE int ui_is_audio_initialized(void) {
    return !audio_engine_is_muted();
}

#endif /* __EMSCRIPTEN__ */
