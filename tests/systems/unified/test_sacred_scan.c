/* test_sacred_scan.c — Tests for sacred number detection across calendar systems.
 *
 * TDD: tests written first for the S89 sacred number frequency module.
 * Tests ss_get_numbers, ss_is_sacred, ss_scan, ss_year_count. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/sacred_scan.h"
#include "../../../src/math/julian.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* Helper: JD for a Gregorian date */
static double jd_from(int y, int m, int d) {
    return gregorian_to_jd(y, m, (double)d);
}

/* ===================================================================
 * ss_get_numbers — list of tracked sacred numbers
 * =================================================================== */

void test_get_numbers_returns_positive_count(void) {
    ss_number_t nums[SS_MAX_NUMBERS];
    int count = ss_get_numbers(nums, SS_MAX_NUMBERS);
    TEST_ASSERT_TRUE(count >= 6);
    TEST_ASSERT_TRUE(count <= SS_MAX_NUMBERS);
}

void test_get_numbers_includes_seven(void) {
    ss_number_t nums[SS_MAX_NUMBERS];
    int count = ss_get_numbers(nums, SS_MAX_NUMBERS);
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (nums[i].value == 7) { found = 1; break; }
    }
    TEST_ASSERT_TRUE(found);
}

void test_get_numbers_includes_thirteen(void) {
    ss_number_t nums[SS_MAX_NUMBERS];
    int count = ss_get_numbers(nums, SS_MAX_NUMBERS);
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (nums[i].value == 13) { found = 1; break; }
    }
    TEST_ASSERT_TRUE(found);
}

void test_get_numbers_includes_sixty_four(void) {
    ss_number_t nums[SS_MAX_NUMBERS];
    int count = ss_get_numbers(nums, SS_MAX_NUMBERS);
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (nums[i].value == 64) { found = 1; break; }
    }
    TEST_ASSERT_TRUE(found);
}

void test_get_numbers_all_have_names(void) {
    ss_number_t nums[SS_MAX_NUMBERS];
    int count = ss_get_numbers(nums, SS_MAX_NUMBERS);
    for (int i = 0; i < count; i++) {
        TEST_ASSERT_NOT_NULL(nums[i].name);
        TEST_ASSERT_TRUE(nums[i].value > 0);
    }
}

void test_get_numbers_null_returns_zero(void) {
    TEST_ASSERT_EQUAL_INT(0, ss_get_numbers(NULL, 10));
}

void test_get_numbers_zero_max_returns_zero(void) {
    ss_number_t nums[SS_MAX_NUMBERS];
    TEST_ASSERT_EQUAL_INT(0, ss_get_numbers(nums, 0));
}

/* ===================================================================
 * ss_is_sacred — check if a value is tracked
 * =================================================================== */

void test_is_sacred_seven(void) {
    const ss_number_t *n = ss_is_sacred(7);
    TEST_ASSERT_NOT_NULL(n);
    TEST_ASSERT_EQUAL_INT(7, n->value);
}

void test_is_sacred_twelve(void) {
    const ss_number_t *n = ss_is_sacred(12);
    TEST_ASSERT_NOT_NULL(n);
    TEST_ASSERT_EQUAL_INT(12, n->value);
}

void test_is_sacred_thirteen(void) {
    const ss_number_t *n = ss_is_sacred(13);
    TEST_ASSERT_NOT_NULL(n);
}

void test_is_sacred_sixty_four(void) {
    const ss_number_t *n = ss_is_sacred(64);
    TEST_ASSERT_NOT_NULL(n);
}

void test_is_sacred_not_sacred_returns_null(void) {
    /* 17 is not in the standard sacred number list */
    TEST_ASSERT_NULL(ss_is_sacred(17));
    TEST_ASSERT_NULL(ss_is_sacred(0));
    TEST_ASSERT_NULL(ss_is_sacred(-1));
}

/* ===================================================================
 * ss_scan — detect sacred numbers in a Julian Day
 * =================================================================== */

void test_scan_null_returns_zero(void) {
    TEST_ASSERT_EQUAL_INT(0, ss_scan(jd_from(2026, 3, 20), NULL));
}

void test_scan_returns_nonnegative(void) {
    ss_result_t r;
    int count = ss_scan(jd_from(2026, 3, 20), &r);
    TEST_ASSERT_TRUE(count >= 0);
    TEST_ASSERT_EQUAL_INT(count, r.count);
}

void test_scan_stores_jd(void) {
    ss_result_t r;
    double jd = jd_from(2026, 3, 20);
    ss_scan(jd, &r);
    TEST_ASSERT_FLOAT_WITHIN(0.5, jd, r.jd);
}

void test_scan_detection_has_system_count(void) {
    ss_result_t r;
    ss_scan(jd_from(2026, 3, 20), &r);
    for (int i = 0; i < r.count; i++) {
        TEST_ASSERT_TRUE(r.detections[i].system_count >= 1);
        TEST_ASSERT_TRUE(r.detections[i].system_count <= SS_MAX_SYSTEMS);
    }
}

void test_scan_detection_has_valid_number(void) {
    ss_result_t r;
    ss_scan(jd_from(2026, 3, 20), &r);
    for (int i = 0; i < r.count; i++) {
        TEST_ASSERT_TRUE(r.detections[i].number > 0);
        /* Must be a recognized sacred number */
        TEST_ASSERT_NOT_NULL(ss_is_sacred(r.detections[i].number));
    }
}

void test_scan_detection_system_names_not_null(void) {
    ss_result_t r;
    ss_scan(jd_from(2026, 3, 20), &r);
    for (int i = 0; i < r.count; i++) {
        for (int j = 0; j < r.detections[i].system_count; j++) {
            TEST_ASSERT_NOT_NULL(r.detections[i].systems[j]);
        }
    }
}

/* On March 20, Gregorian day is 20 — sacred number 20 (Uinal).
 * At minimum, Gregorian should appear. */
void test_scan_march_20_detects_twenty(void) {
    ss_result_t r;
    ss_scan(jd_from(2026, 3, 20), &r);
    int found_20 = 0;
    for (int i = 0; i < r.count; i++) {
        if (r.detections[i].number == 20) {
            found_20 = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found_20);
}

/* March = month 3 — sacred number 3 (Trinity) */
void test_scan_march_detects_three(void) {
    ss_result_t r;
    ss_scan(jd_from(2026, 3, 15), &r);
    int found_3 = 0;
    for (int i = 0; i < r.count; i++) {
        if (r.detections[i].number == 3) {
            found_3 = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found_3);
}

/* On the 7th of a month, Gregorian day = 7 (Heptad) */
void test_scan_day_seven_detects_heptad(void) {
    ss_result_t r;
    ss_scan(jd_from(2026, 1, 7), &r);
    int found_7 = 0;
    for (int i = 0; i < r.count; i++) {
        if (r.detections[i].number == 7) {
            found_7 = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found_7);
}

/* Count does not exceed max */
void test_scan_count_within_max(void) {
    ss_result_t r;
    ss_scan(jd_from(2026, 3, 20), &r);
    TEST_ASSERT_TRUE(r.count <= SS_MAX_DETECTIONS);
}

/* Two different dates produce potentially different detection sets */
void test_scan_different_dates_may_differ(void) {
    ss_result_t r1, r2;
    ss_scan(jd_from(2026, 1, 1), &r1);
    ss_scan(jd_from(2026, 6, 15), &r2);
    /* At least one detection on each day */
    TEST_ASSERT_TRUE(r1.count >= 1);
    TEST_ASSERT_TRUE(r2.count >= 1);
}

/* ===================================================================
 * ss_year_count — days in a year with N+ detections
 * =================================================================== */

void test_year_count_at_least_one_returns_positive(void) {
    int count = ss_year_count(2026, 1);
    /* Every day has at least some sacred number (day 3,4,5,7... always match) */
    TEST_ASSERT_TRUE(count > 0);
    TEST_ASSERT_TRUE(count <= 365);
}

void test_year_count_high_threshold_fewer_days(void) {
    int few = ss_year_count(2026, 5);
    int many = ss_year_count(2026, 1);
    TEST_ASSERT_TRUE(few <= many);
}

void test_year_count_zero_threshold_all_days(void) {
    int count = ss_year_count(2026, 0);
    TEST_ASSERT_EQUAL_INT(365, count);
}

void test_year_count_very_high_threshold_zero(void) {
    /* With threshold higher than any possible detection count */
    int count = ss_year_count(2026, 50);
    TEST_ASSERT_EQUAL_INT(0, count);
}

/* ===================================================================
 * Runner
 * =================================================================== */

int main(void) {
    UNITY_BEGIN();

    /* ss_get_numbers */
    RUN_TEST(test_get_numbers_returns_positive_count);
    RUN_TEST(test_get_numbers_includes_seven);
    RUN_TEST(test_get_numbers_includes_thirteen);
    RUN_TEST(test_get_numbers_includes_sixty_four);
    RUN_TEST(test_get_numbers_all_have_names);
    RUN_TEST(test_get_numbers_null_returns_zero);
    RUN_TEST(test_get_numbers_zero_max_returns_zero);

    /* ss_is_sacred */
    RUN_TEST(test_is_sacred_seven);
    RUN_TEST(test_is_sacred_twelve);
    RUN_TEST(test_is_sacred_thirteen);
    RUN_TEST(test_is_sacred_sixty_four);
    RUN_TEST(test_is_sacred_not_sacred_returns_null);

    /* ss_scan */
    RUN_TEST(test_scan_null_returns_zero);
    RUN_TEST(test_scan_returns_nonnegative);
    RUN_TEST(test_scan_stores_jd);
    RUN_TEST(test_scan_detection_has_system_count);
    RUN_TEST(test_scan_detection_has_valid_number);
    RUN_TEST(test_scan_detection_system_names_not_null);
    RUN_TEST(test_scan_march_20_detects_twenty);
    RUN_TEST(test_scan_march_detects_three);
    RUN_TEST(test_scan_day_seven_detects_heptad);
    RUN_TEST(test_scan_count_within_max);
    RUN_TEST(test_scan_different_dates_may_differ);

    /* ss_year_count */
    RUN_TEST(test_year_count_at_least_one_returns_positive);
    RUN_TEST(test_year_count_high_threshold_fewer_days);
    RUN_TEST(test_year_count_zero_threshold_all_days);
    RUN_TEST(test_year_count_very_high_threshold_zero);

    return UNITY_END();
}
