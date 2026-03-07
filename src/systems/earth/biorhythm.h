/* biorhythm.h — Biological rhythms mapped to astronomical cycles.
 * Pure data + computation. No GL, no malloc, no globals, no side effects.
 *
 * Maps human biological rhythms (heartbeat, breath, circadian, menstrual,
 * sleep, blink, cell division, bone remodeling) to astronomical and sacred
 * numbers (72 = precession degrees, 432 = Kali Yuga, 108 = mala beads).
 *
 * Sources: Meeus (astronomical cycles), various biomedical references.
 */

#ifndef TIME_BIORHYTHM_H
#define TIME_BIORHYTHM_H

#include <stdbool.h>

#define BIORHYTHM_CYCLE_COUNT 8
#define BIORHYTHM_CORRELATION_COUNT 6

/* Biological rhythm types */
typedef enum {
    BIO_HEARTBEAT = 0,
    BIO_BREATH,
    BIO_CIRCADIAN,
    BIO_MENSTRUAL,
    BIO_SLEEP_ULTRADIAN,   /* ~90 min sleep cycle */
    BIO_BLINK,             /* ~15-20 per minute */
    BIO_CELL_DIVISION,     /* ~24h for most cells */
    BIO_BONE_REMODEL       /* ~120 days */
} bio_cycle_t;

/* A biological rhythm */
typedef struct {
    bio_cycle_t type;
    const char *name;
    double rate_per_minute;   /* average events per minute */
    double period_hours;      /* period in hours (for cycles, not rates) */
    const char *description;
} bio_rhythm_t;

/* An astronomical/sacred number correlation */
typedef struct {
    const char *bio_description;     /* e.g., "72 bpm heartbeat" */
    const char *astro_description;   /* e.g., "72 years = 1 degree of precession" */
    double bio_value;
    double astro_value;
    double ratio;                    /* bio/astro or astro/bio, whichever is closer to integer */
    const char *significance;
} bio_correlation_t;

/* Get a biological rhythm by type. */
bio_rhythm_t biorhythm_get(bio_cycle_t type);

/* How many biological cycles in the catalog. */
int biorhythm_cycle_count(void);

/* Total heartbeats over a duration in hours. Default 72 bpm. */
double biorhythm_heartbeats(double hours);

/* Total breaths over a duration in hours. Default 15 per minute. */
double biorhythm_breaths(double hours);

/* Heartbeats per day at given bpm. */
double biorhythm_beats_per_day(double bpm);

/* Breaths per day at given rate per minute. */
double biorhythm_breaths_per_day(double rate_per_min);

/* Get a bio-astro correlation by index. */
bio_correlation_t biorhythm_correlation_get(int index);

/* How many correlations in the catalog. */
int biorhythm_correlation_count(void);

/* Match a biological rate against an astronomical cycle length.
 * Returns ratio and whether it's "close" to a sacred number.
 * Returns the nearest sacred number (7, 12, 27, 64, 72, 108, 432, etc.)
 * or 0 if no close match. */
int biorhythm_match_sacred(double bio_value, double astro_value);

/* Human lifespan in heartbeats/breaths at average rates.
 * years: lifespan in years */
double biorhythm_lifetime_heartbeats(double years);
double biorhythm_lifetime_breaths(double years);

/* Name lookup for bio_cycle_t */
const char *biorhythm_cycle_name(bio_cycle_t type);

#endif /* TIME_BIORHYTHM_H */
