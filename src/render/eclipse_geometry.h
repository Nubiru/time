/*
 * eclipse_geometry.h -- Solar and lunar eclipse geometry
 *
 * Computes eclipse conditions (type, magnitude, gamma), shadow cone
 * geometry (umbra/penumbra radii), and packs renderable vertex data
 * for eclipse shadow visualization. Includes next-eclipse finder and
 * GLSL ES 3.00 shader source strings.
 *
 * Eclipse detection is approximate — suitable for visual rendering,
 * not precise NASA-level prediction.
 *
 * Pure module: no GL calls, no malloc, no globals, no side effects.
 *
 * Dependencies:
 *   - lunar.h — lunar_phase() for moon phase fraction and longitude
 *   - solar_events.h — se_solar_declination() for Sun position
 */

#ifndef TIME_ECLIPSE_GEOMETRY_H
#define TIME_ECLIPSE_GEOMETRY_H

/* Eclipse types */
typedef enum {
    ECL_NONE = 0,
    ECL_SOLAR_PARTIAL,
    ECL_SOLAR_ANNULAR,
    ECL_SOLAR_TOTAL,
    ECL_LUNAR_PENUMBRAL,
    ECL_LUNAR_PARTIAL,
    ECL_LUNAR_TOTAL
} ecl_type_t;

/* Eclipse condition result */
typedef struct {
    ecl_type_t type;           /* eclipse type or ECL_NONE */
    double magnitude;          /* 0.0-1.0+ (fraction of diameter covered) */
    double gamma;              /* shadow axis distance from Earth center (Earth radii) */
    double sun_moon_sep_deg;   /* angular separation Sun-Moon (degrees) */
    double sun_apparent_deg;   /* Sun apparent diameter (degrees) */
    double moon_apparent_deg;  /* Moon apparent diameter (degrees) */
    int is_ascending_node;     /* 1 if near ascending node, 0 if descending */
} ecl_condition_t;

/* Shadow cone geometry for rendering */
typedef struct {
    float umbra_radius;       /* umbra cone radius at Earth surface (scene units) */
    float penumbra_radius;    /* penumbra cone radius at Earth surface */
    float center_x;           /* shadow center X on ecliptic plane */
    float center_y;           /* shadow center Y on ecliptic plane */
    float center_z;           /* shadow center Z */
} ecl_shadow_t;

/* Vertex layout: position(vec3) + color(vec4) = 7 floats */
#define ECL_VERTEX_FLOATS 7
#define ECL_VERTEX_STRIDE (ECL_VERTEX_FLOATS * (int)sizeof(float))

/* Shadow disk segments for rendering */
#define ECL_DISK_SEGMENTS 36

/* Max vertices: umbra disk (36*3) + penumbra ring (36*6) = 108 + 216 = 324 */
#define ECL_MAX_VERTICES 336

/* Configuration */
typedef struct {
    float earth_radius;       /* Earth radius in scene units */
    float umbra_alpha;        /* umbra disk opacity (e.g. 0.7) */
    float penumbra_alpha;     /* penumbra ring opacity (e.g. 0.3) */
} ecl_config_t;

/* Default configuration. */
ecl_config_t ecl_default_config(void);

/* Check eclipse condition for a given Julian Day.
 * Uses lunar phase and Sun-Moon angular separation.
 * Returns ECL_NONE if no eclipse possible. */
ecl_condition_t ecl_check(double jd);

/* Get eclipse type name string. */
const char *ecl_type_name(ecl_type_t type);

/* Compute Sun apparent diameter in degrees for a given JD.
 * Varies with Earth-Sun distance (~0.524 to ~0.542 degrees). */
double ecl_sun_apparent_diameter(double jd);

/* Compute Moon apparent diameter in degrees for a given JD.
 * Varies with Earth-Moon distance (~0.49 to ~0.56 degrees). */
double ecl_moon_apparent_diameter(double jd);

/* Compute angular separation between Sun and Moon in degrees. */
double ecl_sun_moon_separation(double jd);

/* Compute shadow cone geometry from eclipse condition.
 * Returns shadow disk position and radii in scene units. */
ecl_shadow_t ecl_shadow(const ecl_condition_t *cond, const ecl_config_t *config);

/* Pack umbra disk as triangle fan vertices.
 * out: position(vec3) + color(vec4) per vertex.
 * Returns number of vertices written. */
int ecl_pack_umbra(const ecl_shadow_t *shadow, const ecl_config_t *config,
                   float *out);

/* Pack penumbra ring as triangle strip vertices.
 * out: position(vec3) + color(vec4) per vertex.
 * Returns number of vertices written. */
int ecl_pack_penumbra(const ecl_shadow_t *shadow, const ecl_config_t *config,
                      float *out);

/* Pack both umbra + penumbra in one call.
 * Returns total vertices written. */
int ecl_pack(const ecl_shadow_t *shadow, const ecl_config_t *config,
             float *out);

/* Eclipse shadow vertex shader (GLSL ES 3.00). */
const char *ecl_vert_source(void);

/* Eclipse shadow fragment shader (GLSL ES 3.00).
 * Radial gradient: dark center (umbra), fading edge (penumbra). */
const char *ecl_frag_source(void);

/* Next solar eclipse after given JD (searches new moons).
 * Returns JD of eclipse or 0.0 if none within 2 years. */
double ecl_next_solar(double jd);

/* Next lunar eclipse after given JD (searches full moons).
 * Returns JD of eclipse or 0.0 if none within 2 years. */
double ecl_next_lunar(double jd);

#endif /* TIME_ECLIPSE_GEOMETRY_H */
