/* sky_dome.h — Sky hemisphere mesh for Earth View.
 *
 * Generates a hemisphere of vertices centered at the observer's position,
 * colored by atmospheric scattering based on sun altitude. Each vertex stores
 * position (on hemisphere) and sky color at that direction.
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 *
 * Coordinate system: hemisphere in local tangent space (observer at origin).
 * Y-up: zenith is (0,1,0), horizon is XZ plane. */

#ifndef TIME_SKY_DOME_H
#define TIME_SKY_DOME_H

/* Hemisphere resolution */
#define SKY_DOME_ALT_STEPS   16   /* altitude divisions (horizon to zenith) */
#define SKY_DOME_AZ_STEPS    32   /* azimuth divisions (full 360) */
#define SKY_DOME_MAX_VERTICES ((SKY_DOME_ALT_STEPS + 1) * (SKY_DOME_AZ_STEPS + 1))
#define SKY_DOME_MAX_INDICES  (SKY_DOME_ALT_STEPS * SKY_DOME_AZ_STEPS * 6)

/* Vertex layout: position(3) + color(3) + uv(2) = 8 floats */
#define SKY_DOME_VERT_FLOATS  8

/* Sky dome configuration */
typedef struct {
    float  dome_radius;         /* radius of the hemisphere in scene units */
    double sun_altitude_deg;    /* sun elevation above horizon (-90 to 90) */
    double sun_azimuth_deg;     /* sun compass bearing (0=N, 90=E) */
} sky_dome_config_t;

/* Pack result */
typedef struct {
    int vertex_count;
    int index_count;
} sky_dome_info_t;

/* Default configuration */
sky_dome_config_t sky_dome_default_config(void);

/* Generate sky dome hemisphere mesh.
 * config: dome parameters including sun position for coloring.
 * out_vertices: buffer of SKY_DOME_MAX_VERTICES * SKY_DOME_VERT_FLOATS floats.
 * out_indices: buffer of SKY_DOME_MAX_INDICES unsigned ints.
 * Returns sky_dome_info_t with actual counts. */
sky_dome_info_t sky_dome_generate(sky_dome_config_t config,
                                   float *out_vertices,
                                   unsigned int *out_indices);

/* Get sky color at a specific direction.
 * altitude_deg: 0=horizon, 90=zenith.
 * angular_distance_to_sun_deg: angle from this direction to sun.
 * sun_altitude_deg: sun elevation above horizon.
 * Returns RGB in [0,1] via rgb[3]. */
void sky_dome_color_at(double altitude_deg,
                        double angular_distance_to_sun_deg,
                        double sun_altitude_deg,
                        float rgb[3]);

/* Vertex count for current resolution. */
int sky_dome_vertex_count(void);

/* Index count for current resolution. */
int sky_dome_index_count(void);

/* Vertex shader source (GLSL ES 3.00). */
const char *sky_dome_vert_source(void);

/* Fragment shader source (GLSL ES 3.00). */
const char *sky_dome_frag_source(void);

#endif /* TIME_SKY_DOME_H */
