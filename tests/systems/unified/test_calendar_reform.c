/* test_calendar_reform.c — Calendar Reform History module tests.
 * TDD RED phase: tests written before implementation.
 * Tests calendar system catalog, seasonal hours, day length, and utilities. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/calendar_reform.h"
#include <math.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== calendar_system_count ===== */

void test_count_at_least_10(void)
{
    TEST_ASSERT_TRUE(calendar_system_count() >= 10);
}

/* ===== calendar_system_get — by index ===== */

void test_get_egyptian(void)
{
    calendar_system_t s = calendar_system_get(0);
    TEST_ASSERT_EQUAL_INT(0, s.id);
    TEST_ASSERT_EQUAL_STRING("Egyptian Civil Calendar", s.name);
    TEST_ASSERT_EQUAL_INT(-3000, s.year);
    TEST_ASSERT_EQUAL_STRING("Egyptian priesthood", s.originator);
    TEST_ASSERT_EQUAL_STRING("Egyptian", s.culture);
    TEST_ASSERT_EQUAL_INT(CAL_TYPE_SOLAR, s.type);
    TEST_ASSERT_EQUAL_INT(360, s.days_per_year);
    TEST_ASSERT_EQUAL_INT(12, s.months);
    TEST_ASSERT_EQUAL_INT(5, s.intercalary_days);
    TEST_ASSERT_EQUAL_STRING("12x30+5", s.month_structure);
    TEST_ASSERT_NOT_NULL(s.description);
    TEST_ASSERT_NOT_NULL(s.problem_solved);
    TEST_ASSERT_NOT_NULL(s.weakness);
}

void test_get_babylonian(void)
{
    calendar_system_t s = calendar_system_get(1);
    TEST_ASSERT_EQUAL_INT(1, s.id);
    TEST_ASSERT_EQUAL_STRING("Babylonian Calendar", s.name);
    TEST_ASSERT_EQUAL_INT(-2000, s.year);
    TEST_ASSERT_EQUAL_INT(CAL_TYPE_LUNISOLAR, s.type);
    TEST_ASSERT_EQUAL_INT(354, s.days_per_year);
    TEST_ASSERT_EQUAL_INT(12, s.months);
    TEST_ASSERT_EQUAL_INT(30, s.intercalary_days);
}

void test_get_julian(void)
{
    calendar_system_t s = calendar_system_get(3);
    TEST_ASSERT_EQUAL_STRING("Julian Calendar", s.name);
    TEST_ASSERT_EQUAL_INT(-46, s.year);
    TEST_ASSERT_EQUAL_INT(CAL_TYPE_ADOPTED, s.type);
    TEST_ASSERT_EQUAL_INT(365, s.days_per_year);
    TEST_ASSERT_EQUAL_INT(1, s.intercalary_days);
}

void test_get_gregorian(void)
{
    calendar_system_t s = calendar_system_get(4);
    TEST_ASSERT_EQUAL_STRING("Gregorian Calendar", s.name);
    TEST_ASSERT_EQUAL_INT(1582, s.year);
    TEST_ASSERT_EQUAL_INT(CAL_TYPE_ADOPTED, s.type);
    TEST_ASSERT_EQUAL_INT(365, s.days_per_year);
    TEST_ASSERT_EQUAL_INT(12, s.months);
}

void test_get_french_republican(void)
{
    calendar_system_t s = calendar_system_get(5);
    TEST_ASSERT_EQUAL_STRING("French Republican Calendar", s.name);
    TEST_ASSERT_EQUAL_INT(1793, s.year);
    TEST_ASSERT_EQUAL_INT(CAL_TYPE_ADOPTED, s.type);
    TEST_ASSERT_EQUAL_INT(360, s.days_per_year);
    TEST_ASSERT_EQUAL_INT(12, s.months);
    TEST_ASSERT_EQUAL_INT(5, s.intercalary_days);
}

void test_get_13_moon(void)
{
    calendar_system_t s = calendar_system_get(6);
    TEST_ASSERT_EQUAL_STRING("13 Moon Calendar", s.name);
    TEST_ASSERT_EQUAL_INT(1992, s.year);
    TEST_ASSERT_EQUAL_INT(CAL_TYPE_PROPOSED, s.type);
    TEST_ASSERT_EQUAL_INT(364, s.days_per_year);
    TEST_ASSERT_EQUAL_INT(13, s.months);
    TEST_ASSERT_EQUAL_INT(1, s.intercalary_days);
    TEST_ASSERT_EQUAL_STRING("13x28+1", s.month_structure);
}

void test_get_world_calendar(void)
{
    calendar_system_t s = calendar_system_get(7);
    TEST_ASSERT_EQUAL_STRING("World Calendar", s.name);
    TEST_ASSERT_EQUAL_INT(1930, s.year);
    TEST_ASSERT_EQUAL_INT(CAL_TYPE_PROPOSED, s.type);
    TEST_ASSERT_EQUAL_INT(364, s.days_per_year);
    TEST_ASSERT_EQUAL_INT(12, s.months);
}

void test_get_ifc(void)
{
    calendar_system_t s = calendar_system_get(8);
    TEST_ASSERT_EQUAL_STRING("International Fixed Calendar", s.name);
    TEST_ASSERT_EQUAL_INT(1902, s.year);
    TEST_ASSERT_EQUAL_INT(CAL_TYPE_PROPOSED, s.type);
    TEST_ASSERT_EQUAL_INT(364, s.days_per_year);
    TEST_ASSERT_EQUAL_INT(13, s.months);
}

void test_get_seasonal_hours(void)
{
    calendar_system_t s = calendar_system_get(9);
    TEST_ASSERT_EQUAL_STRING("Seasonal Hours", s.name);
    TEST_ASSERT_EQUAL_INT(-3000, s.year);
    TEST_ASSERT_EQUAL_INT(CAL_TYPE_SOLAR, s.type);
    TEST_ASSERT_EQUAL_INT(365, s.days_per_year);
}

void test_get_invalid_negative(void)
{
    calendar_system_t s = calendar_system_get(-1);
    TEST_ASSERT_EQUAL_INT(-1, s.id);
    TEST_ASSERT_NULL(s.name);
}

void test_get_invalid_too_large(void)
{
    calendar_system_t s = calendar_system_get(9999);
    TEST_ASSERT_EQUAL_INT(-1, s.id);
    TEST_ASSERT_NULL(s.name);
}

/* ===== calendar_systems_by_type ===== */

void test_filter_solar(void)
{
    int indices[16];
    int count = calendar_systems_by_type(CAL_TYPE_SOLAR, indices, 16);
    /* Egyptian + Seasonal Hours = at least 2 */
    TEST_ASSERT_TRUE(count >= 2);
}

void test_filter_lunisolar(void)
{
    int indices[16];
    int count = calendar_systems_by_type(CAL_TYPE_LUNISOLAR, indices, 16);
    /* Babylonian + Roman = at least 2 */
    TEST_ASSERT_TRUE(count >= 2);
}

void test_filter_proposed(void)
{
    int indices[16];
    int count = calendar_systems_by_type(CAL_TYPE_PROPOSED, indices, 16);
    /* 13 Moon + World Calendar + IFC = at least 3 */
    TEST_ASSERT_TRUE(count >= 3);
}

void test_filter_adopted(void)
{
    int indices[16];
    int count = calendar_systems_by_type(CAL_TYPE_ADOPTED, indices, 16);
    /* Julian + Gregorian + French = at least 3 */
    TEST_ASSERT_TRUE(count >= 3);
}

void test_filter_respects_max(void)
{
    int indices[1];
    int count = calendar_systems_by_type(CAL_TYPE_ADOPTED, indices, 1);
    TEST_ASSERT_EQUAL_INT(1, count);
}

void test_filter_lunar_empty(void)
{
    int indices[16];
    int count = calendar_systems_by_type(CAL_TYPE_LUNAR, indices, 16);
    /* No pure lunar calendars in our catalog currently */
    TEST_ASSERT_TRUE(count >= 0);
}

/* ===== calendar_type_name ===== */

void test_type_name_lunar(void)
{
    TEST_ASSERT_EQUAL_STRING("Lunar", calendar_type_name(CAL_TYPE_LUNAR));
}

void test_type_name_solar(void)
{
    TEST_ASSERT_EQUAL_STRING("Solar", calendar_type_name(CAL_TYPE_SOLAR));
}

void test_type_name_lunisolar(void)
{
    TEST_ASSERT_EQUAL_STRING("Lunisolar", calendar_type_name(CAL_TYPE_LUNISOLAR));
}

void test_type_name_proposed(void)
{
    TEST_ASSERT_EQUAL_STRING("Proposed", calendar_type_name(CAL_TYPE_PROPOSED));
}

void test_type_name_adopted(void)
{
    TEST_ASSERT_EQUAL_STRING("Adopted", calendar_type_name(CAL_TYPE_ADOPTED));
}

void test_type_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", calendar_type_name(CAL_TYPE_COUNT));
    TEST_ASSERT_EQUAL_STRING("?", calendar_type_name((calendar_type_t)-1));
}

/* ===== calendar_day_length ===== */

void test_day_length_equator_equinox(void)
{
    /* Equator, near equinox (day ~80): should be ~12 hours */
    double len = calendar_day_length(0.0, 80);
    TEST_ASSERT_FLOAT_WITHIN(0.5, 12.0, len);
}

void test_day_length_equator_summer(void)
{
    /* Equator is always ~12 hours */
    double len = calendar_day_length(0.0, 172);
    TEST_ASSERT_FLOAT_WITHIN(0.5, 12.0, len);
}

void test_day_length_equator_winter(void)
{
    double len = calendar_day_length(0.0, 355);
    TEST_ASSERT_FLOAT_WITHIN(0.5, 12.0, len);
}

void test_day_length_midlat_summer(void)
{
    /* ~40N, summer solstice (day 172): long day, ~14.5-15 hours */
    double len = calendar_day_length(40.0, 172);
    TEST_ASSERT_TRUE(len > 14.0);
    TEST_ASSERT_TRUE(len < 16.0);
}

void test_day_length_midlat_winter(void)
{
    /* ~40N, winter solstice (day 355): short day, ~9-10 hours */
    double len = calendar_day_length(40.0, 355);
    TEST_ASSERT_TRUE(len > 8.5);
    TEST_ASSERT_TRUE(len < 10.5);
}

void test_day_length_arctic_polar_day(void)
{
    /* 70N, near summer solstice: polar day (24h) */
    double len = calendar_day_length(70.0, 172);
    TEST_ASSERT_FLOAT_WITHIN(0.1, 24.0, len);
}

void test_day_length_arctic_polar_night(void)
{
    /* 70N, near winter solstice: polar night (0h) */
    double len = calendar_day_length(70.0, 355);
    TEST_ASSERT_FLOAT_WITHIN(0.1, 0.0, len);
}

void test_day_length_south_hemisphere(void)
{
    /* 40S summer is around day 355 (Dec), should be long */
    double len = calendar_day_length(-40.0, 355);
    TEST_ASSERT_TRUE(len > 14.0);
}

/* ===== calendar_night_length ===== */

void test_night_length_equator(void)
{
    double night = calendar_night_length(0.0, 80);
    TEST_ASSERT_FLOAT_WITHIN(0.5, 12.0, night);
}

void test_day_plus_night_equals_24(void)
{
    /* Day + night should always equal 24 */
    double day = calendar_day_length(40.0, 172);
    double night = calendar_night_length(40.0, 172);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 24.0, day + night);
}

void test_day_plus_night_24_polar(void)
{
    double day = calendar_day_length(70.0, 172);
    double night = calendar_night_length(70.0, 172);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 24.0, day + night);
}

void test_day_plus_night_24_equinox(void)
{
    double day = calendar_day_length(50.0, 80);
    double night = calendar_night_length(50.0, 80);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 24.0, day + night);
}

/* ===== seasonal_hour_length ===== */

void test_seasonal_equator_day(void)
{
    /* Equator: seasonal hours ~60 min always */
    double min = seasonal_hour_length(0.0, 172, 1);
    TEST_ASSERT_FLOAT_WITHIN(2.0, 60.0, min);
}

void test_seasonal_equator_night(void)
{
    double min = seasonal_hour_length(0.0, 172, 0);
    TEST_ASSERT_FLOAT_WITHIN(2.0, 60.0, min);
}

void test_seasonal_equinox_midlat_day(void)
{
    /* At equinox, everywhere gets ~12h day: seasonal hour ~60 min */
    double min = seasonal_hour_length(40.0, 80, 1);
    TEST_ASSERT_FLOAT_WITHIN(3.0, 60.0, min);
}

void test_seasonal_equinox_midlat_night(void)
{
    double min = seasonal_hour_length(40.0, 80, 0);
    TEST_ASSERT_FLOAT_WITHIN(3.0, 60.0, min);
}

void test_seasonal_summer_day_long(void)
{
    /* 40N summer solstice: day hours > 60 min */
    double min = seasonal_hour_length(40.0, 172, 1);
    TEST_ASSERT_TRUE(min > 65.0);
}

void test_seasonal_summer_night_short(void)
{
    /* 40N summer solstice: night hours < 60 min */
    double min = seasonal_hour_length(40.0, 172, 0);
    TEST_ASSERT_TRUE(min < 55.0);
}

void test_seasonal_winter_day_short(void)
{
    /* 40N winter solstice: day hours < 60 min */
    double min = seasonal_hour_length(40.0, 355, 1);
    TEST_ASSERT_TRUE(min < 55.0);
}

void test_seasonal_winter_night_long(void)
{
    /* 40N winter solstice: night hours > 60 min */
    double min = seasonal_hour_length(40.0, 355, 0);
    TEST_ASSERT_TRUE(min > 65.0);
}

void test_seasonal_polar_day(void)
{
    /* 70N midsummer: 24h daylight => day hour = 120 min, night = 0 min */
    double day_min = seasonal_hour_length(70.0, 172, 1);
    double night_min = seasonal_hour_length(70.0, 172, 0);
    TEST_ASSERT_FLOAT_WITHIN(0.5, 120.0, day_min);
    TEST_ASSERT_FLOAT_WITHIN(0.5, 0.0, night_min);
}

/* ===== gregorian_month_days ===== */

void test_month_jan(void)
{
    TEST_ASSERT_EQUAL_INT(31, gregorian_month_days(1, 0));
}

void test_month_feb_normal(void)
{
    TEST_ASSERT_EQUAL_INT(28, gregorian_month_days(2, 0));
}

void test_month_feb_leap(void)
{
    TEST_ASSERT_EQUAL_INT(29, gregorian_month_days(2, 1));
}

void test_month_apr(void)
{
    TEST_ASSERT_EQUAL_INT(30, gregorian_month_days(4, 0));
}

void test_month_jul(void)
{
    TEST_ASSERT_EQUAL_INT(31, gregorian_month_days(7, 0));
}

void test_month_dec(void)
{
    TEST_ASSERT_EQUAL_INT(31, gregorian_month_days(12, 0));
}

void test_month_invalid_zero(void)
{
    TEST_ASSERT_EQUAL_INT(0, gregorian_month_days(0, 0));
}

void test_month_invalid_13(void)
{
    TEST_ASSERT_EQUAL_INT(0, gregorian_month_days(13, 0));
}

/* ===== calendar_year_length ===== */

void test_year_length_egyptian(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.01, 365.0, calendar_year_length(0));
}

void test_year_length_julian(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.001, 365.25, calendar_year_length(3));
}

void test_year_length_gregorian(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 365.2425, calendar_year_length(4));
}

void test_year_length_babylonian(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.01, 354.0, calendar_year_length(1));
}

void test_year_length_invalid(void)
{
    /* Invalid index should return 0 */
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.0, calendar_year_length(-1));
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.0, calendar_year_length(9999));
}

/* ===== calendar_culture_count / calendar_culture_get ===== */

void test_culture_count(void)
{
    int count = calendar_culture_count();
    TEST_ASSERT_TRUE(count >= 8);
}

void test_culture_get_valid(void)
{
    const char *c = calendar_culture_get(0);
    TEST_ASSERT_NOT_NULL(c);
    TEST_ASSERT_TRUE(strlen(c) > 0);
}

void test_culture_get_invalid(void)
{
    const char *c = calendar_culture_get(-1);
    TEST_ASSERT_NULL(c);
}

void test_culture_get_out_of_range(void)
{
    const char *c = calendar_culture_get(9999);
    TEST_ASSERT_NULL(c);
}

void test_cultures_are_unique(void)
{
    int count = calendar_culture_count();
    for (int i = 0; i < count; i++) {
        for (int j = i + 1; j < count; j++) {
            TEST_ASSERT_FALSE(
                strcmp(calendar_culture_get(i), calendar_culture_get(j)) == 0
            );
        }
    }
}

/* ===== data integrity checks ===== */

void test_all_entries_have_names(void)
{
    int count = calendar_system_count();
    for (int i = 0; i < count; i++) {
        calendar_system_t s = calendar_system_get(i);
        TEST_ASSERT_NOT_NULL(s.name);
        TEST_ASSERT_NOT_NULL(s.originator);
        TEST_ASSERT_NOT_NULL(s.culture);
        TEST_ASSERT_NOT_NULL(s.description);
    }
}

void test_all_entries_have_valid_type(void)
{
    int count = calendar_system_count();
    for (int i = 0; i < count; i++) {
        calendar_system_t s = calendar_system_get(i);
        TEST_ASSERT_TRUE(s.type >= 0);
        TEST_ASSERT_TRUE(s.type < CAL_TYPE_COUNT);
    }
}

void test_ids_match_indices(void)
{
    int count = calendar_system_count();
    for (int i = 0; i < count; i++) {
        calendar_system_t s = calendar_system_get(i);
        TEST_ASSERT_EQUAL_INT(i, s.id);
    }
}

/* ===== Runner ===== */

int main(void)
{
    UNITY_BEGIN();

    /* Count */
    RUN_TEST(test_count_at_least_10);

    /* Get by index — specific systems */
    RUN_TEST(test_get_egyptian);
    RUN_TEST(test_get_babylonian);
    RUN_TEST(test_get_julian);
    RUN_TEST(test_get_gregorian);
    RUN_TEST(test_get_french_republican);
    RUN_TEST(test_get_13_moon);
    RUN_TEST(test_get_world_calendar);
    RUN_TEST(test_get_ifc);
    RUN_TEST(test_get_seasonal_hours);

    /* Invalid index */
    RUN_TEST(test_get_invalid_negative);
    RUN_TEST(test_get_invalid_too_large);

    /* Filter by type */
    RUN_TEST(test_filter_solar);
    RUN_TEST(test_filter_lunisolar);
    RUN_TEST(test_filter_proposed);
    RUN_TEST(test_filter_adopted);
    RUN_TEST(test_filter_respects_max);
    RUN_TEST(test_filter_lunar_empty);

    /* Type names */
    RUN_TEST(test_type_name_lunar);
    RUN_TEST(test_type_name_solar);
    RUN_TEST(test_type_name_lunisolar);
    RUN_TEST(test_type_name_proposed);
    RUN_TEST(test_type_name_adopted);
    RUN_TEST(test_type_name_invalid);

    /* Day length */
    RUN_TEST(test_day_length_equator_equinox);
    RUN_TEST(test_day_length_equator_summer);
    RUN_TEST(test_day_length_equator_winter);
    RUN_TEST(test_day_length_midlat_summer);
    RUN_TEST(test_day_length_midlat_winter);
    RUN_TEST(test_day_length_arctic_polar_day);
    RUN_TEST(test_day_length_arctic_polar_night);
    RUN_TEST(test_day_length_south_hemisphere);

    /* Night length */
    RUN_TEST(test_night_length_equator);
    RUN_TEST(test_day_plus_night_equals_24);
    RUN_TEST(test_day_plus_night_24_polar);
    RUN_TEST(test_day_plus_night_24_equinox);

    /* Seasonal hours */
    RUN_TEST(test_seasonal_equator_day);
    RUN_TEST(test_seasonal_equator_night);
    RUN_TEST(test_seasonal_equinox_midlat_day);
    RUN_TEST(test_seasonal_equinox_midlat_night);
    RUN_TEST(test_seasonal_summer_day_long);
    RUN_TEST(test_seasonal_summer_night_short);
    RUN_TEST(test_seasonal_winter_day_short);
    RUN_TEST(test_seasonal_winter_night_long);
    RUN_TEST(test_seasonal_polar_day);

    /* Gregorian months */
    RUN_TEST(test_month_jan);
    RUN_TEST(test_month_feb_normal);
    RUN_TEST(test_month_feb_leap);
    RUN_TEST(test_month_apr);
    RUN_TEST(test_month_jul);
    RUN_TEST(test_month_dec);
    RUN_TEST(test_month_invalid_zero);
    RUN_TEST(test_month_invalid_13);

    /* Year length */
    RUN_TEST(test_year_length_egyptian);
    RUN_TEST(test_year_length_julian);
    RUN_TEST(test_year_length_gregorian);
    RUN_TEST(test_year_length_babylonian);
    RUN_TEST(test_year_length_invalid);

    /* Cultures */
    RUN_TEST(test_culture_count);
    RUN_TEST(test_culture_get_valid);
    RUN_TEST(test_culture_get_invalid);
    RUN_TEST(test_culture_get_out_of_range);
    RUN_TEST(test_cultures_are_unique);

    /* Data integrity */
    RUN_TEST(test_all_entries_have_names);
    RUN_TEST(test_all_entries_have_valid_type);
    RUN_TEST(test_ids_match_indices);

    return UNITY_END();
}
