/*
 * moon_pack.c -- Moon render pack: GPU-ready vertex packing
 *
 * Packs 9 major moon positions (Earth:1, Mars:2, Jupiter:4, Saturn:2),
 * albedo-based grayscale colors, sizes, and orbit trail polylines into
 * interleaved vertex arrays for GPU upload.
 *
 * Contains GLSL ES 3.00 shader source strings for moon billboards
 * (point sprites with soft circular edges) and orbit trail lines.
 *
 * Pure module: no GL calls, no malloc, no globals, no side effects.
 */

#include "moon_pack.h"

#include "../systems/astronomy/moon_data.h"
#include "../systems/astronomy/planets.h"

#include <math.h>

#ifndef PI
#define PI 3.14159265358979323846
#endif

#define DEG_TO_RAD (PI / 180.0)

/* Kilometers per Astronomical Unit */
#define AU_KM 149597870.7

/* Planet indices that have cataloged moons */
#define EARTH_IDX   2
#define MARS_IDX    3
#define JUPITER_IDX 4
#define SATURN_IDX  5

/* Ordered list of parent planet indices for iteration */
static const int MOON_PLANETS[] = {
    EARTH_IDX, MARS_IDX, JUPITER_IDX, SATURN_IDX
};
#define MOON_PLANET_COUNT 4

/* ======================================================================
 * Internal: Convert ecliptic spherical (lon, lat, dist AU) to Cartesian
 * scene coordinates. Ecliptic plane is XZ, Y is up.
 * ====================================================================== */

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

/* ======================================================================
 * mp_pack_moons
 * ====================================================================== */

mp_moon_data_t mp_pack_moons(double jd, float scene_scale,
                              float moon_scale, float radius_scale)
{
    mp_moon_data_t result;
    result.moon_count = 0;

    /* Get all planet positions */
    solar_system_t sys = planets_at(jd);

    /* Iterate over planets with moons */
    for (int p = 0; p < MOON_PLANET_COUNT; p++) {
        int planet_idx = MOON_PLANETS[p];
        int num_moons = moon_data_count(planet_idx);

        /* Parent planet position in scene coordinates */
        float px, py, pz;
        spherical_to_cartesian(sys.positions[planet_idx].longitude,
                                sys.positions[planet_idx].latitude,
                                sys.positions[planet_idx].distance,
                                scene_scale, &px, &py, &pz);

        for (int m = 0; m < num_moons; m++) {
            if (result.moon_count >= MP_MAX_MOONS) break;

            int idx = result.moon_count;
            int base = idx * MP_VERTEX_FLOATS;

            /* Moon position relative to parent (km) */
            moon_position_t mpos = moon_data_position(planet_idx, m, jd);

            /* Convert moon offset from km to scene units:
             * offset_km / AU_KM * scene_scale * moon_scale */
            double scale_factor = (double)scene_scale * (double)moon_scale
                                  / AU_KM;
            float mx = (float)(mpos.x_km * scale_factor);
            float my = (float)(mpos.y_km * scale_factor);
            float mz = (float)(mpos.z_km * scale_factor);

            /* Moon position in scene = parent + offset */
            result.vertices[base + 0] = px + mx;
            result.vertices[base + 1] = py + my;
            result.vertices[base + 2] = pz + mz;

            /* Color: grayscale from albedo */
            moon_entry_t entry = moon_data_get(planet_idx, m);
            float albedo = (float)entry.orbit.albedo;
            result.vertices[base + 3] = albedo;
            result.vertices[base + 4] = albedo;
            result.vertices[base + 5] = albedo;

            /* Radius: scene radius * radius_scale */
            double scene_radius = moon_data_scene_radius(
                planet_idx, m, (double)radius_scale);
            result.vertices[base + 6] = (float)scene_radius;

            /* Parent tracking */
            result.moon_parent[idx] = planet_idx;
            result.moon_count++;
        }
    }

    return result;
}

/* ======================================================================
 * mp_pack_trails
 * ====================================================================== */

mp_trail_data_t mp_pack_trails(double jd, float scene_scale,
                                float moon_scale, int samples)
{
    /* Use static to avoid large stack allocation (~26KB) */
    static mp_trail_data_t result;

    /* Clamp samples */
    if (samples < 1) samples = 1;
    if (samples > MP_TRAIL_MAX_SAMPLES) samples = MP_TRAIL_MAX_SAMPLES;

    result.total_vertex_count = 0;

    /* Moon index across all planets (0-8) */
    int moon_idx = 0;

    for (int p = 0; p < MOON_PLANET_COUNT; p++) {
        int planet_idx = MOON_PLANETS[p];
        int num_moons = moon_data_count(planet_idx);

        for (int m = 0; m < num_moons; m++) {
            if (moon_idx >= MP_MAX_MOONS) break;

            result.trail_offsets[moon_idx] = result.total_vertex_count;
            result.trail_counts[moon_idx] = samples;

            /* Get orbital period for this moon */
            double period = moon_data_period(planet_idx, m);

            /* Moon albedo for trail color (grayscale) */
            moon_entry_t entry = moon_data_get(planet_idx, m);
            float albedo = (float)entry.orbit.albedo;

            /* Scale factor for km -> scene units */
            double scale_factor = (double)scene_scale * (double)moon_scale
                                  / AU_KM;

            for (int s = 0; s < samples; s++) {
                /* Sample time: go backward over one orbital period */
                double t = (double)s / (double)samples;
                double sample_jd = jd - t * period;

                /* Get parent planet position at sample time */
                solar_system_t sys = planets_at(sample_jd);
                float px, py, pz;
                spherical_to_cartesian(
                    sys.positions[planet_idx].longitude,
                    sys.positions[planet_idx].latitude,
                    sys.positions[planet_idx].distance,
                    scene_scale, &px, &py, &pz);

                /* Get moon offset at sample time */
                moon_position_t mpos = moon_data_position(planet_idx, m,
                                                           sample_jd);
                float mx = (float)(mpos.x_km * scale_factor);
                float my = (float)(mpos.y_km * scale_factor);
                float mz = (float)(mpos.z_km * scale_factor);

                int vidx = (result.total_vertex_count + s)
                           * MP_TRAIL_VERTEX_FLOATS;

                /* Position = parent + offset */
                result.vertices[vidx + 0] = px + mx;
                result.vertices[vidx + 1] = py + my;
                result.vertices[vidx + 2] = pz + mz;

                /* Color RGB (grayscale from albedo) */
                result.vertices[vidx + 3] = albedo;
                result.vertices[vidx + 4] = albedo;
                result.vertices[vidx + 5] = albedo;

                /* Alpha: fade from 1.0 (current) to 0.1 (one period ago) */
                float alpha = 1.0f - (float)t * 0.9f;
                result.vertices[vidx + 6] = alpha;
            }

            result.total_vertex_count += samples;
            moon_idx++;
        }
    }

    return result;
}

/* ======================================================================
 * Byte size functions
 * ====================================================================== */

int mp_moon_vertex_bytes(const mp_moon_data_t *data)
{
    return data->moon_count * MP_VERTEX_FLOATS * (int)sizeof(float);
}

int mp_trail_vertex_bytes(const mp_trail_data_t *data)
{
    return data->total_vertex_count * MP_TRAIL_VERTEX_FLOATS *
           (int)sizeof(float);
}

/* ======================================================================
 * GLSL ES 3.00 shader source strings
 * ====================================================================== */

/* Moon billboard vertex shader.
 * Uniforms: u_mvp (mat4), u_scale (float).
 * Attributes: a_position (vec3), a_color (vec3), a_radius (float).
 * Uses gl_PointSize for billboard sizing. */
static const char MP_MOON_VERT[] =
    "#version 300 es\n"
    "precision highp float;\n"
    "\n"
    "uniform mat4 u_mvp;\n"
    "uniform float u_scale;\n"
    "\n"
    "layout(location = 0) in vec3 a_position;\n"
    "layout(location = 1) in vec3 a_color;\n"
    "layout(location = 2) in float a_radius;\n"
    "\n"
    "out vec3 v_color;\n"
    "\n"
    "void main() {\n"
    "    gl_Position = u_mvp * vec4(a_position, 1.0);\n"
    "    float ps = a_radius * u_scale / gl_Position.w;\n"
    "    gl_PointSize = clamp(ps, 1.0, 32.0);\n"
    "    v_color = a_color;\n"
    "}\n";

/* Moon billboard fragment shader.
 * Circular dot with soft edge via gl_PointCoord. */
static const char MP_MOON_FRAG[] =
    "#version 300 es\n"
    "precision highp float;\n"
    "\n"
    "in vec3 v_color;\n"
    "out vec4 frag_color;\n"
    "\n"
    "void main() {\n"
    "    vec2 pc = gl_PointCoord - vec2(0.5);\n"
    "    float dist = length(pc) * 2.0;\n"
    "    float body = 1.0 - smoothstep(0.8, 0.95, dist);\n"
    "    if (body < 0.01) discard;\n"
    "    frag_color = vec4(v_color, body);\n"
    "}\n";

/* Moon trail vertex shader.
 * Uniforms: u_mvp (mat4).
 * Attributes: a_position (vec3), a_color (vec4). */
static const char MP_TRAIL_VERT[] =
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

/* Moon trail fragment shader.
 * RGBA passthrough from vertex color. */
static const char MP_TRAIL_FRAG[] =
    "#version 300 es\n"
    "precision highp float;\n"
    "\n"
    "in vec4 v_color;\n"
    "out vec4 frag_color;\n"
    "\n"
    "void main() {\n"
    "    frag_color = v_color;\n"
    "}\n";

const char *mp_moon_vert_source(void)
{
    return MP_MOON_VERT;
}

const char *mp_moon_frag_source(void)
{
    return MP_MOON_FRAG;
}

const char *mp_trail_vert_source(void)
{
    return MP_TRAIL_VERT;
}

const char *mp_trail_frag_source(void)
{
    return MP_TRAIL_FRAG;
}
