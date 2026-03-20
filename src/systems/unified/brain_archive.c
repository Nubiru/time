/* brain_archive.c — Convergence archive scanner implementation.
 *
 * Scans date ranges using cd_scan/cd_significance for each day,
 * aggregates results into an archive structure.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#include "brain_archive.h"
#include <string.h>

int br_archive_scan(double start_jd, int days, br_archive_result_t *out) {
    if (!out) return 0;
    memset(out, 0, sizeof(*out));
    out->start_jd = start_jd;

    if (days <= 0) return 0;
    if (days > BR_ARCHIVE_MAX_DAYS) days = BR_ARCHIVE_MAX_DAYS;

    int max_sys = 0;
    double best_jd = start_jd;

    for (int d = 0; d < days; d++) {
        double jd = start_jd + (double)d;
        cd_result_t cd = cd_scan(jd);
        br_archive_day_t *day = &out->days[d];

        day->jd = jd;

        if (cd.count > 0) {
            const cd_event_t *ev = &cd.events[0];
            day->system_count = ev->system_count;
            day->strength = ev->strength;
            day->significance = ev->significance;

            int tc = ev->system_count;
            if (tc > 8) tc = 8;
            for (int s = 0; s < tc; s++) {
                day->top_systems[s] = (int)ev->systems[s];
            }
            day->top_system_count = tc;
        } else {
            day->system_count = 0;
            day->strength = 0;
            day->significance = 0.0;
            day->top_system_count = 0;
        }

        if (day->system_count >= 3) out->notable_count++;
        if (day->system_count > max_sys) {
            max_sys = day->system_count;
            best_jd = jd;
        }
    }

    out->count = days;
    out->days_scanned = days;
    out->max_systems = max_sys;
    out->best_jd = best_jd;

    return days;
}

int br_archive_notable(const br_archive_result_t *archive,
                       int min_systems,
                       br_archive_day_t *out, int max_out) {
    if (!archive || !out || max_out <= 0) return 0;

    int found = 0;
    for (int i = 0; i < archive->count && found < max_out; i++) {
        if (archive->days[i].system_count >= min_systems) {
            out[found++] = archive->days[i];
        }
    }
    return found;
}

const br_archive_day_t *br_archive_best(const br_archive_result_t *archive) {
    if (!archive || archive->count == 0) return NULL;

    int best_idx = 0;
    int best_count = 0;
    for (int i = 0; i < archive->count; i++) {
        if (archive->days[i].system_count > best_count) {
            best_count = archive->days[i].system_count;
            best_idx = i;
        }
    }
    return &archive->days[best_idx];
}

int br_archive_count_above(const br_archive_result_t *archive,
                           int min_systems) {
    if (!archive) return 0;
    int count = 0;
    for (int i = 0; i < archive->count; i++) {
        if (archive->days[i].system_count >= min_systems) {
            count++;
        }
    }
    return count;
}

void br_archive_year_stats(const br_archive_result_t *archive,
                           int counts[5]) {
    for (int t = 0; t < 5; t++) counts[t] = 0;
    if (!archive) return;

    for (int i = 0; i < archive->count; i++) {
        int sc = archive->days[i].system_count;
        if (sc >= 2) counts[0]++;
        if (sc >= 3) counts[1]++;
        if (sc >= 4) counts[2]++;
        if (sc >= 5) counts[3]++;
        if (sc >= 6) counts[4]++;
    }
}
