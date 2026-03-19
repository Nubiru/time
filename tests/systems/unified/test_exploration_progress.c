/* test_exploration_progress.c — Tests for user exploration journey tracking.
 * TDD: tests written first, implementation follows. */

#include "unity.h"
#include "systems/unified/exploration_progress.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== Default State ===== */

void test_default_state_zeroed(void) {
    ep_state_t s = ep_default_state();
    TEST_ASSERT_EQUAL_INT(0, s.explored_count);
    TEST_ASSERT_EQUAL_INT(EP_TOTAL_SYSTEMS, s.total_systems);
    TEST_ASSERT_EQUAL_INT(0, s.consecutive_days);
    TEST_ASSERT_EQUAL_INT(0, s.longest_streak);
    TEST_ASSERT_EQUAL_INT(0, s.total_visits);
    TEST_ASSERT_EQUAL_INT(0, s.bookmark_count);
    TEST_ASSERT_EQUAL_INT(0, s.cycle_count);
    TEST_ASSERT_TRUE(s.longest_session_sec < 0.001);
    TEST_ASSERT_TRUE(s.total_time_sec < 0.001);
}

void test_default_state_no_systems_explored(void) {
    ep_state_t s = ep_default_state();
    for (int i = 0; i < EP_TOTAL_SYSTEMS; i++) {
        TEST_ASSERT_EQUAL_INT(0, s.explored_systems[i]);
    }
}

/* ===== Basic Progress Computation ===== */

void test_compute_null_returns_empty(void) {
    ep_progress_t p = ep_compute(NULL);
    TEST_ASSERT_TRUE(p.system_coverage < 0.001);
    TEST_ASSERT_EQUAL_INT(0, p.milestone_count);
}

void test_compute_empty_state(void) {
    ep_state_t s = ep_default_state();
    ep_progress_t p = ep_compute(&s);
    TEST_ASSERT_TRUE(p.system_coverage < 0.001);
    TEST_ASSERT_EQUAL_INT(EP_TOTAL_SYSTEMS, p.systems_remaining);
    TEST_ASSERT_EQUAL_INT(0, p.current_streak);
    TEST_ASSERT_EQUAL_INT(0, p.streak_active);
}

/* ===== System Coverage ===== */

void test_coverage_one_system(void) {
    ep_state_t s = ep_default_state();
    s.explored_systems[0] = 1;
    s.explored_count = 1;
    ep_progress_t p = ep_compute(&s);
    TEST_ASSERT_TRUE(p.system_coverage > 0.06 && p.system_coverage < 0.07);
    TEST_ASSERT_EQUAL_INT(EP_TOTAL_SYSTEMS - 1, p.systems_remaining);
}

void test_coverage_half_systems(void) {
    ep_state_t s = ep_default_state();
    for (int i = 0; i < 8; i++) {
        s.explored_systems[i] = 1;
    }
    s.explored_count = 8;
    ep_progress_t p = ep_compute(&s);
    TEST_ASSERT_TRUE(p.system_coverage > 0.499 && p.system_coverage < 0.501);
    TEST_ASSERT_EQUAL_INT(8, p.systems_remaining);
}

void test_coverage_all_systems(void) {
    ep_state_t s = ep_default_state();
    for (int i = 0; i < EP_TOTAL_SYSTEMS; i++) {
        s.explored_systems[i] = 1;
    }
    s.explored_count = EP_TOTAL_SYSTEMS;
    ep_progress_t p = ep_compute(&s);
    TEST_ASSERT_TRUE(p.system_coverage > 0.999 && p.system_coverage <= 1.0);
    TEST_ASSERT_EQUAL_INT(0, p.systems_remaining);
}

/* ===== Streak Tracking ===== */

void test_no_streak(void) {
    ep_state_t s = ep_default_state();
    ep_progress_t p = ep_compute(&s);
    TEST_ASSERT_EQUAL_INT(0, p.current_streak);
    TEST_ASSERT_EQUAL_INT(0, p.longest_streak);
    TEST_ASSERT_EQUAL_INT(0, p.streak_active);
}

void test_streak_active(void) {
    ep_state_t s = ep_default_state();
    s.consecutive_days = 5;
    s.longest_streak = 10;
    ep_progress_t p = ep_compute(&s);
    TEST_ASSERT_EQUAL_INT(5, p.current_streak);
    TEST_ASSERT_EQUAL_INT(10, p.longest_streak);
    TEST_ASSERT_EQUAL_INT(1, p.streak_active);
}

void test_streak_is_longest(void) {
    ep_state_t s = ep_default_state();
    s.consecutive_days = 15;
    s.longest_streak = 15;
    ep_progress_t p = ep_compute(&s);
    TEST_ASSERT_EQUAL_INT(15, p.current_streak);
    TEST_ASSERT_EQUAL_INT(15, p.longest_streak);
}

/* ===== Milestone: First Visit ===== */

void test_milestone_first_visit_not_earned(void) {
    ep_state_t s = ep_default_state();
    ep_progress_t p = ep_compute(&s);
    TEST_ASSERT_EQUAL_INT(0, ep_has_milestone(&p, EP_MS_FIRST_VISIT));
}

void test_milestone_first_visit_earned(void) {
    ep_state_t s = ep_default_state();
    s.total_visits = 1;
    ep_progress_t p = ep_compute(&s);
    TEST_ASSERT_EQUAL_INT(1, ep_has_milestone(&p, EP_MS_FIRST_VISIT));
}

/* ===== Milestone: First System ===== */

void test_milestone_first_system_not_earned(void) {
    ep_state_t s = ep_default_state();
    ep_progress_t p = ep_compute(&s);
    TEST_ASSERT_EQUAL_INT(0, ep_has_milestone(&p, EP_MS_FIRST_SYSTEM));
}

void test_milestone_first_system_earned(void) {
    ep_state_t s = ep_default_state();
    s.explored_systems[3] = 1;
    s.explored_count = 1;
    ep_progress_t p = ep_compute(&s);
    TEST_ASSERT_EQUAL_INT(1, ep_has_milestone(&p, EP_MS_FIRST_SYSTEM));
}

/* ===== Milestone: Half Systems ===== */

void test_milestone_half_systems_not_earned(void) {
    ep_state_t s = ep_default_state();
    s.explored_count = 7;
    ep_progress_t p = ep_compute(&s);
    TEST_ASSERT_EQUAL_INT(0, ep_has_milestone(&p, EP_MS_HALF_SYSTEMS));
}

void test_milestone_half_systems_earned(void) {
    ep_state_t s = ep_default_state();
    for (int i = 0; i < 8; i++) s.explored_systems[i] = 1;
    s.explored_count = 8;
    ep_progress_t p = ep_compute(&s);
    TEST_ASSERT_EQUAL_INT(1, ep_has_milestone(&p, EP_MS_HALF_SYSTEMS));
}

/* ===== Milestone: All Systems ===== */

void test_milestone_all_systems_earned(void) {
    ep_state_t s = ep_default_state();
    for (int i = 0; i < EP_TOTAL_SYSTEMS; i++) s.explored_systems[i] = 1;
    s.explored_count = EP_TOTAL_SYSTEMS;
    ep_progress_t p = ep_compute(&s);
    TEST_ASSERT_EQUAL_INT(1, ep_has_milestone(&p, EP_MS_ALL_SYSTEMS));
}

/* ===== Milestone: Streaks ===== */

void test_milestone_streak_7(void) {
    ep_state_t s = ep_default_state();
    s.consecutive_days = 7;
    s.longest_streak = 7;
    ep_progress_t p = ep_compute(&s);
    TEST_ASSERT_EQUAL_INT(1, ep_has_milestone(&p, EP_MS_STREAK_7));
    TEST_ASSERT_EQUAL_INT(0, ep_has_milestone(&p, EP_MS_STREAK_30));
}

void test_milestone_streak_30(void) {
    ep_state_t s = ep_default_state();
    s.longest_streak = 30;
    ep_progress_t p = ep_compute(&s);
    TEST_ASSERT_EQUAL_INT(1, ep_has_milestone(&p, EP_MS_STREAK_7));
    TEST_ASSERT_EQUAL_INT(1, ep_has_milestone(&p, EP_MS_STREAK_30));
    TEST_ASSERT_EQUAL_INT(0, ep_has_milestone(&p, EP_MS_STREAK_100));
}

void test_milestone_streak_100(void) {
    ep_state_t s = ep_default_state();
    s.longest_streak = 100;
    ep_progress_t p = ep_compute(&s);
    TEST_ASSERT_EQUAL_INT(1, ep_has_milestone(&p, EP_MS_STREAK_100));
    TEST_ASSERT_EQUAL_INT(0, ep_has_milestone(&p, EP_MS_STREAK_365));
}

void test_milestone_streak_365(void) {
    ep_state_t s = ep_default_state();
    s.longest_streak = 365;
    ep_progress_t p = ep_compute(&s);
    TEST_ASSERT_EQUAL_INT(1, ep_has_milestone(&p, EP_MS_STREAK_365));
}

/* ===== Milestone: First Bookmark ===== */

void test_milestone_first_bookmark(void) {
    ep_state_t s = ep_default_state();
    s.bookmark_count = 1;
    ep_progress_t p = ep_compute(&s);
    TEST_ASSERT_EQUAL_INT(1, ep_has_milestone(&p, EP_MS_FIRST_BOOKMARK));
}

void test_milestone_first_bookmark_not_earned(void) {
    ep_state_t s = ep_default_state();
    ep_progress_t p = ep_compute(&s);
    TEST_ASSERT_EQUAL_INT(0, ep_has_milestone(&p, EP_MS_FIRST_BOOKMARK));
}

/* ===== Milestone: First Cycle ===== */

void test_milestone_first_cycle(void) {
    ep_state_t s = ep_default_state();
    s.cycle_count = 1;
    ep_progress_t p = ep_compute(&s);
    TEST_ASSERT_EQUAL_INT(1, ep_has_milestone(&p, EP_MS_FIRST_CYCLE));
}

/* ===== Milestone: Deep Dive ===== */

void test_milestone_deep_dive_not_earned(void) {
    ep_state_t s = ep_default_state();
    s.longest_session_sec = 1200.0; /* 20 minutes */
    ep_progress_t p = ep_compute(&s);
    TEST_ASSERT_EQUAL_INT(0, ep_has_milestone(&p, EP_MS_DEEP_DIVE));
}

void test_milestone_deep_dive_earned(void) {
    ep_state_t s = ep_default_state();
    s.longest_session_sec = 1800.0; /* 30 minutes */
    ep_progress_t p = ep_compute(&s);
    TEST_ASSERT_EQUAL_INT(1, ep_has_milestone(&p, EP_MS_DEEP_DIVE));
}

/* ===== Milestone Count ===== */

void test_milestone_count_none(void) {
    ep_state_t s = ep_default_state();
    ep_progress_t p = ep_compute(&s);
    TEST_ASSERT_EQUAL_INT(0, p.milestone_count);
}

void test_milestone_count_multiple(void) {
    ep_state_t s = ep_default_state();
    s.total_visits = 1;        /* FIRST_VISIT */
    s.explored_count = 1;      /* FIRST_SYSTEM */
    s.explored_systems[0] = 1;
    s.consecutive_days = 7;    /* STREAK_7 */
    s.longest_streak = 7;
    ep_progress_t p = ep_compute(&s);
    TEST_ASSERT_EQUAL_INT(3, p.milestone_count);
}

void test_earned_count_matches(void) {
    ep_state_t s = ep_default_state();
    s.total_visits = 5;
    s.explored_count = 2;
    s.bookmark_count = 3;
    ep_progress_t p = ep_compute(&s);
    TEST_ASSERT_EQUAL_INT(ep_earned_count(&p), p.milestone_count);
}

/* ===== Milestone Names ===== */

void test_milestone_name_valid(void) {
    TEST_ASSERT_EQUAL_STRING("First Visit", ep_milestone_name(EP_MS_FIRST_VISIT));
    TEST_ASSERT_EQUAL_STRING("First System", ep_milestone_name(EP_MS_FIRST_SYSTEM));
    TEST_ASSERT_EQUAL_STRING("Half Explorer", ep_milestone_name(EP_MS_HALF_SYSTEMS));
    TEST_ASSERT_EQUAL_STRING("Full Explorer", ep_milestone_name(EP_MS_ALL_SYSTEMS));
    TEST_ASSERT_EQUAL_STRING("Week Streak", ep_milestone_name(EP_MS_STREAK_7));
    TEST_ASSERT_EQUAL_STRING("Month Streak", ep_milestone_name(EP_MS_STREAK_30));
    TEST_ASSERT_EQUAL_STRING("Century Streak", ep_milestone_name(EP_MS_STREAK_100));
    TEST_ASSERT_EQUAL_STRING("Year Streak", ep_milestone_name(EP_MS_STREAK_365));
    TEST_ASSERT_EQUAL_STRING("First Bookmark", ep_milestone_name(EP_MS_FIRST_BOOKMARK));
    TEST_ASSERT_EQUAL_STRING("First Cycle", ep_milestone_name(EP_MS_FIRST_CYCLE));
    TEST_ASSERT_EQUAL_STRING("Deep Dive", ep_milestone_name(EP_MS_DEEP_DIVE));
}

void test_milestone_name_invalid(void) {
    TEST_ASSERT_EQUAL_STRING("?", ep_milestone_name(EP_MS_COUNT));
    TEST_ASSERT_EQUAL_STRING("?", ep_milestone_name(-1));
}

/* ===== Milestone Descriptions ===== */

void test_milestone_desc_valid(void) {
    const char *d = ep_milestone_desc(EP_MS_FIRST_VISIT);
    TEST_ASSERT_NOT_NULL(d);
    TEST_ASSERT_TRUE(strlen(d) > 0);
}

void test_milestone_desc_invalid(void) {
    TEST_ASSERT_EQUAL_STRING("?", ep_milestone_desc(EP_MS_COUNT));
}

/* ===== Next Milestone ===== */

void test_next_milestone_from_empty(void) {
    ep_state_t s = ep_default_state();
    ep_milestone_t next = ep_next_milestone(&s);
    /* First unearned should be FIRST_VISIT */
    TEST_ASSERT_EQUAL_INT(EP_MS_FIRST_VISIT, next);
}

void test_next_milestone_after_first_visit(void) {
    ep_state_t s = ep_default_state();
    s.total_visits = 1;
    ep_milestone_t next = ep_next_milestone(&s);
    TEST_ASSERT_EQUAL_INT(EP_MS_FIRST_SYSTEM, next);
}

void test_next_milestone_null(void) {
    ep_milestone_t next = ep_next_milestone(NULL);
    TEST_ASSERT_EQUAL_INT(EP_MS_COUNT, next);
}

void test_next_milestone_all_earned(void) {
    ep_state_t s = ep_default_state();
    s.total_visits = 1;
    for (int i = 0; i < EP_TOTAL_SYSTEMS; i++) s.explored_systems[i] = 1;
    s.explored_count = EP_TOTAL_SYSTEMS;
    s.longest_streak = 365;
    s.consecutive_days = 365;
    s.bookmark_count = 1;
    s.cycle_count = 1;
    s.longest_session_sec = 1800.0;
    ep_milestone_t next = ep_next_milestone(&s);
    TEST_ASSERT_EQUAL_INT(EP_MS_COUNT, next); /* all earned */
}

/* ===== has_milestone edge cases ===== */

void test_has_milestone_null(void) {
    TEST_ASSERT_EQUAL_INT(0, ep_has_milestone(NULL, EP_MS_FIRST_VISIT));
}

void test_has_milestone_invalid_id(void) {
    ep_state_t s = ep_default_state();
    ep_progress_t p = ep_compute(&s);
    TEST_ASSERT_EQUAL_INT(0, ep_has_milestone(&p, EP_MS_COUNT));
    TEST_ASSERT_EQUAL_INT(0, ep_has_milestone(&p, -1));
}

/* ===== earned_count edge cases ===== */

void test_earned_count_null(void) {
    TEST_ASSERT_EQUAL_INT(0, ep_earned_count(NULL));
}

/* ===== Format Insight ===== */

void test_format_insight_empty(void) {
    ep_state_t s = ep_default_state();
    ep_progress_t p = ep_compute(&s);
    char buf[128];
    int n = ep_format_insight(&p, buf, sizeof(buf));
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_TRUE(strlen(buf) > 0);
}

void test_format_insight_with_systems(void) {
    ep_state_t s = ep_default_state();
    for (int i = 0; i < 9; i++) s.explored_systems[i] = 1;
    s.explored_count = 9;
    ep_progress_t p = ep_compute(&s);
    char buf[256];
    ep_format_insight(&p, buf, sizeof(buf));
    /* Should mention "9" and "16" */
    TEST_ASSERT_NOT_NULL(strstr(buf, "9"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "16"));
}

void test_format_insight_with_streak(void) {
    ep_state_t s = ep_default_state();
    s.consecutive_days = 30;
    s.longest_streak = 30;
    ep_progress_t p = ep_compute(&s);
    char buf[256];
    ep_format_insight(&p, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "30"));
}

void test_format_insight_null_progress(void) {
    char buf[64];
    int n = ep_format_insight(NULL, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_INT(0, n);
    TEST_ASSERT_EQUAL_STRING("", buf);
}

void test_format_insight_null_buf(void) {
    ep_state_t s = ep_default_state();
    ep_progress_t p = ep_compute(&s);
    int n = ep_format_insight(&p, NULL, 0);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_format_insight_tiny_buf(void) {
    ep_state_t s = ep_default_state();
    s.explored_count = 5;
    ep_progress_t p = ep_compute(&s);
    char buf[4];
    int n = ep_format_insight(&p, buf, sizeof(buf));
    /* Should truncate safely */
    TEST_ASSERT_TRUE(n <= 3);
    TEST_ASSERT_TRUE(buf[3] == '\0' || n == 0);
}

/* ===== Insight Significance ===== */

void test_insight_significance_routine(void) {
    ep_state_t s = ep_default_state();
    s.explored_count = 3;
    s.total_visits = 5;
    ep_progress_t p = ep_compute(&s);
    TEST_ASSERT_EQUAL_INT(0, p.insight_significance);
}

void test_insight_significance_notable(void) {
    ep_state_t s = ep_default_state();
    for (int i = 0; i < 8; i++) s.explored_systems[i] = 1;
    s.explored_count = 8; /* half systems = notable */
    ep_progress_t p = ep_compute(&s);
    TEST_ASSERT_EQUAL_INT(1, p.insight_significance);
}

void test_insight_significance_celebration(void) {
    ep_state_t s = ep_default_state();
    for (int i = 0; i < EP_TOTAL_SYSTEMS; i++) s.explored_systems[i] = 1;
    s.explored_count = EP_TOTAL_SYSTEMS; /* all systems = celebration */
    ep_progress_t p = ep_compute(&s);
    TEST_ASSERT_EQUAL_INT(2, p.insight_significance);
}

int main(void) {
    UNITY_BEGIN();

    /* Default state */
    RUN_TEST(test_default_state_zeroed);
    RUN_TEST(test_default_state_no_systems_explored);

    /* Basic computation */
    RUN_TEST(test_compute_null_returns_empty);
    RUN_TEST(test_compute_empty_state);

    /* Coverage */
    RUN_TEST(test_coverage_one_system);
    RUN_TEST(test_coverage_half_systems);
    RUN_TEST(test_coverage_all_systems);

    /* Streaks */
    RUN_TEST(test_no_streak);
    RUN_TEST(test_streak_active);
    RUN_TEST(test_streak_is_longest);

    /* Milestones */
    RUN_TEST(test_milestone_first_visit_not_earned);
    RUN_TEST(test_milestone_first_visit_earned);
    RUN_TEST(test_milestone_first_system_not_earned);
    RUN_TEST(test_milestone_first_system_earned);
    RUN_TEST(test_milestone_half_systems_not_earned);
    RUN_TEST(test_milestone_half_systems_earned);
    RUN_TEST(test_milestone_all_systems_earned);
    RUN_TEST(test_milestone_streak_7);
    RUN_TEST(test_milestone_streak_30);
    RUN_TEST(test_milestone_streak_100);
    RUN_TEST(test_milestone_streak_365);
    RUN_TEST(test_milestone_first_bookmark);
    RUN_TEST(test_milestone_first_bookmark_not_earned);
    RUN_TEST(test_milestone_first_cycle);
    RUN_TEST(test_milestone_deep_dive_not_earned);
    RUN_TEST(test_milestone_deep_dive_earned);

    /* Milestone count */
    RUN_TEST(test_milestone_count_none);
    RUN_TEST(test_milestone_count_multiple);
    RUN_TEST(test_earned_count_matches);

    /* Milestone names */
    RUN_TEST(test_milestone_name_valid);
    RUN_TEST(test_milestone_name_invalid);

    /* Milestone descriptions */
    RUN_TEST(test_milestone_desc_valid);
    RUN_TEST(test_milestone_desc_invalid);

    /* Next milestone */
    RUN_TEST(test_next_milestone_from_empty);
    RUN_TEST(test_next_milestone_after_first_visit);
    RUN_TEST(test_next_milestone_null);
    RUN_TEST(test_next_milestone_all_earned);

    /* Edge cases */
    RUN_TEST(test_has_milestone_null);
    RUN_TEST(test_has_milestone_invalid_id);
    RUN_TEST(test_earned_count_null);

    /* Format insight */
    RUN_TEST(test_format_insight_empty);
    RUN_TEST(test_format_insight_with_systems);
    RUN_TEST(test_format_insight_with_streak);
    RUN_TEST(test_format_insight_null_progress);
    RUN_TEST(test_format_insight_null_buf);
    RUN_TEST(test_format_insight_tiny_buf);

    /* Significance */
    RUN_TEST(test_insight_significance_routine);
    RUN_TEST(test_insight_significance_notable);
    RUN_TEST(test_insight_significance_celebration);

    return UNITY_END();
}
