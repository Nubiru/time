/* user_context.c -- User Context Tracker
 *
 * Pure implementation of session-level user behavior tracking.
 * All functions are pure: no GL, no malloc, no globals, no side effects. */

#include "user_context.h"
#include <string.h>

/* Affinity bump when a system is toggled */
#define UC_AFFINITY_BUMP   0.1
/* Decay multiplier applied to all affinities on non-toggle actions */
#define UC_AFFINITY_DECAY  0.99

/* ---- helpers ---- */

static int find_max_index_d(const double *arr, int len)
{
    int best = -1;
    double best_val = 0.0;
    for (int i = 0; i < len; i++) {
        if (arr[i] > best_val) {
            best_val = arr[i];
            best = i;
        }
    }
    return best;
}

static int clamp_int(int val, int lo, int hi)
{
    if (val < lo) return lo;
    if (val > hi) return hi;
    return val;
}

static double clamp_d(double val, double lo, double hi)
{
    if (val < lo) return lo;
    if (val > hi) return hi;
    return val;
}

/* Add elapsed time from the previous action to appropriate session buckets.
 * prev_ts is the timestamp of the most recent prior action (or 0 if none).
 * cur_ts is the timestamp of the current action. */
static uc_session_t add_elapsed(uc_session_t s, double prev_ts, double cur_ts,
                                 int prev_view, int prev_scale,
                                 int active_system)
{
    double elapsed = cur_ts - prev_ts;
    if (elapsed <= 0.0) return s;

    if (prev_view >= 0 && prev_view < UC_MAX_VIEWS) {
        s.view_time[prev_view] += elapsed;
    }
    if (prev_scale >= 0 && prev_scale < UC_MAX_SCALES) {
        s.scale_time[prev_scale] += elapsed;
    }
    if (active_system >= 0 && active_system < UC_MAX_SYSTEMS) {
        s.system_time[active_system] += elapsed;
    }
    s.duration_sec = cur_ts;
    return s;
}

/* Recompute dominant fields from time arrays. */
static uc_session_t recompute_dominants(uc_session_t s)
{
    s.dominant_system = find_max_index_d(s.system_time, UC_MAX_SYSTEMS);
    s.dominant_view   = find_max_index_d(s.view_time, UC_MAX_VIEWS);
    s.dominant_scale  = find_max_index_d(s.scale_time, UC_MAX_SCALES);
    return s;
}

/* Count distinct values for view/scale changes in recent history. */
static int count_distinct_nav(const uc_context_t *ctx, int look_back)
{
    if (ctx->history_count == 0) return 0;

    int n = ctx->history_count < look_back ? ctx->history_count : look_back;
    /* Track distinct (type, value) pairs for view/scale changes */
    int seen_values[20]; /* type*10 + value gives unique key */
    int seen_count = 0;

    for (int i = 0; i < n; i++) {
        int idx = ctx->history_head - 1 - i;
        if (idx < 0) idx += UC_MAX_HISTORY;
        const uc_action_t *a = &ctx->history[idx];
        if (a->type == UC_ACT_VIEW_CHANGE || a->type == UC_ACT_SCALE_CHANGE) {
            int key = (int)a->type * 10 + a->value;
            int found = 0;
            for (int j = 0; j < seen_count; j++) {
                if (seen_values[j] == key) { found = 1; break; }
            }
            if (!found && seen_count < 20) {
                seen_values[seen_count++] = key;
            }
        }
    }
    return seen_count;
}

/* Get timestamp of most recent action, or 0.0 if no history. */
static double last_timestamp(const uc_context_t *ctx)
{
    if (ctx->history_count == 0) return 0.0;
    int idx = ctx->history_head - 1;
    if (idx < 0) idx += UC_MAX_HISTORY;
    return ctx->history[idx].timestamp;
}

/* Get the most recently active system from the last SYSTEM_TOGGLE action. */
static int last_active_system(const uc_context_t *ctx)
{
    for (int i = 0; i < ctx->history_count && i < UC_MAX_HISTORY; i++) {
        int idx = ctx->history_head - 1 - i;
        if (idx < 0) idx += UC_MAX_HISTORY;
        if (ctx->history[idx].type == UC_ACT_SYSTEM_TOGGLE) {
            return ctx->history[idx].value;
        }
    }
    return -1;
}

/* ---- public API ---- */

uc_context_t uc_default(void)
{
    uc_context_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    ctx.session.dominant_system = -1;
    ctx.session.dominant_view   = -1;
    ctx.session.dominant_scale  = -1;
    return ctx;
}

uc_context_t uc_set_birth(uc_context_t ctx, const birth_profile_t *profile)
{
    if (!profile) return ctx;
    ctx.birth_kin      = profile->tzolkin.kin;
    ctx.birth_seal     = profile->tzolkin.seal;
    ctx.birth_tone     = profile->tzolkin.tone;
    ctx.birth_hexagram = profile->iching.king_wen;
    ctx.birth_sign     = profile->astrology.sun_sign;
    ctx.birth_gate     = profile->astrology.hd_sun_gate;
    return ctx;
}

uc_context_t uc_update(uc_context_t ctx, uc_action_t action)
{
    double prev_ts = last_timestamp(&ctx);
    int active_sys = last_active_system(&ctx);

    /* Add elapsed time for the period between last action and this one */
    ctx.session = add_elapsed(ctx.session, prev_ts, action.timestamp,
                              ctx.current_view, ctx.current_scale,
                              active_sys);

    /* Process the action */
    switch (action.type) {
    case UC_ACT_VIEW_CHANGE:
        ctx.current_view = clamp_int(action.value, 0, UC_MAX_VIEWS - 1);
        break;
    case UC_ACT_SCALE_CHANGE:
        ctx.current_scale = clamp_int(action.value, 0, UC_MAX_SCALES - 1);
        break;
    case UC_ACT_SYSTEM_TOGGLE: {
        int sid = clamp_int(action.value, 0, UC_MAX_SYSTEMS - 1);
        action.value = sid;
        ctx.system_affinity[sid] += UC_AFFINITY_BUMP;
        ctx.system_affinity[sid] = clamp_d(ctx.system_affinity[sid], 0.0, 1.0);
        ctx.active_systems ^= (1u << (unsigned)sid);
        break;
    }
    default:
        break;
    }

    /* Decay affinities on non-toggle actions */
    if (action.type != UC_ACT_SYSTEM_TOGGLE) {
        for (int i = 0; i < UC_MAX_SYSTEMS; i++) {
            ctx.system_affinity[i] *= UC_AFFINITY_DECAY;
            ctx.system_affinity[i] = clamp_d(ctx.system_affinity[i], 0.0, 1.0);
        }
    }

    /* Record in history ring buffer */
    ctx.history[ctx.history_head] = action;
    ctx.history_head = (ctx.history_head + 1) % UC_MAX_HISTORY;
    if (ctx.history_count < UC_MAX_HISTORY) {
        ctx.history_count++;
    }

    ctx.session.action_count++;
    ctx.session = recompute_dominants(ctx.session);

    return ctx;
}

int uc_top_systems(const uc_context_t *ctx, int *out_ids, int max)
{
    if (!ctx || !out_ids || max <= 0) return 0;

    /* Collect systems with non-zero affinity */
    int indices[UC_MAX_SYSTEMS];
    double values[UC_MAX_SYSTEMS];
    int n = 0;
    for (int i = 0; i < UC_MAX_SYSTEMS; i++) {
        if (ctx->system_affinity[i] > 0.0) {
            indices[n] = i;
            values[n] = ctx->system_affinity[i];
            n++;
        }
    }
    if (n == 0) return 0;

    /* Simple insertion sort descending by value */
    for (int i = 1; i < n; i++) {
        double v = values[i];
        int idx = indices[i];
        int j = i - 1;
        while (j >= 0 && values[j] < v) {
            values[j + 1] = values[j];
            indices[j + 1] = indices[j];
            j--;
        }
        values[j + 1] = v;
        indices[j + 1] = idx;
    }

    int out_count = n < max ? n : max;
    for (int i = 0; i < out_count; i++) {
        out_ids[i] = indices[i];
    }
    return out_count;
}

int uc_is_exploring(const uc_context_t *ctx)
{
    if (!ctx) return 0;
    int distinct = count_distinct_nav(ctx, 10);
    return distinct > 5 ? 1 : 0;
}

int uc_is_focused(const uc_context_t *ctx)
{
    if (!ctx) return 1;
    int distinct = count_distinct_nav(ctx, 10);
    return distinct < 2 ? 1 : 0;
}

we_context_t uc_to_wisdom_context(const uc_context_t *ctx)
{
    we_context_t wc;
    memset(&wc, 0, sizeof(wc));
    if (!ctx) return wc;

    /* Map view: UC views map directly to WE views (same order) */
    wc.view = (we_view_t)clamp_int(ctx->current_view, 0, WE_VIEW_COUNT - 1);

    /* Map scale: UC scales map directly to WE scales (clamped) */
    wc.scale = (we_scale_t)clamp_int(ctx->current_scale, 0, WE_SCALE_COUNT - 1);

    wc.active_systems = ctx->active_systems;
    wc.user_seal = ctx->birth_seal;
    wc.user_tone = ctx->birth_tone;
    wc.convergence_score = 0;

    return wc;
}

ff_context_t uc_to_facts_context(const uc_context_t *ctx)
{
    ff_context_t fc;
    memset(&fc, 0, sizeof(fc));
    if (!ctx) return fc;

    fc.view = ctx->current_view;
    fc.scale = ctx->current_scale;
    fc.active_systems = ctx->active_systems;
    fc.user_seal = ctx->birth_seal;
    fc.convergence_active = 0;

    return fc;
}

double uc_system_time(const uc_context_t *ctx, int system_id)
{
    if (!ctx || system_id < 0 || system_id >= UC_MAX_SYSTEMS) return 0.0;
    return ctx->session.system_time[system_id];
}

double uc_session_duration(const uc_context_t *ctx)
{
    if (!ctx) return 0.0;
    return ctx->session.duration_sec;
}

int uc_dominant_system(const uc_context_t *ctx)
{
    if (!ctx) return -1;
    return ctx->session.dominant_system;
}

int uc_dominant_view(const uc_context_t *ctx)
{
    if (!ctx) return -1;
    return ctx->session.dominant_view;
}

uc_context_t uc_reset_session(uc_context_t ctx)
{
    memset(&ctx.session, 0, sizeof(ctx.session));
    ctx.session.dominant_system = -1;
    ctx.session.dominant_view   = -1;
    ctx.session.dominant_scale  = -1;
    ctx.history_count = 0;
    ctx.history_head = 0;
    ctx.current_view = 0;
    ctx.current_scale = 0;
    ctx.active_systems = 0;
    return ctx;
}

const char *uc_action_name(uc_action_type_t type)
{
    static const char *names[] = {
        "view_change",
        "scale_change",
        "system_toggle",
        "card_view",
        "fact_read",
        "quote_read",
        "time_travel",
        "location_set"
    };
    if (type < 0 || type >= UC_ACT_COUNT) return "unknown";
    return names[type];
}

int uc_action_count(const uc_context_t *ctx)
{
    if (!ctx) return 0;
    return ctx->session.action_count;
}
