/* user_context.h -- User Context Tracker
 *
 * Tracks user behavior for contextual personalization. Maintains
 * session-level navigation patterns (views, scales, systems), integrates
 * birth profile data, and computes behavioral signals.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects.
 * All update functions take context by value and return modified copy. */

#ifndef TIME_USER_CONTEXT_H
#define TIME_USER_CONTEXT_H

#include "birth_profile.h"
#include "fun_facts.h"
#include "wisdom_engine.h"

#define UC_MAX_SYSTEMS      14
#define UC_MAX_VIEWS         6
#define UC_MAX_SCALES        7
#define UC_MAX_HISTORY      32

/* Action types */
typedef enum {
    UC_ACT_VIEW_CHANGE = 0,
    UC_ACT_SCALE_CHANGE,
    UC_ACT_SYSTEM_TOGGLE,
    UC_ACT_CARD_VIEW,
    UC_ACT_FACT_READ,
    UC_ACT_QUOTE_READ,
    UC_ACT_TIME_TRAVEL,
    UC_ACT_LOCATION_SET,
    UC_ACT_COUNT
} uc_action_type_t;

typedef struct {
    uc_action_type_t type;
    int value;
    double timestamp;
} uc_action_t;

typedef struct {
    double system_time[UC_MAX_SYSTEMS];
    double view_time[UC_MAX_VIEWS];
    double scale_time[UC_MAX_SCALES];
    int action_count;
    double duration_sec;
    int dominant_system;
    int dominant_view;
    int dominant_scale;
} uc_session_t;

typedef struct {
    int birth_kin;
    int birth_seal;
    int birth_tone;
    int birth_hexagram;
    int birth_sign;
    int birth_gate;
    int current_view;
    int current_scale;
    unsigned int active_systems;
    uc_session_t session;
    double system_affinity[UC_MAX_SYSTEMS];
    uc_action_t history[UC_MAX_HISTORY];
    int history_count;
    int history_head;
} uc_context_t;

/* Create a default (zeroed) user context. */
uc_context_t uc_default(void);

/* Set birth data from a computed birth profile. */
uc_context_t uc_set_birth(uc_context_t ctx, const birth_profile_t *profile);

/* Process a user action and return updated context. */
uc_context_t uc_update(uc_context_t ctx, uc_action_t action);

/* Get top N systems by affinity. Returns count written (<= max). */
int uc_top_systems(const uc_context_t *ctx, int *out_ids, int max);

/* True if user is exploring (many distinct view/scale changes recently). */
int uc_is_exploring(const uc_context_t *ctx);

/* True if user is focused (few distinct view/scale changes recently). */
int uc_is_focused(const uc_context_t *ctx);

/* Convert user context to wisdom engine context. */
we_context_t uc_to_wisdom_context(const uc_context_t *ctx);

/* Convert user context to fun facts context. */
ff_context_t uc_to_facts_context(const uc_context_t *ctx);

/* Get accumulated time (seconds) for a given system. */
double uc_system_time(const uc_context_t *ctx, int system_id);

/* Get total session duration in seconds. */
double uc_session_duration(const uc_context_t *ctx);

/* Get dominant system index, or -1 if none. */
int uc_dominant_system(const uc_context_t *ctx);

/* Get dominant view index, or -1 if none. */
int uc_dominant_view(const uc_context_t *ctx);

/* Reset session data (keeps birth and affinities). */
uc_context_t uc_reset_session(uc_context_t ctx);

/* Get human-readable name for an action type. */
const char *uc_action_name(uc_action_type_t type);

/* Get total action count for this session. */
int uc_action_count(const uc_context_t *ctx);

#endif /* TIME_USER_CONTEXT_H */
