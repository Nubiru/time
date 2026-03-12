#ifndef TIME_SACRED_GEOMETRY_H
#define TIME_SACRED_GEOMETRY_H

#include "vec3.h"

/* Sacred constants */
#define PHI         1.6180339887498948482
#define PHI_INV     0.6180339887498948482
#define SQRT2       1.4142135623730950488
#define SQRT3       1.7320508075688772935
#define SQRT5       2.2360679774997896964

/* Fibonacci sequence (0-indexed: F(0)=0, F(1)=1, F(2)=1, ...) */
long long fibonacci(int n);
double fibonacci_ratio(int n);

/* Spiral paths */
typedef struct {
    vec3_t points[256];
    int count;
} spiral_path_t;

spiral_path_t golden_spiral(vec3_t center, float start_radius, float turns,
                             int num_points, int clockwise);
/* Regular polygons (inscribed in circle) */
typedef struct {
    vec3_t vertices[64];
    int count;
} polygon_t;

polygon_t regular_polygon(vec3_t center, float radius, int sides,
                           float rotation_deg);

/* Vesica Piscis */
typedef struct {
    vec3_t top;
    vec3_t bottom;
    float eye_width;
    float eye_height;
} vesica_piscis_t;

vesica_piscis_t vesica_piscis(vec3_t center_a, vec3_t center_b, float radius);

/* Vitruvian proportions */
typedef struct {
    float height;
    float navel_height;
    float arm_span;
    float circle_radius;
    float square_side;
    vec3_t circle_center;
    vec3_t square_center;
} vitruvian_t;

vitruvian_t vitruvian_proportions(float height, float base_y);

/* Golden rectangle */
typedef struct {
    vec3_t corners[4];
    float width;
    float height;
} golden_rect_t;

golden_rect_t golden_rectangle(vec3_t center, float width);

/* Golden section: divide segment at golden ratio point */
vec3_t golden_section_point(vec3_t a, vec3_t b);

/* Platonic solids */
typedef struct {
    int vertices;
    int edges;
    int faces;
    int face_sides;
    const char *name;
} platonic_info_t;

platonic_info_t platonic_solid(int index);

#endif
