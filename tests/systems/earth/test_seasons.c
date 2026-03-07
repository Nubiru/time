/* test_seasons.c — Tests for seasonal cycle visualizer data module.
 * TDD RED phase: all tests written before implementation.
 * Models seasons, day length, growing season, cardinal dates,
 * temperature deviation from latitude and day-of-year. */

#include "../../unity/unity.h"
#include "../../../src/systems/earth/seasons.h"

#include <math.h>
#include <string.h>

void setUp(void) { }
void tearDown(void) { }

/* ===== Season Name ===== */

void test_season_name_spring(void) {
    TEST_ASSERT_EQUAL_STRING("Spring", season_name(SEASON_SPRING));
}

void test_season_name_summer(void) {
    TEST_ASSERT_EQUAL_STRING("Summer", season_name(SEASON_SUMMER));
}

void test_season_name_autumn(void) {
    TEST_ASSERT_EQUAL_STRING("Autumn", season_name(SEASON_AUTUMN));
}

void test_season_name_winter(void) {
    TEST_ASSERT_EQUAL_STRING("Winter", season_name(SEASON_WINTER));
}

void test_season_name_invalid(void) {
    TEST_ASSERT_EQUAL_STRING("Unknown", season_name(SEASON_COUNT));
}

/* ===== Cardinal Dates ===== */

void test_cardinal_vernal_equinox(void) {
    cardinal_dates_t d = season_cardinal_dates();
    TEST_ASSERT_EQUAL_INT(80, d.vernal_equinox);
}

void test_cardinal_summer_solstice(void) {
    cardinal_dates_t d = season_cardinal_dates();
    TEST_ASSERT_EQUAL_INT(172, d.summer_solstice);
}

void test_cardinal_autumnal_equinox(void) {
    cardinal_dates_t d = season_cardinal_dates();
    TEST_ASSERT_EQUAL_INT(265, d.autumnal_equinox);
}

void test_cardinal_winter_solstice(void) {
    cardinal_dates_t d = season_cardinal_dates();
    TEST_ASSERT_EQUAL_INT(355, d.winter_solstice);
}

/* ===== Solar Declination ===== */

void test_declination_summer_solstice(void) {
    /* Day 172 (Jun 21): near max declination +23.44 deg */
    double dec = season_solar_declination(172);
    TEST_ASSERT_FLOAT_WITHIN(0.5f, 23.44f, (float)dec);
}

void test_declination_winter_solstice(void) {
    /* Day 355 (Dec 21): near min declination -23.44 deg */
    double dec = season_solar_declination(355);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, -23.44f, (float)dec);
}

void test_declination_vernal_equinox(void) {
    /* Day 81 (Mar 22): declination near 0 */
    double dec = season_solar_declination(81);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 0.0f, (float)dec);
}

void test_declination_autumnal_equinox(void) {
    /* Day 265 (Sep 22): declination near 0 */
    double dec = season_solar_declination(265);
    TEST_ASSERT_FLOAT_WITHIN(2.0f, 0.0f, (float)dec);
}

void test_declination_range(void) {
    /* All declinations must be between -23.45 and +23.45 */
    for (int d = 1; d <= 365; d++) {
        double dec = season_solar_declination(d);
        TEST_ASSERT_TRUE(dec >= -23.45);
        TEST_ASSERT_TRUE(dec <= 23.45);
    }
}

/* ===== Day Length ===== */

void test_day_length_equator_equinox(void) {
    /* At equator on equinox: ~12 hours */
    double len = season_day_length(0.0, 81);
    TEST_ASSERT_FLOAT_WITHIN(0.5f, 12.0f, (float)len);
}

void test_day_length_equator_always_near_12(void) {
    /* Equator gets ~12h year-round */
    for (int d = 1; d <= 365; d += 30) {
        double len = season_day_length(0.0, d);
        TEST_ASSERT_FLOAT_WITHIN(1.0f, 12.0f, (float)len);
    }
}

void test_day_length_northern_summer_longer(void) {
    /* 45N in summer should be > 12 hours */
    double len = season_day_length(45.0, 172);
    TEST_ASSERT_TRUE(len > 14.0);
    TEST_ASSERT_TRUE(len < 18.0);
}

void test_day_length_northern_winter_shorter(void) {
    /* 45N in winter should be < 12 hours */
    double len = season_day_length(45.0, 355);
    TEST_ASSERT_TRUE(len < 10.0);
    TEST_ASSERT_TRUE(len > 6.0);
}

void test_day_length_arctic_summer(void) {
    /* 70N near summer solstice: 24h (midnight sun) */
    double len = season_day_length(70.0, 172);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 24.0f, (float)len);
}

void test_day_length_arctic_winter(void) {
    /* 70N near winter solstice: 0h (polar night) */
    double len = season_day_length(70.0, 355);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, (float)len);
}

void test_day_length_southern_summer_longer(void) {
    /* -45 (SH) in December (day 355): summer, should be > 14h */
    double len = season_day_length(-45.0, 355);
    TEST_ASSERT_TRUE(len > 14.0);
}

void test_day_length_clamped_0_to_24(void) {
    /* All values should be between 0 and 24 */
    for (int lat = -90; lat <= 90; lat += 10) {
        for (int d = 1; d <= 365; d += 30) {
            double len = season_day_length((double)lat, d);
            TEST_ASSERT_TRUE(len >= 0.0);
            TEST_ASSERT_TRUE(len <= 24.0);
        }
    }
}

/* ===== Season At (Northern Hemisphere) ===== */

void test_season_at_nh_spring(void) {
    /* Day 100 (mid-April) at 45N should be Spring */
    season_info_t s = season_at(45.0, 100);
    TEST_ASSERT_EQUAL_INT(SEASON_SPRING, s.season);
    TEST_ASSERT_EQUAL_STRING("Spring", s.name);
    TEST_ASSERT_EQUAL_INT(0, s.is_southern);
}

void test_season_at_nh_summer(void) {
    /* Day 200 (mid-July) at 45N should be Summer */
    season_info_t s = season_at(45.0, 200);
    TEST_ASSERT_EQUAL_INT(SEASON_SUMMER, s.season);
    TEST_ASSERT_EQUAL_STRING("Summer", s.name);
}

void test_season_at_nh_autumn(void) {
    /* Day 300 (late-October) at 45N should be Autumn */
    season_info_t s = season_at(45.0, 300);
    TEST_ASSERT_EQUAL_INT(SEASON_AUTUMN, s.season);
    TEST_ASSERT_EQUAL_STRING("Autumn", s.name);
}

void test_season_at_nh_winter_early_year(void) {
    /* Day 30 (late-January) at 45N should be Winter */
    season_info_t s = season_at(45.0, 30);
    TEST_ASSERT_EQUAL_INT(SEASON_WINTER, s.season);
    TEST_ASSERT_EQUAL_STRING("Winter", s.name);
}

void test_season_at_nh_winter_late_year(void) {
    /* Day 360 (late-December) at 45N should be Winter */
    season_info_t s = season_at(45.0, 360);
    TEST_ASSERT_EQUAL_INT(SEASON_WINTER, s.season);
}

/* ===== Season At (Southern Hemisphere) ===== */

void test_season_at_sh_autumn_in_april(void) {
    /* Day 100 (mid-April) at -30S should be Autumn (NH Spring flipped) */
    season_info_t s = season_at(-30.0, 100);
    TEST_ASSERT_EQUAL_INT(SEASON_AUTUMN, s.season);
    TEST_ASSERT_EQUAL_INT(1, s.is_southern);
}

void test_season_at_sh_winter_in_july(void) {
    /* Day 200 (mid-July) at -30S should be Winter (NH Summer flipped) */
    season_info_t s = season_at(-30.0, 200);
    TEST_ASSERT_EQUAL_INT(SEASON_WINTER, s.season);
}

void test_season_at_sh_spring_in_october(void) {
    /* Day 300 (late-Oct) at -30S should be Spring (NH Autumn flipped) */
    season_info_t s = season_at(-30.0, 300);
    TEST_ASSERT_EQUAL_INT(SEASON_SPRING, s.season);
}

void test_season_at_sh_summer_in_january(void) {
    /* Day 30 (late-Jan) at -30S should be Summer (NH Winter flipped) */
    season_info_t s = season_at(-30.0, 30);
    TEST_ASSERT_EQUAL_INT(SEASON_SUMMER, s.season);
}

/* ===== Season Progress ===== */

void test_season_progress_start(void) {
    /* Day 80 is start of Spring (NH) — progress near 0 */
    season_info_t s = season_at(45.0, 80);
    TEST_ASSERT_EQUAL_INT(SEASON_SPRING, s.season);
    TEST_ASSERT_FLOAT_WITHIN(0.02f, 0.0f, (float)s.progress);
}

void test_season_progress_mid(void) {
    /* Day 126 is roughly mid-Spring (80+46 out of 92 days) — progress ~0.5 */
    season_info_t s = season_at(45.0, 126);
    TEST_ASSERT_EQUAL_INT(SEASON_SPRING, s.season);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 0.5f, (float)s.progress);
}

void test_season_progress_near_end(void) {
    /* Day 171 is last day of Spring — progress near 1.0 */
    season_info_t s = season_at(45.0, 171);
    TEST_ASSERT_EQUAL_INT(SEASON_SPRING, s.season);
    TEST_ASSERT_TRUE(s.progress > 0.9);
}

void test_season_progress_range(void) {
    /* Progress should always be 0.0-1.0 */
    for (int d = 1; d <= 365; d++) {
        season_info_t s = season_at(45.0, d);
        TEST_ASSERT_TRUE(s.progress >= 0.0);
        TEST_ASSERT_TRUE(s.progress <= 1.0);
    }
}

void test_season_day_in_season(void) {
    /* Day 82 is the 3rd day of Spring (80, 81, 82 -> day_in_season=2 zero-based or 3 one-based).
     * Using 0-based: day_in_season = doy - start_doy = 82 - 80 = 2 */
    season_info_t s = season_at(45.0, 82);
    TEST_ASSERT_EQUAL_INT(SEASON_SPRING, s.season);
    TEST_ASSERT_EQUAL_INT(2, s.day_in_season);
}

void test_season_length_spring(void) {
    /* Spring is 92 days (80 to 171 inclusive) */
    season_info_t s = season_at(45.0, 100);
    TEST_ASSERT_EQUAL_INT(92, s.season_length);
}

void test_season_length_summer(void) {
    /* Summer is 93 days (172 to 264 inclusive) */
    season_info_t s = season_at(45.0, 200);
    TEST_ASSERT_EQUAL_INT(93, s.season_length);
}

/* ===== Season Opposite ===== */

void test_opposite_spring(void) {
    TEST_ASSERT_EQUAL_INT(SEASON_AUTUMN, season_opposite(SEASON_SPRING));
}

void test_opposite_summer(void) {
    TEST_ASSERT_EQUAL_INT(SEASON_WINTER, season_opposite(SEASON_SUMMER));
}

void test_opposite_autumn(void) {
    TEST_ASSERT_EQUAL_INT(SEASON_SPRING, season_opposite(SEASON_AUTUMN));
}

void test_opposite_winter(void) {
    TEST_ASSERT_EQUAL_INT(SEASON_SUMMER, season_opposite(SEASON_WINTER));
}

/* ===== Summer Hemisphere ===== */

void test_summer_hemisphere_june(void) {
    /* Day 172 (Jun 21): northern summer */
    TEST_ASSERT_EQUAL_INT(1, season_summer_hemisphere(172));
}

void test_summer_hemisphere_december(void) {
    /* Day 355 (Dec 21): southern summer */
    TEST_ASSERT_EQUAL_INT(-1, season_summer_hemisphere(355));
}

void test_summer_hemisphere_march(void) {
    /* Day 100 (Apr 10): declination positive -> northern summer */
    TEST_ASSERT_EQUAL_INT(1, season_summer_hemisphere(100));
}

/* ===== Days to Next Cardinal ===== */

void test_days_to_next_cardinal_from_day_1(void) {
    /* Day 1: next cardinal is vernal equinox at day 80. 79 days away. */
    const char *event = NULL;
    int days = season_days_to_next_cardinal(1, &event);
    TEST_ASSERT_EQUAL_INT(79, days);
    TEST_ASSERT_NOT_NULL(event);
    TEST_ASSERT_EQUAL_STRING("Vernal Equinox", event);
}

void test_days_to_next_cardinal_from_day_80(void) {
    /* Day 80 is the vernal equinox itself. Next is summer solstice at 172. */
    const char *event = NULL;
    int days = season_days_to_next_cardinal(80, &event);
    TEST_ASSERT_EQUAL_INT(92, days);
    TEST_ASSERT_EQUAL_STRING("Summer Solstice", event);
}

void test_days_to_next_cardinal_from_day_300(void) {
    /* Day 300: next is winter solstice at 355. 55 days away. */
    const char *event = NULL;
    int days = season_days_to_next_cardinal(300, &event);
    TEST_ASSERT_EQUAL_INT(55, days);
    TEST_ASSERT_EQUAL_STRING("Winter Solstice", event);
}

void test_days_to_next_cardinal_wraps(void) {
    /* Day 356: next cardinal wraps to vernal equinox day 80 next year.
     * 365-356+80 = 89 days */
    const char *event = NULL;
    int days = season_days_to_next_cardinal(356, &event);
    TEST_ASSERT_EQUAL_INT(89, days);
    TEST_ASSERT_EQUAL_STRING("Vernal Equinox", event);
}

void test_days_to_next_cardinal_null_event(void) {
    /* Should not crash with NULL event pointer */
    int days = season_days_to_next_cardinal(100, NULL);
    TEST_ASSERT_TRUE(days > 0);
}

/* ===== Growing Season ===== */

void test_growing_season_tropical(void) {
    /* Latitude 5: tropical, year-round */
    growing_season_t g = growing_season(5.0);
    TEST_ASSERT_EQUAL_INT(365, g.length_days);
    TEST_ASSERT_EQUAL_STRING("Tropical", g.classification);
}

void test_growing_season_moderate(void) {
    /* Latitude 40: moderate growing season ~200 days */
    growing_season_t g = growing_season(40.0);
    TEST_ASSERT_TRUE(g.length_days >= 150);
    TEST_ASSERT_TRUE(g.length_days <= 280);
    TEST_ASSERT_EQUAL_STRING("Moderate", g.classification);
}

void test_growing_season_short(void) {
    /* Latitude 50: short growing season ~150 days */
    growing_season_t g = growing_season(50.0);
    TEST_ASSERT_TRUE(g.length_days >= 100);
    TEST_ASSERT_TRUE(g.length_days <= 200);
    TEST_ASSERT_EQUAL_STRING("Short", g.classification);
}

void test_growing_season_none(void) {
    /* Latitude 80: no growing season */
    growing_season_t g = growing_season(80.0);
    TEST_ASSERT_EQUAL_INT(0, g.length_days);
    TEST_ASSERT_EQUAL_STRING("None", g.classification);
}

void test_growing_season_southern_hemisphere(void) {
    /* -5 latitude: also tropical */
    growing_season_t g = growing_season(-5.0);
    TEST_ASSERT_EQUAL_INT(365, g.length_days);
    TEST_ASSERT_EQUAL_STRING("Tropical", g.classification);
}

void test_growing_season_start_before_end(void) {
    /* For non-zero growing season, start < end makes sense */
    growing_season_t g = growing_season(40.0);
    TEST_ASSERT_TRUE(g.start_doy > 0);
    TEST_ASSERT_TRUE(g.end_doy > g.start_doy);
}

void test_growing_season_length_consistency(void) {
    /* length_days should equal end_doy - start_doy + 1 for non-tropical */
    growing_season_t g = growing_season(40.0);
    if (g.length_days > 0 && g.length_days < 365) {
        TEST_ASSERT_EQUAL_INT(g.end_doy - g.start_doy + 1, g.length_days);
    }
}

/* ===== Temperature Deviation ===== */

void test_temp_deviation_equator_minimal(void) {
    /* At equator: amplitude = 0*0.4 = 0, so deviation ~0 year-round */
    double dev = season_temp_deviation(0.0, 172);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, (float)dev);
}

void test_temp_deviation_nh_summer_positive(void) {
    /* 45N in summer (day 172): positive deviation */
    double dev = season_temp_deviation(45.0, 172);
    TEST_ASSERT_TRUE(dev > 10.0);
}

void test_temp_deviation_nh_winter_negative(void) {
    /* 45N in winter (day 355): negative deviation */
    double dev = season_temp_deviation(45.0, 355);
    TEST_ASSERT_TRUE(dev < -10.0);
}

void test_temp_deviation_sh_inverted(void) {
    /* -45S in NH summer (day 172): should be negative (SH winter) */
    double dev = season_temp_deviation(-45.0, 172);
    TEST_ASSERT_TRUE(dev < -10.0);
}

void test_temp_deviation_amplitude_scales_with_lat(void) {
    /* Higher latitudes get bigger swings */
    double dev_30 = fabs(season_temp_deviation(30.0, 172));
    double dev_60 = fabs(season_temp_deviation(60.0, 172));
    TEST_ASSERT_TRUE(dev_60 > dev_30);
}

void test_temp_deviation_symmetry(void) {
    /* Same magnitude at equinox, opposite signs at solstice for NH/SH */
    double dev_n = season_temp_deviation(45.0, 172);
    double dev_s = season_temp_deviation(-45.0, 172);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, (float)fabs(dev_n), (float)fabs(dev_s));
}

/* ===== Edge Cases ===== */

void test_season_at_day_1(void) {
    season_info_t s = season_at(45.0, 1);
    TEST_ASSERT_EQUAL_INT(SEASON_WINTER, s.season);
}

void test_season_at_day_365(void) {
    season_info_t s = season_at(45.0, 365);
    TEST_ASSERT_EQUAL_INT(SEASON_WINTER, s.season);
}

void test_day_length_north_pole_summer(void) {
    /* 90N near solstice: 24h */
    double len = season_day_length(90.0, 172);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 24.0f, (float)len);
}

void test_day_length_south_pole_winter(void) {
    /* -90S near NH summer: 0h (polar night) */
    double len = season_day_length(-90.0, 172);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, (float)len);
}

/* --- Runner --- */

int main(void) {
    UNITY_BEGIN();

    /* Season name */
    RUN_TEST(test_season_name_spring);
    RUN_TEST(test_season_name_summer);
    RUN_TEST(test_season_name_autumn);
    RUN_TEST(test_season_name_winter);
    RUN_TEST(test_season_name_invalid);

    /* Cardinal dates */
    RUN_TEST(test_cardinal_vernal_equinox);
    RUN_TEST(test_cardinal_summer_solstice);
    RUN_TEST(test_cardinal_autumnal_equinox);
    RUN_TEST(test_cardinal_winter_solstice);

    /* Solar declination */
    RUN_TEST(test_declination_summer_solstice);
    RUN_TEST(test_declination_winter_solstice);
    RUN_TEST(test_declination_vernal_equinox);
    RUN_TEST(test_declination_autumnal_equinox);
    RUN_TEST(test_declination_range);

    /* Day length */
    RUN_TEST(test_day_length_equator_equinox);
    RUN_TEST(test_day_length_equator_always_near_12);
    RUN_TEST(test_day_length_northern_summer_longer);
    RUN_TEST(test_day_length_northern_winter_shorter);
    RUN_TEST(test_day_length_arctic_summer);
    RUN_TEST(test_day_length_arctic_winter);
    RUN_TEST(test_day_length_southern_summer_longer);
    RUN_TEST(test_day_length_clamped_0_to_24);

    /* Season at (NH) */
    RUN_TEST(test_season_at_nh_spring);
    RUN_TEST(test_season_at_nh_summer);
    RUN_TEST(test_season_at_nh_autumn);
    RUN_TEST(test_season_at_nh_winter_early_year);
    RUN_TEST(test_season_at_nh_winter_late_year);

    /* Season at (SH) */
    RUN_TEST(test_season_at_sh_autumn_in_april);
    RUN_TEST(test_season_at_sh_winter_in_july);
    RUN_TEST(test_season_at_sh_spring_in_october);
    RUN_TEST(test_season_at_sh_summer_in_january);

    /* Season progress */
    RUN_TEST(test_season_progress_start);
    RUN_TEST(test_season_progress_mid);
    RUN_TEST(test_season_progress_near_end);
    RUN_TEST(test_season_progress_range);
    RUN_TEST(test_season_day_in_season);
    RUN_TEST(test_season_length_spring);
    RUN_TEST(test_season_length_summer);

    /* Season opposite */
    RUN_TEST(test_opposite_spring);
    RUN_TEST(test_opposite_summer);
    RUN_TEST(test_opposite_autumn);
    RUN_TEST(test_opposite_winter);

    /* Summer hemisphere */
    RUN_TEST(test_summer_hemisphere_june);
    RUN_TEST(test_summer_hemisphere_december);
    RUN_TEST(test_summer_hemisphere_march);

    /* Days to next cardinal */
    RUN_TEST(test_days_to_next_cardinal_from_day_1);
    RUN_TEST(test_days_to_next_cardinal_from_day_80);
    RUN_TEST(test_days_to_next_cardinal_from_day_300);
    RUN_TEST(test_days_to_next_cardinal_wraps);
    RUN_TEST(test_days_to_next_cardinal_null_event);

    /* Growing season */
    RUN_TEST(test_growing_season_tropical);
    RUN_TEST(test_growing_season_moderate);
    RUN_TEST(test_growing_season_short);
    RUN_TEST(test_growing_season_none);
    RUN_TEST(test_growing_season_southern_hemisphere);
    RUN_TEST(test_growing_season_start_before_end);
    RUN_TEST(test_growing_season_length_consistency);

    /* Temperature deviation */
    RUN_TEST(test_temp_deviation_equator_minimal);
    RUN_TEST(test_temp_deviation_nh_summer_positive);
    RUN_TEST(test_temp_deviation_nh_winter_negative);
    RUN_TEST(test_temp_deviation_sh_inverted);
    RUN_TEST(test_temp_deviation_amplitude_scales_with_lat);
    RUN_TEST(test_temp_deviation_symmetry);

    /* Edge cases */
    RUN_TEST(test_season_at_day_1);
    RUN_TEST(test_season_at_day_365);
    RUN_TEST(test_day_length_north_pole_summer);
    RUN_TEST(test_day_length_south_pole_winter);

    return UNITY_END();
}
