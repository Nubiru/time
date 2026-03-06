/* tidal.h — Tidal harmonic constants and force computation.
 * Models gravitational tide-generating forces from Moon and Sun,
 * spring/neap/king tide detection, 5-constituent harmonic height.
 * Data source: NOAA tidal constituent tables, Doodson (1921). */

#ifndef TIME_TIDAL_H
#define TIME_TIDAL_H

#define PI 3.14159265358979323846

/* Major tidal constituents — periods in hours */
#define TIDAL_M2_PERIOD_H  12.4206   /* Principal lunar semidiurnal */
#define TIDAL_S2_PERIOD_H  12.0000   /* Principal solar semidiurnal */
#define TIDAL_N2_PERIOD_H  12.6583   /* Larger lunar elliptic */
#define TIDAL_K1_PERIOD_H  23.9345   /* Luni-solar diurnal */
#define TIDAL_O1_PERIOD_H  25.8193   /* Principal lunar diurnal */

/* Relative amplitudes (M2 = 1.0 reference) */
#define TIDAL_M2_AMP  1.0000
#define TIDAL_S2_AMP  0.4652
#define TIDAL_N2_AMP  0.1915
#define TIDAL_K1_AMP  0.5842
#define TIDAL_O1_AMP  0.4151

#define TIDAL_CONSTITUENT_COUNT 5

/* Tidal constituent data */
typedef struct {
    const char *name;        /* e.g., "M2" */
    const char *description; /* e.g., "Principal lunar semidiurnal" */
    double period_hours;     /* period in hours */
    double amplitude;        /* relative amplitude (M2 = 1.0) */
} tidal_constituent_t;

/* Tidal force result */
typedef struct {
    double lunar_force;      /* Moon's tide-generating force (relative) */
    double solar_force;      /* Sun's tide-generating force (relative) */
    double total_force;      /* Combined force */
    double lunar_fraction;   /* Moon's share of total (typically ~0.69) */
} tidal_force_t;

/* Tidal state at a moment */
typedef struct {
    double height;           /* Approximate relative height */
    int is_spring;           /* 1 if spring tide (new/full moon) */
    int is_neap;             /* 1 if neap tide (quarter moon) */
    int is_king;             /* 1 if king tide (spring + perigee) */
    double spring_neap;      /* 0.0 = neap, 1.0 = spring (continuous) */
} tidal_state_t;

/* Get tidal constituent by index (0 to TIDAL_CONSTITUENT_COUNT-1).
 * Returns zeroed struct with NULL name for invalid index. */
tidal_constituent_t tidal_constituent_get(int index);

/* Constituent count (always 5). */
int tidal_constituent_count(void);

/* Compute tide-generating force from Moon and Sun distances.
 * moon_dist_km: Moon's distance in km (~356000 to ~407000).
 * sun_dist_km: Sun's distance in km (~147e6 to ~152e6).
 * Forces scale as 1/r^3 relative to mean distances. */
tidal_force_t tidal_force(double moon_dist_km, double sun_dist_km);

/* Approximate tidal height from 5-constituent harmonic sum.
 * hours_since_epoch: hours since J2000.0 (for phase calculation).
 * Returns value in range approximately -2.66 to +2.66. */
double tidal_harmonic_height(double hours_since_epoch);

/* Spring-neap factor from Moon phase angle.
 * phase_angle_deg: Moon-Sun elongation (0=new, 180=full, 90/270=quarter).
 * Returns 0.0 (neap) to 1.0 (spring). Formula: cos^2(phase). */
double tidal_spring_neap_factor(double phase_angle_deg);

/* Is this a spring tide? (phase within 30 deg of 0 or 180). */
int tidal_is_spring(double phase_angle_deg);

/* Is this a neap tide? (phase within 30 deg of 90 or 270). */
int tidal_is_neap(double phase_angle_deg);

/* Is this a king tide? Spring tide + Moon near perigee (<356500 km).
 * moon_dist_km: current Moon distance.
 * phase_angle_deg: current Moon-Sun elongation. */
int tidal_is_king(double moon_dist_km, double phase_angle_deg);

/* Compute full tidal state.
 * hours_since_epoch: hours since J2000.0.
 * moon_dist_km: Moon distance.
 * sun_dist_km: Sun distance.
 * phase_angle_deg: Moon-Sun elongation. */
tidal_state_t tidal_state(double hours_since_epoch,
                          double moon_dist_km, double sun_dist_km,
                          double phase_angle_deg);

/* Approximate hours until next spring tide from current phase angle.
 * Returns hours until phase reaches next 0 or 180 deg target. */
double tidal_hours_to_next_spring(double phase_angle_deg);

/* Approximate hours until next neap tide from current phase angle.
 * Returns hours until phase reaches next 90 or 270 deg target. */
double tidal_hours_to_next_neap(double phase_angle_deg);

#endif /* TIME_TIDAL_H */
