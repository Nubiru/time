#ifndef TIME_BEZIER_H
#define TIME_BEZIER_H

#include "vec3.h"

typedef struct {
    vec3_t p0, p1, p2;           /* 3 control points */
} bezier_quad_t;

typedef struct {
    vec3_t p0, p1, p2, p3;      /* 4 control points */
} bezier_cubic_t;

typedef struct {
    vec3_t points[64];           /* sampled points along the curve */
    int count;                    /* number of points (2 to 64) */
    float total_length;          /* approximate arc length */
} bezier_path_t;

/* Evaluation */
vec3_t bezier_quad_eval(bezier_quad_t b, float t);
vec3_t bezier_cubic_eval(bezier_cubic_t b, float t);

/* Derivatives (tangent vectors) */
vec3_t bezier_quad_tangent(bezier_quad_t b, float t);
vec3_t bezier_cubic_tangent(bezier_cubic_t b, float t);

/* Path sampling */
bezier_path_t bezier_quad_sample(bezier_quad_t b, int num_points);
bezier_path_t bezier_cubic_sample(bezier_cubic_t b, int num_points);

/* Arc length (approximate by sampling) */
float bezier_quad_length(bezier_quad_t b, int samples);
float bezier_cubic_length(bezier_cubic_t b, int samples);

/* Point at arc-length fraction (uniform-speed parameterization) */
vec3_t bezier_cubic_at_length(bezier_cubic_t b, float fraction, int samples);

/* Subdivision — split a cubic at parameter t into two cubics */
void bezier_cubic_split(bezier_cubic_t b, float t,
                        bezier_cubic_t *left, bezier_cubic_t *right);

/* Construction — auto control points from start/end with curvature */
bezier_cubic_t bezier_cubic_from_points(vec3_t start, vec3_t end,
                                        float curvature);

#endif
