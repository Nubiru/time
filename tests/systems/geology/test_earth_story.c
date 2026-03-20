/* test_earth_story.c — Tests for Earth Story cinematic moments module.
 * TDD RED phase: all tests written before implementation. */

#include "../../unity/unity.h"
#include "../../../src/systems/geology/earth_story.h"

#include <string.h>
#include <math.h>

void setUp(void) { }
void tearDown(void) { }

/* --- Moment count --- */

void test_es_moment_count_is_20(void) {
    TEST_ASSERT_EQUAL_INT(20, es_moment_count());
}

void test_es_moment_count_matches_define(void) {
    TEST_ASSERT_EQUAL_INT(ES_MOMENT_COUNT, es_moment_count());
}

/* --- Moment get by index --- */

void test_es_moment_get_index_0_birth_of_earth(void) {
    es_moment_t m = es_moment_get(0);
    TEST_ASSERT_EQUAL_INT(0, m.index);
    TEST_ASSERT_EQUAL_STRING("Birth of Earth", m.title);
    TEST_ASSERT_FLOAT_WITHIN(1.0, 4540.0, m.ma);
    TEST_ASSERT_EQUAL_INT(ES_DRAMA_CATASTROPHIC, m.drama);
    TEST_ASSERT_NOT_NULL(m.visual);
    TEST_ASSERT_NOT_NULL(m.narrative);
    TEST_ASSERT_NOT_NULL(m.aftermath);
    TEST_ASSERT_NOT_NULL(m.sound_hint);
}

void test_es_moment_get_index_1_theia_impact(void) {
    es_moment_t m = es_moment_get(1);
    TEST_ASSERT_EQUAL_INT(1, m.index);
    TEST_ASSERT_EQUAL_STRING("Theia Impact", m.title);
    TEST_ASSERT_FLOAT_WITHIN(1.0, 4500.0, m.ma);
    TEST_ASSERT_EQUAL_INT(ES_DRAMA_APOCALYPTIC, m.drama);
}

void test_es_moment_get_index_10_the_great_dying(void) {
    es_moment_t m = es_moment_get(10);
    TEST_ASSERT_EQUAL_INT(10, m.index);
    TEST_ASSERT_EQUAL_STRING("The Great Dying", m.title);
    TEST_ASSERT_FLOAT_WITHIN(1.0, 252.0, m.ma);
    TEST_ASSERT_EQUAL_INT(ES_DRAMA_APOCALYPTIC, m.drama);
}

void test_es_moment_get_index_15_the_asteroid(void) {
    es_moment_t m = es_moment_get(15);
    TEST_ASSERT_EQUAL_INT(15, m.index);
    TEST_ASSERT_EQUAL_STRING("The Asteroid", m.title);
    TEST_ASSERT_FLOAT_WITHIN(1.0, 66.0, m.ma);
    TEST_ASSERT_EQUAL_INT(ES_DRAMA_APOCALYPTIC, m.drama);
}

void test_es_moment_get_index_19_anthropocene(void) {
    es_moment_t m = es_moment_get(19);
    TEST_ASSERT_EQUAL_INT(19, m.index);
    TEST_ASSERT_EQUAL_STRING("The Anthropocene", m.title);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.0001, m.ma);
    TEST_ASSERT_EQUAL_INT(ES_DRAMA_CATASTROPHIC, m.drama);
}

void test_es_moment_get_invalid_negative(void) {
    es_moment_t m = es_moment_get(-1);
    TEST_ASSERT_EQUAL_INT(-1, m.index);
    TEST_ASSERT_NULL(m.title);
}

void test_es_moment_get_invalid_too_large(void) {
    es_moment_t m = es_moment_get(20);
    TEST_ASSERT_EQUAL_INT(-1, m.index);
    TEST_ASSERT_NULL(m.title);
}

void test_es_moment_get_invalid_999(void) {
    es_moment_t m = es_moment_get(999);
    TEST_ASSERT_EQUAL_INT(-1, m.index);
    TEST_ASSERT_NULL(m.title);
}

/* --- Data integrity: all moments --- */

void test_es_all_moments_have_non_null_title(void) {
    for (int i = 0; i < ES_MOMENT_COUNT; i++) {
        es_moment_t m = es_moment_get(i);
        TEST_ASSERT_NOT_NULL_MESSAGE(m.title, "Moment title is NULL");
    }
}

void test_es_all_moments_have_non_null_visual(void) {
    for (int i = 0; i < ES_MOMENT_COUNT; i++) {
        es_moment_t m = es_moment_get(i);
        TEST_ASSERT_NOT_NULL_MESSAGE(m.visual, "Moment visual is NULL");
        TEST_ASSERT_TRUE_MESSAGE(strlen(m.visual) > 30,
            "Visual should be at least 30 chars (2-3 sentences)");
    }
}

void test_es_all_moments_have_non_null_narrative(void) {
    for (int i = 0; i < ES_MOMENT_COUNT; i++) {
        es_moment_t m = es_moment_get(i);
        TEST_ASSERT_NOT_NULL_MESSAGE(m.narrative, "Moment narrative is NULL");
        TEST_ASSERT_TRUE_MESSAGE(strlen(m.narrative) > 80,
            "Narrative should be at least 80 chars (3-5 sentences)");
    }
}

void test_es_all_moments_have_non_null_aftermath(void) {
    for (int i = 0; i < ES_MOMENT_COUNT; i++) {
        es_moment_t m = es_moment_get(i);
        TEST_ASSERT_NOT_NULL_MESSAGE(m.aftermath, "Moment aftermath is NULL");
        TEST_ASSERT_TRUE_MESSAGE(strlen(m.aftermath) > 20,
            "Aftermath should be at least 20 chars");
    }
}

void test_es_all_moments_have_non_null_sound_hint(void) {
    for (int i = 0; i < ES_MOMENT_COUNT; i++) {
        es_moment_t m = es_moment_get(i);
        TEST_ASSERT_NOT_NULL_MESSAGE(m.sound_hint, "Moment sound_hint is NULL");
        TEST_ASSERT_TRUE_MESSAGE(strlen(m.sound_hint) > 0,
            "Sound hint must not be empty");
    }
}

void test_es_all_moments_have_valid_index(void) {
    for (int i = 0; i < ES_MOMENT_COUNT; i++) {
        es_moment_t m = es_moment_get(i);
        TEST_ASSERT_EQUAL_INT(i, m.index);
    }
}

void test_es_all_moments_drama_within_range(void) {
    for (int i = 0; i < ES_MOMENT_COUNT; i++) {
        es_moment_t m = es_moment_get(i);
        TEST_ASSERT_GREATER_OR_EQUAL_MESSAGE(1, (int)m.drama,
            "Drama must be >= 1");
        TEST_ASSERT_LESS_OR_EQUAL_MESSAGE(ES_DRAMA_MAX, (int)m.drama,
            "Drama must be <= ES_DRAMA_MAX");
    }
}

/* --- Chronological order (descending Ma, oldest first) --- */

void test_es_moments_descending_ma_order(void) {
    for (int i = 1; i < ES_MOMENT_COUNT; i++) {
        es_moment_t prev = es_moment_get(i - 1);
        es_moment_t curr = es_moment_get(i);
        TEST_ASSERT_TRUE_MESSAGE(prev.ma > curr.ma,
            "Moments must be in descending Ma order (oldest first)");
    }
}

void test_es_first_moment_is_oldest(void) {
    es_moment_t m = es_moment_get(0);
    TEST_ASSERT_FLOAT_WITHIN(1.0, 4540.0, m.ma);
}

void test_es_last_moment_is_youngest(void) {
    es_moment_t m = es_moment_get(ES_MOMENT_COUNT - 1);
    TEST_ASSERT_TRUE(m.ma < 1.0);
}

/* --- Apocalyptic moments (at least 3) --- */

void test_es_at_least_3_apocalyptic_moments(void) {
    int count = 0;
    for (int i = 0; i < ES_MOMENT_COUNT; i++) {
        es_moment_t m = es_moment_get(i);
        if (m.drama == ES_DRAMA_APOCALYPTIC) {
            count++;
        }
    }
    TEST_ASSERT_GREATER_OR_EQUAL(3, count);
}

void test_es_theia_is_apocalyptic(void) {
    es_moment_t m = es_moment_get(1);
    TEST_ASSERT_EQUAL_INT(ES_DRAMA_APOCALYPTIC, m.drama);
}

void test_es_great_dying_is_apocalyptic(void) {
    es_moment_t m = es_moment_get(10);
    TEST_ASSERT_EQUAL_INT(ES_DRAMA_APOCALYPTIC, m.drama);
}

void test_es_asteroid_is_apocalyptic(void) {
    es_moment_t m = es_moment_get(15);
    TEST_ASSERT_EQUAL_INT(ES_DRAMA_APOCALYPTIC, m.drama);
}

/* --- es_moment_nearest --- */

void test_es_moment_nearest_exact_match(void) {
    es_moment_t m = es_moment_nearest(4540.0);
    TEST_ASSERT_EQUAL_STRING("Birth of Earth", m.title);
}

void test_es_moment_nearest_between_two(void) {
    /* 4520 is between Birth (4540) and Theia (4500) — closer to Theia */
    es_moment_t m = es_moment_nearest(4510.0);
    TEST_ASSERT_EQUAL_STRING("Theia Impact", m.title);
}

void test_es_moment_nearest_very_old(void) {
    /* 5000 Ma — closest to Birth of Earth (4540) */
    es_moment_t m = es_moment_nearest(5000.0);
    TEST_ASSERT_EQUAL_STRING("Birth of Earth", m.title);
}

void test_es_moment_nearest_present(void) {
    /* 0.0 Ma — closest to Anthropocene */
    es_moment_t m = es_moment_nearest(0.0);
    TEST_ASSERT_EQUAL_STRING("The Anthropocene", m.title);
}

void test_es_moment_nearest_middle_timeline(void) {
    /* 250 Ma — closest to The Great Dying (252) */
    es_moment_t m = es_moment_nearest(250.0);
    TEST_ASSERT_EQUAL_STRING("The Great Dying", m.title);
}

void test_es_moment_nearest_cambrian(void) {
    /* 540 Ma — closest to Cambrian Explosion (541) */
    es_moment_t m = es_moment_nearest(540.0);
    TEST_ASSERT_EQUAL_STRING("Cambrian Explosion", m.title);
}

/* --- es_moments_by_drama --- */

void test_es_moments_by_drama_apocalyptic(void) {
    int indices[ES_MOMENT_COUNT];
    int count = es_moments_by_drama(ES_DRAMA_APOCALYPTIC, indices, ES_MOMENT_COUNT);
    TEST_ASSERT_GREATER_OR_EQUAL(3, count);
    /* All returned indices must have drama == APOCALYPTIC */
    for (int i = 0; i < count; i++) {
        es_moment_t m = es_moment_get(indices[i]);
        TEST_ASSERT_EQUAL_INT(ES_DRAMA_APOCALYPTIC, m.drama);
    }
}

void test_es_moments_by_drama_catastrophic_or_above(void) {
    int indices[ES_MOMENT_COUNT];
    int count = es_moments_by_drama(ES_DRAMA_CATASTROPHIC, indices, ES_MOMENT_COUNT);
    TEST_ASSERT_GREATER_OR_EQUAL(3, count);
    for (int i = 0; i < count; i++) {
        es_moment_t m = es_moment_get(indices[i]);
        TEST_ASSERT_GREATER_OR_EQUAL(ES_DRAMA_CATASTROPHIC, (int)m.drama);
    }
}

void test_es_moments_by_drama_quiet_returns_all_at_or_above(void) {
    int indices[ES_MOMENT_COUNT];
    int count = es_moments_by_drama(ES_DRAMA_QUIET, indices, ES_MOMENT_COUNT);
    /* All 20 moments have drama >= 1 */
    TEST_ASSERT_EQUAL_INT(ES_MOMENT_COUNT, count);
}

void test_es_moments_by_drama_limited_output(void) {
    int indices[3];
    int count = es_moments_by_drama(ES_DRAMA_QUIET, indices, 3);
    /* Should only write 3 even though there are 20 */
    TEST_ASSERT_EQUAL_INT(3, count);
}

void test_es_moments_by_drama_null_output(void) {
    int count = es_moments_by_drama(ES_DRAMA_QUIET, NULL, 10);
    TEST_ASSERT_EQUAL_INT(0, count);
}

void test_es_moments_by_drama_zero_max(void) {
    int indices[5];
    int count = es_moments_by_drama(ES_DRAMA_QUIET, indices, 0);
    TEST_ASSERT_EQUAL_INT(0, count);
}

/* --- es_moment_title --- */

void test_es_moment_title_valid(void) {
    TEST_ASSERT_EQUAL_STRING("Birth of Earth", es_moment_title(0));
    TEST_ASSERT_EQUAL_STRING("Theia Impact", es_moment_title(1));
    TEST_ASSERT_EQUAL_STRING("The Great Dying", es_moment_title(10));
    TEST_ASSERT_EQUAL_STRING("The Asteroid", es_moment_title(15));
    TEST_ASSERT_EQUAL_STRING("The Anthropocene", es_moment_title(19));
}

void test_es_moment_title_invalid_returns_question_mark(void) {
    TEST_ASSERT_EQUAL_STRING("?", es_moment_title(-1));
    TEST_ASSERT_EQUAL_STRING("?", es_moment_title(20));
    TEST_ASSERT_EQUAL_STRING("?", es_moment_title(999));
}

/* --- es_drama_name --- */

void test_es_drama_name_quiet(void) {
    TEST_ASSERT_EQUAL_STRING("Quiet", es_drama_name(ES_DRAMA_QUIET));
}

void test_es_drama_name_notable(void) {
    TEST_ASSERT_EQUAL_STRING("Notable", es_drama_name(ES_DRAMA_NOTABLE));
}

void test_es_drama_name_dramatic(void) {
    TEST_ASSERT_EQUAL_STRING("Dramatic", es_drama_name(ES_DRAMA_DRAMATIC));
}

void test_es_drama_name_catastrophic(void) {
    TEST_ASSERT_EQUAL_STRING("Catastrophic", es_drama_name(ES_DRAMA_CATASTROPHIC));
}

void test_es_drama_name_apocalyptic(void) {
    TEST_ASSERT_EQUAL_STRING("Apocalyptic", es_drama_name(ES_DRAMA_APOCALYPTIC));
}

void test_es_drama_name_invalid(void) {
    TEST_ASSERT_EQUAL_STRING("?", es_drama_name((es_drama_t)0));
    TEST_ASSERT_EQUAL_STRING("?", es_drama_name((es_drama_t)6));
    TEST_ASSERT_EQUAL_STRING("?", es_drama_name((es_drama_t)99));
}

/* --- es_moment_format --- */

void test_es_moment_format_produces_output(void) {
    char buf[2048];
    int written = es_moment_format(0, buf, (int)sizeof(buf));
    TEST_ASSERT_GREATER_THAN(0, written);
    TEST_ASSERT_GREATER_THAN(10, (int)strlen(buf));
}

void test_es_moment_format_contains_title(void) {
    char buf[2048];
    es_moment_format(0, buf, (int)sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Birth of Earth"));
}

void test_es_moment_format_contains_ma(void) {
    char buf[2048];
    es_moment_format(0, buf, (int)sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "4540"));
}

void test_es_moment_format_contains_drama_name(void) {
    char buf[2048];
    es_moment_format(0, buf, (int)sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Catastrophic"));
}

void test_es_moment_format_invalid_returns_zero(void) {
    char buf[256];
    buf[0] = 'X';
    int written = es_moment_format(-1, buf, (int)sizeof(buf));
    TEST_ASSERT_EQUAL_INT(0, written);
}

void test_es_moment_format_null_buf_returns_zero(void) {
    int written = es_moment_format(0, NULL, 100);
    TEST_ASSERT_EQUAL_INT(0, written);
}

void test_es_moment_format_zero_size_returns_zero(void) {
    char buf[16];
    int written = es_moment_format(0, buf, 0);
    TEST_ASSERT_EQUAL_INT(0, written);
}

void test_es_moment_format_small_buf_truncates(void) {
    char buf[32];
    int written = es_moment_format(0, buf, (int)sizeof(buf));
    TEST_ASSERT_GREATER_THAN(0, written);
    TEST_ASSERT_LESS_THAN((int)sizeof(buf), written);
}

/* --- Specific moment spot checks --- */

void test_es_moment_snowball_earth(void) {
    es_moment_t m = es_moment_get(6);
    TEST_ASSERT_EQUAL_STRING("Snowball Earth", m.title);
    TEST_ASSERT_FLOAT_WITHIN(1.0, 717.0, m.ma);
    TEST_ASSERT_EQUAL_INT(ES_DRAMA_CATASTROPHIC, m.drama);
}

void test_es_moment_cambrian_explosion(void) {
    es_moment_t m = es_moment_get(7);
    TEST_ASSERT_EQUAL_STRING("Cambrian Explosion", m.title);
    TEST_ASSERT_FLOAT_WITHIN(1.0, 541.0, m.ma);
    TEST_ASSERT_EQUAL_INT(ES_DRAMA_DRAMATIC, m.drama);
}

void test_es_moment_great_oxidation(void) {
    es_moment_t m = es_moment_get(5);
    TEST_ASSERT_EQUAL_STRING("Great Oxidation Event", m.title);
    TEST_ASSERT_FLOAT_WITHIN(1.0, 2400.0, m.ma);
    TEST_ASSERT_EQUAL_INT(ES_DRAMA_DRAMATIC, m.drama);
}

void test_es_moment_fire_and_language(void) {
    es_moment_t m = es_moment_get(18);
    TEST_ASSERT_EQUAL_STRING("Fire and Language", m.title);
    TEST_ASSERT_FLOAT_WITHIN(0.05, 0.3, m.ma);
    TEST_ASSERT_EQUAL_INT(ES_DRAMA_DRAMATIC, m.drama);
}

/* --- main --- */

int main(void) {
    UNITY_BEGIN();

    /* Moment count */
    RUN_TEST(test_es_moment_count_is_20);
    RUN_TEST(test_es_moment_count_matches_define);

    /* Moment get by index */
    RUN_TEST(test_es_moment_get_index_0_birth_of_earth);
    RUN_TEST(test_es_moment_get_index_1_theia_impact);
    RUN_TEST(test_es_moment_get_index_10_the_great_dying);
    RUN_TEST(test_es_moment_get_index_15_the_asteroid);
    RUN_TEST(test_es_moment_get_index_19_anthropocene);
    RUN_TEST(test_es_moment_get_invalid_negative);
    RUN_TEST(test_es_moment_get_invalid_too_large);
    RUN_TEST(test_es_moment_get_invalid_999);

    /* Data integrity */
    RUN_TEST(test_es_all_moments_have_non_null_title);
    RUN_TEST(test_es_all_moments_have_non_null_visual);
    RUN_TEST(test_es_all_moments_have_non_null_narrative);
    RUN_TEST(test_es_all_moments_have_non_null_aftermath);
    RUN_TEST(test_es_all_moments_have_non_null_sound_hint);
    RUN_TEST(test_es_all_moments_have_valid_index);
    RUN_TEST(test_es_all_moments_drama_within_range);

    /* Chronological order */
    RUN_TEST(test_es_moments_descending_ma_order);
    RUN_TEST(test_es_first_moment_is_oldest);
    RUN_TEST(test_es_last_moment_is_youngest);

    /* Apocalyptic moments */
    RUN_TEST(test_es_at_least_3_apocalyptic_moments);
    RUN_TEST(test_es_theia_is_apocalyptic);
    RUN_TEST(test_es_great_dying_is_apocalyptic);
    RUN_TEST(test_es_asteroid_is_apocalyptic);

    /* Nearest moment */
    RUN_TEST(test_es_moment_nearest_exact_match);
    RUN_TEST(test_es_moment_nearest_between_two);
    RUN_TEST(test_es_moment_nearest_very_old);
    RUN_TEST(test_es_moment_nearest_present);
    RUN_TEST(test_es_moment_nearest_middle_timeline);
    RUN_TEST(test_es_moment_nearest_cambrian);

    /* Moments by drama */
    RUN_TEST(test_es_moments_by_drama_apocalyptic);
    RUN_TEST(test_es_moments_by_drama_catastrophic_or_above);
    RUN_TEST(test_es_moments_by_drama_quiet_returns_all_at_or_above);
    RUN_TEST(test_es_moments_by_drama_limited_output);
    RUN_TEST(test_es_moments_by_drama_null_output);
    RUN_TEST(test_es_moments_by_drama_zero_max);

    /* Moment title */
    RUN_TEST(test_es_moment_title_valid);
    RUN_TEST(test_es_moment_title_invalid_returns_question_mark);

    /* Drama name */
    RUN_TEST(test_es_drama_name_quiet);
    RUN_TEST(test_es_drama_name_notable);
    RUN_TEST(test_es_drama_name_dramatic);
    RUN_TEST(test_es_drama_name_catastrophic);
    RUN_TEST(test_es_drama_name_apocalyptic);
    RUN_TEST(test_es_drama_name_invalid);

    /* Moment format */
    RUN_TEST(test_es_moment_format_produces_output);
    RUN_TEST(test_es_moment_format_contains_title);
    RUN_TEST(test_es_moment_format_contains_ma);
    RUN_TEST(test_es_moment_format_contains_drama_name);
    RUN_TEST(test_es_moment_format_invalid_returns_zero);
    RUN_TEST(test_es_moment_format_null_buf_returns_zero);
    RUN_TEST(test_es_moment_format_zero_size_returns_zero);
    RUN_TEST(test_es_moment_format_small_buf_truncates);

    /* Spot checks */
    RUN_TEST(test_es_moment_snowball_earth);
    RUN_TEST(test_es_moment_cambrian_explosion);
    RUN_TEST(test_es_moment_great_oxidation);
    RUN_TEST(test_es_moment_fire_and_language);

    return UNITY_END();
}
