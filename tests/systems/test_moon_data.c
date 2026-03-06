/* test_moon_data.c — Tests for moon orbital data and position calculations.
 * TDD RED phase: 28 tests covering all 9 cataloged moons. */

#include "../unity/unity.h"
#include "../../src/systems/astronomy/moon_data.h"
#include "../../src/systems/astronomy/planets.h"
#include <string.h>
#include <math.h>

#define DIST_EPSILON 500.0     /* km tolerance for orbital distance */
#define PERIOD_EPSILON 0.01    /* days tolerance for orbital period */
#define RADIUS_EPSILON 0.5     /* km tolerance for moon radius */
#define ALBEDO_EPSILON 0.01    /* albedo tolerance */
#define SMALL_EPSILON 0.001    /* generic small tolerance */
#define ANGLE_EPSILON 1.0      /* degrees tolerance for angle checks */
#define J2000 2451545.0        /* Julian Day for J2000.0 epoch */

void setUp(void) { }
void tearDown(void) { }

/* --- moon_data_count tests --- */

void test_count_earth_has_1_moon(void) {
    TEST_ASSERT_EQUAL_INT(1, moon_data_count(PLANET_EARTH));
}

void test_count_mars_has_2_moons(void) {
    TEST_ASSERT_EQUAL_INT(2, moon_data_count(PLANET_MARS));
}

void test_count_jupiter_has_4_moons(void) {
    TEST_ASSERT_EQUAL_INT(4, moon_data_count(PLANET_JUPITER));
}

void test_count_saturn_has_2_moons(void) {
    TEST_ASSERT_EQUAL_INT(2, moon_data_count(PLANET_SATURN));
}

void test_count_mercury_has_0(void) {
    TEST_ASSERT_EQUAL_INT(0, moon_data_count(PLANET_MERCURY));
}

void test_count_venus_has_0(void) {
    TEST_ASSERT_EQUAL_INT(0, moon_data_count(PLANET_VENUS));
}

void test_count_uranus_has_0(void) {
    TEST_ASSERT_EQUAL_INT(0, moon_data_count(PLANET_URANUS));
}

void test_count_neptune_has_0(void) {
    TEST_ASSERT_EQUAL_INT(0, moon_data_count(PLANET_NEPTUNE));
}

void test_count_invalid_negative(void) {
    TEST_ASSERT_EQUAL_INT(0, moon_data_count(-1));
}

void test_count_invalid_too_large(void) {
    TEST_ASSERT_EQUAL_INT(0, moon_data_count(8));
}

/* --- moon_data_get tests --- */

void test_get_earth_moon(void) {
    moon_entry_t e = moon_data_get(PLANET_EARTH, 0);
    TEST_ASSERT_NOT_NULL(e.name);
    TEST_ASSERT_EQUAL_STRING("Moon", e.name);
    TEST_ASSERT_FLOAT_WITHIN(DIST_EPSILON, 384400.0, (float)e.orbit.a_km);
    TEST_ASSERT_EQUAL_INT(PLANET_EARTH, e.orbit.parent_planet);
}

void test_get_mars_phobos(void) {
    moon_entry_t e = moon_data_get(PLANET_MARS, 0);
    TEST_ASSERT_NOT_NULL(e.name);
    TEST_ASSERT_EQUAL_STRING("Phobos", e.name);
    TEST_ASSERT_FLOAT_WITHIN(PERIOD_EPSILON, 0.3189, (float)e.orbit.period_days);
}

void test_get_mars_deimos(void) {
    moon_entry_t e = moon_data_get(PLANET_MARS, 1);
    TEST_ASSERT_NOT_NULL(e.name);
    TEST_ASSERT_EQUAL_STRING("Deimos", e.name);
}

void test_get_jupiter_galilean_names(void) {
    TEST_ASSERT_EQUAL_STRING("Io", moon_data_get(PLANET_JUPITER, 0).name);
    TEST_ASSERT_EQUAL_STRING("Europa", moon_data_get(PLANET_JUPITER, 1).name);
    TEST_ASSERT_EQUAL_STRING("Ganymede", moon_data_get(PLANET_JUPITER, 2).name);
    TEST_ASSERT_EQUAL_STRING("Callisto", moon_data_get(PLANET_JUPITER, 3).name);
}

void test_get_saturn_titan(void) {
    moon_entry_t e = moon_data_get(PLANET_SATURN, 0);
    TEST_ASSERT_NOT_NULL(e.name);
    TEST_ASSERT_EQUAL_STRING("Titan", e.name);
    TEST_ASSERT_FLOAT_WITHIN(RADIUS_EPSILON, 2574.7, (float)e.orbit.radius_km);
}

void test_get_invalid_planet_returns_null_name(void) {
    moon_entry_t e = moon_data_get(-1, 0);
    TEST_ASSERT_NULL(e.name);
    TEST_ASSERT_FLOAT_WITHIN(SMALL_EPSILON, 0.0, (float)e.orbit.a_km);
}

void test_get_invalid_moon_index_returns_null_name(void) {
    moon_entry_t e = moon_data_get(PLANET_EARTH, 5);
    TEST_ASSERT_NULL(e.name);
    TEST_ASSERT_FLOAT_WITHIN(SMALL_EPSILON, 0.0, (float)e.orbit.a_km);
}

/* --- moon_data_name tests --- */

void test_name_all_9_moons(void) {
    TEST_ASSERT_EQUAL_STRING("Moon",     moon_data_name(PLANET_EARTH, 0));
    TEST_ASSERT_EQUAL_STRING("Phobos",   moon_data_name(PLANET_MARS, 0));
    TEST_ASSERT_EQUAL_STRING("Deimos",   moon_data_name(PLANET_MARS, 1));
    TEST_ASSERT_EQUAL_STRING("Io",       moon_data_name(PLANET_JUPITER, 0));
    TEST_ASSERT_EQUAL_STRING("Europa",   moon_data_name(PLANET_JUPITER, 1));
    TEST_ASSERT_EQUAL_STRING("Ganymede", moon_data_name(PLANET_JUPITER, 2));
    TEST_ASSERT_EQUAL_STRING("Callisto", moon_data_name(PLANET_JUPITER, 3));
    TEST_ASSERT_EQUAL_STRING("Titan",    moon_data_name(PLANET_SATURN, 0));
    TEST_ASSERT_EQUAL_STRING("Enceladus",moon_data_name(PLANET_SATURN, 1));
}

void test_name_invalid_returns_unknown(void) {
    TEST_ASSERT_EQUAL_STRING("Unknown", moon_data_name(-1, 0));
    TEST_ASSERT_EQUAL_STRING("Unknown", moon_data_name(PLANET_EARTH, 99));
    TEST_ASSERT_EQUAL_STRING("Unknown", moon_data_name(PLANET_MERCURY, 0));
}

/* --- moon_data_period tests --- */

void test_period_earth_moon(void) {
    TEST_ASSERT_FLOAT_WITHIN(PERIOD_EPSILON, 27.322,
                             (float)moon_data_period(PLANET_EARTH, 0));
}

void test_period_io(void) {
    TEST_ASSERT_FLOAT_WITHIN(PERIOD_EPSILON, 1.7691,
                             (float)moon_data_period(PLANET_JUPITER, 0));
}

/* --- moon_data_radius tests --- */

void test_radius_earth_moon(void) {
    TEST_ASSERT_FLOAT_WITHIN(RADIUS_EPSILON, 1737.4,
                             (float)moon_data_radius(PLANET_EARTH, 0));
}

void test_radius_ganymede(void) {
    TEST_ASSERT_FLOAT_WITHIN(RADIUS_EPSILON, 2634.1,
                             (float)moon_data_radius(PLANET_JUPITER, 2));
}

/* --- moon_data_position tests --- */

void test_position_earth_moon_at_j2000(void) {
    moon_position_t pos = moon_data_position(PLANET_EARTH, 0, J2000);
    /* Distance should be near semi-major axis, within orbital variation */
    TEST_ASSERT_FLOAT_WITHIN(25000.0, 384400.0, (float)pos.distance_km);
    /* Angle should be in valid range */
    TEST_ASSERT_TRUE(pos.angle_deg >= 0.0);
    TEST_ASSERT_TRUE(pos.angle_deg < 360.0);
}

void test_position_io_at_j2000(void) {
    moon_position_t pos = moon_data_position(PLANET_JUPITER, 0, J2000);
    /* Io has very low eccentricity, distance ~ semi-major axis */
    TEST_ASSERT_FLOAT_WITHIN(5000.0, 421700.0, (float)pos.distance_km);
}

void test_position_changes_over_time(void) {
    moon_position_t pos1 = moon_data_position(PLANET_EARTH, 0, J2000);
    moon_position_t pos2 = moon_data_position(PLANET_EARTH, 0, J2000 + 1.0);
    /* After 1 day the Moon moves ~13 degrees */
    double delta = fabs(pos2.angle_deg - pos1.angle_deg);
    /* Handle wraparound */
    if (delta > 180.0) delta = 360.0 - delta;
    TEST_ASSERT_TRUE(delta > 5.0);
    TEST_ASSERT_TRUE(delta < 25.0);
}

void test_position_invalid_returns_zero(void) {
    moon_position_t pos = moon_data_position(-1, 0, J2000);
    TEST_ASSERT_FLOAT_WITHIN(SMALL_EPSILON, 0.0, (float)pos.x_km);
    TEST_ASSERT_FLOAT_WITHIN(SMALL_EPSILON, 0.0, (float)pos.y_km);
    TEST_ASSERT_FLOAT_WITHIN(SMALL_EPSILON, 0.0, (float)pos.z_km);
    TEST_ASSERT_FLOAT_WITHIN(SMALL_EPSILON, 0.0, (float)pos.distance_km);
    TEST_ASSERT_FLOAT_WITHIN(SMALL_EPSILON, 0.0, (float)pos.angle_deg);
}

/* --- moon_data_scene_distance tests --- */

void test_scene_distance_scales_linearly(void) {
    double d1 = moon_data_scene_distance(PLANET_EARTH, 0, 0.001);
    double d2 = moon_data_scene_distance(PLANET_EARTH, 0, 0.002);
    TEST_ASSERT_FLOAT_WITHIN(0.01, (float)(d1 * 2.0), (float)d2);
}

void test_scene_distance_invalid_returns_zero(void) {
    TEST_ASSERT_FLOAT_WITHIN(SMALL_EPSILON, 0.0,
                             (float)moon_data_scene_distance(-1, 0, 0.001));
}

/* --- moon_data_scene_radius tests --- */

void test_scene_radius_scales_linearly(void) {
    double r1 = moon_data_scene_radius(PLANET_EARTH, 0, 0.001);
    double r2 = moon_data_scene_radius(PLANET_EARTH, 0, 0.002);
    TEST_ASSERT_FLOAT_WITHIN(0.01, (float)(r1 * 2.0), (float)r2);
}

void test_scene_radius_invalid_returns_zero(void) {
    TEST_ASSERT_FLOAT_WITHIN(SMALL_EPSILON, 0.0,
                             (float)moon_data_scene_radius(8, 0, 0.001));
}

/* --- moon_data_total_count test --- */

void test_total_count_is_9(void) {
    TEST_ASSERT_EQUAL_INT(9, moon_data_total_count());
}

/* --- Purity test --- */

void test_position_is_pure(void) {
    moon_position_t a = moon_data_position(PLANET_JUPITER, 1, J2000 + 100.0);
    moon_position_t b = moon_data_position(PLANET_JUPITER, 1, J2000 + 100.0);
    TEST_ASSERT_FLOAT_WITHIN(SMALL_EPSILON, (float)a.x_km, (float)b.x_km);
    TEST_ASSERT_FLOAT_WITHIN(SMALL_EPSILON, (float)a.y_km, (float)b.y_km);
    TEST_ASSERT_FLOAT_WITHIN(SMALL_EPSILON, (float)a.z_km, (float)b.z_km);
    TEST_ASSERT_FLOAT_WITHIN(SMALL_EPSILON, (float)a.distance_km, (float)b.distance_km);
    TEST_ASSERT_FLOAT_WITHIN(SMALL_EPSILON, (float)a.angle_deg, (float)b.angle_deg);
}

int main(void) {
    UNITY_BEGIN();

    /* Count (10) */
    RUN_TEST(test_count_earth_has_1_moon);
    RUN_TEST(test_count_mars_has_2_moons);
    RUN_TEST(test_count_jupiter_has_4_moons);
    RUN_TEST(test_count_saturn_has_2_moons);
    RUN_TEST(test_count_mercury_has_0);
    RUN_TEST(test_count_venus_has_0);
    RUN_TEST(test_count_uranus_has_0);
    RUN_TEST(test_count_neptune_has_0);
    RUN_TEST(test_count_invalid_negative);
    RUN_TEST(test_count_invalid_too_large);

    /* Get (7) */
    RUN_TEST(test_get_earth_moon);
    RUN_TEST(test_get_mars_phobos);
    RUN_TEST(test_get_mars_deimos);
    RUN_TEST(test_get_jupiter_galilean_names);
    RUN_TEST(test_get_saturn_titan);
    RUN_TEST(test_get_invalid_planet_returns_null_name);
    RUN_TEST(test_get_invalid_moon_index_returns_null_name);

    /* Name (2) */
    RUN_TEST(test_name_all_9_moons);
    RUN_TEST(test_name_invalid_returns_unknown);

    /* Period (2) */
    RUN_TEST(test_period_earth_moon);
    RUN_TEST(test_period_io);

    /* Radius (2) */
    RUN_TEST(test_radius_earth_moon);
    RUN_TEST(test_radius_ganymede);

    /* Position (4) */
    RUN_TEST(test_position_earth_moon_at_j2000);
    RUN_TEST(test_position_io_at_j2000);
    RUN_TEST(test_position_changes_over_time);
    RUN_TEST(test_position_invalid_returns_zero);

    /* Scene distance (2) */
    RUN_TEST(test_scene_distance_scales_linearly);
    RUN_TEST(test_scene_distance_invalid_returns_zero);

    /* Scene radius (2) */
    RUN_TEST(test_scene_radius_scales_linearly);
    RUN_TEST(test_scene_radius_invalid_returns_zero);

    /* Total count (1) */
    RUN_TEST(test_total_count_is_9);

    /* Purity (1) */
    RUN_TEST(test_position_is_pure);

    return UNITY_END();
}
