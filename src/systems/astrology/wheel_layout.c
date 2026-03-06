#include "wheel_layout.h"

#include <math.h>

#define PI 3.14159265358979323846

static float normalize_angle(float deg)
{
    deg = fmodf(deg, 360.0f);
    if (deg < 0.0f) deg += 360.0f;
    return deg;
}

static vec3_t point_on_circle(float angle_deg, float radius)
{
    float rad = angle_deg * (float)PI / 180.0f;
    return vec3_create(radius * cosf(rad), radius * sinf(rad), 0.0f);
}

wheel_layout_t wheel_layout_create(float inner_radius, float outer_radius)
{
    wheel_layout_t layout;
    layout.inner_radius = inner_radius;
    layout.outer_radius = outer_radius;
    layout.label_radius = (inner_radius + outer_radius) * 0.5f;

    for (int i = 0; i < 12; i++) {
        float start_angle = (float)i * 30.0f;
        float mid_angle = start_angle + 15.0f;

        layout.sign_labels[i].angle_deg = mid_angle;
        layout.sign_labels[i].radius = layout.label_radius;
        layout.sign_labels[i].position = point_on_circle(mid_angle, layout.label_radius);

        layout.sign_starts[i].angle_deg = start_angle;
        layout.sign_starts[i].radius = outer_radius;
        layout.sign_starts[i].position = point_on_circle(start_angle, outer_radius);

        layout.cusps[i].angle_deg = start_angle;
        layout.cusps[i].radius = outer_radius;
        layout.cusps[i].position = point_on_circle(start_angle, outer_radius);
    }
    return layout;
}

wheel_point_t wheel_point_at_longitude(float longitude_deg, float radius)
{
    wheel_point_t p;
    float angle = normalize_angle(longitude_deg);
    p.angle_deg = angle;
    p.radius = radius;
    p.position = point_on_circle(angle, radius);
    return p;
}

planet_plot_t wheel_plot_planet(float ecliptic_longitude, float radius)
{
    planet_plot_t p;
    float lon = normalize_angle(ecliptic_longitude);
    p.angle_deg = lon;
    p.position = point_on_circle(lon, radius);
    p.sign = (int)(lon / 30.0f) % 12;
    return p;
}

wheel_aspect_t wheel_aspect_line(float lon_a, float lon_b, float radius)
{
    wheel_aspect_t line;
    float a = normalize_angle(lon_a);
    float b = normalize_angle(lon_b);
    line.angle_a = a;
    line.angle_b = b;
    line.start = point_on_circle(a, radius);
    line.end = point_on_circle(b, radius);
    return line;
}

int wheel_sign_arc(int sign, float radius, int num_points, vec3_t out[], int max_out)
{
    if (num_points <= 0 || max_out <= 0 || sign < 0 || sign > 11)
        return 0;

    int count = num_points < max_out ? num_points : max_out;
    float start_deg = (float)sign * 30.0f;
    float end_deg = start_deg + 30.0f;

    for (int i = 0; i < count; i++) {
        float t = (count > 1) ? (float)i / (float)(count - 1) : 0.0f;
        float angle = start_deg + t * (end_deg - start_deg);
        out[i] = point_on_circle(angle, radius);
    }
    return count;
}

float wheel_angle_from_longitude(float longitude_deg, float ascendant_deg)
{
    if (ascendant_deg < 0.0f)
        return normalize_angle(longitude_deg);
    return normalize_angle(longitude_deg - ascendant_deg + 180.0f);
}
