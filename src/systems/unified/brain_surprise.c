/* brain_surprise.c — Anomaly detector implementation.
 *
 * Samples convergence significance across a window of days to build
 * a statistical baseline (mean + stddev), then compares today's
 * significance to detect anomalies (z-score).
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#include "brain_surprise.h"
#include "convergence_detect.h"
#include <math.h>
#include <string.h>
#include <stdio.h>

void br_surprise_baseline(double center_jd, int sample_days,
                          int sample_step,
                          double *mean, double *stddev) {
    if (!mean || !stddev) return;
    if (sample_days <= 0) sample_days = BR_SURPRISE_SAMPLE_DAYS;
    if (sample_step <= 0) sample_step = BR_SURPRISE_SAMPLE_STEP;

    double sum = 0.0;
    double sum_sq = 0.0;
    int count = 0;

    for (int d = -sample_days; d <= sample_days; d += sample_step) {
        if (d == 0) continue; /* skip the center day itself */
        double sig = cd_significance(center_jd + (double)d);
        sum += sig;
        sum_sq += sig * sig;
        count++;
    }

    if (count == 0) {
        *mean = 0.0;
        *stddev = 0.0;
        return;
    }

    *mean = sum / (double)count;
    double variance = (sum_sq / (double)count) - (*mean * *mean);
    *stddev = (variance > 0.0) ? sqrt(variance) : 0.0;
}

int br_surprise_detect(double jd, double z_threshold,
                       br_surprise_t *out) {
    if (!out) return 0;
    memset(out, 0, sizeof(*out));

    /* Get today's significance */
    out->significance = cd_significance(jd);

    /* Compute baseline */
    br_surprise_baseline(jd, BR_SURPRISE_SAMPLE_DAYS,
                         BR_SURPRISE_SAMPLE_STEP,
                         &out->baseline_mean, &out->baseline_stddev);

    /* Compute z-score */
    if (out->baseline_stddev > 0.001) {
        out->z_score = (out->significance - out->baseline_mean)
                       / out->baseline_stddev;
    } else {
        /* All days have similar significance — no surprise possible
         * unless today is significantly above zero */
        out->z_score = (out->significance > out->baseline_mean + 0.01)
                       ? 2.0 : 0.0;
    }

    out->is_surprise = (out->z_score >= z_threshold) ? 1 : 0;

    /* Generate headline */
    if (out->is_surprise) {
        if (out->z_score >= 3.0) {
            snprintf(out->headline, BR_HEADLINE_MAX,
                     "Rare convergence day (%.1f sigma above average)",
                     out->z_score);
        } else {
            snprintf(out->headline, BR_HEADLINE_MAX,
                     "Notable convergence day (%.1f sigma above average)",
                     out->z_score);
        }
    } else {
        snprintf(out->headline, BR_HEADLINE_MAX,
                 "Average convergence activity");
    }

    return out->is_surprise;
}

int br_surprise_check(double jd) {
    br_surprise_t result;
    return br_surprise_detect(jd, 2.0, &result);
}
