/* earth_drama.h — Dramatized Earth history data module.
 * Composition layer over geo_time, climate_history, and fossil_milestones.
 * Provides atmosphere evolution, biosphere state, dramatic snapshots,
 * human arrival scale, and era one-liners.
 * All pure functions, no side effects, no malloc, no globals. */

#ifndef TIME_EARTH_DRAMA_H
#define TIME_EARTH_DRAMA_H

#define ED_MAX_ALIVE 16
#define ED_EARTH_AGE_MA 4540.0

/* Atmosphere state at a given time */
typedef enum {
    ED_ATMO_REDUCING = 0,   /* Pre-GOE: CO2/N2/CH4, no free O2 */
    ED_ATMO_TRANSITIONAL,   /* GOE underway: trace O2, fluctuating */
    ED_ATMO_OXIDIZING,      /* Post-GOE: significant O2, no ozone yet */
    ED_ATMO_OZONE,          /* Ozone layer formed: UV shielded */
    ED_ATMO_MODERN          /* Modern composition: 21% O2, 78% N2 */
} ed_atmosphere_t;

/* Atmosphere snapshot */
typedef struct {
    ed_atmosphere_t state;
    const char *description;
    double o2_percent;       /* approximate O2 percentage */
    double co2_ppm;          /* approximate CO2 (ppm), 0 if unknown */
    int uv_shielded;         /* 1 if ozone layer present */
} ed_atmosphere_info_t;

/* What was alive at a given time */
typedef struct {
    const char *life_forms[ED_MAX_ALIVE];
    int count;
    const char *dominant;    /* dominant life form description */
    int multicellular;       /* 1 if multicellular life exists */
    int land_life;           /* 1 if life on land */
    int intelligence;        /* 1 if tool-using intelligence exists */
} ed_biosphere_t;

/* Full dramatic snapshot of Earth at a given time */
typedef struct {
    double ma;                       /* query time in Ma */
    const char *era_name;            /* geological era */
    const char *period_name;         /* geological period */
    ed_atmosphere_info_t atmosphere;  /* atmosphere state */
    ed_biosphere_t biosphere;        /* what was alive */
    double temp_anomaly_c;           /* climate anomaly if known */
    int is_glaciated;                /* ice age active */
    int is_extinction;               /* mass extinction underway */
    const char *drama_text;          /* one-line dramatic summary */
} ed_snapshot_t;

/* Human arrival scale */
typedef struct {
    double earth_age_ma;             /* 4540 Ma */
    double human_age_ma;             /* ~0.3 Ma */
    double ratio;                    /* human_age / earth_age */
    double if_earth_were_24h;        /* humans arrive at this second before midnight */
    double if_earth_were_year;       /* humans arrive on this day (1-365) */
    const char *analogy;             /* descriptive analogy string */
} ed_human_scale_t;

/* Get dramatic snapshot of Earth at a time (Ma). */
ed_snapshot_t ed_snapshot_at(double ma);

/* Get atmosphere state at a time (Ma). */
ed_atmosphere_info_t ed_atmosphere_at(double ma);

/* Get biosphere state at a time (Ma). */
ed_biosphere_t ed_biosphere_at(double ma);

/* Human arrival scale computation. */
ed_human_scale_t ed_human_scale(void);

/* Get atmosphere state name string. */
const char *ed_atmosphere_name(ed_atmosphere_t state);

/* Number of atmosphere transition events. */
int ed_atmosphere_transition_count(void);

/* Get atmosphere transition by index (chronological). */
ed_atmosphere_info_t ed_atmosphere_transition_get(int index);

/* Dramatic one-liner for a time period. */
const char *ed_drama_text(double ma);

#endif /* TIME_EARTH_DRAMA_H */
