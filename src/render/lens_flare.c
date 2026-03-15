/*
 * lens_flare.c — Procedural screen-space lens flare geometry
 *
 * Generates screen-space quad vertices for cinematic lens flare effects.
 * All positions are in NDC (-1 to +1). Each flare element becomes a quad
 * (2 triangles = 6 vertices) with position, UV, and color attributes.
 *
 * Element types: starburst, halo, ghost, streak, ring.
 * Ghosts are placed along the flare axis (source -> center -> opposite).
 *
 * Phi-derived sizing creates natural visual hierarchy.
 *
 * Pure module: no GL calls, no malloc, no globals, no side effects.
 */

#include "lens_flare.h"

#include <math.h>
#include <stddef.h>

/* Phi constants for sizing hierarchy */
#define LF_PHI       1.6180339887498948482
#define LF_PHI_INV   0.6180339887498948482
#define LF_PHI_SQ    2.6180339887498948482
#define LF_PHI_INV2  0.3819660112501051518

/* Brand colors */
#define LF_SOLAR_GOLD_R 1.0f
#define LF_SOLAR_GOLD_G 0.85f
#define LF_SOLAR_GOLD_B 0.55f

/* ──────────────────────────────────────────────────────────────────────────
 * Internal: write one vertex into buffer.
 * Returns LF_VERTEX_FLOATS.
 * ────────────────────────────────────────────────────────────────────── */

static int write_vertex(float *out, float px, float py,
                        float u, float v,
                        float r, float g, float b, float a)
{
    out[0] = px;
    out[1] = py;
    out[2] = u;
    out[3] = v;
    out[4] = r;
    out[5] = g;
    out[6] = b;
    out[7] = a;
    return LF_VERTEX_FLOATS;
}

/* ──────────────────────────────────────────────────────────────────────────
 * Internal: pack a single quad (2 triangles = 6 vertices) into buffer.
 * center: NDC position of quad center.
 * half_w, half_h: half-extents in NDC.
 * color: RGBA.
 * rotation: angle in radians.
 * Returns number of floats written (6 * LF_VERTEX_FLOATS).
 * ────────────────────────────────────────────────────────────────────── */

static int pack_quad(float *out, float cx, float cy,
                     float half_w, float half_h,
                     const float color[4], float rotation)
{
    float cr = cosf(rotation);
    float sr = sinf(rotation);

    /* Quad corners in local space, then rotated */
    float corners_x[4] = { -half_w, half_w, half_w, -half_w };
    float corners_y[4] = { -half_h, -half_h, half_h, half_h };
    float uvs_u[4]     = { 0.0f, 1.0f, 1.0f, 0.0f };
    float uvs_v[4]     = { 0.0f, 0.0f, 1.0f, 1.0f };

    /* Apply rotation */
    float rx[4], ry[4];
    for (int i = 0; i < 4; i++) {
        rx[i] = cx + corners_x[i] * cr - corners_y[i] * sr;
        ry[i] = cy + corners_x[i] * sr + corners_y[i] * cr;
    }

    int idx = 0;
    /* Triangle 1: vertices 0, 1, 2 */
    int tri1[3] = { 0, 1, 2 };
    for (int i = 0; i < 3; i++) {
        int vi = tri1[i];
        idx += write_vertex(out + idx, rx[vi], ry[vi],
                            uvs_u[vi], uvs_v[vi],
                            color[0], color[1], color[2], color[3]);
    }
    /* Triangle 2: vertices 0, 2, 3 */
    int tri2[3] = { 0, 2, 3 };
    for (int i = 0; i < 3; i++) {
        int vi = tri2[i];
        idx += write_vertex(out + idx, rx[vi], ry[vi],
                            uvs_u[vi], uvs_v[vi],
                            color[0], color[1], color[2], color[3]);
    }

    return idx;
}

/* ──────────────────────────────────────────────────────────────────────────
 * GLSL shader source strings
 * ────────────────────────────────────────────────────────────────────── */

static const char LF_VERT_SOURCE[] =
    "#version 300 es\n"
    "precision highp float;\n"
    "\n"
    "layout(location = 0) in vec2 a_position;\n"
    "layout(location = 1) in vec2 a_uv;\n"
    "layout(location = 2) in vec4 a_color;\n"
    "\n"
    "out vec2 v_uv;\n"
    "out vec4 v_color;\n"
    "\n"
    "void main() {\n"
    "    gl_Position = vec4(a_position, 0.0, 1.0);\n"
    "    v_uv = a_uv;\n"
    "    v_color = a_color;\n"
    "}\n";

static const char LF_FRAG_SOURCE[] =
    "#version 300 es\n"
    "precision highp float;\n"
    "\n"
    "#define SC_PI 3.14159265358979\n"
    "#define SC_PHI 1.61803398874989\n"
    "\n"
    "uniform int u_type;\n"
    "uniform int u_ray_count;\n"
    "uniform float u_time;\n"
    "\n"
    "in vec2 v_uv;\n"
    "in vec4 v_color;\n"
    "out vec4 frag_color;\n"
    "\n"
    "void main() {\n"
    "    vec2 uv = v_uv * 2.0 - 1.0;\n"
    "    float dist = length(uv);\n"
    "    float alpha = 0.0;\n"
    "\n"
    "    if (u_type == 0) {\n"
    "        /* Starburst: multi-ray pattern */\n"
    "        float angle = atan(uv.y, uv.x);\n"
    "        float rays = cos(angle * float(u_ray_count)) * 0.5 + 0.5;\n"
    "        float falloff = exp(-dist * dist * 2.0);\n"
    "        alpha = rays * falloff;\n"
    "    } else if (u_type == 1) {\n"
    "        /* Halo: soft gaussian-like glow */\n"
    "        alpha = exp(-dist * dist * 4.0);\n"
    "    } else if (u_type == 2) {\n"
    "        /* Ghost: disc with soft edge */\n"
    "        alpha = smoothstep(1.2, 1.0, dist) * smoothstep(0.0, 0.2, dist);\n"
    "    } else if (u_type == 3) {\n"
    "        /* Streak: elongated horizontal line */\n"
    "        alpha = exp(-abs(uv.y) * 10.0) * exp(-abs(uv.x) * 0.5);\n"
    "    } else if (u_type == 4) {\n"
    "        /* Ring: thin bright ring */\n"
    "        float ring_dist = abs(dist - 0.8);\n"
    "        alpha = exp(-pow(ring_dist * 20.0, 2.0));\n"
    "    }\n"
    "\n"
    "    frag_color = vec4(v_color.rgb, v_color.a * alpha);\n"
    "}\n";

/* ──────────────────────────────────────────────────────────────────────────
 * Public API
 * ────────────────────────────────────────────────────────────────────── */

lf_config_t lf_sun_config(void)
{
    lf_config_t cfg;
    float base_size = 0.08f;

    cfg.element_count = 6;
    cfg.global_intensity = 0.9f;
    cfg.fade_start = 0.7f;
    cfg.fade_end = 1.0f;

    /* Element 0: Starburst at source (base_size) */
    cfg.elements[0].type = LF_STARBURST;
    cfg.elements[0].offset = 0.0f;
    cfg.elements[0].size = base_size;
    cfg.elements[0].intensity = 0.9f;
    cfg.elements[0].color[0] = LF_SOLAR_GOLD_R;
    cfg.elements[0].color[1] = LF_SOLAR_GOLD_G;
    cfg.elements[0].color[2] = LF_SOLAR_GOLD_B;
    cfg.elements[0].color[3] = 0.9f;
    cfg.elements[0].ray_count = 6;
    cfg.elements[0].rotation = 0.0f;

    /* Element 1: Halo at source (base_size * PHI) */
    cfg.elements[1].type = LF_HALO;
    cfg.elements[1].offset = 0.0f;
    cfg.elements[1].size = (float)(base_size * LF_PHI);
    cfg.elements[1].intensity = 0.6f;
    cfg.elements[1].color[0] = LF_SOLAR_GOLD_R;
    cfg.elements[1].color[1] = LF_SOLAR_GOLD_G;
    cfg.elements[1].color[2] = LF_SOLAR_GOLD_B;
    cfg.elements[1].color[3] = 0.5f;
    cfg.elements[1].ray_count = 0;
    cfg.elements[1].rotation = 0.0f;

    /* Element 2: Ghost (base_size * PHI_INV) — warm orange */
    cfg.elements[2].type = LF_GHOST;
    cfg.elements[2].offset = 0.6f;
    cfg.elements[2].size = (float)(base_size * LF_PHI_INV);
    cfg.elements[2].intensity = 0.4f;
    cfg.elements[2].color[0] = 1.0f;
    cfg.elements[2].color[1] = 0.6f;
    cfg.elements[2].color[2] = 0.3f;
    cfg.elements[2].color[3] = 0.35f;
    cfg.elements[2].ray_count = 0;
    cfg.elements[2].rotation = 0.0f;

    /* Element 3: Ghost (base_size * PHI_INV2) — warm amber */
    cfg.elements[3].type = LF_GHOST;
    cfg.elements[3].offset = 1.2f;
    cfg.elements[3].size = (float)(base_size * LF_PHI_INV2);
    cfg.elements[3].intensity = 0.3f;
    cfg.elements[3].color[0] = 1.0f;
    cfg.elements[3].color[1] = 0.7f;
    cfg.elements[3].color[2] = 0.2f;
    cfg.elements[3].color[3] = 0.25f;
    cfg.elements[3].ray_count = 0;
    cfg.elements[3].rotation = 0.0f;

    /* Element 4: Streak through source */
    cfg.elements[4].type = LF_STREAK;
    cfg.elements[4].offset = 0.0f;
    cfg.elements[4].size = (float)(base_size * LF_PHI);
    cfg.elements[4].intensity = 0.5f;
    cfg.elements[4].color[0] = LF_SOLAR_GOLD_R;
    cfg.elements[4].color[1] = LF_SOLAR_GOLD_G;
    cfg.elements[4].color[2] = LF_SOLAR_GOLD_B;
    cfg.elements[4].color[3] = 0.4f;
    cfg.elements[4].ray_count = 0;
    cfg.elements[4].rotation = 0.0f;

    /* Element 5: Ring (base_size * PHI_SQ) */
    cfg.elements[5].type = LF_RING;
    cfg.elements[5].offset = 0.0f;
    cfg.elements[5].size = (float)(base_size * LF_PHI_SQ);
    cfg.elements[5].intensity = 0.3f;
    cfg.elements[5].color[0] = LF_SOLAR_GOLD_R;
    cfg.elements[5].color[1] = LF_SOLAR_GOLD_G;
    cfg.elements[5].color[2] = LF_SOLAR_GOLD_B;
    cfg.elements[5].color[3] = 0.2f;
    cfg.elements[5].ray_count = 0;
    cfg.elements[5].rotation = 0.0f;

    return cfg;
}

lf_config_t lf_star_config(void)
{
    lf_config_t cfg;
    float base_size = 0.04f;

    cfg.element_count = 3;
    cfg.global_intensity = 0.5f;
    cfg.fade_start = 0.6f;
    cfg.fade_end = 0.9f;

    /* Element 0: Starburst (subtle, 4-ray) */
    cfg.elements[0].type = LF_STARBURST;
    cfg.elements[0].offset = 0.0f;
    cfg.elements[0].size = base_size;
    cfg.elements[0].intensity = 0.6f;
    cfg.elements[0].color[0] = 0.8f;
    cfg.elements[0].color[1] = 0.9f;
    cfg.elements[0].color[2] = 1.0f;
    cfg.elements[0].color[3] = 0.5f;
    cfg.elements[0].ray_count = 4;
    cfg.elements[0].rotation = 0.0f;

    /* Element 1: Halo (base_size * PHI) */
    cfg.elements[1].type = LF_HALO;
    cfg.elements[1].offset = 0.0f;
    cfg.elements[1].size = (float)(base_size * LF_PHI);
    cfg.elements[1].intensity = 0.4f;
    cfg.elements[1].color[0] = 0.7f;
    cfg.elements[1].color[1] = 0.85f;
    cfg.elements[1].color[2] = 1.0f;
    cfg.elements[1].color[3] = 0.35f;
    cfg.elements[1].ray_count = 0;
    cfg.elements[1].rotation = 0.0f;

    /* Element 2: Ghost (base_size * PHI_INV) — faint blue */
    cfg.elements[2].type = LF_GHOST;
    cfg.elements[2].offset = 0.8f;
    cfg.elements[2].size = (float)(base_size * LF_PHI_INV);
    cfg.elements[2].intensity = 0.2f;
    cfg.elements[2].color[0] = 0.6f;
    cfg.elements[2].color[1] = 0.75f;
    cfg.elements[2].color[2] = 1.0f;
    cfg.elements[2].color[3] = 0.15f;
    cfg.elements[2].ray_count = 0;
    cfg.elements[2].rotation = 0.0f;

    return cfg;
}

int lf_screen_position(const float world_pos[3], const float view_proj[16],
                       float screen_pos_out[2])
{
    if (world_pos == NULL || view_proj == NULL) {
        return 0;
    }

    /* Multiply: clip = view_proj * vec4(world_pos, 1.0) */
    float clip[4];
    for (int i = 0; i < 4; i++) {
        clip[i] = view_proj[i]      * world_pos[0]
                + view_proj[4 + i]  * world_pos[1]
                + view_proj[8 + i]  * world_pos[2]
                + view_proj[12 + i]; /* w component = 1.0 */
    }

    /* Behind camera check */
    if (clip[3] <= 0.0f) {
        return 0;
    }

    /* Perspective divide */
    float ndc_x = clip[0] / clip[3];
    float ndc_y = clip[1] / clip[3];

    /* Bounds check */
    if (fabsf(ndc_x) > 1.0f || fabsf(ndc_y) > 1.0f) {
        return 0;
    }

    if (screen_pos_out != NULL) {
        screen_pos_out[0] = ndc_x;
        screen_pos_out[1] = ndc_y;
    }

    return 1;
}

float lf_flare_axis(const float source_ndc[2], float axis_out[2])
{
    if (source_ndc == NULL) {
        if (axis_out != NULL) {
            axis_out[0] = 0.0f;
            axis_out[1] = 0.0f;
        }
        return 0.0f;
    }

    /* Axis points from source toward center (0,0) */
    float dx = -source_ndc[0];
    float dy = -source_ndc[1];
    float len = sqrtf(dx * dx + dy * dy);

    if (axis_out != NULL) {
        if (len > 1e-8f) {
            axis_out[0] = dx / len;
            axis_out[1] = dy / len;
        } else {
            axis_out[0] = 0.0f;
            axis_out[1] = 0.0f;
        }
    }

    return len;
}

float lf_edge_fade(const float source_ndc[2], float fade_start, float fade_end)
{
    if (source_ndc == NULL) {
        return 0.0f;
    }

    /* Ensure fade_start <= fade_end */
    float fs = fade_start;
    float fe = fade_end;
    if (fs > fe) {
        float tmp = fs;
        fs = fe;
        fe = tmp;
    }

    /* Distance from screen center */
    float dist = sqrtf(source_ndc[0] * source_ndc[0] +
                       source_ndc[1] * source_ndc[1]);

    if (dist <= fs) {
        return 1.0f;
    }
    if (dist >= fe) {
        return 0.0f;
    }

    /* Linear interpolation in fade range */
    return 1.0f - (dist - fs) / (fe - fs);
}

int lf_pack(lf_config_t config, const float source_ndc[2],
            float aspect_ratio, float *out_verts, int max_verts)
{
    if (out_verts == NULL || source_ndc == NULL || max_verts < 6) {
        return 0;
    }

    /* Clamp aspect ratio to reasonable range */
    if (aspect_ratio < 0.1f) {
        aspect_ratio = 0.1f;
    }

    /* Compute flare axis */
    float axis[2];
    float axis_len = lf_flare_axis(source_ndc, axis);

    /* Compute edge fade */
    float edge_mult = lf_edge_fade(source_ndc, config.fade_start,
                                   config.fade_end);

    int total_verts = 0;
    int out_idx = 0;

    for (int e = 0; e < config.element_count && e < LF_MAX_ELEMENTS; e++) {
        /* Check if we can fit another quad (6 verts) */
        if (total_verts + 6 > max_verts) {
            break;
        }

        lf_element_t *elem = &config.elements[e];

        /* Compute element center position along flare axis */
        float elem_x = source_ndc[0] + axis[0] * elem->offset * axis_len;
        float elem_y = source_ndc[1] + axis[1] * elem->offset * axis_len;

        /* Compute element size with aspect correction */
        float half_w = elem->size;
        float half_h = elem->size * aspect_ratio;

        /* Apply global intensity and edge fade to alpha */
        float final_alpha = elem->color[3] * elem->intensity *
                            config.global_intensity * edge_mult;

        float color[4] = {
            elem->color[0],
            elem->color[1],
            elem->color[2],
            final_alpha
        };

        /* Pack quad */
        int floats_written = pack_quad(out_verts + out_idx,
                                       elem_x, elem_y,
                                       half_w, half_h,
                                       color, elem->rotation);
        out_idx += floats_written;
        total_verts += 6;
    }

    return total_verts;
}

const char *lf_vert_source(void)
{
    return LF_VERT_SOURCE;
}

const char *lf_frag_source(void)
{
    return LF_FRAG_SOURCE;
}
