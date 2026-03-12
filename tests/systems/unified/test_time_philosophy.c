/* test_time_philosophy.c -- Philosophy of Time Database tests
 * Tests entry access, tradition filtering, theme filtering, author search,
 * tradition counting, distinct traditions, data integrity, and edge cases. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/time_philosophy.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== tp_entry_count ===== */

void test_entry_count_at_least_30(void)
{
    TEST_ASSERT_TRUE(tp_entry_count() >= 30);
}

void test_entry_count_positive(void)
{
    TEST_ASSERT_TRUE(tp_entry_count() > 0);
}

/* ===== tp_entry_get ===== */

void test_get_first_entry_valid(void)
{
    tp_entry_t e = tp_entry_get(0);
    TEST_ASSERT_EQUAL_INT(0, e.id);
    TEST_ASSERT_TRUE(e.quote != NULL);
    TEST_ASSERT_TRUE(e.author != NULL);
    TEST_ASSERT_TRUE(e.context != NULL);
    TEST_ASSERT_TRUE(e.theme_count > 0);
    TEST_ASSERT_TRUE(e.theme_count <= TP_MAX_THEMES);
}

void test_get_last_entry_valid(void)
{
    int count = tp_entry_count();
    tp_entry_t e = tp_entry_get(count - 1);
    TEST_ASSERT_EQUAL_INT(count - 1, e.id);
    TEST_ASSERT_TRUE(e.quote != NULL);
    TEST_ASSERT_TRUE(e.author != NULL);
}

void test_get_negative_index_returns_invalid(void)
{
    tp_entry_t e = tp_entry_get(-1);
    TEST_ASSERT_EQUAL_INT(-1, e.id);
}

void test_get_out_of_bounds_returns_invalid(void)
{
    tp_entry_t e = tp_entry_get(tp_entry_count());
    TEST_ASSERT_EQUAL_INT(-1, e.id);
}

void test_get_large_index_returns_invalid(void)
{
    tp_entry_t e = tp_entry_get(99999);
    TEST_ASSERT_EQUAL_INT(-1, e.id);
}

/* ===== Data integrity ===== */

void test_each_entry_has_nonempty_quote(void)
{
    int count = tp_entry_count();
    for (int i = 0; i < count; i++) {
        tp_entry_t e = tp_entry_get(i);
        TEST_ASSERT_TRUE(e.quote != NULL);
        TEST_ASSERT_TRUE(strlen(e.quote) > 0);
    }
}

void test_each_entry_has_nonempty_author(void)
{
    int count = tp_entry_count();
    for (int i = 0; i < count; i++) {
        tp_entry_t e = tp_entry_get(i);
        TEST_ASSERT_TRUE(e.author != NULL);
        TEST_ASSERT_TRUE(strlen(e.author) > 0);
    }
}

void test_each_entry_has_nonempty_context(void)
{
    int count = tp_entry_count();
    for (int i = 0; i < count; i++) {
        tp_entry_t e = tp_entry_get(i);
        TEST_ASSERT_TRUE(e.context != NULL);
        TEST_ASSERT_TRUE(strlen(e.context) > 0);
    }
}

void test_each_entry_id_matches_index(void)
{
    int count = tp_entry_count();
    for (int i = 0; i < count; i++) {
        tp_entry_t e = tp_entry_get(i);
        TEST_ASSERT_EQUAL_INT(i, e.id);
    }
}

void test_each_entry_theme_count_in_range(void)
{
    int count = tp_entry_count();
    for (int i = 0; i < count; i++) {
        tp_entry_t e = tp_entry_get(i);
        TEST_ASSERT_TRUE(e.theme_count >= 1);
        TEST_ASSERT_TRUE(e.theme_count <= TP_MAX_THEMES);
    }
}

void test_each_entry_themes_valid(void)
{
    int count = tp_entry_count();
    for (int i = 0; i < count; i++) {
        tp_entry_t e = tp_entry_get(i);
        for (int j = 0; j < e.theme_count; j++) {
            TEST_ASSERT_TRUE(e.themes[j] >= 0);
            TEST_ASSERT_TRUE(e.themes[j] < TP_THEME_COUNT);
        }
    }
}

void test_each_entry_tradition_valid(void)
{
    int count = tp_entry_count();
    for (int i = 0; i < count; i++) {
        tp_entry_t e = tp_entry_get(i);
        TEST_ASSERT_TRUE(e.tradition >= 0);
        TEST_ASSERT_TRUE(e.tradition < TP_TRADITION_COUNT);
    }
}

void test_each_entry_year_in_valid_range(void)
{
    int count = tp_entry_count();
    for (int i = 0; i < count; i++) {
        tp_entry_t e = tp_entry_get(i);
        TEST_ASSERT_TRUE(e.year >= -2000);
        TEST_ASSERT_TRUE(e.year <= 2100);
    }
}

/* ===== tp_tradition_name ===== */

void test_tradition_name_greek(void)
{
    TEST_ASSERT_EQUAL_STRING("Greek", tp_tradition_name(TP_TRADITION_GREEK));
}

void test_tradition_name_roman(void)
{
    TEST_ASSERT_EQUAL_STRING("Roman", tp_tradition_name(TP_TRADITION_ROMAN));
}

void test_tradition_name_christian(void)
{
    TEST_ASSERT_EQUAL_STRING("Christian", tp_tradition_name(TP_TRADITION_CHRISTIAN));
}

void test_tradition_name_islamic(void)
{
    TEST_ASSERT_EQUAL_STRING("Islamic", tp_tradition_name(TP_TRADITION_ISLAMIC));
}

void test_tradition_name_indian(void)
{
    TEST_ASSERT_EQUAL_STRING("Indian", tp_tradition_name(TP_TRADITION_INDIAN));
}

void test_tradition_name_buddhist(void)
{
    TEST_ASSERT_EQUAL_STRING("Buddhist", tp_tradition_name(TP_TRADITION_BUDDHIST));
}

void test_tradition_name_chinese(void)
{
    TEST_ASSERT_EQUAL_STRING("Chinese", tp_tradition_name(TP_TRADITION_CHINESE));
}

void test_tradition_name_mayan(void)
{
    TEST_ASSERT_EQUAL_STRING("Mayan", tp_tradition_name(TP_TRADITION_MAYAN));
}

void test_tradition_name_jewish(void)
{
    TEST_ASSERT_EQUAL_STRING("Jewish", tp_tradition_name(TP_TRADITION_JEWISH));
}

void test_tradition_name_persian(void)
{
    TEST_ASSERT_EQUAL_STRING("Persian", tp_tradition_name(TP_TRADITION_PERSIAN));
}

void test_tradition_name_modern(void)
{
    TEST_ASSERT_EQUAL_STRING("Modern", tp_tradition_name(TP_TRADITION_MODERN));
}

void test_tradition_name_scientific(void)
{
    TEST_ASSERT_EQUAL_STRING("Scientific", tp_tradition_name(TP_TRADITION_SCIENTIFIC));
}

void test_tradition_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("Unknown", tp_tradition_name(TP_TRADITION_COUNT));
}

void test_tradition_name_negative(void)
{
    TEST_ASSERT_EQUAL_STRING("Unknown", tp_tradition_name((tp_tradition_t)-1));
}

/* ===== tp_theme_name ===== */

void test_theme_name_nature_of_time(void)
{
    TEST_ASSERT_EQUAL_STRING("Nature of Time", tp_theme_name(TP_THEME_NATURE_OF_TIME));
}

void test_theme_name_cyclical(void)
{
    TEST_ASSERT_EQUAL_STRING("Cyclical", tp_theme_name(TP_THEME_CYCLICAL));
}

void test_theme_name_linear(void)
{
    TEST_ASSERT_EQUAL_STRING("Linear", tp_theme_name(TP_THEME_LINEAR));
}

void test_theme_name_subjective(void)
{
    TEST_ASSERT_EQUAL_STRING("Subjective", tp_theme_name(TP_THEME_SUBJECTIVE));
}

void test_theme_name_cosmic(void)
{
    TEST_ASSERT_EQUAL_STRING("Cosmic", tp_theme_name(TP_THEME_COSMIC));
}

void test_theme_name_sacred(void)
{
    TEST_ASSERT_EQUAL_STRING("Sacred", tp_theme_name(TP_THEME_SACRED));
}

void test_theme_name_measurement(void)
{
    TEST_ASSERT_EQUAL_STRING("Measurement", tp_theme_name(TP_THEME_MEASUREMENT));
}

void test_theme_name_impermanence(void)
{
    TEST_ASSERT_EQUAL_STRING("Impermanence", tp_theme_name(TP_THEME_IMPERMANENCE));
}

void test_theme_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("Unknown", tp_theme_name(TP_THEME_COUNT));
}

void test_theme_name_negative(void)
{
    TEST_ASSERT_EQUAL_STRING("Unknown", tp_theme_name((tp_theme_t)-1));
}

/* ===== tp_by_tradition ===== */

void test_by_tradition_greek_finds_results(void)
{
    int results[TP_MAX_RESULTS];
    int found = tp_by_tradition(TP_TRADITION_GREEK, results, TP_MAX_RESULTS);
    TEST_ASSERT_EQUAL_INT(4, found);
}

void test_by_tradition_christian_finds_results(void)
{
    int results[TP_MAX_RESULTS];
    int found = tp_by_tradition(TP_TRADITION_CHRISTIAN, results, TP_MAX_RESULTS);
    TEST_ASSERT_EQUAL_INT(2, found);
}

void test_by_tradition_islamic_finds_results(void)
{
    int results[TP_MAX_RESULTS];
    int found = tp_by_tradition(TP_TRADITION_ISLAMIC, results, TP_MAX_RESULTS);
    TEST_ASSERT_EQUAL_INT(3, found);
}

void test_by_tradition_indian_finds_results(void)
{
    int results[TP_MAX_RESULTS];
    int found = tp_by_tradition(TP_TRADITION_INDIAN, results, TP_MAX_RESULTS);
    TEST_ASSERT_EQUAL_INT(3, found);
}

void test_by_tradition_buddhist_finds_results(void)
{
    int results[TP_MAX_RESULTS];
    int found = tp_by_tradition(TP_TRADITION_BUDDHIST, results, TP_MAX_RESULTS);
    TEST_ASSERT_EQUAL_INT(3, found);
}

void test_by_tradition_chinese_finds_results(void)
{
    int results[TP_MAX_RESULTS];
    int found = tp_by_tradition(TP_TRADITION_CHINESE, results, TP_MAX_RESULTS);
    TEST_ASSERT_EQUAL_INT(3, found);
}

void test_by_tradition_jewish_finds_results(void)
{
    int results[TP_MAX_RESULTS];
    int found = tp_by_tradition(TP_TRADITION_JEWISH, results, TP_MAX_RESULTS);
    TEST_ASSERT_EQUAL_INT(2, found);
}

void test_by_tradition_persian_finds_results(void)
{
    int results[TP_MAX_RESULTS];
    int found = tp_by_tradition(TP_TRADITION_PERSIAN, results, TP_MAX_RESULTS);
    TEST_ASSERT_EQUAL_INT(1, found);
}

void test_by_tradition_mayan_finds_results(void)
{
    int results[TP_MAX_RESULTS];
    int found = tp_by_tradition(TP_TRADITION_MAYAN, results, TP_MAX_RESULTS);
    TEST_ASSERT_EQUAL_INT(1, found);
}

void test_by_tradition_modern_finds_results(void)
{
    int results[TP_MAX_RESULTS];
    int found = tp_by_tradition(TP_TRADITION_MODERN, results, TP_MAX_RESULTS);
    TEST_ASSERT_EQUAL_INT(4, found);
}

void test_by_tradition_scientific_finds_results(void)
{
    int results[TP_MAX_RESULTS];
    int found = tp_by_tradition(TP_TRADITION_SCIENTIFIC, results, TP_MAX_RESULTS);
    TEST_ASSERT_EQUAL_INT(4, found);
}

void test_by_tradition_results_have_correct_tradition(void)
{
    int results[TP_MAX_RESULTS];
    int found = tp_by_tradition(TP_TRADITION_GREEK, results, TP_MAX_RESULTS);
    for (int i = 0; i < found; i++) {
        tp_entry_t e = tp_entry_get(results[i]);
        TEST_ASSERT_EQUAL_INT(TP_TRADITION_GREEK, e.tradition);
    }
}

void test_by_tradition_respects_max_results(void)
{
    int results[2];
    int found = tp_by_tradition(TP_TRADITION_GREEK, results, 2);
    TEST_ASSERT_TRUE(found <= 2);
}

void test_by_tradition_invalid_returns_zero(void)
{
    int results[TP_MAX_RESULTS];
    int found = tp_by_tradition(TP_TRADITION_COUNT, results, TP_MAX_RESULTS);
    TEST_ASSERT_EQUAL_INT(0, found);
}

void test_by_tradition_null_results_returns_zero(void)
{
    int found = tp_by_tradition(TP_TRADITION_GREEK, NULL, TP_MAX_RESULTS);
    TEST_ASSERT_EQUAL_INT(0, found);
}

void test_by_tradition_zero_max_returns_zero(void)
{
    int results[TP_MAX_RESULTS];
    int found = tp_by_tradition(TP_TRADITION_GREEK, results, 0);
    TEST_ASSERT_EQUAL_INT(0, found);
}

/* ===== tp_by_theme ===== */

void test_by_theme_nature_of_time_finds_results(void)
{
    int results[TP_MAX_RESULTS];
    int found = tp_by_theme(TP_THEME_NATURE_OF_TIME, results, TP_MAX_RESULTS);
    TEST_ASSERT_TRUE(found > 0);
}

void test_by_theme_cyclical_finds_results(void)
{
    int results[TP_MAX_RESULTS];
    int found = tp_by_theme(TP_THEME_CYCLICAL, results, TP_MAX_RESULTS);
    TEST_ASSERT_TRUE(found > 0);
}

void test_by_theme_sacred_finds_results(void)
{
    int results[TP_MAX_RESULTS];
    int found = tp_by_theme(TP_THEME_SACRED, results, TP_MAX_RESULTS);
    TEST_ASSERT_TRUE(found > 0);
}

void test_by_theme_results_have_correct_theme(void)
{
    int results[TP_MAX_RESULTS];
    int found = tp_by_theme(TP_THEME_COSMIC, results, TP_MAX_RESULTS);
    for (int i = 0; i < found; i++) {
        tp_entry_t e = tp_entry_get(results[i]);
        int has_theme = 0;
        for (int j = 0; j < e.theme_count; j++) {
            if (e.themes[j] == TP_THEME_COSMIC) {
                has_theme = 1;
                break;
            }
        }
        TEST_ASSERT_EQUAL_INT(1, has_theme);
    }
}

void test_by_theme_respects_max_results(void)
{
    int results[2];
    int found = tp_by_theme(TP_THEME_NATURE_OF_TIME, results, 2);
    TEST_ASSERT_TRUE(found <= 2);
}

void test_by_theme_invalid_returns_zero(void)
{
    int results[TP_MAX_RESULTS];
    int found = tp_by_theme(TP_THEME_COUNT, results, TP_MAX_RESULTS);
    TEST_ASSERT_EQUAL_INT(0, found);
}

void test_by_theme_null_results_returns_zero(void)
{
    int found = tp_by_theme(TP_THEME_NATURE_OF_TIME, NULL, TP_MAX_RESULTS);
    TEST_ASSERT_EQUAL_INT(0, found);
}

void test_by_theme_zero_max_returns_zero(void)
{
    int results[TP_MAX_RESULTS];
    int found = tp_by_theme(TP_THEME_NATURE_OF_TIME, results, 0);
    TEST_ASSERT_EQUAL_INT(0, found);
}

void test_by_theme_all_themes_have_entries(void)
{
    for (int t = 0; t < TP_THEME_COUNT; t++) {
        int results[TP_MAX_RESULTS];
        int found = tp_by_theme((tp_theme_t)t, results, TP_MAX_RESULTS);
        TEST_ASSERT_TRUE(found > 0);
    }
}

/* ===== tp_by_author ===== */

void test_by_author_plato(void)
{
    int idx = tp_by_author("Plato");
    TEST_ASSERT_TRUE(idx >= 0);
    tp_entry_t e = tp_entry_get(idx);
    TEST_ASSERT_EQUAL_STRING("Plato", e.author);
}

void test_by_author_einstein(void)
{
    int idx = tp_by_author("Einstein");
    TEST_ASSERT_TRUE(idx >= 0);
}

void test_by_author_augustine(void)
{
    int idx = tp_by_author("Augustine");
    TEST_ASSERT_TRUE(idx >= 0);
}

void test_by_author_case_insensitive(void)
{
    int idx = tp_by_author("plato");
    TEST_ASSERT_TRUE(idx >= 0);
}

void test_by_author_not_found(void)
{
    int idx = tp_by_author("Nobody McFakerson");
    TEST_ASSERT_EQUAL_INT(-1, idx);
}

void test_by_author_null_returns_negative(void)
{
    int idx = tp_by_author(NULL);
    TEST_ASSERT_EQUAL_INT(-1, idx);
}

/* ===== tp_tradition_count ===== */

void test_tradition_count_greek(void)
{
    int count = tp_tradition_count(TP_TRADITION_GREEK);
    TEST_ASSERT_EQUAL_INT(4, count);
}

void test_tradition_count_scientific(void)
{
    int count = tp_tradition_count(TP_TRADITION_SCIENTIFIC);
    TEST_ASSERT_EQUAL_INT(4, count);
}

void test_tradition_count_invalid_returns_zero(void)
{
    int count = tp_tradition_count(TP_TRADITION_COUNT);
    TEST_ASSERT_EQUAL_INT(0, count);
}

void test_tradition_count_roman_zero(void)
{
    /* Roman tradition has no entries in this database */
    int count = tp_tradition_count(TP_TRADITION_ROMAN);
    TEST_ASSERT_EQUAL_INT(0, count);
}

/* ===== tp_distinct_traditions ===== */

void test_distinct_traditions_at_least_10(void)
{
    int count = tp_distinct_traditions();
    TEST_ASSERT_TRUE(count >= 10);
}

void test_distinct_traditions_positive(void)
{
    TEST_ASSERT_TRUE(tp_distinct_traditions() > 0);
}

void test_distinct_traditions_at_most_tradition_count(void)
{
    TEST_ASSERT_TRUE(tp_distinct_traditions() <= TP_TRADITION_COUNT);
}

/* ===== Known entry content ===== */

void test_plato_quote_content(void)
{
    int idx = tp_by_author("Plato");
    TEST_ASSERT_TRUE(idx >= 0);
    tp_entry_t e = tp_entry_get(idx);
    TEST_ASSERT_TRUE(strlen(e.quote) > 10);
    TEST_ASSERT_EQUAL_INT(TP_TRADITION_GREEK, e.tradition);
}

void test_arguelles_present(void)
{
    int idx = tp_by_author("Arguelles");
    TEST_ASSERT_TRUE(idx >= 0);
    tp_entry_t e = tp_entry_get(idx);
    TEST_ASSERT_EQUAL_INT(TP_TRADITION_MAYAN, e.tradition);
}

void test_newton_present(void)
{
    int idx = tp_by_author("Newton");
    TEST_ASSERT_TRUE(idx >= 0);
    tp_entry_t e = tp_entry_get(idx);
    TEST_ASSERT_EQUAL_INT(TP_TRADITION_MODERN, e.tradition);
}

void test_buddha_present(void)
{
    int idx = tp_by_author("Buddha");
    TEST_ASSERT_TRUE(idx >= 0);
    tp_entry_t e = tp_entry_get(idx);
    TEST_ASSERT_EQUAL_INT(TP_TRADITION_BUDDHIST, e.tradition);
}

void test_confucius_present(void)
{
    int idx = tp_by_author("Confucius");
    TEST_ASSERT_TRUE(idx >= 0);
    tp_entry_t e = tp_entry_get(idx);
    TEST_ASSERT_EQUAL_INT(TP_TRADITION_CHINESE, e.tradition);
}

/* ===== main ===== */

int main(void)
{
    UNITY_BEGIN();

    /* tp_entry_count */
    RUN_TEST(test_entry_count_at_least_30);
    RUN_TEST(test_entry_count_positive);

    /* tp_entry_get */
    RUN_TEST(test_get_first_entry_valid);
    RUN_TEST(test_get_last_entry_valid);
    RUN_TEST(test_get_negative_index_returns_invalid);
    RUN_TEST(test_get_out_of_bounds_returns_invalid);
    RUN_TEST(test_get_large_index_returns_invalid);

    /* Data integrity */
    RUN_TEST(test_each_entry_has_nonempty_quote);
    RUN_TEST(test_each_entry_has_nonempty_author);
    RUN_TEST(test_each_entry_has_nonempty_context);
    RUN_TEST(test_each_entry_id_matches_index);
    RUN_TEST(test_each_entry_theme_count_in_range);
    RUN_TEST(test_each_entry_themes_valid);
    RUN_TEST(test_each_entry_tradition_valid);
    RUN_TEST(test_each_entry_year_in_valid_range);

    /* tp_tradition_name */
    RUN_TEST(test_tradition_name_greek);
    RUN_TEST(test_tradition_name_roman);
    RUN_TEST(test_tradition_name_christian);
    RUN_TEST(test_tradition_name_islamic);
    RUN_TEST(test_tradition_name_indian);
    RUN_TEST(test_tradition_name_buddhist);
    RUN_TEST(test_tradition_name_chinese);
    RUN_TEST(test_tradition_name_mayan);
    RUN_TEST(test_tradition_name_jewish);
    RUN_TEST(test_tradition_name_persian);
    RUN_TEST(test_tradition_name_modern);
    RUN_TEST(test_tradition_name_scientific);
    RUN_TEST(test_tradition_name_invalid);
    RUN_TEST(test_tradition_name_negative);

    /* tp_theme_name */
    RUN_TEST(test_theme_name_nature_of_time);
    RUN_TEST(test_theme_name_cyclical);
    RUN_TEST(test_theme_name_linear);
    RUN_TEST(test_theme_name_subjective);
    RUN_TEST(test_theme_name_cosmic);
    RUN_TEST(test_theme_name_sacred);
    RUN_TEST(test_theme_name_measurement);
    RUN_TEST(test_theme_name_impermanence);
    RUN_TEST(test_theme_name_invalid);
    RUN_TEST(test_theme_name_negative);

    /* tp_by_tradition */
    RUN_TEST(test_by_tradition_greek_finds_results);
    RUN_TEST(test_by_tradition_christian_finds_results);
    RUN_TEST(test_by_tradition_islamic_finds_results);
    RUN_TEST(test_by_tradition_indian_finds_results);
    RUN_TEST(test_by_tradition_buddhist_finds_results);
    RUN_TEST(test_by_tradition_chinese_finds_results);
    RUN_TEST(test_by_tradition_jewish_finds_results);
    RUN_TEST(test_by_tradition_persian_finds_results);
    RUN_TEST(test_by_tradition_mayan_finds_results);
    RUN_TEST(test_by_tradition_modern_finds_results);
    RUN_TEST(test_by_tradition_scientific_finds_results);
    RUN_TEST(test_by_tradition_results_have_correct_tradition);
    RUN_TEST(test_by_tradition_respects_max_results);
    RUN_TEST(test_by_tradition_invalid_returns_zero);
    RUN_TEST(test_by_tradition_null_results_returns_zero);
    RUN_TEST(test_by_tradition_zero_max_returns_zero);

    /* tp_by_theme */
    RUN_TEST(test_by_theme_nature_of_time_finds_results);
    RUN_TEST(test_by_theme_cyclical_finds_results);
    RUN_TEST(test_by_theme_sacred_finds_results);
    RUN_TEST(test_by_theme_results_have_correct_theme);
    RUN_TEST(test_by_theme_respects_max_results);
    RUN_TEST(test_by_theme_invalid_returns_zero);
    RUN_TEST(test_by_theme_null_results_returns_zero);
    RUN_TEST(test_by_theme_zero_max_returns_zero);
    RUN_TEST(test_by_theme_all_themes_have_entries);

    /* tp_by_author */
    RUN_TEST(test_by_author_plato);
    RUN_TEST(test_by_author_einstein);
    RUN_TEST(test_by_author_augustine);
    RUN_TEST(test_by_author_case_insensitive);
    RUN_TEST(test_by_author_not_found);
    RUN_TEST(test_by_author_null_returns_negative);

    /* tp_tradition_count */
    RUN_TEST(test_tradition_count_greek);
    RUN_TEST(test_tradition_count_scientific);
    RUN_TEST(test_tradition_count_invalid_returns_zero);
    RUN_TEST(test_tradition_count_roman_zero);

    /* tp_distinct_traditions */
    RUN_TEST(test_distinct_traditions_at_least_10);
    RUN_TEST(test_distinct_traditions_positive);
    RUN_TEST(test_distinct_traditions_at_most_tradition_count);

    /* Known entries */
    RUN_TEST(test_plato_quote_content);
    RUN_TEST(test_arguelles_present);
    RUN_TEST(test_newton_present);
    RUN_TEST(test_buddha_present);
    RUN_TEST(test_confucius_present);

    return UNITY_END();
}
