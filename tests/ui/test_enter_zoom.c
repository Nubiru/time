/* tests/ui/test_enter_zoom.c — "Enter Time" zoom choreography tests */

#include "../unity/unity.h"
#include "ui/enter_zoom.h"
#include <math.h>

#define TOL 0.01f

void setUp(void) {}
void tearDown(void) {}

/* --- test_ez_create_idle --- */
void test_ez_create_idle(void)
{
    enter_zoom_t ez = ez_create();
    TEST_ASSERT_EQUAL_INT(EZ_IDLE, ez_phase(ez));
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, ez.elapsed);
    TEST_ASSERT_FLOAT_WITHIN(TOL, EZ_HOLD_DURATION, ez.hold_duration);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, ez.reveal_progress);
}

/* --- test_ez_start_holding --- */
void test_ez_start_holding(void)
{
    enter_zoom_t ez = ez_create();
    ez = ez_start(ez);
    TEST_ASSERT_EQUAL_INT(EZ_HOLDING, ez_phase(ez));
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, ez.elapsed);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, ez.phase_elapsed);
}

/* --- test_ez_hold_duration_default --- */
void test_ez_hold_duration_default(void)
{
    enter_zoom_t ez = ez_create();
    ez = ez_start(ez);

    /* Tick just under hold duration — should still be HOLDING */
    ez = ez_tick(ez, EZ_HOLD_DURATION - 0.1f);
    TEST_ASSERT_EQUAL_INT(EZ_HOLDING, ez_phase(ez));
}

/* --- test_ez_set_hold_custom --- */
void test_ez_set_hold_custom(void)
{
    enter_zoom_t ez = ez_create();
    ez = ez_set_hold(ez, 1.5f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.5f, ez.hold_duration);

    ez = ez_start(ez);
    /* Tick past default 0.5 but under 1.5 — still holding */
    ez = ez_tick(ez, 1.0f);
    TEST_ASSERT_EQUAL_INT(EZ_HOLDING, ez_phase(ez));

    /* Tick past 1.5 — should transition to zooming */
    ez = ez_tick(ez, 0.6f);
    TEST_ASSERT_EQUAL_INT(EZ_ZOOMING, ez_phase(ez));
}

/* --- test_ez_hold_to_zoom --- */
void test_ez_hold_to_zoom(void)
{
    enter_zoom_t ez = ez_create();
    ez = ez_start(ez);

    /* Tick past hold duration */
    ez = ez_tick(ez, EZ_HOLD_DURATION + 0.01f);
    TEST_ASSERT_EQUAL_INT(EZ_ZOOMING, ez_phase(ez));
}

/* --- test_ez_zooming_active --- */
void test_ez_zooming_active(void)
{
    enter_zoom_t ez = ez_create();
    ez = ez_start(ez);
    ez = ez_tick(ez, EZ_HOLD_DURATION + 0.01f); /* enter ZOOMING */

    TEST_ASSERT_EQUAL_INT(EZ_ZOOMING, ez_phase(ez));
    TEST_ASSERT_EQUAL_INT(1, ez_active(ez));

    /* Mid-zoom should still be ZOOMING */
    ez = ez_tick(ez, EZ_ZOOM_DURATION * 0.5f);
    TEST_ASSERT_EQUAL_INT(EZ_ZOOMING, ez_phase(ez));
}

/* --- test_ez_zoom_to_reveal --- */
void test_ez_zoom_to_reveal(void)
{
    enter_zoom_t ez = ez_create();
    ez = ez_start(ez);

    /* Fast-forward past hold + zoom */
    ez = ez_tick(ez, EZ_HOLD_DURATION + 0.01f);
    TEST_ASSERT_EQUAL_INT(EZ_ZOOMING, ez_phase(ez));

    /* Tick through remaining zoom duration */
    ez = ez_tick(ez, EZ_ZOOM_DURATION + 0.1f);
    TEST_ASSERT_EQUAL_INT(EZ_REVEALING, ez_phase(ez));
}

/* --- test_ez_reveal_progress --- */
void test_ez_reveal_progress(void)
{
    enter_zoom_t ez = ez_create();
    ez = ez_start(ez);

    /* Skip to REVEALING with minimal overflow */
    ez = ez_tick(ez, EZ_HOLD_DURATION + 0.001f);
    ez = ez_tick(ez, EZ_ZOOM_DURATION + 0.001f);
    TEST_ASSERT_EQUAL_INT(EZ_REVEALING, ez_phase(ez));

    /* reveal_progress should start near 0 (tiny overflow only) */
    float initial = ez.reveal_progress;
    TEST_ASSERT_TRUE(initial < 0.1f);

    /* Tick partway through reveal — progress should increase */
    ez = ez_tick(ez, EZ_REVEAL_DURATION * 0.4f);
    TEST_ASSERT_TRUE(ez.reveal_progress > initial);
    TEST_ASSERT_TRUE(ez.reveal_progress < 1.0f);

    /* Tick to end — progress should reach 1.0 */
    ez = ez_tick(ez, EZ_REVEAL_DURATION);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, ez.reveal_progress);
}

/* --- test_ez_reveal_to_done --- */
void test_ez_reveal_to_done(void)
{
    enter_zoom_t ez = ez_create();
    ez = ez_start(ez);

    /* Fast-forward through all phases */
    ez = ez_tick(ez, EZ_HOLD_DURATION + 0.01f);
    ez = ez_tick(ez, EZ_ZOOM_DURATION + 0.1f);
    ez = ez_tick(ez, EZ_REVEAL_DURATION + 0.1f);

    TEST_ASSERT_EQUAL_INT(EZ_DONE, ez_phase(ez));
}

/* --- test_ez_done_to_idle --- */
void test_ez_done_to_idle(void)
{
    enter_zoom_t ez = ez_create();
    ez = ez_start(ez);

    /* Run to DONE */
    ez = ez_tick(ez, EZ_HOLD_DURATION + 0.01f);
    ez = ez_tick(ez, EZ_ZOOM_DURATION + 0.1f);
    ez = ez_tick(ez, EZ_REVEAL_DURATION + 0.1f);
    TEST_ASSERT_EQUAL_INT(EZ_DONE, ez_phase(ez));

    /* Next tick should reset to IDLE */
    ez = ez_tick(ez, 0.016f);
    TEST_ASSERT_EQUAL_INT(EZ_IDLE, ez_phase(ez));
}

/* --- test_ez_pose_at_start --- */
void test_ez_pose_at_start(void)
{
    enter_zoom_t ez = ez_create();
    ez = ez_start(ez);

    camera_pose_t pose = ez_pose(ez);
    TEST_ASSERT_FLOAT_WITHIN(TOL, EZ_START_AZ, pose.azimuth);
    TEST_ASSERT_FLOAT_WITHIN(TOL, EZ_START_EL, pose.elevation);
    TEST_ASSERT_FLOAT_WITHIN(TOL, EZ_START_LOG_ZOOM, pose.log_zoom);
}

/* --- test_ez_pose_at_end --- */
void test_ez_pose_at_end(void)
{
    enter_zoom_t ez = ez_create();
    ez = ez_start(ez);

    /* Run to DONE */
    ez = ez_tick(ez, EZ_HOLD_DURATION + 0.01f);
    ez = ez_tick(ez, EZ_ZOOM_DURATION + 0.1f);
    ez = ez_tick(ez, EZ_REVEAL_DURATION + 0.1f);

    camera_pose_t pose = ez_pose(ez);
    TEST_ASSERT_FLOAT_WITHIN(TOL, EZ_END_AZ, pose.azimuth);
    TEST_ASSERT_FLOAT_WITHIN(TOL, EZ_END_EL, pose.elevation);
    TEST_ASSERT_FLOAT_WITHIN(TOL, EZ_END_LOG_ZOOM, pose.log_zoom);
}

/* --- test_ez_progress_zero --- */
void test_ez_progress_zero(void)
{
    enter_zoom_t ez = ez_create();
    ez = ez_start(ez);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, ez_progress(ez));
}

/* --- test_ez_progress_increases --- */
void test_ez_progress_increases(void)
{
    enter_zoom_t ez = ez_create();
    ez = ez_start(ez);

    float prev = ez_progress(ez);
    ez = ez_tick(ez, 0.3f);
    float next = ez_progress(ez);
    TEST_ASSERT_TRUE(next > prev);

    prev = next;
    ez = ez_tick(ez, 0.5f);
    next = ez_progress(ez);
    TEST_ASSERT_TRUE(next > prev);
}

/* --- test_ez_progress_one_at_done --- */
void test_ez_progress_one_at_done(void)
{
    enter_zoom_t ez = ez_create();
    ez = ez_start(ez);

    /* Run to DONE */
    ez = ez_tick(ez, EZ_HOLD_DURATION + 0.01f);
    ez = ez_tick(ez, EZ_ZOOM_DURATION + 0.1f);
    ez = ez_tick(ez, EZ_REVEAL_DURATION + 0.1f);

    TEST_ASSERT_TRUE(ez_progress(ez) >= 0.99f);
}

/* --- test_ez_active_during_sequence --- */
void test_ez_active_during_sequence(void)
{
    enter_zoom_t ez = ez_create();
    ez = ez_start(ez);

    /* HOLDING */
    TEST_ASSERT_EQUAL_INT(1, ez_active(ez));

    /* ZOOMING */
    ez = ez_tick(ez, EZ_HOLD_DURATION + 0.01f);
    TEST_ASSERT_EQUAL_INT(1, ez_active(ez));

    /* REVEALING */
    ez = ez_tick(ez, EZ_ZOOM_DURATION + 0.1f);
    TEST_ASSERT_EQUAL_INT(1, ez_active(ez));
}

/* --- test_ez_not_active_idle --- */
void test_ez_not_active_idle(void)
{
    enter_zoom_t ez = ez_create();
    TEST_ASSERT_EQUAL_INT(0, ez_active(ez));
}

/* --- test_ez_reveal_blend_zero_during_hold --- */
void test_ez_reveal_blend_zero_during_hold(void)
{
    enter_zoom_t ez = ez_create();
    ez = ez_start(ez);

    /* HOLD phase */
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, ez_reveal_blend(ez));

    /* ZOOM phase */
    ez = ez_tick(ez, EZ_HOLD_DURATION + 0.01f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, ez_reveal_blend(ez));
}

/* --- test_ez_reveal_blend_one_at_done --- */
void test_ez_reveal_blend_one_at_done(void)
{
    enter_zoom_t ez = ez_create();
    ez = ez_start(ez);

    /* Run to DONE */
    ez = ez_tick(ez, EZ_HOLD_DURATION + 0.01f);
    ez = ez_tick(ez, EZ_ZOOM_DURATION + 0.1f);
    ez = ez_tick(ez, EZ_REVEAL_DURATION + 0.1f);

    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, ez_reveal_blend(ez));
}

/* --- test_ez_skip_snaps_to_end --- */
void test_ez_skip_snaps_to_end(void)
{
    enter_zoom_t ez = ez_create();
    ez = ez_start(ez);

    /* Skip mid-hold */
    ez = ez_tick(ez, 0.1f);
    ez = ez_skip(ez);

    TEST_ASSERT_EQUAL_INT(EZ_DONE, ez_phase(ez));
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, ez.reveal_progress);

    camera_pose_t pose = ez_pose(ez);
    TEST_ASSERT_FLOAT_WITHIN(TOL, EZ_END_AZ, pose.azimuth);
    TEST_ASSERT_FLOAT_WITHIN(TOL, EZ_END_EL, pose.elevation);
    TEST_ASSERT_FLOAT_WITHIN(TOL, EZ_END_LOG_ZOOM, pose.log_zoom);
}

/* --- test_ez_just_done_flag --- */
void test_ez_just_done_flag(void)
{
    enter_zoom_t ez = ez_create();
    ez = ez_start(ez);

    /* Not done yet */
    TEST_ASSERT_EQUAL_INT(0, ez_just_done(ez));

    /* Run to DONE */
    ez = ez_tick(ez, EZ_HOLD_DURATION + 0.01f);
    ez = ez_tick(ez, EZ_ZOOM_DURATION + 0.1f);
    ez = ez_tick(ez, EZ_REVEAL_DURATION + 0.1f);

    TEST_ASSERT_EQUAL_INT(1, ez_just_done(ez));

    /* After one more tick, no longer "just done" */
    ez = ez_tick(ez, 0.016f);
    TEST_ASSERT_EQUAL_INT(0, ez_just_done(ez));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_ez_create_idle);
    RUN_TEST(test_ez_start_holding);
    RUN_TEST(test_ez_hold_duration_default);
    RUN_TEST(test_ez_set_hold_custom);
    RUN_TEST(test_ez_hold_to_zoom);
    RUN_TEST(test_ez_zooming_active);
    RUN_TEST(test_ez_zoom_to_reveal);
    RUN_TEST(test_ez_reveal_progress);
    RUN_TEST(test_ez_reveal_to_done);
    RUN_TEST(test_ez_done_to_idle);
    RUN_TEST(test_ez_pose_at_start);
    RUN_TEST(test_ez_pose_at_end);
    RUN_TEST(test_ez_progress_zero);
    RUN_TEST(test_ez_progress_increases);
    RUN_TEST(test_ez_progress_one_at_done);
    RUN_TEST(test_ez_active_during_sequence);
    RUN_TEST(test_ez_not_active_idle);
    RUN_TEST(test_ez_reveal_blend_zero_during_hold);
    RUN_TEST(test_ez_reveal_blend_one_at_done);
    RUN_TEST(test_ez_skip_snaps_to_end);
    RUN_TEST(test_ez_just_done_flag);
    return UNITY_END();
}
