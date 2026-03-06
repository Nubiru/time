#ifndef TIME_RING_GEOMETRY_H
#define TIME_RING_GEOMETRY_H

typedef struct {
    int vertex_count;
    int index_count;
} ring_mesh_info_t;

/* Compute how many vertices/indices a ring will need. */
ring_mesh_info_t ring_mesh_size(int segments_per_sign);

/* Generate ring vertex positions (x,y,z) in ecliptic plane (y=0).
 * positions: float array of size vertex_count * 3.
 * 0 degrees = positive X axis (Aries cusp). */
void ring_generate_positions(float *positions,
                             float inner_radius, float outer_radius,
                             int segments_per_sign);

/* Generate ring UVs for texture mapping.
 * uvs: float array of size vertex_count * 2.
 * U: 0.0 at 0 degrees, 1.0 at 360. V: 0.0 inner, 1.0 outer. */
void ring_generate_uvs(float *uvs, int segments_per_sign);

/* Generate triangle indices for the ring quads.
 * indices: unsigned int array of size index_count. */
void ring_generate_indices(unsigned int *indices, int segments_per_sign);

/* Get sign index (0-11) for a given vertex index. */
int ring_vertex_sign(int vertex_index, int segments_per_sign);

/* Get angular position (degrees, 0-360) for a given vertex index. */
float ring_vertex_angle(int vertex_index, int segments_per_sign);

#endif
