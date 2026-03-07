/*
 * diffraction.h — Diffraction spike vertex packing for bright stars
 *
 * Generates procedural diffraction spike vertex data for the brightest stars
 * (magnitude < threshold). Produces 4 or 6 radial spike lines per star with
 * length proportional to brightness and alpha fade along spike length.
 * Output is GPU-ready float arrays for GL_LINES rendering.
 *
 * Vertex layout: position(vec3) + color(vec4) = 7 floats per vertex.
 * Same format as constellation line vertices for shader compatibility.
 *
 * Pure module: no GL calls, no malloc, no globals, no side effects.
 * Caller provides output buffers.
 */

#ifndef TIME_DIFFRACTION_H
#define TIME_DIFFRACTION_H

/* Diffraction spike vertex layout:
 * position(vec3) + color(vec4) = 7 floats per vertex.
 * Each spike is a line segment: 2 vertices (center + tip).
 * Each star gets DIFFRACTION_SPIKES_PER_STAR spikes = that many line segments. */
#define DIFFRACTION_SPIKES_PER_STAR 4
#define DIFFRACTION_VERTEX_FLOATS 7
#define DIFFRACTION_VERTEX_STRIDE (DIFFRACTION_VERTEX_FLOATS * (int)sizeof(float))
#define DIFFRACTION_VERTS_PER_STAR (DIFFRACTION_SPIKES_PER_STAR * 2)

/* Diffraction spike pack result metadata */
typedef struct {
    int star_count;      /* number of stars with spikes */
    int line_count;      /* total line segments */
    int vertex_count;    /* line_count * 2 */
    int float_count;     /* vertex_count * DIFFRACTION_VERTEX_FLOATS */
} diffraction_info_t;

/* Configuration for spike generation */
typedef struct {
    float base_length;       /* spike length for magnitude 0 star (scene units) */
    float min_magnitude;     /* only stars brighter than this get spikes */
    float center_alpha;      /* alpha at spike center */
    float tip_alpha;         /* alpha at spike tip */
    float rotation_deg;      /* rotation angle for spike cross pattern */
    int spike_count;         /* 4 or 6 spikes per star (clamped) */
} diffraction_config_t;

/* Default configuration. */
diffraction_config_t diffraction_default_config(void);

/* Pack diffraction spikes for bright stars into GL_LINES vertex arrays.
 * star_positions: interleaved xyz (3 floats per star).
 * star_colors: interleaved rgb (3 floats per star).
 * star_magnitudes: one float per star.
 * count: total number of stars (all will be checked against min_magnitude).
 * config: spike generation parameters.
 * out: caller-provided buffer, must hold at least
 *      count * config.spike_count * 2 * DIFFRACTION_VERTEX_FLOATS floats.
 * Returns number of stars that received spikes. */
int diffraction_pack(const float *star_positions, const float *star_colors,
                     const float *star_magnitudes, int count,
                     const diffraction_config_t *config, float *out);

/* Get metadata after packing. star_count: value returned by diffraction_pack(). */
diffraction_info_t diffraction_info(int star_count, int spike_count);

/* Compute spike length from magnitude. Brighter stars get longer spikes.
 * base_length: length for magnitude 0. Returns 0 if mag >= min_magnitude. */
float diffraction_spike_length(float magnitude, float base_length,
                               float min_magnitude);

/* Compute spike alpha from distance along spike (0=center, 1=tip).
 * Linear interpolation from center_alpha to tip_alpha. t is clamped to [0,1]. */
float diffraction_spike_alpha(float t, float center_alpha, float tip_alpha);

/* Diffraction spike vertex shader source (GLSL ES 3.00).
 * Uniforms: u_mvp (mat4).
 * Attributes: a_position (vec3, loc 0), a_color (vec4, loc 1). */
const char *diffraction_vert_source(void);

/* Diffraction spike fragment shader source (GLSL ES 3.00).
 * Per-vertex RGBA passthrough with additive blending hint. */
const char *diffraction_frag_source(void);

#endif /* TIME_DIFFRACTION_H */
