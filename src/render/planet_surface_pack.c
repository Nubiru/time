/*
 * planet_surface_pack.c — Procedural planet surface shader pack
 *
 * Packs UV-sphere meshes with position, normal, UV, and color data
 * for each planet. Provides GLSL ES 3.00 shader source strings
 * implementing gas giant banding, rocky noise surfaces, day/night
 * terminator, and Fresnel atmosphere limb glow.
 *
 * Pure module: no GL calls, no malloc, no globals, no side effects.
 * Exception: static char buffer for shader source concatenation
 * (accepted pattern, same as sun_shader.c).
 *
 * Data sources:
 *   - planet_data.h — NASA Planetary Fact Sheet (colors, physical data)
 *   - orbit.h — JPL Keplerian elements for planet positions
 *   - noise_shader.h — GLSL noise functions (simplex, fbm)
 *   - DELTA digests: shader-procedural-planets.md, shader-atmospheric-scattering.md
 */

#include "planet_surface_pack.h"
#include "noise_shader.h"

#include "../systems/astronomy/orbit.h"
#include "../systems/astronomy/planet_data.h"
#include "../systems/astronomy/planets.h"

#include <math.h>

#ifndef PI
#define PI 3.14159265358979323846
#endif

#define DEG_TO_RAD (PI / 180.0)

/* ──────────────────────────────────────────────────────────────────────────
 * Internal: Planet orbit pointers (indexed 0-7 for Mercury-Neptune)
 * ────────────────────────────────────────────────────────────────────── */

static const planet_orbit_t *PSP_ORBITS[8] = {
    &ORBIT_MERCURY, &ORBIT_VENUS, &ORBIT_EARTH, &ORBIT_MARS,
    &ORBIT_JUPITER, &ORBIT_SATURN, &ORBIT_URANUS, &ORBIT_NEPTUNE
};

/* ──────────────────────────────────────────────────────────────────────────
 * Internal: Scene radii for each planet body (relative, not to scale).
 * Exaggerated for visibility at astronomical distances.
 * Index 0=Sun, 1=Mercury..8=Neptune
 * ────────────────────────────────────────────────────────────────────── */

static const float PSP_SCENE_RADII[PSP_MAX_PLANETS] = {
    0.35f,  /* Sun */
    0.04f,  /* Mercury */
    0.06f,  /* Venus */
    0.065f, /* Earth */
    0.05f,  /* Mars */
    0.18f,  /* Jupiter */
    0.15f,  /* Saturn */
    0.10f,  /* Uranus */
    0.095f  /* Neptune */
};

/* ──────────────────────────────────────────────────────────────────────────
 * psp_planet_config — per-planet visual configuration
 * ────────────────────────────────────────────────────────────────────── */

psp_config_t psp_planet_config(psp_planet_t planet)
{
    psp_config_t c;

    /* Zero-init */
    c.base_color[0] = 0.0f; c.base_color[1] = 0.0f; c.base_color[2] = 0.0f;
    c.band_color[0] = 0.0f; c.band_color[1] = 0.0f; c.band_color[2] = 0.0f;
    c.atmosphere_color[0] = 0.0f; c.atmosphere_color[1] = 0.0f;
    c.atmosphere_color[2] = 0.0f;
    c.atmo_thickness = 0.0f;
    c.band_frequency = 0.0f;
    c.band_amplitude = 0.0f;
    c.noise_scale = 0.0f;
    c.noise_amplitude = 0.0f;
    c.type = PSP_TYPE_STAR;

    switch (planet) {
    case PSP_SUN:
        /* Solar gold — handled by sun_shader, included here for completeness */
        c.base_color[0] = 1.0f; c.base_color[1] = 0.85f; c.base_color[2] = 0.55f;
        c.type = PSP_TYPE_STAR;
        break;

    case PSP_MERCURY: {
        /* Gray rocky, cratered, no atmosphere */
        planet_color_t pc = planet_data_color(PLANET_MERCURY);
        c.base_color[0] = pc.r; c.base_color[1] = pc.g; c.base_color[2] = pc.b;
        c.noise_scale = 5.0f;
        c.noise_amplitude = 0.15f;
        c.atmo_thickness = 0.0f;
        c.type = PSP_TYPE_ROCKY;
        break;
    }

    case PSP_VENUS: {
        /* Pale yellow-white, thick atmosphere, smooth cloud cover */
        planet_color_t pc = planet_data_color(PLANET_VENUS);
        c.base_color[0] = pc.r; c.base_color[1] = pc.g; c.base_color[2] = pc.b;
        c.atmosphere_color[0] = 0.95f; c.atmosphere_color[1] = 0.85f;
        c.atmosphere_color[2] = 0.55f;
        c.noise_scale = 2.0f;
        c.noise_amplitude = 0.08f;
        c.atmo_thickness = 0.12f;
        c.type = PSP_TYPE_ROCKY;
        break;
    }

    case PSP_EARTH: {
        /* Blue ocean base, thin blue atmosphere */
        planet_color_t pc = planet_data_color(PLANET_EARTH);
        c.base_color[0] = pc.r; c.base_color[1] = pc.g; c.base_color[2] = pc.b;
        c.atmosphere_color[0] = 0.4f; c.atmosphere_color[1] = 0.6f;
        c.atmosphere_color[2] = 1.0f;
        c.noise_scale = 3.0f;
        c.noise_amplitude = 0.25f;
        c.atmo_thickness = 0.05f;
        c.type = PSP_TYPE_ROCKY;
        break;
    }

    case PSP_MARS: {
        /* Rusty red-orange, thin atmosphere */
        planet_color_t pc = planet_data_color(PLANET_MARS);
        c.base_color[0] = pc.r; c.base_color[1] = pc.g; c.base_color[2] = pc.b;
        c.atmosphere_color[0] = 0.9f; c.atmosphere_color[1] = 0.6f;
        c.atmosphere_color[2] = 0.4f;
        c.noise_scale = 4.0f;
        c.noise_amplitude = 0.2f;
        c.atmo_thickness = 0.02f;
        c.type = PSP_TYPE_ROCKY;
        break;
    }

    case PSP_JUPITER: {
        /* Orange-white-brown latitude bands, high frequency */
        planet_color_t pc = planet_data_color(PLANET_JUPITER);
        planet_color_t pa = planet_data_get(PLANET_JUPITER).color_alt;
        c.base_color[0] = pc.r; c.base_color[1] = pc.g; c.base_color[2] = pc.b;
        c.band_color[0] = pa.r; c.band_color[1] = pa.g; c.band_color[2] = pa.b;
        c.atmosphere_color[0] = 0.85f; c.atmosphere_color[1] = 0.75f;
        c.atmosphere_color[2] = 0.55f;
        c.band_frequency = 12.0f;
        c.band_amplitude = 0.45f;
        c.noise_scale = 3.0f;
        c.noise_amplitude = 0.15f;
        c.atmo_thickness = 0.06f;
        c.type = PSP_TYPE_GAS_GIANT;
        break;
    }

    case PSP_SATURN: {
        /* Pale gold banding, less contrast than Jupiter */
        planet_color_t pc = planet_data_color(PLANET_SATURN);
        planet_color_t pa = planet_data_get(PLANET_SATURN).color_alt;
        c.base_color[0] = pc.r; c.base_color[1] = pc.g; c.base_color[2] = pc.b;
        c.band_color[0] = pa.r; c.band_color[1] = pa.g; c.band_color[2] = pa.b;
        c.atmosphere_color[0] = 0.9f; c.atmosphere_color[1] = 0.82f;
        c.atmosphere_color[2] = 0.6f;
        c.band_frequency = 8.0f;
        c.band_amplitude = 0.3f;
        c.noise_scale = 2.5f;
        c.noise_amplitude = 0.1f;
        c.atmo_thickness = 0.05f;
        c.type = PSP_TYPE_GAS_GIANT;
        break;
    }

    case PSP_URANUS: {
        /* Teal-cyan, very subtle banding */
        planet_color_t pc = planet_data_color(PLANET_URANUS);
        planet_color_t pa = planet_data_get(PLANET_URANUS).color_alt;
        c.base_color[0] = pc.r; c.base_color[1] = pc.g; c.base_color[2] = pc.b;
        c.band_color[0] = pa.r; c.band_color[1] = pa.g; c.band_color[2] = pa.b;
        c.atmosphere_color[0] = 0.55f; c.atmosphere_color[1] = 0.8f;
        c.atmosphere_color[2] = 0.85f;
        c.band_frequency = 4.0f;
        c.band_amplitude = 0.12f;
        c.noise_scale = 2.0f;
        c.noise_amplitude = 0.08f;
        c.atmo_thickness = 0.04f;
        c.type = PSP_TYPE_ICE_GIANT;
        break;
    }

    case PSP_NEPTUNE: {
        /* Deep blue, subtle dark spots, moderate banding */
        planet_color_t pc = planet_data_color(PLANET_NEPTUNE);
        planet_color_t pa = planet_data_get(PLANET_NEPTUNE).color_alt;
        c.base_color[0] = pc.r; c.base_color[1] = pc.g; c.base_color[2] = pc.b;
        c.band_color[0] = pa.r; c.band_color[1] = pa.g; c.band_color[2] = pa.b;
        c.atmosphere_color[0] = 0.3f; c.atmosphere_color[1] = 0.45f;
        c.atmosphere_color[2] = 0.85f;
        c.band_frequency = 6.0f;
        c.band_amplitude = 0.18f;
        c.noise_scale = 3.0f;
        c.noise_amplitude = 0.12f;
        c.atmo_thickness = 0.045f;
        c.type = PSP_TYPE_ICE_GIANT;
        break;
    }

    default:
        /* Invalid planet — return star type with solar gold */
        c.base_color[0] = 1.0f; c.base_color[1] = 0.85f; c.base_color[2] = 0.55f;
        c.type = PSP_TYPE_STAR;
        break;
    }

    return c;
}

/* ──────────────────────────────────────────────────────────────────────────
 * Internal: Compute sphere vertex (position, normal, uv) at grid (lat, lon)
 * ────────────────────────────────────────────────────────────────────── */

static void sphere_vertex(float center[3], float radius,
                          int lat_idx, int lon_idx, int segments,
                          const psp_config_t *config,
                          float *out)
{
    float theta = (float)lat_idx / (float)segments * (float)PI;
    float phi = (float)lon_idx / (float)segments * 2.0f * (float)PI;

    float sin_theta = sinf(theta);
    float cos_theta = cosf(theta);
    float sin_phi = sinf(phi);
    float cos_phi = cosf(phi);

    /* Normal = unit sphere direction */
    float nx = sin_theta * cos_phi;
    float ny = cos_theta;
    float nz = sin_theta * sin_phi;

    /* Position = center + radius * normal */
    out[0] = center[0] + radius * nx;
    out[1] = center[1] + radius * ny;
    out[2] = center[2] + radius * nz;

    /* Normal */
    out[3] = nx;
    out[4] = ny;
    out[5] = nz;

    /* UV: u = lon / (2*PI), v = lat / PI */
    out[6] = (float)lon_idx / (float)segments;
    out[7] = (float)lat_idx / (float)segments;

    /* Color from config */
    out[8] = config->base_color[0];
    out[9] = config->base_color[1];
    out[10] = config->base_color[2];
}

/* ──────────────────────────────────────────────────────────────────────────
 * psp_pack_sphere — UV-sphere as triangle list
 * ────────────────────────────────────────────────────────────────────── */

int psp_pack_sphere(float center[3], float radius, int segments,
                    psp_config_t config, float *out_verts, int max_verts)
{
    if (radius <= 0.0f) return 0;

    /* Clamp segments to minimum 3 */
    if (segments < 3) segments = 3;

    /* Total vertices: segments * segments * 6 (2 triangles per quad, 3 verts each) */
    int total_verts = segments * segments * 6;
    if (total_verts > max_verts) return 0;

    int idx = 0;
    float v0[PSP_VERTEX_FLOATS], v1[PSP_VERTEX_FLOATS];
    float v2[PSP_VERTEX_FLOATS], v3[PSP_VERTEX_FLOATS];

    for (int lat = 0; lat < segments; lat++) {
        for (int lon = 0; lon < segments; lon++) {
            /* Four corners of the quad */
            sphere_vertex(center, radius, lat, lon, segments, &config, v0);
            sphere_vertex(center, radius, lat + 1, lon, segments, &config, v1);
            sphere_vertex(center, radius, lat, lon + 1, segments, &config, v2);
            sphere_vertex(center, radius, lat + 1, lon + 1, segments,
                          &config, v3);

            /* Triangle 1: v0, v1, v2 */
            for (int f = 0; f < PSP_VERTEX_FLOATS; f++)
                out_verts[idx * PSP_VERTEX_FLOATS + f] = v0[f];
            idx++;
            for (int f = 0; f < PSP_VERTEX_FLOATS; f++)
                out_verts[idx * PSP_VERTEX_FLOATS + f] = v1[f];
            idx++;
            for (int f = 0; f < PSP_VERTEX_FLOATS; f++)
                out_verts[idx * PSP_VERTEX_FLOATS + f] = v2[f];
            idx++;

            /* Triangle 2: v2, v1, v3 */
            for (int f = 0; f < PSP_VERTEX_FLOATS; f++)
                out_verts[idx * PSP_VERTEX_FLOATS + f] = v2[f];
            idx++;
            for (int f = 0; f < PSP_VERTEX_FLOATS; f++)
                out_verts[idx * PSP_VERTEX_FLOATS + f] = v1[f];
            idx++;
            for (int f = 0; f < PSP_VERTEX_FLOATS; f++)
                out_verts[idx * PSP_VERTEX_FLOATS + f] = v3[f];
            idx++;
        }
    }

    return idx;
}

/* ──────────────────────────────────────────────────────────────────────────
 * psp_pack_atmosphere — atmosphere shell (larger sphere with atmo_color)
 * ────────────────────────────────────────────────────────────────────── */

int psp_pack_atmosphere(float center[3], float radius, psp_config_t config,
                        float *out_verts, int max_verts)
{
    /* No atmosphere -> no shell */
    if (config.atmo_thickness <= 0.0f) return 0;

    /* Build a config with atmosphere_color as the base_color */
    psp_config_t atmo_config = config;
    atmo_config.base_color[0] = config.atmosphere_color[0];
    atmo_config.base_color[1] = config.atmosphere_color[1];
    atmo_config.base_color[2] = config.atmosphere_color[2];

    /* Atmosphere shell radius = planet radius * (1 + atmo_thickness) */
    float atmo_radius = radius * (1.0f + config.atmo_thickness);

    /* Use fewer segments for the atmosphere shell (12 is enough for a glow) */
    int atmo_segments = 12;

    return psp_pack_sphere(center, atmo_radius, atmo_segments, atmo_config,
                           out_verts, max_verts);
}

/* ──────────────────────────────────────────────────────────────────────────
 * Internal: Convert ecliptic spherical to Cartesian (Y-up scene coords)
 * ────────────────────────────────────────────────────────────────────── */

static void ecliptic_to_scene(double lon_deg, double lat_deg, double dist_au,
                              float orbit_scale,
                              float *out_x, float *out_y, float *out_z)
{
    double lon_rad = lon_deg * DEG_TO_RAD;
    double lat_rad = lat_deg * DEG_TO_RAD;
    double cos_lat = cos(lat_rad);

    /* sqrt distance compression: r_scene = sqrt(r_AU) * orbit_scale */
    double r_scene = sqrt(dist_au) * (double)orbit_scale;

    /* Ecliptic X -> scene X, ecliptic Y (lat) -> scene Y (up),
     * ecliptic Z (lon sin) -> scene Z */
    *out_x = (float)(r_scene * cos_lat * cos(lon_rad));
    *out_z = (float)(r_scene * cos_lat * sin(lon_rad));
    *out_y = (float)(r_scene * sin(lat_rad));
}

/* ──────────────────────────────────────────────────────────────────────────
 * psp_pack_all — pack all planets as spheres at current positions
 * ────────────────────────────────────────────────────────────────────── */

int psp_pack_all(double jd, float orbit_scale, float *out_verts,
                 int max_verts)
{
    int total = 0;
    int seg = PSP_SPHERE_SEGMENTS;

    for (int i = 0; i < PSP_MAX_PLANETS; i++) {
        psp_config_t config = psp_planet_config((psp_planet_t)i);
        float center[3];
        float radius = PSP_SCENE_RADII[i];

        if (i == PSP_SUN) {
            /* Sun is at the origin */
            center[0] = 0.0f;
            center[1] = 0.0f;
            center[2] = 0.0f;
        } else {
            /* Compute heliocentric position for planet (index i-1 in orbit table) */
            orbital_elements_t elem = orbit_elements_at(PSP_ORBITS[i - 1], jd);
            heliocentric_pos_t pos = orbit_heliocentric(&elem);

            ecliptic_to_scene(pos.longitude, pos.latitude, pos.distance,
                              orbit_scale,
                              &center[0], &center[1], &center[2]);
        }

        /* How many vertices will this planet produce? */
        int needed = seg * seg * 6;
        if (total + needed > max_verts) return 0;

        int count = psp_pack_sphere(center, radius, seg, config,
                                    out_verts + total * PSP_VERTEX_FLOATS,
                                    max_verts - total);
        if (count == 0 && i == 0) {
            /* Sun must succeed at minimum */
            return 0;
        }
        total += count;
    }

    return total;
}

/* ──────────────────────────────────────────────────────────────────────────
 * GLSL ES 3.00 vertex shader — planet surface mesh
 * ────────────────────────────────────────────────────────────────────── */

static const char *s_vert_source =
    "#version 300 es\n"
    "precision highp float;\n"
    "\n"
    "uniform mat4 u_view;\n"
    "uniform mat4 u_proj;\n"
    "uniform mat4 u_model;\n"
    "\n"
    "layout(location = 0) in vec3 a_position;\n"
    "layout(location = 1) in vec3 a_normal;\n"
    "layout(location = 2) in vec2 a_uv;\n"
    "layout(location = 3) in vec3 a_color;\n"
    "\n"
    "out vec3 v_position;\n"
    "out vec3 v_normal;\n"
    "out vec2 v_uv;\n"
    "out vec3 v_color;\n"
    "\n"
    "void main() {\n"
    "    vec4 world_pos = u_model * vec4(a_position, 1.0);\n"
    "    v_position = world_pos.xyz;\n"
    "    v_normal = normalize(mat3(u_model) * a_normal);\n"
    "    v_uv = a_uv;\n"
    "    v_color = a_color;\n"
    "    gl_Position = u_proj * u_view * world_pos;\n"
    "}\n";

/* ──────────────────────────────────────────────────────────────────────────
 * GLSL ES 3.00 fragment shader — procedural planet surfaces
 * ────────────────────────────────────────────────────────────────────── */

static const char *s_frag_preamble =
    "#version 300 es\n"
    "precision highp float;\n"
    "\n"
    "in vec3 v_position;\n"
    "in vec3 v_normal;\n"
    "in vec2 v_uv;\n"
    "in vec3 v_color;\n"
    "\n"
    "uniform int u_planet_type;   /* 0=star, 1=rocky, 2=gas_giant, 3=ice_giant */\n"
    "uniform vec3 u_sun_dir;      /* direction to sun from planet */\n"
    "uniform vec3 u_camera_pos;   /* camera world position */\n"
    "uniform float u_time;        /* animation time in seconds */\n"
    "uniform float u_band_freq;   /* band frequency */\n"
    "uniform float u_band_amp;    /* band amplitude */\n"
    "uniform vec3 u_band_color;   /* secondary band color */\n"
    "uniform float u_noise_scale; /* surface noise scale */\n"
    "uniform float u_noise_amp;   /* surface noise amplitude */\n"
    "uniform vec3 u_atmo_color;   /* atmosphere limb color */\n"
    "uniform float u_atmo_thick;  /* atmosphere thickness */\n"
    "\n"
    "out vec4 frag_color;\n"
    "\n";

/* The noise functions are concatenated between preamble and body */

static const char *s_frag_body =
    "\n"
    "void main() {\n"
    "    vec3 N = normalize(v_normal);\n"
    "    vec3 L = normalize(u_sun_dir);\n"
    "    vec3 V = normalize(u_camera_pos - v_position);\n"
    "\n"
    "    /* Convert normal to spherical for latitude/longitude */\n"
    "    float lat = asin(clamp(N.y, -1.0, 1.0));\n"
    "    float lon = atan(N.z, N.x);\n"
    "\n"
    "    vec3 surface_color = v_color;\n"
    "\n"
    "    /* === Gas Giant Banding === */\n"
    "    if (u_planet_type == 2 || u_planet_type == 3) {\n"
    "        float band_noise = fbm2(vec2(lat * u_band_freq, lon * 0.5\n"
    "                                     + u_time * 0.01));\n"
    "        float bands = sin(lat * u_band_freq + band_noise * 2.0)\n"
    "                      * 0.5 + 0.5;\n"
    "\n"
    "        /* Domain warping for turbulent band edges */\n"
    "        float warp = fbm2(vec2(lat * 4.0 + 100.0,\n"
    "                               lon * 2.0 + u_time * 0.02));\n"
    "        float turb = fbm2(vec2(lat * 6.0 + warp * 1.5,\n"
    "                               lon * 3.0));\n"
    "        float pattern = mix(bands, turb, 0.3) * u_band_amp;\n"
    "\n"
    "        surface_color = mix(surface_color, u_band_color, pattern);\n"
    "\n"
    "        /* Fine surface noise */\n"
    "        float detail = snoise3(vec3(lat * 10.0, lon * 10.0,\n"
    "                                    u_time * 0.05))\n"
    "                       * u_noise_amp * 0.5;\n"
    "        surface_color += detail;\n"
    "    }\n"
    "\n"
    "    /* === Rocky Planet Surface === */\n"
    "    else if (u_planet_type == 1) {\n"
    "        /* Multi-octave noise for surface variation */\n"
    "        float terrain = fbm3(v_normal * u_noise_scale) * 0.5 + 0.5;\n"
    "        float detail = snoise3(v_normal * u_noise_scale * 3.0)\n"
    "                       * u_noise_amp;\n"
    "        surface_color = surface_color * (0.7 + terrain * 0.6)\n"
    "                        + detail * 0.3;\n"
    "    }\n"
    "\n"
    "    /* === Diffuse Lighting === */\n"
    "    float NdotL = dot(N, L);\n"
    "\n"
    "    /* Day/night terminator (smooth transition) */\n"
    "    float day_factor = smoothstep(-0.05, 0.1, NdotL);\n"
    "    vec3 lit_color = surface_color * max(NdotL, 0.0);\n"
    "\n"
    "    /* Night side: very dark with slight ambient */\n"
    "    vec3 night_color = surface_color * 0.02;\n"
    "    vec3 color = mix(night_color, lit_color, day_factor);\n"
    "\n"
    "    /* === Fresnel Atmosphere Limb Glow === */\n"
    "    if (u_atmo_thick > 0.0) {\n"
    "        float rim = 1.0 - max(dot(N, V), 0.0);\n"
    "        float fresnel = pow(rim, 3.0);\n"
    "\n"
    "        /* Modulate by sunlight (some glow on dark side too) */\n"
    "        float sun_facing = max(NdotL, 0.0) * 0.7 + 0.3;\n"
    "        vec3 atmo_glow = u_atmo_color * fresnel * sun_facing\n"
    "                         * u_atmo_thick * 10.0;\n"
    "        color += atmo_glow;\n"
    "    }\n"
    "\n"
    "    /* === Star (Sun) — simple emissive === */\n"
    "    if (u_planet_type == 0) {\n"
    "        color = surface_color;\n"
    "    }\n"
    "\n"
    "    frag_color = vec4(clamp(color, 0.0, 1.0), 1.0);\n"
    "}\n";

/* Concatenated fragment shader: preamble + noise lib + body */
static char s_full_frag[16384];
static int s_frag_built = 0;

static const char *build_frag_source(void)
{
    if (!s_frag_built) {
        char *dst = s_full_frag;
        const char *parts[] = {
            s_frag_preamble,
            noise_shader_source(),
            s_frag_body
        };
        for (int i = 0; i < 3; i++) {
            const char *src = parts[i];
            while (*src && (dst - s_full_frag) < 16382) {
                *dst++ = *src++;
            }
        }
        *dst = '\0';
        s_frag_built = 1;
    }
    return s_full_frag;
}

const char *psp_vert_source(void) { return s_vert_source; }
const char *psp_frag_source(void) { return build_frag_source(); }
