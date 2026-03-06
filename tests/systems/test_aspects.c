#include "../unity/unity.h"
#include "../../src/systems/astrology/aspects.h"

#define DEG_EPSILON 0.001

void setUp(void) { }
void tearDown(void) { }

/* ===== angular_separation ===== */

void test_separation_right_angle(void) {
    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 90.0, angular_separation(0.0, 90.0));
}

void test_separation_wrapping(void) {
    /* 350 to 10 = 20 degrees (shortest arc across 0) */
    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 20.0, angular_separation(350.0, 10.0));
}

void test_separation_opposition(void) {
    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 180.0, angular_separation(0.0, 180.0));
}

void test_separation_identical(void) {
    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 0.0, angular_separation(0.0, 0.0));
}

void test_separation_same_at_180(void) {
    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 0.0, angular_separation(180.0, 180.0));
}

void test_separation_symmetric(void) {
    /* Order of arguments should not matter */
    double ab = angular_separation(45.0, 200.0);
    double ba = angular_separation(200.0, 45.0);
    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, ab, ba);
}

/* ===== aspect_check ===== */

void test_check_conjunction_exact(void) {
    TEST_ASSERT_EQUAL_INT(ASPECT_CONJUNCTION, aspect_check(0.0, 8.0));
}

void test_check_conjunction_within_orb(void) {
    TEST_ASSERT_EQUAL_INT(ASPECT_CONJUNCTION, aspect_check(5.0, 8.0));
}

void test_check_conjunction_outside_orb(void) {
    TEST_ASSERT_EQUAL_INT(ASPECT_NONE, aspect_check(9.0, 8.0));
}

void test_check_square(void) {
    TEST_ASSERT_EQUAL_INT(ASPECT_SQUARE, aspect_check(90.0, 7.0));
}

void test_check_trine_within_orb(void) {
    TEST_ASSERT_EQUAL_INT(ASPECT_TRINE, aspect_check(119.5, 8.0));
}

void test_check_no_aspect_at_45(void) {
    /* 45 degrees is not a Ptolemaic aspect */
    TEST_ASSERT_EQUAL_INT(ASPECT_NONE, aspect_check(45.0, 8.0));
}

void test_check_sextile(void) {
    TEST_ASSERT_EQUAL_INT(ASPECT_SEXTILE, aspect_check(62.0, 6.0));
}

void test_check_opposition(void) {
    TEST_ASSERT_EQUAL_INT(ASPECT_OPPOSITION, aspect_check(176.0, 8.0));
}

/* ===== aspect_orb ===== */

void test_orb_square_2_off(void) {
    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 2.0, aspect_orb(88.0, ASPECT_SQUARE));
}

void test_orb_conjunction_exact(void) {
    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 0.0, aspect_orb(0.0, ASPECT_CONJUNCTION));
}

void test_orb_trine_3_off(void) {
    TEST_ASSERT_FLOAT_WITHIN(DEG_EPSILON, 3.0, aspect_orb(117.0, ASPECT_TRINE));
}

/* ===== aspects_find_all ===== */

void test_find_all_grand_trine(void) {
    /* Three planets at 120 degrees apart = 3 trines.
     * Place remaining 5 planets far from any aspect angles. */
    double lons[8] = {10.0, 130.0, 250.0, 55.0, 155.0, 205.0, 305.0, 345.0};
    aspect_list_t list = aspects_find_all(lons, 8.0);
    /* Count trines */
    int trine_count = 0;
    for (int i = 0; i < list.count; i++) {
        if (list.aspects[i].type == ASPECT_TRINE) trine_count++;
    }
    TEST_ASSERT_EQUAL_INT(3, trine_count);
}

void test_find_all_conjunction(void) {
    /* Two planets at same longitude = conjunction */
    double lons[8] = {100.0, 100.0, 200.0, 300.0, 45.0, 155.0, 265.0, 350.0};
    aspect_list_t list = aspects_find_all(lons, 8.0);
    int conj_count = 0;
    for (int i = 0; i < list.count; i++) {
        if (list.aspects[i].type == ASPECT_CONJUNCTION &&
            list.aspects[i].planet_a == 0 && list.aspects[i].planet_b == 1)
            conj_count++;
    }
    TEST_ASSERT_EQUAL_INT(1, conj_count);
}

void test_find_all_stores_planet_indices(void) {
    /* Verify planet_a < planet_b in results */
    double lons[8] = {0.0, 90.0, 180.0, 270.0, 45.0, 135.0, 225.0, 315.0};
    aspect_list_t list = aspects_find_all(lons, 8.0);
    for (int i = 0; i < list.count; i++) {
        TEST_ASSERT_TRUE(list.aspects[i].planet_a < list.aspects[i].planet_b);
    }
}

void test_find_all_count_bounded(void) {
    /* Never more than 28 aspects (8 choose 2) */
    double lons[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    aspect_list_t list = aspects_find_all(lons, 8.0);
    TEST_ASSERT_TRUE(list.count <= 28);
    TEST_ASSERT_TRUE(list.count >= 0);
}

/* ===== aspect_name ===== */

void test_name_trine(void) {
    TEST_ASSERT_EQUAL_STRING("Trine", aspect_name(ASPECT_TRINE));
}

void test_name_none(void) {
    TEST_ASSERT_EQUAL_STRING("None", aspect_name(ASPECT_NONE));
}

void test_name_all_types(void) {
    TEST_ASSERT_EQUAL_STRING("Conjunction", aspect_name(ASPECT_CONJUNCTION));
    TEST_ASSERT_EQUAL_STRING("Sextile", aspect_name(ASPECT_SEXTILE));
    TEST_ASSERT_EQUAL_STRING("Square", aspect_name(ASPECT_SQUARE));
    TEST_ASSERT_EQUAL_STRING("Opposition", aspect_name(ASPECT_OPPOSITION));
}

/* ===== aspect_symbol ===== */

void test_symbol_non_null(void) {
    aspect_type_t types[] = {
        ASPECT_CONJUNCTION, ASPECT_SEXTILE, ASPECT_SQUARE,
        ASPECT_TRINE, ASPECT_OPPOSITION, ASPECT_NONE
    };
    for (int i = 0; i < 6; i++) {
        const char *sym = aspect_symbol(types[i]);
        TEST_ASSERT_NOT_NULL(sym);
        TEST_ASSERT_TRUE(sym[0] != '\0');
    }
}

/* ===== purity ===== */

void test_purity_same_input_same_output(void) {
    double lons[8] = {10.0, 70.0, 130.0, 190.0, 250.0, 310.0, 45.0, 225.0};
    aspect_list_t a = aspects_find_all(lons, 8.0);
    aspect_list_t b = aspects_find_all(lons, 8.0);
    TEST_ASSERT_EQUAL_INT(a.count, b.count);
    for (int i = 0; i < a.count; i++) {
        TEST_ASSERT_EQUAL_INT(a.aspects[i].type, b.aspects[i].type);
        TEST_ASSERT_EQUAL_FLOAT(a.aspects[i].orb, b.aspects[i].orb);
    }
}

int main(void) {
    UNITY_BEGIN();
    /* angular_separation */
    RUN_TEST(test_separation_right_angle);
    RUN_TEST(test_separation_wrapping);
    RUN_TEST(test_separation_opposition);
    RUN_TEST(test_separation_identical);
    RUN_TEST(test_separation_same_at_180);
    RUN_TEST(test_separation_symmetric);
    /* aspect_check */
    RUN_TEST(test_check_conjunction_exact);
    RUN_TEST(test_check_conjunction_within_orb);
    RUN_TEST(test_check_conjunction_outside_orb);
    RUN_TEST(test_check_square);
    RUN_TEST(test_check_trine_within_orb);
    RUN_TEST(test_check_no_aspect_at_45);
    RUN_TEST(test_check_sextile);
    RUN_TEST(test_check_opposition);
    /* aspect_orb */
    RUN_TEST(test_orb_square_2_off);
    RUN_TEST(test_orb_conjunction_exact);
    RUN_TEST(test_orb_trine_3_off);
    /* aspects_find_all */
    RUN_TEST(test_find_all_grand_trine);
    RUN_TEST(test_find_all_conjunction);
    RUN_TEST(test_find_all_stores_planet_indices);
    RUN_TEST(test_find_all_count_bounded);
    /* aspect_name / aspect_symbol */
    RUN_TEST(test_name_trine);
    RUN_TEST(test_name_none);
    RUN_TEST(test_name_all_types);
    RUN_TEST(test_symbol_non_null);
    /* purity */
    RUN_TEST(test_purity_same_input_same_output);
    return UNITY_END();
}
