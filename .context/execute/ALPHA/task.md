# Task: Planet Render Pack

**Agent**: ALPHA
**Roadmap Reference**: Track 8.6 — "Agent: Planet Render Pack (Agent A)"
**Date**: 2026-03-07
**Status**: COMPLETE

## Goal

Pure vertex packing module that packs planet positions, colors, sizes, and orbit trail polylines into GPU-ready interleaved vertex arrays. Follows star_field/zodiac_pack template: caller-provided buffers or returned structs, metadata, GLSL ES 3.00 shader source strings. No GL calls.

## READ FIRST

- `src/render/star_field.h` — template pattern
- `src/systems/astronomy/planets.h` — planets_at(jd), solar_system_t, planet_position_t
- `src/systems/astronomy/planet_data.h` — planet_data_get(), planet_color_t, scene radius
- `src/systems/astronomy/orbit.h` — orbital elements, orbit_heliocentric(), planet_orbit_t
- `src/render/atmo_ring.h` — atmo_get(), atmo_shell_positions(), ring system data
- `src/math/kepler.h` — solve_kepler() for orbit computation

## Files to Create

- `src/render/planet_pack.h`
- `src/render/planet_pack.c`
- `tests/render/test_planet_pack.c`

## API

```c
#ifndef TIME_PLANET_PACK_H
#define TIME_PLANET_PACK_H

/* Planet vertex: position(vec3) + color(vec3) + radius(float) + atmo_thickness(float) = 8 floats */
#define PP_PLANET_VERTEX_FLOATS 8
#define PP_PLANET_COUNT 8

/* Orbit trail vertex: position(vec3) + color(vec4) = 7 floats */
#define PP_TRAIL_VERTEX_FLOATS 7
#define PP_TRAIL_MAX_SAMPLES 360  /* samples per orbit */
#define PP_TRAIL_MAX_TOTAL (PP_PLANET_COUNT * PP_TRAIL_MAX_SAMPLES)

/* Packed planet data */
typedef struct {
    float vertices[PP_PLANET_COUNT * PP_PLANET_VERTEX_FLOATS];
    int planet_count;
} pp_planet_data_t;

/* Packed orbit trail data for all planets */
typedef struct {
    float vertices[PP_TRAIL_MAX_TOTAL * PP_TRAIL_VERTEX_FLOATS];
    int trail_offsets[PP_PLANET_COUNT];  /* start vertex per planet */
    int trail_counts[PP_PLANET_COUNT];   /* vertex count per planet */
    int total_vertex_count;
} pp_trail_data_t;

/* Pack planet positions, colors, sizes at a given Julian Day.
 * scene_scale: AU-to-scene-units conversion (e.g., 10.0 = 1 AU = 10 units).
 * radius_scale: planet radius multiplier for visibility. */
pp_planet_data_t pp_pack_planets(double jd, float scene_scale, float radius_scale);

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
```

## DONE WHEN

- [ ] All functions declared in .h and implemented in .c
- [ ] pp_pack_planets produces correct 3D positions from Keplerian orbital elements
- [ ] Planet colors from planet_data_color(), sizes from planet_data_scene_radius()
- [ ] Atmosphere thickness from atmo_get()
- [ ] pp_pack_trails samples orbit at N points, fades alpha by trail age
- [ ] Trail offsets allow per-planet draw calls
- [ ] 4 shader source strings valid GLSL ES 3.00
- [ ] >= 30 tests covering: planet packing, positions, colors, sizes, trail sampling, trail offsets, byte sizes, shaders, edge cases
- [ ] All tests pass with zero warnings
- [ ] Purity: no malloc, no globals, no side effects
- [ ] Compiles: `gcc -Wall -Wextra -Werror -std=c11 -pedantic`

## Constraints

- C11, `-Wall -Wextra -Werror -std=c11 -pedantic`
- `#define PI 3.14159265358979323846` (no M_PI)
- No malloc, no globals, no side effects
- Planet positions computed via orbit.h orbital elements
- Colors from planet_data.h — NASA physical data, not theme colors
- Orbit trails are polyline samples at uniform time intervals around one full orbit period
