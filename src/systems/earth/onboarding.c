/* onboarding.c — Welcome Journey state machine.
 * Pure functions. No GL, no malloc, no globals, no side effects. */

#include "onboarding.h"

/* --- State name table --- */

static const char *s_state_names[OB_STATE_COUNT] = {
    "Exploring",
    "Invitation",
    "Birth Date",
    "Birth Place",
    "Reveal",
    "Interests",
    "Complete"
};

/* --- Scene dim table --- */

static const float s_scene_dim[OB_STATE_COUNT] = {
    0.0f,    /* OB_EXPLORING */
    0.0f,    /* OB_INVITATION */
    0.618f,  /* OB_BIRTH_DATE */
    0.618f,  /* OB_BIRTH_PLACE */
    0.618f,  /* OB_REVEAL */
    0.618f,  /* OB_INTERESTS */
    0.3f     /* OB_COMPLETE */
};

/* --- Helper: transition to a new state, resetting state_timer --- */

static ob_flow_t ob_transition(ob_flow_t flow, ob_state_t new_state) {
    flow.state = new_state;
    flow.state_timer = 0.0f;
    return flow;
}

/* --- Creation --- */

ob_flow_t ob_create(void) {
    ob_flow_t flow;
    flow.state = OB_EXPLORING;
    flow.engagement_time = 0.0f;
    flow.state_timer = 0.0f;
    flow.invitation_count = 0;
    flow.is_returning_user = 0;
    flow.birth_date_entered = 0;
    flow.birth_place_entered = 0;
    flow.interests_entered = 0;
    flow.onboarding_completed = 0;
    return flow;
}

ob_flow_t ob_create_returning(void) {
    ob_flow_t flow = ob_create();
    flow.is_returning_user = 1;
    flow.onboarding_completed = 1;
    return flow;
}

ob_timing_t ob_default_timing(void) {
    ob_timing_t t;
    t.invitation_delay = 60.0f;
    t.invitation_duration = 8.0f;
    t.invitation_cooldown = 300.0f;
    t.max_invitations = 3;
    t.complete_duration = 3.0f;
    return t;
}

/* --- Query functions --- */

int ob_should_invite(const ob_flow_t *flow, const ob_timing_t *timing) {
    if (flow->state != OB_EXPLORING) {
        return 0;
    }
    if (flow->is_returning_user) {
        return 0;
    }
    if (flow->onboarding_completed) {
        return 0;
    }
    if (flow->invitation_count >= timing->max_invitations) {
        return 0;
    }
    float threshold = timing->invitation_delay
                      + ((float)flow->invitation_count * timing->invitation_cooldown);
    if (flow->engagement_time < threshold) {
        return 0;
    }
    return 1;
}

int ob_is_active(const ob_flow_t *flow) {
    if (flow->state == OB_EXPLORING) {
        return 0;
    }
    if (flow->onboarding_completed) {
        return 0;
    }
    return 1;
}

int ob_is_complete(const ob_flow_t *flow) {
    return flow->onboarding_completed;
}

const char *ob_state_name(ob_state_t state) {
    if (state < 0 || state >= OB_STATE_COUNT) {
        return "Unknown";
    }
    return s_state_names[state];
}

float ob_scene_dim(const ob_flow_t *flow) {
    if (flow->state < 0 || flow->state >= OB_STATE_COUNT) {
        return 0.0f;
    }
    return s_scene_dim[flow->state];
}

/* --- State machine: per-state event handlers --- */

static ob_flow_t ob_process_exploring(ob_flow_t flow, ob_event_t event,
                                       float event_data,
                                       const ob_timing_t *timing) {
    switch (event) {
    case OB_EVT_TICK:
        flow.engagement_time += event_data;
        if (ob_should_invite(&flow, timing)) {
            flow = ob_transition(flow, OB_INVITATION);
        }
        break;
    case OB_EVT_PROFILE_TAP:
        if (!flow.is_returning_user && !flow.onboarding_completed) {
            flow = ob_transition(flow, OB_BIRTH_DATE);
        }
        break;
    default:
        break;
    }
    return flow;
}

static ob_flow_t ob_process_invitation(ob_flow_t flow, ob_event_t event,
                                        float event_data,
                                        const ob_timing_t *timing) {
    switch (event) {
    case OB_EVT_TICK:
        flow.state_timer += event_data;
        if (flow.state_timer >= timing->invitation_duration) {
            flow.invitation_count++;
            flow = ob_transition(flow, OB_EXPLORING);
        }
        break;
    case OB_EVT_INVITATION_TAP:
        flow = ob_transition(flow, OB_BIRTH_DATE);
        break;
    case OB_EVT_INVITATION_TIMEOUT:
    case OB_EVT_DISMISS:
        flow.invitation_count++;
        flow = ob_transition(flow, OB_EXPLORING);
        break;
    default:
        break;
    }
    return flow;
}

static ob_flow_t ob_process_birth_date(ob_flow_t flow, ob_event_t event) {
    switch (event) {
    case OB_EVT_DATE_ENTERED:
        flow.birth_date_entered = 1;
        flow = ob_transition(flow, OB_BIRTH_PLACE);
        break;
    case OB_EVT_DATE_SKIPPED:
    case OB_EVT_DISMISS:
        flow = ob_transition(flow, OB_EXPLORING);
        break;
    default:
        break;
    }
    return flow;
}

static ob_flow_t ob_process_birth_place(ob_flow_t flow, ob_event_t event) {
    switch (event) {
    case OB_EVT_PLACE_ENTERED:
        flow.birth_place_entered = 1;
        flow = ob_transition(flow, OB_REVEAL);
        break;
    case OB_EVT_PLACE_SKIPPED:
        flow = ob_transition(flow, OB_REVEAL);
        break;
    case OB_EVT_DISMISS:
        flow = ob_transition(flow, OB_EXPLORING);
        break;
    default:
        break;
    }
    return flow;
}

static ob_flow_t ob_process_reveal(ob_flow_t flow, ob_event_t event) {
    switch (event) {
    case OB_EVT_REVEAL_DONE:
    case OB_EVT_DISMISS:
        flow = ob_transition(flow, OB_INTERESTS);
        break;
    default:
        break;
    }
    return flow;
}

static ob_flow_t ob_process_interests(ob_flow_t flow, ob_event_t event) {
    switch (event) {
    case OB_EVT_INTERESTS_DONE:
        flow.interests_entered = 1;
        flow = ob_transition(flow, OB_COMPLETE);
        break;
    case OB_EVT_DISMISS:
        flow = ob_transition(flow, OB_COMPLETE);
        break;
    default:
        break;
    }
    return flow;
}

static ob_flow_t ob_process_complete(ob_flow_t flow, ob_event_t event,
                                      float event_data,
                                      const ob_timing_t *timing) {
    switch (event) {
    case OB_EVT_TICK:
        flow.state_timer += event_data;
        if (flow.state_timer >= timing->complete_duration) {
            flow.onboarding_completed = 1;
            flow = ob_transition(flow, OB_EXPLORING);
        }
        break;
    case OB_EVT_COMPLETE_DONE:
    case OB_EVT_DISMISS:
        flow.onboarding_completed = 1;
        flow = ob_transition(flow, OB_EXPLORING);
        break;
    default:
        break;
    }
    return flow;
}

/* --- Main dispatcher --- */

ob_flow_t ob_process(ob_flow_t flow, ob_event_t event, float event_data,
                      const ob_timing_t *timing) {
    switch (flow.state) {
    case OB_EXPLORING:
        return ob_process_exploring(flow, event, event_data, timing);
    case OB_INVITATION:
        return ob_process_invitation(flow, event, event_data, timing);
    case OB_BIRTH_DATE:
        return ob_process_birth_date(flow, event);
    case OB_BIRTH_PLACE:
        return ob_process_birth_place(flow, event);
    case OB_REVEAL:
        return ob_process_reveal(flow, event);
    case OB_INTERESTS:
        return ob_process_interests(flow, event);
    case OB_COMPLETE:
        return ob_process_complete(flow, event, event_data, timing);
    default:
        break;
    }
    return flow;
}
