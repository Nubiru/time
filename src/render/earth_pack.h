/* earth_pack.h — GPU-ready vertex packing for Earth globe, coastlines,
 * terminator, and atmosphere shell.
 *
 * Combines earth_globe mesh data (UV-sphere with normals/UVs), coastline
 * line segments, day/night terminator great circle, and atmosphere shell
 * mesh into renderable interleaved vertex arrays. Includes GLSL ES 3.00
 * shaders for Earth surface (day/night based on sun direction), atmosphere
 * limb glow, and coastline/terminator overlays.
 *
 * Pure module: no GL calls, no malloc, no globals, no side effects.
 *
 * Data sources:
 *   - earth_globe.h — earth_vertex_t, earth_mesh_info_t, coastlines, terminator
 *   - earth_atmosphere.h — atmo_vertex_t, atmo_mesh_info_t, shell mesh */

#ifndef TIME_EARTH_PACK_H
#define TIME_EARTH_PACK_H

/* Globe vertex layout: position(vec3) + normal(vec3) + uv(vec2) = 8 floats */
#define ERP_GLOBE_VERTEX_FLOATS 8
#define ERP_GLOBE_VERTEX_STRIDE (ERP_GLOBE_VERTEX_FLOATS * (int)sizeof(float))

/* Coastline vertex layout: position(vec3) + color(vec4) = 7 floats */
#define ERP_LINE_VERTEX_FLOATS 7
#define ERP_LINE_VERTEX_STRIDE (ERP_LINE_VERTEX_FLOATS * (int)sizeof(float))

/* Terminator: great circle as line strip */
#define ERP_TERMINATOR_POINTS 72

/* Pack result metadata */
typedef struct {
    int globe_verts;        /* globe sphere vertex count */
    int globe_indices;      /* globe triangle indices */
    int coastline_verts;    /* coastline line vertices (2 per segment) */
    int terminator_verts;   /* terminator line strip vertices */
    int atmo_verts;         /* atmosphere shell vertex count */
    int atmo_indices;       /* atmosphere shell triangle indices */
} erp_info_t;

/* Configuration */
typedef struct {
    float globe_radius;      /* Earth sphere radius in scene units */
    float atmo_radius_ratio; /* atmosphere shell / globe radius (e.g. 1.02) */
    int lat_segments;        /* globe latitude subdivisions (e.g. 32) */
    int lon_segments;        /* globe longitude subdivisions (e.g. 64) */
    float coastline_alpha;   /* coastline line opacity (e.g. 0.7) */
    float terminator_alpha;  /* terminator line opacity (e.g. 0.5) */
} erp_config_t;

/* Default configuration. */
erp_config_t erp_default_config(void);

/* Pack globe mesh into interleaved vertex array.
 * out_verts: position(vec3) + normal(vec3) + uv(vec2) per vertex.
 * out_indices: triangle indices.
 * Returns number of vertices written. */
int erp_pack_globe(const erp_config_t *config,
                   float *out_verts, unsigned int *out_indices);

/* Pack coastline segments into line vertex arrays.
 * out: position(vec3) + color(vec4) per vertex, 2 vertices per segment.
 * Returns number of vertices written. */
int erp_pack_coastlines(const erp_config_t *config, float *out);

/* Pack terminator great circle into line strip vertices.
 * solar_dec_deg: sun declination. solar_ra_hours: sun right ascension.
 * out: position(vec3) + color(vec4) per vertex.
 * Returns number of vertices written. */
int erp_pack_terminator(double solar_dec_deg, double solar_ra_hours,
                         const erp_config_t *config, float *out);

/* Pack atmosphere shell mesh.
 * out_verts: position(vec3) + normal(vec3) + opacity(float) = 7 floats.
 * out_indices: triangle indices.
 * Returns number of vertices written. */
int erp_pack_atmosphere(const erp_config_t *config,
                         float *out_verts, unsigned int *out_indices);

/* Get metadata for a full Earth pack at given config. */
erp_info_t erp_info(const erp_config_t *config);

/* Earth surface vertex shader source (GLSL ES 3.00).
 * Uniforms: u_mvp, u_model (mat4), u_sun_dir (vec3).
 * Attributes: a_position (vec3, 0), a_normal (vec3, 1), a_uv (vec2, 2). */
const char *erp_globe_vert_source(void);

/* Earth surface fragment shader source (GLSL ES 3.00).
 * Day/night coloring: day = warm blue-green, night = dark blue.
 * Terminator blending: smooth transition zone. */
const char *erp_globe_frag_source(void);

/* Atmosphere vertex shader (GLSL ES 3.00). */
const char *erp_atmo_vert_source(void);

/* Atmosphere fragment shader (GLSL ES 3.00).
 * Limb glow: bright blue at edges, transparent at center. */
const char *erp_atmo_frag_source(void);

/* Coastline/terminator line shader — vertex (GLSL ES 3.00). */
const char *erp_line_vert_source(void);

/* Coastline/terminator line shader — fragment (GLSL ES 3.00). */
const char *erp_line_frag_source(void);

#endif /* TIME_EARTH_PACK_H */
