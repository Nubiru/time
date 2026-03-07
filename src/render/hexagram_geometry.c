/* hexagram_geometry.c -- I Ching Hexagram Visual Geometry
 *
 * Generates GPU-ready GL_LINES vertex arrays for I Ching hexagrams.
 * Uses color_theory.h system palette for all colors.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#include "hexagram_geometry.h"
#include "../systems/iching/iching.h"
#include "../render/color_theory.h"
#include "../math/color.h"

#include <stddef.h>

/* Yin line center gap as fraction of line width */
#define HG_YIN_GAP_RATIO (1.0f / 6.0f)

/* Reduced alpha for nuclear hexagram lines */
#define HG_NUCLEAR_ALPHA 0.5f

/* --- Internal helpers --- */

/* Write a vertex into the data buffer. Returns the next write index. */
static int write_vertex(hg_data_t *d, int idx,
                        float x, float y,
                        float r, float g, float b, float a)
{
    int base = idx * HG_VERTEX_FLOATS;
    d->vertices[base + 0] = x;
    d->vertices[base + 1] = y;
    d->vertices[base + 2] = r;
    d->vertices[base + 3] = g;
    d->vertices[base + 4] = b;
    d->vertices[base + 5] = a;
    return idx + 1;
}

/* Pack a single hexagram line (yang or yin) at the given Y position.
 * Returns the number of vertices written.
 * x_offset: left edge of the line bounding region.
 * line_width: total width available for the line. */
static int pack_line(hg_data_t *d, int vert_idx,
                     int is_yang, float x_offset, float line_width, float y,
                     float r, float g, float b, float a)
{
    int written = 0;

    if (is_yang) {
        /* Solid line: one segment, 2 vertices */
        vert_idx = write_vertex(d, vert_idx,
                                x_offset, y, r, g, b, a);
        write_vertex(d, vert_idx,
                     x_offset + line_width, y, r, g, b, a);
        written = 2;
    } else {
        /* Broken line: two segments with center gap */
        float gap = line_width * HG_YIN_GAP_RATIO;
        float half_remaining = (line_width - gap) / 2.0f;

        /* Left segment */
        vert_idx = write_vertex(d, vert_idx,
                                x_offset, y, r, g, b, a);
        vert_idx = write_vertex(d, vert_idx,
                                x_offset + half_remaining, y, r, g, b, a);
        /* Right segment */
        vert_idx = write_vertex(d, vert_idx,
                                x_offset + half_remaining + gap, y, r, g, b, a);
        write_vertex(d, vert_idx,
                     x_offset + line_width, y, r, g, b, a);
        written = 4;
    }

    return written;
}

/* Pack all 6 lines of a hexagram into vertex data.
 * gap_between_trigrams: extra Y spacing between line 2 and line 3 (trigram gap).
 * x_offset, scale_w, scale_h: transform applied to all coordinates.
 * color_r/g/b/a: base line color.
 * changing: optional changing flags (NULL = none).
 * accent_r/g/b/a: color for changing lines.
 * Returns the number of vertices written. */
static int pack_hexagram_lines(hg_data_t *d, int start_vert,
                               const hexagram_t *hex,
                               float x_offset, float width, float height,
                               float trigram_gap,
                               float col_r, float col_g, float col_b, float col_a,
                               float acc_r, float acc_g, float acc_b, float acc_a,
                               const hg_changing_t *changing)
{
    int vert_idx = start_vert;
    int total_verts = 0;
    int total_segs = 0;

    /* Compute Y spacing.
     * 6 lines are evenly spaced within height, minus any trigram gap.
     * Usable height = height - trigram_gap.
     * Spacing between lines = usable_height / 5 (5 gaps for 6 lines).
     * Lines are centered vertically: first line at y_start, last at y_start + usable_height. */
    float usable_height = height - trigram_gap;
    float spacing = (usable_height > 0.0f && 5 > 0) ? usable_height / 5.0f : 0.0f;

    for (int i = 0; i < 6; i++) {
        /* Y position: bottom to top */
        float y = spacing * (float)i;
        /* Add trigram gap for upper trigram (lines 3-5) */
        if (i >= 3) {
            y += trigram_gap;
        }

        /* Choose color based on changing status */
        float r, g, b, a;
        if (changing != NULL && changing->changing[i]) {
            r = acc_r; g = acc_g; b = acc_b; a = acc_a;
        } else {
            r = col_r; g = col_g; b = col_b; a = col_a;
        }

        int verts = pack_line(d, vert_idx, hex->lines[i],
                              x_offset, width, y, r, g, b, a);
        vert_idx += verts;
        total_verts += verts;

        /* Segment count: yang=1, yin=2 */
        total_segs += (hex->lines[i] == 1) ? 1 : 2;
    }

    d->line_count += total_segs;
    return total_verts;
}

/* --- Public API --- */

hg_data_t hg_pack(int king_wen, float width, float height,
                    const hg_changing_t *changing)
{
    hg_data_t d = {.vertex_count = 0, .line_count = 0,
                   .main_vertex_count = 0, .nuclear_vertex_count = 0};

    if (king_wen < 1 || king_wen > 64)
        return d;

    hexagram_t hex = iching_hexagram(king_wen);

    /* Get colors from color_theory system palette */
    color_rgb_t primary = ct_system_primary(CT_SYSTEM_ICHING);
    color_rgb_t accent = ct_system_accent(CT_SYSTEM_ICHING);

    int verts = pack_hexagram_lines(&d, 0, &hex,
                                     0.0f, width, height,
                                     0.0f, /* no trigram gap */
                                     primary.r, primary.g, primary.b, 1.0f,
                                     accent.r, accent.g, accent.b, 1.0f,
                                     changing);

    d.vertex_count = verts;
    d.main_vertex_count = verts;
    return d;
}

hg_data_t hg_pack_with_nuclear(int king_wen, float width, float height,
                                const hg_changing_t *changing,
                                float nuclear_offset_x, float nuclear_scale)
{
    hg_data_t d = {.vertex_count = 0, .line_count = 0,
                   .main_vertex_count = 0, .nuclear_vertex_count = 0};

    if (king_wen < 1 || king_wen > 64)
        return d;

    hexagram_t hex = iching_hexagram(king_wen);

    /* Get colors from color_theory system palette */
    color_rgb_t primary = ct_system_primary(CT_SYSTEM_ICHING);
    color_rgb_t accent = ct_system_accent(CT_SYSTEM_ICHING);
    color_rgb_t secondary = ct_system_secondary(CT_SYSTEM_ICHING);

    /* Pack main hexagram */
    int main_verts = pack_hexagram_lines(&d, 0, &hex,
                                          0.0f, width, height,
                                          0.0f,
                                          primary.r, primary.g, primary.b, 1.0f,
                                          accent.r, accent.g, accent.b, 1.0f,
                                          changing);
    d.main_vertex_count = main_verts;

    /* Derive nuclear hexagram (lower): trigrams from lines[1,2,3] and lines[2,3,4] */
    int nuc_lower_kw = hg_nuclear_lower(king_wen);
    if (nuc_lower_kw > 0) {
        hexagram_t nuc_hex = iching_hexagram(nuc_lower_kw);
        float nuc_w = width * nuclear_scale;
        float nuc_h = height * nuclear_scale;

        int nuc_verts = pack_hexagram_lines(&d, main_verts, &nuc_hex,
                                             nuclear_offset_x, nuc_w, nuc_h,
                                             0.0f,
                                             secondary.r, secondary.g, secondary.b,
                                             HG_NUCLEAR_ALPHA,
                                             accent.r, accent.g, accent.b,
                                             HG_NUCLEAR_ALPHA,
                                             NULL);
        d.nuclear_vertex_count = nuc_verts;
    }

    d.vertex_count = d.main_vertex_count + d.nuclear_vertex_count;
    return d;
}

hg_data_t hg_pack_trigrams(int king_wen, float width, float height,
                            float gap_ratio)
{
    hg_data_t d = {.vertex_count = 0, .line_count = 0,
                   .main_vertex_count = 0, .nuclear_vertex_count = 0};

    if (king_wen < 1 || king_wen > 64)
        return d;

    hexagram_t hex = iching_hexagram(king_wen);

    /* Get colors from color_theory system palette */
    color_rgb_t primary = ct_system_primary(CT_SYSTEM_ICHING);
    color_rgb_t accent = ct_system_accent(CT_SYSTEM_ICHING);

    float trigram_gap = height * gap_ratio;

    int verts = pack_hexagram_lines(&d, 0, &hex,
                                     0.0f, width, height,
                                     trigram_gap,
                                     primary.r, primary.g, primary.b, 1.0f,
                                     accent.r, accent.g, accent.b, 1.0f,
                                     NULL);

    d.vertex_count = verts;
    d.main_vertex_count = verts;
    return d;
}

int hg_nuclear_lower(int king_wen)
{
    if (king_wen < 1 || king_wen > 64)
        return 0;

    hexagram_t hex = iching_hexagram(king_wen);

    /* Nuclear lower hexagram:
     * Lower trigram from lines[1], lines[2], lines[3]
     * Upper trigram from lines[2], lines[3], lines[4] */
    int nuc_lines[6];
    nuc_lines[0] = hex.lines[1];
    nuc_lines[1] = hex.lines[2];
    nuc_lines[2] = hex.lines[3];
    nuc_lines[3] = hex.lines[2];
    nuc_lines[4] = hex.lines[3];
    nuc_lines[5] = hex.lines[4];

    int result = iching_lines_to_king_wen(nuc_lines);
    return (result < 0) ? 0 : result;
}

int hg_nuclear_upper(int king_wen)
{
    if (king_wen < 1 || king_wen > 64)
        return 0;

    hexagram_t hex = iching_hexagram(king_wen);

    /* Nuclear upper hexagram:
     * Lower trigram from lines[2], lines[3], lines[4]
     * Upper trigram from lines[3], lines[4], lines[5] */
    int nuc_lines[6];
    nuc_lines[0] = hex.lines[2];
    nuc_lines[1] = hex.lines[3];
    nuc_lines[2] = hex.lines[4];
    nuc_lines[3] = hex.lines[3];
    nuc_lines[4] = hex.lines[4];
    nuc_lines[5] = hex.lines[5];

    int result = iching_lines_to_king_wen(nuc_lines);
    return (result < 0) ? 0 : result;
}

int hg_line_count(int king_wen)
{
    if (king_wen < 1 || king_wen > 64)
        return 0;
    return 6;
}

int hg_vertex_bytes(const hg_data_t *data)
{
    return data->vertex_count * HG_VERTEX_FLOATS * (int)sizeof(float);
}

/* --- GLSL Shader Sources --- */

static const char HG_VERT_SOURCE[] =
    "#version 300 es\n"
    "precision highp float;\n"
    "\n"
    "layout(location = 0) in vec2 a_position;\n"
    "layout(location = 1) in vec4 a_color;\n"
    "\n"
    "uniform mat4 u_mvp;\n"
    "\n"
    "out vec4 v_color;\n"
    "\n"
    "void main() {\n"
    "    v_color = a_color;\n"
    "    gl_Position = u_mvp * vec4(a_position, 0.0, 1.0);\n"
    "}\n";

static const char HG_FRAG_SOURCE[] =
    "#version 300 es\n"
    "precision highp float;\n"
    "\n"
    "in vec4 v_color;\n"
    "out vec4 out_color;\n"
    "\n"
    "void main() {\n"
    "    out_color = v_color;\n"
    "}\n";

const char *hg_line_vert_source(void)
{
    return HG_VERT_SOURCE;
}

const char *hg_line_frag_source(void)
{
    return HG_FRAG_SOURCE;
}
