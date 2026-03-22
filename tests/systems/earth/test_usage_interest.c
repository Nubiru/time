/* test_usage_interest.c — Tests for usage interest tracking module.
 * TDD RED phase: all tests written before implementation. */

#include "../../unity/unity.h"
#include "../../../src/systems/earth/usage_interest.h"

void setUp(void) { }
void tearDown(void) { }

/* ===== ui_create ===== */

void test_create_all_view_counts_zero(void) {
    ui_tracker_t t = ui_create();
    for (int i = 0; i < UI_MAX_SYSTEMS; i++) {
        TEST_ASSERT_EQUAL_INT(0, t.systems[i].view_count);
    }
}

void test_create_all_last_view_jd_zero(void) {
    ui_tracker_t t = ui_create();
    for (int i = 0; i < UI_MAX_SYSTEMS; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)t.systems[i].last_view_jd);
    }
}

void test_create_all_total_time_zero(void) {
    ui_tracker_t t = ui_create();
    for (int i = 0; i < UI_MAX_SYSTEMS; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)t.systems[i].total_time_sec);
    }
}

void test_create_first_use_jd_zero(void) {
    ui_tracker_t t = ui_create();
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)t.first_use_jd);
}

void test_create_last_event_jd_zero(void) {
    ui_tracker_t t = ui_create();
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)t.last_event_jd);
}

void test_create_total_events_zero(void) {
    ui_tracker_t t = ui_create();
    TEST_ASSERT_EQUAL_INT(0, t.total_events);
}

/* ===== ui_record_view ===== */

void test_record_view_first_event_sets_first_use_jd(void) {
    ui_tracker_t t = ui_create();
    t = ui_record_view(t, 0, 2460000.5, 30.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 2460000.5f, (float)t.first_use_jd);
}

void test_record_view_increments_view_count(void) {
    ui_tracker_t t = ui_create();
    t = ui_record_view(t, 3, 2460000.5, 10.0);
    TEST_ASSERT_EQUAL_INT(1, t.systems[3].view_count);
}

void test_record_view_sets_last_view_jd(void) {
    ui_tracker_t t = ui_create();
    t = ui_record_view(t, 5, 2460000.5, 10.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 2460000.5f, (float)t.systems[5].last_view_jd);
}

void test_record_view_adds_duration(void) {
    ui_tracker_t t = ui_create();
    t = ui_record_view(t, 2, 2460000.5, 45.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 45.0f, (float)t.systems[2].total_time_sec);
}

void test_record_view_updates_last_event_jd(void) {
    ui_tracker_t t = ui_create();
    t = ui_record_view(t, 0, 2460000.5, 10.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 2460000.5f, (float)t.last_event_jd);
}

void test_record_view_increments_total_events(void) {
    ui_tracker_t t = ui_create();
    t = ui_record_view(t, 0, 2460000.5, 10.0);
    TEST_ASSERT_EQUAL_INT(1, t.total_events);
}

void test_record_view_multiple_events_accumulate(void) {
    ui_tracker_t t = ui_create();
    t = ui_record_view(t, 1, 2460000.5, 20.0);
    t = ui_record_view(t, 1, 2460001.5, 30.0);
    t = ui_record_view(t, 1, 2460002.5, 10.0);
    TEST_ASSERT_EQUAL_INT(3, t.systems[1].view_count);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 60.0f, (float)t.systems[1].total_time_sec);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 2460002.5f, (float)t.systems[1].last_view_jd);
}

void test_record_view_first_use_jd_not_overwritten(void) {
    ui_tracker_t t = ui_create();
    t = ui_record_view(t, 0, 2460000.5, 10.0);
    t = ui_record_view(t, 1, 2460005.5, 10.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 2460000.5f, (float)t.first_use_jd);
}

void test_record_view_total_events_across_systems(void) {
    ui_tracker_t t = ui_create();
    t = ui_record_view(t, 0, 2460000.5, 10.0);
    t = ui_record_view(t, 1, 2460001.5, 10.0);
    t = ui_record_view(t, 2, 2460002.5, 10.0);
    TEST_ASSERT_EQUAL_INT(3, t.total_events);
}

void test_record_view_invalid_negative_id_unchanged(void) {
    ui_tracker_t t = ui_create();
    ui_tracker_t before = t;
    t = ui_record_view(t, -1, 2460000.5, 10.0);
    TEST_ASSERT_EQUAL_INT(before.total_events, t.total_events);
}

void test_record_view_invalid_over_max_unchanged(void) {
    ui_tracker_t t = ui_create();
    ui_tracker_t before = t;
    t = ui_record_view(t, 20, 2460000.5, 10.0);
    TEST_ASSERT_EQUAL_INT(before.total_events, t.total_events);
}

void test_record_view_invalid_large_id_unchanged(void) {
    ui_tracker_t t = ui_create();
    t = ui_record_view(t, 999, 2460000.5, 10.0);
    TEST_ASSERT_EQUAL_INT(0, t.total_events);
}

void test_record_view_negative_duration_treated_as_zero(void) {
    ui_tracker_t t = ui_create();
    t = ui_record_view(t, 0, 2460000.5, -15.0);
    TEST_ASSERT_EQUAL_INT(1, t.systems[0].view_count);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)t.systems[0].total_time_sec);
}

/* ===== ui_score ===== */

void test_score_unviewed_system_returns_zero(void) {
    ui_tracker_t t = ui_create();
    ui_score_t s = ui_score(&t, 0, 2460000.5);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)s.score);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)s.frequency);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)s.recency);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)s.engagement);
}

void test_score_single_system_frequency_is_one(void) {
    ui_tracker_t t = ui_create();
    t = ui_record_view(t, 3, 2460000.5, 60.0);
    ui_score_t s = ui_score(&t, 3, 2460000.5);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, (float)s.frequency);
}

void test_score_single_system_engagement_is_one(void) {
    ui_tracker_t t = ui_create();
    t = ui_record_view(t, 3, 2460000.5, 60.0);
    ui_score_t s = ui_score(&t, 3, 2460000.5);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, (float)s.engagement);
}

void test_score_recency_viewed_today_near_one(void) {
    ui_tracker_t t = ui_create();
    t = ui_record_view(t, 0, 2460000.5, 10.0);
    ui_score_t s = ui_score(&t, 0, 2460000.5);
    /* days_since = 0, recency = 1.0 / (1.0 + 0*0.1) = 1.0 */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, (float)s.recency);
}

void test_score_recency_10_days_ago_half(void) {
    ui_tracker_t t = ui_create();
    t = ui_record_view(t, 0, 2460000.5, 10.0);
    /* current_jd is 10 days after last view */
    ui_score_t s = ui_score(&t, 0, 2460010.5);
    /* recency = 1.0 / (1.0 + 10*0.1) = 1.0/2.0 = 0.5 */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.5f, (float)s.recency);
}

void test_score_recency_30_days_ago(void) {
    ui_tracker_t t = ui_create();
    t = ui_record_view(t, 0, 2460000.5, 10.0);
    ui_score_t s = ui_score(&t, 0, 2460030.5);
    /* recency = 1.0 / (1.0 + 30*0.1) = 1.0/4.0 = 0.25 */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.25f, (float)s.recency);
}

void test_score_frequency_normalization(void) {
    ui_tracker_t t = ui_create();
    /* System 0: 10 views, System 1: 5 views */
    for (int i = 0; i < 10; i++) {
        t = ui_record_view(t, 0, 2460000.5 + i, 10.0);
    }
    for (int i = 0; i < 5; i++) {
        t = ui_record_view(t, 1, 2460000.5 + i, 10.0);
    }
    ui_score_t s0 = ui_score(&t, 0, 2460010.5);
    ui_score_t s1 = ui_score(&t, 1, 2460010.5);
    /* Freq of system 0 = 10/10 = 1.0, system 1 = 5/10 = 0.5 */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, (float)s0.frequency);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.5f, (float)s1.frequency);
}

void test_score_engagement_normalization(void) {
    ui_tracker_t t = ui_create();
    t = ui_record_view(t, 0, 2460000.5, 100.0);
    t = ui_record_view(t, 1, 2460000.5, 50.0);
    ui_score_t s0 = ui_score(&t, 0, 2460000.5);
    ui_score_t s1 = ui_score(&t, 1, 2460000.5);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, (float)s0.engagement);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.5f, (float)s1.engagement);
}

void test_score_composite_weights(void) {
    ui_tracker_t t = ui_create();
    t = ui_record_view(t, 0, 2460000.5, 60.0);
    ui_score_t s = ui_score(&t, 0, 2460000.5);
    /* Single system: freq=1.0, recency=1.0, engagement=1.0 */
    /* score = 0.4*1.0 + 0.3*1.0 + 0.3*1.0 = 1.0 */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, (float)s.score);
}

void test_score_composite_partial_values(void) {
    ui_tracker_t t = ui_create();
    /* Two systems: 0 has 10 views/100s, 1 has 5 views/50s */
    for (int i = 0; i < 10; i++) {
        t = ui_record_view(t, 0, 2460000.5 + i, 10.0);
    }
    for (int i = 0; i < 5; i++) {
        t = ui_record_view(t, 1, 2460000.5 + i, 10.0);
    }
    /* Score system 1 at day 10 (5 days since last view of system 1) */
    ui_score_t s1 = ui_score(&t, 1, 2460010.5);
    /* freq = 5/10 = 0.5, recency = 1/(1+5.5*0.1) = 1/1.55 ~= 0.645 */
    /* engagement = 50/100 = 0.5 */
    /* score = 0.4*0.5 + 0.3*0.645 + 0.3*0.5 = 0.2 + 0.1935 + 0.15 = 0.5435 */
    double expected_recency = 1.0 / (1.0 + 5.5 * 0.1);
    double expected_score = 0.4 * 0.5 + 0.3 * expected_recency + 0.3 * 0.5;
    TEST_ASSERT_FLOAT_WITHIN(0.01f, (float)expected_score, (float)s1.score);
}

void test_score_invalid_system_id_returns_zero(void) {
    ui_tracker_t t = ui_create();
    t = ui_record_view(t, 0, 2460000.5, 10.0);
    ui_score_t s = ui_score(&t, -1, 2460000.5);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)s.score);
}

void test_score_invalid_over_max_returns_zero(void) {
    ui_tracker_t t = ui_create();
    t = ui_record_view(t, 0, 2460000.5, 10.0);
    ui_score_t s = ui_score(&t, 20, 2460000.5);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)s.score);
}

void test_score_system_id_field_set(void) {
    ui_tracker_t t = ui_create();
    t = ui_record_view(t, 7, 2460000.5, 10.0);
    ui_score_t s = ui_score(&t, 7, 2460000.5);
    TEST_ASSERT_EQUAL_INT(7, s.system_id);
}

void test_score_null_tracker_returns_zero(void) {
    ui_score_t s = ui_score(NULL, 0, 2460000.5);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)s.score);
}

/* ===== ui_rank ===== */

void test_rank_empty_tracker_count_zero(void) {
    ui_tracker_t t = ui_create();
    ui_ranking_t r = ui_rank(&t, 2460000.5);
    TEST_ASSERT_EQUAL_INT(0, r.count);
}

void test_rank_single_system_count_one(void) {
    ui_tracker_t t = ui_create();
    t = ui_record_view(t, 5, 2460000.5, 30.0);
    ui_ranking_t r = ui_rank(&t, 2460000.5);
    TEST_ASSERT_EQUAL_INT(1, r.count);
    TEST_ASSERT_EQUAL_INT(5, r.scores[0].system_id);
}

void test_rank_multiple_sorted_by_score_desc(void) {
    ui_tracker_t t = ui_create();
    /* System 0: 10 views (high freq), system 1: 1 view (low freq) */
    for (int i = 0; i < 10; i++) {
        t = ui_record_view(t, 0, 2460000.5 + i, 60.0);
    }
    t = ui_record_view(t, 1, 2460000.5, 5.0);
    ui_ranking_t r = ui_rank(&t, 2460010.5);
    TEST_ASSERT_EQUAL_INT(2, r.count);
    TEST_ASSERT_TRUE(r.scores[0].score >= r.scores[1].score);
    TEST_ASSERT_EQUAL_INT(0, r.scores[0].system_id);
}

void test_rank_only_viewed_systems_included(void) {
    ui_tracker_t t = ui_create();
    t = ui_record_view(t, 3, 2460000.5, 10.0);
    t = ui_record_view(t, 7, 2460000.5, 10.0);
    ui_ranking_t r = ui_rank(&t, 2460000.5);
    TEST_ASSERT_EQUAL_INT(2, r.count);
    /* Verify both entries are either 3 or 7 */
    int found3 = 0, found7 = 0;
    for (int i = 0; i < r.count; i++) {
        if (r.scores[i].system_id == 3) found3 = 1;
        if (r.scores[i].system_id == 7) found7 = 1;
    }
    TEST_ASSERT_TRUE(found3);
    TEST_ASSERT_TRUE(found7);
}

void test_rank_null_tracker_returns_empty(void) {
    ui_ranking_t r = ui_rank(NULL, 2460000.5);
    TEST_ASSERT_EQUAL_INT(0, r.count);
}

/* ===== ui_top_n ===== */

void test_top_n_returns_correct_ids(void) {
    ui_tracker_t t = ui_create();
    for (int i = 0; i < 10; i++) {
        t = ui_record_view(t, 0, 2460000.5 + i, 60.0);
    }
    for (int i = 0; i < 5; i++) {
        t = ui_record_view(t, 1, 2460000.5 + i, 30.0);
    }
    t = ui_record_view(t, 2, 2460000.5, 5.0);

    int ids[2];
    int count = ui_top_n(&t, 2460010.5, ids, 2);
    TEST_ASSERT_EQUAL_INT(2, count);
    TEST_ASSERT_EQUAL_INT(0, ids[0]); /* highest score */
}

void test_top_n_exceeds_viewed_count(void) {
    ui_tracker_t t = ui_create();
    t = ui_record_view(t, 0, 2460000.5, 10.0);
    t = ui_record_view(t, 1, 2460000.5, 10.0);
    int ids[10];
    int count = ui_top_n(&t, 2460000.5, ids, 10);
    TEST_ASSERT_EQUAL_INT(2, count);
}

void test_top_n_empty_tracker_returns_zero(void) {
    ui_tracker_t t = ui_create();
    int ids[5];
    int count = ui_top_n(&t, 2460000.5, ids, 5);
    TEST_ASSERT_EQUAL_INT(0, count);
}

void test_top_n_null_tracker_returns_zero(void) {
    int ids[5];
    int count = ui_top_n(NULL, 2460000.5, ids, 5);
    TEST_ASSERT_EQUAL_INT(0, count);
}

/* ===== ui_suggest ===== */

void test_suggest_returns_highest_scored_old_system(void) {
    ui_tracker_t t = ui_create();
    /* System 0: many views, last viewed 20 days ago */
    for (int i = 0; i < 10; i++) {
        t = ui_record_view(t, 0, 2460000.5 + i, 60.0);
    }
    /* System 1: few views, viewed recently */
    t = ui_record_view(t, 1, 2460019.5, 10.0);

    /* min_gap = 5 days, current = day 20 */
    int suggestion = ui_suggest(&t, 2460020.5, 5.0);
    /* System 0 was last viewed at day 9 (11 days ago), gap > 5 */
    /* System 1 was last viewed at day 19 (1 day ago), gap < 5 */
    TEST_ASSERT_EQUAL_INT(0, suggestion);
}

void test_suggest_returns_neg1_if_all_recent(void) {
    ui_tracker_t t = ui_create();
    t = ui_record_view(t, 0, 2460000.5, 10.0);
    t = ui_record_view(t, 1, 2460000.5, 10.0);
    /* All viewed same day, min_gap = 1 day, checking same day */
    int suggestion = ui_suggest(&t, 2460000.5, 1.0);
    TEST_ASSERT_EQUAL_INT(-1, suggestion);
}

void test_suggest_skips_recently_viewed(void) {
    ui_tracker_t t = ui_create();
    /* System 0: high score, viewed yesterday */
    for (int i = 0; i < 10; i++) {
        t = ui_record_view(t, 0, 2460009.5, 60.0);
    }
    /* System 1: lower score, viewed 10 days ago */
    t = ui_record_view(t, 1, 2460000.5, 10.0);

    int suggestion = ui_suggest(&t, 2460010.5, 3.0);
    /* System 0 was viewed 1 day ago (< 3 day gap), skip */
    /* System 1 was viewed 10 days ago (> 3 day gap) */
    TEST_ASSERT_EQUAL_INT(1, suggestion);
}

void test_suggest_empty_tracker_returns_neg1(void) {
    ui_tracker_t t = ui_create();
    int suggestion = ui_suggest(&t, 2460000.5, 1.0);
    TEST_ASSERT_EQUAL_INT(-1, suggestion);
}

/* ===== ui_suggest_unvisited ===== */

void test_suggest_unvisited_returns_first_unvisited(void) {
    ui_tracker_t t = ui_create();
    t = ui_record_view(t, 0, 2460000.5, 10.0);
    /* System 1 is first unvisited */
    int suggestion = ui_suggest_unvisited(&t);
    TEST_ASSERT_EQUAL_INT(1, suggestion);
}

void test_suggest_unvisited_returns_neg1_if_all_visited(void) {
    ui_tracker_t t = ui_create();
    for (int i = 0; i < UI_MAX_SYSTEMS; i++) {
        t = ui_record_view(t, i, 2460000.5, 10.0);
    }
    int suggestion = ui_suggest_unvisited(&t);
    TEST_ASSERT_EQUAL_INT(-1, suggestion);
}

void test_suggest_unvisited_empty_tracker_returns_zero(void) {
    ui_tracker_t t = ui_create();
    int suggestion = ui_suggest_unvisited(&t);
    TEST_ASSERT_EQUAL_INT(0, suggestion);
}

void test_suggest_unvisited_null_tracker_returns_neg1(void) {
    int suggestion = ui_suggest_unvisited(NULL);
    TEST_ASSERT_EQUAL_INT(-1, suggestion);
}

/* ===== ui_reset ===== */

void test_reset_returns_fresh_tracker(void) {
    ui_tracker_t t = ui_reset();
    TEST_ASSERT_EQUAL_INT(0, t.total_events);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)t.first_use_jd);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)t.last_event_jd);
    for (int i = 0; i < UI_MAX_SYSTEMS; i++) {
        TEST_ASSERT_EQUAL_INT(0, t.systems[i].view_count);
    }
}

void test_reset_matches_create(void) {
    ui_tracker_t a = ui_create();
    ui_tracker_t b = ui_reset();
    TEST_ASSERT_EQUAL_INT(a.total_events, b.total_events);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, (float)a.first_use_jd, (float)b.first_use_jd);
}

/* ===== ui_total_views ===== */

void test_total_views_empty_zero(void) {
    ui_tracker_t t = ui_create();
    TEST_ASSERT_EQUAL_INT(0, ui_total_views(&t));
}

void test_total_views_correct_sum(void) {
    ui_tracker_t t = ui_create();
    t = ui_record_view(t, 0, 2460000.5, 10.0);
    t = ui_record_view(t, 0, 2460001.5, 10.0);
    t = ui_record_view(t, 3, 2460002.5, 10.0);
    TEST_ASSERT_EQUAL_INT(3, ui_total_views(&t));
}

void test_total_views_null_returns_zero(void) {
    TEST_ASSERT_EQUAL_INT(0, ui_total_views(NULL));
}

/* ===== ui_viewed_count ===== */

void test_viewed_count_empty_zero(void) {
    ui_tracker_t t = ui_create();
    TEST_ASSERT_EQUAL_INT(0, ui_viewed_count(&t));
}

void test_viewed_count_correct(void) {
    ui_tracker_t t = ui_create();
    t = ui_record_view(t, 0, 2460000.5, 10.0);
    t = ui_record_view(t, 0, 2460001.5, 10.0); /* same system */
    t = ui_record_view(t, 5, 2460002.5, 10.0);
    TEST_ASSERT_EQUAL_INT(2, ui_viewed_count(&t));
}

void test_viewed_count_null_returns_zero(void) {
    TEST_ASSERT_EQUAL_INT(0, ui_viewed_count(NULL));
}

/* ===== ui_has_viewed ===== */

void test_has_viewed_returns_1_for_viewed(void) {
    ui_tracker_t t = ui_create();
    t = ui_record_view(t, 4, 2460000.5, 10.0);
    TEST_ASSERT_EQUAL_INT(1, ui_has_viewed(&t, 4));
}

void test_has_viewed_returns_0_for_not_viewed(void) {
    ui_tracker_t t = ui_create();
    t = ui_record_view(t, 4, 2460000.5, 10.0);
    TEST_ASSERT_EQUAL_INT(0, ui_has_viewed(&t, 5));
}

void test_has_viewed_invalid_id_returns_0(void) {
    ui_tracker_t t = ui_create();
    TEST_ASSERT_EQUAL_INT(0, ui_has_viewed(&t, -1));
    TEST_ASSERT_EQUAL_INT(0, ui_has_viewed(&t, 20));
}

void test_has_viewed_null_returns_0(void) {
    TEST_ASSERT_EQUAL_INT(0, ui_has_viewed(NULL, 0));
}

/* --- ui_reorder_by_interest --- */

void test_reorder_sorts_by_score(void) {
    ui_tracker_t t = ui_create();
    t = ui_record_view(t, 5, 100.0, 300.0);  /* heavy use */
    t = ui_record_view(t, 5, 101.0, 200.0);
    t = ui_record_view(t, 3, 100.0, 50.0);   /* light use */
    t = ui_record_view(t, 8, 100.0, 150.0);  /* medium use */
    t = ui_record_view(t, 8, 101.0, 100.0);

    int in_ids[] = {3, 5, 8};
    int out_ids[3];
    int n = ui_reorder_by_interest(&t, 102.0, in_ids, out_ids, 3);
    TEST_ASSERT_EQUAL_INT(3, n);
    TEST_ASSERT_EQUAL_INT(5, out_ids[0]); /* most viewed first */
}

void test_reorder_unviewed_last(void) {
    ui_tracker_t t = ui_create();
    t = ui_record_view(t, 2, 100.0, 100.0);

    int in_ids[] = {0, 2, 4};
    int out_ids[3];
    ui_reorder_by_interest(&t, 101.0, in_ids, out_ids, 3);
    TEST_ASSERT_EQUAL_INT(2, out_ids[0]); /* only viewed system first */
}

void test_reorder_preserves_count(void) {
    ui_tracker_t t = ui_create();
    int in_ids[] = {0, 1, 2, 3, 4};
    int out_ids[5];
    int n = ui_reorder_by_interest(&t, 100.0, in_ids, out_ids, 5);
    TEST_ASSERT_EQUAL_INT(5, n);
}

void test_reorder_single_element(void) {
    ui_tracker_t t = ui_create();
    t = ui_record_view(t, 7, 100.0, 50.0);
    int in_ids[] = {7};
    int out_ids[1];
    int n = ui_reorder_by_interest(&t, 101.0, in_ids, out_ids, 1);
    TEST_ASSERT_EQUAL_INT(1, n);
    TEST_ASSERT_EQUAL_INT(7, out_ids[0]);
}

void test_reorder_null_tracker(void) {
    int in_ids[] = {0, 1};
    int out_ids[2];
    TEST_ASSERT_EQUAL_INT(0, ui_reorder_by_interest(NULL, 100.0, in_ids, out_ids, 2));
}

void test_reorder_null_ids(void) {
    ui_tracker_t t = ui_create();
    int out_ids[2];
    TEST_ASSERT_EQUAL_INT(0, ui_reorder_by_interest(&t, 100.0, NULL, out_ids, 2));
}

void test_reorder_zero_count(void) {
    ui_tracker_t t = ui_create();
    int in_ids[] = {0};
    int out_ids[1];
    TEST_ASSERT_EQUAL_INT(0, ui_reorder_by_interest(&t, 100.0, in_ids, out_ids, 0));
}

void test_reorder_respects_recency(void) {
    ui_tracker_t t = ui_create();
    /* System 1: viewed long ago, high count */
    t = ui_record_view(t, 1, 50.0, 100.0);
    t = ui_record_view(t, 1, 51.0, 100.0);
    t = ui_record_view(t, 1, 52.0, 100.0);
    /* System 2: viewed recently, lower count */
    t = ui_record_view(t, 2, 99.0, 100.0);

    int in_ids[] = {1, 2};
    int out_ids[2];
    ui_reorder_by_interest(&t, 100.0, in_ids, out_ids, 2);
    /* System 1 has higher frequency (3 vs 1) which dominates */
    TEST_ASSERT_EQUAL_INT(1, out_ids[0]);
}

/* --- Runner --- */

int main(void) {
    UNITY_BEGIN();

    /* ui_create */
    RUN_TEST(test_create_all_view_counts_zero);
    RUN_TEST(test_create_all_last_view_jd_zero);
    RUN_TEST(test_create_all_total_time_zero);
    RUN_TEST(test_create_first_use_jd_zero);
    RUN_TEST(test_create_last_event_jd_zero);
    RUN_TEST(test_create_total_events_zero);

    /* ui_record_view */
    RUN_TEST(test_record_view_first_event_sets_first_use_jd);
    RUN_TEST(test_record_view_increments_view_count);
    RUN_TEST(test_record_view_sets_last_view_jd);
    RUN_TEST(test_record_view_adds_duration);
    RUN_TEST(test_record_view_updates_last_event_jd);
    RUN_TEST(test_record_view_increments_total_events);
    RUN_TEST(test_record_view_multiple_events_accumulate);
    RUN_TEST(test_record_view_first_use_jd_not_overwritten);
    RUN_TEST(test_record_view_total_events_across_systems);
    RUN_TEST(test_record_view_invalid_negative_id_unchanged);
    RUN_TEST(test_record_view_invalid_over_max_unchanged);
    RUN_TEST(test_record_view_invalid_large_id_unchanged);
    RUN_TEST(test_record_view_negative_duration_treated_as_zero);

    /* ui_score */
    RUN_TEST(test_score_unviewed_system_returns_zero);
    RUN_TEST(test_score_single_system_frequency_is_one);
    RUN_TEST(test_score_single_system_engagement_is_one);
    RUN_TEST(test_score_recency_viewed_today_near_one);
    RUN_TEST(test_score_recency_10_days_ago_half);
    RUN_TEST(test_score_recency_30_days_ago);
    RUN_TEST(test_score_frequency_normalization);
    RUN_TEST(test_score_engagement_normalization);
    RUN_TEST(test_score_composite_weights);
    RUN_TEST(test_score_composite_partial_values);
    RUN_TEST(test_score_invalid_system_id_returns_zero);
    RUN_TEST(test_score_invalid_over_max_returns_zero);
    RUN_TEST(test_score_system_id_field_set);
    RUN_TEST(test_score_null_tracker_returns_zero);

    /* ui_rank */
    RUN_TEST(test_rank_empty_tracker_count_zero);
    RUN_TEST(test_rank_single_system_count_one);
    RUN_TEST(test_rank_multiple_sorted_by_score_desc);
    RUN_TEST(test_rank_only_viewed_systems_included);
    RUN_TEST(test_rank_null_tracker_returns_empty);

    /* ui_top_n */
    RUN_TEST(test_top_n_returns_correct_ids);
    RUN_TEST(test_top_n_exceeds_viewed_count);
    RUN_TEST(test_top_n_empty_tracker_returns_zero);
    RUN_TEST(test_top_n_null_tracker_returns_zero);

    /* ui_suggest */
    RUN_TEST(test_suggest_returns_highest_scored_old_system);
    RUN_TEST(test_suggest_returns_neg1_if_all_recent);
    RUN_TEST(test_suggest_skips_recently_viewed);
    RUN_TEST(test_suggest_empty_tracker_returns_neg1);

    /* ui_suggest_unvisited */
    RUN_TEST(test_suggest_unvisited_returns_first_unvisited);
    RUN_TEST(test_suggest_unvisited_returns_neg1_if_all_visited);
    RUN_TEST(test_suggest_unvisited_empty_tracker_returns_zero);
    RUN_TEST(test_suggest_unvisited_null_tracker_returns_neg1);

    /* ui_reset */
    RUN_TEST(test_reset_returns_fresh_tracker);
    RUN_TEST(test_reset_matches_create);

    /* ui_total_views */
    RUN_TEST(test_total_views_empty_zero);
    RUN_TEST(test_total_views_correct_sum);
    RUN_TEST(test_total_views_null_returns_zero);

    /* ui_viewed_count */
    RUN_TEST(test_viewed_count_empty_zero);
    RUN_TEST(test_viewed_count_correct);
    RUN_TEST(test_viewed_count_null_returns_zero);

    /* ui_has_viewed */
    RUN_TEST(test_has_viewed_returns_1_for_viewed);
    RUN_TEST(test_has_viewed_returns_0_for_not_viewed);
    RUN_TEST(test_has_viewed_invalid_id_returns_0);
    RUN_TEST(test_has_viewed_null_returns_0);

    /* ui_reorder_by_interest (8) */
    RUN_TEST(test_reorder_sorts_by_score);
    RUN_TEST(test_reorder_unviewed_last);
    RUN_TEST(test_reorder_preserves_count);
    RUN_TEST(test_reorder_single_element);
    RUN_TEST(test_reorder_null_tracker);
    RUN_TEST(test_reorder_null_ids);
    RUN_TEST(test_reorder_zero_count);
    RUN_TEST(test_reorder_respects_recency);

    return UNITY_END();
}
