/*
 * eclipse_geometry.c -- Solar and lunar eclipse geometry
 *
 * Computes approximate eclipse conditions, shadow cone geometry,
 * and packs renderable vertex data for eclipse shadow visualization.
 * Detection is approximate — suitable for visual rendering.
 *
 * Pure module: no GL calls, no malloc, no globals, no side effects.
 */

#include "eclipse_geometry.h"
#include "../systems/astronomy/lunar.h"
#include "../systems/astronomy/solar_events.h"

#include <math.h>

#define PI 3.14159265358979323846
#define DEG2RAD (PI / 180.0)
#define RAD2DEG (180.0 / PI)

/* ======================================================================
 * Astronomical constants
 * ====================================================================== */

/* Synodic month (new moon to new moon) in days */
static const double SYNODIC_MONTH = 29.53058868;

/* Sun mean apparent diameter at 1 AU (degrees) */
static const double SUN_MEAN_DIAMETER_DEG = 0.533;

/* Moon mean apparent diameter at mean distance (degrees) */
static const double MOON_MEAN_DIAMETER_DEG = 0.518;

/* Solar eclipse limit: max Sun-Moon separation for possible eclipse (degrees) */
static const double SOLAR_ECLIPSE_LIMIT_DEG = 1.58;

/* Lunar eclipse limit: max deviation from 180 for possible eclipse (degrees) */
static const double LUNAR_ECLIPSE_LIMIT_DEG = 1.58;

/* Moon's orbital inclination to ecliptic (degrees) */
static const double MOON_ORBIT_INCLINATION = 5.145;

/* Mean longitude of ascending node at J2000.0 (degrees) */
static const double NODE_LON_J2000 = 125.044;

/* Node regression rate (degrees per Julian century, negative = retrograde) */
static const double NODE_RATE = -1934.136;

/* Two years in days (search limit for next eclipse) */
static const double SEARCH_LIMIT_DAYS = 730.5;

/* ======================================================================
 * Internal helpers
 * ====================================================================== */

static double normalize_angle(double deg)
{
    double r = fmod(deg, 360.0);
    if (r < 0.0) r += 360.0;
    return r;
}

/* Approximate Sun ecliptic longitude (degrees) */
static double sun_ecliptic_longitude(double jd)
{
    double n = jd - 2451545.0;
    double L = normalize_angle(280.460 + 0.9856474 * n);
    double g = normalize_angle(357.528 + 0.9856003 * n);
    double g_rad = g * DEG2RAD;
    return normalize_angle(L + 1.915 * sin(g_rad) + 0.020 * sin(2.0 * g_rad));
}

/* Approximate Moon ecliptic longitude (degrees) — matches lunar.c */
static double moon_ecliptic_longitude(double jd)
{
    double t = (jd - 2451545.0) / 36525.0;
    double l = 218.3164477 + 481267.88123421 * t;
    double m = 134.9633964 + 477198.8675055 * t;
    double lon = l + 6.289 * sin(m * DEG2RAD);
    return normalize_angle(lon);
}

/* Longitude of the Moon's ascending node (degrees) */
static double ascending_node_longitude(double jd)
{
    double t = (jd - 2451545.0) / 36525.0;
    return normalize_angle(NODE_LON_J2000 + NODE_RATE * t);
}

/* Approximate Moon ecliptic latitude (degrees)
 * Uses orbital inclination and distance from ascending node. */
static double moon_ecliptic_latitude(double jd)
{
    double moon_lon = moon_ecliptic_longitude(jd);
    double node_lon = ascending_node_longitude(jd);
    double arg = moon_lon - node_lon;
    return MOON_ORBIT_INCLINATION * sin(arg * DEG2RAD);
}

/* Angular separation between two points on ecliptic (degrees).
 * Uses spherical geometry with ecliptic latitude of Moon. */
static double angular_separation(double lon1, double lat1,
                                 double lon2, double lat2)
{
    double dlon = (lon1 - lon2) * DEG2RAD;
    double la1 = lat1 * DEG2RAD;
    double la2 = lat2 * DEG2RAD;
    double cos_sep = sin(la1) * sin(la2) + cos(la1) * cos(la2) * cos(dlon);
    /* Clamp for numerical safety */
    if (cos_sep > 1.0) cos_sep = 1.0;
    if (cos_sep < -1.0) cos_sep = -1.0;
    return acos(cos_sep) * RAD2DEG;
}

/* Earth-Sun distance in AU (approximate) */
static double earth_sun_distance_au(double jd)
{
    double n = jd - 2451545.0;
    double g = normalize_angle(357.528 + 0.9856003 * n);
    double g_rad = g * DEG2RAD;
    /* Simplified from Meeus: r = 1.00014 - 0.01671*cos(g) - 0.00014*cos(2g) */
    return 1.00014 - 0.01671 * cos(g_rad) - 0.00014 * cos(2.0 * g_rad);
}

/* Earth-Moon distance in Earth radii (approximate).
 * Mean = 60.27 ER, varies ~56-64 ER. Uses anomalistic month. */
static double earth_moon_distance_er(double jd)
{
    double t = (jd - 2451545.0) / 36525.0;
    double m = 134.9633964 + 477198.8675055 * t; /* Moon mean anomaly */
    double m_rad = normalize_angle(m) * DEG2RAD;
    /* Simplified: a/r = 1 + e*cos(M) approximately */
    /* Mean distance 60.27 ER, eccentricity ~0.0549 */
    return 60.27 / (1.0 + 0.0549 * cos(m_rad));
}

/* ======================================================================
 * Public API: apparent diameters
 * ====================================================================== */

double ecl_sun_apparent_diameter(double jd)
{
    double r = earth_sun_distance_au(jd);
    /* Apparent diameter scales inversely with distance.
     * At 1 AU, diameter = SUN_MEAN_DIAMETER_DEG */
    return SUN_MEAN_DIAMETER_DEG / r;
}

double ecl_moon_apparent_diameter(double jd)
{
    double d_er = earth_moon_distance_er(jd);
    /* At mean distance (60.27 ER), diameter = MOON_MEAN_DIAMETER_DEG.
     * Apparent size scales inversely with distance. */
    return MOON_MEAN_DIAMETER_DEG * 60.27 / d_er;
}

/* ======================================================================
 * Public API: Sun-Moon angular separation
 * ====================================================================== */

double ecl_sun_moon_separation(double jd)
{
    double sun_lon = sun_ecliptic_longitude(jd);
    double moon_lon = moon_ecliptic_longitude(jd);
    double moon_lat = moon_ecliptic_latitude(jd);
    /* Sun is approximately on the ecliptic (lat ~0) */
    return angular_separation(sun_lon, 0.0, moon_lon, moon_lat);
}

/* ======================================================================
 * Internal: solar eclipse classification
 * ====================================================================== */

static void classify_solar(ecl_condition_t *result, double moon_lat,
                           double half_sum)
{
    double sep = result->sun_moon_sep_deg;
    if (sep >= SOLAR_ECLIPSE_LIMIT_DEG) return;

    result->gamma = moon_lat / half_sum;

    /* Magnitude: how much of the Sun's disk is covered */
    double sun_r = result->sun_apparent_deg / 2.0;
    double moon_r = result->moon_apparent_deg / 2.0;
    result->magnitude = (sun_r + moon_r - sep) / (2.0 * sun_r);
    if (result->magnitude < 0.0) result->magnitude = 0.0;

    /* Classify solar eclipse type */
    double abs_gamma = fabs(result->gamma);
    if (abs_gamma > 1.0) {
        result->type = ECL_SOLAR_PARTIAL;
    } else if (result->moon_apparent_deg >= result->sun_apparent_deg) {
        result->type = ECL_SOLAR_TOTAL;
    } else {
        result->type = ECL_SOLAR_ANNULAR;
    }
}

/* ======================================================================
 * Internal: lunar eclipse classification
 * ====================================================================== */

static void classify_lunar(ecl_condition_t *result, double moon_lat,
                           double half_sum)
{
    double dev_from_opposition = fabs(result->sun_moon_sep_deg - 180.0);
    if (dev_from_opposition >= LUNAR_ECLIPSE_LIMIT_DEG) return;

    result->gamma = moon_lat / half_sum;

    double moon_r = result->moon_apparent_deg / 2.0;
    /* Earth's shadow angular radius at Moon's distance ~1.3 degrees
     * (umbra ~0.7 deg, penumbra ~1.3 deg) */
    double umbra_angular_r = 0.7;
    double penumbra_angular_r = 1.3;
    double abs_lat = fabs(moon_lat);

    if (abs_lat < umbra_angular_r - moon_r) {
        result->type = ECL_LUNAR_TOTAL;
        result->magnitude = (umbra_angular_r + moon_r - abs_lat)
                           / (2.0 * moon_r);
    } else if (abs_lat < umbra_angular_r + moon_r) {
        result->type = ECL_LUNAR_PARTIAL;
        result->magnitude = (umbra_angular_r + moon_r - abs_lat)
                           / (2.0 * moon_r);
    } else if (abs_lat < penumbra_angular_r + moon_r) {
        result->type = ECL_LUNAR_PENUMBRAL;
        result->magnitude = (penumbra_angular_r + moon_r - abs_lat)
                           / (2.0 * moon_r);
    }
    if (result->magnitude < 0.0) result->magnitude = 0.0;
}

/* ======================================================================
 * Public API: ecl_check — eclipse detection
 * ====================================================================== */

ecl_condition_t ecl_check(double jd)
{
    ecl_condition_t result;
    result.type = ECL_NONE;
    result.magnitude = 0.0;
    result.gamma = 0.0;
    result.is_ascending_node = 0;

    /* Get lunar phase */
    lunar_info_t moon = lunar_phase(jd);
    double phase_frac = moon.phase_fraction;

    /* Get apparent diameters */
    result.sun_apparent_deg = ecl_sun_apparent_diameter(jd);
    result.moon_apparent_deg = ecl_moon_apparent_diameter(jd);

    /* Get angular separation */
    result.sun_moon_sep_deg = ecl_sun_moon_separation(jd);

    /* Determine ascending node proximity */
    double moon_lon = moon_ecliptic_longitude(jd);
    double node_lon = ascending_node_longitude(jd);
    double dist_to_node = fabs(normalize_angle(moon_lon - node_lon));
    if (dist_to_node > 180.0) dist_to_node = 360.0 - dist_to_node;
    result.is_ascending_node = (dist_to_node < 90.0) ? 1 : 0;

    /* Shared geometry */
    double moon_lat = moon_ecliptic_latitude(jd);
    double half_sum = (result.sun_apparent_deg + result.moon_apparent_deg) / 2.0;

    /* Classify by phase */
    if (phase_frac < 0.06 || phase_frac > 0.94) {
        classify_solar(&result, moon_lat, half_sum);
    } else if (phase_frac > 0.44 && phase_frac < 0.56) {
        classify_lunar(&result, moon_lat, half_sum);
    }

    return result;
}

/* ======================================================================
 * Public API: ecl_type_name
 * ====================================================================== */

const char *ecl_type_name(ecl_type_t type)
{
    switch (type) {
    case ECL_NONE:             return "None";
    case ECL_SOLAR_PARTIAL:    return "Solar Partial";
    case ECL_SOLAR_ANNULAR:    return "Solar Annular";
    case ECL_SOLAR_TOTAL:      return "Solar Total";
    case ECL_LUNAR_PENUMBRAL:  return "Lunar Penumbral";
    case ECL_LUNAR_PARTIAL:    return "Lunar Partial";
    case ECL_LUNAR_TOTAL:      return "Lunar Total";
    }
    return "Unknown";
}

/* ======================================================================
 * Public API: ecl_default_config
 * ====================================================================== */

ecl_config_t ecl_default_config(void)
{
    ecl_config_t cfg;
    cfg.earth_radius = 6.0f;
    cfg.umbra_alpha = 0.7f;
    cfg.penumbra_alpha = 0.3f;
    return cfg;
}

/* ======================================================================
 * Public API: ecl_shadow — shadow cone geometry
 * ====================================================================== */

ecl_shadow_t ecl_shadow(const ecl_condition_t *cond, const ecl_config_t *config)
{
    ecl_shadow_t s;
    s.umbra_radius = 0.0f;
    s.penumbra_radius = 0.0f;
    s.center_x = 0.0f;
    s.center_y = 0.0f;
    s.center_z = 0.0f;

    if (cond->type == ECL_NONE) {
        return s;
    }

    double er = (double)config->earth_radius;

    /* Shadow center offset from Earth center: gamma * earth_radius */
    double abs_gamma = fabs(cond->gamma);
    s.center_y = (float)(cond->gamma * er);

    /* Umbra radius: proportional to magnitude and Earth radius.
     * For a total solar eclipse, umbra on Earth is ~100-270 km,
     * here we scale proportionally to scene units. */
    double sun_r = cond->sun_apparent_deg / 2.0;
    double moon_r = cond->moon_apparent_deg / 2.0;

    if (cond->type == ECL_SOLAR_TOTAL || cond->type == ECL_SOLAR_ANNULAR ||
        cond->type == ECL_SOLAR_PARTIAL) {
        /* Umbra: proportional to the angular difference Moon-Sun */
        double umbra_angular = fabs(moon_r - sun_r);
        s.umbra_radius = (float)(umbra_angular / sun_r * er * 0.5);
        if (s.umbra_radius < 0.01f) s.umbra_radius = 0.01f;

        /* Penumbra: proportional to the sum of angular sizes */
        double penumbra_angular = moon_r + sun_r;
        s.penumbra_radius = (float)(penumbra_angular / sun_r * er * 0.5);
    } else {
        /* Lunar eclipse: shadow of Earth on Moon */
        /* Umbra scales with Earth shadow angular size */
        s.umbra_radius = (float)(er * 0.7 / (moon_r + sun_r));
        s.penumbra_radius = (float)(er * 1.3 / (moon_r + sun_r));
    }

    /* Offset center based on gamma direction.
     * Use ascending node info to determine the direction. */
    if (abs_gamma > 0.001) {
        double angle = cond->is_ascending_node ? 0.0 : PI;
        s.center_x = (float)(abs_gamma * er * cos(angle));
        s.center_z = (float)(abs_gamma * er * sin(angle) * 0.1);
    }

    return s;
}

/* ======================================================================
 * Public API: ecl_pack_umbra — triangle fan as separate triangles
 *
 * Packs ECL_DISK_SEGMENTS triangles. Each triangle has 3 vertices:
 *   center, perimeter[i], perimeter[i+1]
 * Total: ECL_DISK_SEGMENTS * 3 vertices.
 * ====================================================================== */

int ecl_pack_umbra(const ecl_shadow_t *shadow, const ecl_config_t *config,
                   float *out)
{
    if (shadow->umbra_radius <= 0.0f) return 0;

    int count = 0;
    double angle_step = 2.0 * PI / ECL_DISK_SEGMENTS;

    for (int i = 0; i < ECL_DISK_SEGMENTS; i++) {
        double a0 = (double)i * angle_step;
        double a1 = (double)((i + 1) % ECL_DISK_SEGMENTS) * angle_step;

        float px0 = shadow->center_x + shadow->umbra_radius * (float)cos(a0);
        float py0 = shadow->center_y + shadow->umbra_radius * (float)sin(a0);
        float px1 = shadow->center_x + shadow->umbra_radius * (float)cos(a1);
        float py1 = shadow->center_y + shadow->umbra_radius * (float)sin(a1);

        /* Vertex 0: center */
        int vi = count * ECL_VERTEX_FLOATS;
        out[vi + 0] = shadow->center_x;
        out[vi + 1] = shadow->center_y;
        out[vi + 2] = shadow->center_z;
        out[vi + 3] = 0.05f;  /* dark grey R */
        out[vi + 4] = 0.05f;  /* dark grey G */
        out[vi + 5] = 0.08f;  /* slight blue B */
        out[vi + 6] = config->umbra_alpha;
        count++;

        /* Vertex 1: perimeter[i] */
        vi = count * ECL_VERTEX_FLOATS;
        out[vi + 0] = px0;
        out[vi + 1] = py0;
        out[vi + 2] = shadow->center_z;
        out[vi + 3] = 0.1f;
        out[vi + 4] = 0.1f;
        out[vi + 5] = 0.12f;
        out[vi + 6] = config->umbra_alpha * 0.5f;
        count++;

        /* Vertex 2: perimeter[i+1] */
        vi = count * ECL_VERTEX_FLOATS;
        out[vi + 0] = px1;
        out[vi + 1] = py1;
        out[vi + 2] = shadow->center_z;
        out[vi + 3] = 0.1f;
        out[vi + 4] = 0.1f;
        out[vi + 5] = 0.12f;
        out[vi + 6] = config->umbra_alpha * 0.5f;
        count++;
    }

    return count;
}

/* ======================================================================
 * Public API: ecl_pack_penumbra — ring as triangles
 *
 * Packs ECL_DISK_SEGMENTS quads (2 triangles each) forming a ring
 * from umbra_radius (inner) to penumbra_radius (outer).
 * Total: ECL_DISK_SEGMENTS * 6 vertices.
 * ====================================================================== */

int ecl_pack_penumbra(const ecl_shadow_t *shadow, const ecl_config_t *config,
                      float *out)
{
    if (shadow->penumbra_radius <= 0.0f) return 0;
    if (shadow->umbra_radius <= 0.0f) return 0;

    int count = 0;
    double angle_step = 2.0 * PI / ECL_DISK_SEGMENTS;
    float inner_r = shadow->umbra_radius;
    float outer_r = shadow->penumbra_radius;

    for (int i = 0; i < ECL_DISK_SEGMENTS; i++) {
        double a0 = (double)i * angle_step;
        double a1 = (double)((i + 1) % ECL_DISK_SEGMENTS) * angle_step;

        float c0 = (float)cos(a0);
        float s0 = (float)sin(a0);
        float c1 = (float)cos(a1);
        float s1 = (float)sin(a1);

        /* Four corners of quad: inner0, outer0, inner1, outer1 */
        float ix0 = shadow->center_x + inner_r * c0;
        float iy0 = shadow->center_y + inner_r * s0;
        float ox0 = shadow->center_x + outer_r * c0;
        float oy0 = shadow->center_y + outer_r * s0;
        float ix1 = shadow->center_x + inner_r * c1;
        float iy1 = shadow->center_y + inner_r * s1;
        float ox1 = shadow->center_x + outer_r * c1;
        float oy1 = shadow->center_y + outer_r * s1;

        float inner_alpha = config->penumbra_alpha;
        float outer_alpha = config->penumbra_alpha * 0.1f;

        /* Triangle 1: inner0, outer0, inner1 */
        int vi = count * ECL_VERTEX_FLOATS;
        out[vi + 0] = ix0; out[vi + 1] = iy0; out[vi + 2] = shadow->center_z;
        out[vi + 3] = 0.1f; out[vi + 4] = 0.1f; out[vi + 5] = 0.12f;
        out[vi + 6] = inner_alpha;
        count++;

        vi = count * ECL_VERTEX_FLOATS;
        out[vi + 0] = ox0; out[vi + 1] = oy0; out[vi + 2] = shadow->center_z;
        out[vi + 3] = 0.15f; out[vi + 4] = 0.15f; out[vi + 5] = 0.18f;
        out[vi + 6] = outer_alpha;
        count++;

        vi = count * ECL_VERTEX_FLOATS;
        out[vi + 0] = ix1; out[vi + 1] = iy1; out[vi + 2] = shadow->center_z;
        out[vi + 3] = 0.1f; out[vi + 4] = 0.1f; out[vi + 5] = 0.12f;
        out[vi + 6] = inner_alpha;
        count++;

        /* Triangle 2: inner1, outer0, outer1 */
        vi = count * ECL_VERTEX_FLOATS;
        out[vi + 0] = ix1; out[vi + 1] = iy1; out[vi + 2] = shadow->center_z;
        out[vi + 3] = 0.1f; out[vi + 4] = 0.1f; out[vi + 5] = 0.12f;
        out[vi + 6] = inner_alpha;
        count++;

        vi = count * ECL_VERTEX_FLOATS;
        out[vi + 0] = ox0; out[vi + 1] = oy0; out[vi + 2] = shadow->center_z;
        out[vi + 3] = 0.15f; out[vi + 4] = 0.15f; out[vi + 5] = 0.18f;
        out[vi + 6] = outer_alpha;
        count++;

        vi = count * ECL_VERTEX_FLOATS;
        out[vi + 0] = ox1; out[vi + 1] = oy1; out[vi + 2] = shadow->center_z;
        out[vi + 3] = 0.15f; out[vi + 4] = 0.15f; out[vi + 5] = 0.18f;
        out[vi + 6] = outer_alpha;
        count++;
    }

    return count;
}

/* ======================================================================
 * Public API: ecl_pack — combined umbra + penumbra
 * ====================================================================== */

int ecl_pack(const ecl_shadow_t *shadow, const ecl_config_t *config,
             float *out)
{
    int n_umbra = ecl_pack_umbra(shadow, config, out);
    int n_penumbra = ecl_pack_penumbra(shadow, config,
                                       out + n_umbra * ECL_VERTEX_FLOATS);
    return n_umbra + n_penumbra;
}

/* ======================================================================
 * Public API: ecl_next_solar — find next solar eclipse
 * ====================================================================== */

double ecl_next_solar(double jd)
{
    double current = lunar_next_new_moon(jd);
    double limit = jd + SEARCH_LIMIT_DAYS;

    while (current < limit) {
        ecl_condition_t cond = ecl_check(current);
        if (cond.type == ECL_SOLAR_PARTIAL ||
            cond.type == ECL_SOLAR_ANNULAR ||
            cond.type == ECL_SOLAR_TOTAL) {
            return current;
        }
        current += SYNODIC_MONTH;
    }
    return 0.0;
}

/* ======================================================================
 * Public API: ecl_next_lunar — find next lunar eclipse
 * ====================================================================== */

double ecl_next_lunar(double jd)
{
    double current = lunar_next_full_moon(jd);
    double limit = jd + SEARCH_LIMIT_DAYS;

    while (current < limit) {
        ecl_condition_t cond = ecl_check(current);
        if (cond.type == ECL_LUNAR_PENUMBRAL ||
            cond.type == ECL_LUNAR_PARTIAL ||
            cond.type == ECL_LUNAR_TOTAL) {
            return current;
        }
        current += SYNODIC_MONTH;
    }
    return 0.0;
}

/* ======================================================================
 * GLSL ES 3.00 shader source strings
 * ====================================================================== */

/* Eclipse shadow vertex shader.
 * Uniforms: u_mvp (mat4).
 * Attributes: a_position (vec3, loc 0), a_color (vec4, loc 1).
 * Passes color to fragment shader. */
static const char ECL_VERT_SRC[] =
    "#version 300 es\n"
    "precision highp float;\n"
    "\n"
    "uniform mat4 u_mvp;\n"
    "\n"
    "layout(location = 0) in vec3 a_position;\n"
    "layout(location = 1) in vec4 a_color;\n"
    "\n"
    "out vec4 v_color;\n"
    "\n"
    "void main() {\n"
    "    gl_Position = u_mvp * vec4(a_position, 1.0);\n"
    "    v_color = a_color;\n"
    "}\n";

/* Eclipse shadow fragment shader.
 * Simple pass-through with vertex color interpolation.
 * The gradient comes from vertex colors (dark center, fading edge). */
static const char ECL_FRAG_SRC[] =
    "#version 300 es\n"
    "precision highp float;\n"
    "\n"
    "in vec4 v_color;\n"
    "out vec4 frag_color;\n"
    "\n"
    "void main() {\n"
    "    if (v_color.a < 0.01) discard;\n"
    "    frag_color = v_color;\n"
    "}\n";

const char *ecl_vert_source(void)
{
    return ECL_VERT_SRC;
}

const char *ecl_frag_source(void)
{
    return ECL_FRAG_SRC;
}
