#ifndef TIME_BILLBOARD_H
#define TIME_BILLBOARD_H

#include "../math/mat4.h"
#include "../math/vec3.h"

/* A single camera-facing quad: 4 vertices, 2 triangles. */
typedef struct {
    float positions[12]; /* 4 vertices x 3 (x,y,z) */
    float uvs[8];        /* 4 vertices x 2 (u,v) */
    unsigned int indices[6]; /* 2 triangles */
} billboard_quad_t;

/* Generate a camera-facing quad at a world position.
 * cam_right, cam_up: camera basis vectors.
 * width, height: billboard size in world units. */
billboard_quad_t billboard_create(vec3_t center,
                                  vec3_t cam_right, vec3_t cam_up,
                                  float width, float height);

/* Y-axis-locked billboard (stays upright, rotates around Y only).
 * cam_pos: camera world position for computing facing direction. */
billboard_quad_t billboard_create_y_locked(vec3_t center, vec3_t cam_pos,
                                           float width, float height);

/* Extract camera right and up vectors from a view matrix. */
void billboard_camera_vectors(const mat4_t *view,
                               vec3_t *out_right, vec3_t *out_up);

/* Compute world-space size to maintain constant pixel size.
 * fov_radians: vertical field of view.
 * distance: distance from camera to billboard.
 * screen_height: viewport height in pixels.
 * desired_px: desired height in pixels. */
float billboard_size_for_pixels(float fov_radians, float distance,
                                 float screen_height, float desired_px);

/* Batch generate N billboards into caller-provided buffers.
 * out_positions: N*12 floats, out_uvs: N*8 floats, out_indices: N*6 uints. */
void billboard_batch(const vec3_t *centers, int count,
                     vec3_t cam_right, vec3_t cam_up,
                     float width, float height,
                     float *out_positions, float *out_uvs,
                     unsigned int *out_indices);

#endif
