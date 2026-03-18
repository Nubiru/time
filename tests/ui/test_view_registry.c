#include "../unity/unity.h"
#include "../../src/ui/view_registry.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ---- 1-3: view_count / view_implemented_count ---- */

void test_view_count_equals_enum(void)
{
    TEST_ASSERT_EQUAL_INT(VIEW_COUNT, view_count());
    TEST_ASSERT_EQUAL_INT(7, view_count());
}

void test_view_implemented_count_is_three(void)
{
    TEST_ASSERT_EQUAL_INT(3, view_implemented_count());
}

void test_view_count_positive(void)
{
    TEST_ASSERT_TRUE(view_count() > 0);
}

/* ---- 4-10: view_get returns correct data for each view ---- */

void test_view_get_space(void)
{
    view_config_t v = view_get(VIEW_SPACE);
    TEST_ASSERT_EQUAL_INT(VIEW_SPACE, v.id);
    TEST_ASSERT_EQUAL_STRING("Space View", v.name);
    TEST_ASSERT_EQUAL_STRING("Space", v.short_name);
    TEST_ASSERT_EQUAL_STRING("Shift+3", v.shortcut);
    TEST_ASSERT_EQUAL_INT(2, v.scale_min);
    TEST_ASSERT_EQUAL_INT(4, v.scale_max);
    TEST_ASSERT_EQUAL_INT(3, v.default_scale);
    TEST_ASSERT_EQUAL_INT(0xFD, v.layers_enabled);
    TEST_ASSERT_EQUAL_INT(1, v.implemented);
}

void test_view_get_earth(void)
{
    view_config_t v = view_get(VIEW_EARTH);
    TEST_ASSERT_EQUAL_INT(VIEW_EARTH, v.id);
    TEST_ASSERT_EQUAL_STRING("Earth View", v.name);
    TEST_ASSERT_EQUAL_STRING("Earth", v.short_name);
    TEST_ASSERT_EQUAL_STRING("E", v.shortcut);
    TEST_ASSERT_EQUAL_INT(0, v.scale_min);
    TEST_ASSERT_EQUAL_INT(1, v.scale_max);
    TEST_ASSERT_EQUAL_INT(1, v.default_scale);
    TEST_ASSERT_EQUAL_INT(0xA9, v.layers_enabled);
    TEST_ASSERT_EQUAL_INT(1, v.implemented);
}

void test_view_get_galaxy(void)
{
    view_config_t v = view_get(VIEW_GALAXY);
    TEST_ASSERT_EQUAL_INT(VIEW_GALAXY, v.id);
    TEST_ASSERT_EQUAL_STRING("Galaxy View", v.name);
    TEST_ASSERT_EQUAL_STRING("Galaxy", v.short_name);
    TEST_ASSERT_EQUAL_STRING("Shift+5", v.shortcut);
    TEST_ASSERT_EQUAL_INT(5, v.scale_min);
    TEST_ASSERT_EQUAL_INT(6, v.scale_max);
    TEST_ASSERT_EQUAL_INT(5, v.default_scale);
    TEST_ASSERT_EQUAL_INT(0x83, v.layers_enabled);
    TEST_ASSERT_EQUAL_INT(1, v.implemented);
}

void test_view_get_personal(void)
{
    view_config_t v = view_get(VIEW_PERSONAL);
    TEST_ASSERT_EQUAL_INT(VIEW_PERSONAL, v.id);
    TEST_ASSERT_EQUAL_STRING("Personal View", v.name);
    TEST_ASSERT_EQUAL_STRING("Personal", v.short_name);
    TEST_ASSERT_EQUAL_STRING("P", v.shortcut);
    TEST_ASSERT_EQUAL_INT(0, v.scale_min);
    TEST_ASSERT_EQUAL_INT(0, v.scale_max);
    TEST_ASSERT_EQUAL_INT(0, v.default_scale);
    TEST_ASSERT_EQUAL_INT(0xF1, v.layers_enabled);
    TEST_ASSERT_EQUAL_INT(0, v.implemented);
}

void test_view_get_deep_time(void)
{
    view_config_t v = view_get(VIEW_DEEP_TIME);
    TEST_ASSERT_EQUAL_INT(VIEW_DEEP_TIME, v.id);
    TEST_ASSERT_EQUAL_STRING("Deep Time View", v.name);
    TEST_ASSERT_EQUAL_STRING("Deep Time", v.short_name);
    TEST_ASSERT_EQUAL_STRING("G", v.shortcut);
    TEST_ASSERT_EQUAL_INT(6, v.scale_min);
    TEST_ASSERT_EQUAL_INT(6, v.scale_max);
    TEST_ASSERT_EQUAL_INT(6, v.default_scale);
    TEST_ASSERT_EQUAL_INT(0x83, v.layers_enabled);
    TEST_ASSERT_EQUAL_INT(0, v.implemented);
}

void test_view_get_city(void)
{
    view_config_t v = view_get(VIEW_CITY);
    TEST_ASSERT_EQUAL_INT(VIEW_CITY, v.id);
    TEST_ASSERT_EQUAL_STRING("City View", v.name);
    TEST_ASSERT_EQUAL_STRING("City", v.short_name);
    TEST_ASSERT_EQUAL_STRING("C", v.shortcut);
    TEST_ASSERT_EQUAL_INT(0, v.scale_min);
    TEST_ASSERT_EQUAL_INT(0, v.scale_max);
    TEST_ASSERT_EQUAL_INT(0, v.default_scale);
    TEST_ASSERT_EQUAL_INT(0xA9, v.layers_enabled);
    TEST_ASSERT_EQUAL_INT(0, v.implemented);
}

void test_view_get_room(void)
{
    view_config_t v = view_get(VIEW_ROOM);
    TEST_ASSERT_EQUAL_INT(VIEW_ROOM, v.id);
    TEST_ASSERT_EQUAL_STRING("Room View", v.name);
    TEST_ASSERT_EQUAL_STRING("Room", v.short_name);
    TEST_ASSERT_EQUAL_STRING("—", v.shortcut);
    TEST_ASSERT_EQUAL_INT(-1, v.scale_min);
    TEST_ASSERT_EQUAL_INT(-1, v.scale_max);
    TEST_ASSERT_EQUAL_INT(-1, v.default_scale);
    TEST_ASSERT_EQUAL_INT(0xC0, v.layers_enabled);
    TEST_ASSERT_EQUAL_INT(0, v.implemented);
}

/* ---- 11-12: view_get out of range ---- */

void test_view_get_negative_id(void)
{
    view_config_t v = view_get((view_id_t)-1);
    TEST_ASSERT_EQUAL_INT(-1, (int)v.id);
    TEST_ASSERT_EQUAL_STRING("Unknown", v.name);
    TEST_ASSERT_EQUAL_STRING("Unknown", v.short_name);
    TEST_ASSERT_EQUAL_INT(0, v.implemented);
}

void test_view_get_beyond_count(void)
{
    view_config_t v = view_get(VIEW_COUNT);
    TEST_ASSERT_EQUAL_INT(-1, (int)v.id);
    TEST_ASSERT_EQUAL_STRING("Unknown", v.name);
}

/* ---- 13-14: view_name ---- */

void test_view_name_space(void)
{
    TEST_ASSERT_EQUAL_STRING("Space View", view_name(VIEW_SPACE));
}

void test_view_name_out_of_range(void)
{
    TEST_ASSERT_EQUAL_STRING("Unknown", view_name(VIEW_COUNT));
    TEST_ASSERT_EQUAL_STRING("Unknown", view_name((view_id_t)-1));
}

/* ---- 15-19: view_for_scale ---- */

void test_view_for_scale_zero(void)
{
    /* Scale 0 = SCALE_PERSONAL -> falls in Earth View (0-1) */
    TEST_ASSERT_EQUAL_INT(VIEW_EARTH, view_for_scale(0));
}

void test_view_for_scale_one(void)
{
    TEST_ASSERT_EQUAL_INT(VIEW_EARTH, view_for_scale(1));
}

void test_view_for_scale_three(void)
{
    /* Scale 3 = SCALE_SOLAR -> falls in Space View (2-4) */
    TEST_ASSERT_EQUAL_INT(VIEW_SPACE, view_for_scale(3));
}

void test_view_for_scale_five(void)
{
    /* Scale 5 = SCALE_GALAXY -> falls in Galaxy View (5-6) */
    TEST_ASSERT_EQUAL_INT(VIEW_GALAXY, view_for_scale(5));
}

void test_view_for_scale_out_of_range(void)
{
    /* Scale -2 is below any view -> return VIEW_SPACE as fallback */
    TEST_ASSERT_EQUAL_INT(VIEW_SPACE, view_for_scale(-2));
    /* Scale 99 is above any view -> return VIEW_SPACE as fallback */
    TEST_ASSERT_EQUAL_INT(VIEW_SPACE, view_for_scale(99));
}

/* ---- 20-24: view_from_name ---- */

void test_view_from_name_full_name(void)
{
    TEST_ASSERT_EQUAL_INT(VIEW_SPACE, view_from_name("Space View"));
    TEST_ASSERT_EQUAL_INT(VIEW_EARTH, view_from_name("Earth View"));
    TEST_ASSERT_EQUAL_INT(VIEW_GALAXY, view_from_name("Galaxy View"));
}

void test_view_from_name_short_name(void)
{
    TEST_ASSERT_EQUAL_INT(VIEW_SPACE, view_from_name("Space"));
    TEST_ASSERT_EQUAL_INT(VIEW_EARTH, view_from_name("Earth"));
    TEST_ASSERT_EQUAL_INT(VIEW_ROOM, view_from_name("Room"));
}

void test_view_from_name_case_insensitive(void)
{
    TEST_ASSERT_EQUAL_INT(VIEW_SPACE, view_from_name("space view"));
    TEST_ASSERT_EQUAL_INT(VIEW_EARTH, view_from_name("EARTH"));
    TEST_ASSERT_EQUAL_INT(VIEW_GALAXY, view_from_name("galaxy"));
}

void test_view_from_name_null(void)
{
    TEST_ASSERT_EQUAL_INT(-1, (int)view_from_name(NULL));
}

void test_view_from_name_empty_and_no_match(void)
{
    TEST_ASSERT_EQUAL_INT(-1, (int)view_from_name(""));
    TEST_ASSERT_EQUAL_INT(-1, (int)view_from_name("Nonexistent"));
    TEST_ASSERT_EQUAL_INT(-1, (int)view_from_name("Space V"));
}

/* ---- 25-27: default_scale within range ---- */

void test_default_scale_within_range_all_views(void)
{
    for (int i = 0; i < VIEW_COUNT; i++) {
        view_config_t v = view_get((view_id_t)i);
        TEST_ASSERT_TRUE(v.default_scale >= v.scale_min);
        TEST_ASSERT_TRUE(v.default_scale <= v.scale_max);
    }
}

void test_all_views_have_non_empty_name(void)
{
    for (int i = 0; i < VIEW_COUNT; i++) {
        view_config_t v = view_get((view_id_t)i);
        TEST_ASSERT_NOT_NULL(v.name);
        TEST_ASSERT_TRUE(v.name[0] != '\0');
        TEST_ASSERT_NOT_NULL(v.short_name);
        TEST_ASSERT_TRUE(v.short_name[0] != '\0');
    }
}

void test_all_views_have_description(void)
{
    for (int i = 0; i < VIEW_COUNT; i++) {
        view_config_t v = view_get((view_id_t)i);
        TEST_ASSERT_NOT_NULL(v.description);
        TEST_ASSERT_TRUE(v.description[0] != '\0');
    }
}

/* ---- 28-30: layer bitmask validation ---- */

void test_space_layers_include_planets_and_orbits(void)
{
    view_config_t v = view_get(VIEW_SPACE);
    /* Planets = bit 3, Orbits = bit 2 */
    TEST_ASSERT_TRUE(v.layers_enabled & (1 << 3));
    TEST_ASSERT_TRUE(v.layers_enabled & (1 << 2));
}

void test_galaxy_layers_exclude_orbits(void)
{
    view_config_t v = view_get(VIEW_GALAXY);
    /* Orbits = bit 2 should NOT be set */
    TEST_ASSERT_FALSE(v.layers_enabled & (1 << 2));
}

void test_room_layers_cards_and_hud_only(void)
{
    view_config_t v = view_get(VIEW_ROOM);
    /* Cards = bit 6, HUD = bit 7, nothing else */
    TEST_ASSERT_EQUAL_INT(0xC0, v.layers_enabled);
}

/* ---- 31-33: implemented views ---- */

void test_space_is_implemented(void)
{
    view_config_t v = view_get(VIEW_SPACE);
    TEST_ASSERT_EQUAL_INT(1, v.implemented);
}

void test_personal_is_not_implemented(void)
{
    view_config_t v = view_get(VIEW_PERSONAL);
    TEST_ASSERT_EQUAL_INT(0, v.implemented);
}

void test_implemented_count_matches_individual_checks(void)
{
    int count = 0;
    for (int i = 0; i < VIEW_COUNT; i++) {
        view_config_t v = view_get((view_id_t)i);
        if (v.implemented) count++;
    }
    TEST_ASSERT_EQUAL_INT(view_implemented_count(), count);
}

/* ---- 34-35: view_for_scale edge values ---- */

void test_view_for_scale_two(void)
{
    /* Scale 2 = SCALE_INNER -> Space View (2-4) */
    TEST_ASSERT_EQUAL_INT(VIEW_SPACE, view_for_scale(2));
}

void test_view_for_scale_six(void)
{
    /* Scale 6 = SCALE_UNIVERSE -> Galaxy View (5-6) */
    TEST_ASSERT_EQUAL_INT(VIEW_GALAXY, view_for_scale(6));
}

/* ---- Main runner ---- */

int main(void)
{
    UNITY_BEGIN();

    /* Count: 1-3 */
    RUN_TEST(test_view_count_equals_enum);
    RUN_TEST(test_view_implemented_count_is_three);
    RUN_TEST(test_view_count_positive);

    /* view_get all views: 4-10 */
    RUN_TEST(test_view_get_space);
    RUN_TEST(test_view_get_earth);
    RUN_TEST(test_view_get_galaxy);
    RUN_TEST(test_view_get_personal);
    RUN_TEST(test_view_get_deep_time);
    RUN_TEST(test_view_get_city);
    RUN_TEST(test_view_get_room);

    /* view_get out of range: 11-12 */
    RUN_TEST(test_view_get_negative_id);
    RUN_TEST(test_view_get_beyond_count);

    /* view_name: 13-14 */
    RUN_TEST(test_view_name_space);
    RUN_TEST(test_view_name_out_of_range);

    /* view_for_scale: 15-19 */
    RUN_TEST(test_view_for_scale_zero);
    RUN_TEST(test_view_for_scale_one);
    RUN_TEST(test_view_for_scale_three);
    RUN_TEST(test_view_for_scale_five);
    RUN_TEST(test_view_for_scale_out_of_range);

    /* view_from_name: 20-24 */
    RUN_TEST(test_view_from_name_full_name);
    RUN_TEST(test_view_from_name_short_name);
    RUN_TEST(test_view_from_name_case_insensitive);
    RUN_TEST(test_view_from_name_null);
    RUN_TEST(test_view_from_name_empty_and_no_match);

    /* Validation: 25-27 */
    RUN_TEST(test_default_scale_within_range_all_views);
    RUN_TEST(test_all_views_have_non_empty_name);
    RUN_TEST(test_all_views_have_description);

    /* Layer bitmasks: 28-30 */
    RUN_TEST(test_space_layers_include_planets_and_orbits);
    RUN_TEST(test_galaxy_layers_exclude_orbits);
    RUN_TEST(test_room_layers_cards_and_hud_only);

    /* Implemented: 31-33 */
    RUN_TEST(test_space_is_implemented);
    RUN_TEST(test_personal_is_not_implemented);
    RUN_TEST(test_implemented_count_matches_individual_checks);

    /* Scale edge values: 34-35 */
    RUN_TEST(test_view_for_scale_two);
    RUN_TEST(test_view_for_scale_six);

    return UNITY_END();
}
