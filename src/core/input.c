/*
 * input.c — Emscripten input event handlers
 *
 * Extracted from main.c. Handles mouse drag (camera rotation),
 * scroll wheel (zoom), and keyboard (time control, toggles, scale jumps).
 */

#ifdef __EMSCRIPTEN__

#include "input.h"
#include <emscripten.h>
#include <emscripten/html5.h>
#include "../render/camera.h"
#include "../render/camera_scale.h"

/* Module-level pointer to the single app state.
 * Set once by input_register(), used by all callbacks. */
static app_state_t *g_input_state = 0;

/* Time speed presets (days per real second) */
static const double TIME_SPEEDS[] = {
    0.0,         /* 0: paused */
    1.0,         /* 1: real-time (1 day/day) */
    60.0,        /* 2: 1 minute/sec */
    3600.0,      /* 3: 1 hour/sec */
    86400.0,     /* 4: 1 day/sec */
    864000.0     /* 5: 10 days/sec */
};

static EM_BOOL on_mouse_down(int type, const EmscriptenMouseEvent *e, void *data) {
    (void)type; (void)data;
    g_input_state->mouse_down = 1;
    g_input_state->mouse_x = e->clientX;
    g_input_state->mouse_y = e->clientY;
    return EM_TRUE;
}

static EM_BOOL on_mouse_up(int type, const EmscriptenMouseEvent *e, void *data) {
    (void)type; (void)e; (void)data;
    g_input_state->mouse_down = 0;
    return EM_TRUE;
}

static EM_BOOL on_mouse_move(int type, const EmscriptenMouseEvent *e, void *data) {
    (void)type; (void)data;
    if (!g_input_state->mouse_down) return EM_FALSE;

    double dx = e->clientX - g_input_state->mouse_x;
    double dy = e->clientY - g_input_state->mouse_y;
    g_input_state->mouse_x = e->clientX;
    g_input_state->mouse_y = e->clientY;

    camera_rotate(&g_input_state->camera, (float)(-dx * 0.005), (float)(-dy * 0.005));
    return EM_TRUE;
}

static EM_BOOL on_wheel(int type, const EmscriptenWheelEvent *e, void *data) {
    (void)type; (void)data;
    camera_zoom(&g_input_state->camera, (float)(e->deltaY * 0.01));
    return EM_TRUE;
}

static EM_BOOL on_key_down(int type, const EmscriptenKeyboardEvent *e, void *data) {
    (void)type; (void)data;

    if (e->key[0] == ' ' && e->key[1] == '\0') {
        if (g_input_state->time_speed == 0.0)
            g_input_state->time_speed = g_input_state->prev_speed;
        else {
            g_input_state->prev_speed = g_input_state->time_speed;
            g_input_state->time_speed = 0.0;
        }
        return EM_TRUE;
    }

    if (e->key[0] >= '0' && e->key[0] <= '5' && e->key[1] == '\0') {
        int idx = e->key[0] - '0';
        double sign = (g_input_state->time_speed < 0.0) ? -1.0 : 1.0;
        g_input_state->time_speed = sign * TIME_SPEEDS[idx];
        return EM_TRUE;
    }

    if (e->key[0] == '-' && e->key[1] == '\0') {
        g_input_state->time_speed = -g_input_state->time_speed;
        return EM_TRUE;
    }

    if ((e->key[0] == 't' || e->key[0] == 'T') && e->key[1] == '\0') {
        g_input_state->show_trails = !g_input_state->show_trails;
        return EM_TRUE;
    }

    if ((e->key[0] == 'h' || e->key[0] == 'H') && e->key[1] == '\0') {
        g_input_state->show_hud = !g_input_state->show_hud;
        EM_ASM({
            var hud = document.getElementById('time-hud');
            if (hud) hud.style.display = $0 ? 'block' : 'none';
        }, g_input_state->show_hud);
        return EM_TRUE;
    }

    /* Shift + 0-6: jump to scale level */
    if (e->shiftKey) {
        int target = -1;
        if (e->code[0] == 'D' && e->code[1] == 'i' && e->code[2] == 'g' &&
            e->code[3] == 'i' && e->code[4] == 't' &&
            e->code[5] >= '0' && e->code[5] <= '6' && e->code[6] == '\0') {
            target = e->code[5] - '0';
        }
        if (target >= 0 && target <= 6) {
            g_input_state->scale_transition = scale_transition_create(
                g_input_state->camera.log_zoom, (scale_id_t)target, 1.2f);
            return EM_TRUE;
        }
    }

    return EM_FALSE;
}

void input_register(app_state_t *state) {
    g_input_state = state;
    emscripten_set_mousedown_callback("#canvas", NULL, EM_TRUE, on_mouse_down);
    emscripten_set_mouseup_callback("#canvas", NULL, EM_TRUE, on_mouse_up);
    emscripten_set_mousemove_callback("#canvas", NULL, EM_TRUE, on_mouse_move);
    emscripten_set_wheel_callback("#canvas", NULL, EM_TRUE, on_wheel);
    emscripten_set_keydown_callback(EMSCRIPTEN_EVENT_TARGET_DOCUMENT, NULL, EM_TRUE, on_key_down);
}

#endif /* __EMSCRIPTEN__ */
