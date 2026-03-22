/* card_pack.c -- Card Quad Pack: GPU-ready vertex arrays for card rendering
 *
 * Converts card_layout screen-space positions into batched vertex data
 * for background quads (indexed triangles) and border/divider lines.
 *
 * Colors are PARAMETERS — the caller provides them from theme_t.
 * This module does NOT import theme.h or color_theory.h.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#include "card_pack.h"

#include <stddef.h>

/* --- Internal helpers --- */

/* Write a quad vertex (position + uv + color). Returns next vertex index. */
static int write_quad_vert(cp_quad_data_t *d, int idx,
                           float x, float y,
                           float u, float v,
                           float r, float g, float b, float a)
{
    int base = idx * CP_VERTEX_FLOATS;
    d->vertices[base + 0] = x;
    d->vertices[base + 1] = y;
    d->vertices[base + 2] = u;
    d->vertices[base + 3] = v;
    d->vertices[base + 4] = r;
    d->vertices[base + 5] = g;
    d->vertices[base + 6] = b;
    d->vertices[base + 7] = a;
    return idx + 1;
}

/* Write a line vertex (position + color). Returns next vertex index. */
static int write_line_vert(cp_line_data_t *d, int idx,
                           float x, float y,
                           float r, float g, float b, float a)
{
    int base = idx * CP_LINE_VERTEX_FLOATS;
    d->vertices[base + 0] = x;
    d->vertices[base + 1] = y;
    d->vertices[base + 2] = r;
    d->vertices[base + 3] = g;
    d->vertices[base + 4] = b;
    d->vertices[base + 5] = a;
    return idx + 1;
}

/* Clamp integer to [lo, hi]. */
static int clamp_int(int val, int lo, int hi)
{
    if (val < lo) return lo;
    if (val > hi) return hi;
    return val;
}

/* --- Public API --- */

cp_quad_data_t cp_pack_quads(const card_layout_t *layout,
                              float viewport_w, float viewport_h,
                              float bg_r, float bg_g, float bg_b, float bg_a)
{
    cp_quad_data_t d;
    d.vertex_count = 0;
    d.index_count = 0;
    d.card_count = 0;

    for (int i = 0; i < CP_MAX_CARDS; i++) {
        d.card_offsets[i] = 0;
        d.card_counts[i] = 0;
    }

    if (!layout) return d;

    int vi = 0;  /* vertex write index */
    int ii = 0;  /* index write index */

    for (int i = 0; i < CARD_TYPE_COUNT && i < CP_MAX_CARDS; i++) {
        const card_rect_t *c = &layout->cards[i];
        if (!c->visible) continue;

        /* Convert normalized [0,1] screen coords to pixel coords.
         * card_layout: x,y = top-left corner (0,0 = top-left of screen).
         * GPU quad: BL, BR, TR, TL with screen-space Y increasing downward. */
        float px = c->x * viewport_w;
        float py = c->y * viewport_h;
        float pw = c->w * viewport_w;
        float ph = c->h * viewport_h;

        /* Bottom-left is (px, py+ph), top-left is (px, py) in screen coords */
        float left   = px;
        float right  = px + pw;
        float top    = py;
        float bottom = py + ph;

        unsigned int base_v = (unsigned int)vi;

        /* BL (u=0, v=0) */
        vi = write_quad_vert(&d, vi, left, bottom, 0.0f, 0.0f,
                             bg_r, bg_g, bg_b, bg_a);
        /* BR (u=1, v=0) */
        vi = write_quad_vert(&d, vi, right, bottom, 1.0f, 0.0f,
                             bg_r, bg_g, bg_b, bg_a);
        /* TR (u=1, v=1) */
        vi = write_quad_vert(&d, vi, right, top, 1.0f, 1.0f,
                             bg_r, bg_g, bg_b, bg_a);
        /* TL (u=0, v=1) */
        vi = write_quad_vert(&d, vi, left, top, 0.0f, 1.0f,
                             bg_r, bg_g, bg_b, bg_a);

        /* Triangle indices: [0,1,2, 2,3,0] (counter-clockwise) */
        d.indices[ii++] = base_v + 0;
        d.indices[ii++] = base_v + 1;
        d.indices[ii++] = base_v + 2;
        d.indices[ii++] = base_v + 2;
        d.indices[ii++] = base_v + 3;
        d.indices[ii++] = base_v + 0;

        /* Per-card tracking */
        int card_idx = d.card_count;
        d.card_offsets[card_idx] = card_idx * CP_INDICES_PER_QUAD;
        d.card_counts[card_idx] = CP_INDICES_PER_QUAD;
        d.card_count++;
    }

    d.vertex_count = vi;
    d.index_count = ii;

    return d;
}

cp_line_data_t cp_pack_lines(const card_layout_t *layout,
                              float viewport_w, float viewport_h,
                              float border_r, float border_g, float border_b, float border_a,
                              int divider_sections)
{
    cp_line_data_t d;
    d.vertex_count = 0;
    d.line_count = 0;
    d.border_line_count = 0;
    d.divider_line_count = 0;

    if (!layout) return d;

    /* Clamp divider_sections to [0, CP_MAX_DIVIDERS_PER_CARD] */
    divider_sections = clamp_int(divider_sections, 0, CP_MAX_DIVIDERS_PER_CARD);

    int vi = 0;  /* vertex write index */

    for (int i = 0; i < CARD_TYPE_COUNT && i < CP_MAX_CARDS; i++) {
        const card_rect_t *c = &layout->cards[i];
        if (!c->visible) continue;

        /* Convert to pixel coords */
        float px = c->x * viewport_w;
        float py = c->y * viewport_h;
        float pw = c->w * viewport_w;
        float ph = c->h * viewport_h;

        float left   = px;
        float right  = px + pw;
        float top    = py;
        float bottom = py + ph;

        /* Top border: TL -> TR */
        vi = write_line_vert(&d, vi, left, top,
                             border_r, border_g, border_b, border_a);
        vi = write_line_vert(&d, vi, right, top,
                             border_r, border_g, border_b, border_a);

        /* Right border: TR -> BR */
        vi = write_line_vert(&d, vi, right, top,
                             border_r, border_g, border_b, border_a);
        vi = write_line_vert(&d, vi, right, bottom,
                             border_r, border_g, border_b, border_a);

        /* Bottom border: BR -> BL */
        vi = write_line_vert(&d, vi, right, bottom,
                             border_r, border_g, border_b, border_a);
        vi = write_line_vert(&d, vi, left, bottom,
                             border_r, border_g, border_b, border_a);

        /* Left border: BL -> TL */
        vi = write_line_vert(&d, vi, left, bottom,
                             border_r, border_g, border_b, border_a);
        vi = write_line_vert(&d, vi, left, top,
                             border_r, border_g, border_b, border_a);

        d.border_line_count += 4;
        d.line_count += 4;

        /* Section dividers: horizontal lines at evenly spaced intervals.
         * divider_sections=1: one line at 25% from top (title bar).
         * divider_sections=N: N lines splitting card into (N+1) sections. */
        for (int s = 0; s < divider_sections; s++) {
            float frac = (float)(s + 1) / (float)(divider_sections + 1);
            float div_y = top + frac * ph;

            vi = write_line_vert(&d, vi, left, div_y,
                                 border_r, border_g, border_b, border_a);
            vi = write_line_vert(&d, vi, right, div_y,
                                 border_r, border_g, border_b, border_a);

            d.divider_line_count++;
            d.line_count++;
        }
    }

    d.vertex_count = vi;

    return d;
}

int cp_visible_count(const card_layout_t *layout)
{
    if (!layout) return 0;

    int count = 0;
    for (int i = 0; i < CARD_TYPE_COUNT; i++) {
        if (layout->cards[i].visible) count++;
    }
    return count;
}

int cp_quad_vertex_bytes(const cp_quad_data_t *data)
{
    if (!data) return 0;
    return data->vertex_count * CP_VERTEX_FLOATS * (int)sizeof(float);
}

int cp_quad_index_bytes(const cp_quad_data_t *data)
{
    if (!data) return 0;
    return data->index_count * (int)sizeof(unsigned int);
}

int cp_line_vertex_bytes(const cp_line_data_t *data)
{
    if (!data) return 0;
    return data->vertex_count * CP_LINE_VERTEX_FLOATS * (int)sizeof(float);
}

/* --- GLSL Shader Sources --- */

/* Quad vertex shader: screen-space quads with UV for rounded corners. */
static const char CP_QUAD_VERT_SOURCE[] =
    "#version 300 es\n"
    "precision highp float;\n"
    "\n"
    "layout(location = 0) in vec2 a_position;\n"
    "layout(location = 1) in vec2 a_uv;\n"
    "layout(location = 2) in vec4 a_color;\n"
    "\n"
    "uniform vec2 u_resolution;\n"
    "uniform float u_slide_offset;\n"
    "\n"
    "out vec2 v_uv;\n"
    "out vec4 v_color;\n"
    "\n"
    "void main() {\n"
    "    v_uv = a_uv;\n"
    "    v_color = a_color;\n"
    "    vec2 ndc = (a_position + vec2(u_slide_offset, 0.0)) / u_resolution * 2.0 - 1.0;\n"
    "    ndc.y = -ndc.y;\n"
    "    gl_Position = vec4(ndc, 0.0, 1.0);\n"
    "}\n";

/* Quad fragment shader: rounded corners via SDF + transparency. */
static const char CP_QUAD_FRAG_SOURCE[] =
    "#version 300 es\n"
    "precision highp float;\n"
    "\n"
    "in vec2 v_uv;\n"
    "in vec4 v_color;\n"
    "\n"
    "uniform float u_corner_radius;\n"
    "\n"
    "out vec4 out_color;\n"
    "\n"
    "void main() {\n"
    "    vec2 p = v_uv * 2.0 - 1.0;\n"
    "    vec2 d = abs(p) - (1.0 - u_corner_radius);\n"
    "    float sdf = length(max(d, 0.0)) - u_corner_radius;\n"
    "    float aa = fwidth(sdf);\n"
    "    float alpha = 1.0 - smoothstep(-aa, aa, sdf);\n"
    "    out_color = vec4(v_color.rgb, v_color.a * alpha);\n"
    "}\n";

/* Line vertex shader: simple position + color passthrough. */
static const char CP_LINE_VERT_SOURCE[] =
    "#version 300 es\n"
    "precision highp float;\n"
    "\n"
    "layout(location = 0) in vec2 a_position;\n"
    "layout(location = 1) in vec4 a_color;\n"
    "\n"
    "uniform vec2 u_resolution;\n"
    "uniform float u_slide_offset;\n"
    "\n"
    "out vec4 v_color;\n"
    "\n"
    "void main() {\n"
    "    v_color = a_color;\n"
    "    vec2 ndc = (a_position + vec2(u_slide_offset, 0.0)) / u_resolution * 2.0 - 1.0;\n"
    "    ndc.y = -ndc.y;\n"
    "    gl_Position = vec4(ndc, 0.0, 1.0);\n"
    "}\n";

/* Line fragment shader: simple color passthrough. */
static const char CP_LINE_FRAG_SOURCE[] =
    "#version 300 es\n"
    "precision highp float;\n"
    "\n"
    "in vec4 v_color;\n"
    "out vec4 out_color;\n"
    "\n"
    "void main() {\n"
    "    out_color = v_color;\n"
    "}\n";

const char *cp_quad_vert_source(void)
{
    return CP_QUAD_VERT_SOURCE;
}

const char *cp_quad_frag_source(void)
{
    return CP_QUAD_FRAG_SOURCE;
}

const char *cp_line_vert_source(void)
{
    return CP_LINE_VERT_SOURCE;
}

const char *cp_line_frag_source(void)
{
    return CP_LINE_FRAG_SOURCE;
}
