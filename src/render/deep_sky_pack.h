/*
 * deep_sky_pack.h -- Deep sky object billboard vertex packing
 *
 * Packs the Messier catalog (110 DSOs) into billboard quad vertex arrays
 * for nebula/cluster/galaxy rendering. Each DSO becomes a camera-facing
 * quad with type-based coloring and magnitude-dependent opacity.
 *
 * Pure module: no GL calls, no malloc, no globals, no side effects.
 *
 * Data sources:
 *   - deep_sky.h — dso_entry_t, dso_catalog_count/get, dso_type_t
 *   - star_catalog.h — RA/Dec to ecliptic 3D coordinate convention
 */

#ifndef TIME_DEEP_SKY_PACK_H
#define TIME_DEEP_SKY_PACK_H

/* Vertex layout: position(vec3) + uv(vec2) + color(vec4) = 9 floats per vertex.
 * Each DSO = 1 quad = 6 vertices (2 triangles). */
#define DSP_VERTEX_FLOATS 9
#define DSP_VERTEX_STRIDE (DSP_VERTEX_FLOATS * (int)sizeof(float))
#define DSP_VERTS_PER_OBJECT 6

/* Pack result metadata */
typedef struct {
    int object_count;    /* number of DSOs packed */
    int vertex_count;    /* object_count * DSP_VERTS_PER_OBJECT */
    int float_count;     /* vertex_count * DSP_VERTEX_FLOATS */
    int nebula_count;    /* emission + reflection + planetary + dark + supernova remnant */
    int cluster_count;   /* open + globular */
    int galaxy_count;    /* spiral + elliptical + irregular */
} dsp_info_t;

/* Configuration for packing */
typedef struct {
    float sphere_radius;     /* celestial sphere radius in scene units (e.g. 100.0) */
    float base_size;         /* base billboard size for angular_size_arcmin=1 (e.g. 0.5) */
    float min_size;          /* minimum billboard size (e.g. 0.3) */
    float max_size;          /* maximum billboard size (e.g. 8.0) */
    float base_alpha;        /* alpha for brightest DSOs (e.g. 0.6) */
    float min_alpha;         /* alpha for faintest DSOs (e.g. 0.1) */
    float bright_mag;        /* magnitude of brightest DSO for alpha scaling (e.g. 3.0) */
    float faint_mag;         /* magnitude of faintest DSO for alpha scaling (e.g. 10.0) */
} dsp_config_t;

/* Default configuration. */
dsp_config_t dsp_default_config(void);

/* Pack all DSOs into billboard quad vertex arrays.
 * cam_right: camera right vector (3 floats).
 * cam_up: camera up vector (3 floats).
 * config: packing parameters.
 * out: caller-provided buffer, must hold at least
 *      dso_catalog_count() * DSP_VERTS_PER_OBJECT * DSP_VERTEX_FLOATS floats.
 * Returns number of objects packed. */
int dsp_pack(const float cam_right[3], const float cam_up[3],
             const dsp_config_t *config, float *out);

/* Get metadata after packing. object_count: value returned by dsp_pack(). */
dsp_info_t dsp_info(int object_count);

/* Convert DSO RA/Dec to 3D position on celestial sphere.
 * Uses ecliptic coordinates matching star_field convention.
 * obliquity_deg: Earth's axial tilt (~23.44). */
void dsp_ra_dec_to_xyz(double ra_hours, double dec_deg, float radius,
                       float obliquity_deg, float out[3]);

/* Compute billboard size from angular size in arcminutes.
 * base_size: size for 1 arcmin. Clamped to [min_size, max_size]. */
float dsp_billboard_size(double angular_size_arcmin, float base_size,
                         float min_size, float max_size);

/* Compute alpha from apparent magnitude.
 * Linear interpolation: bright_mag -> base_alpha, faint_mag -> min_alpha. */
float dsp_magnitude_alpha(double magnitude, float base_alpha, float min_alpha,
                          float bright_mag, float faint_mag);

/* Get render color for a DSO type with alpha.
 * emission=red/pink, reflection=blue, planetary=teal, cluster=warm white,
 * galaxy=pale blue. */
void dsp_type_color(int type, float alpha, float out[4]);

/* Deep sky billboard vertex shader source (GLSL ES 3.00).
 * Uniforms: u_mvp (mat4).
 * Attributes: a_position (vec3, loc 0), a_uv (vec2, loc 1),
 *             a_color (vec4, loc 2). */
const char *dsp_vert_source(void);

/* Deep sky billboard fragment shader source (GLSL ES 3.00).
 * Gaussian radial falloff from UV center for soft nebula appearance. */
const char *dsp_frag_source(void);

#endif /* TIME_DEEP_SKY_PACK_H */
