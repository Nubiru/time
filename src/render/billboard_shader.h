/*
 * billboard_shader.h — GPU-side billboard computation shader source
 *
 * Provides GLSL ES 3.00 shader source strings for camera-facing billboards.
 * The vertex shader extracts camera right/up vectors from the view matrix
 * and computes quad corners on the GPU, replacing CPU-side per-frame
 * billboard vertex rebuilds.
 *
 * Also provides helper functions to pack quad geometry data (offsets,
 * UVs, triangle indices) for billboard quads.
 *
 * Pure module: no GL calls, no malloc, no globals.
 */

#ifndef TIME_BILLBOARD_SHADER_H
#define TIME_BILLBOARD_SHADER_H

/* Billboard vertex shader source (GLSL ES 3.00).
 * Attributes: a_center (vec3, loc 0), a_offset (vec2, loc 1),
 *             a_size (float, loc 2), a_uv (vec2, loc 3).
 * Uniforms: u_view (mat4), u_proj (mat4).
 * Computes billboard quad corners from center position + view matrix. */
const char *billboard_shader_vert_source(void);

/* Simple passthrough fragment shader for textured billboards.
 * Uniforms: u_texture (sampler2D), u_color (vec4 tint). */
const char *billboard_shader_frag_source(void);

/* Pack quad corner offsets for 4 vertices of a unit billboard.
 * out must hold >= 8 floats (4 vertices * 2 floats each).
 * Values: (-1,-1), (1,-1), (1,1), (-1,1).
 * Returns 4 (vertex count), or 0 if out is NULL. */
int billboard_shader_pack_offsets(float *out);

/* Pack UV coordinates for 4 vertices.
 * out must hold >= 8 floats.
 * Values: (0,0), (1,0), (1,1), (0,1).
 * Returns 4 (vertex count), or 0 if out is NULL. */
int billboard_shader_pack_uvs(float *out);

/* Pack triangle indices for a billboard quad.
 * out must hold >= 6 unsigned ints (2 triangles).
 * Returns 6 (index count), or 0 if out is NULL. */
int billboard_shader_pack_indices(unsigned int *out);

#endif /* TIME_BILLBOARD_SHADER_H */
