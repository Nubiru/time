#include "../unity/unity.h"
#include "../../src/render/camera_motion.h"

#include <math.h>

#define EPSILON  0.001f
#define DT       (1.0f / 60.0f)

void setUp(void)  {}
void tearDown(void) {}

/* 1. Create: default friction values set, velocities zero */
void test_create_defaults(void)
{
    camera_motion_t m = camera_motion_create();
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, 0.05f, m.rotation_friction);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, 0.01f, m.zoom_friction);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, 0.0f,  m.az_velocity);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, 0.0f,  m.el_velocity);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, 0.0f,  m.zoom_velocity);
    TEST_ASSERT_EQUAL_INT(0, m.dragging);
}

/* 2. Create custom: custom friction values */
void test_create_custom(void)
{
    camera_motion_t m = camera_motion_create_custom(0.1f, 0.2f);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, 0.1f, m.rotation_friction);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, 0.2f, m.zoom_friction);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, 0.0f, m.az_velocity);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, 0.0f, m.el_velocity);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, 0.0f, m.zoom_velocity);
}

/* 3. Drag outputs direct delta */
void test_drag_outputs_direct_delta(void)
{
    camera_motion_t m = camera_motion_create();
    m = camera_motion_drag(m, 0.5f, -0.3f, DT);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, 0.5f,  m.az_out);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, -0.3f, m.el_out);
}

/* 4. Drag tracks velocity = delta / dt */
void test_drag_tracks_velocity(void)
{
    camera_motion_t m = camera_motion_create();
    float daz = 0.1f;
    float del = -0.05f;
    m = camera_motion_drag(m, daz, del, DT);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, daz / DT, m.az_velocity);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, del / DT, m.el_velocity);
}

/* 5. New drag cancels pre-existing coast velocity */
void test_drag_cancels_coast(void)
{
    camera_motion_t m = camera_motion_create();
    /* Simulate a coast with existing velocity */
    m.az_velocity = 5.0f;
    m.el_velocity = 3.0f;
    m.dragging = 0;
    /* Start a new drag — should zero old velocity first */
    m = camera_motion_drag(m, 0.01f, 0.01f, DT);
    /* Velocity now reflects only current drag */
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.01f / DT, m.az_velocity);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.01f / DT, m.el_velocity);
}

/* 6. Release preserves velocity */
void test_release_preserves_velocity(void)
{
    camera_motion_t m = camera_motion_create();
    m = camera_motion_drag(m, 0.1f, 0.05f, DT);
    float az_vel = m.az_velocity;
    float el_vel = m.el_velocity;
    m = camera_motion_release(m);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, az_vel, m.az_velocity);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, el_vel, m.el_velocity);
    TEST_ASSERT_EQUAL_INT(0, m.dragging);
}

/* 7. After release, update produces nonzero az_out */
void test_coast_produces_output(void)
{
    camera_motion_t m = camera_motion_create();
    m = camera_motion_drag(m, 0.1f, 0.05f, DT);
    m = camera_motion_release(m);
    m = camera_motion_update(m, DT);
    TEST_ASSERT_TRUE(fabsf(m.az_out) > 0.0f);
    TEST_ASSERT_TRUE(fabsf(m.el_out) > 0.0f);
}

/* 8. After several updates, velocity decreases */
void test_coast_decays_velocity(void)
{
    camera_motion_t m = camera_motion_create();
    m = camera_motion_drag(m, 0.1f, 0.05f, DT);
    float initial_az_vel = m.az_velocity;
    m = camera_motion_release(m);
    /* Run several frames */
    for (int i = 0; i < 30; i++) {
        m = camera_motion_update(m, DT);
    }
    TEST_ASSERT_TRUE(fabsf(m.az_velocity) < fabsf(initial_az_vel));
}

/* 9. After enough updates, velocity reaches zero */
void test_coast_stops_eventually(void)
{
    camera_motion_t m = camera_motion_create();
    m = camera_motion_drag(m, 0.1f, 0.05f, DT);
    m = camera_motion_release(m);
    /* Run for 5 seconds (300 frames at 60fps) */
    for (int i = 0; i < 300; i++) {
        m = camera_motion_update(m, DT);
    }
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, 0.0f, m.az_velocity);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, 0.0f, m.el_velocity);
}

/* 10. Scroll adds to zoom velocity */
void test_scroll_adds_velocity(void)
{
    camera_motion_t m = camera_motion_create();
    m = camera_motion_scroll(m, 1.5f);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, 1.5f, m.zoom_velocity);
}

/* 11. Multiple scroll events accumulate */
void test_scroll_cumulative(void)
{
    camera_motion_t m = camera_motion_create();
    m = camera_motion_scroll(m, 1.0f);
    m = camera_motion_scroll(m, 0.5f);
    m = camera_motion_scroll(m, -0.3f);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, 1.2f, m.zoom_velocity);
}

/* 12. Update produces nonzero zoom_out when zoom velocity exists */
void test_zoom_coast_produces_output(void)
{
    camera_motion_t m = camera_motion_create();
    m = camera_motion_scroll(m, 2.0f);
    m = camera_motion_update(m, DT);
    TEST_ASSERT_TRUE(fabsf(m.zoom_out) > 0.0f);
}

/* 13. Zoom velocity decays over time */
void test_zoom_coast_decays(void)
{
    camera_motion_t m = camera_motion_create();
    m = camera_motion_scroll(m, 5.0f);
    float initial = m.zoom_velocity;
    for (int i = 0; i < 30; i++) {
        m = camera_motion_update(m, DT);
    }
    TEST_ASSERT_TRUE(fabsf(m.zoom_velocity) < fabsf(initial));
}

/* 14. Update with zero dt produces all-zero outputs */
void test_update_zero_dt(void)
{
    camera_motion_t m = camera_motion_create();
    m.az_velocity   = 1.0f;
    m.el_velocity   = 1.0f;
    m.zoom_velocity = 1.0f;
    m = camera_motion_update(m, 0.0f);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, 0.0f, m.az_out);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, 0.0f, m.el_out);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, 0.0f, m.zoom_out);
}

/* 15. Active returns 1 when velocity exists */
void test_active_true_during_coast(void)
{
    camera_motion_t m = camera_motion_create();
    m = camera_motion_drag(m, 0.1f, 0.05f, DT);
    m = camera_motion_release(m);
    TEST_ASSERT_TRUE(camera_motion_active(m, 0.001f));
}

/* 16. Active returns 0 when all velocities zero */
void test_active_false_when_stopped(void)
{
    camera_motion_t m = camera_motion_create();
    TEST_ASSERT_FALSE(camera_motion_active(m, 0.001f));
}

/* 17. Stop zeroes everything */
void test_stop_zeroes_everything(void)
{
    camera_motion_t m = camera_motion_create();
    m.az_velocity   = 5.0f;
    m.el_velocity   = 3.0f;
    m.zoom_velocity = 2.0f;
    m.az_out        = 1.0f;
    m.el_out        = 1.0f;
    m.zoom_out      = 1.0f;
    m.dragging      = 1;
    m = camera_motion_stop(m);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, 0.0f, m.az_velocity);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, 0.0f, m.el_velocity);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, 0.0f, m.zoom_velocity);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, 0.0f, m.az_out);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, 0.0f, m.el_out);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, 0.0f, m.zoom_out);
    TEST_ASSERT_EQUAL_INT(0, m.dragging);
}

/* 18. Friction is frame-rate independent:
 *     same total motion at 30fps vs 60fps over 1 second (within tolerance).
 *     Both start with the same velocity by using proportional drag deltas. */
void test_friction_frame_rate_independent(void)
{
    float dt_30fps = 1.0f / 30.0f;
    float dt_60fps = 1.0f / 60.0f;
    float speed = 3.0f; /* radians/second — same drag speed */

    /* 30fps path: delta = speed * dt so velocity = delta/dt = speed */
    camera_motion_t m30 = camera_motion_create();
    m30 = camera_motion_drag(m30, speed * dt_30fps, 0.0f, dt_30fps);
    m30 = camera_motion_release(m30);
    float total_az_30 = 0.0f;
    for (int i = 0; i < 30; i++) { /* 30 frames = 1 second */
        m30 = camera_motion_update(m30, dt_30fps);
        total_az_30 += m30.az_out;
    }

    /* 60fps path: delta = speed * dt so velocity = delta/dt = speed */
    camera_motion_t m60 = camera_motion_create();
    m60 = camera_motion_drag(m60, speed * dt_60fps, 0.0f, dt_60fps);
    m60 = camera_motion_release(m60);
    float total_az_60 = 0.0f;
    for (int i = 0; i < 60; i++) { /* 60 frames = 1 second */
        m60 = camera_motion_update(m60, dt_60fps);
        total_az_60 += m60.az_out;
    }

    /* Both should produce similar total displacement (within 5%) */
    float ratio = total_az_30 / total_az_60;
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 1.0f, ratio);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_create_defaults);
    RUN_TEST(test_create_custom);
    RUN_TEST(test_drag_outputs_direct_delta);
    RUN_TEST(test_drag_tracks_velocity);
    RUN_TEST(test_drag_cancels_coast);
    RUN_TEST(test_release_preserves_velocity);
    RUN_TEST(test_coast_produces_output);
    RUN_TEST(test_coast_decays_velocity);
    RUN_TEST(test_coast_stops_eventually);
    RUN_TEST(test_scroll_adds_velocity);
    RUN_TEST(test_scroll_cumulative);
    RUN_TEST(test_zoom_coast_produces_output);
    RUN_TEST(test_zoom_coast_decays);
    RUN_TEST(test_update_zero_dt);
    RUN_TEST(test_active_true_during_coast);
    RUN_TEST(test_active_false_when_stopped);
    RUN_TEST(test_stop_zeroes_everything);
    RUN_TEST(test_friction_frame_rate_independent);
    return UNITY_END();
}
