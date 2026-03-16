/* test_contract_orbit.c — Exhaustive contract tests for orbit.h */

#include "../unity/unity.h"
#include "../../src/systems/astronomy/orbit.h"
#include <math.h>

#define J2000 2451545.0

void setUp(void) { }
void tearDown(void) { }

static const planet_orbit_t *ALL_PLANETS[ORBIT_PLANET_COUNT] = {
    &ORBIT_MERCURY, &ORBIT_VENUS, &ORBIT_EARTH, &ORBIT_MARS,
    &ORBIT_JUPITER, &ORBIT_SATURN, &ORBIT_URANUS, &ORBIT_NEPTUNE
};
static const char *PNAMES[ORBIT_PLANET_COUNT] = {
    "Mercury","Venus","Earth","Mars","Jupiter","Saturn","Uranus","Neptune"
};

void test_all_planets_finite_at_j2000(void) {
    for (int i = 0; i < ORBIT_PLANET_COUNT; i++) {
        orbital_elements_t elem = orbit_elements_at(ALL_PLANETS[i], J2000);
        heliocentric_pos_t pos = orbit_heliocentric(&elem);
        TEST_ASSERT_TRUE_MESSAGE(isfinite(pos.longitude), PNAMES[i]);
        TEST_ASSERT_TRUE_MESSAGE(isfinite(pos.latitude), PNAMES[i]);
        TEST_ASSERT_TRUE_MESSAGE(isfinite(pos.distance), PNAMES[i]);
    }
}

void test_all_planets_finite_sweep(void) {
    int failures = 0;
    for (int i = 0; i < ORBIT_PLANET_COUNT; i++) {
        for (double jd = J2000; jd < J2000 + 18262.5; jd += 30.44) {
            orbital_elements_t elem = orbit_elements_at(ALL_PLANETS[i], jd);
            heliocentric_pos_t pos = orbit_heliocentric(&elem);
            if (!isfinite(pos.longitude) || !isfinite(pos.latitude) || !isfinite(pos.distance))
                failures++;
        }
    }
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, failures, "Non-finite position in 50-year sweep");
}

void test_distance_positive_at_j2000(void) {
    for (int i = 0; i < ORBIT_PLANET_COUNT; i++) {
        orbital_elements_t elem = orbit_elements_at(ALL_PLANETS[i], J2000);
        heliocentric_pos_t pos = orbit_heliocentric(&elem);
        TEST_ASSERT_TRUE_MESSAGE(pos.distance > 0.0, PNAMES[i]);
    }
}

void test_distance_positive_sweep(void) {
    int failures = 0;
    for (int i = 0; i < ORBIT_PLANET_COUNT; i++) {
        for (double jd = J2000; jd < J2000 + 18262.5; jd += 30.44) {
            orbital_elements_t elem = orbit_elements_at(ALL_PLANETS[i], jd);
            heliocentric_pos_t pos = orbit_heliocentric(&elem);
            if (pos.distance <= 0.0) failures++;
        }
    }
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, failures, "Non-positive distance in sweep");
}

void test_longitude_range_at_j2000(void) {
    for (int i = 0; i < ORBIT_PLANET_COUNT; i++) {
        orbital_elements_t elem = orbit_elements_at(ALL_PLANETS[i], J2000);
        heliocentric_pos_t pos = orbit_heliocentric(&elem);
        TEST_ASSERT_TRUE_MESSAGE(pos.longitude >= 0.0, PNAMES[i]);
        TEST_ASSERT_TRUE_MESSAGE(pos.longitude < 360.0, PNAMES[i]);
    }
}

void test_longitude_range_sweep(void) {
    int failures = 0;
    for (int i = 0; i < ORBIT_PLANET_COUNT; i++) {
        for (double jd = J2000; jd < J2000 + 18262.5; jd += 30.44) {
            orbital_elements_t elem = orbit_elements_at(ALL_PLANETS[i], jd);
            heliocentric_pos_t pos = orbit_heliocentric(&elem);
            if (pos.longitude < 0.0 || pos.longitude >= 360.0) failures++;
        }
    }
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, failures, "Longitude out of [0, 360)");
}

void test_latitude_range_at_j2000(void) {
    for (int i = 0; i < ORBIT_PLANET_COUNT; i++) {
        orbital_elements_t elem = orbit_elements_at(ALL_PLANETS[i], J2000);
        heliocentric_pos_t pos = orbit_heliocentric(&elem);
        TEST_ASSERT_TRUE_MESSAGE(pos.latitude >= -90.0, PNAMES[i]);
        TEST_ASSERT_TRUE_MESSAGE(pos.latitude <= 90.0, PNAMES[i]);
    }
}

void test_latitude_range_sweep(void) {
    int failures = 0;
    for (int i = 0; i < ORBIT_PLANET_COUNT; i++) {
        for (double jd = J2000; jd < J2000 + 18262.5; jd += 30.44) {
            orbital_elements_t elem = orbit_elements_at(ALL_PLANETS[i], jd);
            heliocentric_pos_t pos = orbit_heliocentric(&elem);
            if (pos.latitude < -90.0 || pos.latitude > 90.0) failures++;
        }
    }
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, failures, "Latitude out of [-90, 90]");
}

void test_mercury_distance_sanity(void) {
    orbital_elements_t elem = orbit_elements_at(&ORBIT_MERCURY, J2000);
    TEST_ASSERT_TRUE(orbit_heliocentric(&elem).distance < 1.0);
}

void test_neptune_distance_sanity(void) {
    orbital_elements_t elem = orbit_elements_at(&ORBIT_NEPTUNE, J2000);
    TEST_ASSERT_TRUE(orbit_heliocentric(&elem).distance > 25.0);
}

void test_earth_distance_sanity(void) {
    orbital_elements_t elem = orbit_elements_at(&ORBIT_EARTH, J2000);
    TEST_ASSERT_FLOAT_WITHIN(0.05, 1.0, orbit_heliocentric(&elem).distance);
}

void test_jupiter_distance_sanity(void) {
    orbital_elements_t elem = orbit_elements_at(&ORBIT_JUPITER, J2000);
    TEST_ASSERT_TRUE(orbit_heliocentric(&elem).distance > 4.0);
}

void test_elements_finite(void) {
    for (int i = 0; i < ORBIT_PLANET_COUNT; i++) {
        orbital_elements_t elem = orbit_elements_at(ALL_PLANETS[i], J2000);
        TEST_ASSERT_TRUE_MESSAGE(isfinite(elem.a), PNAMES[i]);
        TEST_ASSERT_TRUE_MESSAGE(isfinite(elem.e), PNAMES[i]);
        TEST_ASSERT_TRUE_MESSAGE(isfinite(elem.i), PNAMES[i]);
        TEST_ASSERT_TRUE_MESSAGE(isfinite(elem.omega_n), PNAMES[i]);
        TEST_ASSERT_TRUE_MESSAGE(isfinite(elem.omega_p), PNAMES[i]);
        TEST_ASSERT_TRUE_MESSAGE(isfinite(elem.L), PNAMES[i]);
    }
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_all_planets_finite_at_j2000);
    RUN_TEST(test_all_planets_finite_sweep);
    RUN_TEST(test_distance_positive_at_j2000);
    RUN_TEST(test_distance_positive_sweep);
    RUN_TEST(test_longitude_range_at_j2000);
    RUN_TEST(test_longitude_range_sweep);
    RUN_TEST(test_latitude_range_at_j2000);
    RUN_TEST(test_latitude_range_sweep);
    RUN_TEST(test_mercury_distance_sanity);
    RUN_TEST(test_neptune_distance_sanity);
    RUN_TEST(test_earth_distance_sanity);
    RUN_TEST(test_jupiter_distance_sanity);
    RUN_TEST(test_elements_finite);
    return UNITY_END();
}
