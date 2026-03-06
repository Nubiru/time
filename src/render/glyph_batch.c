#include "glyph_batch.h"
#include <math.h>

void glyph_batch_uv(glyph_atlas_t atlas, int glyph_id,
                     float *u0, float *v0, float *u1, float *v1)
{
    if (glyph_id < atlas.first_id || glyph_id > atlas.last_id) {
        *u0 = 0.0f;
        *v0 = 0.0f;
        *u1 = 0.0f;
        *v1 = 0.0f;
        return;
    }

    int idx = glyph_id - atlas.first_id;
    int col = idx % atlas.cols;
    int row = idx / atlas.cols;

    float inv_cols = 1.0f / (float)atlas.cols;
    float inv_rows = 1.0f / (float)atlas.rows;

    *u0 = (float)col * inv_cols;
    *v0 = (float)row * inv_rows;
    *u1 = (float)(col + 1) * inv_cols;
    *v1 = (float)(row + 1) * inv_rows;
}

/* Write one vertex into the interleaved buffer */
static void write_vertex(float *vertices, int vertex_index,
                          float x, float y, float z,
                          float u, float v,
                          float r, float g, float b, float a)
{
    int off = vertex_index * GLYPH_VERTEX_STRIDE;
    vertices[off + 0] = x;
    vertices[off + 1] = y;
    vertices[off + 2] = z;
    vertices[off + 3] = u;
    vertices[off + 4] = v;
    vertices[off + 5] = r;
    vertices[off + 6] = g;
    vertices[off + 7] = b;
    vertices[off + 8] = a;
}

/* Build one glyph quad into the batch using given right/up vectors */
static void build_glyph(glyph_batch_t *batch, int glyph_index,
                         const glyph_instance_t *inst,
                         glyph_atlas_t atlas,
                         vec3_t right, vec3_t up,
                         float base_width, float base_height)
{
    float hw = (base_width * inst->scale) * 0.5f;
    float hh = (base_height * inst->scale) * 0.5f;

    vec3_t half_right = vec3_scale(right, hw);
    vec3_t half_up    = vec3_scale(up, hh);

    /* Compute UV from atlas */
    float u0, v0, u1, v1;
    glyph_batch_uv(atlas, inst->glyph_id, &u0, &v0, &u1, &v1);

    float cr = inst->color.r;
    float cg = inst->color.g;
    float cb = inst->color.b;
    float ca = inst->color.a;

    vec3_t p = inst->position;
    int base_v = glyph_index * 4;

    /* v0: bottom-left = center - right - up  -> UV: (u0, v1) */
    vec3_t bl = vec3_sub(vec3_sub(p, half_right), half_up);
    write_vertex(batch->vertices, base_v + 0,
                 bl.x, bl.y, bl.z, u0, v1, cr, cg, cb, ca);

    /* v1: bottom-right = center + right - up  -> UV: (u1, v1) */
    vec3_t br = vec3_sub(vec3_add(p, half_right), half_up);
    write_vertex(batch->vertices, base_v + 1,
                 br.x, br.y, br.z, u1, v1, cr, cg, cb, ca);

    /* v2: top-right = center + right + up  -> UV: (u1, v0) */
    vec3_t tr = vec3_add(vec3_add(p, half_right), half_up);
    write_vertex(batch->vertices, base_v + 2,
                 tr.x, tr.y, tr.z, u1, v0, cr, cg, cb, ca);

    /* v3: top-left = center - right + up  -> UV: (u0, v0) */
    vec3_t tl = vec3_add(vec3_sub(p, half_right), half_up);
    write_vertex(batch->vertices, base_v + 3,
                 tl.x, tl.y, tl.z, u0, v0, cr, cg, cb, ca);

    /* Indices: {b+0, b+1, b+2, b+2, b+3, b+0} */
    int idx_base = glyph_index * 6;
    unsigned int vb = (unsigned int)base_v;
    batch->indices[idx_base + 0] = vb + 0;
    batch->indices[idx_base + 1] = vb + 1;
    batch->indices[idx_base + 2] = vb + 2;
    batch->indices[idx_base + 3] = vb + 2;
    batch->indices[idx_base + 4] = vb + 3;
    batch->indices[idx_base + 5] = vb + 0;
}

glyph_batch_t glyph_batch_create(
    const glyph_instance_t *instances, int count,
    glyph_atlas_t atlas,
    vec3_t cam_right, vec3_t cam_up,
    float base_width, float base_height)
{
    glyph_batch_t batch;

    if (count < 0) count = 0;
    if (count > GLYPH_BATCH_MAX) count = GLYPH_BATCH_MAX;

    batch.glyph_count = count;
    batch.vertex_count = count * 4;
    batch.index_count = count * 6;

    for (int i = 0; i < count; i++) {
        build_glyph(&batch, i, &instances[i], atlas,
                     cam_right, cam_up, base_width, base_height);
    }

    return batch;
}

glyph_batch_t glyph_batch_create_y_locked(
    const glyph_instance_t *instances, int count,
    glyph_atlas_t atlas,
    vec3_t cam_pos,
    float base_width, float base_height)
{
    glyph_batch_t batch;

    if (count < 0) count = 0;
    if (count > GLYPH_BATCH_MAX) count = GLYPH_BATCH_MAX;

    batch.glyph_count = count;
    batch.vertex_count = count * 4;
    batch.index_count = count * 6;

    vec3_t y_up = vec3_create(0.0f, 1.0f, 0.0f);

    for (int i = 0; i < count; i++) {
        /* Compute per-glyph right vector from XZ plane projection */
        vec3_t diff = vec3_sub(instances[i].position, cam_pos);
        /* Project to XZ plane (zero out Y) */
        vec3_t forward_xz = vec3_create(diff.x, 0.0f, diff.z);
        float len = vec3_length(forward_xz);

        vec3_t right;
        if (len < 1e-8f) {
            /* Degenerate: glyph directly above/below camera.
             * Use zero right vector -> degenerate quad (all at center). */
            right = vec3_create(0.0f, 0.0f, 0.0f);
        } else {
            forward_xz = vec3_scale(forward_xz, 1.0f / len);
            /* right = cross(Y_up, forward_xz) */
            right = vec3_cross(y_up, forward_xz);
            right = vec3_normalize(right);
        }

        build_glyph(&batch, i, &instances[i], atlas,
                     right, y_up, base_width, base_height);
    }

    return batch;
}

void glyph_batch_read_vertex(const glyph_batch_t *batch, int vertex_index,
                              float *x, float *y, float *z,
                              float *u, float *v,
                              float *r, float *g, float *b, float *a)
{
    int off = vertex_index * GLYPH_VERTEX_STRIDE;
    *x = batch->vertices[off + 0];
    *y = batch->vertices[off + 1];
    *z = batch->vertices[off + 2];
    *u = batch->vertices[off + 3];
    *v = batch->vertices[off + 4];
    *r = batch->vertices[off + 5];
    *g = batch->vertices[off + 6];
    *b = batch->vertices[off + 7];
    *a = batch->vertices[off + 8];
}

int glyph_batch_vertex_floats(const glyph_batch_t *batch)
{
    return batch->vertex_count * GLYPH_VERTEX_STRIDE;
}

int glyph_batch_vertex_bytes(const glyph_batch_t *batch)
{
    return batch->vertex_count * GLYPH_VERTEX_STRIDE * (int)sizeof(float);
}

int glyph_batch_index_bytes(const glyph_batch_t *batch)
{
    return batch->index_count * (int)sizeof(unsigned int);
}
