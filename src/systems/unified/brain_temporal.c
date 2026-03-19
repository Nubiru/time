/* brain_temporal.c — Temporal context implementation.
 *
 * Scans Julian Days forward and backward to find when convergence
 * patterns of similar strength occur. Uses cd_significance() from
 * convergence_detect as the metric.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#include "brain_temporal.h"
#include "convergence_detect.h"

double br_temporal_last(double jd, double min_strength, int range_days) {
    if (range_days <= 0) return 0.0;
    if (range_days > BR_TEMPORAL_MAX_RANGE)
        range_days = BR_TEMPORAL_MAX_RANGE;

    for (int d = 1; d <= range_days; d += BR_TEMPORAL_STEP) {
        double test_jd = jd - (double)d;
        double sig = cd_significance(test_jd);
        if (sig >= min_strength) {
            return test_jd;
        }
    }
    return 0.0;
}

double br_temporal_next(double jd, double min_strength, int range_days) {
    if (range_days <= 0) return 0.0;
    if (range_days > BR_TEMPORAL_MAX_RANGE)
        range_days = BR_TEMPORAL_MAX_RANGE;

    for (int d = 1; d <= range_days; d += BR_TEMPORAL_STEP) {
        double test_jd = jd + (double)d;
        double sig = cd_significance(test_jd);
        if (sig >= min_strength) {
            return test_jd;
        }
    }
    return 0.0;
}

double br_temporal_recurrence(double jd, double min_strength, int range_days) {
    if (range_days <= 0) return 0.0;
    if (range_days > BR_TEMPORAL_MAX_RANGE)
        range_days = BR_TEMPORAL_MAX_RANGE;

    /* Count occurrences in the scan range (backward only for stable average) */
    int count = 0;
    double first_jd = 0.0;
    double last_found_jd = 0.0;

    for (int d = 1; d <= range_days; d += BR_TEMPORAL_STEP) {
        double test_jd = jd - (double)d;
        double sig = cd_significance(test_jd);
        if (sig >= min_strength) {
            count++;
            last_found_jd = test_jd;
            if (first_jd == 0.0) first_jd = test_jd;
        }
    }

    if (count < 2) return 0.0;

    /* Average gap between occurrences */
    double span = first_jd - last_found_jd; /* positive: first is closer to jd */
    if (span <= 0.0) return 0.0;
    return span / (double)(count - 1);
}

br_temporal_t br_temporal_analyze(double jd, double min_strength,
                                  int range_days) {
    br_temporal_t result;
    result.scan_range = range_days;

    if (range_days <= 0 || range_days > BR_TEMPORAL_MAX_RANGE) {
        range_days = BR_TEMPORAL_DEFAULT_RANGE;
        result.scan_range = range_days;
    }

    result.last_jd = br_temporal_last(jd, min_strength, range_days);
    result.next_jd = br_temporal_next(jd, min_strength, range_days);
    result.recurrence_days = br_temporal_recurrence(jd, min_strength, range_days);

    /* Count total occurrences */
    int count = 0;
    for (int d = 1; d <= range_days; d += BR_TEMPORAL_STEP) {
        if (cd_significance(jd - (double)d) >= min_strength) count++;
        if (cd_significance(jd + (double)d) >= min_strength) count++;
    }
    result.occurrences = count;

    return result;
}
