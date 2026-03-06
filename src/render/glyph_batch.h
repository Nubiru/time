#ifndef TIME_GLYPH_BATCH_H
#define TIME_GLYPH_BATCH_H

/* Interleaved vertex stride: position(3) + uv(2) + color(4) = 9 floats */
#define GLYPH_BATCH_STRIDE 9
#define GLYPH_BATCH_MAX 128

/* A single glyph instance to be rendered as a billboard quad. */
typedef struct {
    float px, py, pz;     /* world position (center of quad) */
    float width, height;  /* world-space size */
    float u0, v0, u1, v1; /* UV rect in texture atlas */
    float r, g, b, a;     /* tint color with alpha */
} glyph_instance_t;

/* Buffer size information for a glyph batch. */
typedef struct {
    int vertex_count;  /* 4 per glyph */
    int index_count;   /* 6 per glyph */
} glyph_batch_info_t;

/* Get buffer sizes needed for count glyphs (clamped to GLYPH_BATCH_MAX). */
glyph_batch_info_t glyph_batch_size(int count);

/* Generate interleaved vertex data for a batch of billboard glyphs.
 * cam_right/cam_up: camera basis vectors (unit length).
 * out_vertices: count * 4 * GLYPH_BATCH_STRIDE floats.
 * out_indices: count * 6 unsigned ints.
 * count is clamped to GLYPH_BATCH_MAX. */
void glyph_batch_generate(const glyph_instance_t *instances, int count,
                           float cam_right_x, float cam_right_y, float cam_right_z,
                           float cam_up_x, float cam_up_y, float cam_up_z,
                           float *out_vertices, unsigned int *out_indices);

/* Attribute layout queries (float offsets within stride). */
int glyph_batch_stride(void);
int glyph_batch_position_offset(void);
int glyph_batch_uv_offset(void);
int glyph_batch_color_offset(void);

#endif
