/* test_ratio_analysis.c — Ratio Analyzer tests
 * TDD RED phase: tests written before implementation.
 * Tests ratio analysis between cycle lengths: simple fractions,
 * mathematical constant matching, scale factors, harmonic series,
 * continued fractions. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/ratio_analysis.h"
#include <math.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== ratio_is_integer ===== */

void test_is_integer_exact(void)
{
    TEST_ASSERT_EQUAL_INT(1, ratio_is_integer(5.0, 0.01));
}

void test_is_integer_close(void)
{
    TEST_ASSERT_EQUAL_INT(1, ratio_is_integer(5.005, 0.01));
}

void test_is_integer_not_close(void)
{
    TEST_ASSERT_EQUAL_INT(0, ratio_is_integer(5.1, 0.01));
}

void test_is_integer_zero(void)
{
    TEST_ASSERT_EQUAL_INT(1, ratio_is_integer(0.0, 0.01));
}

void test_is_integer_negative(void)
{
    TEST_ASSERT_EQUAL_INT(1, ratio_is_integer(-3.0, 0.01));
}

/* ===== ratio_nearest_fraction ===== */

void test_nearest_fraction_phi(void)
{
    ratio_fraction_t f = ratio_nearest_fraction(1.618, 32);
    /* Fibonacci: 21/13 = 1.6153..., 34/21 = 1.6190... */
    /* 34/21 is closer to 1.618 */
    TEST_ASSERT_TRUE(
        (f.numerator == 34 && f.denominator == 21) ||
        (f.numerator == 21 && f.denominator == 13)
    );
    TEST_ASSERT_FLOAT_WITHIN(0.01, 1.618, f.value);
}

void test_nearest_fraction_exact_half(void)
{
    ratio_fraction_t f = ratio_nearest_fraction(0.5, 32);
    TEST_ASSERT_EQUAL_INT(1, f.numerator);
    TEST_ASSERT_EQUAL_INT(2, f.denominator);
    TEST_ASSERT_FLOAT_WITHIN(1e-10, 0.0, f.error);
}

void test_nearest_fraction_exact_third(void)
{
    ratio_fraction_t f = ratio_nearest_fraction(1.0 / 3.0, 32);
    TEST_ASSERT_EQUAL_INT(1, f.numerator);
    TEST_ASSERT_EQUAL_INT(3, f.denominator);
    TEST_ASSERT_FLOAT_WITHIN(1e-10, 0.0, f.error);
}

void test_nearest_fraction_two(void)
{
    ratio_fraction_t f = ratio_nearest_fraction(2.0, 32);
    TEST_ASSERT_EQUAL_INT(2, f.numerator);
    TEST_ASSERT_EQUAL_INT(1, f.denominator);
    TEST_ASSERT_FLOAT_WITHIN(1e-10, 2.0, f.value);
}

void test_nearest_fraction_three_halves(void)
{
    ratio_fraction_t f = ratio_nearest_fraction(1.5, 32);
    TEST_ASSERT_EQUAL_INT(3, f.numerator);
    TEST_ASSERT_EQUAL_INT(2, f.denominator);
    TEST_ASSERT_FLOAT_WITHIN(1e-10, 0.0, f.error);
}

void test_nearest_fraction_max_denom_1(void)
{
    ratio_fraction_t f = ratio_nearest_fraction(1.618, 1);
    TEST_ASSERT_EQUAL_INT(2, f.numerator);
    TEST_ASSERT_EQUAL_INT(1, f.denominator);
}

/* ===== ratio_check_phi ===== */

void test_check_phi_match(void)
{
    ratio_constant_match_t m = ratio_check_phi(1.618, 0.01);
    TEST_ASSERT_EQUAL_INT(1, m.matched);
    TEST_ASSERT_EQUAL_STRING("phi", m.name);
    TEST_ASSERT_FLOAT_WITHIN(0.01, PHI, m.value);
}

void test_check_phi_inverse(void)
{
    /* 1/phi = phi - 1 = 0.618... */
    ratio_constant_match_t m = ratio_check_phi(0.618, 0.01);
    TEST_ASSERT_EQUAL_INT(1, m.matched);
}

void test_check_phi_no_match(void)
{
    ratio_constant_match_t m = ratio_check_phi(2.0, 0.01);
    TEST_ASSERT_EQUAL_INT(0, m.matched);
}

/* ===== ratio_check_e ===== */

void test_check_e_match(void)
{
    ratio_constant_match_t m = ratio_check_e(2.718, 0.01);
    TEST_ASSERT_EQUAL_INT(1, m.matched);
    TEST_ASSERT_EQUAL_STRING("e", m.name);
}

void test_check_e_no_match(void)
{
    ratio_constant_match_t m = ratio_check_e(3.0, 0.01);
    TEST_ASSERT_EQUAL_INT(0, m.matched);
}

/* ===== ratio_check_pi ===== */

void test_check_pi_match(void)
{
    ratio_constant_match_t m = ratio_check_pi(3.1416, 0.001);
    TEST_ASSERT_EQUAL_INT(1, m.matched);
    TEST_ASSERT_EQUAL_STRING("pi", m.name);
}

void test_check_pi_no_match(void)
{
    ratio_constant_match_t m = ratio_check_pi(3.0, 0.01);
    TEST_ASSERT_EQUAL_INT(0, m.matched);
}

/* ===== ratio_check_sqrt2 ===== */

void test_check_sqrt2_match(void)
{
    ratio_constant_match_t m = ratio_check_sqrt2(1.4142, 0.001);
    TEST_ASSERT_EQUAL_INT(1, m.matched);
    TEST_ASSERT_EQUAL_STRING("sqrt2", m.name);
}

void test_check_sqrt2_no_match(void)
{
    ratio_constant_match_t m = ratio_check_sqrt2(1.5, 0.01);
    TEST_ASSERT_EQUAL_INT(0, m.matched);
}

/* ===== ratio_check_sqrt3 ===== */

void test_check_sqrt3_match(void)
{
    ratio_constant_match_t m = ratio_check_sqrt3(1.7321, 0.001);
    TEST_ASSERT_EQUAL_INT(1, m.matched);
    TEST_ASSERT_EQUAL_STRING("sqrt3", m.name);
}

void test_check_sqrt3_no_match(void)
{
    ratio_constant_match_t m = ratio_check_sqrt3(1.8, 0.01);
    TEST_ASSERT_EQUAL_INT(0, m.matched);
}

/* ===== ratio_check_scale ===== */

void test_scale_mahayuga_kaliyuga(void)
{
    /* 4,320,000 / 432,000 = 10 = 10^1 */
    ratio_scale_t s = ratio_check_scale(4320000.0, 432000.0, 0.01);
    TEST_ASSERT_EQUAL_INT(1, s.matched);
    TEST_ASSERT_EQUAL_INT(1, s.power_of_10);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 1.0, s.base_ratio);
}

void test_scale_hundred(void)
{
    ratio_scale_t s = ratio_check_scale(500.0, 5.0, 0.01);
    TEST_ASSERT_EQUAL_INT(1, s.matched);
    TEST_ASSERT_EQUAL_INT(2, s.power_of_10);
}

void test_scale_no_match(void)
{
    ratio_scale_t s = ratio_check_scale(260.0, 365.0, 0.01);
    TEST_ASSERT_EQUAL_INT(0, s.matched);
}

void test_scale_equal(void)
{
    ratio_scale_t s = ratio_check_scale(100.0, 100.0, 0.01);
    TEST_ASSERT_EQUAL_INT(1, s.matched);
    TEST_ASSERT_EQUAL_INT(0, s.power_of_10);
}

/* ===== ratio_harmonic_series ===== */

void test_harmonic_series_440(void)
{
    double h[4];
    int n = ratio_harmonic_series(440.0, 4, h);
    TEST_ASSERT_EQUAL_INT(4, n);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 440.0, h[0]);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 880.0, h[1]);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 1320.0, h[2]);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 1760.0, h[3]);
}

void test_harmonic_series_clamp(void)
{
    double h[RATIO_MAX_HARMONICS];
    int n = ratio_harmonic_series(100.0, 100, h);
    TEST_ASSERT_EQUAL_INT(RATIO_MAX_HARMONICS, n);
}

void test_harmonic_series_zero_count(void)
{
    double h[1];
    int n = ratio_harmonic_series(100.0, 0, h);
    TEST_ASSERT_EQUAL_INT(0, n);
}

/* ===== ratio_subharmonic_series ===== */

void test_subharmonic_series(void)
{
    double s[4];
    int n = ratio_subharmonic_series(440.0, 4, s);
    TEST_ASSERT_EQUAL_INT(4, n);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 440.0, s[0]);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 220.0, s[1]);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 440.0 / 3.0, s[2]);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 110.0, s[3]);
}

void test_subharmonic_series_clamp(void)
{
    double s[RATIO_MAX_HARMONICS];
    int n = ratio_subharmonic_series(100.0, 100, s);
    TEST_ASSERT_EQUAL_INT(RATIO_MAX_HARMONICS, n);
}

/* ===== ratio_continued_fraction ===== */

void test_cf_phi(void)
{
    int cf[10];
    int n = ratio_continued_fraction(PHI, 10, cf);
    TEST_ASSERT_TRUE(n >= 5);
    /* phi = [1; 1, 1, 1, 1, ...] */
    for (int i = 0; i < n; i++) {
        TEST_ASSERT_EQUAL_INT(1, cf[i]);
    }
}

void test_cf_pi(void)
{
    int cf[5];
    int n = ratio_continued_fraction(PI, 5, cf);
    TEST_ASSERT_EQUAL_INT(5, n);
    /* pi = [3; 7, 15, 1, 292, ...] */
    TEST_ASSERT_EQUAL_INT(3, cf[0]);
    TEST_ASSERT_EQUAL_INT(7, cf[1]);
    TEST_ASSERT_EQUAL_INT(15, cf[2]);
    TEST_ASSERT_EQUAL_INT(1, cf[3]);
    TEST_ASSERT_EQUAL_INT(292, cf[4]);
}

void test_cf_e(void)
{
    int cf[6];
    int n = ratio_continued_fraction(EULER_E, 6, cf);
    TEST_ASSERT_EQUAL_INT(6, n);
    /* e = [2; 1, 2, 1, 1, 4, ...] */
    TEST_ASSERT_EQUAL_INT(2, cf[0]);
    TEST_ASSERT_EQUAL_INT(1, cf[1]);
    TEST_ASSERT_EQUAL_INT(2, cf[2]);
    TEST_ASSERT_EQUAL_INT(1, cf[3]);
    TEST_ASSERT_EQUAL_INT(1, cf[4]);
    TEST_ASSERT_EQUAL_INT(4, cf[5]);
}

void test_cf_integer(void)
{
    int cf[5];
    int n = ratio_continued_fraction(7.0, 5, cf);
    TEST_ASSERT_EQUAL_INT(1, n);
    TEST_ASSERT_EQUAL_INT(7, cf[0]);
}

void test_cf_simple_fraction(void)
{
    int cf[5];
    int n = ratio_continued_fraction(3.0 / 2.0, 5, cf);
    /* 3/2 = [1; 2] */
    TEST_ASSERT_TRUE(n >= 2);
    TEST_ASSERT_EQUAL_INT(1, cf[0]);
    TEST_ASSERT_EQUAL_INT(2, cf[1]);
}

/* ===== ratio_from_continued_fraction ===== */

void test_from_cf_phi(void)
{
    int cf[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    double val = ratio_from_continued_fraction(cf, 10);
    TEST_ASSERT_FLOAT_WITHIN(0.001, PHI, val);
}

void test_from_cf_pi(void)
{
    int cf[] = {3, 7, 15, 1, 292};
    double val = ratio_from_continued_fraction(cf, 5);
    TEST_ASSERT_FLOAT_WITHIN(1e-8, PI, val);
}

void test_from_cf_single(void)
{
    int cf[] = {5};
    double val = ratio_from_continued_fraction(cf, 1);
    TEST_ASSERT_FLOAT_WITHIN(1e-10, 5.0, val);
}

void test_from_cf_roundtrip(void)
{
    /* Decompose then reconstruct should recover original */
    double original = 2.75;
    int cf[10];
    int n = ratio_continued_fraction(original, 10, cf);
    double recovered = ratio_from_continued_fraction(cf, n);
    TEST_ASSERT_FLOAT_WITHIN(1e-6, original, recovered);
}

/* ===== ratio_analyze: Calendar Round / Tzolkin ===== */

void test_analyze_calendar_round_tzolkin(void)
{
    ratio_result_t r = ratio_analyze(18980.0, 260.0, 0.01);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 73.0, r.ratio);
    TEST_ASSERT_EQUAL_INT(1, r.is_harmonic);
    TEST_ASSERT_EQUAL_INT(73, r.harmonic_number);
}

/* ===== ratio_analyze: Calendar Round / Haab ===== */

void test_analyze_calendar_round_haab(void)
{
    ratio_result_t r = ratio_analyze(18980.0, 365.0, 0.01);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 52.0, r.ratio);
    TEST_ASSERT_EQUAL_INT(1, r.is_harmonic);
    TEST_ASSERT_EQUAL_INT(52, r.harmonic_number);
}

/* ===== ratio_analyze: scale factor detection ===== */

void test_analyze_mahayuga_scale(void)
{
    ratio_result_t r = ratio_analyze(4320000.0, 432000.0, 0.01);
    TEST_ASSERT_EQUAL_INT(1, r.scale.matched);
    TEST_ASSERT_EQUAL_INT(1, r.scale.power_of_10);
}

/* ===== ratio_analyze: simple fraction ===== */

void test_analyze_simple_fraction(void)
{
    /* 3/2 ratio */
    ratio_result_t r = ratio_analyze(3.0, 2.0, 0.01);
    TEST_ASSERT_EQUAL_INT(3, r.nearest_fraction.numerator);
    TEST_ASSERT_EQUAL_INT(2, r.nearest_fraction.denominator);
    TEST_ASSERT_EQUAL_INT(1, r.is_simple);
}

/* ===== ratio_analyze: inverse ===== */

void test_analyze_inverse(void)
{
    ratio_result_t r = ratio_analyze(260.0, 18980.0, 0.01);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 260.0 / 18980.0, r.ratio);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 18980.0 / 260.0, r.inverse);
}

/* ===== ratio_analyze: constant detection ===== */

void test_analyze_phi_ratio(void)
{
    /* Two values with phi ratio */
    ratio_result_t r = ratio_analyze(1618.0, 1000.0, 0.01);
    TEST_ASSERT_EQUAL_INT(1, r.phi_match.matched);
    TEST_ASSERT_EQUAL_INT(1, r.is_constant);
}

/* ===== ratio_analyze: non-harmonic ===== */

void test_analyze_non_harmonic(void)
{
    ratio_result_t r = ratio_analyze(25772.0, 12.0, 0.01);
    /* 25772 / 12 = 2147.667... not an integer */
    TEST_ASSERT_EQUAL_INT(0, r.is_harmonic);
}

/* ===== main ===== */

int main(void)
{
    UNITY_BEGIN();

    /* ratio_is_integer */
    RUN_TEST(test_is_integer_exact);
    RUN_TEST(test_is_integer_close);
    RUN_TEST(test_is_integer_not_close);
    RUN_TEST(test_is_integer_zero);
    RUN_TEST(test_is_integer_negative);

    /* ratio_nearest_fraction */
    RUN_TEST(test_nearest_fraction_phi);
    RUN_TEST(test_nearest_fraction_exact_half);
    RUN_TEST(test_nearest_fraction_exact_third);
    RUN_TEST(test_nearest_fraction_two);
    RUN_TEST(test_nearest_fraction_three_halves);
    RUN_TEST(test_nearest_fraction_max_denom_1);

    /* constant checks */
    RUN_TEST(test_check_phi_match);
    RUN_TEST(test_check_phi_inverse);
    RUN_TEST(test_check_phi_no_match);
    RUN_TEST(test_check_e_match);
    RUN_TEST(test_check_e_no_match);
    RUN_TEST(test_check_pi_match);
    RUN_TEST(test_check_pi_no_match);
    RUN_TEST(test_check_sqrt2_match);
    RUN_TEST(test_check_sqrt2_no_match);
    RUN_TEST(test_check_sqrt3_match);
    RUN_TEST(test_check_sqrt3_no_match);

    /* scale checks */
    RUN_TEST(test_scale_mahayuga_kaliyuga);
    RUN_TEST(test_scale_hundred);
    RUN_TEST(test_scale_no_match);
    RUN_TEST(test_scale_equal);

    /* harmonic series */
    RUN_TEST(test_harmonic_series_440);
    RUN_TEST(test_harmonic_series_clamp);
    RUN_TEST(test_harmonic_series_zero_count);
    RUN_TEST(test_subharmonic_series);
    RUN_TEST(test_subharmonic_series_clamp);

    /* continued fractions */
    RUN_TEST(test_cf_phi);
    RUN_TEST(test_cf_pi);
    RUN_TEST(test_cf_e);
    RUN_TEST(test_cf_integer);
    RUN_TEST(test_cf_simple_fraction);
    RUN_TEST(test_from_cf_phi);
    RUN_TEST(test_from_cf_pi);
    RUN_TEST(test_from_cf_single);
    RUN_TEST(test_from_cf_roundtrip);

    /* ratio_analyze integration */
    RUN_TEST(test_analyze_calendar_round_tzolkin);
    RUN_TEST(test_analyze_calendar_round_haab);
    RUN_TEST(test_analyze_mahayuga_scale);
    RUN_TEST(test_analyze_simple_fraction);
    RUN_TEST(test_analyze_inverse);
    RUN_TEST(test_analyze_phi_ratio);
    RUN_TEST(test_analyze_non_harmonic);

    return UNITY_END();
}
