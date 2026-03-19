/*
 * zodiac_pack.c — Zodiac Render Pack: GPU-ready batched vertex arrays
 *
 * Packs zodiac wheel elements into interleaved vertex arrays:
 *   1. Ring: 12 colored segments from ring_geometry.h
 *   2. Lines: degree ticks + house cusps + aspect lines
 *   3. Glyphs: sign + planet billboard quads
 *
 * Uses ring_geometry, cusp_lines, aspect_lines for geometry data,
 * then interleaves position + color + UV into flat float arrays.
 *
 * Pure module: no GL calls, no malloc, no globals, no side effects.
 */

#include "zodiac_pack.h"
#include "ring_geometry.h"
#include "cusp_lines.h"
#include "aspect_lines.h"
#include "billboard.h"
#include "color_palette.h"
#include "../math/vec3.h"

#include <math.h>
#include <stddef.h>

#define PI 3.14159265358979323846

/* ──────────────────────────────────────────────────────────────────────
 * Default sign colors by element (Fire=red, Earth=green, Air=yellow, Water=blue)
 * Elements cycle: Fire(0,3,6,9), Earth(1,4,7,10), Air(2,5,8,11), Water(3...) wait...
 * Traditional: Ari=Fire, Tau=Earth, Gem=Air, Can=Water, Leo=Fire, Vir=Earth,
 *              Lib=Air, Sco=Water, Sag=Fire, Cap=Earth, Aqu=Air, Pis=Water
 * ────────────────────────────────────────────────────────────────── */

static void default_sign_color(int sign, float *r, float *g, float *b, float *a)
{
    color_rgb_t c = color_zodiac_sign(sign);
    *r = c.r;
    *g = c.g;
    *b = c.b;
    *a = 1.0f;
}

/* ──────────────────────────────────────────────────────────────────────
 * zp_pack_ring — 12 colored ring segments
 * ────────────────────────────────────────────────────────────────── */

zp_ring_data_t zp_pack_ring(float inner_radius, float outer_radius,
                             int segments_per_sign,
                             const float *sign_colors)
{
    zp_ring_data_t out;

    if (segments_per_sign < 1) segments_per_sign = 1;

    /* Get mesh dimensions from ring_geometry */
    ring_mesh_info_t info = ring_mesh_size(segments_per_sign);

    /* Clamp to max buffer size */
    int vert_count = info.vertex_count;
    if (vert_count > ZP_MAX_RING_VERTS) vert_count = ZP_MAX_RING_VERTS;
    int idx_count = info.index_count;
    if (idx_count > ZP_MAX_RING_INDICES) idx_count = ZP_MAX_RING_INDICES;

    /* Generate raw positions, UVs, and indices from ring_geometry */
    float positions[ZP_MAX_RING_VERTS * 3];
    float uvs[ZP_MAX_RING_VERTS * 2];
    unsigned int indices[ZP_MAX_RING_INDICES];

    ring_generate_positions(positions, inner_radius, outer_radius,
                            segments_per_sign);
    ring_generate_uvs(uvs, segments_per_sign);
    ring_generate_indices(indices, segments_per_sign);

    /* Interleave: position(3) + color(4) + uv(2) = 9 floats per vertex */
    out.vertex_count = vert_count;
    out.index_count = idx_count;

    /* Track sign offsets and counts */
    for (int s = 0; s < 12; s++) {
        out.sign_offsets[s] = 0;
        out.sign_counts[s] = 0;
    }

    int current_sign = -1;
    for (int i = 0; i < vert_count; i++) {
        int dst = i * ZP_RING_VERTEX_FLOATS;
        int p_idx = i * 3;
        int u_idx = i * 2;

        /* Position */
        out.vertices[dst + 0] = positions[p_idx + 0];
        out.vertices[dst + 1] = positions[p_idx + 1];
        out.vertices[dst + 2] = positions[p_idx + 2];

        /* Determine which sign this vertex belongs to */
        int sign = ring_vertex_sign(i, segments_per_sign);
        if (sign < 0) sign = 0;
        if (sign > 11) sign = 11;

        /* Color from custom palette or defaults */
        float cr, cg, cb, ca;
        if (sign_colors != NULL) {
            cr = sign_colors[sign * 4 + 0];
            cg = sign_colors[sign * 4 + 1];
            cb = sign_colors[sign * 4 + 2];
            ca = sign_colors[sign * 4 + 3];
        } else {
            default_sign_color(sign, &cr, &cg, &cb, &ca);
        }
        out.vertices[dst + 3] = cr;
        out.vertices[dst + 4] = cg;
        out.vertices[dst + 5] = cb;
        out.vertices[dst + 6] = ca;

        /* UV */
        out.vertices[dst + 7] = uvs[u_idx + 0];
        out.vertices[dst + 8] = uvs[u_idx + 1];

        /* Track sign boundaries */
        if (sign != current_sign) {
            current_sign = sign;
            out.sign_offsets[sign] = i;
        }
        out.sign_counts[sign]++;
    }

    /* Copy indices */
    for (int i = 0; i < idx_count; i++) {
        out.indices[i] = indices[i];
    }

    return out;
}

/* ──────────────────────────────────────────────────────────────────────
 * Internal: write a line vertex (position + RGBA) into the vertex buffer
 * ────────────────────────────────────────────────────────────────── */

static void write_line_vert(float *buf, int vertex_idx,
                             float x, float y, float z,
                             float r, float g, float b, float a)
{
    int base = vertex_idx * ZP_LINE_VERTEX_FLOATS;
    buf[base + 0] = x;
    buf[base + 1] = y;
    buf[base + 2] = z;
    buf[base + 3] = r;
    buf[base + 4] = g;
    buf[base + 5] = b;
    buf[base + 6] = a;
}

/* ──────────────────────────────────────────────────────────────────────
 * zp_pack_lines — Tick marks + House cusps + Aspect lines
 * ────────────────────────────────────────────────────────────────── */

zp_line_data_t zp_pack_lines(float inner_radius, float outer_radius,
                              const double *cusps,
                              const float *planet_positions,
                              const double *planet_longitudes,
                              int num_planets, double aspect_orb)
{
    zp_line_data_t out;
    int vi = 0; /* vertex index cursor */

    /* Tick mark color: subtle white */
    float tick_r = 0.5f;
    float tick_g = 0.5f;
    float tick_b = 0.5f;
    float tick_a = 0.3f;

    /* Tick inner/outer: small radial extent between outer ring and slightly beyond */
    float tick_inner = outer_radius;
    float tick_outer = outer_radius + (outer_radius - inner_radius) * 0.1f;

    /* --- Section 1: Tick marks (360 degree marks) --- */
    out.tick_offset = 0;
    out.tick_count = ZP_MAX_TICK_LINES;

    for (int d = 0; d < 360; d++) {
        float angle_rad = (float)d * (float)PI / 180.0f;
        float c = cosf(angle_rad);
        float s = sinf(angle_rad);

        /* Every 5 degrees: slightly longer tick */
        float this_outer = tick_outer;
        float this_alpha = tick_a;
        if (d % 5 == 0) {
            this_outer = outer_radius + (outer_radius - inner_radius) * 0.15f;
            this_alpha = 0.5f;
        }

        /* Inner point */
        write_line_vert(out.vertices, vi,
                        tick_inner * c, 0.0f, tick_inner * s,
                        tick_r, tick_g, tick_b, this_alpha);
        vi++;
        /* Outer point */
        write_line_vert(out.vertices, vi,
                        this_outer * c, 0.0f, this_outer * s,
                        tick_r, tick_g, tick_b, this_alpha);
        vi++;
    }

    /* --- Section 2: House cusps --- */
    out.cusp_offset = vi;
    out.cusp_count = 0;

    if (cusps != NULL) {
        cusp_line_set_t cset = cusp_lines_generate(cusps, inner_radius, outer_radius);
        out.cusp_count = cset.count;

        for (int i = 0; i < cset.count; i++) {
            cusp_line_t cl = cset.lines[i];
            float alpha = cl.is_angular ? 1.0f : 0.5f;
            float brightness = cl.is_angular ? 1.0f : 0.7f;

            write_line_vert(out.vertices, vi,
                            cl.x1, cl.y1, cl.z1,
                            brightness, brightness, brightness, alpha);
            vi++;
            write_line_vert(out.vertices, vi,
                            cl.x2, cl.y2, cl.z2,
                            brightness, brightness, brightness, alpha);
            vi++;
        }
    }

    /* --- Section 3: Aspect lines --- */
    out.aspect_offset = vi;
    out.aspect_count = 0;

    if (num_planets > 1 && planet_positions != NULL && planet_longitudes != NULL) {
        aspect_line_set_t aset = aspect_lines_generate(
            planet_positions, planet_longitudes, num_planets, aspect_orb);
        out.aspect_count = aset.count;

        for (int i = 0; i < aset.count; i++) {
            aspect_line_t al = aset.lines[i];
            write_line_vert(out.vertices, vi,
                            al.x1, al.y1, al.z1,
                            al.r, al.g, al.b, al.opacity);
            vi++;
            write_line_vert(out.vertices, vi,
                            al.x2, al.y2, al.z2,
                            al.r, al.g, al.b, al.opacity);
            vi++;
        }
    }

    out.total_vertex_count = vi;
    return out;
}

/* ──────────────────────────────────────────────────────────────────────
 * Internal: write a glyph vertex (position + uv + RGBA) into the buffer
 * ────────────────────────────────────────────────────────────────── */

static void write_glyph_vert(float *buf, int vertex_idx,
                               float x, float y, float z,
                               float u, float v,
                               float r, float g, float b, float a)
{
    int base = vertex_idx * ZP_GLYPH_VERTEX_FLOATS;
    buf[base + 0] = x;
    buf[base + 1] = y;
    buf[base + 2] = z;
    buf[base + 3] = u;
    buf[base + 4] = v;
    buf[base + 5] = r;
    buf[base + 6] = g;
    buf[base + 7] = b;
    buf[base + 8] = a;
}

/* ──────────────────────────────────────────────────────────────────────
 * Internal: pack a single billboard quad at a position with given color
 * ────────────────────────────────────────────────────────────────── */

static void pack_quad(float *vert_buf, unsigned int *idx_buf,
                       int *vert_cursor, int *idx_cursor,
                       vec3_t center, vec3_t cam_right, vec3_t cam_up,
                       float scale,
                       float u0, float v0, float u1, float v1,
                       float r, float g, float b, float a)
{
    billboard_quad_t bb = billboard_create(center, cam_right, cam_up,
                                           scale, scale);

    int v_base = *vert_cursor;

    /* 4 corner vertices: BL, BR, TR, TL */
    /* Billboard returns positions[12] and uvs[8] */
    /* Override billboard UVs with our atlas UVs */
    float quad_uvs[8] = {u0, v0, u1, v0, u1, v1, u0, v1};

    for (int i = 0; i < 4; i++) {
        write_glyph_vert(vert_buf, v_base + i,
                          bb.positions[i * 3 + 0],
                          bb.positions[i * 3 + 1],
                          bb.positions[i * 3 + 2],
                          quad_uvs[i * 2 + 0],
                          quad_uvs[i * 2 + 1],
                          r, g, b, a);
    }

    /* 6 indices for 2 triangles */
    int i_base = *idx_cursor;
    idx_buf[i_base + 0] = (unsigned int)(v_base + 0);
    idx_buf[i_base + 1] = (unsigned int)(v_base + 1);
    idx_buf[i_base + 2] = (unsigned int)(v_base + 2);
    idx_buf[i_base + 3] = (unsigned int)(v_base + 0);
    idx_buf[i_base + 4] = (unsigned int)(v_base + 2);
    idx_buf[i_base + 5] = (unsigned int)(v_base + 3);

    *vert_cursor = v_base + 4;
    *idx_cursor = i_base + 6;
}

/* ──────────────────────────────────────────────────────────────────────
 * zp_pack_glyphs — Sign + planet glyph quads
 * ────────────────────────────────────────────────────────────────── */

zp_glyph_data_t zp_pack_glyphs(float ring_radius,
                                 const float cam_right[3],
                                 const float cam_up[3],
                                 const double *planet_longitudes,
                                 int num_planets,
                                 float glyph_scale)
{
    zp_glyph_data_t out;
    int vert_cursor = 0;
    int idx_cursor = 0;

    vec3_t cr = vec3_create(cam_right[0], cam_right[1], cam_right[2]);
    vec3_t cu = vec3_create(cam_up[0], cam_up[1], cam_up[2]);

    if (num_planets < 0) num_planets = 0;
    if (num_planets > 12) num_planets = 12;

    /* --- 12 sign glyphs at midpoint of each 30-degree segment --- */
    out.sign_glyph_count = 12;

    /* Atlas layout: 4 columns x 3 rows for 12 zodiac signs */
    for (int s = 0; s < 12; s++) {
        /* Midpoint angle of this sign: s * 30 + 15 degrees */
        float angle_deg = (float)s * 30.0f + 15.0f;
        float angle_rad = angle_deg * (float)PI / 180.0f;

        /* Position on the ring */
        float px = ring_radius * cosf(angle_rad);
        float pz = ring_radius * sinf(angle_rad);
        vec3_t center = vec3_create(px, 0.0f, pz);

        /* Sign color from palette */
        color_rgb_t sc = color_zodiac_sign(s);

        /* Atlas UV: 4x3 grid */
        int col = s % 4;
        int row = s / 4;
        float u0 = (float)col * 0.25f;
        float v0 = (float)row / 3.0f;
        float u1 = (float)(col + 1) * 0.25f;
        float v1 = (float)(row + 1) / 3.0f;

        pack_quad(out.vertices, out.indices,
                  &vert_cursor, &idx_cursor,
                  center, cr, cu, glyph_scale,
                  u0, v0, u1, v1,
                  sc.r, sc.g, sc.b, 1.0f);
    }

    /* --- Planet glyphs at their ecliptic longitudes --- */
    out.planet_glyph_count = 0;

    if (planet_longitudes != NULL && num_planets > 0) {
        out.planet_glyph_count = num_planets;

        /* Planet radius: slightly inside the sign glyph ring */
        float planet_radius = ring_radius * 0.8f;

        for (int p = 0; p < num_planets; p++) {
            float angle_rad = (float)(planet_longitudes[p]) * (float)PI / 180.0f;
            float px = planet_radius * cosf(angle_rad);
            float pz = planet_radius * sinf(angle_rad);
            vec3_t center = vec3_create(px, 0.0f, pz);

            /* Planet color from palette */
            color_rgb_t pc = color_planet(p);

            /* Planet atlas UV: assume second row of 4x3 atlas or similar.
             * Use a simple mapping: planet p gets a unique tile. */
            int col = p % 4;
            int row = (p / 4) + 3; /* offset past sign rows */
            float u0 = (float)col * 0.25f;
            float v0 = (float)row / 6.0f;
            float u1 = (float)(col + 1) * 0.25f;
            float v1 = (float)(row + 1) / 6.0f;

            pack_quad(out.vertices, out.indices,
                      &vert_cursor, &idx_cursor,
                      center, cr, cu, glyph_scale,
                      u0, v0, u1, v1,
                      pc.r, pc.g, pc.b, 1.0f);
        }
    }

    out.vertex_count = vert_cursor;
    out.index_count = idx_cursor;

    return out;
}

/* ──────────────────────────────────────────────────────────────────────
 * Byte size queries
 * ────────────────────────────────────────────────────────────────── */

int zp_ring_vertex_bytes(const zp_ring_data_t *data)
{
    return data->vertex_count * ZP_RING_VERTEX_FLOATS * (int)sizeof(float);
}

int zp_ring_index_bytes(const zp_ring_data_t *data)
{
    return data->index_count * (int)sizeof(unsigned int);
}

int zp_line_vertex_bytes(const zp_line_data_t *data)
{
    return data->total_vertex_count * ZP_LINE_VERTEX_FLOATS * (int)sizeof(float);
}

int zp_glyph_vertex_bytes(const zp_glyph_data_t *data)
{
    return data->vertex_count * ZP_GLYPH_VERTEX_FLOATS * (int)sizeof(float);
}

int zp_glyph_index_bytes(const zp_glyph_data_t *data)
{
    return data->index_count * (int)sizeof(unsigned int);
}

/* ──────────────────────────────────────────────────────────────────────
 * Shader source strings — GLSL ES 3.00
 * ────────────────────────────────────────────────────────────────── */

const char *zp_ring_vert_source(void)
{
    return
        "#version 300 es\n"
        "precision highp float;\n"
        "uniform mat4 u_mvp;\n"
        "layout(location = 0) in vec3 a_position;\n"
        "layout(location = 1) in vec4 a_color;\n"
        "layout(location = 2) in vec2 a_uv;\n"
        "out vec4 v_color;\n"
        "out vec2 v_uv;\n"
        "void main() {\n"
        "    gl_Position = u_mvp * vec4(a_position, 1.0);\n"
        "    v_color = a_color;\n"
        "    v_uv = a_uv;\n"
        "}\n";
}

const char *zp_ring_frag_source(void)
{
    return
        "#version 300 es\n"
        "precision highp float;\n"
        "in vec4 v_color;\n"
        "in vec2 v_uv;\n"
        "out vec4 frag_color;\n"
        "void main() {\n"
        "    /* Radial glow: brightest at ring midline, fading at edges */\n"
        "    float radial = v_uv.y;\n"
        "    float center = 0.5;\n"
        "    float dist_from_center = abs(radial - center) / center;\n"
        "    float glow = 1.0 - dist_from_center * dist_from_center;\n"
        "    glow = clamp(glow, 0.0, 1.0);\n"
        "    /* Color: enhance brightness at glow center */\n"
        "    vec3 color = v_color.rgb * (0.5 + 0.5 * glow);\n"
        "    /* Alpha: fade at ring edges */\n"
        "    float alpha = v_color.a * (0.3 + 0.7 * glow);\n"
        "    frag_color = vec4(color, alpha);\n"
        "}\n";
}

const char *zp_line_vert_source(void)
{
    return
        "#version 300 es\n"
        "precision highp float;\n"
        "uniform mat4 u_mvp;\n"
        "layout(location = 0) in vec3 a_position;\n"
        "layout(location = 1) in vec4 a_color;\n"
        "out vec4 v_color;\n"
        "void main() {\n"
        "    gl_Position = u_mvp * vec4(a_position, 1.0);\n"
        "    v_color = a_color;\n"
        "}\n";
}

const char *zp_line_frag_source(void)
{
    return
        "#version 300 es\n"
        "precision highp float;\n"
        "in vec4 v_color;\n"
        "out vec4 frag_color;\n"
        "void main() {\n"
        "    frag_color = v_color;\n"
        "}\n";
}

const char *zp_glyph_vert_source(void)
{
    return
        "#version 300 es\n"
        "precision highp float;\n"
        "uniform mat4 u_mvp;\n"
        "layout(location = 0) in vec3 a_position;\n"
        "layout(location = 1) in vec2 a_uv;\n"
        "layout(location = 2) in vec4 a_color;\n"
        "out vec2 v_uv;\n"
        "out vec4 v_color;\n"
        "void main() {\n"
        "    gl_Position = u_mvp * vec4(a_position, 1.0);\n"
        "    v_uv = a_uv;\n"
        "    v_color = a_color;\n"
        "}\n";
}

const char *zp_glyph_frag_source(void)
{
    return
        "#version 300 es\n"
        "precision highp float;\n"
        "uniform sampler2D u_atlas;\n"
        "in vec2 v_uv;\n"
        "in vec4 v_color;\n"
        "out vec4 frag_color;\n"
        "void main() {\n"
        "    vec4 tex = texture(u_atlas, v_uv);\n"
        "    frag_color = tex * v_color;\n"
        "    if (frag_color.a < 0.01) discard;\n"
        "}\n";
}
