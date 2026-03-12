/*
 * milkyway_pack.h -- Procedural Milky Way galaxy band vertex packing
 *
 * Generates a lat/lon mesh on the celestial sphere in galactic coordinates
 * with named galactic regions (bright clouds and dark rifts) modulating
 * brightness. Converts galactic (l, b) to equatorial (RA, Dec) then to
 * Cartesian XYZ on the celestial sphere. Includes noise-based GLSL shaders.
 *
 * Pure module: no GL calls, no malloc, no globals, no side effects.
 * EXCEPTION: static buffer for shader string concatenation (same as sun_shader.c).
 *
 * Data sources:
 *   - IAU galactic coordinate system (J2000)
 *   - Named galactic regions: Sagittarius, Cygnus, Scutum, etc.
 *   - noise_shader.h for procedural GLSL cloud texture
 */

#ifndef TIME_MILKYWAY_PACK_H
#define TIME_MILKYWAY_PACK_H

#ifndef PI
#define PI 3.14159265358979323846
#endif

typedef struct {
    float sphere_radius;
    float band_width_deg;       /* angular width (default: 30.0) */
    float core_brightness;      /* galactic center multiplier (default: 1.5) */
    float edge_falloff;         /* edge fade exponent (default: 2.0) */
    int   longitude_segments;   /* around galactic equator (default: 72) */
    int   latitude_segments;    /* across band width (default: 12) */
} mw_config_t;

mw_config_t mw_default_config(void);

#define MW_VERTEX_FLOATS 6
#define MW_MAX_VERTICES 2000

typedef struct {
    int vertex_count;
    int index_count;
    int triangle_count;
} mw_info_t;

mw_info_t mw_pack(mw_config_t config, float *out_verts, unsigned short *out_indices);
float mw_brightness_at(float l_deg, float b_deg);

typedef struct {
    const char *name;
    float l_center_deg;
    float b_center_deg;
    float angular_radius;
    float brightness;
} mw_region_t;

int mw_region_count(void);
mw_region_t mw_region_get(int index);

void mw_galactic_to_equatorial(float l_deg, float b_deg, float *ra_hours, float *dec_deg);

const char *mw_vert_source(void);
const char *mw_frag_source(void);

#endif /* TIME_MILKYWAY_PACK_H */
