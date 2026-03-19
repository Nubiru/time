#include "concentric_ring.h"

#include <math.h>
#include <stddef.h>

#define CR_PI 3.14159265358979323846

/* Ring width: constant for all rings = innermost_radius * 0.15 */
static float cr_width(const cr_layout_t *layout)
{
    return layout->innermost_radius * 0.15f;
}

cr_layout_t cr_default_layout(void)
{
    cr_layout_t layout;
    layout.innermost_radius = 5.0f;
    layout.ring_gap = 0.3f;
    layout.ring_count = 3;

    /* Ring 0: zodiac (12 segments) */
    layout.rings[0].segment_count = 12;
    layout.rings[0].highlight_index = -1;
    layout.rings[0].highlight_boost = 1.5f;
    layout.rings[0].segment_colors = NULL;
    layout.rings[0].uniform_color[0] = 0.8f;
    layout.rings[0].uniform_color[1] = 0.7f;
    layout.rings[0].uniform_color[2] = 0.3f;
    layout.rings[0].base_alpha = 0.6f;

    /* Ring 1: I Ching (64 segments) */
    layout.rings[1].segment_count = 64;
    layout.rings[1].highlight_index = -1;
    layout.rings[1].highlight_boost = 1.5f;
    layout.rings[1].segment_colors = NULL;
    layout.rings[1].uniform_color[0] = 0.3f;
    layout.rings[1].uniform_color[1] = 0.6f;
    layout.rings[1].uniform_color[2] = 0.8f;
    layout.rings[1].base_alpha = 0.5f;

    /* Ring 2: Tzolkin kin (260 segments) */
    layout.rings[2].segment_count = 260;
    layout.rings[2].highlight_index = -1;
    layout.rings[2].highlight_boost = 1.5f;
    layout.rings[2].segment_colors = NULL;
    layout.rings[2].uniform_color[0] = 0.6f;
    layout.rings[2].uniform_color[1] = 0.3f;
    layout.rings[2].uniform_color[2] = 0.7f;
    layout.rings[2].base_alpha = 0.4f;

    /* Zero out unused rings */
    for (int i = 3; i < CR_MAX_RINGS; i++) {
        layout.rings[i].segment_count = 0;
        layout.rings[i].highlight_index = -1;
        layout.rings[i].highlight_boost = 1.0f;
        layout.rings[i].segment_colors = NULL;
        layout.rings[i].uniform_color[0] = 0.0f;
        layout.rings[i].uniform_color[1] = 0.0f;
        layout.rings[i].uniform_color[2] = 0.0f;
        layout.rings[i].base_alpha = 0.0f;
    }

    return layout;
}

float cr_ring_inner(const cr_layout_t *layout, int ring_index)
{
    if (ring_index < 0) ring_index = 0;
    float w = cr_width(layout);
    return layout->innermost_radius + (float)ring_index * (w + layout->ring_gap);
}

float cr_ring_outer(const cr_layout_t *layout, int ring_index)
{
    return cr_ring_inner(layout, ring_index) + cr_width(layout);
}

float cr_ring_width(const cr_layout_t *layout, int ring_index)
{
    (void)ring_index;
    return cr_width(layout);
}

int cr_total_verts(const cr_layout_t *layout)
{
    int total = 0;
    int count = layout->ring_count;
    if (count < 0) count = 0;
    if (count > CR_MAX_RINGS) count = CR_MAX_RINGS;
    for (int r = 0; r < count; r++) {
        int segs = layout->rings[r].segment_count;
        if (segs < 1) segs = 1;
        if (segs > CR_MAX_SEGMENTS) segs = CR_MAX_SEGMENTS;
        total += (segs + 1) * 2;
    }
    return total;
}

int cr_total_indices(const cr_layout_t *layout)
{
    int total = 0;
    int count = layout->ring_count;
    if (count < 0) count = 0;
    if (count > CR_MAX_RINGS) count = CR_MAX_RINGS;
    for (int r = 0; r < count; r++) {
        int segs = layout->rings[r].segment_count;
        if (segs < 1) segs = 1;
        if (segs > CR_MAX_SEGMENTS) segs = CR_MAX_SEGMENTS;
        total += segs * 6;
    }
    return total;
}

cr_info_t cr_generate(const cr_layout_t *layout,
                       float *out_verts,
                       unsigned int *out_indices)
{
    cr_info_t info;
    info.total_vertices = 0;
    info.total_indices = 0;

    int count = layout->ring_count;
    if (count < 0) count = 0;
    if (count > CR_MAX_RINGS) count = CR_MAX_RINGS;

    /* Zero out all per-ring info */
    for (int r = 0; r < CR_MAX_RINGS; r++) {
        info.ring_vert_offset[r] = 0;
        info.ring_vert_count[r] = 0;
        info.ring_idx_offset[r] = 0;
        info.ring_idx_count[r] = 0;
    }

    int vert_cursor = 0;
    int idx_cursor = 0;

    for (int r = 0; r < count; r++) {
        const cr_ring_def_t *ring = &layout->rings[r];
        int segs = ring->segment_count;
        if (segs < 1) segs = 1;
        if (segs > CR_MAX_SEGMENTS) segs = CR_MAX_SEGMENTS;

        float inner_r = cr_ring_inner(layout, r);
        float outer_r = cr_ring_outer(layout, r);
        float step = 2.0f * (float)CR_PI / (float)segs;

        int ring_verts = (segs + 1) * 2;
        int ring_indices = segs * 6;

        info.ring_vert_offset[r] = vert_cursor;
        info.ring_vert_count[r] = ring_verts;
        info.ring_idx_offset[r] = idx_cursor;
        info.ring_idx_count[r] = ring_indices;

        /* Generate vertices: for each column i (0..segs), emit inner then outer */
        for (int i = 0; i <= segs; i++) {
            float angle = (float)i * step;
            float c = cosf(angle);
            float s = sinf(angle);

            /* Determine which segment this column belongs to.
             * Column i is the LEFT edge of segment i (for i < segs).
             * Column segs wraps back to segment 0, but we color it as segment segs-1
             * to close the ring properly. */
            int seg_idx = (i < segs) ? i : segs - 1;

            /* Look up color */
            float cr, cg, cb;
            if (ring->segment_colors != NULL) {
                cr = ring->segment_colors[seg_idx * 3 + 0];
                cg = ring->segment_colors[seg_idx * 3 + 1];
                cb = ring->segment_colors[seg_idx * 3 + 2];
            } else {
                cr = ring->uniform_color[0];
                cg = ring->uniform_color[1];
                cb = ring->uniform_color[2];
            }

            /* Alpha: base, boosted if highlighted */
            float alpha = ring->base_alpha;
            if (seg_idx == ring->highlight_index) {
                alpha *= ring->highlight_boost;
            }

            int base = vert_cursor + i * 2;

            /* Inner vertex */
            out_verts[base * CR_VERT_FLOATS + 0] = inner_r * c;
            out_verts[base * CR_VERT_FLOATS + 1] = 0.0f;
            out_verts[base * CR_VERT_FLOATS + 2] = inner_r * s;
            out_verts[base * CR_VERT_FLOATS + 3] = cr;
            out_verts[base * CR_VERT_FLOATS + 4] = cg;
            out_verts[base * CR_VERT_FLOATS + 5] = cb;
            out_verts[base * CR_VERT_FLOATS + 6] = alpha;

            /* Outer vertex */
            out_verts[(base + 1) * CR_VERT_FLOATS + 0] = outer_r * c;
            out_verts[(base + 1) * CR_VERT_FLOATS + 1] = 0.0f;
            out_verts[(base + 1) * CR_VERT_FLOATS + 2] = outer_r * s;
            out_verts[(base + 1) * CR_VERT_FLOATS + 3] = cr;
            out_verts[(base + 1) * CR_VERT_FLOATS + 4] = cg;
            out_verts[(base + 1) * CR_VERT_FLOATS + 5] = cb;
            out_verts[(base + 1) * CR_VERT_FLOATS + 6] = alpha;
        }

        /* Generate indices: for each segment, two triangles */
        for (int i = 0; i < segs; i++) {
            unsigned int bl = (unsigned int)(vert_cursor + i * 2);       /* bottom-left (inner) */
            unsigned int tl = bl + 1;                                     /* top-left (outer) */
            unsigned int br = (unsigned int)(vert_cursor + (i + 1) * 2); /* bottom-right (inner) */
            unsigned int tr = br + 1;                                     /* top-right (outer) */

            int off = idx_cursor + i * 6;
            out_indices[off + 0] = bl;
            out_indices[off + 1] = tl;
            out_indices[off + 2] = tr;
            out_indices[off + 3] = bl;
            out_indices[off + 4] = tr;
            out_indices[off + 5] = br;
        }

        vert_cursor += ring_verts;
        idx_cursor += ring_indices;
    }

    info.total_vertices = vert_cursor;
    info.total_indices = idx_cursor;

    return info;
}

int cr_segment_at_angle(int segment_count, float angle_deg)
{
    if (segment_count < 1) return 0;
    float seg_width = 360.0f / (float)segment_count;
    int idx = (int)(angle_deg / seg_width);
    if (idx < 0) idx = 0;
    if (idx >= segment_count) idx = segment_count - 1;
    return idx;
}

const char *cr_vert_source(void)
{
    return
        "#version 300 es\n"
        "precision highp float;\n"
        "layout(location = 0) in vec3 a_position;\n"
        "layout(location = 1) in vec4 a_color;\n"
        "uniform mat4 u_mvp;\n"
        "out vec4 v_color;\n"
        "void main() {\n"
        "    gl_Position = u_mvp * vec4(a_position, 1.0);\n"
        "    v_color = a_color;\n"
        "}\n";
}

const char *cr_frag_source(void)
{
    return
        "#version 300 es\n"
        "precision highp float;\n"
        "in vec4 v_color;\n"
        "out vec4 frag_color;\n"
        "void main() {\n"
        "    frag_color = v_color;\n"
        "}\n";
}
