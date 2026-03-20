/* test_fossil_narrative.c — Tests for fossil narrative arcs module.
 * TDD RED phase: all tests written before implementation. */

#include "../../unity/unity.h"
#include "../../../src/systems/geology/fossil_narrative.h"

#include <string.h>

void setUp(void) { }
void tearDown(void) { }

/* --- Era count --- */

void test_fn_era_count_returns_8(void) {
    TEST_ASSERT_EQUAL_INT(8, fn_era_count());
}

void test_fn_era_count_matches_define(void) {
    TEST_ASSERT_EQUAL_INT(FN_ERA_COUNT, fn_era_count());
}

/* --- Era lookup by index --- */

void test_fn_era_get_index_0_primordial_soup(void) {
    fn_era_t e = fn_era_get(0);
    TEST_ASSERT_EQUAL_INT(0, e.index);
    TEST_ASSERT_EQUAL_STRING("Primordial Soup", e.name);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 4000.0f, (float)e.start_ma);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 3500.0f, (float)e.end_ma);
    TEST_ASSERT_NOT_NULL(e.narrative);
    TEST_ASSERT_NOT_NULL(e.key_species);
    TEST_ASSERT_NOT_NULL(e.rise_story);
    TEST_ASSERT_NOT_NULL(e.fall_story);
}

void test_fn_era_get_index_4_age_of_fish(void) {
    fn_era_t e = fn_era_get(4);
    TEST_ASSERT_EQUAL_INT(4, e.index);
    TEST_ASSERT_EQUAL_STRING("Age of Fish", e.name);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 480.0f, (float)e.start_ma);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 360.0f, (float)e.end_ma);
}

void test_fn_era_get_index_6_age_of_reptiles(void) {
    fn_era_t e = fn_era_get(6);
    TEST_ASSERT_EQUAL_INT(6, e.index);
    TEST_ASSERT_EQUAL_STRING("Age of Reptiles", e.name);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 300.0f, (float)e.start_ma);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 66.0f, (float)e.end_ma);
}

void test_fn_era_get_index_7_age_of_mammals(void) {
    fn_era_t e = fn_era_get(7);
    TEST_ASSERT_EQUAL_INT(7, e.index);
    TEST_ASSERT_EQUAL_STRING("Age of Mammals", e.name);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 66.0f, (float)e.start_ma);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, (float)e.end_ma);
}

void test_fn_era_get_invalid_negative(void) {
    fn_era_t e = fn_era_get(-1);
    TEST_ASSERT_EQUAL_INT(-1, e.index);
    TEST_ASSERT_NULL(e.name);
}

void test_fn_era_get_invalid_too_large(void) {
    fn_era_t e = fn_era_get(999);
    TEST_ASSERT_EQUAL_INT(-1, e.index);
    TEST_ASSERT_NULL(e.name);
}

/* --- Era at time --- */

void test_fn_era_at_time_3800_primordial(void) {
    fn_era_t e = fn_era_at_time(3800.0);
    TEST_ASSERT_EQUAL_STRING("Primordial Soup", e.name);
}

void test_fn_era_at_time_3000_bacteria(void) {
    fn_era_t e = fn_era_at_time(3000.0);
    TEST_ASSERT_EQUAL_STRING("Age of Bacteria", e.name);
}

void test_fn_era_at_time_1000_protists(void) {
    fn_era_t e = fn_era_at_time(1000.0);
    TEST_ASSERT_EQUAL_STRING("Age of Protists", e.name);
}

void test_fn_era_at_time_500_invertebrates(void) {
    fn_era_t e = fn_era_at_time(500.0);
    TEST_ASSERT_EQUAL_STRING("Age of Invertebrates", e.name);
}

void test_fn_era_at_time_400_fish(void) {
    fn_era_t e = fn_era_at_time(400.0);
    TEST_ASSERT_EQUAL_STRING("Age of Fish", e.name);
}

void test_fn_era_at_time_320_amphibians(void) {
    fn_era_t e = fn_era_at_time(320.0);
    TEST_ASSERT_EQUAL_STRING("Age of Amphibians", e.name);
}

void test_fn_era_at_time_150_reptiles(void) {
    fn_era_t e = fn_era_at_time(150.0);
    TEST_ASSERT_EQUAL_STRING("Age of Reptiles", e.name);
}

void test_fn_era_at_time_30_mammals(void) {
    fn_era_t e = fn_era_at_time(30.0);
    TEST_ASSERT_EQUAL_STRING("Age of Mammals", e.name);
}

void test_fn_era_at_time_outside_range_above(void) {
    fn_era_t e = fn_era_at_time(5000.0);
    TEST_ASSERT_EQUAL_INT(-1, e.index);
    TEST_ASSERT_NULL(e.name);
}

void test_fn_era_at_time_outside_range_below(void) {
    /* Negative Ma has no meaning */
    fn_era_t e = fn_era_at_time(-5.0);
    TEST_ASSERT_EQUAL_INT(-1, e.index);
    TEST_ASSERT_NULL(e.name);
}

void test_fn_era_at_time_boundary_start(void) {
    /* Exactly at era boundary: 3500 Ma is end of Primordial / start of Bacteria.
     * The boundary belongs to the older era (Primordial has end_ma=3500). */
    fn_era_t e = fn_era_at_time(3500.0);
    /* 3500 is the end_ma of era 0 and start_ma of era 1.
     * Convention: start_ma inclusive, end_ma exclusive for the era.
     * So 3500 should land in Age of Bacteria (index 1). */
    TEST_ASSERT_EQUAL_STRING("Age of Bacteria", e.name);
}

/* --- Era name --- */

void test_fn_era_name_valid(void) {
    TEST_ASSERT_EQUAL_STRING("Primordial Soup", fn_era_name(0));
    TEST_ASSERT_EQUAL_STRING("Age of Mammals", fn_era_name(7));
}

void test_fn_era_name_invalid(void) {
    TEST_ASSERT_EQUAL_STRING("?", fn_era_name(-1));
    TEST_ASSERT_EQUAL_STRING("?", fn_era_name(999));
}

/* --- Dinosaur event count --- */

void test_fn_dino_event_count_returns_10(void) {
    TEST_ASSERT_EQUAL_INT(10, fn_dino_event_count());
}

void test_fn_dino_event_count_matches_define(void) {
    TEST_ASSERT_EQUAL_INT(FN_DINO_EVENT_COUNT, fn_dino_event_count());
}

/* --- Dinosaur event lookup --- */

void test_fn_dino_event_get_index_0_first_archosaurs(void) {
    fn_dino_event_t d = fn_dino_event_get(0);
    TEST_ASSERT_EQUAL_INT(0, d.index);
    TEST_ASSERT_NOT_NULL(d.title);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 250.0f, (float)d.ma);
    TEST_ASSERT_NOT_NULL(d.story);
    TEST_ASSERT_NOT_NULL(d.significance);
}

void test_fn_dino_event_get_index_7_trex(void) {
    fn_dino_event_t d = fn_dino_event_get(7);
    TEST_ASSERT_EQUAL_INT(7, d.index);
    TEST_ASSERT_NOT_NULL(d.title);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 68.0f, (float)d.ma);
}

void test_fn_dino_event_get_index_9_asteroid(void) {
    fn_dino_event_t d = fn_dino_event_get(9);
    TEST_ASSERT_EQUAL_INT(9, d.index);
    TEST_ASSERT_NOT_NULL(d.title);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 66.0f, (float)d.ma);
}

void test_fn_dino_event_get_invalid_negative(void) {
    fn_dino_event_t d = fn_dino_event_get(-1);
    TEST_ASSERT_EQUAL_INT(-1, d.index);
    TEST_ASSERT_NULL(d.title);
}

void test_fn_dino_event_get_invalid_too_large(void) {
    fn_dino_event_t d = fn_dino_event_get(999);
    TEST_ASSERT_EQUAL_INT(-1, d.index);
    TEST_ASSERT_NULL(d.title);
}

/* --- Data integrity --- */

void test_fn_era_all_strings_non_null(void) {
    for (int i = 0; i < fn_era_count(); i++) {
        fn_era_t e = fn_era_get(i);
        TEST_ASSERT_NOT_NULL(e.name);
        TEST_ASSERT_NOT_NULL(e.narrative);
        TEST_ASSERT_NOT_NULL(e.key_species);
        TEST_ASSERT_NOT_NULL(e.rise_story);
        TEST_ASSERT_NOT_NULL(e.fall_story);
    }
}

void test_fn_dino_all_strings_non_null(void) {
    for (int i = 0; i < fn_dino_event_count(); i++) {
        fn_dino_event_t d = fn_dino_event_get(i);
        TEST_ASSERT_NOT_NULL(d.title);
        TEST_ASSERT_NOT_NULL(d.story);
        TEST_ASSERT_NOT_NULL(d.significance);
    }
}

void test_fn_era_start_before_end(void) {
    /* Each era: start_ma > end_ma (older time = larger Ma) */
    for (int i = 0; i < fn_era_count(); i++) {
        fn_era_t e = fn_era_get(i);
        TEST_ASSERT_TRUE_MESSAGE(
            e.start_ma > e.end_ma,
            "Era start_ma must be greater than end_ma (older first)");
    }
}

void test_fn_era_contiguous_coverage(void) {
    /* Each era's end_ma should equal the next era's start_ma */
    for (int i = 0; i < fn_era_count() - 1; i++) {
        fn_era_t curr = fn_era_get(i);
        fn_era_t next = fn_era_get(i + 1);
        TEST_ASSERT_FLOAT_WITHIN(0.1f, (float)curr.end_ma, (float)next.start_ma);
    }
}

void test_fn_era_chronological_order(void) {
    /* Eras should be in chronological order (oldest first = highest start_ma) */
    for (int i = 1; i < fn_era_count(); i++) {
        fn_era_t prev = fn_era_get(i - 1);
        fn_era_t curr = fn_era_get(i);
        TEST_ASSERT_TRUE_MESSAGE(
            prev.start_ma > curr.start_ma,
            "Eras must be in chronological order (oldest first)");
    }
}

void test_fn_dino_events_chronological_order(void) {
    /* Dinosaur events oldest first (highest Ma first) */
    for (int i = 1; i < fn_dino_event_count(); i++) {
        fn_dino_event_t prev = fn_dino_event_get(i - 1);
        fn_dino_event_t curr = fn_dino_event_get(i);
        TEST_ASSERT_TRUE_MESSAGE(
            prev.ma > curr.ma,
            "Dino events must be in chronological order (oldest first)");
    }
}

void test_fn_dino_events_within_reptile_era(void) {
    /* All dinosaur events should fall within the Age of Reptiles range.
     * Age of Reptiles is 300-66 Ma. But first archosaurs at 250 Ma
     * and asteroid at 66 Ma are within [300, 66]. */
    fn_era_t reptile_era = fn_era_get(6);
    for (int i = 0; i < fn_dino_event_count(); i++) {
        fn_dino_event_t d = fn_dino_event_get(i);
        TEST_ASSERT_TRUE_MESSAGE(
            d.ma <= reptile_era.start_ma && d.ma >= reptile_era.end_ma,
            "Dinosaur event must fall within Age of Reptiles time range");
    }
}

/* --- Format functions --- */

void test_fn_era_format_produces_output(void) {
    char buf[512];
    int len = fn_era_format(0, buf, (int)sizeof(buf));
    TEST_ASSERT_GREATER_THAN(0, len);
    TEST_ASSERT_LESS_THAN((int)sizeof(buf), len);
    /* Should contain era name */
    TEST_ASSERT_NOT_NULL(strstr(buf, "Primordial Soup"));
}

void test_fn_era_format_invalid_index(void) {
    char buf[256];
    int len = fn_era_format(-1, buf, (int)sizeof(buf));
    TEST_ASSERT_EQUAL_INT(0, len);
}

void test_fn_era_format_null_buffer(void) {
    int len = fn_era_format(0, NULL, 256);
    TEST_ASSERT_EQUAL_INT(0, len);
}

void test_fn_era_format_zero_size(void) {
    char buf[16];
    int len = fn_era_format(0, buf, 0);
    TEST_ASSERT_EQUAL_INT(0, len);
}

void test_fn_era_format_small_buffer_truncates(void) {
    char buf[32];
    int len = fn_era_format(0, buf, (int)sizeof(buf));
    /* Should write something even if truncated */
    TEST_ASSERT_GREATER_THAN(0, len);
    TEST_ASSERT_LESS_THAN((int)sizeof(buf), len);
}

void test_fn_dino_format_produces_output(void) {
    char buf[512];
    int len = fn_dino_format(0, buf, (int)sizeof(buf));
    TEST_ASSERT_GREATER_THAN(0, len);
    TEST_ASSERT_LESS_THAN((int)sizeof(buf), len);
}

void test_fn_dino_format_invalid_index(void) {
    char buf[256];
    int len = fn_dino_format(-1, buf, (int)sizeof(buf));
    TEST_ASSERT_EQUAL_INT(0, len);
}

void test_fn_dino_format_null_buffer(void) {
    int len = fn_dino_format(0, NULL, 256);
    TEST_ASSERT_EQUAL_INT(0, len);
}

/* --- Narrative content quality --- */

void test_fn_era_narratives_have_substance(void) {
    /* Each narrative should be at least 80 characters (a few sentences) */
    for (int i = 0; i < fn_era_count(); i++) {
        fn_era_t e = fn_era_get(i);
        TEST_ASSERT_GREATER_THAN(80, (int)strlen(e.narrative));
    }
}

void test_fn_dino_stories_have_substance(void) {
    /* Each dino story should be at least 80 characters */
    for (int i = 0; i < fn_dino_event_count(); i++) {
        fn_dino_event_t d = fn_dino_event_get(i);
        TEST_ASSERT_GREATER_THAN(80, (int)strlen(d.story));
    }
}

/* --- main --- */

int main(void) {
    UNITY_BEGIN();

    /* Era count */
    RUN_TEST(test_fn_era_count_returns_8);
    RUN_TEST(test_fn_era_count_matches_define);

    /* Era lookup by index */
    RUN_TEST(test_fn_era_get_index_0_primordial_soup);
    RUN_TEST(test_fn_era_get_index_4_age_of_fish);
    RUN_TEST(test_fn_era_get_index_6_age_of_reptiles);
    RUN_TEST(test_fn_era_get_index_7_age_of_mammals);
    RUN_TEST(test_fn_era_get_invalid_negative);
    RUN_TEST(test_fn_era_get_invalid_too_large);

    /* Era at time */
    RUN_TEST(test_fn_era_at_time_3800_primordial);
    RUN_TEST(test_fn_era_at_time_3000_bacteria);
    RUN_TEST(test_fn_era_at_time_1000_protists);
    RUN_TEST(test_fn_era_at_time_500_invertebrates);
    RUN_TEST(test_fn_era_at_time_400_fish);
    RUN_TEST(test_fn_era_at_time_320_amphibians);
    RUN_TEST(test_fn_era_at_time_150_reptiles);
    RUN_TEST(test_fn_era_at_time_30_mammals);
    RUN_TEST(test_fn_era_at_time_outside_range_above);
    RUN_TEST(test_fn_era_at_time_outside_range_below);
    RUN_TEST(test_fn_era_at_time_boundary_start);

    /* Era name */
    RUN_TEST(test_fn_era_name_valid);
    RUN_TEST(test_fn_era_name_invalid);

    /* Dinosaur event count */
    RUN_TEST(test_fn_dino_event_count_returns_10);
    RUN_TEST(test_fn_dino_event_count_matches_define);

    /* Dinosaur event lookup */
    RUN_TEST(test_fn_dino_event_get_index_0_first_archosaurs);
    RUN_TEST(test_fn_dino_event_get_index_7_trex);
    RUN_TEST(test_fn_dino_event_get_index_9_asteroid);
    RUN_TEST(test_fn_dino_event_get_invalid_negative);
    RUN_TEST(test_fn_dino_event_get_invalid_too_large);

    /* Data integrity */
    RUN_TEST(test_fn_era_all_strings_non_null);
    RUN_TEST(test_fn_dino_all_strings_non_null);
    RUN_TEST(test_fn_era_start_before_end);
    RUN_TEST(test_fn_era_contiguous_coverage);
    RUN_TEST(test_fn_era_chronological_order);
    RUN_TEST(test_fn_dino_events_chronological_order);
    RUN_TEST(test_fn_dino_events_within_reptile_era);

    /* Format functions */
    RUN_TEST(test_fn_era_format_produces_output);
    RUN_TEST(test_fn_era_format_invalid_index);
    RUN_TEST(test_fn_era_format_null_buffer);
    RUN_TEST(test_fn_era_format_zero_size);
    RUN_TEST(test_fn_era_format_small_buffer_truncates);
    RUN_TEST(test_fn_dino_format_produces_output);
    RUN_TEST(test_fn_dino_format_invalid_index);
    RUN_TEST(test_fn_dino_format_null_buffer);

    /* Narrative content quality */
    RUN_TEST(test_fn_era_narratives_have_substance);
    RUN_TEST(test_fn_dino_stories_have_substance);

    return UNITY_END();
}
