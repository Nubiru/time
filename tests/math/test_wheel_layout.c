#include "../unity/unity.h"
#include "../../src/math/wheel_layout.h"

#include <math.h>

#define PI_F 3.14159265358979323846f

void setUp(void) {}
void tearDown(void) {}

/* Position at cardinal angles: 1-4 */

void test_position_0_deg(void)
{
    wheel_point_t p = wheel_position(0.0f, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, p.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, p.y);
}

void test_position_90_deg(void)
{
    wheel_point_t p = wheel_position(90.0f, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, p.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, p.y);
}

void test_position_180_deg(void)
{
    wheel_point_t p = wheel_position(180.0f, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, -1.0f, p.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, p.y);
}

void test_position_270_deg(void)
{
    wheel_point_t p = wheel_position(270.0f, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, p.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, -1.0f, p.y);
}

/* Radius scaling: 5 */

void test_position_radius(void)
{
    wheel_point_t p = wheel_position(0.0f, 5.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 5.0f, p.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, p.y);
}

/* Sign centers: 6-8 */

void test_sign_center_aries(void)
{
    /* Aries center at 15 degrees */
    wheel_point_t p = wheel_sign_center(0, 1.0f);
    wheel_point_t expected = wheel_position(15.0f, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, expected.x, p.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, expected.y, p.y);
}

void test_sign_center_cancer(void)
{
    /* Cancer (3) center at 105 degrees */
    wheel_point_t p = wheel_sign_center(3, 1.0f);
    wheel_point_t expected = wheel_position(105.0f, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, expected.x, p.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, expected.y, p.y);
}

void test_sign_center_pisces(void)
{
    /* Pisces (11) center at 345 degrees */
    wheel_point_t p = wheel_sign_center(11, 1.0f);
    wheel_point_t expected = wheel_position(345.0f, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, expected.x, p.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, expected.y, p.y);
}

/* All 12 sign centers at correct radius: 9 */

void test_all_sign_centers_on_circle(void)
{
    float radius = 3.0f;
    for (int i = 0; i < 12; i++) {
        wheel_point_t p = wheel_sign_center(i, radius);
        float dist = sqrtf(p.x * p.x + p.y * p.y);
        TEST_ASSERT_FLOAT_WITHIN(0.01f, radius, dist);
    }
}

/* Sign segments: 10-12 */

void test_segment_aries(void)
{
    wheel_segment_t seg = wheel_sign_segment(0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, seg.start_deg);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 30.0f, seg.end_deg);
}

void test_segment_pisces(void)
{
    wheel_segment_t seg = wheel_sign_segment(11);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 330.0f, seg.start_deg);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 360.0f, seg.end_deg);
}

void test_in_segment(void)
{
    wheel_segment_t seg = wheel_sign_segment(0); /* 0-30 */
    TEST_ASSERT_EQUAL_INT(1, wheel_in_segment(15.0f, seg));
    TEST_ASSERT_EQUAL_INT(1, wheel_in_segment(0.0f, seg));
    TEST_ASSERT_EQUAL_INT(0, wheel_in_segment(30.0f, seg));
    TEST_ASSERT_EQUAL_INT(0, wheel_in_segment(350.0f, seg));
}

/* Angular distance: 13-15 */

void test_angular_distance_same(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, wheel_angular_distance(45.0f, 45.0f));
}

void test_angular_distance_90(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 90.0f, wheel_angular_distance(0.0f, 90.0f));
}

void test_angular_distance_wraps(void)
{
    /* 10 to 350 = shortest path is 20 degrees */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 20.0f, wheel_angular_distance(10.0f, 350.0f));
}

/* Ring points: 16-17 */

void test_ring_points_count(void)
{
    wheel_point_t pts[36];
    wheel_ring_points(1.0f, 36, pts);
    /* First point at 0 degrees */
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, pts[0].x);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, pts[0].y);
}

void test_ring_points_on_circle(void)
{
    wheel_point_t pts[12];
    wheel_ring_points(2.0f, 12, pts);
    for (int i = 0; i < 12; i++) {
        float dist = sqrtf(pts[i].x * pts[i].x + pts[i].y * pts[i].y);
        TEST_ASSERT_FLOAT_WITHIN(0.01f, 2.0f, dist);
    }
}

/* Billboard angle: 18 */

void test_billboard_angle(void)
{
    float result = wheel_billboard_angle(0.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, PI_F, result);

    float result2 = wheel_billboard_angle(PI_F / 2.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, PI_F * 1.5f, result2);
}

/* Longitude: 19 */

void test_longitude_normalized(void)
{
    wheel_point_t a = wheel_longitude(370.0f, 1.0f);
    wheel_point_t b = wheel_longitude(10.0f, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, b.x, a.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, b.y, a.y);
}

/* Negative longitude: 20 */

void test_longitude_negative(void)
{
    wheel_point_t a = wheel_longitude(-10.0f, 1.0f);
    wheel_point_t b = wheel_longitude(350.0f, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, b.x, a.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, b.y, a.y);
}

/* Purity: 21 */

void test_purity(void)
{
    wheel_point_t a = wheel_position(123.456f, 7.89f);
    wheel_point_t b = wheel_position(123.456f, 7.89f);
    TEST_ASSERT_EQUAL_FLOAT(a.x, b.x);
    TEST_ASSERT_EQUAL_FLOAT(a.y, b.y);
}

/* Angular distance max 180: 22 */

void test_angular_distance_opposite(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 180.0f, wheel_angular_distance(0.0f, 180.0f));
}

/* Segment wrapping: 23 */

void test_in_segment_wrapping(void)
{
    /* Custom wrapping segment: 350-10 degrees */
    wheel_segment_t seg = {350.0f, 10.0f};
    TEST_ASSERT_EQUAL_INT(1, wheel_in_segment(355.0f, seg));
    TEST_ASSERT_EQUAL_INT(1, wheel_in_segment(5.0f, seg));
    TEST_ASSERT_EQUAL_INT(0, wheel_in_segment(20.0f, seg));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_position_0_deg);
    RUN_TEST(test_position_90_deg);
    RUN_TEST(test_position_180_deg);
    RUN_TEST(test_position_270_deg);
    RUN_TEST(test_position_radius);
    RUN_TEST(test_sign_center_aries);
    RUN_TEST(test_sign_center_cancer);
    RUN_TEST(test_sign_center_pisces);
    RUN_TEST(test_all_sign_centers_on_circle);
    RUN_TEST(test_segment_aries);
    RUN_TEST(test_segment_pisces);
    RUN_TEST(test_in_segment);
    RUN_TEST(test_angular_distance_same);
    RUN_TEST(test_angular_distance_90);
    RUN_TEST(test_angular_distance_wraps);
    RUN_TEST(test_ring_points_count);
    RUN_TEST(test_ring_points_on_circle);
    RUN_TEST(test_billboard_angle);
    RUN_TEST(test_longitude_normalized);
    RUN_TEST(test_longitude_negative);
    RUN_TEST(test_purity);
    RUN_TEST(test_angular_distance_opposite);
    RUN_TEST(test_in_segment_wrapping);
    return UNITY_END();
}
