#include "../unity/unity.h"
#include "../../src/systems/astronomy/planet_data.h"
#include "../../src/systems/astronomy/planets.h"

#define RADIUS_EPSILON 0.5f
#define COLOR_EPSILON 0.01f
#define ALBEDO_EPSILON 0.001f
#define TILT_EPSILON 0.01f
#define ROTATION_EPSILON 0.5f
#define SMALL_EPSILON 0.001f

void setUp(void) { }
void tearDown(void) { }

/* --- Radius tests --- */

void test_earth_radius(void) {
    TEST_ASSERT_FLOAT_WITHIN(RADIUS_EPSILON, 6371.0f, (float)planet_data_radius(PLANET_EARTH));
}

void test_mercury_radius(void) {
    TEST_ASSERT_FLOAT_WITHIN(RADIUS_EPSILON, 2439.7f, (float)planet_data_radius(PLANET_MERCURY));
}

void test_jupiter_radius(void) {
    TEST_ASSERT_FLOAT_WITHIN(RADIUS_EPSILON, 69911.0f, (float)planet_data_radius(PLANET_JUPITER));
}

void test_invalid_index_radius(void) {
    /* Out of range should return 0.0 */
    TEST_ASSERT_FLOAT_WITHIN(SMALL_EPSILON, 0.0f, (float)planet_data_radius(-1));
    TEST_ASSERT_FLOAT_WITHIN(SMALL_EPSILON, 0.0f, (float)planet_data_radius(8));
}

/* --- Mass tests --- */

void test_earth_mass(void) {
    planet_physical_t p = planet_data_get(PLANET_EARTH);
    TEST_ASSERT_FLOAT_WITHIN(0.001e24f, 5.9724e24f, (float)p.mass_kg);
}

void test_jupiter_mass(void) {
    planet_physical_t p = planet_data_get(PLANET_JUPITER);
    /* Float precision limits for large values; check order of magnitude */
    TEST_ASSERT_TRUE(p.mass_kg > 1898.0e24);
    TEST_ASSERT_TRUE(p.mass_kg < 1899.0e24);
}

/* --- Albedo tests --- */

void test_venus_albedo_highest(void) {
    /* Venus has the highest albedo of any planet */
    planet_physical_t venus = planet_data_get(PLANET_VENUS);
    TEST_ASSERT_FLOAT_WITHIN(ALBEDO_EPSILON, 0.689f, (float)venus.albedo);
    for (int i = 0; i < PLANET_DATA_COUNT; i++) {
        if (i == PLANET_VENUS) continue;
        planet_physical_t other = planet_data_get(i);
        TEST_ASSERT_TRUE(venus.albedo > other.albedo);
    }
}

void test_mercury_albedo_lowest(void) {
    planet_physical_t p = planet_data_get(PLANET_MERCURY);
    TEST_ASSERT_FLOAT_WITHIN(ALBEDO_EPSILON, 0.068f, (float)p.albedo);
}

/* --- Axial tilt tests --- */

void test_earth_tilt(void) {
    planet_physical_t p = planet_data_get(PLANET_EARTH);
    TEST_ASSERT_FLOAT_WITHIN(TILT_EPSILON, 23.44f, (float)p.axial_tilt_deg);
}

void test_uranus_extreme_tilt(void) {
    /* Uranus is nearly sideways */
    planet_physical_t p = planet_data_get(PLANET_URANUS);
    TEST_ASSERT_FLOAT_WITHIN(TILT_EPSILON, 97.77f, (float)p.axial_tilt_deg);
}

/* --- Rotation tests --- */

void test_venus_retrograde_rotation(void) {
    /* Venus rotates retrograde (negative) */
    planet_physical_t p = planet_data_get(PLANET_VENUS);
    TEST_ASSERT_TRUE(p.rotation_hours < 0.0);
    TEST_ASSERT_FLOAT_WITHIN(ROTATION_EPSILON, -5832.5f, (float)p.rotation_hours);
}

void test_uranus_retrograde_rotation(void) {
    planet_physical_t p = planet_data_get(PLANET_URANUS);
    TEST_ASSERT_TRUE(p.rotation_hours < 0.0);
}

void test_jupiter_fastest_rotation(void) {
    /* Jupiter has the shortest rotation period (in absolute value) */
    planet_physical_t jup = planet_data_get(PLANET_JUPITER);
    double jup_abs = jup.rotation_hours > 0 ? jup.rotation_hours : -jup.rotation_hours;
    for (int i = 0; i < PLANET_DATA_COUNT; i++) {
        if (i == PLANET_JUPITER) continue;
        planet_physical_t other = planet_data_get(i);
        double other_abs = other.rotation_hours > 0 ? other.rotation_hours : -other.rotation_hours;
        TEST_ASSERT_TRUE(jup_abs < other_abs);
    }
}

/* --- Color tests --- */

void test_earth_primary_color_blue(void) {
    planet_color_t c = planet_data_color(PLANET_EARTH);
    /* Blue channel should dominate */
    TEST_ASSERT_TRUE(c.b > c.r);
    TEST_ASSERT_TRUE(c.b > c.g);
}

void test_mars_primary_color_red(void) {
    planet_color_t c = planet_data_color(PLANET_MARS);
    /* Red channel should dominate */
    TEST_ASSERT_TRUE(c.r > c.g);
    TEST_ASSERT_TRUE(c.r > c.b);
}

void test_color_values_in_range(void) {
    for (int i = 0; i < PLANET_DATA_COUNT; i++) {
        planet_color_t c = planet_data_color(i);
        TEST_ASSERT_TRUE(c.r >= 0.0f && c.r <= 1.0f);
        TEST_ASSERT_TRUE(c.g >= 0.0f && c.g <= 1.0f);
        TEST_ASSERT_TRUE(c.b >= 0.0f && c.b <= 1.0f);
    }
}

void test_invalid_index_color(void) {
    planet_color_t c = planet_data_color(-1);
    TEST_ASSERT_FLOAT_WITHIN(COLOR_EPSILON, 0.0f, c.r);
    TEST_ASSERT_FLOAT_WITHIN(COLOR_EPSILON, 0.0f, c.g);
    TEST_ASSERT_FLOAT_WITHIN(COLOR_EPSILON, 0.0f, c.b);
}

/* --- Relative size tests --- */

void test_earth_relative_size_is_one(void) {
    TEST_ASSERT_FLOAT_WITHIN(SMALL_EPSILON, 1.0f, (float)planet_data_relative_size(PLANET_EARTH));
}

void test_jupiter_largest_relative_size(void) {
    double jup = planet_data_relative_size(PLANET_JUPITER);
    for (int i = 0; i < PLANET_DATA_COUNT; i++) {
        if (i == PLANET_JUPITER) continue;
        double other = planet_data_relative_size(i);
        TEST_ASSERT_TRUE(jup > other);
    }
}

void test_mercury_smallest_relative_size(void) {
    double merc = planet_data_relative_size(PLANET_MERCURY);
    for (int i = 0; i < PLANET_DATA_COUNT; i++) {
        if (i == PLANET_MERCURY) continue;
        double other = planet_data_relative_size(i);
        TEST_ASSERT_TRUE(merc < other);
    }
}

void test_invalid_index_relative_size(void) {
    TEST_ASSERT_FLOAT_WITHIN(SMALL_EPSILON, 0.0f, (float)planet_data_relative_size(99));
}

/* --- Type tests --- */

void test_earth_is_terrestrial(void) {
    const char *t = planet_data_type(PLANET_EARTH);
    TEST_ASSERT_EQUAL_STRING("Terrestrial", t);
}

void test_jupiter_is_gas_giant(void) {
    const char *t = planet_data_type(PLANET_JUPITER);
    TEST_ASSERT_EQUAL_STRING("Gas Giant", t);
}

void test_neptune_is_ice_giant(void) {
    const char *t = planet_data_type(PLANET_NEPTUNE);
    TEST_ASSERT_EQUAL_STRING("Ice Giant", t);
}

void test_invalid_index_type(void) {
    const char *t = planet_data_type(-1);
    TEST_ASSERT_EQUAL_STRING("Unknown", t);
}

/* --- Major moons tests --- */

void test_earth_has_one_moon(void) {
    TEST_ASSERT_EQUAL_INT(1, planet_data_major_moons(PLANET_EARTH));
}

void test_mercury_has_no_moons(void) {
    TEST_ASSERT_EQUAL_INT(0, planet_data_major_moons(PLANET_MERCURY));
}

void test_saturn_most_moons(void) {
    int sat = planet_data_major_moons(PLANET_SATURN);
    for (int i = 0; i < PLANET_DATA_COUNT; i++) {
        if (i == PLANET_SATURN) continue;
        TEST_ASSERT_TRUE(sat > planet_data_major_moons(i));
    }
}

void test_invalid_index_moons(void) {
    TEST_ASSERT_EQUAL_INT(0, planet_data_major_moons(42));
}

/* --- Scene radius tests --- */

void test_scene_radius_scales_linearly(void) {
    float r1 = (float)planet_data_scene_radius(PLANET_EARTH, 0.01);
    float r2 = (float)planet_data_scene_radius(PLANET_EARTH, 0.02);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, r1 * 2.0f, r2);
}

void test_scene_radius_zero_for_invalid(void) {
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.0f, (float)planet_data_scene_radius(-1, 0.01));
}

/* --- Ring tests --- */

void test_saturn_has_rings(void) {
    planet_physical_t p = planet_data_get(PLANET_SATURN);
    TEST_ASSERT_EQUAL_INT(1, p.has_rings);
    TEST_ASSERT_TRUE(p.ring_inner_radii > 1.0);
    TEST_ASSERT_TRUE(p.ring_outer_radii > p.ring_inner_radii);
}

void test_earth_has_no_rings(void) {
    planet_physical_t p = planet_data_get(PLANET_EARTH);
    TEST_ASSERT_EQUAL_INT(0, p.has_rings);
    TEST_ASSERT_FLOAT_WITHIN(SMALL_EPSILON, 0.0f, (float)p.ring_inner_radii);
    TEST_ASSERT_FLOAT_WITHIN(SMALL_EPSILON, 0.0f, (float)p.ring_outer_radii);
}

void test_saturn_ring_scene_radii(void) {
    double inner = planet_data_ring_inner(PLANET_SATURN, 0.01);
    double outer = planet_data_ring_outer(PLANET_SATURN, 0.01);
    TEST_ASSERT_TRUE(inner > 0.0);
    TEST_ASSERT_TRUE(outer > inner);
}

void test_earth_ring_scene_radii_zero(void) {
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.0f, (float)planet_data_ring_inner(PLANET_EARTH, 0.01));
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.0f, (float)planet_data_ring_outer(PLANET_EARTH, 0.01));
}

/* --- Gas giant flag tests --- */

void test_terrestrial_not_gas_giant(void) {
    TEST_ASSERT_EQUAL_INT(0, planet_data_get(PLANET_MERCURY).is_gas_giant);
    TEST_ASSERT_EQUAL_INT(0, planet_data_get(PLANET_VENUS).is_gas_giant);
    TEST_ASSERT_EQUAL_INT(0, planet_data_get(PLANET_EARTH).is_gas_giant);
    TEST_ASSERT_EQUAL_INT(0, planet_data_get(PLANET_MARS).is_gas_giant);
}

void test_outer_planets_are_gas_giant(void) {
    TEST_ASSERT_EQUAL_INT(1, planet_data_get(PLANET_JUPITER).is_gas_giant);
    TEST_ASSERT_EQUAL_INT(1, planet_data_get(PLANET_SATURN).is_gas_giant);
    TEST_ASSERT_EQUAL_INT(1, planet_data_get(PLANET_URANUS).is_gas_giant);
    TEST_ASSERT_EQUAL_INT(1, planet_data_get(PLANET_NEPTUNE).is_gas_giant);
}

/* --- Purity test --- */

void test_planet_data_get_is_pure(void) {
    planet_physical_t a = planet_data_get(PLANET_MARS);
    planet_physical_t b = planet_data_get(PLANET_MARS);
    TEST_ASSERT_FLOAT_WITHIN(SMALL_EPSILON, (float)a.radius_km, (float)b.radius_km);
    TEST_ASSERT_FLOAT_WITHIN(SMALL_EPSILON, (float)a.albedo, (float)b.albedo);
    TEST_ASSERT_EQUAL_FLOAT(a.color.r, b.color.r);
    TEST_ASSERT_EQUAL_FLOAT(a.color.g, b.color.g);
    TEST_ASSERT_EQUAL_FLOAT(a.color.b, b.color.b);
}

/* --- Alt color test --- */

void test_earth_alt_color_is_green(void) {
    planet_physical_t p = planet_data_get(PLANET_EARTH);
    /* Alt color for Earth is land green: green channel dominates */
    TEST_ASSERT_TRUE(p.color_alt.g > p.color_alt.r);
    TEST_ASSERT_TRUE(p.color_alt.g > p.color_alt.b);
}

int main(void) {
    UNITY_BEGIN();

    /* Radius (4) */
    RUN_TEST(test_earth_radius);
    RUN_TEST(test_mercury_radius);
    RUN_TEST(test_jupiter_radius);
    RUN_TEST(test_invalid_index_radius);

    /* Mass (2) */
    RUN_TEST(test_earth_mass);
    RUN_TEST(test_jupiter_mass);

    /* Albedo (2) */
    RUN_TEST(test_venus_albedo_highest);
    RUN_TEST(test_mercury_albedo_lowest);

    /* Tilt (2) */
    RUN_TEST(test_earth_tilt);
    RUN_TEST(test_uranus_extreme_tilt);

    /* Rotation (3) */
    RUN_TEST(test_venus_retrograde_rotation);
    RUN_TEST(test_uranus_retrograde_rotation);
    RUN_TEST(test_jupiter_fastest_rotation);

    /* Color (4) */
    RUN_TEST(test_earth_primary_color_blue);
    RUN_TEST(test_mars_primary_color_red);
    RUN_TEST(test_color_values_in_range);
    RUN_TEST(test_invalid_index_color);

    /* Relative size (4) */
    RUN_TEST(test_earth_relative_size_is_one);
    RUN_TEST(test_jupiter_largest_relative_size);
    RUN_TEST(test_mercury_smallest_relative_size);
    RUN_TEST(test_invalid_index_relative_size);

    /* Type (4) */
    RUN_TEST(test_earth_is_terrestrial);
    RUN_TEST(test_jupiter_is_gas_giant);
    RUN_TEST(test_neptune_is_ice_giant);
    RUN_TEST(test_invalid_index_type);

    /* Moons (4) */
    RUN_TEST(test_earth_has_one_moon);
    RUN_TEST(test_mercury_has_no_moons);
    RUN_TEST(test_saturn_most_moons);
    RUN_TEST(test_invalid_index_moons);

    /* Scene radius (2) */
    RUN_TEST(test_scene_radius_scales_linearly);
    RUN_TEST(test_scene_radius_zero_for_invalid);

    /* Rings (4) */
    RUN_TEST(test_saturn_has_rings);
    RUN_TEST(test_earth_has_no_rings);
    RUN_TEST(test_saturn_ring_scene_radii);
    RUN_TEST(test_earth_ring_scene_radii_zero);

    /* Gas giant flag (2) */
    RUN_TEST(test_terrestrial_not_gas_giant);
    RUN_TEST(test_outer_planets_are_gas_giant);

    /* Purity (1) */
    RUN_TEST(test_planet_data_get_is_pure);

    /* Alt color (1) */
    RUN_TEST(test_earth_alt_color_is_green);

    return UNITY_END();
}
