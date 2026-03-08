/*
 * mesh_shader.h — Shared lit 3D mesh shader source strings
 *
 * Used by planet_pass (Sun sphere) and zodiac_pass (ring markers).
 * Each pass compiles its own copy of the program.
 *
 * Pure module: returns pointers to static const strings.
 */

#ifndef TIME_MESH_SHADER_H
#define TIME_MESH_SHADER_H

/* Lit mesh vertex shader (GLSL ES 3.00).
 * Attributes: a_position (vec3, loc 0), a_normal (vec3, loc 1).
 * Uniforms: u_model (mat4), u_view (mat4), u_proj (mat4). */
const char *mesh_shader_vert_source(void);

/* Lit mesh fragment shader (GLSL ES 3.00).
 * Uniforms: u_color (vec3), u_light_dir (vec3),
 *           u_emissive (float), u_opacity (float). */
const char *mesh_shader_frag_source(void);

#endif /* TIME_MESH_SHADER_H */
