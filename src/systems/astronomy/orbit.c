#include "orbit.h"
#include "../../math/kepler.h"
#include <math.h>

#define PI 3.14159265358979323846
#define DEG_TO_RAD (PI / 180.0)
#define RAD_TO_DEG (180.0 / PI)

/* Reduce angle to 0-360 range. */
static double normalize_deg(double deg) {
    deg = fmod(deg, 360.0);
    if (deg < 0.0) deg += 360.0;
    return deg;
}

/* All orbital elements from JPL "Approximate Positions of the Planets"
 * Table 1 (valid 1800 AD – 2050 AD). Base at J2000.0, rates per Julian century.
 * Source: https://ssd.jpl.nasa.gov/planets/approx_pos.html */

const planet_orbit_t ORBIT_MERCURY = {
    .base = {
        .a       = 0.38709927,
        .e       = 0.20563593,
        .i       = 7.00497902,
        .omega_n = 48.33076593,
        .omega_p = 77.45779628,
        .L       = 252.25032350
    },
    .rate = {
        .a       = 0.00000037,
        .e       = 0.00001906,
        .i       = -0.00594749,
        .omega_n = -0.12534081,
        .omega_p = 0.16047689,
        .L       = 149472.67411175
    }
};

const planet_orbit_t ORBIT_VENUS = {
    .base = {
        .a       = 0.72333566,
        .e       = 0.00677672,
        .i       = 3.39467605,
        .omega_n = 76.67984255,
        .omega_p = 131.60246718,
        .L       = 181.97909950
    },
    .rate = {
        .a       = 0.00000390,
        .e       = -0.00004107,
        .i       = -0.00078890,
        .omega_n = -0.27769418,
        .omega_p = 0.00268329,
        .L       = 58517.81538729
    }
};

/* Earth orbital elements at J2000.0 (JPL Table 1).
 * Rates are per Julian century. */
const planet_orbit_t ORBIT_EARTH = {
    .base = {
        .a       = 1.00000261,
        .e       = 0.01671123,
        .i       = -0.00001531,
        .omega_n = 0.0,
        .omega_p = 102.93768193,
        .L       = 100.46457166
    },
    .rate = {
        .a       = 0.00000562,
        .e       = -0.00004392,
        .i       = -0.01294668,
        .omega_n = 0.0,
        .omega_p = 0.32327364,
        .L       = 35999.37244981
    }
};

const planet_orbit_t ORBIT_MARS = {
    .base = {
        .a       = 1.52371034,
        .e       = 0.09339410,
        .i       = 1.84969142,
        .omega_n = 49.55953891,
        .omega_p = -23.94362959,
        .L       = -4.55343205
    },
    .rate = {
        .a       = 0.00001847,
        .e       = 0.00007882,
        .i       = -0.00813131,
        .omega_n = -0.29257343,
        .omega_p = 0.44441088,
        .L       = 19140.30268499
    }
};

const planet_orbit_t ORBIT_JUPITER = {
    .base = {
        .a       = 5.20288700,
        .e       = 0.04838624,
        .i       = 1.30439695,
        .omega_n = 100.47390909,
        .omega_p = 14.72847983,
        .L       = 34.39644051
    },
    .rate = {
        .a       = -0.00011607,
        .e       = -0.00013253,
        .i       = -0.00183714,
        .omega_n = 0.20469106,
        .omega_p = 0.21252668,
        .L       = 3034.74612775
    }
};

const planet_orbit_t ORBIT_SATURN = {
    .base = {
        .a       = 9.53667594,
        .e       = 0.05386179,
        .i       = 2.48599187,
        .omega_n = 113.66242448,
        .omega_p = 92.59887831,
        .L       = 49.95424423
    },
    .rate = {
        .a       = -0.00125060,
        .e       = -0.00050991,
        .i       = 0.00193609,
        .omega_n = -0.28867794,
        .omega_p = -0.41897216,
        .L       = 1222.49362201
    }
};

const planet_orbit_t ORBIT_URANUS = {
    .base = {
        .a       = 19.18916464,
        .e       = 0.04725744,
        .i       = 0.77263783,
        .omega_n = 74.01692503,
        .omega_p = 170.95427630,
        .L       = 313.23810451
    },
    .rate = {
        .a       = -0.00196176,
        .e       = -0.00004397,
        .i       = -0.00242939,
        .omega_n = 0.04240589,
        .omega_p = 0.40805281,
        .L       = 428.48202785
    }
};

const planet_orbit_t ORBIT_NEPTUNE = {
    .base = {
        .a       = 30.06992276,
        .e       = 0.00859048,
        .i       = 1.77004347,
        .omega_n = 131.78422574,
        .omega_p = 44.96476227,
        .L       = -55.12002969
    },
    .rate = {
        .a       = 0.00026291,
        .e       = 0.00005105,
        .i       = -0.00508664,
        .omega_n = -0.00508664,
        .omega_p = -0.32241464,
        .L       = 218.45945325
    }
};

orbital_elements_t orbit_elements_at(const planet_orbit_t *planet, double jd) {
    double T = (jd - 2451545.0) / 36525.0; /* Julian centuries from J2000.0 */
    orbital_elements_t e;
    e.a       = planet->base.a       + planet->rate.a       * T;
    e.e       = planet->base.e       + planet->rate.e       * T;
    e.i       = planet->base.i       + planet->rate.i       * T;
    e.omega_n = planet->base.omega_n + planet->rate.omega_n * T;
    e.omega_p = planet->base.omega_p + planet->rate.omega_p * T;
    e.L       = planet->base.L       + planet->rate.L       * T;
    return e;
}

heliocentric_pos_t orbit_heliocentric(const orbital_elements_t *elem) {
    /* Card 014 Q2: Full calculation chain */
    heliocentric_pos_t pos;

    /* 1. Mean anomaly: M = L - omega_p (longitude of perihelion) */
    double M = normalize_deg(elem->L - elem->omega_p);
    double M_rad = M * DEG_TO_RAD;

    /* 2. Solve Kepler's equation for eccentric anomaly */
    double E = kepler_solve(M_rad, elem->e);

    /* 3. True anomaly */
    double v = eccentric_to_true_anomaly(E, elem->e);

    /* 4. Distance (radius vector) */
    double r = elem->a * (1.0 - elem->e * cos(E));

    /* 5. Argument of perihelion: omega = omega_p - omega_n */
    double omega = (elem->omega_p - elem->omega_n) * DEG_TO_RAD;
    double inc = elem->i * DEG_TO_RAD;

    /* 6. Argument of latitude: u = v + omega */
    double u = v + omega;

    /* 7. Heliocentric ecliptic coordinates (card 014 Q2 step 4) */
    double sin_u = sin(u);
    double cos_u = cos(u);
    double cos_i = cos(inc);
    double sin_i = sin(inc);

    /* Latitude: sin(psi) = sin(u) * sin(i) */
    pos.latitude = asin(sin_u * sin_i) * RAD_TO_DEG;

    /* Longitude: atan2(sin(u)*cos(i), cos(u)) + Omega */
    pos.longitude = normalize_deg(
        atan2(sin_u * cos_i, cos_u) * RAD_TO_DEG + elem->omega_n
    );

    /* Distance */
    pos.distance = r;

    return pos;
}
