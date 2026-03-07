/* test_wind_patterns.c — Tests for Coriolis & wind pattern data module.
 * TDD RED phase: all tests written before implementation.
 * Data source: General atmospheric circulation, Coriolis effect. */

#include "../../unity/unity.h"
#include "../../../src/systems/earth/wind_patterns.h"

#include <math.h>
#include <string.h>

void setUp(void) { }
void tearDown(void) { }

/* ===== Coriolis Parameter ===== */

void test_coriolis_parameter_equator_is_zero(void) {
    double f = wind_coriolis_parameter(0.0);
    TEST_ASSERT_FLOAT_WITHIN(1e-10f, 0.0f, (float)f);
}

void test_coriolis_parameter_30n(void) {
    /* f = 2 * 7.2921e-5 * sin(30deg) = 2 * 7.2921e-5 * 0.5 = 7.2921e-5 */
    double f = wind_coriolis_parameter(30.0);
    TEST_ASSERT_FLOAT_WITHIN(1e-7f, 7.2921e-5f, (float)f);
}

void test_coriolis_parameter_90n(void) {
    /* f = 2 * 7.2921e-5 * sin(90deg) = 2 * 7.2921e-5 * 1 = 1.45842e-4 */
    double f = wind_coriolis_parameter(90.0);
    TEST_ASSERT_FLOAT_WITHIN(1e-7f, 1.45842e-4f, (float)f);
}

void test_coriolis_parameter_sh_negative(void) {
    /* Southern hemisphere: negative */
    double f = wind_coriolis_parameter(-30.0);
    TEST_ASSERT_TRUE(f < 0.0);
    TEST_ASSERT_FLOAT_WITHIN(1e-7f, -7.2921e-5f, (float)f);
}

/* ===== Prevailing Wind ===== */

void test_prevailing_equator_trade_winds(void) {
    wind_pattern_t w = wind_prevailing(10.0);
    TEST_ASSERT_EQUAL_STRING("Trade Winds", w.name);
    TEST_ASSERT_EQUAL_INT(WIND_CELL_HADLEY, w.cell_type);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 45.0f, (float)w.direction_deg);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.7f, (float)w.strength_relative);
}

void test_prevailing_nh_midlat_westerlies(void) {
    wind_pattern_t w = wind_prevailing(45.0);
    TEST_ASSERT_EQUAL_STRING("Westerlies", w.name);
    TEST_ASSERT_EQUAL_INT(WIND_CELL_FERREL, w.cell_type);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 225.0f, (float)w.direction_deg);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.6f, (float)w.strength_relative);
}

void test_prevailing_sh_trade_winds(void) {
    /* Southern hemisphere trades: direction=135 (SE) */
    wind_pattern_t w = wind_prevailing(-15.0);
    TEST_ASSERT_EQUAL_STRING("Trade Winds", w.name);
    TEST_ASSERT_EQUAL_INT(WIND_CELL_HADLEY, w.cell_type);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 135.0f, (float)w.direction_deg);
}

void test_prevailing_nh_polar_easterlies(void) {
    wind_pattern_t w = wind_prevailing(75.0);
    TEST_ASSERT_EQUAL_STRING("Polar Easterlies", w.name);
    TEST_ASSERT_EQUAL_INT(WIND_CELL_POLAR, w.cell_type);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 45.0f, (float)w.direction_deg);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.4f, (float)w.strength_relative);
}

void test_prevailing_sh_westerlies(void) {
    /* SH westerlies: direction=315 (NW) */
    wind_pattern_t w = wind_prevailing(-45.0);
    TEST_ASSERT_EQUAL_STRING("Westerlies", w.name);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 315.0f, (float)w.direction_deg);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.6f, (float)w.strength_relative);
}

void test_prevailing_sh_polar_easterlies(void) {
    wind_pattern_t w = wind_prevailing(-80.0);
    TEST_ASSERT_EQUAL_STRING("Polar Easterlies", w.name);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 135.0f, (float)w.direction_deg);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.4f, (float)w.strength_relative);
}

/* ===== Cell Get ===== */

void test_cell_get_hadley(void) {
    circulation_cell_t c = wind_cell_get(WIND_CELL_HADLEY);
    TEST_ASSERT_EQUAL_INT(0, c.id);
    TEST_ASSERT_EQUAL_STRING("Hadley", c.name);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, (float)c.lat_min);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 30.0f, (float)c.lat_max);
    TEST_ASSERT_EQUAL_STRING("Trade Winds", c.surface_wind);
    TEST_ASSERT_NOT_NULL(c.description);
}

void test_cell_get_ferrel(void) {
    circulation_cell_t c = wind_cell_get(WIND_CELL_FERREL);
    TEST_ASSERT_EQUAL_INT(1, c.id);
    TEST_ASSERT_EQUAL_STRING("Ferrel", c.name);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 30.0f, (float)c.lat_min);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 60.0f, (float)c.lat_max);
    TEST_ASSERT_EQUAL_STRING("Westerlies", c.surface_wind);
}

void test_cell_get_polar(void) {
    circulation_cell_t c = wind_cell_get(WIND_CELL_POLAR);
    TEST_ASSERT_EQUAL_INT(2, c.id);
    TEST_ASSERT_EQUAL_STRING("Polar", c.name);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 60.0f, (float)c.lat_min);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 90.0f, (float)c.lat_max);
    TEST_ASSERT_EQUAL_STRING("Polar Easterlies", c.surface_wind);
}

void test_cell_get_invalid(void) {
    circulation_cell_t c = wind_cell_get(5);
    TEST_ASSERT_EQUAL_INT(-1, c.id);
    TEST_ASSERT_NULL(c.name);
}

/* ===== Cell Count ===== */

void test_cell_count_is_3(void) {
    TEST_ASSERT_EQUAL_INT(WIND_CELL_COUNT, wind_cell_count());
    TEST_ASSERT_EQUAL_INT(3, wind_cell_count());
}

/* ===== Cell at Latitude ===== */

void test_cell_at_latitude_equator(void) {
    TEST_ASSERT_EQUAL_INT(WIND_CELL_HADLEY, wind_cell_at_latitude(5.0));
}

void test_cell_at_latitude_45n(void) {
    TEST_ASSERT_EQUAL_INT(WIND_CELL_FERREL, wind_cell_at_latitude(45.0));
}

void test_cell_at_latitude_75n(void) {
    TEST_ASSERT_EQUAL_INT(WIND_CELL_POLAR, wind_cell_at_latitude(75.0));
}

void test_cell_at_latitude_sh(void) {
    /* Southern hemisphere should use absolute latitude */
    TEST_ASSERT_EQUAL_INT(WIND_CELL_HADLEY, wind_cell_at_latitude(-20.0));
    TEST_ASSERT_EQUAL_INT(WIND_CELL_FERREL, wind_cell_at_latitude(-50.0));
    TEST_ASSERT_EQUAL_INT(WIND_CELL_POLAR, wind_cell_at_latitude(-70.0));
}

/* ===== ITCZ Latitude ===== */

void test_itcz_january(void) {
    double lat = wind_itcz_latitude(1);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 5.0f, (float)lat);
}

void test_itcz_july(void) {
    double lat = wind_itcz_latitude(7);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 15.0f, (float)lat);
}

void test_itcz_all_months_positive(void) {
    /* ITCZ is always north of equator (positive latitude) */
    for (int m = 1; m <= 12; m++) {
        double lat = wind_itcz_latitude(m);
        TEST_ASSERT_TRUE(lat > 0.0);
    }
}

void test_itcz_invalid_month_clamps(void) {
    /* Out-of-range months should be clamped or handled gracefully */
    double lat0 = wind_itcz_latitude(0);
    double lat13 = wind_itcz_latitude(13);
    /* Should return valid positive values regardless */
    TEST_ASSERT_TRUE(lat0 > 0.0);
    TEST_ASSERT_TRUE(lat13 > 0.0);
}

/* ===== Coriolis Deflection ===== */

void test_coriolis_deflection_equator_zero(void) {
    double d = wind_coriolis_deflection(0.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, (float)d);
}

void test_coriolis_deflection_45n(void) {
    /* deflection = 45 * sin(45deg) = 45 * 0.7071 = ~31.82 */
    double d = wind_coriolis_deflection(45.0);
    double expected = 45.0 * sin(45.0 * PI / 180.0);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, (float)expected, (float)d);
}

void test_coriolis_deflection_90n(void) {
    /* deflection = 45 * sin(90deg) = 45.0 */
    double d = wind_coriolis_deflection(90.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 45.0f, (float)d);
}

void test_coriolis_deflection_sh_negative(void) {
    /* Southern hemisphere deflection is negative (leftward) */
    double d = wind_coriolis_deflection(-45.0);
    TEST_ASSERT_TRUE(d < 0.0);
    double expected = 45.0 * sin(-45.0 * PI / 180.0);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, (float)expected, (float)d);
}

/* ===== Doldrums ===== */

void test_doldrums_at_itcz_january(void) {
    /* Jan ITCZ is at 5N. Lat=5 should be in doldrums. */
    TEST_ASSERT_EQUAL_INT(1, wind_is_doldrums(5.0, 1));
}

void test_doldrums_far_from_itcz(void) {
    /* 30N is far from January ITCZ (5N) */
    TEST_ASSERT_EQUAL_INT(0, wind_is_doldrums(30.0, 1));
}

void test_doldrums_seasonal_shift(void) {
    /* July ITCZ is at 15N. Lat=14 should be in doldrums. */
    TEST_ASSERT_EQUAL_INT(1, wind_is_doldrums(14.0, 7));
    /* But lat=5 is 10 deg away in July, NOT doldrums */
    TEST_ASSERT_EQUAL_INT(0, wind_is_doldrums(5.0, 7));
}

/* ===== Horse Latitudes ===== */

void test_horse_latitudes_at_30n(void) {
    TEST_ASSERT_EQUAL_INT(1, wind_is_horse_latitudes(30.0));
}

void test_horse_latitudes_at_equator_no(void) {
    TEST_ASSERT_EQUAL_INT(0, wind_is_horse_latitudes(0.0));
}

void test_horse_latitudes_at_30s(void) {
    /* Should also detect at 30S */
    TEST_ASSERT_EQUAL_INT(1, wind_is_horse_latitudes(-30.0));
}

/* ===== Additional Coverage ===== */

void test_prevailing_boundary_30n(void) {
    /* Right at the boundary: lat=30 should be in Ferrel (30-60) */
    wind_pattern_t w = wind_prevailing(30.0);
    TEST_ASSERT_EQUAL_STRING("Westerlies", w.name);
}

void test_prevailing_boundary_60n(void) {
    /* Right at lat=60: should be in Polar (60-90) */
    wind_pattern_t w = wind_prevailing(60.0);
    TEST_ASSERT_EQUAL_STRING("Polar Easterlies", w.name);
}

void test_cell_at_latitude_boundary_30(void) {
    /* lat=30 -> Ferrel cell */
    TEST_ASSERT_EQUAL_INT(WIND_CELL_FERREL, wind_cell_at_latitude(30.0));
}

void test_cell_at_latitude_boundary_60(void) {
    /* lat=60 -> Polar cell */
    TEST_ASSERT_EQUAL_INT(WIND_CELL_POLAR, wind_cell_at_latitude(60.0));
}

void test_horse_latitudes_boundary_25(void) {
    /* 25 is exactly at the boundary (|lat|-30 = -5, within 5) */
    TEST_ASSERT_EQUAL_INT(1, wind_is_horse_latitudes(25.0));
}

void test_horse_latitudes_boundary_35(void) {
    /* 35 is exactly at the boundary (|lat|-30 = 5, within 5) */
    TEST_ASSERT_EQUAL_INT(1, wind_is_horse_latitudes(35.0));
}

void test_horse_latitudes_just_outside(void) {
    /* 36 is outside: |36-30| = 6, > 5 */
    TEST_ASSERT_EQUAL_INT(0, wind_is_horse_latitudes(36.0));
}

void test_cell_get_negative_index(void) {
    circulation_cell_t c = wind_cell_get(-1);
    TEST_ASSERT_EQUAL_INT(-1, c.id);
    TEST_ASSERT_NULL(c.name);
}

void test_itcz_summer_peak(void) {
    /* August ITCZ should be 14 */
    double lat = wind_itcz_latitude(8);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 14.0f, (float)lat);
}

void test_coriolis_parameter_45n(void) {
    /* f = 2 * 7.2921e-5 * sin(45deg) */
    double expected = 2.0 * EARTH_OMEGA * sin(45.0 * PI / 180.0);
    double f = wind_coriolis_parameter(45.0);
    TEST_ASSERT_FLOAT_WITHIN(1e-7f, (float)expected, (float)f);
}

/* --- Runner --- */

int main(void) {
    UNITY_BEGIN();

    /* Coriolis parameter */
    RUN_TEST(test_coriolis_parameter_equator_is_zero);
    RUN_TEST(test_coriolis_parameter_30n);
    RUN_TEST(test_coriolis_parameter_90n);
    RUN_TEST(test_coriolis_parameter_sh_negative);
    RUN_TEST(test_coriolis_parameter_45n);

    /* Prevailing wind */
    RUN_TEST(test_prevailing_equator_trade_winds);
    RUN_TEST(test_prevailing_nh_midlat_westerlies);
    RUN_TEST(test_prevailing_sh_trade_winds);
    RUN_TEST(test_prevailing_nh_polar_easterlies);
    RUN_TEST(test_prevailing_sh_westerlies);
    RUN_TEST(test_prevailing_sh_polar_easterlies);
    RUN_TEST(test_prevailing_boundary_30n);
    RUN_TEST(test_prevailing_boundary_60n);

    /* Cell get */
    RUN_TEST(test_cell_get_hadley);
    RUN_TEST(test_cell_get_ferrel);
    RUN_TEST(test_cell_get_polar);
    RUN_TEST(test_cell_get_invalid);
    RUN_TEST(test_cell_get_negative_index);

    /* Cell count */
    RUN_TEST(test_cell_count_is_3);

    /* Cell at latitude */
    RUN_TEST(test_cell_at_latitude_equator);
    RUN_TEST(test_cell_at_latitude_45n);
    RUN_TEST(test_cell_at_latitude_75n);
    RUN_TEST(test_cell_at_latitude_sh);
    RUN_TEST(test_cell_at_latitude_boundary_30);
    RUN_TEST(test_cell_at_latitude_boundary_60);

    /* ITCZ */
    RUN_TEST(test_itcz_january);
    RUN_TEST(test_itcz_july);
    RUN_TEST(test_itcz_all_months_positive);
    RUN_TEST(test_itcz_invalid_month_clamps);
    RUN_TEST(test_itcz_summer_peak);

    /* Coriolis deflection */
    RUN_TEST(test_coriolis_deflection_equator_zero);
    RUN_TEST(test_coriolis_deflection_45n);
    RUN_TEST(test_coriolis_deflection_90n);
    RUN_TEST(test_coriolis_deflection_sh_negative);

    /* Doldrums */
    RUN_TEST(test_doldrums_at_itcz_january);
    RUN_TEST(test_doldrums_far_from_itcz);
    RUN_TEST(test_doldrums_seasonal_shift);

    /* Horse latitudes */
    RUN_TEST(test_horse_latitudes_at_30n);
    RUN_TEST(test_horse_latitudes_at_equator_no);
    RUN_TEST(test_horse_latitudes_at_30s);
    RUN_TEST(test_horse_latitudes_boundary_25);
    RUN_TEST(test_horse_latitudes_boundary_35);
    RUN_TEST(test_horse_latitudes_just_outside);

    return UNITY_END();
}
