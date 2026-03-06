#include "../unity/unity.h"
#include "../../src/systems/astronomy/solar_events.h"

#include <math.h>

void setUp(void) {}
void tearDown(void) {}

/* Known JD values (noon UT):
 * Mar 20 2024 (vernal equinox): 2460390.0
 * Jun 21 2024 (summer solstice): 2460483.0
 * Dec 21 2024 (winter solstice): 2460666.0
 * Sep 22 2024 (autumnal equinox): 2460576.0
 */
#define JD_EQUINOX_MAR  2460390.0
#define JD_SOLSTICE_JUN 2460483.0
#define JD_SOLSTICE_DEC 2460666.0

/* Solar declination: 1-3 */

void test_declination_equinox(void)
{
    double dec = se_solar_declination(JD_EQUINOX_MAR);
    TEST_ASSERT_FLOAT_WITHIN(2.0f, 0.0f, (float)dec);
}

void test_declination_summer_solstice(void)
{
    double dec = se_solar_declination(JD_SOLSTICE_JUN);
    TEST_ASSERT_FLOAT_WITHIN(2.0f, 23.44f, (float)dec);
}

void test_declination_winter_solstice(void)
{
    double dec = se_solar_declination(JD_SOLSTICE_DEC);
    TEST_ASSERT_FLOAT_WITHIN(2.0f, -23.44f, (float)dec);
}

/* Equator at equinox: 4-5 */

void test_equator_equinox_day_length(void)
{
    solar_day_t sd = se_solar_events(JD_EQUINOX_MAR, 0.0, 0.0);
    TEST_ASSERT_FLOAT_WITHIN(0.5f, 12.0f, (float)sd.day_length);
}

void test_equator_equinox_valid(void)
{
    solar_day_t sd = se_solar_events(JD_EQUINOX_MAR, 0.0, 0.0);
    TEST_ASSERT_FALSE(isnan(sd.sunrise_jd));
    TEST_ASSERT_FALSE(isnan(sd.sunset_jd));
    TEST_ASSERT_EQUAL_INT(0, sd.always_day);
    TEST_ASSERT_EQUAL_INT(0, sd.always_night);
}

/* Mid-latitude seasonal variation: 6-8 */

void test_summer_45n_day_length(void)
{
    solar_day_t sd = se_solar_events(JD_SOLSTICE_JUN, 45.0, 0.0);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 15.5f, (float)sd.day_length);
}

void test_winter_45n_day_length(void)
{
    solar_day_t sd = se_solar_events(JD_SOLSTICE_DEC, 45.0, 0.0);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 8.5f, (float)sd.day_length);
}

void test_summer_longer_than_winter(void)
{
    solar_day_t summer = se_solar_events(JD_SOLSTICE_JUN, 45.0, 0.0);
    solar_day_t winter = se_solar_events(JD_SOLSTICE_DEC, 45.0, 0.0);
    TEST_ASSERT_TRUE(summer.day_length > winter.day_length);
}

/* Polar cases: 9-11 */

void test_arctic_summer_always_day(void)
{
    solar_day_t sd = se_solar_events(JD_SOLSTICE_JUN, 70.0, 0.0);
    TEST_ASSERT_EQUAL_INT(1, sd.always_day);
    TEST_ASSERT_EQUAL_INT(0, sd.always_night);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 24.0f, (float)sd.day_length);
}

void test_arctic_winter_always_night(void)
{
    solar_day_t sd = se_solar_events(JD_SOLSTICE_DEC, 70.0, 0.0);
    TEST_ASSERT_EQUAL_INT(0, sd.always_day);
    TEST_ASSERT_EQUAL_INT(1, sd.always_night);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, (float)sd.day_length);
}

void test_antarctic_june_always_night(void)
{
    solar_day_t sd = se_solar_events(JD_SOLSTICE_JUN, -70.0, 0.0);
    TEST_ASSERT_EQUAL_INT(0, sd.always_day);
    TEST_ASSERT_EQUAL_INT(1, sd.always_night);
}

/* Hour angle: 12-13 */

void test_hour_angle_equator_equinox(void)
{
    double dec = se_solar_declination(JD_EQUINOX_MAR);
    double ha = se_sunrise_hour_angle(0.0, dec);
    TEST_ASSERT_FLOAT_WITHIN(5.0f, 90.0f, (float)ha);
}

void test_hour_angle_polar_nan(void)
{
    double dec = se_solar_declination(JD_SOLSTICE_JUN);
    double ha = se_sunrise_hour_angle(70.0, dec);
    TEST_ASSERT_TRUE(isnan(ha));
}

/* Solar ordering: 14-15 */

void test_sunrise_noon_sunset_order(void)
{
    solar_day_t sd = se_solar_events(JD_EQUINOX_MAR, 45.0, 0.0);
    TEST_ASSERT_TRUE(sd.sunrise_jd < sd.solar_noon_jd);
    TEST_ASSERT_TRUE(sd.solar_noon_jd < sd.sunset_jd);
}

void test_day_length_consistency(void)
{
    solar_day_t sd = se_solar_events(JD_EQUINOX_MAR, 45.0, 0.0);
    double computed_length = (sd.sunset_jd - sd.sunrise_jd) * 24.0;
    TEST_ASSERT_FLOAT_WITHIN(0.01f, (float)computed_length, (float)sd.day_length);
}

/* Equation of time: 16-17 */

void test_eot_magnitude(void)
{
    /* EoT should be within about +-17 minutes for any date */
    double eot1 = se_equation_of_time(JD_EQUINOX_MAR);
    double eot2 = se_equation_of_time(JD_SOLSTICE_JUN);
    double eot3 = se_equation_of_time(JD_SOLSTICE_DEC);
    TEST_ASSERT_TRUE(fabs(eot1) < 17.0);
    TEST_ASSERT_TRUE(fabs(eot2) < 17.0);
    TEST_ASSERT_TRUE(fabs(eot3) < 17.0);
}

void test_eot_varies(void)
{
    double eot1 = se_equation_of_time(JD_EQUINOX_MAR);
    double eot2 = se_equation_of_time(JD_SOLSTICE_JUN);
    TEST_ASSERT_TRUE(fabs(eot1 - eot2) > 0.1);
}

/* Twilight: 18-19 */

void test_civil_dawn_before_sunrise(void)
{
    solar_day_t sd = se_solar_events(JD_EQUINOX_MAR, 45.0, 0.0);
    twilight_t tw = se_twilight(JD_EQUINOX_MAR, 45.0, 0.0);
    TEST_ASSERT_FALSE(isnan(tw.civil_dawn_jd));
    TEST_ASSERT_TRUE(tw.civil_dawn_jd < sd.sunrise_jd);
}

void test_astro_dusk_after_civil_dusk(void)
{
    twilight_t tw = se_twilight(JD_EQUINOX_MAR, 45.0, 0.0);
    TEST_ASSERT_FALSE(isnan(tw.astro_dusk_jd));
    TEST_ASSERT_FALSE(isnan(tw.civil_dusk_jd));
    TEST_ASSERT_TRUE(tw.astro_dusk_jd > tw.civil_dusk_jd);
}

/* Purity: 20 */

void test_purity(void)
{
    solar_day_t a = se_solar_events(JD_EQUINOX_MAR, 40.0, -74.0);
    solar_day_t b = se_solar_events(JD_EQUINOX_MAR, 40.0, -74.0);
    TEST_ASSERT_EQUAL_FLOAT((float)a.sunrise_jd, (float)b.sunrise_jd);
    TEST_ASSERT_EQUAL_FLOAT((float)a.sunset_jd, (float)b.sunset_jd);
    TEST_ASSERT_EQUAL_FLOAT((float)a.day_length, (float)b.day_length);
}

/* Extra tests */

void test_solar_altitude_at_noon(void)
{
    /* At solar noon, altitude should be near maximum */
    solar_day_t sd = se_solar_events(JD_EQUINOX_MAR, 45.0, 0.0);
    double alt = se_solar_altitude(sd.solar_noon_jd, 45.0, 0.0);
    /* At equinox, 45N: altitude ~45 degrees at noon */
    TEST_ASSERT_FLOAT_WITHIN(5.0f, 45.0f, (float)alt);
}

void test_longitude_shifts_times(void)
{
    /* East longitude -> earlier sunrise */
    solar_day_t west = se_solar_events(JD_EQUINOX_MAR, 45.0, -90.0);
    solar_day_t east = se_solar_events(JD_EQUINOX_MAR, 45.0, 90.0);
    TEST_ASSERT_TRUE(east.sunrise_jd < west.sunrise_jd);
}

void test_southern_hemisphere_reversed(void)
{
    /* Southern hemisphere: June is winter, December is summer */
    solar_day_t jun_south = se_solar_events(JD_SOLSTICE_JUN, -45.0, 0.0);
    solar_day_t dec_south = se_solar_events(JD_SOLSTICE_DEC, -45.0, 0.0);
    TEST_ASSERT_TRUE(dec_south.day_length > jun_south.day_length);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_declination_equinox);
    RUN_TEST(test_declination_summer_solstice);
    RUN_TEST(test_declination_winter_solstice);
    RUN_TEST(test_equator_equinox_day_length);
    RUN_TEST(test_equator_equinox_valid);
    RUN_TEST(test_summer_45n_day_length);
    RUN_TEST(test_winter_45n_day_length);
    RUN_TEST(test_summer_longer_than_winter);
    RUN_TEST(test_arctic_summer_always_day);
    RUN_TEST(test_arctic_winter_always_night);
    RUN_TEST(test_antarctic_june_always_night);
    RUN_TEST(test_hour_angle_equator_equinox);
    RUN_TEST(test_hour_angle_polar_nan);
    RUN_TEST(test_sunrise_noon_sunset_order);
    RUN_TEST(test_day_length_consistency);
    RUN_TEST(test_eot_magnitude);
    RUN_TEST(test_eot_varies);
    RUN_TEST(test_civil_dawn_before_sunrise);
    RUN_TEST(test_astro_dusk_after_civil_dusk);
    RUN_TEST(test_purity);
    RUN_TEST(test_solar_altitude_at_noon);
    RUN_TEST(test_longitude_shifts_times);
    RUN_TEST(test_southern_hemisphere_reversed);
    return UNITY_END();
}
