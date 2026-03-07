/* test_solar_radiation.c — Tests for solar radiation model.
 * TDD RED phase: all tests written before implementation.
 * Covers: declination, irradiance, day length, noon elevation,
 * seasonal temperature, freezing elevation, daily mean irradiance. */

#include "../../unity/unity.h"
#include "../../../src/systems/earth/solar_radiation.h"

#include <math.h>

void setUp(void) { }
void tearDown(void) { }

/* ===== Solar Declination ===== */

void test_declination_summer_solstice(void) {
    /* Day 172 (June 21): declination ~+23.44 degrees */
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 23.44f,
                             (float)solar_declination(172));
}

void test_declination_winter_solstice(void) {
    /* Day 355 (Dec 21): declination ~-23.44 degrees */
    TEST_ASSERT_FLOAT_WITHIN(1.0f, -23.44f,
                             (float)solar_declination(355));
}

void test_declination_vernal_equinox(void) {
    /* Day 80 (March 21): declination ~0 */
    TEST_ASSERT_FLOAT_WITHIN(1.5f, 0.0f,
                             (float)solar_declination(80));
}

void test_declination_autumnal_equinox(void) {
    /* Day 266 (Sep 23): declination ~0 */
    TEST_ASSERT_FLOAT_WITHIN(1.5f, 0.0f,
                             (float)solar_declination(266));
}

void test_declination_range(void) {
    /* Declination must always be in [-23.5, +23.5] */
    for (int doy = 1; doy <= 365; doy++) {
        double d = solar_declination(doy);
        TEST_ASSERT_TRUE(d >= -23.5 && d <= 23.5);
    }
}

/* ===== Solar Irradiance ===== */

void test_irradiance_equator_noon_equinox(void) {
    /* Equator, noon (hour_angle=0), equinox (decl=0): max irradiance */
    solar_irradiance_t r = solar_irradiance(0.0, 0.0, 0.0);
    TEST_ASSERT_FLOAT_WITHIN(10.0f, (float)SOLAR_CONSTANT,
                             (float)r.irradiance_toa);
    TEST_ASSERT_TRUE(r.irradiance_surface > 0.0);
    TEST_ASSERT_TRUE(r.irradiance_surface < r.irradiance_toa);
}

void test_irradiance_air_mass_zenith(void) {
    /* At zenith, air mass should be ~1.0 */
    solar_irradiance_t r = solar_irradiance(0.0, 0.0, 0.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, (float)r.air_mass);
}

void test_irradiance_elevation_zenith(void) {
    /* Equator, equinox, noon: elevation = 90 degrees */
    solar_irradiance_t r = solar_irradiance(0.0, 0.0, 0.0);
    TEST_ASSERT_FLOAT_WITHIN(0.5f, 90.0f, (float)r.solar_elevation);
}

void test_irradiance_high_latitude_reduced(void) {
    /* 60N at noon equinox: lower irradiance than equator */
    solar_irradiance_t equator = solar_irradiance(0.0, 0.0, 0.0);
    solar_irradiance_t high_lat = solar_irradiance(60.0, 0.0, 0.0);
    TEST_ASSERT_TRUE(high_lat.irradiance_toa < equator.irradiance_toa);
}

void test_irradiance_below_horizon(void) {
    /* Sun below horizon: irradiance should be 0 */
    solar_irradiance_t r = solar_irradiance(60.0, -23.44, 150.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, (float)r.irradiance_toa);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, (float)r.irradiance_surface);
}

void test_irradiance_surface_less_than_toa(void) {
    /* Surface irradiance always less than TOA due to atmosphere */
    solar_irradiance_t r = solar_irradiance(30.0, 10.0, 0.0);
    TEST_ASSERT_TRUE(r.irradiance_surface <= r.irradiance_toa);
}

void test_irradiance_air_mass_increases_at_low_elevation(void) {
    /* Air mass increases as sun is lower in the sky */
    solar_irradiance_t noon = solar_irradiance(0.0, 0.0, 0.0);
    solar_irradiance_t low = solar_irradiance(0.0, 0.0, 75.0);
    TEST_ASSERT_TRUE(low.air_mass > noon.air_mass);
}

/* ===== Day Length ===== */

void test_day_length_equator_equinox(void) {
    /* Equator at equinox: ~12 hours */
    day_length_t dl = solar_day_length(0.0, 0.0);
    TEST_ASSERT_FLOAT_WITHIN(0.5f, 12.0f, (float)dl.hours);
    TEST_ASSERT_EQUAL_INT(0, dl.is_polar_day);
    TEST_ASSERT_EQUAL_INT(0, dl.is_polar_night);
}

void test_day_length_equator_sunrise_sunset(void) {
    /* Equator equinox: sunrise ~6, sunset ~18 */
    day_length_t dl = solar_day_length(0.0, 0.0);
    TEST_ASSERT_FLOAT_WITHIN(0.5f, 6.0f, (float)dl.sunrise_hour);
    TEST_ASSERT_FLOAT_WITHIN(0.5f, 18.0f, (float)dl.sunset_hour);
}

void test_day_length_arctic_summer_polar_day(void) {
    /* 70N, summer solstice declination +23.44: polar day (24h) */
    day_length_t dl = solar_day_length(70.0, 23.44);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 24.0f, (float)dl.hours);
    TEST_ASSERT_EQUAL_INT(1, dl.is_polar_day);
    TEST_ASSERT_EQUAL_INT(0, dl.is_polar_night);
}

void test_day_length_arctic_winter_polar_night(void) {
    /* 70N, winter solstice declination -23.44: polar night (0h) */
    day_length_t dl = solar_day_length(70.0, -23.44);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, (float)dl.hours);
    TEST_ASSERT_EQUAL_INT(0, dl.is_polar_day);
    TEST_ASSERT_EQUAL_INT(1, dl.is_polar_night);
}

void test_day_length_mid_latitude_summer(void) {
    /* 45N, summer: longer than 12 hours */
    day_length_t dl = solar_day_length(45.0, 23.44);
    TEST_ASSERT_TRUE(dl.hours > 12.0);
    TEST_ASSERT_TRUE(dl.hours < 24.0);
}

void test_day_length_mid_latitude_winter(void) {
    /* 45N, winter: shorter than 12 hours */
    day_length_t dl = solar_day_length(45.0, -23.44);
    TEST_ASSERT_TRUE(dl.hours > 0.0);
    TEST_ASSERT_TRUE(dl.hours < 12.0);
}

void test_day_length_southern_hemisphere(void) {
    /* 45S with decl +23.44 should be short day (winter in south) */
    day_length_t dl = solar_day_length(-45.0, 23.44);
    TEST_ASSERT_TRUE(dl.hours < 12.0);
}

/* ===== Day Length from DOY (wrapper) ===== */

void test_day_length_doy_equator_equinox(void) {
    /* Equator, day 80 (equinox): ~12 hours */
    day_length_t dl = solar_day_length_doy(0.0, 80);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 12.0f, (float)dl.hours);
}

void test_day_length_doy_arctic_summer(void) {
    /* 70N, day 172 (summer solstice): polar day */
    day_length_t dl = solar_day_length_doy(70.0, 172);
    TEST_ASSERT_EQUAL_INT(1, dl.is_polar_day);
}

/* ===== Solar Noon Elevation ===== */

void test_noon_elevation_equator_equinox(void) {
    /* Equator at equinox: sun at zenith (90 degrees) */
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 90.0f,
                             (float)solar_noon_elevation(0.0, 0.0));
}

void test_noon_elevation_equator_summer_solstice(void) {
    /* Equator at summer solstice: 90 - 23.44 = ~66.56 */
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 66.56f,
                             (float)solar_noon_elevation(0.0, 23.44));
}

void test_noon_elevation_tropic_of_cancer_summer(void) {
    /* 23.44N at summer solstice: sun directly overhead (90) */
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 90.0f,
                             (float)solar_noon_elevation(23.44, 23.44));
}

void test_noon_elevation_arctic_circle_summer(void) {
    /* 66.56N at summer solstice (decl 23.44): 90 - (66.56 - 23.44) = ~46.88 */
    TEST_ASSERT_FLOAT_WITHIN(0.5f, 46.88f,
                             (float)solar_noon_elevation(66.56, 23.44));
}

void test_noon_elevation_high_latitude_winter(void) {
    /* 60N at winter solstice: 90 - (60 + 23.44) = 6.56 */
    TEST_ASSERT_FLOAT_WITHIN(0.5f, 6.56f,
                             (float)solar_noon_elevation(60.0, -23.44));
}

/* ===== Seasonal Temperature ===== */

void test_temp_equator_sea_level(void) {
    /* Equator at sea level: ~27 C year-round */
    seasonal_temp_t t = solar_seasonal_temp(0.0, 80, 0.0);
    TEST_ASSERT_FLOAT_WITHIN(5.0f, 27.0f, (float)t.temp_c);
}

void test_temp_equator_kelvin(void) {
    /* Kelvin = Celsius + 273.15 */
    seasonal_temp_t t = solar_seasonal_temp(0.0, 80, 0.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, (float)(t.temp_c + 273.15),
                             (float)t.temp_k);
}

void test_temp_poles_cold(void) {
    /* 90N: very cold */
    seasonal_temp_t t = solar_seasonal_temp(90.0, 355, 0.0);
    TEST_ASSERT_TRUE(t.temp_c < -20.0);
}

void test_temp_elevation_effect(void) {
    /* Higher elevation = colder (lapse rate -6.5 C/km) */
    seasonal_temp_t sea_level = solar_seasonal_temp(30.0, 172, 0.0);
    seasonal_temp_t mountain = solar_seasonal_temp(30.0, 172, 3000.0);
    TEST_ASSERT_TRUE(mountain.temp_c < sea_level.temp_c);
    /* At 3000m: should be about 19.5 C colder */
    float expected_diff = 6.5f * 3.0f;
    float actual_diff = (float)(sea_level.temp_c - mountain.temp_c);
    TEST_ASSERT_FLOAT_WITHIN(0.5f, expected_diff, actual_diff);
}

void test_temp_summer_warmer_than_winter(void) {
    /* Northern hemisphere: day 172 (summer) warmer than day 355 (winter) */
    seasonal_temp_t summer = solar_seasonal_temp(45.0, 172, 0.0);
    seasonal_temp_t winter = solar_seasonal_temp(45.0, 355, 0.0);
    TEST_ASSERT_TRUE(summer.temp_c > winter.temp_c);
}

void test_temp_daily_range_equator(void) {
    /* Equator: daily range ~10 C */
    seasonal_temp_t t = solar_seasonal_temp(0.0, 80, 0.0);
    TEST_ASSERT_FLOAT_WITHIN(3.0f, 10.0f, (float)t.daily_range_c);
}

void test_temp_daily_range_mid_latitude(void) {
    /* Mid-latitude: daily range > equator range */
    seasonal_temp_t equator = solar_seasonal_temp(0.0, 80, 0.0);
    seasonal_temp_t mid = solar_seasonal_temp(45.0, 80, 0.0);
    TEST_ASSERT_TRUE(mid.daily_range_c > equator.daily_range_c);
}

/* ===== Freezing Elevation ===== */

void test_freezing_elevation_tropics_high(void) {
    /* Tropics: warm temps -> freezing elevation is high */
    double h = solar_freezing_elevation(0.0, 172);
    TEST_ASSERT_TRUE(h > 3000.0);
}

void test_freezing_elevation_arctic_low(void) {
    /* Arctic in winter: freezing at or near sea level */
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f,
                             (float)solar_freezing_elevation(80.0, 355));
}

void test_freezing_elevation_nonnegative(void) {
    /* Freezing elevation should never be negative */
    for (int lat = 0; lat <= 90; lat += 15) {
        for (int doy = 1; doy <= 365; doy += 60) {
            double h = solar_freezing_elevation((double)lat, doy);
            TEST_ASSERT_TRUE(h >= 0.0);
        }
    }
}

/* ===== Daily Mean Irradiance ===== */

void test_daily_mean_equator_vs_pole(void) {
    /* Equator receives more daily mean irradiance than pole (equinox) */
    double eq = solar_daily_mean_irradiance(0.0, 80);
    double pole = solar_daily_mean_irradiance(80.0, 80);
    TEST_ASSERT_TRUE(eq > pole);
}

void test_daily_mean_summer_vs_winter(void) {
    /* 45N: more irradiance in summer than winter */
    double summer = solar_daily_mean_irradiance(45.0, 172);
    double winter = solar_daily_mean_irradiance(45.0, 355);
    TEST_ASSERT_TRUE(summer > winter);
}

void test_daily_mean_nonnegative(void) {
    /* Daily mean irradiance is always >= 0 */
    double val = solar_daily_mean_irradiance(70.0, 355);
    TEST_ASSERT_TRUE(val >= 0.0);
}

void test_daily_mean_not_exceed_solar_constant(void) {
    /* Daily mean can't exceed the solar constant */
    double val = solar_daily_mean_irradiance(0.0, 80);
    TEST_ASSERT_TRUE(val <= SOLAR_CONSTANT);
}

int main(void) {
    UNITY_BEGIN();

    /* Solar declination (5 tests) */
    RUN_TEST(test_declination_summer_solstice);
    RUN_TEST(test_declination_winter_solstice);
    RUN_TEST(test_declination_vernal_equinox);
    RUN_TEST(test_declination_autumnal_equinox);
    RUN_TEST(test_declination_range);

    /* Solar irradiance (7 tests) */
    RUN_TEST(test_irradiance_equator_noon_equinox);
    RUN_TEST(test_irradiance_air_mass_zenith);
    RUN_TEST(test_irradiance_elevation_zenith);
    RUN_TEST(test_irradiance_high_latitude_reduced);
    RUN_TEST(test_irradiance_below_horizon);
    RUN_TEST(test_irradiance_surface_less_than_toa);
    RUN_TEST(test_irradiance_air_mass_increases_at_low_elevation);

    /* Day length (7 tests) */
    RUN_TEST(test_day_length_equator_equinox);
    RUN_TEST(test_day_length_equator_sunrise_sunset);
    RUN_TEST(test_day_length_arctic_summer_polar_day);
    RUN_TEST(test_day_length_arctic_winter_polar_night);
    RUN_TEST(test_day_length_mid_latitude_summer);
    RUN_TEST(test_day_length_mid_latitude_winter);
    RUN_TEST(test_day_length_southern_hemisphere);

    /* Day length from DOY (2 tests) */
    RUN_TEST(test_day_length_doy_equator_equinox);
    RUN_TEST(test_day_length_doy_arctic_summer);

    /* Noon elevation (5 tests) */
    RUN_TEST(test_noon_elevation_equator_equinox);
    RUN_TEST(test_noon_elevation_equator_summer_solstice);
    RUN_TEST(test_noon_elevation_tropic_of_cancer_summer);
    RUN_TEST(test_noon_elevation_arctic_circle_summer);
    RUN_TEST(test_noon_elevation_high_latitude_winter);

    /* Seasonal temperature (7 tests) */
    RUN_TEST(test_temp_equator_sea_level);
    RUN_TEST(test_temp_equator_kelvin);
    RUN_TEST(test_temp_poles_cold);
    RUN_TEST(test_temp_elevation_effect);
    RUN_TEST(test_temp_summer_warmer_than_winter);
    RUN_TEST(test_temp_daily_range_equator);
    RUN_TEST(test_temp_daily_range_mid_latitude);

    /* Freezing elevation (3 tests) */
    RUN_TEST(test_freezing_elevation_tropics_high);
    RUN_TEST(test_freezing_elevation_arctic_low);
    RUN_TEST(test_freezing_elevation_nonnegative);

    /* Daily mean irradiance (4 tests) */
    RUN_TEST(test_daily_mean_equator_vs_pole);
    RUN_TEST(test_daily_mean_summer_vs_winter);
    RUN_TEST(test_daily_mean_nonnegative);
    RUN_TEST(test_daily_mean_not_exceed_solar_constant);

    return UNITY_END();
}
