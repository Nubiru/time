/*
 * milkyway_pack.c -- Milky Way band vertex packing
 *
 * Packs galaxy_geometry spiral arm data into triangle quad vertex arrays
 * for Milky Way band rendering. Each consecutive pair of arm points
 * forms a quad segment: two vertices offset above and below the galactic
 * plane, projected onto the celestial sphere. Brightness fades from
 * core to edge. The Great Rift dark lane reduces opacity in the
 * galactic center region.
 *
 * Pure module: no GL calls, no malloc, no globals, no side effects.
 */

#include "milkyway_pack.h"
#include "galaxy_geometry.h"

#include <math.h>

#define PI 3.14159265358979323846

/* Milky Way band base color: milky white with slight blue tint (physical/observational) */
#define MW_COLOR_R 0.85f
#define MW_COLOR_G 0.88f
#define MW_COLOR_B 0.95f

/* ======================================================================
 * mwp_default_config
 * ====================================================================== */

mwp_config_t mwp_default_config(void)
{
    mwp_config_t cfg;
    cfg.sphere_radius   = 200.0f;
    cfg.band_width      = 15.0f;
    cfg.core_brightness = 0.8f;
    cfg.edge_brightness = 0.15f;
    cfg.base_alpha      = 0.4f;
    cfg.dark_lane_factor = 0.3f;
    cfg.galaxy_radius   = 50000.0f;
    cfg.sun_distance    = 26000.0f;
    cfg.rotation_deg    = 0.0f;
    cfg.points_per_arm  = 64;
    return cfg;
}

/* ======================================================================
 * mwp_galactic_to_sphere
 *
 * Convert a galactic plane position (gal_x, gal_z) plus a latitude
 * offset to a 3D point on the celestial sphere.
 *
 * 1. Compute galactic longitude from atan2(gal_z, gal_x)
 * 2. Combine with galactic latitude offset
 * 3. Spherical to Cartesian on the sphere surface
 *
 * Convention: XZ = ecliptic/galactic plane, Y = up (north)
 * ====================================================================== */

void mwp_galactic_to_sphere(float gal_x, float gal_z, float galactic_lat_deg,
                             float sphere_radius, float out[3])
{
    float lon = atan2f(gal_z, gal_x);
    float lat = galactic_lat_deg * (float)PI / 180.0f;

    float cl = cosf(lat);
    out[0] = sphere_radius * cl * cosf(lon);
    out[1] = sphere_radius * sinf(lat);
    out[2] = sphere_radius * cl * sinf(lon);
}

/* ======================================================================
 * mwp_arm_brightness
 *
 * Quadratic falloff from core to edge (smooth taper).
 * t: 0 = center, 1 = edge. Clamped to [0,1].
 * ====================================================================== */

float mwp_arm_brightness(float t, float core_brightness, float edge_brightness)
{
    if (t <= 0.0f) return core_brightness;
    if (t >= 1.0f) return edge_brightness;

    /* Quadratic ease-out: (1-t)^2 maps [0,1] -> [1,0] */
    float inv = 1.0f - t;
    float factor = inv * inv;
    return edge_brightness + factor * (core_brightness - edge_brightness);
}

/* ======================================================================
 * mwp_dark_lane_factor
 *
 * The Great Rift spans roughly galactic longitude 300-360 and 0-30 degrees.
 * We model this as a smooth absorption band using a cosine window centered
 * on longitude 345 with half-width 45 degrees (covers 300-30).
 * ====================================================================== */

float mwp_dark_lane_factor(float galactic_lon_deg, float dark_factor)
{
    if (dark_factor >= 1.0f) return 1.0f;

    /* Normalize longitude to [0, 360) */
    float lon = fmodf(galactic_lon_deg, 360.0f);
    if (lon < 0.0f) lon += 360.0f;

    /* Angular distance from rift center (345 deg), wrapping at 360 */
    float center = 345.0f;
    float half_width = 45.0f;

    float delta = fabsf(lon - center);
    if (delta > 180.0f) delta = 360.0f - delta;

    if (delta >= half_width) return 1.0f;

    /* Smooth cosine window inside the rift zone */
    float t = delta / half_width; /* 0 at center, 1 at edge */
    float window = 0.5f * (1.0f - cosf(t * (float)PI));
    /* window: 0 at center -> 1 at edge */
    return dark_factor + window * (1.0f - dark_factor);
}

/* ======================================================================
 * mwp_pack
 *
 * Algorithm:
 * 1. Generate galaxy arm geometry via galaxy_generate()
 * 2. For each arm, iterate consecutive point pairs
 * 3. For each pair, compute the arm direction vector on the galactic plane
 * 4. Compute a perpendicular (normal) direction
 * 5. Place two vertices above and below the galactic plane
 *    (offset by band_width in latitude)
 * 6. Map all four points to the celestial sphere
 * 7. Form a quad (6 vertices: 2 triangles)
 * 8. Apply brightness and dark lane factor to color/alpha
 *
 * Vertex layout per vertex: pos(3) + uv(2) + color(4) = 9 floats
 * ====================================================================== */

/* UV for quad corners: BL, BR, TR, BL, TR, TL */
static const float QUAD_UVS[6][2] = {
    {0.0f, 0.0f},
    {1.0f, 0.0f},
    {1.0f, 1.0f},
    {0.0f, 0.0f},
    {1.0f, 1.0f},
    {0.0f, 1.0f},
};

static void emit_vertex(float *out, int idx,
                         const float pos[3], float u, float v,
                         float r, float g, float b, float a)
{
    int base = idx * MWP_VERTEX_FLOATS;
    out[base + 0] = pos[0];
    out[base + 1] = pos[1];
    out[base + 2] = pos[2];
    out[base + 3] = u;
    out[base + 4] = v;
    out[base + 5] = r;
    out[base + 6] = g;
    out[base + 7] = b;
    out[base + 8] = a;
}

int mwp_pack(const mwp_config_t *config, float *out)
{
    /* Generate galaxy arm data */
    galaxy_mesh_t mesh = galaxy_generate(config->galaxy_radius,
                                          config->sun_distance,
                                          config->rotation_deg,
                                          config->points_per_arm);

    int vertex_count = 0;

    for (int arm = 0; arm < GALAXY_ARM_COUNT; arm++) {
        const galaxy_arm_t *a = &mesh.arms[arm];
        if (a->count < 2) continue;

        for (int i = 0; i < a->count - 1; i++) {
            /* Current and next point on the galactic plane */
            float x0 = a->positions[i * 3 + 0];
            float z0 = a->positions[i * 3 + 2];
            float x1 = a->positions[(i + 1) * 3 + 0];
            float z1 = a->positions[(i + 1) * 3 + 2];

            /* Galactic longitude (degrees) of current point for dark lane */
            float lon_deg = atan2f(z0, x0) * 180.0f / (float)PI;
            if (lon_deg < 0.0f) lon_deg += 360.0f;

            /* Average brightness of the two endpoints */
            float bright0 = a->brightnesses[i];
            float bright1 = a->brightnesses[i + 1];
            float avg_bright = 0.5f * (bright0 + bright1);

            /* Map brightness through the gradient:
             * bright=1.0 -> core_brightness, bright=0.0 -> edge_brightness */
            float t_edge = 1.0f - avg_bright;
            float brightness = mwp_arm_brightness(t_edge,
                                                   config->core_brightness,
                                                   config->edge_brightness);

            /* Dark lane factor */
            float dlf = mwp_dark_lane_factor(lon_deg, config->dark_lane_factor);

            /* Final alpha */
            float alpha = config->base_alpha * brightness * dlf;

            /* Final color (physical milky white/blue-white glow, not theme) */
            float cr = MW_COLOR_R * brightness;
            float cg = MW_COLOR_G * brightness;
            float cb = MW_COLOR_B * brightness;

            /* Four sphere positions for the quad:
             * Two at current point (upper/lower latitude offset)
             * Two at next point (upper/lower latitude offset) */
            float p0_upper[3], p0_lower[3], p1_upper[3], p1_lower[3];

            mwp_galactic_to_sphere(x0, z0, config->band_width,
                                    config->sphere_radius, p0_upper);
            mwp_galactic_to_sphere(x0, z0, -config->band_width,
                                    config->sphere_radius, p0_lower);
            mwp_galactic_to_sphere(x1, z1, config->band_width,
                                    config->sphere_radius, p1_upper);
            mwp_galactic_to_sphere(x1, z1, -config->band_width,
                                    config->sphere_radius, p1_lower);

            /* Guard against buffer overflow */
            if (vertex_count + 6 > MWP_MAX_VERTICES) goto done;

            /* Quad: BL=p0_lower, BR=p1_lower, TR=p1_upper, TL=p0_upper */
            /* Triangle 1: BL, BR, TR */
            emit_vertex(out, vertex_count + 0, p0_lower,
                         QUAD_UVS[0][0], QUAD_UVS[0][1], cr, cg, cb, alpha);
            emit_vertex(out, vertex_count + 1, p1_lower,
                         QUAD_UVS[1][0], QUAD_UVS[1][1], cr, cg, cb, alpha);
            emit_vertex(out, vertex_count + 2, p1_upper,
                         QUAD_UVS[2][0], QUAD_UVS[2][1], cr, cg, cb, alpha);

            /* Triangle 2: BL, TR, TL */
            emit_vertex(out, vertex_count + 3, p0_lower,
                         QUAD_UVS[3][0], QUAD_UVS[3][1], cr, cg, cb, alpha);
            emit_vertex(out, vertex_count + 4, p1_upper,
                         QUAD_UVS[4][0], QUAD_UVS[4][1], cr, cg, cb, alpha);
            emit_vertex(out, vertex_count + 5, p0_upper,
                         QUAD_UVS[5][0], QUAD_UVS[5][1], cr, cg, cb, alpha);

            vertex_count += 6;
        }
    }

done:
    return vertex_count;
}

/* ======================================================================
 * mwp_info
 * ====================================================================== */

mwp_info_t mwp_info(int vertex_count)
{
    mwp_info_t info;
    info.vertex_count  = vertex_count;
    info.float_count   = vertex_count * MWP_VERTEX_FLOATS;
    info.arm_count     = GALAXY_ARM_COUNT;
    info.segment_count = vertex_count / 6;
    return info;
}

/* ======================================================================
 * GLSL ES 3.00 shader source strings
 * ====================================================================== */

/* Milky Way band vertex shader.
 * Passes position, UV, and per-vertex color to fragment stage. */
static const char MWP_VERT_SRC[] =
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

/* Milky Way band fragment shader.
 * Lateral falloff using UV.y distance from band center (0.5).
 * Exponential decay gives the soft milky glow effect.
 * v_uv.y: 0 = lower edge, 1 = upper edge, 0.5 = band center. */
static const char MWP_FRAG_SRC[] =
    "#version 300 es\n"
    "precision highp float;\n"
    "\n"
    "in vec2 v_uv;\n"
    "in vec4 v_color;\n"
    "out vec4 frag_color;\n"
    "\n"
    "void main() {\n"
    "    float d = abs(v_uv.y - 0.5) * 2.0;\n"
    "    float falloff = exp(-4.0 * d * d);\n"
    "    if (falloff < 0.01) discard;\n"
    "    frag_color = vec4(v_color.rgb, v_color.a * falloff);\n"
    "}\n";

const char *mwp_vert_source(void)
{
    return MWP_VERT_SRC;
}

const char *mwp_frag_source(void)
{
    return MWP_FRAG_SRC;
}
