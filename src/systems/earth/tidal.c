/* tidal.c — Tidal harmonic constants and force computation.
 * Pure functions only. No malloc, no globals, no side effects.
 * Data source: NOAA tidal constituent tables, Doodson (1921). */

#include "tidal.h"

#include <math.h>
#include <stddef.h>

/* Mean orbital distances in km */
static const double MEAN_MOON_DIST_KM = 384400.0;
static const double MEAN_SUN_DIST_KM  = 149597870.7;

/* Moon perigee threshold for king tides (km) */
static const double KING_TIDE_PERIGEE_KM = 356500.0;

/* Spring/neap detection threshold (degrees from target) */
static const double SPRING_NEAP_THRESHOLD_DEG = 30.0;

/* Moon phase advance rate (degrees per hour) */
static const double MOON_PHASE_RATE_DEG_H = 360.0 / (29.530589 * 24.0);

/* Tidal force ratio: at mean distances, lunar/solar ~ 2.17.
 * We normalize so that at mean distances lunar_force = 2.17, solar_force = 1.0. */
static const double LUNAR_FORCE_REF = 2.17;
static const double SOLAR_FORCE_REF = 1.0;

/* Static constituent table */
static const tidal_constituent_t CONSTITUENTS[TIDAL_CONSTITUENT_COUNT] = {
    { "M2", "Principal lunar semidiurnal",  TIDAL_M2_PERIOD_H, TIDAL_M2_AMP },
    { "S2", "Principal solar semidiurnal",  TIDAL_S2_PERIOD_H, TIDAL_S2_AMP },
    { "N2", "Larger lunar elliptic",        TIDAL_N2_PERIOD_H, TIDAL_N2_AMP },
    { "K1", "Luni-solar diurnal",           TIDAL_K1_PERIOD_H, TIDAL_K1_AMP },
    { "O1", "Principal lunar diurnal",      TIDAL_O1_PERIOD_H, TIDAL_O1_AMP }
};

/* Normalize angle to [0, 360) */
static double normalize_deg(double deg)
{
    deg = fmod(deg, 360.0);
    if (deg < 0.0) {
        deg += 360.0;
    }
    return deg;
}

/* Angular distance between two angles on a circle (0-180) */
static double angular_dist_deg(double a, double b)
{
    double diff = fabs(normalize_deg(a) - normalize_deg(b));
    if (diff > 180.0) {
        diff = 360.0 - diff;
    }
    return diff;
}

tidal_constituent_t tidal_constituent_get(int index)
{
    if (index < 0 || index >= TIDAL_CONSTITUENT_COUNT) {
        tidal_constituent_t empty = { NULL, NULL, 0.0, 0.0 };
        return empty;
    }
    return CONSTITUENTS[index];
}

int tidal_constituent_count(void)
{
    return TIDAL_CONSTITUENT_COUNT;
}

tidal_force_t tidal_force(double moon_dist_km, double sun_dist_km)
{
    /* Force scales as (mean_dist / actual_dist)^3 relative to reference */
    double moon_ratio = MEAN_MOON_DIST_KM / moon_dist_km;
    double sun_ratio  = MEAN_SUN_DIST_KM / sun_dist_km;

    double lunar = LUNAR_FORCE_REF * moon_ratio * moon_ratio * moon_ratio;
    double solar = SOLAR_FORCE_REF * sun_ratio * sun_ratio * sun_ratio;
    double total = lunar + solar;

    tidal_force_t result;
    result.lunar_force = lunar;
    result.solar_force = solar;
    result.total_force = total;
    result.lunar_fraction = (total > 0.0) ? lunar / total : 0.0;

    return result;
}

double tidal_harmonic_height(double hours_since_epoch)
{
    /* Sum of 5 sinusoidal constituents.
     * h = sum( amplitude_i * cos(2*PI * hours / period_i) )
     * All initial phases are 0 (simplified model). */
    double height = 0.0;
    for (int i = 0; i < TIDAL_CONSTITUENT_COUNT; i++) {
        double omega = 2.0 * PI / CONSTITUENTS[i].period_hours;
        height += CONSTITUENTS[i].amplitude * cos(omega * hours_since_epoch);
    }
    return height;
}

double tidal_spring_neap_factor(double phase_angle_deg)
{
    /* cos^2(phase): max at 0 and 180, min at 90 and 270 */
    double phase_rad = normalize_deg(phase_angle_deg) * PI / 180.0;
    double c = cos(phase_rad);
    return c * c;
}

int tidal_is_spring(double phase_angle_deg)
{
    double angle = normalize_deg(phase_angle_deg);
    double dist_to_0   = angular_dist_deg(angle, 0.0);
    double dist_to_180 = angular_dist_deg(angle, 180.0);
    return (dist_to_0 <= SPRING_NEAP_THRESHOLD_DEG ||
            dist_to_180 <= SPRING_NEAP_THRESHOLD_DEG) ? 1 : 0;
}

int tidal_is_neap(double phase_angle_deg)
{
    double angle = normalize_deg(phase_angle_deg);
    double dist_to_90  = angular_dist_deg(angle, 90.0);
    double dist_to_270 = angular_dist_deg(angle, 270.0);
    return (dist_to_90 <= SPRING_NEAP_THRESHOLD_DEG ||
            dist_to_270 <= SPRING_NEAP_THRESHOLD_DEG) ? 1 : 0;
}

int tidal_is_king(double moon_dist_km, double phase_angle_deg)
{
    return (tidal_is_spring(phase_angle_deg) &&
            moon_dist_km < KING_TIDE_PERIGEE_KM) ? 1 : 0;
}

tidal_state_t tidal_state(double hours_since_epoch,
                          double moon_dist_km, double sun_dist_km,
                          double phase_angle_deg)
{
    (void)sun_dist_km;   /* available for future force-weighted height */

    tidal_state_t s;
    s.height      = tidal_harmonic_height(hours_since_epoch);
    s.is_spring   = tidal_is_spring(phase_angle_deg);
    s.is_neap     = tidal_is_neap(phase_angle_deg);
    s.is_king     = tidal_is_king(moon_dist_km, phase_angle_deg);
    s.spring_neap = tidal_spring_neap_factor(phase_angle_deg);
    return s;
}

double tidal_hours_to_next_spring(double phase_angle_deg)
{
    /* Spring targets: 0 and 180. Find angular distance to each,
     * take the minimum, convert to hours via Moon's phase rate.
     * "Next" means strictly forward — current position excluded
     * by computing distance to the NEXT target after current. */
    double angle = normalize_deg(phase_angle_deg);

    /* Targets the Moon phase will reach going forward: 180, then 360/0 */
    double dist_to_180 = normalize_deg(180.0 - angle);
    double dist_to_360 = normalize_deg(360.0 - angle);

    /* Pick the nearer forward target (both are > 0 by normalize) */
    double min_dist = dist_to_180;
    if (dist_to_360 < min_dist) {
        min_dist = dist_to_360;
    }

    /* If we're exactly on a spring target, next is half a synodic month away */
    if (min_dist < 0.001) {
        min_dist = 180.0;
    }

    return min_dist / MOON_PHASE_RATE_DEG_H;
}

double tidal_hours_to_next_neap(double phase_angle_deg)
{
    /* Neap targets: 90 and 270. */
    double angle = normalize_deg(phase_angle_deg);

    double dist_to_90  = normalize_deg(90.0 - angle);
    double dist_to_270 = normalize_deg(270.0 - angle);

    double min_dist = dist_to_90;
    if (dist_to_270 < min_dist) {
        min_dist = dist_to_270;
    }

    /* If exactly on a neap target, next is half a synodic month away */
    if (min_dist < 0.001) {
        min_dist = 180.0;
    }

    return min_dist / MOON_PHASE_RATE_DEG_H;
}
