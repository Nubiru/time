#include "../unity/unity.h"
#include "../../src/ui/command_palette.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ---- 1: cmd_palette_count — returns correct total ---- */

void test_palette_count_returns_correct_total(void)
{
    int count = cmd_palette_count();
    TEST_ASSERT_EQUAL_INT(33, count);
}

/* ---- 2: cmd_palette_all with large buffer — returns full count ---- */

void test_palette_all_large_buffer(void)
{
    cmd_entry_t buf[64];
    int count = cmd_palette_all(buf, 64);
    TEST_ASSERT_EQUAL_INT(33, count);
}

/* ---- 3: cmd_palette_all with max=0 — returns 0 ---- */

void test_palette_all_max_zero(void)
{
    cmd_entry_t buf[1];
    int count = cmd_palette_all(buf, 0);
    TEST_ASSERT_EQUAL_INT(0, count);
}

/* ---- 4: cmd_palette_all with max=5 — returns 5 ---- */

void test_palette_all_max_five(void)
{
    cmd_entry_t buf[5];
    int count = cmd_palette_all(buf, 5);
    TEST_ASSERT_EQUAL_INT(5, count);
}

/* ---- 5: cmd_category_name — all 7 return non-NULL ---- */

void test_category_name_all_non_null(void)
{
    for (int i = 0; i < CMD_CAT_COUNT; i++) {
        const char *name = cmd_category_name((cmd_category_t)i);
        TEST_ASSERT_NOT_NULL(name);
        TEST_ASSERT_GREATER_THAN(0, (int)strlen(name));
    }
}

/* ---- 6: cmd_category_total — returns 7 ---- */

void test_category_total_returns_seven(void)
{
    TEST_ASSERT_EQUAL_INT(7, cmd_category_total());
}

/* ---- 7: cmd_palette_by_category TIME — returns 8 ---- */

void test_by_category_time_returns_eight(void)
{
    cmd_entry_t buf[16];
    int count = cmd_palette_by_category(CMD_CAT_TIME, buf, 16);
    TEST_ASSERT_EQUAL_INT(8, count);
}

/* ---- 8: cmd_palette_by_category CAMERA — returns 7 ---- */

void test_by_category_camera_returns_seven(void)
{
    cmd_entry_t buf[16];
    int count = cmd_palette_by_category(CMD_CAT_CAMERA, buf, 16);
    TEST_ASSERT_EQUAL_INT(7, count);
}

/* ---- 9: cmd_palette_by_category LAYER — returns 5 ---- */

void test_by_category_layer_returns_five(void)
{
    cmd_entry_t buf[16];
    int count = cmd_palette_by_category(CMD_CAT_LAYER, buf, 16);
    TEST_ASSERT_EQUAL_INT(5, count);
}

/* ---- 10: cmd_palette_by_category SYSTEM — returns 5 ---- */

void test_by_category_system_returns_five(void)
{
    cmd_entry_t buf[16];
    int count = cmd_palette_by_category(CMD_CAT_SYSTEM, buf, 16);
    TEST_ASSERT_EQUAL_INT(5, count);
}

/* ---- 11: cmd_palette_category_count — matches by_category result ---- */

void test_category_count_matches_by_category(void)
{
    for (int i = 0; i < CMD_CAT_COUNT; i++) {
        cmd_entry_t buf[64];
        int by_cat = cmd_palette_by_category((cmd_category_t)i, buf, 64);
        int cat_count = cmd_palette_category_count((cmd_category_t)i);
        TEST_ASSERT_EQUAL_INT(by_cat, cat_count);
    }
}

/* ---- 12: cmd_palette_search "" — returns all commands ---- */

void test_search_empty_returns_all(void)
{
    cmd_result_t result = cmd_palette_search("");
    TEST_ASSERT_EQUAL_INT(33, result.count);
}

/* ---- 13: cmd_palette_search NULL — returns all commands ---- */

void test_search_null_returns_all(void)
{
    cmd_result_t result = cmd_palette_search(NULL);
    TEST_ASSERT_EQUAL_INT(33, result.count);
}

/* ---- 14: cmd_palette_search "pause" — finds Pause (case-insensitive) ---- */

void test_search_pause_lowercase(void)
{
    cmd_result_t result = cmd_palette_search("pause");
    TEST_ASSERT_GREATER_THAN(0, result.count);
    /* First result should be "Pause" (name match) */
    TEST_ASSERT_EQUAL_STRING("Pause", result.entries[0].name);
}

/* ---- 15: cmd_palette_search "PAUSE" — finds Pause (case-insensitive) ---- */

void test_search_pause_uppercase(void)
{
    cmd_result_t result = cmd_palette_search("PAUSE");
    TEST_ASSERT_GREATER_THAN(0, result.count);
    TEST_ASSERT_EQUAL_STRING("Pause", result.entries[0].name);
}

/* ---- 16: cmd_palette_search "star" — finds Toggle Stars ---- */

void test_search_star_finds_toggle_stars(void)
{
    cmd_result_t result = cmd_palette_search("star");
    TEST_ASSERT_GREATER_THAN(0, result.count);
    /* Should find "Toggle Stars" */
    int found = 0;
    for (int i = 0; i < result.count; i++) {
        if (strcmp(result.entries[i].name, "Toggle Stars") == 0) {
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);
}

/* ---- 17: cmd_palette_search "zoom" — finds camera scale commands ---- */

void test_search_zoom_finds_camera_commands(void)
{
    cmd_result_t result = cmd_palette_search("zoom");
    TEST_ASSERT_GREATER_THAN(0, result.count);
    /* All camera scale commands have "Zoom to" in description */
    for (int i = 0; i < result.count; i++) {
        TEST_ASSERT_EQUAL(CMD_CAT_CAMERA, result.entries[i].category);
    }
}

/* ---- 18: cmd_palette_search "zzzzz" — returns 0 results ---- */

void test_search_nonsense_returns_zero(void)
{
    cmd_result_t result = cmd_palette_search("zzzzz");
    TEST_ASSERT_EQUAL_INT(0, result.count);
}

/* ---- 19: cmd_palette_search "shift" — finds all Shift+ shortcuts ---- */

void test_search_shift_finds_shift_shortcuts(void)
{
    cmd_result_t result = cmd_palette_search("shift");
    /* All 7 camera commands have Shift+ shortcuts */
    TEST_ASSERT_GREATER_OR_EQUAL(7, result.count);
}

/* ---- 20: cmd_palette_search "a" — finds multiple matches ---- */

void test_search_a_finds_multiple(void)
{
    cmd_result_t result = cmd_palette_search("a");
    TEST_ASSERT_GREATER_THAN(1, result.count);
}

/* ---- 21: cmd_palette_find "Pause" — found, shortcut is "Space" ---- */

void test_find_pause(void)
{
    cmd_entry_t entry = cmd_palette_find("Pause");
    TEST_ASSERT_NOT_NULL(entry.name);
    TEST_ASSERT_EQUAL_STRING("Space", entry.shortcut);
}

/* ---- 22: cmd_palette_find "Toggle Stars" — found, shortcut is "S" ---- */

void test_find_toggle_stars(void)
{
    cmd_entry_t entry = cmd_palette_find("Toggle Stars");
    TEST_ASSERT_NOT_NULL(entry.name);
    TEST_ASSERT_EQUAL_STRING("S", entry.shortcut);
}

/* ---- 23: cmd_palette_find "nonexistent" — name is NULL ---- */

void test_find_nonexistent(void)
{
    cmd_entry_t entry = cmd_palette_find("nonexistent");
    TEST_ASSERT_NULL(entry.name);
}

/* ---- 24: cmd_palette_find NULL — name is NULL ---- */

void test_find_null(void)
{
    cmd_entry_t entry = cmd_palette_find(NULL);
    TEST_ASSERT_NULL(entry.name);
}

/* ---- 25: cmd_palette_find_by_shortcut "Space" — found, name is "Pause" ---- */

void test_find_by_shortcut_space(void)
{
    cmd_entry_t entry = cmd_palette_find_by_shortcut("Space");
    TEST_ASSERT_NOT_NULL(entry.name);
    TEST_ASSERT_EQUAL_STRING("Pause", entry.name);
}

/* ---- 26: cmd_palette_find_by_shortcut "S" — found, name is "Toggle Stars" ---- */

void test_find_by_shortcut_s(void)
{
    cmd_entry_t entry = cmd_palette_find_by_shortcut("S");
    TEST_ASSERT_NOT_NULL(entry.name);
    TEST_ASSERT_EQUAL_STRING("Toggle Stars", entry.name);
}

/* ---- 27: cmd_palette_find_by_shortcut "Z" — not found ---- */

void test_find_by_shortcut_z(void)
{
    cmd_entry_t entry = cmd_palette_find_by_shortcut("Z");
    TEST_ASSERT_NULL(entry.name);
}

/* ---- 28: cmd_palette_find_by_shortcut NULL — not found ---- */

void test_find_by_shortcut_null(void)
{
    cmd_entry_t entry = cmd_palette_find_by_shortcut(NULL);
    TEST_ASSERT_NULL(entry.name);
}

/* ---- 29: cmd_format_entry — returns > 0 ---- */

void test_format_entry_returns_positive(void)
{
    cmd_entry_t entry = cmd_palette_find("Pause");
    char buf[256];
    int written = cmd_format_entry(&entry, buf, 256);
    TEST_ASSERT_GREATER_THAN(0, written);
}

/* ---- 30: cmd_format_entry — contains name and description ---- */

void test_format_entry_contains_name_and_desc(void)
{
    cmd_entry_t entry = cmd_palette_find("Pause");
    char buf[256];
    cmd_format_entry(&entry, buf, 256);
    TEST_ASSERT_NOT_NULL(strstr(buf, "Pause"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Pause or resume time flow"));
}

/* ---- 31: cmd_format_entry with tiny buffer — handles gracefully ---- */

void test_format_entry_tiny_buffer(void)
{
    cmd_entry_t entry = cmd_palette_find("Pause");
    char buf[4];
    int written = cmd_format_entry(&entry, buf, 4);
    /* Should not crash, should return >= 0 */
    TEST_ASSERT_GREATER_OR_EQUAL(0, written);
}

/* ---- 32: cmd_format_results — returns > 0 for non-empty results ---- */

void test_format_results_nonempty(void)
{
    cmd_result_t result = cmd_palette_search("pause");
    char buf[2048];
    int written = cmd_format_results(&result, buf, 2048);
    TEST_ASSERT_GREATER_THAN(0, written);
}

/* ---- 33: cmd_format_results — empty results returns 0 ---- */

void test_format_results_empty(void)
{
    cmd_result_t result = cmd_palette_search("zzzzz");
    char buf[256];
    int written = cmd_format_results(&result, buf, 256);
    TEST_ASSERT_EQUAL_INT(0, written);
}

/* ---- 34: All entries have non-NULL name ---- */

void test_all_entries_have_name(void)
{
    cmd_entry_t buf[64];
    int count = cmd_palette_all(buf, 64);
    for (int i = 0; i < count; i++) {
        TEST_ASSERT_NOT_NULL(buf[i].name);
        TEST_ASSERT_GREATER_THAN(0, (int)strlen(buf[i].name));
    }
}

/* ---- 35: All entries have non-NULL description ---- */

void test_all_entries_have_description(void)
{
    cmd_entry_t buf[64];
    int count = cmd_palette_all(buf, 64);
    for (int i = 0; i < count; i++) {
        TEST_ASSERT_NOT_NULL(buf[i].description);
        TEST_ASSERT_GREATER_THAN(0, (int)strlen(buf[i].description));
    }
}

/* ---- 36: All entries have valid category (< CMD_CAT_COUNT) ---- */

void test_all_entries_have_valid_category(void)
{
    cmd_entry_t buf[64];
    int count = cmd_palette_all(buf, 64);
    for (int i = 0; i < count; i++) {
        TEST_ASSERT_TRUE(buf[i].category >= CMD_CAT_TIME);
        TEST_ASSERT_TRUE(buf[i].category < CMD_CAT_COUNT);
    }
}

/* ---- 37: Search results: name match ranked before description match ---- */

void test_search_name_match_ranked_first(void)
{
    /* "Pause" appears in name of "Pause" and in description of "Stop" (Stop = 0x speed)
     * but "pause" is a substring of "Pause or resume time flow" (description).
     * The command named "Pause" should appear before any pure-description matches. */
    cmd_result_t result = cmd_palette_search("pause");
    TEST_ASSERT_GREATER_THAN(0, result.count);
    /* First result must be the one with "Pause" in its name */
    TEST_ASSERT_NOT_NULL(strstr(result.entries[0].name, "Pause"));
}

/* ---- main ---- */

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_palette_count_returns_correct_total);
    RUN_TEST(test_palette_all_large_buffer);
    RUN_TEST(test_palette_all_max_zero);
    RUN_TEST(test_palette_all_max_five);
    RUN_TEST(test_category_name_all_non_null);
    RUN_TEST(test_category_total_returns_seven);
    RUN_TEST(test_by_category_time_returns_eight);
    RUN_TEST(test_by_category_camera_returns_seven);
    RUN_TEST(test_by_category_layer_returns_five);
    RUN_TEST(test_by_category_system_returns_five);
    RUN_TEST(test_category_count_matches_by_category);
    RUN_TEST(test_search_empty_returns_all);
    RUN_TEST(test_search_null_returns_all);
    RUN_TEST(test_search_pause_lowercase);
    RUN_TEST(test_search_pause_uppercase);
    RUN_TEST(test_search_star_finds_toggle_stars);
    RUN_TEST(test_search_zoom_finds_camera_commands);
    RUN_TEST(test_search_nonsense_returns_zero);
    RUN_TEST(test_search_shift_finds_shift_shortcuts);
    RUN_TEST(test_search_a_finds_multiple);
    RUN_TEST(test_find_pause);
    RUN_TEST(test_find_toggle_stars);
    RUN_TEST(test_find_nonexistent);
    RUN_TEST(test_find_null);
    RUN_TEST(test_find_by_shortcut_space);
    RUN_TEST(test_find_by_shortcut_s);
    RUN_TEST(test_find_by_shortcut_z);
    RUN_TEST(test_find_by_shortcut_null);
    RUN_TEST(test_format_entry_returns_positive);
    RUN_TEST(test_format_entry_contains_name_and_desc);
    RUN_TEST(test_format_entry_tiny_buffer);
    RUN_TEST(test_format_results_nonempty);
    RUN_TEST(test_format_results_empty);
    RUN_TEST(test_all_entries_have_name);
    RUN_TEST(test_all_entries_have_description);
    RUN_TEST(test_all_entries_have_valid_category);
    RUN_TEST(test_search_name_match_ranked_first);
    return UNITY_END();
}
