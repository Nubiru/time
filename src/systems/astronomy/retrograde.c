#include "retrograde.h"
#include "planets.h"

#include <math.h>

#define PI 3.14159265358979323846
#define DEG2RAD (PI / 180.0)
#define RAD2DEG (180.0 / PI)
#define PLANET_EARTH 2

/* Stationary thresholds in degrees/day */
static const double THRESHOLDS[8] = {
    0.05,   /* Mercury */
    0.03,   /* Venus */
    0.0,    /* Earth — never retrograde from own perspective */
    0.02,   /* Mars */
    0.005,  /* Jupiter */
    0.003,  /* Saturn */
    0.001,  /* Uranus */
    0.001   /* Neptune */
};

/* Normalize angle to 0-360 */
static double normalize(double deg)
{
    deg = fmod(deg, 360.0);
    if (deg < 0.0) deg += 360.0;
    return deg;
}

/* Compute geocentric ecliptic longitude of a planet.
 * Transforms heliocentric positions of planet and Earth to get
 * the apparent longitude as seen from Earth. */
static double geocentric_longitude(int planet, double jd)
{
    solar_system_t sys = planets_at(jd);

    if (planet == PLANET_EARTH) {
        /* Earth's "geocentric" longitude doesn't apply; return heliocentric */
        return sys.positions[PLANET_EARTH].longitude;
    }

    /* Heliocentric cartesian coordinates */
    double lon_p = sys.positions[planet].longitude * DEG2RAD;
    double dist_p = sys.positions[planet].distance;
    double xp = dist_p * cos(lon_p);
    double yp = dist_p * sin(lon_p);

    double lon_e = sys.positions[PLANET_EARTH].longitude * DEG2RAD;
    double dist_e = sys.positions[PLANET_EARTH].distance;
    double xe = dist_e * cos(lon_e);
    double ye = dist_e * sin(lon_e);

    /* Geocentric vector */
    double dx = xp - xe;
    double dy = yp - ye;

    double geo_lon = atan2(dy, dx) * RAD2DEG;
    return normalize(geo_lon);
}

double planet_daily_motion(int planet, double jd)
{
    if (planet < 0 || planet > 7) return 0.0;

    double lon1 = geocentric_longitude(planet, jd);
    double lon2 = geocentric_longitude(planet, jd + 1.0);

    double delta = lon2 - lon1;
    if (delta > 180.0) delta -= 360.0;
    if (delta < -180.0) delta += 360.0;

    return delta;
}

retrograde_info_t retrograde_check(int planet, double jd)
{
    retrograde_info_t info;
    info.planet = planet;

    if (planet < 0 || planet > 7) {
        info.motion = MOTION_DIRECT;
        info.daily_motion = 0.0;
        return info;
    }

    info.daily_motion = planet_daily_motion(planet, jd);
    double threshold = THRESHOLDS[planet];

    if (fabs(info.daily_motion) <= threshold) {
        info.motion = MOTION_STATIONARY;
    } else if (info.daily_motion < 0.0) {
        info.motion = MOTION_RETROGRADE;
    } else {
        info.motion = MOTION_DIRECT;
    }

    return info;
}

int is_retrograde(int planet, double jd)
{
    retrograde_info_t info = retrograde_check(planet, jd);
    return info.motion == MOTION_RETROGRADE;
}

const char *motion_name(planet_motion_t motion)
{
    switch (motion) {
        case MOTION_DIRECT:      return "Direct";
        case MOTION_STATIONARY:  return "Stationary";
        case MOTION_RETROGRADE:  return "Retrograde";
    }
    return "Direct";
}

const char *motion_symbol(planet_motion_t motion)
{
    switch (motion) {
        case MOTION_DIRECT:      return "D";
        case MOTION_STATIONARY:  return "S";
        case MOTION_RETROGRADE:  return "\xe2\x84\x9e"; /* ℞ */
    }
    return "D";
}

double retrograde_stationary_threshold(int planet)
{
    if (planet < 0 || planet > 7) return 0.0;
    return THRESHOLDS[planet];
}
