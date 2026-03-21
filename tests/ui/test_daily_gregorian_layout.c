#include "../unity/unity.h"
#include "../../src/ui/daily_gregorian_layout.h"
#include <string.h>
#include <math.h>

void setUp(void) {}
void tearDown(void) {}

/* Test date: JD 2460755.5 = 2025-03-21 = Friday (Vernal Equinox) */
#define TEST_JD 2460755.5

/* --- Date extraction --- */

void test_date_string(void)
{
    daily_gregorian_layout_t layout = daily_gregorian_compute(TEST_JD);
    TEST_ASSERT_EQUAL_STRING("2025-03-21", layout.date_str);
}

void test_year(void)
{
    daily_gregorian_layout_t layout = daily_gregorian_compute(TEST_JD);
    TEST_ASSERT_EQUAL_INT(2025, layout.year);
}

void test_month(void)
{
    daily_gregorian_layout_t layout = daily_gregorian_compute(TEST_JD);
    TEST_ASSERT_EQUAL_INT(3, layout.month);
}

void test_day(void)
{
    daily_gregorian_layout_t layout = daily_gregorian_compute(TEST_JD);
    TEST_ASSERT_EQUAL_INT(21, layout.day);
}

/* --- Day of week --- */

void test_day_name_friday(void)
{
    daily_gregorian_layout_t layout = daily_gregorian_compute(TEST_JD);
    TEST_ASSERT_EQUAL_STRING("Friday", layout.day_name);
}

void test_day_name_saturday(void)
{
    /* JD 2460756.5 = 2025-03-22 = Saturday */
    daily_gregorian_layout_t layout = daily_gregorian_compute(2460756.5);
    TEST_ASSERT_EQUAL_STRING("Saturday", layout.day_name);
}

/* --- Month interpretation --- */

void test_month_name_march(void)
{
    daily_gregorian_layout_t layout = daily_gregorian_compute(TEST_JD);
    TEST_ASSERT_EQUAL_STRING("March", layout.month_name);
}

void test_month_origin(void)
{
    daily_gregorian_layout_t layout = daily_gregorian_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.month_origin);
    TEST_ASSERT_TRUE(strlen(layout.month_origin) > 0);
}

void test_month_quality(void)
{
    daily_gregorian_layout_t layout = daily_gregorian_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.month_quality);
    TEST_ASSERT_TRUE(strlen(layout.month_quality) > 0);
}

/* --- Day interpretation --- */

void test_day_planet_friday(void)
{
    /* Friday = Venus */
    daily_gregorian_layout_t layout = daily_gregorian_compute(TEST_JD);
    TEST_ASSERT_EQUAL_STRING("Venus", layout.day_planet);
}

void test_day_planet_monday(void)
{
    /* JD 2460752.5 = 2025-03-18 = Tuesday (Mars) */
    daily_gregorian_layout_t layout = daily_gregorian_compute(2460752.5);
    TEST_ASSERT_EQUAL_STRING("Mars", layout.day_planet);
}

void test_day_origin(void)
{
    daily_gregorian_layout_t layout = daily_gregorian_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.day_origin);
    TEST_ASSERT_TRUE(strlen(layout.day_origin) > 0);
}

/* --- Season --- */

void test_season_spring(void)
{
    /* March = Spring (season 0) */
    daily_gregorian_layout_t layout = daily_gregorian_compute(TEST_JD);
    TEST_ASSERT_EQUAL_INT(0, layout.season);
}

void test_season_name_spring(void)
{
    daily_gregorian_layout_t layout = daily_gregorian_compute(TEST_JD);
    TEST_ASSERT_EQUAL_STRING("Spring", layout.season_name);
}

void test_season_summer(void)
{
    /* JD 2460847.5 ~ 2025-06-21 = Summer (season 1) */
    daily_gregorian_layout_t layout = daily_gregorian_compute(2460847.5);
    TEST_ASSERT_EQUAL_INT(1, layout.season);
}

void test_season_autumn(void)
{
    /* JD 2460939.5 ~ 2025-09-21 = Autumn (season 2) */
    daily_gregorian_layout_t layout = daily_gregorian_compute(2460939.5);
    TEST_ASSERT_EQUAL_INT(2, layout.season);
}

void test_season_winter(void)
{
    /* JD 2460676.5 = 2025-01-01 = Winter (season 3) */
    daily_gregorian_layout_t layout = daily_gregorian_compute(2460676.5);
    TEST_ASSERT_EQUAL_INT(3, layout.season);
}

void test_season_theme(void)
{
    daily_gregorian_layout_t layout = daily_gregorian_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.season_theme);
    TEST_ASSERT_TRUE(strlen(layout.season_theme) > 0);
}

/* --- Slot positions --- */

void test_title_slot(void)
{
    daily_gregorian_layout_t layout = daily_gregorian_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.title_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.title_slot.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.title_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.title_slot.h);
}

void test_date_slot(void)
{
    daily_gregorian_layout_t layout = daily_gregorian_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.date_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.18f, layout.date_slot.y);
}

void test_day_slot(void)
{
    daily_gregorian_layout_t layout = daily_gregorian_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.day_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.32f, layout.day_slot.y);
}

void test_month_slot(void)
{
    daily_gregorian_layout_t layout = daily_gregorian_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.month_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.46f, layout.month_slot.y);
}

void test_season_slot(void)
{
    daily_gregorian_layout_t layout = daily_gregorian_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.season_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.60f, layout.season_slot.y);
}

void test_quality_slot(void)
{
    daily_gregorian_layout_t layout = daily_gregorian_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.quality_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.74f, layout.quality_slot.y);
}

void test_all_slots_same_width(void)
{
    daily_gregorian_layout_t layout = daily_gregorian_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.title_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.date_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.day_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.month_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.season_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.quality_slot.w);
}

/* --- Card frame --- */

void test_card_bounds(void)
{
    daily_gregorian_layout_t layout = daily_gregorian_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.card_x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.02f, layout.card_y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.90f, layout.card_w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.96f, layout.card_h);
}

/* --- Accessor --- */

void test_day_planet_accessor(void)
{
    daily_gregorian_layout_t layout = daily_gregorian_compute(TEST_JD);
    TEST_ASSERT_EQUAL_STRING("Venus", daily_gregorian_day_planet(&layout));
}

void test_day_planet_accessor_null(void)
{
    TEST_ASSERT_EQUAL_STRING("?", daily_gregorian_day_planet(NULL));
}

/* --- Purity --- */

void test_purity_same_input_same_output(void)
{
    daily_gregorian_layout_t a = daily_gregorian_compute(TEST_JD);
    daily_gregorian_layout_t b = daily_gregorian_compute(TEST_JD);
    TEST_ASSERT_EQUAL_INT(a.year, b.year);
    TEST_ASSERT_EQUAL_INT(a.month, b.month);
    TEST_ASSERT_EQUAL_INT(a.day, b.day);
    TEST_ASSERT_EQUAL_STRING(a.day_name, b.day_name);
    TEST_ASSERT_EQUAL_STRING(a.month_name, b.month_name);
    TEST_ASSERT_EQUAL_STRING(a.day_planet, b.day_planet);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, a.card_x, b.card_x);
}

void test_purity_different_dates(void)
{
    daily_gregorian_layout_t a = daily_gregorian_compute(2460755.5);
    daily_gregorian_layout_t b = daily_gregorian_compute(2460785.5); /* 30 days later */
    TEST_ASSERT_TRUE(a.day != b.day || a.month != b.month);
}

/* --- Different month test (January) --- */

void test_january(void)
{
    /* JD 2460676.5 = 2025-01-01 */
    daily_gregorian_layout_t layout = daily_gregorian_compute(2460676.5);
    TEST_ASSERT_EQUAL_STRING("January", layout.month_name);
    TEST_ASSERT_EQUAL_INT(1, layout.month);
    TEST_ASSERT_EQUAL_INT(3, layout.season); /* Winter */
}

int main(void)
{
    UNITY_BEGIN();
    /* Date extraction */
    RUN_TEST(test_date_string);
    RUN_TEST(test_year);
    RUN_TEST(test_month);
    RUN_TEST(test_day);
    /* Day of week */
    RUN_TEST(test_day_name_friday);
    RUN_TEST(test_day_name_saturday);
    /* Month interpretation */
    RUN_TEST(test_month_name_march);
    RUN_TEST(test_month_origin);
    RUN_TEST(test_month_quality);
    /* Day interpretation */
    RUN_TEST(test_day_planet_friday);
    RUN_TEST(test_day_planet_monday);
    RUN_TEST(test_day_origin);
    /* Season */
    RUN_TEST(test_season_spring);
    RUN_TEST(test_season_name_spring);
    RUN_TEST(test_season_summer);
    RUN_TEST(test_season_autumn);
    RUN_TEST(test_season_winter);
    RUN_TEST(test_season_theme);
    /* Slot positions */
    RUN_TEST(test_title_slot);
    RUN_TEST(test_date_slot);
    RUN_TEST(test_day_slot);
    RUN_TEST(test_month_slot);
    RUN_TEST(test_season_slot);
    RUN_TEST(test_quality_slot);
    RUN_TEST(test_all_slots_same_width);
    /* Card frame */
    RUN_TEST(test_card_bounds);
    /* Accessor */
    RUN_TEST(test_day_planet_accessor);
    RUN_TEST(test_day_planet_accessor_null);
    /* Purity */
    RUN_TEST(test_purity_same_input_same_output);
    RUN_TEST(test_purity_different_dates);
    /* Different month */
    RUN_TEST(test_january);
    return UNITY_END();
}
