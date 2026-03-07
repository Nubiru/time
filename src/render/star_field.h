/*
 * star_field.h — Star field GPU data packing
 *
 * Packs star_catalog (195 stars) + star_catalog_ext (400 stars) into
 * interleaved vertex arrays ready for glBufferData. Also packs
 * constellation line segments. Includes GLSL ES 3.00 shader source strings.
 *
 * Pure module: no GL calls, no malloc, no globals, no side effects.
 * Caller provides output buffers.
 */

#ifndef TIME_STAR_FIELD_H
#define TIME_STAR_FIELD_H

/* Obliquity of ecliptic for coordinate conversion */
#define STAR_FIELD_OBLIQUITY 23.4393f

/* Vertex layout: position (vec3) + color (vec3) + size (float) = 7 floats per star */
#define STAR_VERTEX_FLOATS 7
#define STAR_VERTEX_STRIDE (STAR_VERTEX_FLOATS * (int)sizeof(float))

/* Constellation line vertex: position (vec3) + color (vec4) = 7 floats per endpoint */
#define CLINE_VERTEX_FLOATS 7
#define CLINE_VERTEX_STRIDE (CLINE_VERTEX_FLOATS * (int)sizeof(float))

/* Star field vertex data — packed and ready for GPU upload */
typedef struct {
    int vertex_count;        /* number of stars packed */
    int float_count;         /* vertex_count * STAR_VERTEX_FLOATS */
    int tier_offsets[4];     /* starting vertex index per brightness tier */
    int tier_counts[4];      /* vertex count per tier */
} star_field_info_t;

/* Constellation line vertex data info */
typedef struct {
    int line_count;          /* number of line segments packed */
    int vertex_count;        /* line_count * 2 */
    int float_count;         /* vertex_count * CLINE_VERTEX_FLOATS */
    int zodiac_line_count;   /* first N lines are zodiac constellations (brighter) */
} constellation_line_info_t;

/* Pack all stars into GPU-ready vertex array.
 * Combines star_catalog (195) + star_catalog_ext (400), deduplicating overlapping stars.
 * Stars sorted by magnitude (brightest first).
 * base_point_size: size for magnitude 0 star (suggested: 8.0).
 * sphere_radius: radius of celestial sphere in scene units (suggested: 100.0).
 * Writes into caller-provided buffer. Returns star count written.
 * Buffer must be at least max_stars * STAR_VERTEX_FLOATS floats. */
int star_field_pack(float *out, int max_stars, float base_point_size,
                    float sphere_radius);

/* Get star field metadata (tier offsets, counts) after packing.
 * star_count: value returned by star_field_pack(). */
star_field_info_t star_field_info(int star_count);

/* Pack constellation lines into GPU-ready vertex array.
 * Resolves star indices to 3D positions via star_to_ecliptic_xyz().
 * Zodiac constellations packed first with higher alpha.
 * base_alpha: opacity for non-zodiac lines (suggested: 0.25).
 * zodiac_alpha: opacity for zodiac lines (suggested: 0.4).
 * sphere_radius: must match star_field_pack radius.
 * Buffer must be at least max_lines * 2 * CLINE_VERTEX_FLOATS floats.
 * Returns line count written. */
int constellation_lines_pack(float *out, int max_lines, float base_alpha,
                             float zodiac_alpha, float sphere_radius);

/* Get constellation line metadata after packing.
 * line_count: value returned by constellation_lines_pack(). */
constellation_line_info_t constellation_lines_info(int line_count);

/* Total star count available (both catalogs combined, deduplicated). */
int star_field_total_count(void);

/* Star point-sprite vertex shader source (GLSL ES 3.00).
 * Uniforms: u_mvp (mat4), u_scale_factor (float).
 * Attributes: a_position (vec3, loc 0), a_color (vec3, loc 1), a_size (float, loc 2).
 * Returns pointer to static const null-terminated string. */
const char *star_field_vert_source(void);

/* Star point-sprite fragment shader source (GLSL ES 3.00).
 * Circular dot with soft radial falloff via gl_PointCoord. */
const char *star_field_frag_source(void);

/* Constellation line vertex shader source (GLSL ES 3.00).
 * Uniforms: u_mvp (mat4). Attributes: a_position (vec3, loc 0), a_color (vec4, loc 1). */
const char *constellation_line_vert_source(void);

/* Constellation line fragment shader source (GLSL ES 3.00).
 * Per-vertex RGBA passthrough. */
const char *constellation_line_frag_source(void);

#endif /* TIME_STAR_FIELD_H */
