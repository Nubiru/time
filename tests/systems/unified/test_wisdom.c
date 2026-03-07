/* test_wisdom.c -- Wisdom Quote Database tests
 * Tests quote access, tag filtering, culture filtering, author search,
 * distinct culture counting, and edge cases. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/wisdom.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== wisdom_quote_count ===== */

void test_quote_count_at_least_120(void)
{
    TEST_ASSERT_TRUE(wisdom_quote_count() >= 120);
}

void test_quote_count_positive(void)
{
    TEST_ASSERT_TRUE(wisdom_quote_count() > 0);
}

/* ===== wisdom_quote_get ===== */

void test_get_first_quote_valid(void)
{
    wisdom_t q = wisdom_quote_get(0);
    TEST_ASSERT_EQUAL_INT(0, q.id);
    TEST_ASSERT_TRUE(q.text != NULL);
    TEST_ASSERT_TRUE(q.author != NULL);
    TEST_ASSERT_TRUE(q.culture != NULL);
    TEST_ASSERT_TRUE(q.tag_count > 0);
    TEST_ASSERT_TRUE(q.tag_count <= WISDOM_MAX_TAGS);
}

void test_get_last_quote_valid(void)
{
    int count = wisdom_quote_count();
    wisdom_t q = wisdom_quote_get(count - 1);
    TEST_ASSERT_EQUAL_INT(count - 1, q.id);
    TEST_ASSERT_TRUE(q.text != NULL);
    TEST_ASSERT_TRUE(q.author != NULL);
}

void test_get_negative_index_returns_invalid(void)
{
    wisdom_t q = wisdom_quote_get(-1);
    TEST_ASSERT_EQUAL_INT(-1, q.id);
}

void test_get_out_of_bounds_returns_invalid(void)
{
    wisdom_t q = wisdom_quote_get(wisdom_quote_count());
    TEST_ASSERT_EQUAL_INT(-1, q.id);
}

void test_get_large_index_returns_invalid(void)
{
    wisdom_t q = wisdom_quote_get(99999);
    TEST_ASSERT_EQUAL_INT(-1, q.id);
}

void test_each_quote_has_nonempty_text(void)
{
    int count = wisdom_quote_count();
    for (int i = 0; i < count; i++) {
        wisdom_t q = wisdom_quote_get(i);
        TEST_ASSERT_TRUE(strlen(q.text) > 0);
    }
}

void test_each_quote_has_nonempty_author(void)
{
    int count = wisdom_quote_count();
    for (int i = 0; i < count; i++) {
        wisdom_t q = wisdom_quote_get(i);
        TEST_ASSERT_TRUE(strlen(q.author) > 0);
    }
}

void test_each_quote_has_nonempty_culture(void)
{
    int count = wisdom_quote_count();
    for (int i = 0; i < count; i++) {
        wisdom_t q = wisdom_quote_get(i);
        TEST_ASSERT_TRUE(strlen(q.culture) > 0);
    }
}

void test_each_quote_id_matches_index(void)
{
    int count = wisdom_quote_count();
    for (int i = 0; i < count; i++) {
        wisdom_t q = wisdom_quote_get(i);
        TEST_ASSERT_EQUAL_INT(i, q.id);
    }
}

void test_each_quote_tag_count_in_range(void)
{
    int count = wisdom_quote_count();
    for (int i = 0; i < count; i++) {
        wisdom_t q = wisdom_quote_get(i);
        TEST_ASSERT_TRUE(q.tag_count >= 1);
        TEST_ASSERT_TRUE(q.tag_count <= WISDOM_MAX_TAGS);
    }
}

void test_each_quote_tags_valid(void)
{
    int count = wisdom_quote_count();
    for (int i = 0; i < count; i++) {
        wisdom_t q = wisdom_quote_get(i);
        for (int j = 0; j < q.tag_count; j++) {
            TEST_ASSERT_TRUE(q.tags[j] >= 0);
            TEST_ASSERT_TRUE(q.tags[j] < WISDOM_TAG_COUNT);
        }
    }
}

/* ===== wisdom_by_tag ===== */

void test_by_tag_astronomy_finds_results(void)
{
    int results[WISDOM_MAX_RESULTS];
    int found = wisdom_by_tag(WISDOM_TAG_ASTRONOMY, results, WISDOM_MAX_RESULTS);
    TEST_ASSERT_TRUE(found > 0);
}

void test_by_tag_time_finds_results(void)
{
    int results[WISDOM_MAX_RESULTS];
    int found = wisdom_by_tag(WISDOM_TAG_TIME, results, WISDOM_MAX_RESULTS);
    TEST_ASSERT_TRUE(found > 0);
}

void test_by_tag_results_have_correct_tag(void)
{
    int results[WISDOM_MAX_RESULTS];
    int found = wisdom_by_tag(WISDOM_TAG_COSMOS, results, WISDOM_MAX_RESULTS);
    for (int i = 0; i < found; i++) {
        wisdom_t q = wisdom_quote_get(results[i]);
        int has_tag = 0;
        for (int j = 0; j < q.tag_count; j++) {
            if (q.tags[j] == WISDOM_TAG_COSMOS) {
                has_tag = 1;
                break;
            }
        }
        TEST_ASSERT_EQUAL_INT(1, has_tag);
    }
}

void test_by_tag_respects_max_results(void)
{
    int results[2];
    int found = wisdom_by_tag(WISDOM_TAG_TIME, results, 2);
    TEST_ASSERT_TRUE(found <= 2);
}

void test_by_tag_invalid_returns_zero(void)
{
    int results[WISDOM_MAX_RESULTS];
    int found = wisdom_by_tag(WISDOM_TAG_COUNT, results, WISDOM_MAX_RESULTS);
    TEST_ASSERT_EQUAL_INT(0, found);
}

void test_by_tag_null_results_returns_zero(void)
{
    int found = wisdom_by_tag(WISDOM_TAG_TIME, NULL, WISDOM_MAX_RESULTS);
    TEST_ASSERT_EQUAL_INT(0, found);
}

void test_by_tag_zero_max_returns_zero(void)
{
    int results[WISDOM_MAX_RESULTS];
    int found = wisdom_by_tag(WISDOM_TAG_TIME, results, 0);
    TEST_ASSERT_EQUAL_INT(0, found);
}

void test_by_tag_all_tags_have_quotes(void)
{
    /* Every tag enum should have at least one quote */
    for (int t = 0; t < WISDOM_TAG_COUNT; t++) {
        int results[WISDOM_MAX_RESULTS];
        int found = wisdom_by_tag((wisdom_tag_t)t, results, WISDOM_MAX_RESULTS);
        TEST_ASSERT_TRUE(found > 0);
    }
}

/* ===== wisdom_by_culture ===== */

void test_by_culture_greek_finds_results(void)
{
    int results[WISDOM_MAX_RESULTS];
    int found = wisdom_by_culture("Greek", results, WISDOM_MAX_RESULTS);
    TEST_ASSERT_TRUE(found >= 10);
}

void test_by_culture_case_insensitive(void)
{
    int results1[WISDOM_MAX_RESULTS];
    int results2[WISDOM_MAX_RESULTS];
    int found1 = wisdom_by_culture("greek", results1, WISDOM_MAX_RESULTS);
    int found2 = wisdom_by_culture("GREEK", results2, WISDOM_MAX_RESULTS);
    TEST_ASSERT_EQUAL_INT(found1, found2);
}

void test_by_culture_substring_match(void)
{
    int results[WISDOM_MAX_RESULTS];
    int found = wisdom_by_culture("Islam", results, WISDOM_MAX_RESULTS);
    TEST_ASSERT_TRUE(found > 0);
}

void test_by_culture_nonexistent_returns_zero(void)
{
    int results[WISDOM_MAX_RESULTS];
    int found = wisdom_by_culture("Atlantean", results, WISDOM_MAX_RESULTS);
    TEST_ASSERT_EQUAL_INT(0, found);
}

void test_by_culture_null_returns_zero(void)
{
    int results[WISDOM_MAX_RESULTS];
    int found = wisdom_by_culture(NULL, results, WISDOM_MAX_RESULTS);
    TEST_ASSERT_EQUAL_INT(0, found);
}

void test_by_culture_null_results_returns_zero(void)
{
    int found = wisdom_by_culture("Greek", NULL, WISDOM_MAX_RESULTS);
    TEST_ASSERT_EQUAL_INT(0, found);
}

void test_by_culture_respects_max_results(void)
{
    int results[3];
    int found = wisdom_by_culture("Greek", results, 3);
    TEST_ASSERT_TRUE(found <= 3);
}

/* ===== wisdom_by_author ===== */

void test_by_author_plato(void)
{
    int idx = wisdom_by_author("Plato");
    TEST_ASSERT_TRUE(idx >= 0);
    wisdom_t q = wisdom_quote_get(idx);
    TEST_ASSERT_EQUAL_STRING("Plato", q.author);
}

void test_by_author_case_insensitive(void)
{
    int idx = wisdom_by_author("plato");
    TEST_ASSERT_TRUE(idx >= 0);
}

void test_by_author_substring_match(void)
{
    int idx = wisdom_by_author("Einstein");
    TEST_ASSERT_TRUE(idx >= 0);
}

void test_by_author_not_found(void)
{
    int idx = wisdom_by_author("Nobody McFakerson");
    TEST_ASSERT_EQUAL_INT(-1, idx);
}

void test_by_author_null_returns_negative(void)
{
    int idx = wisdom_by_author(NULL);
    TEST_ASSERT_EQUAL_INT(-1, idx);
}

/* ===== wisdom_tag_name ===== */

void test_tag_name_astronomy(void)
{
    TEST_ASSERT_EQUAL_STRING("Astronomy", wisdom_tag_name(WISDOM_TAG_ASTRONOMY));
}

void test_tag_name_time(void)
{
    TEST_ASSERT_EQUAL_STRING("Time", wisdom_tag_name(WISDOM_TAG_TIME));
}

void test_tag_name_cycles(void)
{
    TEST_ASSERT_EQUAL_STRING("Cycles", wisdom_tag_name(WISDOM_TAG_CYCLES));
}

void test_tag_name_unity(void)
{
    TEST_ASSERT_EQUAL_STRING("Unity", wisdom_tag_name(WISDOM_TAG_UNITY));
}

void test_tag_name_mathematics(void)
{
    TEST_ASSERT_EQUAL_STRING("Mathematics", wisdom_tag_name(WISDOM_TAG_MATHEMATICS));
}

void test_tag_name_nature(void)
{
    TEST_ASSERT_EQUAL_STRING("Nature", wisdom_tag_name(WISDOM_TAG_NATURE));
}

void test_tag_name_consciousness(void)
{
    TEST_ASSERT_EQUAL_STRING("Consciousness", wisdom_tag_name(WISDOM_TAG_CONSCIOUSNESS));
}

void test_tag_name_cosmos(void)
{
    TEST_ASSERT_EQUAL_STRING("Cosmos", wisdom_tag_name(WISDOM_TAG_COSMOS));
}

void test_tag_name_harmony(void)
{
    TEST_ASSERT_EQUAL_STRING("Harmony", wisdom_tag_name(WISDOM_TAG_HARMONY));
}

void test_tag_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("Unknown", wisdom_tag_name(WISDOM_TAG_COUNT));
}

void test_tag_name_negative(void)
{
    TEST_ASSERT_EQUAL_STRING("Unknown", wisdom_tag_name((wisdom_tag_t)-1));
}

/* ===== wisdom_culture_count ===== */

void test_culture_count_greek(void)
{
    int count = wisdom_culture_count("Greek");
    TEST_ASSERT_TRUE(count >= 10);
}

void test_culture_count_case_insensitive(void)
{
    int c1 = wisdom_culture_count("Indian");
    int c2 = wisdom_culture_count("indian");
    TEST_ASSERT_EQUAL_INT(c1, c2);
}

void test_culture_count_nonexistent_returns_zero(void)
{
    int count = wisdom_culture_count("Atlantean");
    TEST_ASSERT_EQUAL_INT(0, count);
}

void test_culture_count_null_returns_zero(void)
{
    int count = wisdom_culture_count(NULL);
    TEST_ASSERT_EQUAL_INT(0, count);
}

/* ===== wisdom_distinct_cultures ===== */

void test_distinct_cultures_at_least_8(void)
{
    int count = wisdom_distinct_cultures();
    TEST_ASSERT_TRUE(count >= 8);
}

void test_distinct_cultures_positive(void)
{
    TEST_ASSERT_TRUE(wisdom_distinct_cultures() > 0);
}

/* ===== Coverage: at least 8 cultural traditions ===== */

void test_cultures_greek_present(void)
{
    TEST_ASSERT_TRUE(wisdom_culture_count("Greek") > 0);
}

void test_cultures_eastern_present(void)
{
    TEST_ASSERT_TRUE(wisdom_culture_count("Eastern") > 0);
}

void test_cultures_islamic_present(void)
{
    TEST_ASSERT_TRUE(wisdom_culture_count("Islamic") > 0);
}

void test_cultures_indian_present(void)
{
    TEST_ASSERT_TRUE(wisdom_culture_count("Indian") > 0);
}

void test_cultures_mayan_present(void)
{
    TEST_ASSERT_TRUE(wisdom_culture_count("Mayan") > 0);
}

void test_cultures_hebrew_present(void)
{
    TEST_ASSERT_TRUE(wisdom_culture_count("Hebrew") > 0);
}

void test_cultures_modern_science_present(void)
{
    TEST_ASSERT_TRUE(wisdom_culture_count("Modern Science") > 0);
}

void test_cultures_other_present(void)
{
    /* At least one quote from a tradition outside the main seven */
    int total = wisdom_quote_count();
    int greek = wisdom_culture_count("Greek");
    int eastern = wisdom_culture_count("Eastern");
    int islamic = wisdom_culture_count("Islamic");
    int indian = wisdom_culture_count("Indian");
    int mayan = wisdom_culture_count("Mayan");
    int hebrew = wisdom_culture_count("Hebrew");
    int modern = wisdom_culture_count("Modern Science");
    int other = total - greek - eastern - islamic - indian - mayan - hebrew - modern;
    TEST_ASSERT_TRUE(other > 0);
}

/* ===== Known quote content ===== */

void test_plato_time_quote(void)
{
    int idx = wisdom_by_author("Plato");
    TEST_ASSERT_TRUE(idx >= 0);
    wisdom_t q = wisdom_quote_get(idx);
    TEST_ASSERT_TRUE(q.text != NULL);
    TEST_ASSERT_TRUE(strlen(q.text) > 10);
}

void test_einstein_quote_present(void)
{
    int idx = wisdom_by_author("Einstein");
    TEST_ASSERT_TRUE(idx >= 0);
    wisdom_t q = wisdom_quote_get(idx);
    TEST_ASSERT_EQUAL_STRING("Modern Science", q.culture);
}

void test_sagan_quote_present(void)
{
    int idx = wisdom_by_author("Carl Sagan");
    TEST_ASSERT_TRUE(idx >= 0);
}

void test_rumi_quote_present(void)
{
    int idx = wisdom_by_author("Rumi");
    TEST_ASSERT_TRUE(idx >= 0);
    wisdom_t q = wisdom_quote_get(idx);
    TEST_ASSERT_EQUAL_STRING("Islamic", q.culture);
}

void test_lao_tzu_quote_present(void)
{
    int idx = wisdom_by_author("Lao Tzu");
    TEST_ASSERT_TRUE(idx >= 0);
    wisdom_t q = wisdom_quote_get(idx);
    TEST_ASSERT_EQUAL_STRING("Eastern", q.culture);
}

void test_arguelles_quote_present(void)
{
    int idx = wisdom_by_author("Arguelles");
    TEST_ASSERT_TRUE(idx >= 0);
}

/* ===== main ===== */

int main(void)
{
    UNITY_BEGIN();

    /* wisdom_quote_count */
    RUN_TEST(test_quote_count_at_least_120);
    RUN_TEST(test_quote_count_positive);

    /* wisdom_quote_get */
    RUN_TEST(test_get_first_quote_valid);
    RUN_TEST(test_get_last_quote_valid);
    RUN_TEST(test_get_negative_index_returns_invalid);
    RUN_TEST(test_get_out_of_bounds_returns_invalid);
    RUN_TEST(test_get_large_index_returns_invalid);
    RUN_TEST(test_each_quote_has_nonempty_text);
    RUN_TEST(test_each_quote_has_nonempty_author);
    RUN_TEST(test_each_quote_has_nonempty_culture);
    RUN_TEST(test_each_quote_id_matches_index);
    RUN_TEST(test_each_quote_tag_count_in_range);
    RUN_TEST(test_each_quote_tags_valid);

    /* wisdom_by_tag */
    RUN_TEST(test_by_tag_astronomy_finds_results);
    RUN_TEST(test_by_tag_time_finds_results);
    RUN_TEST(test_by_tag_results_have_correct_tag);
    RUN_TEST(test_by_tag_respects_max_results);
    RUN_TEST(test_by_tag_invalid_returns_zero);
    RUN_TEST(test_by_tag_null_results_returns_zero);
    RUN_TEST(test_by_tag_zero_max_returns_zero);
    RUN_TEST(test_by_tag_all_tags_have_quotes);

    /* wisdom_by_culture */
    RUN_TEST(test_by_culture_greek_finds_results);
    RUN_TEST(test_by_culture_case_insensitive);
    RUN_TEST(test_by_culture_substring_match);
    RUN_TEST(test_by_culture_nonexistent_returns_zero);
    RUN_TEST(test_by_culture_null_returns_zero);
    RUN_TEST(test_by_culture_null_results_returns_zero);
    RUN_TEST(test_by_culture_respects_max_results);

    /* wisdom_by_author */
    RUN_TEST(test_by_author_plato);
    RUN_TEST(test_by_author_case_insensitive);
    RUN_TEST(test_by_author_substring_match);
    RUN_TEST(test_by_author_not_found);
    RUN_TEST(test_by_author_null_returns_negative);

    /* wisdom_tag_name */
    RUN_TEST(test_tag_name_astronomy);
    RUN_TEST(test_tag_name_time);
    RUN_TEST(test_tag_name_cycles);
    RUN_TEST(test_tag_name_unity);
    RUN_TEST(test_tag_name_mathematics);
    RUN_TEST(test_tag_name_nature);
    RUN_TEST(test_tag_name_consciousness);
    RUN_TEST(test_tag_name_cosmos);
    RUN_TEST(test_tag_name_harmony);
    RUN_TEST(test_tag_name_invalid);
    RUN_TEST(test_tag_name_negative);

    /* wisdom_culture_count */
    RUN_TEST(test_culture_count_greek);
    RUN_TEST(test_culture_count_case_insensitive);
    RUN_TEST(test_culture_count_nonexistent_returns_zero);
    RUN_TEST(test_culture_count_null_returns_zero);

    /* wisdom_distinct_cultures */
    RUN_TEST(test_distinct_cultures_at_least_8);
    RUN_TEST(test_distinct_cultures_positive);

    /* Culture presence */
    RUN_TEST(test_cultures_greek_present);
    RUN_TEST(test_cultures_eastern_present);
    RUN_TEST(test_cultures_islamic_present);
    RUN_TEST(test_cultures_indian_present);
    RUN_TEST(test_cultures_mayan_present);
    RUN_TEST(test_cultures_hebrew_present);
    RUN_TEST(test_cultures_modern_science_present);
    RUN_TEST(test_cultures_other_present);

    /* Known quotes */
    RUN_TEST(test_plato_time_quote);
    RUN_TEST(test_einstein_quote_present);
    RUN_TEST(test_sagan_quote_present);
    RUN_TEST(test_rumi_quote_present);
    RUN_TEST(test_lao_tzu_quote_present);
    RUN_TEST(test_arguelles_quote_present);

    return UNITY_END();
}
