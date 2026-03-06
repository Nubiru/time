/* radiometric.c — Radiometric dating constants and decay math.
 * Pure functions. IUPAC recommended half-life values. */

#include "radiometric.h"

#include <math.h>
#include <stddef.h>

static const double LN2 = 0.693147180559945309417;

/* Static decay system data table (IUPAC recommended values) */
static const decay_data_t SYSTEMS[RADIOMETRIC_SYSTEM_COUNT] = {
    {
        .id = DECAY_U238_PB206,
        .parent = "U-238",
        .daughter = "Pb-206",
        .half_life_years = 4.468e9,
        .decay_constant = 0.693147180559945309417 / 4.468e9,
        .useful_range_min_yr = 1e6,
        .useful_range_max_yr = 4.6e9
    },
    {
        .id = DECAY_U235_PB207,
        .parent = "U-235",
        .daughter = "Pb-207",
        .half_life_years = 7.04e8,
        .decay_constant = 0.693147180559945309417 / 7.04e8,
        .useful_range_min_yr = 1e6,
        .useful_range_max_yr = 4.6e9
    },
    {
        .id = DECAY_K40_AR40,
        .parent = "K-40",
        .daughter = "Ar-40",
        .half_life_years = 1.248e9,
        .decay_constant = 0.693147180559945309417 / 1.248e9,
        .useful_range_min_yr = 1e5,
        .useful_range_max_yr = 4.6e9
    },
    {
        .id = DECAY_RB87_SR87,
        .parent = "Rb-87",
        .daughter = "Sr-87",
        .half_life_years = 4.88e10,
        .decay_constant = 0.693147180559945309417 / 4.88e10,
        .useful_range_min_yr = 1e7,
        .useful_range_max_yr = 4.6e9
    },
    {
        .id = DECAY_C14_N14,
        .parent = "C-14",
        .daughter = "N-14",
        .half_life_years = 5730.0,
        .decay_constant = 0.693147180559945309417 / 5730.0,
        .useful_range_min_yr = 100.0,
        .useful_range_max_yr = 50000.0
    },
    {
        .id = DECAY_SM147_ND143,
        .parent = "Sm-147",
        .daughter = "Nd-143",
        .half_life_years = 1.06e11,
        .decay_constant = 0.693147180559945309417 / 1.06e11,
        .useful_range_min_yr = 1e8,
        .useful_range_max_yr = 4.6e9
    },
    {
        .id = DECAY_LU176_HF176,
        .parent = "Lu-176",
        .daughter = "Hf-176",
        .half_life_years = 3.71e10,
        .decay_constant = 0.693147180559945309417 / 3.71e10,
        .useful_range_min_yr = 1e8,
        .useful_range_max_yr = 4.6e9
    },
    {
        .id = DECAY_TH232_PB208,
        .parent = "Th-232",
        .daughter = "Pb-208",
        .half_life_years = 1.405e10,
        .decay_constant = 0.693147180559945309417 / 1.405e10,
        .useful_range_min_yr = 1e7,
        .useful_range_max_yr = 4.6e9
    }
};

static int valid_id(decay_system_t id) {
    return id >= 0 && id < DECAY_SYSTEM_COUNT;
}

decay_data_t radiometric_system(decay_system_t id) {
    if (!valid_id(id)) {
        decay_data_t empty = {
            .id = DECAY_SYSTEM_COUNT,
            .parent = NULL,
            .daughter = NULL,
            .half_life_years = 0.0,
            .decay_constant = 0.0,
            .useful_range_min_yr = 0.0,
            .useful_range_max_yr = 0.0
        };
        return empty;
    }
    return SYSTEMS[id];
}

const char *radiometric_parent(decay_system_t id) {
    if (!valid_id(id)) { return NULL; }
    return SYSTEMS[id].parent;
}

const char *radiometric_daughter(decay_system_t id) {
    if (!valid_id(id)) { return NULL; }
    return SYSTEMS[id].daughter;
}

double radiometric_half_life(decay_system_t id) {
    if (!valid_id(id)) { return 0.0; }
    return SYSTEMS[id].half_life_years;
}

double decay_fraction(double half_life_years, double elapsed_years) {
    if (half_life_years <= 0.0) { return 0.0; }
    /* pow(0.5, t/t_half) is numerically stable for large t */
    return pow(0.5, elapsed_years / half_life_years);
}

double age_from_ratio(double parent_amount, double daughter_amount,
                      double half_life_years) {
    if (parent_amount <= 0.0 || half_life_years <= 0.0) { return 0.0; }
    if (daughter_amount < 0.0) { daughter_amount = 0.0; }
    /* t = (t_half / ln(2)) * ln(1 + D/P) */
    return (half_life_years / LN2) * log(1.0 + daughter_amount / parent_amount);
}

double half_lives_elapsed(double half_life_years, double elapsed_years) {
    if (half_life_years <= 0.0) { return 0.0; }
    return elapsed_years / half_life_years;
}

int decay_curve(decay_system_t id, decay_point_t *points,
                int n_points, double max_time_years) {
    if (!valid_id(id) || points == NULL || n_points < 1) { return 0; }
    double hl = SYSTEMS[id].half_life_years;
    for (int i = 0; i < n_points; i++) {
        double frac = (n_points > 1)
            ? (double)i / (double)(n_points - 1)
            : 0.0;
        double t = frac * max_time_years;
        double parent = decay_fraction(hl, t);
        points[i].time_fraction = (float)frac;
        points[i].parent_fraction = (float)parent;
        points[i].daughter_fraction = (float)(1.0 - parent);
    }
    return n_points;
}

int radiometric_in_range(decay_system_t id, double age_years) {
    if (!valid_id(id)) { return 0; }
    return age_years >= SYSTEMS[id].useful_range_min_yr
        && age_years <= SYSTEMS[id].useful_range_max_yr;
}

decay_system_t radiometric_best_system(double age_years) {
    if (age_years <= 0.0) { return DECAY_SYSTEM_COUNT; }

    double log_age = log(age_years);
    double best_dist = 1e30;
    decay_system_t best = DECAY_SYSTEM_COUNT;

    for (int i = 0; i < RADIOMETRIC_SYSTEM_COUNT; i++) {
        double rmin = SYSTEMS[i].useful_range_min_yr;
        double rmax = SYSTEMS[i].useful_range_max_yr;
        if (age_years < rmin || age_years > rmax) { continue; }

        /* Center of useful range in log space */
        double log_center = (log(rmin) + log(rmax)) * 0.5;
        double dist = fabs(log_age - log_center);
        if (dist < best_dist) {
            best_dist = dist;
            best = (decay_system_t)i;
        }
    }
    return best;
}
