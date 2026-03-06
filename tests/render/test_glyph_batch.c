#include "../unity/unity.h"
#include "../../src/render/glyph_batch.h"

#include <math.h>

#define TOL 0.001f
#define NEAR(exp, act) ((act) >= (exp) - TOL && (act) <= (exp) + TOL)

void setUp(void) {}
void tearDown(void) {}

/* Camera facing +Z: right=(1,0,0), up=(0,1,0) */
#define CR_X 1.0f
#define CR_Y 0.0f
#define CR_Z 0.0f
#define CU_X 0.0f
#define CU_Y 1.0f
#define CU_Z 0.0f

static glyph_instance_t make_instance(float px, float py, float pz,
                                       float w, float h,
                                       float u0, float v0, float u1, float v1,
                                       float r, float g, float b, float a)
{
    glyph_instance_t inst;
    inst.px = px; inst.py = py; inst.pz = pz;
    inst.width = w; inst.height = h;
    inst.u0 = u0; inst.v0 = v0; inst.u1 = u1; inst.v1 = v1;
    inst.r = r; inst.g = g; inst.b = b; inst.a = a;
    return inst;
}

/* 1. Stride is 9 */
void test_stride(void)
{
    TEST_ASSERT_EQUAL_INT(9, glyph_batch_stride());
}

/* 2. Position offset is 0 */
void test_position_offset(void)
{
    TEST_ASSERT_EQUAL_INT(0, glyph_batch_position_offset());
}

/* 3. UV offset is 3 */
void test_uv_offset(void)
{
    TEST_ASSERT_EQUAL_INT(3, glyph_batch_uv_offset());
}

/* 4. Color offset is 5 */
void test_color_offset(void)
{
    TEST_ASSERT_EQUAL_INT(5, glyph_batch_color_offset());
}

/* 5. Size for 1 glyph: 4 verts, 6 indices */
void test_size_one(void)
{
    glyph_batch_info_t info = glyph_batch_size(1);
    TEST_ASSERT_EQUAL_INT(4, info.vertex_count);
    TEST_ASSERT_EQUAL_INT(6, info.index_count);
}

/* 6. Size for 5 glyphs */
void test_size_five(void)
{
    glyph_batch_info_t info = glyph_batch_size(5);
    TEST_ASSERT_EQUAL_INT(20, info.vertex_count);
    TEST_ASSERT_EQUAL_INT(30, info.index_count);
}

/* 7. Size for 0 glyphs */
void test_size_zero(void)
{
    glyph_batch_info_t info = glyph_batch_size(0);
    TEST_ASSERT_EQUAL_INT(0, info.vertex_count);
    TEST_ASSERT_EQUAL_INT(0, info.index_count);
}

/* 8. Size clamped to GLYPH_BATCH_MAX */
void test_size_clamped(void)
{
    glyph_batch_info_t info = glyph_batch_size(999);
    TEST_ASSERT_EQUAL_INT(GLYPH_BATCH_MAX * 4, info.vertex_count);
    TEST_ASSERT_EQUAL_INT(GLYPH_BATCH_MAX * 6, info.index_count);
}

/* 9. Single glyph at origin — 4 corners form a billboard quad */
void test_single_glyph_positions(void)
{
    glyph_instance_t inst = make_instance(
        0.0f, 0.0f, 0.0f,   /* position */
        2.0f, 2.0f,          /* size */
        0.0f, 0.0f, 1.0f, 1.0f, /* UVs */
        1.0f, 1.0f, 1.0f, 1.0f  /* color */
    );
    float verts[4 * GLYPH_BATCH_STRIDE];
    unsigned int idx[6];
    glyph_batch_generate(&inst, 1, CR_X, CR_Y, CR_Z, CU_X, CU_Y, CU_Z, verts, idx);

    /* half-width=1, half-height=1 */
    /* v0: center - right*hw - up*hh = (-1, -1, 0) */
    TEST_ASSERT_TRUE(NEAR(-1.0f, verts[0 * GLYPH_BATCH_STRIDE + 0]));
    TEST_ASSERT_TRUE(NEAR(-1.0f, verts[0 * GLYPH_BATCH_STRIDE + 1]));
    TEST_ASSERT_TRUE(NEAR( 0.0f, verts[0 * GLYPH_BATCH_STRIDE + 2]));

    /* v1: center + right*hw - up*hh = (1, -1, 0) */
    TEST_ASSERT_TRUE(NEAR( 1.0f, verts[1 * GLYPH_BATCH_STRIDE + 0]));
    TEST_ASSERT_TRUE(NEAR(-1.0f, verts[1 * GLYPH_BATCH_STRIDE + 1]));

    /* v2: center + right*hw + up*hh = (1, 1, 0) */
    TEST_ASSERT_TRUE(NEAR( 1.0f, verts[2 * GLYPH_BATCH_STRIDE + 0]));
    TEST_ASSERT_TRUE(NEAR( 1.0f, verts[2 * GLYPH_BATCH_STRIDE + 1]));

    /* v3: center - right*hw + up*hh = (-1, 1, 0) */
    TEST_ASSERT_TRUE(NEAR(-1.0f, verts[3 * GLYPH_BATCH_STRIDE + 0]));
    TEST_ASSERT_TRUE(NEAR( 1.0f, verts[3 * GLYPH_BATCH_STRIDE + 1]));
}

/* 10. Glyph at offset position */
void test_glyph_offset_position(void)
{
    glyph_instance_t inst = make_instance(
        5.0f, 3.0f, -2.0f,
        1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f
    );
    float verts[4 * GLYPH_BATCH_STRIDE];
    unsigned int idx[6];
    glyph_batch_generate(&inst, 1, CR_X, CR_Y, CR_Z, CU_X, CU_Y, CU_Z, verts, idx);

    /* v0: (5,3,-2) - (0.5,0,0) - (0,0.5,0) = (4.5, 2.5, -2) */
    TEST_ASSERT_TRUE(NEAR(4.5f, verts[0 * GLYPH_BATCH_STRIDE + 0]));
    TEST_ASSERT_TRUE(NEAR(2.5f, verts[0 * GLYPH_BATCH_STRIDE + 1]));
    TEST_ASSERT_TRUE(NEAR(-2.0f, verts[0 * GLYPH_BATCH_STRIDE + 2]));
}

/* 11. UV mapping — corners match instance UVs */
void test_uv_mapping(void)
{
    glyph_instance_t inst = make_instance(
        0.0f, 0.0f, 0.0f,
        1.0f, 1.0f,
        0.25f, 0.5f, 0.75f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f
    );
    float verts[4 * GLYPH_BATCH_STRIDE];
    unsigned int idx[6];
    glyph_batch_generate(&inst, 1, CR_X, CR_Y, CR_Z, CU_X, CU_Y, CU_Z, verts, idx);

    int uv = glyph_batch_uv_offset();
    /* v0 (bottom-left): u0, v0 */
    TEST_ASSERT_TRUE(NEAR(0.25f, verts[0 * GLYPH_BATCH_STRIDE + uv + 0]));
    TEST_ASSERT_TRUE(NEAR(0.5f,  verts[0 * GLYPH_BATCH_STRIDE + uv + 1]));
    /* v1 (bottom-right): u1, v0 */
    TEST_ASSERT_TRUE(NEAR(0.75f, verts[1 * GLYPH_BATCH_STRIDE + uv + 0]));
    TEST_ASSERT_TRUE(NEAR(0.5f,  verts[1 * GLYPH_BATCH_STRIDE + uv + 1]));
    /* v2 (top-right): u1, v1 */
    TEST_ASSERT_TRUE(NEAR(0.75f, verts[2 * GLYPH_BATCH_STRIDE + uv + 0]));
    TEST_ASSERT_TRUE(NEAR(1.0f,  verts[2 * GLYPH_BATCH_STRIDE + uv + 1]));
    /* v3 (top-left): u0, v1 */
    TEST_ASSERT_TRUE(NEAR(0.25f, verts[3 * GLYPH_BATCH_STRIDE + uv + 0]));
    TEST_ASSERT_TRUE(NEAR(1.0f,  verts[3 * GLYPH_BATCH_STRIDE + uv + 1]));
}

/* 12. Color written to all 4 vertices */
void test_color_all_vertices(void)
{
    glyph_instance_t inst = make_instance(
        0.0f, 0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
        0.8f, 0.2f, 0.5f, 0.7f
    );
    float verts[4 * GLYPH_BATCH_STRIDE];
    unsigned int idx[6];
    glyph_batch_generate(&inst, 1, CR_X, CR_Y, CR_Z, CU_X, CU_Y, CU_Z, verts, idx);

    int co = glyph_batch_color_offset();
    for (int v = 0; v < 4; v++) {
        int off = v * GLYPH_BATCH_STRIDE + co;
        TEST_ASSERT_TRUE(NEAR(0.8f, verts[off + 0]));
        TEST_ASSERT_TRUE(NEAR(0.2f, verts[off + 1]));
        TEST_ASSERT_TRUE(NEAR(0.5f, verts[off + 2]));
        TEST_ASSERT_TRUE(NEAR(0.7f, verts[off + 3]));
    }
}

/* 13. Indices for single glyph: two triangles */
void test_single_indices(void)
{
    glyph_instance_t inst = make_instance(
        0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f
    );
    float verts[4 * GLYPH_BATCH_STRIDE];
    unsigned int idx[6];
    glyph_batch_generate(&inst, 1, CR_X, CR_Y, CR_Z, CU_X, CU_Y, CU_Z, verts, idx);

    /* Triangle 1: 0,1,2 */
    TEST_ASSERT_EQUAL_UINT(0, idx[0]);
    TEST_ASSERT_EQUAL_UINT(1, idx[1]);
    TEST_ASSERT_EQUAL_UINT(2, idx[2]);
    /* Triangle 2: 0,2,3 */
    TEST_ASSERT_EQUAL_UINT(0, idx[3]);
    TEST_ASSERT_EQUAL_UINT(2, idx[4]);
    TEST_ASSERT_EQUAL_UINT(3, idx[5]);
}

/* 14. Multi-glyph batch — second glyph indices offset by 4 */
void test_multi_glyph_indices(void)
{
    glyph_instance_t insts[2];
    insts[0] = make_instance(0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                              0.0f, 0.0f, 1.0f, 1.0f,
                              1.0f, 0.0f, 0.0f, 1.0f);
    insts[1] = make_instance(5.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                              0.0f, 0.0f, 1.0f, 1.0f,
                              0.0f, 1.0f, 0.0f, 1.0f);
    float verts[8 * GLYPH_BATCH_STRIDE];
    unsigned int idx[12];
    glyph_batch_generate(insts, 2, CR_X, CR_Y, CR_Z, CU_X, CU_Y, CU_Z, verts, idx);

    /* Second glyph indices offset by 4 */
    TEST_ASSERT_EQUAL_UINT(4, idx[6]);
    TEST_ASSERT_EQUAL_UINT(5, idx[7]);
    TEST_ASSERT_EQUAL_UINT(6, idx[8]);
    TEST_ASSERT_EQUAL_UINT(4, idx[9]);
    TEST_ASSERT_EQUAL_UINT(6, idx[10]);
    TEST_ASSERT_EQUAL_UINT(7, idx[11]);
}

/* 15. Multi-glyph — different colors preserved */
void test_multi_glyph_colors(void)
{
    glyph_instance_t insts[2];
    insts[0] = make_instance(0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                              0.0f, 0.0f, 1.0f, 1.0f,
                              1.0f, 0.0f, 0.0f, 1.0f);
    insts[1] = make_instance(5.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                              0.0f, 0.0f, 1.0f, 1.0f,
                              0.0f, 0.0f, 1.0f, 0.5f);
    float verts[8 * GLYPH_BATCH_STRIDE];
    unsigned int idx[12];
    glyph_batch_generate(insts, 2, CR_X, CR_Y, CR_Z, CU_X, CU_Y, CU_Z, verts, idx);

    int co = glyph_batch_color_offset();
    /* Glyph 0, vertex 0: red */
    TEST_ASSERT_TRUE(NEAR(1.0f, verts[0 * GLYPH_BATCH_STRIDE + co + 0]));
    TEST_ASSERT_TRUE(NEAR(0.0f, verts[0 * GLYPH_BATCH_STRIDE + co + 1]));
    /* Glyph 1, vertex 0 (index 4): blue, alpha 0.5 */
    TEST_ASSERT_TRUE(NEAR(0.0f, verts[4 * GLYPH_BATCH_STRIDE + co + 0]));
    TEST_ASSERT_TRUE(NEAR(0.0f, verts[4 * GLYPH_BATCH_STRIDE + co + 1]));
    TEST_ASSERT_TRUE(NEAR(1.0f, verts[4 * GLYPH_BATCH_STRIDE + co + 2]));
    TEST_ASSERT_TRUE(NEAR(0.5f, verts[4 * GLYPH_BATCH_STRIDE + co + 3]));
}

/* 16. All indices valid for multi-glyph batch */
void test_all_indices_valid(void)
{
    glyph_instance_t insts[3];
    for (int i = 0; i < 3; i++)
        insts[i] = make_instance((float)i * 2.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                                  0.0f, 0.0f, 1.0f, 1.0f,
                                  1.0f, 1.0f, 1.0f, 1.0f);
    glyph_batch_info_t info = glyph_batch_size(3);
    float verts[12 * GLYPH_BATCH_STRIDE];
    unsigned int idx[18];
    glyph_batch_generate(insts, 3, CR_X, CR_Y, CR_Z, CU_X, CU_Y, CU_Z, verts, idx);

    for (int i = 0; i < info.index_count; i++)
        TEST_ASSERT_TRUE(idx[i] < (unsigned int)info.vertex_count);
}

/* 17. Index count always divisible by 3 */
void test_indices_divisible_by_3(void)
{
    for (int n = 1; n <= 10; n++) {
        glyph_batch_info_t info = glyph_batch_size(n);
        TEST_ASSERT_EQUAL_INT(0, info.index_count % 3);
    }
}

/* 18. Non-uniform width/height */
void test_non_uniform_size(void)
{
    glyph_instance_t inst = make_instance(
        0.0f, 0.0f, 0.0f,
        4.0f, 2.0f,      /* wide glyph */
        0.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f
    );
    float verts[4 * GLYPH_BATCH_STRIDE];
    unsigned int idx[6];
    glyph_batch_generate(&inst, 1, CR_X, CR_Y, CR_Z, CU_X, CU_Y, CU_Z, verts, idx);

    /* hw=2, hh=1 */
    /* v0: (-2, -1, 0) */
    TEST_ASSERT_TRUE(NEAR(-2.0f, verts[0 * GLYPH_BATCH_STRIDE + 0]));
    TEST_ASSERT_TRUE(NEAR(-1.0f, verts[0 * GLYPH_BATCH_STRIDE + 1]));
    /* v2: (2, 1, 0) */
    TEST_ASSERT_TRUE(NEAR( 2.0f, verts[2 * GLYPH_BATCH_STRIDE + 0]));
    TEST_ASSERT_TRUE(NEAR( 1.0f, verts[2 * GLYPH_BATCH_STRIDE + 1]));
}

/* 19. Angled camera vectors */
void test_angled_camera(void)
{
    glyph_instance_t inst = make_instance(
        0.0f, 0.0f, 0.0f,
        2.0f, 2.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f
    );
    float verts[4 * GLYPH_BATCH_STRIDE];
    unsigned int idx[6];
    /* Camera looking down -Y: right=(1,0,0), up=(0,0,-1) */
    glyph_batch_generate(&inst, 1, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, verts, idx);

    /* v0: (0,0,0) - (1,0,0)*1 - (0,0,-1)*1 = (-1, 0, 1) */
    TEST_ASSERT_TRUE(NEAR(-1.0f, verts[0 * GLYPH_BATCH_STRIDE + 0]));
    TEST_ASSERT_TRUE(NEAR( 0.0f, verts[0 * GLYPH_BATCH_STRIDE + 1]));
    TEST_ASSERT_TRUE(NEAR( 1.0f, verts[0 * GLYPH_BATCH_STRIDE + 2]));
}

/* 20. Zero count produces no output (doesn't crash) */
void test_zero_count(void)
{
    float verts[GLYPH_BATCH_STRIDE];
    unsigned int idx[6];
    /* Should not crash; just test it runs */
    glyph_batch_generate(NULL, 0, CR_X, CR_Y, CR_Z, CU_X, CU_Y, CU_Z, verts, idx);
    glyph_batch_info_t info = glyph_batch_size(0);
    TEST_ASSERT_EQUAL_INT(0, info.vertex_count);
}

/* 21. Alpha channel preserved */
void test_alpha_channel(void)
{
    glyph_instance_t inst = make_instance(
        0.0f, 0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 0.3f
    );
    float verts[4 * GLYPH_BATCH_STRIDE];
    unsigned int idx[6];
    glyph_batch_generate(&inst, 1, CR_X, CR_Y, CR_Z, CU_X, CU_Y, CU_Z, verts, idx);

    int co = glyph_batch_color_offset();
    for (int v = 0; v < 4; v++)
        TEST_ASSERT_TRUE(NEAR(0.3f, verts[v * GLYPH_BATCH_STRIDE + co + 3]));
}

/* 22. Purity — same input produces same output */
void test_purity(void)
{
    glyph_instance_t inst = make_instance(
        3.0f, 1.0f, -1.0f,
        2.0f, 1.5f,
        0.1f, 0.2f, 0.9f, 0.8f,
        0.5f, 0.6f, 0.7f, 0.8f
    );
    float va[4 * GLYPH_BATCH_STRIDE], vb[4 * GLYPH_BATCH_STRIDE];
    unsigned int ia[6], ib[6];
    glyph_batch_generate(&inst, 1, CR_X, CR_Y, CR_Z, CU_X, CU_Y, CU_Z, va, ia);
    glyph_batch_generate(&inst, 1, CR_X, CR_Y, CR_Z, CU_X, CU_Y, CU_Z, vb, ib);

    for (int i = 0; i < 4 * GLYPH_BATCH_STRIDE; i++)
        TEST_ASSERT_TRUE(va[i] == vb[i]);
    for (int i = 0; i < 6; i++)
        TEST_ASSERT_TRUE(ia[i] == ib[i]);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_stride);
    RUN_TEST(test_position_offset);
    RUN_TEST(test_uv_offset);
    RUN_TEST(test_color_offset);
    RUN_TEST(test_size_one);
    RUN_TEST(test_size_five);
    RUN_TEST(test_size_zero);
    RUN_TEST(test_size_clamped);
    RUN_TEST(test_single_glyph_positions);
    RUN_TEST(test_glyph_offset_position);
    RUN_TEST(test_uv_mapping);
    RUN_TEST(test_color_all_vertices);
    RUN_TEST(test_single_indices);
    RUN_TEST(test_multi_glyph_indices);
    RUN_TEST(test_multi_glyph_colors);
    RUN_TEST(test_all_indices_valid);
    RUN_TEST(test_indices_divisible_by_3);
    RUN_TEST(test_non_uniform_size);
    RUN_TEST(test_angled_camera);
    RUN_TEST(test_zero_count);
    RUN_TEST(test_alpha_channel);
    RUN_TEST(test_purity);
    return UNITY_END();
}
