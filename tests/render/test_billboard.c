#include "../unity/unity.h"
#include "../../src/render/billboard.h"

#include <math.h>

#define PI 3.14159265358979323846
#define TOL 0.01f
#define NEAR(exp, act) ((act) >= (exp) - TOL && (act) <= (exp) + TOL)

void setUp(void) {}
void tearDown(void) {}

/* --- billboard_create tests --- */

/* 1. Quad has 6 indices (2 triangles) */
void test_create_indices(void)
{
    billboard_quad_t q = billboard_create(
        vec3_create(0, 0, 0),
        vec3_create(1, 0, 0), vec3_create(0, 1, 0),
        2.0f, 1.0f);
    TEST_ASSERT_EQUAL_UINT(0, q.indices[0]);
    TEST_ASSERT_EQUAL_UINT(1, q.indices[1]);
    TEST_ASSERT_EQUAL_UINT(2, q.indices[2]);
    TEST_ASSERT_EQUAL_UINT(0, q.indices[3]);
    TEST_ASSERT_EQUAL_UINT(2, q.indices[4]);
    TEST_ASSERT_EQUAL_UINT(3, q.indices[5]);
}

/* 2. Vertices centered on position */
void test_create_centered(void)
{
    billboard_quad_t q = billboard_create(
        vec3_create(5, 3, 7),
        vec3_create(1, 0, 0), vec3_create(0, 1, 0),
        2.0f, 2.0f);
    /* Average of 4 vertices should be center */
    float avg_x = 0, avg_y = 0, avg_z = 0;
    for (int i = 0; i < 4; i++) {
        avg_x += q.positions[i * 3 + 0];
        avg_y += q.positions[i * 3 + 1];
        avg_z += q.positions[i * 3 + 2];
    }
    TEST_ASSERT_TRUE(NEAR(5.0f, avg_x / 4.0f));
    TEST_ASSERT_TRUE(NEAR(3.0f, avg_y / 4.0f));
    TEST_ASSERT_TRUE(NEAR(7.0f, avg_z / 4.0f));
}

/* 3. Vertices at correct distance from center */
void test_create_size(void)
{
    billboard_quad_t q = billboard_create(
        vec3_create(0, 0, 0),
        vec3_create(1, 0, 0), vec3_create(0, 1, 0),
        4.0f, 2.0f);
    /* Bottom-left should be at (-2, -1, 0) */
    TEST_ASSERT_TRUE(NEAR(-2.0f, q.positions[0]));
    TEST_ASSERT_TRUE(NEAR(-1.0f, q.positions[1]));
    /* Top-right should be at (2, 1, 0) */
    TEST_ASSERT_TRUE(NEAR(2.0f, q.positions[6]));
    TEST_ASSERT_TRUE(NEAR(1.0f, q.positions[7]));
}

/* 4. UVs correct */
void test_create_uvs(void)
{
    billboard_quad_t q = billboard_create(
        vec3_create(0, 0, 0),
        vec3_create(1, 0, 0), vec3_create(0, 1, 0),
        1.0f, 1.0f);
    /* BL(0,0) BR(1,0) TR(1,1) TL(0,1) */
    TEST_ASSERT_TRUE(q.uvs[0] == 0.0f && q.uvs[1] == 0.0f);
    TEST_ASSERT_TRUE(q.uvs[2] == 1.0f && q.uvs[3] == 0.0f);
    TEST_ASSERT_TRUE(q.uvs[4] == 1.0f && q.uvs[5] == 1.0f);
    TEST_ASSERT_TRUE(q.uvs[6] == 0.0f && q.uvs[7] == 1.0f);
}

/* 5. Quad faces camera (all vertices in plane perpendicular to forward) */
void test_create_faces_camera(void)
{
    vec3_t right = vec3_create(1, 0, 0);
    vec3_t up = vec3_create(0, 1, 0);
    billboard_quad_t q = billboard_create(
        vec3_create(0, 0, -5), right, up, 2.0f, 2.0f);
    /* All z should be -5 (quad in the plane z=-5) */
    for (int i = 0; i < 4; i++)
        TEST_ASSERT_TRUE(NEAR(-5.0f, q.positions[i * 3 + 2]));
}

/* 6. Non-axis-aligned camera basis */
void test_create_rotated_camera(void)
{
    /* Camera tilted 45 degrees */
    float s = (float)(1.0 / sqrt(2.0));
    vec3_t right = vec3_create(s, 0, -s);
    vec3_t up = vec3_create(0, 1, 0);
    billboard_quad_t q = billboard_create(
        vec3_create(0, 0, 0), right, up, 2.0f, 2.0f);
    /* Center should still be (0,0,0) */
    float avg_x = 0, avg_z = 0;
    for (int i = 0; i < 4; i++) {
        avg_x += q.positions[i * 3 + 0];
        avg_z += q.positions[i * 3 + 2];
    }
    TEST_ASSERT_TRUE(NEAR(0.0f, avg_x / 4.0f));
    TEST_ASSERT_TRUE(NEAR(0.0f, avg_z / 4.0f));
}

/* --- billboard_create_y_locked tests --- */

/* 7. Y-locked billboard stays upright (up = Y) */
void test_y_locked_upright(void)
{
    billboard_quad_t q = billboard_create_y_locked(
        vec3_create(0, 0, 0), vec3_create(0, 0, 10), 2.0f, 2.0f);
    /* Top-left and top-right should have y > bottom vertices */
    TEST_ASSERT_TRUE(q.positions[7] > q.positions[1]);  /* TL.y > BL.y */
    TEST_ASSERT_TRUE(q.positions[10] > q.positions[4]); /* TR.y > BR.y */
}

/* 8. Y-locked right vector is horizontal */
void test_y_locked_horizontal(void)
{
    billboard_quad_t q = billboard_create_y_locked(
        vec3_create(0, 0, 0), vec3_create(5, 3, 10), 2.0f, 2.0f);
    /* Bottom-left to bottom-right difference should have y=0 */
    float dy = q.positions[4] - q.positions[1]; /* BR.y - BL.y */
    TEST_ASSERT_TRUE(NEAR(0.0f, dy));
}

/* --- billboard_camera_vectors tests --- */

/* 9. Identity view: right=(1,0,0), up=(0,1,0) */
void test_camera_vectors_identity(void)
{
    mat4_t id = mat4_identity();
    vec3_t right, up;
    billboard_camera_vectors(&id, &right, &up);
    TEST_ASSERT_TRUE(NEAR(1.0f, right.x));
    TEST_ASSERT_TRUE(NEAR(0.0f, right.y));
    TEST_ASSERT_TRUE(NEAR(0.0f, right.z));
    TEST_ASSERT_TRUE(NEAR(0.0f, up.x));
    TEST_ASSERT_TRUE(NEAR(1.0f, up.y));
    TEST_ASSERT_TRUE(NEAR(0.0f, up.z));
}

/* 10. look_at view gives correct vectors */
void test_camera_vectors_look_at(void)
{
    mat4_t v = mat4_look_at(
        vec3_create(0, 0, 10), vec3_create(0, 0, 0), vec3_create(0, 1, 0));
    vec3_t right, up;
    billboard_camera_vectors(&v, &right, &up);
    TEST_ASSERT_TRUE(NEAR(1.0f, right.x));
    TEST_ASSERT_TRUE(NEAR(0.0f, right.y));
    TEST_ASSERT_TRUE(NEAR(0.0f, up.x));
    TEST_ASSERT_TRUE(NEAR(1.0f, up.y));
}

/* --- billboard_size_for_pixels tests --- */

/* 11. Size proportional to distance */
void test_size_proportional_distance(void)
{
    float s1 = billboard_size_for_pixels((float)(PI / 4), 10.0f, 600.0f, 32.0f);
    float s2 = billboard_size_for_pixels((float)(PI / 4), 20.0f, 600.0f, 32.0f);
    TEST_ASSERT_TRUE(NEAR(s1 * 2.0f, s2));
}

/* 12. Size proportional to desired pixels */
void test_size_proportional_pixels(void)
{
    float s1 = billboard_size_for_pixels((float)(PI / 4), 10.0f, 600.0f, 16.0f);
    float s2 = billboard_size_for_pixels((float)(PI / 4), 10.0f, 600.0f, 32.0f);
    TEST_ASSERT_TRUE(NEAR(s1 * 2.0f, s2));
}

/* 13. Size inversely proportional to screen height */
void test_size_inverse_screen(void)
{
    float s1 = billboard_size_for_pixels((float)(PI / 4), 10.0f, 600.0f, 32.0f);
    float s2 = billboard_size_for_pixels((float)(PI / 4), 10.0f, 1200.0f, 32.0f);
    TEST_ASSERT_TRUE(NEAR(s1 / 2.0f, s2));
}

/* 14. Zero screen height returns 0 */
void test_size_zero_screen(void)
{
    float s = billboard_size_for_pixels((float)(PI / 4), 10.0f, 0.0f, 32.0f);
    TEST_ASSERT_TRUE(s == 0.0f);
}

/* --- billboard_batch tests --- */

/* 15. Batch vertex count */
void test_batch_count(void)
{
    vec3_t centers[3] = {{0,0,0}, {1,0,0}, {2,0,0}};
    float positions[36]; /* 3*12 */
    float uvs[24];       /* 3*8 */
    unsigned int indices[18]; /* 3*6 */
    billboard_batch(centers, 3,
                    vec3_create(1, 0, 0), vec3_create(0, 1, 0),
                    1.0f, 1.0f,
                    positions, uvs, indices);
    /* First billboard at center (0,0,0) — verify positions exist */
    float avg = 0;
    for (int i = 0; i < 4; i++)
        avg += positions[i * 3 + 0];
    TEST_ASSERT_TRUE(NEAR(0.0f, avg / 4.0f));
}

/* 16. Batch indices offset correctly */
void test_batch_indices_offset(void)
{
    vec3_t centers[2] = {{0,0,0}, {5,0,0}};
    float positions[24];
    float uvs[16];
    unsigned int indices[12];
    billboard_batch(centers, 2,
                    vec3_create(1, 0, 0), vec3_create(0, 1, 0),
                    1.0f, 1.0f,
                    positions, uvs, indices);
    /* Second billboard indices should start at vertex 4 */
    TEST_ASSERT_EQUAL_UINT(4, indices[6]);
    TEST_ASSERT_EQUAL_UINT(5, indices[7]);
    TEST_ASSERT_EQUAL_UINT(6, indices[8]);
    TEST_ASSERT_EQUAL_UINT(4, indices[9]);
    TEST_ASSERT_EQUAL_UINT(6, indices[10]);
    TEST_ASSERT_EQUAL_UINT(7, indices[11]);
}

/* 17. Batch second billboard centered correctly */
void test_batch_second_center(void)
{
    vec3_t centers[2] = {{0,0,0}, {10,0,0}};
    float positions[24];
    float uvs[16];
    unsigned int indices[12];
    billboard_batch(centers, 2,
                    vec3_create(1, 0, 0), vec3_create(0, 1, 0),
                    2.0f, 2.0f,
                    positions, uvs, indices);
    /* Second billboard center: average of vertices 4-7 x coords */
    float avg = 0;
    for (int i = 4; i < 8; i++)
        avg += positions[i * 3 + 0];
    TEST_ASSERT_TRUE(NEAR(10.0f, avg / 4.0f));
}

/* 18. Batch matches single create */
void test_batch_matches_single(void)
{
    vec3_t center = vec3_create(3, 4, 5);
    vec3_t right = vec3_create(1, 0, 0);
    vec3_t up = vec3_create(0, 1, 0);

    billboard_quad_t single = billboard_create(center, right, up, 2.0f, 1.0f);

    float positions[12];
    float uvs[8];
    unsigned int indices[6];
    billboard_batch(&center, 1, right, up, 2.0f, 1.0f, positions, uvs, indices);

    for (int i = 0; i < 12; i++)
        TEST_ASSERT_TRUE(single.positions[i] == positions[i]);
    for (int i = 0; i < 8; i++)
        TEST_ASSERT_TRUE(single.uvs[i] == uvs[i]);
}

/* 19. Purity of billboard_create */
void test_create_purity(void)
{
    billboard_quad_t a = billboard_create(
        vec3_create(1, 2, 3),
        vec3_create(1, 0, 0), vec3_create(0, 1, 0),
        3.0f, 2.0f);
    billboard_quad_t b = billboard_create(
        vec3_create(1, 2, 3),
        vec3_create(1, 0, 0), vec3_create(0, 1, 0),
        3.0f, 2.0f);
    for (int i = 0; i < 12; i++)
        TEST_ASSERT_TRUE(a.positions[i] == b.positions[i]);
}

/* 20. Size is positive for valid inputs */
void test_size_positive(void)
{
    float s = billboard_size_for_pixels((float)(PI / 4), 10.0f, 600.0f, 32.0f);
    TEST_ASSERT_TRUE(s > 0.0f);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_create_indices);
    RUN_TEST(test_create_centered);
    RUN_TEST(test_create_size);
    RUN_TEST(test_create_uvs);
    RUN_TEST(test_create_faces_camera);
    RUN_TEST(test_create_rotated_camera);
    RUN_TEST(test_y_locked_upright);
    RUN_TEST(test_y_locked_horizontal);
    RUN_TEST(test_camera_vectors_identity);
    RUN_TEST(test_camera_vectors_look_at);
    RUN_TEST(test_size_proportional_distance);
    RUN_TEST(test_size_proportional_pixels);
    RUN_TEST(test_size_inverse_screen);
    RUN_TEST(test_size_zero_screen);
    RUN_TEST(test_batch_count);
    RUN_TEST(test_batch_indices_offset);
    RUN_TEST(test_batch_second_center);
    RUN_TEST(test_batch_matches_single);
    RUN_TEST(test_create_purity);
    RUN_TEST(test_size_positive);
    return UNITY_END();
}
