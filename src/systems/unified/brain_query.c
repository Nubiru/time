/* brain_query.c — Compound question engine implementation.
 *
 * Scans a date range, checking each day for convergence events
 * that involve the queried systems. AND queries require all systems
 * to be active; OR queries require at least one.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#include "brain_query.h"
#include <string.h>
#include <stdio.h>

/* Check if a cd_result_t contains activity for a specific system */
static int system_active_in(const cd_result_t *cd, cd_system_t sys) {
    for (int e = 0; e < cd->count; e++) {
        for (int s = 0; s < cd->events[e].system_count; s++) {
            if (cd->events[e].systems[s] == sys) return 1;
        }
    }
    return 0;
}

/* Check if a day matches the query */
static int matches_query(const br_query_t *query, const cd_result_t *cd,
                         int *matched_count) {
    int matched = 0;
    for (int i = 0; i < query->system_count; i++) {
        if (system_active_in(cd, query->systems[i])) {
            matched++;
        }
    }

    if (matched_count) *matched_count = matched;

    if (query->op == BR_QUERY_AND) {
        return matched == query->system_count;
    } else { /* OR */
        return matched > 0;
    }
}

br_query_t br_query_and(const cd_system_t *systems, int count) {
    br_query_t q;
    memset(&q, 0, sizeof(q));
    q.op = BR_QUERY_AND;
    q.min_significance = 0.0;
    if (count > BR_QUERY_MAX_SYSTEMS) count = BR_QUERY_MAX_SYSTEMS;
    for (int i = 0; i < count; i++) {
        q.systems[i] = systems[i];
    }
    q.system_count = count;
    return q;
}

br_query_t br_query_or(const cd_system_t *systems, int count) {
    br_query_t q = br_query_and(systems, count);
    q.op = BR_QUERY_OR;
    return q;
}

int br_query_search(const br_query_t *query, double start_jd,
                    int range_days, br_query_result_t *out) {
    if (!query || !out) return 0;
    memset(out, 0, sizeof(*out));

    if (query->system_count <= 0) return 0;
    if (range_days == 0) return 0;

    int step = (range_days > 0) ? 1 : -1;
    int abs_range = range_days > 0 ? range_days : -range_days;
    if (abs_range > BR_QUERY_MAX_RANGE) abs_range = BR_QUERY_MAX_RANGE;
    out->days_scanned = abs_range;

    for (int d = 1; d <= abs_range && out->count < BR_QUERY_MAX_RESULTS; d++) {
        double test_jd = start_jd + (double)(d * step);
        cd_result_t cd = cd_scan(test_jd);

        int matched_count = 0;
        if (matches_query(query, &cd, &matched_count)) {
            double sig = cd_significance(test_jd);
            if (sig < query->min_significance) continue;

            br_query_match_t *m = &out->matches[out->count];
            m->jd = test_jd;
            m->significance = sig;
            m->matched_count = matched_count;

            /* Build headline listing matched systems */
            int written = 0;
            int remaining = BR_HEADLINE_MAX;
            for (int i = 0; i < query->system_count && remaining > 2; i++) {
                if (system_active_in(&cd, query->systems[i])) {
                    const char *name = cd_system_name(query->systems[i]);
                    int n;
                    if (written > 0) {
                        n = snprintf(m->headline + written,
                                     (size_t)remaining, " + %s", name);
                    } else {
                        n = snprintf(m->headline + written,
                                     (size_t)remaining, "%s", name);
                    }
                    if (n > 0) {
                        int u = n < remaining ? n : remaining - 1;
                        written += u;
                        remaining -= u;
                    }
                }
            }

            out->count++;
        }
    }

    return out->count;
}
