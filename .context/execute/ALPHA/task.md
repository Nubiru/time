# Task: Star Field GPU Data

**Agent**: ALPHA
**Roadmap Reference**: Track 7.7 — "Star Point Sprite Shader" (data prep component)
**Date**: 2026-03-07
**Status**: COMPLETE

## Goal

Pure data module that packs the star_catalog (195 stars) and star_catalog_ext (400 stars) into GPU-ready interleaved vertex arrays. Also packs constellation line data into a separate vertex array. Includes star point-sprite shader source and constellation line shader source as static const strings. This module bridges our pure star data to the rendering pipeline — it produces the exact float arrays that `glBufferData` will consume.

This is the HIGHEST PRIORITY visual task. Stars on screen transforms the scene from "demo" to "art."

## READ FIRST

- `src/render/star_catalog.h` — star_entry_t, star_to_ecliptic_xyz(), star_bv_to_rgb(), star_mag_to_size()
- `src/render/star_catalog_ext.h` — ext_star_t, ext_star_get(), ext_star_count(), tier system
- `src/render/star_colors.h` — star_color_from_bv() (Ballesteros formula, more accurate)
- `src/render/constellation.h` — constellation_t, cline_t, constellation_get()
- `src/render/render_layers.h` — LAYER_STARS, layer opacity gating

## Files to Create

- `src/render/star_field.h`
- `src/render/star_field.c`
- `tests/render/test_star_field.c`

## API

```c
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
```

## Shader Specifications

### Star Vertex Shader (GLSL ES 3.00)
- Transform position by MVP matrix
- Set `gl_PointSize` = `a_size * u_scale_factor / gl_Position.w` (perspective scaling)
- Clamp gl_PointSize to reasonable range (1.0 to 32.0)
- Pass color to fragment shader

### Star Fragment Shader (GLSL ES 3.00)
- Use `gl_PointCoord` to compute distance from point center
- Circular shape: `alpha = 1.0 - smoothstep(0.5, 1.0, dist * 2.0)`
- Multiply alpha by vertex color, output RGBA
- Designed for additive blending (GL_SRC_ALPHA, GL_ONE)

### Constellation Line Vertex Shader
- Transform position by MVP matrix
- Pass vec4 color to fragment

### Constellation Line Fragment Shader
- Output per-vertex RGBA color directly

## DONE WHEN

- [ ] `star_field_pack()` produces correct interleaved vertex data for all available stars
- [ ] Stars sorted by magnitude, tier offsets match ext catalog tier boundaries
- [ ] Deduplication removes stars appearing in both catalogs (RA/Dec proximity < 0.01 deg)
- [ ] `constellation_lines_pack()` resolves star indices to 3D ecliptic positions
- [ ] Zodiac constellations (first 12) packed first with higher alpha
- [ ] All 4 shader source strings are syntactically valid GLSL ES 3.00
- [ ] Shader sources declare correct attribute locations, uniform names, precision
- [ ] `star_field_total_count()` returns correct deduplicated count
- [ ] >= 35 tests covering: vertex packing correctness, tier offsets, deduplication, constellation line resolution, shader string presence, metadata, edge cases (invalid index, zero buffer)
- [ ] All tests pass with zero warnings
- [ ] Purity: no malloc, no globals, no side effects
- [ ] Compiles: `gcc -Wall -Wextra -Werror -std=c11 -pedantic`

## Constraints

- C11, `-Wall -Wextra -Werror -std=c11 -pedantic`
- No malloc — caller provides output buffers (stack or static)
- No globals, no side effects
- Depends on: star_catalog.h, star_catalog_ext.h, star_colors.h, constellation.h
- Shader strings as `static const char []` in the .c file
- GLSL ES 3.00 (WebGL2 target) — must include `#version 300 es` and `precision` qualifiers
- Positions on celestial sphere at configurable radius (default 100 scene units)
- Use `star_color_from_bv()` from star_colors.h (Ballesteros formula, physically accurate)
- Interleaved vertex layout for cache coherence
- Constellation line colors: cool blue-white (0.6, 0.7, 0.9, alpha)
- This module does NOT call any GL functions — only produces float arrays + shader strings
