#include "../unity/unity.h"
#include "../../src/render/camera_path.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static const float EPS = 0.01f;

void setUp(void) {}
void tearDown(void) {}

/* ---- camera_pose_create ---- */

void test_camera_pose_create(void)
{
    vec3_t t = vec3_create(1.0f, 2.0f, 3.0f);
    camera_pose_t p = camera_pose_create(0.5f, 0.3f, 2.0f, t);
    TEST_ASSERT_FLOAT_WITHIN(EPS, 0.5f, p.azimuth);
    TEST_ASSERT_FLOAT_WITHIN(EPS, 0.3f, p.elevation);
    TEST_ASSERT_FLOAT_WITHIN(EPS, 2.0f, p.log_zoom);
    TEST_ASSERT_FLOAT_WITHIN(EPS, 1.0f, p.target.x);
}

/* ---- Ease mode ---- */

void test_ease_not_done_initially(void)
{
    camera_pose_t a = camera_pose_create(0, 0, 1.0f, vec3_create(0, 0, 0));
    camera_pose_t b = camera_pose_create(1, 0.5f, 3.0f, vec3_create(0, 0, 0));
    camera_path_t p = camera_path_ease(a, b, 1.0f, 0.0f);
    TEST_ASSERT_EQUAL_INT(0, camera_path_done(p));
}

void test_ease_done_after_duration(void)
{
    camera_pose_t a = camera_pose_create(0, 0, 1.0f, vec3_create(0, 0, 0));
    camera_pose_t b = camera_pose_create(1, 0.5f, 3.0f, vec3_create(0, 0, 0));
    camera_path_t p = camera_path_ease(a, b, 0.5f, 0.0f);
    p = camera_path_update(p, 0.6f);
    TEST_ASSERT_EQUAL_INT(1, camera_path_done(p));
}

void test_ease_pose_at_start(void)
{
    camera_pose_t a = camera_pose_create(0.0f, 0.2f, 1.0f, vec3_create(0, 0, 0));
    camera_pose_t b = camera_pose_create(1.0f, 0.8f, 3.0f, vec3_create(5, 0, 0));
    camera_path_t p = camera_path_ease(a, b, 2.0f, 0.0f);
    camera_pose_t pose = camera_path_pose(p);
    TEST_ASSERT_FLOAT_WITHIN(EPS, 0.0f, pose.azimuth);
    TEST_ASSERT_FLOAT_WITHIN(EPS, 0.2f, pose.elevation);
    TEST_ASSERT_FLOAT_WITHIN(EPS, 1.0f, pose.log_zoom);
}

void test_ease_pose_at_end(void)
{
    camera_pose_t a = camera_pose_create(0.0f, 0.0f, 1.0f, vec3_create(0, 0, 0));
    camera_pose_t b = camera_pose_create(1.0f, 0.5f, 3.0f, vec3_create(5, 0, 0));
    camera_path_t p = camera_path_ease(a, b, 1.0f, 0.0f);
    p = camera_path_update(p, 1.5f);
    camera_pose_t pose = camera_path_pose(p);
    TEST_ASSERT_FLOAT_WITHIN(EPS, 1.0f, pose.azimuth);
    TEST_ASSERT_FLOAT_WITHIN(EPS, 0.5f, pose.elevation);
    TEST_ASSERT_FLOAT_WITHIN(EPS, 3.0f, pose.log_zoom);
    TEST_ASSERT_FLOAT_WITHIN(EPS, 5.0f, pose.target.x);
}

void test_ease_pose_midpoint(void)
{
    camera_pose_t a = camera_pose_create(0.0f, 0.0f, 0.0f, vec3_create(0, 0, 0));
    camera_pose_t b = camera_pose_create(2.0f, 1.0f, 4.0f, vec3_create(0, 0, 0));
    camera_path_t p = camera_path_ease(a, b, 2.0f, 0.0f);
    p = camera_path_update(p, 1.0f);
    camera_pose_t pose = camera_path_pose(p);
    /* At t=0.5 with cubic ease-in-out, eased value = 0.5 */
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 1.0f, pose.azimuth);
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 0.5f, pose.elevation);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 2.0f, pose.log_zoom);
}

void test_ease_azimuth_shortest_path(void)
{
    /* 350 deg to 10 deg should go through 360/0, not backwards */
    float az_start = 350.0f * (float)M_PI / 180.0f;  /* ~6.109 rad */
    float az_end   =  10.0f * (float)M_PI / 180.0f;  /* ~0.175 rad */
    camera_pose_t a = camera_pose_create(az_start, 0, 1.0f, vec3_create(0, 0, 0));
    camera_pose_t b = camera_pose_create(az_end, 0, 1.0f, vec3_create(0, 0, 0));
    camera_path_t p = camera_path_ease(a, b, 1.0f, 0.0f);
    p = camera_path_update(p, 0.5f);
    camera_pose_t pose = camera_path_pose(p);
    /* Midpoint should be near 0/360 degrees, not near 180 */
    float az_deg = pose.azimuth * 180.0f / (float)M_PI;
    /* Normalize to 0-360 */
    while (az_deg < 0) { az_deg += 360.0f; }
    while (az_deg >= 360.0f) { az_deg -= 360.0f; }
    /* Should be near 0 or 360, definitely not near 180 */
    int near_zero = (az_deg < 30.0f || az_deg > 330.0f);
    TEST_ASSERT_TRUE(near_zero);
}

void test_ease_arc_height_bump(void)
{
    camera_pose_t a = camera_pose_create(0, 0, 2.0f, vec3_create(0, 0, 0));
    camera_pose_t b = camera_pose_create(0, 0, 2.0f, vec3_create(0, 0, 0));
    /* Same start/end zoom, but arc_height=1.0 should bump midpoint */
    camera_path_t p = camera_path_ease(a, b, 2.0f, 1.0f);
    p = camera_path_update(p, 1.0f);
    camera_pose_t pose = camera_path_pose(p);
    /* At midpoint, zoom should be above 2.0 */
    TEST_ASSERT_TRUE(pose.log_zoom > 2.5f);
}

void test_ease_arc_height_zero_no_bump(void)
{
    camera_pose_t a = camera_pose_create(0, 0, 2.0f, vec3_create(0, 0, 0));
    camera_pose_t b = camera_pose_create(0, 0, 4.0f, vec3_create(0, 0, 0));
    camera_path_t p = camera_path_ease(a, b, 2.0f, 0.0f);
    p = camera_path_update(p, 1.0f);
    camera_pose_t pose = camera_path_pose(p);
    /* Without arc, zoom should be between start and end */
    TEST_ASSERT_TRUE(pose.log_zoom >= 2.0f);
    TEST_ASSERT_TRUE(pose.log_zoom <= 4.0f);
}

void test_ease_progress(void)
{
    camera_pose_t a = camera_pose_create(0, 0, 1.0f, vec3_create(0, 0, 0));
    camera_pose_t b = camera_pose_create(1, 0, 3.0f, vec3_create(0, 0, 0));
    camera_path_t p = camera_path_ease(a, b, 2.0f, 0.0f);

    TEST_ASSERT_FLOAT_WITHIN(EPS, 0.0f, camera_path_progress(p));
    p = camera_path_update(p, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(EPS, 0.5f, camera_path_progress(p));
    p = camera_path_update(p, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(EPS, 1.0f, camera_path_progress(p));
}

void test_ease_zero_dt_no_change(void)
{
    camera_pose_t a = camera_pose_create(0, 0, 1.0f, vec3_create(0, 0, 0));
    camera_pose_t b = camera_pose_create(1, 0, 3.0f, vec3_create(0, 0, 0));
    camera_path_t p = camera_path_ease(a, b, 1.0f, 0.0f);
    p = camera_path_update(p, 0.0f);
    TEST_ASSERT_FLOAT_WITHIN(EPS, 0.0f, camera_path_progress(p));
    TEST_ASSERT_EQUAL_INT(0, camera_path_done(p));
}

void test_ease_negative_dt_ignored(void)
{
    camera_pose_t a = camera_pose_create(0, 0, 1.0f, vec3_create(0, 0, 0));
    camera_pose_t b = camera_pose_create(1, 0, 3.0f, vec3_create(0, 0, 0));
    camera_path_t p = camera_path_ease(a, b, 1.0f, 0.0f);
    p = camera_path_update(p, 0.5f);
    float prog_before = camera_path_progress(p);
    p = camera_path_update(p, -0.3f);
    TEST_ASSERT_FLOAT_WITHIN(EPS, prog_before, camera_path_progress(p));
}

/* ---- Spring mode ---- */

void test_spring_not_done_initially(void)
{
    camera_pose_t a = camera_pose_create(0, 0, 1.0f, vec3_create(0, 0, 0));
    camera_pose_t b = camera_pose_create(1, 0.5f, 3.0f, vec3_create(5, 0, 0));
    camera_path_t p = camera_path_spring(a, b, SPRING_SNAPPY);
    TEST_ASSERT_EQUAL_INT(0, camera_path_done(p));
}

void test_spring_converges(void)
{
    camera_pose_t a = camera_pose_create(0, 0, 1.0f, vec3_create(0, 0, 0));
    camera_pose_t b = camera_pose_create(1, 0.5f, 3.0f, vec3_create(5, 0, 0));
    camera_path_t p = camera_path_spring(a, b, SPRING_SNAPPY);

    /* Run 5 seconds in small steps */
    for (int i = 0; i < 300; i++) {
        p = camera_path_update(p, 1.0f / 60.0f);
    }

    camera_pose_t pose = camera_path_pose(p);
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 1.0f, pose.azimuth);
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 0.5f, pose.elevation);
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 3.0f, pose.log_zoom);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 5.0f, pose.target.x);
}

void test_spring_done_after_settling(void)
{
    camera_pose_t a = camera_pose_create(0, 0, 1.0f, vec3_create(0, 0, 0));
    camera_pose_t b = camera_pose_create(0.5f, 0.3f, 2.0f, vec3_create(0, 0, 0));
    camera_path_t p = camera_path_spring(a, b, SPRING_GENTLE);

    for (int i = 0; i < 600; i++) {
        p = camera_path_update(p, 1.0f / 60.0f);
    }

    TEST_ASSERT_EQUAL_INT(1, camera_path_done(p));
}

void test_spring_pose_at_start(void)
{
    camera_pose_t a = camera_pose_create(0.5f, 0.3f, 2.0f, vec3_create(1, 2, 3));
    camera_pose_t b = camera_pose_create(1.0f, 0.8f, 4.0f, vec3_create(5, 6, 7));
    camera_path_t p = camera_path_spring(a, b, SPRING_SNAPPY);
    camera_pose_t pose = camera_path_pose(p);
    TEST_ASSERT_FLOAT_WITHIN(EPS, 0.5f, pose.azimuth);
    TEST_ASSERT_FLOAT_WITHIN(EPS, 0.3f, pose.elevation);
    TEST_ASSERT_FLOAT_WITHIN(EPS, 2.0f, pose.log_zoom);
}

void test_spring_retarget(void)
{
    camera_pose_t a = camera_pose_create(0, 0, 1.0f, vec3_create(0, 0, 0));
    camera_pose_t b = camera_pose_create(1, 0, 3.0f, vec3_create(0, 0, 0));
    camera_path_t p = camera_path_spring(a, b, SPRING_SNAPPY);

    /* Advance partway */
    for (int i = 0; i < 30; i++) {
        p = camera_path_update(p, 1.0f / 60.0f);
    }

    /* Retarget to new destination */
    camera_pose_t c = camera_pose_create(2, 1, 5.0f, vec3_create(10, 0, 0));
    p = camera_path_retarget(p, c);
    TEST_ASSERT_EQUAL_INT(0, camera_path_done(p));

    /* Should converge to new target */
    for (int i = 0; i < 600; i++) {
        p = camera_path_update(p, 1.0f / 60.0f);
    }

    camera_pose_t pose = camera_path_pose(p);
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 2.0f, pose.azimuth);
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 1.0f, pose.elevation);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 5.0f, pose.log_zoom);
}

void test_spring_azimuth_shortest_path(void)
{
    /* 350 deg to 10 deg — spring should take short path */
    float az_start = 350.0f * (float)M_PI / 180.0f;
    float az_end   =  10.0f * (float)M_PI / 180.0f;
    camera_pose_t a = camera_pose_create(az_start, 0, 1.0f, vec3_create(0, 0, 0));
    camera_pose_t b = camera_pose_create(az_end, 0, 1.0f, vec3_create(0, 0, 0));
    camera_path_t p = camera_path_spring(a, b, SPRING_SNAPPY);

    /* After a few frames the azimuth should be moving toward 360/0,
     * not toward 180. Check after 10 frames. */
    for (int i = 0; i < 10; i++) {
        p = camera_path_update(p, 1.0f / 60.0f);
    }

    camera_pose_t pose = camera_path_pose(p);
    float az_deg = pose.azimuth * 180.0f / (float)M_PI;
    while (az_deg < 0) { az_deg += 360.0f; }
    while (az_deg >= 360.0f) { az_deg -= 360.0f; }

    /* Should still be near 350-360 range, not heading toward 180 */
    int near_wrap = (az_deg > 330.0f || az_deg < 30.0f);
    TEST_ASSERT_TRUE(near_wrap);
}

void test_spring_retarget_preserves_velocity(void)
{
    camera_pose_t a = camera_pose_create(0, 0, 1.0f, vec3_create(0, 0, 0));
    camera_pose_t b = camera_pose_create(2, 0, 3.0f, vec3_create(0, 0, 0));
    camera_path_t p = camera_path_spring(a, b, SPRING_SNAPPY);

    /* Build up velocity */
    for (int i = 0; i < 5; i++) {
        p = camera_path_update(p, 1.0f / 60.0f);
    }

    float vel_before = p.az_spring.velocity;
    TEST_ASSERT_TRUE(fabsf(vel_before) > 0.01f);

    /* Retarget — velocity should be preserved */
    camera_pose_t c = camera_pose_create(3, 0, 5.0f, vec3_create(0, 0, 0));
    p = camera_path_retarget(p, c);

    TEST_ASSERT_FLOAT_WITHIN(EPS, vel_before, p.az_spring.velocity);
}

void test_retarget_ease_mode_ignored(void)
{
    camera_pose_t a = camera_pose_create(0, 0, 1.0f, vec3_create(0, 0, 0));
    camera_pose_t b = camera_pose_create(1, 0, 3.0f, vec3_create(0, 0, 0));
    camera_path_t p = camera_path_ease(a, b, 1.0f, 0.0f);

    camera_pose_t c = camera_pose_create(5, 0, 10.0f, vec3_create(0, 0, 0));
    camera_path_t p2 = camera_path_retarget(p, c);
    /* Should not change the end pose for ease mode */
    TEST_ASSERT_FLOAT_WITHIN(EPS, 1.0f, p2.end.azimuth);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_camera_pose_create);
    RUN_TEST(test_ease_not_done_initially);
    RUN_TEST(test_ease_done_after_duration);
    RUN_TEST(test_ease_pose_at_start);
    RUN_TEST(test_ease_pose_at_end);
    RUN_TEST(test_ease_pose_midpoint);
    RUN_TEST(test_ease_azimuth_shortest_path);
    RUN_TEST(test_ease_arc_height_bump);
    RUN_TEST(test_ease_arc_height_zero_no_bump);
    RUN_TEST(test_ease_progress);
    RUN_TEST(test_ease_zero_dt_no_change);
    RUN_TEST(test_ease_negative_dt_ignored);
    RUN_TEST(test_spring_not_done_initially);
    RUN_TEST(test_spring_converges);
    RUN_TEST(test_spring_done_after_settling);
    RUN_TEST(test_spring_pose_at_start);
    RUN_TEST(test_spring_retarget);
    RUN_TEST(test_spring_azimuth_shortest_path);
    RUN_TEST(test_spring_retarget_preserves_velocity);
    RUN_TEST(test_retarget_ease_mode_ignored);
    return UNITY_END();
}
