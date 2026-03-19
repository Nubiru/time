/* brain_predict.c — Future event feed implementation.
 *
 * Scans upcoming Julian Days using cd_significance() to find days
 * with notable convergence activity. Collects the top events and
 * sorts by significance.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#include "brain_predict.h"
#include "convergence_detect.h"
#include <string.h>
#include <stdio.h>

/* Minimum significance threshold to consider an event worth reporting */
#define MIN_REPORT_SIG 0.05

int br_predict(double jd, int days_ahead, br_prediction_t *out) {
    if (!out) return 0;
    memset(out, 0, sizeof(*out));
    out->start_jd = jd;

    if (days_ahead <= 0) return 0;
    if (days_ahead > BR_PREDICT_MAX_DAYS)
        days_ahead = BR_PREDICT_MAX_DAYS;
    out->days_scanned = days_ahead;

    /* Scan each day, collect events above threshold */
    for (int d = 1; d <= days_ahead; d++) {
        double test_jd = jd + (double)d;
        double sig = cd_significance(test_jd);

        if (sig < MIN_REPORT_SIG) continue;

        /* Find insertion point (sorted by significance, descending) */
        int insert_at = out->count;
        for (int i = 0; i < out->count; i++) {
            if (sig > out->events[i].significance) {
                insert_at = i;
                break;
            }
        }

        /* If we have room, or this event is more significant than the last */
        if (out->count < BR_PREDICT_MAX || insert_at < BR_PREDICT_MAX) {
            /* Shift lower entries down */
            int end = out->count < BR_PREDICT_MAX ? out->count : BR_PREDICT_MAX - 1;
            for (int i = end; i > insert_at; i--) {
                if (i < BR_PREDICT_MAX) {
                    out->events[i] = out->events[i - 1];
                }
            }

            /* Insert */
            if (insert_at < BR_PREDICT_MAX) {
                br_predicted_event_t *e = &out->events[insert_at];
                e->jd = test_jd;
                e->days_from_now = d;
                e->significance = sig;

                /* Get convergence description for headline */
                cd_result_t cd = cd_scan(test_jd);
                if (cd.count > 0 && cd.events[0].description) {
                    snprintf(e->headline, BR_HEADLINE_MAX,
                             "In %d days: %s", d, cd.events[0].description);
                } else {
                    snprintf(e->headline, BR_HEADLINE_MAX,
                             "In %d days: convergence (%.0f%% significance)",
                             d, sig * 100.0);
                }

                if (out->count < BR_PREDICT_MAX) out->count++;
            }
        }
    }

    return out->count;
}

double br_predict_next(double jd, int days_ahead, double min_sig) {
    if (days_ahead <= 0) return 0.0;
    if (days_ahead > BR_PREDICT_MAX_DAYS)
        days_ahead = BR_PREDICT_MAX_DAYS;

    for (int d = 1; d <= days_ahead; d++) {
        double test_jd = jd + (double)d;
        double sig = cd_significance(test_jd);
        if (sig >= min_sig) return test_jd;
    }
    return 0.0;
}
