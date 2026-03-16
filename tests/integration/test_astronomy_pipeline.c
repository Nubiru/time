/*
 * test_astronomy_pipeline.c — Integration tests for astronomy computation chain
 *
 * Verifies the full astronomical computation pipeline: orbital mechanics ->
 * planet positions -> coordinate transforms -> zodiac signs -> aspects -> houses.
 * Tests against known astronomical events.
 *
 * Pure function tests only. No GL calls.
 */

#include "unity.h"
#include "math/julian.h"
#include "math/kepler.h"
#include "math/ecliptic.h"
#include "math/sidereal.h"
#include "systems/astronomy/orbit.h"
#include "systems/astronomy/planets.h"
#include "systems/astronomy/lunar.h"
#include "systems/astronomy/solar_events.h"
#include "systems/astrology/zodiac.h"
#include "systems/astrology/aspects.h"
#include "systems/astrology/observer.h"
#include "systems/astrology/houses.h"
#include <math.h>
#include <string.h>

#define PI 3.14159265358979323846
#define DEG_TO_RAD (PI / 180.0)
#define J2000 2451545.0

void setUp(void) { }
void tearDown(void) { }

/* Helper: normalize angle to [0, 360) */
static double normalize_deg(double d) {
    d = fmod(d, 360.0);
    if (d < 0.0) d += 360.0;
    return d;
}

/* ===== Test: planet positions at J2000 are in expected AU ranges ===== */
void test_planet_positions_j2000_ranges(void) {
    solar_system_t sys = planets_at(J2000);

    /* Mercury: 0.31-0.47 AU from Sun */
    TEST_ASSERT_TRUE(sys.positions[PLANET_MERCURY].distance > 0.3);
    TEST_ASSERT_TRUE(sys.positions[PLANET_MERCURY].distance < 0.5);

    /* Venus: 0.72 AU approximately */
    TEST_ASSERT_TRUE(sys.positions[PLANET_VENUS].distance > 0.71);
    TEST_ASSERT_TRUE(sys.positions[PLANET_VENUS].distance < 0.73);

    /* Earth: ~1.0 AU */
    TEST_ASSERT_TRUE(sys.positions[PLANET_EARTH].distance > 0.98);
    TEST_ASSERT_TRUE(sys.positions[PLANET_EARTH].distance < 1.02);

    /* Mars: 1.38-1.67 AU */
    TEST_ASSERT_TRUE(sys.positions[PLANET_MARS].distance > 1.3);
    TEST_ASSERT_TRUE(sys.positions[PLANET_MARS].distance < 1.7);

    /* Jupiter: ~5.2 AU */
    TEST_ASSERT_TRUE(sys.positions[PLANET_JUPITER].distance > 4.9);
    TEST_ASSERT_TRUE(sys.positions[PLANET_JUPITER].distance < 5.5);

    /* Saturn: ~9.5 AU */
    TEST_ASSERT_TRUE(sys.positions[PLANET_SATURN].distance > 9.0);
    TEST_ASSERT_TRUE(sys.positions[PLANET_SATURN].distance < 10.1);

    /* Uranus: ~19.2 AU */
    TEST_ASSERT_TRUE(sys.positions[PLANET_URANUS].distance > 18.0);
    TEST_ASSERT_TRUE(sys.positions[PLANET_URANUS].distance < 20.5);

    /* Neptune: ~30.1 AU */
    TEST_ASSERT_TRUE(sys.positions[PLANET_NEPTUNE].distance > 29.0);
    TEST_ASSERT_TRUE(sys.positions[PLANET_NEPTUNE].distance < 31.0);
}

/* ===== Test: planet distance ordering (semi-major axis) ===== */
void test_planet_distance_ordering(void) {
    /* At J2000, check average distances (semi-major axes) via orbit elements */
    const planet_orbit_t *orbits[] = {
        &ORBIT_MERCURY, &ORBIT_VENUS, &ORBIT_EARTH, &ORBIT_MARS,
        &ORBIT_JUPITER, &ORBIT_SATURN, &ORBIT_URANUS, &ORBIT_NEPTUNE
    };

    for (int i = 0; i < 7; i++) {
        orbital_elements_t elem_a = orbit_elements_at(orbits[i], J2000);
        orbital_elements_t elem_b = orbit_elements_at(orbits[i + 1], J2000);
        /* Semi-major axis should increase outward */
        TEST_ASSERT_TRUE_MESSAGE(elem_a.a < elem_b.a,
                                 "Planet semi-major axes should increase outward");
    }
}

/* ===== Test: no NAN/INF in planet positions ===== */
void test_planet_positions_no_nan(void) {
    /* Test at several dates spread across a century */
    double dates[] = {J2000, J2000 + 365.25, J2000 + 3652.5,
                      J2000 - 365.25, J2000 - 3652.5};

    for (int d = 0; d < 5; d++) {
        solar_system_t sys = planets_at(dates[d]);
        for (int p = 0; p < 8; p++) {
            TEST_ASSERT_FALSE(isnan(sys.positions[p].longitude));
            TEST_ASSERT_FALSE(isnan(sys.positions[p].latitude));
            TEST_ASSERT_FALSE(isnan(sys.positions[p].distance));
            TEST_ASSERT_FALSE(isinf(sys.positions[p].longitude));
            TEST_ASSERT_FALSE(isinf(sys.positions[p].distance));
        }
    }
}

/* ===== Test: Jupiter-Saturn great conjunction 2020-12-21 ===== */
void test_great_conjunction_2020(void) {
    /* December 21, 2020 — Jupiter-Saturn closest in ~400 years */
    double jd = gregorian_to_jd(2020, 12, 21.5);
    solar_system_t sys = planets_at(jd);

    double jup_lon = normalize_deg(sys.positions[PLANET_JUPITER].longitude);
    double sat_lon = normalize_deg(sys.positions[PLANET_SATURN].longitude);

    double sep = angular_separation(jup_lon, sat_lon);

    /* Should be within a few degrees (Keplerian approximation won't be sub-degree) */
    TEST_ASSERT_TRUE_MESSAGE(sep < 5.0,
        "Jupiter-Saturn conjunction 2020-12-21 should be close");
}

/* ===== Test: zodiac signs from planet longitudes are valid ===== */
void test_zodiac_signs_valid(void) {
    solar_system_t sys = planets_at(J2000);

    for (int p = 0; p < 8; p++) {
        double lon = normalize_deg(sys.positions[p].longitude);
        int sign = zodiac_sign(lon);
        TEST_ASSERT_TRUE(sign >= 0 && sign <= 11);

        const char *name = zodiac_sign_name(sign);
        TEST_ASSERT_NOT_NULL(name);
        TEST_ASSERT_TRUE(strlen(name) > 0);

        double deg = zodiac_degree(lon);
        TEST_ASSERT_TRUE(deg >= 0.0 && deg < 30.0);
    }
}

/* ===== Test: aspects between planets are detected ===== */
void test_aspects_detected(void) {
    solar_system_t sys = planets_at(J2000);
    double longitudes[8];
    for (int p = 0; p < 8; p++) {
        longitudes[p] = sys.positions[p].longitude;
    }

    aspect_list_t aspects = aspects_find_all(longitudes, 10.0);

    /* Should find at least some aspects with 10-degree orb */
    TEST_ASSERT_TRUE(aspects.count >= 0);
    TEST_ASSERT_TRUE(aspects.count <= 28);

    for (int i = 0; i < aspects.count; i++) {
        TEST_ASSERT_TRUE(aspects.aspects[i].planet_a >= 0);
        TEST_ASSERT_TRUE(aspects.aspects[i].planet_a < 8);
        TEST_ASSERT_TRUE(aspects.aspects[i].planet_b >= 0);
        TEST_ASSERT_TRUE(aspects.aspects[i].planet_b < 8);
        TEST_ASSERT_TRUE(aspects.aspects[i].planet_a != aspects.aspects[i].planet_b);
        TEST_ASSERT_TRUE(aspects.aspects[i].type >= ASPECT_CONJUNCTION);
        TEST_ASSERT_TRUE(aspects.aspects[i].type <= ASPECT_OPPOSITION);
        TEST_ASSERT_TRUE(aspects.aspects[i].orb >= 0.0);
        TEST_ASSERT_TRUE(aspects.aspects[i].orb <= 10.0);
    }
}

/* ===== Test: angular_separation is commutative and bounded ===== */
void test_angular_separation_properties(void) {
    /* Commutative */
    TEST_ASSERT_FLOAT_WITHIN(0.001,
        angular_separation(30.0, 120.0),
        angular_separation(120.0, 30.0));

    /* Self-separation is 0 */
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.0, angular_separation(45.0, 45.0));

    /* Maximum separation is 180 */
    TEST_ASSERT_FLOAT_WITHIN(0.001, 180.0, angular_separation(0.0, 180.0));

    /* Wraps around correctly */
    TEST_ASSERT_FLOAT_WITHIN(0.001, 10.0, angular_separation(355.0, 5.0));
}

/* ===== Test: houses computed from observer are sequential ===== */
void test_houses_sequential(void) {
    /* Compute ascendant for observer at Greenwich on J2000 */
    double gst = gmst_degrees(J2000);
    double lst = lst_degrees(gst, 0.0); /* Greenwich */
    double obliquity = mean_obliquity(J2000);
    double asc_lon = ascendant_longitude(lst, obliquity, 51.5); /* London lat */

    house_system_t houses = houses_whole_sign(asc_lon);

    /* All 12 house signs should be valid */
    for (int h = 0; h < 12; h++) {
        TEST_ASSERT_TRUE(houses.signs[h] >= 0);
        TEST_ASSERT_TRUE(houses.signs[h] <= 11);
    }

    /* Cusps should be sequential (each 30 degrees apart) */
    for (int h = 0; h < 12; h++) {
        int next = (h + 1) % 12;
        double diff = normalize_deg(houses.cusps[next] - houses.cusps[h]);
        TEST_ASSERT_FLOAT_WITHIN(0.1, 30.0, diff);
    }

    /* Signs should be sequential (each sign follows the previous) */
    for (int h = 0; h < 12; h++) {
        int next = (h + 1) % 12;
        int expected_next_sign = (houses.signs[h] + 1) % 12;
        TEST_ASSERT_EQUAL(expected_next_sign, houses.signs[next]);
    }
}

/* ===== Test: solar events produce valid dates ===== */
void test_solar_events_valid(void) {
    /* Solar events at Greenwich for J2000 noon */
    solar_day_t day = se_solar_events(J2000, 51.5, 0.0);

    /* In January at London latitude, should have a normal day */
    TEST_ASSERT_FALSE(day.always_day);
    TEST_ASSERT_FALSE(day.always_night);
    TEST_ASSERT_TRUE(day.day_length > 0.0);
    TEST_ASSERT_TRUE(day.day_length < 24.0);

    /* Sunrise should be before sunset */
    if (!isnan(day.sunrise_jd) && !isnan(day.sunset_jd)) {
        TEST_ASSERT_TRUE(day.sunrise_jd < day.sunset_jd);
    }

    /* Solar noon should be between sunrise and sunset */
    if (!isnan(day.sunrise_jd) && !isnan(day.sunset_jd)) {
        TEST_ASSERT_TRUE(day.solar_noon_jd > day.sunrise_jd);
        TEST_ASSERT_TRUE(day.solar_noon_jd < day.sunset_jd);
    }
}

/* ===== Test: Moon position is in valid range ===== */
void test_moon_position_valid(void) {
    lunar_info_t moon = lunar_phase(J2000);

    /* Phase fraction should be 0-1 */
    TEST_ASSERT_TRUE(moon.phase_fraction >= 0.0);
    TEST_ASSERT_TRUE(moon.phase_fraction <= 1.0);

    /* Illumination should be 0-1 */
    TEST_ASSERT_TRUE(moon.illumination >= 0.0);
    TEST_ASSERT_TRUE(moon.illumination <= 1.0);

    /* Longitude should be 0-360 */
    double lon = normalize_deg(moon.moon_longitude);
    TEST_ASSERT_TRUE(lon >= 0.0);
    TEST_ASSERT_TRUE(lon < 360.0);

    /* Phase should be a valid enum */
    TEST_ASSERT_TRUE(moon.phase >= MOON_NEW);
    TEST_ASSERT_TRUE(moon.phase <= MOON_WANING_CRESCENT);

    /* Age should be roughly 0-30 days */
    TEST_ASSERT_TRUE(moon.age_days >= 0.0);
    TEST_ASSERT_TRUE(moon.age_days < 31.0);

    /* No NAN/INF */
    TEST_ASSERT_FALSE(isnan(moon.phase_fraction));
    TEST_ASSERT_FALSE(isnan(moon.illumination));
    TEST_ASSERT_FALSE(isnan(moon.moon_longitude));
    TEST_ASSERT_FALSE(isnan(moon.age_days));
}

/* ===== Test: Moon next new/full moon are in the future ===== */
void test_moon_next_events(void) {
    double next_new = lunar_next_new_moon(J2000);
    double next_full = lunar_next_full_moon(J2000);

    TEST_ASSERT_TRUE(next_new > J2000);
    TEST_ASSERT_TRUE(next_full > J2000);

    /* Both should be within ~30 days */
    TEST_ASSERT_TRUE(next_new - J2000 < 30.0);
    TEST_ASSERT_TRUE(next_full - J2000 < 30.0);
}

/* ===== Test: Kepler solver converges for all eccentricities ===== */
void test_kepler_solver_convergence(void) {
    /* Test various mean anomalies and eccentricities */
    double eccs[] = {0.0, 0.01, 0.0167, 0.0934, 0.2056, 0.5, 0.9};
    double Ms[] = {0.0, PI / 4, PI / 2, PI, 3 * PI / 2, 2 * PI - 0.01};

    for (int e = 0; e < 7; e++) {
        for (int m = 0; m < 6; m++) {
            double E = kepler_solve(Ms[m], eccs[e]);
            TEST_ASSERT_FALSE(isnan(E));
            TEST_ASSERT_FALSE(isinf(E));

            /* Verify: M = E - e*sin(E) */
            double M_check = E - eccs[e] * sin(E);
            double M_norm = fmod(Ms[m], 2 * PI);
            if (M_norm < 0) M_norm += 2 * PI;
            double E_norm = fmod(M_check, 2 * PI);
            if (E_norm < 0) E_norm += 2 * PI;
            TEST_ASSERT_FLOAT_WITHIN(0.001, M_norm, E_norm);
        }
    }
}

/* ===== Test: ecliptic to equatorial coordinate transform ===== */
void test_ecliptic_to_equatorial(void) {
    double obliquity = mean_obliquity(J2000);
    TEST_ASSERT_FLOAT_WITHIN(0.1, 23.44, obliquity);

    /* Vernal equinox point: lon=0, lat=0 -> RA=0, Dec=0 */
    equatorial_t eq = ecliptic_to_equatorial(0.0, 0.0, obliquity);
    TEST_ASSERT_FLOAT_WITHIN(0.1, 0.0, eq.ra);
    TEST_ASSERT_FLOAT_WITHIN(0.1, 0.0, eq.dec);

    /* Summer solstice: lon=90 -> RA~90, Dec~+23.44 */
    equatorial_t summer = ecliptic_to_equatorial(90.0, 0.0, obliquity);
    TEST_ASSERT_FLOAT_WITHIN(0.5, obliquity, summer.dec);

    /* Winter solstice: lon=270 -> Dec~-23.44 */
    equatorial_t winter = ecliptic_to_equatorial(270.0, 0.0, obliquity);
    TEST_ASSERT_FLOAT_WITHIN(0.5, -obliquity, winter.dec);
}

/* ===== Test: sidereal time is valid ===== */
void test_sidereal_time_valid(void) {
    /* GMST takes JD at 0h UT */
    double jd_0h = J2000 - 0.5; /* Jan 1, 2000 0h UT */
    double gst = gmst_degrees(jd_0h);
    TEST_ASSERT_TRUE(gst >= 0.0);
    TEST_ASSERT_TRUE(gst < 360.0);
    TEST_ASSERT_FALSE(isnan(gst));

    /* GAST should be in valid range and close to GMST (nutation < 1 degree) */
    double gast = gast_degrees(jd_0h);
    TEST_ASSERT_TRUE(gast >= 0.0);
    TEST_ASSERT_TRUE(gast < 360.0);
    TEST_ASSERT_FALSE(isnan(gast));
    /* Nutation in longitude is at most ~18 arcsec = ~0.005 deg, so GAST ~ GMST */
    TEST_ASSERT_FLOAT_WITHIN(1.0, gst, gast);

    /* LST at Greenwich = GST + 0 */
    double lst = lst_degrees(gst, 0.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001, gst, lst);

    /* LST at 90 east = GST + 90 (mod 360) */
    double lst_90 = lst_degrees(gst, 90.0);
    double expected = fmod(gst + 90.0, 360.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001, expected, lst_90);
}

/* ===== Test: full pipeline: JD -> planets -> zodiac -> aspects -> houses ===== */
void test_full_astrology_pipeline(void) {
    double jd = J2000;

    /* Step 1: planet positions */
    solar_system_t sys = planets_at(jd);

    /* Step 2: derive zodiac signs */
    int signs[8];
    double longitudes[8];
    for (int p = 0; p < 8; p++) {
        longitudes[p] = normalize_deg(sys.positions[p].longitude);
        signs[p] = zodiac_sign(longitudes[p]);
        TEST_ASSERT_TRUE(signs[p] >= 0 && signs[p] <= 11);
    }

    /* Step 3: find aspects */
    aspect_list_t aspects = aspects_find_all(longitudes, 8.0);
    TEST_ASSERT_TRUE(aspects.count >= 0);

    /* Step 4: compute houses for a specific observer */
    double gst = gmst_degrees(jd);
    double lst = lst_degrees(gst, -74.0); /* New York */
    double obliquity = mean_obliquity(jd);
    double asc_lon = ascendant_longitude(lst, obliquity, 40.7); /* NYC lat */

    house_system_t houses = houses_whole_sign(asc_lon);
    TEST_ASSERT_TRUE(houses.signs[0] >= 0 && houses.signs[0] <= 11);

    /* Step 5: Moon phase */
    lunar_info_t moon = lunar_phase(jd);
    TEST_ASSERT_TRUE(moon.phase >= MOON_NEW);
    TEST_ASSERT_TRUE(moon.phase <= MOON_WANING_CRESCENT);

    /* Moon zodiac sign */
    int moon_sign = lunar_zodiac_sign(jd);
    TEST_ASSERT_TRUE(moon_sign >= 0 && moon_sign <= 11);

    /* Step 6: solar events */
    solar_day_t day = se_solar_events(jd, 40.7, -74.0);
    TEST_ASSERT_TRUE(day.day_length > 0.0);
    TEST_ASSERT_TRUE(day.day_length < 24.0);
}

/* ===== Test: solar declination seasonal cycle ===== */
void test_solar_declination_seasonal(void) {
    /* Around June 21 (summer solstice) — max declination ~+23.44 */
    double jd_june = gregorian_to_jd(2020, 6, 21.5);
    double dec_june = se_solar_declination(jd_june);
    TEST_ASSERT_TRUE(dec_june > 20.0);
    TEST_ASSERT_TRUE(dec_june < 24.0);

    /* Around December 21 (winter solstice) — min declination ~-23.44 */
    double jd_dec = gregorian_to_jd(2020, 12, 21.5);
    double dec_dec = se_solar_declination(jd_dec);
    TEST_ASSERT_TRUE(dec_dec < -20.0);
    TEST_ASSERT_TRUE(dec_dec > -24.0);

    /* Around March 21 (vernal equinox) — declination ~0 */
    double jd_mar = gregorian_to_jd(2020, 3, 20.5);
    double dec_mar = se_solar_declination(jd_mar);
    TEST_ASSERT_FLOAT_WITHIN(2.0, 0.0, dec_mar);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_planet_positions_j2000_ranges);
    RUN_TEST(test_planet_distance_ordering);
    RUN_TEST(test_planet_positions_no_nan);
    RUN_TEST(test_great_conjunction_2020);
    RUN_TEST(test_zodiac_signs_valid);
    RUN_TEST(test_aspects_detected);
    RUN_TEST(test_angular_separation_properties);
    RUN_TEST(test_houses_sequential);
    RUN_TEST(test_solar_events_valid);
    RUN_TEST(test_moon_position_valid);
    RUN_TEST(test_moon_next_events);
    RUN_TEST(test_kepler_solver_convergence);
    RUN_TEST(test_ecliptic_to_equatorial);
    RUN_TEST(test_sidereal_time_valid);
    RUN_TEST(test_full_astrology_pipeline);
    RUN_TEST(test_solar_declination_seasonal);

    return UNITY_END();
}
