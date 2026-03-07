/* card_pack.h -- Card Quad Pack: GPU-ready vertex arrays for card rendering
 *
 * Pure vertex packing module that converts card_layout screen-space
 * positions into GPU-ready quad vertex arrays for batched card rendering.
 * Generates background quads, border lines, and section dividers.
 *
 * Three draw call types:
 *   1. Quads (card backgrounds, indexed triangles)
 *   2. Lines (card borders + section dividers, GL_LINES)
 *
 * Includes GLSL ES 3.00 shader source strings for each draw call.
 *
 * Pure module: no GL calls, no malloc, no globals, no side effects. */

#ifndef TIME_CARD_PACK_H
#define TIME_CARD_PACK_H

#include "../ui/card_layout.h"

/* Quad vertex: position(vec2) + uv(vec2) + color(vec4) = 8 floats */
#define CP_VERTEX_FLOATS 8
/* Border line vertex: position(vec2) + color(vec4) = 6 floats */
#define CP_LINE_VERTEX_FLOATS 6

#define CP_MAX_CARDS 5
#define CP_VERTS_PER_QUAD 4
#define CP_INDICES_PER_QUAD 6
/* Section dividers: title bar + up to 3 content sections per card */
#define CP_MAX_DIVIDERS_PER_CARD 4
#define CP_MAX_DIVIDERS (CP_MAX_CARDS * CP_MAX_DIVIDERS_PER_CARD)

/* Packed card quad vertex data (background rectangles) */
typedef struct {
    float vertices[CP_MAX_CARDS * CP_VERTS_PER_QUAD * CP_VERTEX_FLOATS];
    unsigned int indices[CP_MAX_CARDS * CP_INDICES_PER_QUAD];
    int vertex_count;
    int index_count;
    int card_count;       /* number of visible cards packed */
    int card_offsets[CP_MAX_CARDS];  /* index offset per card */
    int card_counts[CP_MAX_CARDS];   /* index count per card */
} cp_quad_data_t;

/* Packed card border line data (GL_LINES) */
typedef struct {
    float vertices[(CP_MAX_CARDS * 8 + CP_MAX_DIVIDERS * 2) * CP_LINE_VERTEX_FLOATS];
    int vertex_count;
    int line_count;       /* number of GL_LINES segments */
    int border_line_count;  /* border segments (4 per card) */
    int divider_line_count; /* section divider segments */
} cp_line_data_t;

/* Pack visible cards as background quads.
 * layout: computed card layout.
 * viewport_w, viewport_h: screen dimensions in pixels.
 * bg_r/g/b/a: card background color (use theme.bg_surface).
 * Returns quad vertex data ready for indexed draw. */
cp_quad_data_t cp_pack_quads(const card_layout_t *layout,
                              float viewport_w, float viewport_h,
                              float bg_r, float bg_g, float bg_b, float bg_a);

/* Pack card borders and section dividers as GL_LINES.
 * border_r/g/b/a: border line color (use theme.border).
 * divider_sections: number of section dividers per card (0-4).
 * Returns line vertex data. */
cp_line_data_t cp_pack_lines(const card_layout_t *layout,
                              float viewport_w, float viewport_h,
                              float border_r, float border_g, float border_b, float border_a,
                              int divider_sections);

/* Get number of visible cards in a layout. */
int cp_visible_count(const card_layout_t *layout);

/* Get vertex byte size for quad data. */
int cp_quad_vertex_bytes(const cp_quad_data_t *data);

/* Get index byte size for quad data. */
int cp_quad_index_bytes(const cp_quad_data_t *data);

/* Get vertex byte size for line data. */
int cp_line_vertex_bytes(const cp_line_data_t *data);

/* Card quad vertex shader (GLSL ES 3.00) — screen-space quads with rounded corners. */
const char *cp_quad_vert_source(void);
/* Card quad fragment shader (GLSL ES 3.00) — rounded corners + transparency. */
const char *cp_quad_frag_source(void);
/* Card line vertex shader (GLSL ES 3.00). */
const char *cp_line_vert_source(void);
/* Card line fragment shader (GLSL ES 3.00). */
const char *cp_line_frag_source(void);

#endif /* TIME_CARD_PACK_H */
