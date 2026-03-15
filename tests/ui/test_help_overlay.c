#include "../unity/unity.h"
#include "../../src/ui/help_overlay.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ---- 1-6: Category names — all return non-NULL, non-empty ---- */

void test_category_name_time(void)
{
    const char *name = help_category_name(HELP_CAT_TIME);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_TRUE(strlen(name) > 0);
    TEST_ASSERT_EQUAL_STRING("Time Controls", name);
}

void test_category_name_camera(void)
{
    const char *name = help_category_name(HELP_CAT_CAMERA);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_TRUE(strlen(name) > 0);
    TEST_ASSERT_EQUAL_STRING("Camera", name);
}

void test_category_name_layers(void)
{
    const char *name = help_category_name(HELP_CAT_LAYERS);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_EQUAL_STRING("Layers", name);
}

void test_category_name_views(void)
{
    const char *name = help_category_name(HELP_CAT_VIEWS);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_EQUAL_STRING("Views", name);
}

void test_category_name_systems(void)
{
    const char *name = help_category_name(HELP_CAT_SYSTEMS);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_EQUAL_STRING("Systems", name);
}

void test_category_name_ui(void)
{
    const char *name = help_category_name(HELP_CAT_UI);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_EQUAL_STRING("UI", name);
}

/* ---- 7-8: Category descriptions — all return non-NULL ---- */

void test_category_desc_all_non_null(void)
{
    for (int i = 0; i < HELP_CAT_COUNT; i++) {
        const char *desc = help_category_desc((help_category_t)i);
        TEST_ASSERT_NOT_NULL(desc);
        TEST_ASSERT_TRUE(strlen(desc) > 0);
    }
}

void test_category_desc_invalid(void)
{
    const char *desc = help_category_desc(HELP_CAT_COUNT);
    TEST_ASSERT_NOT_NULL(desc);
    TEST_ASSERT_EQUAL_STRING("Unknown", desc);
}

/* ---- 9-11: help_get_entries — correct counts ---- */

void test_get_entries_time_count(void)
{
    help_entry_t entries[HELP_MAX_ENTRIES_PER_CAT];
    int count = help_get_entries(HELP_CAT_TIME, entries, HELP_MAX_ENTRIES_PER_CAT);
    TEST_ASSERT_EQUAL_INT(8, count);
}

void test_get_entries_camera_count(void)
{
    help_entry_t entries[HELP_MAX_ENTRIES_PER_CAT];
    int count = help_get_entries(HELP_CAT_CAMERA, entries, HELP_MAX_ENTRIES_PER_CAT);
    TEST_ASSERT_EQUAL_INT(9, count);
}

void test_get_entries_all_have_key_and_action(void)
{
    for (int cat = 0; cat < HELP_CAT_COUNT; cat++) {
        help_entry_t entries[HELP_MAX_ENTRIES_PER_CAT];
        int count = help_get_entries((help_category_t)cat, entries,
                                     HELP_MAX_ENTRIES_PER_CAT);
        TEST_ASSERT_TRUE(count > 0);
        for (int i = 0; i < count; i++) {
            TEST_ASSERT_NOT_NULL(entries[i].key);
            TEST_ASSERT_NOT_NULL(entries[i].action);
            TEST_ASSERT_TRUE(strlen(entries[i].key) > 0);
            TEST_ASSERT_TRUE(strlen(entries[i].action) > 0);
        }
    }
}

/* ---- 12-13: help_get_group ---- */

void test_get_group_name_matches(void)
{
    for (int cat = 0; cat < HELP_CAT_COUNT; cat++) {
        help_group_t group = help_get_group((help_category_t)cat);
        const char *expected = help_category_name((help_category_t)cat);
        TEST_ASSERT_EQUAL_STRING(expected, group.name);
    }
}

void test_get_group_count_positive(void)
{
    for (int cat = 0; cat < HELP_CAT_COUNT; cat++) {
        help_group_t group = help_get_group((help_category_t)cat);
        TEST_ASSERT_TRUE(group.count > 0);
    }
}

/* ---- 14: help_total_entries ---- */

void test_total_entries_sum(void)
{
    int expected = 0;
    for (int cat = 0; cat < HELP_CAT_COUNT; cat++) {
        help_entry_t entries[HELP_MAX_ENTRIES_PER_CAT];
        expected += help_get_entries((help_category_t)cat, entries,
                                     HELP_MAX_ENTRIES_PER_CAT);
    }
    TEST_ASSERT_EQUAL_INT(expected, help_total_entries());
    /* Exact: 8+9+5+2+5+2 = 31 */
    TEST_ASSERT_EQUAL_INT(31, help_total_entries());
}

/* ---- 15-17: help_format_entry ---- */

void test_format_entry_returns_positive(void)
{
    help_entry_t entry = { "Space", "Toggle pause/resume", HELP_CAT_TIME };
    char buf[256];
    int n = help_format_entry(&entry, buf, 256);
    TEST_ASSERT_TRUE(n > 0);
}

void test_format_entry_contains_key_and_action(void)
{
    help_entry_t entry = { "Shift+3", "Solar System (default)", HELP_CAT_CAMERA };
    char buf[256];
    help_format_entry(&entry, buf, 256);
    TEST_ASSERT_NOT_NULL(strstr(buf, "Shift+3"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Solar System"));
}

void test_format_entry_small_buffer(void)
{
    help_entry_t entry = { "Space", "Toggle pause/resume", HELP_CAT_TIME };
    char buf[8];
    int n = help_format_entry(&entry, buf, 8);
    TEST_ASSERT_TRUE(n >= 0);
    TEST_ASSERT_TRUE(n < 8);
    TEST_ASSERT_TRUE(strlen(buf) < 8);
}

/* ---- 18-20: help_format_group ---- */

void test_format_group_contains_header(void)
{
    char buf[HELP_FORMAT_BUF_SIZE];
    int n = help_format_group(HELP_CAT_TIME, buf, HELP_FORMAT_BUF_SIZE);
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "=== Time Controls ==="));
}

void test_format_group_contains_entries(void)
{
    char buf[HELP_FORMAT_BUF_SIZE];
    help_format_group(HELP_CAT_TIME, buf, HELP_FORMAT_BUF_SIZE);
    TEST_ASSERT_NOT_NULL(strstr(buf, "Space"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Toggle pause/resume"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Reverse time direction"));
}

void test_format_group_camera_has_shift_entries(void)
{
    char buf[HELP_FORMAT_BUF_SIZE];
    help_format_group(HELP_CAT_CAMERA, buf, HELP_FORMAT_BUF_SIZE);
    TEST_ASSERT_NOT_NULL(strstr(buf, "Shift+0"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Shift+6"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Observable Universe"));
}

/* ---- 21-23: help_format_all ---- */

void test_format_all_contains_every_category(void)
{
    char buf[HELP_FORMAT_BUF_SIZE];
    help_format_all(buf, HELP_FORMAT_BUF_SIZE);
    for (int cat = 0; cat < HELP_CAT_COUNT; cat++) {
        const char *name = help_category_name((help_category_t)cat);
        TEST_ASSERT_NOT_NULL_MESSAGE(strstr(buf, name),
                                     "Missing category in format_all output");
    }
}

void test_format_all_returns_positive(void)
{
    char buf[HELP_FORMAT_BUF_SIZE];
    int n = help_format_all(buf, HELP_FORMAT_BUF_SIZE);
    TEST_ASSERT_TRUE(n > 0);
}

void test_format_all_small_buffer(void)
{
    char buf[32];
    int n = help_format_all(buf, 32);
    TEST_ASSERT_TRUE(n >= 0);
    TEST_ASSERT_TRUE(n < 32);
    TEST_ASSERT_TRUE(strlen(buf) < 32);
}

/* ---- 24: help_category_count ---- */

void test_category_count_equals_enum(void)
{
    TEST_ASSERT_EQUAL_INT(HELP_CAT_COUNT, help_category_count());
    TEST_ASSERT_EQUAL_INT(6, help_category_count());
}

/* ---- 25-30: Specific entry existence tests ---- */

void test_space_in_time_category(void)
{
    help_entry_t entries[HELP_MAX_ENTRIES_PER_CAT];
    int count = help_get_entries(HELP_CAT_TIME, entries, HELP_MAX_ENTRIES_PER_CAT);
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].key, "Space") == 0) {
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE_MESSAGE(found, "Space key not found in TIME category");
}

void test_question_mark_in_ui_category(void)
{
    help_entry_t entries[HELP_MAX_ENTRIES_PER_CAT];
    int count = help_get_entries(HELP_CAT_UI, entries, HELP_MAX_ENTRIES_PER_CAT);
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].key, "?") == 0) {
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE_MESSAGE(found, "? key not found in UI category");
}

void test_shift3_in_camera_category(void)
{
    help_entry_t entries[HELP_MAX_ENTRIES_PER_CAT];
    int count = help_get_entries(HELP_CAT_CAMERA, entries, HELP_MAX_ENTRIES_PER_CAT);
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].key, "Shift+3") == 0) {
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE_MESSAGE(found, "Shift+3 key not found in CAMERA category");
}

void test_t_in_layers_category(void)
{
    help_entry_t entries[HELP_MAX_ENTRIES_PER_CAT];
    int count = help_get_entries(HELP_CAT_LAYERS, entries, HELP_MAX_ENTRIES_PER_CAT);
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].key, "T") == 0) {
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE_MESSAGE(found, "T key not found in LAYERS category");
}

void test_a_in_systems_category(void)
{
    help_entry_t entries[HELP_MAX_ENTRIES_PER_CAT];
    int count = help_get_entries(HELP_CAT_SYSTEMS, entries, HELP_MAX_ENTRIES_PER_CAT);
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].key, "A") == 0) {
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE_MESSAGE(found, "A key not found in SYSTEMS category");
}

void test_e_in_views_category(void)
{
    help_entry_t entries[HELP_MAX_ENTRIES_PER_CAT];
    int count = help_get_entries(HELP_CAT_VIEWS, entries, HELP_MAX_ENTRIES_PER_CAT);
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].key, "E") == 0) {
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE_MESSAGE(found, "E key not found in VIEWS category");
}

/* ---- 31-32: Edge cases ---- */

void test_get_entries_max_zero_returns_zero(void)
{
    help_entry_t entries[1];
    int count = help_get_entries(HELP_CAT_TIME, entries, 0);
    TEST_ASSERT_EQUAL_INT(0, count);
}

void test_format_entry_buf_size_one(void)
{
    help_entry_t entry = { "Space", "Toggle", HELP_CAT_TIME };
    char buf[2];
    int n = help_format_entry(&entry, buf, 1);
    /* Should write nothing or empty string, not crash */
    TEST_ASSERT_TRUE(n >= 0);
}

/* ---- 33: Every entry category matches its group ---- */

void test_all_entries_category_matches_group(void)
{
    for (int cat = 0; cat < HELP_CAT_COUNT; cat++) {
        help_entry_t entries[HELP_MAX_ENTRIES_PER_CAT];
        int count = help_get_entries((help_category_t)cat, entries,
                                     HELP_MAX_ENTRIES_PER_CAT);
        for (int i = 0; i < count; i++) {
            TEST_ASSERT_EQUAL_INT(cat, entries[i].category);
        }
    }
}

/* ---- 34-36: Additional robustness ---- */

void test_category_name_invalid_returns_unknown(void)
{
    const char *name = help_category_name(HELP_CAT_COUNT);
    TEST_ASSERT_EQUAL_STRING("Unknown", name);
}

void test_get_entries_invalid_category(void)
{
    help_entry_t entries[HELP_MAX_ENTRIES_PER_CAT];
    int count = help_get_entries(HELP_CAT_COUNT, entries, HELP_MAX_ENTRIES_PER_CAT);
    TEST_ASSERT_EQUAL_INT(0, count);
}

void test_format_group_invalid_category(void)
{
    char buf[256];
    int n = help_format_group(HELP_CAT_COUNT, buf, 256);
    TEST_ASSERT_EQUAL_INT(0, n);
}

/* ---- 37-38: Null pointer handling ---- */

void test_format_entry_null_entry(void)
{
    char buf[256];
    int n = help_format_entry(NULL, buf, 256);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_format_all_null_buf(void)
{
    int n = help_format_all(NULL, 256);
    TEST_ASSERT_EQUAL_INT(0, n);
}

/* ---- 39: Group description populated ---- */

void test_get_group_has_description(void)
{
    for (int cat = 0; cat < HELP_CAT_COUNT; cat++) {
        help_group_t group = help_get_group((help_category_t)cat);
        TEST_ASSERT_NOT_NULL(group.description);
        TEST_ASSERT_TRUE(strlen(group.description) > 0);
    }
}

/* ---- 40: Get entries with limited capacity ---- */

void test_get_entries_limited_capacity(void)
{
    /* TIME has 8 entries, but only ask for 3 */
    help_entry_t entries[3];
    int count = help_get_entries(HELP_CAT_TIME, entries, 3);
    TEST_ASSERT_EQUAL_INT(3, count);
    /* First entry should still be Space */
    TEST_ASSERT_EQUAL_STRING("Space", entries[0].key);
}

/* ---- Main runner ---- */

int main(void)
{
    UNITY_BEGIN();

    /* Category names: 1-6 */
    RUN_TEST(test_category_name_time);
    RUN_TEST(test_category_name_camera);
    RUN_TEST(test_category_name_layers);
    RUN_TEST(test_category_name_views);
    RUN_TEST(test_category_name_systems);
    RUN_TEST(test_category_name_ui);

    /* Category descriptions: 7-8 */
    RUN_TEST(test_category_desc_all_non_null);
    RUN_TEST(test_category_desc_invalid);

    /* help_get_entries: 9-11 */
    RUN_TEST(test_get_entries_time_count);
    RUN_TEST(test_get_entries_camera_count);
    RUN_TEST(test_get_entries_all_have_key_and_action);

    /* help_get_group: 12-13 */
    RUN_TEST(test_get_group_name_matches);
    RUN_TEST(test_get_group_count_positive);

    /* help_total_entries: 14 */
    RUN_TEST(test_total_entries_sum);

    /* help_format_entry: 15-17 */
    RUN_TEST(test_format_entry_returns_positive);
    RUN_TEST(test_format_entry_contains_key_and_action);
    RUN_TEST(test_format_entry_small_buffer);

    /* help_format_group: 18-20 */
    RUN_TEST(test_format_group_contains_header);
    RUN_TEST(test_format_group_contains_entries);
    RUN_TEST(test_format_group_camera_has_shift_entries);

    /* help_format_all: 21-23 */
    RUN_TEST(test_format_all_contains_every_category);
    RUN_TEST(test_format_all_returns_positive);
    RUN_TEST(test_format_all_small_buffer);

    /* help_category_count: 24 */
    RUN_TEST(test_category_count_equals_enum);

    /* Specific entry existence: 25-30 */
    RUN_TEST(test_space_in_time_category);
    RUN_TEST(test_question_mark_in_ui_category);
    RUN_TEST(test_shift3_in_camera_category);
    RUN_TEST(test_t_in_layers_category);
    RUN_TEST(test_a_in_systems_category);
    RUN_TEST(test_e_in_views_category);

    /* Edge cases: 31-32 */
    RUN_TEST(test_get_entries_max_zero_returns_zero);
    RUN_TEST(test_format_entry_buf_size_one);

    /* Category matching: 33 */
    RUN_TEST(test_all_entries_category_matches_group);

    /* Robustness: 34-36 */
    RUN_TEST(test_category_name_invalid_returns_unknown);
    RUN_TEST(test_get_entries_invalid_category);
    RUN_TEST(test_format_group_invalid_category);

    /* Null handling: 37-38 */
    RUN_TEST(test_format_entry_null_entry);
    RUN_TEST(test_format_all_null_buf);

    /* Description: 39 */
    RUN_TEST(test_get_group_has_description);

    /* Limited capacity: 40 */
    RUN_TEST(test_get_entries_limited_capacity);

    return UNITY_END();
}
