/*
 * noise_shader.h — GLSL ES 3.00 noise function library
 *
 * Provides simplex noise, fbm, and related functions as GLSL source strings.
 * These are #included (concatenated) into other shaders that need procedural
 * effects: sun plasma, planet surfaces, milky way, atmosphere.
 *
 * Pure module: returns pointers to static const strings.
 * No GL calls, no malloc, no globals.
 *
 * Based on Ashima Arts / Ian McEwan simplex noise (MIT license).
 */

#ifndef TIME_NOISE_SHADER_H
#define TIME_NOISE_SHADER_H

/* GLSL ES 3.00 simplex noise functions.
 * Provides: snoise2(vec2), snoise3(vec3), fbm2(vec2), fbm3(vec3).
 * Prepend this to any fragment shader that needs noise.
 * ~80 lines of GLSL. */
const char *noise_shader_source(void);

#endif /* TIME_NOISE_SHADER_H */
