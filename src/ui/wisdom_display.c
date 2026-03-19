/* wisdom_display.c -- Wisdom display trigger and formatting.
 *
 * Determines WHEN wisdom should appear and formats display text.
 * Uses wisdom_engine for quote selection.
 *
 * Pure module: no GL, no malloc, no globals.
 * Uses file-scope static buffers for formatted output. */

#include "wisdom_display.h"

#include <stdio.h>
#include <string.h>

/* ---- static buffers ---- */

static char s_quote_buf[WD_MAX_DISPLAY_LEN];
static char s_attr_buf[256];

/* ---- name tables ---- */

static const char *TRIGGER_NAMES[WD_TRIGGER_COUNT] = {
    "None",
    "Convergence",
    "View Change",
    "Idle",
    "Kin Align",
    "Manual"
};

static const char *PHASE_NAMES[WD_PHASE_COUNT] = {
    "Hidden",
    "Fade In",
    "Hold",
    "Fade Out"
};

/* ---- helpers ---- */

/* Clamp a double to [lo, hi]. */
static double clamp_d(double v, double lo, double hi)
{
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

/* Format year as "Year BCE" or "Year CE". Writes to buf, returns buf. */
static const char *format_year(int year, char *buf, int buf_size)
{
    if (year < 0) {
        snprintf(buf, (size_t)buf_size, "%d BCE", -year);
    } else {
        snprintf(buf, (size_t)buf_size, "%d CE", year);
    }
    return buf;
}

/* ---- public API ---- */

wd_state_t wd_init(void)
{
    wd_state_t s;
    memset(&s, 0, sizeof(s));
    s.quote_index = -1;
    s.trigger = WD_TRIGGER_NONE;
    s.phase = WD_PHASE_HIDDEN;
    s.elapsed = 0.0;
    s.opacity = 0.0;
    s.cooldown_remaining = 0.0;
    s.seen_count = 0;
    return s;
}

wd_trigger_t wd_should_trigger(const wd_state_t *state,
                                const we_context_t *ctx,
                                int convergence_active,
                                int view_just_changed,
                                double idle_seconds)
{
    (void)ctx; /* used for future kin alignment check */

    if (state == NULL) return WD_TRIGGER_NONE;

    /* Cannot trigger while displaying or in cooldown */
    if (state->phase != WD_PHASE_HIDDEN) return WD_TRIGGER_NONE;
    if (state->cooldown_remaining > 0.0) return WD_TRIGGER_NONE;

    /* Priority order */
    if (convergence_active) return WD_TRIGGER_CONVERGENCE;
    if (view_just_changed)  return WD_TRIGGER_VIEW_CHANGE;
    if (idle_seconds >= WD_IDLE_THRESHOLD) return WD_TRIGGER_IDLE;

    return WD_TRIGGER_NONE;
}

int wd_activate(wd_state_t *state, wd_trigger_t trigger,
                const we_context_t *ctx)
{
    if (state == NULL || ctx == NULL) return -1;

    int idx = we_best_quote(ctx, state->seen, state->seen_count);
    if (idx < 0) return -1;

    state->quote_index = idx;
    state->trigger = trigger;
    state->phase = WD_PHASE_FADE_IN;
    state->elapsed = 0.0;
    state->opacity = 0.0;

    /* Add to seen list (wraps if full) */
    if (state->seen_count < WD_MAX_SEEN) {
        state->seen[state->seen_count++] = idx;
    }

    return idx;
}

wd_phase_t wd_update(wd_state_t *state, double dt)
{
    if (state == NULL) return WD_PHASE_HIDDEN;

    if (state->phase == WD_PHASE_HIDDEN) {
        /* Decrease cooldown while hidden */
        if (state->cooldown_remaining > 0.0) {
            state->cooldown_remaining -= dt;
            if (state->cooldown_remaining < 0.0) {
                state->cooldown_remaining = 0.0;
            }
        }
        return WD_PHASE_HIDDEN;
    }

    state->elapsed += dt;

    if (state->elapsed < WD_FADE_IN_DURATION) {
        /* Fade in */
        state->phase = WD_PHASE_FADE_IN;
        state->opacity = clamp_d(state->elapsed / WD_FADE_IN_DURATION,
                                  0.0, 1.0);
    } else if (state->elapsed < WD_FADE_IN_DURATION + WD_HOLD_DURATION) {
        /* Hold */
        state->phase = WD_PHASE_HOLD;
        state->opacity = 1.0;
    } else if (state->elapsed < WD_TOTAL_DURATION) {
        /* Fade out */
        state->phase = WD_PHASE_FADE_OUT;
        double fade_progress = state->elapsed - WD_FADE_IN_DURATION
                               - WD_HOLD_DURATION;
        state->opacity = clamp_d(1.0 - fade_progress / WD_FADE_OUT_DURATION,
                                  0.0, 1.0);
    } else {
        /* Done — return to hidden */
        state->phase = WD_PHASE_HIDDEN;
        state->opacity = 0.0;
        state->quote_index = -1;
        state->trigger = WD_TRIGGER_NONE;
        state->elapsed = 0.0;
        state->cooldown_remaining = WD_COOLDOWN;
    }

    return state->phase;
}

const char *wd_format_quote(const wd_state_t *state)
{
    if (state == NULL || state->quote_index < 0) return "";

    wisdom_t q = wisdom_quote_get(state->quote_index);
    if (q.id < 0) return "";

    char year_buf[32];
    format_year(q.year, year_buf, (int)sizeof(year_buf));

    if (q.work != NULL) {
        snprintf(s_quote_buf, sizeof(s_quote_buf),
                 "%s\n\xe2\x80\x94 %s, %s (%s)",
                 q.text, q.author, q.work, year_buf);
    } else {
        snprintf(s_quote_buf, sizeof(s_quote_buf),
                 "%s\n\xe2\x80\x94 %s (%s)",
                 q.text, q.author, year_buf);
    }

    return s_quote_buf;
}

const char *wd_format_attribution(const wd_state_t *state)
{
    if (state == NULL || state->quote_index < 0) return "";

    wisdom_t q = wisdom_quote_get(state->quote_index);
    if (q.id < 0) return "";

    char year_buf[32];
    format_year(q.year, year_buf, (int)sizeof(year_buf));

    if (q.work != NULL) {
        snprintf(s_attr_buf, sizeof(s_attr_buf),
                 "\xe2\x80\x94 %s, %s (%s)",
                 q.author, q.work, year_buf);
    } else {
        snprintf(s_attr_buf, sizeof(s_attr_buf),
                 "\xe2\x80\x94 %s (%s)",
                 q.author, year_buf);
    }

    return s_attr_buf;
}

const char *wd_trigger_name(wd_trigger_t trigger)
{
    if (trigger < 0 || trigger >= WD_TRIGGER_COUNT) return "Unknown";
    return TRIGGER_NAMES[trigger];
}

const char *wd_phase_name(wd_phase_t phase)
{
    if (phase < 0 || phase >= WD_PHASE_COUNT) return "Unknown";
    return PHASE_NAMES[phase];
}

int wd_is_visible(const wd_state_t *state)
{
    if (state == NULL) return 0;
    return (state->phase != WD_PHASE_HIDDEN && state->opacity > 0.0) ? 1 : 0;
}

void wd_reset_seen(wd_state_t *state)
{
    if (state == NULL) return;
    state->seen_count = 0;
}
