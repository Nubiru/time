/* test_precession_detect.c — Precession Encoder Detector tests
 * TDD RED phase: tests written before implementation.
 * Tests whether ancient time systems encode the axial precession cycle
 * (~25,772 years). Tests cycle × multiplier matching, zodiacal age
 * computation, pre-registered cultural systems, and full report generation. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/precession_detect.h"
#include <math.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== Constants verification ===== */

void test_precession_period_value(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.1, 25772.0, PRECESSION_PERIOD_YEARS);
}

void test_precession_rate_deg_per_year(void)
{
    double expected = 360.0 / 25772.0;
    TEST_ASSERT_FLOAT_WITHIN(1e-10, expected, PRECESSION_RATE_DEG_PER_YEAR);
}

void test_precession_age_years(void)
{
    double expected = 25772.0 / 12.0;
    TEST_ASSERT_FLOAT_WITHIN(0.01, expected, PRECESSION_AGE_YEARS);
}

void test_precession_years_per_degree_macro(void)
{
    double expected = 25772.0 / 360.0;
    TEST_ASSERT_FLOAT_WITHIN(0.01, expected, PRECESSION_YEARS_PER_DEGREE_VAL);
}

/* ===== precession_test ===== */

void test_precession_test_exact(void)
{
    /* 25772 × 1 = exact match */
    precession_match_t m = precession_test(25772.0, 1);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 25772.0, m.product);
    TEST_ASSERT_FLOAT_WITHIN(1e-6, 0.0, m.error_percent);
}

void test_precession_test_mayan(void)
{
    /* 5125.36 × 5 = 25626.8 */
    precession_match_t m = precession_test(5125.36, 5);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 25626.8, m.product);
    TEST_ASSERT_TRUE(m.error_percent < 1.0);
    TEST_ASSERT_FLOAT_WITHIN(0.1, 145.2, m.error_years);
}

void test_precession_test_hindu(void)
{
    /* 2160 × 12 = 25920 */
    precession_match_t m = precession_test(2160.0, 12);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 25920.0, m.product);
    TEST_ASSERT_TRUE(m.error_percent < 1.0);
}

void test_precession_test_hebrew(void)
{
    /* 50 × 516 = 25800 — very close (0.11%) */
    precession_match_t m = precession_test(50.0, 516);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 25800.0, m.product);
    TEST_ASSERT_TRUE(m.error_percent < 0.2);
}

void test_precession_test_error_sign(void)
{
    /* error_years should be absolute (positive) */
    precession_match_t m = precession_test(50.0, 516);
    TEST_ASSERT_TRUE(m.error_years >= 0.0);
}

void test_precession_test_stores_inputs(void)
{
    precession_match_t m = precession_test(3600.0, 7);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 3600.0, m.cycle_length);
    TEST_ASSERT_EQUAL_INT(7, m.multiplier);
}

/* ===== precession_best_match ===== */

void test_best_match_exact_divisor(void)
{
    /* 25772 itself: mult=1 should be best */
    precession_match_t m = precession_best_match(25772.0, 10);
    TEST_ASSERT_EQUAL_INT(1, m.multiplier);
    TEST_ASSERT_FLOAT_WITHIN(1e-6, 0.0, m.error_percent);
}

void test_best_match_mayan(void)
{
    precession_match_t m = precession_best_match(5125.36, 10);
    TEST_ASSERT_EQUAL_INT(5, m.multiplier);
    TEST_ASSERT_TRUE(m.error_percent < 1.0);
}

void test_best_match_jubilee(void)
{
    /* 50 × mult up to 600: best should be 515 or 516 */
    precession_match_t m = precession_best_match(50.0, 600);
    /* 50 × 515 = 25750 (err 0.085%) or 50 × 516 = 25800 (err 0.109%) */
    TEST_ASSERT_TRUE(m.multiplier == 515 || m.multiplier == 516);
    TEST_ASSERT_TRUE(m.error_percent < 0.2);
}

void test_best_match_chinese(void)
{
    /* 60 × mult up to 500: best should be 429 or 430 */
    precession_match_t m = precession_best_match(60.0, 500);
    TEST_ASSERT_TRUE(m.multiplier >= 428 && m.multiplier <= 430);
    TEST_ASSERT_TRUE(m.error_percent < 0.5);
}

void test_best_match_max_mult_1(void)
{
    /* With max_mult=1, only one option */
    precession_match_t m = precession_best_match(1000.0, 1);
    TEST_ASSERT_EQUAL_INT(1, m.multiplier);
}

/* ===== precession_is_match ===== */

void test_is_match_good(void)
{
    precession_match_t m = precession_test(50.0, 516);
    TEST_ASSERT_EQUAL_INT(1, precession_is_match(m, 1.0));
}

void test_is_match_bad(void)
{
    precession_match_t m = precession_test(1000.0, 1);
    /* 1000 vs 25772: huge error */
    TEST_ASSERT_EQUAL_INT(0, precession_is_match(m, 1.0));
}

void test_is_match_exact(void)
{
    precession_match_t m = precession_test(25772.0, 1);
    TEST_ASSERT_EQUAL_INT(1, precession_is_match(m, 0.001));
}

/* ===== precession_degrees ===== */

void test_degrees_one_year(void)
{
    double deg = precession_degrees(1.0);
    double expected = 360.0 / 25772.0;
    TEST_ASSERT_FLOAT_WITHIN(1e-8, expected, deg);
}

void test_degrees_72_years(void)
{
    /* ~72 years per degree */
    double deg = precession_degrees(71.6);
    TEST_ASSERT_FLOAT_WITHIN(0.02, 1.0, deg);
}

void test_degrees_full_cycle(void)
{
    double deg = precession_degrees(25772.0);
    TEST_ASSERT_FLOAT_WITHIN(1e-6, 360.0, deg);
}

/* ===== precession_cycles_per_precession ===== */

void test_cycles_per_precession_jubilee(void)
{
    double c = precession_cycles_per_precession(50.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 515.44, c);
}

void test_cycles_per_precession_full(void)
{
    double c = precession_cycles_per_precession(25772.0);
    TEST_ASSERT_FLOAT_WITHIN(1e-6, 1.0, c);
}

/* ===== precession_angle ===== */

void test_angle_at_j2000(void)
{
    /* At J2000, vernal equinox is at ~5 degrees into Pisces */
    double angle = precession_angle(0.0);
    TEST_ASSERT_FLOAT_WITHIN(1.0, 5.0, angle);
}

void test_angle_increases_with_time(void)
{
    /* Precession moves forward in angle over time */
    double a1 = precession_angle(0.0);
    double a2 = precession_angle(100.0);
    double diff = a2 - a1;
    if (diff < 0) diff += 360.0;
    TEST_ASSERT_TRUE(diff > 0.0);
    TEST_ASSERT_FLOAT_WITHIN(0.1, 100.0 * 360.0 / 25772.0, diff);
}

void test_angle_wraps_360(void)
{
    /* After full precession cycle, angle wraps back */
    double angle = precession_angle(25772.0);
    /* Should be back near 5 degrees */
    TEST_ASSERT_FLOAT_WITHIN(1.0, 5.0, angle);
}

void test_angle_negative_time(void)
{
    /* Going backward in time */
    double angle = precession_angle(-1000.0);
    TEST_ASSERT_TRUE(angle >= 0.0 && angle < 360.0);
}

/* ===== precession_current_age ===== */

void test_current_age_at_j2000(void)
{
    /* At J2000, we are in Pisces = age 0 */
    int age = precession_current_age(0.0);
    TEST_ASSERT_EQUAL_INT(0, age);
}

void test_current_age_aquarius(void)
{
    /* ~2150 years from J2000, entering Aquarius (age 1) */
    int age = precession_current_age(2200.0);
    TEST_ASSERT_EQUAL_INT(1, age);
}

void test_current_age_range(void)
{
    /* Age index must be 0-11 */
    for (int i = -5000; i <= 30000; i += 1000) {
        int age = precession_current_age((double)i);
        TEST_ASSERT_TRUE(age >= 0 && age <= 11);
    }
}

/* ===== precession_age_name ===== */

void test_age_name_pisces(void)
{
    const char *name = precession_age_name(0);
    TEST_ASSERT_EQUAL_STRING("Pisces", name);
}

void test_age_name_aquarius(void)
{
    const char *name = precession_age_name(1);
    TEST_ASSERT_EQUAL_STRING("Aquarius", name);
}

void test_age_name_aries(void)
{
    const char *name = precession_age_name(11);
    TEST_ASSERT_EQUAL_STRING("Aries", name);
}

void test_age_name_capricorn(void)
{
    const char *name = precession_age_name(2);
    TEST_ASSERT_EQUAL_STRING("Capricorn", name);
}

void test_age_name_invalid(void)
{
    const char *name = precession_age_name(12);
    TEST_ASSERT_EQUAL_STRING("Unknown", name);
}

void test_age_name_negative(void)
{
    const char *name = precession_age_name(-1);
    TEST_ASSERT_EQUAL_STRING("Unknown", name);
}

/* ===== precession_system_count ===== */

void test_system_count_minimum(void)
{
    int count = precession_system_count();
    TEST_ASSERT_TRUE(count >= 8);
}

/* ===== precession_system_get ===== */

void test_system_get_first(void)
{
    precession_system_t sys = precession_system_get(0);
    TEST_ASSERT_NOT_NULL(sys.system_name);
    TEST_ASSERT_TRUE(strlen(sys.system_name) > 0);
}

void test_system_get_has_matches(void)
{
    /* First system should have at least one match pre-registered */
    precession_system_t sys = precession_system_get(0);
    TEST_ASSERT_TRUE(sys.match_count >= 1);
}

void test_system_get_out_of_range(void)
{
    precession_system_t sys = precession_system_get(999);
    TEST_ASSERT_NULL(sys.system_name);
    TEST_ASSERT_EQUAL_INT(0, sys.match_count);
}

void test_system_greek_exact(void)
{
    /* Greek Platonic Year should have ~0% error */
    int count = precession_system_count();
    int found = 0;
    for (int i = 0; i < count; i++) {
        precession_system_t sys = precession_system_get(i);
        if (strcmp(sys.system_name, "Greek") == 0) {
            found = 1;
            TEST_ASSERT_TRUE(sys.best_error_percent < 0.01);
            break;
        }
    }
    TEST_ASSERT_EQUAL_INT(1, found);
}

/* ===== precession_full_report ===== */

void test_full_report_system_count(void)
{
    precession_report_t report = precession_full_report(3.0, 600);
    TEST_ASSERT_TRUE(report.system_count >= 8);
}

void test_full_report_finds_matches(void)
{
    precession_report_t report = precession_full_report(3.0, 600);
    TEST_ASSERT_TRUE(report.total_matches > 0);
}

void test_full_report_best_system(void)
{
    precession_report_t report = precession_full_report(3.0, 600);
    TEST_ASSERT_NOT_NULL(report.best_system);
    /* Greek (Platonic Year) should have the lowest error (0%) */
    TEST_ASSERT_EQUAL_STRING("Greek", report.best_system);
}

void test_full_report_best_error(void)
{
    precession_report_t report = precession_full_report(3.0, 600);
    TEST_ASSERT_TRUE(report.best_error_percent < 0.01);
}

void test_full_report_threshold_filters(void)
{
    /* Very tight threshold should exclude more systems */
    precession_report_t tight = precession_full_report(0.05, 600);
    precession_report_t loose = precession_full_report(5.0, 600);
    TEST_ASSERT_TRUE(tight.total_matches <= loose.total_matches);
}

/* ===== precession_years_per_degree ===== */

void test_years_per_degree_value(void)
{
    double ypd = precession_years_per_degree();
    double expected = 25772.0 / 360.0;
    TEST_ASSERT_FLOAT_WITHIN(0.01, expected, ypd);
}

void test_years_per_degree_approx_72(void)
{
    double ypd = precession_years_per_degree();
    TEST_ASSERT_FLOAT_WITHIN(0.5, 71.6, ypd);
}

/* ===== Cross-system validation ===== */

void test_all_registered_have_culture(void)
{
    int count = precession_system_count();
    for (int i = 0; i < count; i++) {
        precession_system_t sys = precession_system_get(i);
        TEST_ASSERT_NOT_NULL(sys.culture);
        TEST_ASSERT_TRUE(strlen(sys.culture) > 0);
    }
}

void test_all_best_errors_computed(void)
{
    int count = precession_system_count();
    for (int i = 0; i < count; i++) {
        precession_system_t sys = precession_system_get(i);
        /* best_error_percent should be a valid positive number */
        TEST_ASSERT_TRUE(sys.best_error_percent >= 0.0);
        TEST_ASSERT_TRUE(sys.best_error_percent < 100.0);
    }
}

void test_hebrew_best_is_low(void)
{
    /* Hebrew Jubilee × 515 or 516 should be < 0.2% */
    int count = precession_system_count();
    for (int i = 0; i < count; i++) {
        precession_system_t sys = precession_system_get(i);
        if (strcmp(sys.system_name, "Hebrew") == 0) {
            TEST_ASSERT_TRUE(sys.best_error_percent < 0.2);
            break;
        }
    }
}

int main(void)
{
    UNITY_BEGIN();

    /* Constants */
    RUN_TEST(test_precession_period_value);
    RUN_TEST(test_precession_rate_deg_per_year);
    RUN_TEST(test_precession_age_years);
    RUN_TEST(test_precession_years_per_degree_macro);

    /* precession_test */
    RUN_TEST(test_precession_test_exact);
    RUN_TEST(test_precession_test_mayan);
    RUN_TEST(test_precession_test_hindu);
    RUN_TEST(test_precession_test_hebrew);
    RUN_TEST(test_precession_test_error_sign);
    RUN_TEST(test_precession_test_stores_inputs);

    /* precession_best_match */
    RUN_TEST(test_best_match_exact_divisor);
    RUN_TEST(test_best_match_mayan);
    RUN_TEST(test_best_match_jubilee);
    RUN_TEST(test_best_match_chinese);
    RUN_TEST(test_best_match_max_mult_1);

    /* precession_is_match */
    RUN_TEST(test_is_match_good);
    RUN_TEST(test_is_match_bad);
    RUN_TEST(test_is_match_exact);

    /* precession_degrees */
    RUN_TEST(test_degrees_one_year);
    RUN_TEST(test_degrees_72_years);
    RUN_TEST(test_degrees_full_cycle);

    /* precession_cycles_per_precession */
    RUN_TEST(test_cycles_per_precession_jubilee);
    RUN_TEST(test_cycles_per_precession_full);

    /* precession_angle */
    RUN_TEST(test_angle_at_j2000);
    RUN_TEST(test_angle_increases_with_time);
    RUN_TEST(test_angle_wraps_360);
    RUN_TEST(test_angle_negative_time);

    /* precession_current_age */
    RUN_TEST(test_current_age_at_j2000);
    RUN_TEST(test_current_age_aquarius);
    RUN_TEST(test_current_age_range);

    /* precession_age_name */
    RUN_TEST(test_age_name_pisces);
    RUN_TEST(test_age_name_aquarius);
    RUN_TEST(test_age_name_aries);
    RUN_TEST(test_age_name_capricorn);
    RUN_TEST(test_age_name_invalid);
    RUN_TEST(test_age_name_negative);

    /* precession_system_count */
    RUN_TEST(test_system_count_minimum);

    /* precession_system_get */
    RUN_TEST(test_system_get_first);
    RUN_TEST(test_system_get_has_matches);
    RUN_TEST(test_system_get_out_of_range);
    RUN_TEST(test_system_greek_exact);

    /* precession_full_report */
    RUN_TEST(test_full_report_system_count);
    RUN_TEST(test_full_report_finds_matches);
    RUN_TEST(test_full_report_best_system);
    RUN_TEST(test_full_report_best_error);
    RUN_TEST(test_full_report_threshold_filters);

    /* precession_years_per_degree */
    RUN_TEST(test_years_per_degree_value);
    RUN_TEST(test_years_per_degree_approx_72);

    /* Cross-system validation */
    RUN_TEST(test_all_registered_have_culture);
    RUN_TEST(test_all_best_errors_computed);
    RUN_TEST(test_hebrew_best_is_low);

    return UNITY_END();
}
