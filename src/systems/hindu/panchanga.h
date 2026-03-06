/* panchanga.h -- Panchanga (five-limbed Hindu calendar) calculator
 * Computes the five daily elements: Tithi, Vara, Nakshatra, Yoga, Karana.
 * All functions are pure (no GL, no malloc, no globals, no side effects). */

#ifndef TIME_PANCHANGA_H
#define TIME_PANCHANGA_H

#include <stdbool.h>

#define PANCHANGA_TITHI_COUNT  30
#define PANCHANGA_VARA_COUNT   7
#define PANCHANGA_YOGA_COUNT   27
#define PANCHANGA_KARANA_COUNT 11

typedef struct {
    int number;          /* 1-30 */
    const char *name;    /* Sanskrit name */
    const char *paksha;  /* "Shukla" or "Krishna" */
    int paksha_day;      /* 1-15 */
} tithi_t;

typedef struct {
    int number;          /* 0=Sunday..6=Saturday */
    const char *name;    /* Sanskrit: Ravivara, Somavara... */
    const char *deity;   /* Surya, Chandra... */
    const char *planet;  /* Sun, Moon... */
} vara_t;

typedef struct {
    int number;          /* 0-26 */
    const char *name;    /* Vishkambha, Priti... */
    const char *quality; /* "Auspicious"/"Inauspicious"/"Neutral" */
} yoga_t;

typedef struct {
    int number;          /* 0-10 */
    const char *name;    /* Bava, Balava... */
    bool is_fixed;       /* true for 4 fixed karanas */
} karana_t;

typedef struct {
    tithi_t tithi;
    vara_t vara;
    int nakshatra_index; /* 0-26 */
    yoga_t yoga;
    karana_t karana;
} panchanga_t;

/* Compute full Panchanga for a given Julian Day and tropical longitudes. */
panchanga_t panchanga_compute(double jd, double sun_longitude,
                              double moon_longitude);

/* Individual limb calculators. */
tithi_t panchanga_tithi(double elongation);
vara_t panchanga_vara(double jd);
yoga_t panchanga_yoga(double sun_sidereal, double moon_sidereal);
karana_t panchanga_karana(double elongation);

/* Name lookups. */
const char *panchanga_tithi_name(int tithi_number);
const char *panchanga_vara_name(int vara_number);
const char *panchanga_yoga_name(int yoga_number);
const char *panchanga_karana_name(int karana_number);

/* Count accessors. */
int panchanga_tithi_count(void);
int panchanga_vara_count(void);
int panchanga_yoga_count(void);
int panchanga_karana_count(void);

#endif /* TIME_PANCHANGA_H */
