#include "wheel_layout.h"

#include <math.h>

#define PI 3.14159265358979323846

static float deg2rad(float d)
{
    return d * (float)(PI / 180.0);
}

static float normalize_deg(float d)
{
    d = fmodf(d, 360.0f);
    if (d < 0.0f) d += 360.0f;
    return d;
}

wheel_point_t wheel_position(float degrees, float radius)
{
    wheel_point_t p;
    float rad = deg2rad(degrees);
    p.x = radius * cosf(rad);
    p.y = radius * sinf(rad);
    return p;
}

wheel_point_t wheel_sign_center(int sign, float radius)
{
    if (sign < 0) sign = 0;
    if (sign > 11) sign = 11;
    float center_deg = (float)(sign * 30 + 15);
    return wheel_position(center_deg, radius);
}

wheel_point_t wheel_longitude(float ecl_lon_deg, float radius)
{
    return wheel_position(normalize_deg(ecl_lon_deg), radius);
}

wheel_segment_t wheel_sign_segment(int sign)
{
    wheel_segment_t seg;
    if (sign < 0) sign = 0;
    if (sign > 11) sign = 11;
    seg.start_deg = (float)(sign * 30);
    seg.end_deg = (float)(sign * 30 + 30);
    return seg;
}

int wheel_in_segment(float lon_deg, wheel_segment_t seg)
{
    float norm = normalize_deg(lon_deg);
    float start = normalize_deg(seg.start_deg);
    float end = normalize_deg(seg.end_deg);

    if (start < end) {
        return (norm >= start && norm < end) ? 1 : 0;
    }
    /* Wrapping segment (e.g. 350-10) */
    return (norm >= start || norm < end) ? 1 : 0;
}

float wheel_angular_distance(float deg_a, float deg_b)
{
    float diff = normalize_deg(deg_a) - normalize_deg(deg_b);
    if (diff < 0.0f) diff = -diff;
    if (diff > 180.0f) diff = 360.0f - diff;
    return diff;
}

void wheel_ring_points(float radius, int n, wheel_point_t *out)
{
    if (n <= 0 || !out) return;
    for (int i = 0; i < n; i++) {
        float deg = (float)i / (float)n * 360.0f;
        out[i] = wheel_position(deg, radius);
    }
}

float wheel_billboard_angle(float camera_azimuth_rad)
{
    return camera_azimuth_rad + (float)PI;
}
