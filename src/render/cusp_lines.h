#ifndef TIME_CUSP_LINES_H
#define TIME_CUSP_LINES_H

#define MAX_CUSP_LINES 12

typedef struct {
    float x1, y1, z1;  /* inner point */
    float x2, y2, z2;  /* outer point */
    float thickness;     /* 1.0 normal, 2.0 for angular cusps */
    int house_index;     /* 0-11 */
    int is_angular;      /* 1 for ASC(0), IC(3), DSC(6), MC(9) */
} cusp_line_t;

typedef struct {
    cusp_line_t lines[MAX_CUSP_LINES];
    int count;
} cusp_line_set_t;

/* Generate 12 radial cusp lines on the ecliptic plane (y=0).
 * cusps: 12 house cusp longitudes in degrees.
 * inner_radius, outer_radius: ring bounds. */
cusp_line_set_t cusp_lines_generate(const double *cusps,
                                     float inner_radius, float outer_radius);

/* Check if a house index is angular (ASC/IC/DSC/MC). */
int cusp_is_angular(int house_index);

/* Get axis name for angular cusps.
 * Returns NULL for non-angular houses. */
const char *cusp_axis_name(int house_index);

#endif
