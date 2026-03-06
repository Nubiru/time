#include "../unity/unity.h"
#include "../../src/math/sacred_geometry.h"

#include <math.h>
#include <string.h>

#define PI_F  3.14159265f
#define TOL   0.01f
#define NEAR(exp, act) ((act) >= (exp) - TOL && (act) <= (exp) + TOL)

void setUp(void) {}
void tearDown(void) {}

/* 1. fibonacci(0) = 0 */
void test_fib_0(void)
{
    TEST_ASSERT_EQUAL_INT64(0, fibonacci(0));
}

/* 2. fibonacci(1) = 1 */
void test_fib_1(void)
{
    TEST_ASSERT_EQUAL_INT64(1, fibonacci(1));
}

/* 3. fibonacci(10) = 55 */
void test_fib_10(void)
{
    TEST_ASSERT_EQUAL_INT64(55, fibonacci(10));
}

/* 4. fibonacci(20) = 6765 */
void test_fib_20(void)
{
    TEST_ASSERT_EQUAL_INT64(6765, fibonacci(20));
}

/* 5. fibonacci_ratio(10) ~ PHI within 0.001 */
void test_fib_ratio_10(void)
{
    double r = fibonacci_ratio(10);
    TEST_ASSERT_TRUE(r > PHI - 0.001 && r < PHI + 0.001);
}

/* 6. fibonacci_ratio(20) ~ PHI within 0.00001 */
void test_fib_ratio_20(void)
{
    double r = fibonacci_ratio(20);
    TEST_ASSERT_TRUE(r > PHI - 0.00001 && r < PHI + 0.00001);
}

/* 7. Golden spiral correct number of points */
void test_spiral_count(void)
{
    vec3_t c = vec3_create(0, 0, 0);
    spiral_path_t s = golden_spiral(c, 1.0f, 2.0f, 50, 0);
    TEST_ASSERT_EQUAL_INT(50, s.count);
}

/* 8. First point at start_radius from center */
void test_spiral_start_radius(void)
{
    vec3_t c = vec3_create(0, 0, 0);
    spiral_path_t s = golden_spiral(c, 3.0f, 2.0f, 50, 0);
    float dist = vec3_length(s.points[0]);
    TEST_ASSERT_TRUE(NEAR(3.0f, dist));
}

/* 9. Spiral radius grows with angle */
void test_spiral_grows(void)
{
    vec3_t c = vec3_create(0, 0, 0);
    spiral_path_t s = golden_spiral(c, 1.0f, 2.0f, 50, 0);
    float r_first = vec3_length(s.points[0]);
    float r_last = vec3_length(s.points[49]);
    TEST_ASSERT_TRUE(r_last > r_first);
}

/* 10. Triangle: 3 vertices at radius */
void test_triangle(void)
{
    vec3_t c = vec3_create(0, 0, 0);
    polygon_t p = regular_polygon(c, 5.0f, 3, 0.0f);
    TEST_ASSERT_EQUAL_INT(3, p.count);
    for (int i = 0; i < 3; i++) {
        float dist = vec3_length(p.vertices[i]);
        TEST_ASSERT_TRUE(NEAR(5.0f, dist));
    }
}

/* 11. Square: 4 vertices at radius */
void test_square(void)
{
    vec3_t c = vec3_create(0, 0, 0);
    polygon_t p = regular_polygon(c, 4.0f, 4, 0.0f);
    TEST_ASSERT_EQUAL_INT(4, p.count);
    for (int i = 0; i < 4; i++) {
        float dist = vec3_length(p.vertices[i]);
        TEST_ASSERT_TRUE(NEAR(4.0f, dist));
    }
}

/* 12. Hexagon: 6 vertices at radius */
void test_hexagon(void)
{
    vec3_t c = vec3_create(0, 0, 0);
    polygon_t p = regular_polygon(c, 3.0f, 6, 0.0f);
    TEST_ASSERT_EQUAL_INT(6, p.count);
    for (int i = 0; i < 6; i++) {
        float dist = vec3_length(p.vertices[i]);
        TEST_ASSERT_TRUE(NEAR(3.0f, dist));
    }
}

/* 13. Pentagon: angles 72 degrees apart */
void test_pentagon_angles(void)
{
    vec3_t c = vec3_create(0, 0, 0);
    polygon_t p = regular_polygon(c, 1.0f, 5, 0.0f);
    TEST_ASSERT_EQUAL_INT(5, p.count);
    for (int i = 0; i < 5; i++) {
        int j = (i + 1) % 5;
        float a1 = atan2f(p.vertices[i].y, p.vertices[i].x);
        float a2 = atan2f(p.vertices[j].y, p.vertices[j].x);
        float diff = fmodf(a2 - a1 + 4.0f * PI_F, 2.0f * PI_F);
        float deg = diff * 180.0f / PI_F;
        TEST_ASSERT_TRUE(NEAR(72.0f, deg));
    }
}

/* 14. Vesica piscis: intersection points exist */
void test_vesica_exists(void)
{
    vec3_t a = vec3_create(-0.5f, 0, 0);
    vec3_t b = vec3_create(0.5f, 0, 0);
    vesica_piscis_t v = vesica_piscis(a, b, 1.0f);
    TEST_ASSERT_TRUE(v.top.y > 0.0f);
    TEST_ASSERT_TRUE(v.bottom.y < 0.0f);
}

/* 15. Eye height > eye width */
void test_vesica_proportions(void)
{
    vec3_t a = vec3_create(-0.5f, 0, 0);
    vec3_t b = vec3_create(0.5f, 0, 0);
    vesica_piscis_t v = vesica_piscis(a, b, 1.0f);
    TEST_ASSERT_TRUE(v.eye_height > v.eye_width);
    TEST_ASSERT_TRUE(v.eye_height > 0.0f);
    TEST_ASSERT_TRUE(v.eye_width > 0.0f);
}

/* 16. Vitruvian navel at golden ratio */
void test_vitruvian_navel(void)
{
    vitruvian_t v = vitruvian_proportions(1.0f, 0.0f);
    TEST_ASSERT_TRUE(NEAR((float)PHI_INV, v.navel_height));
}

/* 17. arm_span = height */
void test_vitruvian_arm_span(void)
{
    vitruvian_t v = vitruvian_proportions(1.8f, 0.0f);
    TEST_ASSERT_TRUE(v.arm_span == v.height);
}

/* 18. circle center at navel height */
void test_vitruvian_circle_center(void)
{
    vitruvian_t v = vitruvian_proportions(1.0f, 0.0f);
    TEST_ASSERT_TRUE(NEAR(v.navel_height, v.circle_center.y));
}

/* 19. Golden rectangle height ~ 1.0 for width ~ PHI */
void test_golden_rect_height(void)
{
    vec3_t c = vec3_create(0, 0, 0);
    golden_rect_t r = golden_rectangle(c, (float)PHI);
    TEST_ASSERT_TRUE(NEAR(1.0f, r.height));
}

/* 20. Width/height ratio ~ PHI */
void test_golden_rect_ratio(void)
{
    vec3_t c = vec3_create(0, 0, 0);
    golden_rect_t r = golden_rectangle(c, 5.0f);
    float ratio = r.width / r.height;
    TEST_ASSERT_TRUE(NEAR((float)PHI, ratio));
}

/* 21. Golden section divides at PHI_INV */
void test_golden_section(void)
{
    vec3_t a = vec3_create(0, 0, 0);
    vec3_t b = vec3_create(10, 0, 0);
    vec3_t p = golden_section_point(a, b);
    TEST_ASSERT_TRUE(NEAR(10.0f * (float)PHI_INV, p.x));
    TEST_ASSERT_TRUE(NEAR(0.0f, p.y));
}

/* 22. Tetrahedron: 4V, 6E, 4F */
void test_tetrahedron(void)
{
    platonic_info_t s = platonic_solid(0);
    TEST_ASSERT_EQUAL_INT(4, s.vertices);
    TEST_ASSERT_EQUAL_INT(6, s.edges);
    TEST_ASSERT_EQUAL_INT(4, s.faces);
}

/* 23. Cube: 8V, 12E, 6F */
void test_cube(void)
{
    platonic_info_t s = platonic_solid(1);
    TEST_ASSERT_EQUAL_INT(8, s.vertices);
    TEST_ASSERT_EQUAL_INT(12, s.edges);
    TEST_ASSERT_EQUAL_INT(6, s.faces);
}

/* 24. All 5 Platonic solids satisfy Euler: V - E + F = 2 */
void test_euler_formula(void)
{
    for (int i = 0; i < 5; i++) {
        platonic_info_t s = platonic_solid(i);
        TEST_ASSERT_EQUAL_INT(2, s.vertices - s.edges + s.faces);
    }
}

/* 25. Purity */
void test_purity(void)
{
    vec3_t c = vec3_create(0, 0, 0);
    polygon_t a = regular_polygon(c, 5.0f, 6, 30.0f);
    polygon_t b = regular_polygon(c, 5.0f, 6, 30.0f);
    TEST_ASSERT_EQUAL_INT(a.count, b.count);
    for (int i = 0; i < a.count; i++) {
        TEST_ASSERT_TRUE(a.vertices[i].x == b.vertices[i].x);
        TEST_ASSERT_TRUE(a.vertices[i].y == b.vertices[i].y);
    }
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_fib_0);
    RUN_TEST(test_fib_1);
    RUN_TEST(test_fib_10);
    RUN_TEST(test_fib_20);
    RUN_TEST(test_fib_ratio_10);
    RUN_TEST(test_fib_ratio_20);
    RUN_TEST(test_spiral_count);
    RUN_TEST(test_spiral_start_radius);
    RUN_TEST(test_spiral_grows);
    RUN_TEST(test_triangle);
    RUN_TEST(test_square);
    RUN_TEST(test_hexagon);
    RUN_TEST(test_pentagon_angles);
    RUN_TEST(test_vesica_exists);
    RUN_TEST(test_vesica_proportions);
    RUN_TEST(test_vitruvian_navel);
    RUN_TEST(test_vitruvian_arm_span);
    RUN_TEST(test_vitruvian_circle_center);
    RUN_TEST(test_golden_rect_height);
    RUN_TEST(test_golden_rect_ratio);
    RUN_TEST(test_golden_section);
    RUN_TEST(test_tetrahedron);
    RUN_TEST(test_cube);
    RUN_TEST(test_euler_formula);
    RUN_TEST(test_purity);
    return UNITY_END();
}
