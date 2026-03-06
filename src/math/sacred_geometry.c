#include "sacred_geometry.h"

#include <math.h>

#define PI 3.14159265358979323846

/* --- Fibonacci --- */

long long fibonacci(int n)
{
    if (n <= 0) return 0;
    if (n > 92) n = 92;
    long long a = 0, b = 1;
    for (int i = 2; i <= n; i++) {
        long long c = a + b;
        a = b;
        b = c;
    }
    return b;
}

double fibonacci_ratio(int n)
{
    if (n <= 0) return 0.0;
    if (n > 92) n = 92;
    long long a = 0, b = 1;
    for (int i = 2; i <= n + 1; i++) {
        long long c = a + b;
        a = b;
        b = c;
    }
    /* a = F(n), b = F(n+1) */
    return (double)b / (double)a;
}

/* --- Golden Spiral (logarithmic, PHI growth per quarter turn) --- */

spiral_path_t golden_spiral(vec3_t center, float start_radius, float turns,
                             int num_points, int clockwise)
{
    spiral_path_t path;
    if (num_points < 2) num_points = 2;
    if (num_points > 256) num_points = 256;
    path.count = num_points;

    float total = turns * 2.0f * (float)PI;
    float b = logf((float)PHI) / ((float)PI / 2.0f);
    float sign = clockwise ? -1.0f : 1.0f;

    for (int i = 0; i < num_points; i++) {
        float t = (float)i / (float)(num_points - 1);
        float theta = t * total;
        float r = start_radius * expf(b * theta);
        path.points[i] = vec3_create(
            center.x + r * cosf(sign * theta),
            center.y + r * sinf(sign * theta),
            center.z
        );
    }
    return path;
}

/* --- Fibonacci Spiral (quarter-circle arcs in Fibonacci-sized squares) --- */

spiral_path_t fibonacci_spiral(vec3_t center, int n_squares, float scale,
                                int points_per_arc)
{
    spiral_path_t path;
    path.count = 0;
    if (n_squares < 1) n_squares = 1;
    if (points_per_arc < 2) points_per_arc = 2;

    float cx = 0.0f, cy = 0.0f;
    float angle_offset = (float)PI;

    for (int s = 0; s < n_squares; s++) {
        float r = (float)fibonacci(s + 1) * scale;
        int start_i = (s > 0) ? 1 : 0;

        for (int i = start_i; i < points_per_arc; i++) {
            if (path.count >= 256) break;
            float t = (float)i / (float)(points_per_arc - 1);
            float angle = angle_offset + t * ((float)PI / 2.0f);
            path.points[path.count++] = vec3_create(
                center.x + cx + r * cosf(angle),
                center.y + cy + r * sinf(angle),
                center.z
            );
        }
        if (path.count >= 256) break;

        /* Compute arc endpoint and shift center for next arc */
        float end_angle = angle_offset + (float)PI / 2.0f;
        float end_x = cx + r * cosf(end_angle);
        float end_y = cy + r * sinf(end_angle);

        float next_r = (float)fibonacci(s + 2) * scale;
        cx = end_x - next_r * cosf(end_angle);
        cy = end_y - next_r * sinf(end_angle);
        angle_offset = end_angle;
    }

    return path;
}

/* --- Regular Polygon --- */

polygon_t regular_polygon(vec3_t center, float radius, int sides,
                           float rotation_deg)
{
    polygon_t p;
    if (sides < 3) sides = 3;
    if (sides > 64) sides = 64;
    p.count = sides;

    float rot = rotation_deg * (float)PI / 180.0f;
    float step = 2.0f * (float)PI / (float)sides;

    for (int i = 0; i < sides; i++) {
        float angle = rot + (float)i * step;
        p.vertices[i] = vec3_create(
            center.x + radius * cosf(angle),
            center.y + radius * sinf(angle),
            center.z
        );
    }
    return p;
}

/* --- Vesica Piscis --- */

vesica_piscis_t vesica_piscis(vec3_t center_a, vec3_t center_b, float radius)
{
    vesica_piscis_t v;
    vec3_t diff = vec3_sub(center_b, center_a);
    float dist = vec3_length(diff);

    if (dist < 0.0001f || dist > 2.0f * radius) {
        vec3_t mid = vec3_scale(vec3_add(center_a, center_b), 0.5f);
        v.top = mid;
        v.bottom = mid;
        v.eye_width = 0.0f;
        v.eye_height = 0.0f;
        return v;
    }

    vec3_t mid = vec3_scale(vec3_add(center_a, center_b), 0.5f);
    float half_d = dist / 2.0f;
    float h = sqrtf(radius * radius - half_d * half_d);
    vec3_t perp = vec3_normalize(vec3_create(-diff.y, diff.x, 0.0f));

    v.top = vec3_add(mid, vec3_scale(perp, h));
    v.bottom = vec3_sub(mid, vec3_scale(perp, h));
    v.eye_height = 2.0f * h;
    v.eye_width = 2.0f * radius - dist;
    return v;
}

/* --- Vitruvian Proportions --- */

vitruvian_t vitruvian_proportions(float height, float base_y)
{
    vitruvian_t v;
    v.height = height;
    v.navel_height = height * (float)PHI_INV;
    v.arm_span = height;
    v.square_side = height;
    v.circle_radius = v.navel_height;
    v.circle_center = vec3_create(0.0f, base_y + v.navel_height, 0.0f);
    v.square_center = vec3_create(0.0f, base_y + height / 2.0f, 0.0f);
    return v;
}

/* --- Golden Rectangle --- */

golden_rect_t golden_rectangle(vec3_t center, float width)
{
    golden_rect_t r;
    r.width = width;
    r.height = width / (float)PHI;

    float hw = width / 2.0f;
    float hh = r.height / 2.0f;

    r.corners[0] = vec3_create(center.x - hw, center.y - hh, center.z);
    r.corners[1] = vec3_create(center.x + hw, center.y - hh, center.z);
    r.corners[2] = vec3_create(center.x + hw, center.y + hh, center.z);
    r.corners[3] = vec3_create(center.x - hw, center.y + hh, center.z);
    return r;
}

/* --- Golden Subdivision --- */

golden_subdivision_t golden_subdivide(golden_rect_t rect, int depth)
{
    golden_subdivision_t s;
    s.count = 0;
    if (depth < 1) return s;
    if (depth > 32) depth = 32;

    float left  = rect.corners[0].x;
    float bot   = rect.corners[0].y;
    float right = rect.corners[2].x;
    float top   = rect.corners[2].y;
    float z     = rect.corners[0].z;

    for (int i = 0; i < depth && s.count < 32; i++) {
        float w = right - left;
        float h = top - bot;
        float sq = (w > h) ? h : w;
        float cx = 0.0f, cy = 0.0f;

        switch (i % 4) {
        case 0:
            cx = left + sq / 2.0f;
            cy = bot + sq / 2.0f;
            left += sq;
            break;
        case 1:
            cx = left + sq / 2.0f;
            cy = bot + sq / 2.0f;
            bot += sq;
            break;
        case 2:
            cx = right - sq / 2.0f;
            cy = bot + sq / 2.0f;
            right -= sq;
            break;
        default:
            cx = left + sq / 2.0f;
            cy = top - sq / 2.0f;
            top -= sq;
            break;
        }

        s.centers[s.count] = vec3_create(cx, cy, z);
        s.sizes[s.count] = sq;
        s.count++;
    }
    return s;
}

/* --- Golden Section --- */

vec3_t golden_section_point(vec3_t a, vec3_t b)
{
    return vec3_add(a, vec3_scale(vec3_sub(b, a), (float)PHI_INV));
}

/* --- Platonic Solids --- */

platonic_info_t platonic_solid(int index)
{
    static const platonic_info_t SOLIDS[5] = {
        {4,  6,  4, 3, "Tetrahedron"},
        {8, 12,  6, 4, "Cube"},
        {6, 12,  8, 3, "Octahedron"},
        {20, 30, 12, 5, "Dodecahedron"},
        {12, 30, 20, 3, "Icosahedron"}
    };

    if (index < 0 || index > 4) {
        platonic_info_t inv = {0, 0, 0, 0, "?"};
        return inv;
    }
    return SOLIDS[index];
}
