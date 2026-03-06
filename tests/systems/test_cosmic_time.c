#include "../unity/unity.h"
#include "../../src/systems/astronomy/cosmic_time.h"

#include <math.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

#define J2000_JD 2451545.0

/* Cosmic context: 1-6 */

void test_universe_age(void)
{
    cosmic_context_t ctx = cosmic_context(J2000_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.1e9f, 13.787e9f, (float)ctx.universe_age_years);
}

void test_sun_age(void)
{
    cosmic_context_t ctx = cosmic_context(J2000_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.1e9f, 4.603e9f, (float)ctx.sun_age_years);
}

void test_earth_age(void)
{
    cosmic_context_t ctx = cosmic_context(J2000_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.1e9f, 4.543e9f, (float)ctx.earth_age_years);
}

void test_galactic_year(void)
{
    cosmic_context_t ctx = cosmic_context(J2000_JD);
    TEST_ASSERT_FLOAT_WITHIN(25.0e6f, 225.0e6f, (float)ctx.galactic_year_years);
}

void test_galactic_orbits(void)
{
    cosmic_context_t ctx = cosmic_context(J2000_JD);
    /* Sun age / galactic year ≈ 4.603e9 / 225e6 ≈ 20.5 */
    TEST_ASSERT_FLOAT_WITHIN(2.0f, 20.5f, (float)ctx.sun_galactic_orbits);
}

void test_galactic_phase_range(void)
{
    cosmic_context_t ctx = cosmic_context(J2000_JD);
    TEST_ASSERT_TRUE(ctx.sun_galactic_phase >= 0.0);
    TEST_ASSERT_TRUE(ctx.sun_galactic_phase < 1.0);
}

/* Format age: 7-11 */

void test_format_gyr(void)
{
    char buf[64];
    cosmic_format_age(13.787e9, buf, sizeof(buf));
    TEST_ASSERT_TRUE(strstr(buf, "Gyr") != NULL);
    TEST_ASSERT_TRUE(strstr(buf, "13.787") != NULL);
}

void test_format_myr(void)
{
    char buf[64];
    cosmic_format_age(4.5e6, buf, sizeof(buf));
    TEST_ASSERT_TRUE(strstr(buf, "Myr") != NULL);
}

void test_format_kyr(void)
{
    char buf[64];
    cosmic_format_age(11700.0, buf, sizeof(buf));
    TEST_ASSERT_TRUE(strstr(buf, "kyr") != NULL);
}

void test_format_yr(void)
{
    char buf[64];
    cosmic_format_age(365.0, buf, sizeof(buf));
    TEST_ASSERT_TRUE(strstr(buf, "yr") != NULL);
    TEST_ASSERT_TRUE(strstr(buf, "kyr") == NULL);
}

void test_format_null_safe(void)
{
    /* Should not crash */
    cosmic_format_age(1.0e9, NULL, 0);
}

/* Geological eras: 12-16 */

void test_era_holocene(void)
{
    TEST_ASSERT_EQUAL_STRING("Holocene", cosmic_geological_era(0.0));
    TEST_ASSERT_EQUAL_STRING("Holocene", cosmic_geological_era(5000.0));
}

void test_era_pleistocene(void)
{
    TEST_ASSERT_EQUAL_STRING("Pleistocene", cosmic_geological_era(100000.0));
}

void test_era_cretaceous(void)
{
    TEST_ASSERT_EQUAL_STRING("Cretaceous", cosmic_geological_era(66.0e6));
    TEST_ASSERT_EQUAL_STRING("Cretaceous", cosmic_geological_era(100.0e6));
}

void test_era_permian(void)
{
    TEST_ASSERT_EQUAL_STRING("Permian", cosmic_geological_era(252.0e6));
}

void test_era_hadean(void)
{
    TEST_ASSERT_EQUAL_STRING("Hadean", cosmic_geological_era(4.2e9));
}

/* Universe fraction: 17 */

void test_universe_fraction(void)
{
    double frac = cosmic_universe_fraction(J2000_JD);
    /* At J2000 (year 2000), fraction should be very close to 1.0 */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, (float)frac);
}

/* Distance labels: 18-19 */

void test_distance_au(void)
{
    char buf[64];
    cosmic_distance_label(5.0f, buf, sizeof(buf));
    TEST_ASSERT_TRUE(strstr(buf, "AU") != NULL);
}

void test_distance_ly(void)
{
    char buf[64];
    cosmic_distance_label(100.0f, buf, sizeof(buf));
    TEST_ASSERT_TRUE(strstr(buf, "ly") != NULL);
}

/* Unit constants: 20-22 */

void test_seconds_per_day(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 86400.0f, (float)cosmic_seconds_per_day());
}

void test_seconds_per_year(void)
{
    double spy = cosmic_seconds_per_year();
    /* 365.25 * 86400 = 31,557,600 */
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 31557600.0f, (float)spy);
}

void test_days_per_year(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 365.25f, (float)cosmic_days_per_year());
}

/* Purity: 23 */

void test_purity(void)
{
    cosmic_context_t a = cosmic_context(2460000.0);
    cosmic_context_t b = cosmic_context(2460000.0);
    TEST_ASSERT_EQUAL_FLOAT((float)a.universe_age_years, (float)b.universe_age_years);
    TEST_ASSERT_EQUAL_FLOAT((float)a.sun_galactic_phase, (float)b.sun_galactic_phase);
}

/* Extra */

void test_era_future(void)
{
    TEST_ASSERT_EQUAL_STRING("Future", cosmic_geological_era(-100.0));
}

void test_distance_kly(void)
{
    char buf[64];
    cosmic_distance_label(1000.0f, buf, sizeof(buf));
    TEST_ASSERT_TRUE(strstr(buf, "kly") != NULL);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_universe_age);
    RUN_TEST(test_sun_age);
    RUN_TEST(test_earth_age);
    RUN_TEST(test_galactic_year);
    RUN_TEST(test_galactic_orbits);
    RUN_TEST(test_galactic_phase_range);
    RUN_TEST(test_format_gyr);
    RUN_TEST(test_format_myr);
    RUN_TEST(test_format_kyr);
    RUN_TEST(test_format_yr);
    RUN_TEST(test_format_null_safe);
    RUN_TEST(test_era_holocene);
    RUN_TEST(test_era_pleistocene);
    RUN_TEST(test_era_cretaceous);
    RUN_TEST(test_era_permian);
    RUN_TEST(test_era_hadean);
    RUN_TEST(test_universe_fraction);
    RUN_TEST(test_distance_au);
    RUN_TEST(test_distance_ly);
    RUN_TEST(test_seconds_per_day);
    RUN_TEST(test_seconds_per_year);
    RUN_TEST(test_days_per_year);
    RUN_TEST(test_purity);
    RUN_TEST(test_era_future);
    RUN_TEST(test_distance_kly);
    return UNITY_END();
}
