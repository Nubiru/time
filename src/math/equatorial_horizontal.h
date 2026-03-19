/* equatorial_horizontal.h — Equatorial <-> Horizontal coordinate transform.
 *
 * Converts celestial RA/Dec to observer-relative altitude/azimuth and back.
 * Uses Meeus Chapter 13 formulas.
 *
 * All angles in degrees. Azimuth: 0=North, 90=East (compass convention).
 * Altitude: 0=horizon, +90=zenith, negative=below horizon.
 *
 * Pure module: no GL, no malloc, no globals. */

#ifndef TIME_EQUATORIAL_HORIZONTAL_H
#define TIME_EQUATORIAL_HORIZONTAL_H

#include "ecliptic.h" /* equatorial_t */

/* Horizontal coordinates */
typedef struct {
    double altitude; /* degrees above horizon (-90 to +90) */
    double azimuth;  /* compass bearing (0=N, 90=E, 180=S, 270=W, 0-360) */
} horizontal_t;

/* Hour angle from RA and Local Sidereal Time.
 * lst_deg: Local Sidereal Time in degrees (0-360).
 * ra_deg: Right Ascension in degrees (0-360).
 * Returns hour angle in degrees (0-360). */
double eh_hour_angle(double lst_deg, double ra_deg);

/* Convert equatorial (RA/Dec) to horizontal (alt/az).
 * eq: equatorial coordinates (RA and Dec in degrees).
 * observer_lat_deg: observer latitude (-90 to +90).
 * lst_deg: Local Sidereal Time in degrees (0-360).
 * Returns horizontal_t with altitude and azimuth in degrees. */
horizontal_t eh_to_horizontal(equatorial_t eq, double observer_lat_deg,
                              double lst_deg);

/* Convert horizontal (alt/az) back to equatorial (RA/Dec).
 * hz: horizontal coordinates.
 * observer_lat_deg: observer latitude.
 * lst_deg: Local Sidereal Time in degrees.
 * Returns equatorial_t with RA and Dec in degrees. */
equatorial_t eh_to_equatorial(horizontal_t hz, double observer_lat_deg,
                              double lst_deg);

/* Convenience: equatorial to horizontal using Julian Day (computes LST internally).
 * eq: equatorial coordinates.
 * observer_lat_deg: observer latitude.
 * observer_lon_deg: observer longitude (positive east).
 * jd: Julian Day.
 * Returns horizontal_t. */
horizontal_t eh_to_horizontal_jd(equatorial_t eq, double observer_lat_deg,
                                 double observer_lon_deg, double jd);

/* Is the object above the horizon? (altitude >= 0) */
int eh_is_visible(horizontal_t hz);

/* Atmospheric refraction correction (Bennett formula, Meeus Ch. 16).
 * altitude_deg: apparent (observed) altitude in degrees.
 * Returns correction in degrees (always positive). */
double eh_refraction(double altitude_deg);

#endif /* TIME_EQUATORIAL_HORIZONTAL_H */
