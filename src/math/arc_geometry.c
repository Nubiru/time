#include "arc_geometry.h"
#include <math.h>

#define PI 3.14159265358979323846

static float deg_to_rad(float deg) {
    return deg * (float)(PI / 180.0);
}

/* --- Utility --- */

vec2_t point_on_circle(vec2_t center, float radius, float angle_deg) {
    float rad = deg_to_rad(angle_deg);
    vec2_t p;
    p.x = center.x + radius * cosf(rad);
    p.y = center.y + radius * sinf(rad);
    return p;
}

float arc_length(float radius, float start_deg, float end_deg) {
    float span = fabsf(end_deg - start_deg);
    return radius * deg_to_rad(span);
}

/* --- Circles --- */

vertex_buffer_t arc_points(vec2_t center, float radius,
                           float start_deg, float end_deg, int num_points) {
    vertex_buffer_t buf;
    buf.count = 0;

    if (num_points < 2) num_points = 2;
    if (num_points > ARC_MAX_POINTS) num_points = ARC_MAX_POINTS;

    float span = end_deg - start_deg;
    /* Handle wrap: if span is negative, go the long way (e.g., 350 to 10 = 20 degrees) */
    if (span <= 0.0f) span += 360.0f;

    for (int i = 0; i < num_points; i++) {
        float t = (float)i / (float)(num_points - 1);
        float angle = start_deg + span * t;
        vec2_t p = point_on_circle(center, radius, angle);

        buf.vertices[buf.count].position = p;
        buf.vertices[buf.count].uv.x = t;
        buf.vertices[buf.count].uv.y = 0.0f;
        buf.count++;
    }

    return buf;
}

vertex_buffer_t circle_points(vec2_t center, float radius, int num_points) {
    return arc_points(center, radius, 0.0f, 360.0f, num_points);
}

/* --- Rings --- */

ring_buffer_t ring_segment(vec2_t center, float inner_radius, float outer_radius,
                           float start_deg, float end_deg, int num_segments) {
    ring_buffer_t buf;
    buf.count = 0;

    if (num_segments < 1) num_segments = 1;
    int num_pairs = num_segments + 1;
    if (num_pairs > ARC_MAX_POINTS) num_pairs = ARC_MAX_POINTS;

    float span = end_deg - start_deg;
    if (span <= 0.0f) span += 360.0f;

    for (int i = 0; i < num_pairs; i++) {
        float t = (float)i / (float)(num_pairs - 1);
        float angle = start_deg + span * t;

        /* Inner vertex */
        vec2_t pi = point_on_circle(center, inner_radius, angle);
        buf.vertices[buf.count].position = pi;
        buf.vertices[buf.count].uv.x = t;
        buf.vertices[buf.count].uv.y = 0.0f;
        buf.count++;

        /* Outer vertex */
        vec2_t po = point_on_circle(center, outer_radius, angle);
        buf.vertices[buf.count].position = po;
        buf.vertices[buf.count].uv.x = t;
        buf.vertices[buf.count].uv.y = 1.0f;
        buf.count++;
    }

    return buf;
}

ring_buffer_t ring_full(vec2_t center, float inner_radius, float outer_radius,
                        int num_segments) {
    return ring_segment(center, inner_radius, outer_radius,
                        0.0f, 360.0f, num_segments);
}

/* --- Zodiac --- */

zodiac_ring_t zodiac_ring(vec2_t center, float inner_radius, float outer_radius,
                           int points_per_segment, float gap_deg) {
    zodiac_ring_t zr;
    float seg_span = 30.0f - gap_deg;

    for (int i = 0; i < 12; i++) {
        float start = (float)i * 30.0f + gap_deg / 2.0f;
        float end = start + seg_span;
        zr.segments[i] = ring_segment(center, inner_radius, outer_radius,
                                       start, end, points_per_segment);
    }

    return zr;
}

/* --- Cusp lines --- */

vertex_buffer_t cusp_line(vec2_t center, float inner_radius, float outer_radius,
                          float angle_deg) {
    vertex_buffer_t buf;
    buf.count = 0;

    vec2_t inner = point_on_circle(center, inner_radius, angle_deg);
    vec2_t outer = point_on_circle(center, outer_radius, angle_deg);

    buf.vertices[0].position = inner;
    buf.vertices[0].uv.x = 0.0f;
    buf.vertices[0].uv.y = 0.0f;

    buf.vertices[1].position = outer;
    buf.vertices[1].uv.x = 1.0f;
    buf.vertices[1].uv.y = 0.0f;

    buf.count = 2;
    return buf;
}

cusp_lines_t house_cusp_lines(vec2_t center, float inner_radius, float outer_radius,
                               const float angles_deg[12]) {
    cusp_lines_t cl;
    for (int i = 0; i < 12; i++) {
        cl.lines[i] = cusp_line(center, inner_radius, outer_radius, angles_deg[i]);
    }
    return cl;
}

/* --- Aspect lines --- */

vertex_buffer_t chord_line(vec2_t center, float radius,
                           float angle_a_deg, float angle_b_deg) {
    vertex_buffer_t buf;
    buf.count = 0;

    vec2_t a = point_on_circle(center, radius, angle_a_deg);
    vec2_t b = point_on_circle(center, radius, angle_b_deg);

    buf.vertices[0].position = a;
    buf.vertices[0].uv.x = 0.0f;
    buf.vertices[0].uv.y = 0.0f;

    buf.vertices[1].position = b;
    buf.vertices[1].uv.x = 1.0f;
    buf.vertices[1].uv.y = 0.0f;

    buf.count = 2;
    return buf;
}

/* --- Tick marks --- */

vertex_buffer_t tick_marks(vec2_t center, float inner_radius, float outer_radius,
                           int num_ticks) {
    vertex_buffer_t buf;
    buf.count = 0;

    if (num_ticks < 1) return buf;
    int max_ticks = ARC_MAX_POINTS / 2;
    if (num_ticks > max_ticks) num_ticks = max_ticks;

    float step = 360.0f / (float)num_ticks;

    for (int i = 0; i < num_ticks; i++) {
        float angle = (float)i * step;
        vec2_t inner = point_on_circle(center, inner_radius, angle);
        vec2_t outer = point_on_circle(center, outer_radius, angle);

        buf.vertices[buf.count].position = inner;
        buf.vertices[buf.count].uv.x = 0.0f;
        buf.vertices[buf.count].uv.y = 0.0f;
        buf.count++;

        buf.vertices[buf.count].position = outer;
        buf.vertices[buf.count].uv.x = 1.0f;
        buf.vertices[buf.count].uv.y = 0.0f;
        buf.count++;
    }

    return buf;
}
