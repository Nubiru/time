/* atmo_ring.h — Planet atmosphere & ring geometry.
 * Pure module: per-planet atmosphere properties, ring system data,
 * and annular geometry generation for atmospheric limb shells and
 * planetary ring systems.
 *
 * Data sources: NASA Planetary Fact Sheet (atmosphere composition,
 * scale heights), JPL ring parameters. */

#ifndef TIME_ATMO_RING_H
#define TIME_ATMO_RING_H

#define PI 3.14159265358979323846

#define ATMO_RING_SEGMENTS 32
#define ATMO_SHELL_VERTS ((ATMO_RING_SEGMENTS + 1) * 2)
#define ATMO_SHELL_INDICES (ATMO_RING_SEGMENTS * 6)
#define RING_QUAD_SEGMENTS 64
#define RING_QUAD_VERTS ((RING_QUAD_SEGMENTS + 1) * 2)
#define RING_QUAD_INDICES (RING_QUAD_SEGMENTS * 6)

/* Atmosphere properties for a planet */
typedef struct {
    float thickness_ratio;  /* atmosphere height / planet radius (0 = no atmo) */
    float scatter_r;        /* Rayleigh scatter color R (0-1) */
    float scatter_g;        /* Rayleigh scatter color G (0-1) */
    float scatter_b;        /* Rayleigh scatter color B (0-1) */
    float density;          /* relative density (0-1, Earth=1.0) */
    int has_atmosphere;     /* 1 if planet has visible atmosphere */
} atmo_properties_t;

/* Saturn ring gap data */
typedef struct {
    float inner_radius;     /* gap inner edge in planet radii */
    float outer_radius;     /* gap outer edge in planet radii */
    float opacity;          /* ring opacity outside gap (0-1) */
} ring_gap_t;

/* Ring system data for a planet */
typedef struct {
    float inner_radius;     /* inner ring edge in planet radii */
    float outer_radius;     /* outer ring edge in planet radii */
    float opacity;          /* overall ring opacity */
    ring_gap_t cassini;     /* Cassini Division gap data */
    int has_rings;          /* 1 if planet has visible rings */
    int gap_count;          /* number of significant gaps */
} ring_system_t;

/* Get atmosphere properties for a planet (0-7). */
atmo_properties_t atmo_get(int planet_index);

/* Get ring system data for a planet (0-7). */
ring_system_t ring_system_get(int planet_index);

/* Generate atmosphere shell vertex positions (annular ring in XZ plane).
 * positions: float array of size ATMO_SHELL_VERTS * 3.
 * planet_radius: scene-space radius of the planet body.
 * Returns vertex count. */
int atmo_shell_positions(float *positions, float planet_radius,
                         float thickness_ratio);

/* Generate atmosphere shell triangle indices.
 * indices: unsigned int array of size ATMO_SHELL_INDICES.
 * Returns index count. */
int atmo_shell_indices(unsigned int *indices);

/* Generate atmosphere shell vertex colors (RGBA).
 * colors: float array of size ATMO_SHELL_VERTS * 4.
 * Inner ring: scatter color at full opacity. Outer ring: scatter color fading to 0.
 * Returns vertex count. */
int atmo_shell_colors(float *colors, atmo_properties_t atmo);

/* Generate ring quad vertex positions (annular ring in XZ plane).
 * positions: float array of size RING_QUAD_VERTS * 3.
 * inner_radius, outer_radius: scene-space ring extents.
 * Returns vertex count. */
int ring_quad_positions(float *positions, float inner_radius,
                        float outer_radius);

/* Generate ring quad triangle indices.
 * indices: unsigned int array of size RING_QUAD_INDICES.
 * Returns index count. */
int ring_quad_indices(unsigned int *indices);

/* Generate ring quad vertex colors with Cassini gap.
 * colors: float array of size RING_QUAD_VERTS * 4.
 * Ring color fades radially; gap region has zero opacity.
 * Returns vertex count. */
int ring_quad_colors(float *colors, ring_system_t ring,
                     float ring_r, float ring_g, float ring_b);

/* Fresnel limb brightening factor.
 * dot_product: dot(view_dir, surface_normal), range [0,1].
 * Returns brightness multiplier (1.0 at edges, 0.0 at center). */
float atmo_fresnel(float dot_product, float power);

#endif /* TIME_ATMO_RING_H */
