#ifndef TIME_GLYPH_BATCH_H
#define TIME_GLYPH_BATCH_H

#include "../math/vec3.h"

#define GLYPH_BATCH_MAX 512

/* Interleaved vertex stride: x,y,z, u,v, r,g,b,a = 9 floats */
#define GLYPH_VERTEX_STRIDE 9

/* RGBA tint color */
typedef struct {
    float r, g, b, a;
} glyph_color_t;

/* Input: one glyph to render */
typedef struct {
    int glyph_id;          /* character code or glyph index */
    vec3_t position;       /* world-space center position */
    float scale;           /* size multiplier (1.0 = default) */
    glyph_color_t color;   /* per-glyph RGBA tint */
} glyph_instance_t;

/* Atlas layout description (for UV computation) */
typedef struct {
    int cols;              /* columns in atlas texture */
    int rows;              /* rows in atlas texture */
    int first_id;          /* first valid glyph ID */
    int last_id;           /* last valid glyph ID */
} glyph_atlas_t;

/* Output: batched vertex data ready for GPU */
typedef struct {
    /* Interleaved: [x,y,z, u,v, r,g,b,a] per vertex, 4 vertices per glyph */
    float vertices[GLYPH_BATCH_MAX * 4 * GLYPH_VERTEX_STRIDE];
    /* Index buffer: 6 indices per glyph (2 triangles) */
    unsigned int indices[GLYPH_BATCH_MAX * 6];
    int vertex_count;      /* total vertices emitted */
    int index_count;       /* total indices emitted */
    int glyph_count;       /* number of glyphs batched */
} glyph_batch_t;

/* Compute UV coordinates for a glyph ID in an atlas.
 * Out-of-range glyph_id returns (0,0,0,0). */
void glyph_batch_uv(glyph_atlas_t atlas, int glyph_id,
                     float *u0, float *v0, float *u1, float *v1);

/* Generate a batch of camera-facing textured quads.
 * cam_right, cam_up: camera basis vectors for billboarding.
 * base_width, base_height: default glyph size in world units.
 * Per-glyph scale is multiplied with base size. */
glyph_batch_t glyph_batch_create(
    const glyph_instance_t *instances, int count,
    glyph_atlas_t atlas,
    vec3_t cam_right, vec3_t cam_up,
    float base_width, float base_height);

/* Generate Y-locked batch (billboards stay upright, rotate around Y only).
 * cam_pos: camera world position for computing facing direction. */
glyph_batch_t glyph_batch_create_y_locked(
    const glyph_instance_t *instances, int count,
    glyph_atlas_t atlas,
    vec3_t cam_pos,
    float base_width, float base_height);

/* Read back one vertex from the interleaved buffer.
 * Fills position, uv, and color from the vertex at given index. */
void glyph_batch_read_vertex(const glyph_batch_t *batch, int vertex_index,
                              float *x, float *y, float *z,
                              float *u, float *v,
                              float *r, float *g, float *b, float *a);

/* Total floats in the vertex buffer. */
int glyph_batch_vertex_floats(const glyph_batch_t *batch);

/* Byte size of vertex buffer (for glBufferData). */
int glyph_batch_vertex_bytes(const glyph_batch_t *batch);

/* Byte size of index buffer (for glBufferData). */
int glyph_batch_index_bytes(const glyph_batch_t *batch);

#endif /* TIME_GLYPH_BATCH_H */
