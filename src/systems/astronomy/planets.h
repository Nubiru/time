#ifndef TIME_PLANETS_H
#define TIME_PLANETS_H

/* Ecliptic position of a single planet. */
typedef struct {
    double longitude; /* ecliptic longitude (degrees, 0-360) */
    double latitude;  /* ecliptic latitude (degrees) */
    double distance;  /* distance from Sun (AU) */
} planet_position_t;

/* Positions of all 8 planets. */
typedef struct {
    planet_position_t positions[8]; /* Mercury=0 .. Neptune=7 */
} solar_system_t;

enum {
    PLANET_MERCURY = 0,
    PLANET_VENUS,
    PLANET_EARTH,
    PLANET_MARS,
    PLANET_JUPITER,
    PLANET_SATURN,
    PLANET_URANUS,
    PLANET_NEPTUNE
};

/* Compute heliocentric ecliptic positions for all 8 planets at a Julian Day. */
solar_system_t planets_at(double jd);

/* Human-readable planet name. Returns "Unknown" for invalid index. */
const char *planet_name(int index);

/* Unicode planet symbol. Returns "?" for invalid index. */
const char *planet_symbol(int index);

#endif
