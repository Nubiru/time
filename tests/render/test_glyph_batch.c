#include "../unity/unity.h"
#include "../../src/render/glyph_batch.h"

#include <math.h>
#include <string.h>

#define TOL 0.0001f
#define NEAR(exp, act) (fabsf((exp) - (act)) < TOL)

void setUp(void) {}
void tearDown(void) {}

/* ===== UV computation tests ===== */

/* 1. UV for first glyph in 4x4 atlas */
void test_uv_first_glyph(void)
{
    glyph_atlas_t atlas = {4, 4, 0, 15};
    float u0, v0, u1, v1;
    glyph_batch_uv(atlas, 0, &u0, &v0, &u1, &v1);
    /* glyph 0 -> idx 0, col=0, row=0 */
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f,  u0);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f,  v0);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.25f, u1);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.25f, v1);
}

/* 2. UV for last glyph in 4x4 atlas */
void test_uv_last_glyph(void)
{
    glyph_atlas_t atlas = {4, 4, 0, 15};
    float u0, v0, u1, v1;
    glyph_batch_uv(atlas, 15, &u0, &v0, &u1, &v1);
    /* glyph 15 -> idx 15, col=3, row=3 */
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.75f, u0);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.75f, v0);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f,  u1);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f,  v1);
}

/* 3. UV for glyph in middle of 4x4 atlas */
void test_uv_middle_glyph(void)
{
    glyph_atlas_t atlas = {4, 4, 0, 15};
    float u0, v0, u1, v1;
    glyph_batch_uv(atlas, 5, &u0, &v0, &u1, &v1);
    /* glyph 5 -> idx 5, col=1, row=1 */
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.25f, u0);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.25f, v0);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.5f,  u1);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.5f,  v1);
}

/* 4. UV with non-zero first_id */
void test_uv_offset_first_id(void)
{
    glyph_atlas_t atlas = {4, 4, 32, 47};
    float u0, v0, u1, v1;
    glyph_batch_uv(atlas, 33, &u0, &v0, &u1, &v1);
    /* glyph 33 -> idx 1, col=1, row=0 */
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.25f, u0);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f,  v0);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.5f,  u1);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.25f, v1);
}

/* 5. UV for non-square atlas (8 cols, 2 rows) */
void test_uv_non_square_atlas(void)
{
    glyph_atlas_t atlas = {8, 2, 0, 15};
    float u0, v0, u1, v1;
    glyph_batch_uv(atlas, 9, &u0, &v0, &u1, &v1);
    /* glyph 9 -> idx 9, col=1, row=1 */
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.125f, u0);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.5f,   v0);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.25f,  u1);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f,   v1);
}

/* 6. UV for out-of-range glyph_id (below first_id) -> fallback (0,0,0,0) */
void test_uv_below_range(void)
{
    glyph_atlas_t atlas = {4, 4, 10, 25};
    float u0, v0, u1, v1;
    glyph_batch_uv(atlas, 5, &u0, &v0, &u1, &v1);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, u0);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, v0);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, u1);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, v1);
}

/* 7. UV for out-of-range glyph_id (above last_id) -> fallback (0,0,0,0) */
void test_uv_above_range(void)
{
    glyph_atlas_t atlas = {4, 4, 0, 15};
    float u0, v0, u1, v1;
    glyph_batch_uv(atlas, 20, &u0, &v0, &u1, &v1);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, u0);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, v0);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, u1);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, v1);
}

/* ===== glyph_batch_create tests ===== */

/* Helper: camera facing +Z: right=(1,0,0), up=(0,1,0) */
static const vec3_t CAM_RIGHT = {1.0f, 0.0f, 0.0f};
static const vec3_t CAM_UP    = {0.0f, 1.0f, 0.0f};

static glyph_instance_t make_inst(int glyph_id, float x, float y, float z,
                                   float scale, float r, float g, float b, float a)
{
    glyph_instance_t inst;
    inst.glyph_id = glyph_id;
    inst.position = vec3_create(x, y, z);
    inst.scale = scale;
    inst.color.r = r;
    inst.color.g = g;
    inst.color.b = b;
    inst.color.a = a;
    return inst;
}

/* 8. Single glyph — correct vertex/index counts */
void test_create_single_counts(void)
{
    glyph_atlas_t atlas = {4, 4, 0, 15};
    glyph_instance_t inst = make_inst(0, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    glyph_batch_t batch = glyph_batch_create(&inst, 1, atlas,
                                              CAM_RIGHT, CAM_UP, 2.0f, 2.0f);
    TEST_ASSERT_EQUAL_INT(4, batch.vertex_count);
    TEST_ASSERT_EQUAL_INT(6, batch.index_count);
    TEST_ASSERT_EQUAL_INT(1, batch.glyph_count);
}

/* 9. Empty batch — count=0 */
void test_create_empty(void)
{
    glyph_atlas_t atlas = {4, 4, 0, 15};
    glyph_batch_t batch = glyph_batch_create(NULL, 0, atlas,
                                              CAM_RIGHT, CAM_UP, 2.0f, 2.0f);
    TEST_ASSERT_EQUAL_INT(0, batch.vertex_count);
    TEST_ASSERT_EQUAL_INT(0, batch.index_count);
    TEST_ASSERT_EQUAL_INT(0, batch.glyph_count);
}

/* 10. Clamp to GLYPH_BATCH_MAX */
void test_create_clamped(void)
{
    glyph_atlas_t atlas = {16, 16, 0, 255};
    glyph_instance_t insts[200];
    for (int i = 0; i < 200; i++)
        insts[i] = make_inst(i % 256, (float)i, 0.0f, 0.0f,
                              1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    glyph_batch_t batch = glyph_batch_create(insts, 200, atlas,
                                              CAM_RIGHT, CAM_UP, 1.0f, 1.0f);
    TEST_ASSERT_EQUAL_INT(GLYPH_BATCH_MAX, batch.glyph_count);
    TEST_ASSERT_EQUAL_INT(GLYPH_BATCH_MAX * 4, batch.vertex_count);
    TEST_ASSERT_EQUAL_INT(GLYPH_BATCH_MAX * 6, batch.index_count);
}

/* 11. Single glyph at origin — billboard quad positions */
void test_create_single_positions(void)
{
    glyph_atlas_t atlas = {4, 4, 0, 15};
    glyph_instance_t inst = make_inst(0, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    glyph_batch_t batch = glyph_batch_create(&inst, 1, atlas,
                                              CAM_RIGHT, CAM_UP, 2.0f, 2.0f);
    float x, y, z, u, v, r, g, b, a;

    /* v0: bottom-left = center - right*1 - up*1 = (-1,-1,0) */
    glyph_batch_read_vertex(&batch, 0, &x, &y, &z, &u, &v, &r, &g, &b, &a);
    TEST_ASSERT_FLOAT_WITHIN(TOL, -1.0f, x);
    TEST_ASSERT_FLOAT_WITHIN(TOL, -1.0f, y);
    TEST_ASSERT_FLOAT_WITHIN(TOL,  0.0f, z);

    /* v1: bottom-right = center + right*1 - up*1 = (1,-1,0) */
    glyph_batch_read_vertex(&batch, 1, &x, &y, &z, &u, &v, &r, &g, &b, &a);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, x);
    TEST_ASSERT_FLOAT_WITHIN(TOL, -1.0f, y);

    /* v2: top-right = (1,1,0) */
    glyph_batch_read_vertex(&batch, 2, &x, &y, &z, &u, &v, &r, &g, &b, &a);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, x);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, y);

    /* v3: top-left = (-1,1,0) */
    glyph_batch_read_vertex(&batch, 3, &x, &y, &z, &u, &v, &r, &g, &b, &a);
    TEST_ASSERT_FLOAT_WITHIN(TOL, -1.0f, x);
    TEST_ASSERT_FLOAT_WITHIN(TOL,  1.0f, y);
}

/* 12. Glyph at offset position */
void test_create_offset_position(void)
{
    glyph_atlas_t atlas = {4, 4, 0, 15};
    glyph_instance_t inst = make_inst(0, 5.0f, 3.0f, -2.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    glyph_batch_t batch = glyph_batch_create(&inst, 1, atlas,
                                              CAM_RIGHT, CAM_UP, 1.0f, 1.0f);
    float x, y, z, u, v, r, g, b, a;

    /* v0: (5,3,-2) - right*0.5 - up*0.5 = (4.5, 2.5, -2) */
    glyph_batch_read_vertex(&batch, 0, &x, &y, &z, &u, &v, &r, &g, &b, &a);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 4.5f, x);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 2.5f, y);
    TEST_ASSERT_FLOAT_WITHIN(TOL, -2.0f, z);
}

/* 13. Per-glyph scale */
void test_create_scale(void)
{
    glyph_atlas_t atlas = {4, 4, 0, 15};
    glyph_instance_t inst = make_inst(0, 0.0f, 0.0f, 0.0f, 2.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    glyph_batch_t batch = glyph_batch_create(&inst, 1, atlas,
                                              CAM_RIGHT, CAM_UP, 2.0f, 2.0f);
    float x, y, z, u, v, r, g, b, a;

    /* base_width=2, scale=2 -> half_w = (2*2)/2 = 2 */
    /* v0: (-2,-2,0), v2: (2,2,0) */
    glyph_batch_read_vertex(&batch, 0, &x, &y, &z, &u, &v, &r, &g, &b, &a);
    TEST_ASSERT_FLOAT_WITHIN(TOL, -2.0f, x);
    TEST_ASSERT_FLOAT_WITHIN(TOL, -2.0f, y);

    glyph_batch_read_vertex(&batch, 2, &x, &y, &z, &u, &v, &r, &g, &b, &a);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 2.0f, x);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 2.0f, y);
}

/* 14. UV mapping in created batch — glyph 0 in 4x4 atlas */
void test_create_uv_mapping(void)
{
    glyph_atlas_t atlas = {4, 4, 0, 15};
    glyph_instance_t inst = make_inst(0, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    glyph_batch_t batch = glyph_batch_create(&inst, 1, atlas,
                                              CAM_RIGHT, CAM_UP, 1.0f, 1.0f);
    float x, y, z, u, v, r, g, b, a;

    /* glyph 0: u0=0, v0=0, u1=0.25, v1=0.25 */
    /* v0 (bottom-left): (u0, v1) = (0, 0.25) */
    glyph_batch_read_vertex(&batch, 0, &x, &y, &z, &u, &v, &r, &g, &b, &a);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f,  u);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.25f, v);

    /* v1 (bottom-right): (u1, v1) = (0.25, 0.25) */
    glyph_batch_read_vertex(&batch, 1, &x, &y, &z, &u, &v, &r, &g, &b, &a);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.25f, u);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.25f, v);

    /* v2 (top-right): (u1, v0) = (0.25, 0) */
    glyph_batch_read_vertex(&batch, 2, &x, &y, &z, &u, &v, &r, &g, &b, &a);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.25f, u);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f,  v);

    /* v3 (top-left): (u0, v0) = (0, 0) */
    glyph_batch_read_vertex(&batch, 3, &x, &y, &z, &u, &v, &r, &g, &b, &a);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, u);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, v);
}

/* 15. Color stored in all 4 vertices */
void test_create_color(void)
{
    glyph_atlas_t atlas = {4, 4, 0, 15};
    glyph_instance_t inst = make_inst(0, 0.0f, 0.0f, 0.0f, 1.0f, 0.8f, 0.2f, 0.5f, 0.7f);
    glyph_batch_t batch = glyph_batch_create(&inst, 1, atlas,
                                              CAM_RIGHT, CAM_UP, 1.0f, 1.0f);
    float x, y, z, u, v, r, g, b, a;

    for (int i = 0; i < 4; i++) {
        glyph_batch_read_vertex(&batch, i, &x, &y, &z, &u, &v, &r, &g, &b, &a);
        TEST_ASSERT_FLOAT_WITHIN(TOL, 0.8f, r);
        TEST_ASSERT_FLOAT_WITHIN(TOL, 0.2f, g);
        TEST_ASSERT_FLOAT_WITHIN(TOL, 0.5f, b);
        TEST_ASSERT_FLOAT_WITHIN(TOL, 0.7f, a);
    }
}

/* 16. Index pattern: {b+0, b+1, b+2, b+2, b+3, b+0} */
void test_create_index_pattern(void)
{
    glyph_atlas_t atlas = {4, 4, 0, 15};
    glyph_instance_t inst = make_inst(0, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    glyph_batch_t batch = glyph_batch_create(&inst, 1, atlas,
                                              CAM_RIGHT, CAM_UP, 1.0f, 1.0f);

    TEST_ASSERT_EQUAL_UINT(0, batch.indices[0]);
    TEST_ASSERT_EQUAL_UINT(1, batch.indices[1]);
    TEST_ASSERT_EQUAL_UINT(2, batch.indices[2]);
    TEST_ASSERT_EQUAL_UINT(2, batch.indices[3]);
    TEST_ASSERT_EQUAL_UINT(3, batch.indices[4]);
    TEST_ASSERT_EQUAL_UINT(0, batch.indices[5]);
}

/* 17. Multi-glyph — second glyph indices offset by 4 */
void test_create_multi_indices(void)
{
    glyph_atlas_t atlas = {4, 4, 0, 15};
    glyph_instance_t insts[2];
    insts[0] = make_inst(0, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f);
    insts[1] = make_inst(1, 5.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f);
    glyph_batch_t batch = glyph_batch_create(insts, 2, atlas,
                                              CAM_RIGHT, CAM_UP, 1.0f, 1.0f);

    TEST_ASSERT_EQUAL_INT(8, batch.vertex_count);
    TEST_ASSERT_EQUAL_INT(12, batch.index_count);

    /* Second glyph: base=4 -> {4,5,6, 6,7,4} */
    TEST_ASSERT_EQUAL_UINT(4, batch.indices[6]);
    TEST_ASSERT_EQUAL_UINT(5, batch.indices[7]);
    TEST_ASSERT_EQUAL_UINT(6, batch.indices[8]);
    TEST_ASSERT_EQUAL_UINT(6, batch.indices[9]);
    TEST_ASSERT_EQUAL_UINT(7, batch.indices[10]);
    TEST_ASSERT_EQUAL_UINT(4, batch.indices[11]);
}

/* 18. Multi-glyph — different colors preserved */
void test_create_multi_colors(void)
{
    glyph_atlas_t atlas = {4, 4, 0, 15};
    glyph_instance_t insts[2];
    insts[0] = make_inst(0, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f);
    insts[1] = make_inst(1, 5.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.5f);
    glyph_batch_t batch = glyph_batch_create(insts, 2, atlas,
                                              CAM_RIGHT, CAM_UP, 1.0f, 1.0f);
    float x, y, z, u, v, r, g, b, a;

    /* Glyph 0, vertex 0: red */
    glyph_batch_read_vertex(&batch, 0, &x, &y, &z, &u, &v, &r, &g, &b, &a);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, r);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, g);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, b);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, a);

    /* Glyph 1, vertex 0 (index 4): blue, alpha 0.5 */
    glyph_batch_read_vertex(&batch, 4, &x, &y, &z, &u, &v, &r, &g, &b, &a);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, r);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, g);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, b);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.5f, a);
}

/* 19. Angled camera vectors */
void test_create_angled_camera(void)
{
    glyph_atlas_t atlas = {4, 4, 0, 15};
    glyph_instance_t inst = make_inst(0, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    /* Camera looking down -Y: right=(1,0,0), up=(0,0,-1) */
    vec3_t right = {1.0f, 0.0f, 0.0f};
    vec3_t up    = {0.0f, 0.0f, -1.0f};
    glyph_batch_t batch = glyph_batch_create(&inst, 1, atlas,
                                              right, up, 2.0f, 2.0f);
    float x, y, z, u, v, r, g, b, a;

    /* v0: (0,0,0) - (1,0,0)*1 - (0,0,-1)*1 = (-1, 0, 1) */
    glyph_batch_read_vertex(&batch, 0, &x, &y, &z, &u, &v, &r, &g, &b, &a);
    TEST_ASSERT_FLOAT_WITHIN(TOL, -1.0f, x);
    TEST_ASSERT_FLOAT_WITHIN(TOL,  0.0f, y);
    TEST_ASSERT_FLOAT_WITHIN(TOL,  1.0f, z);
}

/* 20. Non-uniform base_width/base_height */
void test_create_non_uniform_size(void)
{
    glyph_atlas_t atlas = {4, 4, 0, 15};
    glyph_instance_t inst = make_inst(0, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    glyph_batch_t batch = glyph_batch_create(&inst, 1, atlas,
                                              CAM_RIGHT, CAM_UP, 4.0f, 2.0f);
    float x, y, z, u, v, r, g, b, a;

    /* hw=2, hh=1 -> v0=(-2,-1,0), v2=(2,1,0) */
    glyph_batch_read_vertex(&batch, 0, &x, &y, &z, &u, &v, &r, &g, &b, &a);
    TEST_ASSERT_FLOAT_WITHIN(TOL, -2.0f, x);
    TEST_ASSERT_FLOAT_WITHIN(TOL, -1.0f, y);

    glyph_batch_read_vertex(&batch, 2, &x, &y, &z, &u, &v, &r, &g, &b, &a);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 2.0f, x);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, y);
}

/* 21. Scale=0 produces degenerate quad (all 4 verts at center) */
void test_create_zero_scale(void)
{
    glyph_atlas_t atlas = {4, 4, 0, 15};
    glyph_instance_t inst = make_inst(0, 3.0f, 4.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    glyph_batch_t batch = glyph_batch_create(&inst, 1, atlas,
                                              CAM_RIGHT, CAM_UP, 2.0f, 2.0f);
    float x, y, z, u, v, r, g, b, a;

    for (int i = 0; i < 4; i++) {
        glyph_batch_read_vertex(&batch, i, &x, &y, &z, &u, &v, &r, &g, &b, &a);
        TEST_ASSERT_FLOAT_WITHIN(TOL, 3.0f, x);
        TEST_ASSERT_FLOAT_WITHIN(TOL, 4.0f, y);
        TEST_ASSERT_FLOAT_WITHIN(TOL, 5.0f, z);
    }
}

/* 22. Out-of-range glyph_id gets fallback UV (0,0,0,0) in batch */
void test_create_out_of_range_uv(void)
{
    glyph_atlas_t atlas = {4, 4, 0, 15};
    glyph_instance_t inst = make_inst(99, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    glyph_batch_t batch = glyph_batch_create(&inst, 1, atlas,
                                              CAM_RIGHT, CAM_UP, 1.0f, 1.0f);
    float x, y, z, u, v, r, g, b, a;

    /* All UVs should be 0 */
    for (int i = 0; i < 4; i++) {
        glyph_batch_read_vertex(&batch, i, &x, &y, &z, &u, &v, &r, &g, &b, &a);
        TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, u);
        TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, v);
    }
}

/* 23. All indices valid for multi-glyph batch */
void test_create_all_indices_valid(void)
{
    glyph_atlas_t atlas = {4, 4, 0, 15};
    glyph_instance_t insts[5];
    for (int i = 0; i < 5; i++)
        insts[i] = make_inst(i, (float)i * 2.0f, 0.0f, 0.0f,
                              1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    glyph_batch_t batch = glyph_batch_create(insts, 5, atlas,
                                              CAM_RIGHT, CAM_UP, 1.0f, 1.0f);

    for (int i = 0; i < batch.index_count; i++)
        TEST_ASSERT_TRUE(batch.indices[i] < (unsigned int)batch.vertex_count);
}

/* ===== glyph_batch_create_y_locked tests ===== */

/* 24. Y-locked billboard — up is always (0,1,0) */
void test_ylocked_up_is_y(void)
{
    glyph_atlas_t atlas = {4, 4, 0, 15};
    glyph_instance_t inst = make_inst(0, 0.0f, 0.0f, 5.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    /* Camera at origin looking +Z */
    vec3_t cam_pos = {0.0f, 0.0f, 0.0f};
    glyph_batch_t batch = glyph_batch_create_y_locked(&inst, 1, atlas,
                                                       cam_pos, 2.0f, 2.0f);
    float x, y, z, u, v, r, g, b, a;

    /* Glyph at (0,0,5), camera at (0,0,0).
     * XZ direction: glyph-cam = (0,0,5)-(0,0,0) = (0,0,5), normalize XZ = (0,0,1)
     * right = cross(forward_xz, Y) -> cross((0,0,1),(0,1,0)) = (-1,0,0)...
     * Actually: right = normalize(cross((0,1,0), (0,0,1))) = normalize((1,0,0)) = (1,0,0)
     * Wait, let me think about this more carefully.
     * forward = normalize(glyph_pos - cam_pos) projected to XZ = (0,0,1)
     * right = normalize(cross(Y_up, forward)) = cross((0,1,0),(0,0,1)) = (1,0,0)
     * up = (0,1,0)
     * half_w = 1, half_h = 1
     * v0: (0,0,5) - (1,0,0)*1 - (0,1,0)*1 = (-1,-1,5)
     */
    glyph_batch_read_vertex(&batch, 0, &x, &y, &z, &u, &v, &r, &g, &b, &a);
    TEST_ASSERT_FLOAT_WITHIN(TOL, -1.0f, x);
    TEST_ASSERT_FLOAT_WITHIN(TOL, -1.0f, y);
    TEST_ASSERT_FLOAT_WITHIN(TOL,  5.0f, z);

    /* v2: top-right = (0,0,5) + (1,0,0)*1 + (0,1,0)*1 = (1,1,5) */
    glyph_batch_read_vertex(&batch, 2, &x, &y, &z, &u, &v, &r, &g, &b, &a);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, x);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, y);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 5.0f, z);
}

/* 25. Y-locked — camera off to the side */
void test_ylocked_camera_side(void)
{
    glyph_atlas_t atlas = {4, 4, 0, 15};
    glyph_instance_t inst = make_inst(0, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    /* Camera at (5,0,0) -> glyph faces -X direction */
    vec3_t cam_pos = {5.0f, 0.0f, 0.0f};
    glyph_batch_t batch = glyph_batch_create_y_locked(&inst, 1, atlas,
                                                       cam_pos, 2.0f, 2.0f);
    float x, y, z, u, v, r, g, b, a;

    /* forward (glyph to cam in XZ) = normalize((0,0,0)-(5,0,0)) = (-1,0,0)
     * right = normalize(cross((0,1,0), (-1,0,0))) = normalize((0,0,1)) = (0,0,1)
     * up = (0,1,0)
     * half_w=1, half_h=1
     * v0: (0,0,0) - (0,0,1)*1 - (0,1,0)*1 = (0,-1,-1) */
    glyph_batch_read_vertex(&batch, 0, &x, &y, &z, &u, &v, &r, &g, &b, &a);
    TEST_ASSERT_FLOAT_WITHIN(TOL,  0.0f, x);
    TEST_ASSERT_FLOAT_WITHIN(TOL, -1.0f, y);
    TEST_ASSERT_FLOAT_WITHIN(TOL, -1.0f, z);
}

/* 26. Y-locked — counts match standard create */
void test_ylocked_counts(void)
{
    glyph_atlas_t atlas = {4, 4, 0, 15};
    glyph_instance_t insts[3];
    for (int i = 0; i < 3; i++)
        insts[i] = make_inst(i, (float)i * 2.0f, 0.0f, 0.0f,
                              1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    vec3_t cam_pos = {0.0f, 0.0f, -5.0f};
    glyph_batch_t batch = glyph_batch_create_y_locked(insts, 3, atlas,
                                                       cam_pos, 1.0f, 1.0f);
    TEST_ASSERT_EQUAL_INT(12, batch.vertex_count);
    TEST_ASSERT_EQUAL_INT(18, batch.index_count);
    TEST_ASSERT_EQUAL_INT(3, batch.glyph_count);
}

/* 27. Y-locked — empty batch */
void test_ylocked_empty(void)
{
    glyph_atlas_t atlas = {4, 4, 0, 15};
    vec3_t cam_pos = {0.0f, 0.0f, 0.0f};
    glyph_batch_t batch = glyph_batch_create_y_locked(NULL, 0, atlas,
                                                       cam_pos, 1.0f, 1.0f);
    TEST_ASSERT_EQUAL_INT(0, batch.vertex_count);
    TEST_ASSERT_EQUAL_INT(0, batch.index_count);
    TEST_ASSERT_EQUAL_INT(0, batch.glyph_count);
}

/* ===== Size query tests ===== */

/* 28. vertex_floats = vertex_count * GLYPH_VERTEX_STRIDE */
void test_vertex_floats(void)
{
    glyph_atlas_t atlas = {4, 4, 0, 15};
    glyph_instance_t insts[3];
    for (int i = 0; i < 3; i++)
        insts[i] = make_inst(i, (float)i, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    glyph_batch_t batch = glyph_batch_create(insts, 3, atlas,
                                              CAM_RIGHT, CAM_UP, 1.0f, 1.0f);
    TEST_ASSERT_EQUAL_INT(12 * GLYPH_VERTEX_STRIDE, glyph_batch_vertex_floats(&batch));
}

/* 29. vertex_bytes = vertex_floats * sizeof(float) */
void test_vertex_bytes(void)
{
    glyph_atlas_t atlas = {4, 4, 0, 15};
    glyph_instance_t inst = make_inst(0, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    glyph_batch_t batch = glyph_batch_create(&inst, 1, atlas,
                                              CAM_RIGHT, CAM_UP, 1.0f, 1.0f);
    TEST_ASSERT_EQUAL_INT(4 * GLYPH_VERTEX_STRIDE * (int)sizeof(float),
                          glyph_batch_vertex_bytes(&batch));
}

/* 30. index_bytes = index_count * sizeof(unsigned int) */
void test_index_bytes(void)
{
    glyph_atlas_t atlas = {4, 4, 0, 15};
    glyph_instance_t inst = make_inst(0, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    glyph_batch_t batch = glyph_batch_create(&inst, 1, atlas,
                                              CAM_RIGHT, CAM_UP, 1.0f, 1.0f);
    TEST_ASSERT_EQUAL_INT(6 * (int)sizeof(unsigned int),
                          glyph_batch_index_bytes(&batch));
}

/* ===== Purity test ===== */

/* 31. Same input produces same output */
void test_purity(void)
{
    glyph_atlas_t atlas = {4, 4, 0, 15};
    glyph_instance_t inst = make_inst(5, 3.0f, 1.0f, -1.0f, 1.5f,
                                       0.5f, 0.6f, 0.7f, 0.8f);
    glyph_batch_t a = glyph_batch_create(&inst, 1, atlas,
                                          CAM_RIGHT, CAM_UP, 2.0f, 1.5f);
    glyph_batch_t b = glyph_batch_create(&inst, 1, atlas,
                                          CAM_RIGHT, CAM_UP, 2.0f, 1.5f);

    TEST_ASSERT_EQUAL_INT(a.vertex_count, b.vertex_count);
    TEST_ASSERT_EQUAL_INT(a.index_count, b.index_count);
    TEST_ASSERT_EQUAL_INT(a.glyph_count, b.glyph_count);

    int nf = glyph_batch_vertex_floats(&a);
    for (int i = 0; i < nf; i++)
        TEST_ASSERT_EQUAL_FLOAT(a.vertices[i], b.vertices[i]);

    for (int i = 0; i < a.index_count; i++)
        TEST_ASSERT_EQUAL_UINT(a.indices[i], b.indices[i]);
}

/* 32. UV for glyph at exact boundary of atlas row */
void test_uv_row_boundary(void)
{
    glyph_atlas_t atlas = {4, 4, 0, 15};
    float u0, v0, u1, v1;
    /* glyph 4 -> idx 4, col=0, row=1 */
    glyph_batch_uv(atlas, 4, &u0, &v0, &u1, &v1);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f,  u0);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.25f, v0);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.25f, u1);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.5f,  v1);
}

/* 33. Y-locked — glyph directly above camera (XZ diff is zero → fallback) */
void test_ylocked_directly_above(void)
{
    glyph_atlas_t atlas = {4, 4, 0, 15};
    glyph_instance_t inst = make_inst(0, 0.0f, 10.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    vec3_t cam_pos = {0.0f, 0.0f, 0.0f};
    glyph_batch_t batch = glyph_batch_create_y_locked(&inst, 1, atlas,
                                                       cam_pos, 2.0f, 2.0f);
    /* When XZ diff is zero, the right vector degenerates.
     * Should produce a valid batch (not crash), degenerate quad is OK. */
    TEST_ASSERT_EQUAL_INT(4, batch.vertex_count);
    TEST_ASSERT_EQUAL_INT(6, batch.index_count);
}

int main(void)
{
    UNITY_BEGIN();

    /* UV computation */
    RUN_TEST(test_uv_first_glyph);
    RUN_TEST(test_uv_last_glyph);
    RUN_TEST(test_uv_middle_glyph);
    RUN_TEST(test_uv_offset_first_id);
    RUN_TEST(test_uv_non_square_atlas);
    RUN_TEST(test_uv_below_range);
    RUN_TEST(test_uv_above_range);

    /* glyph_batch_create */
    RUN_TEST(test_create_single_counts);
    RUN_TEST(test_create_empty);
    RUN_TEST(test_create_clamped);
    RUN_TEST(test_create_single_positions);
    RUN_TEST(test_create_offset_position);
    RUN_TEST(test_create_scale);
    RUN_TEST(test_create_uv_mapping);
    RUN_TEST(test_create_color);
    RUN_TEST(test_create_index_pattern);
    RUN_TEST(test_create_multi_indices);
    RUN_TEST(test_create_multi_colors);
    RUN_TEST(test_create_angled_camera);
    RUN_TEST(test_create_non_uniform_size);
    RUN_TEST(test_create_zero_scale);
    RUN_TEST(test_create_out_of_range_uv);
    RUN_TEST(test_create_all_indices_valid);

    /* glyph_batch_create_y_locked */
    RUN_TEST(test_ylocked_up_is_y);
    RUN_TEST(test_ylocked_camera_side);
    RUN_TEST(test_ylocked_counts);
    RUN_TEST(test_ylocked_empty);
    RUN_TEST(test_ylocked_directly_above);

    /* Size queries */
    RUN_TEST(test_vertex_floats);
    RUN_TEST(test_vertex_bytes);
    RUN_TEST(test_index_bytes);

    /* Purity */
    RUN_TEST(test_purity);
    RUN_TEST(test_uv_row_boundary);

    return UNITY_END();
}
