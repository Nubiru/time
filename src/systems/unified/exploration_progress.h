/* exploration_progress.h — User exploration journey tracking.
 *
 * Computes exploration metrics from user state: system coverage,
 * visit streaks, time milestones, and gentle progress insights.
 * "You've explored 9 of 16 systems." "30 consecutive days with Time."
 *
 * L3.7 data backbone — gentle, never pushy.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_EXPLORATION_PROGRESS_H
#define TIME_EXPLORATION_PROGRESS_H

#define EP_TOTAL_SYSTEMS   16
#define EP_INSIGHT_MAX     128
#define EP_DEEP_DIVE_SEC   1800.0  /* 30 minutes */

/* Milestone types */
typedef enum {
    EP_MS_FIRST_VISIT = 0,     /* First time opening Time */
    EP_MS_FIRST_SYSTEM,        /* Explored first system beyond default */
    EP_MS_HALF_SYSTEMS,        /* Explored 8 of 16 systems */
    EP_MS_ALL_SYSTEMS,         /* Explored all 16 systems */
    EP_MS_STREAK_7,            /* 7 consecutive days */
    EP_MS_STREAK_30,           /* 30 consecutive days */
    EP_MS_STREAK_100,          /* 100 consecutive days */
    EP_MS_STREAK_365,          /* Full year streak */
    EP_MS_FIRST_BOOKMARK,      /* Saved first bookmark */
    EP_MS_FIRST_CYCLE,         /* Tracked first cycle */
    EP_MS_DEEP_DIVE,           /* 30+ minutes in one session */
    EP_MS_COUNT
} ep_milestone_t;

/* Input: user's exploration state */
typedef struct {
    int explored_systems[EP_TOTAL_SYSTEMS]; /* 1=visited, 0=not */
    int explored_count;                     /* number of systems explored */
    int total_systems;                      /* total available (16) */
    int consecutive_days;                   /* current streak */
    int longest_streak;                     /* all-time longest */
    int total_visits;                       /* lifetime session count */
    int bookmark_count;                     /* bookmarks saved */
    int cycle_count;                        /* cycles tracked */
    double longest_session_sec;             /* longest single session */
    double total_time_sec;                  /* total time spent */
} ep_state_t;

/* Progress snapshot */
typedef struct {
    /* Coverage */
    double system_coverage;     /* 0.0-1.0 (explored / total) */
    int systems_remaining;      /* total - explored */

    /* Streaks */
    int current_streak;
    int longest_streak;
    int streak_active;          /* 1 if currently on a streak */

    /* Milestones */
    int milestones[EP_MS_COUNT]; /* 1=earned, 0=not */
    int milestone_count;         /* total earned */

    /* Insights */
    char insight[EP_INSIGHT_MAX]; /* Gentle text */
    int insight_significance;     /* 0=routine, 1=notable, 2=celebration */
} ep_progress_t;

/* Create default (empty) state. */
ep_state_t ep_default_state(void);

/* Compute progress from state. NULL state returns empty progress. */
ep_progress_t ep_compute(const ep_state_t *state);

/* Check if a specific milestone is earned. Returns 0 for NULL or invalid. */
int ep_has_milestone(const ep_progress_t *progress, ep_milestone_t ms);

/* Get milestone name. Returns "?" for invalid. */
const char *ep_milestone_name(ep_milestone_t ms);

/* Get milestone description. Returns "?" for invalid. */
const char *ep_milestone_desc(ep_milestone_t ms);

/* Count earned milestones. Returns 0 for NULL. */
int ep_earned_count(const ep_progress_t *progress);

/* Get next unearned milestone. Returns EP_MS_COUNT if all earned or NULL. */
ep_milestone_t ep_next_milestone(const ep_state_t *state);

/* Format progress as a gentle insight text.
 * Returns bytes written (excluding NUL). 0 for NULL args. */
int ep_format_insight(const ep_progress_t *progress, char *buf, int buf_size);

#endif /* TIME_EXPLORATION_PROGRESS_H */
