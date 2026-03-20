/* time_entropy.c — Information-theoretic time analysis implementation.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#include "time_entropy.h"
#include <math.h>

double te_binary_entropy(int sig_count, int total)
{
    if (total <= 0) return 0.0;
    if (sig_count <= 0 || sig_count >= total) return 0.0;

    double p = (double)sig_count / (double)total;
    double q = 1.0 - p;

    /* H = -p*log2(p) - q*log2(q) */
    return -(p * log2(p)) - (q * log2(q));
}

double te_entropy(double jd, te_result_t *out)
{
    /* Get significance ratio from convergence detector */
    double sig_ratio = cd_significance(jd);
    int total = CD_SYS_COUNT;

    /* Derive significant count: round(ratio * total) */
    int sig_count = (int)(sig_ratio * (double)total + 0.5);
    if (sig_count < 0) sig_count = 0;
    if (sig_count > total) sig_count = total;

    double h = te_binary_entropy(sig_count, total);
    double h_max = log2((double)total);
    double norm = (h_max > 0.0) ? h / h_max : 0.0;

    if (out) {
        out->entropy = h;
        out->max_entropy = h_max;
        out->normalized = norm;
        out->significant_count = sig_count;
        out->total_systems = total;
    }

    return h;
}

const char *te_interpret(double normalized_entropy)
{
    if (normalized_entropy < 0.15) return "ordered";
    if (normalized_entropy < 0.35) return "structured";
    if (normalized_entropy < 0.65) return "balanced";
    return "complex";
}
