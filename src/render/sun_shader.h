/*
 * sun_shader.h — Procedural animated Sun shader source strings
 *
 * Renders the Sun as a billboard quad with ray-sphere intersection,
 * layered animated noise for plasma surface, corona glow, and
 * solar prominences. Uses noise_shader for simplex/fbm functions.
 *
 * Pure module: returns pointers to static const strings.
 * No GL calls, no malloc, no globals.
 */

#ifndef TIME_SUN_SHADER_H
#define TIME_SUN_SHADER_H

/* Sun billboard vertex shader (GLSL ES 3.00).
 * Attributes: a_position (vec3, loc 0), a_uv (vec2, loc 1).
 * Uniforms: u_mvp (mat4), u_sun_pos (vec3), u_cam_right (vec3),
 *           u_cam_up (vec3), u_size (float). */
const char *sun_shader_vert_source(void);

/* Sun procedural fragment shader (GLSL ES 3.00).
 * Uniforms: u_time (float — seconds), u_resolution (vec2).
 * Produces animated plasma surface with corona glow. */
const char *sun_shader_frag_source(void);

#endif /* TIME_SUN_SHADER_H */
