/* ratio_analysis.h — Pure number theory for ratio analysis between cycle lengths.
 *
 * Detects when a ratio approximates simple fractions (2/1, 3/2, 4/3, etc.),
 * mathematical constants (phi, e, pi, sqrt2, sqrt3), or scale factors
 * (powers of 10). Generates harmonic/subharmonic series. Computes continued
 * fraction decomposition and reconstruction.
 *
 * Extends cycle_analysis with deeper ratio investigation for the Unified
 * Sacred Cycles correlation dashboard.
 *
 * All functions are pure: no globals, no malloc, no side effects. */

#ifndef TIME_RATIO_ANALYSIS_H
#define TIME_RATIO_ANALYSIS_H

#define PI 3.14159265358979323846
#define PHI 1.61803398874989484820
#define EULER_E 2.71828182845904523536

/* Maximum fraction denominator to search */
#define RATIO_MAX_DENOM 32

/* Maximum harmonic count */
#define RATIO_MAX_HARMONICS 16

/* Maximum continued fraction terms */
#define RATIO_MAX_CF_TERMS 32

/* Fraction representation */
typedef struct {
    int numerator;
    int denominator;
    double value;          /* numerator / denominator as double */
    double error;          /* absolute error from target ratio */
} ratio_fraction_t;

/* Mathematical constant match */
typedef struct {
    const char *name;      /* "phi", "e", "pi", "sqrt2", "sqrt3" */
    double value;          /* the constant's value */
    double error;          /* absolute error from target ratio */
    int matched;           /* 1 if within tolerance */
} ratio_constant_match_t;

/* Scale factor result */
typedef struct {
    int power_of_10;       /* exponent: a = b * 10^n */
    double base_ratio;     /* a / (b * 10^n) — should be ~1.0 if match */
    int matched;           /* 1 if base_ratio within tolerance of 1.0 */
} ratio_scale_t;

/* Full ratio analysis result */
typedef struct {
    double ratio;                    /* a / b */
    double inverse;                  /* b / a */
    ratio_fraction_t nearest_fraction;  /* best simple fraction approximation */
    int is_simple;                   /* 1 if nearest fraction error < tolerance */
    ratio_constant_match_t phi_match;
    ratio_constant_match_t e_match;
    ratio_constant_match_t pi_match;
    ratio_constant_match_t sqrt2_match;
    ratio_constant_match_t sqrt3_match;
    int is_constant;                /* 1 if any constant matched */
    ratio_scale_t scale;            /* power-of-10 relationship */
    int is_harmonic;                /* 1 if ratio is near integer (within tolerance) */
    int harmonic_number;            /* nearest integer if is_harmonic */
} ratio_result_t;

/* Analyze the ratio between two cycle lengths.
 * tolerance: maximum absolute error for match detection (e.g., 0.01).
 * Returns full analysis result. */
ratio_result_t ratio_analyze(double cycle_a, double cycle_b, double tolerance);

/* Find the nearest simple fraction to a value.
 * Searches all fractions with denominator <= max_denom.
 * Returns best match. */
ratio_fraction_t ratio_nearest_fraction(double value, int max_denom);

/* Check if value matches a mathematical constant within tolerance. */
ratio_constant_match_t ratio_check_phi(double value, double tolerance);
ratio_constant_match_t ratio_check_e(double value, double tolerance);
ratio_constant_match_t ratio_check_pi(double value, double tolerance);
ratio_constant_match_t ratio_check_sqrt2(double value, double tolerance);
ratio_constant_match_t ratio_check_sqrt3(double value, double tolerance);

/* Check if a = b * 10^n for some integer n.
 * tolerance: how close base_ratio must be to 1.0. */
ratio_scale_t ratio_check_scale(double a, double b, double tolerance);

/* Generate harmonic overtone series.
 * fundamental: base frequency/period.
 * count: number of harmonics to generate (1 to RATIO_MAX_HARMONICS).
 * out_harmonics: output array of harmonic values.
 * Returns number of harmonics written. */
int ratio_harmonic_series(double fundamental, int count, double *out_harmonics);

/* Generate subharmonic series (undertones).
 * fundamental: base frequency/period.
 * count: number of subharmonics (1 to RATIO_MAX_HARMONICS).
 * out_subharmonics: output array.
 * Returns number written. */
int ratio_subharmonic_series(double fundamental, int count, double *out_subharmonics);

/* Is value close to an integer? Returns 1 if fabs(value - round(value)) < tolerance. */
int ratio_is_integer(double value, double tolerance);

/* Continued fraction coefficients of a value.
 * Fills out_coefficients with up to max_terms terms.
 * Returns number of terms written. */
int ratio_continued_fraction(double value, int max_terms, int *out_coefficients);

/* Reconstruct value from continued fraction coefficients.
 * Returns the fraction as a double. */
double ratio_from_continued_fraction(const int *coefficients, int count);

#endif /* TIME_RATIO_ANALYSIS_H */
