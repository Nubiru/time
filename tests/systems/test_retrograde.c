#include "../unity/unity.h"
#include "../../src/systems/astronomy/retrograde.h"

#include <math.h>

void setUp(void) {}
void tearDown(void) {}

/* J2000 epoch */
#define J2000_JD 2451545.0

/* Planet indices */
#define P_MERCURY 0
#define P_VENUS   1
#define P_EARTH   2
#define P_MARS    3
#define P_JUPITER 4
#define P_SATURN  5
#define P_URANUS  6
#define P_NEPTUNE 7

/* Earth never retrograde: 1-2 */

void test_earth_never_retrograde(void)
{
    /* Test over several years */
    for (double jd = J2000_JD; jd < J2000_JD + 3650.0; jd += 30.0) {
        TEST_ASSERT_FALSE(is_retrograde(P_EARTH, jd));
    }
}

void test_earth_daily_motion_positive(void)
{
    /* Earth's heliocentric motion is always prograde */
    double dm = planet_daily_motion(P_EARTH, J2000_JD);
    TEST_ASSERT_TRUE(dm > 0.0);
}

/* Daily motion sign matches motion type: 3-4 */

void test_motion_type_matches_daily_motion(void)
{
    /* Check that retrograde_check's motion field is consistent with daily_motion sign */
    retrograde_info_t info = retrograde_check(P_MERCURY, J2000_JD);
    if (info.motion == MOTION_RETROGRADE) {
        TEST_ASSERT_TRUE(info.daily_motion < 0.0);
    } else if (info.motion == MOTION_DIRECT) {
        TEST_ASSERT_TRUE(info.daily_motion > 0.0);
    }
    /* Stationary can have either very small sign */
}

void test_retrograde_info_planet_field(void)
{
    retrograde_info_t info = retrograde_check(P_MARS, J2000_JD);
    TEST_ASSERT_EQUAL_INT(P_MARS, info.planet);
}

/* Outer planets can be retrograde: 5-7 */

void test_outer_planets_sometimes_retrograde(void)
{
    /* Scan Jupiter over 12 years — it should be retrograde at some point */
    int found_retro = 0;
    for (double jd = J2000_JD; jd < J2000_JD + 4380.0; jd += 10.0) {
        if (is_retrograde(P_JUPITER, jd)) {
            found_retro = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found_retro);
}

void test_saturn_sometimes_retrograde(void)
{
    int found_retro = 0;
    for (double jd = J2000_JD; jd < J2000_JD + 4380.0; jd += 10.0) {
        if (is_retrograde(P_SATURN, jd)) {
            found_retro = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found_retro);
}

void test_mars_sometimes_retrograde(void)
{
    /* Mars retrograde every ~26 months, scan 3 years */
    int found_retro = 0;
    for (double jd = J2000_JD; jd < J2000_JD + 1100.0; jd += 5.0) {
        if (is_retrograde(P_MARS, jd)) {
            found_retro = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found_retro);
}

/* Motion names: 8-10 */

void test_motion_name_direct(void)
{
    TEST_ASSERT_EQUAL_STRING("Direct", motion_name(MOTION_DIRECT));
}

void test_motion_name_stationary(void)
{
    TEST_ASSERT_EQUAL_STRING("Stationary", motion_name(MOTION_STATIONARY));
}

void test_motion_name_retrograde(void)
{
    TEST_ASSERT_EQUAL_STRING("Retrograde", motion_name(MOTION_RETROGRADE));
}

/* Symbols: 11-13 */

void test_symbol_direct(void)
{
    TEST_ASSERT_EQUAL_STRING("D", motion_symbol(MOTION_DIRECT));
}

void test_symbol_stationary(void)
{
    TEST_ASSERT_EQUAL_STRING("S", motion_symbol(MOTION_STATIONARY));
}

void test_symbol_retrograde(void)
{
    const char *s = motion_symbol(MOTION_RETROGRADE);
    TEST_ASSERT_NOT_NULL(s);
    TEST_ASSERT_TRUE(s[0] != '\0');
    /* ℞ is 3 bytes in UTF-8 */
    TEST_ASSERT_TRUE(s[0] != 'D' && s[0] != 'S');
}

/* Stationary thresholds: 14-16 */

void test_threshold_mercury(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, (float)retrograde_stationary_threshold(P_MERCURY));
}

void test_threshold_earth_zero(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.0f, (float)retrograde_stationary_threshold(P_EARTH));
}

void test_threshold_invalid_planet(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.0f, (float)retrograde_stationary_threshold(-1));
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.0f, (float)retrograde_stationary_threshold(8));
}

/* Invalid input: 17 */

void test_invalid_planet_index(void)
{
    retrograde_info_t info = retrograde_check(-1, J2000_JD);
    TEST_ASSERT_EQUAL_INT(MOTION_DIRECT, info.motion);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)info.daily_motion);

    info = retrograde_check(99, J2000_JD);
    TEST_ASSERT_EQUAL_INT(MOTION_DIRECT, info.motion);
}

/* Daily motion magnitude reasonable: 18-19 */

void test_mercury_daily_motion_magnitude(void)
{
    /* Mercury's geocentric daily motion: typically 0-2°/day, can be negative */
    double dm = planet_daily_motion(P_MERCURY, J2000_JD);
    TEST_ASSERT_TRUE(fabs(dm) < 3.0);
}

void test_neptune_daily_motion_small(void)
{
    /* Neptune moves very slowly: < 0.1°/day */
    double dm = planet_daily_motion(P_NEPTUNE, J2000_JD);
    TEST_ASSERT_TRUE(fabs(dm) < 0.1);
}

/* Purity: 20 */

void test_purity(void)
{
    retrograde_info_t a = retrograde_check(P_MARS, 2460000.0);
    retrograde_info_t b = retrograde_check(P_MARS, 2460000.0);
    TEST_ASSERT_EQUAL_FLOAT((float)a.daily_motion, (float)b.daily_motion);
    TEST_ASSERT_EQUAL_INT(a.motion, b.motion);

    double dm1 = planet_daily_motion(P_JUPITER, J2000_JD);
    double dm2 = planet_daily_motion(P_JUPITER, J2000_JD);
    TEST_ASSERT_EQUAL_FLOAT((float)dm1, (float)dm2);
}

/* Direct motion planets: 21 */

void test_planets_mostly_direct(void)
{
    /* At a random date, most planets should be direct */
    int direct_count = 0;
    for (int p = 0; p < 8; p++) {
        if (!is_retrograde(p, J2000_JD)) direct_count++;
    }
    /* At least 5 of 8 planets should be direct at any given time */
    TEST_ASSERT_TRUE(direct_count >= 5);
}

/* Convenience wrapper consistency: 22 */

void test_is_retrograde_matches_check(void)
{
    for (int p = 0; p < 8; p++) {
        retrograde_info_t info = retrograde_check(p, J2000_JD);
        int retro = is_retrograde(p, J2000_JD);
        if (info.motion == MOTION_RETROGRADE) {
            TEST_ASSERT_TRUE(retro);
        } else {
            TEST_ASSERT_FALSE(retro);
        }
    }
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_earth_never_retrograde);
    RUN_TEST(test_earth_daily_motion_positive);
    RUN_TEST(test_motion_type_matches_daily_motion);
    RUN_TEST(test_retrograde_info_planet_field);
    RUN_TEST(test_outer_planets_sometimes_retrograde);
    RUN_TEST(test_saturn_sometimes_retrograde);
    RUN_TEST(test_mars_sometimes_retrograde);
    RUN_TEST(test_motion_name_direct);
    RUN_TEST(test_motion_name_stationary);
    RUN_TEST(test_motion_name_retrograde);
    RUN_TEST(test_symbol_direct);
    RUN_TEST(test_symbol_stationary);
    RUN_TEST(test_symbol_retrograde);
    RUN_TEST(test_threshold_mercury);
    RUN_TEST(test_threshold_earth_zero);
    RUN_TEST(test_threshold_invalid_planet);
    RUN_TEST(test_invalid_planet_index);
    RUN_TEST(test_mercury_daily_motion_magnitude);
    RUN_TEST(test_neptune_daily_motion_small);
    RUN_TEST(test_purity);
    RUN_TEST(test_planets_mostly_direct);
    RUN_TEST(test_is_retrograde_matches_check);
    return UNITY_END();
}
