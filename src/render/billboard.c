#include "billboard.h"

#include <math.h>

static void set_quad(billboard_quad_t *q, vec3_t center,
                     vec3_t right, vec3_t up,
                     float width, float height)
{
    vec3_t half_r = vec3_scale(right, width * 0.5f);
    vec3_t half_u = vec3_scale(up, height * 0.5f);

    /* 0=bottom-left, 1=bottom-right, 2=top-right, 3=top-left */
    vec3_t bl = vec3_sub(vec3_sub(center, half_r), half_u);
    vec3_t br = vec3_sub(vec3_add(center, half_r), half_u);
    vec3_t tr = vec3_add(vec3_add(center, half_r), half_u);
    vec3_t tl = vec3_add(vec3_sub(center, half_r), half_u);

    q->positions[0]  = bl.x; q->positions[1]  = bl.y; q->positions[2]  = bl.z;
    q->positions[3]  = br.x; q->positions[4]  = br.y; q->positions[5]  = br.z;
    q->positions[6]  = tr.x; q->positions[7]  = tr.y; q->positions[8]  = tr.z;
    q->positions[9]  = tl.x; q->positions[10] = tl.y; q->positions[11] = tl.z;

    q->uvs[0] = 0.0f; q->uvs[1] = 0.0f;
    q->uvs[2] = 1.0f; q->uvs[3] = 0.0f;
    q->uvs[4] = 1.0f; q->uvs[5] = 1.0f;
    q->uvs[6] = 0.0f; q->uvs[7] = 1.0f;

    q->indices[0] = 0; q->indices[1] = 1; q->indices[2] = 2;
    q->indices[3] = 0; q->indices[4] = 2; q->indices[5] = 3;
}

billboard_quad_t billboard_create(vec3_t center,
                                  vec3_t cam_right, vec3_t cam_up,
                                  float width, float height)
{
    billboard_quad_t q;
    vec3_t r = vec3_normalize(cam_right);
    vec3_t u = vec3_normalize(cam_up);
    set_quad(&q, center, r, u, width, height);
    return q;
}

billboard_quad_t billboard_create_y_locked(vec3_t center, vec3_t cam_pos,
                                           float width, float height)
{
    billboard_quad_t q;
    vec3_t to_cam = vec3_sub(cam_pos, center);
    to_cam.y = 0.0f; /* project to XZ plane */

    float len = vec3_length(to_cam);
    vec3_t right;
    if (len < 1e-8f) {
        right = vec3_create(1.0f, 0.0f, 0.0f);
    } else {
        vec3_t forward = vec3_scale(to_cam, 1.0f / len);
        right = vec3_cross(vec3_create(0.0f, 1.0f, 0.0f), forward);
        right = vec3_normalize(right);
    }

    vec3_t up = vec3_create(0.0f, 1.0f, 0.0f);
    set_quad(&q, center, right, up, width, height);
    return q;
}

void billboard_camera_vectors(const mat4_t *view,
                               vec3_t *out_right, vec3_t *out_up)
{
    /* View matrix row 0 = right, row 1 = up (column-major: m[col*4+row]) */
    out_right->x = view->m[0];
    out_right->y = view->m[4];
    out_right->z = view->m[8];

    out_up->x = view->m[1];
    out_up->y = view->m[5];
    out_up->z = view->m[9];
}

float billboard_size_for_pixels(float fov_radians, float distance,
                                 float screen_height, float desired_px)
{
    if (screen_height <= 0.0f || distance <= 0.0f) return 0.0f;
    float world_height = 2.0f * distance * tanf(fov_radians * 0.5f);
    return (desired_px / screen_height) * world_height;
}

void billboard_batch(const vec3_t *centers, int count,
                     vec3_t cam_right, vec3_t cam_up,
                     float width, float height,
                     float *out_positions, float *out_uvs,
                     unsigned int *out_indices)
{
    vec3_t r = vec3_normalize(cam_right);
    vec3_t u = vec3_normalize(cam_up);

    for (int i = 0; i < count; i++) {
        billboard_quad_t q;
        set_quad(&q, centers[i], r, u, width, height);

        int po = i * 12;
        int uo = i * 8;
        int io = i * 6;
        unsigned int vo = (unsigned int)(i * 4);

        for (int j = 0; j < 12; j++)
            out_positions[po + j] = q.positions[j];
        for (int j = 0; j < 8; j++)
            out_uvs[uo + j] = q.uvs[j];

        out_indices[io + 0] = vo + 0;
        out_indices[io + 1] = vo + 1;
        out_indices[io + 2] = vo + 2;
        out_indices[io + 3] = vo + 0;
        out_indices[io + 4] = vo + 2;
        out_indices[io + 5] = vo + 3;
    }
}
