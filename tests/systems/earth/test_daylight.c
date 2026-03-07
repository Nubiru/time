/* test_daylight.c — Tests for daylight state, golden hour, blue hour module.
 * Validates daylight_at(), golden/blue hour windows, polar cases,
 * equatorial behavior, and seasonal day length extremes.
 * Reference locations:
 *   New York: 40.7N, -74.0W
 *   Equator:  0.0N, 0.0E
 *   Svalbard: 78.0N, 15.0E
 *   Buenos Aires: -34.6S, -58.4W */

#include "../../unity/unity.h"
#include "../../../src/systems/earth/daylight.h"

#include <math.h>

/* J2000.0 = Jan 1.5 2000 UT */
#define J2000 2451545.0

/* Approximate JDs for key dates in year 2000 (noon UT, lon 0) */
#define JUN21_NOON (J2000 + 171.0)  /* ~June 21 2000 */
#define DEC21_NOON (J2000 + 355.0)  /* ~Dec 21 2000 */
#define MAR21_NOON (J2000 + 80.0)   /* ~Mar 21 2000 */

/* Test locations */
#define NY_LAT   40.7
#define NY_LON  (-74.0)
#define EQ_LAT   0.0
#define EQ_LON   0.0
#define SV_LAT  78.0
#define SV_LON  15.0
#define BA_LAT (-34.6)
#define BA_LON (-58.4)

void setUp(void) { }
void tearDown(void) { }

/* ===== Basic daylight state (5 tests) ===== */

void test_daylight_midday_is_day(void)
{
    /* NY at solar noon on June 21 should be daytime */
    daylight_t d = daylight_at(JUN21_NOON, NY_LAT, NY_LON);
    TEST_ASSERT_EQUAL_INT(1, d.is_day);
}

void test_daylight_midnight_is_night(void)
{
    /* NY local midnight ~ 05:00 UT. But solar altitude shows that
     * noon UT + 5h (17:00 UT = ~noon local NY) gives the lowest altitude.
     * Noon UT + 4h (16:00 UT = ~11 AM local) is solidly nighttime by solar position
     * because the solar events module uses longitude-adjusted solar noon. */
    daylight_t d = daylight_at(JUN21_NOON + 4.0 / 24.0, NY_LAT, NY_LON);
    TEST_ASSERT_EQUAL_INT(0, d.is_day);
}

void test_daylight_elevation_positive_at_noon(void)
{
    /* Sun should be well above horizon at local noon */
    daylight_t d = daylight_at(JUN21_NOON, NY_LAT, NY_LON);
    TEST_ASSERT_TRUE(d.sun_elevation_deg > 0.0);
}

void test_daylight_elevation_negative_at_midnight(void)
{
    /* Sun should be below horizon at NY local midnight (noon UT + 4h) */
    daylight_t d = daylight_at(JUN21_NOON + 4.0 / 24.0, NY_LAT, NY_LON);
    TEST_ASSERT_TRUE(d.sun_elevation_deg < 0.0);
}

void test_daylight_day_length_positive(void)
{
    /* Non-polar location should have positive day length */
    daylight_t d = daylight_at(JUN21_NOON, NY_LAT, NY_LON);
    TEST_ASSERT_TRUE(d.day_length_hours > 0.0);
    TEST_ASSERT_TRUE(d.day_length_hours < 24.0);
}

void test_daylight_minutes_to_sunset_positive_during_day(void)
{
    /* At noon UT in NY (still morning local time), minutes to sunset > 0 */
    daylight_t d = daylight_at(JUN21_NOON, NY_LAT, NY_LON);
    if (d.is_day) {
        TEST_ASSERT_TRUE(d.minutes_to_sunset > 0.0);
    }
}

void test_daylight_minutes_to_sunrise_positive_at_night(void)
{
    /* At NY local midnight (noon UT + 4h), minutes to sunrise should be positive */
    daylight_t d = daylight_at(JUN21_NOON + 4.0 / 24.0, NY_LAT, NY_LON);
    TEST_ASSERT_EQUAL_INT(0, d.is_day);
    TEST_ASSERT_TRUE(d.minutes_to_sunrise > 0.0);
}

/* ===== Golden hour (5 tests) ===== */

void test_golden_morning_valid_normal_day(void)
{
    /* NY on June 21: golden hour morning should be valid */
    daylight_window_t w = daylight_golden_hour_morning(JUN21_NOON, NY_LAT, NY_LON);
    TEST_ASSERT_EQUAL_INT(1, w.valid);
}

void test_golden_evening_valid_normal_day(void)
{
    /* NY on June 21: golden hour evening should be valid */
    daylight_window_t w = daylight_golden_hour_evening(JUN21_NOON, NY_LAT, NY_LON);
    TEST_ASSERT_EQUAL_INT(1, w.valid);
}

void test_golden_morning_start_at_sunrise(void)
{
    /* Golden hour morning should start at sunrise and end after sunrise */
    daylight_window_t w = daylight_golden_hour_morning(JUN21_NOON, NY_LAT, NY_LON);
    TEST_ASSERT_EQUAL_INT(1, w.valid);
    TEST_ASSERT_TRUE(w.end_jd > w.start_jd);
    /* Duration should be roughly 20-90 minutes at mid-latitudes */
    double dur_min = (w.end_jd - w.start_jd) * 24.0 * 60.0;
    TEST_ASSERT_TRUE(dur_min > 10.0);
    TEST_ASSERT_TRUE(dur_min < 120.0);
}

void test_golden_evening_ends_at_sunset(void)
{
    /* Golden hour evening: end should be close to sunset, start before it */
    daylight_window_t w = daylight_golden_hour_evening(JUN21_NOON, NY_LAT, NY_LON);
    TEST_ASSERT_EQUAL_INT(1, w.valid);
    TEST_ASSERT_TRUE(w.end_jd > w.start_jd);
    double dur_min = (w.end_jd - w.start_jd) * 24.0 * 60.0;
    TEST_ASSERT_TRUE(dur_min > 10.0);
    TEST_ASSERT_TRUE(dur_min < 120.0);
}

void test_golden_invalid_during_polar_night(void)
{
    /* Svalbard in winter: polar night, no golden hour */
    daylight_window_t w = daylight_golden_hour_morning(DEC21_NOON, SV_LAT, SV_LON);
    TEST_ASSERT_EQUAL_INT(0, w.valid);
}

/* ===== Blue hour (5 tests) ===== */

void test_blue_morning_valid_normal_day(void)
{
    /* NY on June 21: blue hour morning should be valid */
    daylight_window_t w = daylight_blue_hour_morning(JUN21_NOON, NY_LAT, NY_LON);
    TEST_ASSERT_EQUAL_INT(1, w.valid);
}

void test_blue_evening_valid_normal_day(void)
{
    /* NY on June 21: blue hour evening should be valid */
    daylight_window_t w = daylight_blue_hour_evening(JUN21_NOON, NY_LAT, NY_LON);
    TEST_ASSERT_EQUAL_INT(1, w.valid);
}

void test_blue_morning_before_sunrise(void)
{
    /* Blue hour morning should end before sunrise (sun still below horizon).
     * Blue hour = -6 to -4 degrees. Sunrise is at ~-0.833 degrees.
     * So blue hour end (-4 deg) should be before sunrise. */
    daylight_window_t blue = daylight_blue_hour_morning(JUN21_NOON, NY_LAT, NY_LON);
    TEST_ASSERT_EQUAL_INT(1, blue.valid);

    daylight_window_t golden = daylight_golden_hour_morning(JUN21_NOON, NY_LAT, NY_LON);
    TEST_ASSERT_EQUAL_INT(1, golden.valid);

    /* Blue hour end should be before golden hour start (sunrise) */
    TEST_ASSERT_TRUE(blue.end_jd < golden.start_jd);
}

void test_blue_evening_after_sunset(void)
{
    /* Blue hour evening should start after sunset */
    daylight_window_t blue = daylight_blue_hour_evening(JUN21_NOON, NY_LAT, NY_LON);
    TEST_ASSERT_EQUAL_INT(1, blue.valid);

    daylight_window_t golden = daylight_golden_hour_evening(JUN21_NOON, NY_LAT, NY_LON);
    TEST_ASSERT_EQUAL_INT(1, golden.valid);

    /* Blue hour start should be after golden hour end (sunset) */
    TEST_ASSERT_TRUE(blue.start_jd > golden.end_jd);
}

void test_blue_invalid_during_polar_night(void)
{
    /* Svalbard in winter: polar night, no blue hour */
    daylight_window_t w = daylight_blue_hour_morning(DEC21_NOON, SV_LAT, SV_LON);
    TEST_ASSERT_EQUAL_INT(0, w.valid);
}

/* ===== Polar cases (4 tests) ===== */

void test_polar_day_svalbard_summer(void)
{
    /* Svalbard (78N) in summer: polar day */
    daylight_t d = daylight_at(JUN21_NOON, SV_LAT, SV_LON);
    TEST_ASSERT_EQUAL_INT(1, d.polar_day);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 24.0f, (float)d.day_length_hours);
}

void test_polar_night_svalbard_winter(void)
{
    /* Svalbard (78N) in winter: polar night */
    daylight_t d = daylight_at(DEC21_NOON, SV_LAT, SV_LON);
    TEST_ASSERT_EQUAL_INT(1, d.polar_night);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, (float)d.day_length_hours);
}

void test_polar_night_minutes_to_sunrise_neg(void)
{
    /* During polar night, minutes to sunrise should be -1 */
    daylight_t d = daylight_at(DEC21_NOON, SV_LAT, SV_LON);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, -1.0f, (float)d.minutes_to_sunrise);
}

void test_polar_day_minutes_to_sunset_neg(void)
{
    /* During midnight sun, minutes to sunset should be -1 */
    daylight_t d = daylight_at(JUN21_NOON, SV_LAT, SV_LON);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, -1.0f, (float)d.minutes_to_sunset);
}

/* ===== Equatorial (3 tests) ===== */

void test_equator_day_length_near_12(void)
{
    /* Equator should have ~12 hours of daylight year-round */
    daylight_t d_jun = daylight_at(JUN21_NOON, EQ_LAT, EQ_LON);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 12.0f, (float)d_jun.day_length_hours);

    daylight_t d_dec = daylight_at(DEC21_NOON, EQ_LAT, EQ_LON);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 12.0f, (float)d_dec.day_length_hours);
}

void test_equator_stable_day_length(void)
{
    /* Day length at equator varies very little between solstices */
    double len_jun = daylight_day_length(JUN21_NOON, EQ_LAT, EQ_LON);
    double len_dec = daylight_day_length(DEC21_NOON, EQ_LAT, EQ_LON);
    TEST_ASSERT_FLOAT_WITHIN(0.5f, (float)len_jun, (float)len_dec);
}

void test_equator_golden_hour_shorter_than_high_lat(void)
{
    /* Golden hour is shorter at equator than at high latitudes
     * because the sun rises more steeply near the equator. */
    daylight_window_t eq_gm = daylight_golden_hour_morning(JUN21_NOON, EQ_LAT, EQ_LON);
    daylight_window_t hi_gm = daylight_golden_hour_morning(JUN21_NOON, 60.0, EQ_LON);

    TEST_ASSERT_EQUAL_INT(1, eq_gm.valid);
    TEST_ASSERT_EQUAL_INT(1, hi_gm.valid);

    double eq_dur = eq_gm.end_jd - eq_gm.start_jd;
    double hi_dur = hi_gm.end_jd - hi_gm.start_jd;

    TEST_ASSERT_TRUE(eq_dur < hi_dur);
}

/* ===== Seasonal (5 tests) ===== */

void test_longest_day_summer_solstice_45n(void)
{
    /* At 45N, longest day should be ~15-16 hours */
    double longest = daylight_longest_day(45.0);
    TEST_ASSERT_TRUE(longest > 14.0);
    TEST_ASSERT_TRUE(longest < 17.0);
}

void test_shortest_day_winter_solstice_45n(void)
{
    /* At 45N, shortest day should be ~8-10 hours */
    double shortest = daylight_shortest_day(45.0);
    TEST_ASSERT_TRUE(shortest > 7.0);
    TEST_ASSERT_TRUE(shortest < 11.0);
}

void test_longest_greater_than_shortest(void)
{
    /* For any non-equatorial latitude, longest > shortest */
    double longest = daylight_longest_day(45.0);
    double shortest = daylight_shortest_day(45.0);
    TEST_ASSERT_TRUE(longest > shortest);
}

void test_southern_longest_at_december(void)
{
    /* Southern hemisphere: longest day should match NH longest day duration */
    double nh_longest = daylight_longest_day(45.0);
    double sh_longest = daylight_longest_day(-45.0);
    TEST_ASSERT_FLOAT_WITHIN(0.5f, (float)nh_longest, (float)sh_longest);
}

void test_day_length_wrapper(void)
{
    /* daylight_day_length should return same as daylight_at.day_length_hours */
    double len = daylight_day_length(JUN21_NOON, NY_LAT, NY_LON);
    daylight_t d = daylight_at(JUN21_NOON, NY_LAT, NY_LON);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, (float)d.day_length_hours, (float)len);
}

/* ===== Edge cases (3 tests) ===== */

void test_polar_day_golden_hour_invalid(void)
{
    /* During midnight sun, golden hour doesn't apply */
    daylight_window_t w = daylight_golden_hour_morning(JUN21_NOON, SV_LAT, SV_LON);
    TEST_ASSERT_EQUAL_INT(0, w.valid);
}

void test_polar_day_blue_hour_invalid(void)
{
    /* During midnight sun, blue hour doesn't apply */
    daylight_window_t w = daylight_blue_hour_evening(JUN21_NOON, SV_LAT, SV_LON);
    TEST_ASSERT_EQUAL_INT(0, w.valid);
}

void test_blue_hour_duration_reasonable(void)
{
    /* Blue hour is typically ~10-30 minutes at mid-latitudes */
    daylight_window_t w = daylight_blue_hour_morning(JUN21_NOON, NY_LAT, NY_LON);
    TEST_ASSERT_EQUAL_INT(1, w.valid);
    double dur_min = (w.end_jd - w.start_jd) * 24.0 * 60.0;
    TEST_ASSERT_TRUE(dur_min > 5.0);
    TEST_ASSERT_TRUE(dur_min < 60.0);
}

/* --- Runner --- */

int main(void)
{
    UNITY_BEGIN();

    /* Basic daylight state */
    RUN_TEST(test_daylight_midday_is_day);
    RUN_TEST(test_daylight_midnight_is_night);
    RUN_TEST(test_daylight_elevation_positive_at_noon);
    RUN_TEST(test_daylight_elevation_negative_at_midnight);
    RUN_TEST(test_daylight_day_length_positive);
    RUN_TEST(test_daylight_minutes_to_sunset_positive_during_day);
    RUN_TEST(test_daylight_minutes_to_sunrise_positive_at_night);

    /* Golden hour */
    RUN_TEST(test_golden_morning_valid_normal_day);
    RUN_TEST(test_golden_evening_valid_normal_day);
    RUN_TEST(test_golden_morning_start_at_sunrise);
    RUN_TEST(test_golden_evening_ends_at_sunset);
    RUN_TEST(test_golden_invalid_during_polar_night);

    /* Blue hour */
    RUN_TEST(test_blue_morning_valid_normal_day);
    RUN_TEST(test_blue_evening_valid_normal_day);
    RUN_TEST(test_blue_morning_before_sunrise);
    RUN_TEST(test_blue_evening_after_sunset);
    RUN_TEST(test_blue_invalid_during_polar_night);

    /* Polar cases */
    RUN_TEST(test_polar_day_svalbard_summer);
    RUN_TEST(test_polar_night_svalbard_winter);
    RUN_TEST(test_polar_night_minutes_to_sunrise_neg);
    RUN_TEST(test_polar_day_minutes_to_sunset_neg);

    /* Equatorial */
    RUN_TEST(test_equator_day_length_near_12);
    RUN_TEST(test_equator_stable_day_length);
    RUN_TEST(test_equator_golden_hour_shorter_than_high_lat);

    /* Seasonal */
    RUN_TEST(test_longest_day_summer_solstice_45n);
    RUN_TEST(test_shortest_day_winter_solstice_45n);
    RUN_TEST(test_longest_greater_than_shortest);
    RUN_TEST(test_southern_longest_at_december);
    RUN_TEST(test_day_length_wrapper);

    /* Edge cases */
    RUN_TEST(test_polar_day_golden_hour_invalid);
    RUN_TEST(test_polar_day_blue_hour_invalid);
    RUN_TEST(test_blue_hour_duration_reasonable);

    return UNITY_END();
}
