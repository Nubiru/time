#include "glyph_batch.h"

int glyph_batch_stride(void)           { return GLYPH_BATCH_STRIDE; }
int glyph_batch_position_offset(void)  { return 0; }
int glyph_batch_uv_offset(void)        { return 3; }
int glyph_batch_color_offset(void)     { return 5; }

glyph_batch_info_t glyph_batch_size(int count)
{
    glyph_batch_info_t info;
    if (count < 0) count = 0;
    if (count > GLYPH_BATCH_MAX) count = GLYPH_BATCH_MAX;
    info.vertex_count = count * 4;
    info.index_count = count * 6;
    return info;
}

void glyph_batch_generate(const glyph_instance_t *instances, int count,
                           float cam_right_x, float cam_right_y, float cam_right_z,
                           float cam_up_x, float cam_up_y, float cam_up_z,
                           float *out_vertices, unsigned int *out_indices)
{
    if (count <= 0) return;
    if (count > GLYPH_BATCH_MAX) count = GLYPH_BATCH_MAX;

    for (int i = 0; i < count; i++) {
        const glyph_instance_t *g = &instances[i];
        float hw = g->width * 0.5f;
        float hh = g->height * 0.5f;

        /* right * half_width */
        float rx = cam_right_x * hw;
        float ry = cam_right_y * hw;
        float rz = cam_right_z * hw;

        /* up * half_height */
        float ux = cam_up_x * hh;
        float uy = cam_up_y * hh;
        float uz = cam_up_z * hh;

        int base_v = i * 4;
        int off;

        /* v0: bottom-left = center - right - up */
        off = base_v * GLYPH_BATCH_STRIDE;
        out_vertices[off + 0] = g->px - rx - ux;
        out_vertices[off + 1] = g->py - ry - uy;
        out_vertices[off + 2] = g->pz - rz - uz;
        out_vertices[off + 3] = g->u0;
        out_vertices[off + 4] = g->v0;
        out_vertices[off + 5] = g->r;
        out_vertices[off + 6] = g->g;
        out_vertices[off + 7] = g->b;
        out_vertices[off + 8] = g->a;

        /* v1: bottom-right = center + right - up */
        off = (base_v + 1) * GLYPH_BATCH_STRIDE;
        out_vertices[off + 0] = g->px + rx - ux;
        out_vertices[off + 1] = g->py + ry - uy;
        out_vertices[off + 2] = g->pz + rz - uz;
        out_vertices[off + 3] = g->u1;
        out_vertices[off + 4] = g->v0;
        out_vertices[off + 5] = g->r;
        out_vertices[off + 6] = g->g;
        out_vertices[off + 7] = g->b;
        out_vertices[off + 8] = g->a;

        /* v2: top-right = center + right + up */
        off = (base_v + 2) * GLYPH_BATCH_STRIDE;
        out_vertices[off + 0] = g->px + rx + ux;
        out_vertices[off + 1] = g->py + ry + uy;
        out_vertices[off + 2] = g->pz + rz + uz;
        out_vertices[off + 3] = g->u1;
        out_vertices[off + 4] = g->v1;
        out_vertices[off + 5] = g->r;
        out_vertices[off + 6] = g->g;
        out_vertices[off + 7] = g->b;
        out_vertices[off + 8] = g->a;

        /* v3: top-left = center - right + up */
        off = (base_v + 3) * GLYPH_BATCH_STRIDE;
        out_vertices[off + 0] = g->px - rx + ux;
        out_vertices[off + 1] = g->py - ry + uy;
        out_vertices[off + 2] = g->pz - rz + uz;
        out_vertices[off + 3] = g->u0;
        out_vertices[off + 4] = g->v1;
        out_vertices[off + 5] = g->r;
        out_vertices[off + 6] = g->g;
        out_vertices[off + 7] = g->b;
        out_vertices[off + 8] = g->a;

        /* Two triangles: 0,1,2 and 0,2,3 */
        int idx_base = i * 6;
        unsigned int vb = (unsigned int)base_v;
        out_indices[idx_base + 0] = vb;
        out_indices[idx_base + 1] = vb + 1;
        out_indices[idx_base + 2] = vb + 2;
        out_indices[idx_base + 3] = vb;
        out_indices[idx_base + 4] = vb + 2;
        out_indices[idx_base + 5] = vb + 3;
    }
}
