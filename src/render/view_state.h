/* view_state.h — Master view state for the render loop.
 *
 * Tracks current view (Space/Earth/Galaxy), focus mode, LOD tier,
 * and view transition progress. Produces a single per-frame output
 * that the render loop reads.
 *
 * The render loop does:
 *   1. vs_tick(state, dt) — advance time
 *   2. vs_schedule(state) — get pass schedule
 *   3. Iterate schedule, draw enabled passes
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 * State is value-type (returned by copy). */

#ifndef TIME_VIEW_STATE_H
#define TIME_VIEW_STATE_H

#include "pass_schedule.h"

/* Transition duration (phi-based): 1.618 seconds */
#define VS_TRANSITION_DURATION 1.618f

/* Focus transition duration: 0.5 seconds (quick, responsive) */
#define VS_FOCUS_TRANSITION_DURATION 0.5f

typedef struct {
    int   current_view;      /* view_id_t value */
    int   target_view;       /* view transitioning to (same as current if idle) */
    float transition_t;      /* 0.0 = at current, 1.0 = arrived at target */
    int   focus_mode;        /* focus_mode_e value */
    int   target_focus;      /* focus mode transitioning to */
    float focus_blend;       /* 0.0 = old focus, 1.0 = new focus fully active */
    int   focus_transitioning; /* 1 if focus change in progress */
    int   lod_tier;          /* lod_tier_t value */
    int   is_transitioning;  /* 1 if view change in progress */
} view_state_t;

/* Initialize to Space View, no focus, LOD ULTRA. */
view_state_t vs_init(void);

/* Request a view change. Starts transition if different from current. */
view_state_t vs_set_view(view_state_t state, int view_id);

/* Set focus mode (no transition — instant). */
view_state_t vs_set_focus(view_state_t state, int focus_mode);

/* Set LOD tier. */
view_state_t vs_set_lod(view_state_t state, int lod_tier);

/* Advance transition timer. Call each frame with dt in seconds. */
view_state_t vs_tick(view_state_t state, float dt);

/* Get current pass schedule based on view + focus + LOD. */
pass_schedule_t vs_schedule(const view_state_t *state);

/* Get blended pass schedule during transition (lerps opacity between views). */
pass_schedule_t vs_blended_schedule(const view_state_t *state);

/* Is a view transition in progress? */
int vs_is_transitioning(const view_state_t *state);

/* Is a focus transition in progress? */
int vs_is_focus_transitioning(const view_state_t *state);

/* Focus blend progress (0.0 to 1.0). Returns 1.0 if not transitioning. */
float vs_focus_blend(const view_state_t *state);

/* Transition progress (0.0 to 1.0). Returns 1.0 if not transitioning. */
float vs_transition_progress(const view_state_t *state);

/* Current effective view (target view once transition > 50%). */
int vs_effective_view(const view_state_t *state);

#endif /* TIME_VIEW_STATE_H */
