#ifndef TIME_COSMIC_TIME_H
#define TIME_COSMIC_TIME_H

#include <stddef.h>

typedef struct {
    double universe_age_years;      /* ~13.787 billion years */
    double sun_age_years;           /* ~4.603 billion years */
    double earth_age_years;         /* ~4.543 billion years */
    double galactic_year_years;     /* ~225-250 million years per orbit */
    double sun_galactic_orbits;     /* how many times Sun has orbited galaxy */
    double sun_galactic_phase;      /* 0.0-1.0: position in current galactic orbit */
} cosmic_context_t;

/* Get cosmic context. Derived values depend on current JD. */
cosmic_context_t cosmic_context(double jd);

/* Format years to human-readable scale string.
 * e.g. 13.787e9 -> "13.787 Gyr", 4.5e6 -> "4.500 Myr" */
void cosmic_format_age(double years, char *buf, size_t sz);

/* Geological era name from years-before-present.
 * Returns best-match era name. */
const char *cosmic_geological_era(double years_bp);

/* Time since Big Bang as fraction of universe age (0=Big Bang, 1=now). */
double cosmic_universe_fraction(double jd);

/* Distance label for a given camera distance (scene units). */
void cosmic_distance_label(float distance, char *buf, size_t sz);

/* Time unit constants. */
double cosmic_seconds_per_day(void);
double cosmic_seconds_per_year(void);
double cosmic_days_per_year(void);

#endif
