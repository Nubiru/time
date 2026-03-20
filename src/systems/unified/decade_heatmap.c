/* decade_heatmap.c — Convergence heatmap data implementation (S88).
 *
 * Uses br_archive_scan for per-year convergence scanning, then
 * compacts results into a grid structure for decade visualization.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#include "decade_heatmap.h"
#include "../../math/julian.h"
#include <string.h>

/* Check if a Gregorian year is a leap year */
static int is_leap(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int dh_compute(int start_year, int year_count, dh_heatmap_t *out) {
    if (!out) return 0;
    memset(out, 0, sizeof(*out));

    if (year_count <= 0) return 0;
    if (year_count > DH_MAX_YEARS) year_count = DH_MAX_YEARS;

    out->start_year = start_year;
    out->year_count = year_count;

    int best_total = 0;
    int global_max = 0;

    for (int y = 0; y < year_count; y++) {
        int year = start_year + y;
        int days = is_leap(year) ? 366 : 365;
        out->days_in_year[y] = days;

        double jan1 = gregorian_to_jd(year, 1, 1.0);

        /* Scan in chunks of BR_ARCHIVE_MAX_DAYS (400) */
        br_archive_result_t ar;
        br_archive_scan(jan1, days, &ar);

        int year_total = 0;
        for (int d = 0; d < ar.count && d < days; d++) {
            int sc = ar.days[d].system_count;
            out->grid[y][d] = sc;
            year_total += sc;

            if (sc > global_max) {
                global_max = sc;
                out->best_day_year = y;
                out->best_day_doy = d;
                out->max_system_count = sc;
            }
        }

        if (year_total > best_total) {
            best_total = year_total;
            out->best_year_index = y;
        }
    }

    return year_count;
}

int dh_row_total(const dh_heatmap_t *map, int year_index) {
    if (!map || year_index < 0 || year_index >= map->year_count) return 0;

    int total = 0;
    for (int d = 0; d < map->days_in_year[year_index]; d++) {
        total += map->grid[year_index][d];
    }
    return total;
}

int dh_col_max(const dh_heatmap_t *map, int day_of_year) {
    if (!map || day_of_year < 0 || day_of_year >= DH_MAX_DAYS) return 0;

    int max = 0;
    for (int y = 0; y < map->year_count; y++) {
        if (day_of_year < map->days_in_year[y]) {
            if (map->grid[y][day_of_year] > max) {
                max = map->grid[y][day_of_year];
            }
        }
    }
    return max;
}

double dh_year_average(const dh_heatmap_t *map, int year_index) {
    if (!map || year_index < 0 || year_index >= map->year_count) return 0.0;
    if (map->days_in_year[year_index] == 0) return 0.0;

    int total = dh_row_total(map, year_index);
    return (double)total / (double)map->days_in_year[year_index];
}

int dh_year_summary(const dh_heatmap_t *map, int year_index,
                    dh_year_summary_t *out) {
    if (!map || !out || year_index < 0 || year_index >= map->year_count) return 0;
    memset(out, 0, sizeof(*out));

    out->year = map->start_year + year_index;
    int days = map->days_in_year[year_index];

    for (int d = 0; d < days; d++) {
        int sc = map->grid[year_index][d];
        out->total_convergence += sc;
        if (sc >= 3) out->notable_days++;
        if (sc > out->max_day_count) out->max_day_count = sc;
    }

    out->average = (days > 0) ?
        (double)out->total_convergence / (double)days : 0.0;

    return 1;
}
