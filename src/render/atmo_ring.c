/* atmo_ring.c — Planet atmosphere & ring geometry implementation.
 *
 * Data sources:
 *   - NASA Planetary Fact Sheet (atmosphere composition, scale heights)
 *   - JPL Saturn ring parameters (Cassini Division, ring radii)
 *   - Fresnel equation: Schlick's approximation (Christophe Schlick, 1994)
 *
 * All functions are pure: no malloc, no globals, no side effects. */

#include "atmo_ring.h"
#include <math.h>

/* ---------- Atmosphere data per planet ---------- */

/* Planet atmosphere table indexed by planet (0=Mercury..7=Neptune).
 * thickness_ratio = approximate visible atmosphere extent / planet radius.
 * scatter color = dominant Rayleigh/Mie scatter hue.
 * density = relative atmospheric density (Earth = 1.0). */
static const atmo_properties_t ATMO_TABLE[8] = {
    /* Mercury: no atmosphere */
    { 0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0 },
    /* Venus: very dense CO2 atmosphere, yellowish scatter */
    { 0.08f, 0.95f, 0.85f, 0.55f, 0.9f,  1 },
    /* Earth: moderate N2/O2, blue Rayleigh scatter */
    { 0.04f, 0.4f,  0.6f,  0.95f, 1.0f,  1 },
    /* Mars: very thin CO2, reddish dust scatter */
    { 0.02f, 0.85f, 0.55f, 0.35f, 0.006f, 1 },
    /* Jupiter: very thick H2/He, white/tan bands */
    { 0.05f, 0.9f,  0.85f, 0.75f, 0.7f,  1 },
    /* Saturn: thick H2/He, pale gold */
    { 0.05f, 0.95f, 0.88f, 0.65f, 0.5f,  1 },
    /* Uranus: thick H2/He/CH4, cyan methane scatter */
    { 0.04f, 0.55f, 0.85f, 0.9f,  0.4f,  1 },
    /* Neptune: thick H2/He/CH4, deep blue methane */
    { 0.04f, 0.35f, 0.5f,  0.9f,  0.45f, 1 },
};

/* Null atmosphere for invalid indices */
static const atmo_properties_t ATMO_NONE = {
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0
};

atmo_properties_t atmo_get(int planet_index)
{
    if (planet_index < 0 || planet_index > 7)
        return ATMO_NONE;
    return ATMO_TABLE[planet_index];
}

/* ---------- Ring system data per planet ---------- */

/* Ring system table. Radii in planet radii.
 * Saturn: D-C-B-Cassini-A-F rings. Inner edge D ring ~1.239R,
 * outer edge F ring ~2.27R. Cassini Division at 1.95-2.02R.
 * Uranus: thin faint rings, inner ~1.64R, outer ~2.0R.
 * Neptune: thin faint rings, inner ~1.69R, outer ~2.54R. */
static const ring_system_t RING_TABLE[8] = {
    /* Mercury */ { 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f }, 0, 0 },
    /* Venus   */ { 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f }, 0, 0 },
    /* Earth   */ { 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f }, 0, 0 },
    /* Mars    */ { 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f }, 0, 0 },
    /* Jupiter */ { 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f }, 0, 0 },
    /* Saturn  */ { 1.239f, 2.27f, 0.8f,
                    { 1.95f, 2.02f, 0.05f }, 1, 1 },
    /* Uranus  */ { 1.64f, 2.0f, 0.15f,
                    { 0.0f, 0.0f, 0.0f }, 1, 0 },
    /* Neptune */ { 1.69f, 2.54f, 0.1f,
                    { 0.0f, 0.0f, 0.0f }, 1, 0 },
};

static const ring_system_t RING_NONE = {
    0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f }, 0, 0
};

ring_system_t ring_system_get(int planet_index)
{
    if (planet_index < 0 || planet_index > 7)
        return RING_NONE;
    return RING_TABLE[planet_index];
}

/* ---------- Annular ring geometry (shared logic) ---------- */

/* Generate an annular ring of vertices in the XZ plane (y=0).
 * Vertices alternate: inner[0], outer[0], inner[1], outer[1], ...
 * segments: number of quad strips around the circle.
 * Returns vertex count = (segments + 1) * 2. */
static int annular_ring_positions(float *positions,
                                  float inner_r, float outer_r,
                                  int segments)
{
    int verts = (segments + 1) * 2;
    for (int i = 0; i <= segments; i++) {
        float angle = (float)(2.0 * PI * (double)i / (double)segments);
        float c = cosf(angle);
        float s = sinf(angle);

        int inner_v = i * 2;
        int outer_v = i * 2 + 1;

        positions[inner_v * 3 + 0] = inner_r * c;
        positions[inner_v * 3 + 1] = 0.0f;
        positions[inner_v * 3 + 2] = inner_r * s;

        positions[outer_v * 3 + 0] = outer_r * c;
        positions[outer_v * 3 + 1] = 0.0f;
        positions[outer_v * 3 + 2] = outer_r * s;
    }
    return verts;
}

/* Generate triangle indices for an annular ring of quad strips.
 * Each quad = 2 triangles = 6 indices. Returns index count. */
static int annular_ring_indices(unsigned int *indices, int segments)
{
    int idx = 0;
    for (int i = 0; i < segments; i++) {
        unsigned int inner0 = (unsigned int)(i * 2);
        unsigned int outer0 = (unsigned int)(i * 2 + 1);
        unsigned int inner1 = (unsigned int)((i + 1) * 2);
        unsigned int outer1 = (unsigned int)((i + 1) * 2 + 1);

        /* Triangle 1: inner0, outer0, inner1 */
        indices[idx++] = inner0;
        indices[idx++] = outer0;
        indices[idx++] = inner1;

        /* Triangle 2: inner1, outer0, outer1 */
        indices[idx++] = inner1;
        indices[idx++] = outer0;
        indices[idx++] = outer1;
    }
    return idx;
}

/* ---------- Atmosphere shell ---------- */

int atmo_shell_positions(float *positions, float planet_radius,
                         float thickness_ratio)
{
    float outer_r = planet_radius * (1.0f + thickness_ratio);
    return annular_ring_positions(positions, planet_radius, outer_r,
                                  ATMO_RING_SEGMENTS);
}

int atmo_shell_indices(unsigned int *indices)
{
    return annular_ring_indices(indices, ATMO_RING_SEGMENTS);
}

int atmo_shell_colors(float *colors, atmo_properties_t atmo)
{
    int verts = ATMO_SHELL_VERTS;
    for (int i = 0; i <= ATMO_RING_SEGMENTS; i++) {
        int inner_v = i * 2;
        int outer_v = i * 2 + 1;

        /* Inner vertex: scatter color at full density alpha */
        colors[inner_v * 4 + 0] = atmo.scatter_r;
        colors[inner_v * 4 + 1] = atmo.scatter_g;
        colors[inner_v * 4 + 2] = atmo.scatter_b;
        colors[inner_v * 4 + 3] = atmo.density;

        /* Outer vertex: scatter color at zero alpha (fade out) */
        colors[outer_v * 4 + 0] = atmo.scatter_r;
        colors[outer_v * 4 + 1] = atmo.scatter_g;
        colors[outer_v * 4 + 2] = atmo.scatter_b;
        colors[outer_v * 4 + 3] = 0.0f;
    }
    return verts;
}

/* ---------- Ring quad ---------- */

int ring_quad_positions(float *positions, float inner_radius,
                        float outer_radius)
{
    return annular_ring_positions(positions, inner_radius, outer_radius,
                                  RING_QUAD_SEGMENTS);
}

int ring_quad_indices(unsigned int *indices)
{
    return annular_ring_indices(indices, RING_QUAD_SEGMENTS);
}

int ring_quad_colors(float *colors, ring_system_t ring,
                     float ring_r, float ring_g, float ring_b)
{
    int verts = RING_QUAD_VERTS;

    if (!ring.has_rings) {
        /* No rings: all zero alpha */
        for (int v = 0; v < verts; v++) {
            colors[v * 4 + 0] = 0.0f;
            colors[v * 4 + 1] = 0.0f;
            colors[v * 4 + 2] = 0.0f;
            colors[v * 4 + 3] = 0.0f;
        }
        return verts;
    }

    float range = ring.outer_radius - ring.inner_radius;
    /* Avoid division by zero if inner == outer */
    if (range < 0.0001f)
        range = 1.0f;

    for (int i = 0; i <= RING_QUAD_SEGMENTS; i++) {
        int inner_v = i * 2;
        int outer_v = i * 2 + 1;

        /* Inner vertex radius (in planet radii) = inner_radius */
        float r_inner = ring.inner_radius;
        /* Outer vertex radius = outer_radius */
        float r_outer = ring.outer_radius;

        /* Compute opacity for each vertex based on radial position */
        float alpha_inner = ring.opacity;
        float alpha_outer = ring.opacity;

        /* Apply Cassini Division gap: reduce opacity for vertices
         * whose radius falls within the gap region */
        if (ring.gap_count > 0) {
            if (r_inner >= ring.cassini.inner_radius &&
                r_inner <= ring.cassini.outer_radius) {
                alpha_inner = ring.cassini.opacity;
            }
            if (r_outer >= ring.cassini.inner_radius &&
                r_outer <= ring.cassini.outer_radius) {
                alpha_outer = ring.cassini.opacity;
            }
        }

        colors[inner_v * 4 + 0] = ring_r;
        colors[inner_v * 4 + 1] = ring_g;
        colors[inner_v * 4 + 2] = ring_b;
        colors[inner_v * 4 + 3] = alpha_inner;

        colors[outer_v * 4 + 0] = ring_r;
        colors[outer_v * 4 + 1] = ring_g;
        colors[outer_v * 4 + 2] = ring_b;
        colors[outer_v * 4 + 3] = alpha_outer;
    }
    return verts;
}

/* ---------- Fresnel ---------- */

/* Schlick's Fresnel approximation: f = (1 - dot)^power.
 * At dot=1 (face-on): f=0. At dot=0 (edge-on): f=1.
 * Source: Christophe Schlick, "An Inexpensive BRDF Model for
 * Physically-based Rendering" (1994). */
float atmo_fresnel(float dot_product, float power)
{
    float clamped = dot_product;
    if (clamped < 0.0f) clamped = 0.0f;
    if (clamped > 1.0f) clamped = 1.0f;
    return powf(1.0f - clamped, power);
}
