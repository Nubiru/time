#include "ui_state.h"

/* --- Internal helpers --- */

static float clampf(float v, float lo, float hi)
{
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

/* Smoothstep: 3t^2 - 2t^3 where t is clamped to [0,1] */
static float smoothstep(float edge0, float edge1, float x)
{
    if (edge0 >= edge1) return 0.0f;
    float t = (x - edge0) / (edge1 - edge0);
    t = clampf(t, 0.0f, 1.0f);
    return t * t * (3.0f - 2.0f * t);
}

/* --- Phi-derived timing constants --- */
#define PHI         1.6180339887498948482f
#define PHI_INV2    0.3819660112501051518f  /* 1/phi^2 */

/* --- Public API --- */

ui_state_t ui_state_init(void)
{
    ui_state_t s;
    s.mode = UI_MODE_DEFAULT;
    s.prev_mode = UI_MODE_DEFAULT;
    s.mode_progress = 0.0f;
    s.mode_opening = 0;

    s.bottom_proximity = 0.0f;
    s.left_proximity = 0.0f;
    s.right_proximity = 0.0f;

    s.hint_opacity = 1.0f;
    s.hint_timer = 0.0f;

    s.scene_dim = 0.0f;

    return s;
}

ui_timing_t ui_timing_default(void)
{
    ui_timing_t t;
    t.panel_duration = PHI_INV2;       /* ~0.382s -- snappy */
    t.hint_duration = 5.0f;            /* 5s before fade starts */
    t.hint_fade = PHI;                 /* ~1.618s for fade-out */
    t.proximity_threshold = 0.1f;      /* 10% of screen dimension */
    t.dim_amount = PHI_INV2;           /* ~0.382 -- subtle dim */
    return t;
}

ui_state_t ui_state_toggle_mode(ui_state_t state, ui_mode_t mode)
{
    if (mode <= UI_MODE_DEFAULT || mode >= UI_MODE_COUNT) {
        return state;
    }

    if (state.mode == mode) {
        /* Same mode: start closing */
        state.mode_opening = 0;
    } else {
        /* Different mode (or DEFAULT): switch to new mode */
        state.prev_mode = state.mode;
        state.mode = mode;
        state.mode_opening = 1;
        /* If switching from one panel to another, keep progress for smooth transition */
        if (state.prev_mode == UI_MODE_DEFAULT) {
            state.mode_progress = 0.0f;
        }
    }

    return state;
}

ui_state_t ui_state_close(ui_state_t state)
{
    if (state.mode == UI_MODE_DEFAULT) {
        return state;
    }
    state.mode_opening = 0;
    return state;
}

int ui_state_is_mode(ui_state_t state, ui_mode_t mode)
{
    return state.mode == mode ? 1 : 0;
}

ui_state_t ui_state_update(ui_state_t state, float dt, ui_timing_t timing)
{
    if (dt <= 0.0f) return state;

    /* --- Mode progress animation --- */
    if (state.mode != UI_MODE_DEFAULT) {
        float rate = 1.0f / timing.panel_duration;
        if (state.mode_opening) {
            state.mode_progress += rate * dt;
            if (state.mode_progress > 1.0f) {
                state.mode_progress = 1.0f;
            }
        } else {
            state.mode_progress -= rate * dt;
            if (state.mode_progress <= 0.0f) {
                state.mode_progress = 0.0f;
                state.prev_mode = state.mode;
                state.mode = UI_MODE_DEFAULT;
            }
        }
    }

    /* --- Scene dim --- */
    float target_dim = (state.mode != UI_MODE_DEFAULT) ? timing.dim_amount : 0.0f;
    float dim_rate = 1.0f / timing.panel_duration;
    if (state.scene_dim < target_dim) {
        state.scene_dim += dim_rate * dt;
        if (state.scene_dim > target_dim) {
            state.scene_dim = target_dim;
        }
    } else if (state.scene_dim > target_dim) {
        state.scene_dim -= dim_rate * dt;
        if (state.scene_dim < target_dim) {
            state.scene_dim = target_dim;
        }
    }

    /* --- Onboarding hint --- */
    state.hint_timer += dt;
    if (state.hint_timer > timing.hint_duration) {
        float fade_elapsed = state.hint_timer - timing.hint_duration;
        if (timing.hint_fade > 0.0f) {
            state.hint_opacity = 1.0f - (fade_elapsed / timing.hint_fade);
        } else {
            state.hint_opacity = 0.0f;
        }
        if (state.hint_opacity < 0.0f) {
            state.hint_opacity = 0.0f;
        }
    }

    return state;
}

ui_state_t ui_state_update_mouse(ui_state_t state, float mouse_x, float mouse_y,
                                  float screen_w, float screen_h, ui_timing_t timing)
{
    if (screen_w <= 0.0f || screen_h <= 0.0f) return state;

    float threshold = timing.proximity_threshold;

    /* Bottom proximity: distance from bottom edge (0=at edge, 1=far) */
    float dist_bottom = (screen_h - mouse_y) / screen_h;
    state.bottom_proximity = 1.0f - smoothstep(0.0f, threshold, dist_bottom);

    /* Left proximity: distance from left edge */
    float dist_left = mouse_x / screen_w;
    state.left_proximity = 1.0f - smoothstep(0.0f, threshold, dist_left);

    /* Right proximity: distance from right edge */
    float dist_right = (screen_w - mouse_x) / screen_w;
    state.right_proximity = 1.0f - smoothstep(0.0f, threshold, dist_right);

    return state;
}

float ui_state_panel_opacity(ui_state_t state, ui_mode_t mode)
{
    if (state.mode != mode) return 0.0f;
    return state.mode_progress;
}

float ui_state_time_bar_opacity(ui_state_t state)
{
    return state.bottom_proximity;
}

float ui_state_layer_panel_opacity(ui_state_t state)
{
    float layer_progress = (state.mode == UI_MODE_LAYER_PANEL) ? state.mode_progress : 0.0f;
    return (state.left_proximity > layer_progress) ? state.left_proximity : layer_progress;
}

float ui_state_scene_brightness(ui_state_t state, ui_timing_t timing)
{
    (void)timing;
    return 1.0f - state.scene_dim;
}

const char *ui_mode_name(ui_mode_t mode)
{
    switch (mode) {
    case UI_MODE_DEFAULT:     return "Default";
    case UI_MODE_HELP:        return "Help";
    case UI_MODE_PALETTE:     return "Palette";
    case UI_MODE_LAYER_PANEL: return "Layer Panel";
    case UI_MODE_SETTINGS:    return "Settings";
    default:                  return "Unknown";
    }
}

int ui_mode_count(void)
{
    return UI_MODE_COUNT;
}
