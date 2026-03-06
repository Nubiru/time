#include "ring_geometry.h"

#include <math.h>

#define PI 3.14159265358979323846

ring_mesh_info_t ring_mesh_size(int segments_per_sign)
{
    ring_mesh_info_t info;
    if (segments_per_sign < 1) segments_per_sign = 1;
    int total = 12 * segments_per_sign;
    info.vertex_count = (total + 1) * 2;
    info.index_count = total * 6;
    return info;
}

void ring_generate_positions(float *positions,
                             float inner_radius, float outer_radius,
                             int segments_per_sign)
{
    if (segments_per_sign < 1) segments_per_sign = 1;
    int total = 12 * segments_per_sign;
    float step = 2.0f * (float)PI / (float)total;

    for (int i = 0; i <= total; i++) {
        float angle = (float)i * step;
        float c = cosf(angle);
        float s = sinf(angle);
        int base = i * 2;

        positions[base * 3 + 0] = inner_radius * c;
        positions[base * 3 + 1] = 0.0f;
        positions[base * 3 + 2] = inner_radius * s;

        positions[(base + 1) * 3 + 0] = outer_radius * c;
        positions[(base + 1) * 3 + 1] = 0.0f;
        positions[(base + 1) * 3 + 2] = outer_radius * s;
    }
}

void ring_generate_uvs(float *uvs, int segments_per_sign)
{
    if (segments_per_sign < 1) segments_per_sign = 1;
    int total = 12 * segments_per_sign;

    for (int i = 0; i <= total; i++) {
        float u = (float)i / (float)total;
        int base = i * 2;

        uvs[base * 2 + 0] = u;
        uvs[base * 2 + 1] = 0.0f;

        uvs[(base + 1) * 2 + 0] = u;
        uvs[(base + 1) * 2 + 1] = 1.0f;
    }
}

void ring_generate_indices(unsigned int *indices, int segments_per_sign)
{
    if (segments_per_sign < 1) segments_per_sign = 1;
    int total = 12 * segments_per_sign;

    for (int i = 0; i < total; i++) {
        unsigned int bl = (unsigned int)(i * 2);
        unsigned int tl = bl + 1;
        unsigned int br = (unsigned int)((i + 1) * 2);
        unsigned int tr = br + 1;
        int off = i * 6;

        indices[off + 0] = bl;
        indices[off + 1] = tl;
        indices[off + 2] = tr;
        indices[off + 3] = bl;
        indices[off + 4] = tr;
        indices[off + 5] = br;
    }
}

int ring_vertex_sign(int vertex_index, int segments_per_sign)
{
    if (segments_per_sign < 1) segments_per_sign = 1;
    int column = vertex_index / 2;
    int sign = column / segments_per_sign;
    if (sign > 11) sign = 11;
    return sign;
}

float ring_vertex_angle(int vertex_index, int segments_per_sign)
{
    if (segments_per_sign < 1) segments_per_sign = 1;
    int total = 12 * segments_per_sign;
    int column = vertex_index / 2;
    return (float)column * 360.0f / (float)total;
}
