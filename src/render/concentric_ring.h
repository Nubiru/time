/* concentric_ring.h — Multi-ring layout with arbitrary segment counts.
 *
 * Generates GPU-ready vertex data for multiple concentric annular rings,
 * each with a configurable number of segments. Rings are spaced using
 * golden ratio proportions. Each segment can have its own color, and
 * one segment per ring can be highlighted as "today."
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 *
 * Coordinate system: XZ plane (y=0), 0 degrees = +X axis, CCW.
 * Vertex layout: position(3) + color(4) = 7 floats per vertex. */

#ifndef TIME_CONCENTRIC_RING_H
#define TIME_CONCENTRIC_RING_H

#define CR_VERT_FLOATS 7
#define CR_MAX_RINGS 8
#define CR_MAX_SEGMENTS 360

/* Single ring definition */
typedef struct {
    int segment_count;            /* cells in this ring (12, 20, 60, 64, 260, etc.) */
    int highlight_index;          /* "today" cell index (-1 = none) */
    float highlight_boost;        /* alpha multiplier for highlighted cell */
    const float *segment_colors;  /* per-segment RGB: segment_count * 3 floats, or NULL */
    float uniform_color[3];       /* fallback RGB when segment_colors is NULL */
    float base_alpha;             /* base alpha for non-highlighted segments (0-1) */
} cr_ring_def_t;

/* Multi-ring layout */
typedef struct {
    float innermost_radius;       /* inner edge of first ring */
    float ring_gap;               /* gap between rings (scene units) */
    int   ring_count;             /* number of rings (1 to CR_MAX_RINGS) */
    cr_ring_def_t rings[CR_MAX_RINGS];
} cr_layout_t;

/* Generation result */
typedef struct {
    int total_vertices;
    int total_indices;
    int ring_vert_offset[CR_MAX_RINGS];  /* first vertex index for each ring */
    int ring_vert_count[CR_MAX_RINGS];   /* vertex count per ring */
    int ring_idx_offset[CR_MAX_RINGS];   /* first index offset for each ring */
    int ring_idx_count[CR_MAX_RINGS];    /* index count per ring */
} cr_info_t;

/* Create a default layout: 3 demo rings (12, 64, 260 segments). */
cr_layout_t cr_default_layout(void);

/* Compute inner radius of a ring by index.
 * All rings have the same width = innermost_radius * 0.15.
 * Rings are separated by ring_gap. */
float cr_ring_inner(const cr_layout_t *layout, int ring_index);

/* Compute outer radius of a ring by index. */
float cr_ring_outer(const cr_layout_t *layout, int ring_index);

/* Ring width (outer - inner). */
float cr_ring_width(const cr_layout_t *layout, int ring_index);

/* Generate all rings into vertex/index buffers.
 * out_verts: must hold at least cr_total_verts(layout) * CR_VERT_FLOATS floats.
 * out_indices: must hold at least cr_total_indices(layout) unsigned ints.
 * Returns info with offsets per ring. */
cr_info_t cr_generate(const cr_layout_t *layout,
                       float *out_verts,
                       unsigned int *out_indices);

/* Compute total vertex count needed for a layout. */
int cr_total_verts(const cr_layout_t *layout);

/* Compute total index count needed for a layout. */
int cr_total_indices(const cr_layout_t *layout);

/* Given an angle (degrees, 0-360), which segment index is it in for a ring
 * with the given segment count? */
int cr_segment_at_angle(int segment_count, float angle_deg);

/* Vertex shader source (GLSL ES 3.00). */
const char *cr_vert_source(void);

/* Fragment shader source (GLSL ES 3.00). */
const char *cr_frag_source(void);

#endif /* TIME_CONCENTRIC_RING_H */
