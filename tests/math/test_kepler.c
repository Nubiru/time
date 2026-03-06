#include "../unity/unity.h"
#include "../../src/math/kepler.h"
#include <math.h>

#define RAD_EPSILON 1e-6
#define PI 3.14159265358979323846
#define DEG_TO_RAD (PI / 180.0)

void setUp(void) { }
void tearDown(void) { }

/* --- Card 007 Q4 worked examples --- */

void test_earth_orbit(void) {
    /* e=0.0167, M=30° -> E ≈ 0.53207 rad (30.485°) */
    double M = 30.0 * DEG_TO_RAD;
    double E = kepler_solve(M, 0.0167);
    TEST_ASSERT_FLOAT_WITHIN(RAD_EPSILON, 0.53207007, E);
}

void test_mars_orbit(void) {
    /* e=0.0934, M=120° -> E ≈ 2.17145 rad */
    double M = 120.0 * DEG_TO_RAD;
    double E = kepler_solve(M, 0.0934);
    TEST_ASSERT_FLOAT_WITHIN(RAD_EPSILON, 2.171447112, E);
}

void test_high_eccentricity(void) {
    /* e=0.9, M=45° -> E ≈ 1.68003 rad */
    double M = 45.0 * DEG_TO_RAD;
    double E = kepler_solve(M, 0.9);
    TEST_ASSERT_FLOAT_WITHIN(RAD_EPSILON, 1.680033736, E);
}

/* --- Edge cases --- */

void test_circular_orbit(void) {
    /* e=0: E = M exactly */
    double M = 1.5;
    double E = kepler_solve(M, 0.0);
    TEST_ASSERT_FLOAT_WITHIN(RAD_EPSILON, M, E);
}

void test_zero_anomaly(void) {
    /* M=0: E=0 for any eccentricity */
    double E = kepler_solve(0.0, 0.5);
    TEST_ASSERT_FLOAT_WITHIN(RAD_EPSILON, 0.0, E);
}

void test_pi_anomaly(void) {
    /* M=pi: E=pi for any eccentricity (sin(pi) = 0) */
    double E = kepler_solve(PI, 0.5);
    TEST_ASSERT_FLOAT_WITHIN(RAD_EPSILON, PI, E);
}

/* --- Verify Kepler's equation holds: M = E - e*sin(E) --- */

void test_equation_holds_earth(void) {
    double M = 30.0 * DEG_TO_RAD;
    double e = 0.0167;
    double E = kepler_solve(M, e);
    double M_check = E - e * sin(E);
    TEST_ASSERT_FLOAT_WITHIN(1e-9, M, M_check);
}

void test_equation_holds_high_ecc(void) {
    double M = 45.0 * DEG_TO_RAD;
    double e = 0.9;
    double E = kepler_solve(M, e);
    double M_check = E - e * sin(E);
    TEST_ASSERT_FLOAT_WITHIN(1e-9, M, M_check);
}

/* --- True anomaly tests --- */

void test_true_anomaly_circular(void) {
    /* e=0: true anomaly = eccentric anomaly */
    double v = eccentric_to_true_anomaly(1.0, 0.0);
    TEST_ASSERT_FLOAT_WITHIN(RAD_EPSILON, 1.0, v);
}

void test_true_anomaly_earth(void) {
    /* e=0.0167, E=0.53207 rad -> v ≈ 0.5413 rad */
    double v = eccentric_to_true_anomaly(0.53207007, 0.0167);
    /* For near-circular orbits, v is close to E but slightly larger */
    TEST_ASSERT_TRUE(v > 0.53207007);
    TEST_ASSERT_FLOAT_WITHIN(0.02, 0.5413, v);
}

void test_true_anomaly_zero(void) {
    /* E=0 -> v=0 for any eccentricity */
    double v = eccentric_to_true_anomaly(0.0, 0.5);
    TEST_ASSERT_FLOAT_WITHIN(RAD_EPSILON, 0.0, v);
}

/* --- Runner --- */

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_earth_orbit);
    RUN_TEST(test_mars_orbit);
    RUN_TEST(test_high_eccentricity);
    RUN_TEST(test_circular_orbit);
    RUN_TEST(test_zero_anomaly);
    RUN_TEST(test_pi_anomaly);
    RUN_TEST(test_equation_holds_earth);
    RUN_TEST(test_equation_holds_high_ecc);
    RUN_TEST(test_true_anomaly_circular);
    RUN_TEST(test_true_anomaly_earth);
    RUN_TEST(test_true_anomaly_zero);
    return UNITY_END();
}
