/*
 * moon_pack.h -- Moon render pack: GPU-ready vertex packing
 *
 * Packs 9 major moon positions, albedo-based colors, sizes, and orbit
 * trail polylines into interleaved vertex arrays for GPU upload. Includes
 * GLSL ES 3.00 shader source strings for moon billboards and trail lines.
 *
 * Pure module: no GL calls, no malloc, no globals, no side effects.
 *
 * Data sources:
 *   - moon_data.h — NASA/JPL orbital elements for 9 moons
 *   - planets.h — Keplerian heliocentric positions for parent planets
 */

#ifndef TIME_MOON_PACK_H
#define TIME_MOON_PACK_H

/* Moon vertex: position(vec3) + color(vec3) + radius(float) = 7 floats */
#define MP_VERTEX_FLOATS 7
#define MP_MAX_MOONS 9  /* Earth:1, Mars:2, Jupiter:4, Saturn:2 */

/* Moon orbit trail vertex: position(vec3) + color(vec4) = 7 floats */
#define MP_TRAIL_VERTEX_FLOATS 7
#define MP_TRAIL_MAX_SAMPLES 60
#define MP_TRAIL_MAX_TOTAL (MP_MAX_MOONS * MP_TRAIL_MAX_SAMPLES)

/* Packed moon vertex data */
typedef struct {
    float vertices[MP_MAX_MOONS * MP_VERTEX_FLOATS];
    int moon_count;
    int moon_parent[MP_MAX_MOONS];  /* parent planet index per moon */
} mp_moon_data_t;

/* Packed moon trail data */
typedef struct {
    float vertices[MP_TRAIL_MAX_TOTAL * MP_TRAIL_VERTEX_FLOATS];
    int trail_offsets[MP_MAX_MOONS]; /* start vertex per moon */
    int trail_counts[MP_MAX_MOONS];  /* vertex count per moon */
    int total_vertex_count;
} mp_trail_data_t;

/* Pack all 9 major moon positions at a given Julian Day.
 * jd: Julian Day for position computation.
 * scene_scale: AU-to-scene-units (e.g., 10.0 = 1 AU = 10 units).
 * moon_scale: additional scale factor for moon orbital distances (suggested: 5.0).
 * radius_scale: moon radius multiplier for visibility. */
mp_moon_data_t mp_pack_moons(double jd, float scene_scale,
                              float moon_scale, float radius_scale);

/* Pack moon orbit trails.
 * jd: current time. samples: points per orbit (max MP_TRAIL_MAX_SAMPLES). */
mp_trail_data_t mp_pack_trails(double jd, float scene_scale,
                                float moon_scale, int samples);

/* Get byte size of moon vertex data. */
int mp_moon_vertex_bytes(const mp_moon_data_t *data);

/* Get byte size of trail vertex data. */
int mp_trail_vertex_bytes(const mp_trail_data_t *data);

/* Moon billboard vertex shader (GLSL ES 3.00). */
const char *mp_moon_vert_source(void);
/* Moon billboard fragment shader (GLSL ES 3.00). */
const char *mp_moon_frag_source(void);
/* Moon trail vertex shader (GLSL ES 3.00). */
const char *mp_trail_vert_source(void);
/* Moon trail fragment shader (GLSL ES 3.00). */
const char *mp_trail_frag_source(void);

#endif /* TIME_MOON_PACK_H */
