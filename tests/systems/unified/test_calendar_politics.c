/* test_calendar_politics.c -- Calendar Politics tests
 * TDD RED phase: tests written before implementation.
 * Tests pure data module documenting key historical moments where
 * political power shaped global timekeeping. Each decision recorded
 * with who decided, when, what changed, what was lost, who benefited. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/calendar_politics.h"
#include <string.h>
#include <ctype.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== cp_decision_count ===== */

void test_decision_count_returns_10(void)
{
    TEST_ASSERT_EQUAL_INT(10, cp_decision_count());
}

void test_decision_count_within_max(void)
{
    TEST_ASSERT_TRUE(cp_decision_count() <= CP_MAX_DECISIONS);
}

/* ===== cp_decision_get — boundary checks ===== */

void test_decision_get_negative_index(void)
{
    cp_decision_t d = cp_decision_get(-1);
    TEST_ASSERT_NULL(d.title);
}

void test_decision_get_out_of_range(void)
{
    cp_decision_t d = cp_decision_get(10);
    TEST_ASSERT_NULL(d.title);
}

void test_decision_get_way_out_of_range(void)
{
    cp_decision_t d = cp_decision_get(999);
    TEST_ASSERT_NULL(d.title);
}

/* ===== cp_decision_get — chronological order ===== */

void test_decisions_chronological_order(void)
{
    int count = cp_decision_count();
    for (int i = 1; i < count; i++) {
        cp_decision_t prev = cp_decision_get(i - 1);
        cp_decision_t curr = cp_decision_get(i);
        TEST_ASSERT_TRUE_MESSAGE(prev.year <= curr.year,
            "Decisions must be in chronological order");
    }
}

/* ===== cp_decision_get — entry 0: Julian Calendar Reform ===== */

void test_decision_0_title(void)
{
    cp_decision_t d = cp_decision_get(0);
    TEST_ASSERT_EQUAL_STRING("Julian Calendar Reform", d.title);
}

void test_decision_0_year(void)
{
    cp_decision_t d = cp_decision_get(0);
    TEST_ASSERT_EQUAL_INT(-46, d.year);
}

void test_decision_0_who(void)
{
    cp_decision_t d = cp_decision_get(0);
    TEST_ASSERT_NOT_NULL(d.who);
    TEST_ASSERT_NOT_NULL(strstr(d.who, "Julius Caesar"));
}

void test_decision_0_category(void)
{
    cp_decision_t d = cp_decision_get(0);
    TEST_ASSERT_EQUAL_INT(CP_CAT_IMPERIAL, d.category);
}

void test_decision_0_what_changed_nonempty(void)
{
    cp_decision_t d = cp_decision_get(0);
    TEST_ASSERT_NOT_NULL(d.what_changed);
    TEST_ASSERT_TRUE(strlen(d.what_changed) > 0);
}

void test_decision_0_what_lost_nonempty(void)
{
    cp_decision_t d = cp_decision_get(0);
    TEST_ASSERT_NOT_NULL(d.what_lost);
    TEST_ASSERT_TRUE(strlen(d.what_lost) > 0);
}

void test_decision_0_who_benefited_nonempty(void)
{
    cp_decision_t d = cp_decision_get(0);
    TEST_ASSERT_NOT_NULL(d.who_benefited);
    TEST_ASSERT_TRUE(strlen(d.who_benefited) > 0);
}

/* ===== cp_decision_get — entry 1: Constantine's Sabbath Shift ===== */

void test_decision_1_title(void)
{
    cp_decision_t d = cp_decision_get(1);
    TEST_ASSERT_EQUAL_STRING("Constantine's Sabbath Shift", d.title);
}

void test_decision_1_year(void)
{
    cp_decision_t d = cp_decision_get(1);
    TEST_ASSERT_EQUAL_INT(321, d.year);
}

void test_decision_1_category(void)
{
    cp_decision_t d = cp_decision_get(1);
    TEST_ASSERT_EQUAL_INT(CP_CAT_RELIGIOUS, d.category);
}

/* ===== cp_decision_get — entry 2: Council of Nicaea ===== */

void test_decision_2_title(void)
{
    cp_decision_t d = cp_decision_get(2);
    TEST_ASSERT_EQUAL_STRING("Council of Nicaea", d.title);
}

void test_decision_2_year(void)
{
    cp_decision_t d = cp_decision_get(2);
    TEST_ASSERT_EQUAL_INT(325, d.year);
}

void test_decision_2_category(void)
{
    cp_decision_t d = cp_decision_get(2);
    TEST_ASSERT_EQUAL_INT(CP_CAT_RELIGIOUS, d.category);
}

/* ===== cp_decision_get — entry 3: Anno Domini Epoch ===== */

void test_decision_3_title(void)
{
    cp_decision_t d = cp_decision_get(3);
    TEST_ASSERT_EQUAL_STRING("Anno Domini Epoch", d.title);
}

void test_decision_3_year(void)
{
    cp_decision_t d = cp_decision_get(3);
    TEST_ASSERT_EQUAL_INT(525, d.year);
}

void test_decision_3_who(void)
{
    cp_decision_t d = cp_decision_get(3);
    TEST_ASSERT_NOT_NULL(strstr(d.who, "Dionysius Exiguus"));
}

void test_decision_3_category(void)
{
    cp_decision_t d = cp_decision_get(3);
    TEST_ASSERT_EQUAL_INT(CP_CAT_RELIGIOUS, d.category);
}

/* ===== cp_decision_get — entry 4: Gregorian Calendar Reform ===== */

void test_decision_4_title(void)
{
    cp_decision_t d = cp_decision_get(4);
    TEST_ASSERT_EQUAL_STRING("Gregorian Calendar Reform", d.title);
}

void test_decision_4_year(void)
{
    cp_decision_t d = cp_decision_get(4);
    TEST_ASSERT_EQUAL_INT(1582, d.year);
}

void test_decision_4_who(void)
{
    cp_decision_t d = cp_decision_get(4);
    TEST_ASSERT_NOT_NULL(strstr(d.who, "Pope Gregory XIII"));
}

void test_decision_4_category(void)
{
    cp_decision_t d = cp_decision_get(4);
    TEST_ASSERT_EQUAL_INT(CP_CAT_SCIENTIFIC, d.category);
}

/* ===== cp_decision_get — entry 5: British Calendar Act ===== */

void test_decision_5_title(void)
{
    cp_decision_t d = cp_decision_get(5);
    TEST_ASSERT_EQUAL_STRING("British Calendar Act", d.title);
}

void test_decision_5_year(void)
{
    cp_decision_t d = cp_decision_get(5);
    TEST_ASSERT_EQUAL_INT(1752, d.year);
}

void test_decision_5_category(void)
{
    cp_decision_t d = cp_decision_get(5);
    TEST_ASSERT_EQUAL_INT(CP_CAT_IMPERIAL, d.category);
}

/* ===== cp_decision_get — entry 6: Railroad Standard Time ===== */

void test_decision_6_title(void)
{
    cp_decision_t d = cp_decision_get(6);
    TEST_ASSERT_EQUAL_STRING("Railroad Standard Time", d.title);
}

void test_decision_6_year(void)
{
    cp_decision_t d = cp_decision_get(6);
    TEST_ASSERT_EQUAL_INT(1883, d.year);
}

void test_decision_6_category(void)
{
    cp_decision_t d = cp_decision_get(6);
    TEST_ASSERT_EQUAL_INT(CP_CAT_COMMERCIAL, d.category);
}

/* ===== cp_decision_get — entry 7: International Meridian Conference ===== */

void test_decision_7_title(void)
{
    cp_decision_t d = cp_decision_get(7);
    TEST_ASSERT_EQUAL_STRING("International Meridian Conference", d.title);
}

void test_decision_7_year(void)
{
    cp_decision_t d = cp_decision_get(7);
    TEST_ASSERT_EQUAL_INT(1884, d.year);
}

void test_decision_7_category(void)
{
    cp_decision_t d = cp_decision_get(7);
    TEST_ASSERT_EQUAL_INT(CP_CAT_COMMERCIAL, d.category);
}

/* ===== cp_decision_get — entry 8: Standard Time Act ===== */

void test_decision_8_title(void)
{
    cp_decision_t d = cp_decision_get(8);
    TEST_ASSERT_EQUAL_STRING("Standard Time Act", d.title);
}

void test_decision_8_year(void)
{
    cp_decision_t d = cp_decision_get(8);
    TEST_ASSERT_EQUAL_INT(1918, d.year);
}

void test_decision_8_category(void)
{
    cp_decision_t d = cp_decision_get(8);
    TEST_ASSERT_EQUAL_INT(CP_CAT_COMMERCIAL, d.category);
}

/* ===== cp_decision_get — entry 9: Soviet Revolutionary Calendar ===== */

void test_decision_9_title(void)
{
    cp_decision_t d = cp_decision_get(9);
    TEST_ASSERT_EQUAL_STRING("Soviet Revolutionary Calendar", d.title);
}

void test_decision_9_year(void)
{
    cp_decision_t d = cp_decision_get(9);
    TEST_ASSERT_EQUAL_INT(1930, d.year);
}

void test_decision_9_who(void)
{
    cp_decision_t d = cp_decision_get(9);
    TEST_ASSERT_NOT_NULL(strstr(d.who, "Stalin"));
}

void test_decision_9_category(void)
{
    cp_decision_t d = cp_decision_get(9);
    TEST_ASSERT_EQUAL_INT(CP_CAT_IMPERIAL, d.category);
}

/* ===== All decisions have non-null required fields ===== */

void test_all_decisions_have_title(void)
{
    int count = cp_decision_count();
    for (int i = 0; i < count; i++) {
        cp_decision_t d = cp_decision_get(i);
        TEST_ASSERT_NOT_NULL(d.title);
        TEST_ASSERT_TRUE(strlen(d.title) > 0);
    }
}

void test_all_decisions_have_who(void)
{
    int count = cp_decision_count();
    for (int i = 0; i < count; i++) {
        cp_decision_t d = cp_decision_get(i);
        TEST_ASSERT_NOT_NULL(d.who);
        TEST_ASSERT_TRUE(strlen(d.who) > 0);
    }
}

void test_all_decisions_have_what_changed(void)
{
    int count = cp_decision_count();
    for (int i = 0; i < count; i++) {
        cp_decision_t d = cp_decision_get(i);
        TEST_ASSERT_NOT_NULL(d.what_changed);
        TEST_ASSERT_TRUE(strlen(d.what_changed) > 0);
    }
}

void test_all_decisions_have_who_benefited(void)
{
    int count = cp_decision_count();
    for (int i = 0; i < count; i++) {
        cp_decision_t d = cp_decision_get(i);
        TEST_ASSERT_NOT_NULL(d.who_benefited);
        TEST_ASSERT_TRUE(strlen(d.who_benefited) > 0);
    }
}

void test_all_decisions_have_valid_category(void)
{
    int count = cp_decision_count();
    for (int i = 0; i < count; i++) {
        cp_decision_t d = cp_decision_get(i);
        TEST_ASSERT_TRUE(d.category >= 0);
        TEST_ASSERT_TRUE(d.category < CP_CAT_COUNT);
    }
}

void test_all_descriptions_within_max_len(void)
{
    int count = cp_decision_count();
    for (int i = 0; i < count; i++) {
        cp_decision_t d = cp_decision_get(i);
        TEST_ASSERT_TRUE(strlen(d.what_changed) < CP_MAX_DESC);
        TEST_ASSERT_TRUE(strlen(d.what_lost) < CP_MAX_DESC);
        TEST_ASSERT_TRUE(strlen(d.who_benefited) < CP_MAX_DESC);
    }
}

/* ===== cp_category_name ===== */

void test_category_name_religious(void)
{
    TEST_ASSERT_EQUAL_STRING("Religious", cp_category_name(CP_CAT_RELIGIOUS));
}

void test_category_name_imperial(void)
{
    TEST_ASSERT_EQUAL_STRING("Imperial", cp_category_name(CP_CAT_IMPERIAL));
}

void test_category_name_commercial(void)
{
    TEST_ASSERT_EQUAL_STRING("Commercial", cp_category_name(CP_CAT_COMMERCIAL));
}

void test_category_name_scientific(void)
{
    TEST_ASSERT_EQUAL_STRING("Scientific", cp_category_name(CP_CAT_SCIENTIFIC));
}

void test_category_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("Unknown", cp_category_name(CP_CAT_COUNT));
}

void test_category_name_negative(void)
{
    TEST_ASSERT_EQUAL_STRING("Unknown", cp_category_name((cp_category_t)-1));
}

/* ===== cp_decisions_by_category ===== */

void test_decisions_by_category_religious(void)
{
    /* Entries 1,2,3 are RELIGIOUS */
    TEST_ASSERT_EQUAL_INT(3, cp_decisions_by_category(CP_CAT_RELIGIOUS));
}

void test_decisions_by_category_imperial(void)
{
    /* Entries 0,5,9 are IMPERIAL */
    TEST_ASSERT_EQUAL_INT(3, cp_decisions_by_category(CP_CAT_IMPERIAL));
}

void test_decisions_by_category_commercial(void)
{
    /* Entries 6,7,8 are COMMERCIAL */
    TEST_ASSERT_EQUAL_INT(3, cp_decisions_by_category(CP_CAT_COMMERCIAL));
}

void test_decisions_by_category_scientific(void)
{
    /* Entry 4 is SCIENTIFIC */
    TEST_ASSERT_EQUAL_INT(1, cp_decisions_by_category(CP_CAT_SCIENTIFIC));
}

void test_decisions_by_category_invalid(void)
{
    TEST_ASSERT_EQUAL_INT(0, cp_decisions_by_category(CP_CAT_COUNT));
}

void test_category_sum_equals_total(void)
{
    int total = 0;
    for (int c = 0; c < CP_CAT_COUNT; c++) {
        total += cp_decisions_by_category((cp_category_t)c);
    }
    TEST_ASSERT_EQUAL_INT(cp_decision_count(), total);
}

/* ===== cp_find_by_keyword ===== */

void test_find_by_keyword_julian(void)
{
    TEST_ASSERT_EQUAL_INT(0, cp_find_by_keyword("Julian"));
}

void test_find_by_keyword_case_insensitive(void)
{
    TEST_ASSERT_EQUAL_INT(0, cp_find_by_keyword("julian"));
}

void test_find_by_keyword_partial(void)
{
    /* "Railroad" should match "Railroad Standard Time" */
    TEST_ASSERT_EQUAL_INT(6, cp_find_by_keyword("Railroad"));
}

void test_find_by_keyword_soviet(void)
{
    TEST_ASSERT_EQUAL_INT(9, cp_find_by_keyword("soviet"));
}

void test_find_by_keyword_gregorian(void)
{
    TEST_ASSERT_EQUAL_INT(4, cp_find_by_keyword("Gregorian"));
}

void test_find_by_keyword_nicaea(void)
{
    TEST_ASSERT_EQUAL_INT(2, cp_find_by_keyword("Nicaea"));
}

void test_find_by_keyword_meridian(void)
{
    TEST_ASSERT_EQUAL_INT(7, cp_find_by_keyword("Meridian"));
}

void test_find_by_keyword_not_found(void)
{
    TEST_ASSERT_EQUAL_INT(-1, cp_find_by_keyword("Xyzzy"));
}

void test_find_by_keyword_null(void)
{
    TEST_ASSERT_EQUAL_INT(-1, cp_find_by_keyword(NULL));
}

void test_find_by_keyword_empty(void)
{
    /* Empty string matches first entry (substring of everything) */
    TEST_ASSERT_EQUAL_INT(0, cp_find_by_keyword(""));
}

void test_find_by_keyword_sabbath(void)
{
    TEST_ASSERT_EQUAL_INT(1, cp_find_by_keyword("Sabbath"));
}

void test_find_by_keyword_anno(void)
{
    TEST_ASSERT_EQUAL_INT(3, cp_find_by_keyword("Anno"));
}

void test_find_by_keyword_british(void)
{
    TEST_ASSERT_EQUAL_INT(5, cp_find_by_keyword("british"));
}

void test_find_by_keyword_standard_time(void)
{
    /* "Standard Time" appears in both entries 6 and 8;
     * should return the first match (6) */
    int idx = cp_find_by_keyword("Standard Time");
    TEST_ASSERT_TRUE(idx == 6 || idx == 8);
}

/* ===== cp_decisions_in_range ===== */

void test_range_all(void)
{
    int out[16];
    int count = cp_decisions_in_range(-100, 2100, out, 16);
    TEST_ASSERT_EQUAL_INT(10, count);
}

void test_range_ancient_only(void)
{
    int out[16];
    int count = cp_decisions_in_range(-100, 0, out, 16);
    /* Only Julian Calendar Reform at -46 */
    TEST_ASSERT_EQUAL_INT(1, count);
    TEST_ASSERT_EQUAL_INT(0, out[0]);
}

void test_range_modern_only(void)
{
    int out[16];
    int count = cp_decisions_in_range(1800, 2000, out, 16);
    /* Entries 6(1883), 7(1884), 8(1918), 9(1930) */
    TEST_ASSERT_EQUAL_INT(4, count);
}

void test_range_medieval(void)
{
    int out[16];
    int count = cp_decisions_in_range(300, 600, out, 16);
    /* Entries 1(321), 2(325), 3(525) */
    TEST_ASSERT_EQUAL_INT(3, count);
}

void test_range_empty(void)
{
    int out[16];
    int count = cp_decisions_in_range(100, 200, out, 16);
    TEST_ASSERT_EQUAL_INT(0, count);
}

void test_range_max_out_limits(void)
{
    int out[2];
    int count = cp_decisions_in_range(-100, 2100, out, 2);
    TEST_ASSERT_EQUAL_INT(2, count);
    /* Should be the first two */
    TEST_ASSERT_EQUAL_INT(0, out[0]);
    TEST_ASSERT_EQUAL_INT(1, out[1]);
}

void test_range_exact_year_match(void)
{
    int out[16];
    int count = cp_decisions_in_range(1582, 1582, out, 16);
    TEST_ASSERT_EQUAL_INT(1, count);
    TEST_ASSERT_EQUAL_INT(4, out[0]);
}

void test_range_null_out(void)
{
    int count = cp_decisions_in_range(-100, 2100, NULL, 0);
    TEST_ASSERT_EQUAL_INT(0, count);
}

void test_range_indices_chronological(void)
{
    int out[16];
    int count = cp_decisions_in_range(-100, 2100, out, 16);
    for (int i = 1; i < count; i++) {
        TEST_ASSERT_TRUE(out[i] > out[i - 1]);
    }
}

/* ===== cp_natural_losses_count ===== */

void test_natural_losses_count(void)
{
    int losses = cp_natural_losses_count();
    /* All 10 decisions have what_lost non-empty */
    TEST_ASSERT_EQUAL_INT(10, losses);
}

void test_natural_losses_not_negative(void)
{
    TEST_ASSERT_TRUE(cp_natural_losses_count() >= 0);
}

void test_natural_losses_at_most_total(void)
{
    TEST_ASSERT_TRUE(cp_natural_losses_count() <= cp_decision_count());
}

/* ===== cp_arguelles_quote ===== */

void test_arguelles_quote_not_null(void)
{
    TEST_ASSERT_NOT_NULL(cp_arguelles_quote());
}

void test_arguelles_quote_content(void)
{
    const char *q = cp_arguelles_quote();
    TEST_ASSERT_NOT_NULL(strstr(q, "Time is not money"));
    TEST_ASSERT_NOT_NULL(strstr(q, "Time is art"));
}

/* ===== cp_decision_get — data content spot checks ===== */

void test_decision_4_mentions_10_days(void)
{
    cp_decision_t d = cp_decision_get(4);
    TEST_ASSERT_NOT_NULL(strstr(d.what_changed, "10 days"));
}

void test_decision_5_mentions_11_days(void)
{
    cp_decision_t d = cp_decision_get(5);
    TEST_ASSERT_NOT_NULL(strstr(d.what_changed, "11 days"));
}

void test_decision_6_mentions_local_solar(void)
{
    cp_decision_t d = cp_decision_get(6);
    TEST_ASSERT_NOT_NULL(strstr(d.what_lost, "solar noon"));
}

void test_decision_7_mentions_greenwich(void)
{
    cp_decision_t d = cp_decision_get(7);
    TEST_ASSERT_NOT_NULL(strstr(d.what_changed, "Greenwich"));
}

void test_decision_8_mentions_daylight_saving(void)
{
    cp_decision_t d = cp_decision_get(8);
    TEST_ASSERT_NOT_NULL(strstr(d.what_changed, "Daylight Saving"));
}

void test_decision_9_mentions_5_day(void)
{
    cp_decision_t d = cp_decision_get(9);
    TEST_ASSERT_NOT_NULL(strstr(d.what_changed, "5-day"));
}

/* ===== Constants ===== */

void test_max_decisions_value(void)
{
    TEST_ASSERT_EQUAL_INT(16, CP_MAX_DECISIONS);
}

void test_max_desc_value(void)
{
    TEST_ASSERT_EQUAL_INT(256, CP_MAX_DESC);
}

void test_category_count_value(void)
{
    TEST_ASSERT_EQUAL_INT(4, CP_CAT_COUNT);
}

/* ===== main ===== */

int main(void)
{
    UNITY_BEGIN();

    /* cp_decision_count */
    RUN_TEST(test_decision_count_returns_10);
    RUN_TEST(test_decision_count_within_max);

    /* cp_decision_get boundary */
    RUN_TEST(test_decision_get_negative_index);
    RUN_TEST(test_decision_get_out_of_range);
    RUN_TEST(test_decision_get_way_out_of_range);

    /* chronological order */
    RUN_TEST(test_decisions_chronological_order);

    /* entry 0 */
    RUN_TEST(test_decision_0_title);
    RUN_TEST(test_decision_0_year);
    RUN_TEST(test_decision_0_who);
    RUN_TEST(test_decision_0_category);
    RUN_TEST(test_decision_0_what_changed_nonempty);
    RUN_TEST(test_decision_0_what_lost_nonempty);
    RUN_TEST(test_decision_0_who_benefited_nonempty);

    /* entry 1 */
    RUN_TEST(test_decision_1_title);
    RUN_TEST(test_decision_1_year);
    RUN_TEST(test_decision_1_category);

    /* entry 2 */
    RUN_TEST(test_decision_2_title);
    RUN_TEST(test_decision_2_year);
    RUN_TEST(test_decision_2_category);

    /* entry 3 */
    RUN_TEST(test_decision_3_title);
    RUN_TEST(test_decision_3_year);
    RUN_TEST(test_decision_3_who);
    RUN_TEST(test_decision_3_category);

    /* entry 4 */
    RUN_TEST(test_decision_4_title);
    RUN_TEST(test_decision_4_year);
    RUN_TEST(test_decision_4_who);
    RUN_TEST(test_decision_4_category);

    /* entry 5 */
    RUN_TEST(test_decision_5_title);
    RUN_TEST(test_decision_5_year);
    RUN_TEST(test_decision_5_category);

    /* entry 6 */
    RUN_TEST(test_decision_6_title);
    RUN_TEST(test_decision_6_year);
    RUN_TEST(test_decision_6_category);

    /* entry 7 */
    RUN_TEST(test_decision_7_title);
    RUN_TEST(test_decision_7_year);
    RUN_TEST(test_decision_7_category);

    /* entry 8 */
    RUN_TEST(test_decision_8_title);
    RUN_TEST(test_decision_8_year);
    RUN_TEST(test_decision_8_category);

    /* entry 9 */
    RUN_TEST(test_decision_9_title);
    RUN_TEST(test_decision_9_year);
    RUN_TEST(test_decision_9_who);
    RUN_TEST(test_decision_9_category);

    /* all decisions field validation */
    RUN_TEST(test_all_decisions_have_title);
    RUN_TEST(test_all_decisions_have_who);
    RUN_TEST(test_all_decisions_have_what_changed);
    RUN_TEST(test_all_decisions_have_who_benefited);
    RUN_TEST(test_all_decisions_have_valid_category);
    RUN_TEST(test_all_descriptions_within_max_len);

    /* cp_category_name */
    RUN_TEST(test_category_name_religious);
    RUN_TEST(test_category_name_imperial);
    RUN_TEST(test_category_name_commercial);
    RUN_TEST(test_category_name_scientific);
    RUN_TEST(test_category_name_invalid);
    RUN_TEST(test_category_name_negative);

    /* cp_decisions_by_category */
    RUN_TEST(test_decisions_by_category_religious);
    RUN_TEST(test_decisions_by_category_imperial);
    RUN_TEST(test_decisions_by_category_commercial);
    RUN_TEST(test_decisions_by_category_scientific);
    RUN_TEST(test_decisions_by_category_invalid);
    RUN_TEST(test_category_sum_equals_total);

    /* cp_find_by_keyword */
    RUN_TEST(test_find_by_keyword_julian);
    RUN_TEST(test_find_by_keyword_case_insensitive);
    RUN_TEST(test_find_by_keyword_partial);
    RUN_TEST(test_find_by_keyword_soviet);
    RUN_TEST(test_find_by_keyword_gregorian);
    RUN_TEST(test_find_by_keyword_nicaea);
    RUN_TEST(test_find_by_keyword_meridian);
    RUN_TEST(test_find_by_keyword_not_found);
    RUN_TEST(test_find_by_keyword_null);
    RUN_TEST(test_find_by_keyword_empty);
    RUN_TEST(test_find_by_keyword_sabbath);
    RUN_TEST(test_find_by_keyword_anno);
    RUN_TEST(test_find_by_keyword_british);
    RUN_TEST(test_find_by_keyword_standard_time);

    /* cp_decisions_in_range */
    RUN_TEST(test_range_all);
    RUN_TEST(test_range_ancient_only);
    RUN_TEST(test_range_modern_only);
    RUN_TEST(test_range_medieval);
    RUN_TEST(test_range_empty);
    RUN_TEST(test_range_max_out_limits);
    RUN_TEST(test_range_exact_year_match);
    RUN_TEST(test_range_null_out);
    RUN_TEST(test_range_indices_chronological);

    /* cp_natural_losses_count */
    RUN_TEST(test_natural_losses_count);
    RUN_TEST(test_natural_losses_not_negative);
    RUN_TEST(test_natural_losses_at_most_total);

    /* cp_arguelles_quote */
    RUN_TEST(test_arguelles_quote_not_null);
    RUN_TEST(test_arguelles_quote_content);

    /* data content spot checks */
    RUN_TEST(test_decision_4_mentions_10_days);
    RUN_TEST(test_decision_5_mentions_11_days);
    RUN_TEST(test_decision_6_mentions_local_solar);
    RUN_TEST(test_decision_7_mentions_greenwich);
    RUN_TEST(test_decision_8_mentions_daylight_saving);
    RUN_TEST(test_decision_9_mentions_5_day);

    /* constants */
    RUN_TEST(test_max_decisions_value);
    RUN_TEST(test_max_desc_value);
    RUN_TEST(test_category_count_value);

    return UNITY_END();
}
