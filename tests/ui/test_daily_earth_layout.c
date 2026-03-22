#include "../unity/unity.h"
#include "../../src/ui/daily_earth_layout.h"
#include <string.h>
#include <math.h>

void setUp(void) {}
void tearDown(void) {}

/* Test location: Madrid, Spain (40.4N, 3.7W), Spring equinox noon */
#define TEST_JD    2460755.5  /* 2025-03-21 noon UT */
#define TEST_LAT   40.4
#define TEST_LON  -3.7

/* --- Daylight data --- */

void test_is_day_at_noon(void)
{
    daily_earth_layout_t layout = daily_earth_compute(TEST_JD, TEST_LAT, TEST_LON);
    /* At noon in Madrid, it should be daytime */
    TEST_ASSERT_EQUAL_INT(1, layout.is_day);
}

void test_sun_elevation_positive_at_noon(void)
{
    daily_earth_layout_t layout = daily_earth_compute(TEST_JD, TEST_LAT, TEST_LON);
    /* Sun should be above horizon at noon */
    TEST_ASSERT_TRUE(layout.sun_elevation_deg > 0.0);
}

void test_day_length_reasonable(void)
{
    daily_earth_layout_t layout = daily_earth_compute(TEST_JD, TEST_LAT, TEST_LON);
    /* Near equinox, day length ~12 hours */
    TEST_ASSERT_FLOAT_WITHIN(2.0, 12.0, layout.day_length_hours);
}

void test_not_polar_day(void)
{
    daily_earth_layout_t layout = daily_earth_compute(TEST_JD, TEST_LAT, TEST_LON);
    TEST_ASSERT_EQUAL_INT(0, layout.polar_day);
}

void test_not_polar_night(void)
{
    daily_earth_layout_t layout = daily_earth_compute(TEST_JD, TEST_LAT, TEST_LON);
    TEST_ASSERT_EQUAL_INT(0, layout.polar_night);
}

/* --- Polar regions --- */

void test_arctic_summer_polar_day(void)
{
    /* JD 2460847.5 ~ 2025-06-21, 70N latitude */
    daily_earth_layout_t layout = daily_earth_compute(2460847.5, 70.0, 0.0);
    TEST_ASSERT_EQUAL_INT(1, layout.polar_day);
}

void test_arctic_winter_polar_night(void)
{
    /* JD 2461031.5 ~ 2025-12-22, 70N latitude */
    daily_earth_layout_t layout = daily_earth_compute(2461031.5, 70.0, 0.0);
    TEST_ASSERT_EQUAL_INT(1, layout.polar_night);
}

/* --- Season data --- */

void test_season_spring(void)
{
    /* March 21, 40.4N should be spring */
    daily_earth_layout_t layout = daily_earth_compute(TEST_JD, TEST_LAT, TEST_LON);
    TEST_ASSERT_EQUAL_INT(0, layout.season); /* SEASON_SPRING = 0 */
}

void test_season_name_spring(void)
{
    daily_earth_layout_t layout = daily_earth_compute(TEST_JD, TEST_LAT, TEST_LON);
    TEST_ASSERT_EQUAL_STRING("Spring", layout.season_name);
}

void test_season_progress_range(void)
{
    daily_earth_layout_t layout = daily_earth_compute(TEST_JD, TEST_LAT, TEST_LON);
    TEST_ASSERT_TRUE(layout.season_progress >= 0.0);
    TEST_ASSERT_TRUE(layout.season_progress <= 1.0);
}

void test_southern_hemisphere_autumn(void)
{
    /* March 21, 34S should be autumn */
    daily_earth_layout_t layout = daily_earth_compute(TEST_JD, -34.0, TEST_LON);
    TEST_ASSERT_EQUAL_INT(2, layout.season); /* SEASON_AUTUMN = 2 */
}

void test_summer_season(void)
{
    /* JD 2460847.5 ~ 2025-06-21, 40.4N */
    daily_earth_layout_t layout = daily_earth_compute(2460847.5, TEST_LAT, TEST_LON);
    TEST_ASSERT_EQUAL_INT(1, layout.season); /* SEASON_SUMMER = 1 */
}

/* --- Slot positions --- */

void test_title_slot(void)
{
    daily_earth_layout_t layout = daily_earth_compute(TEST_JD, TEST_LAT, TEST_LON);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.title_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.title_slot.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.title_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.title_slot.h);
}

void test_daylight_slot(void)
{
    daily_earth_layout_t layout = daily_earth_compute(TEST_JD, TEST_LAT, TEST_LON);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.daylight_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.18f, layout.daylight_slot.y);
}

void test_sun_slot(void)
{
    daily_earth_layout_t layout = daily_earth_compute(TEST_JD, TEST_LAT, TEST_LON);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.sun_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.32f, layout.sun_slot.y);
}

void test_season_slot(void)
{
    daily_earth_layout_t layout = daily_earth_compute(TEST_JD, TEST_LAT, TEST_LON);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.season_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.46f, layout.season_slot.y);
}

void test_polar_slot(void)
{
    daily_earth_layout_t layout = daily_earth_compute(TEST_JD, TEST_LAT, TEST_LON);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.polar_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.60f, layout.polar_slot.y);
}

void test_length_slot(void)
{
    daily_earth_layout_t layout = daily_earth_compute(TEST_JD, TEST_LAT, TEST_LON);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.length_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.74f, layout.length_slot.y);
}

void test_all_slots_same_width(void)
{
    daily_earth_layout_t layout = daily_earth_compute(TEST_JD, TEST_LAT, TEST_LON);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.title_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.daylight_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.sun_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.season_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.polar_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.length_slot.w);
}

/* --- Card frame --- */

void test_card_bounds(void)
{
    daily_earth_layout_t layout = daily_earth_compute(TEST_JD, TEST_LAT, TEST_LON);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.card_x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.02f, layout.card_y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.90f, layout.card_w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.96f, layout.card_h);
}

/* --- Accessor --- */

void test_is_day_accessor(void)
{
    daily_earth_layout_t layout = daily_earth_compute(TEST_JD, TEST_LAT, TEST_LON);
    TEST_ASSERT_EQUAL_INT(1, daily_earth_is_day(&layout));
}

void test_is_day_accessor_null(void)
{
    TEST_ASSERT_EQUAL_INT(0, daily_earth_is_day(NULL));
}

/* --- Interpretation --- */

void test_glance_not_empty(void)
{
    daily_earth_layout_t layout = daily_earth_compute(TEST_JD, TEST_LAT, TEST_LON);
    TEST_ASSERT_TRUE(layout.glance[0] != '\0');
}

void test_detail_not_empty(void)
{
    daily_earth_layout_t layout = daily_earth_compute(TEST_JD, TEST_LAT, TEST_LON);
    TEST_ASSERT_TRUE(layout.detail[0] != '\0');
}

/* --- Purity --- */

void test_purity_same_input_same_output(void)
{
    daily_earth_layout_t a = daily_earth_compute(TEST_JD, TEST_LAT, TEST_LON);
    daily_earth_layout_t b = daily_earth_compute(TEST_JD, TEST_LAT, TEST_LON);
    TEST_ASSERT_EQUAL_INT(a.is_day, b.is_day);
    TEST_ASSERT_FLOAT_WITHIN(0.001, a.sun_elevation_deg, b.sun_elevation_deg);
    TEST_ASSERT_FLOAT_WITHIN(0.001, a.day_length_hours, b.day_length_hours);
    TEST_ASSERT_EQUAL_INT(a.season, b.season);
    TEST_ASSERT_EQUAL_STRING(a.season_name, b.season_name);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, a.card_x, b.card_x);
}

void test_purity_different_locations(void)
{
    daily_earth_layout_t a = daily_earth_compute(TEST_JD, 40.0, 0.0);
    daily_earth_layout_t b = daily_earth_compute(TEST_JD, -40.0, 0.0);
    /* Northern and southern should have opposite seasons */
    TEST_ASSERT_TRUE(a.season != b.season);
}

int main(void)
{
    UNITY_BEGIN();
    /* Daylight data */
    RUN_TEST(test_is_day_at_noon);
    RUN_TEST(test_sun_elevation_positive_at_noon);
    RUN_TEST(test_day_length_reasonable);
    RUN_TEST(test_not_polar_day);
    RUN_TEST(test_not_polar_night);
    /* Polar regions */
    RUN_TEST(test_arctic_summer_polar_day);
    RUN_TEST(test_arctic_winter_polar_night);
    /* Season data */
    RUN_TEST(test_season_spring);
    RUN_TEST(test_season_name_spring);
    RUN_TEST(test_season_progress_range);
    RUN_TEST(test_southern_hemisphere_autumn);
    RUN_TEST(test_summer_season);
    /* Slot positions */
    RUN_TEST(test_title_slot);
    RUN_TEST(test_daylight_slot);
    RUN_TEST(test_sun_slot);
    RUN_TEST(test_season_slot);
    RUN_TEST(test_polar_slot);
    RUN_TEST(test_length_slot);
    RUN_TEST(test_all_slots_same_width);
    /* Card frame */
    RUN_TEST(test_card_bounds);
    /* Accessor */
    RUN_TEST(test_is_day_accessor);
    RUN_TEST(test_is_day_accessor_null);
    /* Interpretation */
    RUN_TEST(test_glance_not_empty);
    RUN_TEST(test_detail_not_empty);
    /* Purity */
    RUN_TEST(test_purity_same_input_same_output);
    RUN_TEST(test_purity_different_locations);
    return UNITY_END();
}
