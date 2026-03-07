/*
 * milkyway_pack.h -- Milky Way band vertex packing
 *
 * Packs galaxy_geometry spiral arm data into triangle quad vertex arrays
 * for Milky Way band rendering on the celestial sphere. Each arm point
 * becomes a cross-section of the galactic band: two vertices offset
 * perpendicular to the arm direction. Includes brightness gradient,
 * dark lane (Great Rift) opacity reduction, and procedural glow shader.
 *
 * Pure module: no GL calls, no malloc, no globals, no side effects.
 *
 * Data sources:
 *   - galaxy_geometry.h -- galaxy_mesh_t, galaxy_arm_t, galaxy_generate
 */

#ifndef TIME_MILKYWAY_PACK_H
#define TIME_MILKYWAY_PACK_H

/* Vertex layout: position(vec3) + uv(vec2) + color(vec4) = 9 floats per vertex */
#define MWP_VERTEX_FLOATS 9
#define MWP_VERTEX_STRIDE (MWP_VERTEX_FLOATS * (int)sizeof(float))

/* Maximum vertices: 4 arms * 128 points * 2 sides * 6 verts per quad segment */
#define MWP_MAX_VERTICES (4 * 128 * 2 * 6)

/* Pack result metadata */
typedef struct {
    int vertex_count;   /* total vertices packed */
    int float_count;    /* vertex_count * MWP_VERTEX_FLOATS */
    int arm_count;      /* number of spiral arms rendered */
    int segment_count;  /* total strip segments across all arms */
} mwp_info_t;

/* Configuration for packing */
typedef struct {
    float sphere_radius;     /* celestial sphere radius (e.g. 200.0) */
    float band_width;        /* half-width of the galactic band (e.g. 15.0) */
    float core_brightness;   /* brightness at galactic center (e.g. 0.8) */
    float edge_brightness;   /* brightness at arm edges (e.g. 0.15) */
    float base_alpha;        /* base opacity (e.g. 0.4) */
    float dark_lane_factor;  /* opacity reduction in dark lanes (e.g. 0.3) */
    float galaxy_radius;     /* for galaxy_generate call */
    float sun_distance;      /* for galaxy_generate call */
    float rotation_deg;      /* galactic rotation for galaxy_generate */
    int points_per_arm;      /* resolution per arm (clamped to 128) */
} mwp_config_t;

/* Default configuration. */
mwp_config_t mwp_default_config(void);

/* Pack Milky Way band into triangle strip vertex arrays.
 * out: caller-provided buffer, must hold at least MWP_MAX_VERTICES * MWP_VERTEX_FLOATS floats.
 * Returns number of vertices packed. */
int mwp_pack(const mwp_config_t *config, float *out);

/* Get metadata from vertex count. */
mwp_info_t mwp_info(int vertex_count);

/* Map a galactic arm point to a position on the celestial sphere.
 * Transforms from galactic plane (x,z) to 3D sphere coordinates.
 * galactic_lat_deg: angular offset from galactic plane.
 * out: xyz position on sphere. */
void mwp_galactic_to_sphere(float gal_x, float gal_z, float galactic_lat_deg,
                             float sphere_radius, float out[3]);

/* Compute brightness factor along an arm from center to edge.
 * t: 0.0 = arm center, 1.0 = arm edge.
 * core_brightness: at center, edge_brightness: at edge. */
float mwp_arm_brightness(float t, float core_brightness, float edge_brightness);

/* Compute dark lane opacity reduction.
 * galactic_lon_deg: longitude from galactic center.
 * Returns multiplier (1.0 = no reduction, dark_lane_factor = max reduction).
 * Great Rift spans roughly 300-360 and 0-30 degrees galactic longitude. */
float mwp_dark_lane_factor(float galactic_lon_deg, float dark_lane_factor);

/* Milky Way vertex shader source (GLSL ES 3.00).
 * Uniforms: u_mvp (mat4).
 * Attributes: a_position (vec3, loc 0), a_uv (vec2, loc 1), a_color (vec4, loc 2). */
const char *mwp_vert_source(void);

/* Milky Way fragment shader source (GLSL ES 3.00).
 * Procedural glow with lateral falloff from band center. */
const char *mwp_frag_source(void);

#endif /* TIME_MILKYWAY_PACK_H */
