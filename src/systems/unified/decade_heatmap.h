/* decade_heatmap.h — Convergence heatmap data for multi-year spans (S88)
 *
 * Computes a grid of convergence intensity: [year][day_of_year] = system_count.
 * Used by VISUALS for rendering decade-scale heatmap displays.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_DECADE_HEATMAP_H
#define TIME_DECADE_HEATMAP_H

#include "brain_archive.h"

/* Maximum years in a heatmap */
#define DH_MAX_YEARS 12

/* Maximum days per year (366 for leap years) */
#define DH_MAX_DAYS 366

/* Heatmap grid: [year][day] = convergence system_count */
typedef struct {
    int  start_year;
    int  year_count;
    int  grid[DH_MAX_YEARS][DH_MAX_DAYS];     /* system_count per cell */
    int  days_in_year[DH_MAX_YEARS];           /* 365 or 366 */
    int  best_year_index;                       /* year with highest total */
    int  best_day_year;                         /* year index of single best day */
    int  best_day_doy;                          /* day-of-year of single best day */
    int  max_system_count;                      /* highest single-cell value */
} dh_heatmap_t;

/* Row summary for a single year */
typedef struct {
    int  year;
    int  total_convergence;     /* sum of system_count across all days */
    int  notable_days;          /* days with 3+ systems */
    int  max_day_count;         /* highest single-day system_count */
    double average;             /* total_convergence / days_in_year */
} dh_year_summary_t;

/* Compute heatmap for a range of years.
 * year_count must be <= DH_MAX_YEARS.
 * Returns number of years computed. */
int dh_compute(int start_year, int year_count, dh_heatmap_t *out);

/* Get total convergence sum for a year row.
 * Returns 0 if year_index out of range. */
int dh_row_total(const dh_heatmap_t *map, int year_index);

/* Get max system_count across all years for a given day-of-year.
 * Returns 0 if day_of_year out of range. */
int dh_col_max(const dh_heatmap_t *map, int day_of_year);

/* Average convergence per day for a year.
 * Returns 0.0 if year_index out of range. */
double dh_year_average(const dh_heatmap_t *map, int year_index);

/* Get full summary for a year row. Returns 1 on success. */
int dh_year_summary(const dh_heatmap_t *map, int year_index,
                    dh_year_summary_t *out);

#endif /* TIME_DECADE_HEATMAP_H */
