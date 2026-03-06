#ifndef TIME_ASTRO_WHEEL_LAYOUT_H
#define TIME_ASTRO_WHEEL_LAYOUT_H

#include "../../math/vec3.h"

typedef struct {
    vec3_t position;     /* center position of this element */
    float angle_deg;     /* angle in degrees (0-360) */
    float radius;        /* distance from center */
} wheel_point_t;

typedef struct {
    wheel_point_t sign_labels[12];     /* center of each sign's label area */
    wheel_point_t sign_starts[12];     /* start of each sign's arc (on outer ring) */
    wheel_point_t cusps[12];           /* house cusp markers */
    float inner_radius;
    float outer_radius;
    float label_radius;
} wheel_layout_t;

typedef struct {
    vec3_t position;     /* position on the wheel */
    float angle_deg;     /* ecliptic longitude mapped to wheel angle */
    int sign;            /* zodiac sign index 0-11 */
} planet_plot_t;

typedef struct {
    vec3_t start;        /* start point of aspect line */
    vec3_t end;          /* end point of aspect line */
    float angle_a;       /* angle of first planet */
    float angle_b;       /* angle of second planet */
} wheel_aspect_t;

/* Main layout — call once when wheel size changes */
wheel_layout_t wheel_layout_create(float inner_radius, float outer_radius);

/* Map ecliptic longitude to wheel position */
wheel_point_t wheel_point_at_longitude(float longitude_deg, float radius);

/* Place a planet on the wheel */
planet_plot_t wheel_plot_planet(float ecliptic_longitude, float radius);

/* Compute aspect line between two ecliptic longitudes */
wheel_aspect_t wheel_aspect_line(float lon_a, float lon_b, float radius);

/* Generate arc points for a sign segment.
 * Returns number of points written (min of num_points and max_out). */
int wheel_sign_arc(int sign, float radius, int num_points, vec3_t out[], int max_out);

/* Compute wheel angle from ecliptic longitude.
 * ascendant_deg < 0: standard mode (angle = longitude).
 * ascendant_deg >= 0: rotated mode (ascendant placed at 180 on wheel). */
float wheel_angle_from_longitude(float longitude_deg, float ascendant_deg);

#endif
