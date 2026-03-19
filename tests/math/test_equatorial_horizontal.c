#include "../unity/unity.h"
#include "../../src/math/equatorial_horizontal.h"
#include "../../src/math/sidereal.h"
#include <math.h>

#define DEG_EPSILON      0.01
#define DEG_EPSILON_ASTRO 0.5
#define PI 3.14159265358979323846

void setUp(void) { }
void tearDown(void) { }

/* --- Hour angle tests --- */

void test_eh_hour_angle_basic(void) {
    /* LST=90, RA=45 -> H = 45 degrees */
    double h = eh_hour_angle(90.0, 45.0);
    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 45.0, h);
}

void test_eh_hour_angle_wraps(void) {
    /* LST=10, RA=350 -> H = 10 - 350 = -340 -> normalized = 20 degrees */
    double h = eh_hour_angle(10.0, 350.0);
    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 20.0, h);
}

void test_eh_hour_angle_zero(void) {
    /* LST = RA -> H = 0 (object on meridian) */
    double h = eh_hour_angle(123.456, 123.456);
    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 0.0, h);
}

/* --- Equatorial to horizontal tests --- */

void test_eh_zenith_overhead(void) {
    /* Star with Dec = observer lat and H = 0 (on meridian) -> altitude = 90 (zenith) */
    equatorial_t eq = { .ra = 100.0, .dec = 40.0 };
    double lat = 40.0;
    double lst = 100.0; /* LST = RA so H = 0 */
    horizontal_t hz = eh_to_horizontal(eq, lat, lst);
    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 90.0, hz.altitude);
}

void test_eh_on_horizon(void) {
    /* Observer at equator (lat=0), star at Dec=0, H=90 degrees (6 hours) ->
     * sin(alt) = sin(0)*sin(0) + cos(0)*cos(0)*cos(90) = 0 -> alt = 0 */
    equatorial_t eq = { .ra = 0.0, .dec = 0.0 };
    double lat = 0.0;
    double lst = 90.0; /* H = LST - RA = 90 */
    horizontal_t hz = eh_to_horizontal(eq, lat, lst);
    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 0.0, hz.altitude);
}

void test_eh_polaris_north(void) {
    /* Polaris: Dec ≈ 89.26, RA ≈ 37.95 degrees (2h 31m 49s).
     * Observer at lat = 40N. LST = RA so H = 0.
     * With H=0 and Dec≈90: altitude ≈ observer latitude.
     * For Dec=89.26 exactly: sin(alt) = sin(89.26)*sin(40) + cos(89.26)*cos(40)*cos(0)
     *   = 0.99992 * 0.64279 + 0.01292 * 0.76604 = 0.64274 + 0.00990 = 0.65264
     *   Actually Polaris altitude ~ latitude (well known approximation)
     *   More precisely: for H=0, alt = 90 - |lat - dec| = 90 - |40 - 89.26| = 90 - 49.26 = 40.74
     *   But actually the exact formula: sin(alt) = sin(dec)*sin(lat) + cos(dec)*cos(lat)
     *     = sin(89.26)*sin(40) + cos(89.26)*cos(40)
     *     Using exact formula: this gives alt ≈ 40.74 but Polaris is so close to pole
     *   At H=0 it transits above pole, alt = 90 - (lat - dec)... no.
     *   Formula: alt = 90 - |dec_comp - lat| at upper culmination H=0 where dec_comp = 90-dec = 0.74
     *   Wait: H=0 upper culmination: alt = 90 - |lat - dec| = 90 - |40 - 89.26| = 40.74. Yes.
     *   Azimuth should be 0 (North) or 180 (South) — Polaris dec > lat, so it transits North of zenith.
     *   For dec > lat and H=0, azimuth = 0 (due North). */
    equatorial_t eq = { .ra = 37.95, .dec = 89.26 };
    double lat = 40.0;
    double lst = 37.95; /* H = 0 */
    horizontal_t hz = eh_to_horizontal(eq, lat, lst);
    /* Altitude ≈ latitude for Polaris */
    TEST_ASSERT_FLOAT_WITHIN(1.0, 40.74, hz.altitude);
    /* Azimuth should be near North (0 or 360) */
    double az_near_north = hz.azimuth;
    if (az_near_north > 180.0) az_near_north -= 360.0;
    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON_ASTRO, 0.0, az_near_north);
}

void test_eh_south_celestial_pole(void) {
    /* Dec = -90 (south celestial pole), observer at lat = -45 (southern hemisphere).
     * H = 0 (on meridian).
     * sin(alt) = sin(-90)*sin(-45) + cos(-90)*cos(-45)*cos(0)
     *          = (-1)*(-0.7071) + 0 = 0.7071 -> alt = 45 degrees.
     * South celestial pole from southern hemisphere: azimuth = 180 (due South). */
    equatorial_t eq = { .ra = 50.0, .dec = -90.0 };
    double lat = -45.0;
    double lst = 50.0; /* H = 0 */
    horizontal_t hz = eh_to_horizontal(eq, lat, lst);
    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 45.0, hz.altitude);
    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON_ASTRO, 180.0, hz.azimuth);
}

/* --- Roundtrip test --- */

void test_eh_roundtrip(void) {
    /* Convert equatorial -> horizontal -> equatorial. Should recover original RA/Dec. */
    equatorial_t eq_in = { .ra = 150.0, .dec = 35.0 };
    double lat = 42.0;
    double lst = 200.0;
    horizontal_t hz = eh_to_horizontal(eq_in, lat, lst);
    equatorial_t eq_out = eh_to_equatorial(hz, lat, lst);
    /* Normalize RA to 0-360 for comparison */
    double ra_out = fmod(eq_out.ra, 360.0);
    if (ra_out < 0.0) ra_out += 360.0;
    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, eq_in.ra, ra_out);
    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, eq_in.dec, eq_out.dec);
}

void test_eh_roundtrip_negative_dec(void) {
    /* Test roundtrip with negative declination */
    equatorial_t eq_in = { .ra = 270.0, .dec = -25.0 };
    double lat = 51.5; /* London */
    double lst = 300.0;
    horizontal_t hz = eh_to_horizontal(eq_in, lat, lst);
    equatorial_t eq_out = eh_to_equatorial(hz, lat, lst);
    double ra_out = fmod(eq_out.ra, 360.0);
    if (ra_out < 0.0) ra_out += 360.0;
    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, eq_in.ra, ra_out);
    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, eq_in.dec, eq_out.dec);
}

/* --- Visibility tests --- */

void test_eh_is_visible_above(void) {
    horizontal_t hz = { .altitude = 10.0, .azimuth = 90.0 };
    TEST_ASSERT_EQUAL_INT(1, eh_is_visible(hz));
}

void test_eh_is_visible_below(void) {
    horizontal_t hz = { .altitude = -5.0, .azimuth = 180.0 };
    TEST_ASSERT_EQUAL_INT(0, eh_is_visible(hz));
}

void test_eh_is_visible_exactly_zero(void) {
    horizontal_t hz = { .altitude = 0.0, .azimuth = 0.0 };
    /* On the horizon: technically visible (altitude >= 0) */
    TEST_ASSERT_EQUAL_INT(1, eh_is_visible(hz));
}

/* --- Refraction tests --- */

void test_eh_refraction_horizon(void) {
    /* At horizon (0 degrees): Bennett formula gives ≈ 0.48 degrees (≈29 arcmin).
     * (Some sources cite ~34 arcmin from other formulae; Bennett is slightly lower.) */
    double r = eh_refraction(0.0);
    TEST_ASSERT_FLOAT_WITHIN(0.1, 0.48, r);
}

void test_eh_refraction_zenith(void) {
    /* At zenith (90 degrees): refraction ≈ 0 degrees */
    double r = eh_refraction(90.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 0.0, r);
}

void test_eh_refraction_positive(void) {
    /* Refraction should always be positive for any altitude */
    double alts[] = {0.0, 5.0, 10.0, 30.0, 45.0, 60.0, 90.0};
    for (int i = 0; i < 7; i++) {
        double r = eh_refraction(alts[i]);
        TEST_ASSERT_TRUE(r >= 0.0);
    }
}

void test_eh_refraction_decreases_with_altitude(void) {
    /* Refraction should decrease as altitude increases */
    double r_low = eh_refraction(5.0);
    double r_mid = eh_refraction(45.0);
    double r_high = eh_refraction(85.0);
    TEST_ASSERT_TRUE(r_low > r_mid);
    TEST_ASSERT_TRUE(r_mid > r_high);
}

/* --- Convenience JD function test --- */

void test_eh_to_horizontal_jd(void) {
    /* Verify convenience function matches manual computation.
     * Use J2000.0 epoch: JD = 2451545.0 */
    equatorial_t eq = { .ra = 100.0, .dec = 30.0 };
    double lat = 40.0;
    double lon = -74.0; /* New York longitude */
    double jd = 2451545.0;

    /* Manual computation */
    double gast = gast_degrees(jd);
    double lst = lst_degrees(gast, lon);
    horizontal_t hz_manual = eh_to_horizontal(eq, lat, lst);

    /* Convenience function */
    horizontal_t hz_auto = eh_to_horizontal_jd(eq, lat, lon, jd);

    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, hz_manual.altitude, hz_auto.altitude);
    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, hz_manual.azimuth, hz_auto.azimuth);
}

/* --- Azimuth range test --- */

void test_eh_azimuth_range(void) {
    /* Verify azimuth is always in [0, 360) for many different inputs */
    double ras[] = {0.0, 45.0, 90.0, 135.0, 180.0, 225.0, 270.0, 315.0, 359.0};
    double decs[] = {-60.0, -30.0, 0.0, 30.0, 60.0};
    double lat = 40.0;
    double lst = 100.0;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 5; j++) {
            equatorial_t eq = { .ra = ras[i], .dec = decs[j] };
            horizontal_t hz = eh_to_horizontal(eq, lat, lst);
            TEST_ASSERT_TRUE(hz.azimuth >= 0.0);
            TEST_ASSERT_TRUE(hz.azimuth < 360.0);
        }
    }
}

/* --- Additional edge case tests --- */

void test_eh_equator_observer_south_star(void) {
    /* Observer at equator, star at Dec=-45 on meridian (H=0).
     * alt = 90 - |0 - (-45)| = 90 - 45 = 45 degrees
     * Should be south (azimuth=180) */
    equatorial_t eq = { .ra = 50.0, .dec = -45.0 };
    double lat = 0.0;
    double lst = 50.0; /* H=0 */
    horizontal_t hz = eh_to_horizontal(eq, lat, lst);
    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 45.0, hz.altitude);
    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON_ASTRO, 180.0, hz.azimuth);
}

void test_eh_purity(void) {
    /* Same inputs should always produce same outputs (pure function) */
    equatorial_t eq = { .ra = 100.0, .dec = 30.0 };
    horizontal_t a = eh_to_horizontal(eq, 40.0, 200.0);
    horizontal_t b = eh_to_horizontal(eq, 40.0, 200.0);
    TEST_ASSERT_EQUAL_FLOAT(a.altitude, b.altitude);
    TEST_ASSERT_EQUAL_FLOAT(a.azimuth, b.azimuth);
}

int main(void) {
    UNITY_BEGIN();
    /* Hour angle */
    RUN_TEST(test_eh_hour_angle_basic);
    RUN_TEST(test_eh_hour_angle_wraps);
    RUN_TEST(test_eh_hour_angle_zero);
    /* Equatorial to horizontal */
    RUN_TEST(test_eh_zenith_overhead);
    RUN_TEST(test_eh_on_horizon);
    RUN_TEST(test_eh_polaris_north);
    RUN_TEST(test_eh_south_celestial_pole);
    /* Roundtrip */
    RUN_TEST(test_eh_roundtrip);
    RUN_TEST(test_eh_roundtrip_negative_dec);
    /* Visibility */
    RUN_TEST(test_eh_is_visible_above);
    RUN_TEST(test_eh_is_visible_below);
    RUN_TEST(test_eh_is_visible_exactly_zero);
    /* Refraction */
    RUN_TEST(test_eh_refraction_horizon);
    RUN_TEST(test_eh_refraction_zenith);
    RUN_TEST(test_eh_refraction_positive);
    RUN_TEST(test_eh_refraction_decreases_with_altitude);
    /* Convenience */
    RUN_TEST(test_eh_to_horizontal_jd);
    /* Azimuth range */
    RUN_TEST(test_eh_azimuth_range);
    /* Edge cases */
    RUN_TEST(test_eh_equator_observer_south_star);
    RUN_TEST(test_eh_purity);
    return UNITY_END();
}
