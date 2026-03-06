/* test_radiometric.c — Tests for radiometric dating constants and decay math.
 * TDD RED phase: all tests written before implementation.
 * 8 isotope systems, decay fractions, age estimation, decay curves. */

#include "../../unity/unity.h"
#include "../../../src/systems/geology/radiometric.h"

#include <math.h>
#include <string.h>

/* Tolerances */
#define FRAC_EPS  1e-5f    /* fraction tolerance */
#define AGE_REL   0.001f   /* 0.1% relative for age round-trips */

void setUp(void) { }
void tearDown(void) { }

/* ==== System count ==== */

void test_system_count_returns_8(void) {
    TEST_ASSERT_EQUAL_INT(8, radiometric_system_count());
}

void test_system_count_matches_define(void) {
    TEST_ASSERT_EQUAL_INT(RADIOMETRIC_SYSTEM_COUNT, radiometric_system_count());
}

/* ==== System data: radiometric_system_get ==== */

void test_system_get_c14(void) {
    radiometric_system_t s = radiometric_system_get(0);
    TEST_ASSERT_EQUAL_INT(0, s.id);
    TEST_ASSERT_EQUAL_STRING("C-14", s.parent);
    TEST_ASSERT_EQUAL_STRING("N-14", s.daughter);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 5730.0f, (float)s.half_life_years);
    TEST_ASSERT_EQUAL_STRING("Radiocarbon", s.method_name);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 100.0f, (float)s.useful_range_min);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 50000.0f, (float)s.useful_range_max);
}

void test_system_get_k40(void) {
    radiometric_system_t s = radiometric_system_get(1);
    TEST_ASSERT_EQUAL_STRING("K-40", s.parent);
    TEST_ASSERT_EQUAL_STRING("Ar-40", s.daughter);
    TEST_ASSERT_FLOAT_WITHIN(1e5f, 1.248e9f, (float)s.half_life_years);
    TEST_ASSERT_EQUAL_STRING("Potassium-Argon", s.method_name);
}

void test_system_get_rb87(void) {
    radiometric_system_t s = radiometric_system_get(2);
    TEST_ASSERT_EQUAL_STRING("Rb-87", s.parent);
    TEST_ASSERT_EQUAL_STRING("Sr-87", s.daughter);
    TEST_ASSERT_FLOAT_WITHIN(1e6f, 4.88e10f, (float)s.half_life_years);
    TEST_ASSERT_EQUAL_STRING("Rubidium-Strontium", s.method_name);
}

void test_system_get_u235(void) {
    radiometric_system_t s = radiometric_system_get(3);
    TEST_ASSERT_EQUAL_STRING("U-235", s.parent);
    TEST_ASSERT_EQUAL_STRING("Pb-207", s.daughter);
    TEST_ASSERT_FLOAT_WITHIN(1e4f, 7.04e8f, (float)s.half_life_years);
    TEST_ASSERT_EQUAL_STRING("Uranium-Lead (235)", s.method_name);
}

void test_system_get_u238(void) {
    radiometric_system_t s = radiometric_system_get(4);
    TEST_ASSERT_EQUAL_STRING("U-238", s.parent);
    TEST_ASSERT_EQUAL_STRING("Pb-206", s.daughter);
    TEST_ASSERT_FLOAT_WITHIN(1e5f, 4.468e9f, (float)s.half_life_years);
    TEST_ASSERT_EQUAL_STRING("Uranium-Lead (238)", s.method_name);
}

void test_system_get_th232(void) {
    radiometric_system_t s = radiometric_system_get(5);
    TEST_ASSERT_EQUAL_STRING("Th-232", s.parent);
    TEST_ASSERT_EQUAL_STRING("Pb-208", s.daughter);
    TEST_ASSERT_FLOAT_WITHIN(1e6f, 1.405e10f, (float)s.half_life_years);
    TEST_ASSERT_EQUAL_STRING("Thorium-Lead", s.method_name);
}

void test_system_get_sm147(void) {
    radiometric_system_t s = radiometric_system_get(6);
    TEST_ASSERT_EQUAL_STRING("Sm-147", s.parent);
    TEST_ASSERT_EQUAL_STRING("Nd-143", s.daughter);
    TEST_ASSERT_FLOAT_WITHIN(1e7f, 1.06e11f, (float)s.half_life_years);
    TEST_ASSERT_EQUAL_STRING("Samarium-Neodymium", s.method_name);
}

void test_system_get_lu176(void) {
    radiometric_system_t s = radiometric_system_get(7);
    TEST_ASSERT_EQUAL_STRING("Lu-176", s.parent);
    TEST_ASSERT_EQUAL_STRING("Hf-176", s.daughter);
    TEST_ASSERT_FLOAT_WITHIN(1e6f, 3.71e10f, (float)s.half_life_years);
    TEST_ASSERT_EQUAL_STRING("Lutetium-Hafnium", s.method_name);
}

void test_system_get_invalid_returns_negative_id(void) {
    radiometric_system_t s = radiometric_system_get(99);
    TEST_ASSERT_EQUAL_INT(-1, s.id);
}

void test_system_get_negative_index_returns_negative_id(void) {
    radiometric_system_t s = radiometric_system_get(-1);
    TEST_ASSERT_EQUAL_INT(-1, s.id);
}

/* ==== Decay constant ==== */

void test_decay_constant_c14(void) {
    float expected = (float)(log(2.0) / 5730.0);
    TEST_ASSERT_FLOAT_WITHIN(expected * 1e-4f, expected,
                             (float)radiometric_decay_constant(5730.0));
}

void test_decay_constant_u238(void) {
    float expected = (float)(log(2.0) / 4.468e9);
    TEST_ASSERT_FLOAT_WITHIN(expected * 1e-4f, expected,
                             (float)radiometric_decay_constant(4.468e9));
}

void test_decay_constant_stored_matches_computed(void) {
    int i;
    for (i = 0; i < radiometric_system_count(); i++) {
        radiometric_system_t s = radiometric_system_get(i);
        float expected = (float)(log(2.0) / s.half_life_years);
        TEST_ASSERT_FLOAT_WITHIN(expected * 1e-4f, expected,
                                 (float)s.decay_constant);
    }
}

/* ==== Find by parent ==== */

void test_find_by_parent_c14(void) {
    radiometric_system_t s = radiometric_find_by_parent("C-14");
    TEST_ASSERT_EQUAL_INT(0, s.id);
    TEST_ASSERT_EQUAL_STRING("C-14", s.parent);
}

void test_find_by_parent_u238(void) {
    radiometric_system_t s = radiometric_find_by_parent("U-238");
    TEST_ASSERT_EQUAL_INT(4, s.id);
}

void test_find_by_parent_not_found(void) {
    radiometric_system_t s = radiometric_find_by_parent("Unobtanium");
    TEST_ASSERT_EQUAL_INT(-1, s.id);
}

void test_find_by_parent_null(void) {
    radiometric_system_t s = radiometric_find_by_parent(NULL);
    TEST_ASSERT_EQUAL_INT(-1, s.id);
}

/* ==== Remaining fraction (parent) ==== */

void test_remaining_fraction_at_zero(void) {
    TEST_ASSERT_FLOAT_WITHIN(FRAC_EPS, 1.0f,
        (float)radiometric_remaining_fraction(5730.0, 0.0));
}

void test_remaining_fraction_at_one_half_life(void) {
    TEST_ASSERT_FLOAT_WITHIN(FRAC_EPS, 0.5f,
        (float)radiometric_remaining_fraction(5730.0, 5730.0));
}

void test_remaining_fraction_at_two_half_lives(void) {
    TEST_ASSERT_FLOAT_WITHIN(FRAC_EPS, 0.25f,
        (float)radiometric_remaining_fraction(5730.0, 2.0 * 5730.0));
}

void test_remaining_fraction_at_three_half_lives(void) {
    TEST_ASSERT_FLOAT_WITHIN(FRAC_EPS, 0.125f,
        (float)radiometric_remaining_fraction(5730.0, 3.0 * 5730.0));
}

void test_remaining_fraction_u238_one_half_life(void) {
    TEST_ASSERT_FLOAT_WITHIN(FRAC_EPS, 0.5f,
        (float)radiometric_remaining_fraction(4.468e9, 4.468e9));
}

/* ==== Daughter fraction ==== */

void test_daughter_fraction_at_zero(void) {
    TEST_ASSERT_FLOAT_WITHIN(FRAC_EPS, 0.0f,
        (float)radiometric_daughter_fraction(5730.0, 0.0));
}

void test_daughter_fraction_at_one_half_life(void) {
    TEST_ASSERT_FLOAT_WITHIN(FRAC_EPS, 0.5f,
        (float)radiometric_daughter_fraction(5730.0, 5730.0));
}

void test_daughter_fraction_at_two_half_lives(void) {
    TEST_ASSERT_FLOAT_WITHIN(FRAC_EPS, 0.75f,
        (float)radiometric_daughter_fraction(5730.0, 2.0 * 5730.0));
}

void test_parent_plus_daughter_equals_one(void) {
    double hl = 5730.0;
    double t = 8000.0;
    float sum = (float)(radiometric_remaining_fraction(hl, t) +
                        radiometric_daughter_fraction(hl, t));
    TEST_ASSERT_FLOAT_WITHIN(FRAC_EPS, 1.0f, sum);
}

/* ==== Age from ratio ==== */

void test_age_from_ratio_one_half_life(void) {
    /* parent=0.5, daughter=0.5 -> age = 1 half-life */
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 5730.0f,
        (float)radiometric_age_from_ratio(5730.0, 0.5, 0.5));
}

void test_age_from_ratio_two_half_lives(void) {
    /* parent=0.25, daughter=0.75 -> age = 2 half-lives */
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 11460.0f,
        (float)radiometric_age_from_ratio(5730.0, 0.25, 0.75));
}

void test_age_from_ratio_zero_daughter(void) {
    /* No daughter -> age = 0 */
    TEST_ASSERT_FLOAT_WITHIN(FRAC_EPS, 0.0f,
        (float)radiometric_age_from_ratio(5730.0, 1.0, 0.0));
}

void test_age_from_ratio_round_trip_u238(void) {
    double hl = 4.468e9;
    double elapsed = 1.5e9;
    double parent = radiometric_remaining_fraction(hl, elapsed);
    double daughter = radiometric_daughter_fraction(hl, elapsed);
    float recovered = (float)radiometric_age_from_ratio(hl, parent, daughter);
    TEST_ASSERT_FLOAT_WITHIN((float)(elapsed * 0.001), (float)elapsed, recovered);
}

void test_age_from_ratio_round_trip_c14(void) {
    double hl = 5730.0;
    double elapsed = 8000.0;
    double parent = radiometric_remaining_fraction(hl, elapsed);
    double daughter = radiometric_daughter_fraction(hl, elapsed);
    float recovered = (float)radiometric_age_from_ratio(hl, parent, daughter);
    TEST_ASSERT_FLOAT_WITHIN((float)(elapsed * 0.001), (float)elapsed, recovered);
}

void test_age_from_ratio_invalid_parent_zero(void) {
    TEST_ASSERT_FLOAT_WITHIN(FRAC_EPS, -1.0f,
        (float)radiometric_age_from_ratio(5730.0, 0.0, 0.5));
}

void test_age_from_ratio_invalid_negative_daughter(void) {
    TEST_ASSERT_FLOAT_WITHIN(FRAC_EPS, -1.0f,
        (float)radiometric_age_from_ratio(5730.0, 0.5, -0.1));
}

/* ==== Decay curve ==== */

void test_decay_curve_returns_n_points(void) {
    decay_point_t points[10];
    int n = radiometric_decay_curve(5.0, points, 10);
    TEST_ASSERT_EQUAL_INT(10, n);
}

void test_decay_curve_first_point(void) {
    decay_point_t points[10];
    radiometric_decay_curve(5.0, points, 10);
    TEST_ASSERT_FLOAT_WITHIN(FRAC_EPS, 0.0f, (float)points[0].time);
    TEST_ASSERT_FLOAT_WITHIN(FRAC_EPS, 1.0f, (float)points[0].parent_fraction);
    TEST_ASSERT_FLOAT_WITHIN(FRAC_EPS, 0.0f, (float)points[0].daughter_fraction);
}

void test_decay_curve_last_point_time(void) {
    decay_point_t points[10];
    radiometric_decay_curve(5.0, points, 10);
    TEST_ASSERT_FLOAT_WITHIN(FRAC_EPS, 5.0f, (float)points[9].time);
}

void test_decay_curve_parent_monotonically_decreasing(void) {
    decay_point_t points[20];
    int i;
    radiometric_decay_curve(5.0, points, 20);
    for (i = 1; i < 20; i++) {
        TEST_ASSERT_TRUE(points[i].parent_fraction <= points[i - 1].parent_fraction);
    }
}

void test_decay_curve_daughter_monotonically_increasing(void) {
    decay_point_t points[20];
    int i;
    radiometric_decay_curve(5.0, points, 20);
    for (i = 1; i < 20; i++) {
        TEST_ASSERT_TRUE(points[i].daughter_fraction >= points[i - 1].daughter_fraction);
    }
}

void test_decay_curve_sum_equals_one(void) {
    decay_point_t points[10];
    int i;
    radiometric_decay_curve(5.0, points, 10);
    for (i = 0; i < 10; i++) {
        float sum = (float)(points[i].parent_fraction + points[i].daughter_fraction);
        TEST_ASSERT_FLOAT_WITHIN(1e-4f, 1.0f, sum);
    }
}

void test_decay_curve_at_one_half_life(void) {
    /* With max_half_lives=1 and n_points=2, point[1] is at t=1 half-life */
    decay_point_t points[2];
    radiometric_decay_curve(1.0, points, 2);
    TEST_ASSERT_FLOAT_WITHIN(FRAC_EPS, 0.5f, (float)points[1].parent_fraction);
    TEST_ASSERT_FLOAT_WITHIN(FRAC_EPS, 0.5f, (float)points[1].daughter_fraction);
}

void test_decay_curve_zero_points_returns_zero(void) {
    decay_point_t points[1];
    int n = radiometric_decay_curve(5.0, points, 0);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_decay_curve_null_returns_zero(void) {
    int n = radiometric_decay_curve(5.0, NULL, 10);
    TEST_ASSERT_EQUAL_INT(0, n);
}

/* ==== In range ==== */

void test_in_range_c14_valid(void) {
    TEST_ASSERT_TRUE(radiometric_in_range(0, 5000.0));
}

void test_in_range_c14_too_young(void) {
    TEST_ASSERT_FALSE(radiometric_in_range(0, 50.0));
}

void test_in_range_c14_too_old(void) {
    TEST_ASSERT_FALSE(radiometric_in_range(0, 60000.0));
}

void test_in_range_u238_valid(void) {
    TEST_ASSERT_TRUE(radiometric_in_range(4, 1e9));
}

void test_in_range_invalid_index(void) {
    TEST_ASSERT_FALSE(radiometric_in_range(99, 1000.0));
}

void test_in_range_negative_index(void) {
    TEST_ASSERT_FALSE(radiometric_in_range(-1, 1000.0));
}

void test_in_range_at_boundary_min(void) {
    TEST_ASSERT_TRUE(radiometric_in_range(0, 100.0));
}

void test_in_range_at_boundary_max(void) {
    TEST_ASSERT_TRUE(radiometric_in_range(0, 50000.0));
}

/* ==== Best system ==== */

void test_best_system_for_young_organic(void) {
    /* ~5000 years -> C-14 (index 0) */
    TEST_ASSERT_EQUAL_INT(0, radiometric_best_system(5000.0));
}

void test_best_system_for_old_rocks(void) {
    /* ~4 billion years: multiple systems valid, must be in range */
    int best = radiometric_best_system(4.0e9);
    TEST_ASSERT_TRUE(best >= 0 && best < RADIOMETRIC_SYSTEM_COUNT);
    TEST_ASSERT_TRUE(radiometric_in_range(best, 4.0e9));
}

void test_best_system_for_very_young(void) {
    /* 10 years: no system covers this, return -1 */
    TEST_ASSERT_EQUAL_INT(-1, radiometric_best_system(10.0));
}

void test_best_system_prefers_c14_for_young(void) {
    /* 1000 years: only C-14 covers it */
    TEST_ASSERT_EQUAL_INT(0, radiometric_best_system(1000.0));
}

void test_best_system_for_medium_age(void) {
    /* ~500 million years: several systems valid, must be in range */
    int best = radiometric_best_system(5.0e8);
    TEST_ASSERT_TRUE(best >= 0 && best < RADIOMETRIC_SYSTEM_COUNT);
    TEST_ASSERT_TRUE(radiometric_in_range(best, 5.0e8));
}

/* ==== All systems validation ==== */

void test_all_systems_have_positive_half_life(void) {
    int i;
    for (i = 0; i < radiometric_system_count(); i++) {
        radiometric_system_t s = radiometric_system_get(i);
        TEST_ASSERT_TRUE(s.half_life_years > 0.0);
    }
}

void test_all_systems_have_names(void) {
    int i;
    for (i = 0; i < radiometric_system_count(); i++) {
        radiometric_system_t s = radiometric_system_get(i);
        TEST_ASSERT_NOT_NULL(s.parent);
        TEST_ASSERT_NOT_NULL(s.daughter);
        TEST_ASSERT_NOT_NULL(s.method_name);
    }
}

void test_all_systems_range_min_lt_max(void) {
    int i;
    for (i = 0; i < radiometric_system_count(); i++) {
        radiometric_system_t s = radiometric_system_get(i);
        TEST_ASSERT_TRUE(s.useful_range_min < s.useful_range_max);
    }
}

void test_all_systems_id_matches_index(void) {
    int i;
    for (i = 0; i < radiometric_system_count(); i++) {
        radiometric_system_t s = radiometric_system_get(i);
        TEST_ASSERT_EQUAL_INT(i, s.id);
    }
}

/* ==== Runner ==== */

int main(void) {
    UNITY_BEGIN();

    /* System count (2 tests) */
    RUN_TEST(test_system_count_returns_8);
    RUN_TEST(test_system_count_matches_define);

    /* System data (10 tests) */
    RUN_TEST(test_system_get_c14);
    RUN_TEST(test_system_get_k40);
    RUN_TEST(test_system_get_rb87);
    RUN_TEST(test_system_get_u235);
    RUN_TEST(test_system_get_u238);
    RUN_TEST(test_system_get_th232);
    RUN_TEST(test_system_get_sm147);
    RUN_TEST(test_system_get_lu176);
    RUN_TEST(test_system_get_invalid_returns_negative_id);
    RUN_TEST(test_system_get_negative_index_returns_negative_id);

    /* Decay constant (3 tests) */
    RUN_TEST(test_decay_constant_c14);
    RUN_TEST(test_decay_constant_u238);
    RUN_TEST(test_decay_constant_stored_matches_computed);

    /* Find by parent (4 tests) */
    RUN_TEST(test_find_by_parent_c14);
    RUN_TEST(test_find_by_parent_u238);
    RUN_TEST(test_find_by_parent_not_found);
    RUN_TEST(test_find_by_parent_null);

    /* Remaining fraction (5 tests) */
    RUN_TEST(test_remaining_fraction_at_zero);
    RUN_TEST(test_remaining_fraction_at_one_half_life);
    RUN_TEST(test_remaining_fraction_at_two_half_lives);
    RUN_TEST(test_remaining_fraction_at_three_half_lives);
    RUN_TEST(test_remaining_fraction_u238_one_half_life);

    /* Daughter fraction (4 tests) */
    RUN_TEST(test_daughter_fraction_at_zero);
    RUN_TEST(test_daughter_fraction_at_one_half_life);
    RUN_TEST(test_daughter_fraction_at_two_half_lives);
    RUN_TEST(test_parent_plus_daughter_equals_one);

    /* Age from ratio (7 tests) */
    RUN_TEST(test_age_from_ratio_one_half_life);
    RUN_TEST(test_age_from_ratio_two_half_lives);
    RUN_TEST(test_age_from_ratio_zero_daughter);
    RUN_TEST(test_age_from_ratio_round_trip_u238);
    RUN_TEST(test_age_from_ratio_round_trip_c14);
    RUN_TEST(test_age_from_ratio_invalid_parent_zero);
    RUN_TEST(test_age_from_ratio_invalid_negative_daughter);

    /* Decay curve (9 tests) */
    RUN_TEST(test_decay_curve_returns_n_points);
    RUN_TEST(test_decay_curve_first_point);
    RUN_TEST(test_decay_curve_last_point_time);
    RUN_TEST(test_decay_curve_parent_monotonically_decreasing);
    RUN_TEST(test_decay_curve_daughter_monotonically_increasing);
    RUN_TEST(test_decay_curve_sum_equals_one);
    RUN_TEST(test_decay_curve_at_one_half_life);
    RUN_TEST(test_decay_curve_zero_points_returns_zero);
    RUN_TEST(test_decay_curve_null_returns_zero);

    /* In range (8 tests) */
    RUN_TEST(test_in_range_c14_valid);
    RUN_TEST(test_in_range_c14_too_young);
    RUN_TEST(test_in_range_c14_too_old);
    RUN_TEST(test_in_range_u238_valid);
    RUN_TEST(test_in_range_invalid_index);
    RUN_TEST(test_in_range_negative_index);
    RUN_TEST(test_in_range_at_boundary_min);
    RUN_TEST(test_in_range_at_boundary_max);

    /* Best system (5 tests) */
    RUN_TEST(test_best_system_for_young_organic);
    RUN_TEST(test_best_system_for_old_rocks);
    RUN_TEST(test_best_system_for_very_young);
    RUN_TEST(test_best_system_prefers_c14_for_young);
    RUN_TEST(test_best_system_for_medium_age);

    /* All systems validation (4 tests) */
    RUN_TEST(test_all_systems_have_positive_half_life);
    RUN_TEST(test_all_systems_have_names);
    RUN_TEST(test_all_systems_range_min_lt_max);
    RUN_TEST(test_all_systems_id_matches_index);

    return UNITY_END();
}
