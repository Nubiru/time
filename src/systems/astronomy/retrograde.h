#ifndef TIME_RETROGRADE_H
#define TIME_RETROGRADE_H

typedef enum {
    MOTION_DIRECT = 0,
    MOTION_STATIONARY,
    MOTION_RETROGRADE
} planet_motion_t;

typedef struct {
    planet_motion_t motion;
    double daily_motion;   /* degrees/day (negative = retrograde) */
    int planet;            /* planet index (0-7) */
} retrograde_info_t;

/* Check retrograde status for a planet at a given JD.
 * Computes geocentric longitude at jd and jd+1, compares.
 * planet: 0=Mercury .. 7=Neptune (uses planets_at). */
retrograde_info_t retrograde_check(int planet, double jd);

/* Is the planet retrograde at this JD? Convenience wrapper. */
int is_retrograde(int planet, double jd);

/* Geocentric daily motion in degrees/day (positive=direct, negative=retrograde). */
double planet_daily_motion(int planet, double jd);

/* Motion type name: "Direct", "Stationary", "Retrograde". */
const char *motion_name(planet_motion_t motion);

/* Retrograde symbol. */
const char *motion_symbol(planet_motion_t motion);

/* Stationary threshold in degrees/day.
 * Below this absolute rate, planet is considered stationary. */
double retrograde_stationary_threshold(int planet);

#endif
