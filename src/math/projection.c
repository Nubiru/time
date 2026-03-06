#include "projection.h"

#include <math.h>

static void mat4_mul_vec4(const mat4_t *m, const float v[4], float out[4])
{
    for (int r = 0; r < 4; r++) {
        out[r] = m->m[0 + r] * v[0] +
                 m->m[4 + r] * v[1] +
                 m->m[8 + r] * v[2] +
                 m->m[12 + r] * v[3];
    }
}

static int mat4_invert(const mat4_t *m, mat4_t *out)
{
    const float *a = m->m;
    float inv[16];

    inv[0] = a[5]*a[10]*a[15] - a[5]*a[11]*a[14] -
             a[9]*a[6]*a[15]  + a[9]*a[7]*a[14]  +
             a[13]*a[6]*a[11] - a[13]*a[7]*a[10];

    inv[4] = -a[4]*a[10]*a[15] + a[4]*a[11]*a[14] +
              a[8]*a[6]*a[15]  - a[8]*a[7]*a[14]  -
              a[12]*a[6]*a[11] + a[12]*a[7]*a[10];

    inv[8] = a[4]*a[9]*a[15]  - a[4]*a[11]*a[13] -
             a[8]*a[5]*a[15]  + a[8]*a[7]*a[13]  +
             a[12]*a[5]*a[11] - a[12]*a[7]*a[9];

    inv[12] = -a[4]*a[9]*a[14]  + a[4]*a[10]*a[13] +
               a[8]*a[5]*a[14]  - a[8]*a[6]*a[13]  -
               a[12]*a[5]*a[10] + a[12]*a[6]*a[9];

    float det = a[0]*inv[0] + a[1]*inv[4] + a[2]*inv[8] + a[3]*inv[12];
    if (fabsf(det) < 1e-10f) {
        *out = mat4_identity();
        return 0;
    }

    inv[1] = -a[1]*a[10]*a[15] + a[1]*a[11]*a[14] +
              a[9]*a[2]*a[15]  - a[9]*a[3]*a[14]  -
              a[13]*a[2]*a[11] + a[13]*a[3]*a[10];

    inv[5] = a[0]*a[10]*a[15]  - a[0]*a[11]*a[14] -
             a[8]*a[2]*a[15]   + a[8]*a[3]*a[14]  +
             a[12]*a[2]*a[11]  - a[12]*a[3]*a[10];

    inv[9] = -a[0]*a[9]*a[15] + a[0]*a[11]*a[13] +
              a[8]*a[1]*a[15]  - a[8]*a[3]*a[13]  -
              a[12]*a[1]*a[11] + a[12]*a[3]*a[9];

    inv[13] = a[0]*a[9]*a[14]  - a[0]*a[10]*a[13] -
              a[8]*a[1]*a[14]  + a[8]*a[2]*a[13]  +
              a[12]*a[1]*a[10] - a[12]*a[2]*a[9];

    inv[2] = a[1]*a[6]*a[15]   - a[1]*a[7]*a[14] -
             a[5]*a[2]*a[15]   + a[5]*a[3]*a[14] +
             a[13]*a[2]*a[7]   - a[13]*a[3]*a[6];

    inv[6] = -a[0]*a[6]*a[15]  + a[0]*a[7]*a[14] +
              a[4]*a[2]*a[15]  - a[4]*a[3]*a[14] -
              a[12]*a[2]*a[7]  + a[12]*a[3]*a[6];

    inv[10] = a[0]*a[5]*a[15]  - a[0]*a[7]*a[13] -
              a[4]*a[1]*a[15]  + a[4]*a[3]*a[13] +
              a[12]*a[1]*a[7]  - a[12]*a[3]*a[5];

    inv[14] = -a[0]*a[5]*a[14] + a[0]*a[6]*a[13] +
               a[4]*a[1]*a[14] - a[4]*a[2]*a[13] -
               a[12]*a[1]*a[6] + a[12]*a[2]*a[5];

    inv[3] = -a[1]*a[6]*a[11]  + a[1]*a[7]*a[10] +
              a[5]*a[2]*a[11]  - a[5]*a[3]*a[10] -
              a[9]*a[2]*a[7]   + a[9]*a[3]*a[6];

    inv[7] = a[0]*a[6]*a[11]   - a[0]*a[7]*a[10] -
             a[4]*a[2]*a[11]   + a[4]*a[3]*a[10] +
             a[8]*a[2]*a[7]    - a[8]*a[3]*a[6];

    inv[11] = -a[0]*a[5]*a[11] + a[0]*a[7]*a[9] +
               a[4]*a[1]*a[11] - a[4]*a[3]*a[9] -
               a[8]*a[1]*a[7]  + a[8]*a[3]*a[5];

    inv[15] = a[0]*a[5]*a[10]  - a[0]*a[6]*a[9] -
              a[4]*a[1]*a[10]  + a[4]*a[2]*a[9] +
              a[8]*a[1]*a[6]   - a[8]*a[2]*a[5];

    float inv_det = 1.0f / det;
    for (int i = 0; i < 16; i++)
        out->m[i] = inv[i] * inv_det;

    return 1;
}

screen_point_t project_to_screen(vec3_t world_pos,
                                  const mat4_t *view,
                                  const mat4_t *proj,
                                  float screen_w, float screen_h)
{
    screen_point_t result;
    mat4_t mvp = mat4_multiply(*proj, *view);
    float v[4] = {world_pos.x, world_pos.y, world_pos.z, 1.0f};
    float clip[4];
    mat4_mul_vec4(&mvp, v, clip);

    if (clip[3] <= 0.0f) {
        result.x = 0.0f;
        result.y = 0.0f;
        result.depth = 0.0f;
        result.visible = 0;
        return result;
    }

    float inv_w = 1.0f / clip[3];
    float ndc_x = clip[0] * inv_w;
    float ndc_y = clip[1] * inv_w;
    float ndc_z = clip[2] * inv_w;

    result.x = (ndc_x * 0.5f + 0.5f) * screen_w;
    result.y = (1.0f - (ndc_y * 0.5f + 0.5f)) * screen_h;
    result.depth = ndc_z * 0.5f + 0.5f;
    result.visible = 1;

    return result;
}

vec3_t unproject_ray(float screen_x, float screen_y,
                     const mat4_t *view, const mat4_t *proj,
                     float screen_w, float screen_h)
{
    if (screen_w <= 0.0f || screen_h <= 0.0f)
        return vec3_create(0.0f, 0.0f, -1.0f);

    float ndc_x = (screen_x / screen_w) * 2.0f - 1.0f;
    float ndc_y = 1.0f - (screen_y / screen_h) * 2.0f;

    float clip_near[4] = {ndc_x, ndc_y, -1.0f, 1.0f};
    float clip_far[4]  = {ndc_x, ndc_y,  1.0f, 1.0f};

    mat4_t mvp = mat4_multiply(*proj, *view);
    mat4_t inv;
    if (!mat4_invert(&mvp, &inv))
        return vec3_create(0.0f, 0.0f, -1.0f);

    float wn[4], wf[4];
    mat4_mul_vec4(&inv, clip_near, wn);
    mat4_mul_vec4(&inv, clip_far, wf);

    if (fabsf(wn[3]) < 1e-10f || fabsf(wf[3]) < 1e-10f)
        return vec3_create(0.0f, 0.0f, -1.0f);

    vec3_t near_pt = vec3_create(wn[0] / wn[3], wn[1] / wn[3], wn[2] / wn[3]);
    vec3_t far_pt  = vec3_create(wf[0] / wf[3], wf[1] / wf[3], wf[2] / wf[3]);

    return vec3_normalize(vec3_sub(far_pt, near_pt));
}

int point_near_screen(vec3_t world_pos,
                      float screen_x, float screen_y, float radius_px,
                      const mat4_t *view, const mat4_t *proj,
                      float screen_w, float screen_h)
{
    screen_point_t sp = project_to_screen(world_pos, view, proj, screen_w, screen_h);
    if (!sp.visible) return 0;

    float dx = sp.x - screen_x;
    float dy = sp.y - screen_y;
    return (dx * dx + dy * dy <= radius_px * radius_px) ? 1 : 0;
}

float screen_distance(vec3_t a, vec3_t b,
                      const mat4_t *view, const mat4_t *proj,
                      float screen_w, float screen_h)
{
    screen_point_t sa = project_to_screen(a, view, proj, screen_w, screen_h);
    screen_point_t sb = project_to_screen(b, view, proj, screen_w, screen_h);

    if (!sa.visible || !sb.visible) return -1.0f;

    float dx = sa.x - sb.x;
    float dy = sa.y - sb.y;
    return sqrtf(dx * dx + dy * dy);
}
