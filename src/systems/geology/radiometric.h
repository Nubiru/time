/* radiometric.h — Radiometric dating constants and decay math.
 * Isotope half-lives, decay fractions, age estimation, decay curves.
 * All pure functions, no side effects. IUPAC recommended values. */

#ifndef TIME_RADIOMETRIC_H
#define TIME_RADIOMETRIC_H

#define RADIOMETRIC_SYSTEM_COUNT 8

/* Decay system identifiers */
typedef enum {
    DECAY_U238_PB206 = 0,   /* Uranium-238 to Lead-206 */
    DECAY_U235_PB207,        /* Uranium-235 to Lead-207 */
    DECAY_K40_AR40,          /* Potassium-40 to Argon-40 */
    DECAY_RB87_SR87,         /* Rubidium-87 to Strontium-87 */
    DECAY_C14_N14,           /* Carbon-14 to Nitrogen-14 */
    DECAY_SM147_ND143,       /* Samarium-147 to Neodymium-143 */
    DECAY_LU176_HF176,       /* Lutetium-176 to Hafnium-176 */
    DECAY_TH232_PB208,       /* Thorium-232 to Lead-208 */
    DECAY_SYSTEM_COUNT
} decay_system_t;

/* Isotope decay system data */
typedef struct {
    decay_system_t id;
    const char *parent;         /* parent isotope name */
    const char *daughter;       /* daughter isotope name */
    double half_life_years;     /* half-life in years */
    double decay_constant;      /* lambda = ln(2) / half_life */
    double useful_range_min_yr; /* minimum datable age (years) */
    double useful_range_max_yr; /* maximum datable age (years) */
} decay_data_t;

/* Decay curve sample point */
typedef struct {
    float time_fraction;     /* 0.0 = t=0, 1.0 = t=max_time */
    float parent_fraction;   /* remaining parent (0-1) */
    float daughter_fraction; /* accumulated daughter (0-1) */
} decay_point_t;

/* Get decay system data by ID. Returns zeroed struct for invalid ID. */
decay_data_t radiometric_system(decay_system_t id);

/* Parent isotope name. Returns NULL for invalid ID. */
const char *radiometric_parent(decay_system_t id);

/* Daughter isotope name. Returns NULL for invalid ID. */
const char *radiometric_daughter(decay_system_t id);

/* Half-life in years. Returns 0 for invalid ID. */
double radiometric_half_life(decay_system_t id);

/* Fraction of parent remaining after elapsed_years.
 * N/N0 = (1/2)^(t/t_half). Uses pow(0.5,...) for stability. */
double decay_fraction(double half_life_years, double elapsed_years);

/* Age estimate from parent/daughter ratio.
 * t = (t_half / ln(2)) * ln(1 + daughter/parent)
 * Returns age in years. Returns 0 if parent <= 0. */
double age_from_ratio(double parent_amount, double daughter_amount,
                      double half_life_years);

/* Number of half-lives elapsed. */
double half_lives_elapsed(double half_life_years, double elapsed_years);

/* Generate decay curve sample points for visualization.
 * points: output array of at least n_points elements.
 * max_time_years: time span to sample.
 * Returns number of points written (= n_points), or 0 on error. */
int decay_curve(decay_system_t id, decay_point_t *points,
                int n_points, double max_time_years);

/* Is a given age within the useful dating range of this system?
 * Returns 1 if in range, 0 otherwise. */
int radiometric_in_range(decay_system_t id, double age_years);

/* Suggest best dating system for a given age (years).
 * Returns the system whose useful range best covers the age.
 * Prefers the system where age is closest to center of range in log space.
 * Returns DECAY_SYSTEM_COUNT if no system covers it. */
decay_system_t radiometric_best_system(double age_years);

#endif /* TIME_RADIOMETRIC_H */
