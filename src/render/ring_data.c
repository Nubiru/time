#include "ring_data.h"

#include <math.h>

#define PI 3.14159265358979323846

int ring_data_stride(void)           { return RING_DATA_STRIDE; }
int ring_data_position_offset(void)  { return 0; }
int ring_data_color_offset(void)     { return 3; }
int ring_data_uv_offset(void)        { return 6; }

void ring_data_generate(float *out_vertices,
                         unsigned int *out_indices,
                         float inner_radius, float outer_radius,
                         int segments_per_sign,
                         const float *sign_colors)
{
    if (segments_per_sign < 1) segments_per_sign = 1;
    int total = 12 * segments_per_sign;
    float step = 2.0f * (float)PI / (float)total;

    for (int i = 0; i <= total; i++) {
        float angle = (float)i * step;
        float c = cosf(angle);
        float s = sinf(angle);
        float u = (float)i / (float)total;

        int sign = i / segments_per_sign;
        if (sign > 11) sign = 11;

        float cr = 1.0f, cg = 1.0f, cb = 1.0f;
        if (sign_colors) {
            cr = sign_colors[sign * 3 + 0];
            cg = sign_colors[sign * 3 + 1];
            cb = sign_colors[sign * 3 + 2];
        }

        /* Inner vertex */
        int vi = i * 2;
        int off = vi * RING_DATA_STRIDE;
        out_vertices[off + 0] = inner_radius * c;
        out_vertices[off + 1] = 0.0f;
        out_vertices[off + 2] = inner_radius * s;
        out_vertices[off + 3] = cr;
        out_vertices[off + 4] = cg;
        out_vertices[off + 5] = cb;
        out_vertices[off + 6] = u;
        out_vertices[off + 7] = 0.0f;

        /* Outer vertex */
        off = (vi + 1) * RING_DATA_STRIDE;
        out_vertices[off + 0] = outer_radius * c;
        out_vertices[off + 1] = 0.0f;
        out_vertices[off + 2] = outer_radius * s;
        out_vertices[off + 3] = cr;
        out_vertices[off + 4] = cg;
        out_vertices[off + 5] = cb;
        out_vertices[off + 6] = u;
        out_vertices[off + 7] = 1.0f;
    }

    /* Indices: two triangles per quad segment */
    for (int i = 0; i < total; i++) {
        unsigned int bl = (unsigned int)(i * 2);
        unsigned int tl = bl + 1;
        unsigned int br = (unsigned int)((i + 1) * 2);
        unsigned int tr = br + 1;
        int idx = i * 6;
        out_indices[idx + 0] = bl;
        out_indices[idx + 1] = tl;
        out_indices[idx + 2] = tr;
        out_indices[idx + 3] = bl;
        out_indices[idx + 4] = tr;
        out_indices[idx + 5] = br;
    }
}
