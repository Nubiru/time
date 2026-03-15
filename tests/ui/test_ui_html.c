#include "../unity/unity.h"
#include "../../src/ui/ui_html.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ==================================================================
 * 1-5: ui_html_help — basic structure
 * ================================================================== */

void test_help_returns_positive(void)
{
    char buf[UI_HTML_BUF_SIZE];
    int n = ui_html_help(buf, UI_HTML_BUF_SIZE);
    TEST_ASSERT_TRUE(n > 0);
}

void test_help_contains_help_group_div(void)
{
    char buf[UI_HTML_BUF_SIZE];
    ui_html_help(buf, UI_HTML_BUF_SIZE);
    TEST_ASSERT_NOT_NULL(strstr(buf, "<div class=\"help-group\">"));
}

void test_help_contains_group_title(void)
{
    char buf[UI_HTML_BUF_SIZE];
    ui_html_help(buf, UI_HTML_BUF_SIZE);
    TEST_ASSERT_NOT_NULL(strstr(buf, "<div class=\"help-group-title\">"));
    /* First group should be "Time Controls" */
    TEST_ASSERT_NOT_NULL(strstr(buf, "Time Controls"));
}

void test_help_contains_help_entry(void)
{
    char buf[UI_HTML_BUF_SIZE];
    ui_html_help(buf, UI_HTML_BUF_SIZE);
    TEST_ASSERT_NOT_NULL(strstr(buf, "<div class=\"help-entry\">"));
}

void test_help_contains_kbd_and_action(void)
{
    char buf[UI_HTML_BUF_SIZE];
    ui_html_help(buf, UI_HTML_BUF_SIZE);
    TEST_ASSERT_NOT_NULL(strstr(buf, "<kbd>"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "<span class=\"help-action\">"));
}

/* ==================================================================
 * 6-7: ui_html_help — edge cases
 * ================================================================== */

void test_help_null_buf_returns_zero(void)
{
    int n = ui_html_help(NULL, UI_HTML_BUF_SIZE);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_help_small_buffer_no_crash(void)
{
    char buf[32];
    int n = ui_html_help(buf, 32);
    TEST_ASSERT_TRUE(n >= 0);
    TEST_ASSERT_TRUE(n < 32);
    TEST_ASSERT_TRUE(strlen(buf) < 32);
}

/* ==================================================================
 * 8-12: ui_html_cmd_results
 * ================================================================== */

void test_cmd_results_empty_returns_zero(void)
{
    cmd_result_t results;
    results.count = 0;
    char buf[1024];
    int n = ui_html_cmd_results(&results, buf, 1024);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_cmd_results_null_returns_zero(void)
{
    char buf[1024];
    int n = ui_html_cmd_results(NULL, buf, 1024);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_cmd_results_contains_cmd_item(void)
{
    cmd_result_t results;
    results.count = 1;
    results.entries[0].name = "Pause";
    results.entries[0].shortcut = "Space";
    results.entries[0].description = "Pause time";
    results.entries[0].category = CMD_CAT_TIME;

    char buf[1024];
    ui_html_cmd_results(&results, buf, 1024);
    TEST_ASSERT_NOT_NULL(strstr(buf, "<div class=\"cmd-item\">"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "<span class=\"cmd-item-name\">Pause</span>"));
}

void test_cmd_results_with_shortcut(void)
{
    cmd_result_t results;
    results.count = 1;
    results.entries[0].name = "Toggle Stars";
    results.entries[0].shortcut = "S";
    results.entries[0].description = "Toggle star layer";
    results.entries[0].category = CMD_CAT_LAYER;

    char buf[1024];
    ui_html_cmd_results(&results, buf, 1024);
    TEST_ASSERT_NOT_NULL(strstr(buf, "<kbd class=\"cmd-item-shortcut\">S</kbd>"));
}

void test_cmd_results_without_shortcut(void)
{
    cmd_result_t results;
    results.count = 1;
    results.entries[0].name = "Jump to Date";
    results.entries[0].shortcut = NULL;
    results.entries[0].description = "Navigate to a date";
    results.entries[0].category = CMD_CAT_NAVIGATE;

    char buf[1024];
    ui_html_cmd_results(&results, buf, 1024);
    /* Should NOT contain cmd-item-shortcut */
    TEST_ASSERT_NULL(strstr(buf, "cmd-item-shortcut"));
    /* But should still contain cmd-item-name */
    TEST_ASSERT_NOT_NULL(strstr(buf, "Jump to Date"));
}

/* ==================================================================
 * 13-17: ui_html_layers
 * ================================================================== */

void test_layers_returns_positive(void)
{
    char buf[UI_HTML_BUF_SIZE];
    int n = ui_html_layers(0xFFFFFFFF, buf, UI_HTML_BUF_SIZE);
    TEST_ASSERT_TRUE(n > 0);
}

void test_layers_contains_layer_group(void)
{
    char buf[UI_HTML_BUF_SIZE];
    ui_html_layers(0, buf, UI_HTML_BUF_SIZE);
    TEST_ASSERT_NOT_NULL(strstr(buf, "<div class=\"layer-group\">"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "<div class=\"layer-group-title\">"));
}

void test_layers_contains_layer_item(void)
{
    char buf[UI_HTML_BUF_SIZE];
    ui_html_layers(0, buf, UI_HTML_BUF_SIZE);
    TEST_ASSERT_NOT_NULL(strstr(buf, "<div class=\"layer-item\">"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "<span class=\"layer-name\">"));
}

void test_layers_active_dot(void)
{
    /* First item active (bit 0 set) */
    char buf[UI_HTML_BUF_SIZE];
    ui_html_layers(0x1, buf, UI_HTML_BUF_SIZE);
    TEST_ASSERT_NOT_NULL(strstr(buf, "layer-dot active"));
}

void test_layers_inactive_dot(void)
{
    /* No bits set -- all inactive */
    char buf[UI_HTML_BUF_SIZE];
    ui_html_layers(0x0, buf, UI_HTML_BUF_SIZE);
    /* Should have layer-dot but NOT layer-dot active */
    TEST_ASSERT_NOT_NULL(strstr(buf, "\"layer-dot\""));
    TEST_ASSERT_NULL(strstr(buf, "layer-dot active"));
}

/* ==================================================================
 * 18-19: ui_html_layers — edge cases
 * ================================================================== */

void test_layers_null_buf_returns_zero(void)
{
    int n = ui_html_layers(0, NULL, 1024);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_layers_small_buffer(void)
{
    char buf[16];
    int n = ui_html_layers(0, buf, 16);
    TEST_ASSERT_TRUE(n >= 0);
    TEST_ASSERT_TRUE(strlen(buf) < 16);
}

/* ==================================================================
 * 20-23: ui_html_toasts
 * ================================================================== */

void test_toasts_empty_queue_returns_zero(void)
{
    toast_queue_t queue = toast_queue_init();
    char buf[1024];
    int n = ui_html_toasts(&queue, buf, 1024);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_toasts_null_queue_returns_zero(void)
{
    char buf[1024];
    int n = ui_html_toasts(NULL, buf, 1024);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_toasts_info_level(void)
{
    toast_queue_t queue = toast_queue_init();
    queue = toast_push(queue, "Speed: 100x", TOAST_INFO, 2.0f);

    char buf[1024];
    int n = ui_html_toasts(&queue, buf, 1024);
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "toast-info"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Speed: 100x"));
}

void test_toasts_warning_level(void)
{
    toast_queue_t queue = toast_queue_init();
    queue = toast_push(queue, "Reverse mode", TOAST_WARNING, 3.0f);

    char buf[1024];
    ui_html_toasts(&queue, buf, 1024);
    TEST_ASSERT_NOT_NULL(strstr(buf, "toast-warning"));
}

/* ==================================================================
 * 24: ui_html_toasts — opacity in style
 * ================================================================== */

void test_toasts_opacity_in_style(void)
{
    toast_queue_t queue = toast_queue_init();
    queue = toast_push(queue, "Test", TOAST_SUCCESS, 2.0f);

    char buf[1024];
    ui_html_toasts(&queue, buf, 1024);
    TEST_ASSERT_NOT_NULL(strstr(buf, "opacity:"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "toast-success"));
}

/* ==================================================================
 * 25-29: ui_html_time_bar
 * ================================================================== */

void test_time_bar_paused_shows_play_icon(void)
{
    char buf[512];
    int n = ui_html_time_bar(1, "1x", "2026-03-15", buf, 512);
    TEST_ASSERT_TRUE(n > 0);
    /* Play triangle: &#9654; */
    TEST_ASSERT_NOT_NULL(strstr(buf, "&#9654;"));
}

void test_time_bar_playing_shows_pause_icon(void)
{
    char buf[512];
    ui_html_time_bar(0, "1x", "2026-03-15", buf, 512);
    /* Pause bars: &#9646;&#9646; */
    TEST_ASSERT_NOT_NULL(strstr(buf, "&#9646;&#9646;"));
}

void test_time_bar_contains_speed(void)
{
    char buf[512];
    ui_html_time_bar(1, "3600x", "2026-03-15", buf, 512);
    TEST_ASSERT_NOT_NULL(strstr(buf, "3600x"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "text-secondary"));
}

void test_time_bar_contains_date(void)
{
    char buf[512];
    ui_html_time_bar(0, "1x", "Mar 15, 2026", buf, 512);
    TEST_ASSERT_NOT_NULL(strstr(buf, "Mar 15, 2026"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "text-primary"));
}

void test_time_bar_contains_now_button(void)
{
    char buf[512];
    ui_html_time_bar(1, "1x", "2026-03-15", buf, 512);
    TEST_ASSERT_NOT_NULL(strstr(buf, "Now"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "text-accent"));
}

/* ==================================================================
 * 30-32: ui_html_time_bar — edge cases
 * ================================================================== */

void test_time_bar_null_buf_returns_zero(void)
{
    int n = ui_html_time_bar(1, "1x", "2026", NULL, 512);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_time_bar_null_speed_returns_zero(void)
{
    char buf[512];
    int n = ui_html_time_bar(1, NULL, "2026", buf, 512);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_time_bar_null_date_returns_zero(void)
{
    char buf[512];
    int n = ui_html_time_bar(0, "1x", NULL, buf, 512);
    TEST_ASSERT_EQUAL_INT(0, n);
}

/* ==================================================================
 * 33-35: Multiple items
 * ================================================================== */

void test_cmd_results_multiple_items(void)
{
    cmd_result_t results;
    results.count = 3;
    results.entries[0].name = "Alpha";
    results.entries[0].shortcut = "A";
    results.entries[0].description = "desc";
    results.entries[0].category = CMD_CAT_UI;
    results.entries[1].name = "Beta";
    results.entries[1].shortcut = NULL;
    results.entries[1].description = "desc";
    results.entries[1].category = CMD_CAT_UI;
    results.entries[2].name = "Gamma";
    results.entries[2].shortcut = "G";
    results.entries[2].description = "desc";
    results.entries[2].category = CMD_CAT_UI;

    char buf[2048];
    int n = ui_html_cmd_results(&results, buf, 2048);
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "Alpha"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Beta"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Gamma"));
}

void test_toasts_multiple_items(void)
{
    toast_queue_t queue = toast_queue_init();
    queue = toast_push(queue, "First", TOAST_INFO, 2.0f);
    queue = toast_push(queue, "Second", TOAST_SUCCESS, 2.0f);

    char buf[2048];
    int n = ui_html_toasts(&queue, buf, 2048);
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "First"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Second"));
}

void test_help_all_categories_present(void)
{
    char buf[UI_HTML_BUF_SIZE];
    ui_html_help(buf, UI_HTML_BUF_SIZE);

    for (int c = 0; c < HELP_CAT_COUNT; c++) {
        const char *name = help_category_name((help_category_t)c);
        TEST_ASSERT_NOT_NULL_MESSAGE(strstr(buf, name),
                                     "Missing category in HTML output");
    }
}

/* ==================================================================
 * 36: time_bar button class
 * ================================================================== */

void test_time_bar_tb_btn_class(void)
{
    char buf[512];
    ui_html_time_bar(1, "1x", "2026", buf, 512);
    TEST_ASSERT_NOT_NULL(strstr(buf, "class=\"tb-btn\""));
}

/* ==================================================================
 * 37: buf_size zero returns zero
 * ================================================================== */

void test_help_zero_buf_size_returns_zero(void)
{
    char buf[1];
    int n = ui_html_help(buf, 0);
    TEST_ASSERT_EQUAL_INT(0, n);
}

/* ---- Main runner ---- */

int main(void)
{
    UNITY_BEGIN();

    /* ui_html_help: 1-7 */
    RUN_TEST(test_help_returns_positive);
    RUN_TEST(test_help_contains_help_group_div);
    RUN_TEST(test_help_contains_group_title);
    RUN_TEST(test_help_contains_help_entry);
    RUN_TEST(test_help_contains_kbd_and_action);
    RUN_TEST(test_help_null_buf_returns_zero);
    RUN_TEST(test_help_small_buffer_no_crash);

    /* ui_html_cmd_results: 8-12 */
    RUN_TEST(test_cmd_results_empty_returns_zero);
    RUN_TEST(test_cmd_results_null_returns_zero);
    RUN_TEST(test_cmd_results_contains_cmd_item);
    RUN_TEST(test_cmd_results_with_shortcut);
    RUN_TEST(test_cmd_results_without_shortcut);

    /* ui_html_layers: 13-19 */
    RUN_TEST(test_layers_returns_positive);
    RUN_TEST(test_layers_contains_layer_group);
    RUN_TEST(test_layers_contains_layer_item);
    RUN_TEST(test_layers_active_dot);
    RUN_TEST(test_layers_inactive_dot);
    RUN_TEST(test_layers_null_buf_returns_zero);
    RUN_TEST(test_layers_small_buffer);

    /* ui_html_toasts: 20-24 */
    RUN_TEST(test_toasts_empty_queue_returns_zero);
    RUN_TEST(test_toasts_null_queue_returns_zero);
    RUN_TEST(test_toasts_info_level);
    RUN_TEST(test_toasts_warning_level);
    RUN_TEST(test_toasts_opacity_in_style);

    /* ui_html_time_bar: 25-32 */
    RUN_TEST(test_time_bar_paused_shows_play_icon);
    RUN_TEST(test_time_bar_playing_shows_pause_icon);
    RUN_TEST(test_time_bar_contains_speed);
    RUN_TEST(test_time_bar_contains_date);
    RUN_TEST(test_time_bar_contains_now_button);
    RUN_TEST(test_time_bar_null_buf_returns_zero);
    RUN_TEST(test_time_bar_null_speed_returns_zero);
    RUN_TEST(test_time_bar_null_date_returns_zero);

    /* Multiple items: 33-35 */
    RUN_TEST(test_cmd_results_multiple_items);
    RUN_TEST(test_toasts_multiple_items);
    RUN_TEST(test_help_all_categories_present);

    /* Additional: 36-37 */
    RUN_TEST(test_time_bar_tb_btn_class);
    RUN_TEST(test_help_zero_buf_size_returns_zero);

    return UNITY_END();
}
