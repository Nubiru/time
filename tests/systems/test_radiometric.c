/* test_radiometric.c — Tests for radiometric dating constants and decay math.
 * TDD RED phase: all tests written before implementation. */

#include "../unity/unity.h"
#include "../../src/systems/geology/radiometric.h"

#include <math.h>
#include <string.h>

/* Tolerances */
#define HL_EPS    1e-4f   /* relative tolerance for half-life checks */
#define FRAC_EPS  1e-5f   /* fraction tolerance */
#define AGE_EPS   0.001f  /* 0.1% relative for age round-trips */

void setUp(void) { }
void tearDown(void) { }

/* ---- System data tests ---- */

void test_system_count_is_8(void) {
    TEST_ASSERT_EQUAL_INT(8, RADIOMETRIC_SYSTEM_COUNT);
    TEST_ASSERT_EQUAL_INT(8, DECAY_SYSTEM_COUNT);
}

void test_u238_half_life(void) {
    TEST_ASSERT_FLOAT_WITHIN(1e5f, 4.468e9f, (float)radiometric_half_life(DECAY_U238_PB206));
}

void test_u235_half_life(void) {
    TEST_ASSERT_FLOAT_WITHIN(1e4f, 7.04e8f, (float)radiometric_half_life(DECAY_U235_PB207));
}

void test_c14_half_life(void) {
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 5730.0f, (float)radiometric_half_life(DECAY_C14_N14));
}

void test_k40_half_life(void) {
    TEST_ASSERT_FLOAT_WITHIN(1e5f, 1.248e9f, (float)radiometric_half_life(DECAY_K40_AR40));
}

void test_rb87_half_life(void) {
    TEST_ASSERT_FLOAT_WITHIN(1e6f, 4.88e10f, (float)radiometric_half_life(DECAY_RB87_SR87));
}

void test_invalid_system_half_life_returns_zero(void) {
    TEST_ASSERT_FLOAT_WITHIN(1e-10f, 0.0f, (float)radiometric_half_life(DECAY_SYSTEM_COUNT));
}

void test_parent_name_u238(void) {
    TEST_ASSERT_EQUAL_STRING("U-238", radiometric_parent(DECAY_U238_PB206));
}

void test_daughter_name_u238(void) {
    TEST_ASSERT_EQUAL_STRING("Pb-206", radiometric_daughter(DECAY_U238_PB206));
}

void test_parent_name_c14(void) {
    TEST_ASSERT_EQUAL_STRING("C-14", radiometric_parent(DECAY_C14_N14));
}

void test_daughter_name_c14(void) {
    TEST_ASSERT_EQUAL_STRING("N-14", radiometric_daughter(DECAY_C14_N14));
}

void test_system_data_decay_constant(void) {
    /* lambda = ln(2) / half_life */
    decay_data_t data = radiometric_system(DECAY_C14_N14);
    float expected = (float)(log(2.0) / 5730.0);
    TEST_ASSERT_FLOAT_WITHIN(1e-10f, expected, (float)data.decay_constant);
}

void test_th232_system_data(void) {
    decay_data_t data = radiometric_system(DECAY_TH232_PB208);
    TEST_ASSERT_EQUAL_STRING("Th-232", data.parent);
    TEST_ASSERT_EQUAL_STRING("Pb-208", data.daughter);
    TEST_ASSERT_FLOAT_WITHIN(1e6f, 1.405e10f, (float)data.half_life_years);
}

/* ---- Decay fraction tests ---- */

void test_decay_fraction_at_zero_time(void) {
    /* At t=0, parent fraction = 1.0 */
    TEST_ASSERT_FLOAT_WITHIN(FRAC_EPS, 1.0f, (float)decay_fraction(5730.0, 0.0));
}

void test_decay_fraction_at_one_half_life(void) {
    /* At t = half_life, parent = 0.5 exactly */
    TEST_ASSERT_FLOAT_WITHIN(FRAC_EPS, 0.5f, (float)decay_fraction(5730.0, 5730.0));
}

void test_decay_fraction_at_two_half_lives(void) {
    /* At t = 2 * half_life, parent = 0.25 */
    TEST_ASSERT_FLOAT_WITHIN(FRAC_EPS, 0.25f, (float)decay_fraction(5730.0, 2.0 * 5730.0));
}

void test_decay_fraction_at_three_half_lives(void) {
    /* At t = 3 * half_life, parent = 0.125 */
    TEST_ASSERT_FLOAT_WITHIN(FRAC_EPS, 0.125f, (float)decay_fraction(5730.0, 3.0 * 5730.0));
}

void test_decay_fraction_u238_one_half_life(void) {
    /* Universal: at 1 half-life, parent = 0.5 for any system */
    TEST_ASSERT_FLOAT_WITHIN(FRAC_EPS, 0.5f, (float)decay_fraction(4.468e9, 4.468e9));
}

/* ---- Half-lives elapsed ---- */

void test_half_lives_elapsed_exact(void) {
    TEST_ASSERT_FLOAT_WITHIN(FRAC_EPS, 1.0f, (float)half_lives_elapsed(5730.0, 5730.0));
}

void test_half_lives_elapsed_two(void) {
    TEST_ASSERT_FLOAT_WITHIN(FRAC_EPS, 2.0f, (float)half_lives_elapsed(5730.0, 11460.0));
}

void test_half_lives_elapsed_fractional(void) {
    TEST_ASSERT_FLOAT_WITHIN(FRAC_EPS, 0.5f, (float)half_lives_elapsed(5730.0, 2865.0));
}

/* ---- Age from ratio tests ---- */

void test_age_from_ratio_one_half_life(void) {
    /* parent=0.5, daughter=0.5 -> age = 1 half-life */
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 5730.0f, (float)age_from_ratio(0.5, 0.5, 5730.0));
}

void test_age_from_ratio_two_half_lives(void) {
    /* parent=0.25, daughter=0.75 -> age = 2 half-lives */
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 11460.0f, (float)age_from_ratio(0.25, 0.75, 5730.0));
}

void test_age_from_ratio_zero_daughter(void) {
    /* parent=1.0, daughter=0.0 -> age = 0 */
    TEST_ASSERT_FLOAT_WITHIN(FRAC_EPS, 0.0f, (float)age_from_ratio(1.0, 0.0, 5730.0));
}

void test_age_from_ratio_round_trip(void) {
    /* Generate a parent fraction, then invert to recover the age */
    double half_life = 4.468e9;
    double elapsed = 1.5e9;
    double parent = decay_fraction(half_life, elapsed);
    double daughter = 1.0 - parent;
    double recovered = age_from_ratio(parent, daughter, half_life);
    /* Allow 0.1% relative tolerance */
    TEST_ASSERT_FLOAT_WITHIN((float)(elapsed * 0.001), (float)elapsed, (float)recovered);
}

void test_age_from_ratio_round_trip_c14(void) {
    double half_life = 5730.0;
    double elapsed = 8000.0;
    double parent = decay_fraction(half_life, elapsed);
    double daughter = 1.0 - parent;
    double recovered = age_from_ratio(parent, daughter, half_life);
    TEST_ASSERT_FLOAT_WITHIN((float)(elapsed * 0.001), (float)elapsed, (float)recovered);
}

/* ---- Decay curve tests ---- */

void test_decay_curve_returns_n_points(void) {
    decay_point_t points[10];
    int n = decay_curve(DECAY_C14_N14, points, 10, 50000.0);
    TEST_ASSERT_EQUAL_INT(10, n);
}

void test_decay_curve_first_point_is_full_parent(void) {
    decay_point_t points[10];
    decay_curve(DECAY_C14_N14, points, 10, 50000.0);
    TEST_ASSERT_FLOAT_WITHIN(FRAC_EPS, 0.0f, points[0].time_fraction);
    TEST_ASSERT_FLOAT_WITHIN(FRAC_EPS, 1.0f, points[0].parent_fraction);
    TEST_ASSERT_FLOAT_WITHIN(FRAC_EPS, 0.0f, points[0].daughter_fraction);
}

void test_decay_curve_last_point_time_fraction_is_one(void) {
    decay_point_t points[10];
    decay_curve(DECAY_C14_N14, points, 10, 50000.0);
    TEST_ASSERT_FLOAT_WITHIN(FRAC_EPS, 1.0f, points[9].time_fraction);
}

void test_decay_curve_parent_monotonically_decreasing(void) {
    decay_point_t points[20];
    decay_curve(DECAY_U238_PB206, points, 20, 4.468e9);
    int i;
    for (i = 1; i < 20; i++) {
        TEST_ASSERT_TRUE(points[i].parent_fraction <= points[i - 1].parent_fraction);
    }
}

void test_decay_curve_daughter_monotonically_increasing(void) {
    decay_point_t points[20];
    decay_curve(DECAY_U238_PB206, points, 20, 4.468e9);
    int i;
    for (i = 1; i < 20; i++) {
        TEST_ASSERT_TRUE(points[i].daughter_fraction >= points[i - 1].daughter_fraction);
    }
}

void test_decay_curve_parent_plus_daughter_equals_one(void) {
    decay_point_t points[10];
    int i;
    decay_curve(DECAY_C14_N14, points, 10, 50000.0);
    for (i = 0; i < 10; i++) {
        float sum = points[i].parent_fraction + points[i].daughter_fraction;
        TEST_ASSERT_FLOAT_WITHIN(1e-4f, 1.0f, sum);
    }
}

void test_decay_curve_invalid_system_returns_zero(void) {
    decay_point_t points[5];
    int n = decay_curve(DECAY_SYSTEM_COUNT, points, 5, 1000.0);
    TEST_ASSERT_EQUAL_INT(0, n);
}

/* ---- Range checking tests ---- */

void test_in_range_c14_valid(void) {
    TEST_ASSERT_EQUAL_INT(1, radiometric_in_range(DECAY_C14_N14, 5000.0));
}

void test_in_range_c14_too_young(void) {
    TEST_ASSERT_EQUAL_INT(0, radiometric_in_range(DECAY_C14_N14, 50.0));
}

void test_in_range_c14_too_old(void) {
    TEST_ASSERT_EQUAL_INT(0, radiometric_in_range(DECAY_C14_N14, 60000.0));
}

void test_in_range_u238_valid(void) {
    TEST_ASSERT_EQUAL_INT(1, radiometric_in_range(DECAY_U238_PB206, 1e9));
}

void test_in_range_invalid_system(void) {
    TEST_ASSERT_EQUAL_INT(0, radiometric_in_range(DECAY_SYSTEM_COUNT, 1000.0));
}

/* ---- Best system selection tests ---- */

void test_best_system_for_young_organic(void) {
    /* ~5000 years -> C-14 */
    TEST_ASSERT_EQUAL_INT(DECAY_C14_N14, radiometric_best_system(5000.0));
}

void test_best_system_for_old_rocks(void) {
    /* ~4 billion years: multiple systems valid, should pick one */
    decay_system_t best = radiometric_best_system(4.0e9);
    TEST_ASSERT_TRUE(best < DECAY_SYSTEM_COUNT);
    /* Must be in range */
    TEST_ASSERT_EQUAL_INT(1, radiometric_in_range(best, 4.0e9));
}

void test_best_system_for_very_young(void) {
    /* 10 years: no system covers this */
    TEST_ASSERT_EQUAL_INT(DECAY_SYSTEM_COUNT, radiometric_best_system(10.0));
}

void test_best_system_for_medium_age(void) {
    /* ~500 million years: several systems valid */
    decay_system_t best = radiometric_best_system(5.0e8);
    TEST_ASSERT_TRUE(best < DECAY_SYSTEM_COUNT);
    TEST_ASSERT_EQUAL_INT(1, radiometric_in_range(best, 5.0e8));
}

/* ---- All 8 systems have valid data ---- */

void test_all_systems_have_positive_half_life(void) {
    int i;
    for (i = 0; i < RADIOMETRIC_SYSTEM_COUNT; i++) {
        decay_data_t data = radiometric_system((decay_system_t)i);
        TEST_ASSERT_TRUE(data.half_life_years > 0.0);
    }
}

void test_all_systems_have_valid_decay_constant(void) {
    int i;
    for (i = 0; i < RADIOMETRIC_SYSTEM_COUNT; i++) {
        decay_data_t data = radiometric_system((decay_system_t)i);
        float expected = (float)(log(2.0) / data.half_life_years);
        TEST_ASSERT_FLOAT_WITHIN(expected * 1e-4f, expected, (float)data.decay_constant);
    }
}

void test_all_systems_have_names(void) {
    int i;
    for (i = 0; i < RADIOMETRIC_SYSTEM_COUNT; i++) {
        TEST_ASSERT_NOT_NULL(radiometric_parent((decay_system_t)i));
        TEST_ASSERT_NOT_NULL(radiometric_daughter((decay_system_t)i));
    }
}

void test_all_systems_useful_range_min_lt_max(void) {
    int i;
    for (i = 0; i < RADIOMETRIC_SYSTEM_COUNT; i++) {
        decay_data_t data = radiometric_system((decay_system_t)i);
        TEST_ASSERT_TRUE(data.useful_range_min_yr < data.useful_range_max_yr);
    }
}

/* ---- Runner ---- */

int main(void) {
    UNITY_BEGIN();

    /* System data (13 tests) */
    RUN_TEST(test_system_count_is_8);
    RUN_TEST(test_u238_half_life);
    RUN_TEST(test_u235_half_life);
    RUN_TEST(test_c14_half_life);
    RUN_TEST(test_k40_half_life);
    RUN_TEST(test_rb87_half_life);
    RUN_TEST(test_invalid_system_half_life_returns_zero);
    RUN_TEST(test_parent_name_u238);
    RUN_TEST(test_daughter_name_u238);
    RUN_TEST(test_parent_name_c14);
    RUN_TEST(test_daughter_name_c14);
    RUN_TEST(test_system_data_decay_constant);
    RUN_TEST(test_th232_system_data);

    /* Decay fraction (5 tests) */
    RUN_TEST(test_decay_fraction_at_zero_time);
    RUN_TEST(test_decay_fraction_at_one_half_life);
    RUN_TEST(test_decay_fraction_at_two_half_lives);
    RUN_TEST(test_decay_fraction_at_three_half_lives);
    RUN_TEST(test_decay_fraction_u238_one_half_life);

    /* Half-lives elapsed (3 tests) */
    RUN_TEST(test_half_lives_elapsed_exact);
    RUN_TEST(test_half_lives_elapsed_two);
    RUN_TEST(test_half_lives_elapsed_fractional);

    /* Age from ratio (5 tests) */
    RUN_TEST(test_age_from_ratio_one_half_life);
    RUN_TEST(test_age_from_ratio_two_half_lives);
    RUN_TEST(test_age_from_ratio_zero_daughter);
    RUN_TEST(test_age_from_ratio_round_trip);
    RUN_TEST(test_age_from_ratio_round_trip_c14);

    /* Decay curve (7 tests) */
    RUN_TEST(test_decay_curve_returns_n_points);
    RUN_TEST(test_decay_curve_first_point_is_full_parent);
    RUN_TEST(test_decay_curve_last_point_time_fraction_is_one);
    RUN_TEST(test_decay_curve_parent_monotonically_decreasing);
    RUN_TEST(test_decay_curve_daughter_monotonically_increasing);
    RUN_TEST(test_decay_curve_parent_plus_daughter_equals_one);
    RUN_TEST(test_decay_curve_invalid_system_returns_zero);

    /* Range checking (5 tests) */
    RUN_TEST(test_in_range_c14_valid);
    RUN_TEST(test_in_range_c14_too_young);
    RUN_TEST(test_in_range_c14_too_old);
    RUN_TEST(test_in_range_u238_valid);
    RUN_TEST(test_in_range_invalid_system);

    /* Best system selection (4 tests) */
    RUN_TEST(test_best_system_for_young_organic);
    RUN_TEST(test_best_system_for_old_rocks);
    RUN_TEST(test_best_system_for_very_young);
    RUN_TEST(test_best_system_for_medium_age);

    /* All systems validation (4 tests) */
    RUN_TEST(test_all_systems_have_positive_half_life);
    RUN_TEST(test_all_systems_have_valid_decay_constant);
    RUN_TEST(test_all_systems_have_names);
    RUN_TEST(test_all_systems_useful_range_min_lt_max);

    return UNITY_END();
}
