/*
 * lens_flare.h — Procedural screen-space lens flare geometry
 *
 * Generates vertex data for cinematic lens flare effects caused by the Sun
 * and bright stars. Produces screen-space quads for starburst, halo, ghost,
 * streak, and ring elements positioned along the flare axis.
 *
 * All geometry is in NDC (Normalized Device Coordinates): x,y from -1 to +1.
 * No view/projection transform in vertex shader — just pass-through NDC.
 *
 * Vertex layout: position(vec2 NDC) + uv(vec2) + color(vec4) = 8 floats.
 * Each element is a quad: 2 triangles = 6 vertices.
 *
 * Pure module: no GL calls, no malloc, no globals, no side effects.
 */

#ifndef TIME_LENS_FLARE_H
#define TIME_LENS_FLARE_H

#ifndef PI
#define PI 3.14159265358979323846
#endif

/* Maximum flare elements per light source */
#define LF_MAX_ELEMENTS 8

/* Vertex layout: position(vec2 NDC) + uv(vec2) + color(vec4) = 8 floats */
#define LF_VERTEX_FLOATS 8
#define LF_MAX_VERTICES 256

/* Flare element types */
typedef enum {
    LF_STARBURST = 0,  /* Central multi-ray starburst */
    LF_HALO,           /* Soft circular halo around source */
    LF_GHOST,          /* Secondary reflection disc along flare axis */
    LF_STREAK,         /* Anamorphic horizontal/vertical streak */
    LF_RING            /* Thin bright ring (aperture artifact) */
} lf_element_type_t;

/* Single flare element descriptor */
typedef struct {
    lf_element_type_t type;
    float offset;       /* Position along flare axis: 0.0=source, 1.0=center, 2.0=opposite */
    float size;         /* Element radius in NDC (0.0-1.0) */
    float intensity;    /* Brightness multiplier (0.0-1.0) */
    float color[4];     /* RGBA tint */
    int ray_count;      /* For starburst: number of rays (6, 8) */
    float rotation;     /* Rotation angle in radians */
} lf_element_t;

/* Complete flare configuration for a light source */
typedef struct {
    lf_element_t elements[LF_MAX_ELEMENTS];
    int element_count;
    float global_intensity;  /* Overall flare strength (fades with occlusion) */
    float fade_start;        /* Screen edge fade start (NDC distance from center) */
    float fade_end;          /* Screen edge fade end */
} lf_config_t;

/* Default flare configuration for the Sun (warm gold tones, 6-ray starburst). */
lf_config_t lf_sun_config(void);

/* Default flare configuration for bright stars (subtle, blue-white). */
lf_config_t lf_star_config(void);

/* Compute screen position and visibility of a light source.
 * world_pos: 3D position of light source.
 * view_proj: combined view*projection matrix (16 floats, column-major).
 * screen_pos_out: receives NDC x,y (-1 to +1). NULL-safe.
 * Returns 1 if visible (in front of camera and on screen), 0 if off-screen/behind. */
int lf_screen_position(const float world_pos[3], const float view_proj[16],
                       float screen_pos_out[2]);

/* Compute flare axis: line from source through screen center to opposite side.
 * source_ndc: light source screen position in NDC.
 * axis_out: receives normalized flare axis direction (2 floats). NULL-safe.
 * Returns axis length (distance from source to center in NDC). */
float lf_flare_axis(const float source_ndc[2], float axis_out[2]);

/* Compute intensity falloff based on screen-edge proximity.
 * source_ndc: light source position in NDC.
 * fade_start, fade_end: fade range (from config).
 * Returns intensity multiplier 0.0-1.0. */
float lf_edge_fade(const float source_ndc[2], float fade_start, float fade_end);

/* Pack all flare elements for a light source into vertex arrays.
 * config: flare configuration.
 * source_ndc: screen position of light source.
 * aspect_ratio: viewport width/height (for circular elements).
 * out_verts: output buffer, LF_VERTEX_FLOATS per vertex.
 * max_verts: buffer capacity.
 * Returns vertex count (triangle list, 6 verts per quad element). */
int lf_pack(lf_config_t config, const float source_ndc[2],
            float aspect_ratio, float *out_verts, int max_verts);

/* Lens flare vertex shader source (GLSL ES 3.00).
 * Screen-space quads, no view/proj transform needed. */
const char *lf_vert_source(void);

/* Lens flare fragment shader source (GLSL ES 3.00).
 * Per-element: starburst rays, soft halo, ghost disc, streak, ring.
 * Uniforms: u_type (int), u_ray_count (int), u_time (float). */
const char *lf_frag_source(void);

#endif /* TIME_LENS_FLARE_H */
