/* moon_data.c -- Orbital elements and position calculations for 9 major moons.
 * Data source: NASA/JPL Planetary Satellite Physical Parameters (public domain).
 * All functions are pure (no side effects, no global mutation). */

#include "moon_data.h"
#include "../../math/kepler.h"
#include <math.h>
#include <stddef.h>

#define PI 3.14159265358979323846
#define DEG_TO_RAD (PI / 180.0)
#define RAD_TO_DEG (180.0 / PI)
#define J2000_JD 2451545.0

/* ---------- Static moon catalog ---------- */

/* Earth moons (1) */
static const moon_entry_t EARTH_MOONS[MOON_DATA_EARTH_COUNT] = {
    {
        .name = "Moon",
        .orbit = {
            .a_km           = 384400.0,
            .e              = 0.0549,
            .i_deg          = 5.145,
            .period_days    = 27.322,
            .mean_lon_epoch = 218.32,
            .radius_km      = 1737.4,
            .albedo         = 0.136,
            .parent_planet  = 2 /* PLANET_EARTH */
        }
    }
};

/* Mars moons (2) */
static const moon_entry_t MARS_MOONS[MOON_DATA_MARS_COUNT] = {
    {
        .name = "Phobos",
        .orbit = {
            .a_km           = 9376.0,
            .e              = 0.0151,
            .i_deg          = 1.093,
            .period_days    = 0.3189,
            .mean_lon_epoch = 35.0,
            .radius_km      = 11.267,
            .albedo         = 0.071,
            .parent_planet  = 3 /* PLANET_MARS */
        }
    },
    {
        .name = "Deimos",
        .orbit = {
            .a_km           = 23458.0,
            .e              = 0.0002,
            .i_deg          = 0.93,
            .period_days    = 1.2624,
            .mean_lon_epoch = 162.0,
            .radius_km      = 6.2,
            .albedo         = 0.068,
            .parent_planet  = 3 /* PLANET_MARS */
        }
    }
};

/* Jupiter moons (4 Galilean) */
static const moon_entry_t JUPITER_MOONS[MOON_DATA_JUPITER_COUNT] = {
    {
        .name = "Io",
        .orbit = {
            .a_km           = 421700.0,
            .e              = 0.0041,
            .i_deg          = 0.050,
            .period_days    = 1.7691,
            .mean_lon_epoch = 106.1,
            .radius_km      = 1821.6,
            .albedo         = 0.63,
            .parent_planet  = 4 /* PLANET_JUPITER */
        }
    },
    {
        .name = "Europa",
        .orbit = {
            .a_km           = 671034.0,
            .e              = 0.0094,
            .i_deg          = 0.471,
            .period_days    = 3.5512,
            .mean_lon_epoch = 176.3,
            .radius_km      = 1560.8,
            .albedo         = 0.67,
            .parent_planet  = 4 /* PLANET_JUPITER */
        }
    },
    {
        .name = "Ganymede",
        .orbit = {
            .a_km           = 1070412.0,
            .e              = 0.0013,
            .i_deg          = 0.204,
            .period_days    = 7.1546,
            .mean_lon_epoch = 121.0,
            .radius_km      = 2634.1,
            .albedo         = 0.43,
            .parent_planet  = 4 /* PLANET_JUPITER */
        }
    },
    {
        .name = "Callisto",
        .orbit = {
            .a_km           = 1882709.0,
            .e              = 0.0074,
            .i_deg          = 0.205,
            .period_days    = 16.689,
            .mean_lon_epoch = 52.6,
            .radius_km      = 2410.3,
            .albedo         = 0.17,
            .parent_planet  = 4 /* PLANET_JUPITER */
        }
    }
};

/* Saturn moons (2) */
static const moon_entry_t SATURN_MOONS[MOON_DATA_SATURN_COUNT] = {
    {
        .name = "Titan",
        .orbit = {
            .a_km           = 1221870.0,
            .e              = 0.0288,
            .i_deg          = 0.348,
            .period_days    = 15.945,
            .mean_lon_epoch = 15.0,
            .radius_km      = 2574.7,
            .albedo         = 0.22,
            .parent_planet  = 5 /* PLANET_SATURN */
        }
    },
    {
        .name = "Enceladus",
        .orbit = {
            .a_km           = 237948.0,
            .e              = 0.0047,
            .i_deg          = 0.009,
            .period_days    = 1.3702,
            .mean_lon_epoch = 200.0,
            .radius_km      = 252.1,
            .albedo         = 0.99,
            .parent_planet  = 5 /* PLANET_SATURN */
        }
    }
};

/* Lookup table: planet index -> (array pointer, count) */

typedef struct {
    const moon_entry_t *moons;
    int count;
} moon_catalog_entry_t;

static const moon_catalog_entry_t CATALOG[8] = {
    { NULL,          0 },                       /* Mercury */
    { NULL,          0 },                       /* Venus */
    { EARTH_MOONS,   MOON_DATA_EARTH_COUNT },   /* Earth */
    { MARS_MOONS,    MOON_DATA_MARS_COUNT },     /* Mars */
    { JUPITER_MOONS, MOON_DATA_JUPITER_COUNT },  /* Jupiter */
    { SATURN_MOONS,  MOON_DATA_SATURN_COUNT },   /* Saturn */
    { NULL,          0 },                       /* Uranus */
    { NULL,          0 }                        /* Neptune */
};

/* ---------- Validation helpers ---------- */

static int valid_planet(int i) {
    return i >= 0 && i < 8;
}

static int valid_moon(int planet_index, int moon_index) {
    if (!valid_planet(planet_index)) return 0;
    return moon_index >= 0 && moon_index < CATALOG[planet_index].count;
}

/* ---------- Normalize angle to [0, 360) ---------- */

static double normalize_deg(double deg) {
    deg = fmod(deg, 360.0);
    if (deg < 0.0) deg += 360.0;
    return deg;
}

/* ---------- Public API ---------- */

int moon_data_count(int planet_index) {
    if (!valid_planet(planet_index)) return 0;
    return CATALOG[planet_index].count;
}

moon_entry_t moon_data_get(int planet_index, int moon_index) {
    if (!valid_moon(planet_index, moon_index)) {
        moon_entry_t empty;
        empty.name = NULL;
        empty.orbit.a_km           = 0.0;
        empty.orbit.e              = 0.0;
        empty.orbit.i_deg          = 0.0;
        empty.orbit.period_days    = 0.0;
        empty.orbit.mean_lon_epoch = 0.0;
        empty.orbit.radius_km      = 0.0;
        empty.orbit.albedo         = 0.0;
        empty.orbit.parent_planet  = -1;
        return empty;
    }
    return CATALOG[planet_index].moons[moon_index];
}

const char *moon_data_name(int planet_index, int moon_index) {
    if (!valid_moon(planet_index, moon_index)) return "Unknown";
    return CATALOG[planet_index].moons[moon_index].name;
}

moon_position_t moon_data_position(int planet_index, int moon_index, double jd) {
    moon_position_t pos;
    pos.x_km        = 0.0;
    pos.y_km        = 0.0;
    pos.z_km        = 0.0;
    pos.distance_km = 0.0;
    pos.angle_deg   = 0.0;

    if (!valid_moon(planet_index, moon_index)) return pos;

    const moon_orbit_t *orb = &CATALOG[planet_index].moons[moon_index].orbit;

    /* Days since J2000.0 */
    double T = jd - J2000_JD;

    /* Mean motion (degrees/day) */
    double n = 360.0 / orb->period_days;

    /* Mean anomaly (degrees), normalized to [0, 360) */
    double M_deg = normalize_deg(orb->mean_lon_epoch + n * T);

    /* Convert to radians for Kepler solver */
    double M_rad = M_deg * DEG_TO_RAD;

    /* Eccentric anomaly */
    double E = kepler_solve(M_rad, orb->e);

    /* True anomaly */
    double v = eccentric_to_true_anomaly(E, orb->e);

    /* Distance from parent (km) */
    double r = orb->a_km * (1.0 - orb->e * cos(E));

    /* Position in orbital plane */
    double x = r * cos(v);
    double y_plane = r * sin(v);

    /* Apply inclination */
    double i_rad = orb->i_deg * DEG_TO_RAD;
    double y = y_plane * cos(i_rad);
    double z = y_plane * sin(i_rad);

    /* Angle in degrees, normalized to [0, 360) */
    double angle = normalize_deg(v * RAD_TO_DEG);

    pos.x_km        = x;
    pos.y_km        = y;
    pos.z_km        = z;
    pos.distance_km = r;
    pos.angle_deg   = angle;

    return pos;
}

double moon_data_period(int planet_index, int moon_index) {
    if (!valid_moon(planet_index, moon_index)) return 0.0;
    return CATALOG[planet_index].moons[moon_index].orbit.period_days;
}

double moon_data_radius(int planet_index, int moon_index) {
    if (!valid_moon(planet_index, moon_index)) return 0.0;
    return CATALOG[planet_index].moons[moon_index].orbit.radius_km;
}

double moon_data_scene_distance(int planet_index, int moon_index, double base_scale) {
    if (!valid_moon(planet_index, moon_index)) return 0.0;
    return CATALOG[planet_index].moons[moon_index].orbit.a_km * base_scale;
}

double moon_data_scene_radius(int planet_index, int moon_index, double base_scale) {
    if (!valid_moon(planet_index, moon_index)) return 0.0;
    return CATALOG[planet_index].moons[moon_index].orbit.radius_km * base_scale;
}

int moon_data_total_count(void) {
    int total = 0;
    for (int i = 0; i < 8; i++) {
        total += CATALOG[i].count;
    }
    return total;
}
