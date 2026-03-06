#include "../unity/unity.h"
#include "../../src/systems/astronomy/planets.h"

#define DIST_EPSILON 0.02  /* AU tolerance */
#define J2000_JD 2451545.0

void setUp(void) { }
void tearDown(void) { }

void test_all_positions_valid_at_j2000(void) {
    solar_system_t sys = planets_at(J2000_JD);
    for (int i = 0; i < 8; i++) {
        TEST_ASSERT_TRUE(sys.positions[i].longitude >= 0.0);
        TEST_ASSERT_TRUE(sys.positions[i].longitude < 360.0);
        TEST_ASSERT_TRUE(sys.positions[i].latitude >= -90.0);
        TEST_ASSERT_TRUE(sys.positions[i].latitude <= 90.0);
        TEST_ASSERT_TRUE(sys.positions[i].distance > 0.0);
    }
}

void test_earth_distance_at_j2000(void) {
    /* Earth near perihelion at J2000 (Jan 1.5), ~0.983 AU */
    solar_system_t sys = planets_at(J2000_JD);
    TEST_ASSERT_FLOAT_WITHIN(DIST_EPSILON, 0.983, sys.positions[PLANET_EARTH].distance);
}

void test_mercury_closer_than_neptune(void) {
    solar_system_t sys = planets_at(J2000_JD);
    TEST_ASSERT_TRUE(sys.positions[PLANET_MERCURY].distance < sys.positions[PLANET_NEPTUNE].distance);
}

void test_planet_name_mercury(void) {
    TEST_ASSERT_EQUAL_STRING("Mercury", planet_name(PLANET_MERCURY));
}

void test_planet_name_neptune(void) {
    TEST_ASSERT_EQUAL_STRING("Neptune", planet_name(PLANET_NEPTUNE));
}

void test_planet_name_invalid(void) {
    TEST_ASSERT_EQUAL_STRING("Unknown", planet_name(8));
    TEST_ASSERT_EQUAL_STRING("Unknown", planet_name(-1));
}

void test_planet_symbol_earth(void) {
    TEST_ASSERT_EQUAL_STRING("\xe2\x8a\x95", planet_symbol(PLANET_EARTH)); /* UTF-8 for ⊕ */
}

void test_planets_at_is_pure(void) {
    solar_system_t a = planets_at(2460000.0);
    solar_system_t b = planets_at(2460000.0);
    for (int i = 0; i < 8; i++) {
        TEST_ASSERT_EQUAL_FLOAT(a.positions[i].longitude, b.positions[i].longitude);
        TEST_ASSERT_EQUAL_FLOAT(a.positions[i].latitude, b.positions[i].latitude);
        TEST_ASSERT_EQUAL_FLOAT(a.positions[i].distance, b.positions[i].distance);
    }
}

void test_all_longitudes_change_after_100_days(void) {
    solar_system_t sys1 = planets_at(J2000_JD);
    solar_system_t sys2 = planets_at(J2000_JD + 100.0);
    for (int i = 0; i < 8; i++) {
        double diff = sys1.positions[i].longitude - sys2.positions[i].longitude;
        if (diff < 0.0) diff = -diff;
        TEST_ASSERT_TRUE(diff > 0.1); /* all planets move at least 0.1 deg in 100 days */
    }
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_all_positions_valid_at_j2000);
    RUN_TEST(test_earth_distance_at_j2000);
    RUN_TEST(test_mercury_closer_than_neptune);
    RUN_TEST(test_planet_name_mercury);
    RUN_TEST(test_planet_name_neptune);
    RUN_TEST(test_planet_name_invalid);
    RUN_TEST(test_planet_symbol_earth);
    RUN_TEST(test_planets_at_is_pure);
    RUN_TEST(test_all_longitudes_change_after_100_days);
    return UNITY_END();
}
