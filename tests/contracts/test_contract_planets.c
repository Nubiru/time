/* test_contract_planets.c — Exhaustive contract tests for planets.h */

#include "../unity/unity.h"
#include "../../src/systems/astronomy/planets.h"
#include <math.h>
#include <string.h>

#define J2000 2451545.0

void setUp(void) { }
void tearDown(void) { }

void test_all_positions_finite_at_j2000(void) {
    solar_system_t ss = planets_at(J2000);
    for (int i = 0; i < 8; i++) {
        TEST_ASSERT_TRUE(isfinite(ss.positions[i].longitude));
        TEST_ASSERT_TRUE(isfinite(ss.positions[i].latitude));
        TEST_ASSERT_TRUE(isfinite(ss.positions[i].distance));
    }
}

void test_planet_name_valid_range(void) {
    for (int i = 0; i < 8; i++) {
        const char *name = planet_name(i);
        TEST_ASSERT_NOT_NULL(name);
        TEST_ASSERT_TRUE(strcmp(name, "Unknown") != 0);
    }
}

void test_planet_name_invalid_low(void) {
    TEST_ASSERT_EQUAL_STRING("Unknown", planet_name(-1));
}

void test_planet_name_invalid_high(void) {
    TEST_ASSERT_EQUAL_STRING("Unknown", planet_name(8));
}

void test_planet_symbol_valid_range(void) {
    for (int i = 0; i < 8; i++) { TEST_ASSERT_NOT_NULL(planet_symbol(i)); }
}

void test_mercury_distance_bound(void) {
    solar_system_t ss = planets_at(J2000);
    TEST_ASSERT_TRUE(ss.positions[PLANET_MERCURY].distance < 1.0);
    TEST_ASSERT_TRUE(ss.positions[PLANET_MERCURY].distance > 0.0);
}

void test_jupiter_distance_bound(void) {
    solar_system_t ss = planets_at(J2000);
    TEST_ASSERT_TRUE(ss.positions[PLANET_JUPITER].distance > 4.0);
}

void test_neptune_distance_bound(void) {
    solar_system_t ss = planets_at(J2000);
    TEST_ASSERT_TRUE(ss.positions[PLANET_NEPTUNE].distance > 25.0);
}

void test_earth_distance_approx(void) {
    solar_system_t ss = planets_at(J2000);
    TEST_ASSERT_FLOAT_WITHIN(0.05, 1.0, ss.positions[PLANET_EARTH].distance);
}

void test_fifty_year_sweep_all_finite(void) {
    int failures = 0;
    for (double jd = J2000; jd < J2000 + 18262.5; jd += 30.44) {
        solar_system_t ss = planets_at(jd);
        for (int i = 0; i < 8; i++) {
            if (!isfinite(ss.positions[i].longitude) || !isfinite(ss.positions[i].latitude) ||
                !isfinite(ss.positions[i].distance)) failures++;
        }
    }
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, failures, "Non-finite in 50-year sweep");
}

void test_fifty_year_sweep_distance_positive(void) {
    int failures = 0;
    for (double jd = J2000; jd < J2000 + 18262.5; jd += 30.44) {
        solar_system_t ss = planets_at(jd);
        for (int i = 0; i < 8; i++) {
            if (ss.positions[i].distance <= 0.0) failures++;
        }
    }
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, failures, "Non-positive distance in sweep");
}

void test_fifty_year_sweep_longitude_range(void) {
    int failures = 0;
    for (double jd = J2000; jd < J2000 + 18262.5; jd += 30.44) {
        solar_system_t ss = planets_at(jd);
        for (int i = 0; i < 8; i++) {
            if (ss.positions[i].longitude < 0.0 || ss.positions[i].longitude >= 360.0) failures++;
        }
    }
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, failures, "Longitude out of [0, 360)");
}

void test_planet_name_order(void) {
    TEST_ASSERT_EQUAL_STRING("Mercury", planet_name(PLANET_MERCURY));
    TEST_ASSERT_EQUAL_STRING("Venus", planet_name(PLANET_VENUS));
    TEST_ASSERT_EQUAL_STRING("Earth", planet_name(PLANET_EARTH));
    TEST_ASSERT_EQUAL_STRING("Mars", planet_name(PLANET_MARS));
    TEST_ASSERT_EQUAL_STRING("Jupiter", planet_name(PLANET_JUPITER));
    TEST_ASSERT_EQUAL_STRING("Saturn", planet_name(PLANET_SATURN));
    TEST_ASSERT_EQUAL_STRING("Uranus", planet_name(PLANET_URANUS));
    TEST_ASSERT_EQUAL_STRING("Neptune", planet_name(PLANET_NEPTUNE));
}

void test_planet_symbol_invalid(void) {
    TEST_ASSERT_NOT_NULL(planet_symbol(-1));
    TEST_ASSERT_NOT_NULL(planet_symbol(8));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_all_positions_finite_at_j2000);
    RUN_TEST(test_planet_name_valid_range);
    RUN_TEST(test_planet_name_invalid_low);
    RUN_TEST(test_planet_name_invalid_high);
    RUN_TEST(test_planet_symbol_valid_range);
    RUN_TEST(test_mercury_distance_bound);
    RUN_TEST(test_jupiter_distance_bound);
    RUN_TEST(test_neptune_distance_bound);
    RUN_TEST(test_earth_distance_approx);
    RUN_TEST(test_fifty_year_sweep_all_finite);
    RUN_TEST(test_fifty_year_sweep_distance_positive);
    RUN_TEST(test_fifty_year_sweep_longitude_range);
    RUN_TEST(test_planet_name_order);
    RUN_TEST(test_planet_symbol_invalid);
    return UNITY_END();
}
