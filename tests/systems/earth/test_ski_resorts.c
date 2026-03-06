/* test_ski_resorts.c — Tests for world ski resort database module. */

#include "../../unity/unity.h"
#include "../../../src/systems/earth/ski_resorts.h"

#include <string.h>

void setUp(void) { }
void tearDown(void) { }

/* --- Count --- */

void test_count_is_50(void) {
    TEST_ASSERT_EQUAL_INT(50, ski_resort_count());
}

void test_count_matches_define(void) {
    TEST_ASSERT_EQUAL_INT(SKI_RESORT_COUNT, ski_resort_count());
}

/* --- Get: Whistler (index 0) --- */

void test_get_whistler_id(void) {
    ski_resort_t r = ski_resort_get(0);
    TEST_ASSERT_EQUAL_INT(0, r.id);
}

void test_get_whistler_name(void) {
    ski_resort_t r = ski_resort_get(0);
    TEST_ASSERT_EQUAL_STRING("Whistler Blackcomb", r.name);
}

void test_get_whistler_country(void) {
    ski_resort_t r = ski_resort_get(0);
    TEST_ASSERT_EQUAL_STRING("Canada", r.country);
}

void test_get_whistler_coords(void) {
    ski_resort_t r = ski_resort_get(0);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 50.116f, (float)r.lat);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, -122.957f, (float)r.lon);
}

void test_get_whistler_elevation(void) {
    ski_resort_t r = ski_resort_get(0);
    TEST_ASSERT_EQUAL_INT(675, r.base_elev_m);
    TEST_ASSERT_EQUAL_INT(2284, r.summit_elev_m);
}

void test_get_whistler_season(void) {
    ski_resort_t r = ski_resort_get(0);
    TEST_ASSERT_EQUAL_INT(11, r.season_start);
    TEST_ASSERT_EQUAL_INT(5, r.season_end);
}

void test_get_whistler_description(void) {
    ski_resort_t r = ski_resort_get(0);
    TEST_ASSERT_NOT_NULL(r.description);
    TEST_ASSERT_TRUE(strlen(r.description) > 0);
}

/* --- Get: specific resorts --- */

void test_get_chamonix(void) {
    ski_resort_t r = ski_resort_get(13);
    TEST_ASSERT_EQUAL_STRING("Chamonix Mont-Blanc", r.name);
    TEST_ASSERT_EQUAL_STRING("France", r.country);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 45.924f, (float)r.lat);
}

void test_get_niseko(void) {
    ski_resort_t r = ski_resort_get(25);
    TEST_ASSERT_EQUAL_STRING("Niseko", r.name);
    TEST_ASSERT_EQUAL_STRING("Japan", r.country);
}

void test_get_portillo(void) {
    ski_resort_t r = ski_resort_get(29);
    TEST_ASSERT_EQUAL_STRING("Portillo", r.name);
    TEST_ASSERT_EQUAL_STRING("Chile", r.country);
    TEST_ASSERT_EQUAL_INT(6, r.season_start);
    TEST_ASSERT_EQUAL_INT(10, r.season_end);
}

void test_get_sierra_nevada_last(void) {
    ski_resort_t r = ski_resort_get(49);
    TEST_ASSERT_EQUAL_INT(49, r.id);
    TEST_ASSERT_EQUAL_STRING("Sierra Nevada", r.name);
    TEST_ASSERT_EQUAL_STRING("Spain", r.country);
}

/* --- Get: out of bounds --- */

void test_get_negative_returns_invalid(void) {
    ski_resort_t r = ski_resort_get(-1);
    TEST_ASSERT_EQUAL_INT(-1, r.id);
}

void test_get_over_max_returns_invalid(void) {
    ski_resort_t r = ski_resort_get(50);
    TEST_ASSERT_EQUAL_INT(-1, r.id);
}

/* --- Name --- */

void test_name_whistler(void) {
    TEST_ASSERT_EQUAL_STRING("Whistler Blackcomb", ski_resort_name(0));
}

void test_name_last(void) {
    TEST_ASSERT_EQUAL_STRING("Sierra Nevada", ski_resort_name(49));
}

void test_name_invalid(void) {
    TEST_ASSERT_NULL(ski_resort_name(-1));
    TEST_ASSERT_NULL(ski_resort_name(50));
}

/* --- Vertical --- */

void test_vertical_whistler(void) {
    /* 2284 - 675 = 1609 */
    TEST_ASSERT_EQUAL_INT(1609, ski_resort_vertical(0));
}

void test_vertical_revelstoke(void) {
    /* 2225 - 512 = 1713 */
    TEST_ASSERT_EQUAL_INT(1713, ski_resort_vertical(10));
}

void test_vertical_invalid(void) {
    TEST_ASSERT_EQUAL_INT(0, ski_resort_vertical(-1));
}

/* --- Nearest (Haversine) --- */

void test_nearest_to_whistler(void) {
    int idx = ski_resort_nearest(50.1, -123.0);
    TEST_ASSERT_EQUAL_INT(0, idx);
}

void test_nearest_to_chamonix(void) {
    int idx = ski_resort_nearest(45.9, 6.9);
    TEST_ASSERT_EQUAL_INT(13, idx);
}

void test_nearest_to_niseko(void) {
    int idx = ski_resort_nearest(42.9, 140.7);
    TEST_ASSERT_EQUAL_INT(25, idx);
}

void test_nearest_to_portillo(void) {
    int idx = ski_resort_nearest(-32.8, -70.1);
    TEST_ASSERT_EQUAL_INT(29, idx);
}

/* --- In season --- */

void test_in_season_whistler_january(void) {
    /* Whistler: Nov-May, Jan should be in season */
    TEST_ASSERT_TRUE(ski_resort_in_season(0, 1));
}

void test_in_season_whistler_july(void) {
    /* Whistler: Nov-May, Jul should be off season */
    TEST_ASSERT_FALSE(ski_resort_in_season(0, 7));
}

void test_in_season_portillo_august(void) {
    /* Portillo: Jun-Oct (southern hemisphere), Aug in season */
    TEST_ASSERT_TRUE(ski_resort_in_season(29, 8));
}

void test_in_season_portillo_january(void) {
    /* Portillo: Jun-Oct, Jan off season */
    TEST_ASSERT_FALSE(ski_resort_in_season(29, 1));
}

void test_in_season_invalid_index(void) {
    TEST_ASSERT_FALSE(ski_resort_in_season(-1, 1));
}

void test_in_season_invalid_month(void) {
    TEST_ASSERT_FALSE(ski_resort_in_season(0, 0));
    TEST_ASSERT_FALSE(ski_resort_in_season(0, 13));
}

/* --- Snow quality --- */

void test_snow_quality_peak_season(void) {
    /* Whistler Nov-May, peak around Feb. Jan should be high quality */
    float q = (float)ski_resort_snow_quality(0, 1);
    TEST_ASSERT_TRUE(q > 0.5f);
}

void test_snow_quality_off_season_zero(void) {
    /* Whistler, Aug = off season */
    float q = (float)ski_resort_snow_quality(0, 8);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, q);
}

void test_snow_quality_southern_hemisphere(void) {
    /* Portillo Jun-Oct, Aug is peak */
    float q = (float)ski_resort_snow_quality(29, 8);
    TEST_ASSERT_TRUE(q > 0.5f);
}

void test_snow_quality_invalid_index(void) {
    float q = (float)ski_resort_snow_quality(-1, 1);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, q);
}

void test_snow_quality_invalid_month(void) {
    float q = (float)ski_resort_snow_quality(0, 0);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, q);
}

void test_snow_quality_clamped(void) {
    for (int m = 1; m <= 12; m++) {
        float q = (float)ski_resort_snow_quality(0, m);
        TEST_ASSERT_TRUE(q >= 0.0f && q <= 1.0f);
    }
}

/* --- Region search --- */

void test_region_alps(void) {
    /* Alps region: lat 45-48, lon 6-13 */
    int indices[SKI_RESORT_COUNT];
    int n = ski_resorts_in_region(45.0, 48.0, 6.0, 13.0,
                                  indices, SKI_RESORT_COUNT);
    /* Should include Chamonix, Val d'Isere, Courchevel, Verbier, Zermatt, St. Anton, Kitzbuhel, Lech, Cortina, Val Gardena, Andermatt */
    TEST_ASSERT_TRUE(n >= 8);
}

void test_region_japan(void) {
    /* Japan: lat 35-44, lon 135-143 */
    int indices[SKI_RESORT_COUNT];
    int n = ski_resorts_in_region(35.0, 44.0, 135.0, 143.0,
                                  indices, SKI_RESORT_COUNT);
    /* Niseko, Hakuba, Furano, Myoko, Shiga Kogen */
    TEST_ASSERT_TRUE(n >= 4);
}

void test_region_empty(void) {
    /* Middle of Pacific */
    int indices[SKI_RESORT_COUNT];
    int n = ski_resorts_in_region(0.0, 1.0, -170.0, -160.0,
                                  indices, SKI_RESORT_COUNT);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_region_limited_output(void) {
    int indices[2];
    int n = ski_resorts_in_region(-90.0, 90.0, -180.0, 180.0,
                                  indices, 2);
    TEST_ASSERT_EQUAL_INT(2, n);
}

/* --- Data integrity --- */

void test_all_have_names(void) {
    for (int i = 0; i < SKI_RESORT_COUNT; i++) {
        ski_resort_t r = ski_resort_get(i);
        TEST_ASSERT_NOT_NULL(r.name);
        TEST_ASSERT_TRUE(strlen(r.name) > 0);
    }
}

void test_all_have_countries(void) {
    for (int i = 0; i < SKI_RESORT_COUNT; i++) {
        ski_resort_t r = ski_resort_get(i);
        TEST_ASSERT_NOT_NULL(r.country);
        TEST_ASSERT_TRUE(strlen(r.country) > 0);
    }
}

void test_all_valid_coords(void) {
    for (int i = 0; i < SKI_RESORT_COUNT; i++) {
        ski_resort_t r = ski_resort_get(i);
        TEST_ASSERT_TRUE(r.lat >= -90.0 && r.lat <= 90.0);
        TEST_ASSERT_TRUE(r.lon >= -180.0 && r.lon <= 180.0);
    }
}

void test_all_summit_above_base(void) {
    for (int i = 0; i < SKI_RESORT_COUNT; i++) {
        ski_resort_t r = ski_resort_get(i);
        TEST_ASSERT_TRUE(r.summit_elev_m > r.base_elev_m);
    }
}

void test_all_valid_seasons(void) {
    for (int i = 0; i < SKI_RESORT_COUNT; i++) {
        ski_resort_t r = ski_resort_get(i);
        TEST_ASSERT_TRUE(r.season_start >= 1 && r.season_start <= 12);
        TEST_ASSERT_TRUE(r.season_end >= 1 && r.season_end <= 12);
    }
}

void test_all_have_descriptions(void) {
    for (int i = 0; i < SKI_RESORT_COUNT; i++) {
        ski_resort_t r = ski_resort_get(i);
        TEST_ASSERT_NOT_NULL(r.description);
        TEST_ASSERT_TRUE(strlen(r.description) > 0);
    }
}

/* --- Runner --- */

int main(void) {
    UNITY_BEGIN();

    /* Count */
    RUN_TEST(test_count_is_50);
    RUN_TEST(test_count_matches_define);

    /* Get: Whistler */
    RUN_TEST(test_get_whistler_id);
    RUN_TEST(test_get_whistler_name);
    RUN_TEST(test_get_whistler_country);
    RUN_TEST(test_get_whistler_coords);
    RUN_TEST(test_get_whistler_elevation);
    RUN_TEST(test_get_whistler_season);
    RUN_TEST(test_get_whistler_description);

    /* Get: specific */
    RUN_TEST(test_get_chamonix);
    RUN_TEST(test_get_niseko);
    RUN_TEST(test_get_portillo);
    RUN_TEST(test_get_sierra_nevada_last);

    /* Get: bounds */
    RUN_TEST(test_get_negative_returns_invalid);
    RUN_TEST(test_get_over_max_returns_invalid);

    /* Name */
    RUN_TEST(test_name_whistler);
    RUN_TEST(test_name_last);
    RUN_TEST(test_name_invalid);

    /* Vertical */
    RUN_TEST(test_vertical_whistler);
    RUN_TEST(test_vertical_revelstoke);
    RUN_TEST(test_vertical_invalid);

    /* Nearest */
    RUN_TEST(test_nearest_to_whistler);
    RUN_TEST(test_nearest_to_chamonix);
    RUN_TEST(test_nearest_to_niseko);
    RUN_TEST(test_nearest_to_portillo);

    /* In season */
    RUN_TEST(test_in_season_whistler_january);
    RUN_TEST(test_in_season_whistler_july);
    RUN_TEST(test_in_season_portillo_august);
    RUN_TEST(test_in_season_portillo_january);
    RUN_TEST(test_in_season_invalid_index);
    RUN_TEST(test_in_season_invalid_month);

    /* Snow quality */
    RUN_TEST(test_snow_quality_peak_season);
    RUN_TEST(test_snow_quality_off_season_zero);
    RUN_TEST(test_snow_quality_southern_hemisphere);
    RUN_TEST(test_snow_quality_invalid_index);
    RUN_TEST(test_snow_quality_invalid_month);
    RUN_TEST(test_snow_quality_clamped);

    /* Region */
    RUN_TEST(test_region_alps);
    RUN_TEST(test_region_japan);
    RUN_TEST(test_region_empty);
    RUN_TEST(test_region_limited_output);

    /* Data integrity */
    RUN_TEST(test_all_have_names);
    RUN_TEST(test_all_have_countries);
    RUN_TEST(test_all_valid_coords);
    RUN_TEST(test_all_summit_above_base);
    RUN_TEST(test_all_valid_seasons);
    RUN_TEST(test_all_have_descriptions);

    return UNITY_END();
}
