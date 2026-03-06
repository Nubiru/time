#include "../unity/unity.h"
#include "../../src/render/ring_geometry.h"

#include <math.h>

#define TOL 0.001f
#define NEAR(exp, act) ((act) >= (exp) - TOL && (act) <= (exp) + TOL)

void setUp(void) {}
void tearDown(void) {}

/* 1. Mesh size with segments_per_sign=1 */
void test_mesh_size_1(void)
{
    ring_mesh_info_t info = ring_mesh_size(1);
    TEST_ASSERT_EQUAL_INT(26, info.vertex_count);   /* (12+1)*2 */
    TEST_ASSERT_EQUAL_INT(72, info.index_count);     /* 12*6 */
}

/* 2. Mesh size with segments_per_sign=4 */
void test_mesh_size_4(void)
{
    ring_mesh_info_t info = ring_mesh_size(4);
    TEST_ASSERT_EQUAL_INT(98, info.vertex_count);   /* (48+1)*2 */
    TEST_ASSERT_EQUAL_INT(288, info.index_count);   /* 48*6 */
}

/* 3. Inner vertices at correct radius */
void test_positions_inner_radius(void)
{
    float pos[78]; /* 26 verts * 3 */
    ring_generate_positions(pos, 5.0f, 8.0f, 1);
    for (int i = 0; i < 13; i++) {
        int v = i * 2; /* inner vertex */
        float x = pos[v * 3 + 0];
        float z = pos[v * 3 + 2];
        float r = sqrtf(x * x + z * z);
        TEST_ASSERT_TRUE(NEAR(5.0f, r));
    }
}

/* 4. Outer vertices at correct radius */
void test_positions_outer_radius(void)
{
    float pos[78];
    ring_generate_positions(pos, 5.0f, 8.0f, 1);
    for (int i = 0; i < 13; i++) {
        int v = i * 2 + 1; /* outer vertex */
        float x = pos[v * 3 + 0];
        float z = pos[v * 3 + 2];
        float r = sqrtf(x * x + z * z);
        TEST_ASSERT_TRUE(NEAR(8.0f, r));
    }
}

/* 5. All y coordinates are 0 (ecliptic plane) */
void test_positions_y_zero(void)
{
    float pos[78];
    ring_generate_positions(pos, 5.0f, 8.0f, 1);
    for (int v = 0; v < 26; v++)
        TEST_ASSERT_TRUE(pos[v * 3 + 1] == 0.0f);
}

/* 6. First vertex at (inner_r, 0, 0) — 0 degrees */
void test_positions_first_vertex(void)
{
    float pos[78];
    ring_generate_positions(pos, 5.0f, 8.0f, 1);
    TEST_ASSERT_TRUE(NEAR(5.0f, pos[0]));  /* x */
    TEST_ASSERT_TRUE(NEAR(0.0f, pos[2]));  /* z */
}

/* 7. Last column same position as first (360 = 0) */
void test_positions_last_matches_first(void)
{
    float pos[78];
    ring_generate_positions(pos, 5.0f, 8.0f, 1);
    /* Last inner vertex = index 24, first inner = index 0 */
    TEST_ASSERT_TRUE(NEAR(pos[0], pos[24 * 3 + 0]));
    TEST_ASSERT_TRUE(NEAR(pos[2], pos[24 * 3 + 2]));
}

/* 8. UV U values in [0, 1] */
void test_uvs_u_range(void)
{
    float uvs[52]; /* 26 * 2 */
    ring_generate_uvs(uvs, 1);
    for (int v = 0; v < 26; v++) {
        float u = uvs[v * 2 + 0];
        TEST_ASSERT_TRUE(u >= -TOL && u <= 1.0f + TOL);
    }
}

/* 9. UV V values are 0 (inner) or 1 (outer) */
void test_uvs_v_values(void)
{
    float uvs[52];
    ring_generate_uvs(uvs, 1);
    for (int i = 0; i < 13; i++) {
        TEST_ASSERT_TRUE(NEAR(0.0f, uvs[(i * 2) * 2 + 1]));      /* inner */
        TEST_ASSERT_TRUE(NEAR(1.0f, uvs[(i * 2 + 1) * 2 + 1]));  /* outer */
    }
}

/* 10. First column U = 0 */
void test_uvs_first_zero(void)
{
    float uvs[52];
    ring_generate_uvs(uvs, 1);
    TEST_ASSERT_TRUE(NEAR(0.0f, uvs[0]));
}

/* 11. Last column U = 1 */
void test_uvs_last_one(void)
{
    float uvs[52];
    ring_generate_uvs(uvs, 1);
    /* Last column inner = vertex 24 */
    TEST_ASSERT_TRUE(NEAR(1.0f, uvs[24 * 2 + 0]));
}

/* 12. All indices < vertex_count */
void test_indices_valid(void)
{
    unsigned int idx[72];
    ring_generate_indices(idx, 1);
    ring_mesh_info_t info = ring_mesh_size(1);
    for (int i = 0; i < 72; i++)
        TEST_ASSERT_TRUE(idx[i] < (unsigned int)info.vertex_count);
}

/* 13. Index count divisible by 3 (triangles) */
void test_indices_triangles(void)
{
    ring_mesh_info_t info = ring_mesh_size(1);
    TEST_ASSERT_EQUAL_INT(0, info.index_count % 3);
}

/* 14. First vertex → sign 0 */
void test_sign_first(void)
{
    TEST_ASSERT_EQUAL_INT(0, ring_vertex_sign(0, 1));
    TEST_ASSERT_EQUAL_INT(0, ring_vertex_sign(1, 1));
}

/* 15. Last vertices → sign 11 */
void test_sign_last(void)
{
    ring_mesh_info_t info = ring_mesh_size(1);
    TEST_ASSERT_EQUAL_INT(11, ring_vertex_sign(info.vertex_count - 1, 1));
    TEST_ASSERT_EQUAL_INT(11, ring_vertex_sign(info.vertex_count - 2, 1));
}

/* 16. All 12 signs represented */
void test_sign_all_12(void)
{
    int seen[12] = {0};
    ring_mesh_info_t info = ring_mesh_size(1);
    for (int v = 0; v < info.vertex_count; v++)
        seen[ring_vertex_sign(v, 1)] = 1;
    for (int s = 0; s < 12; s++)
        TEST_ASSERT_EQUAL_INT(1, seen[s]);
}

/* 17. First vertex angle = 0 */
void test_angle_first(void)
{
    TEST_ASSERT_TRUE(NEAR(0.0f, ring_vertex_angle(0, 1)));
}

/* 18. Angles increase monotonically */
void test_angle_monotonic(void)
{
    ring_mesh_info_t info = ring_mesh_size(2);
    float prev = -1.0f;
    int total_columns = 12 * 2 + 1;
    for (int col = 0; col < total_columns; col++) {
        int v = col * 2; /* inner vertex of each column */
        float a = ring_vertex_angle(v, 2);
        TEST_ASSERT_TRUE(a >= prev);
        prev = a;
    }
    (void)info;
}

/* 19. Last column angle = 360 */
void test_angle_last(void)
{
    ring_mesh_info_t info = ring_mesh_size(1);
    float a = ring_vertex_angle(info.vertex_count - 2, 1);
    TEST_ASSERT_TRUE(NEAR(360.0f, a));
}

/* 20. Purity */
void test_purity(void)
{
    float pos_a[78], pos_b[78];
    ring_generate_positions(pos_a, 3.0f, 6.0f, 1);
    ring_generate_positions(pos_b, 3.0f, 6.0f, 1);
    for (int i = 0; i < 78; i++)
        TEST_ASSERT_TRUE(pos_a[i] == pos_b[i]);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_mesh_size_1);
    RUN_TEST(test_mesh_size_4);
    RUN_TEST(test_positions_inner_radius);
    RUN_TEST(test_positions_outer_radius);
    RUN_TEST(test_positions_y_zero);
    RUN_TEST(test_positions_first_vertex);
    RUN_TEST(test_positions_last_matches_first);
    RUN_TEST(test_uvs_u_range);
    RUN_TEST(test_uvs_v_values);
    RUN_TEST(test_uvs_first_zero);
    RUN_TEST(test_uvs_last_one);
    RUN_TEST(test_indices_valid);
    RUN_TEST(test_indices_triangles);
    RUN_TEST(test_sign_first);
    RUN_TEST(test_sign_last);
    RUN_TEST(test_sign_all_12);
    RUN_TEST(test_angle_first);
    RUN_TEST(test_angle_monotonic);
    RUN_TEST(test_angle_last);
    RUN_TEST(test_purity);
    return UNITY_END();
}
