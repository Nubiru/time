/* radiometric.c — Radiometric dating constants and decay math.
 * Pure functions. IUPAC recommended half-life values.
 * 8 isotope decay systems for geological age determination. */

#include "radiometric.h"

#include <math.h>
#include <stddef.h>
#include <string.h>

#define LN2 0.693147180559945309417

/* Static isotope system data table (IUPAC recommended values) */
static const radiometric_system_t SYSTEMS[RADIOMETRIC_SYSTEM_COUNT] = {
    {
        .id = 0,
        .parent = "C-14",
        .daughter = "N-14",
        .half_life_years = 5730.0,
        .decay_constant = LN2 / 5730.0,
        .method_name = "Radiocarbon",
        .useful_range_min = 100.0,
        .useful_range_max = 50000.0
    },
    {
        .id = 1,
        .parent = "K-40",
        .daughter = "Ar-40",
        .half_life_years = 1.248e9,
        .decay_constant = LN2 / 1.248e9,
        .method_name = "Potassium-Argon",
        .useful_range_min = 100000.0,
        .useful_range_max = 4.5e9
    },
    {
        .id = 2,
        .parent = "Rb-87",
        .daughter = "Sr-87",
        .half_life_years = 4.88e10,
        .decay_constant = LN2 / 4.88e10,
        .method_name = "Rubidium-Strontium",
        .useful_range_min = 10e6,
        .useful_range_max = 4.5e9
    },
    {
        .id = 3,
        .parent = "U-235",
        .daughter = "Pb-207",
        .half_life_years = 7.04e8,
        .decay_constant = LN2 / 7.04e8,
        .method_name = "Uranium-Lead (235)",
        .useful_range_min = 1e6,
        .useful_range_max = 4.5e9
    },
    {
        .id = 4,
        .parent = "U-238",
        .daughter = "Pb-206",
        .half_life_years = 4.468e9,
        .decay_constant = LN2 / 4.468e9,
        .method_name = "Uranium-Lead (238)",
        .useful_range_min = 1e6,
        .useful_range_max = 4.5e9
    },
    {
        .id = 5,
        .parent = "Th-232",
        .daughter = "Pb-208",
        .half_life_years = 1.405e10,
        .decay_constant = LN2 / 1.405e10,
        .method_name = "Thorium-Lead",
        .useful_range_min = 1e6,
        .useful_range_max = 4.5e9
    },
    {
        .id = 6,
        .parent = "Sm-147",
        .daughter = "Nd-143",
        .half_life_years = 1.06e11,
        .decay_constant = LN2 / 1.06e11,
        .method_name = "Samarium-Neodymium",
        .useful_range_min = 1e8,
        .useful_range_max = 4.5e9
    },
    {
        .id = 7,
        .parent = "Lu-176",
        .daughter = "Hf-176",
        .half_life_years = 3.71e10,
        .decay_constant = LN2 / 3.71e10,
        .method_name = "Lutetium-Hafnium",
        .useful_range_min = 1e8,
        .useful_range_max = 4.5e9
    }
};

static int valid_index(int index) {
    return index >= 0 && index < RADIOMETRIC_SYSTEM_COUNT;
}

radiometric_system_t radiometric_system_get(int index) {
    if (!valid_index(index)) {
        radiometric_system_t empty = {
            .id = -1,
            .parent = NULL,
            .daughter = NULL,
            .half_life_years = 0.0,
            .decay_constant = 0.0,
            .method_name = NULL,
            .useful_range_min = 0.0,
            .useful_range_max = 0.0
        };
        return empty;
    }
    return SYSTEMS[index];
}

int radiometric_system_count(void) {
    return RADIOMETRIC_SYSTEM_COUNT;
}

radiometric_system_t radiometric_find_by_parent(const char *parent_name) {
    if (parent_name == NULL) {
        return radiometric_system_get(-1);
    }
    for (int i = 0; i < RADIOMETRIC_SYSTEM_COUNT; i++) {
        if (strcmp(SYSTEMS[i].parent, parent_name) == 0) {
            return SYSTEMS[i];
        }
    }
    return radiometric_system_get(-1);
}

double radiometric_decay_constant(double half_life_years) {
    if (half_life_years <= 0.0) { return 0.0; }
    return LN2 / half_life_years;
}

double radiometric_remaining_fraction(double half_life_years, double elapsed_years) {
    if (half_life_years <= 0.0) { return 0.0; }
    double lambda = LN2 / half_life_years;
    return exp(-lambda * elapsed_years);
}

double radiometric_daughter_fraction(double half_life_years, double elapsed_years) {
    return 1.0 - radiometric_remaining_fraction(half_life_years, elapsed_years);
}

double radiometric_age_from_ratio(double half_life_years, double parent_ratio, double daughter_ratio) {
    if (parent_ratio <= 0.0 || daughter_ratio < 0.0 || half_life_years <= 0.0) {
        return -1.0;
    }
    double lambda = LN2 / half_life_years;
    /* t = ln(1 + D/P) / lambda */
    return log(1.0 + daughter_ratio / parent_ratio) / lambda;
}

int radiometric_decay_curve(double max_half_lives, decay_point_t *out_points, int n_points) {
    if (out_points == NULL || n_points < 1) { return 0; }

    for (int i = 0; i < n_points; i++) {
        double t = (n_points > 1)
            ? max_half_lives * (double)i / (double)(n_points - 1)
            : 0.0;
        /* At t half-lives, parent = (1/2)^t = e^(-ln2 * t) */
        double parent = exp(-LN2 * t);
        out_points[i].time = t;
        out_points[i].parent_fraction = parent;
        out_points[i].daughter_fraction = 1.0 - parent;
    }
    return n_points;
}

bool radiometric_in_range(int system_index, double elapsed_years) {
    if (!valid_index(system_index)) { return false; }
    return elapsed_years >= SYSTEMS[system_index].useful_range_min
        && elapsed_years <= SYSTEMS[system_index].useful_range_max;
}

int radiometric_best_system(double elapsed_years) {
    if (elapsed_years <= 0.0) { return -1; }

    /* Prefer C-14 for young samples (it's the most precise for organics) */
    if (radiometric_in_range(0, elapsed_years)) {
        return 0;
    }

    double log_age = log(elapsed_years);
    double best_dist = 1e30;
    int best = -1;

    for (int i = 0; i < RADIOMETRIC_SYSTEM_COUNT; i++) {
        if (!radiometric_in_range(i, elapsed_years)) { continue; }

        /* Center of useful range in log space */
        double log_center = (log(SYSTEMS[i].useful_range_min) +
                             log(SYSTEMS[i].useful_range_max)) * 0.5;
        double dist = fabs(log_age - log_center);
        if (dist < best_dist) {
            best_dist = dist;
            best = i;
        }
    }
    return best;
}
