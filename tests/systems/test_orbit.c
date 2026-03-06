#include "../unity/unity.h"
#include "../../src/systems/astronomy/orbit.h"
#include <math.h>

#define DEG_EPSILON 1.0     /* 1 degree tolerance for orbital positions */
#define DIST_EPSILON 0.01   /* 0.01 AU tolerance */

void setUp(void) { }
void tearDown(void) { }

/* Helper: angular difference wrapped to [0, 180] */
static double angle_diff(double a, double b) {
    double d = fabs(a - b);
    if (d > 180.0) d = 360.0 - d;
    return d;
}

/* Helper: test that a planet returns near start longitude after one orbital period */
static void assert_orbital_period(const planet_orbit_t *planet, double period_days, double tolerance) {
    double jd = 2451545.0;
    orbital_elements_t e1 = orbit_elements_at(planet, jd);
    orbital_elements_t e2 = orbit_elements_at(planet, jd + period_days);
    heliocentric_pos_t p1 = orbit_heliocentric(&e1);
    heliocentric_pos_t p2 = orbit_heliocentric(&e2);
    TEST_ASSERT_TRUE(angle_diff(p1.longitude, p2.longitude) < tolerance);
}

/* J2000.0 epoch: 2000 January 1.5 = JD 2451545.0 */

/* ===== Earth ===== */

void test_earth_elements_at_j2000(void) {
    orbital_elements_t e = orbit_elements_at(&ORBIT_EARTH, 2451545.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 1.0, e.a);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.0167, e.e);
}

void test_earth_distance_range(void) {
    /* Earth at J2000.0 (Jan 1.5) is near perihelion, ~0.983 AU */
    orbital_elements_t e = orbit_elements_at(&ORBIT_EARTH, 2451545.0);
    heliocentric_pos_t pos = orbit_heliocentric(&e);
    TEST_ASSERT_FLOAT_WITHIN(DIST_EPSILON, 0.983, pos.distance);
}

void test_earth_latitude_near_zero(void) {
    orbital_elements_t e = orbit_elements_at(&ORBIT_EARTH, 2451545.0);
    heliocentric_pos_t pos = orbit_heliocentric(&e);
    TEST_ASSERT_FLOAT_WITHIN(0.1, 0.0, pos.latitude);
}

void test_earth_longitude_changes_over_year(void) {
    assert_orbital_period(&ORBIT_EARTH, 365.25, 5.0);
}

void test_earth_half_year_opposite(void) {
    double jd = 2451545.0;
    orbital_elements_t e1 = orbit_elements_at(&ORBIT_EARTH, jd);
    orbital_elements_t e2 = orbit_elements_at(&ORBIT_EARTH, jd + 182.625);
    heliocentric_pos_t p1 = orbit_heliocentric(&e1);
    heliocentric_pos_t p2 = orbit_heliocentric(&e2);
    TEST_ASSERT_TRUE(angle_diff(p1.longitude, p2.longitude) > 160.0);
}

/* ===== Mars ===== */

void test_mars_distance_range(void) {
    orbital_elements_t e = orbit_elements_at(&ORBIT_MARS, 2451545.0);
    heliocentric_pos_t pos = orbit_heliocentric(&e);
    TEST_ASSERT_FLOAT_WITHIN(0.2, 1.52, pos.distance);
}

void test_mars_orbital_period(void) {
    assert_orbital_period(&ORBIT_MARS, 687.0, 10.0);
}

/* ===== Mercury ===== */

void test_mercury_distance_range(void) {
    /* Mercury: ~0.387 AU, eccentricity ~0.206 -> range 0.31-0.47 AU */
    orbital_elements_t e = orbit_elements_at(&ORBIT_MERCURY, 2451545.0);
    heliocentric_pos_t pos = orbit_heliocentric(&e);
    TEST_ASSERT_TRUE(pos.distance > 0.30 && pos.distance < 0.48);
}

void test_mercury_elements_at_j2000(void) {
    orbital_elements_t e = orbit_elements_at(&ORBIT_MERCURY, 2451545.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.387, e.a);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 0.206, e.e);
}

void test_mercury_orbital_period(void) {
    /* Mercury period: ~87.97 days */
    assert_orbital_period(&ORBIT_MERCURY, 87.97, 10.0);
}

/* ===== Venus ===== */

void test_venus_distance_range(void) {
    /* Venus: ~0.723 AU, nearly circular (e ~0.007) */
    orbital_elements_t e = orbit_elements_at(&ORBIT_VENUS, 2451545.0);
    heliocentric_pos_t pos = orbit_heliocentric(&e);
    TEST_ASSERT_FLOAT_WITHIN(0.02, 0.723, pos.distance);
}

void test_venus_elements_at_j2000(void) {
    orbital_elements_t e = orbit_elements_at(&ORBIT_VENUS, 2451545.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.723, e.a);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.007, e.e);
}

void test_venus_orbital_period(void) {
    /* Venus period: ~224.7 days */
    assert_orbital_period(&ORBIT_VENUS, 224.7, 10.0);
}

/* ===== Jupiter ===== */

void test_jupiter_distance_range(void) {
    /* Jupiter: ~5.20 AU, eccentricity ~0.048 -> range ~4.95-5.46 AU */
    orbital_elements_t e = orbit_elements_at(&ORBIT_JUPITER, 2451545.0);
    heliocentric_pos_t pos = orbit_heliocentric(&e);
    TEST_ASSERT_TRUE(pos.distance > 4.9 && pos.distance < 5.5);
}

void test_jupiter_elements_at_j2000(void) {
    orbital_elements_t e = orbit_elements_at(&ORBIT_JUPITER, 2451545.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 5.203, e.a);
    TEST_ASSERT_FLOAT_WITHIN(0.005, 0.048, e.e);
}

void test_jupiter_orbital_period(void) {
    /* Jupiter period: ~4332.6 days (~11.86 years) */
    assert_orbital_period(&ORBIT_JUPITER, 4332.6, 10.0);
}

/* ===== Saturn ===== */

void test_saturn_distance_range(void) {
    /* Saturn: ~9.54 AU, eccentricity ~0.054 -> range ~9.02-10.05 AU */
    orbital_elements_t e = orbit_elements_at(&ORBIT_SATURN, 2451545.0);
    heliocentric_pos_t pos = orbit_heliocentric(&e);
    TEST_ASSERT_TRUE(pos.distance > 9.0 && pos.distance < 10.1);
}

void test_saturn_elements_at_j2000(void) {
    orbital_elements_t e = orbit_elements_at(&ORBIT_SATURN, 2451545.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 9.537, e.a);
    TEST_ASSERT_FLOAT_WITHIN(0.005, 0.054, e.e);
}

void test_saturn_orbital_period(void) {
    /* Saturn period: ~10759 days (~29.46 years) */
    assert_orbital_period(&ORBIT_SATURN, 10759.0, 15.0);
}

/* ===== Uranus ===== */

void test_uranus_distance_range(void) {
    /* Uranus: ~19.19 AU, eccentricity ~0.047 -> range ~18.3-20.1 AU */
    orbital_elements_t e = orbit_elements_at(&ORBIT_URANUS, 2451545.0);
    heliocentric_pos_t pos = orbit_heliocentric(&e);
    TEST_ASSERT_TRUE(pos.distance > 18.2 && pos.distance < 20.2);
}

void test_uranus_elements_at_j2000(void) {
    orbital_elements_t e = orbit_elements_at(&ORBIT_URANUS, 2451545.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 19.189, e.a);
    TEST_ASSERT_FLOAT_WITHIN(0.005, 0.047, e.e);
}

void test_uranus_orbital_period(void) {
    /* Uranus period: ~30687 days (~84.01 years) */
    assert_orbital_period(&ORBIT_URANUS, 30687.0, 15.0);
}

/* ===== Neptune ===== */

void test_neptune_distance_range(void) {
    /* Neptune: ~30.07 AU, nearly circular (e ~0.009) */
    orbital_elements_t e = orbit_elements_at(&ORBIT_NEPTUNE, 2451545.0);
    heliocentric_pos_t pos = orbit_heliocentric(&e);
    TEST_ASSERT_FLOAT_WITHIN(0.5, 30.07, pos.distance);
}

void test_neptune_elements_at_j2000(void) {
    orbital_elements_t e = orbit_elements_at(&ORBIT_NEPTUNE, 2451545.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 30.070, e.a);
    TEST_ASSERT_FLOAT_WITHIN(0.005, 0.009, e.e);
}

void test_neptune_orbital_period(void) {
    /* Neptune period: ~60190 days (~164.8 years) */
    assert_orbital_period(&ORBIT_NEPTUNE, 60190.0, 15.0);
}

/* ===== Cross-planet ===== */

void test_orbit_is_pure(void) {
    /* Same input always gives same output (deterministic) */
    orbital_elements_t e1 = orbit_elements_at(&ORBIT_EARTH, 2460000.0);
    orbital_elements_t e2 = orbit_elements_at(&ORBIT_EARTH, 2460000.0);
    heliocentric_pos_t p1 = orbit_heliocentric(&e1);
    heliocentric_pos_t p2 = orbit_heliocentric(&e2);
    TEST_ASSERT_EQUAL_FLOAT(p1.longitude, p2.longitude);
    TEST_ASSERT_EQUAL_FLOAT(p1.distance, p2.distance);
}

void test_inner_planets_closer_than_outer(void) {
    /* Basic sanity: Mercury < Venus < Earth < Mars < Jupiter < Saturn < Uranus < Neptune */
    double jd = 2451545.0;
    const planet_orbit_t *planets[] = {
        &ORBIT_MERCURY, &ORBIT_VENUS, &ORBIT_EARTH, &ORBIT_MARS,
        &ORBIT_JUPITER, &ORBIT_SATURN, &ORBIT_URANUS, &ORBIT_NEPTUNE
    };
    double prev_a = 0.0;
    for (int i = 0; i < 8; i++) {
        orbital_elements_t e = orbit_elements_at(planets[i], jd);
        TEST_ASSERT_TRUE(e.a > prev_a);
        prev_a = e.a;
    }
}

void test_all_planets_valid_positions(void) {
    /* Every planet should produce valid lon [0,360], lat [-90,90], distance > 0 */
    double jd = 2451545.0;
    const planet_orbit_t *planets[] = {
        &ORBIT_MERCURY, &ORBIT_VENUS, &ORBIT_EARTH, &ORBIT_MARS,
        &ORBIT_JUPITER, &ORBIT_SATURN, &ORBIT_URANUS, &ORBIT_NEPTUNE
    };
    for (int i = 0; i < 8; i++) {
        orbital_elements_t e = orbit_elements_at(planets[i], jd);
        heliocentric_pos_t pos = orbit_heliocentric(&e);
        TEST_ASSERT_TRUE(pos.longitude >= 0.0 && pos.longitude < 360.0);
        TEST_ASSERT_TRUE(pos.latitude >= -90.0 && pos.latitude <= 90.0);
        TEST_ASSERT_TRUE(pos.distance > 0.0);
    }
}

int main(void) {
    UNITY_BEGIN();
    /* Earth */
    RUN_TEST(test_earth_elements_at_j2000);
    RUN_TEST(test_earth_distance_range);
    RUN_TEST(test_earth_latitude_near_zero);
    RUN_TEST(test_earth_longitude_changes_over_year);
    RUN_TEST(test_earth_half_year_opposite);
    /* Mars */
    RUN_TEST(test_mars_distance_range);
    RUN_TEST(test_mars_orbital_period);
    /* Mercury */
    RUN_TEST(test_mercury_distance_range);
    RUN_TEST(test_mercury_elements_at_j2000);
    RUN_TEST(test_mercury_orbital_period);
    /* Venus */
    RUN_TEST(test_venus_distance_range);
    RUN_TEST(test_venus_elements_at_j2000);
    RUN_TEST(test_venus_orbital_period);
    /* Jupiter */
    RUN_TEST(test_jupiter_distance_range);
    RUN_TEST(test_jupiter_elements_at_j2000);
    RUN_TEST(test_jupiter_orbital_period);
    /* Saturn */
    RUN_TEST(test_saturn_distance_range);
    RUN_TEST(test_saturn_elements_at_j2000);
    RUN_TEST(test_saturn_orbital_period);
    /* Uranus */
    RUN_TEST(test_uranus_distance_range);
    RUN_TEST(test_uranus_elements_at_j2000);
    RUN_TEST(test_uranus_orbital_period);
    /* Neptune */
    RUN_TEST(test_neptune_distance_range);
    RUN_TEST(test_neptune_elements_at_j2000);
    RUN_TEST(test_neptune_orbital_period);
    /* Cross-planet */
    RUN_TEST(test_orbit_is_pure);
    RUN_TEST(test_inner_planets_closer_than_outer);
    RUN_TEST(test_all_planets_valid_positions);
    return UNITY_END();
}
