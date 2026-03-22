/* usage_interest.h — Usage-based interest tracking for adaptive experience.
 * Tracks which systems user views most, computes interest scores,
 * suggests content from preferred systems. Pure computation, no storage. */

#ifndef TIME_USAGE_INTEREST_H
#define TIME_USAGE_INTEREST_H

#include <stddef.h>

#define UI_MAX_SYSTEMS 16

/* Per-system usage statistics. */
typedef struct {
    int view_count;
    double last_view_jd;
    double total_time_sec;
} ui_system_stats_t;

/* Aggregate tracker for all system usage. */
typedef struct {
    ui_system_stats_t systems[UI_MAX_SYSTEMS];
    double first_use_jd;
    double last_event_jd;
    int total_events;
} ui_tracker_t;

/* Computed interest score for one system. */
typedef struct {
    int system_id;
    double score;       /* 0.0-1.0 composite */
    double frequency;   /* 0.0-1.0 normalized view count */
    double recency;     /* 0.0-1.0 decay from last view */
    double engagement;  /* 0.0-1.0 normalized time spent */
} ui_score_t;

/* Ranked list of all viewed systems. */
typedef struct {
    ui_score_t scores[UI_MAX_SYSTEMS];
    int count;
} ui_ranking_t;

/* Create a fresh tracker with all fields zero. */
ui_tracker_t ui_create(void);

/* Record a view event. Returns updated tracker.
 * Invalid system_id (< 0 or >= UI_MAX_SYSTEMS) is ignored.
 * Negative duration is treated as zero. */
ui_tracker_t ui_record_view(ui_tracker_t t, int system_id,
                            double jd, double duration_sec);

/* Compute interest score for one system. */
ui_score_t ui_score(const ui_tracker_t *t, int system_id, double current_jd);

/* Rank all viewed systems by score, descending. */
ui_ranking_t ui_rank(const ui_tracker_t *t, double current_jd);

/* Fill ids with top N system IDs by score. Returns actual count. */
int ui_top_n(const ui_tracker_t *t, double current_jd, int *ids, int n);

/* Suggest a previously-viewed system not seen in min_gap_days.
 * Returns system_id or -1 if none qualifies. */
int ui_suggest(const ui_tracker_t *t, double current_jd, double min_gap_days);

/* Suggest first system never viewed. Returns system_id or -1 if all visited. */
int ui_suggest_unvisited(const ui_tracker_t *t);

/* Reset tracker to fresh state (same as ui_create). */
ui_tracker_t ui_reset(void);

/* Total view events across all systems. */
int ui_total_views(const ui_tracker_t *t);

/* Number of distinct systems viewed at least once. */
int ui_viewed_count(const ui_tracker_t *t);

/* Whether a specific system has been viewed. Returns 1 or 0. */
int ui_has_viewed(const ui_tracker_t *t, int system_id);

/* Reorder system IDs by interest score (descending).
 * Takes an array of system IDs (in_ids) and writes them sorted by
 * score to out_ids. Returns count written (same as count).
 * Unviewed systems sort to the end. */
int ui_reorder_by_interest(const ui_tracker_t *t, double current_jd,
                           const int *in_ids, int *out_ids, int count);

#endif /* TIME_USAGE_INTEREST_H */
