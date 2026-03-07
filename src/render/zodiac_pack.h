/*
 * zodiac_pack.h — Zodiac Render Pack: GPU-ready batched vertex arrays
 *
 * Packs ALL zodiac wheel elements into interleaved vertex arrays ready
 * for glBufferData. Unifies ring_geometry, arc_geometry, cusp_lines,
 * aspect_lines, glyph_batch, and billboard into a single draw-ready
 * data set. Follows the star_field.h template pattern.
 *
 * Three draw calls:
 *   1. Ring (12 colored zodiac segments)
 *   2. Lines (degree ticks + house cusps + aspect lines)
 *   3. Glyphs (sign + planet textured quads)
 *
 * Includes GLSL ES 3.00 shader source strings for each draw call.
 *
 * Pure module: no GL calls, no malloc, no globals, no side effects.
 */

#ifndef TIME_ZODIAC_PACK_H
#define TIME_ZODIAC_PACK_H

/* Ring vertex: position(vec3) + color(vec4) + uv(vec2) = 9 floats */
#define ZP_RING_VERTEX_FLOATS 9

/* Line vertex: position(vec3) + color(vec4) = 7 floats */
#define ZP_LINE_VERTEX_FLOATS 7

/* Glyph vertex: position(vec3) + uv(vec2) + color(vec4) = 9 floats */
#define ZP_GLYPH_VERTEX_FLOATS 9

/* Max sizes */
#define ZP_MAX_RING_VERTS 2048   /* 12 segments * ~24 verts each */
#define ZP_MAX_RING_INDICES 3072 /* 12 segments * ~48 indices each */
#define ZP_MAX_TICK_LINES 360    /* degree tick marks */
#define ZP_MAX_CUSP_LINES 12     /* house cusps */
#define ZP_MAX_ASPECT_LINES 28   /* 8 planets, C(8,2) max */

/* Ring segment data packed for GPU */
typedef struct {
    float vertices[ZP_MAX_RING_VERTS * ZP_RING_VERTEX_FLOATS];
    unsigned int indices[ZP_MAX_RING_INDICES];
    int vertex_count;
    int index_count;
    int sign_offsets[12];  /* vertex start index per sign */
    int sign_counts[12];   /* vertex count per sign */
} zp_ring_data_t;

/* Line data packed for GPU (ticks + cusps + aspects) */
typedef struct {
    float vertices[((ZP_MAX_TICK_LINES + ZP_MAX_CUSP_LINES + ZP_MAX_ASPECT_LINES) * 2) * ZP_LINE_VERTEX_FLOATS];
    int tick_count;        /* degree tick line segments */
    int cusp_count;        /* house cusp line segments */
    int aspect_count;      /* aspect line segments */
    int tick_offset;       /* vertex offset for ticks */
    int cusp_offset;       /* vertex offset for cusps */
    int aspect_offset;     /* vertex offset for aspects */
    int total_vertex_count;
} zp_line_data_t;

/* Glyph quad data packed for GPU (sign + planet glyphs) */
typedef struct {
    float vertices[24 * 4 * ZP_GLYPH_VERTEX_FLOATS]; /* 12 sign + 12 planet max */
    unsigned int indices[24 * 6]; /* 6 indices per quad */
    int vertex_count;
    int index_count;
    int sign_glyph_count;   /* always 12 */
    int planet_glyph_count; /* 0-12 */
} zp_glyph_data_t;

/* Pack the 12 zodiac ring segments with per-sign colors.
 * inner_radius, outer_radius: ring bounds.
 * segments_per_sign: tessellation level (suggested: 8).
 * sign_colors: 12 * 4 floats (RGBA per sign), NULL for defaults. */
zp_ring_data_t zp_pack_ring(float inner_radius, float outer_radius,
                             int segments_per_sign,
                             const float *sign_colors);

/* Pack all line data: degree ticks + house cusps + aspect lines.
 * inner_radius, outer_radius: ring bounds for ticks/cusps.
 * cusps: 12 house cusp longitudes in degrees (NULL to skip cusps).
 * planet_positions: flat float array of num_planets * 3 (x,y,z).
 * planet_longitudes: num_planets ecliptic longitudes in degrees.
 * num_planets: number of planets (0 to skip aspects).
 * aspect_orb: maximum orb in degrees. */
zp_line_data_t zp_pack_lines(float inner_radius, float outer_radius,
                              const double *cusps,
                              const float *planet_positions,
                              const double *planet_longitudes,
                              int num_planets, double aspect_orb);

/* Pack sign and planet glyph quads.
 * ring_radius: radius for glyph placement (midpoint of ring).
 * cam_right, cam_up: camera basis vectors for billboarding.
 * planet_longitudes: ecliptic longitudes for planet markers.
 * num_planets: number of planets.
 * glyph_scale: size of glyph quads in world units. */
zp_glyph_data_t zp_pack_glyphs(float ring_radius,
                                 const float cam_right[3],
                                 const float cam_up[3],
                                 const double *planet_longitudes,
                                 int num_planets,
                                 float glyph_scale);

/* Get byte sizes for glBufferData */
int zp_ring_vertex_bytes(const zp_ring_data_t *data);
int zp_ring_index_bytes(const zp_ring_data_t *data);
int zp_line_vertex_bytes(const zp_line_data_t *data);
int zp_glyph_vertex_bytes(const zp_glyph_data_t *data);
int zp_glyph_index_bytes(const zp_glyph_data_t *data);

/* Zodiac ring vertex shader (GLSL ES 3.00) */
const char *zp_ring_vert_source(void);
/* Zodiac ring fragment shader (GLSL ES 3.00) */
const char *zp_ring_frag_source(void);
/* Line vertex shader (GLSL ES 3.00) — shared for ticks/cusps/aspects */
const char *zp_line_vert_source(void);
/* Line fragment shader (GLSL ES 3.00) */
const char *zp_line_frag_source(void);
/* Glyph vertex shader (GLSL ES 3.00) */
const char *zp_glyph_vert_source(void);
/* Glyph fragment shader (GLSL ES 3.00) */
const char *zp_glyph_frag_source(void);

#endif /* TIME_ZODIAC_PACK_H */
