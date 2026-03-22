/*
 * planet_pack.c — Planet render pack: GPU-ready vertex packing
 *
 * Packs planet positions (via Keplerian orbital elements), colors
 * (NASA Planetary Fact Sheet), sizes, atmosphere thickness, and orbit
 * trail polylines into interleaved vertex arrays for GPU upload.
 *
 * Contains GLSL ES 3.00 shader source strings for planet billboards
 * (point sprites with atmospheric glow) and orbit trail lines.
 *
 * Pure module: no GL calls, no malloc, no globals, no side effects.
 */

#include "planet_pack.h"
#include "atmo_ring.h"
#include "noise_shader.h"
#include "shader_builder.h"

#include "../systems/astronomy/orbit.h"
#include "../systems/astronomy/planet_data.h"
#include "../systems/astronomy/planets.h"

#include <math.h>

/* PI is already defined in atmo_ring.h */
#ifndef PI
#define PI 3.14159265358979323846
#endif

#define DEG_TO_RAD (PI / 180.0)

/* ──────────────────────────────────────────────────────────────────────────
 * Internal: Pre-defined orbit pointers indexed by planet (0-7)
 * ────────────────────────────────────────────────────────────────────── */

static const planet_orbit_t *PLANET_ORBITS[PP_PLANET_COUNT] = {
    &ORBIT_MERCURY, &ORBIT_VENUS, &ORBIT_EARTH, &ORBIT_MARS,
    &ORBIT_JUPITER, &ORBIT_SATURN, &ORBIT_URANUS, &ORBIT_NEPTUNE
};

/* ──────────────────────────────────────────────────────────────────────────
 * Internal: Convert ecliptic spherical (lon_deg, lat_deg, dist_au) to
 * Cartesian (x, y, z) in scene space. Ecliptic plane is XZ, Y is up.
 * ────────────────────────────────────────────────────────────────────── */

static void spherical_to_cartesian(double lon_deg, double lat_deg,
                                    double dist_au, float scene_scale,
                                    float *out_x, float *out_y, float *out_z)
{
    double lon_rad = lon_deg * DEG_TO_RAD;
    double lat_rad = lat_deg * DEG_TO_RAD;
    double cos_lat = cos(lat_rad);

    *out_x = (float)(dist_au * cos_lat * cos(lon_rad) * (double)scene_scale);
    *out_z = (float)(dist_au * cos_lat * sin(lon_rad) * (double)scene_scale);
    *out_y = (float)(dist_au * sin(lat_rad) * (double)scene_scale);
}

/* ──────────────────────────────────────────────────────────────────────────
 * Internal: Compute orbital period in days from semimajor axis (AU).
 * Kepler's third law: T^2 = a^3 (years), T = sqrt(a^3) * 365.25 (days).
 * ────────────────────────────────────────────────────────────────────── */

static double orbital_period_days(double a_au)
{
    double a3 = a_au * a_au * a_au;
    return sqrt(a3) * 365.25;
}

/* ──────────────────────────────────────────────────────────────────────────
 * pp_pack_planets
 * ────────────────────────────────────────────────────────────────────── */

pp_planet_data_t pp_pack_planets(double jd, float scene_scale,
                                  float radius_scale)
{
    pp_planet_data_t result;
    result.planet_count = PP_PLANET_COUNT;

    solar_system_t sys = planets_at(jd);

    for (int i = 0; i < PP_PLANET_COUNT; i++) {
        int base = i * PP_PLANET_VERTEX_FLOATS;

        /* Position: ecliptic spherical -> Cartesian */
        spherical_to_cartesian(sys.positions[i].longitude,
                               sys.positions[i].latitude,
                               sys.positions[i].distance,
                               scene_scale,
                               &result.vertices[base + 0],
                               &result.vertices[base + 1],
                               &result.vertices[base + 2]);

        /* Color from planet_data */
        planet_color_t color = planet_data_color(i);
        result.vertices[base + 3] = color.r;
        result.vertices[base + 4] = color.g;
        result.vertices[base + 5] = color.b;

        /* Scene radius */
        result.vertices[base + 6] = (float)planet_data_scene_radius(
            i, (double)radius_scale);

        /* Atmosphere thickness */
        atmo_properties_t atmo = atmo_get(i);
        result.vertices[base + 7] = atmo.thickness_ratio;
    }

    return result;
}

/* ──────────────────────────────────────────────────────────────────────────
 * pp_pack_trails
 * ────────────────────────────────────────────────────────────────────── */

pp_trail_data_t pp_pack_trails(double jd, float scene_scale, int samples)
{
    /* Use static to avoid ~7MB stack allocation */
    static pp_trail_data_t result;

    /* Clamp samples */
    if (samples < 1) samples = 1;
    if (samples > PP_TRAIL_MAX_SAMPLES) samples = PP_TRAIL_MAX_SAMPLES;

    result.total_vertex_count = 0;

    for (int i = 0; i < PP_PLANET_COUNT; i++) {
        result.trail_offsets[i] = result.total_vertex_count;
        result.trail_counts[i] = samples;

        /* Get orbital period for this planet */
        orbital_elements_t elem0 = orbit_elements_at(PLANET_ORBITS[i], jd);
        double period = orbital_period_days(elem0.a);

        /* Planet color */
        planet_color_t color = planet_data_color(i);

        for (int s = 0; s < samples; s++) {
            /* Sample uniformly across one full orbit, starting at current JD
             * and going backward in time */
            double t = (double)s / (double)samples;
            double sample_jd = jd - t * period;

            /* Compute position at sample time */
            orbital_elements_t elem = orbit_elements_at(PLANET_ORBITS[i],
                                                         sample_jd);
            heliocentric_pos_t pos = orbit_heliocentric(&elem);

            int vidx = (result.total_vertex_count + s) * PP_TRAIL_VERTEX_FLOATS;

            /* Position */
            float px, py, pz;
            spherical_to_cartesian(pos.longitude, pos.latitude, pos.distance,
                                    scene_scale, &px, &py, &pz);
            result.vertices[vidx + 0] = px;
            result.vertices[vidx + 1] = py;
            result.vertices[vidx + 2] = pz;

            /* Color RGB */
            result.vertices[vidx + 3] = color.r;
            result.vertices[vidx + 4] = color.g;
            result.vertices[vidx + 5] = color.b;

            /* Alpha: fade from 1.0 (current) to 0.1 (oldest) */
            float alpha = 1.0f - (float)t * 0.9f;
            result.vertices[vidx + 6] = alpha;
        }

        result.total_vertex_count += samples;
    }

    return result;
}

/* ──────────────────────────────────────────────────────────────────────────
 * Byte size functions
 * ────────────────────────────────────────────────────────────────────── */

int pp_planet_vertex_bytes(const pp_planet_data_t *data)
{
    return data->planet_count * PP_PLANET_VERTEX_FLOATS * (int)sizeof(float);
}

int pp_trail_vertex_bytes(const pp_trail_data_t *data)
{
    return data->total_vertex_count * PP_TRAIL_VERTEX_FLOATS *
           (int)sizeof(float);
}

/* ──────────────────────────────────────────────────────────────────────────
 * GLSL ES 3.00 shader source strings
 * ────────────────────────────────────────────────────────────────────── */

/* Planet billboard vertex shader.
 * Uniforms: u_mvp (mat4).
 * Attributes: a_position (vec3), a_color (vec3), a_radius (float),
 *             a_atmo (float).
 * Uses gl_PointSize for billboard sizing. */
static const char PP_PLANET_VERT[] =
    "#version 300 es\n"
    "precision highp float;\n"
    "\n"
    "uniform mat4 u_mvp;\n"
    "uniform float u_scale_factor;\n"
    "\n"
    "layout(location = 0) in vec3 a_position;\n"
    "layout(location = 1) in vec3 a_color;\n"
    "layout(location = 2) in float a_radius;\n"
    "layout(location = 3) in float a_atmo;\n"
    "\n"
    "out vec3 v_color;\n"
    "out float v_atmo;\n"
    "\n"
    "void main() {\n"
    "    gl_Position = u_mvp * vec4(a_position, 1.0);\n"
    "    float ps = a_radius * u_scale_factor / gl_Position.w;\n"
    "    gl_PointSize = clamp(ps, 2.0, 64.0);\n"
    "    v_color = a_color;\n"
    "    v_atmo = a_atmo;\n"
    "}\n";

/* Planet billboard fragment shader — preamble (before noise library) */
static const char *s_pp_frag_preamble =
    "#version 300 es\n"
    "precision highp float;\n"
    "\n"
    "in vec3 v_color;\n"
    "in float v_atmo;\n"
    "uniform float u_time;\n"
    "out vec4 frag_color;\n"
    "\n";

/* Planet billboard fragment shader — body (after noise library).
 * Procedural surface noise + Lambertian lighting + Fresnel rim.
 * Uses fbm2/snoise2 from noise_shader.h for surface detail. */
static const char *s_pp_frag_body =
    "void main() {\n"
    "    vec2 pc = gl_PointCoord - vec2(0.5);\n"
    "    float dist = length(pc) * 2.0;\n"
    "    float body = 1.0 - smoothstep(0.7, 0.75, dist);\n"
    "    float glow_edge = 0.75 + v_atmo * 3.0;\n"
    "    float glow = (1.0 - smoothstep(0.75, glow_edge, dist)) * 0.4;\n"
    "    float alpha = max(body, glow);\n"
    "    vec3 col = mix(v_color * 0.6, v_color, body);\n"
    "\n"
    "    /* Procedural surface noise + Lambertian lighting.\n"
    "     * Project billboard to pseudo-sphere for 3D surface. */\n"
    "    if (body > 0.5) {\n"
    "        vec2 uv = pc * 2.0 / 0.7;\n"
    "        float r2 = dot(uv, uv);\n"
    "        if (r2 < 1.0) {\n"
    "            float z = sqrt(1.0 - r2);\n"
    "            vec3 sp = vec3(uv.x, uv.y, z);\n"
    "\n"
    "            /* Surface noise: latitude-stretched FBM for banding effect.\n"
    "             * Gas giants: strong bands. Rocky: terrain-like. */\n"
    "            float lat = sp.y;\n"
    "            float lon = atan(sp.z, sp.x);\n"
    "            float surface = fbm2(vec2(lat * 6.0, lon * 2.0 + u_time * 0.01));\n"
    "            surface = surface * 0.12 + 0.88;\n"
    "            col *= surface;\n"
    "\n"
    "            /* Lambertian diffuse lighting (fixed sun direction) */\n"
    "            float ndl = max(dot(sp, vec3(0.577, 0.577, 0.577)), 0.15);\n"
    "            col *= ndl;\n"
    "        }\n"
    "    }\n"
    "\n"
    "    /* Fresnel rim glow — atmospheric scattering at grazing angles.\n"
    "     * v_atmo (thickness_ratio 0.02-0.08) scaled to visible range.\n"
    "     * Color: body color mixed toward blue (Rayleigh bias). */\n"
    "    float rim = pow(smoothstep(0.3, 0.7, dist), 2.0) * v_atmo * 25.0 * body;\n"
    "    vec3 rim_color = mix(v_color, vec3(0.4, 0.6, 1.0), 0.5);\n"
    "    col += rim_color * rim;\n"
    "\n"
    "    if (alpha < 0.01) discard;\n"
    "    frag_color = vec4(col, alpha);\n"
    "}\n";

/* Concatenated planet fragment source: preamble + noise lib + body */
static shader_src_t s_pp_frag_src;
static int s_pp_frag_built = 0;

static const char *build_planet_frag_source(void)
{
    if (!s_pp_frag_built) {
        shader_src_init(&s_pp_frag_src);
        shader_src_append(&s_pp_frag_src, s_pp_frag_preamble);
        shader_src_append(&s_pp_frag_src, noise_shader_source());
        shader_src_append(&s_pp_frag_src, s_pp_frag_body);
        s_pp_frag_built = 1;
    }
    return shader_src_get(&s_pp_frag_src);
}

/* Orbit trail line vertex shader.
 * Uniforms: u_mvp (mat4).
 * Attributes: a_position (vec3), a_color (vec4). */
static const char PP_TRAIL_VERT[] =
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

/* Orbit trail line fragment shader.
 * RGBA passthrough from vertex color. */
static const char PP_TRAIL_FRAG[] =
    "#version 300 es\n"
    "precision highp float;\n"
    "\n"
    "in vec4 v_color;\n"
    "out vec4 frag_color;\n"
    "\n"
    "void main() {\n"
    "    frag_color = v_color;\n"
    "}\n";

const char *pp_planet_vert_source(void)
{
    return PP_PLANET_VERT;
}

const char *pp_planet_frag_source(void)
{
    return build_planet_frag_source();
}

const char *pp_trail_vert_source(void)
{
    return PP_TRAIL_VERT;
}

const char *pp_trail_frag_source(void)
{
    return PP_TRAIL_FRAG;
}
