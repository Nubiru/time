/* view_state.c — Master view state for the render loop.
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "view_state.h"

view_state_t vs_init(void) {
    view_state_t s;
    s.current_view    = 0;   /* VIEW_SPACE */
    s.target_view     = 0;
    s.transition_t    = 1.0f;
    s.focus_mode      = 0;   /* FOCUS_MODE_OVERVIEW */
    s.lod_tier        = 0;   /* LOD_ULTRA */
    s.is_transitioning = 0;
    return s;
}

view_state_t vs_set_view(view_state_t state, int view_id) {
    if (view_id == state.current_view && !state.is_transitioning) {
        return state;
    }
    if (view_id == state.target_view && state.is_transitioning) {
        return state;
    }
    state.target_view     = view_id;
    state.transition_t    = 0.0f;
    state.is_transitioning = 1;
    return state;
}

view_state_t vs_set_focus(view_state_t state, int focus_mode) {
    state.focus_mode = focus_mode;
    return state;
}

view_state_t vs_set_lod(view_state_t state, int lod_tier) {
    state.lod_tier = lod_tier;
    return state;
}

view_state_t vs_tick(view_state_t state, float dt) {
    if (!state.is_transitioning) {
        return state;
    }
    if (dt <= 0.0f) {
        return state;
    }

    state.transition_t += dt / VS_TRANSITION_DURATION;

    if (state.transition_t >= 1.0f) {
        state.transition_t    = 1.0f;
        state.current_view    = state.target_view;
        state.is_transitioning = 0;
    }

    return state;
}

pass_schedule_t vs_schedule(const view_state_t *state) {
    pass_schedule_t sched = ps_for_view(state->current_view, state->lod_tier);
    sched = ps_apply_focus(sched, state->focus_mode);
    return sched;
}

pass_schedule_t vs_blended_schedule(const view_state_t *state) {
    if (!state->is_transitioning) {
        return vs_schedule(state);
    }

    /* Get schedules for both views (without focus — blend raw, then apply focus) */
    pass_schedule_t from = ps_for_view(state->current_view, state->lod_tier);
    pass_schedule_t to   = ps_for_view(state->target_view, state->lod_tier);

    float t = state->transition_t;

    /* Blend: lerp opacity between the two schedules */
    pass_schedule_t blended;
    int active = 0;
    for (int i = 0; i < PASS_COUNT; i++) {
        /* A pass is enabled if either view has it enabled */
        int en = from.passes[i].enabled || to.passes[i].enabled;
        blended.passes[i].enabled = en;

        /* Opacity: lerp between from and to.
         * If disabled in one view, treat its opacity as 0. */
        float o_from = from.passes[i].enabled ? from.passes[i].opacity_scale : 0.0f;
        float o_to   = to.passes[i].enabled   ? to.passes[i].opacity_scale   : 0.0f;
        blended.passes[i].opacity_scale = o_from * (1.0f - t) + o_to * t;

        /* Use target view's draw order past 50%, else source */
        blended.passes[i].draw_order = (t >= 0.5f)
            ? to.passes[i].draw_order
            : from.passes[i].draw_order;

        if (en) {
            active++;
        }
    }
    blended.active_count = active;

    /* Apply focus mode on top of blended result */
    blended = ps_apply_focus(blended, state->focus_mode);

    return blended;
}

int vs_is_transitioning(const view_state_t *state) {
    return state->is_transitioning;
}

float vs_transition_progress(const view_state_t *state) {
    return state->transition_t;
}

int vs_effective_view(const view_state_t *state) {
    if (!state->is_transitioning) {
        return state->current_view;
    }
    return (state->transition_t >= 0.5f) ? state->target_view : state->current_view;
}
