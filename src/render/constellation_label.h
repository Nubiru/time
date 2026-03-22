/*
 * constellation_label.h — Constellation name label positions
 *
 * Computes label placement for all 88 IAU constellations by averaging
 * boundary vertices to find the centroid. Projects RA/Dec centroids to
 * Cartesian positions on the celestial sphere for 3D text rendering.
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 */

#ifndef TIME_CONSTELLATION_LABEL_H
#define TIME_CONSTELLATION_LABEL_H

#define CL_COUNT 88

/* A single constellation label: name, abbreviation, and 3D position */
typedef struct {
    const char *name;       /* full name (e.g., "Orion") */
    const char *abbr;       /* IAU 3-letter (e.g., "Ori") */
    float x, y, z;          /* position on celestial sphere */
    float ra_hours;         /* centroid RA (0-24h) */
    float dec_deg;          /* centroid Dec (-90 to +90) */
    int is_zodiac;          /* 1 if zodiac constellation */
} cl_label_t;

/* Compute all 88 labels with positions on a sphere of given radius.
 * Returns the count written (always 88 if buffer is large enough).
 * out must have space for at least CL_COUNT elements. */
int cl_compute(float sphere_radius, cl_label_t *out);

/* Get a single label by constellation index. Returns empty if invalid. */
cl_label_t cl_get(int index, float sphere_radius);

#endif /* TIME_CONSTELLATION_LABEL_H */
