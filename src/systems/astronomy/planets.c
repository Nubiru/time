#include "planets.h"
#include "orbit.h"

static const planet_orbit_t *PLANET_ORBITS[8] = {
    &ORBIT_MERCURY, &ORBIT_VENUS, &ORBIT_EARTH, &ORBIT_MARS,
    &ORBIT_JUPITER, &ORBIT_SATURN, &ORBIT_URANUS, &ORBIT_NEPTUNE
};

static const char *PLANET_NAMES[8] = {
    "Mercury", "Venus", "Earth", "Mars",
    "Jupiter", "Saturn", "Uranus", "Neptune"
};

/* UTF-8 encoded Unicode symbols */
static const char *PLANET_SYMBOLS[8] = {
    "\xe2\x98\xbf", /* Mercury ☿ */
    "\xe2\x99\x80", /* Venus   ♀ */
    "\xe2\x8a\x95", /* Earth   ⊕ */
    "\xe2\x99\x82", /* Mars    ♂ */
    "\xe2\x99\x83", /* Jupiter ♃ */
    "\xe2\x99\x84", /* Saturn  ♄ */
    "\xe2\x99\x85", /* Uranus  ♅ */
    "\xe2\x99\x86"  /* Neptune ♆ */
};

solar_system_t planets_at(double jd) {
    solar_system_t sys;
    for (int i = 0; i < 8; i++) {
        orbital_elements_t elem = orbit_elements_at(PLANET_ORBITS[i], jd);
        heliocentric_pos_t pos = orbit_heliocentric(&elem);
        sys.positions[i].longitude = pos.longitude;
        sys.positions[i].latitude = pos.latitude;
        sys.positions[i].distance = pos.distance;
    }
    return sys;
}

const char *planet_name(int index) {
    if (index < 0 || index > 7) return "Unknown";
    return PLANET_NAMES[index];
}

const char *planet_symbol(int index) {
    if (index < 0 || index > 7) return "?";
    return PLANET_SYMBOLS[index];
}
