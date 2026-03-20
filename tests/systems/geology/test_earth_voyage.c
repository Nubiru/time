/* test_earth_voyage.c — Tests for Earth History Voyage narrative module.
 * TDD RED phase: all tests written before implementation. */

#include "../../unity/unity.h"
#include "../../../src/systems/geology/earth_voyage.h"

#include <string.h>

void setUp(void) { }
void tearDown(void) { }

/* --- Chapter count --- */

void test_ev_chapter_count_is_12(void) {
    TEST_ASSERT_EQUAL_INT(12, ev_chapter_count());
}

void test_ev_chapter_count_matches_define(void) {
    TEST_ASSERT_EQUAL_INT(EV_CHAPTER_COUNT, ev_chapter_count());
}

/* --- Chapter get by index --- */

void test_ev_chapter_get_index_0_birth_of_earth(void) {
    ev_chapter_t ch = ev_chapter_get(0);
    TEST_ASSERT_EQUAL_INT(0, ch.index);
    TEST_ASSERT_EQUAL_STRING("Birth of Earth", ch.title);
    TEST_ASSERT_FLOAT_WITHIN(1.0, 4540.0, ch.start_ma);
    TEST_ASSERT_FLOAT_WITHIN(1.0, 4000.0, ch.end_ma);
    TEST_ASSERT_NOT_NULL(ch.narrative);
    TEST_ASSERT_NOT_NULL(ch.visual_hint);
    TEST_ASSERT_GREATER_THAN(0, ch.event_count);
}

void test_ev_chapter_get_index_6_cambrian_explosion(void) {
    ev_chapter_t ch = ev_chapter_get(6);
    TEST_ASSERT_EQUAL_INT(6, ch.index);
    TEST_ASSERT_EQUAL_STRING("The Cambrian Explosion", ch.title);
    TEST_ASSERT_FLOAT_WITHIN(1.0, 540.0, ch.start_ma);
    TEST_ASSERT_FLOAT_WITHIN(1.0, 420.0, ch.end_ma);
}

void test_ev_chapter_get_index_9_dinosaur_dominion(void) {
    ev_chapter_t ch = ev_chapter_get(9);
    TEST_ASSERT_EQUAL_INT(9, ch.index);
    TEST_ASSERT_EQUAL_STRING("Dinosaur Dominion", ch.title);
    TEST_ASSERT_FLOAT_WITHIN(1.0, 200.0, ch.start_ma);
    TEST_ASSERT_FLOAT_WITHIN(1.0, 66.0, ch.end_ma);
}

void test_ev_chapter_get_index_11_rise_of_humanity(void) {
    ev_chapter_t ch = ev_chapter_get(11);
    TEST_ASSERT_EQUAL_INT(11, ch.index);
    TEST_ASSERT_EQUAL_STRING("Rise of Humanity", ch.title);
    TEST_ASSERT_FLOAT_WITHIN(1.0, 23.0, ch.start_ma);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 0.0, ch.end_ma);
}

void test_ev_chapter_get_invalid_negative(void) {
    ev_chapter_t ch = ev_chapter_get(-1);
    TEST_ASSERT_EQUAL_INT(-1, ch.index);
    TEST_ASSERT_NULL(ch.title);
}

void test_ev_chapter_get_invalid_too_large(void) {
    ev_chapter_t ch = ev_chapter_get(12);
    TEST_ASSERT_EQUAL_INT(-1, ch.index);
    TEST_ASSERT_NULL(ch.title);
}

void test_ev_chapter_get_invalid_999(void) {
    ev_chapter_t ch = ev_chapter_get(999);
    TEST_ASSERT_EQUAL_INT(-1, ch.index);
    TEST_ASSERT_NULL(ch.title);
}

/* --- Chapter data integrity --- */

void test_ev_all_chapters_have_non_null_title(void) {
    for (int i = 0; i < EV_CHAPTER_COUNT; i++) {
        ev_chapter_t ch = ev_chapter_get(i);
        TEST_ASSERT_NOT_NULL_MESSAGE(ch.title, "Chapter title is NULL");
    }
}

void test_ev_all_chapters_have_non_null_narrative(void) {
    for (int i = 0; i < EV_CHAPTER_COUNT; i++) {
        ev_chapter_t ch = ev_chapter_get(i);
        TEST_ASSERT_NOT_NULL_MESSAGE(ch.narrative, "Chapter narrative is NULL");
        TEST_ASSERT_TRUE_MESSAGE(strlen(ch.narrative) > 50,
            "Narrative should be at least 50 chars (multi-sentence)");
    }
}

void test_ev_all_chapters_have_non_null_visual_hint(void) {
    for (int i = 0; i < EV_CHAPTER_COUNT; i++) {
        ev_chapter_t ch = ev_chapter_get(i);
        TEST_ASSERT_NOT_NULL_MESSAGE(ch.visual_hint, "Visual hint is NULL");
        TEST_ASSERT_TRUE_MESSAGE(strlen(ch.visual_hint) > 0,
            "Visual hint must not be empty");
    }
}

void test_ev_all_chapters_have_at_least_one_event(void) {
    for (int i = 0; i < EV_CHAPTER_COUNT; i++) {
        ev_chapter_t ch = ev_chapter_get(i);
        TEST_ASSERT_GREATER_THAN_MESSAGE(0, ch.event_count,
            "Each chapter must have at least 1 event");
        for (int e = 0; e < ch.event_count; e++) {
            TEST_ASSERT_NOT_NULL_MESSAGE(ch.events[e],
                "Event string must not be NULL");
        }
    }
}

void test_ev_all_chapters_event_count_within_max(void) {
    for (int i = 0; i < EV_CHAPTER_COUNT; i++) {
        ev_chapter_t ch = ev_chapter_get(i);
        TEST_ASSERT_LESS_OR_EQUAL_MESSAGE(EV_EVENTS_PER_CHAPTER, ch.event_count,
            "event_count must not exceed EV_EVENTS_PER_CHAPTER");
    }
}

void test_ev_all_chapters_have_valid_index(void) {
    for (int i = 0; i < EV_CHAPTER_COUNT; i++) {
        ev_chapter_t ch = ev_chapter_get(i);
        TEST_ASSERT_EQUAL_INT(i, ch.index);
    }
}

/* --- Timeline coverage (no gaps, descending Ma order) --- */

void test_ev_chapters_descending_time_order(void) {
    /* start_ma should decrease chapter by chapter (older first) */
    for (int i = 1; i < EV_CHAPTER_COUNT; i++) {
        ev_chapter_t prev = ev_chapter_get(i - 1);
        ev_chapter_t curr = ev_chapter_get(i);
        TEST_ASSERT_TRUE_MESSAGE(prev.start_ma > curr.start_ma,
            "Chapters must be in descending start_ma order");
    }
}

void test_ev_chapters_end_ma_less_than_start_ma(void) {
    for (int i = 0; i < EV_CHAPTER_COUNT; i++) {
        ev_chapter_t ch = ev_chapter_get(i);
        TEST_ASSERT_TRUE_MESSAGE(ch.start_ma > ch.end_ma,
            "start_ma must be greater than end_ma (older to younger)");
    }
}

void test_ev_chapters_no_gaps(void) {
    /* Each chapter's end_ma should equal the next chapter's start_ma */
    for (int i = 0; i < EV_CHAPTER_COUNT - 1; i++) {
        ev_chapter_t curr = ev_chapter_get(i);
        ev_chapter_t next = ev_chapter_get(i + 1);
        TEST_ASSERT_FLOAT_WITHIN(0.01, curr.end_ma, next.start_ma);
    }
}

void test_ev_first_chapter_starts_at_4540(void) {
    ev_chapter_t ch = ev_chapter_get(0);
    TEST_ASSERT_FLOAT_WITHIN(1.0, 4540.0, ch.start_ma);
}

void test_ev_last_chapter_ends_at_0(void) {
    ev_chapter_t ch = ev_chapter_get(EV_CHAPTER_COUNT - 1);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 0.0, ch.end_ma);
}

/* --- Chapter at time --- */

void test_ev_chapter_at_time_4500_birth(void) {
    ev_chapter_t ch = ev_chapter_at_time(4500.0);
    TEST_ASSERT_EQUAL_INT(0, ch.index);
    TEST_ASSERT_EQUAL_STRING("Birth of Earth", ch.title);
}

void test_ev_chapter_at_time_3000_life_begins(void) {
    ev_chapter_t ch = ev_chapter_at_time(3000.0);
    TEST_ASSERT_EQUAL_INT(2, ch.index);
    TEST_ASSERT_EQUAL_STRING("Life Begins", ch.title);
}

void test_ev_chapter_at_time_500_cambrian(void) {
    ev_chapter_t ch = ev_chapter_at_time(500.0);
    TEST_ASSERT_EQUAL_INT(6, ch.index);
    TEST_ASSERT_EQUAL_STRING("The Cambrian Explosion", ch.title);
}

void test_ev_chapter_at_time_100_dinosaur_dominion(void) {
    ev_chapter_t ch = ev_chapter_at_time(100.0);
    TEST_ASSERT_EQUAL_INT(9, ch.index);
    TEST_ASSERT_EQUAL_STRING("Dinosaur Dominion", ch.title);
}

void test_ev_chapter_at_time_1_rise_of_humanity(void) {
    ev_chapter_t ch = ev_chapter_at_time(1.0);
    TEST_ASSERT_EQUAL_INT(11, ch.index);
    TEST_ASSERT_EQUAL_STRING("Rise of Humanity", ch.title);
}

void test_ev_chapter_at_time_0_rise_of_humanity(void) {
    /* 0.0 Ma = present = last chapter's end, inside last chapter */
    ev_chapter_t ch = ev_chapter_at_time(0.0);
    TEST_ASSERT_EQUAL_INT(11, ch.index);
}

void test_ev_chapter_at_time_boundary_exact(void) {
    /* At the boundary between chapters, belongs to the newer chapter */
    ev_chapter_t ch = ev_chapter_at_time(4000.0);
    TEST_ASSERT_EQUAL_INT(1, ch.index);
}

void test_ev_chapter_at_time_outside_range_too_old(void) {
    ev_chapter_t ch = ev_chapter_at_time(5000.0);
    TEST_ASSERT_EQUAL_INT(-1, ch.index);
    TEST_ASSERT_NULL(ch.title);
}

void test_ev_chapter_at_time_outside_range_negative(void) {
    ev_chapter_t ch = ev_chapter_at_time(-1.0);
    TEST_ASSERT_EQUAL_INT(-1, ch.index);
    TEST_ASSERT_NULL(ch.title);
}

/* --- Chapter title --- */

void test_ev_chapter_title_valid_indices(void) {
    TEST_ASSERT_EQUAL_STRING("Birth of Earth", ev_chapter_title(0));
    TEST_ASSERT_EQUAL_STRING("The First Oceans", ev_chapter_title(1));
    TEST_ASSERT_EQUAL_STRING("Life Begins", ev_chapter_title(2));
    TEST_ASSERT_EQUAL_STRING("The Great Oxidation", ev_chapter_title(3));
    TEST_ASSERT_EQUAL_STRING("The Boring Billion", ev_chapter_title(4));
    TEST_ASSERT_EQUAL_STRING("Snowball Earth", ev_chapter_title(5));
    TEST_ASSERT_EQUAL_STRING("The Cambrian Explosion", ev_chapter_title(6));
    TEST_ASSERT_EQUAL_STRING("Life Conquers Land", ev_chapter_title(7));
    TEST_ASSERT_EQUAL_STRING("Age of Reptiles Begins", ev_chapter_title(8));
    TEST_ASSERT_EQUAL_STRING("Dinosaur Dominion", ev_chapter_title(9));
    TEST_ASSERT_EQUAL_STRING("The Impact", ev_chapter_title(10));
    TEST_ASSERT_EQUAL_STRING("Rise of Humanity", ev_chapter_title(11));
}

void test_ev_chapter_title_invalid_returns_question_mark(void) {
    TEST_ASSERT_EQUAL_STRING("?", ev_chapter_title(-1));
    TEST_ASSERT_EQUAL_STRING("?", ev_chapter_title(12));
    TEST_ASSERT_EQUAL_STRING("?", ev_chapter_title(999));
}

/* --- Chapter format --- */

void test_ev_chapter_format_produces_output(void) {
    char buf[1024];
    int written = ev_chapter_format(0, buf, (int)sizeof(buf));
    TEST_ASSERT_GREATER_THAN(0, written);
    TEST_ASSERT_GREATER_THAN(10, (int)strlen(buf));
}

void test_ev_chapter_format_contains_title(void) {
    char buf[1024];
    ev_chapter_format(0, buf, (int)sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Birth of Earth"));
}

void test_ev_chapter_format_contains_time_range(void) {
    char buf[1024];
    ev_chapter_format(0, buf, (int)sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "4540"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "4000"));
}

void test_ev_chapter_format_invalid_returns_zero(void) {
    char buf[256];
    buf[0] = 'X';
    int written = ev_chapter_format(-1, buf, (int)sizeof(buf));
    TEST_ASSERT_EQUAL_INT(0, written);
}

void test_ev_chapter_format_null_buf_returns_zero(void) {
    int written = ev_chapter_format(0, NULL, 100);
    TEST_ASSERT_EQUAL_INT(0, written);
}

void test_ev_chapter_format_zero_size_returns_zero(void) {
    char buf[16];
    int written = ev_chapter_format(0, buf, 0);
    TEST_ASSERT_EQUAL_INT(0, written);
}

void test_ev_chapter_format_small_buf_truncates(void) {
    char buf[32];
    int written = ev_chapter_format(0, buf, (int)sizeof(buf));
    /* Should write something but less than a full format */
    TEST_ASSERT_GREATER_THAN(0, written);
    TEST_ASSERT_LESS_THAN((int)sizeof(buf), written);
}

/* --- main --- */

int main(void) {
    UNITY_BEGIN();

    /* Chapter count */
    RUN_TEST(test_ev_chapter_count_is_12);
    RUN_TEST(test_ev_chapter_count_matches_define);

    /* Chapter get by index */
    RUN_TEST(test_ev_chapter_get_index_0_birth_of_earth);
    RUN_TEST(test_ev_chapter_get_index_6_cambrian_explosion);
    RUN_TEST(test_ev_chapter_get_index_9_dinosaur_dominion);
    RUN_TEST(test_ev_chapter_get_index_11_rise_of_humanity);
    RUN_TEST(test_ev_chapter_get_invalid_negative);
    RUN_TEST(test_ev_chapter_get_invalid_too_large);
    RUN_TEST(test_ev_chapter_get_invalid_999);

    /* Data integrity */
    RUN_TEST(test_ev_all_chapters_have_non_null_title);
    RUN_TEST(test_ev_all_chapters_have_non_null_narrative);
    RUN_TEST(test_ev_all_chapters_have_non_null_visual_hint);
    RUN_TEST(test_ev_all_chapters_have_at_least_one_event);
    RUN_TEST(test_ev_all_chapters_event_count_within_max);
    RUN_TEST(test_ev_all_chapters_have_valid_index);

    /* Timeline coverage */
    RUN_TEST(test_ev_chapters_descending_time_order);
    RUN_TEST(test_ev_chapters_end_ma_less_than_start_ma);
    RUN_TEST(test_ev_chapters_no_gaps);
    RUN_TEST(test_ev_first_chapter_starts_at_4540);
    RUN_TEST(test_ev_last_chapter_ends_at_0);

    /* Chapter at time */
    RUN_TEST(test_ev_chapter_at_time_4500_birth);
    RUN_TEST(test_ev_chapter_at_time_3000_life_begins);
    RUN_TEST(test_ev_chapter_at_time_500_cambrian);
    RUN_TEST(test_ev_chapter_at_time_100_dinosaur_dominion);
    RUN_TEST(test_ev_chapter_at_time_1_rise_of_humanity);
    RUN_TEST(test_ev_chapter_at_time_0_rise_of_humanity);
    RUN_TEST(test_ev_chapter_at_time_boundary_exact);
    RUN_TEST(test_ev_chapter_at_time_outside_range_too_old);
    RUN_TEST(test_ev_chapter_at_time_outside_range_negative);

    /* Chapter title */
    RUN_TEST(test_ev_chapter_title_valid_indices);
    RUN_TEST(test_ev_chapter_title_invalid_returns_question_mark);

    /* Chapter format */
    RUN_TEST(test_ev_chapter_format_produces_output);
    RUN_TEST(test_ev_chapter_format_contains_title);
    RUN_TEST(test_ev_chapter_format_contains_time_range);
    RUN_TEST(test_ev_chapter_format_invalid_returns_zero);
    RUN_TEST(test_ev_chapter_format_null_buf_returns_zero);
    RUN_TEST(test_ev_chapter_format_zero_size_returns_zero);
    RUN_TEST(test_ev_chapter_format_small_buf_truncates);

    return UNITY_END();
}
