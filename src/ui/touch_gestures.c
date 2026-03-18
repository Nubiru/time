#include "touch_gestures.h"

#include <math.h>

/* --- Internal helpers --- */

/* Find slot index for a given touch ID. Returns -1 if not found. */
static int find_slot(const tg_state_t *state, int id)
{
    for (int i = 0; i < TG_MAX_TOUCH_POINTS; i++) {
        if (state->points[i].active && state->points[i].id == id) {
            return i;
        }
    }
    return -1;
}

/* Find first inactive slot. Returns -1 if all slots full. */
static int find_free_slot(const tg_state_t *state)
{
    for (int i = 0; i < TG_MAX_TOUCH_POINTS; i++) {
        if (!state->points[i].active) {
            return i;
        }
    }
    return -1;
}

/* Euclidean distance between two 2D points */
static float distance_2d(float x1, float y1, float x2, float y2)
{
    float dx = x2 - x1;
    float dy = y2 - y1;
    return sqrtf(dx * dx + dy * dy);
}

/* Initialize zero-filled gesture data */
static tg_gesture_data_t gesture_data_zero(void)
{
    tg_gesture_data_t d;
    d.x = 0.0f;
    d.y = 0.0f;
    d.dx = 0.0f;
    d.dy = 0.0f;
    d.pinch_scale = 1.0f;
    d.rotate_angle = 0.0f;
    d.velocity = 0.0f;
    return d;
}

/* Find the two active touch point indices (for pinch/rotate).
 * Returns 1 if exactly two found, 0 otherwise. */
static int find_two_active(const tg_state_t *state, int *idx_a, int *idx_b)
{
    int found = 0;
    for (int i = 0; i < TG_MAX_TOUCH_POINTS; i++) {
        if (state->points[i].active) {
            if (found == 0) {
                *idx_a = i;
            } else if (found == 1) {
                *idx_b = i;
            }
            found++;
        }
    }
    return (found == 2) ? 1 : 0;
}

/* Classify a single-finger gesture on touch end */
static tg_gesture_t classify_single_finger(const tg_touch_point_t *pt,
                                            float end_x, float end_y,
                                            double end_time,
                                            tg_gesture_data_t *out)
{
    float dx = end_x - pt->start_x;
    float dy = end_y - pt->start_y;
    double duration = end_time - pt->start_time;
    float dist = distance_2d(pt->start_x, pt->start_y, end_x, end_y);

    out->x = end_x;
    out->y = end_y;
    out->dx = dx;
    out->dy = dy;
    out->pinch_scale = 1.0f;
    out->rotate_angle = 0.0f;

    /* Velocity: normalized units per second */
    if (duration > 0.0) {
        out->velocity = dist / (float)(duration / 1000.0);
    } else {
        out->velocity = 0.0f;
    }

    /* Check swipe first: needs enough distance and tight angle */
    if (dist >= TG_SWIPE_MIN_DIST) {
        float angle = atan2f(fabsf(dy), fabsf(dx));
        if (angle <= TG_SWIPE_MAX_ANGLE) {
            /* Horizontal swipe */
            return (dx > 0.0f) ? TG_SWIPE_RIGHT : TG_SWIPE_LEFT;
        }
        float vert_angle = atan2f(fabsf(dx), fabsf(dy));
        if (vert_angle <= TG_SWIPE_MAX_ANGLE) {
            /* Vertical swipe */
            return (dy > 0.0f) ? TG_SWIPE_DOWN : TG_SWIPE_UP;
        }
        /* Too diagonal for swipe */
        return TG_NONE;
    }

    /* Check tap: needs short duration and small movement */
    if (duration <= TG_TAP_MAX_MS && dist <= TG_TAP_MAX_MOVEMENT) {
        return TG_TAP;
    }

    return TG_NONE;
}

/* --- Public API --- */

tg_state_t tg_init(void)
{
    tg_state_t s;
    for (int i = 0; i < TG_MAX_TOUCH_POINTS; i++) {
        s.points[i].id = 0;
        s.points[i].active = 0;
        s.points[i].start_x = 0.0f;
        s.points[i].start_y = 0.0f;
        s.points[i].current_x = 0.0f;
        s.points[i].current_y = 0.0f;
        s.points[i].start_time = 0.0;
        s.points[i].long_press_fired = 0;
    }
    s.active_count = 0;
    s.last_gesture = TG_NONE;
    s.last_tap_time = 0.0;
    s.last_tap_x = 0.0f;
    s.last_tap_y = 0.0f;
    s.three_finger_started = 0;
    s.three_finger_ended = 0;
    return s;
}

tg_state_t tg_touch_start(tg_state_t state, int id, float x, float y,
                           double time_ms)
{
    int slot = find_free_slot(&state);
    if (slot < 0) {
        return state;  /* All slots full */
    }

    state.points[slot].id = id;
    state.points[slot].active = 1;
    state.points[slot].start_x = x;
    state.points[slot].start_y = y;
    state.points[slot].current_x = x;
    state.points[slot].current_y = y;
    state.points[slot].start_time = time_ms;
    state.points[slot].long_press_fired = 0;
    state.active_count++;

    /* Track peak simultaneous touches for 3-finger detection */
    if (state.active_count >= 3 && state.three_finger_started < state.active_count) {
        state.three_finger_started = state.active_count;
        state.three_finger_ended = 0;
    }

    return state;
}

tg_state_t tg_touch_move(tg_state_t state, int id, float x, float y,
                          double time_ms)
{
    int slot = find_slot(&state, id);
    if (slot < 0) {
        return state;  /* Unknown touch ID */
    }
    (void)time_ms;

    state.points[slot].current_x = x;
    state.points[slot].current_y = y;
    return state;
}

tg_gesture_t tg_touch_end(tg_state_t *state, int id, float x, float y,
                           double time_ms, tg_gesture_data_t *out)
{
    *out = gesture_data_zero();

    int slot = find_slot(state, id);
    if (slot < 0) {
        return TG_NONE;
    }

    tg_touch_point_t pt = state->points[slot];

    /* Update position for final calculation */
    pt.current_x = x;
    pt.current_y = y;

    /* Check for three-finger tap completion */
    if (state->three_finger_started >= 3) {
        state->three_finger_ended++;
        double duration = time_ms - pt.start_time;
        float dist = distance_2d(pt.start_x, pt.start_y, x, y);

        /* Deactivate this point */
        state->points[slot].active = 0;
        state->active_count--;

        if (state->three_finger_ended >= 3 &&
            duration <= TG_TAP_MAX_MS && dist <= TG_TAP_MAX_MOVEMENT) {
            /* All three fingers have lifted */
            out->x = x;
            out->y = y;
            out->dx = 0.0f;
            out->dy = 0.0f;
            state->three_finger_started = 0;
            state->three_finger_ended = 0;
            state->last_gesture = TG_THREE_FINGER_TAP;
            return TG_THREE_FINGER_TAP;
        }

        /* If not all lifted yet, or didn't qualify */
        if (state->active_count == 0) {
            state->three_finger_started = 0;
            state->three_finger_ended = 0;
        }
        return TG_NONE;
    }

    /* Check two-finger gestures (pinch/rotate) if there were 2 active points */
    if (state->active_count == 2) {
        int idx_a, idx_b;
        if (find_two_active(state, &idx_a, &idx_b)) {
            /* Determine which is the ending one */
            int other = (idx_a == slot) ? idx_b : idx_a;

            /* Start positions */
            float s_ax = state->points[slot].start_x;
            float s_ay = state->points[slot].start_y;
            float s_bx = state->points[other].start_x;
            float s_by = state->points[other].start_y;

            /* Current positions (use end position for the ending finger) */
            float c_ax = x;
            float c_ay = y;
            float c_bx = state->points[other].current_x;
            float c_by = state->points[other].current_y;

            float start_dist = distance_2d(s_ax, s_ay, s_bx, s_by);
            float curr_dist = distance_2d(c_ax, c_ay, c_bx, c_by);

            /* Angle between the two fingers */
            float start_angle = atan2f(s_by - s_ay, s_bx - s_ax);
            float curr_angle = atan2f(c_by - c_ay, c_bx - c_ax);
            float angle_diff = curr_angle - start_angle;

            /* Normalize angle to [-PI, PI] */
            while (angle_diff > (float)PI) angle_diff -= 2.0f * (float)PI;
            while (angle_diff < -(float)PI) angle_diff += 2.0f * (float)PI;

            out->x = (c_ax + c_bx) * 0.5f;
            out->y = (c_ay + c_by) * 0.5f;
            out->dx = 0.0f;
            out->dy = 0.0f;
            out->rotate_angle = angle_diff;
            out->pinch_scale = (start_dist > 0.001f) ?
                               (curr_dist / start_dist) : 1.0f;
            out->velocity = 0.0f;

            /* Deactivate the ending touch point */
            state->points[slot].active = 0;
            state->active_count--;

            /* Decide: rotation or pinch? Rotation takes priority if significant */
            if (fabsf(angle_diff) >= TG_ROTATE_MIN_ANGLE) {
                state->last_gesture = TG_TWO_FINGER_ROTATE;
                return TG_TWO_FINGER_ROTATE;
            }

            float scale_change = fabsf(out->pinch_scale - 1.0f);
            if (scale_change >= TG_PINCH_MIN_CHANGE) {
                tg_gesture_t g = (out->pinch_scale > 1.0f) ?
                                 TG_PINCH_OUT : TG_PINCH_IN;
                state->last_gesture = g;
                return g;
            }

            return TG_NONE;
        }
    }

    /* Single finger gesture classification */
    state->points[slot].active = 0;
    state->active_count--;

    tg_gesture_t g = classify_single_finger(&pt, x, y, time_ms, out);

    if (g == TG_TAP) {
        /* Check for double tap */
        if (state->last_tap_time > 0.0 &&
            (time_ms - state->last_tap_time) <= TG_DOUBLE_TAP_MAX_MS) {
            state->last_tap_time = 0.0;
            state->last_gesture = TG_DOUBLE_TAP;
            return TG_DOUBLE_TAP;
        }
        state->last_tap_time = time_ms;
        state->last_tap_x = x;
        state->last_tap_y = y;
    }

    state->last_gesture = g;
    return g;
}

tg_gesture_t tg_check_long_press(tg_state_t *state, double time_ms,
                                  tg_gesture_data_t *out)
{
    *out = gesture_data_zero();

    if (state->active_count != 1) {
        return TG_NONE;
    }

    /* Find the single active touch point */
    int slot = -1;
    for (int i = 0; i < TG_MAX_TOUCH_POINTS; i++) {
        if (state->points[i].active) {
            slot = i;
            break;
        }
    }
    if (slot < 0) {
        return TG_NONE;
    }

    tg_touch_point_t *pt = &state->points[slot];

    if (pt->long_press_fired) {
        return TG_NONE;
    }

    double duration = time_ms - pt->start_time;
    float dist = distance_2d(pt->start_x, pt->start_y,
                             pt->current_x, pt->current_y);

    if (duration >= TG_LONG_PRESS_MIN_MS && dist <= TG_TAP_MAX_MOVEMENT) {
        pt->long_press_fired = 1;
        out->x = pt->start_x;
        out->y = pt->start_y;
        out->dx = pt->current_x - pt->start_x;
        out->dy = pt->current_y - pt->start_y;
        state->last_gesture = TG_LONG_PRESS;
        return TG_LONG_PRESS;
    }

    return TG_NONE;
}

const char *tg_gesture_name(tg_gesture_t gesture)
{
    switch (gesture) {
        case TG_NONE:              return "none";
        case TG_TAP:               return "tap";
        case TG_DOUBLE_TAP:        return "double_tap";
        case TG_LONG_PRESS:        return "long_press";
        case TG_SWIPE_LEFT:        return "swipe_left";
        case TG_SWIPE_RIGHT:       return "swipe_right";
        case TG_SWIPE_UP:          return "swipe_up";
        case TG_SWIPE_DOWN:        return "swipe_down";
        case TG_PINCH_IN:          return "pinch_in";
        case TG_PINCH_OUT:         return "pinch_out";
        case TG_TWO_FINGER_ROTATE: return "two_finger_rotate";
        case TG_THREE_FINGER_TAP:  return "three_finger_tap";
    }
    return "unknown";
}
