/* test_drama_narrator.c — Tests for drama_narrator (LANGUAGE L2.2)
 *
 * TDD: these tests define the expected behavior of drama_narrator.h/.c.
 * The drama narrator provides pre-authored dramatic voice for achievement,
 * calendar_reform, and earth_drama data entries. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/drama_narrator.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ================================================================
 * Total count: 17 achievements + 10 calendars + 12 earth = 39
 * ================================================================ */

void test_total_count_is_39(void)
{
    TEST_ASSERT_EQUAL_INT(39, drama_narrator_count());
}

/* ================================================================
 * Source counts match source data modules
 * ================================================================ */

void test_achievement_count_is_17(void)
{
    TEST_ASSERT_EQUAL_INT(17, drama_narrator_count_by_source(DRAMA_SRC_ACHIEVEMENT));
}

void test_calendar_count_is_10(void)
{
    TEST_ASSERT_EQUAL_INT(10, drama_narrator_count_by_source(DRAMA_SRC_CALENDAR));
}

void test_earth_count_is_12(void)
{
    TEST_ASSERT_EQUAL_INT(12, drama_narrator_count_by_source(DRAMA_SRC_EARTH));
}

void test_invalid_source_count_is_zero(void)
{
    TEST_ASSERT_EQUAL_INT(0, drama_narrator_count_by_source(DRAMA_SRC_COUNT));
    TEST_ASSERT_EQUAL_INT(0, drama_narrator_count_by_source((drama_source_t)99));
}

/* ================================================================
 * Get by global index: valid entries
 * ================================================================ */

void test_get_first_entry_is_achievement_0(void)
{
    drama_entry_t e = drama_narrator_get(0);
    TEST_ASSERT_EQUAL_INT(0, e.id);
    TEST_ASSERT_EQUAL_INT(DRAMA_SRC_ACHIEVEMENT, e.source);
    TEST_ASSERT_EQUAL_INT(0, e.source_index);
    TEST_ASSERT_NOT_NULL(e.hook);
    TEST_ASSERT_NOT_NULL(e.body);
}

void test_get_last_achievement_is_index_16(void)
{
    drama_entry_t e = drama_narrator_get(16);
    TEST_ASSERT_EQUAL_INT(16, e.id);
    TEST_ASSERT_EQUAL_INT(DRAMA_SRC_ACHIEVEMENT, e.source);
    TEST_ASSERT_EQUAL_INT(16, e.source_index);
}

void test_get_first_calendar_is_index_17(void)
{
    drama_entry_t e = drama_narrator_get(17);
    TEST_ASSERT_EQUAL_INT(17, e.id);
    TEST_ASSERT_EQUAL_INT(DRAMA_SRC_CALENDAR, e.source);
    TEST_ASSERT_EQUAL_INT(0, e.source_index);
}

void test_get_last_calendar_is_index_26(void)
{
    drama_entry_t e = drama_narrator_get(26);
    TEST_ASSERT_EQUAL_INT(26, e.id);
    TEST_ASSERT_EQUAL_INT(DRAMA_SRC_CALENDAR, e.source);
    TEST_ASSERT_EQUAL_INT(9, e.source_index);
}

void test_get_first_earth_is_index_27(void)
{
    drama_entry_t e = drama_narrator_get(27);
    TEST_ASSERT_EQUAL_INT(27, e.id);
    TEST_ASSERT_EQUAL_INT(DRAMA_SRC_EARTH, e.source);
    TEST_ASSERT_EQUAL_INT(0, e.source_index);
}

void test_get_last_earth_is_index_38(void)
{
    drama_entry_t e = drama_narrator_get(38);
    TEST_ASSERT_EQUAL_INT(38, e.id);
    TEST_ASSERT_EQUAL_INT(DRAMA_SRC_EARTH, e.source);
    TEST_ASSERT_EQUAL_INT(11, e.source_index);
}

/* ================================================================
 * Invalid index returns id=-1
 * ================================================================ */

void test_get_negative_index_returns_invalid(void)
{
    drama_entry_t e = drama_narrator_get(-1);
    TEST_ASSERT_EQUAL_INT(-1, e.id);
    TEST_ASSERT_NULL(e.hook);
    TEST_ASSERT_NULL(e.body);
}

void test_get_out_of_range_returns_invalid(void)
{
    drama_entry_t e = drama_narrator_get(39);
    TEST_ASSERT_EQUAL_INT(-1, e.id);
    TEST_ASSERT_NULL(e.hook);
    TEST_ASSERT_NULL(e.body);
}

void test_get_large_index_returns_invalid(void)
{
    drama_entry_t e = drama_narrator_get(9999);
    TEST_ASSERT_EQUAL_INT(-1, e.id);
}

/* ================================================================
 * Lookup by source type + source index
 * ================================================================ */

void test_for_achievement_0_returns_eratosthenes(void)
{
    drama_entry_t e = drama_narrator_for(DRAMA_SRC_ACHIEVEMENT, 0);
    TEST_ASSERT_EQUAL_INT(0, e.id);
    TEST_ASSERT_EQUAL_INT(DRAMA_SRC_ACHIEVEMENT, e.source);
    TEST_ASSERT_EQUAL_INT(0, e.source_index);
    TEST_ASSERT_NOT_NULL(e.hook);
}

void test_for_achievement_4_returns_aryabhata(void)
{
    drama_entry_t e = drama_narrator_for(DRAMA_SRC_ACHIEVEMENT, 4);
    TEST_ASSERT_EQUAL_INT(4, e.id);
    TEST_ASSERT_EQUAL_INT(DRAMA_SRC_ACHIEVEMENT, e.source);
    TEST_ASSERT_EQUAL_INT(4, e.source_index);
}

void test_for_calendar_4_returns_gregorian(void)
{
    drama_entry_t e = drama_narrator_for(DRAMA_SRC_CALENDAR, 4);
    TEST_ASSERT_EQUAL_INT(21, e.id);
    TEST_ASSERT_EQUAL_INT(DRAMA_SRC_CALENDAR, e.source);
    TEST_ASSERT_EQUAL_INT(4, e.source_index);
}

void test_for_earth_0_returns_formation(void)
{
    drama_entry_t e = drama_narrator_for(DRAMA_SRC_EARTH, 0);
    TEST_ASSERT_EQUAL_INT(27, e.id);
    TEST_ASSERT_EQUAL_INT(DRAMA_SRC_EARTH, e.source);
    TEST_ASSERT_EQUAL_INT(0, e.source_index);
}

void test_for_invalid_source_returns_invalid(void)
{
    drama_entry_t e = drama_narrator_for(DRAMA_SRC_COUNT, 0);
    TEST_ASSERT_EQUAL_INT(-1, e.id);
}

void test_for_invalid_source_index_returns_invalid(void)
{
    drama_entry_t e = drama_narrator_for(DRAMA_SRC_ACHIEVEMENT, 17);
    TEST_ASSERT_EQUAL_INT(-1, e.id);

    e = drama_narrator_for(DRAMA_SRC_CALENDAR, 10);
    TEST_ASSERT_EQUAL_INT(-1, e.id);

    e = drama_narrator_for(DRAMA_SRC_EARTH, 12);
    TEST_ASSERT_EQUAL_INT(-1, e.id);
}

void test_for_negative_source_index_returns_invalid(void)
{
    drama_entry_t e = drama_narrator_for(DRAMA_SRC_ACHIEVEMENT, -1);
    TEST_ASSERT_EQUAL_INT(-1, e.id);
}

/* ================================================================
 * By source: fills indices correctly
 * ================================================================ */

void test_by_source_achievement_fills_17(void)
{
    int indices[20];
    int count = drama_narrator_by_source(DRAMA_SRC_ACHIEVEMENT, indices, 20);
    TEST_ASSERT_EQUAL_INT(17, count);
    /* First and last */
    TEST_ASSERT_EQUAL_INT(0, indices[0]);
    TEST_ASSERT_EQUAL_INT(16, indices[16]);
}

void test_by_source_calendar_fills_10(void)
{
    int indices[20];
    int count = drama_narrator_by_source(DRAMA_SRC_CALENDAR, indices, 20);
    TEST_ASSERT_EQUAL_INT(10, count);
    TEST_ASSERT_EQUAL_INT(17, indices[0]);
    TEST_ASSERT_EQUAL_INT(26, indices[9]);
}

void test_by_source_earth_fills_12(void)
{
    int indices[20];
    int count = drama_narrator_by_source(DRAMA_SRC_EARTH, indices, 20);
    TEST_ASSERT_EQUAL_INT(12, count);
    TEST_ASSERT_EQUAL_INT(27, indices[0]);
    TEST_ASSERT_EQUAL_INT(38, indices[11]);
}

void test_by_source_respects_max(void)
{
    int indices[3];
    int count = drama_narrator_by_source(DRAMA_SRC_ACHIEVEMENT, indices, 3);
    TEST_ASSERT_EQUAL_INT(3, count);
    TEST_ASSERT_EQUAL_INT(0, indices[0]);
    TEST_ASSERT_EQUAL_INT(1, indices[1]);
    TEST_ASSERT_EQUAL_INT(2, indices[2]);
}

void test_by_source_invalid_returns_zero(void)
{
    int indices[5];
    TEST_ASSERT_EQUAL_INT(0, drama_narrator_by_source(DRAMA_SRC_COUNT, indices, 5));
}

void test_by_source_null_indices_returns_zero(void)
{
    TEST_ASSERT_EQUAL_INT(0, drama_narrator_by_source(DRAMA_SRC_ACHIEVEMENT, NULL, 5));
}

void test_by_source_zero_max_returns_zero(void)
{
    int indices[5];
    TEST_ASSERT_EQUAL_INT(0, drama_narrator_by_source(DRAMA_SRC_ACHIEVEMENT, indices, 0));
}

/* ================================================================
 * Source name strings
 * ================================================================ */

void test_source_name_achievement(void)
{
    TEST_ASSERT_EQUAL_STRING("Achievement", drama_source_name(DRAMA_SRC_ACHIEVEMENT));
}

void test_source_name_calendar(void)
{
    TEST_ASSERT_EQUAL_STRING("Calendar", drama_source_name(DRAMA_SRC_CALENDAR));
}

void test_source_name_earth(void)
{
    TEST_ASSERT_EQUAL_STRING("Earth", drama_source_name(DRAMA_SRC_EARTH));
}

void test_source_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", drama_source_name(DRAMA_SRC_COUNT));
    TEST_ASSERT_EQUAL_STRING("?", drama_source_name((drama_source_t)-1));
}

/* ================================================================
 * Content quality: every entry has non-empty hook and body
 * ================================================================ */

void test_all_entries_have_nonempty_hook(void)
{
    int count = drama_narrator_count();
    for (int i = 0; i < count; i++) {
        drama_entry_t e = drama_narrator_get(i);
        TEST_ASSERT_NOT_NULL(e.hook);
        TEST_ASSERT_TRUE(strlen(e.hook) > 10);
    }
}

void test_all_entries_have_nonempty_body(void)
{
    int count = drama_narrator_count();
    for (int i = 0; i < count; i++) {
        drama_entry_t e = drama_narrator_get(i);
        TEST_ASSERT_NOT_NULL(e.body);
        TEST_ASSERT_TRUE(strlen(e.body) > 50);
    }
}

void test_hook_is_shorter_than_body(void)
{
    int count = drama_narrator_count();
    for (int i = 0; i < count; i++) {
        drama_entry_t e = drama_narrator_get(i);
        TEST_ASSERT_TRUE(strlen(e.hook) < strlen(e.body));
    }
}

/* ================================================================
 * Specific content spot-checks
 * ================================================================ */

void test_aryabhata_hook_mentions_107_km(void)
{
    drama_entry_t e = drama_narrator_for(DRAMA_SRC_ACHIEVEMENT, 4);
    TEST_ASSERT_NOT_NULL(strstr(e.hook, "107"));
}

void test_gregorian_hook_mentions_10_days(void)
{
    drama_entry_t e = drama_narrator_for(DRAMA_SRC_CALENDAR, 4);
    TEST_ASSERT_NOT_NULL(strstr(e.hook, "10 days"));
}

void test_snowball_body_mentions_85_million(void)
{
    /* Earth drama index 5: Snowball Earth (1000-541 Ma) */
    drama_entry_t e = drama_narrator_for(DRAMA_SRC_EARTH, 5);
    TEST_ASSERT_NOT_NULL(strstr(e.body, "85 million"));
}

void test_humans_hook_mentions_heartbeat(void)
{
    /* Earth drama index 11: Humans (0.3-0 Ma) */
    drama_entry_t e = drama_narrator_for(DRAMA_SRC_EARTH, 11);
    TEST_ASSERT_NOT_NULL(strstr(e.hook, "heartbeat"));
}

/* ================================================================
 * Consistency: sequential IDs, correct source mapping
 * ================================================================ */

void test_all_ids_are_sequential(void)
{
    int count = drama_narrator_count();
    for (int i = 0; i < count; i++) {
        drama_entry_t e = drama_narrator_get(i);
        TEST_ASSERT_EQUAL_INT(i, e.id);
    }
}

void test_source_indices_are_sequential_within_source(void)
{
    int prev_src_idx = -1;
    drama_source_t prev_source = DRAMA_SRC_ACHIEVEMENT;

    int count = drama_narrator_count();
    for (int i = 0; i < count; i++) {
        drama_entry_t e = drama_narrator_get(i);
        if (e.source != prev_source) {
            /* Source changed — reset */
            prev_src_idx = -1;
            prev_source = e.source;
        }
        TEST_ASSERT_EQUAL_INT(prev_src_idx + 1, e.source_index);
        prev_src_idx = e.source_index;
    }
}

/* ================================================================
 * Runner
 * ================================================================ */

int main(void)
{
    UNITY_BEGIN();

    /* Counts */
    RUN_TEST(test_total_count_is_39);
    RUN_TEST(test_achievement_count_is_17);
    RUN_TEST(test_calendar_count_is_10);
    RUN_TEST(test_earth_count_is_12);
    RUN_TEST(test_invalid_source_count_is_zero);

    /* Get by global index */
    RUN_TEST(test_get_first_entry_is_achievement_0);
    RUN_TEST(test_get_last_achievement_is_index_16);
    RUN_TEST(test_get_first_calendar_is_index_17);
    RUN_TEST(test_get_last_calendar_is_index_26);
    RUN_TEST(test_get_first_earth_is_index_27);
    RUN_TEST(test_get_last_earth_is_index_38);

    /* Invalid index */
    RUN_TEST(test_get_negative_index_returns_invalid);
    RUN_TEST(test_get_out_of_range_returns_invalid);
    RUN_TEST(test_get_large_index_returns_invalid);

    /* Lookup by source + source_index */
    RUN_TEST(test_for_achievement_0_returns_eratosthenes);
    RUN_TEST(test_for_achievement_4_returns_aryabhata);
    RUN_TEST(test_for_calendar_4_returns_gregorian);
    RUN_TEST(test_for_earth_0_returns_formation);
    RUN_TEST(test_for_invalid_source_returns_invalid);
    RUN_TEST(test_for_invalid_source_index_returns_invalid);
    RUN_TEST(test_for_negative_source_index_returns_invalid);

    /* By source fills */
    RUN_TEST(test_by_source_achievement_fills_17);
    RUN_TEST(test_by_source_calendar_fills_10);
    RUN_TEST(test_by_source_earth_fills_12);
    RUN_TEST(test_by_source_respects_max);
    RUN_TEST(test_by_source_invalid_returns_zero);
    RUN_TEST(test_by_source_null_indices_returns_zero);
    RUN_TEST(test_by_source_zero_max_returns_zero);

    /* Source names */
    RUN_TEST(test_source_name_achievement);
    RUN_TEST(test_source_name_calendar);
    RUN_TEST(test_source_name_earth);
    RUN_TEST(test_source_name_invalid);

    /* Content quality */
    RUN_TEST(test_all_entries_have_nonempty_hook);
    RUN_TEST(test_all_entries_have_nonempty_body);
    RUN_TEST(test_hook_is_shorter_than_body);

    /* Specific content */
    RUN_TEST(test_aryabhata_hook_mentions_107_km);
    RUN_TEST(test_gregorian_hook_mentions_10_days);
    RUN_TEST(test_snowball_body_mentions_85_million);
    RUN_TEST(test_humans_hook_mentions_heartbeat);

    /* Consistency */
    RUN_TEST(test_all_ids_are_sequential);
    RUN_TEST(test_source_indices_are_sequential_within_source);

    return UNITY_END();
}
