/* time_entropy.h — Information-theoretic time analysis (Track 26, S101)
 *
 * Shannon entropy of today's binary significance vector across
 * 21 convergence detector systems. Measures how "structured" or
 * "random" the day's pattern is.
 *
 * Low entropy = most systems agree (all significant or all quiet)
 * High entropy = mixed (unusual pattern — half significant, half not)
 *
 * Pure functions: no GL, no malloc, no globals. */

#ifndef TIME_TIME_ENTROPY_H
#define TIME_TIME_ENTROPY_H

#include "convergence_detect.h"

/* Entropy result */
typedef struct {
    double  entropy;            /* Shannon entropy in bits (0.0 to ~4.4) */
    double  max_entropy;        /* maximum possible entropy (log2(21)) */
    double  normalized;         /* entropy / max_entropy (0.0 to 1.0) */
    int     significant_count;  /* how many systems are significant */
    int     total_systems;      /* always CD_SYS_COUNT */
} te_result_t;

/* Compute entropy for a Julian Day.
 * Returns entropy in bits. Fills out if non-NULL. */
double te_entropy(double jd, te_result_t *out);

/* Compute entropy from a pre-computed significance count.
 * Pure math function: H = -p*log2(p) - (1-p)*log2(1-p)
 * where p = sig_count / total. */
double te_binary_entropy(int sig_count, int total);

/* Interpret the entropy value as a category string.
 * Returns static string: "ordered" / "structured" / "balanced" / "complex" */
const char *te_interpret(double normalized_entropy);

#endif /* TIME_TIME_ENTROPY_H */
