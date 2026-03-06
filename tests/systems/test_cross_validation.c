#include "../unity/unity.h"
#include "../../src/math/julian.h"
#include "../../src/math/sidereal.h"
#include "../../src/math/ecliptic.h"
#include "../../src/math/kepler.h"
#include "../../src/systems/astronomy/orbit.h"
#include "../../src/systems/astronomy/planets.h"
#include "../../src/systems/gregorian/gregorian.h"
#include "../../src/systems/astrology/zodiac.h"
#include "../../src/systems/astrology/aspects.h"
#include "../../src/systems/astrology/observer.h"
#include "../../src/systems/astrology/houses.h"
#include "../../src/systems/tzolkin/tzolkin.h"
#include "../../src/systems/chinese/chinese.h"
#include <math.h>

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

/* ===== Category 1: Orbital Math Consistency ===== */

void test_orbit_earth_distance_j2000(void) {
    orbital_elements_t e = orbit_elements_at(&ORBIT_EARTH, J2000);
    heliocentric_pos_t pos = orbit_heliocentric(&e);
    TEST_ASSERT_FLOAT_WITHIN(0.02, 1.0, pos.distance);
}

void test_orbit_mars_distance_j2000(void) {
    orbital_elements_t e = orbit_elements_at(&ORBIT_MARS, J2000);
    heliocentric_pos_t pos = orbit_heliocentric(&e);
    TEST_ASSERT_TRUE(pos.distance > 1.38 && pos.distance < 1.67);
}

void test_orbit_jupiter_distance_j2000(void) {
    orbital_elements_t e = orbit_elements_at(&ORBIT_JUPITER, J2000);
    heliocentric_pos_t pos = orbit_heliocentric(&e);
    TEST_ASSERT_TRUE(pos.distance > 4.9 && pos.distance < 5.5);
}

void test_orbit_polar_equation_consistency(void) {
    /* Verify orbit_heliocentric distance matches polar equation for all planets */
    const planet_orbit_t *orbits[] = {
        &ORBIT_MERCURY, &ORBIT_VENUS, &ORBIT_EARTH, &ORBIT_MARS,
        &ORBIT_JUPITER, &ORBIT_SATURN, &ORBIT_URANUS, &ORBIT_NEPTUNE
    };
    for (int i = 0; i < 8; i++) {
        orbital_elements_t elem = orbit_elements_at(orbits[i], J2000);
        heliocentric_pos_t pos = orbit_heliocentric(&elem);

        /* Recompute distance from polar equation */
        double M = normalize_deg(elem.L - elem.omega_p) * DEG_TO_RAD;
        double E = kepler_solve(M, elem.e);
        double v = eccentric_to_true_anomaly(E, elem.e);
        double r_polar = elem.a * (1.0 - elem.e * elem.e) / (1.0 + elem.e * cos(v));

        TEST_ASSERT_FLOAT_WITHIN(0.001, r_polar, pos.distance);
    }
}

void test_orbit_distance_positive(void) {
    /* All planet distances are positive at multiple dates */
    double dates[] = {J2000, J2000 + 365.25, J2000 + 730.5, J2000 - 365.25};
    for (int d = 0; d < 4; d++) {
        solar_system_t sys = planets_at(dates[d]);
        for (int i = 0; i < 8; i++) {
            TEST_ASSERT_TRUE(sys.positions[i].distance > 0.0);
        }
    }
}

/* ===== Category 2: Zodiac/Ecliptic Consistency ===== */

void test_zodiac_aries_boundary_start(void) {
    TEST_ASSERT_EQUAL_INT(0, zodiac_sign(0.0));
}

void test_zodiac_aries_boundary_end(void) {
    TEST_ASSERT_EQUAL_INT(0, zodiac_sign(29.999));
}

void test_zodiac_taurus_boundary(void) {
    TEST_ASSERT_EQUAL_INT(1, zodiac_sign(30.0));
}

void test_zodiac_all_12_boundaries(void) {
    for (int i = 0; i < 12; i++) {
        double lon = i * 30.0;
        TEST_ASSERT_EQUAL_INT(i, zodiac_sign(lon));
    }
}

void test_zodiac_formula_consistency(void) {
    /* For longitudes in [0, 360), zodiac_sign matches floor(lon/30) */
    double lons[] = {0.0, 15.5, 30.0, 59.9, 90.0, 120.0, 179.5, 210.3, 270.0, 330.0, 359.9};
    for (int i = 0; i < 11; i++) {
        int expected = (int)floor(lons[i] / 30.0) % 12;
        TEST_ASSERT_EQUAL_INT(expected, zodiac_sign(lons[i]));
    }
}

void test_zodiac_wrap_at_360(void) {
    /* 360.0 should wrap to Aries (0) */
    TEST_ASSERT_EQUAL_INT(0, zodiac_sign(360.0));
    TEST_ASSERT_EQUAL_INT(0, zodiac_sign(720.0));
}

/* ===== Category 3: Calendar Roundtrips ===== */

void test_jd_roundtrip_modern_dates(void) {
    int years[]  = {2000, 2000, 2024, 2050, 2100};
    int months[] = {1,    7,    2,    3,    12};
    int days[]   = {1,    4,    29,   21,   31};
    for (int i = 0; i < 5; i++) {
        double jd = gregorian_to_jd(years[i], months[i], (double)days[i]);
        int y, m;
        double d = jd_to_gregorian(jd, &y, &m);
        TEST_ASSERT_EQUAL_INT(years[i], y);
        TEST_ASSERT_EQUAL_INT(months[i], m);
        TEST_ASSERT_FLOAT_WITHIN(0.01, (double)days[i], d);
    }
}

void test_jd_roundtrip_historical_dates(void) {
    int years[]  = {1582, 1776, 1900, 1969, 1999};
    int months[] = {10,   7,    1,    7,    12};
    int days[]   = {15,   4,    1,    20,   31};
    for (int i = 0; i < 5; i++) {
        double jd = gregorian_to_jd(years[i], months[i], (double)days[i]);
        int y, m;
        double d = jd_to_gregorian(jd, &y, &m);
        TEST_ASSERT_EQUAL_INT(years[i], y);
        TEST_ASSERT_EQUAL_INT(months[i], m);
        TEST_ASSERT_FLOAT_WITHIN(0.01, (double)days[i], d);
    }
}

void test_chinese_jd_matches_gregorian(void) {
    /* For J2000 (2000-01-01.5), both paths should give identical results */
    chinese_year_t from_jd = chinese_year_from_jd(J2000);
    chinese_year_t from_greg = chinese_year_from_gregorian(2000);
    TEST_ASSERT_EQUAL_INT(from_greg.stem, from_jd.stem);
    TEST_ASSERT_EQUAL_INT(from_greg.branch, from_jd.branch);
    TEST_ASSERT_EQUAL_INT(from_greg.animal, from_jd.animal);
    TEST_ASSERT_EQUAL_INT(from_greg.element, from_jd.element);
    TEST_ASSERT_EQUAL_INT(from_greg.cycle_year, from_jd.cycle_year);
}

void test_tzolkin_daily_increment(void) {
    /* Kin number should increment by 1 each day (mod 260) */
    for (int d = 0; d < 30; d++) {
        tzolkin_day_t today = tzolkin_from_jd(J2000 + d);
        tzolkin_day_t tomorrow = tzolkin_from_jd(J2000 + d + 1);
        int expected_kin = (today.kin % 260) + 1;
        TEST_ASSERT_EQUAL_INT(expected_kin, tomorrow.kin);
    }
}

void test_chinese_cycle_consistency(void) {
    /* 60-year cycle repeats: year Y and Y+60 have same stem, branch, element */
    chinese_year_t y2000 = chinese_year_from_gregorian(2000);
    chinese_year_t y2060 = chinese_year_from_gregorian(2060);
    TEST_ASSERT_EQUAL_INT(y2000.stem, y2060.stem);
    TEST_ASSERT_EQUAL_INT(y2000.branch, y2060.branch);
    TEST_ASSERT_EQUAL_INT(y2000.animal, y2060.animal);
    TEST_ASSERT_EQUAL_INT(y2000.element, y2060.element);
    TEST_ASSERT_EQUAL_INT(y2000.polarity, y2060.polarity);
}

/* ===== Category 4: Astrology System Consistency ===== */

void test_houses_all_signs_present(void) {
    /* For any ascendant, all 12 signs appear exactly once */
    double ascs[] = {0.0, 45.0, 100.0, 200.0, 300.0, 350.0};
    for (int a = 0; a < 6; a++) {
        house_system_t h = houses_whole_sign(ascs[a]);
        int seen[12] = {0};
        for (int i = 0; i < 12; i++) {
            TEST_ASSERT_TRUE(h.signs[i] >= 0 && h.signs[i] <= 11);
            seen[h.signs[i]]++;
        }
        for (int i = 0; i < 12; i++) {
            TEST_ASSERT_EQUAL_INT(1, seen[i]);
        }
    }
}

void test_houses_permutation_check(void) {
    /* Signs form a consecutive sequence mod 12 */
    house_system_t h = houses_whole_sign(175.0);
    for (int i = 1; i < 12; i++) {
        TEST_ASSERT_EQUAL_INT((h.signs[i - 1] + 1) % 12, h.signs[i]);
    }
}

void test_aspects_symmetric(void) {
    /* angular_separation(a,b) == angular_separation(b,a), so aspect_check agrees */
    double pairs[][2] = {{10, 100}, {350, 10}, {0, 180}, {45, 225}, {120, 300}};
    for (int i = 0; i < 5; i++) {
        double sep_ab = angular_separation(pairs[i][0], pairs[i][1]);
        double sep_ba = angular_separation(pairs[i][1], pairs[i][0]);
        TEST_ASSERT_FLOAT_WITHIN(0.0001, sep_ab, sep_ba);
        TEST_ASSERT_EQUAL_INT(
            aspect_check(sep_ab, 8.0),
            aspect_check(sep_ba, 8.0)
        );
    }
}

void test_separation_range(void) {
    /* angular_separation always in [0, 180] */
    double vals[] = {0, 30, 60, 90, 120, 150, 180, 210, 240, 270, 300, 330, 359.9};
    for (int i = 0; i < 13; i++) {
        for (int j = 0; j < 13; j++) {
            double sep = angular_separation(vals[i], vals[j]);
            TEST_ASSERT_TRUE(sep >= 0.0);
            TEST_ASSERT_TRUE(sep <= 180.0);
        }
    }
}

void test_ascendant_range(void) {
    /* Ascendant always in [0, 360) for 36 evenly-spaced LST values */
    double obl = 23.44;
    double lats[] = {0.0, 30.0, 51.5, -33.9};
    for (int lat = 0; lat < 4; lat++) {
        for (int lst_i = 0; lst_i < 36; lst_i++) {
            double lst = lst_i * 10.0;
            double asc = ascendant_longitude(lst, obl, lats[lat]);
            TEST_ASSERT_TRUE(asc >= 0.0);
            TEST_ASSERT_TRUE(asc < 360.0);
        }
    }
}

/* ===== Category 5: Time System Consistency ===== */

void test_sidereal_increases_with_jd(void) {
    /* GMST should differ between consecutive JDs (not constant) */
    double prev = gmst_degrees(J2000);
    for (int i = 1; i <= 10; i++) {
        double curr = gmst_degrees(J2000 + i);
        TEST_ASSERT_TRUE(fabs(curr - prev) > 0.1);
        prev = curr;
    }
}

void test_obliquity_j2000(void) {
    double obl = mean_obliquity(J2000);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 23.44, obl);
}

void test_lst_equals_gst_at_greenwich(void) {
    /* LST at longitude 0 should equal GST */
    double gst = gmst_degrees(J2000);
    double lst = lst_degrees(gst, 0.0);
    TEST_ASSERT_FLOAT_WITHIN(0.0001, gst, lst);
}

/* ===== Category 6: Multi-System Same Date ===== */

void test_multiple_systems_same_date(void) {
    /* At J2000, all systems should produce coherent results */
    double jd = J2000;

    /* Planets: all valid */
    solar_system_t sys = planets_at(jd);
    for (int i = 0; i < 8; i++) {
        TEST_ASSERT_TRUE(sys.positions[i].longitude >= 0.0);
        TEST_ASSERT_TRUE(sys.positions[i].longitude < 360.0);
    }

    /* Zodiac: Earth's longitude maps to a valid sign */
    int sign = zodiac_sign(sys.positions[PLANET_EARTH].longitude);
    TEST_ASSERT_TRUE(sign >= 0 && sign <= 11);

    /* Gregorian: J2000 = 2000-01-01 12:00 */
    int y, m;
    double d = jd_to_gregorian(jd, &y, &m);
    TEST_ASSERT_EQUAL_INT(2000, y);
    TEST_ASSERT_EQUAL_INT(1, m);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 1.5, d);

    /* Chinese: year 2000 */
    chinese_year_t cy = chinese_year_from_jd(jd);
    TEST_ASSERT_TRUE(cy.stem >= 0 && cy.stem <= 9);
    TEST_ASSERT_TRUE(cy.branch >= 0 && cy.branch <= 11);

    /* Tzolkin: valid kin */
    tzolkin_day_t tz = tzolkin_from_jd(jd);
    TEST_ASSERT_TRUE(tz.kin >= 1 && tz.kin <= 260);

    /* Sidereal: valid range */
    double gst = gast_degrees(jd);
    TEST_ASSERT_TRUE(gst >= 0.0 && gst < 360.0);

    /* Obliquity: valid */
    double obl = mean_obliquity(jd);
    TEST_ASSERT_TRUE(obl > 23.0 && obl < 24.0);
}

int main(void) {
    UNITY_BEGIN();
    /* Category 1: Orbital Math Consistency */
    RUN_TEST(test_orbit_earth_distance_j2000);
    RUN_TEST(test_orbit_mars_distance_j2000);
    RUN_TEST(test_orbit_jupiter_distance_j2000);
    RUN_TEST(test_orbit_polar_equation_consistency);
    RUN_TEST(test_orbit_distance_positive);
    /* Category 2: Zodiac/Ecliptic Consistency */
    RUN_TEST(test_zodiac_aries_boundary_start);
    RUN_TEST(test_zodiac_aries_boundary_end);
    RUN_TEST(test_zodiac_taurus_boundary);
    RUN_TEST(test_zodiac_all_12_boundaries);
    RUN_TEST(test_zodiac_formula_consistency);
    RUN_TEST(test_zodiac_wrap_at_360);
    /* Category 3: Calendar Roundtrips */
    RUN_TEST(test_jd_roundtrip_modern_dates);
    RUN_TEST(test_jd_roundtrip_historical_dates);
    RUN_TEST(test_chinese_jd_matches_gregorian);
    RUN_TEST(test_tzolkin_daily_increment);
    RUN_TEST(test_chinese_cycle_consistency);
    /* Category 4: Astrology System Consistency */
    RUN_TEST(test_houses_all_signs_present);
    RUN_TEST(test_houses_permutation_check);
    RUN_TEST(test_aspects_symmetric);
    RUN_TEST(test_separation_range);
    RUN_TEST(test_ascendant_range);
    /* Category 5: Time System Consistency */
    RUN_TEST(test_sidereal_increases_with_jd);
    RUN_TEST(test_obliquity_j2000);
    RUN_TEST(test_lst_equals_gst_at_greenwich);
    /* Category 6: Multi-System Same Date */
    RUN_TEST(test_multiple_systems_same_date);
    return UNITY_END();
}
