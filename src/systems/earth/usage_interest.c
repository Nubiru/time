/* usage_interest.c — Usage-based interest tracking implementation.
 * Pure computation: no GL, no IO, no global state. */

#include "usage_interest.h"
#include <string.h>

/* Scoring weights: frequency 40%, recency 30%, engagement 30%. */
#define UI_WEIGHT_FREQ   0.4
#define UI_WEIGHT_RECENCY 0.3
#define UI_WEIGHT_ENGAGE  0.3

/* Recency decay rate: recency = 1 / (1 + days_since * DECAY). */
#define UI_RECENCY_DECAY 0.1

static int ui_valid_id(int id)
{
    return id >= 0 && id < UI_MAX_SYSTEMS;
}

ui_tracker_t ui_create(void)
{
    ui_tracker_t t;
    memset(&t, 0, sizeof(t));
    return t;
}

ui_tracker_t ui_record_view(ui_tracker_t t, int system_id,
                            double jd, double duration_sec)
{
    if (!ui_valid_id(system_id)) {
        return t;
    }

    if (duration_sec < 0.0) {
        duration_sec = 0.0;
    }

    t.systems[system_id].view_count++;
    t.systems[system_id].last_view_jd = jd;
    t.systems[system_id].total_time_sec += duration_sec;

    if (t.first_use_jd == 0.0) {
        t.first_use_jd = jd;
    }
    t.last_event_jd = jd;
    t.total_events++;

    return t;
}

ui_score_t ui_score(const ui_tracker_t *t, int system_id, double current_jd)
{
    ui_score_t s;
    memset(&s, 0, sizeof(s));
    s.system_id = system_id;

    if (!t || !ui_valid_id(system_id)) {
        return s;
    }

    if (t->systems[system_id].view_count == 0) {
        return s;
    }

    /* Find max view_count and max total_time across all systems. */
    int max_views = 0;
    double max_time = 0.0;
    for (int i = 0; i < UI_MAX_SYSTEMS; i++) {
        if (t->systems[i].view_count > max_views) {
            max_views = t->systems[i].view_count;
        }
        if (t->systems[i].total_time_sec > max_time) {
            max_time = t->systems[i].total_time_sec;
        }
    }

    /* Frequency: normalized to max. */
    s.frequency = (max_views > 0)
        ? (double)t->systems[system_id].view_count / (double)max_views
        : 0.0;

    /* Recency: inverse decay from days since last view. */
    double days_since = current_jd - t->systems[system_id].last_view_jd;
    if (days_since < 0.0) {
        days_since = 0.0;
    }
    s.recency = 1.0 / (1.0 + days_since * UI_RECENCY_DECAY);

    /* Engagement: normalized to max. */
    s.engagement = (max_time > 0.0)
        ? t->systems[system_id].total_time_sec / max_time
        : 0.0;

    /* Composite score. */
    s.score = UI_WEIGHT_FREQ * s.frequency
            + UI_WEIGHT_RECENCY * s.recency
            + UI_WEIGHT_ENGAGE * s.engagement;

    return s;
}

ui_ranking_t ui_rank(const ui_tracker_t *t, double current_jd)
{
    ui_ranking_t r;
    memset(&r, 0, sizeof(r));

    if (!t) {
        return r;
    }

    /* Collect scores for viewed systems only. */
    for (int i = 0; i < UI_MAX_SYSTEMS; i++) {
        if (t->systems[i].view_count > 0) {
            r.scores[r.count] = ui_score(t, i, current_jd);
            r.count++;
        }
    }

    /* Insertion sort descending by score. */
    for (int i = 1; i < r.count; i++) {
        ui_score_t key = r.scores[i];
        int j = i - 1;
        while (j >= 0 && r.scores[j].score < key.score) {
            r.scores[j + 1] = r.scores[j];
            j--;
        }
        r.scores[j + 1] = key;
    }

    return r;
}

int ui_top_n(const ui_tracker_t *t, double current_jd, int *ids, int n)
{
    if (!t || !ids || n <= 0) {
        return 0;
    }

    ui_ranking_t r = ui_rank(t, current_jd);
    int count = (n < r.count) ? n : r.count;

    for (int i = 0; i < count; i++) {
        ids[i] = r.scores[i].system_id;
    }

    return count;
}

int ui_suggest(const ui_tracker_t *t, double current_jd, double min_gap_days)
{
    if (!t) {
        return -1;
    }

    ui_ranking_t r = ui_rank(t, current_jd);

    /* Walk ranked list, return first system with gap > min_gap_days. */
    for (int i = 0; i < r.count; i++) {
        int sid = r.scores[i].system_id;
        double gap = current_jd - t->systems[sid].last_view_jd;
        if (gap > min_gap_days) {
            return sid;
        }
    }

    return -1;
}

int ui_suggest_unvisited(const ui_tracker_t *t)
{
    if (!t) {
        return -1;
    }

    for (int i = 0; i < UI_MAX_SYSTEMS; i++) {
        if (t->systems[i].view_count == 0) {
            return i;
        }
    }

    return -1;
}

ui_tracker_t ui_reset(void)
{
    return ui_create();
}

int ui_total_views(const ui_tracker_t *t)
{
    if (!t) {
        return 0;
    }

    int total = 0;
    for (int i = 0; i < UI_MAX_SYSTEMS; i++) {
        total += t->systems[i].view_count;
    }

    return total;
}

int ui_viewed_count(const ui_tracker_t *t)
{
    if (!t) {
        return 0;
    }

    int count = 0;
    for (int i = 0; i < UI_MAX_SYSTEMS; i++) {
        if (t->systems[i].view_count > 0) {
            count++;
        }
    }

    return count;
}

int ui_has_viewed(const ui_tracker_t *t, int system_id)
{
    if (!t || !ui_valid_id(system_id)) {
        return 0;
    }

    return t->systems[system_id].view_count > 0 ? 1 : 0;
}
