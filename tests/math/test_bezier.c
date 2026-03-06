#include "../unity/unity.h"
#include "../../src/math/bezier.h"

#include <math.h>

#define TOL 0.01f
#define NEAR(exp, act) ((act) >= (exp) - TOL && (act) <= (exp) + TOL)

void setUp(void) {}
void tearDown(void) {}

static bezier_cubic_t make_straight_cubic(void)
{
    bezier_cubic_t b;
    b.p0 = vec3_create(0, 0, 0);
    b.p1 = vec3_create(1, 0, 0);
    b.p2 = vec3_create(2, 0, 0);
    b.p3 = vec3_create(3, 0, 0);
    return b;
}

static bezier_cubic_t make_curved_cubic(void)
{
    bezier_cubic_t b;
    b.p0 = vec3_create(0, 0, 0);
    b.p1 = vec3_create(1, 2, 0);
    b.p2 = vec3_create(3, 2, 0);
    b.p3 = vec3_create(4, 0, 0);
    return b;
}

/* 1. Cubic eval at t=0 returns P0 */
void test_cubic_eval_t0(void)
{
    bezier_cubic_t b = make_curved_cubic();
    vec3_t p = bezier_cubic_eval(b, 0.0f);
    TEST_ASSERT_TRUE(NEAR(0.0f, p.x));
    TEST_ASSERT_TRUE(NEAR(0.0f, p.y));
}

/* 2. Cubic eval at t=1 returns P3 */
void test_cubic_eval_t1(void)
{
    bezier_cubic_t b = make_curved_cubic();
    vec3_t p = bezier_cubic_eval(b, 1.0f);
    TEST_ASSERT_TRUE(NEAR(4.0f, p.x));
    TEST_ASSERT_TRUE(NEAR(0.0f, p.y));
}

/* 3. Quad eval at t=0 returns P0 */
void test_quad_eval_t0(void)
{
    bezier_quad_t q;
    q.p0 = vec3_create(0, 0, 0);
    q.p1 = vec3_create(1, 2, 0);
    q.p2 = vec3_create(2, 0, 0);
    vec3_t p = bezier_quad_eval(q, 0.0f);
    TEST_ASSERT_TRUE(NEAR(0.0f, p.x));
    TEST_ASSERT_TRUE(NEAR(0.0f, p.y));
}

/* 4. Quad eval at t=1 returns P2 */
void test_quad_eval_t1(void)
{
    bezier_quad_t q;
    q.p0 = vec3_create(0, 0, 0);
    q.p1 = vec3_create(1, 2, 0);
    q.p2 = vec3_create(2, 0, 0);
    vec3_t p = bezier_quad_eval(q, 1.0f);
    TEST_ASSERT_TRUE(NEAR(2.0f, p.x));
    TEST_ASSERT_TRUE(NEAR(0.0f, p.y));
}

/* 5. Straight line cubic at t=0.5 = midpoint */
void test_cubic_straight_midpoint(void)
{
    bezier_cubic_t b = make_straight_cubic();
    vec3_t p = bezier_cubic_eval(b, 0.5f);
    TEST_ASSERT_TRUE(NEAR(1.5f, p.x));
    TEST_ASSERT_TRUE(NEAR(0.0f, p.y));
}

/* 6. Symmetric quad at t=0.5 */
void test_quad_symmetric_midpoint(void)
{
    bezier_quad_t q;
    q.p0 = vec3_create(0, 0, 0);
    q.p1 = vec3_create(1, 1, 0);
    q.p2 = vec3_create(2, 0, 0);
    vec3_t p = bezier_quad_eval(q, 0.5f);
    TEST_ASSERT_TRUE(NEAR(1.0f, p.x));
    TEST_ASSERT_TRUE(NEAR(0.5f, p.y));
}

/* 7. Cubic tangent at t=0 parallel to (P1-P0) */
void test_cubic_tangent_t0(void)
{
    bezier_cubic_t b = make_curved_cubic();
    vec3_t tang = bezier_cubic_tangent(b, 0.0f);
    /* Should be 3*(P1-P0) = 3*(1,2,0) = (3,6,0) */
    TEST_ASSERT_TRUE(NEAR(3.0f, tang.x));
    TEST_ASSERT_TRUE(NEAR(6.0f, tang.y));
}

/* 8. Cubic tangent at t=1 parallel to (P3-P2) */
void test_cubic_tangent_t1(void)
{
    bezier_cubic_t b = make_curved_cubic();
    vec3_t tang = bezier_cubic_tangent(b, 1.0f);
    /* Should be 3*(P3-P2) = 3*(1,-2,0) = (3,-6,0) */
    TEST_ASSERT_TRUE(NEAR(3.0f, tang.x));
    TEST_ASSERT_TRUE(NEAR(-6.0f, tang.y));
}

/* 9. Tangent has non-zero length */
void test_tangent_nonzero(void)
{
    bezier_cubic_t b = make_curved_cubic();
    vec3_t tang = bezier_cubic_tangent(b, 0.5f);
    TEST_ASSERT_TRUE(vec3_length(tang) > 0.1f);
}

/* 10. Cubic sample returns correct count */
void test_cubic_sample_count(void)
{
    bezier_cubic_t b = make_curved_cubic();
    bezier_path_t path = bezier_cubic_sample(b, 10);
    TEST_ASSERT_EQUAL_INT(10, path.count);
}

/* 11. First sample = P0, last = P3 */
void test_cubic_sample_endpoints(void)
{
    bezier_cubic_t b = make_curved_cubic();
    bezier_path_t path = bezier_cubic_sample(b, 10);
    TEST_ASSERT_TRUE(NEAR(b.p0.x, path.points[0].x));
    TEST_ASSERT_TRUE(NEAR(b.p0.y, path.points[0].y));
    TEST_ASSERT_TRUE(NEAR(b.p3.x, path.points[9].x));
    TEST_ASSERT_TRUE(NEAR(b.p3.y, path.points[9].y));
}

/* 12. Total length > 0 */
void test_sample_positive_length(void)
{
    bezier_cubic_t b = make_curved_cubic();
    bezier_path_t path = bezier_cubic_sample(b, 20);
    TEST_ASSERT_TRUE(path.total_length > 0.0f);
}

/* 13. Straight line length = point distance */
void test_straight_line_length(void)
{
    bezier_cubic_t b = make_straight_cubic();
    float len = bezier_cubic_length(b, 100);
    TEST_ASSERT_TRUE(NEAR(3.0f, len));
}

/* 14. Curved path length > straight distance */
void test_curved_longer_than_straight(void)
{
    bezier_cubic_t b = make_curved_cubic();
    float len = bezier_cubic_length(b, 100);
    float straight = vec3_length(vec3_sub(b.p3, b.p0));
    TEST_ASSERT_TRUE(len > straight);
}

/* 15. Split: left.p0 = P0, right.p3 = P3 */
void test_split_endpoints(void)
{
    bezier_cubic_t b = make_curved_cubic();
    bezier_cubic_t left, right;
    bezier_cubic_split(b, 0.5f, &left, &right);
    TEST_ASSERT_TRUE(NEAR(b.p0.x, left.p0.x));
    TEST_ASSERT_TRUE(NEAR(b.p0.y, left.p0.y));
    TEST_ASSERT_TRUE(NEAR(b.p3.x, right.p3.x));
    TEST_ASSERT_TRUE(NEAR(b.p3.y, right.p3.y));
}

/* 16. Split halves match original curve */
void test_split_halves_on_curve(void)
{
    bezier_cubic_t b = make_curved_cubic();
    bezier_cubic_t left, right;
    bezier_cubic_split(b, 0.5f, &left, &right);

    /* left.eval(0.5) should = original.eval(0.25) */
    vec3_t lp = bezier_cubic_eval(left, 0.5f);
    vec3_t op1 = bezier_cubic_eval(b, 0.25f);
    TEST_ASSERT_TRUE(NEAR(op1.x, lp.x));
    TEST_ASSERT_TRUE(NEAR(op1.y, lp.y));

    /* right.eval(0.5) should = original.eval(0.75) */
    vec3_t rp = bezier_cubic_eval(right, 0.5f);
    vec3_t op2 = bezier_cubic_eval(b, 0.75f);
    TEST_ASSERT_TRUE(NEAR(op2.x, rp.x));
    TEST_ASSERT_TRUE(NEAR(op2.y, rp.y));
}

/* 17. From points with curvature=0 is near-straight */
void test_from_points_straight(void)
{
    vec3_t start = vec3_create(0, 0, 0);
    vec3_t end = vec3_create(6, 0, 0);
    bezier_cubic_t b = bezier_cubic_from_points(start, end, 0.0f);
    vec3_t mid = bezier_cubic_eval(b, 0.5f);
    TEST_ASSERT_TRUE(NEAR(3.0f, mid.x));
    TEST_ASSERT_TRUE(NEAR(0.0f, mid.y));
}

/* 18. Purity */
void test_purity(void)
{
    bezier_cubic_t b = make_curved_cubic();
    vec3_t a = bezier_cubic_eval(b, 0.3f);
    vec3_t c = bezier_cubic_eval(b, 0.3f);
    TEST_ASSERT_TRUE(a.x == c.x);
    TEST_ASSERT_TRUE(a.y == c.y);
}

/* 19. At-length fraction=0 returns P0, fraction=1 returns P3 */
void test_at_length_endpoints(void)
{
    bezier_cubic_t b = make_curved_cubic();
    vec3_t p0 = bezier_cubic_at_length(b, 0.0f, 50);
    vec3_t p1 = bezier_cubic_at_length(b, 1.0f, 50);
    TEST_ASSERT_TRUE(NEAR(b.p0.x, p0.x));
    TEST_ASSERT_TRUE(NEAR(b.p3.x, p1.x));
}

/* 20. Quad tangent at t=0 parallel to (P1-P0) */
void test_quad_tangent_t0(void)
{
    bezier_quad_t q;
    q.p0 = vec3_create(0, 0, 0);
    q.p1 = vec3_create(1, 2, 0);
    q.p2 = vec3_create(2, 0, 0);
    vec3_t tang = bezier_quad_tangent(q, 0.0f);
    /* 2*(P1-P0) = (2, 4, 0) */
    TEST_ASSERT_TRUE(NEAR(2.0f, tang.x));
    TEST_ASSERT_TRUE(NEAR(4.0f, tang.y));
}

/* 21. Quad length of straight line */
void test_quad_straight_length(void)
{
    bezier_quad_t q;
    q.p0 = vec3_create(0, 0, 0);
    q.p1 = vec3_create(2, 0, 0);
    q.p2 = vec3_create(4, 0, 0);
    float len = bezier_quad_length(q, 100);
    TEST_ASSERT_TRUE(NEAR(4.0f, len));
}

/* 22. From points with curvature produces curve */
void test_from_points_curved(void)
{
    vec3_t start = vec3_create(0, 0, 0);
    vec3_t end = vec3_create(4, 0, 0);
    bezier_cubic_t b = bezier_cubic_from_points(start, end, 0.5f);
    vec3_t mid = bezier_cubic_eval(b, 0.5f);
    /* With curvature, mid.y should be non-zero */
    TEST_ASSERT_TRUE(fabsf(mid.y) > 0.1f);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_cubic_eval_t0);
    RUN_TEST(test_cubic_eval_t1);
    RUN_TEST(test_quad_eval_t0);
    RUN_TEST(test_quad_eval_t1);
    RUN_TEST(test_cubic_straight_midpoint);
    RUN_TEST(test_quad_symmetric_midpoint);
    RUN_TEST(test_cubic_tangent_t0);
    RUN_TEST(test_cubic_tangent_t1);
    RUN_TEST(test_tangent_nonzero);
    RUN_TEST(test_cubic_sample_count);
    RUN_TEST(test_cubic_sample_endpoints);
    RUN_TEST(test_sample_positive_length);
    RUN_TEST(test_straight_line_length);
    RUN_TEST(test_curved_longer_than_straight);
    RUN_TEST(test_split_endpoints);
    RUN_TEST(test_split_halves_on_curve);
    RUN_TEST(test_from_points_straight);
    RUN_TEST(test_purity);
    RUN_TEST(test_at_length_endpoints);
    RUN_TEST(test_quad_tangent_t0);
    RUN_TEST(test_quad_straight_length);
    RUN_TEST(test_from_points_curved);
    return UNITY_END();
}
