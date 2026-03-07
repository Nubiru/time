/* test_cultural_stories.c -- Cultural Story Database tests
 * TDD RED phase: tests written before implementation.
 * Tests story retrieval, culture-based search (case-insensitive substring),
 * system-based filtering, category-based filtering, distinct culture counting,
 * per-culture story counting, and name lookups for enums.
 *
 * Requires >= 30 stories across >= 9 cultures. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/cultural_stories.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== cs_story_count ===== */

void test_story_count_at_least_30(void)
{
    TEST_ASSERT_TRUE(cs_story_count() >= 30);
}

void test_story_count_positive(void)
{
    TEST_ASSERT_TRUE(cs_story_count() > 0);
}

/* ===== cs_story_get ===== */

void test_story_get_first_valid(void)
{
    cs_story_t s = cs_story_get(0);
    TEST_ASSERT_EQUAL_INT(0, s.id);
    TEST_ASSERT_TRUE(s.culture != NULL);
    TEST_ASSERT_TRUE(s.title != NULL);
    TEST_ASSERT_TRUE(s.text != NULL);
}

void test_story_get_last_valid(void)
{
    int last = cs_story_count() - 1;
    cs_story_t s = cs_story_get(last);
    TEST_ASSERT_EQUAL_INT(last, s.id);
    TEST_ASSERT_TRUE(s.culture != NULL);
    TEST_ASSERT_TRUE(s.title != NULL);
    TEST_ASSERT_TRUE(s.text != NULL);
}

void test_story_get_negative_returns_invalid(void)
{
    cs_story_t s = cs_story_get(-1);
    TEST_ASSERT_EQUAL_INT(-1, s.id);
}

void test_story_get_too_large_returns_invalid(void)
{
    cs_story_t s = cs_story_get(9999);
    TEST_ASSERT_EQUAL_INT(-1, s.id);
}

void test_story_get_invalid_has_null_fields(void)
{
    cs_story_t s = cs_story_get(-5);
    TEST_ASSERT_EQUAL_INT(-1, s.id);
    TEST_ASSERT_TRUE(s.culture == NULL);
    TEST_ASSERT_TRUE(s.title == NULL);
    TEST_ASSERT_TRUE(s.text == NULL);
    TEST_ASSERT_TRUE(s.era == NULL);
}

void test_story_get_has_era(void)
{
    cs_story_t s = cs_story_get(0);
    TEST_ASSERT_TRUE(s.era != NULL);
    TEST_ASSERT_TRUE(strlen(s.era) > 0);
}

void test_story_get_has_category(void)
{
    cs_story_t s = cs_story_get(0);
    TEST_ASSERT_TRUE(s.category >= 0);
    TEST_ASSERT_TRUE(s.category < CS_CAT_COUNT);
}

void test_story_get_has_systems(void)
{
    cs_story_t s = cs_story_get(0);
    TEST_ASSERT_TRUE(s.system_count > 0);
    TEST_ASSERT_TRUE(s.system_count <= CS_MAX_TAGS);
}

void test_story_ids_sequential(void)
{
    int count = cs_story_count();
    for (int i = 0; i < count; i++) {
        cs_story_t s = cs_story_get(i);
        TEST_ASSERT_EQUAL_INT(i, s.id);
    }
}

void test_story_all_have_culture(void)
{
    int count = cs_story_count();
    for (int i = 0; i < count; i++) {
        cs_story_t s = cs_story_get(i);
        TEST_ASSERT_TRUE(s.culture != NULL);
        TEST_ASSERT_TRUE(strlen(s.culture) > 0);
    }
}

void test_story_all_have_title(void)
{
    int count = cs_story_count();
    for (int i = 0; i < count; i++) {
        cs_story_t s = cs_story_get(i);
        TEST_ASSERT_TRUE(s.title != NULL);
        TEST_ASSERT_TRUE(strlen(s.title) > 0);
    }
}

void test_story_all_have_text(void)
{
    int count = cs_story_count();
    for (int i = 0; i < count; i++) {
        cs_story_t s = cs_story_get(i);
        TEST_ASSERT_TRUE(s.text != NULL);
        TEST_ASSERT_TRUE(strlen(s.text) > 0);
    }
}

void test_story_all_systems_valid(void)
{
    int count = cs_story_count();
    for (int i = 0; i < count; i++) {
        cs_story_t s = cs_story_get(i);
        for (int j = 0; j < s.system_count; j++) {
            TEST_ASSERT_TRUE(s.related_systems[j] >= 0);
            TEST_ASSERT_TRUE(s.related_systems[j] < CS_SYSTEM_COUNT);
        }
    }
}

void test_story_all_categories_valid(void)
{
    int count = cs_story_count();
    for (int i = 0; i < count; i++) {
        cs_story_t s = cs_story_get(i);
        TEST_ASSERT_TRUE(s.category >= 0);
        TEST_ASSERT_TRUE(s.category < CS_CAT_COUNT);
    }
}

/* ===== cs_by_culture ===== */

void test_by_culture_norse(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_culture("Norse", results, CS_MAX_RESULTS);
    TEST_ASSERT_TRUE(count >= 3);
}

void test_by_culture_greek(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_culture("Greek", results, CS_MAX_RESULTS);
    TEST_ASSERT_TRUE(count >= 3);
}

void test_by_culture_roman(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_culture("Roman", results, CS_MAX_RESULTS);
    TEST_ASSERT_TRUE(count >= 2);
}

void test_by_culture_indian(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_culture("Indian", results, CS_MAX_RESULTS);
    TEST_ASSERT_TRUE(count >= 3);
}

void test_by_culture_chinese(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_culture("Chinese", results, CS_MAX_RESULTS);
    TEST_ASSERT_TRUE(count >= 3);
}

void test_by_culture_buddhist(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_culture("Buddhist", results, CS_MAX_RESULTS);
    TEST_ASSERT_TRUE(count >= 3);
}

void test_by_culture_celtic(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_culture("Celtic", results, CS_MAX_RESULTS);
    TEST_ASSERT_TRUE(count >= 2);
}

void test_by_culture_egyptian(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_culture("Egyptian", results, CS_MAX_RESULTS);
    TEST_ASSERT_TRUE(count >= 3);
}

void test_by_culture_mayan(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_culture("Mayan", results, CS_MAX_RESULTS);
    TEST_ASSERT_TRUE(count >= 3);
}

void test_by_culture_case_insensitive(void)
{
    int results_upper[CS_MAX_RESULTS];
    int results_lower[CS_MAX_RESULTS];
    int count_upper = cs_by_culture("NORSE", results_upper, CS_MAX_RESULTS);
    int count_lower = cs_by_culture("norse", results_lower, CS_MAX_RESULTS);
    TEST_ASSERT_EQUAL_INT(count_upper, count_lower);
    TEST_ASSERT_TRUE(count_upper > 0);
}

void test_by_culture_mixed_case(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_culture("gReEk", results, CS_MAX_RESULTS);
    TEST_ASSERT_TRUE(count >= 3);
}

void test_by_culture_substring(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_culture("nor", results, CS_MAX_RESULTS);
    TEST_ASSERT_TRUE(count >= 3);
}

void test_by_culture_null_returns_zero(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_culture(NULL, results, CS_MAX_RESULTS);
    TEST_ASSERT_EQUAL_INT(0, count);
}

void test_by_culture_empty_returns_all(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_culture("", results, CS_MAX_RESULTS);
    /* Empty substring matches everything, capped by max_results */
    TEST_ASSERT_TRUE(count > 0);
}

void test_by_culture_nonexistent(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_culture("Atlantean", results, CS_MAX_RESULTS);
    TEST_ASSERT_EQUAL_INT(0, count);
}

void test_by_culture_null_results(void)
{
    int count = cs_by_culture("Norse", NULL, CS_MAX_RESULTS);
    TEST_ASSERT_EQUAL_INT(0, count);
}

void test_by_culture_zero_max(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_culture("Norse", results, 0);
    TEST_ASSERT_EQUAL_INT(0, count);
}

void test_by_culture_limited_results(void)
{
    int results[1];
    int count = cs_by_culture("Norse", results, 1);
    TEST_ASSERT_EQUAL_INT(1, count);
}

void test_by_culture_results_are_valid_indices(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_culture("Greek", results, CS_MAX_RESULTS);
    int total = cs_story_count();
    for (int i = 0; i < count; i++) {
        TEST_ASSERT_TRUE(results[i] >= 0);
        TEST_ASSERT_TRUE(results[i] < total);
    }
}

void test_by_culture_hebrew(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_culture("Hebrew", results, CS_MAX_RESULTS);
    TEST_ASSERT_TRUE(count >= 2);
}

void test_by_culture_islamic(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_culture("Islamic", results, CS_MAX_RESULTS);
    TEST_ASSERT_TRUE(count >= 2);
}

void test_by_culture_persian(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_culture("Persian", results, CS_MAX_RESULTS);
    TEST_ASSERT_TRUE(count >= 2);
}

/* ===== cs_by_system ===== */

void test_by_system_gregorian(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_system(CS_SYSTEM_GREGORIAN, results, CS_MAX_RESULTS);
    TEST_ASSERT_TRUE(count >= 2);
}

void test_by_system_astronomy(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_system(CS_SYSTEM_ASTRONOMY, results, CS_MAX_RESULTS);
    TEST_ASSERT_TRUE(count >= 4);
}

void test_by_system_mayan(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_system(CS_SYSTEM_MAYAN, results, CS_MAX_RESULTS);
    TEST_ASSERT_TRUE(count >= 3);
}

void test_by_system_hindu(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_system(CS_SYSTEM_HINDU, results, CS_MAX_RESULTS);
    TEST_ASSERT_TRUE(count >= 2);
}

void test_by_system_buddhist(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_system(CS_SYSTEM_BUDDHIST, results, CS_MAX_RESULTS);
    TEST_ASSERT_TRUE(count >= 2);
}

void test_by_system_chinese(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_system(CS_SYSTEM_CHINESE, results, CS_MAX_RESULTS);
    TEST_ASSERT_TRUE(count >= 2);
}

void test_by_system_iching(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_system(CS_SYSTEM_ICHING, results, CS_MAX_RESULTS);
    TEST_ASSERT_TRUE(count >= 1);
}

void test_by_system_astrology(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_system(CS_SYSTEM_ASTROLOGY, results, CS_MAX_RESULTS);
    TEST_ASSERT_TRUE(count >= 2);
}

void test_by_system_celtic(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_system(CS_SYSTEM_CELTIC, results, CS_MAX_RESULTS);
    TEST_ASSERT_TRUE(count >= 2);
}

void test_by_system_egyptian(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_system(CS_SYSTEM_EGYPTIAN, results, CS_MAX_RESULTS);
    TEST_ASSERT_TRUE(count >= 2);
}

void test_by_system_null_results(void)
{
    int count = cs_by_system(CS_SYSTEM_ASTRONOMY, NULL, CS_MAX_RESULTS);
    TEST_ASSERT_EQUAL_INT(0, count);
}

void test_by_system_zero_max(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_system(CS_SYSTEM_ASTRONOMY, results, 0);
    TEST_ASSERT_EQUAL_INT(0, count);
}

void test_by_system_limited(void)
{
    int results[2];
    int count = cs_by_system(CS_SYSTEM_ASTRONOMY, results, 2);
    TEST_ASSERT_TRUE(count <= 2);
    TEST_ASSERT_TRUE(count > 0);
}

void test_by_system_results_valid_indices(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_system(CS_SYSTEM_GREGORIAN, results, CS_MAX_RESULTS);
    int total = cs_story_count();
    for (int i = 0; i < count; i++) {
        TEST_ASSERT_TRUE(results[i] >= 0);
        TEST_ASSERT_TRUE(results[i] < total);
    }
}

void test_by_system_invalid_negative(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_system((cs_system_t)-1, results, CS_MAX_RESULTS);
    TEST_ASSERT_EQUAL_INT(0, count);
}

void test_by_system_invalid_too_large(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_system(CS_SYSTEM_COUNT, results, CS_MAX_RESULTS);
    TEST_ASSERT_EQUAL_INT(0, count);
}

/* ===== cs_by_category ===== */

void test_by_category_origin_myth(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_category(CS_CAT_ORIGIN_MYTH, results, CS_MAX_RESULTS);
    TEST_ASSERT_TRUE(count >= 3);
}

void test_by_category_time_philosophy(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_category(CS_CAT_TIME_PHILOSOPHY, results, CS_MAX_RESULTS);
    TEST_ASSERT_TRUE(count >= 3);
}

void test_by_category_key_figure(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_category(CS_CAT_KEY_FIGURE, results, CS_MAX_RESULTS);
    TEST_ASSERT_TRUE(count >= 3);
}

void test_by_category_golden_age(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_category(CS_CAT_GOLDEN_AGE, results, CS_MAX_RESULTS);
    TEST_ASSERT_TRUE(count >= 2);
}

void test_by_category_calendar_reform(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_category(CS_CAT_CALENDAR_REFORM, results, CS_MAX_RESULTS);
    TEST_ASSERT_TRUE(count >= 2);
}

void test_by_category_cosmic_cycle(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_category(CS_CAT_COSMIC_CYCLE, results, CS_MAX_RESULTS);
    TEST_ASSERT_TRUE(count >= 4);
}

void test_by_category_null_results(void)
{
    int count = cs_by_category(CS_CAT_ORIGIN_MYTH, NULL, CS_MAX_RESULTS);
    TEST_ASSERT_EQUAL_INT(0, count);
}

void test_by_category_zero_max(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_category(CS_CAT_ORIGIN_MYTH, results, 0);
    TEST_ASSERT_EQUAL_INT(0, count);
}

void test_by_category_limited(void)
{
    int results[1];
    int count = cs_by_category(CS_CAT_COSMIC_CYCLE, results, 1);
    TEST_ASSERT_EQUAL_INT(1, count);
}

void test_by_category_invalid_negative(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_category((cs_category_t)-1, results, CS_MAX_RESULTS);
    TEST_ASSERT_EQUAL_INT(0, count);
}

void test_by_category_invalid_too_large(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_category(CS_CAT_COUNT, results, CS_MAX_RESULTS);
    TEST_ASSERT_EQUAL_INT(0, count);
}

void test_by_category_results_valid_indices(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_category(CS_CAT_KEY_FIGURE, results, CS_MAX_RESULTS);
    int total = cs_story_count();
    for (int i = 0; i < count; i++) {
        TEST_ASSERT_TRUE(results[i] >= 0);
        TEST_ASSERT_TRUE(results[i] < total);
    }
}

void test_by_category_all_categories_covered(void)
{
    /* Every category must have at least one story */
    for (int cat = 0; cat < CS_CAT_COUNT; cat++) {
        int results[CS_MAX_RESULTS];
        int count = cs_by_category((cs_category_t)cat, results, CS_MAX_RESULTS);
        TEST_ASSERT_TRUE(count >= 1);
    }
}

/* ===== cs_category_name ===== */

void test_category_name_origin_myth(void)
{
    TEST_ASSERT_EQUAL_STRING("Origin Myth", cs_category_name(CS_CAT_ORIGIN_MYTH));
}

void test_category_name_time_philosophy(void)
{
    TEST_ASSERT_EQUAL_STRING("Time Philosophy", cs_category_name(CS_CAT_TIME_PHILOSOPHY));
}

void test_category_name_key_figure(void)
{
    TEST_ASSERT_EQUAL_STRING("Key Figure", cs_category_name(CS_CAT_KEY_FIGURE));
}

void test_category_name_golden_age(void)
{
    TEST_ASSERT_EQUAL_STRING("Golden Age", cs_category_name(CS_CAT_GOLDEN_AGE));
}

void test_category_name_calendar_reform(void)
{
    TEST_ASSERT_EQUAL_STRING("Calendar Reform", cs_category_name(CS_CAT_CALENDAR_REFORM));
}

void test_category_name_cosmic_cycle(void)
{
    TEST_ASSERT_EQUAL_STRING("Cosmic Cycle", cs_category_name(CS_CAT_COSMIC_CYCLE));
}

void test_category_name_invalid_negative(void)
{
    TEST_ASSERT_EQUAL_STRING("Unknown", cs_category_name((cs_category_t)-1));
}

void test_category_name_invalid_too_large(void)
{
    TEST_ASSERT_EQUAL_STRING("Unknown", cs_category_name(CS_CAT_COUNT));
}

/* ===== cs_system_name ===== */

void test_system_name_gregorian(void)
{
    TEST_ASSERT_EQUAL_STRING("Gregorian", cs_system_name(CS_SYSTEM_GREGORIAN));
}

void test_system_name_hebrew(void)
{
    TEST_ASSERT_EQUAL_STRING("Hebrew", cs_system_name(CS_SYSTEM_HEBREW));
}

void test_system_name_islamic(void)
{
    TEST_ASSERT_EQUAL_STRING("Islamic", cs_system_name(CS_SYSTEM_ISLAMIC));
}

void test_system_name_chinese(void)
{
    TEST_ASSERT_EQUAL_STRING("Chinese", cs_system_name(CS_SYSTEM_CHINESE));
}

void test_system_name_hindu(void)
{
    TEST_ASSERT_EQUAL_STRING("Hindu", cs_system_name(CS_SYSTEM_HINDU));
}

void test_system_name_buddhist(void)
{
    TEST_ASSERT_EQUAL_STRING("Buddhist", cs_system_name(CS_SYSTEM_BUDDHIST));
}

void test_system_name_mayan(void)
{
    TEST_ASSERT_EQUAL_STRING("Mayan", cs_system_name(CS_SYSTEM_MAYAN));
}

void test_system_name_coptic(void)
{
    TEST_ASSERT_EQUAL_STRING("Coptic", cs_system_name(CS_SYSTEM_COPTIC));
}

void test_system_name_ethiopian(void)
{
    TEST_ASSERT_EQUAL_STRING("Ethiopian", cs_system_name(CS_SYSTEM_ETHIOPIAN));
}

void test_system_name_zoroastrian(void)
{
    TEST_ASSERT_EQUAL_STRING("Zoroastrian", cs_system_name(CS_SYSTEM_ZOROASTRIAN));
}

void test_system_name_celtic(void)
{
    TEST_ASSERT_EQUAL_STRING("Celtic", cs_system_name(CS_SYSTEM_CELTIC));
}

void test_system_name_myanmar(void)
{
    TEST_ASSERT_EQUAL_STRING("Myanmar", cs_system_name(CS_SYSTEM_MYANMAR));
}

void test_system_name_egyptian(void)
{
    TEST_ASSERT_EQUAL_STRING("Egyptian", cs_system_name(CS_SYSTEM_EGYPTIAN));
}

void test_system_name_astrology(void)
{
    TEST_ASSERT_EQUAL_STRING("Astrology", cs_system_name(CS_SYSTEM_ASTROLOGY));
}

void test_system_name_iching(void)
{
    TEST_ASSERT_EQUAL_STRING("I Ching", cs_system_name(CS_SYSTEM_ICHING));
}

void test_system_name_human_design(void)
{
    TEST_ASSERT_EQUAL_STRING("Human Design", cs_system_name(CS_SYSTEM_HUMAN_DESIGN));
}

void test_system_name_kabbalah(void)
{
    TEST_ASSERT_EQUAL_STRING("Kabbalah", cs_system_name(CS_SYSTEM_KABBALAH));
}

void test_system_name_geology(void)
{
    TEST_ASSERT_EQUAL_STRING("Geology", cs_system_name(CS_SYSTEM_GEOLOGY));
}

void test_system_name_astronomy(void)
{
    TEST_ASSERT_EQUAL_STRING("Astronomy", cs_system_name(CS_SYSTEM_ASTRONOMY));
}

void test_system_name_invalid_negative(void)
{
    TEST_ASSERT_EQUAL_STRING("Unknown", cs_system_name((cs_system_t)-1));
}

void test_system_name_invalid_too_large(void)
{
    TEST_ASSERT_EQUAL_STRING("Unknown", cs_system_name(CS_SYSTEM_COUNT));
}

/* ===== cs_distinct_cultures ===== */

void test_distinct_cultures_at_least_9(void)
{
    TEST_ASSERT_TRUE(cs_distinct_cultures() >= 9);
}

void test_distinct_cultures_positive(void)
{
    TEST_ASSERT_TRUE(cs_distinct_cultures() > 0);
}

void test_distinct_cultures_not_exceed_story_count(void)
{
    TEST_ASSERT_TRUE(cs_distinct_cultures() <= cs_story_count());
}

/* ===== cs_culture_story_count ===== */

void test_culture_story_count_norse(void)
{
    int count = cs_culture_story_count("Norse");
    TEST_ASSERT_TRUE(count >= 3);
}

void test_culture_story_count_greek(void)
{
    int count = cs_culture_story_count("Greek");
    TEST_ASSERT_TRUE(count >= 3);
}

void test_culture_story_count_case_insensitive(void)
{
    int upper = cs_culture_story_count("NORSE");
    int lower = cs_culture_story_count("norse");
    TEST_ASSERT_EQUAL_INT(upper, lower);
}

void test_culture_story_count_null(void)
{
    int count = cs_culture_story_count(NULL);
    TEST_ASSERT_EQUAL_INT(0, count);
}

void test_culture_story_count_nonexistent(void)
{
    int count = cs_culture_story_count("Atlantean");
    TEST_ASSERT_EQUAL_INT(0, count);
}

void test_culture_story_count_matches_by_culture(void)
{
    /* cs_culture_story_count and cs_by_culture should agree */
    int results[CS_MAX_RESULTS];
    int by_count = cs_by_culture("Egyptian", results, CS_MAX_RESULTS);
    int c_count = cs_culture_story_count("Egyptian");
    TEST_ASSERT_EQUAL_INT(by_count, c_count);
}

/* ===== Content validation: specific stories ===== */

void test_norse_yggdrasil_exists(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_culture("Norse", results, CS_MAX_RESULTS);
    int found = 0;
    for (int i = 0; i < count; i++) {
        cs_story_t s = cs_story_get(results[i]);
        if (strstr(s.title, "Yggdrasil") != NULL) {
            found = 1;
            TEST_ASSERT_EQUAL_INT(CS_CAT_ORIGIN_MYTH, s.category);
        }
    }
    TEST_ASSERT_TRUE(found);
}

void test_norse_ragnarok_exists(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_culture("Norse", results, CS_MAX_RESULTS);
    int found = 0;
    for (int i = 0; i < count; i++) {
        cs_story_t s = cs_story_get(results[i]);
        if (strstr(s.title, "Ragnarok") != NULL ||
            strstr(s.title, "Ragnar") != NULL) {
            found = 1;
            TEST_ASSERT_EQUAL_INT(CS_CAT_COSMIC_CYCLE, s.category);
        }
    }
    TEST_ASSERT_TRUE(found);
}

void test_greek_chronos_kairos_exists(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_culture("Greek", results, CS_MAX_RESULTS);
    int found = 0;
    for (int i = 0; i < count; i++) {
        cs_story_t s = cs_story_get(results[i]);
        if (strstr(s.title, "Chronos") != NULL ||
            strstr(s.title, "Kairos") != NULL) {
            found = 1;
            TEST_ASSERT_EQUAL_INT(CS_CAT_TIME_PHILOSOPHY, s.category);
        }
    }
    TEST_ASSERT_TRUE(found);
}

void test_roman_caesar_reform_exists(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_culture("Roman", results, CS_MAX_RESULTS);
    int found = 0;
    for (int i = 0; i < count; i++) {
        cs_story_t s = cs_story_get(results[i]);
        if (strstr(s.title, "Caesar") != NULL ||
            strstr(s.title, "Julian") != NULL) {
            found = 1;
            TEST_ASSERT_EQUAL_INT(CS_CAT_CALENDAR_REFORM, s.category);
        }
    }
    TEST_ASSERT_TRUE(found);
}

void test_indian_shiva_exists(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_culture("Indian", results, CS_MAX_RESULTS);
    int found = 0;
    for (int i = 0; i < count; i++) {
        cs_story_t s = cs_story_get(results[i]);
        if (strstr(s.title, "Shiva") != NULL ||
            strstr(s.title, "Nataraja") != NULL) {
            found = 1;
        }
    }
    TEST_ASSERT_TRUE(found);
}

void test_mayan_popol_vuh_exists(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_culture("Mayan", results, CS_MAX_RESULTS);
    int found = 0;
    for (int i = 0; i < count; i++) {
        cs_story_t s = cs_story_get(results[i]);
        if (strstr(s.title, "Popol") != NULL ||
            strstr(s.title, "Hero Twins") != NULL) {
            found = 1;
        }
    }
    TEST_ASSERT_TRUE(found);
}

void test_egyptian_maat_exists(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_culture("Egyptian", results, CS_MAX_RESULTS);
    int found = 0;
    for (int i = 0; i < count; i++) {
        cs_story_t s = cs_story_get(results[i]);
        if (strstr(s.title, "Ma'at") != NULL ||
            strstr(s.title, "Maat") != NULL) {
            found = 1;
        }
    }
    TEST_ASSERT_TRUE(found);
}

void test_celtic_newgrange_exists(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_culture("Celtic", results, CS_MAX_RESULTS);
    int found = 0;
    for (int i = 0; i < count; i++) {
        cs_story_t s = cs_story_get(results[i]);
        if (strstr(s.title, "Newgrange") != NULL) {
            found = 1;
        }
    }
    TEST_ASSERT_TRUE(found);
}

void test_buddhist_anicca_exists(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_culture("Buddhist", results, CS_MAX_RESULTS);
    int found = 0;
    for (int i = 0; i < count; i++) {
        cs_story_t s = cs_story_get(results[i]);
        if (strstr(s.title, "Anicca") != NULL ||
            strstr(s.title, "Impermanence") != NULL) {
            found = 1;
        }
    }
    TEST_ASSERT_TRUE(found);
}

/* ===== Cross-referencing: system tagging validation ===== */

void test_norse_stories_have_gregorian_or_astronomy(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_culture("Norse", results, CS_MAX_RESULTS);
    for (int i = 0; i < count; i++) {
        cs_story_t s = cs_story_get(results[i]);
        int has_match = 0;
        for (int j = 0; j < s.system_count; j++) {
            if (s.related_systems[j] == CS_SYSTEM_GREGORIAN ||
                s.related_systems[j] == CS_SYSTEM_ASTRONOMY) {
                has_match = 1;
            }
        }
        TEST_ASSERT_TRUE(has_match);
    }
}

void test_mayan_stories_have_mayan_system(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_culture("Mayan", results, CS_MAX_RESULTS);
    for (int i = 0; i < count; i++) {
        cs_story_t s = cs_story_get(results[i]);
        int has_mayan = 0;
        for (int j = 0; j < s.system_count; j++) {
            if (s.related_systems[j] == CS_SYSTEM_MAYAN) {
                has_mayan = 1;
            }
        }
        TEST_ASSERT_TRUE(has_mayan);
    }
}

void test_buddhist_stories_have_buddhist_system(void)
{
    int results[CS_MAX_RESULTS];
    int count = cs_by_culture("Buddhist", results, CS_MAX_RESULTS);
    for (int i = 0; i < count; i++) {
        cs_story_t s = cs_story_get(results[i]);
        int has_buddhist = 0;
        for (int j = 0; j < s.system_count; j++) {
            if (s.related_systems[j] == CS_SYSTEM_BUDDHIST) {
                has_buddhist = 1;
            }
        }
        TEST_ASSERT_TRUE(has_buddhist);
    }
}

/* ===== main ===== */

int main(void)
{
    UNITY_BEGIN();

    /* cs_story_count */
    RUN_TEST(test_story_count_at_least_30);
    RUN_TEST(test_story_count_positive);

    /* cs_story_get */
    RUN_TEST(test_story_get_first_valid);
    RUN_TEST(test_story_get_last_valid);
    RUN_TEST(test_story_get_negative_returns_invalid);
    RUN_TEST(test_story_get_too_large_returns_invalid);
    RUN_TEST(test_story_get_invalid_has_null_fields);
    RUN_TEST(test_story_get_has_era);
    RUN_TEST(test_story_get_has_category);
    RUN_TEST(test_story_get_has_systems);
    RUN_TEST(test_story_ids_sequential);
    RUN_TEST(test_story_all_have_culture);
    RUN_TEST(test_story_all_have_title);
    RUN_TEST(test_story_all_have_text);
    RUN_TEST(test_story_all_systems_valid);
    RUN_TEST(test_story_all_categories_valid);

    /* cs_by_culture */
    RUN_TEST(test_by_culture_norse);
    RUN_TEST(test_by_culture_greek);
    RUN_TEST(test_by_culture_roman);
    RUN_TEST(test_by_culture_indian);
    RUN_TEST(test_by_culture_chinese);
    RUN_TEST(test_by_culture_buddhist);
    RUN_TEST(test_by_culture_celtic);
    RUN_TEST(test_by_culture_egyptian);
    RUN_TEST(test_by_culture_mayan);
    RUN_TEST(test_by_culture_case_insensitive);
    RUN_TEST(test_by_culture_mixed_case);
    RUN_TEST(test_by_culture_substring);
    RUN_TEST(test_by_culture_null_returns_zero);
    RUN_TEST(test_by_culture_empty_returns_all);
    RUN_TEST(test_by_culture_nonexistent);
    RUN_TEST(test_by_culture_null_results);
    RUN_TEST(test_by_culture_zero_max);
    RUN_TEST(test_by_culture_limited_results);
    RUN_TEST(test_by_culture_results_are_valid_indices);
    RUN_TEST(test_by_culture_hebrew);
    RUN_TEST(test_by_culture_islamic);
    RUN_TEST(test_by_culture_persian);

    /* cs_by_system */
    RUN_TEST(test_by_system_gregorian);
    RUN_TEST(test_by_system_astronomy);
    RUN_TEST(test_by_system_mayan);
    RUN_TEST(test_by_system_hindu);
    RUN_TEST(test_by_system_buddhist);
    RUN_TEST(test_by_system_chinese);
    RUN_TEST(test_by_system_iching);
    RUN_TEST(test_by_system_astrology);
    RUN_TEST(test_by_system_celtic);
    RUN_TEST(test_by_system_egyptian);
    RUN_TEST(test_by_system_null_results);
    RUN_TEST(test_by_system_zero_max);
    RUN_TEST(test_by_system_limited);
    RUN_TEST(test_by_system_results_valid_indices);
    RUN_TEST(test_by_system_invalid_negative);
    RUN_TEST(test_by_system_invalid_too_large);

    /* cs_by_category */
    RUN_TEST(test_by_category_origin_myth);
    RUN_TEST(test_by_category_time_philosophy);
    RUN_TEST(test_by_category_key_figure);
    RUN_TEST(test_by_category_golden_age);
    RUN_TEST(test_by_category_calendar_reform);
    RUN_TEST(test_by_category_cosmic_cycle);
    RUN_TEST(test_by_category_null_results);
    RUN_TEST(test_by_category_zero_max);
    RUN_TEST(test_by_category_limited);
    RUN_TEST(test_by_category_invalid_negative);
    RUN_TEST(test_by_category_invalid_too_large);
    RUN_TEST(test_by_category_results_valid_indices);
    RUN_TEST(test_by_category_all_categories_covered);

    /* cs_category_name */
    RUN_TEST(test_category_name_origin_myth);
    RUN_TEST(test_category_name_time_philosophy);
    RUN_TEST(test_category_name_key_figure);
    RUN_TEST(test_category_name_golden_age);
    RUN_TEST(test_category_name_calendar_reform);
    RUN_TEST(test_category_name_cosmic_cycle);
    RUN_TEST(test_category_name_invalid_negative);
    RUN_TEST(test_category_name_invalid_too_large);

    /* cs_system_name */
    RUN_TEST(test_system_name_gregorian);
    RUN_TEST(test_system_name_hebrew);
    RUN_TEST(test_system_name_islamic);
    RUN_TEST(test_system_name_chinese);
    RUN_TEST(test_system_name_hindu);
    RUN_TEST(test_system_name_buddhist);
    RUN_TEST(test_system_name_mayan);
    RUN_TEST(test_system_name_coptic);
    RUN_TEST(test_system_name_ethiopian);
    RUN_TEST(test_system_name_zoroastrian);
    RUN_TEST(test_system_name_celtic);
    RUN_TEST(test_system_name_myanmar);
    RUN_TEST(test_system_name_egyptian);
    RUN_TEST(test_system_name_astrology);
    RUN_TEST(test_system_name_iching);
    RUN_TEST(test_system_name_human_design);
    RUN_TEST(test_system_name_kabbalah);
    RUN_TEST(test_system_name_geology);
    RUN_TEST(test_system_name_astronomy);
    RUN_TEST(test_system_name_invalid_negative);
    RUN_TEST(test_system_name_invalid_too_large);

    /* cs_distinct_cultures */
    RUN_TEST(test_distinct_cultures_at_least_9);
    RUN_TEST(test_distinct_cultures_positive);
    RUN_TEST(test_distinct_cultures_not_exceed_story_count);

    /* cs_culture_story_count */
    RUN_TEST(test_culture_story_count_norse);
    RUN_TEST(test_culture_story_count_greek);
    RUN_TEST(test_culture_story_count_case_insensitive);
    RUN_TEST(test_culture_story_count_null);
    RUN_TEST(test_culture_story_count_nonexistent);
    RUN_TEST(test_culture_story_count_matches_by_culture);

    /* Content validation */
    RUN_TEST(test_norse_yggdrasil_exists);
    RUN_TEST(test_norse_ragnarok_exists);
    RUN_TEST(test_greek_chronos_kairos_exists);
    RUN_TEST(test_roman_caesar_reform_exists);
    RUN_TEST(test_indian_shiva_exists);
    RUN_TEST(test_mayan_popol_vuh_exists);
    RUN_TEST(test_egyptian_maat_exists);
    RUN_TEST(test_celtic_newgrange_exists);
    RUN_TEST(test_buddhist_anicca_exists);

    /* Cross-referencing */
    RUN_TEST(test_norse_stories_have_gregorian_or_astronomy);
    RUN_TEST(test_mayan_stories_have_mayan_system);
    RUN_TEST(test_buddhist_stories_have_buddhist_system);

    return UNITY_END();
}
