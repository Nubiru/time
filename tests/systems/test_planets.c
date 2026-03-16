#include "../unity/unity.h"
#include "../../src/systems/astronomy/planets.h"
#include <math.h>

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
    TEST_ASSERT_EQUAL_STRING("\xe2\x8a\x95", planet_symbol(PLANET_EARTH));
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
        TEST_ASSERT_TRUE(diff > 0.1);
    }
}

/* === NEW TESTS === */

void test_mercury_distance_range(void) {
    solar_system_t sys = planets_at(J2000_JD);
    TEST_ASSERT_TRUE(sys.positions[PLANET_MERCURY].distance > 0.3);
    TEST_ASSERT_TRUE(sys.positions[PLANET_MERCURY].distance < 0.5);
}

void test_venus_distance_range(void) {
    solar_system_t sys = planets_at(J2000_JD);
    TEST_ASSERT_TRUE(sys.positions[PLANET_VENUS].distance > 0.7);
    TEST_ASSERT_TRUE(sys.positions[PLANET_VENUS].distance < 0.74);
}

void test_mars_distance_range(void) {
    solar_system_t sys = planets_at(J2000_JD);
    TEST_ASSERT_TRUE(sys.positions[PLANET_MARS].distance > 1.3);
    TEST_ASSERT_TRUE(sys.positions[PLANET_MARS].distance < 1.7);
}

void test_jupiter_distance_range(void) {
    solar_system_t sys = planets_at(J2000_JD);
    TEST_ASSERT_TRUE(sys.positions[PLANET_JUPITER].distance > 4.9);
    TEST_ASSERT_TRUE(sys.positions[PLANET_JUPITER].distance < 5.5);
}

void test_neptune_distance_range(void) {
    solar_system_t sys = planets_at(J2000_JD);
    TEST_ASSERT_TRUE(sys.positions[PLANET_NEPTUNE].distance > 29.5);
    TEST_ASSERT_TRUE(sys.positions[PLANET_NEPTUNE].distance < 30.5);
}

void test_planet_distance_ordering(void) {
    solar_system_t sys = planets_at(J2000_JD);
    TEST_ASSERT_TRUE(sys.positions[PLANET_MERCURY].distance < sys.positions[PLANET_VENUS].distance);
    TEST_ASSERT_TRUE(sys.positions[PLANET_VENUS].distance < sys.positions[PLANET_EARTH].distance);
    TEST_ASSERT_TRUE(sys.positions[PLANET_EARTH].distance < sys.positions[PLANET_MARS].distance);
    TEST_ASSERT_TRUE(sys.positions[PLANET_MARS].distance < sys.positions[PLANET_JUPITER].distance);
    TEST_ASSERT_TRUE(sys.positions[PLANET_JUPITER].distance < sys.positions[PLANET_SATURN].distance);
    TEST_ASSERT_TRUE(sys.positions[PLANET_SATURN].distance < sys.positions[PLANET_URANUS].distance);
    TEST_ASSERT_TRUE(sys.positions[PLANET_URANUS].distance < sys.positions[PLANET_NEPTUNE].distance);
}

void test_planet_name_all(void) {
    TEST_ASSERT_EQUAL_STRING("Mercury", planet_name(PLANET_MERCURY));
    TEST_ASSERT_EQUAL_STRING("Venus", planet_name(PLANET_VENUS));
    TEST_ASSERT_EQUAL_STRING("Earth", planet_name(PLANET_EARTH));
    TEST_ASSERT_EQUAL_STRING("Mars", planet_name(PLANET_MARS));
    TEST_ASSERT_EQUAL_STRING("Jupiter", planet_name(PLANET_JUPITER));
    TEST_ASSERT_EQUAL_STRING("Saturn", planet_name(PLANET_SATURN));
    TEST_ASSERT_EQUAL_STRING("Uranus", planet_name(PLANET_URANUS));
    TEST_ASSERT_EQUAL_STRING("Neptune", planet_name(PLANET_NEPTUNE));
}

void test_planet_name_invalid_large(void) {
    TEST_ASSERT_EQUAL_STRING("Unknown", planet_name(100));
}

void test_planet_symbol_all_valid(void) {
    for (int i = 0; i < 8; i++) {
        const char *sym = planet_symbol(i);
        TEST_ASSERT_NOT_NULL(sym);
        TEST_ASSERT_TRUE(sym[0] != '?');
    }
}

void test_planet_symbol_invalid(void) {
    TEST_ASSERT_EQUAL_STRING("?", planet_symbol(8));
    TEST_ASSERT_EQUAL_STRING("?", planet_symbol(-1));
    TEST_ASSERT_EQUAL_STRING("?", planet_symbol(100));
}

void test_positions_valid_at_2010(void) {
    double jd_2010 = 2455197.5;
    solar_system_t sys = planets_at(jd_2010);
    for (int i = 0; i < 8; i++) {
        TEST_ASSERT_TRUE(sys.positions[i].longitude >= 0.0);
        TEST_ASSERT_TRUE(sys.positions[i].longitude < 360.0);
        TEST_ASSERT_TRUE(sys.positions[i].distance > 0.0);
    }
}

void test_positions_valid_at_1950(void) {
    double jd_1950 = 2433282.5;
    solar_system_t sys = planets_at(jd_1950);
    for (int i = 0; i < 8; i++) {
        TEST_ASSERT_TRUE(sys.positions[i].longitude >= 0.0);
        TEST_ASSERT_TRUE(sys.positions[i].longitude < 360.0);
        TEST_ASSERT_TRUE(sys.positions[i].distance > 0.0);
    }
}

void test_earth_latitude_near_zero(void) {
    solar_system_t sys = planets_at(J2000_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.5, 0.0, sys.positions[PLANET_EARTH].latitude);
}

void test_mercury_moves_fastest(void) {
    solar_system_t sys1 = planets_at(J2000_JD);
    solar_system_t sys2 = planets_at(J2000_JD + 30.0);
    double merc_move = fabs(sys2.positions[PLANET_MERCURY].longitude - sys1.positions[PLANET_MERCURY].longitude);
    if (merc_move > 180.0) merc_move = 360.0 - merc_move;
    double nept_move = fabs(sys2.positions[PLANET_NEPTUNE].longitude - sys1.positions[PLANET_NEPTUNE].longitude);
    if (nept_move > 180.0) nept_move = 360.0 - nept_move;
    TEST_ASSERT_TRUE(merc_move > nept_move);
}

void test_planet_name_is_pure(void) {
    const char *a = planet_name(PLANET_MARS);
    const char *b = planet_name(PLANET_MARS);
    TEST_ASSERT_EQUAL_STRING(a, b);
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
    RUN_TEST(test_mercury_distance_range);
    RUN_TEST(test_venus_distance_range);
    RUN_TEST(test_mars_distance_range);
    RUN_TEST(test_jupiter_distance_range);
    RUN_TEST(test_neptune_distance_range);
    RUN_TEST(test_planet_distance_ordering);
    RUN_TEST(test_planet_name_all);
    RUN_TEST(test_planet_name_invalid_large);
    RUN_TEST(test_planet_symbol_all_valid);
    RUN_TEST(test_planet_symbol_invalid);
    RUN_TEST(test_positions_valid_at_2010);
    RUN_TEST(test_positions_valid_at_1950);
    RUN_TEST(test_earth_latitude_near_zero);
    RUN_TEST(test_mercury_moves_fastest);
    RUN_TEST(test_planet_name_is_pure);
    return UNITY_END();
}
