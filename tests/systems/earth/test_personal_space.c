/* test_personal_space.c -- Tests for personal space data module.
 * TDD: tests written first, implementation follows. */

#include "../../unity/unity.h"
#include "../../../src/systems/earth/personal_space.h"

#include <string.h>
#include <math.h>

void setUp(void) { }
void tearDown(void) { }

/* ===== ps_create ===== */

void test_create_empty_space(void) {
    ps_space_t s = ps_create();
    TEST_ASSERT_EQUAL_INT(0, s.favorite_count);
    TEST_ASSERT_EQUAL_INT(0, s.cycle_count);
    TEST_ASSERT_EQUAL_INT(0, s.bookmark_count);
    TEST_ASSERT_EQUAL_INT(0, s.explored_count);
}

void test_create_archetype_defaults(void) {
    ps_space_t s = ps_create();
    TEST_ASSERT_EQUAL_INT(-1, s.archetype.kin_seal);
    TEST_ASSERT_EQUAL_INT(-1, s.archetype.kin_tone);
    TEST_ASSERT_EQUAL_INT(-1, s.archetype.zodiac_sign);
    TEST_ASSERT_EQUAL_INT(-1, s.archetype.chinese_animal);
    TEST_ASSERT_EQUAL_INT(0, s.archetype.hexagram);
    TEST_ASSERT_EQUAL_INT(0, s.archetype.archetype_score);
}

void test_create_favorites_terminated(void) {
    ps_space_t s = ps_create();
    TEST_ASSERT_EQUAL_INT(-1, s.favorite_systems[0]);
}

/* ===== ps_add_favorite / ps_remove_favorite / ps_is_favorite ===== */

void test_add_favorite(void) {
    ps_space_t s = ps_create();
    s = ps_add_favorite(s, 3);
    TEST_ASSERT_EQUAL_INT(1, s.favorite_count);
    TEST_ASSERT_EQUAL_INT(3, s.favorite_systems[0]);
}

void test_add_multiple_favorites(void) {
    ps_space_t s = ps_create();
    s = ps_add_favorite(s, 1);
    s = ps_add_favorite(s, 5);
    s = ps_add_favorite(s, 9);
    TEST_ASSERT_EQUAL_INT(3, s.favorite_count);
    TEST_ASSERT_EQUAL_INT(1, s.favorite_systems[0]);
    TEST_ASSERT_EQUAL_INT(5, s.favorite_systems[1]);
    TEST_ASSERT_EQUAL_INT(9, s.favorite_systems[2]);
}

void test_add_duplicate_favorite_ignored(void) {
    ps_space_t s = ps_create();
    s = ps_add_favorite(s, 3);
    s = ps_add_favorite(s, 3);
    TEST_ASSERT_EQUAL_INT(1, s.favorite_count);
}

void test_add_favorite_overflow(void) {
    ps_space_t s = ps_create();
    for (int i = 0; i < PS_MAX_FAVORITES; i++) {
        s = ps_add_favorite(s, i);
    }
    TEST_ASSERT_EQUAL_INT(PS_MAX_FAVORITES, s.favorite_count);
    /* One more should be ignored */
    s = ps_add_favorite(s, 99);
    TEST_ASSERT_EQUAL_INT(PS_MAX_FAVORITES, s.favorite_count);
}

void test_is_favorite_true(void) {
    ps_space_t s = ps_create();
    s = ps_add_favorite(s, 7);
    TEST_ASSERT_EQUAL_INT(1, ps_is_favorite(&s, 7));
}

void test_is_favorite_false(void) {
    ps_space_t s = ps_create();
    s = ps_add_favorite(s, 7);
    TEST_ASSERT_EQUAL_INT(0, ps_is_favorite(&s, 2));
}

void test_is_favorite_empty(void) {
    ps_space_t s = ps_create();
    TEST_ASSERT_EQUAL_INT(0, ps_is_favorite(&s, 0));
}

void test_remove_favorite(void) {
    ps_space_t s = ps_create();
    s = ps_add_favorite(s, 3);
    s = ps_add_favorite(s, 7);
    s = ps_remove_favorite(s, 3);
    TEST_ASSERT_EQUAL_INT(1, s.favorite_count);
    TEST_ASSERT_EQUAL_INT(0, ps_is_favorite(&s, 3));
    TEST_ASSERT_EQUAL_INT(1, ps_is_favorite(&s, 7));
}

void test_remove_favorite_not_found(void) {
    ps_space_t s = ps_create();
    s = ps_add_favorite(s, 3);
    s = ps_remove_favorite(s, 99);
    TEST_ASSERT_EQUAL_INT(1, s.favorite_count);
}

void test_add_favorite_functional(void) {
    ps_space_t s = ps_create();
    ps_space_t s2 = ps_add_favorite(s, 5);
    /* Original unchanged */
    TEST_ASSERT_EQUAL_INT(0, s.favorite_count);
    TEST_ASSERT_EQUAL_INT(1, s2.favorite_count);
}

/* ===== ps_add_cycle / ps_remove_cycle / ps_toggle_cycle ===== */

void test_add_cycle(void) {
    ps_space_t s = ps_create();
    s = ps_add_cycle(s, "Saturn Return", 2, 2460000.0, 2460300.0);
    TEST_ASSERT_EQUAL_INT(1, s.cycle_count);
    TEST_ASSERT_EQUAL_STRING("Saturn Return", s.cycles[0].name);
    TEST_ASSERT_EQUAL_INT(2, s.cycles[0].system_id);
    TEST_ASSERT_FLOAT_WITHIN(0.1, 2460000.0, s.cycles[0].start_jd);
    TEST_ASSERT_FLOAT_WITHIN(0.1, 2460300.0, s.cycles[0].next_event_jd);
    TEST_ASSERT_EQUAL_INT(1, s.cycles[0].active);
}

void test_add_cycle_overflow(void) {
    ps_space_t s = ps_create();
    for (int i = 0; i < PS_MAX_CYCLES; i++) {
        s = ps_add_cycle(s, "cycle", 0, 0.0, 0.0);
    }
    TEST_ASSERT_EQUAL_INT(PS_MAX_CYCLES, s.cycle_count);
    s = ps_add_cycle(s, "overflow", 0, 0.0, 0.0);
    TEST_ASSERT_EQUAL_INT(PS_MAX_CYCLES, s.cycle_count);
}

void test_remove_cycle(void) {
    ps_space_t s = ps_create();
    s = ps_add_cycle(s, "A", 0, 0.0, 0.0);
    s = ps_add_cycle(s, "B", 1, 0.0, 0.0);
    s = ps_remove_cycle(s, 0);
    TEST_ASSERT_EQUAL_INT(1, s.cycle_count);
    TEST_ASSERT_EQUAL_STRING("B", s.cycles[0].name);
}

void test_remove_cycle_invalid_index(void) {
    ps_space_t s = ps_create();
    s = ps_add_cycle(s, "A", 0, 0.0, 0.0);
    s = ps_remove_cycle(s, 5);
    TEST_ASSERT_EQUAL_INT(1, s.cycle_count);
    s = ps_remove_cycle(s, -1);
    TEST_ASSERT_EQUAL_INT(1, s.cycle_count);
}

void test_toggle_cycle(void) {
    ps_space_t s = ps_create();
    s = ps_add_cycle(s, "X", 0, 0.0, 0.0);
    TEST_ASSERT_EQUAL_INT(1, s.cycles[0].active);
    s = ps_toggle_cycle(s, 0);
    TEST_ASSERT_EQUAL_INT(0, s.cycles[0].active);
    s = ps_toggle_cycle(s, 0);
    TEST_ASSERT_EQUAL_INT(1, s.cycles[0].active);
}

void test_toggle_cycle_invalid_index(void) {
    ps_space_t s = ps_create();
    s = ps_add_cycle(s, "X", 0, 0.0, 0.0);
    s = ps_toggle_cycle(s, 10);
    /* No crash, active unchanged */
    TEST_ASSERT_EQUAL_INT(1, s.cycles[0].active);
}

void test_add_cycle_name_truncation(void) {
    ps_space_t s = ps_create();
    char long_name[200];
    memset(long_name, 'A', sizeof(long_name) - 1);
    long_name[sizeof(long_name) - 1] = '\0';
    s = ps_add_cycle(s, long_name, 0, 0.0, 0.0);
    TEST_ASSERT_EQUAL_INT(1, s.cycle_count);
    TEST_ASSERT_TRUE(strlen(s.cycles[0].name) < PS_CYCLE_NAME_MAX);
}

/* ===== ps_bookmark / ps_remove_bookmark ===== */

void test_add_bookmark(void) {
    ps_space_t s = ps_create();
    s = ps_bookmark(s, 2460000.5, "Winter solstice", 0);
    TEST_ASSERT_EQUAL_INT(1, s.bookmark_count);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 2460000.5, s.bookmarks[0].jd);
    TEST_ASSERT_EQUAL_STRING("Winter solstice", s.bookmarks[0].note);
    TEST_ASSERT_EQUAL_INT(0, s.bookmarks[0].system_id);
}

void test_add_bookmark_overflow(void) {
    ps_space_t s = ps_create();
    for (int i = 0; i < PS_MAX_BOOKMARKS; i++) {
        s = ps_bookmark(s, (double)i, "note", 0);
    }
    TEST_ASSERT_EQUAL_INT(PS_MAX_BOOKMARKS, s.bookmark_count);
    s = ps_bookmark(s, 9999.0, "overflow", 0);
    TEST_ASSERT_EQUAL_INT(PS_MAX_BOOKMARKS, s.bookmark_count);
}

void test_remove_bookmark(void) {
    ps_space_t s = ps_create();
    s = ps_bookmark(s, 100.0, "A", 0);
    s = ps_bookmark(s, 200.0, "B", 1);
    s = ps_remove_bookmark(s, 0);
    TEST_ASSERT_EQUAL_INT(1, s.bookmark_count);
    TEST_ASSERT_EQUAL_STRING("B", s.bookmarks[0].note);
}

void test_remove_bookmark_invalid_index(void) {
    ps_space_t s = ps_create();
    s = ps_bookmark(s, 100.0, "A", 0);
    s = ps_remove_bookmark(s, 5);
    TEST_ASSERT_EQUAL_INT(1, s.bookmark_count);
    s = ps_remove_bookmark(s, -1);
    TEST_ASSERT_EQUAL_INT(1, s.bookmark_count);
}

void test_bookmark_note_truncation(void) {
    ps_space_t s = ps_create();
    char long_note[300];
    memset(long_note, 'N', sizeof(long_note) - 1);
    long_note[sizeof(long_note) - 1] = '\0';
    s = ps_bookmark(s, 100.0, long_note, 0);
    TEST_ASSERT_EQUAL_INT(1, s.bookmark_count);
    TEST_ASSERT_TRUE(strlen(s.bookmarks[0].note) < PS_NOTE_MAX);
}

/* ===== ps_mark_explored / ps_is_explored / ps_explored_count ===== */

void test_mark_explored(void) {
    ps_space_t s = ps_create();
    s = ps_mark_explored(s, 4);
    TEST_ASSERT_EQUAL_INT(1, s.explored_count);
    TEST_ASSERT_EQUAL_INT(1, ps_is_explored(&s, 4));
}

void test_mark_explored_duplicate(void) {
    ps_space_t s = ps_create();
    s = ps_mark_explored(s, 4);
    s = ps_mark_explored(s, 4);
    TEST_ASSERT_EQUAL_INT(1, s.explored_count);
}

void test_is_explored_false(void) {
    ps_space_t s = ps_create();
    TEST_ASSERT_EQUAL_INT(0, ps_is_explored(&s, 4));
}

void test_explored_count(void) {
    ps_space_t s = ps_create();
    s = ps_mark_explored(s, 0);
    s = ps_mark_explored(s, 3);
    s = ps_mark_explored(s, 7);
    TEST_ASSERT_EQUAL_INT(3, ps_explored_count(&s));
}

void test_mark_explored_overflow(void) {
    ps_space_t s = ps_create();
    for (int i = 0; i < PS_MAX_EXPLORED; i++) {
        s = ps_mark_explored(s, i);
    }
    TEST_ASSERT_EQUAL_INT(PS_MAX_EXPLORED, s.explored_count);
    s = ps_mark_explored(s, 99);
    TEST_ASSERT_EQUAL_INT(PS_MAX_EXPLORED, s.explored_count);
}

/* ===== ps_exploration_progress ===== */

void test_exploration_progress_zero(void) {
    ps_space_t s = ps_create();
    double prog = ps_exploration_progress(&s, 16);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.0, prog);
}

void test_exploration_progress_partial(void) {
    ps_space_t s = ps_create();
    s = ps_mark_explored(s, 0);
    s = ps_mark_explored(s, 1);
    s = ps_mark_explored(s, 2);
    s = ps_mark_explored(s, 3);
    double prog = ps_exploration_progress(&s, 16);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.25, prog);
}

void test_exploration_progress_full(void) {
    ps_space_t s = ps_create();
    for (int i = 0; i < 16; i++) {
        s = ps_mark_explored(s, i);
    }
    double prog = ps_exploration_progress(&s, 16);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 1.0, prog);
}

void test_exploration_progress_clamped(void) {
    /* More explored than total (hypothetically) */
    ps_space_t s = ps_create();
    for (int i = 0; i < 10; i++) {
        s = ps_mark_explored(s, i);
    }
    double prog = ps_exploration_progress(&s, 5);
    TEST_ASSERT_TRUE(prog <= 1.0);
}

void test_exploration_progress_zero_total(void) {
    ps_space_t s = ps_create();
    double prog = ps_exploration_progress(&s, 0);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.0, prog);
}

/* ===== ps_compute_archetype ===== */

void test_archetype_basic(void) {
    ps_archetype_t a = ps_compute_archetype(0, 1, 0, 0, 1);
    TEST_ASSERT_EQUAL_INT(0, a.kin_seal);
    TEST_ASSERT_EQUAL_INT(1, a.kin_tone);
    TEST_ASSERT_EQUAL_INT(0, a.zodiac_sign);
    TEST_ASSERT_EQUAL_INT(0, a.chinese_animal);
    TEST_ASSERT_EQUAL_INT(1, a.hexagram);
    TEST_ASSERT_TRUE(a.archetype_score >= 0);
    TEST_ASSERT_TRUE(a.archetype_score <= 100);
}

void test_archetype_no_birth_data(void) {
    ps_archetype_t a = ps_compute_archetype(-1, -1, -1, -1, 0);
    TEST_ASSERT_EQUAL_INT(-1, a.kin_seal);
    TEST_ASSERT_EQUAL_INT(-1, a.kin_tone);
    TEST_ASSERT_EQUAL_INT(-1, a.zodiac_sign);
    TEST_ASSERT_EQUAL_INT(-1, a.chinese_animal);
    TEST_ASSERT_EQUAL_INT(0, a.hexagram);
    TEST_ASSERT_EQUAL_INT(0, a.archetype_score);
}

void test_archetype_score_varies(void) {
    ps_archetype_t a1 = ps_compute_archetype(0, 1, 0, 0, 1);
    ps_archetype_t a2 = ps_compute_archetype(10, 13, 6, 6, 32);
    /* Different inputs should generally produce different scores */
    TEST_ASSERT_TRUE(a1.archetype_score != a2.archetype_score ||
                     (a1.kin_tone == a2.kin_tone &&
                      a1.zodiac_sign == a2.zodiac_sign &&
                      a1.chinese_animal == a2.chinese_animal));
}

void test_archetype_score_range(void) {
    /* Test several combos to ensure score stays 0-100 */
    for (int tone = 1; tone <= 13; tone++) {
        for (int sign = 0; sign < 12; sign += 3) {
            ps_archetype_t a = ps_compute_archetype(0, tone, sign, sign, 1);
            TEST_ASSERT_TRUE(a.archetype_score >= 0);
            TEST_ASSERT_TRUE(a.archetype_score <= 100);
        }
    }
}

/* ===== ps_active_cycles ===== */

void test_active_cycles_all_active(void) {
    ps_space_t s = ps_create();
    s = ps_add_cycle(s, "A", 0, 0.0, 0.0);
    s = ps_add_cycle(s, "B", 1, 0.0, 0.0);
    ps_cycle_t out[PS_MAX_CYCLES];
    int n = ps_active_cycles(&s, out, PS_MAX_CYCLES);
    TEST_ASSERT_EQUAL_INT(2, n);
}

void test_active_cycles_with_paused(void) {
    ps_space_t s = ps_create();
    s = ps_add_cycle(s, "A", 0, 0.0, 0.0);
    s = ps_add_cycle(s, "B", 1, 0.0, 0.0);
    s = ps_toggle_cycle(s, 1); /* Pause B */
    ps_cycle_t out[PS_MAX_CYCLES];
    int n = ps_active_cycles(&s, out, PS_MAX_CYCLES);
    TEST_ASSERT_EQUAL_INT(1, n);
    TEST_ASSERT_EQUAL_STRING("A", out[0].name);
}

void test_active_cycles_none(void) {
    ps_space_t s = ps_create();
    s = ps_add_cycle(s, "A", 0, 0.0, 0.0);
    s = ps_toggle_cycle(s, 0);
    ps_cycle_t out[PS_MAX_CYCLES];
    int n = ps_active_cycles(&s, out, PS_MAX_CYCLES);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_active_cycles_limited(void) {
    ps_space_t s = ps_create();
    s = ps_add_cycle(s, "A", 0, 0.0, 0.0);
    s = ps_add_cycle(s, "B", 1, 0.0, 0.0);
    s = ps_add_cycle(s, "C", 2, 0.0, 0.0);
    ps_cycle_t out[2];
    int n = ps_active_cycles(&s, out, 2);
    TEST_ASSERT_EQUAL_INT(2, n);
}

/* ===== ps_upcoming_bookmarks ===== */

void test_upcoming_bookmarks_in_range(void) {
    ps_space_t s = ps_create();
    s = ps_bookmark(s, 100.0, "early", 0);
    s = ps_bookmark(s, 200.0, "mid", 0);
    s = ps_bookmark(s, 300.0, "late", 0);
    ps_bookmark_t out[PS_MAX_BOOKMARKS];
    int n = ps_upcoming_bookmarks(&s, 150.0, 350.0, out, PS_MAX_BOOKMARKS);
    TEST_ASSERT_EQUAL_INT(2, n);
}

void test_upcoming_bookmarks_sorted(void) {
    ps_space_t s = ps_create();
    s = ps_bookmark(s, 300.0, "C", 0);
    s = ps_bookmark(s, 100.0, "A", 0);
    s = ps_bookmark(s, 200.0, "B", 0);
    ps_bookmark_t out[PS_MAX_BOOKMARKS];
    int n = ps_upcoming_bookmarks(&s, 50.0, 400.0, out, PS_MAX_BOOKMARKS);
    TEST_ASSERT_EQUAL_INT(3, n);
    TEST_ASSERT_TRUE(out[0].jd <= out[1].jd);
    TEST_ASSERT_TRUE(out[1].jd <= out[2].jd);
}

void test_upcoming_bookmarks_none_in_range(void) {
    ps_space_t s = ps_create();
    s = ps_bookmark(s, 100.0, "A", 0);
    ps_bookmark_t out[PS_MAX_BOOKMARKS];
    int n = ps_upcoming_bookmarks(&s, 200.0, 300.0, out, PS_MAX_BOOKMARKS);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_upcoming_bookmarks_limited(void) {
    ps_space_t s = ps_create();
    s = ps_bookmark(s, 100.0, "A", 0);
    s = ps_bookmark(s, 200.0, "B", 0);
    s = ps_bookmark(s, 300.0, "C", 0);
    ps_bookmark_t out[1];
    int n = ps_upcoming_bookmarks(&s, 50.0, 400.0, out, 1);
    TEST_ASSERT_EQUAL_INT(1, n);
}

/* ===== ps_summary ===== */

void test_summary_empty(void) {
    ps_space_t s = ps_create();
    char buf[256];
    int len = ps_summary(&s, buf, sizeof(buf));
    TEST_ASSERT_TRUE(len > 0);
    /* Should contain "0 favorites" */
    TEST_ASSERT_NOT_NULL(strstr(buf, "0 favorites"));
}

void test_summary_with_data(void) {
    ps_space_t s = ps_create();
    s = ps_add_favorite(s, 1);
    s = ps_add_favorite(s, 2);
    s = ps_add_favorite(s, 3);
    s = ps_add_cycle(s, "A", 0, 0.0, 0.0);
    s = ps_add_cycle(s, "B", 1, 0.0, 0.0);
    s = ps_bookmark(s, 100.0, "test", 0);
    for (int i = 0; i < 12; i++) {
        s = ps_mark_explored(s, i);
    }
    char buf[256];
    int len = ps_summary(&s, buf, sizeof(buf));
    TEST_ASSERT_TRUE(len > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "3 favorites"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "2 active cycles"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "1 bookmarks"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "12"));
}

void test_summary_truncation(void) {
    ps_space_t s = ps_create();
    char buf[10];
    int len = ps_summary(&s, buf, sizeof(buf));
    TEST_ASSERT_TRUE(len > 0);
    TEST_ASSERT_TRUE(len < 10);
}

/* --- Runner --- */

int main(void) {
    UNITY_BEGIN();

    /* Create */
    RUN_TEST(test_create_empty_space);
    RUN_TEST(test_create_archetype_defaults);
    RUN_TEST(test_create_favorites_terminated);

    /* Favorites */
    RUN_TEST(test_add_favorite);
    RUN_TEST(test_add_multiple_favorites);
    RUN_TEST(test_add_duplicate_favorite_ignored);
    RUN_TEST(test_add_favorite_overflow);
    RUN_TEST(test_is_favorite_true);
    RUN_TEST(test_is_favorite_false);
    RUN_TEST(test_is_favorite_empty);
    RUN_TEST(test_remove_favorite);
    RUN_TEST(test_remove_favorite_not_found);
    RUN_TEST(test_add_favorite_functional);

    /* Cycles */
    RUN_TEST(test_add_cycle);
    RUN_TEST(test_add_cycle_overflow);
    RUN_TEST(test_remove_cycle);
    RUN_TEST(test_remove_cycle_invalid_index);
    RUN_TEST(test_toggle_cycle);
    RUN_TEST(test_toggle_cycle_invalid_index);
    RUN_TEST(test_add_cycle_name_truncation);

    /* Bookmarks */
    RUN_TEST(test_add_bookmark);
    RUN_TEST(test_add_bookmark_overflow);
    RUN_TEST(test_remove_bookmark);
    RUN_TEST(test_remove_bookmark_invalid_index);
    RUN_TEST(test_bookmark_note_truncation);

    /* Explored */
    RUN_TEST(test_mark_explored);
    RUN_TEST(test_mark_explored_duplicate);
    RUN_TEST(test_is_explored_false);
    RUN_TEST(test_explored_count);
    RUN_TEST(test_mark_explored_overflow);

    /* Exploration progress */
    RUN_TEST(test_exploration_progress_zero);
    RUN_TEST(test_exploration_progress_partial);
    RUN_TEST(test_exploration_progress_full);
    RUN_TEST(test_exploration_progress_clamped);
    RUN_TEST(test_exploration_progress_zero_total);

    /* Archetype */
    RUN_TEST(test_archetype_basic);
    RUN_TEST(test_archetype_no_birth_data);
    RUN_TEST(test_archetype_score_varies);
    RUN_TEST(test_archetype_score_range);

    /* Active cycles */
    RUN_TEST(test_active_cycles_all_active);
    RUN_TEST(test_active_cycles_with_paused);
    RUN_TEST(test_active_cycles_none);
    RUN_TEST(test_active_cycles_limited);

    /* Upcoming bookmarks */
    RUN_TEST(test_upcoming_bookmarks_in_range);
    RUN_TEST(test_upcoming_bookmarks_sorted);
    RUN_TEST(test_upcoming_bookmarks_none_in_range);
    RUN_TEST(test_upcoming_bookmarks_limited);

    /* Summary */
    RUN_TEST(test_summary_empty);
    RUN_TEST(test_summary_with_data);
    RUN_TEST(test_summary_truncation);

    return UNITY_END();
}
