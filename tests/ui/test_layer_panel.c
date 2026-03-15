#include "../unity/unity.h"
#include "../../src/ui/layer_panel.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ---- 1-6: lpanel_category_name — all 6 return non-NULL, non-empty ---- */

void test_category_name_astronomy(void)
{
    const char *n = lpanel_category_name(LPANEL_CAT_ASTRONOMY);
    TEST_ASSERT_NOT_NULL(n);
    TEST_ASSERT_GREATER_THAN(0, (int)strlen(n));
    TEST_ASSERT_EQUAL_STRING("Astronomy", n);
}

void test_category_name_astrology(void)
{
    const char *n = lpanel_category_name(LPANEL_CAT_ASTROLOGY);
    TEST_ASSERT_NOT_NULL(n);
    TEST_ASSERT_EQUAL_STRING("Astrology", n);
}

void test_category_name_calendar(void)
{
    const char *n = lpanel_category_name(LPANEL_CAT_CALENDAR);
    TEST_ASSERT_NOT_NULL(n);
    TEST_ASSERT_EQUAL_STRING("Calendar Systems", n);
}

void test_category_name_sacred(void)
{
    const char *n = lpanel_category_name(LPANEL_CAT_SACRED);
    TEST_ASSERT_NOT_NULL(n);
    TEST_ASSERT_EQUAL_STRING("Sacred Systems", n);
}

void test_category_name_earth(void)
{
    const char *n = lpanel_category_name(LPANEL_CAT_EARTH);
    TEST_ASSERT_NOT_NULL(n);
    TEST_ASSERT_EQUAL_STRING("Earth", n);
}

void test_category_name_display(void)
{
    const char *n = lpanel_category_name(LPANEL_CAT_DISPLAY);
    TEST_ASSERT_NOT_NULL(n);
    TEST_ASSERT_EQUAL_STRING("Display", n);
}

/* ---- 7: lpanel_category_count ---- */

void test_category_count_returns_six(void)
{
    TEST_ASSERT_EQUAL_INT(6, lpanel_category_count());
    TEST_ASSERT_EQUAL_INT(LPANEL_CAT_COUNT, lpanel_category_count());
}

/* ---- 8-13: lpanel_get_group per category — count > 0 ---- */

void test_get_group_astronomy_has_items(void)
{
    lpanel_group_t g = lpanel_get_group(LPANEL_CAT_ASTRONOMY);
    TEST_ASSERT_GREATER_THAN(0, g.count);
    TEST_ASSERT_EQUAL_INT(5, g.count); /* Stars, Galaxy, Orbits, Planets, Orbit Trails */
}

void test_get_group_astrology_has_items(void)
{
    lpanel_group_t g = lpanel_get_group(LPANEL_CAT_ASTROLOGY);
    TEST_ASSERT_GREATER_THAN(0, g.count);
    TEST_ASSERT_EQUAL_INT(4, g.count); /* Zodiac Ring, Aspects, Houses, Planetary Hours */
}

void test_get_group_calendar_has_items(void)
{
    lpanel_group_t g = lpanel_get_group(LPANEL_CAT_CALENDAR);
    TEST_ASSERT_GREATER_THAN(0, g.count);
    TEST_ASSERT_EQUAL_INT(5, g.count);
}

void test_get_group_sacred_has_items(void)
{
    lpanel_group_t g = lpanel_get_group(LPANEL_CAT_SACRED);
    TEST_ASSERT_GREATER_THAN(0, g.count);
    TEST_ASSERT_EQUAL_INT(4, g.count);
}

void test_get_group_earth_has_items(void)
{
    lpanel_group_t g = lpanel_get_group(LPANEL_CAT_EARTH);
    TEST_ASSERT_GREATER_THAN(0, g.count);
    TEST_ASSERT_EQUAL_INT(4, g.count);
}

void test_get_group_display_has_items(void)
{
    lpanel_group_t g = lpanel_get_group(LPANEL_CAT_DISPLAY);
    TEST_ASSERT_GREATER_THAN(0, g.count);
    TEST_ASSERT_EQUAL_INT(4, g.count);
}

/* ---- 14: lpanel_get_group — name matches category name ---- */

void test_get_group_name_matches_category(void)
{
    for (int i = 0; i < LPANEL_CAT_COUNT; i++) {
        lpanel_group_t g = lpanel_get_group((lpanel_category_t)i);
        const char *expected = lpanel_category_name((lpanel_category_t)i);
        TEST_ASSERT_EQUAL_STRING(expected, g.name);
    }
}

/* ---- 15-16: lpanel_get_items ---- */

void test_get_items_returns_correct_count(void)
{
    lpanel_item_t items[LPANEL_MAX_ITEMS_PER_GROUP];
    int count = lpanel_get_items(LPANEL_CAT_ASTRONOMY, items, LPANEL_MAX_ITEMS_PER_GROUP);
    TEST_ASSERT_EQUAL_INT(5, count);
    /* Verify first item */
    TEST_ASSERT_EQUAL_STRING("Stars", items[0].name);
}

void test_get_items_max_zero_returns_zero(void)
{
    int count = lpanel_get_items(LPANEL_CAT_ASTRONOMY, NULL, 0);
    TEST_ASSERT_EQUAL_INT(0, count);
}

/* ---- 17: lpanel_total_items ---- */

void test_total_items_is_sum_of_all(void)
{
    int expected = 0;
    for (int i = 0; i < LPANEL_CAT_COUNT; i++) {
        lpanel_group_t g = lpanel_get_group((lpanel_category_t)i);
        expected += g.count;
    }
    TEST_ASSERT_EQUAL_INT(expected, lpanel_total_items());
    /* 5 + 4 + 5 + 4 + 4 + 4 = 26 */
    TEST_ASSERT_EQUAL_INT(26, lpanel_total_items());
}

/* ---- 18-20: lpanel_find_item ---- */

void test_find_item_stars(void)
{
    lpanel_item_t item = lpanel_find_item("Stars");
    TEST_ASSERT_NOT_NULL(item.name);
    TEST_ASSERT_EQUAL_STRING("Stars", item.name);
    TEST_ASSERT_EQUAL_STRING("S", item.shortcut);
    TEST_ASSERT_EQUAL_INT(LPANEL_CAT_ASTRONOMY, item.category);
    TEST_ASSERT_EQUAL_INT(0, item.render_layer_id); /* LAYER_STARS */
    TEST_ASSERT_EQUAL_INT(1, item.default_on);
}

void test_find_item_tzolkin(void)
{
    lpanel_item_t item = lpanel_find_item("Tzolkin");
    TEST_ASSERT_NOT_NULL(item.name);
    TEST_ASSERT_EQUAL_STRING("Tzolkin", item.name);
    TEST_ASSERT_EQUAL_STRING("K", item.shortcut);
    TEST_ASSERT_EQUAL_INT(LPANEL_CAT_SACRED, item.category);
    TEST_ASSERT_EQUAL_INT(-1, item.render_layer_id);
    TEST_ASSERT_EQUAL_INT(1, item.default_on);
}

void test_find_item_nonexistent(void)
{
    lpanel_item_t item = lpanel_find_item("nonexistent");
    TEST_ASSERT_NULL(item.name);
}

/* ---- 21-23: lpanel_find_by_shortcut ---- */

void test_find_by_shortcut_s_is_stars(void)
{
    lpanel_item_t item = lpanel_find_by_shortcut("S");
    TEST_ASSERT_NOT_NULL(item.name);
    TEST_ASSERT_EQUAL_STRING("Stars", item.name);
}

void test_find_by_shortcut_h_is_hud(void)
{
    lpanel_item_t item = lpanel_find_by_shortcut("H");
    TEST_ASSERT_NOT_NULL(item.name);
    TEST_ASSERT_EQUAL_STRING("HUD", item.name);
}

void test_find_by_shortcut_z_not_found(void)
{
    lpanel_item_t item = lpanel_find_by_shortcut("Z");
    TEST_ASSERT_NULL(item.name);
}

/* ---- 24-25: lpanel_default_on_count ---- */

void test_default_on_count_astronomy(void)
{
    /* Stars, Galaxy, Orbits, Planets are on; Orbit Trails is off */
    TEST_ASSERT_EQUAL_INT(4, lpanel_default_on_count(LPANEL_CAT_ASTRONOMY));
}

void test_default_on_count_display(void)
{
    /* Labels, Cards, HUD are on; Grid is off */
    TEST_ASSERT_EQUAL_INT(3, lpanel_default_on_count(LPANEL_CAT_DISPLAY));
}

/* ---- 26-28: lpanel_format_group ---- */

void test_format_group_returns_positive(void)
{
    char buf[1024];
    int written = lpanel_format_group(LPANEL_CAT_ASTRONOMY, buf, sizeof(buf));
    TEST_ASSERT_GREATER_THAN(0, written);
}

void test_format_group_contains_header(void)
{
    char buf[1024];
    lpanel_format_group(LPANEL_CAT_ASTRONOMY, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Astronomy"));
}

void test_format_group_contains_items(void)
{
    char buf[1024];
    lpanel_format_group(LPANEL_CAT_ASTRONOMY, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Stars"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Galaxy"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Planets"));
}

/* ---- 29-30: lpanel_format_all ---- */

void test_format_all_returns_positive(void)
{
    char buf[4096];
    int written = lpanel_format_all(buf, sizeof(buf));
    TEST_ASSERT_GREATER_THAN(0, written);
}

void test_format_all_contains_all_categories(void)
{
    char buf[4096];
    lpanel_format_all(buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Astronomy"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Astrology"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Calendar Systems"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Sacred Systems"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Earth"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Display"));
}

/* ---- 31: All items have non-NULL name ---- */

void test_all_items_have_name(void)
{
    for (int i = 0; i < LPANEL_CAT_COUNT; i++) {
        lpanel_group_t g = lpanel_get_group((lpanel_category_t)i);
        for (int j = 0; j < g.count; j++) {
            TEST_ASSERT_NOT_NULL(g.items[j].name);
            TEST_ASSERT_GREATER_THAN(0, (int)strlen(g.items[j].name));
        }
    }
}

/* ---- 32: All items have category matching their group ---- */

void test_all_items_category_matches_group(void)
{
    for (int i = 0; i < LPANEL_CAT_COUNT; i++) {
        lpanel_group_t g = lpanel_get_group((lpanel_category_t)i);
        for (int j = 0; j < g.count; j++) {
            TEST_ASSERT_EQUAL_INT(i, g.items[j].category);
        }
    }
}

/* ---- 33: Stars has render_layer_id 0 (LAYER_STARS) ---- */

void test_stars_render_layer_id(void)
{
    lpanel_item_t item = lpanel_find_item("Stars");
    TEST_ASSERT_EQUAL_INT(0, item.render_layer_id);
}

/* ---- 34: Tzolkin has render_layer_id -1 ---- */

void test_tzolkin_render_layer_id(void)
{
    lpanel_item_t item = lpanel_find_item("Tzolkin");
    TEST_ASSERT_EQUAL_INT(-1, item.render_layer_id);
}

/* ---- 35: HUD has render_layer_id 7 (LAYER_HUD) ---- */

void test_hud_render_layer_id(void)
{
    lpanel_item_t item = lpanel_find_item("HUD");
    TEST_ASSERT_EQUAL_INT(7, item.render_layer_id);
}

/* ---- 36: Format with buf_size=1 handles gracefully ---- */

void test_format_group_tiny_buffer(void)
{
    char buf[1];
    int written = lpanel_format_group(LPANEL_CAT_ASTRONOMY, buf, 1);
    TEST_ASSERT_EQUAL_INT(0, written);
}

/* ---- 37: Items with shortcuts — verify all expected shortcuts exist ---- */

void test_expected_shortcuts_exist(void)
{
    /* S=Stars, T=Orbit Trails, C=Chinese, K=Tzolkin, I=I Ching,
     * D=Human Design, L=Labels, H=HUD, G=Grid */
    const char *expected[] = {"S", "T", "C", "K", "I", "D", "L", "H", "G"};
    int count = (int)(sizeof(expected) / sizeof(expected[0]));
    for (int i = 0; i < count; i++) {
        lpanel_item_t item = lpanel_find_by_shortcut(expected[i]);
        TEST_ASSERT_NOT_NULL_MESSAGE(item.name, expected[i]);
    }
}

/* ---- 38: Format shows checkbox state ---- */

void test_format_group_shows_checkbox_on(void)
{
    char buf[1024];
    lpanel_format_group(LPANEL_CAT_ASTRONOMY, buf, sizeof(buf));
    /* Stars is default_on, should show [x] */
    TEST_ASSERT_NOT_NULL(strstr(buf, "[x]"));
}

void test_format_group_shows_checkbox_off(void)
{
    char buf[1024];
    lpanel_format_group(LPANEL_CAT_ASTRONOMY, buf, sizeof(buf));
    /* Orbit Trails is default_off, should show [ ] */
    TEST_ASSERT_NOT_NULL(strstr(buf, "[ ]"));
}

/* ---- 40: Zodiac Ring render layer ---- */

void test_zodiac_ring_render_layer_id(void)
{
    lpanel_item_t item = lpanel_find_item("Zodiac Ring");
    TEST_ASSERT_NOT_NULL(item.name);
    TEST_ASSERT_EQUAL_INT(4, item.render_layer_id); /* LAYER_ZODIAC_RING */
}

/* ---- 41: Labels render layer ---- */

void test_labels_render_layer_id(void)
{
    lpanel_item_t item = lpanel_find_item("Labels");
    TEST_ASSERT_NOT_NULL(item.name);
    TEST_ASSERT_EQUAL_INT(5, item.render_layer_id); /* LAYER_LABELS */
}

/* ---- 42: Cards render layer ---- */

void test_cards_render_layer_id(void)
{
    lpanel_item_t item = lpanel_find_item("Cards");
    TEST_ASSERT_NOT_NULL(item.name);
    TEST_ASSERT_EQUAL_INT(6, item.render_layer_id); /* LAYER_CARDS */
}

/* ---- 43: Default on count for calendar (only Gregorian and Chinese) ---- */

void test_default_on_count_calendar(void)
{
    TEST_ASSERT_EQUAL_INT(2, lpanel_default_on_count(LPANEL_CAT_CALENDAR));
}

/* ---- 44: Default on count for sacred (Tzolkin and I Ching) ---- */

void test_default_on_count_sacred(void)
{
    TEST_ASSERT_EQUAL_INT(2, lpanel_default_on_count(LPANEL_CAT_SACRED));
}

/* ---- 45: Default on count for earth (all off) ---- */

void test_default_on_count_earth(void)
{
    TEST_ASSERT_EQUAL_INT(0, lpanel_default_on_count(LPANEL_CAT_EARTH));
}

/* ---- 46: get_items clamps to max ---- */

void test_get_items_clamps_to_max(void)
{
    lpanel_item_t items[2];
    int count = lpanel_get_items(LPANEL_CAT_ASTRONOMY, items, 2);
    TEST_ASSERT_EQUAL_INT(2, count);
    TEST_ASSERT_EQUAL_STRING("Stars", items[0].name);
    TEST_ASSERT_EQUAL_STRING("Galaxy", items[1].name);
}

/* ---- 47: category_name with invalid returns NULL ---- */

void test_category_name_invalid_returns_null(void)
{
    const char *n = lpanel_category_name(LPANEL_CAT_COUNT);
    TEST_ASSERT_NULL(n);
    n = lpanel_category_name((lpanel_category_t)99);
    TEST_ASSERT_NULL(n);
}

/* ---- 48: find_item with NULL returns not-found ---- */

void test_find_item_null_returns_not_found(void)
{
    lpanel_item_t item = lpanel_find_item(NULL);
    TEST_ASSERT_NULL(item.name);
}

/* ---- 49: find_by_shortcut with NULL returns not-found ---- */

void test_find_by_shortcut_null_returns_not_found(void)
{
    lpanel_item_t item = lpanel_find_by_shortcut(NULL);
    TEST_ASSERT_NULL(item.name);
}

/* ---- 50: group category field matches ---- */

void test_group_category_field_matches(void)
{
    for (int i = 0; i < LPANEL_CAT_COUNT; i++) {
        lpanel_group_t g = lpanel_get_group((lpanel_category_t)i);
        TEST_ASSERT_EQUAL_INT(i, g.category);
    }
}

/* ---- 51: format_all tiny buffer ---- */

void test_format_all_tiny_buffer(void)
{
    char buf[1];
    int written = lpanel_format_all(buf, 1);
    TEST_ASSERT_EQUAL_INT(0, written);
}

/* ---- 52: Galaxy has no shortcut ---- */

void test_galaxy_has_no_shortcut(void)
{
    lpanel_item_t item = lpanel_find_item("Galaxy");
    TEST_ASSERT_NOT_NULL(item.name);
    TEST_ASSERT_NULL(item.shortcut);
}

/* ---- main ---- */

int main(void)
{
    UNITY_BEGIN();

    /* 1-6: category names */
    RUN_TEST(test_category_name_astronomy);
    RUN_TEST(test_category_name_astrology);
    RUN_TEST(test_category_name_calendar);
    RUN_TEST(test_category_name_sacred);
    RUN_TEST(test_category_name_earth);
    RUN_TEST(test_category_name_display);

    /* 7: category count */
    RUN_TEST(test_category_count_returns_six);

    /* 8-13: get_group per category */
    RUN_TEST(test_get_group_astronomy_has_items);
    RUN_TEST(test_get_group_astrology_has_items);
    RUN_TEST(test_get_group_calendar_has_items);
    RUN_TEST(test_get_group_sacred_has_items);
    RUN_TEST(test_get_group_earth_has_items);
    RUN_TEST(test_get_group_display_has_items);

    /* 14: group name matches */
    RUN_TEST(test_get_group_name_matches_category);

    /* 15-16: get_items */
    RUN_TEST(test_get_items_returns_correct_count);
    RUN_TEST(test_get_items_max_zero_returns_zero);

    /* 17: total items */
    RUN_TEST(test_total_items_is_sum_of_all);

    /* 18-20: find_item */
    RUN_TEST(test_find_item_stars);
    RUN_TEST(test_find_item_tzolkin);
    RUN_TEST(test_find_item_nonexistent);

    /* 21-23: find_by_shortcut */
    RUN_TEST(test_find_by_shortcut_s_is_stars);
    RUN_TEST(test_find_by_shortcut_h_is_hud);
    RUN_TEST(test_find_by_shortcut_z_not_found);

    /* 24-25: default_on_count */
    RUN_TEST(test_default_on_count_astronomy);
    RUN_TEST(test_default_on_count_display);

    /* 26-28: format_group */
    RUN_TEST(test_format_group_returns_positive);
    RUN_TEST(test_format_group_contains_header);
    RUN_TEST(test_format_group_contains_items);

    /* 29-30: format_all */
    RUN_TEST(test_format_all_returns_positive);
    RUN_TEST(test_format_all_contains_all_categories);

    /* 31-32: all items integrity */
    RUN_TEST(test_all_items_have_name);
    RUN_TEST(test_all_items_category_matches_group);

    /* 33-35: render layer IDs */
    RUN_TEST(test_stars_render_layer_id);
    RUN_TEST(test_tzolkin_render_layer_id);
    RUN_TEST(test_hud_render_layer_id);

    /* 36: tiny buffer */
    RUN_TEST(test_format_group_tiny_buffer);

    /* 37: all shortcuts */
    RUN_TEST(test_expected_shortcuts_exist);

    /* 38-39: checkbox format */
    RUN_TEST(test_format_group_shows_checkbox_on);
    RUN_TEST(test_format_group_shows_checkbox_off);

    /* 40-42: more render layer IDs */
    RUN_TEST(test_zodiac_ring_render_layer_id);
    RUN_TEST(test_labels_render_layer_id);
    RUN_TEST(test_cards_render_layer_id);

    /* 43-45: more default_on_count */
    RUN_TEST(test_default_on_count_calendar);
    RUN_TEST(test_default_on_count_sacred);
    RUN_TEST(test_default_on_count_earth);

    /* 46: get_items clamp */
    RUN_TEST(test_get_items_clamps_to_max);

    /* 47: invalid category */
    RUN_TEST(test_category_name_invalid_returns_null);

    /* 48-49: NULL args */
    RUN_TEST(test_find_item_null_returns_not_found);
    RUN_TEST(test_find_by_shortcut_null_returns_not_found);

    /* 50: group category field */
    RUN_TEST(test_group_category_field_matches);

    /* 51: format_all tiny buffer */
    RUN_TEST(test_format_all_tiny_buffer);

    /* 52: Galaxy no shortcut */
    RUN_TEST(test_galaxy_has_no_shortcut);

    return UNITY_END();
}
