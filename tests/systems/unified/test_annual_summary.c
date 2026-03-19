/* test_annual_summary.c — Tests for "Your Year in Time" annual summary.
 * TDD: tests written first, implementation follows. */

#include "unity.h"
#include "systems/unified/annual_summary.h"
#include "systems/unified/birth_profile.h"

#include <string.h>

/* Shared: born 1990-06-15, summarizing year 2026 */
static birth_profile_t s_profile;
static as_summary_t s_summary;
static as_summary_t s_no_engagement;

void setUp(void) {
    s_profile = bp_compute(1990, 6, 15);

    as_engagement_t eng = as_default_engagement();
    eng.systems_explored = 9;
    eng.total_visits = 120;
    eng.longest_streak = 30;
    eng.bookmarks_saved = 5;
    eng.total_hours = 42.5;
    s_summary = as_compute(&s_profile, 2026, &eng);
    s_no_engagement = as_compute(&s_profile, 2026, NULL);
}

void tearDown(void) {}

/* ===== Default Engagement ===== */

void test_default_engagement_zeroed(void) {
    as_engagement_t e = as_default_engagement();
    TEST_ASSERT_EQUAL_INT(0, e.systems_explored);
    TEST_ASSERT_EQUAL_INT(0, e.total_visits);
    TEST_ASSERT_EQUAL_INT(0, e.longest_streak);
    TEST_ASSERT_EQUAL_INT(0, e.bookmarks_saved);
    TEST_ASSERT_TRUE(e.total_hours < 0.001);
}

/* ===== NULL Safety ===== */

void test_null_profile_returns_empty(void) {
    as_engagement_t e = as_default_engagement();
    as_summary_t s = as_compute(NULL, 2026, &e);
    TEST_ASSERT_EQUAL_INT(0, s.highlight_count);
    TEST_ASSERT_EQUAL_INT(0, s.stats.year);
}

void test_null_engagement_still_works(void) {
    TEST_ASSERT_TRUE(s_no_engagement.highlight_count > 0);
    TEST_ASSERT_EQUAL_INT(2026, s_no_engagement.stats.year);
}

/* ===== Stats ===== */

void test_stats_year(void) {
    TEST_ASSERT_EQUAL_INT(2026, s_summary.stats.year);
}

void test_stats_age(void) {
    /* Born 1990, year 2026 -> turning 36 */
    TEST_ASSERT_EQUAL_INT(36, s_summary.stats.age);
}

void test_stats_not_leap(void) {
    TEST_ASSERT_EQUAL_INT(0, s_summary.stats.is_leap);
}

void test_stats_leap_year(void) {
    as_summary_t s = as_compute(&s_profile, 2024, NULL);
    TEST_ASSERT_EQUAL_INT(1, s.stats.is_leap);
}

void test_stats_birthday_kin_valid(void) {
    TEST_ASSERT_TRUE(s_summary.stats.birthday_kin >= 1);
    TEST_ASSERT_TRUE(s_summary.stats.birthday_kin <= 260);
}

void test_stats_birthday_seal_valid(void) {
    TEST_ASSERT_TRUE(s_summary.stats.birthday_seal >= 0);
    TEST_ASSERT_TRUE(s_summary.stats.birthday_seal <= 19);
}

void test_stats_birthday_tone_valid(void) {
    TEST_ASSERT_TRUE(s_summary.stats.birthday_tone >= 1);
    TEST_ASSERT_TRUE(s_summary.stats.birthday_tone <= 13);
}

void test_stats_birthday_hexagram_valid(void) {
    TEST_ASSERT_TRUE(s_summary.stats.birthday_hexagram >= 1);
    TEST_ASSERT_TRUE(s_summary.stats.birthday_hexagram <= 64);
}

void test_stats_chinese_animal_valid(void) {
    TEST_ASSERT_TRUE(s_summary.stats.chinese_animal >= 0);
    TEST_ASSERT_TRUE(s_summary.stats.chinese_animal <= 11);
}

void test_stats_chinese_element_valid(void) {
    TEST_ASSERT_TRUE(s_summary.stats.chinese_element >= 0);
    TEST_ASSERT_TRUE(s_summary.stats.chinese_element <= 4);
}

void test_stats_galactic_returns_range(void) {
    /* 260-day cycle in 365-day year: always 1 or 2 returns */
    TEST_ASSERT_TRUE(s_summary.stats.galactic_returns >= 1);
    TEST_ASSERT_TRUE(s_summary.stats.galactic_returns <= 2);
}

void test_stats_wavespells(void) {
    /* 365/13 = 28 */
    TEST_ASSERT_TRUE(s_summary.stats.wavespells >= 27);
    TEST_ASSERT_TRUE(s_summary.stats.wavespells <= 29);
}

/* ===== Highlights ===== */

void test_highlights_count_positive(void) {
    TEST_ASSERT_TRUE(s_summary.highlight_count > 0);
}

void test_highlights_count_within_max(void) {
    TEST_ASSERT_TRUE(s_summary.highlight_count <= AS_MAX_HIGHLIGHTS);
}

void test_highlights_have_headlines(void) {
    for (int i = 0; i < s_summary.highlight_count; i++) {
        TEST_ASSERT_TRUE(strlen(s_summary.highlights[i].headline) > 0);
    }
}

void test_highlights_have_bodies(void) {
    for (int i = 0; i < s_summary.highlight_count; i++) {
        TEST_ASSERT_TRUE(strlen(s_summary.highlights[i].body) > 0);
    }
}

void test_highlights_valid_categories(void) {
    for (int i = 0; i < s_summary.highlight_count; i++) {
        TEST_ASSERT_TRUE((int)s_summary.highlights[i].category >= 0);
        TEST_ASSERT_TRUE(s_summary.highlights[i].category < AS_CAT_COUNT);
    }
}

void test_highlights_valid_significance(void) {
    for (int i = 0; i < s_summary.highlight_count; i++) {
        TEST_ASSERT_TRUE(s_summary.highlights[i].significance >= AS_SIG_ROUTINE);
        TEST_ASSERT_TRUE(s_summary.highlights[i].significance <= AS_SIG_EXTRAORDINARY);
    }
}

void test_has_birthday_highlight(void) {
    int found = 0;
    for (int i = 0; i < s_summary.highlight_count; i++) {
        if (s_summary.highlights[i].category == AS_CAT_BIRTHDAY) {
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);
}

void test_has_cycle_highlight(void) {
    int found = 0;
    for (int i = 0; i < s_summary.highlight_count; i++) {
        if (s_summary.highlights[i].category == AS_CAT_CYCLE) {
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);
}

/* ===== Engagement Highlights ===== */

void test_engagement_adds_journey_highlight(void) {
    int with_eng = 0;
    int without_eng = 0;
    for (int i = 0; i < s_summary.highlight_count; i++) {
        if (s_summary.highlights[i].category == AS_CAT_JOURNEY) with_eng++;
    }
    for (int i = 0; i < s_no_engagement.highlight_count; i++) {
        if (s_no_engagement.highlights[i].category == AS_CAT_JOURNEY)
            without_eng++;
    }
    TEST_ASSERT_TRUE(with_eng > without_eng);
}

void test_no_engagement_fewer_highlights(void) {
    TEST_ASSERT_TRUE(s_no_engagement.highlight_count < s_summary.highlight_count);
}

/* ===== Top Highlights ===== */

void test_top_highlights_sorted(void) {
    as_highlight_t top[AS_MAX_HIGHLIGHTS];
    int n = as_top_highlights(&s_summary, top, AS_MAX_HIGHLIGHTS);
    TEST_ASSERT_TRUE(n > 0);
    for (int i = 1; i < n; i++) {
        TEST_ASSERT_TRUE(top[i - 1].significance >= top[i].significance);
    }
}

void test_top_highlights_limited(void) {
    as_highlight_t top[2];
    int n = as_top_highlights(&s_summary, top, 2);
    TEST_ASSERT_TRUE(n <= 2);
}

void test_top_highlights_null(void) {
    int n = as_top_highlights(NULL, NULL, 0);
    TEST_ASSERT_EQUAL_INT(0, n);
}

/* ===== Filter by Category ===== */

void test_by_category_birthday(void) {
    as_highlight_t out[AS_MAX_HIGHLIGHTS];
    int n = as_by_category(&s_summary, AS_CAT_BIRTHDAY, out, AS_MAX_HIGHLIGHTS);
    TEST_ASSERT_TRUE(n > 0);
    for (int i = 0; i < n; i++) {
        TEST_ASSERT_EQUAL_INT(AS_CAT_BIRTHDAY, out[i].category);
    }
}

void test_by_category_null(void) {
    int n = as_by_category(NULL, AS_CAT_BIRTHDAY, NULL, 0);
    TEST_ASSERT_EQUAL_INT(0, n);
}

/* ===== Count Significant ===== */

void test_count_significant_routine(void) {
    /* Every highlight has significance >= ROUTINE (0) */
    int n = as_count_significant(&s_summary, AS_SIG_ROUTINE);
    TEST_ASSERT_EQUAL_INT(s_summary.highlight_count, n);
}

void test_count_significant_notable(void) {
    int n = as_count_significant(&s_summary, AS_SIG_NOTABLE);
    TEST_ASSERT_TRUE(n <= s_summary.highlight_count);
}

void test_count_significant_null(void) {
    int n = as_count_significant(NULL, AS_SIG_ROUTINE);
    TEST_ASSERT_EQUAL_INT(0, n);
}

/* ===== Category Names ===== */

void test_category_name_birthday(void) {
    TEST_ASSERT_EQUAL_STRING("Birthday", as_category_name(AS_CAT_BIRTHDAY));
}

void test_category_name_cycle(void) {
    TEST_ASSERT_EQUAL_STRING("Cycle", as_category_name(AS_CAT_CYCLE));
}

void test_category_name_coincidence(void) {
    TEST_ASSERT_EQUAL_STRING("Coincidence", as_category_name(AS_CAT_COINCIDENCE));
}

void test_category_name_milestone(void) {
    TEST_ASSERT_EQUAL_STRING("Milestone", as_category_name(AS_CAT_MILESTONE));
}

void test_category_name_journey(void) {
    TEST_ASSERT_EQUAL_STRING("Journey", as_category_name(AS_CAT_JOURNEY));
}

void test_category_name_invalid(void) {
    TEST_ASSERT_EQUAL_STRING("?", as_category_name(AS_CAT_COUNT));
    TEST_ASSERT_EQUAL_STRING("?", as_category_name((as_category_t)-1));
}

/* ===== Significance Names ===== */

void test_significance_name_routine(void) {
    TEST_ASSERT_EQUAL_STRING("Routine", as_significance_name(AS_SIG_ROUTINE));
}

void test_significance_name_notable(void) {
    TEST_ASSERT_EQUAL_STRING("Notable", as_significance_name(AS_SIG_NOTABLE));
}

void test_significance_name_rare(void) {
    TEST_ASSERT_EQUAL_STRING("Rare", as_significance_name(AS_SIG_RARE));
}

void test_significance_name_extraordinary(void) {
    TEST_ASSERT_EQUAL_STRING("Extraordinary",
                             as_significance_name(AS_SIG_EXTRAORDINARY));
}

void test_significance_name_invalid(void) {
    TEST_ASSERT_EQUAL_STRING("?", as_significance_name((as_significance_t)-1));
    TEST_ASSERT_EQUAL_STRING("?", as_significance_name((as_significance_t)4));
}

/* ===== Narrative Text ===== */

void test_opening_not_empty(void) {
    TEST_ASSERT_TRUE(strlen(s_summary.opening) > 0);
}

void test_closing_not_empty(void) {
    TEST_ASSERT_TRUE(strlen(s_summary.closing) > 0);
}

/* ===== Format Summary ===== */

void test_format_summary(void) {
    char buf[4096];
    int n = as_format_summary(&s_summary, buf, (int)sizeof(buf));
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_TRUE(strlen(buf) > 0);
}

void test_format_summary_null(void) {
    int n = as_format_summary(NULL, NULL, 0);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_format_summary_small_buf(void) {
    char buf[32];
    int n = as_format_summary(&s_summary, buf, (int)sizeof(buf));
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_TRUE(n < 32);
}

/* ===== Age Milestone Detection ===== */

void test_milestone_age_18(void) {
    birth_profile_t p = bp_compute(2000, 1, 1);
    as_summary_t s = as_compute(&p, 2018, NULL);
    int found = 0;
    for (int i = 0; i < s.highlight_count; i++) {
        if (s.highlights[i].category == AS_CAT_MILESTONE) {
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);
}

void test_no_milestone_age_19(void) {
    birth_profile_t p = bp_compute(2000, 1, 1);
    as_summary_t s = as_compute(&p, 2019, NULL);
    int milestone_count = 0;
    for (int i = 0; i < s.highlight_count; i++) {
        if (s.highlights[i].category == AS_CAT_MILESTONE)
            milestone_count++;
    }
    TEST_ASSERT_EQUAL_INT(0, milestone_count);
}

void test_milestone_age_52_calendar_round(void) {
    birth_profile_t p = bp_compute(1970, 3, 1);
    as_summary_t s = as_compute(&p, 2022, NULL);
    int found_rare = 0;
    for (int i = 0; i < s.highlight_count; i++) {
        if (s.highlights[i].category == AS_CAT_MILESTONE &&
            s.highlights[i].significance >= AS_SIG_RARE) {
            found_rare = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found_rare);
}

void test_milestone_age_60_sexagenary(void) {
    birth_profile_t p = bp_compute(1960, 6, 1);
    as_summary_t s = as_compute(&p, 2020, NULL);
    int found_rare = 0;
    for (int i = 0; i < s.highlight_count; i++) {
        if (s.highlights[i].category == AS_CAT_MILESTONE &&
            s.highlights[i].significance >= AS_SIG_RARE) {
            found_rare = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found_rare);
}

void test_milestone_age_100_extraordinary(void) {
    birth_profile_t p = bp_compute(1920, 4, 10);
    as_summary_t s = as_compute(&p, 2020, NULL);
    int found = 0;
    for (int i = 0; i < s.highlight_count; i++) {
        if (s.highlights[i].category == AS_CAT_MILESTONE &&
            s.highlights[i].significance == AS_SIG_EXTRAORDINARY) {
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);
}

/* ===== Different Years ===== */

void test_different_years_different_kins(void) {
    as_summary_t s1 = as_compute(&s_profile, 2025, NULL);
    as_summary_t s2 = as_compute(&s_profile, 2026, NULL);
    /* 365 mod 260 = 105, so birthday kin shifts each year */
    TEST_ASSERT_TRUE(s1.stats.birthday_kin != s2.stats.birthday_kin);
}

void test_different_years_different_animals(void) {
    as_summary_t s1 = as_compute(&s_profile, 2025, NULL);
    as_summary_t s2 = as_compute(&s_profile, 2026, NULL);
    /* Consecutive years always have different Chinese animals */
    TEST_ASSERT_TRUE(s1.stats.chinese_animal != s2.stats.chinese_animal);
}

/* ===== Streak Significance ===== */

void test_long_streak_notable(void) {
    as_engagement_t eng = as_default_engagement();
    eng.total_visits = 50;
    eng.longest_streak = 45;
    eng.systems_explored = 12;
    eng.total_hours = 60.0;
    as_summary_t s = as_compute(&s_profile, 2026, &eng);
    int found_notable_journey = 0;
    for (int i = 0; i < s.highlight_count; i++) {
        if (s.highlights[i].category == AS_CAT_JOURNEY &&
            s.highlights[i].significance >= AS_SIG_NOTABLE) {
            found_notable_journey = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found_notable_journey);
}

void test_short_streak_routine(void) {
    as_engagement_t eng = as_default_engagement();
    eng.total_visits = 10;
    eng.longest_streak = 5;
    eng.systems_explored = 3;
    eng.total_hours = 4.0;
    as_summary_t s = as_compute(&s_profile, 2026, &eng);
    int found_routine_journey = 0;
    for (int i = 0; i < s.highlight_count; i++) {
        if (s.highlights[i].category == AS_CAT_JOURNEY &&
            s.highlights[i].significance == AS_SIG_ROUTINE) {
            found_routine_journey = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found_routine_journey);
}

int main(void) {
    UNITY_BEGIN();

    /* Default engagement */
    RUN_TEST(test_default_engagement_zeroed);

    /* NULL safety */
    RUN_TEST(test_null_profile_returns_empty);
    RUN_TEST(test_null_engagement_still_works);

    /* Stats */
    RUN_TEST(test_stats_year);
    RUN_TEST(test_stats_age);
    RUN_TEST(test_stats_not_leap);
    RUN_TEST(test_stats_leap_year);
    RUN_TEST(test_stats_birthday_kin_valid);
    RUN_TEST(test_stats_birthday_seal_valid);
    RUN_TEST(test_stats_birthday_tone_valid);
    RUN_TEST(test_stats_birthday_hexagram_valid);
    RUN_TEST(test_stats_chinese_animal_valid);
    RUN_TEST(test_stats_chinese_element_valid);
    RUN_TEST(test_stats_galactic_returns_range);
    RUN_TEST(test_stats_wavespells);

    /* Highlights */
    RUN_TEST(test_highlights_count_positive);
    RUN_TEST(test_highlights_count_within_max);
    RUN_TEST(test_highlights_have_headlines);
    RUN_TEST(test_highlights_have_bodies);
    RUN_TEST(test_highlights_valid_categories);
    RUN_TEST(test_highlights_valid_significance);
    RUN_TEST(test_has_birthday_highlight);
    RUN_TEST(test_has_cycle_highlight);

    /* Engagement */
    RUN_TEST(test_engagement_adds_journey_highlight);
    RUN_TEST(test_no_engagement_fewer_highlights);

    /* Top highlights */
    RUN_TEST(test_top_highlights_sorted);
    RUN_TEST(test_top_highlights_limited);
    RUN_TEST(test_top_highlights_null);

    /* Filter by category */
    RUN_TEST(test_by_category_birthday);
    RUN_TEST(test_by_category_null);

    /* Count significant */
    RUN_TEST(test_count_significant_routine);
    RUN_TEST(test_count_significant_notable);
    RUN_TEST(test_count_significant_null);

    /* Category names */
    RUN_TEST(test_category_name_birthday);
    RUN_TEST(test_category_name_cycle);
    RUN_TEST(test_category_name_coincidence);
    RUN_TEST(test_category_name_milestone);
    RUN_TEST(test_category_name_journey);
    RUN_TEST(test_category_name_invalid);

    /* Significance names */
    RUN_TEST(test_significance_name_routine);
    RUN_TEST(test_significance_name_notable);
    RUN_TEST(test_significance_name_rare);
    RUN_TEST(test_significance_name_extraordinary);
    RUN_TEST(test_significance_name_invalid);

    /* Narrative */
    RUN_TEST(test_opening_not_empty);
    RUN_TEST(test_closing_not_empty);

    /* Format */
    RUN_TEST(test_format_summary);
    RUN_TEST(test_format_summary_null);
    RUN_TEST(test_format_summary_small_buf);

    /* Age milestones */
    RUN_TEST(test_milestone_age_18);
    RUN_TEST(test_no_milestone_age_19);
    RUN_TEST(test_milestone_age_52_calendar_round);
    RUN_TEST(test_milestone_age_60_sexagenary);
    RUN_TEST(test_milestone_age_100_extraordinary);

    /* Different years */
    RUN_TEST(test_different_years_different_kins);
    RUN_TEST(test_different_years_different_animals);

    /* Streak significance */
    RUN_TEST(test_long_streak_notable);
    RUN_TEST(test_short_streak_routine);

    return UNITY_END();
}
