/* brain_stats.c — Statistical convergence analysis implementation.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#include "brain_stats.h"
#include "../../math/julian.h"
#include <string.h>

int br_stats_percentile(double jd, br_stats_percentile_t *out) {
    if (!out) return 0;
    memset(out, 0, sizeof(*out));

    /* Determine the year */
    int year, month;
    jd_to_gregorian(jd, &year, &month);

    /* Scan the full year */
    double jan1 = gregorian_to_jd(year, 1, 1.0);
    br_archive_result_t ar;
    br_archive_scan(jan1, 365, &ar);

    /* Find today's system count */
    int today_count = 0;
    int today_idx = (int)(jd - jan1 + 0.5);
    if (today_idx >= 0 && today_idx < ar.count) {
        today_count = ar.days[today_idx].system_count;
    } else {
        /* jd outside year range — compute directly */
        cd_result_t cd = cd_scan(jd);
        today_count = (cd.count > 0) ? cd.events[0].system_count : 0;
    }

    /* Count days with higher convergence */
    int higher = 0;
    for (int i = 0; i < ar.count; i++) {
        if (ar.days[i].system_count > today_count) higher++;
    }

    out->jd = jd;
    out->system_count = today_count;
    out->rank = higher + 1;
    out->total_days = ar.count;
    out->percentile = (double)(higher + 1) / (double)ar.count * 100.0;

    return 1;
}

int br_stats_year_score(int year, br_stats_year_score_t *out) {
    if (!out) return 0;
    memset(out, 0, sizeof(*out));

    double jan1 = gregorian_to_jd(year, 1, 1.0);
    br_archive_result_t ar;
    br_archive_scan(jan1, 365, &ar);

    out->year = year;
    for (int i = 0; i < ar.count; i++) {
        int sc = ar.days[i].system_count;
        out->total_convergence += sc;
        if (sc >= 3) out->notable_days++;
        if (sc >= 5) out->rare_days++;
        if (sc > out->max_systems) out->max_systems = sc;
    }

    return 1;
}

int br_stats_year_compare(int start_year, int count,
                          br_stats_year_compare_t *out) {
    if (!out) return 0;
    memset(out, 0, sizeof(*out));

    if (count <= 0) return 0;
    if (count > BR_STATS_MAX_YEARS) count = BR_STATS_MAX_YEARS;

    int best = 0, worst = 0;

    for (int i = 0; i < count; i++) {
        br_stats_year_score(start_year + i, &out->years[i]);
        if (out->years[i].total_convergence > out->years[best].total_convergence)
            best = i;
        if (out->years[i].total_convergence < out->years[worst].total_convergence)
            worst = i;
    }

    out->count = count;
    out->best_year_index = best;
    out->worst_year_index = worst;

    return count;
}

double br_stats_signature_last(double jd, const int *systems,
                               int system_count, int max_days) {
    if (!systems || system_count <= 0 || max_days <= 0) return 0.0;

    for (int d = 1; d <= max_days; d++) {
        double test_jd = jd - (double)d;
        cd_result_t cd = cd_scan(test_jd);

        if (cd.count == 0) continue;

        /* Check if all requested systems are in the convergence */
        int matched = 0;
        for (int s = 0; s < system_count; s++) {
            for (int e = 0; e < cd.events[0].system_count; e++) {
                if ((int)cd.events[0].systems[e] == systems[s]) {
                    matched++;
                    break;
                }
            }
        }

        if (matched == system_count) return test_jd;
    }

    return 0.0;
}

double br_stats_signature_next(double jd, const int *systems,
                               int system_count, int max_days) {
    if (!systems || system_count <= 0 || max_days <= 0) return 0.0;

    for (int d = 1; d <= max_days; d++) {
        double test_jd = jd + (double)d;
        cd_result_t cd = cd_scan(test_jd);

        if (cd.count == 0) continue;

        int matched = 0;
        for (int s = 0; s < system_count; s++) {
            for (int e = 0; e < cd.events[0].system_count; e++) {
                if ((int)cd.events[0].systems[e] == systems[s]) {
                    matched++;
                    break;
                }
            }
        }

        if (matched == system_count) return test_jd;
    }

    return 0.0;
}
