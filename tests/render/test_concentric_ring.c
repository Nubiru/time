#include "../unity/unity.h"
#include "../../src/render/concentric_ring.h"

#include <math.h>
#include <string.h>

#define TOL 0.001f
#define NEAR(exp, act) ((act) >= (exp) - TOL && (act) <= (exp) + TOL)

void setUp(void) {}
void tearDown(void) {}

/* 1. Default layout has 3 rings with sane radii */
void test_cr_default_layout(void)
{
    cr_layout_t layout = cr_default_layout();
    TEST_ASSERT_EQUAL_INT(3, layout.ring_count);
    TEST_ASSERT_TRUE(layout.innermost_radius > 0.0f);
    TEST_ASSERT_TRUE(layout.ring_gap > 0.0f);
    TEST_ASSERT_EQUAL_INT(12, layout.rings[0].segment_count);
    TEST_ASSERT_EQUAL_INT(64, layout.rings[1].segment_count);
    TEST_ASSERT_EQUAL_INT(260, layout.rings[2].segment_count);
}

/* 2. Each ring's inner radius > previous ring's outer radius */
void test_cr_ring_inner_increases(void)
{
    cr_layout_t layout = cr_default_layout();
    for (int i = 1; i < layout.ring_count; i++) {
        float prev_outer = cr_ring_outer(&layout, i - 1);
        float this_inner = cr_ring_inner(&layout, i);
        TEST_ASSERT_TRUE(this_inner > prev_outer);
    }
}

/* 3. Outer > inner for all rings */
void test_cr_ring_outer_gt_inner(void)
{
    cr_layout_t layout = cr_default_layout();
    for (int i = 0; i < layout.ring_count; i++) {
        float inner = cr_ring_inner(&layout, i);
        float outer = cr_ring_outer(&layout, i);
        TEST_ASSERT_TRUE(outer > inner);
    }
}

/* 4. Ring width matches outer - inner */
void test_cr_ring_width_consistent(void)
{
    cr_layout_t layout = cr_default_layout();
    for (int i = 0; i < layout.ring_count; i++) {
        float inner = cr_ring_inner(&layout, i);
        float outer = cr_ring_outer(&layout, i);
        float width = cr_ring_width(&layout, i);
        TEST_ASSERT_TRUE(NEAR(outer - inner, width));
    }
}

/* 5. Single ring with N segments: verts = (N+1)*2 */
void test_cr_total_verts_single_ring(void)
{
    cr_layout_t layout = cr_default_layout();
    layout.ring_count = 1;

    /* 12 segments */
    layout.rings[0].segment_count = 12;
    TEST_ASSERT_EQUAL_INT((12 + 1) * 2, cr_total_verts(&layout));

    /* 64 segments */
    layout.rings[0].segment_count = 64;
    TEST_ASSERT_EQUAL_INT((64 + 1) * 2, cr_total_verts(&layout));

    /* 260 segments */
    layout.rings[0].segment_count = 260;
    TEST_ASSERT_EQUAL_INT((260 + 1) * 2, cr_total_verts(&layout));
}

/* 6. Single ring with N segments: indices = N*6 */
void test_cr_total_indices_single_ring(void)
{
    cr_layout_t layout = cr_default_layout();
    layout.ring_count = 1;

    layout.rings[0].segment_count = 12;
    TEST_ASSERT_EQUAL_INT(12 * 6, cr_total_indices(&layout));

    layout.rings[0].segment_count = 64;
    TEST_ASSERT_EQUAL_INT(64 * 6, cr_total_indices(&layout));

    layout.rings[0].segment_count = 260;
    TEST_ASSERT_EQUAL_INT(260 * 6, cr_total_indices(&layout));
}

/* 7. Generated counts match cr_total_verts/indices */
void test_cr_generate_counts_match(void)
{
    cr_layout_t layout = cr_default_layout();
    int nv = cr_total_verts(&layout);
    int ni = cr_total_indices(&layout);
    float verts[nv * CR_VERT_FLOATS];
    unsigned int indices[ni];

    cr_info_t info = cr_generate(&layout, verts, indices);
    TEST_ASSERT_EQUAL_INT(nv, info.total_vertices);
    TEST_ASSERT_EQUAL_INT(ni, info.total_indices);

    /* Sum per-ring counts should match totals */
    int sum_v = 0, sum_i = 0;
    for (int r = 0; r < layout.ring_count; r++) {
        sum_v += info.ring_vert_count[r];
        sum_i += info.ring_idx_count[r];
    }
    TEST_ASSERT_EQUAL_INT(nv, sum_v);
    TEST_ASSERT_EQUAL_INT(ni, sum_i);
}

/* 8. All Y coordinates = 0 (XZ plane) */
void test_cr_generate_vertices_in_plane(void)
{
    cr_layout_t layout = cr_default_layout();
    int nv = cr_total_verts(&layout);
    int ni = cr_total_indices(&layout);
    float verts[nv * CR_VERT_FLOATS];
    unsigned int indices[ni];

    cr_generate(&layout, verts, indices);
    for (int v = 0; v < nv; v++) {
        float y = verts[v * CR_VERT_FLOATS + 1]; /* position.y */
        TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, y);
    }
}

/* 9. Vertex distances from origin between inner and outer */
void test_cr_generate_radii_correct(void)
{
    cr_layout_t layout = cr_default_layout();
    int nv = cr_total_verts(&layout);
    int ni = cr_total_indices(&layout);
    float verts[nv * CR_VERT_FLOATS];
    unsigned int indices[ni];

    cr_info_t info = cr_generate(&layout, verts, indices);
    for (int r = 0; r < layout.ring_count; r++) {
        float inner = cr_ring_inner(&layout, r);
        float outer = cr_ring_outer(&layout, r);
        int start = info.ring_vert_offset[r];
        int count = info.ring_vert_count[r];
        for (int v = start; v < start + count; v++) {
            float x = verts[v * CR_VERT_FLOATS + 0];
            float z = verts[v * CR_VERT_FLOATS + 2];
            float dist = sqrtf(x * x + z * z);
            TEST_ASSERT_TRUE(dist >= inner - TOL);
            TEST_ASSERT_TRUE(dist <= outer + TOL);
        }
    }
}

/* 10. All indices < total vertex count */
void test_cr_generate_indices_valid(void)
{
    cr_layout_t layout = cr_default_layout();
    int nv = cr_total_verts(&layout);
    int ni = cr_total_indices(&layout);
    float verts[nv * CR_VERT_FLOATS];
    unsigned int indices[ni];

    cr_info_t info = cr_generate(&layout, verts, indices);
    for (int i = 0; i < ni; i++) {
        TEST_ASSERT_TRUE(indices[i] < (unsigned int)info.total_vertices);
    }
}

/* 11. Highlighted segment has higher alpha than neighbors */
void test_cr_generate_highlight_brighter(void)
{
    cr_layout_t layout = cr_default_layout();
    layout.ring_count = 1;
    layout.rings[0].segment_count = 12;
    layout.rings[0].highlight_index = 3;
    layout.rings[0].highlight_boost = 2.0f;
    layout.rings[0].base_alpha = 0.5f;
    layout.rings[0].segment_colors = NULL;

    int nv = cr_total_verts(&layout);
    int ni = cr_total_indices(&layout);
    float verts[nv * CR_VERT_FLOATS];
    unsigned int indices[ni];

    cr_generate(&layout, verts, indices);

    /* Highlighted segment is index 3: vertices at columns 3 and 4 */
    /* Column 3, inner vertex = vertex index 3*2 = 6 */
    int hi_v = 3 * 2; /* first vertex of highlighted segment */
    float hi_alpha = verts[hi_v * CR_VERT_FLOATS + 6]; /* RGBA: index 6 = A */

    /* Non-highlighted neighbor: segment 2 */
    int lo_v = 2 * 2;
    float lo_alpha = verts[lo_v * CR_VERT_FLOATS + 6];

    TEST_ASSERT_TRUE(hi_alpha > lo_alpha);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.5f * 2.0f, hi_alpha);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.5f, lo_alpha);
}

/* 12. 12 segments: angle 15 -> segment 0, angle 45 -> segment 1 */
void test_cr_segment_at_angle_basic(void)
{
    /* 12 segments: each segment = 30 degrees */
    TEST_ASSERT_EQUAL_INT(0, cr_segment_at_angle(12, 0.0f));
    TEST_ASSERT_EQUAL_INT(0, cr_segment_at_angle(12, 15.0f));
    TEST_ASSERT_EQUAL_INT(0, cr_segment_at_angle(12, 29.9f));
    TEST_ASSERT_EQUAL_INT(1, cr_segment_at_angle(12, 30.0f));
    TEST_ASSERT_EQUAL_INT(1, cr_segment_at_angle(12, 45.0f));
    TEST_ASSERT_EQUAL_INT(11, cr_segment_at_angle(12, 350.0f));
}

/* 13. 64 segments: angle 0 -> 0, angle 180 -> 32 */
void test_cr_segment_at_angle_64(void)
{
    TEST_ASSERT_EQUAL_INT(0, cr_segment_at_angle(64, 0.0f));
    TEST_ASSERT_EQUAL_INT(32, cr_segment_at_angle(64, 180.0f));
    TEST_ASSERT_EQUAL_INT(63, cr_segment_at_angle(64, 359.0f));
}

/* 14. Segments get their assigned per-segment colors */
void test_cr_generate_per_segment_colors(void)
{
    cr_layout_t layout = cr_default_layout();
    layout.ring_count = 1;
    layout.rings[0].segment_count = 3;
    layout.rings[0].highlight_index = -1;
    layout.rings[0].base_alpha = 0.8f;

    /* 3 segments, each with distinct RGB */
    float colors[9] = {
        1.0f, 0.0f, 0.0f,  /* segment 0: red */
        0.0f, 1.0f, 0.0f,  /* segment 1: green */
        0.0f, 0.0f, 1.0f   /* segment 2: blue */
    };
    layout.rings[0].segment_colors = colors;

    int nv = cr_total_verts(&layout);
    int ni = cr_total_indices(&layout);
    float verts[nv * CR_VERT_FLOATS];
    unsigned int indices[ni];

    cr_generate(&layout, verts, indices);

    /* Segment 0: column 0 inner vertex = vertex 0 */
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, verts[0 * CR_VERT_FLOATS + 3]); /* R */
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, verts[0 * CR_VERT_FLOATS + 4]); /* G */
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, verts[0 * CR_VERT_FLOATS + 5]); /* B */

    /* Segment 1: column 1 inner vertex = vertex 2 */
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, verts[2 * CR_VERT_FLOATS + 3]); /* R */
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, verts[2 * CR_VERT_FLOATS + 4]); /* G */
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, verts[2 * CR_VERT_FLOATS + 5]); /* B */

    /* Segment 2: column 2 inner vertex = vertex 4 */
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, verts[4 * CR_VERT_FLOATS + 3]); /* R */
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, verts[4 * CR_VERT_FLOATS + 4]); /* G */
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, verts[4 * CR_VERT_FLOATS + 5]); /* B */
}

/* 15. When segment_colors is NULL, uses uniform_color */
void test_cr_generate_uniform_color(void)
{
    cr_layout_t layout = cr_default_layout();
    layout.ring_count = 1;
    layout.rings[0].segment_count = 4;
    layout.rings[0].highlight_index = -1;
    layout.rings[0].base_alpha = 0.7f;
    layout.rings[0].segment_colors = NULL;
    layout.rings[0].uniform_color[0] = 0.3f;
    layout.rings[0].uniform_color[1] = 0.6f;
    layout.rings[0].uniform_color[2] = 0.9f;

    int nv = cr_total_verts(&layout);
    int ni = cr_total_indices(&layout);
    float verts[nv * CR_VERT_FLOATS];
    unsigned int indices[ni];

    cr_generate(&layout, verts, indices);

    /* Every vertex should have the uniform color */
    for (int v = 0; v < nv; v++) {
        TEST_ASSERT_FLOAT_WITHIN(TOL, 0.3f, verts[v * CR_VERT_FLOATS + 3]);
        TEST_ASSERT_FLOAT_WITHIN(TOL, 0.6f, verts[v * CR_VERT_FLOATS + 4]);
        TEST_ASSERT_FLOAT_WITHIN(TOL, 0.9f, verts[v * CR_VERT_FLOATS + 5]);
        TEST_ASSERT_FLOAT_WITHIN(TOL, 0.7f, verts[v * CR_VERT_FLOATS + 6]);
    }
}

/* 16. Both shader sources are non-NULL */
void test_cr_shader_sources_not_null(void)
{
    TEST_ASSERT_NOT_NULL(cr_vert_source());
    TEST_ASSERT_NOT_NULL(cr_frag_source());
    /* Shader sources should contain version directive */
    TEST_ASSERT_NOT_NULL(strstr(cr_vert_source(), "#version"));
    TEST_ASSERT_NOT_NULL(strstr(cr_frag_source(), "#version"));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_cr_default_layout);
    RUN_TEST(test_cr_ring_inner_increases);
    RUN_TEST(test_cr_ring_outer_gt_inner);
    RUN_TEST(test_cr_ring_width_consistent);
    RUN_TEST(test_cr_total_verts_single_ring);
    RUN_TEST(test_cr_total_indices_single_ring);
    RUN_TEST(test_cr_generate_counts_match);
    RUN_TEST(test_cr_generate_vertices_in_plane);
    RUN_TEST(test_cr_generate_radii_correct);
    RUN_TEST(test_cr_generate_indices_valid);
    RUN_TEST(test_cr_generate_highlight_brighter);
    RUN_TEST(test_cr_segment_at_angle_basic);
    RUN_TEST(test_cr_segment_at_angle_64);
    RUN_TEST(test_cr_generate_per_segment_colors);
    RUN_TEST(test_cr_generate_uniform_color);
    RUN_TEST(test_cr_shader_sources_not_null);
    return UNITY_END();
}
