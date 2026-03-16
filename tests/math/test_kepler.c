#include "../unity/unity.h"
#include "../../src/math/kepler.h"
#include <math.h>
#define RAD_EPSILON 1e-6
#define PI 3.14159265358979323846
#define DEG_TO_RAD (PI / 180.0)
void setUp(void) { }
void tearDown(void) { }
void test_earth_orbit(void) { double M = 30.0 * DEG_TO_RAD; double E = kepler_solve(M, 0.0167); TEST_ASSERT_FLOAT_WITHIN(RAD_EPSILON, 0.53207007, E); }
void test_mars_orbit(void) { double M = 120.0 * DEG_TO_RAD; double E = kepler_solve(M, 0.0934); TEST_ASSERT_FLOAT_WITHIN(RAD_EPSILON, 2.171447112, E); }
void test_high_eccentricity(void) { double M = 45.0 * DEG_TO_RAD; double E = kepler_solve(M, 0.9); TEST_ASSERT_FLOAT_WITHIN(RAD_EPSILON, 1.680033736, E); }
void test_circular_orbit(void) { double M = 1.5; double E = kepler_solve(M, 0.0); TEST_ASSERT_FLOAT_WITHIN(RAD_EPSILON, M, E); }
void test_zero_anomaly(void) { double E = kepler_solve(0.0, 0.5); TEST_ASSERT_FLOAT_WITHIN(RAD_EPSILON, 0.0, E); }
void test_pi_anomaly(void) { double E = kepler_solve(PI, 0.5); TEST_ASSERT_FLOAT_WITHIN(RAD_EPSILON, PI, E); }
void test_equation_holds_earth(void) { double M = 30.0 * DEG_TO_RAD; double e = 0.0167; double E = kepler_solve(M, e); double Mc = E - e * sin(E); TEST_ASSERT_FLOAT_WITHIN(1e-9, M, Mc); }
void test_equation_holds_high_ecc(void) { double M = 45.0 * DEG_TO_RAD; double e = 0.9; double E = kepler_solve(M, e); double Mc = E - e * sin(E); TEST_ASSERT_FLOAT_WITHIN(1e-9, M, Mc); }
void test_true_anomaly_circular(void) { double v = eccentric_to_true_anomaly(1.0, 0.0); TEST_ASSERT_FLOAT_WITHIN(RAD_EPSILON, 1.0, v); }
void test_true_anomaly_earth(void) { double v = eccentric_to_true_anomaly(0.53207007, 0.0167); TEST_ASSERT_TRUE(v > 0.53207007); TEST_ASSERT_FLOAT_WITHIN(0.02, 0.5413, v); }
void test_true_anomaly_zero(void) { double v = eccentric_to_true_anomaly(0.0, 0.5); TEST_ASSERT_FLOAT_WITHIN(RAD_EPSILON, 0.0, v); }
void test_venus_orbit(void) { double M = 60.0 * DEG_TO_RAD; double e = 0.0068; double E = kepler_solve(M, e); double Mc = E - e * sin(E); TEST_ASSERT_FLOAT_WITHIN(1e-9, M, Mc); }
void test_jupiter_orbit(void) { double M = 200.0 * DEG_TO_RAD; double e = 0.0484; double E = kepler_solve(M, e); double Mc = E - e * sin(E); TEST_ASSERT_FLOAT_WITHIN(1e-9, M, Mc); }
void test_saturn_orbit(void) { double M = 300.0 * DEG_TO_RAD; double e = 0.0539; double E = kepler_solve(M, e); double Mc = E - e * sin(E); TEST_ASSERT_FLOAT_WITHIN(1e-9, M, Mc); }
void test_near_parabolic(void) { double M = 10.0 * DEG_TO_RAD; double e = 0.99; double E = kepler_solve(M, e); double Mc = E - e * sin(E); TEST_ASSERT_FLOAT_WITHIN(1e-6, M, Mc); }
void test_equation_holds_many_anomalies(void) { double e = 0.0934; for (int deg = 0; deg <= 350; deg += 10) { double M = deg * DEG_TO_RAD; double E = kepler_solve(M, e); double Mc = E - e * sin(E); TEST_ASSERT_FLOAT_WITHIN(1e-8, M, Mc); } }
void test_equation_holds_various_eccentricities(void) { double M = 90.0 * DEG_TO_RAD; double eccs[] = {0.0, 0.01, 0.1, 0.3, 0.5, 0.7, 0.9}; for (int i = 0; i < 7; i++) { double E = kepler_solve(M, eccs[i]); double Mc = E - eccs[i] * sin(E); TEST_ASSERT_FLOAT_WITHIN(1e-8, M, Mc); } }
void test_E_increases_with_M(void) { double e = 0.3; double Ep = kepler_solve(0.0, e); for (int deg = 10; deg <= 180; deg += 10) { double M = deg * DEG_TO_RAD; double E = kepler_solve(M, e); TEST_ASSERT_TRUE(E > Ep); Ep = E; } }
void test_true_anomaly_pi(void) { double v = eccentric_to_true_anomaly(PI, 0.5); TEST_ASSERT_FLOAT_WITHIN(RAD_EPSILON, PI, v); }
void test_true_anomaly_greater_than_E_first_half(void) { double e = 0.3; for (int deg = 10; deg <= 170; deg += 20) { double E = deg * DEG_TO_RAD; double v = eccentric_to_true_anomaly(E, e); TEST_ASSERT_TRUE(v > E); } }
void test_true_anomaly_roundtrip_through_kepler(void) { double M = 45.0 * DEG_TO_RAD; double e = 0.3; double E = kepler_solve(M, e); double v = eccentric_to_true_anomaly(E, e); TEST_ASSERT_TRUE(v > M); }
void test_kepler_solve_is_pure(void) { double M = 123.456 * DEG_TO_RAD; double e = 0.42; double E1 = kepler_solve(M, e); double E2 = kepler_solve(M, e); TEST_ASSERT_EQUAL_FLOAT(E1, E2); }
void test_true_anomaly_is_pure(void) { double v1 = eccentric_to_true_anomaly(1.5, 0.3); double v2 = eccentric_to_true_anomaly(1.5, 0.3); TEST_ASSERT_EQUAL_FLOAT(v1, v2); }
void test_kepler_solve_small_M(void) { double M = 0.001; double e = 0.5; double E = kepler_solve(M, e); double Mc = E - e * sin(E); TEST_ASSERT_FLOAT_WITHIN(1e-9, M, Mc); }
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_earth_orbit); RUN_TEST(test_mars_orbit); RUN_TEST(test_high_eccentricity); RUN_TEST(test_circular_orbit); RUN_TEST(test_zero_anomaly); RUN_TEST(test_pi_anomaly); RUN_TEST(test_equation_holds_earth); RUN_TEST(test_equation_holds_high_ecc); RUN_TEST(test_true_anomaly_circular); RUN_TEST(test_true_anomaly_earth); RUN_TEST(test_true_anomaly_zero);
    RUN_TEST(test_venus_orbit); RUN_TEST(test_jupiter_orbit); RUN_TEST(test_saturn_orbit); RUN_TEST(test_near_parabolic); RUN_TEST(test_equation_holds_many_anomalies); RUN_TEST(test_equation_holds_various_eccentricities); RUN_TEST(test_E_increases_with_M); RUN_TEST(test_true_anomaly_pi); RUN_TEST(test_true_anomaly_greater_than_E_first_half); RUN_TEST(test_true_anomaly_roundtrip_through_kepler); RUN_TEST(test_kepler_solve_is_pure); RUN_TEST(test_true_anomaly_is_pure); RUN_TEST(test_kepler_solve_small_M);
    return UNITY_END();
}
