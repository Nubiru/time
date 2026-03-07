/* ratio_analysis.c — Pure number theory for ratio analysis between cycle lengths.
 *
 * All functions are pure: no globals, no malloc, no side effects. */

#include "ratio_analysis.h"
#include <math.h>

/* Internal constant values for matching */
static const double SQRT2 = 1.41421356237309504880;
static const double SQRT3 = 1.73205080756887729353;

/* ===== Helper: check value against a constant (direct + inverse) ===== */

static ratio_constant_match_t check_constant(double value, double constant,
                                              const char *name, double tolerance)
{
    ratio_constant_match_t result;
    result.name = name;
    result.value = constant;

    double err_direct = fabs(value - constant);
    double err_inverse = fabs(value - 1.0 / constant);

    if (err_direct <= err_inverse) {
        result.error = err_direct;
    } else {
        result.error = err_inverse;
    }

    result.matched = (result.error < tolerance) ? 1 : 0;
    return result;
}

/* ===== ratio_is_integer ===== */

int ratio_is_integer(double value, double tolerance)
{
    return (fabs(value - round(value)) < tolerance) ? 1 : 0;
}

/* ===== ratio_nearest_fraction ===== */

ratio_fraction_t ratio_nearest_fraction(double value, int max_denom)
{
    ratio_fraction_t best;
    best.numerator = (int)round(value);
    best.denominator = 1;
    best.value = (double)best.numerator;
    best.error = fabs(value - best.value);

    if (max_denom < 1) {
        max_denom = 1;
    }
    if (max_denom > RATIO_MAX_DENOM) {
        max_denom = RATIO_MAX_DENOM;
    }

    for (int d = 1; d <= max_denom; d++) {
        int n = (int)round(value * d);
        double frac_val = (double)n / (double)d;
        double err = fabs(value - frac_val);

        if (err < best.error) {
            best.numerator = n;
            best.denominator = d;
            best.value = frac_val;
            best.error = err;
        }
    }

    return best;
}

/* ===== Constant checks ===== */

ratio_constant_match_t ratio_check_phi(double value, double tolerance)
{
    return check_constant(value, PHI, "phi", tolerance);
}

ratio_constant_match_t ratio_check_e(double value, double tolerance)
{
    return check_constant(value, EULER_E, "e", tolerance);
}

ratio_constant_match_t ratio_check_pi(double value, double tolerance)
{
    return check_constant(value, PI, "pi", tolerance);
}

ratio_constant_match_t ratio_check_sqrt2(double value, double tolerance)
{
    return check_constant(value, SQRT2, "sqrt2", tolerance);
}

ratio_constant_match_t ratio_check_sqrt3(double value, double tolerance)
{
    return check_constant(value, SQRT3, "sqrt3", tolerance);
}

/* ===== ratio_check_scale ===== */

ratio_scale_t ratio_check_scale(double a, double b, double tolerance)
{
    ratio_scale_t result;
    result.power_of_10 = 0;
    result.base_ratio = 1.0;
    result.matched = 0;

    if (b == 0.0 || a == 0.0) {
        return result;
    }

    double ratio = a / b;
    if (ratio <= 0.0) {
        return result;
    }

    double log_ratio = log10(ratio);
    int power = (int)round(log_ratio);

    double scale = pow(10.0, (double)power);
    double base = a / (b * scale);

    result.power_of_10 = power;
    result.base_ratio = base;
    result.matched = (fabs(base - 1.0) < tolerance) ? 1 : 0;

    return result;
}

/* ===== ratio_harmonic_series ===== */

int ratio_harmonic_series(double fundamental, int count, double *out_harmonics)
{
    if (count < 0) {
        count = 0;
    }
    if (count > RATIO_MAX_HARMONICS) {
        count = RATIO_MAX_HARMONICS;
    }

    for (int i = 0; i < count; i++) {
        out_harmonics[i] = fundamental * (double)(i + 1);
    }

    return count;
}

/* ===== ratio_subharmonic_series ===== */

int ratio_subharmonic_series(double fundamental, int count, double *out_subharmonics)
{
    if (count < 0) {
        count = 0;
    }
    if (count > RATIO_MAX_HARMONICS) {
        count = RATIO_MAX_HARMONICS;
    }

    for (int i = 0; i < count; i++) {
        out_subharmonics[i] = fundamental / (double)(i + 1);
    }

    return count;
}

/* ===== ratio_continued_fraction ===== */

int ratio_continued_fraction(double value, int max_terms, int *out_coefficients)
{
    if (max_terms < 1) {
        return 0;
    }
    if (max_terms > RATIO_MAX_CF_TERMS) {
        max_terms = RATIO_MAX_CF_TERMS;
    }

    double x = value;
    int count = 0;

    for (int i = 0; i < max_terms; i++) {
        int a = (int)floor(x);
        out_coefficients[count] = a;
        count++;

        double remainder = x - (double)a;
        if (fabs(remainder) < 1e-10) {
            break;
        }

        x = 1.0 / remainder;

        /* Guard against overflow / runaway */
        if (x > 1e12) {
            break;
        }
    }

    return count;
}

/* ===== ratio_from_continued_fraction ===== */

double ratio_from_continued_fraction(const int *coefficients, int count)
{
    if (count <= 0) {
        return 0.0;
    }

    /* Fold from right: start with last term, work back */
    double result = (double)coefficients[count - 1];

    for (int i = count - 2; i >= 0; i--) {
        result = (double)coefficients[i] + 1.0 / result;
    }

    return result;
}

/* ===== ratio_analyze ===== */

ratio_result_t ratio_analyze(double cycle_a, double cycle_b, double tolerance)
{
    ratio_result_t r;

    /* Basic ratio */
    r.ratio = (cycle_b != 0.0) ? cycle_a / cycle_b : 0.0;
    r.inverse = (cycle_a != 0.0) ? cycle_b / cycle_a : 0.0;

    /* Nearest fraction */
    r.nearest_fraction = ratio_nearest_fraction(r.ratio, RATIO_MAX_DENOM);
    r.is_simple = (r.nearest_fraction.error < tolerance) ? 1 : 0;

    /* Constant matching */
    r.phi_match = ratio_check_phi(r.ratio, tolerance);
    r.e_match = ratio_check_e(r.ratio, tolerance);
    r.pi_match = ratio_check_pi(r.ratio, tolerance);
    r.sqrt2_match = ratio_check_sqrt2(r.ratio, tolerance);
    r.sqrt3_match = ratio_check_sqrt3(r.ratio, tolerance);
    r.is_constant = (r.phi_match.matched || r.e_match.matched ||
                     r.pi_match.matched || r.sqrt2_match.matched ||
                     r.sqrt3_match.matched) ? 1 : 0;

    /* Scale factor */
    r.scale = ratio_check_scale(cycle_a, cycle_b, tolerance);

    /* Harmonic (integer) test */
    r.is_harmonic = ratio_is_integer(r.ratio, tolerance);
    r.harmonic_number = (int)round(r.ratio);

    return r;
}
