#ifndef TIME_RING_DATA_H
#define TIME_RING_DATA_H

/* Interleaved vertex stride: position(3) + color(3) + uv(2) = 8 floats */
#define RING_DATA_STRIDE 8

/* Generate interleaved vertex data for a zodiac ring mesh.
 * Uses same geometry as ring_geometry (XZ plane, y=0, 0°=+X).
 * Buffer sizes: use ring_mesh_size() from ring_geometry.h.
 * out_vertices: float array of vertex_count * RING_DATA_STRIDE.
 * out_indices: unsigned int array of index_count.
 * sign_colors: 36 floats (12 signs × RGB) or NULL for white. */
void ring_data_generate(float *out_vertices,
                         unsigned int *out_indices,
                         float inner_radius, float outer_radius,
                         int segments_per_sign,
                         const float *sign_colors);

/* Attribute layout (byte offsets for glVertexAttribPointer). */
int ring_data_stride(void);
int ring_data_position_offset(void);
int ring_data_color_offset(void);
int ring_data_uv_offset(void);

#endif
