#include "bezier.h"

#include <math.h>

#define MAX_SAMPLES 256

static vec3_t v3_lerp(vec3_t a, vec3_t b, float t)
{
    return vec3_add(vec3_scale(a, 1.0f - t), vec3_scale(b, t));
}

/* --- Evaluation --- */

vec3_t bezier_quad_eval(bezier_quad_t b, float t)
{
    float u = 1.0f - t;
    vec3_t r = vec3_scale(b.p0, u * u);
    r = vec3_add(r, vec3_scale(b.p1, 2.0f * u * t));
    r = vec3_add(r, vec3_scale(b.p2, t * t));
    return r;
}

vec3_t bezier_cubic_eval(bezier_cubic_t b, float t)
{
    float u = 1.0f - t;
    float uu = u * u;
    float tt = t * t;
    vec3_t r = vec3_scale(b.p0, uu * u);
    r = vec3_add(r, vec3_scale(b.p1, 3.0f * uu * t));
    r = vec3_add(r, vec3_scale(b.p2, 3.0f * u * tt));
    r = vec3_add(r, vec3_scale(b.p3, tt * t));
    return r;
}

/* --- Tangents --- */

vec3_t bezier_quad_tangent(bezier_quad_t b, float t)
{
    float u = 1.0f - t;
    vec3_t r = vec3_scale(vec3_sub(b.p1, b.p0), 2.0f * u);
    r = vec3_add(r, vec3_scale(vec3_sub(b.p2, b.p1), 2.0f * t));
    return r;
}

vec3_t bezier_cubic_tangent(bezier_cubic_t b, float t)
{
    float u = 1.0f - t;
    vec3_t r = vec3_scale(vec3_sub(b.p1, b.p0), 3.0f * u * u);
    r = vec3_add(r, vec3_scale(vec3_sub(b.p2, b.p1), 6.0f * u * t));
    r = vec3_add(r, vec3_scale(vec3_sub(b.p3, b.p2), 3.0f * t * t));
    return r;
}

/* --- Path sampling --- */

bezier_path_t bezier_quad_sample(bezier_quad_t b, int num_points)
{
    bezier_path_t path;
    if (num_points < 2) num_points = 2;
    if (num_points > 64) num_points = 64;
    path.count = num_points;
    path.total_length = 0.0f;

    float step = 1.0f / (float)(num_points - 1);
    for (int i = 0; i < num_points; i++) {
        path.points[i] = bezier_quad_eval(b, (float)i * step);
        if (i > 0)
            path.total_length += vec3_length(vec3_sub(path.points[i], path.points[i - 1]));
    }
    return path;
}

bezier_path_t bezier_cubic_sample(bezier_cubic_t b, int num_points)
{
    bezier_path_t path;
    if (num_points < 2) num_points = 2;
    if (num_points > 64) num_points = 64;
    path.count = num_points;
    path.total_length = 0.0f;

    float step = 1.0f / (float)(num_points - 1);
    for (int i = 0; i < num_points; i++) {
        path.points[i] = bezier_cubic_eval(b, (float)i * step);
        if (i > 0)
            path.total_length += vec3_length(vec3_sub(path.points[i], path.points[i - 1]));
    }
    return path;
}

/* --- Arc length --- */

float bezier_quad_length(bezier_quad_t b, int samples)
{
    if (samples < 2) samples = 2;
    if (samples > MAX_SAMPLES) samples = MAX_SAMPLES;
    float step = 1.0f / (float)(samples - 1);
    vec3_t prev = b.p0;
    float total = 0.0f;
    for (int i = 1; i < samples; i++) {
        vec3_t cur = bezier_quad_eval(b, (float)i * step);
        total += vec3_length(vec3_sub(cur, prev));
        prev = cur;
    }
    return total;
}

float bezier_cubic_length(bezier_cubic_t b, int samples)
{
    if (samples < 2) samples = 2;
    if (samples > MAX_SAMPLES) samples = MAX_SAMPLES;
    float step = 1.0f / (float)(samples - 1);
    vec3_t prev = b.p0;
    float total = 0.0f;
    for (int i = 1; i < samples; i++) {
        vec3_t cur = bezier_cubic_eval(b, (float)i * step);
        total += vec3_length(vec3_sub(cur, prev));
        prev = cur;
    }
    return total;
}

/* --- Arc-length parameterization --- */

vec3_t bezier_cubic_at_length(bezier_cubic_t b, float fraction, int samples)
{
    if (fraction <= 0.0f) return b.p0;
    if (fraction >= 1.0f) return b.p3;
    if (samples < 2) samples = 2;
    if (samples > MAX_SAMPLES) samples = MAX_SAMPLES;

    float cum[MAX_SAMPLES];
    float step = 1.0f / (float)(samples - 1);
    vec3_t prev = b.p0;
    cum[0] = 0.0f;

    for (int i = 1; i < samples; i++) {
        vec3_t cur = bezier_cubic_eval(b, (float)i * step);
        cum[i] = cum[i - 1] + vec3_length(vec3_sub(cur, prev));
        prev = cur;
    }

    float target = fraction * cum[samples - 1];

    for (int i = 1; i < samples; i++) {
        if (cum[i] >= target) {
            float seg = (cum[i] > cum[i - 1])
                ? (target - cum[i - 1]) / (cum[i] - cum[i - 1])
                : 0.0f;
            float t = ((float)(i - 1) + seg) * step;
            return bezier_cubic_eval(b, t);
        }
    }
    return b.p3;
}

/* --- Subdivision (De Casteljau) --- */

void bezier_cubic_split(bezier_cubic_t b, float t,
                        bezier_cubic_t *left, bezier_cubic_t *right)
{
    vec3_t q0 = v3_lerp(b.p0, b.p1, t);
    vec3_t q1 = v3_lerp(b.p1, b.p2, t);
    vec3_t q2 = v3_lerp(b.p2, b.p3, t);
    vec3_t r0 = v3_lerp(q0, q1, t);
    vec3_t r1 = v3_lerp(q1, q2, t);
    vec3_t s  = v3_lerp(r0, r1, t);

    left->p0 = b.p0;
    left->p1 = q0;
    left->p2 = r0;
    left->p3 = s;

    right->p0 = s;
    right->p1 = r1;
    right->p2 = q2;
    right->p3 = b.p3;
}

/* --- Construction --- */

bezier_cubic_t bezier_cubic_from_points(vec3_t start, vec3_t end,
                                        float curvature)
{
    bezier_cubic_t b;
    b.p0 = start;
    b.p3 = end;

    vec3_t dir = vec3_sub(end, start);
    vec3_t perp = vec3_create(-dir.y * curvature, dir.x * curvature, 0.0f);
    vec3_t third = vec3_scale(dir, 1.0f / 3.0f);

    b.p1 = vec3_add(vec3_add(start, third), perp);
    b.p2 = vec3_add(vec3_sub(end, third), perp);
    return b;
}
