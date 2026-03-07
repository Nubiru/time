/*
 * planet_pack.h — Planet render pack: GPU-ready vertex packing
 *
 * Packs planet positions, colors, sizes, atmosphere data, and orbit trail
 * polylines into interleaved vertex arrays for GPU upload. Includes
 * GLSL ES 3.00 shader source strings for planet billboards and trail lines.
 *
 * Pure module: no GL calls, no malloc, no globals, no side effects.
 *
 * Data sources:
 *   - planets.h / orbit.h — Keplerian orbital elements (JPL Table 1)
 *   - planet_data.h — NASA Planetary Fact Sheet (colors, radii)
 *   - atmo_ring.h — Atmosphere thickness ratios
 */

#ifndef TIME_PLANET_PACK_H
#define TIME_PLANET_PACK_H

/* Planet vertex: position(vec3) + color(vec3) + radius(float) + atmo_thickness(float) = 8 floats */
#define PP_PLANET_VERTEX_FLOATS 8
#define PP_PLANET_COUNT 8

/* Orbit trail vertex: position(vec3) + color(vec4) = 7 floats */
#define PP_TRAIL_VERTEX_FLOATS 7
#define PP_TRAIL_MAX_SAMPLES 360
#define PP_TRAIL_MAX_TOTAL (PP_PLANET_COUNT * PP_TRAIL_MAX_SAMPLES)

/* Packed planet data */
typedef struct {
    float vertices[PP_PLANET_COUNT * PP_PLANET_VERTEX_FLOATS];
    int planet_count;
} pp_planet_data_t;

/* Packed orbit trail data for all planets */
typedef struct {
    float vertices[PP_TRAIL_MAX_TOTAL * PP_TRAIL_VERTEX_FLOATS];
    int trail_offsets[PP_PLANET_COUNT];
    int trail_counts[PP_PLANET_COUNT];
    int total_vertex_count;
} pp_trail_data_t;

/* Pack planet positions, colors, sizes at a given Julian Day.
 * scene_scale: AU-to-scene-units conversion (e.g., 10.0 = 1 AU = 10 units).
 * radius_scale: planet radius multiplier for visibility. */
pp_planet_data_t pp_pack_planets(double jd, float scene_scale,
                                  float radius_scale);

/* Pack orbit trail polylines for all 8 planets.
 * jd: current Julian Day (trail fades from here backward).
 * scene_scale: same as pp_pack_planets.
 * samples: number of points per orbit (max PP_TRAIL_MAX_SAMPLES). */
pp_trail_data_t pp_pack_trails(double jd, float scene_scale, int samples);

/* Get byte size of planet vertex data. */
int pp_planet_vertex_bytes(const pp_planet_data_t *data);

/* Get byte size of trail vertex data. */
int pp_trail_vertex_bytes(const pp_trail_data_t *data);

/* Planet billboard vertex shader (GLSL ES 3.00). */
const char *pp_planet_vert_source(void);

/* Planet billboard fragment shader (GLSL ES 3.00). */
const char *pp_planet_frag_source(void);

/* Orbit trail line vertex shader (GLSL ES 3.00). */
const char *pp_trail_vert_source(void);

/* Orbit trail line fragment shader (GLSL ES 3.00). */
const char *pp_trail_frag_source(void);

#endif /* TIME_PLANET_PACK_H */
