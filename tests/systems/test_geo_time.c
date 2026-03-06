/* test_geo_time.c — Tests for International Chronostratigraphic Chart data.
 * TDD RED phase: all tests written before implementation. */

#include "../unity/unity.h"
#include "../../src/systems/geology/geo_time.h"

#include <math.h>
#include <string.h>

#define MA_EPSILON 0.01f
#define J2000_JD   2451545.0

void setUp(void) { }
void tearDown(void) { }

/* --- Eon tests --- */

void test_geo_eon_count_returns_4(void) {
    TEST_ASSERT_EQUAL_INT(4, geo_eon_count());
}

void test_geo_eon_at_present_returns_phanerozoic(void) {
    geo_unit_t eon = geo_eon_at(0.0);
    TEST_ASSERT_EQUAL_STRING("Phanerozoic", eon.name);
}

void test_geo_eon_at_3000_returns_archean(void) {
    geo_unit_t eon = geo_eon_at(3000.0);
    TEST_ASSERT_EQUAL_STRING("Archean", eon.name);
}

void test_geo_eon_at_4500_returns_hadean(void) {
    geo_unit_t eon = geo_eon_at(4500.0);
    TEST_ASSERT_EQUAL_STRING("Hadean", eon.name);
}

void test_geo_eon_at_boundary_538_8_returns_phanerozoic(void) {
    /* Boundary: ma == start_ma belongs to that unit */
    geo_unit_t eon = geo_eon_at(538.8);
    TEST_ASSERT_EQUAL_STRING("Phanerozoic", eon.name);
}

void test_geo_eon_by_index_0_returns_hadean(void) {
    geo_unit_t eon = geo_eon_by_index(0);
    TEST_ASSERT_EQUAL_STRING("Hadean", eon.name);
    TEST_ASSERT_FLOAT_WITHIN(MA_EPSILON, 4600.0f, (float)eon.start_ma);
    TEST_ASSERT_FLOAT_WITHIN(MA_EPSILON, 4000.0f, (float)eon.end_ma);
}

void test_geo_eon_by_index_3_returns_phanerozoic(void) {
    geo_unit_t eon = geo_eon_by_index(3);
    TEST_ASSERT_EQUAL_STRING("Phanerozoic", eon.name);
    TEST_ASSERT_EQUAL_INT(154, eon.color.r);
    TEST_ASSERT_EQUAL_INT(217, eon.color.g);
    TEST_ASSERT_EQUAL_INT(221, eon.color.b);
}

void test_geo_eon_by_index_invalid_returns_null_name(void) {
    geo_unit_t eon = geo_eon_by_index(-1);
    TEST_ASSERT_NULL(eon.name);
    eon = geo_eon_by_index(4);
    TEST_ASSERT_NULL(eon.name);
}

/* --- Era tests --- */

void test_geo_era_count_returns_10(void) {
    TEST_ASSERT_EQUAL_INT(10, geo_era_count());
}

void test_geo_era_at_100_returns_mesozoic(void) {
    geo_unit_t era = geo_era_at(100.0);
    TEST_ASSERT_EQUAL_STRING("Mesozoic", era.name);
}

void test_geo_era_at_50_returns_cenozoic(void) {
    geo_unit_t era = geo_era_at(50.0);
    TEST_ASSERT_EQUAL_STRING("Cenozoic", era.name);
}

void test_geo_era_at_300_returns_paleozoic(void) {
    geo_unit_t era = geo_era_at(300.0);
    TEST_ASSERT_EQUAL_STRING("Paleozoic", era.name);
}

void test_geo_era_by_index_9_returns_cenozoic(void) {
    geo_unit_t era = geo_era_by_index(9);
    TEST_ASSERT_EQUAL_STRING("Cenozoic", era.name);
    TEST_ASSERT_FLOAT_WITHIN(MA_EPSILON, 66.0f, (float)era.start_ma);
    TEST_ASSERT_FLOAT_WITHIN(MA_EPSILON, 0.0f, (float)era.end_ma);
}

/* --- Period tests --- */

void test_geo_period_count_returns_22(void) {
    TEST_ASSERT_EQUAL_INT(22, geo_period_count());
}

void test_geo_period_at_0_5_returns_quaternary(void) {
    geo_unit_t period = geo_period_at(0.5);
    TEST_ASSERT_EQUAL_STRING("Quaternary", period.name);
}

void test_geo_period_at_70_returns_cretaceous(void) {
    geo_unit_t period = geo_period_at(70.0);
    TEST_ASSERT_EQUAL_STRING("Cretaceous", period.name);
}

void test_geo_period_at_500_returns_cambrian(void) {
    geo_unit_t period = geo_period_at(500.0);
    TEST_ASSERT_EQUAL_STRING("Cambrian", period.name);
}

void test_geo_period_at_boundary_538_8_returns_cambrian(void) {
    geo_unit_t period = geo_period_at(538.8);
    TEST_ASSERT_EQUAL_STRING("Cambrian", period.name);
}

void test_geo_period_by_index_0_returns_siderian(void) {
    geo_unit_t period = geo_period_by_index(0);
    TEST_ASSERT_EQUAL_STRING("Siderian", period.name);
    TEST_ASSERT_FLOAT_WITHIN(MA_EPSILON, 2500.0f, (float)period.start_ma);
    TEST_ASSERT_FLOAT_WITHIN(MA_EPSILON, 2300.0f, (float)period.end_ma);
}

void test_geo_period_by_index_21_returns_quaternary(void) {
    geo_unit_t period = geo_period_by_index(21);
    TEST_ASSERT_EQUAL_STRING("Quaternary", period.name);
}

void test_geo_period_by_index_invalid_returns_null(void) {
    geo_unit_t period = geo_period_by_index(-1);
    TEST_ASSERT_NULL(period.name);
    period = geo_period_by_index(22);
    TEST_ASSERT_NULL(period.name);
}

/* --- Epoch tests --- */

void test_geo_epoch_count_returns_34(void) {
    TEST_ASSERT_EQUAL_INT(34, geo_epoch_count());
}

void test_geo_epoch_at_near_zero_returns_anthropocene(void) {
    /* 0.00001 Ma = present-ish, falls in Anthropocene (0.000045 - 0) */
    geo_unit_t epoch = geo_epoch_at(0.00001);
    TEST_ASSERT_EQUAL_STRING("Anthropocene", epoch.name);
}

void test_geo_epoch_at_40_returns_eocene(void) {
    geo_unit_t epoch = geo_epoch_at(40.0);
    TEST_ASSERT_EQUAL_STRING("Eocene", epoch.name);
}

void test_geo_epoch_at_150_returns_upper_jurassic(void) {
    geo_unit_t epoch = geo_epoch_at(150.0);
    TEST_ASSERT_EQUAL_STRING("Upper Jurassic", epoch.name);
}

void test_geo_epoch_at_510_returns_lower_cambrian(void) {
    geo_unit_t epoch = geo_epoch_at(510.0);
    TEST_ASSERT_EQUAL_STRING("Lower Cambrian", epoch.name);
}

void test_geo_epoch_by_index_33_returns_anthropocene(void) {
    geo_unit_t epoch = geo_epoch_by_index(33);
    TEST_ASSERT_EQUAL_STRING("Anthropocene", epoch.name);
}

void test_geo_epoch_by_index_invalid_returns_null(void) {
    geo_unit_t epoch = geo_epoch_by_index(-1);
    TEST_ASSERT_NULL(epoch.name);
    epoch = geo_epoch_by_index(34);
    TEST_ASSERT_NULL(epoch.name);
}

/* --- JD to Ma conversion tests --- */

void test_geo_jd_to_ma_j2000_returns_zero(void) {
    float ma = (float)geo_jd_to_ma(J2000_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, ma);
}

void test_geo_jd_to_ma_1_million_years_ago(void) {
    /* 1 Ma = 1,000,000 years = 365,250,000 days before J2000 */
    double jd = J2000_JD - 365250000.0;
    float ma = (float)geo_jd_to_ma(jd);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, ma);
}

void test_geo_jd_to_ma_future_is_negative(void) {
    /* JD after J2000 -> negative Ma (future) */
    double jd = J2000_JD + 365250.0; /* ~1000 years in future */
    double ma = geo_jd_to_ma(jd);
    TEST_ASSERT_TRUE(ma < 0.0);
}

/* --- Extinction tests --- */

void test_geo_extinction_count_returns_5(void) {
    TEST_ASSERT_EQUAL_INT(5, geo_extinction_count());
}

void test_geo_extinction_by_index_0_ordovician(void) {
    geo_extinction_t ext = geo_extinction_by_index(0);
    TEST_ASSERT_EQUAL_STRING("Ordovician-Silurian", ext.name);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 443.8f, (float)ext.ma);
    TEST_ASSERT_NOT_NULL(ext.cause);
}

void test_geo_extinction_by_index_4_cretaceous(void) {
    geo_extinction_t ext = geo_extinction_by_index(4);
    TEST_ASSERT_EQUAL_STRING("Cretaceous-Paleogene", ext.name);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 66.0f, (float)ext.ma);
}

void test_geo_extinction_by_index_invalid_returns_null(void) {
    geo_extinction_t ext = geo_extinction_by_index(-1);
    TEST_ASSERT_NULL(ext.name);
    ext = geo_extinction_by_index(5);
    TEST_ASSERT_NULL(ext.name);
}

void test_geo_extinction_nearest_65_returns_kpg(void) {
    geo_extinction_t ext = geo_extinction_nearest(65.0);
    TEST_ASSERT_EQUAL_STRING("Cretaceous-Paleogene", ext.name);
}

void test_geo_extinction_nearest_250_returns_permian(void) {
    geo_extinction_t ext = geo_extinction_nearest(250.0);
    TEST_ASSERT_EQUAL_STRING("Permian-Triassic", ext.name);
}

void test_geo_extinction_nearest_440_returns_ordovician(void) {
    geo_extinction_t ext = geo_extinction_nearest(440.0);
    TEST_ASSERT_EQUAL_STRING("Ordovician-Silurian", ext.name);
}

void test_geo_extinction_nearest_200_returns_triassic(void) {
    geo_extinction_t ext = geo_extinction_nearest(200.0);
    TEST_ASSERT_EQUAL_STRING("Triassic-Jurassic", ext.name);
}

/* --- Purity tests --- */

void test_geo_eon_at_is_pure(void) {
    geo_unit_t a = geo_eon_at(100.0);
    geo_unit_t b = geo_eon_at(100.0);
    TEST_ASSERT_EQUAL_STRING(a.name, b.name);
    TEST_ASSERT_FLOAT_WITHIN(MA_EPSILON, (float)a.start_ma, (float)b.start_ma);
    TEST_ASSERT_FLOAT_WITHIN(MA_EPSILON, (float)a.end_ma, (float)b.end_ma);
    TEST_ASSERT_EQUAL_INT(a.color.r, b.color.r);
    TEST_ASSERT_EQUAL_INT(a.color.g, b.color.g);
    TEST_ASSERT_EQUAL_INT(a.color.b, b.color.b);
}

void test_geo_period_at_is_pure(void) {
    geo_unit_t a = geo_period_at(300.0);
    geo_unit_t b = geo_period_at(300.0);
    TEST_ASSERT_EQUAL_STRING(a.name, b.name);
    TEST_ASSERT_FLOAT_WITHIN(MA_EPSILON, (float)a.start_ma, (float)b.start_ma);
}

void test_geo_jd_to_ma_is_pure(void) {
    float a = (float)geo_jd_to_ma(2451545.0);
    float b = (float)geo_jd_to_ma(2451545.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, a, b);
}

/* --- Color validation tests --- */

void test_geo_eon_hadean_color(void) {
    geo_unit_t eon = geo_eon_by_index(0);
    TEST_ASSERT_EQUAL_INT(204, eon.color.r);
    TEST_ASSERT_EQUAL_INT(0, eon.color.g);
    TEST_ASSERT_EQUAL_INT(45, eon.color.b);
}

void test_geo_period_cambrian_color(void) {
    geo_unit_t period = geo_period_at(500.0);
    TEST_ASSERT_EQUAL_INT(127, period.color.r);
    TEST_ASSERT_EQUAL_INT(160, period.color.g);
    TEST_ASSERT_EQUAL_INT(86, period.color.b);
}

/* --- Boundary and edge cases --- */

void test_geo_eon_at_exact_4000_returns_archean(void) {
    /* 4000 Ma is the start of Archean (and end of Hadean).
     * start_ma >= ma means 4000 >= 4000, so Archean matches. */
    geo_unit_t eon = geo_eon_at(4000.0);
    TEST_ASSERT_EQUAL_STRING("Archean", eon.name);
}

void test_geo_era_at_2500_returns_paleoproterozoic(void) {
    geo_unit_t era = geo_era_at(2500.0);
    TEST_ASSERT_EQUAL_STRING("Paleoproterozoic", era.name);
}

int main(void) {
    UNITY_BEGIN();

    /* Eon tests (1-8) */
    RUN_TEST(test_geo_eon_count_returns_4);
    RUN_TEST(test_geo_eon_at_present_returns_phanerozoic);
    RUN_TEST(test_geo_eon_at_3000_returns_archean);
    RUN_TEST(test_geo_eon_at_4500_returns_hadean);
    RUN_TEST(test_geo_eon_at_boundary_538_8_returns_phanerozoic);
    RUN_TEST(test_geo_eon_by_index_0_returns_hadean);
    RUN_TEST(test_geo_eon_by_index_3_returns_phanerozoic);
    RUN_TEST(test_geo_eon_by_index_invalid_returns_null_name);

    /* Era tests (9-13) */
    RUN_TEST(test_geo_era_count_returns_10);
    RUN_TEST(test_geo_era_at_100_returns_mesozoic);
    RUN_TEST(test_geo_era_at_50_returns_cenozoic);
    RUN_TEST(test_geo_era_at_300_returns_paleozoic);
    RUN_TEST(test_geo_era_by_index_9_returns_cenozoic);

    /* Period tests (14-21) */
    RUN_TEST(test_geo_period_count_returns_22);
    RUN_TEST(test_geo_period_at_0_5_returns_quaternary);
    RUN_TEST(test_geo_period_at_70_returns_cretaceous);
    RUN_TEST(test_geo_period_at_500_returns_cambrian);
    RUN_TEST(test_geo_period_at_boundary_538_8_returns_cambrian);
    RUN_TEST(test_geo_period_by_index_0_returns_siderian);
    RUN_TEST(test_geo_period_by_index_21_returns_quaternary);
    RUN_TEST(test_geo_period_by_index_invalid_returns_null);

    /* Epoch tests (22-28) */
    RUN_TEST(test_geo_epoch_count_returns_34);
    RUN_TEST(test_geo_epoch_at_near_zero_returns_anthropocene);
    RUN_TEST(test_geo_epoch_at_40_returns_eocene);
    RUN_TEST(test_geo_epoch_at_150_returns_upper_jurassic);
    RUN_TEST(test_geo_epoch_at_510_returns_lower_cambrian);
    RUN_TEST(test_geo_epoch_by_index_33_returns_anthropocene);
    RUN_TEST(test_geo_epoch_by_index_invalid_returns_null);

    /* JD conversion tests (29-31) */
    RUN_TEST(test_geo_jd_to_ma_j2000_returns_zero);
    RUN_TEST(test_geo_jd_to_ma_1_million_years_ago);
    RUN_TEST(test_geo_jd_to_ma_future_is_negative);

    /* Extinction tests (32-38) */
    RUN_TEST(test_geo_extinction_count_returns_5);
    RUN_TEST(test_geo_extinction_by_index_0_ordovician);
    RUN_TEST(test_geo_extinction_by_index_4_cretaceous);
    RUN_TEST(test_geo_extinction_by_index_invalid_returns_null);
    RUN_TEST(test_geo_extinction_nearest_65_returns_kpg);
    RUN_TEST(test_geo_extinction_nearest_250_returns_permian);
    RUN_TEST(test_geo_extinction_nearest_440_returns_ordovician);
    RUN_TEST(test_geo_extinction_nearest_200_returns_triassic);

    /* Purity tests (39-41) */
    RUN_TEST(test_geo_eon_at_is_pure);
    RUN_TEST(test_geo_period_at_is_pure);
    RUN_TEST(test_geo_jd_to_ma_is_pure);

    /* Color tests (42-43) */
    RUN_TEST(test_geo_eon_hadean_color);
    RUN_TEST(test_geo_period_cambrian_color);

    /* Boundary tests (44-45) */
    RUN_TEST(test_geo_eon_at_exact_4000_returns_archean);
    RUN_TEST(test_geo_era_at_2500_returns_paleoproterozoic);

    return UNITY_END();
}
