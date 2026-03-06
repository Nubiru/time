#ifndef TIME_ORBIT_H
#define TIME_ORBIT_H

/* Orbital elements for a planet at a given epoch.
 * All angles in degrees. */
typedef struct {
    double a;       /* semimajor axis (AU) */
    double e;       /* eccentricity */
    double i;       /* inclination (degrees) */
    double omega_n; /* longitude of ascending node (degrees) */
    double omega_p; /* longitude of perihelion (degrees) */
    double L;       /* mean longitude (degrees) */
} orbital_elements_t;

/* Rates of change per Julian century. */
typedef struct {
    orbital_elements_t base; /* elements at epoch J2000.0 */
    orbital_elements_t rate; /* change per century */
} planet_orbit_t;

/* Heliocentric position in ecliptic coordinates. */
typedef struct {
    double longitude; /* ecliptic longitude (degrees) */
    double latitude;  /* ecliptic latitude (degrees) */
    double distance;  /* distance from Sun (AU) */
} heliocentric_pos_t;

/* Get orbital elements for a planet at a given Julian Day.
 * Interpolates from J2000.0 base + rate. */
orbital_elements_t orbit_elements_at(const planet_orbit_t *planet, double jd);

/* Compute heliocentric ecliptic position from orbital elements. */
heliocentric_pos_t orbit_heliocentric(const orbital_elements_t *elem);

/* Pre-defined planet orbits (J2000.0 epoch, JPL Table 1: 1800-2050 AD). */
extern const planet_orbit_t ORBIT_MERCURY;
extern const planet_orbit_t ORBIT_VENUS;
extern const planet_orbit_t ORBIT_EARTH;
extern const planet_orbit_t ORBIT_MARS;
extern const planet_orbit_t ORBIT_JUPITER;
extern const planet_orbit_t ORBIT_SATURN;
extern const planet_orbit_t ORBIT_URANUS;
extern const planet_orbit_t ORBIT_NEPTUNE;

#define ORBIT_PLANET_COUNT 8

#endif
