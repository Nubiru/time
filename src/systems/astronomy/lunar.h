#ifndef TIME_LUNAR_H
#define TIME_LUNAR_H

typedef enum {
    MOON_NEW = 0,
    MOON_WAXING_CRESCENT,
    MOON_FIRST_QUARTER,
    MOON_WAXING_GIBBOUS,
    MOON_FULL,
    MOON_WANING_GIBBOUS,
    MOON_LAST_QUARTER,
    MOON_WANING_CRESCENT
} moon_phase_t;

typedef struct {
    double phase_fraction;    /* 0.0 (new) to 1.0 (next new) */
    double illumination;      /* 0.0 (dark) to 1.0 (full) */
    double moon_longitude;    /* approximate ecliptic longitude, degrees */
    moon_phase_t phase;       /* which of the 8 phases */
    double age_days;          /* days since last new moon */
} lunar_info_t;

/* Core */
lunar_info_t lunar_phase(double jd);

/* Names and symbols */
const char *lunar_phase_name(moon_phase_t phase);
const char *lunar_phase_symbol(moon_phase_t phase);

/* Queries */
double lunar_next_new_moon(double jd);
double lunar_next_full_moon(double jd);
int    lunar_zodiac_sign(double jd);

#endif
