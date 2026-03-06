#ifndef TIME_ASPECT_LINES_H
#define TIME_ASPECT_LINES_H

#define MAX_ASPECT_LINES 28  /* max pairs from 8 planets = 8*7/2 */

typedef struct {
    float x1, y1, z1;  /* start point (planet A position) */
    float x2, y2, z2;  /* end point (planet B position) */
    float r, g, b;      /* line color */
    float opacity;       /* line opacity (0.0-1.0, based on orb tightness) */
    int type;            /* aspect type: 0=conj, 1=opp, 2=trine, 3=square, 4=sextile */
} aspect_line_t;

typedef struct {
    aspect_line_t lines[MAX_ASPECT_LINES];
    int count;
} aspect_line_set_t;

/* Generate aspect lines from planet 3D positions and ecliptic longitudes.
 * positions: flat float array of num_planets*3 (x,y,z for each planet).
 * longitudes: num_planets-element double array (ecliptic longitude in degrees).
 * orb: maximum orb in degrees to consider an aspect active.
 * Returns set of lines with colors based on aspect type. */
aspect_line_set_t aspect_lines_generate(const float *positions,
                                        const double *longitudes,
                                        int num_planets,
                                        double orb);

/* Color for an aspect type. */
void aspect_line_color(int type, float *r, float *g, float *b);

/* Opacity from orb (tighter orb = more opaque). 0 orb = 1.0, max_orb = 0.2. */
float aspect_line_opacity(double orb_degrees, double max_orb);

#endif
