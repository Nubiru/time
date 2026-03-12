/*
 * saturn_ring_pack.h — Saturn ring system render pack: GPU-ready vertex packing
 *
 * Packs Saturn's ring system (D, C, B, Cassini Division, A, Encke Gap, F)
 * into an annular disc mesh with per-vertex opacity, color data, and normals.
 * Includes GLSL ES 3.00 shader source strings for ring rendering with
 * diffuse lighting and Saturn shadow.
 *
 * Pure module: no GL calls, no malloc, no globals, no side effects.
 *
 * Data sources:
 *   - NASA/JPL Saturn ring measurements (radii in Saturn radii, 1 Rs = 60,268 km)
 *   - Ring opacity and albedo from Voyager/Cassini observations
 */

#ifndef TIME_SATURN_RING_PACK_H
#define TIME_SATURN_RING_PACK_H

#ifndef PI
#define PI 3.14159265358979323846
#endif

/* Saturn's ring system physical data (radii in Saturn radii, 1 Rs = 60,268 km) */
#define SRP_SATURN_RADIUS_KM 60268.0f

/* Ring boundaries */
typedef struct {
    const char *name;
    float inner_radius;   /* in Saturn radii */
    float outer_radius;   /* in Saturn radii */
    float opacity;        /* 0.0 = transparent, 1.0 = opaque */
    float albedo;         /* reflectivity 0-1 */
} srp_ring_t;

/* Ring system configuration */
typedef struct {
    float saturn_scene_radius;   /* Saturn body radius in scene units */
    float axial_tilt_deg;        /* Saturn's axial tilt (default: 26.73) */
    int   radial_segments;       /* segments from inner to outer (default: 16) */
    int   angular_segments;      /* segments around the ring (default: 72) */
} srp_config_t;

/* Default ring system configuration. */
srp_config_t srp_default_config(void);

/* Number of ring bands (D, C, B, Cassini, A, Encke, F). */
int srp_ring_count(void);

/* Get ring data by index. Returns sentinel (name=NULL) if out of bounds. */
srp_ring_t srp_ring_get(int index);

/* Find ring index by name. Returns -1 if not found or name is NULL. */
int srp_find_ring(const char *name);

/* Vertex: position(vec3) + normal(vec3) + texcoord(vec2) + opacity(float) = 9 floats */
#define SRP_VERTEX_FLOATS 9
#define SRP_MAX_VERTICES 4000

/* Pack result metadata */
typedef struct {
    int vertex_count;
    int index_count;
    int triangle_count;
    int ring_count;
} srp_info_t;

/* Pack ring system mesh into interleaved vertex arrays.
 * out_verts: SRP_VERTEX_FLOATS floats per vertex (pos + normal + uv + opacity).
 * out_indices: triangle indices (unsigned short).
 * Returns pack metadata. */
srp_info_t srp_pack(srp_config_t config, float *out_verts,
                     unsigned short *out_indices);

/* Convert radius in Saturn radii to scene units. */
float srp_to_scene_radius(float saturn_radii, float saturn_scene_radius);

/* Get interpolated opacity at a given radius (in Saturn radii).
 * Returns 0 if radius is outside all ring bands. */
float srp_opacity_at_radius(float radius_sr);

/* Get interpolated color at a given radius (in Saturn radii).
 * Writes RGB values to rgb[3]. Zeros if outside rings. */
void srp_color_at_radius(float radius_sr, float rgb[3]);

/* Check if a radius falls in a gap (Cassini Division or Encke Gap).
 * Returns 1 if gap, 0 otherwise. */
int srp_is_gap(float radius_sr);

/* Saturn ring vertex shader source (GLSL ES 3.00). */
const char *srp_vert_source(void);

/* Saturn ring fragment shader source (GLSL ES 3.00). */
const char *srp_frag_source(void);

#endif /* TIME_SATURN_RING_PACK_H */
