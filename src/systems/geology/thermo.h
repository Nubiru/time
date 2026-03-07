/* thermo.h — Thermochronology data module.
 * 8 thermal events, 6 mineral dating systems, Earth cooling model.
 * All pure functions, no side effects.
 * Data sources: Jaupart & Mareschal (Heat Generation and Transport in the Earth). */

#ifndef TIME_THERMO_H
#define TIME_THERMO_H

#include <stdbool.h>

#define THERMO_EVENT_COUNT 8
#define THERMO_MINERAL_COUNT 6

typedef struct {
    int id;
    const char *name;
    double age_ma;            /* millions of years ago */
    double duration_ma;       /* duration in Ma */
    double temp_effect_k;     /* approximate temperature effect (K) */
    const char *description;
} thermal_event_t;

typedef struct {
    double age_ma;
    double core_temp_k;       /* core temperature (Kelvin) */
    double mantle_temp_k;     /* upper mantle temperature (Kelvin) */
    double surface_temp_k;    /* surface temperature (Kelvin) */
    double heat_flow_mw;      /* surface heat flow (mW/m^2) */
} earth_thermal_t;

typedef struct {
    int id;
    const char *mineral;
    const char *system;       /* e.g., "U-Pb", "(U-Th)/He", "Fission Track" */
    double closure_temp_c;    /* closure temperature in Celsius */
    double closure_temp_k;    /* closure temperature in Kelvin */
    double useful_range_min_ma;
    double useful_range_max_ma;
} mineral_system_t;

/* Get thermal event by index (0..7). Returns id=-1 for invalid. */
thermal_event_t thermo_event_get(int index);

/* Number of thermal events (always 8). */
int thermo_event_count(void);

/* Find event with minimum |event.age_ma - age_ma|. */
int thermo_event_nearest(double age_ma);

/* Simplified Earth cooling model at a given geological age (Ma ago).
 * age_ma=0 is present, age_ma=4540 is Earth formation. */
earth_thermal_t thermo_earth_at(double age_ma);

/* Get mineral dating system by index (0..5). Returns id=-1 for invalid. */
mineral_system_t thermo_mineral_get(int index);

/* Number of mineral dating systems (always 6). */
int thermo_mineral_count(void);

/* Find mineral system by mineral name (strcmp). Returns id=-1 if not found. */
mineral_system_t thermo_mineral_find(const char *mineral_name);

/* Find best mineral system for a given age. Prefers higher closure temperature
 * for older ages (more robust). Returns -1 if no system covers. */
int thermo_best_mineral(double age_ma);

/* Fill arrays from age 0 to max_age_ma with n_points evenly spaced core temps.
 * Returns count written, or 0 on error (NULL, n_points < 1). */
int thermo_core_cooling_curve(double max_age_ma, double *out_temps_k,
                              double *out_ages_ma, int n_points);

/* Same as core cooling curve but for mantle temperature. */
int thermo_mantle_cooling_curve(double max_age_ma, double *out_temps_k,
                                double *out_ages_ma, int n_points);

/* Check if age_ma falls within mineral system's useful range.
 * Returns false for out-of-bounds index. */
bool thermo_mineral_in_range(int mineral_index, double age_ma);

/* Convenience: thermo_earth_at(0.0). */
earth_thermal_t thermo_earth_present(void);

#endif /* TIME_THERMO_H */
