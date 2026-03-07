/* cosmic_duality.h — Bundahishn cosmic duality timeline
 * Pure functions modeling the 12,000-year Zoroastrian cosmic cycle:
 * Creation (spiritual + material) -> Mixture -> Separation (Frashokereti).
 * 7 Amesha Spentas with Kabbalistic Sefirot parallels.
 * Geological timeline mapping (4540 Ma proportional). */

#ifndef TIME_COSMIC_DUALITY_H
#define TIME_COSMIC_DUALITY_H

#define CD_CYCLE_YEARS    12000
#define CD_AMESHA_COUNT   7

typedef enum {
    CD_ERA_CREATION = 0,
    CD_ERA_MIXTURE,
    CD_ERA_SEPARATION,
    CD_ERA_COUNT
} cd_era_t;

typedef enum {
    CD_PERIOD_SPIRITUAL_CREATION = 0,
    CD_PERIOD_MATERIAL_CREATION,
    CD_PERIOD_MIXTURE,
    CD_PERIOD_RENOVATION,
    CD_PERIOD_COUNT
} cd_period_t;

typedef struct {
    int index;
    const char *avestan_name;
    const char *meaning;
    const char *domain;
    const char *element;
    const char *sefirah_parallel;
    int day_number;
} cd_amesha_spenta_t;

typedef struct {
    cd_era_t era;
    cd_period_t period;
    int cycle_year;
    double progress;
    double era_progress;
    const char *description;
} cd_position_t;

typedef struct {
    double cosmic_years;
    double geological_ma;
    const char *geological_parallel;
} cd_geo_map_t;

/* Determine which era a cosmic year falls in. Clamps out-of-range. */
cd_era_t cd_era(int cosmic_year);

/* Era name string. Returns "?" for invalid. */
const char *cd_era_name(cd_era_t era);

/* Era description string. Returns "?" for invalid. */
const char *cd_era_description(cd_era_t era);

/* Determine which period (of 4) a cosmic year falls in. Clamps out-of-range. */
cd_period_t cd_period(int cosmic_year);

/* Period name string. Returns "?" for invalid. */
const char *cd_period_name(cd_period_t period);

/* Full position within the cosmic cycle. */
cd_position_t cd_position(int cosmic_year);

/* Overall cycle progress [0.0, 1.0]. Clamps out-of-range. */
double cd_progress(int cosmic_year);

/* Retrieve Amesha Spenta by index (0-6). Invalid index returns sentinel. */
cd_amesha_spenta_t cd_amesha_spenta(int index);

/* Retrieve Amesha Spenta by Zoroastrian month day (1-7). Day 8+ invalid. */
cd_amesha_spenta_t cd_amesha_spenta_for_day(int day);

/* Map cosmic year to geological timeline (Ma). Clamps out-of-range. */
cd_geo_map_t cd_geological_parallel(int cosmic_year);

/* Number of Amesha Spentas (always 7). */
int cd_amesha_spenta_count(void);

/* Convert Yazdgerdi calendar year to cosmic cycle year. Clamps to 0-11999. */
int cd_cycle_year_from_yazdgerdi(int yazdgerdi_year);

#endif /* TIME_COSMIC_DUALITY_H */
