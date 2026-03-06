/* radiometric.h — Radiometric dating constants and decay math.
 * 8 isotope systems with half-lives, decay fractions, age estimation,
 * and decay curves. All pure functions, no side effects.
 * IUPAC recommended values. */

#ifndef TIME_RADIOMETRIC_H
#define TIME_RADIOMETRIC_H

#include <stdbool.h>

#define RADIOMETRIC_SYSTEM_COUNT 8

typedef struct {
    int id;
    const char *parent;
    const char *daughter;
    double half_life_years;
    double decay_constant;    /* lambda = ln(2) / half_life */
    const char *method_name;
    double useful_range_min;
    double useful_range_max;
} radiometric_system_t;

typedef struct {
    double time;              /* elapsed time (in half-lives) */
    double parent_fraction;
    double daughter_fraction;
} decay_point_t;

/* Get isotope system data by index (0..7). Returns id=-1 for invalid. */
radiometric_system_t radiometric_system_get(int index);

/* Number of isotope systems (always 8). */
int radiometric_system_count(void);

/* Find system by parent isotope name (strcmp match). id=-1 if not found. */
radiometric_system_t radiometric_find_by_parent(const char *parent_name);

/* Fraction of parent remaining: N/N0 = e^(-lambda * t). */
double radiometric_remaining_fraction(double half_life_years, double elapsed_years);

/* Fraction decayed to daughter: D/N0 = 1 - e^(-lambda * t). */
double radiometric_daughter_fraction(double half_life_years, double elapsed_years);

/* Age from parent/daughter ratio: t = ln(1 + D/P) / lambda.
 * Returns -1.0 for invalid inputs (parent_ratio <= 0, daughter_ratio < 0). */
double radiometric_age_from_ratio(double half_life_years, double parent_ratio, double daughter_ratio);

/* Generate decay curve: n_points from time=0 to time=max_half_lives.
 * Returns count written, or 0 on error. */
int radiometric_decay_curve(double max_half_lives, decay_point_t *out_points, int n_points);

/* Check if elapsed_years falls within system's useful range.
 * Returns false for out-of-bounds index. */
bool radiometric_in_range(int system_index, double elapsed_years);

/* Find best system for a given age. Prefers C-14 for young samples.
 * Returns -1 if no system covers the age. */
int radiometric_best_system(double elapsed_years);

/* Compute decay constant: lambda = ln(2) / half_life. */
double radiometric_decay_constant(double half_life_years);

#endif /* TIME_RADIOMETRIC_H */
