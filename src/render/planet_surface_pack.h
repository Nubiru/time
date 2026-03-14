/*
 * planet_surface_pack.h — Procedural planet surface shader pack
 *
 * GPU-ready UV-sphere mesh data + GLSL shader source strings for
 * per-planet procedural surface rendering. Each planet gets a unique
 * visual identity: gas giant banding, rocky noise surfaces, atmosphere
 * Fresnel limb glow.
 *
 * Pure module: no GL calls, no malloc, no globals, no side effects.
 * Exception: static char buffer for shader source concatenation.
 *
 * Data sources:
 *   - planet_data.h — NASA Planetary Fact Sheet (colors)
 *   - orbit.h — JPL Keplerian orbital elements
 *   - noise_shader.h — simplex noise / fbm GLSL library
 *   - DELTA research digests — procedural planet GLSL recipes
 */

#ifndef TIME_PLANET_SURFACE_PACK_H
#define TIME_PLANET_SURFACE_PACK_H

#define PSP_MAX_PLANETS 9
#define PSP_VERTEX_FLOATS 11   /* pos(3) + normal(3) + uv(2) + color(3) */
#define PSP_SPHERE_SEGMENTS 24 /* lat/lon resolution per planet sphere */

typedef enum {
    PSP_SUN = 0,
    PSP_MERCURY, PSP_VENUS, PSP_EARTH, PSP_MARS,
    PSP_JUPITER, PSP_SATURN, PSP_URANUS, PSP_NEPTUNE
} psp_planet_t;

typedef enum {
    PSP_TYPE_STAR = 0,
    PSP_TYPE_ROCKY,
    PSP_TYPE_GAS_GIANT,
    PSP_TYPE_ICE_GIANT
} psp_type_t;

typedef struct {
    float base_color[3];        /* primary surface color (linear RGB) */
    float band_color[3];        /* secondary band color (gas giants) */
    float atmosphere_color[3];  /* limb glow tint */
    float atmo_thickness;       /* atmosphere shell ratio (0 = none, 0.1 = thin) */
    float band_frequency;       /* latitude band count (gas giants: 8-12) */
    float band_amplitude;       /* band contrast (0 = solid, 1 = strong bands) */
    float noise_scale;          /* surface noise frequency */
    float noise_amplitude;      /* surface noise intensity */
    psp_type_t type;
} psp_config_t;

/* Get per-planet surface config from planet_data.h colors. */
psp_config_t psp_planet_config(psp_planet_t planet);

/* Pack a UV-sphere mesh for a single planet.
 * center: world position (vec3).
 * radius: scene-space radius.
 * segments: lat/lon resolution (clamped to >= 3).
 * config: planet visual config.
 * out_verts: PSP_VERTEX_FLOATS floats per vertex.
 * max_verts: buffer capacity in vertices.
 * Returns vertex count (triangle list). */
int psp_pack_sphere(float center[3], float radius, int segments,
                    psp_config_t config, float *out_verts, int max_verts);

/* Pack atmosphere shell (slightly larger sphere for Fresnel limb glow).
 * Returns 0 for planets with no atmosphere (atmo_thickness == 0).
 * Returns vertex count. */
int psp_pack_atmosphere(float center[3], float radius, psp_config_t config,
                        float *out_verts, int max_verts);

/* Pack all planets as spheres at current positions.
 * jd: Julian Day for position computation.
 * orbit_scale: sqrt distance scale factor (3.0f).
 * out_verts: output buffer.
 * max_verts: buffer capacity in vertices.
 * Returns total vertex count. */
int psp_pack_all(double jd, float orbit_scale, float *out_verts,
                 int max_verts);

/* Planet surface vertex shader source (GLSL ES 3.00). */
const char *psp_vert_source(void);

/* Planet surface fragment shader source (GLSL ES 3.00).
 * Per-planet procedural surface:
 * - Gas giants: latitude-stretched FBM banding
 * - Rocky planets: noise-based color variation
 * - Earth: day/night from sun direction uniform
 * - Atmosphere: Fresnel limb glow */
const char *psp_frag_source(void);

#endif /* TIME_PLANET_SURFACE_PACK_H */
