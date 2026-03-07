/*
 * deep_sky_pack.c -- Deep sky object billboard vertex packing
 *
 * Packs the Messier catalog (110 DSOs) into billboard quad vertex arrays.
 * Each DSO becomes a camera-facing quad with type-based coloring and
 * magnitude-dependent opacity. Includes GLSL ES 3.00 shader source
 * with gaussian radial falloff.
 *
 * Pure module: no GL calls, no malloc, no globals, no side effects.
 */

#include "deep_sky_pack.h"
#include "deep_sky.h"

#include <math.h>

#ifndef PI
#define PI 3.14159265358979323846
#endif

/* Earth's mean obliquity J2000 in degrees */
#define OBLIQUITY_DEG 23.44f

/* ======================================================================
 * dsp_default_config
 * ====================================================================== */

dsp_config_t dsp_default_config(void)
{
    dsp_config_t cfg;
    cfg.sphere_radius = 100.0f;
    cfg.base_size     = 0.5f;
    cfg.min_size      = 0.3f;
    cfg.max_size      = 8.0f;
    cfg.base_alpha    = 0.6f;
    cfg.min_alpha     = 0.1f;
    cfg.bright_mag    = 3.0f;
    cfg.faint_mag     = 10.0f;
    return cfg;
}

/* ======================================================================
 * dsp_ra_dec_to_xyz — RA/Dec to ecliptic Cartesian
 *
 * Matches star_to_ecliptic_xyz convention from star_catalog.c:
 *   1. RA/Dec -> equatorial Cartesian (unit sphere)
 *   2. Rotate by obliquity around X-axis (equatorial -> ecliptic)
 *   3. Project convention: XZ = ecliptic plane, Y = ecliptic north
 *   4. Scale by radius
 * ====================================================================== */

void dsp_ra_dec_to_xyz(double ra_hours, double dec_deg, float radius,
                       float obliquity_deg, float out[3])
{
    /* Step 1: equatorial Cartesian on unit sphere */
    double ra_rad  = ra_hours * PI / 12.0;
    double dec_rad = dec_deg  * PI / 180.0;
    double cd      = cos(dec_rad);
    double eq_x    = cd * cos(ra_rad);
    double eq_y    = cd * sin(ra_rad);
    double eq_z    = sin(dec_rad);

    /* Step 2: rotate around X-axis by obliquity */
    double obl = (double)obliquity_deg * PI / 180.0;
    double co  = cos(obl);
    double so  = sin(obl);
    double x_ecl =  eq_x;
    double y_ecl =  eq_y * co + eq_z * so;
    double z_ecl = -eq_y * so + eq_z * co;

    /* Step 3: project convention — XZ = ecliptic plane, Y = ecliptic north */
    out[0] = (float)(x_ecl * (double)radius);
    out[1] = (float)(z_ecl * (double)radius);
    out[2] = (float)(y_ecl * (double)radius);
}

/* ======================================================================
 * dsp_billboard_size — angular size to billboard size
 * ====================================================================== */

float dsp_billboard_size(double angular_size_arcmin, float base_size,
                         float min_size, float max_size)
{
    float raw = (float)angular_size_arcmin * base_size;
    if (raw < min_size) return min_size;
    if (raw > max_size) return max_size;
    return raw;
}

/* ======================================================================
 * dsp_magnitude_alpha — apparent magnitude to alpha
 * ====================================================================== */

float dsp_magnitude_alpha(double magnitude, float base_alpha, float min_alpha,
                          float bright_mag, float faint_mag)
{
    if (magnitude <= (double)bright_mag) return base_alpha;
    if (magnitude >= (double)faint_mag)  return min_alpha;

    /* Linear interpolation: bright_mag -> base_alpha, faint_mag -> min_alpha */
    double t = (magnitude - (double)bright_mag)
             / ((double)faint_mag - (double)bright_mag);
    return base_alpha + (float)t * (min_alpha - base_alpha);
}

/* ======================================================================
 * dsp_type_color — observational color per DSO type
 *
 * Physical/observational basis:
 *   Emission nebula: H-alpha red/pink
 *   Reflection nebula: scattered blue
 *   Planetary nebula: O-III teal/green
 *   Dark nebula: dark grey (absorption)
 *   Supernova remnant: bright pink/white
 *   Open cluster: warm white (young stars)
 *   Globular cluster: yellow-white (old stars)
 *   Spiral galaxy: pale blue
 *   Elliptical galaxy: warm gold
 *   Irregular galaxy: blue-white
 * ====================================================================== */

static const float DSP_TYPE_COLORS[10][3] = {
    {0.90f, 0.90f, 0.80f},  /* DSO_OPEN_CLUSTER      — warm white */
    {0.90f, 0.85f, 0.70f},  /* DSO_GLOBULAR_CLUSTER   — yellow-white */
    {0.90f, 0.30f, 0.30f},  /* DSO_EMISSION_NEBULA    — H-alpha red/pink */
    {0.30f, 0.40f, 0.90f},  /* DSO_REFLECTION_NEBULA  — blue */
    {0.20f, 0.80f, 0.70f},  /* DSO_PLANETARY_NEBULA   — teal/green */
    {0.15f, 0.15f, 0.20f},  /* DSO_DARK_NEBULA        — dark grey */
    {0.90f, 0.50f, 0.60f},  /* DSO_SUPERNOVA_REMNANT  — bright pink/white */
    {0.70f, 0.75f, 0.90f},  /* DSO_SPIRAL_GALAXY      — pale blue */
    {0.90f, 0.80f, 0.60f},  /* DSO_ELLIPTICAL_GALAXY  — warm gold */
    {0.70f, 0.80f, 0.95f},  /* DSO_IRREGULAR_GALAXY   — blue-white */
};

void dsp_type_color(int type, float alpha, float out[4])
{
    if (type < 0 || type >= 10) {
        out[0] = 0.5f;
        out[1] = 0.5f;
        out[2] = 0.5f;
        out[3] = alpha;
        return;
    }
    out[0] = DSP_TYPE_COLORS[type][0];
    out[1] = DSP_TYPE_COLORS[type][1];
    out[2] = DSP_TYPE_COLORS[type][2];
    out[3] = alpha;
}

/* ======================================================================
 * dsp_pack — pack all DSOs into billboard quads
 *
 * Quad vertex order per DSO (2 triangles):
 *   0: BL (center - half*right - half*up)   UV(0,0)
 *   1: BR (center + half*right - half*up)   UV(1,0)
 *   2: TR (center + half*right + half*up)   UV(1,1)
 *   3: BL (same as 0)                       UV(0,0)
 *   4: TR (same as 2)                       UV(1,1)
 *   5: TL (center - half*right + half*up)   UV(0,1)
 * ====================================================================== */

/* UV coordinates for the 6 vertices (BL, BR, TR, BL, TR, TL) */
static const float QUAD_UVS[6][2] = {
    {0.0f, 0.0f}, /* BL */
    {1.0f, 0.0f}, /* BR */
    {1.0f, 1.0f}, /* TR */
    {0.0f, 0.0f}, /* BL */
    {1.0f, 1.0f}, /* TR */
    {0.0f, 1.0f}, /* TL */
};

/* Corner offsets: -1 or +1 for right and up directions */
static const float QUAD_OFFSETS[6][2] = {
    {-1.0f, -1.0f}, /* BL */
    { 1.0f, -1.0f}, /* BR */
    { 1.0f,  1.0f}, /* TR */
    {-1.0f, -1.0f}, /* BL */
    { 1.0f,  1.0f}, /* TR */
    {-1.0f,  1.0f}, /* TL */
};

int dsp_pack(const float cam_right[3], const float cam_up[3],
             const dsp_config_t *config, float *out)
{
    int count = dso_catalog_count();
    int packed = 0;

    for (int i = 0; i < count; i++) {
        dso_entry_t entry = dso_catalog_get(i);
        if (entry.designation == (void *)0) continue;

        /* 3D position on celestial sphere */
        float center[3];
        dsp_ra_dec_to_xyz(entry.ra_hours, entry.dec_deg,
                          config->sphere_radius, OBLIQUITY_DEG, center);

        /* Billboard half-size from angular diameter */
        float half = dsp_billboard_size(entry.angular_size_arcmin,
                                        config->base_size,
                                        config->min_size,
                                        config->max_size) * 0.5f;

        /* Alpha from apparent magnitude */
        float alpha = dsp_magnitude_alpha(entry.apparent_mag,
                                          config->base_alpha,
                                          config->min_alpha,
                                          config->bright_mag,
                                          config->faint_mag);

        /* Type-based color */
        float color[4];
        dsp_type_color((int)entry.type, alpha, color);

        /* Pack 6 vertices */
        int base = packed * DSP_VERTS_PER_OBJECT * DSP_VERTEX_FLOATS;
        for (int v = 0; v < DSP_VERTS_PER_OBJECT; v++) {
            int vi = base + v * DSP_VERTEX_FLOATS;
            float or_x = QUAD_OFFSETS[v][0];
            float ou_y = QUAD_OFFSETS[v][1];

            /* Position = center + half * (or_x * cam_right + ou_y * cam_up) */
            out[vi + 0] = center[0]
                        + half * (or_x * cam_right[0] + ou_y * cam_up[0]);
            out[vi + 1] = center[1]
                        + half * (or_x * cam_right[1] + ou_y * cam_up[1]);
            out[vi + 2] = center[2]
                        + half * (or_x * cam_right[2] + ou_y * cam_up[2]);

            /* UV */
            out[vi + 3] = QUAD_UVS[v][0];
            out[vi + 4] = QUAD_UVS[v][1];

            /* Color RGBA */
            out[vi + 5] = color[0];
            out[vi + 6] = color[1];
            out[vi + 7] = color[2];
            out[vi + 8] = color[3];
        }

        packed++;
    }

    return packed;
}

/* ======================================================================
 * dsp_info — metadata from object count
 * ====================================================================== */

dsp_info_t dsp_info(int object_count)
{
    dsp_info_t info;
    info.object_count  = object_count;
    info.vertex_count  = object_count * DSP_VERTS_PER_OBJECT;
    info.float_count   = info.vertex_count * DSP_VERTEX_FLOATS;

    /* Count by category from catalog */
    info.nebula_count  = 0;
    info.cluster_count = 0;
    info.galaxy_count  = 0;

    int catalog_size = dso_catalog_count();
    int limit = object_count < catalog_size ? object_count : catalog_size;

    for (int i = 0; i < limit; i++) {
        dso_entry_t entry = dso_catalog_get(i);
        switch (entry.type) {
        case DSO_EMISSION_NEBULA:
        case DSO_REFLECTION_NEBULA:
        case DSO_PLANETARY_NEBULA:
        case DSO_DARK_NEBULA:
        case DSO_SUPERNOVA_REMNANT:
            info.nebula_count++;
            break;
        case DSO_OPEN_CLUSTER:
        case DSO_GLOBULAR_CLUSTER:
            info.cluster_count++;
            break;
        case DSO_SPIRAL_GALAXY:
        case DSO_ELLIPTICAL_GALAXY:
        case DSO_IRREGULAR_GALAXY:
            info.galaxy_count++;
            break;
        default:
            break;
        }
    }

    return info;
}

/* ======================================================================
 * GLSL ES 3.00 shader source strings
 * ====================================================================== */

/* Deep sky billboard vertex shader.
 * Uniforms: u_mvp (mat4).
 * Attributes: a_position (vec3, loc 0), a_uv (vec2, loc 1),
 *             a_color (vec4, loc 2).
 * Passes UV and color to fragment shader. */
static const char DSP_VERT_SRC[] =
    "#version 300 es\n"
    "precision highp float;\n"
    "\n"
    "uniform mat4 u_mvp;\n"
    "\n"
    "layout(location = 0) in vec3 a_position;\n"
    "layout(location = 1) in vec2 a_uv;\n"
    "layout(location = 2) in vec4 a_color;\n"
    "\n"
    "out vec2 v_uv;\n"
    "out vec4 v_color;\n"
    "\n"
    "void main() {\n"
    "    gl_Position = u_mvp * vec4(a_position, 1.0);\n"
    "    v_uv = a_uv;\n"
    "    v_color = a_color;\n"
    "}\n";

/* Deep sky billboard fragment shader.
 * Gaussian radial falloff from UV center (0.5, 0.5).
 * exp(-8.0 * r*r) gives soft circular nebula appearance.
 * Output alpha = gaussian * vertex color alpha. */
static const char DSP_FRAG_SRC[] =
    "#version 300 es\n"
    "precision highp float;\n"
    "\n"
    "in vec2 v_uv;\n"
    "in vec4 v_color;\n"
    "out vec4 frag_color;\n"
    "\n"
    "void main() {\n"
    "    vec2 d = v_uv - vec2(0.5);\n"
    "    float r = length(d) * 2.0;\n"
    "    float falloff = exp(-8.0 * r * r);\n"
    "    if (falloff < 0.01) discard;\n"
    "    frag_color = vec4(v_color.rgb, v_color.a * falloff);\n"
    "}\n";

const char *dsp_vert_source(void)
{
    return DSP_VERT_SRC;
}

const char *dsp_frag_source(void)
{
    return DSP_FRAG_SRC;
}
