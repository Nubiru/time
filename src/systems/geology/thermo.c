/* thermo.c — Thermochronology data module.
 * Pure functions. 8 thermal events, 6 mineral dating systems,
 * simplified Earth cooling model.
 * Data sources: Jaupart & Mareschal (Heat Generation and Transport in the Earth). */

#include "thermo.h"

#include <math.h>
#include <stddef.h>
#include <string.h>

/* Physical constants for Earth cooling model */
static const double EARTH_AGE_MA       = 4540.0;
static const double CORE_TEMP_PRESENT  = 5200.0;  /* K */
static const double CORE_TEMP_INITIAL  = 6500.0;  /* K at formation */
static const double MANTLE_TEMP_PRESENT = 1600.0;  /* K */
static const double MANTLE_TEMP_INITIAL = 2200.0;  /* K at formation */
static const double SURFACE_TEMP_PRESENT = 288.0;  /* K */
static const double SURFACE_TEMP_INITIAL = 1500.0; /* K at formation */
static const double HEAT_FLOW_PRESENT  = 87.0;     /* mW/m^2 */
static const double HEAT_FLOW_INITIAL  = 300.0;    /* mW/m^2 at formation */
/* ~3800 Ma ago: transition from magma ocean to habitable surface */
static const double SURFACE_TRANSITION_MA = 3800.0;

/* Static thermal event data table */
static const thermal_event_t EVENTS[THERMO_EVENT_COUNT] = {
    {
        .id = 0,
        .name = "Core Formation",
        .age_ma = 4500.0,
        .duration_ma = 100.0,
        .temp_effect_k = 2000.0,
        .description = "Iron catastrophe — gravitational energy heats proto-Earth"
    },
    {
        .id = 1,
        .name = "Late Heavy Bombardment",
        .age_ma = 4100.0,
        .duration_ma = 300.0,
        .temp_effect_k = 500.0,
        .description = "Asteroid impacts add thermal energy"
    },
    {
        .id = 2,
        .name = "Mantle Convection Onset",
        .age_ma = 4400.0,
        .duration_ma = 200.0,
        .temp_effect_k = -200.0,
        .description = "Convective cooling begins"
    },
    {
        .id = 3,
        .name = "Archean Mantle Plumes",
        .age_ma = 3500.0,
        .duration_ma = 500.0,
        .temp_effect_k = 100.0,
        .description = "Large igneous provinces, komatiite eruptions"
    },
    {
        .id = 4,
        .name = "Great Oxidation Event",
        .age_ma = 2400.0,
        .duration_ma = 200.0,
        .temp_effect_k = -50.0,
        .description = "Atmosphere change, minor cooling effect"
    },
    {
        .id = 5,
        .name = "Snowball Earth (Sturtian)",
        .age_ma = 717.0,
        .duration_ma = 57.0,
        .temp_effect_k = -100.0,
        .description = "Global glaciation, reduced heat flow"
    },
    {
        .id = 6,
        .name = "Pangaea Assembly",
        .age_ma = 335.0,
        .duration_ma = 65.0,
        .temp_effect_k = 30.0,
        .description = "Continental insulation effect"
    },
    {
        .id = 7,
        .name = "Present Equilibrium",
        .age_ma = 0.0,
        .duration_ma = 0.0,
        .temp_effect_k = 0.0,
        .description = "Current thermal steady state"
    }
};

/* Static mineral dating system data table */
static const mineral_system_t MINERALS[THERMO_MINERAL_COUNT] = {
    {
        .id = 0,
        .mineral = "Zircon",
        .system = "U-Pb",
        .closure_temp_c = 900.0,
        .closure_temp_k = 900.0 + 273.15,
        .useful_range_min_ma = 1.0,
        .useful_range_max_ma = 4500.0
    },
    {
        .id = 1,
        .mineral = "Zircon",
        .system = "Fission Track",
        .closure_temp_c = 240.0,
        .closure_temp_k = 240.0 + 273.15,
        .useful_range_min_ma = 1.0,
        .useful_range_max_ma = 2000.0
    },
    {
        .id = 2,
        .mineral = "Apatite",
        .system = "Fission Track",
        .closure_temp_c = 110.0,
        .closure_temp_k = 110.0 + 273.15,
        .useful_range_min_ma = 0.1,
        .useful_range_max_ma = 500.0
    },
    {
        .id = 3,
        .mineral = "Apatite",
        .system = "(U-Th)/He",
        .closure_temp_c = 70.0,
        .closure_temp_k = 70.0 + 273.15,
        .useful_range_min_ma = 0.01,
        .useful_range_max_ma = 200.0
    },
    {
        .id = 4,
        .mineral = "Muscovite",
        .system = "K-Ar",
        .closure_temp_c = 350.0,
        .closure_temp_k = 350.0 + 273.15,
        .useful_range_min_ma = 1.0,
        .useful_range_max_ma = 4500.0
    },
    {
        .id = 5,
        .mineral = "Hornblende",
        .system = "K-Ar",
        .closure_temp_c = 500.0,
        .closure_temp_k = 500.0 + 273.15,
        .useful_range_min_ma = 10.0,
        .useful_range_max_ma = 4500.0
    }
};

static int valid_event_index(int index) {
    return index >= 0 && index < THERMO_EVENT_COUNT;
}

static int valid_mineral_index(int index) {
    return index >= 0 && index < THERMO_MINERAL_COUNT;
}

thermal_event_t thermo_event_get(int index) {
    if (!valid_event_index(index)) {
        thermal_event_t empty = {
            .id = -1,
            .name = NULL,
            .age_ma = 0.0,
            .duration_ma = 0.0,
            .temp_effect_k = 0.0,
            .description = NULL
        };
        return empty;
    }
    return EVENTS[index];
}

int thermo_event_count(void) {
    return THERMO_EVENT_COUNT;
}

int thermo_event_nearest(double age_ma) {
    double best_dist = 1e30;
    int best = 0;
    int i;
    for (i = 0; i < THERMO_EVENT_COUNT; i++) {
        double dist = fabs(EVENTS[i].age_ma - age_ma);
        if (dist < best_dist) {
            best_dist = dist;
            best = i;
        }
    }
    return best;
}

earth_thermal_t thermo_earth_at(double age_ma) {
    earth_thermal_t result;
    double frac;

    /* Clamp age to valid range */
    if (age_ma < 0.0) { age_ma = 0.0; }
    if (age_ma > EARTH_AGE_MA) { age_ma = EARTH_AGE_MA; }

    result.age_ma = age_ma;

    /* Linear interpolation: T = T_present + (T_initial - T_present) * (age_ma / 4540) */
    frac = age_ma / EARTH_AGE_MA;

    result.core_temp_k = CORE_TEMP_PRESENT +
        (CORE_TEMP_INITIAL - CORE_TEMP_PRESENT) * frac;

    result.mantle_temp_k = MANTLE_TEMP_PRESENT +
        (MANTLE_TEMP_INITIAL - MANTLE_TEMP_PRESENT) * frac;

    result.heat_flow_mw = HEAT_FLOW_PRESENT +
        (HEAT_FLOW_INITIAL - HEAT_FLOW_PRESENT) * frac;

    /* Surface temperature: two-phase model.
     * Before ~3800 Ma ago: magma ocean, linearly interpolate 1500K -> 288K
     * After ~3800 Ma ago: roughly habitable range, interpolate more gently */
    if (age_ma >= SURFACE_TRANSITION_MA) {
        /* Early Earth: magma ocean phase */
        double early_frac = (age_ma - SURFACE_TRANSITION_MA) /
                            (EARTH_AGE_MA - SURFACE_TRANSITION_MA);
        /* At transition (3800 Ma): ~400K. At formation (4540 Ma): ~1500K */
        result.surface_temp_k = 400.0 +
            (SURFACE_TEMP_INITIAL - 400.0) * early_frac;
    } else {
        /* Post-magma ocean: gradual cooling from ~400K to 288K */
        double late_frac = age_ma / SURFACE_TRANSITION_MA;
        result.surface_temp_k = SURFACE_TEMP_PRESENT +
            (400.0 - SURFACE_TEMP_PRESENT) * late_frac;
    }

    return result;
}

mineral_system_t thermo_mineral_get(int index) {
    if (!valid_mineral_index(index)) {
        mineral_system_t empty = {
            .id = -1,
            .mineral = NULL,
            .system = NULL,
            .closure_temp_c = 0.0,
            .closure_temp_k = 0.0,
            .useful_range_min_ma = 0.0,
            .useful_range_max_ma = 0.0
        };
        return empty;
    }
    return MINERALS[index];
}

int thermo_mineral_count(void) {
    return THERMO_MINERAL_COUNT;
}

mineral_system_t thermo_mineral_find(const char *mineral_name) {
    if (mineral_name == NULL) {
        return thermo_mineral_get(-1);
    }
    int i;
    for (i = 0; i < THERMO_MINERAL_COUNT; i++) {
        if (strcmp(MINERALS[i].mineral, mineral_name) == 0) {
            return MINERALS[i];
        }
    }
    return thermo_mineral_get(-1);
}

bool thermo_mineral_in_range(int mineral_index, double age_ma) {
    if (!valid_mineral_index(mineral_index)) { return false; }
    return age_ma >= MINERALS[mineral_index].useful_range_min_ma
        && age_ma <= MINERALS[mineral_index].useful_range_max_ma;
}

int thermo_best_mineral(double age_ma) {
    double best_closure = -1.0;
    int best = -1;
    int i;

    for (i = 0; i < THERMO_MINERAL_COUNT; i++) {
        if (!thermo_mineral_in_range(i, age_ma)) { continue; }
        /* Prefer higher closure temperature (more robust for older ages) */
        if (MINERALS[i].closure_temp_c > best_closure) {
            best_closure = MINERALS[i].closure_temp_c;
            best = i;
        }
    }
    return best;
}

int thermo_core_cooling_curve(double max_age_ma, double *out_temps_k,
                              double *out_ages_ma, int n_points) {
    if (out_temps_k == NULL || out_ages_ma == NULL || n_points < 1) {
        return 0;
    }

    int i;
    for (i = 0; i < n_points; i++) {
        double age = (n_points > 1)
            ? max_age_ma * (double)i / (double)(n_points - 1)
            : 0.0;
        earth_thermal_t state = thermo_earth_at(age);
        out_ages_ma[i] = age;
        out_temps_k[i] = state.core_temp_k;
    }
    return n_points;
}

int thermo_mantle_cooling_curve(double max_age_ma, double *out_temps_k,
                                double *out_ages_ma, int n_points) {
    if (out_temps_k == NULL || out_ages_ma == NULL || n_points < 1) {
        return 0;
    }

    int i;
    for (i = 0; i < n_points; i++) {
        double age = (n_points > 1)
            ? max_age_ma * (double)i / (double)(n_points - 1)
            : 0.0;
        earth_thermal_t state = thermo_earth_at(age);
        out_ages_ma[i] = age;
        out_temps_k[i] = state.mantle_temp_k;
    }
    return n_points;
}

earth_thermal_t thermo_earth_present(void) {
    return thermo_earth_at(0.0);
}
