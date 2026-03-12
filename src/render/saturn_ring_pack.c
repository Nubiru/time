/*
 * saturn_ring_pack.c — Saturn ring system render pack: GPU-ready vertex packing
 *
 * Packs Saturn's 7 ring components (D, C, B, Cassini Division, A, Encke Gap, F)
 * into an annular disc mesh with per-vertex opacity and color. Ring radii are
 * sourced from NASA/JPL Voyager/Cassini measurements in Saturn radii
 * (1 Rs = 60,268 km).
 *
 * The mesh is a tilted annular disc: vertices are placed in the XZ plane then
 * rotated around X by Saturn's axial tilt (26.73 degrees). Normals are the
 * tilted plane normal. Texcoords encode angular (u) and radial (v) position.
 *
 * Pure module: no GL calls, no malloc, no globals, no side effects.
 */

#include "saturn_ring_pack.h"

#include <math.h>
#include <string.h>
#include <stddef.h>

#define DEG_TO_RAD (PI / 180.0)

/* ══════════════════════════════════════════════════════════════════════════
 * Ring data — NASA/JPL (radii in Saturn radii)
 * ══════════════════════════════════════════════════════════════════════════ */

#define SRP_RING_COUNT 7

static const srp_ring_t RING_DATA[SRP_RING_COUNT] = {
    { "D ring",           1.110f, 1.236f, 0.01f, 0.10f },
    { "C ring",           1.239f, 1.527f, 0.15f, 0.20f },
    { "B ring",           1.527f, 1.951f, 0.80f, 0.60f },
    { "Cassini Division", 1.951f, 2.025f, 0.05f, 0.10f },
    { "A ring",           2.025f, 2.269f, 0.50f, 0.50f },
    { "Encke Gap",        2.214f, 2.222f, 0.01f, 0.05f },
    { "F ring",           2.320f, 2.328f, 0.10f, 0.30f },
};

/* ══════════════════════════════════════════════════════════════════════════
 * Ring color data — radial color gradient
 *
 * Colors based on Cassini true-color composites:
 *   D ring: very dark gray
 *   C ring: dark gray-brown (contaminants)
 *   B ring inner: warm beige/cream (dirtier ice, silicates)
 *   B ring mid: bright cream (cleanest ice)
 *   A ring: slightly cooler gray-cream
 *   F ring: medium gray
 * ══════════════════════════════════════════════════════════════════════════ */

typedef struct {
    float radius;    /* Saturn radii */
    float r, g, b;
} srp_color_stop_t;

#define SRP_COLOR_STOPS 8

static const srp_color_stop_t COLOR_STOPS[SRP_COLOR_STOPS] = {
    { 1.110f, 0.30f, 0.28f, 0.25f },  /* D ring: very dark */
    { 1.239f, 0.45f, 0.42f, 0.38f },  /* C ring inner: dark gray-brown */
    { 1.527f, 0.65f, 0.60f, 0.52f },  /* C-B boundary */
    { 1.700f, 0.85f, 0.80f, 0.70f },  /* Inner B ring: warm beige */
    { 1.800f, 0.90f, 0.87f, 0.82f },  /* Mid B ring: bright cream */
    { 1.951f, 0.88f, 0.85f, 0.78f },  /* B-Cassini boundary */
    { 2.025f, 0.82f, 0.82f, 0.80f },  /* A ring: cooler gray-cream */
    { 2.328f, 0.55f, 0.52f, 0.48f },  /* F ring: medium gray */
};

/* ══════════════════════════════════════════════════════════════════════════
 * Default configuration
 * ══════════════════════════════════════════════════════════════════════════ */

srp_config_t srp_default_config(void)
{
    srp_config_t cfg;
    cfg.saturn_scene_radius = 1.0f;
    cfg.axial_tilt_deg = 26.73f;
    cfg.radial_segments = 16;
    cfg.angular_segments = 72;
    return cfg;
}

/* ══════════════════════════════════════════════════════════════════════════
 * Ring data access
 * ══════════════════════════════════════════════════════════════════════════ */

int srp_ring_count(void)
{
    return SRP_RING_COUNT;
}

srp_ring_t srp_ring_get(int index)
{
    if (index < 0 || index >= SRP_RING_COUNT) {
        srp_ring_t sentinel;
        sentinel.name = NULL;
        sentinel.inner_radius = 0.0f;
        sentinel.outer_radius = 0.0f;
        sentinel.opacity = 0.0f;
        sentinel.albedo = 0.0f;
        return sentinel;
    }
    return RING_DATA[index];
}

int srp_find_ring(const char *name)
{
    if (name == NULL) return -1;
    for (int i = 0; i < SRP_RING_COUNT; i++) {
        if (strcmp(RING_DATA[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

/* ══════════════════════════════════════════════════════════════════════════
 * Scene radius conversion
 * ══════════════════════════════════════════════════════════════════════════ */

float srp_to_scene_radius(float saturn_radii, float saturn_scene_radius)
{
    return saturn_radii * saturn_scene_radius;
}

/* ══════════════════════════════════════════════════════════════════════════
 * Opacity interpolation
 * ══════════════════════════════════════════════════════════════════════════ */

float srp_opacity_at_radius(float radius_sr)
{
    /* Find the narrowest ring band containing this radius.
     * Needed because the Encke Gap (2.214-2.222) is physically inside
     * the A ring (2.025-2.269). The narrower band takes priority. */
    int best = -1;
    float best_width = 1e10f;

    for (int i = 0; i < SRP_RING_COUNT; i++) {
        if (radius_sr >= RING_DATA[i].inner_radius &&
            radius_sr <= RING_DATA[i].outer_radius) {
            float width = RING_DATA[i].outer_radius - RING_DATA[i].inner_radius;
            if (width < best_width) {
                best_width = width;
                best = i;
            }
        }
    }

    return (best >= 0) ? RING_DATA[best].opacity : 0.0f;
}

/* ══════════════════════════════════════════════════════════════════════════
 * Color interpolation
 * ══════════════════════════════════════════════════════════════════════════ */

void srp_color_at_radius(float radius_sr, float rgb[3])
{
    /* Check if radius is within the ring system at all */
    if (radius_sr < COLOR_STOPS[0].radius ||
        radius_sr > COLOR_STOPS[SRP_COLOR_STOPS - 1].radius) {
        rgb[0] = 0.0f;
        rgb[1] = 0.0f;
        rgb[2] = 0.0f;
        return;
    }

    /* Find the two color stops bracketing this radius and interpolate */
    for (int i = 0; i < SRP_COLOR_STOPS - 1; i++) {
        if (radius_sr >= COLOR_STOPS[i].radius &&
            radius_sr <= COLOR_STOPS[i + 1].radius) {
            float range = COLOR_STOPS[i + 1].radius - COLOR_STOPS[i].radius;
            float t = 0.0f;
            if (range > 0.0001f) {
                t = (radius_sr - COLOR_STOPS[i].radius) / range;
            }
            rgb[0] = COLOR_STOPS[i].r + t * (COLOR_STOPS[i + 1].r - COLOR_STOPS[i].r);
            rgb[1] = COLOR_STOPS[i].g + t * (COLOR_STOPS[i + 1].g - COLOR_STOPS[i].g);
            rgb[2] = COLOR_STOPS[i].b + t * (COLOR_STOPS[i + 1].b - COLOR_STOPS[i].b);
            return;
        }
    }

    /* Exact match on last stop */
    rgb[0] = COLOR_STOPS[SRP_COLOR_STOPS - 1].r;
    rgb[1] = COLOR_STOPS[SRP_COLOR_STOPS - 1].g;
    rgb[2] = COLOR_STOPS[SRP_COLOR_STOPS - 1].b;
}

/* ══════════════════════════════════════════════════════════════════════════
 * Gap detection
 * ══════════════════════════════════════════════════════════════════════════ */

int srp_is_gap(float radius_sr)
{
    /* Cassini Division: index 3 */
    if (radius_sr >= RING_DATA[3].inner_radius &&
        radius_sr <= RING_DATA[3].outer_radius) {
        return 1;
    }
    /* Encke Gap: index 5 */
    if (radius_sr >= RING_DATA[5].inner_radius &&
        radius_sr <= RING_DATA[5].outer_radius) {
        return 1;
    }
    return 0;
}

/* ══════════════════════════════════════════════════════════════════════════
 * Mesh packing — annular disc with axial tilt
 * ══════════════════════════════════════════════════════════════════════════ */

srp_info_t srp_pack(srp_config_t config, float *out_verts,
                     unsigned short *out_indices)
{
    srp_info_t info;
    info.vertex_count = 0;
    info.index_count = 0;
    info.triangle_count = 0;
    info.ring_count = SRP_RING_COUNT;

    float tilt_rad = (float)(config.axial_tilt_deg * DEG_TO_RAD);
    float cos_tilt = cosf(tilt_rad);
    float sin_tilt = sinf(tilt_rad);

    /* Normal vector for tilted ring plane: (0, cos(tilt), -sin(tilt)) */
    float norm_x = 0.0f;
    float norm_y = cos_tilt;
    float norm_z = -sin_tilt;

    float scale = config.saturn_scene_radius;
    int ang_segs = config.angular_segments;
    int rad_segs = config.radial_segments;

    /* Clamp segments to reasonable minimums */
    if (ang_segs < 3) ang_segs = 3;
    if (rad_segs < 1) rad_segs = 1;

    /* For each ring band, generate vertices and indices */
    for (int ring = 0; ring < SRP_RING_COUNT; ring++) {
        float r_inner = RING_DATA[ring].inner_radius;
        float r_outer = RING_DATA[ring].outer_radius;
        float opacity = RING_DATA[ring].opacity;

        /* Normalized radial position within entire ring system for texcoord v */
        float sys_inner = RING_DATA[0].inner_radius;
        float sys_outer = RING_DATA[SRP_RING_COUNT - 1].outer_radius;
        float sys_range = sys_outer - sys_inner;

        int first_vert = info.vertex_count;

        /* Generate vertices: (rad_segs + 1) radial * (ang_segs + 1) angular */
        for (int ai = 0; ai <= ang_segs; ai++) {
            float angle = (float)ai / (float)ang_segs * 2.0f * (float)PI;
            float u = (float)ai / (float)ang_segs;
            float cos_a = cosf(angle);
            float sin_a = sinf(angle);

            for (int ri = 0; ri <= rad_segs; ri++) {
                float t = (float)ri / (float)rad_segs;
                float radius = r_inner + t * (r_outer - r_inner);

                /* Position in ring plane (XZ), then rotate by tilt around X */
                float px = radius * cos_a * scale;
                float py_flat = 0.0f;
                float pz_flat = radius * sin_a * scale;

                /* Apply axial tilt rotation around X axis:
                 * y' = y*cos(tilt) - z*sin(tilt)
                 * z' = y*sin(tilt) + z*cos(tilt) */
                float py = py_flat * cos_tilt - pz_flat * sin_tilt;
                float pz = py_flat * sin_tilt + pz_flat * cos_tilt;

                /* Texcoord: u = angle/360, v = radial position in system */
                float v = (sys_range > 0.0001f) ?
                    (radius - sys_inner) / sys_range : 0.0f;

                /* Per-vertex opacity from ring data */
                float vert_opacity = opacity;

                int vi = info.vertex_count;
                if (vi >= SRP_MAX_VERTICES) {
                    /* Safety: don't overflow buffer */
                    goto done;
                }

                int base = vi * SRP_VERTEX_FLOATS;
                out_verts[base + 0] = px;
                out_verts[base + 1] = py;
                out_verts[base + 2] = pz;
                out_verts[base + 3] = norm_x;
                out_verts[base + 4] = norm_y;
                out_verts[base + 5] = norm_z;
                out_verts[base + 6] = u;
                out_verts[base + 7] = v;
                out_verts[base + 8] = vert_opacity;

                info.vertex_count++;
            }
        }

        /* Generate triangle indices for this ring band's quad grid.
         * Grid is (ang_segs + 1) wide x (rad_segs + 1) tall.
         * Each quad: 2 triangles. */
        int row_stride = rad_segs + 1;

        for (int ai = 0; ai < ang_segs; ai++) {
            for (int ri = 0; ri < rad_segs; ri++) {
                int v00 = first_vert + ai * row_stride + ri;
                int v01 = first_vert + ai * row_stride + ri + 1;
                int v10 = first_vert + (ai + 1) * row_stride + ri;
                int v11 = first_vert + (ai + 1) * row_stride + ri + 1;

                /* Triangle 1: v00, v10, v01 */
                out_indices[info.index_count++] = (unsigned short)v00;
                out_indices[info.index_count++] = (unsigned short)v10;
                out_indices[info.index_count++] = (unsigned short)v01;

                /* Triangle 2: v01, v10, v11 */
                out_indices[info.index_count++] = (unsigned short)v01;
                out_indices[info.index_count++] = (unsigned short)v10;
                out_indices[info.index_count++] = (unsigned short)v11;

                info.triangle_count += 2;
            }
        }
    }

done:
    return info;
}

/* ══════════════════════════════════════════════════════════════════════════
 * GLSL ES 3.00 Shader Source Strings
 * ══════════════════════════════════════════════════════════════════════════ */

static const char SRP_VERT_SOURCE[] =
    "#version 300 es\n"
    "precision highp float;\n"
    "\n"
    "layout(location = 0) in vec3 a_position;\n"
    "layout(location = 1) in vec3 a_normal;\n"
    "layout(location = 2) in vec2 a_texcoord;\n"
    "layout(location = 3) in float a_opacity;\n"
    "\n"
    "uniform mat4 u_mvp;\n"
    "uniform mat4 u_model;\n"
    "\n"
    "out vec2 v_texcoord;\n"
    "out vec3 v_normal;\n"
    "out vec3 v_world_pos;\n"
    "out float v_opacity;\n"
    "\n"
    "void main() {\n"
    "    vec4 world = u_model * vec4(a_position, 1.0);\n"
    "    v_world_pos = world.xyz;\n"
    "    v_normal = mat3(u_model) * a_normal;\n"
    "    v_texcoord = a_texcoord;\n"
    "    v_opacity = a_opacity;\n"
    "    gl_Position = u_mvp * vec4(a_position, 1.0);\n"
    "}\n";

static const char SRP_FRAG_SOURCE[] =
    "#version 300 es\n"
    "precision highp float;\n"
    "\n"
    "in vec2 v_texcoord;\n"
    "in vec3 v_normal;\n"
    "in vec3 v_world_pos;\n"
    "in float v_opacity;\n"
    "\n"
    "uniform vec3 u_sun_dir;\n"
    "uniform vec3 u_saturn_pos;\n"
    "uniform float u_saturn_radius;\n"
    "\n"
    "out vec4 frag_color;\n"
    "\n"
    "void main() {\n"
    "    /* Lighting: simple diffuse from sun */\n"
    "    float ndl = max(dot(normalize(v_normal), normalize(u_sun_dir)), 0.0);\n"
    "    float ambient = 0.15;\n"
    "    float light = ambient + (1.0 - ambient) * ndl;\n"
    "\n"
    "    /* Ring color from v_texcoord.y (radial position) */\n"
    "    /* Warm beige inner -> cooler gray outer */\n"
    "    vec3 inner_color = vec3(0.90, 0.87, 0.82);\n"
    "    vec3 outer_color = vec3(0.78, 0.77, 0.75);\n"
    "    vec3 color = mix(inner_color, outer_color, v_texcoord.y);\n"
    "\n"
    "    /* Saturn shadow: check if ring point is behind Saturn body */\n"
    "    vec3 to_saturn = u_saturn_pos - v_world_pos;\n"
    "    float dist_to_axis = length(to_saturn - dot(to_saturn, normalize(u_sun_dir)) * normalize(u_sun_dir));\n"
    "    float in_shadow = 1.0 - smoothstep(u_saturn_radius * 0.9, u_saturn_radius * 1.1, dist_to_axis);\n"
    "    light *= (1.0 - in_shadow * 0.7);\n"
    "\n"
    "    color *= light;\n"
    "    float alpha = v_opacity;\n"
    "    if (alpha < 0.01) discard;\n"
    "    frag_color = vec4(color, alpha);\n"
    "}\n";

const char *srp_vert_source(void)
{
    return SRP_VERT_SOURCE;
}

const char *srp_frag_source(void)
{
    return SRP_FRAG_SOURCE;
}
