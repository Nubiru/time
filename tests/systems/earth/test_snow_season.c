/* test_snow_season.c — Tests for snow season modeling.
 * TDD RED phase: all tests written before implementation.
 * Covers: temperature model, snow probability, ski season window,
 * powder likelihood, avalanche risk, snow line, winter detection. */

#include "../../unity/unity.h"
#include "../../../src/systems/earth/snow_season.h"

#include <math.h>

void setUp(void) { }
void tearDown(void) { }

/* ===== Temperature Model ===== */

void test_temp_equator_sea_level_warm(void) {
    /* Equator at sea level in April (mean month): ~30 C (base temp) */
    double t = snow_temperature(0.0, 0.0, 4);
    TEST_ASSERT_FLOAT_WITHIN(5.0, 30.0, t);
}

void test_temp_pole_very_cold(void) {
    /* North pole in January: extremely cold */
    double t = snow_temperature(90.0, 0.0, 1);
    TEST_ASSERT_TRUE(t < -20.0);
}

void test_temp_lapse_rate(void) {
    /* 3000m higher should be ~19.5 C colder (6.5 * 3) */
    double t_sea = snow_temperature(45.0, 0.0, 7);
    double t_mtn = snow_temperature(45.0, 3000.0, 7);
    double diff = t_sea - t_mtn;
    TEST_ASSERT_FLOAT_WITHIN(1.0, 19.5, diff);
}

void test_temp_northern_summer_warmer(void) {
    /* 45N: July warmer than January */
    double t_jan = snow_temperature(45.0, 0.0, 1);
    double t_jul = snow_temperature(45.0, 0.0, 7);
    TEST_ASSERT_TRUE(t_jul > t_jan);
}

void test_temp_southern_hemisphere_inverted(void) {
    /* 45S: January (summer) warmer than July (winter) */
    double t_jan = snow_temperature(-45.0, 0.0, 1);
    double t_jul = snow_temperature(-45.0, 0.0, 7);
    TEST_ASSERT_TRUE(t_jan > t_jul);
}

void test_temp_high_elevation_colder(void) {
    /* Higher elevation is always colder for same lat/month */
    double t_low = snow_temperature(30.0, 500.0, 6);
    double t_high = snow_temperature(30.0, 4000.0, 6);
    TEST_ASSERT_TRUE(t_high < t_low);
}

/* ===== Snow Probability ===== */

void test_snow_prob_equator_sea_level_zero(void) {
    /* Equator at sea level, any month: too warm for snow */
    double p = snow_probability(0.0, 0.0, 7);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 0.0, p);
}

void test_snow_prob_arctic_winter_certain(void) {
    /* 80N in January at sea level: very high snow probability */
    double p = snow_probability(80.0, 0.0, 1);
    TEST_ASSERT_TRUE(p > 0.8);
}

void test_snow_prob_high_elevation_tropical(void) {
    /* Near equator but very high elevation (5000m) in coolest month:
     * lapse rate drops T well below freezing */
    double p = snow_probability(5.0, 5000.0, 1);
    TEST_ASSERT_TRUE(p > 0.5);
}

void test_snow_prob_range_0_to_1(void) {
    /* Probability is always in [0, 1] */
    for (int m = 1; m <= 12; m++) {
        double p = snow_probability(45.0, 2000.0, m);
        TEST_ASSERT_TRUE(p >= 0.0);
        TEST_ASSERT_TRUE(p <= 1.0);
    }
}

void test_snow_prob_summer_lower_than_winter(void) {
    /* 50N, 1500m: winter snow prob > summer snow prob */
    double p_jan = snow_probability(50.0, 1500.0, 1);
    double p_jul = snow_probability(50.0, 1500.0, 7);
    TEST_ASSERT_TRUE(p_jan > p_jul);
}

void test_snow_prob_cold_temp_gives_1(void) {
    /* Very cold location: probability = 1.0 */
    double p = snow_probability(80.0, 3000.0, 1);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 1.0, p);
}

void test_snow_prob_warm_temp_gives_0(void) {
    /* Very warm location: probability = 0.0 */
    double p = snow_probability(10.0, 0.0, 7);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 0.0, p);
}

/* ===== Ski Season Window ===== */

void test_ski_season_arctic_long(void) {
    /* 70N, 1000m: should have a long ski season */
    season_window_t s = snow_ski_season(70.0, 1000.0);
    TEST_ASSERT_TRUE(s.duration >= 5);
}

void test_ski_season_tropics_sea_level_none(void) {
    /* Equator at sea level: no ski season */
    season_window_t s = snow_ski_season(0.0, 0.0);
    TEST_ASSERT_EQUAL_INT(0, s.start_month);
    TEST_ASSERT_EQUAL_INT(0, s.end_month);
    TEST_ASSERT_EQUAL_INT(0, s.duration);
}

void test_ski_season_alpine(void) {
    /* 47N (Alps), 2500m: long ski season Sep-May (wraps around) */
    season_window_t s = snow_ski_season(47.0, 2500.0);
    TEST_ASSERT_TRUE(s.duration >= 5);
    /* Start month should be in autumn (Sep-Oct) */
    TEST_ASSERT_TRUE(s.start_month >= 8 && s.start_month <= 11);
}

void test_ski_season_southern_hemisphere(void) {
    /* 45S, 2000m: ski season should be in southern winter (May-Sep) */
    season_window_t s = snow_ski_season(-45.0, 2000.0);
    TEST_ASSERT_TRUE(s.duration >= 2);
    TEST_ASSERT_TRUE(s.start_month >= 4 && s.start_month <= 7);
}

void test_ski_season_duration_nonnegative(void) {
    /* Duration is never negative */
    season_window_t s = snow_ski_season(30.0, 500.0);
    TEST_ASSERT_TRUE(s.duration >= 0);
}

/* ===== Powder Likelihood ===== */

void test_powder_cold_dry_conditions(void) {
    /* T near -10 C gives best powder. 25N/500m/Feb gives T ~ -10 */
    double p = snow_powder_likelihood(25.0, 500.0, 2);
    TEST_ASSERT_TRUE(p > 0.3);
}

void test_powder_warm_conditions_low(void) {
    /* Warm location: no powder */
    double p = snow_powder_likelihood(10.0, 0.0, 7);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 0.0, p);
}

void test_powder_range_0_to_1(void) {
    /* Powder likelihood is always in [0, 1] */
    for (int m = 1; m <= 12; m++) {
        double p = snow_powder_likelihood(50.0, 2500.0, m);
        TEST_ASSERT_TRUE(p >= 0.0);
        TEST_ASSERT_TRUE(p <= 1.0);
    }
}

void test_powder_peak_around_minus10(void) {
    /* The formula peaks when T ~ -10 C.
     * 25N/0m/Jan gives T = -10, which is exact peak. */
    double p = snow_powder_likelihood(25.0, 0.0, 1);
    TEST_ASSERT_TRUE(p > 0.2);
}

/* ===== Avalanche Risk ===== */

void test_avalanche_risk_range(void) {
    /* All risk factors in [0, 1] */
    avalanche_risk_t r = snow_avalanche_risk(50.0, 3000.0, 3);
    TEST_ASSERT_TRUE(r.temperature_risk >= 0.0 && r.temperature_risk <= 1.0);
    TEST_ASSERT_TRUE(r.snowpack_risk >= 0.0 && r.snowpack_risk <= 1.0);
    TEST_ASSERT_TRUE(r.wind_risk >= 0.0 && r.wind_risk <= 1.0);
    TEST_ASSERT_TRUE(r.overall_risk >= 0.0 && r.overall_risk <= 1.0);
}

void test_avalanche_risk_high_elevation_wind(void) {
    /* High elevation: significant wind risk */
    avalanche_risk_t r = snow_avalanche_risk(45.0, 4000.0, 2);
    TEST_ASSERT_TRUE(r.wind_risk >= 0.5);
}

void test_avalanche_risk_low_elevation_less_wind(void) {
    /* Low elevation: less wind risk */
    avalanche_risk_t r = snow_avalanche_risk(45.0, 500.0, 2);
    TEST_ASSERT_TRUE(r.wind_risk < 0.5);
}

void test_avalanche_risk_overall_is_average(void) {
    /* Overall risk = average of three components */
    avalanche_risk_t r = snow_avalanche_risk(50.0, 2500.0, 3);
    double expected = (r.temperature_risk + r.snowpack_risk + r.wind_risk) / 3.0;
    TEST_ASSERT_FLOAT_WITHIN(0.01, expected, r.overall_risk);
}

void test_avalanche_risk_summer_lower(void) {
    /* Summer at moderate elevation: lower overall risk than winter */
    avalanche_risk_t winter = snow_avalanche_risk(50.0, 2500.0, 1);
    avalanche_risk_t summer = snow_avalanche_risk(50.0, 2500.0, 7);
    TEST_ASSERT_TRUE(winter.overall_risk >= summer.overall_risk);
}

/* ===== Snow Line Elevation ===== */

void test_snow_line_tropics_high(void) {
    /* Near equator: snow line very high */
    double line = snow_line_elevation(5.0, 7);
    TEST_ASSERT_TRUE(line > 3000.0);
}

void test_snow_line_arctic_winter_zero(void) {
    /* Arctic in winter: snow to sea level */
    double line = snow_line_elevation(80.0, 1);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 0.0, line);
}

void test_snow_line_nonnegative(void) {
    /* Snow line is always >= 0 */
    for (int lat = 0; lat <= 90; lat += 15) {
        for (int m = 1; m <= 12; m += 2) {
            double line = snow_line_elevation((double)lat, m);
            TEST_ASSERT_TRUE(line >= 0.0);
        }
    }
}

void test_snow_line_lower_in_winter(void) {
    /* 50N: snow line lower in January than July */
    double jan = snow_line_elevation(50.0, 1);
    double jul = snow_line_elevation(50.0, 7);
    TEST_ASSERT_TRUE(jan < jul);
}

/* ===== Winter Detection ===== */

void test_is_winter_northern_january(void) {
    /* 45N in January: winter */
    TEST_ASSERT_TRUE(snow_is_winter(45.0, 1));
}

void test_is_winter_northern_july(void) {
    /* 45N in July: not winter */
    TEST_ASSERT_TRUE(!snow_is_winter(45.0, 7));
}

void test_is_winter_southern_july(void) {
    /* 45S in July: winter (southern hemisphere) */
    TEST_ASSERT_TRUE(snow_is_winter(-45.0, 7));
}

void test_is_winter_southern_january(void) {
    /* 45S in January: not winter (southern summer) */
    TEST_ASSERT_TRUE(!snow_is_winter(-45.0, 1));
}

void test_is_winter_equator_always_false(void) {
    /* Equator: never truly winter */
    for (int m = 1; m <= 12; m++) {
        TEST_ASSERT_TRUE(!snow_is_winter(0.0, m));
    }
}

int main(void) {
    UNITY_BEGIN();

    /* Temperature model (6 tests) */
    RUN_TEST(test_temp_equator_sea_level_warm);
    RUN_TEST(test_temp_pole_very_cold);
    RUN_TEST(test_temp_lapse_rate);
    RUN_TEST(test_temp_northern_summer_warmer);
    RUN_TEST(test_temp_southern_hemisphere_inverted);
    RUN_TEST(test_temp_high_elevation_colder);

    /* Snow probability (7 tests) */
    RUN_TEST(test_snow_prob_equator_sea_level_zero);
    RUN_TEST(test_snow_prob_arctic_winter_certain);
    RUN_TEST(test_snow_prob_high_elevation_tropical);
    RUN_TEST(test_snow_prob_range_0_to_1);
    RUN_TEST(test_snow_prob_summer_lower_than_winter);
    RUN_TEST(test_snow_prob_cold_temp_gives_1);
    RUN_TEST(test_snow_prob_warm_temp_gives_0);

    /* Ski season window (5 tests) */
    RUN_TEST(test_ski_season_arctic_long);
    RUN_TEST(test_ski_season_tropics_sea_level_none);
    RUN_TEST(test_ski_season_alpine);
    RUN_TEST(test_ski_season_southern_hemisphere);
    RUN_TEST(test_ski_season_duration_nonnegative);

    /* Powder likelihood (4 tests) */
    RUN_TEST(test_powder_cold_dry_conditions);
    RUN_TEST(test_powder_warm_conditions_low);
    RUN_TEST(test_powder_range_0_to_1);
    RUN_TEST(test_powder_peak_around_minus10);

    /* Avalanche risk (5 tests) */
    RUN_TEST(test_avalanche_risk_range);
    RUN_TEST(test_avalanche_risk_high_elevation_wind);
    RUN_TEST(test_avalanche_risk_low_elevation_less_wind);
    RUN_TEST(test_avalanche_risk_overall_is_average);
    RUN_TEST(test_avalanche_risk_summer_lower);

    /* Snow line elevation (4 tests) */
    RUN_TEST(test_snow_line_tropics_high);
    RUN_TEST(test_snow_line_arctic_winter_zero);
    RUN_TEST(test_snow_line_nonnegative);
    RUN_TEST(test_snow_line_lower_in_winter);

    /* Winter detection (5 tests) */
    RUN_TEST(test_is_winter_northern_january);
    RUN_TEST(test_is_winter_northern_july);
    RUN_TEST(test_is_winter_southern_july);
    RUN_TEST(test_is_winter_southern_january);
    RUN_TEST(test_is_winter_equator_always_false);

    return UNITY_END();
}
