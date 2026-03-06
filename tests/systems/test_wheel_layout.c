#include "../unity/unity.h"
#include "../../src/systems/astrology/wheel_layout.h"
#include "../../src/math/vec3.h"

#include <math.h>

#define TOL 0.01f
#define NEAR(exp, act) ((act) >= (exp) - TOL && (act) <= (exp) + TOL)

void setUp(void) {}
void tearDown(void) {}

/* 1. Layout radii */
void test_layout_radii(void)
{
    wheel_layout_t w = wheel_layout_create(3.0f, 5.0f);
    TEST_ASSERT_TRUE(NEAR(3.0f, w.inner_radius));
    TEST_ASSERT_TRUE(NEAR(5.0f, w.outer_radius));
}

/* 2. Label radius between inner and outer */
void test_label_radius_between(void)
{
    wheel_layout_t w = wheel_layout_create(3.0f, 5.0f);
    TEST_ASSERT_TRUE(w.label_radius > w.inner_radius);
    TEST_ASSERT_TRUE(w.label_radius < w.outer_radius);
}

/* 3. Sign labels at correct angles (midpoints: 15, 45, 75, ...) */
void test_sign_label_angles(void)
{
    wheel_layout_t w = wheel_layout_create(3.0f, 5.0f);
    for (int i = 0; i < 12; i++) {
        float expected = (float)i * 30.0f + 15.0f;
        TEST_ASSERT_TRUE(NEAR(expected, w.sign_labels[i].angle_deg));
    }
}

/* 4. Sign labels at label_radius distance */
void test_sign_labels_at_radius(void)
{
    wheel_layout_t w = wheel_layout_create(3.0f, 5.0f);
    for (int i = 0; i < 12; i++) {
        float dist = vec3_length(w.sign_labels[i].position);
        TEST_ASSERT_TRUE(NEAR(w.label_radius, dist));
    }
}

/* 5. Point at 0 = (5, 0, 0) */
void test_point_at_zero(void)
{
    wheel_point_t p = wheel_point_at_longitude(0.0f, 5.0f);
    TEST_ASSERT_TRUE(NEAR(5.0f, p.position.x));
    TEST_ASSERT_TRUE(NEAR(0.0f, p.position.y));
    TEST_ASSERT_TRUE(NEAR(0.0f, p.position.z));
}

/* 6. Point at 90 ~ (0, 5, 0) */
void test_point_at_90(void)
{
    wheel_point_t p = wheel_point_at_longitude(90.0f, 5.0f);
    TEST_ASSERT_TRUE(NEAR(0.0f, p.position.x));
    TEST_ASSERT_TRUE(NEAR(5.0f, p.position.y));
    TEST_ASSERT_TRUE(NEAR(0.0f, p.position.z));
}

/* 7. Point at 180 = (-5, 0, 0) */
void test_point_at_180(void)
{
    wheel_point_t p = wheel_point_at_longitude(180.0f, 5.0f);
    TEST_ASSERT_TRUE(NEAR(-5.0f, p.position.x));
    TEST_ASSERT_TRUE(NEAR(0.0f, p.position.y));
}

/* 8. Point at 45 has equal x and y */
void test_point_at_45(void)
{
    wheel_point_t p = wheel_point_at_longitude(45.0f, 5.0f);
    TEST_ASSERT_TRUE(NEAR(p.position.x, p.position.y));
    TEST_ASSERT_TRUE(p.position.x > 0.0f);
}

/* 9. Planet at 0 is sign 0 (Aries) */
void test_planet_sign_aries(void)
{
    planet_plot_t p = wheel_plot_planet(0.0f, 5.0f);
    TEST_ASSERT_EQUAL_INT(0, p.sign);
}

/* 10. Planet at 45 is sign 1 (Taurus) */
void test_planet_sign_taurus(void)
{
    planet_plot_t p = wheel_plot_planet(45.0f, 5.0f);
    TEST_ASSERT_EQUAL_INT(1, p.sign);
}

/* 11. Planet position on given radius */
void test_planet_on_radius(void)
{
    planet_plot_t p = wheel_plot_planet(123.0f, 7.0f);
    float dist = vec3_length(p.position);
    TEST_ASSERT_TRUE(NEAR(7.0f, dist));
}

/* 12. Aspect line 0-180 is horizontal diameter */
void test_aspect_line_horizontal(void)
{
    wheel_aspect_t line = wheel_aspect_line(0.0f, 180.0f, 5.0f);
    TEST_ASSERT_TRUE(NEAR(5.0f, line.start.x));
    TEST_ASSERT_TRUE(NEAR(0.0f, line.start.y));
    TEST_ASSERT_TRUE(NEAR(-5.0f, line.end.x));
    TEST_ASSERT_TRUE(NEAR(0.0f, line.end.y));
}

/* 13. Aspect line endpoints at correct longitudes */
void test_aspect_line_angles(void)
{
    wheel_aspect_t line = wheel_aspect_line(30.0f, 150.0f, 5.0f);
    TEST_ASSERT_TRUE(NEAR(30.0f, line.angle_a));
    TEST_ASSERT_TRUE(NEAR(150.0f, line.angle_b));
    TEST_ASSERT_TRUE(NEAR(5.0f, vec3_length(line.start)));
    TEST_ASSERT_TRUE(NEAR(5.0f, vec3_length(line.end)));
}

/* 14. Arc generates correct number of points */
void test_arc_point_count(void)
{
    vec3_t pts[10];
    int n = wheel_sign_arc(0, 5.0f, 4, pts, 10);
    TEST_ASSERT_EQUAL_INT(4, n);
}

/* 15. All arc points at correct radius */
void test_arc_points_on_radius(void)
{
    vec3_t pts[10];
    int n = wheel_sign_arc(0, 5.0f, 4, pts, 10);
    for (int i = 0; i < n; i++) {
        float dist = vec3_length(pts[i]);
        TEST_ASSERT_TRUE(NEAR(5.0f, dist));
    }
}

/* 16. First arc point at sign start, last at sign end */
void test_arc_endpoints(void)
{
    vec3_t pts[10];
    wheel_sign_arc(0, 5.0f, 4, pts, 10);
    /* Sign 0 start: 0 -> (5, 0, 0) */
    TEST_ASSERT_TRUE(NEAR(5.0f, pts[0].x));
    TEST_ASSERT_TRUE(NEAR(0.0f, pts[0].y));
    /* Sign 0 end: 30 -> (5*cos30, 5*sin30, 0) = (4.33, 2.5, 0) */
    TEST_ASSERT_TRUE(NEAR(4.33f, pts[3].x));
    TEST_ASSERT_TRUE(NEAR(2.5f, pts[3].y));
}

/* 17. Standard mode: angle = longitude */
void test_angle_standard_mode(void)
{
    float a = wheel_angle_from_longitude(0.0f, -1.0f);
    TEST_ASSERT_TRUE(NEAR(0.0f, a));
    a = wheel_angle_from_longitude(123.0f, -1.0f);
    TEST_ASSERT_TRUE(NEAR(123.0f, a));
}

/* 18. Rotated mode: ascendant mapped to 180 */
void test_angle_rotated_mode(void)
{
    float a = wheel_angle_from_longitude(90.0f, 90.0f);
    TEST_ASSERT_TRUE(NEAR(180.0f, a));
}

/* 19. Purity */
void test_purity(void)
{
    wheel_point_t a = wheel_point_at_longitude(77.7f, 4.0f);
    wheel_point_t b = wheel_point_at_longitude(77.7f, 4.0f);
    TEST_ASSERT_TRUE(a.position.x == b.position.x);
    TEST_ASSERT_TRUE(a.position.y == b.position.y);
    TEST_ASSERT_TRUE(a.angle_deg == b.angle_deg);
}

/* 20. Sign starts at correct angles */
void test_sign_starts(void)
{
    wheel_layout_t w = wheel_layout_create(3.0f, 5.0f);
    for (int i = 0; i < 12; i++) {
        float expected = (float)i * 30.0f;
        TEST_ASSERT_TRUE(NEAR(expected, w.sign_starts[i].angle_deg));
    }
}

/* 21. Arc max_out clamping */
void test_arc_clamped(void)
{
    vec3_t pts[2];
    int n = wheel_sign_arc(0, 5.0f, 10, pts, 2);
    TEST_ASSERT_EQUAL_INT(2, n);
}

/* 22. Negative longitude normalizes */
void test_negative_longitude(void)
{
    wheel_point_t p = wheel_point_at_longitude(-90.0f, 5.0f);
    TEST_ASSERT_TRUE(NEAR(270.0f, p.angle_deg));
}

/* 23. Rotation wraps correctly */
void test_rotation_wrap(void)
{
    float a = wheel_angle_from_longitude(10.0f, 350.0f);
    /* 10 - 350 + 180 = -160, normalized = 200 */
    TEST_ASSERT_TRUE(NEAR(200.0f, a));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_layout_radii);
    RUN_TEST(test_label_radius_between);
    RUN_TEST(test_sign_label_angles);
    RUN_TEST(test_sign_labels_at_radius);
    RUN_TEST(test_point_at_zero);
    RUN_TEST(test_point_at_90);
    RUN_TEST(test_point_at_180);
    RUN_TEST(test_point_at_45);
    RUN_TEST(test_planet_sign_aries);
    RUN_TEST(test_planet_sign_taurus);
    RUN_TEST(test_planet_on_radius);
    RUN_TEST(test_aspect_line_horizontal);
    RUN_TEST(test_aspect_line_angles);
    RUN_TEST(test_arc_point_count);
    RUN_TEST(test_arc_points_on_radius);
    RUN_TEST(test_arc_endpoints);
    RUN_TEST(test_angle_standard_mode);
    RUN_TEST(test_angle_rotated_mode);
    RUN_TEST(test_purity);
    RUN_TEST(test_sign_starts);
    RUN_TEST(test_arc_clamped);
    RUN_TEST(test_negative_longitude);
    RUN_TEST(test_rotation_wrap);
    return UNITY_END();
}
