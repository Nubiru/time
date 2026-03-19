/* exploration_progress.c — User exploration journey tracking.
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#include "exploration_progress.h"
#include <string.h>
#include <stdio.h>

/* ===== Static Data ===== */

static const char *const s_names[EP_MS_COUNT] = {
    "First Visit",
    "First System",
    "Half Explorer",
    "Full Explorer",
    "Week Streak",
    "Month Streak",
    "Century Streak",
    "Year Streak",
    "First Bookmark",
    "First Cycle",
    "Deep Dive"
};

static const char *const s_descs[EP_MS_COUNT] = {
    "You opened Time for the first time",
    "You explored your first knowledge system",
    "You've explored half of all systems",
    "You've explored every system",
    "7 consecutive days with Time",
    "30 consecutive days with Time",
    "100 consecutive days with Time",
    "A full year of daily visits",
    "You saved your first moment",
    "You started tracking your first cycle",
    "30 minutes immersed in one session"
};

/* ===== State ===== */

ep_state_t ep_default_state(void) {
    ep_state_t s;
    memset(&s, 0, sizeof(s));
    s.total_systems = EP_TOTAL_SYSTEMS;
    return s;
}

/* ===== Internal: check milestone condition ===== */

static int check_milestone(const ep_state_t *state, ep_milestone_t ms) {
    switch (ms) {
        case EP_MS_FIRST_VISIT:    return state->total_visits >= 1;
        case EP_MS_FIRST_SYSTEM:   return state->explored_count >= 1;
        case EP_MS_HALF_SYSTEMS:   return state->explored_count >= (EP_TOTAL_SYSTEMS / 2);
        case EP_MS_ALL_SYSTEMS:    return state->explored_count >= EP_TOTAL_SYSTEMS;
        case EP_MS_STREAK_7:       return state->longest_streak >= 7;
        case EP_MS_STREAK_30:      return state->longest_streak >= 30;
        case EP_MS_STREAK_100:     return state->longest_streak >= 100;
        case EP_MS_STREAK_365:     return state->longest_streak >= 365;
        case EP_MS_FIRST_BOOKMARK: return state->bookmark_count >= 1;
        case EP_MS_FIRST_CYCLE:    return state->cycle_count >= 1;
        case EP_MS_DEEP_DIVE:     return state->longest_session_sec >= EP_DEEP_DIVE_SEC;
        default:                   return 0;
    }
}

/* ===== Compute ===== */

ep_progress_t ep_compute(const ep_state_t *state) {
    ep_progress_t p;
    memset(&p, 0, sizeof(p));

    if (!state) {
        return p;
    }

    /* Coverage */
    int total = state->total_systems > 0 ? state->total_systems : EP_TOTAL_SYSTEMS;
    p.system_coverage = (double)state->explored_count / (double)total;
    p.systems_remaining = total - state->explored_count;
    if (p.systems_remaining < 0) p.systems_remaining = 0;

    /* Streaks */
    p.current_streak = state->consecutive_days;
    p.longest_streak = state->longest_streak;
    p.streak_active = state->consecutive_days > 0 ? 1 : 0;

    /* Milestones */
    p.milestone_count = 0;
    for (int i = 0; i < EP_MS_COUNT; i++) {
        p.milestones[i] = check_milestone(state, (ep_milestone_t)i);
        if (p.milestones[i]) {
            p.milestone_count++;
        }
    }

    /* Significance */
    if (state->explored_count >= EP_TOTAL_SYSTEMS ||
        state->longest_streak >= 365) {
        p.insight_significance = 2; /* celebration */
    } else if (state->explored_count >= (EP_TOTAL_SYSTEMS / 2) ||
               state->longest_streak >= 30) {
        p.insight_significance = 1; /* notable */
    } else {
        p.insight_significance = 0; /* routine */
    }

    /* Pre-compute insight text */
    ep_format_insight(&p, p.insight, EP_INSIGHT_MAX);

    return p;
}

/* ===== Milestone Queries ===== */

int ep_has_milestone(const ep_progress_t *progress, ep_milestone_t ms) {
    if (!progress || ms < 0 || ms >= EP_MS_COUNT) {
        return 0;
    }
    return progress->milestones[ms];
}

const char *ep_milestone_name(ep_milestone_t ms) {
    if (ms < 0 || ms >= EP_MS_COUNT) {
        return "?";
    }
    return s_names[ms];
}

const char *ep_milestone_desc(ep_milestone_t ms) {
    if (ms < 0 || ms >= EP_MS_COUNT) {
        return "?";
    }
    return s_descs[ms];
}

int ep_earned_count(const ep_progress_t *progress) {
    if (!progress) return 0;
    return progress->milestone_count;
}

ep_milestone_t ep_next_milestone(const ep_state_t *state) {
    if (!state) return EP_MS_COUNT;

    for (int i = 0; i < EP_MS_COUNT; i++) {
        if (!check_milestone(state, (ep_milestone_t)i)) {
            return (ep_milestone_t)i;
        }
    }
    return EP_MS_COUNT;
}

/* ===== Format ===== */

int ep_format_insight(const ep_progress_t *progress, char *buf, int buf_size) {
    if (!buf || buf_size <= 0) return 0;
    buf[0] = '\0';
    if (!progress) return 0;

    int n = 0;

    if (progress->system_coverage >= 1.0) {
        n = snprintf(buf, (size_t)buf_size,
                     "You've explored all %d systems", EP_TOTAL_SYSTEMS);
    } else if (progress->current_streak >= 7) {
        int explored = EP_TOTAL_SYSTEMS - progress->systems_remaining;
        n = snprintf(buf, (size_t)buf_size,
                     "%d consecutive days with Time, %d of %d systems explored",
                     progress->current_streak, explored, EP_TOTAL_SYSTEMS);
    } else if (progress->systems_remaining < EP_TOTAL_SYSTEMS) {
        int explored = EP_TOTAL_SYSTEMS - progress->systems_remaining;
        n = snprintf(buf, (size_t)buf_size,
                     "You've explored %d of %d systems",
                     explored, EP_TOTAL_SYSTEMS);
    } else {
        n = snprintf(buf, (size_t)buf_size, "Your journey begins");
    }

    if (n < 0) {
        buf[0] = '\0';
        return 0;
    }
    if (n >= buf_size) {
        n = buf_size - 1;
    }
    return n;
}
