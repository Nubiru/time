/*
 * constellation_bounds.h — IAU constellation boundary polygon data
 *
 * Provides boundary polygons for all 88 IAU constellations as GPU-ready
 * GL_LINES vertex arrays. Boundaries divide the celestial sphere into
 * 88 regions following constant-RA and constant-Dec arcs (J2000.0).
 *
 * Data: Simplified from Eugène Delporte (1930), Davenhall & Leggett (1989).
 *
 * Pure module: no GL calls, no malloc, no globals, no side effects.
 * Caller provides output buffers.
 */

#ifndef TIME_CONSTELLATION_BOUNDS_H
#define TIME_CONSTELLATION_BOUNDS_H

#ifndef PI
#define PI 3.14159265358979323846
#endif

#define CB_CONSTELLATION_COUNT 88
#define CB_VERTEX_FLOATS 7
#define CB_VERTEX_STRIDE (CB_VERTEX_FLOATS * (int)sizeof(float))
#define CB_MAX_TOTAL_SEGMENTS 1200
#define CB_MAX_TOTAL_VERTICES (CB_MAX_TOTAL_SEGMENTS * 2)
#define CB_MAX_BOUNDARY_VERTICES 40

/* A point on the celestial sphere in equatorial coordinates */
typedef struct {
    float ra_hours;    /* 0-24 */
    float dec_deg;     /* -90 to +90 */
} cb_point_t;

/* Constellation metadata */
typedef struct {
    const char *name;
    const char *abbr;          /* IAU 3-letter */
    int vertex_count;
    int is_zodiac;
} cb_constellation_t;

/* Packing metadata */
typedef struct {
    int total_segments;
    int total_vertices;
    int zodiac_segments;
    int constellation_offsets[CB_CONSTELLATION_COUNT];
    int constellation_counts[CB_CONSTELLATION_COUNT];
} cb_pack_info_t;

/* Get constellation metadata by index (0-87).
 * Returns sentinel (name=NULL) for invalid index. */
cb_constellation_t cb_get(int index);

/* Get boundary vertex count for constellation at index. Returns 0 if invalid. */
int cb_vertex_count(int index);

/* Get a single boundary vertex (RA/Dec) by constellation and vertex index.
 * Returns {0,0} for invalid indices. */
cb_point_t cb_vertex(int constellation_index, int vertex_index);

/* Find constellation index by IAU 3-letter abbreviation.
 * Returns -1 if not found. */
int cb_find(const char *abbr);

/* Total number of boundary segments across all 88 constellations.
 * Each closed polygon of N vertices has N segments. */
int cb_total_segments(void);

/* Find which constellation contains a given RA/Dec point.
 * Returns constellation index, or -1 if not found.
 * Uses ray-casting point-in-polygon on RA/Dec coordinates. */
int cb_containing(float ra_hours, float dec_deg);

/* Pack all constellation boundaries into GPU-ready GL_LINES vertex array.
 * Zodiac constellations packed first with zodiac_alpha, others with base_alpha.
 * Vertex layout: position(vec3) + color(vec4) = 7 floats per vertex.
 * Buffer must be at least cb_total_segments() * 2 * CB_VERTEX_FLOATS floats.
 * Returns number of segments written. */
int cb_pack(float sphere_radius, float base_alpha, float zodiac_alpha,
            float *out);

/* Get packing metadata for a given segment count. */
cb_pack_info_t cb_pack_info(int segment_count);

/* Pack a single constellation's boundary into GL_LINES vertex array.
 * Buffer must be at least cb_vertex_count(index) * 2 * CB_VERTEX_FLOATS floats.
 * Returns number of segments written. */
int cb_pack_one(int index, float sphere_radius, float alpha, float *out);

/* Boundary line vertex shader source (GLSL ES 3.00).
 * Uniforms: u_mvp (mat4). Attributes: a_position (vec3, loc 0), a_color (vec4, loc 1). */
const char *cb_vert_source(void);

/* Boundary line fragment shader source (GLSL ES 3.00).
 * Per-vertex RGBA passthrough. */
const char *cb_frag_source(void);

#endif /* TIME_CONSTELLATION_BOUNDS_H */
