/* onboarding.h — Welcome Journey state machine.
 *
 * Tracks user's progress through Time's onboarding flow:
 * EXPLORING -> INVITATION -> BIRTH_DATE -> BIRTH_PLACE -> REVEAL -> INTERESTS -> COMPLETE
 *
 * Pure functions: no GL, no malloc, no globals, no side effects.
 * All functions take state by value and return new state by value. */

#ifndef TIME_ONBOARDING_H
#define TIME_ONBOARDING_H

/* Onboarding states */
typedef enum {
    OB_EXPLORING = 0,    /* Default: user is in 3D scene, no persona UI */
    OB_INVITATION,       /* Gentle visual cue: "Tell Time when you arrived" */
    OB_BIRTH_DATE,       /* Date input (day/month/year spinning wheels) */
    OB_BIRTH_PLACE,      /* Globe location picker */
    OB_REVEAL,           /* 16-system identity reveal animation */
    OB_INTERESTS,        /* "What fascinates you?" icon grid */
    OB_COMPLETE,         /* "Welcome home" moment, then back to exploring */
    OB_STATE_COUNT
} ob_state_t;

/* Events that trigger transitions */
typedef enum {
    OB_EVT_TICK = 0,         /* Time passes (dt seconds) */
    OB_EVT_INTERACTION,      /* User clicked, scrolled, or pressed a key */
    OB_EVT_PROFILE_TAP,     /* User taps profile icon */
    OB_EVT_INVITATION_TAP,  /* User taps the invitation text */
    OB_EVT_INVITATION_TIMEOUT, /* Invitation faded without interaction */
    OB_EVT_DATE_ENTERED,    /* User confirmed birth date */
    OB_EVT_DATE_SKIPPED,    /* User skipped birth date */
    OB_EVT_PLACE_ENTERED,   /* User selected birth location */
    OB_EVT_PLACE_SKIPPED,   /* User skipped birth location */
    OB_EVT_REVEAL_DONE,     /* Reveal animation completed */
    OB_EVT_INTERESTS_DONE,  /* User selected interests or skipped */
    OB_EVT_COMPLETE_DONE,   /* "Welcome home" moment finished */
    OB_EVT_DISMISS,         /* User clicked outside / pressed Escape */
    OB_EVT_COUNT
} ob_event_t;

/* Timing configuration */
typedef struct {
    float invitation_delay;    /* Seconds of engagement before first invitation (60.0) */
    float invitation_duration; /* How long invitation is visible (8.0 seconds) */
    float invitation_cooldown; /* Seconds between re-attempts (300.0 = 5 minutes) */
    int   max_invitations;     /* Max times to show per session (3) */
    float complete_duration;   /* How long "Welcome home" shows (3.0 seconds) */
} ob_timing_t;

/* Complete onboarding state */
typedef struct {
    ob_state_t state;          /* Current state */
    float engagement_time;     /* Cumulative active engagement seconds */
    float state_timer;         /* Time spent in current state */
    int invitation_count;      /* How many times invitation has been shown */
    int is_returning_user;     /* 1 if birth data already in storage */
    int has_interacted;        /* 1 if user clicked/scrolled/pressed a key */
    int birth_date_entered;    /* 1 if user entered birth date this session */
    int birth_place_entered;   /* 1 if user entered birth location this session */
    int interests_entered;     /* 1 if user selected interests this session */
    int onboarding_completed;  /* 1 if user reached COMPLETE state ever */
} ob_flow_t;

/* Create initial onboarding state. */
ob_flow_t ob_create(void);

/* Create state for returning user (skips all onboarding). */
ob_flow_t ob_create_returning(void);

/* Get default timing configuration. */
ob_timing_t ob_default_timing(void);

/* Process an event and return updated state.
 * For OB_EVT_TICK, pass dt (seconds since last tick) as the event_data.
 * For other events, event_data is unused (pass 0.0). */
ob_flow_t ob_process(ob_flow_t flow, ob_event_t event, float event_data,
                      const ob_timing_t *timing);

/* Check if it's time to show the invitation (called during EXPLORING). */
int ob_should_invite(const ob_flow_t *flow, const ob_timing_t *timing);

/* Check if onboarding is active (any state other than EXPLORING with completed=0). */
int ob_is_active(const ob_flow_t *flow);

/* Check if user has completed onboarding. */
int ob_is_complete(const ob_flow_t *flow);

/* Get human-readable name for a state. */
const char *ob_state_name(ob_state_t state);

/* Get scene dim amount for current state (0.0=full bright, ~0.618 for modal states). */
float ob_scene_dim(const ob_flow_t *flow);

/* --- Reward Framing & UX Copy ---
 * Returns display-ready strings for onboarding UI.
 * About Face V2: frame birth entry as reward, not task. */

/* Invitation title: "See yourself across every calendar" */
const char *ob_invitation_title(void);

/* Invitation subtitle: "Enter your birth date to unlock your identity..." */
const char *ob_invitation_subtitle(void);

/* Keyboard shortcut hint for birth entry: "P" */
const char *ob_shortcut_hint(void);

/* Reassurance text for skip links: "You can always change this later" */
const char *ob_reassurance_text(void);

/* Step title for birth date entry (reward-framed). */
const char *ob_birth_date_title(void);

/* Step title for birth place entry (reward-framed). */
const char *ob_birth_place_title(void);

#endif /* TIME_ONBOARDING_H */
