#include "../unity/unity.h"
#include "../../src/render/ring_data.h"
#include "../../src/render/ring_geometry.h"

#include <math.h>

#define TOL 0.01f
#define NEAR(exp, act) ((act) >= (exp) - TOL && (act) <= (exp) + TOL)

void setUp(void) {}
void tearDown(void) {}

/* Test sign colors: distinct per sign */
static void make_colors(float *colors)
{
    for (int i = 0; i < 12; i++) {
        colors[i * 3 + 0] = (float)i / 11.0f;       /* r ramps 0→1 */
        colors[i * 3 + 1] = 1.0f - (float)i / 11.0f; /* g ramps 1→0 */
        colors[i * 3 + 2] = 0.5f;                     /* b constant */
    }
}

/* sps=1: 26 verts, 72 indices */
#define SPS 1
#define VCOUNT 26
#define ICOUNT 72

/* 1. Stride is 8 */
void test_stride(void)
{
    TEST_ASSERT_EQUAL_INT(8, ring_data_stride());
}

/* 2. Position offset is 0 */
void test_position_offset(void)
{
    TEST_ASSERT_EQUAL_INT(0, ring_data_position_offset());
}

/* 3. Color offset is 3 */
void test_color_offset(void)
{
    TEST_ASSERT_EQUAL_INT(3, ring_data_color_offset());
}

/* 4. UV offset is 6 */
void test_uv_offset(void)
{
    TEST_ASSERT_EQUAL_INT(6, ring_data_uv_offset());
}

/* 5. Compatible with ring_mesh_size */
void test_compatible_sizes(void)
{
    ring_mesh_info_t info = ring_mesh_size(SPS);
    TEST_ASSERT_EQUAL_INT(VCOUNT, info.vertex_count);
    TEST_ASSERT_EQUAL_INT(ICOUNT, info.index_count);
}

/* 6. Inner vertices at correct radius */
void test_inner_radius(void)
{
    float verts[VCOUNT * RING_DATA_STRIDE];
    unsigned int idx[ICOUNT];
    float colors[36];
    make_colors(colors);
    ring_data_generate(verts, idx, 5.0f, 8.0f, SPS, colors);

    for (int i = 0; i < 13; i++) {
        int v = i * 2; /* inner */
        int off = v * RING_DATA_STRIDE;
        float x = verts[off + 0];
        float z = verts[off + 2];
        float r = sqrtf(x * x + z * z);
        TEST_ASSERT_TRUE(NEAR(5.0f, r));
    }
}

/* 7. Outer vertices at correct radius */
void test_outer_radius(void)
{
    float verts[VCOUNT * RING_DATA_STRIDE];
    unsigned int idx[ICOUNT];
    ring_data_generate(verts, idx, 5.0f, 8.0f, SPS, NULL);

    for (int i = 0; i < 13; i++) {
        int v = i * 2 + 1; /* outer */
        int off = v * RING_DATA_STRIDE;
        float x = verts[off + 0];
        float z = verts[off + 2];
        float r = sqrtf(x * x + z * z);
        TEST_ASSERT_TRUE(NEAR(8.0f, r));
    }
}

/* 8. All y = 0 */
void test_y_zero(void)
{
    float verts[VCOUNT * RING_DATA_STRIDE];
    unsigned int idx[ICOUNT];
    ring_data_generate(verts, idx, 5.0f, 8.0f, SPS, NULL);

    for (int v = 0; v < VCOUNT; v++)
        TEST_ASSERT_TRUE(verts[v * RING_DATA_STRIDE + 1] == 0.0f);
}

/* 9. First vertex at +X axis */
void test_first_vertex(void)
{
    float verts[VCOUNT * RING_DATA_STRIDE];
    unsigned int idx[ICOUNT];
    ring_data_generate(verts, idx, 5.0f, 8.0f, SPS, NULL);
    TEST_ASSERT_TRUE(NEAR(5.0f, verts[0]));
    TEST_ASSERT_TRUE(NEAR(0.0f, verts[2]));
}

/* 10. Sign 0 (Aries) gets correct color */
void test_sign0_color(void)
{
    float verts[VCOUNT * RING_DATA_STRIDE];
    unsigned int idx[ICOUNT];
    float colors[36];
    make_colors(colors);
    ring_data_generate(verts, idx, 5.0f, 8.0f, SPS, colors);

    /* Vertex 0 (inner, column 0, sign 0) */
    int off = 0 * RING_DATA_STRIDE;
    TEST_ASSERT_TRUE(NEAR(colors[0], verts[off + 3]));
    TEST_ASSERT_TRUE(NEAR(colors[1], verts[off + 4]));
    TEST_ASSERT_TRUE(NEAR(colors[2], verts[off + 5]));
}

/* 11. Sign 11 (Pisces) gets correct color */
void test_sign11_color(void)
{
    float verts[VCOUNT * RING_DATA_STRIDE];
    unsigned int idx[ICOUNT];
    float colors[36];
    make_colors(colors);
    ring_data_generate(verts, idx, 5.0f, 8.0f, SPS, colors);

    /* Last column (12) is clamped to sign 11.
     * Inner vertex = column 12 * 2 = vertex 24 */
    int off = 24 * RING_DATA_STRIDE;
    TEST_ASSERT_TRUE(NEAR(colors[11 * 3 + 0], verts[off + 3]));
    TEST_ASSERT_TRUE(NEAR(colors[11 * 3 + 1], verts[off + 4]));
    TEST_ASSERT_TRUE(NEAR(colors[11 * 3 + 2], verts[off + 5]));
}

/* 12. Adjacent signs have different colors */
void test_adjacent_sign_colors_differ(void)
{
    float verts[VCOUNT * RING_DATA_STRIDE];
    unsigned int idx[ICOUNT];
    float colors[36];
    make_colors(colors);
    ring_data_generate(verts, idx, 5.0f, 8.0f, SPS, colors);

    /* Column 0 = sign 0, column 1 = sign 1 */
    int off0 = 0 * RING_DATA_STRIDE + 3;
    int off1 = 2 * RING_DATA_STRIDE + 3; /* vertex 2 = column 1 inner */
    float r0 = verts[off0];
    float r1 = verts[off1];
    TEST_ASSERT_TRUE(r0 != r1); /* Different red values */
}

/* 13. NULL colors → white */
void test_null_colors_white(void)
{
    float verts[VCOUNT * RING_DATA_STRIDE];
    unsigned int idx[ICOUNT];
    ring_data_generate(verts, idx, 5.0f, 8.0f, SPS, NULL);

    for (int v = 0; v < VCOUNT; v++) {
        int off = v * RING_DATA_STRIDE;
        TEST_ASSERT_TRUE(verts[off + 3] == 1.0f);
        TEST_ASSERT_TRUE(verts[off + 4] == 1.0f);
        TEST_ASSERT_TRUE(verts[off + 5] == 1.0f);
    }
}

/* 14. UV U in [0, 1] */
void test_uv_u_range(void)
{
    float verts[VCOUNT * RING_DATA_STRIDE];
    unsigned int idx[ICOUNT];
    ring_data_generate(verts, idx, 5.0f, 8.0f, SPS, NULL);

    for (int v = 0; v < VCOUNT; v++) {
        float u = verts[v * RING_DATA_STRIDE + 6];
        TEST_ASSERT_TRUE(u >= -TOL && u <= 1.0f + TOL);
    }
}

/* 15. UV V = 0 (inner) and 1 (outer) */
void test_uv_v_values(void)
{
    float verts[VCOUNT * RING_DATA_STRIDE];
    unsigned int idx[ICOUNT];
    ring_data_generate(verts, idx, 5.0f, 8.0f, SPS, NULL);

    for (int i = 0; i < 13; i++) {
        float v_inner = verts[(i * 2) * RING_DATA_STRIDE + 7];
        float v_outer = verts[(i * 2 + 1) * RING_DATA_STRIDE + 7];
        TEST_ASSERT_TRUE(NEAR(0.0f, v_inner));
        TEST_ASSERT_TRUE(NEAR(1.0f, v_outer));
    }
}

/* 16. All indices valid */
void test_indices_valid(void)
{
    float verts[VCOUNT * RING_DATA_STRIDE];
    unsigned int idx[ICOUNT];
    ring_data_generate(verts, idx, 5.0f, 8.0f, SPS, NULL);

    for (int i = 0; i < ICOUNT; i++)
        TEST_ASSERT_TRUE(idx[i] < (unsigned int)VCOUNT);
}

/* 17. Index count divisible by 3 */
void test_indices_triangles(void)
{
    ring_mesh_info_t info = ring_mesh_size(SPS);
    TEST_ASSERT_EQUAL_INT(0, info.index_count % 3);
}

/* 18. Inner and outer share same color per column */
void test_inner_outer_same_color(void)
{
    float verts[VCOUNT * RING_DATA_STRIDE];
    unsigned int idx[ICOUNT];
    float colors[36];
    make_colors(colors);
    ring_data_generate(verts, idx, 5.0f, 8.0f, SPS, colors);

    for (int i = 0; i < 13; i++) {
        int inner = (i * 2) * RING_DATA_STRIDE;
        int outer = (i * 2 + 1) * RING_DATA_STRIDE;
        TEST_ASSERT_TRUE(verts[inner + 3] == verts[outer + 3]);
        TEST_ASSERT_TRUE(verts[inner + 4] == verts[outer + 4]);
        TEST_ASSERT_TRUE(verts[inner + 5] == verts[outer + 5]);
    }
}

/* 19. Higher segments_per_sign produces more vertices */
void test_more_segments(void)
{
    ring_mesh_info_t a = ring_mesh_size(1);
    ring_mesh_info_t b = ring_mesh_size(4);
    TEST_ASSERT_TRUE(b.vertex_count > a.vertex_count);
    TEST_ASSERT_TRUE(b.index_count > a.index_count);
}

/* 20. Purity */
void test_purity(void)
{
    float va[VCOUNT * RING_DATA_STRIDE], vb[VCOUNT * RING_DATA_STRIDE];
    unsigned int ia[ICOUNT], ib[ICOUNT];
    float colors[36];
    make_colors(colors);
    ring_data_generate(va, ia, 5.0f, 8.0f, SPS, colors);
    ring_data_generate(vb, ib, 5.0f, 8.0f, SPS, colors);

    for (int i = 0; i < VCOUNT * RING_DATA_STRIDE; i++)
        TEST_ASSERT_TRUE(va[i] == vb[i]);
    for (int i = 0; i < ICOUNT; i++)
        TEST_ASSERT_TRUE(ia[i] == ib[i]);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_stride);
    RUN_TEST(test_position_offset);
    RUN_TEST(test_color_offset);
    RUN_TEST(test_uv_offset);
    RUN_TEST(test_compatible_sizes);
    RUN_TEST(test_inner_radius);
    RUN_TEST(test_outer_radius);
    RUN_TEST(test_y_zero);
    RUN_TEST(test_first_vertex);
    RUN_TEST(test_sign0_color);
    RUN_TEST(test_sign11_color);
    RUN_TEST(test_adjacent_sign_colors_differ);
    RUN_TEST(test_null_colors_white);
    RUN_TEST(test_uv_u_range);
    RUN_TEST(test_uv_v_values);
    RUN_TEST(test_indices_valid);
    RUN_TEST(test_indices_triangles);
    RUN_TEST(test_inner_outer_same_color);
    RUN_TEST(test_more_segments);
    RUN_TEST(test_purity);
    return UNITY_END();
}
