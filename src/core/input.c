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
#include "../ui/touch_gestures.h"
#include <math.h>

/* Module-level pointer to the single app state.
 * Set once by input_register(), used by all callbacks. */
static app_state_t *g_input_state = 0;

/* Touch gesture state */
static tg_state_t s_touch;
static float s_prev_touch_x;
static float s_prev_touch_y;
static float s_prev_pinch_dist;
static int s_touch_drag_active;

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

    if (e->key[0] == 't' && e->key[1] == '\0' && !e->shiftKey) {
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

    /* E: switch to Earth View (toggle Space ↔ Earth) */
    if ((e->key[0] == 'e' || e->key[0] == 'E') && e->key[1] == '\0' && !e->shiftKey) {
        int cur = g_input_state->view.current_view;
        g_input_state->view = vs_set_view(g_input_state->view,
                                           cur == 1 ? 0 : 1); /* VIEW_EARTH=1, VIEW_SPACE=0 */
        return EM_TRUE;
    }

    /* Focus mode keys: A=Astrology, K=Kin, I=I Ching, C=Chinese, D=HD */
    if (!e->shiftKey && e->key[1] == '\0') {
        int focus = -1;
        switch (e->key[0]) {
            case 'a': case 'A': focus = 1; break; /* FOCUS_MODE_ASTROLOGY */
            case 'k': case 'K': focus = 2; break; /* FOCUS_MODE_KIN */
            case 'i': case 'I': focus = 3; break; /* FOCUS_MODE_ICHING */
            case 'c': case 'C': focus = 4; break; /* FOCUS_MODE_CHINESE */
            case 'd': case 'D': focus = 5; break; /* FOCUS_MODE_HD */
            default: break;
        }
        if (focus >= 0) {
            /* Toggle: if already in this focus, return to overview */
            int cur_focus = g_input_state->view.focus_mode;
            g_input_state->view = vs_set_focus(g_input_state->view,
                                                cur_focus == focus ? 0 : focus);
            return EM_TRUE;
        }
    }

    /* Escape: return to overview mode */
    if (e->key[0] == 27 || (e->key[0] == 'E' && e->key[1] == 's')) {
        g_input_state->view = vs_set_focus(g_input_state->view, 0); /* FOCUS_MODE_OVERVIEW */
        return EM_TRUE;
    }

    /* Shift + M: toggle meditation mode */
    if (e->shiftKey && e->key[0] == 'M' && e->key[1] == '\0') {
        g_input_state->meditation_active = !g_input_state->meditation_active;
        return EM_TRUE;
    }

    /* Shift + T: toggle theme (Cosmos / Dawn) */
    if (e->shiftKey && e->key[0] == 'T' && e->key[1] == '\0') {
        EM_ASM({ if (Module._ui_toggle_theme) Module._ui_toggle_theme(); });
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

/* --- Touch helpers --- */

static float touch_pinch_distance(const EmscriptenTouchEvent *e) {
    if (e->numTouches < 2) return 0.0f;
    /* Find first two active touches */
    int found = 0;
    float x0 = 0, y0 = 0, x1 = 0, y1 = 0;
    for (int i = 0; i < e->numTouches && found < 2; i++) {
        if (found == 0) { x0 = (float)e->touches[i].clientX; y0 = (float)e->touches[i].clientY; }
        else { x1 = (float)e->touches[i].clientX; y1 = (float)e->touches[i].clientY; }
        found++;
    }
    float dx = x1 - x0;
    float dy = y1 - y0;
    return sqrtf(dx * dx + dy * dy);
}

/* --- Touch callbacks --- */

static EM_BOOL on_touchstart(int type, const EmscriptenTouchEvent *e, void *data) {
    (void)type; (void)data;
    double now = emscripten_get_now();

    for (int i = 0; i < e->numTouches; i++) {
        if (e->touches[i].isChanged) {
            float nx = (float)e->touches[i].clientX;
            float ny = (float)e->touches[i].clientY;
            s_touch = tg_touch_start(s_touch, e->touches[i].identifier, nx, ny, now);
        }
    }

    /* For single-finger: start camera drag tracking */
    if (s_touch.active_count == 1) {
        for (int i = 0; i < TG_MAX_TOUCH_POINTS; i++) {
            if (s_touch.points[i].active) {
                s_prev_touch_x = s_touch.points[i].current_x;
                s_prev_touch_y = s_touch.points[i].current_y;
                s_touch_drag_active = 1;
                break;
            }
        }
    }

    /* For two-finger: initialize pinch distance */
    if (e->numTouches >= 2) {
        s_prev_pinch_dist = touch_pinch_distance(e);
        s_touch_drag_active = 0; /* cancel single-finger drag */
    }

    return EM_TRUE;
}

static EM_BOOL on_touchmove(int type, const EmscriptenTouchEvent *e, void *data) {
    (void)type; (void)data;
    double now = emscripten_get_now();

    /* Feed all moves to gesture recognizer */
    for (int i = 0; i < e->numTouches; i++) {
        if (e->touches[i].isChanged) {
            float nx = (float)e->touches[i].clientX;
            float ny = (float)e->touches[i].clientY;
            s_touch = tg_touch_move(s_touch, e->touches[i].identifier, nx, ny, now);
        }
    }

    /* Single-finger continuous camera orbit */
    if (s_touch.active_count == 1 && s_touch_drag_active) {
        for (int i = 0; i < TG_MAX_TOUCH_POINTS; i++) {
            if (s_touch.points[i].active) {
                float dx = s_touch.points[i].current_x - s_prev_touch_x;
                float dy = s_touch.points[i].current_y - s_prev_touch_y;
                s_prev_touch_x = s_touch.points[i].current_x;
                s_prev_touch_y = s_touch.points[i].current_y;
                /* Scale: touch coords are pixels, similar sensitivity to mouse */
                camera_rotate(&g_input_state->camera, -dx * 0.005f, -dy * 0.005f);
                break;
            }
        }
    }

    /* Two-finger continuous pinch zoom */
    if (e->numTouches >= 2) {
        float dist = touch_pinch_distance(e);
        if (s_prev_pinch_dist > 1.0f && dist > 1.0f) {
            float delta = (s_prev_pinch_dist - dist) * 0.005f;
            camera_zoom(&g_input_state->camera, delta);
        }
        s_prev_pinch_dist = dist;
    }

    return EM_TRUE;
}

static EM_BOOL on_touchend(int type, const EmscriptenTouchEvent *e, void *data) {
    (void)type; (void)data;
    double now = emscripten_get_now();

    for (int i = 0; i < e->numTouches; i++) {
        if (e->touches[i].isChanged) {
            float nx = (float)e->touches[i].clientX;
            float ny = (float)e->touches[i].clientY;
            tg_gesture_data_t gdata;
            tg_gesture_t gesture = tg_touch_end(
                &s_touch, e->touches[i].identifier, nx, ny, now, &gdata);

            /* Handle discrete gestures */
            switch (gesture) {
                case TG_DOUBLE_TAP:
                    /* Toggle pause */
                    if (g_input_state->time_speed == 0.0) {
                        g_input_state->time_speed = g_input_state->prev_speed;
                        if (g_input_state->time_speed == 0.0)
                            g_input_state->time_speed = 1.0; /* default to real-time */
                    } else {
                        g_input_state->prev_speed = g_input_state->time_speed;
                        g_input_state->time_speed = 0.0;
                    }
                    break;

                case TG_TAP:
                case TG_LONG_PRESS:
                case TG_THREE_FINGER_TAP:
                    /* Future: tap->select, long-press->context, 3-finger->help */
                    break;

                default:
                    break;
            }
        }
    }

    /* Reset drag state when no fingers remain */
    if (s_touch.active_count == 0) {
        s_touch_drag_active = 0;
    }

    return EM_TRUE;
}

void input_register(app_state_t *state) {
    g_input_state = state;
    emscripten_set_mousedown_callback("#canvas", NULL, EM_TRUE, on_mouse_down);
    emscripten_set_mouseup_callback("#canvas", NULL, EM_TRUE, on_mouse_up);
    emscripten_set_mousemove_callback("#canvas", NULL, EM_TRUE, on_mouse_move);
    emscripten_set_wheel_callback("#canvas", NULL, EM_TRUE, on_wheel);
    emscripten_set_keydown_callback(EMSCRIPTEN_EVENT_TARGET_DOCUMENT, NULL, EM_TRUE, on_key_down);

    /* Touch gesture callbacks */
    s_touch = tg_init();
    emscripten_set_touchstart_callback("#canvas", NULL, EM_TRUE, on_touchstart);
    emscripten_set_touchmove_callback("#canvas", NULL, EM_TRUE, on_touchmove);
    emscripten_set_touchend_callback("#canvas", NULL, EM_TRUE, on_touchend);
}

#endif /* __EMSCRIPTEN__ */
