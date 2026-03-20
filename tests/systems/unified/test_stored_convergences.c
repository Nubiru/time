/* test_stored_convergences.c — Tests for stored convergence date database.
 *
 * Tests sc_list, sc_compute, sc_nearest, sc_count_above,
 * sc_filter_category with NULL safety and data integrity checks. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/stored_convergences.h"
#include "../../../src/math/julian.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* Helper: JD for a Gregorian date */
static double jd_from(int y, int m, int d) {
    return gregorian_to_jd(y, m, (double)d);
}

/* ===================================================================
 * sc_list — basic
 * =================================================================== */

void test_list_null_returns_zero(void) {
    TEST_ASSERT_EQUAL_INT(0, sc_list(NULL));
}

void test_list_returns_positive_count(void) {
    sc_result_t res;
    int n = sc_list(&res);
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_EQUAL_INT(n, res.count);
}

void test_list_count_within_max(void) {
    sc_result_t res;
    int n = sc_list(&res);
    TEST_ASSERT_TRUE(n <= SC_MAX_ENTRIES);
}

void test_list_names_non_null(void) {
    sc_result_t res;
    sc_list(&res);
    for (int i = 0; i < res.count; i++) {
        TEST_ASSERT_NOT_NULL(res.entries[i].name);
    }
}

void test_list_categories_non_null(void) {
    sc_result_t res;
    sc_list(&res);
    for (int i = 0; i < res.count; i++) {
        TEST_ASSERT_NOT_NULL(res.entries[i].category);
    }
}

void test_list_jds_positive(void) {
    sc_result_t res;
    sc_list(&res);
    for (int i = 0; i < res.count; i++) {
        TEST_ASSERT_TRUE(res.entries[i].jd > 0.0);
    }
}

void test_list_system_count_zeroed(void) {
    sc_result_t res;
    sc_list(&res);
    for (int i = 0; i < res.count; i++) {
        TEST_ASSERT_EQUAL_INT(0, res.entries[i].system_count);
    }
}

void test_list_significance_zeroed(void) {
    sc_result_t res;
    sc_list(&res);
    for (int i = 0; i < res.count; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.001, 0.0, res.entries[i].significance);
    }
}

/* ===================================================================
 * sc_list — specific dates
 * =================================================================== */

void test_list_contains_moon_landing(void) {
    sc_result_t res;
    sc_list(&res);
    int found = 0;
    for (int i = 0; i < res.count; i++) {
        if (strcmp(res.entries[i].name, "Moon Landing") == 0) {
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);
}

void test_list_moon_landing_jd_correct(void) {
    sc_result_t res;
    sc_list(&res);
    double expected_jd = jd_from(1969, 7, 20);
    for (int i = 0; i < res.count; i++) {
        if (strcmp(res.entries[i].name, "Moon Landing") == 0) {
            TEST_ASSERT_FLOAT_WITHIN(0.5, expected_jd, res.entries[i].jd);
            return;
        }
    }
    TEST_FAIL_MESSAGE("Moon Landing not found");
}

void test_list_contains_spring_equinox_2026(void) {
    sc_result_t res;
    sc_list(&res);
    int found = 0;
    for (int i = 0; i < res.count; i++) {
        if (strcmp(res.entries[i].name, "Spring Equinox 2026") == 0) {
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);
}

void test_list_spring_equinox_jd_correct(void) {
    sc_result_t res;
    sc_list(&res);
    double expected_jd = jd_from(2026, 3, 20);
    for (int i = 0; i < res.count; i++) {
        if (strcmp(res.entries[i].name, "Spring Equinox 2026") == 0) {
            TEST_ASSERT_FLOAT_WITHIN(0.5, expected_jd, res.entries[i].jd);
            return;
        }
    }
    TEST_FAIL_MESSAGE("Spring Equinox 2026 not found");
}

/* ===================================================================
 * sc_compute
 * =================================================================== */

void test_compute_null_returns_zero(void) {
    TEST_ASSERT_EQUAL_INT(0, sc_compute(NULL));
}

void test_compute_fills_system_count(void) {
    sc_result_t res;
    sc_list(&res);
    sc_compute(&res);
    /* At least one entry should have system_count > 0 */
    int any_nonzero = 0;
    for (int i = 0; i < res.count; i++) {
        if (res.entries[i].system_count > 0) {
            any_nonzero = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(any_nonzero);
}

void test_compute_returns_count(void) {
    sc_result_t res;
    sc_list(&res);
    int n = sc_compute(&res);
    TEST_ASSERT_EQUAL_INT(res.count, n);
}

void test_compute_spring_equinox_has_convergence(void) {
    sc_result_t res;
    sc_list(&res);
    sc_compute(&res);
    for (int i = 0; i < res.count; i++) {
        if (strcmp(res.entries[i].name, "Spring Equinox 2026") == 0) {
            TEST_ASSERT_TRUE(res.entries[i].system_count >= 1);
            return;
        }
    }
    TEST_FAIL_MESSAGE("Spring Equinox 2026 not found");
}

/* ===================================================================
 * sc_nearest
 * =================================================================== */

void test_nearest_null_result_returns_null(void) {
    TEST_ASSERT_NULL(sc_nearest(NULL, 2460000.5));
}

void test_nearest_empty_result_returns_null(void) {
    sc_result_t res;
    memset(&res, 0, sizeof(res));
    res.count = 0;
    TEST_ASSERT_NULL(sc_nearest(&res, 2460000.5));
}

void test_nearest_finds_moon_landing(void) {
    sc_result_t res;
    sc_list(&res);
    double moon_jd = jd_from(1969, 7, 20);
    const sc_entry_t *nearest = sc_nearest(&res, moon_jd + 0.1);
    TEST_ASSERT_NOT_NULL(nearest);
    TEST_ASSERT_EQUAL_STRING("Moon Landing", nearest->name);
}

void test_nearest_finds_millennium(void) {
    sc_result_t res;
    sc_list(&res);
    double mill_jd = jd_from(2000, 1, 1);
    const sc_entry_t *nearest = sc_nearest(&res, mill_jd + 0.2);
    TEST_ASSERT_NOT_NULL(nearest);
    TEST_ASSERT_EQUAL_STRING("Millennium Day", nearest->name);
}

/* ===================================================================
 * sc_count_above
 * =================================================================== */

void test_count_above_null_returns_zero(void) {
    TEST_ASSERT_EQUAL_INT(0, sc_count_above(NULL, 1));
}

void test_count_above_zero_threshold(void) {
    sc_result_t res;
    sc_list(&res);
    sc_compute(&res);
    /* With threshold 0, everything counts */
    int n = sc_count_above(&res, 0);
    TEST_ASSERT_EQUAL_INT(res.count, n);
}

void test_count_above_high_threshold(void) {
    sc_result_t res;
    sc_list(&res);
    sc_compute(&res);
    /* With very high threshold, count should be low or zero */
    int n = sc_count_above(&res, 100);
    TEST_ASSERT_TRUE(n >= 0);
    TEST_ASSERT_TRUE(n <= res.count);
}

void test_count_above_monotonic(void) {
    sc_result_t res;
    sc_list(&res);
    sc_compute(&res);
    int c1 = sc_count_above(&res, 1);
    int c2 = sc_count_above(&res, 2);
    int c3 = sc_count_above(&res, 3);
    /* Higher threshold => fewer or equal results */
    TEST_ASSERT_TRUE(c1 >= c2);
    TEST_ASSERT_TRUE(c2 >= c3);
}

/* ===================================================================
 * sc_filter_category
 * =================================================================== */

void test_filter_null_result_returns_zero(void) {
    sc_entry_t buf[10];
    TEST_ASSERT_EQUAL_INT(0, sc_filter_category(NULL, "2026", buf, 10));
}

void test_filter_null_category_returns_zero(void) {
    sc_result_t res;
    sc_list(&res);
    sc_entry_t buf[10];
    TEST_ASSERT_EQUAL_INT(0, sc_filter_category(&res, NULL, buf, 10));
}

void test_filter_null_out_returns_zero(void) {
    sc_result_t res;
    sc_list(&res);
    TEST_ASSERT_EQUAL_INT(0, sc_filter_category(&res, "2026", NULL, 10));
}

void test_filter_zero_max_returns_zero(void) {
    sc_result_t res;
    sc_list(&res);
    sc_entry_t buf[10];
    TEST_ASSERT_EQUAL_INT(0, sc_filter_category(&res, "2026", buf, 0));
}

void test_filter_2026_finds_entries(void) {
    sc_result_t res;
    sc_list(&res);
    sc_entry_t buf[SC_MAX_ENTRIES];
    int n = sc_filter_category(&res, "2026", buf, SC_MAX_ENTRIES);
    TEST_ASSERT_TRUE(n > 0);
    /* All returned entries should be category "2026" */
    for (int i = 0; i < n; i++) {
        TEST_ASSERT_EQUAL_STRING("2026", buf[i].category);
    }
}

void test_filter_historical_finds_entries(void) {
    sc_result_t res;
    sc_list(&res);
    sc_entry_t buf[SC_MAX_ENTRIES];
    int n = sc_filter_category(&res, "historical", buf, SC_MAX_ENTRIES);
    TEST_ASSERT_TRUE(n > 0);
    for (int i = 0; i < n; i++) {
        TEST_ASSERT_EQUAL_STRING("historical", buf[i].category);
    }
}

void test_filter_astronomical_finds_entries(void) {
    sc_result_t res;
    sc_list(&res);
    sc_entry_t buf[SC_MAX_ENTRIES];
    int n = sc_filter_category(&res, "astronomical", buf, SC_MAX_ENTRIES);
    TEST_ASSERT_TRUE(n > 0);
    for (int i = 0; i < n; i++) {
        TEST_ASSERT_EQUAL_STRING("astronomical", buf[i].category);
    }
}

void test_filter_unknown_category_returns_zero(void) {
    sc_result_t res;
    sc_list(&res);
    sc_entry_t buf[10];
    int n = sc_filter_category(&res, "nonexistent", buf, 10);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_filter_respects_max_out(void) {
    sc_result_t res;
    sc_list(&res);
    sc_entry_t buf[2];
    int n = sc_filter_category(&res, "2026", buf, 2);
    TEST_ASSERT_TRUE(n <= 2);
}

/* ===================================================================
 * Main
 * =================================================================== */

int main(void) {
    UNITY_BEGIN();

    /* sc_list — basic */
    RUN_TEST(test_list_null_returns_zero);
    RUN_TEST(test_list_returns_positive_count);
    RUN_TEST(test_list_count_within_max);
    RUN_TEST(test_list_names_non_null);
    RUN_TEST(test_list_categories_non_null);
    RUN_TEST(test_list_jds_positive);
    RUN_TEST(test_list_system_count_zeroed);
    RUN_TEST(test_list_significance_zeroed);

    /* sc_list — specific dates */
    RUN_TEST(test_list_contains_moon_landing);
    RUN_TEST(test_list_moon_landing_jd_correct);
    RUN_TEST(test_list_contains_spring_equinox_2026);
    RUN_TEST(test_list_spring_equinox_jd_correct);

    /* sc_compute */
    RUN_TEST(test_compute_null_returns_zero);
    RUN_TEST(test_compute_fills_system_count);
    RUN_TEST(test_compute_returns_count);
    RUN_TEST(test_compute_spring_equinox_has_convergence);

    /* sc_nearest */
    RUN_TEST(test_nearest_null_result_returns_null);
    RUN_TEST(test_nearest_empty_result_returns_null);
    RUN_TEST(test_nearest_finds_moon_landing);
    RUN_TEST(test_nearest_finds_millennium);

    /* sc_count_above */
    RUN_TEST(test_count_above_null_returns_zero);
    RUN_TEST(test_count_above_zero_threshold);
    RUN_TEST(test_count_above_high_threshold);
    RUN_TEST(test_count_above_monotonic);

    /* sc_filter_category */
    RUN_TEST(test_filter_null_result_returns_zero);
    RUN_TEST(test_filter_null_category_returns_zero);
    RUN_TEST(test_filter_null_out_returns_zero);
    RUN_TEST(test_filter_zero_max_returns_zero);
    RUN_TEST(test_filter_2026_finds_entries);
    RUN_TEST(test_filter_historical_finds_entries);
    RUN_TEST(test_filter_astronomical_finds_entries);
    RUN_TEST(test_filter_unknown_category_returns_zero);
    RUN_TEST(test_filter_respects_max_out);

    return UNITY_END();
}
