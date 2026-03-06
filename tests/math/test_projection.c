#include "../unity/unity.h"
#include "../../src/math/projection.h"

#include <math.h>

#define PI 3.14159265358979323846
#define TOL 1.0f
#define DTOL 0.05f
#define NEAR(exp, act) ((act) >= (exp) - TOL && (act) <= (exp) + TOL)
#define DNEAR(exp, act) ((act) >= (exp) - DTOL && (act) <= (exp) + DTOL)

void setUp(void) {}
void tearDown(void) {}

/* Standard test camera: at (0,0,10), looking at origin, up=(0,1,0) */
static mat4_t test_view(void)
{
    return mat4_look_at(vec3_create(0, 0, 10),
                        vec3_create(0, 0, 0),
                        vec3_create(0, 1, 0));
}

static mat4_t test_proj(void)
{
    return mat4_perspective((float)(PI / 4.0), 800.0f / 600.0f, 0.1f, 100.0f);
}

/* 1. Origin projects to screen center */
void test_project_origin_center(void)
{
    mat4_t v = test_view();
    mat4_t p = test_proj();
    screen_point_t sp = project_to_screen(vec3_create(0, 0, 0), &v, &p, 800, 600);
    TEST_ASSERT_TRUE(NEAR(400.0f, sp.x));
    TEST_ASSERT_TRUE(NEAR(300.0f, sp.y));
}

/* 2. Origin is visible */
void test_project_origin_visible(void)
{
    mat4_t v = test_view();
    mat4_t p = test_proj();
    screen_point_t sp = project_to_screen(vec3_create(0, 0, 0), &v, &p, 800, 600);
    TEST_ASSERT_EQUAL_INT(1, sp.visible);
}

/* 3. Behind-camera point not visible */
void test_project_behind_not_visible(void)
{
    mat4_t v = test_view();
    mat4_t p = test_proj();
    screen_point_t sp = project_to_screen(vec3_create(0, 0, 15), &v, &p, 800, 600);
    TEST_ASSERT_EQUAL_INT(0, sp.visible);
}

/* 4. Point at +X projects right of center */
void test_project_right(void)
{
    mat4_t v = test_view();
    mat4_t p = test_proj();
    screen_point_t sp = project_to_screen(vec3_create(1, 0, 0), &v, &p, 800, 600);
    TEST_ASSERT_TRUE(sp.x > 400.0f);
    TEST_ASSERT_TRUE(NEAR(300.0f, sp.y));
}

/* 5. Point at +Y projects above center (lower screen Y) */
void test_project_up(void)
{
    mat4_t v = test_view();
    mat4_t p = test_proj();
    screen_point_t sp = project_to_screen(vec3_create(0, 1, 0), &v, &p, 800, 600);
    TEST_ASSERT_TRUE(NEAR(400.0f, sp.x));
    TEST_ASSERT_TRUE(sp.y < 300.0f);
}

/* 6. Depth between 0 and 1 for visible points */
void test_project_depth_range(void)
{
    mat4_t v = test_view();
    mat4_t p = test_proj();
    screen_point_t sp = project_to_screen(vec3_create(0, 0, 0), &v, &p, 800, 600);
    TEST_ASSERT_TRUE(sp.depth >= 0.0f && sp.depth <= 1.0f);
}

/* 7. Closer point has smaller depth */
void test_project_depth_ordering(void)
{
    mat4_t v = test_view();
    mat4_t p = test_proj();
    screen_point_t near_pt = project_to_screen(vec3_create(0, 0, 5), &v, &p, 800, 600);
    screen_point_t far_pt = project_to_screen(vec3_create(0, 0, -10), &v, &p, 800, 600);
    TEST_ASSERT_TRUE(near_pt.visible);
    TEST_ASSERT_TRUE(far_pt.visible);
    TEST_ASSERT_TRUE(near_pt.depth < far_pt.depth);
}

/* 8. Identity matrices project origin to screen center */
void test_project_identity(void)
{
    mat4_t id = mat4_identity();
    screen_point_t sp = project_to_screen(vec3_create(0, 0, 0), &id, &id, 800, 600);
    TEST_ASSERT_TRUE(NEAR(400.0f, sp.x));
    TEST_ASSERT_TRUE(NEAR(300.0f, sp.y));
    TEST_ASSERT_EQUAL_INT(1, sp.visible);
}

/* 9. Unproject center ray points toward -Z */
void test_unproject_center(void)
{
    mat4_t v = test_view();
    mat4_t p = test_proj();
    vec3_t ray = unproject_ray(400, 300, &v, &p, 800, 600);
    TEST_ASSERT_TRUE(DNEAR(0.0f, ray.x));
    TEST_ASSERT_TRUE(DNEAR(0.0f, ray.y));
    TEST_ASSERT_TRUE(ray.z < -0.9f);
}

/* 10. Unproject ray is normalized */
void test_unproject_normalized(void)
{
    mat4_t v = test_view();
    mat4_t p = test_proj();
    vec3_t ray = unproject_ray(400, 300, &v, &p, 800, 600);
    float len = vec3_length(ray);
    TEST_ASSERT_TRUE(DNEAR(1.0f, len));
}

/* 11. Unproject right-of-center has positive X component */
void test_unproject_right(void)
{
    mat4_t v = test_view();
    mat4_t p = test_proj();
    vec3_t ray = unproject_ray(600, 300, &v, &p, 800, 600);
    TEST_ASSERT_TRUE(ray.x > 0.0f);
}

/* 12. point_near_screen hit at center */
void test_near_screen_hit(void)
{
    mat4_t v = test_view();
    mat4_t p = test_proj();
    int hit = point_near_screen(vec3_create(0, 0, 0),
                                400, 300, 10.0f,
                                &v, &p, 800, 600);
    TEST_ASSERT_EQUAL_INT(1, hit);
}

/* 13. point_near_screen miss (cursor far from projection) */
void test_near_screen_miss(void)
{
    mat4_t v = test_view();
    mat4_t p = test_proj();
    int hit = point_near_screen(vec3_create(0, 0, 0),
                                0, 0, 10.0f,
                                &v, &p, 800, 600);
    TEST_ASSERT_EQUAL_INT(0, hit);
}

/* 14. point_near_screen miss for behind-camera point */
void test_near_screen_behind(void)
{
    mat4_t v = test_view();
    mat4_t p = test_proj();
    int hit = point_near_screen(vec3_create(0, 0, 15),
                                400, 300, 1000.0f,
                                &v, &p, 800, 600);
    TEST_ASSERT_EQUAL_INT(0, hit);
}

/* 15. screen_distance of same point = 0 */
void test_distance_same_point(void)
{
    mat4_t v = test_view();
    mat4_t p = test_proj();
    float d = screen_distance(vec3_create(0, 0, 0),
                              vec3_create(0, 0, 0),
                              &v, &p, 800, 600);
    TEST_ASSERT_TRUE(DNEAR(0.0f, d));
}

/* 16. screen_distance is symmetric */
void test_distance_symmetric(void)
{
    mat4_t v = test_view();
    mat4_t p = test_proj();
    vec3_t a = vec3_create(1, 0, 0);
    vec3_t b = vec3_create(-1, 0, 0);
    float d1 = screen_distance(a, b, &v, &p, 800, 600);
    float d2 = screen_distance(b, a, &v, &p, 800, 600);
    TEST_ASSERT_TRUE(DNEAR(d1, d2));
}

/* 17. screen_distance > 0 for distinct visible points */
void test_distance_positive(void)
{
    mat4_t v = test_view();
    mat4_t p = test_proj();
    float d = screen_distance(vec3_create(1, 0, 0),
                              vec3_create(-1, 0, 0),
                              &v, &p, 800, 600);
    TEST_ASSERT_TRUE(d > 0.0f);
}

/* 18. screen_distance returns -1 for behind-camera point */
void test_distance_behind(void)
{
    mat4_t v = test_view();
    mat4_t p = test_proj();
    float d = screen_distance(vec3_create(0, 0, 0),
                              vec3_create(0, 0, 15),
                              &v, &p, 800, 600);
    TEST_ASSERT_TRUE(d < 0.0f);
}

/* 19. Purity of project_to_screen */
void test_project_purity(void)
{
    mat4_t v = test_view();
    mat4_t p = test_proj();
    vec3_t pos = vec3_create(2, 3, -5);
    screen_point_t a = project_to_screen(pos, &v, &p, 800, 600);
    screen_point_t b = project_to_screen(pos, &v, &p, 800, 600);
    TEST_ASSERT_TRUE(a.x == b.x);
    TEST_ASSERT_TRUE(a.y == b.y);
    TEST_ASSERT_TRUE(a.depth == b.depth);
    TEST_ASSERT_EQUAL_INT(a.visible, b.visible);
}

/* 20. Purity of unproject_ray */
void test_unproject_purity(void)
{
    mat4_t v = test_view();
    mat4_t p = test_proj();
    vec3_t a = unproject_ray(200, 150, &v, &p, 800, 600);
    vec3_t b = unproject_ray(200, 150, &v, &p, 800, 600);
    TEST_ASSERT_TRUE(a.x == b.x);
    TEST_ASSERT_TRUE(a.y == b.y);
    TEST_ASSERT_TRUE(a.z == b.z);
}

/* 21. Unproject top-of-screen has positive Y */
void test_unproject_up(void)
{
    mat4_t v = test_view();
    mat4_t p = test_proj();
    vec3_t ray = unproject_ray(400, 100, &v, &p, 800, 600);
    TEST_ASSERT_TRUE(ray.y > 0.0f);
}

/* 22. Symmetric projection: (-x) mirrors (+x) around center */
void test_project_symmetric(void)
{
    mat4_t v = test_view();
    mat4_t p = test_proj();
    screen_point_t left = project_to_screen(vec3_create(-2, 0, 0), &v, &p, 800, 600);
    screen_point_t right = project_to_screen(vec3_create(2, 0, 0), &v, &p, 800, 600);
    /* Both should be equidistant from center x=400 */
    float dl = 400.0f - left.x;
    float dr = right.x - 400.0f;
    TEST_ASSERT_TRUE(DNEAR(dl, dr));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_project_origin_center);
    RUN_TEST(test_project_origin_visible);
    RUN_TEST(test_project_behind_not_visible);
    RUN_TEST(test_project_right);
    RUN_TEST(test_project_up);
    RUN_TEST(test_project_depth_range);
    RUN_TEST(test_project_depth_ordering);
    RUN_TEST(test_project_identity);
    RUN_TEST(test_unproject_center);
    RUN_TEST(test_unproject_normalized);
    RUN_TEST(test_unproject_right);
    RUN_TEST(test_near_screen_hit);
    RUN_TEST(test_near_screen_miss);
    RUN_TEST(test_near_screen_behind);
    RUN_TEST(test_distance_same_point);
    RUN_TEST(test_distance_symmetric);
    RUN_TEST(test_distance_positive);
    RUN_TEST(test_distance_behind);
    RUN_TEST(test_project_purity);
    RUN_TEST(test_unproject_purity);
    RUN_TEST(test_unproject_up);
    RUN_TEST(test_project_symmetric);
    return UNITY_END();
}
