/* earth_fraction.h — Awe-inspiring fractions: human existence vs Earth/cosmos.
 * "Humans have existed for 0.0066% of Earth's history."
 * "If Earth's history were 24 hours, humans appear at 23:59:56."
 *
 * Pure functions: no GL, no malloc, no globals, no side effects.
 * Data: Earth age 4.54 Ga, Homo sapiens ~300 ka, PRB population estimates. */

#ifndef TIME_EARTH_FRACTION_H
#define TIME_EARTH_FRACTION_H

#define EF_NAME_MAX    64
#define EF_DESC_MAX    256
#define EF_UNIT_MAX    32
#define EF_ANALOGY_MAX 256

/* A single awe-inspiring fraction/fact about human existence on Earth. */
typedef struct {
    const char *fact_name;       /* e.g., "human_fraction" */
    const char *description;     /* e.g., "Humans have existed for 0.0066% ..." */
    double      value;           /* the fraction or number */
    const char *unit;            /* e.g., "%", "years", "people" */
    const char *visual_analogy;  /* e.g., "If Earth's history were 24 hours..." */
} earth_fraction_t;

/* Number of facts in the catalog (at least 8). */
int ef_fact_count(void);

/* Get fact by index (0-based). Returns zeroed struct if out of range. */
earth_fraction_t ef_fact_get(int index);

/* Human existence as percentage of Earth's age (~0.0066). */
double ef_human_fraction(void);

/* Total humans estimated to have ever been born (~109e9). */
double ef_humans_ever_born(void);

/* Current world population (~8.1e9). */
double ef_current_population(void);

/* Percentage of all humans ever born who are alive now (~7.4). */
double ef_alive_fraction(void);

/* Age of Earth in years (~4.54e9). */
double ef_earth_age_years(void);

/* Age of Homo sapiens in years (~300,000). */
double ef_human_age_years(void);

/* Seconds before midnight that humans appear on a 24-hour Earth clock (~5.7). */
double ef_clock_analogy_seconds(void);

#endif /* TIME_EARTH_FRACTION_H */
