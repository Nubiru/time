#ifndef TIME_ARC_GEOMETRY_H
#define TIME_ARC_GEOMETRY_H

#define ARC_MAX_POINTS 512

typedef struct {
    float x, y;
} vec2_t;

typedef struct {
    vec2_t position;
    vec2_t uv;
} vertex2d_t;

typedef struct {
    vertex2d_t vertices[ARC_MAX_POINTS];
    int count;
} vertex_buffer_t;

typedef struct {
    vertex2d_t vertices[ARC_MAX_POINTS * 2];
    int count;
} ring_buffer_t;

typedef struct {
    ring_buffer_t segments[12];
} zodiac_ring_t;

typedef struct {
    vertex_buffer_t lines[12];
} cusp_lines_t;

/* Circles */
vertex_buffer_t arc_points(vec2_t center, float radius,
                           float start_deg, float end_deg, int num_points);
vertex_buffer_t circle_points(vec2_t center, float radius, int num_points);

/* Rings */
ring_buffer_t ring_segment(vec2_t center, float inner_radius, float outer_radius,
                           float start_deg, float end_deg, int num_segments);
ring_buffer_t ring_full(vec2_t center, float inner_radius, float outer_radius,
                        int num_segments);

/* Zodiac */
zodiac_ring_t zodiac_ring(vec2_t center, float inner_radius, float outer_radius,
                           int points_per_segment, float gap_deg);

/* Cusp lines */
vertex_buffer_t cusp_line(vec2_t center, float inner_radius, float outer_radius,
                          float angle_deg);
cusp_lines_t house_cusp_lines(vec2_t center, float inner_radius, float outer_radius,
                               const float angles_deg[12]);

/* Aspect lines */
vertex_buffer_t chord_line(vec2_t center, float radius,
                           float angle_a_deg, float angle_b_deg);

/* Tick marks */
vertex_buffer_t tick_marks(vec2_t center, float inner_radius, float outer_radius,
                           int num_ticks);

/* Utility */
vec2_t point_on_circle(vec2_t center, float radius, float angle_deg);
float arc_length(float radius, float start_deg, float end_deg);

#endif
