/* wisdom_display.h -- Wisdom display trigger and formatting.
 *
 * Determines WHEN wisdom should appear (trigger conditions) and
 * formats the display text (quote + attribution). Uses wisdom_engine
 * for quote selection and sensitivity for display rules.
 *
 * Trigger conditions (not random, not constant):
 * - Convergence event detected -> unity/harmony quote
 * - User enters new view -> system-relevant quote
 * - User idle for threshold -> contemplative quote
 * - Birth kin alignment -> culturally matched quote
 *
 * Timing (phi-based):
 * - Fade in: 1.5s -> Hold: 8.0s -> Fade out: 1.5s
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_WISDOM_DISPLAY_H
#define TIME_WISDOM_DISPLAY_H

#include "../systems/unified/wisdom_engine.h"
#include "../systems/unified/wisdom.h"
#include "sensitivity.h"

#define WD_MAX_DISPLAY_LEN 512
#define WD_MAX_SEEN 64

/* Why wisdom was triggered */
typedef enum {
    WD_TRIGGER_NONE = 0,        /* no trigger active */
    WD_TRIGGER_CONVERGENCE,     /* cross-system convergence event */
    WD_TRIGGER_VIEW_CHANGE,     /* user switched view mode */
    WD_TRIGGER_IDLE,            /* user idle for threshold time */
    WD_TRIGGER_KIN_ALIGN,       /* birth kin alignment with current day */
    WD_TRIGGER_MANUAL,          /* user requested wisdom */
    WD_TRIGGER_COUNT
} wd_trigger_t;

/* Animation phase */
typedef enum {
    WD_PHASE_HIDDEN = 0,
    WD_PHASE_FADE_IN,
    WD_PHASE_HOLD,
    WD_PHASE_FADE_OUT,
    WD_PHASE_COUNT
} wd_phase_t;

/* Timing constants (phi-based) */
#define WD_FADE_IN_DURATION   1.5
#define WD_HOLD_DURATION      8.0
#define WD_FADE_OUT_DURATION  1.5
#define WD_TOTAL_DURATION    (WD_FADE_IN_DURATION + WD_HOLD_DURATION + WD_FADE_OUT_DURATION)
#define WD_IDLE_THRESHOLD    30.0   /* seconds before idle trigger */
#define WD_COOLDOWN          45.0   /* seconds between wisdom displays */

/* Current wisdom display state (value type, no pointers to free) */
typedef struct {
    int quote_index;            /* -1 if no quote active */
    wd_trigger_t trigger;       /* why this was triggered */
    wd_phase_t phase;           /* current animation phase */
    double elapsed;             /* seconds into current display */
    double opacity;             /* 0.0-1.0 for fade animation */
    double cooldown_remaining;  /* seconds until next display allowed */
    int seen[WD_MAX_SEEN];      /* indices of seen quotes */
    int seen_count;
} wd_state_t;

/* Initialize a fresh display state. */
wd_state_t wd_init(void);

/* Check if wisdom should trigger now. Returns trigger type, or NONE.
 * Does NOT modify state -- caller uses result to call wd_activate(). */
wd_trigger_t wd_should_trigger(const wd_state_t *state,
                                const we_context_t *ctx,
                                int convergence_active,
                                int view_just_changed,
                                double idle_seconds);

/* Activate a wisdom display. Selects best quote for context.
 * Modifies state: sets quote_index, trigger, starts fade-in.
 * Returns quote_index selected, or -1 if none available. */
int wd_activate(wd_state_t *state, wd_trigger_t trigger,
                const we_context_t *ctx);

/* Update display state by dt seconds. Advances phase/opacity.
 * Returns current phase. */
wd_phase_t wd_update(wd_state_t *state, double dt);

/* Format the current wisdom quote for display.
 * Returns pointer to static buffer: "quote text\n-- Author, Work (Year)"
 * Returns "" if no quote active. */
const char *wd_format_quote(const wd_state_t *state);

/* Format just the attribution line: "-- Author, Work (Year BCE/CE)"
 * Returns "" if no quote active. */
const char *wd_format_attribution(const wd_state_t *state);

/* Get trigger name string. Returns "Unknown" for invalid. */
const char *wd_trigger_name(wd_trigger_t trigger);

/* Get phase name string. Returns "Unknown" for invalid. */
const char *wd_phase_name(wd_phase_t phase);

/* Is the display currently visible (not hidden, opacity > 0)? */
int wd_is_visible(const wd_state_t *state);

/* Reset seen history (start fresh rotation). */
void wd_reset_seen(wd_state_t *state);

#endif /* TIME_WISDOM_DISPLAY_H */
