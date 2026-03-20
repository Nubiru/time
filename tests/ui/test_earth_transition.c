/* tests/ui/test_earth_transition.c — Earth View transition choreography tests */

#include "../../tests/unity/unity.h"
#include "../../src/ui/earth_transition.h"
#include <math.h>

void setUp(void) {}
void tearDown(void) {}

/* ---- Helper: run transition to completion ---- */
static earth_transition_t run_to_phase(earth_transition_t et, et_phase_e target,
                                        float dt, int max_steps)
{
    for (int i = 0; i < max_steps && et_phase(et) != target; i++) {
        et = et_tick(et, dt);
    }
    return et;
}

/* ---- Tests ---- */

void test_et_create_idle(void)
{
    earth_transition_t et = et_create();
    TEST_ASSERT_EQUAL_INT(ET_IDLE, et_phase(et));
    TEST_ASSERT_EQUAL_INT(0, et_active(et));
}

void test_et_begin_to_earth_starts_anticipate(void)
{
    earth_transition_t et = et_create();
    camera_pose_t current = et_space_pose();
    et = et_begin(et, ET_DIR_TO_EARTH, current);
    TEST_ASSERT_EQUAL_INT(ET_ANTICIPATE, et_phase(et));
    TEST_ASSERT_EQUAL_INT(1, et_active(et));
}

void test_et_begin_to_space_starts_anticipate(void)
{
    earth_transition_t et = et_create();
    camera_pose_t current = et_earth_pose();
    et = et_begin(et, ET_DIR_TO_SPACE, current);
    TEST_ASSERT_EQUAL_INT(ET_ANTICIPATE, et_phase(et));
    TEST_ASSERT_EQUAL_INT(ET_DIR_TO_SPACE, et_direction(et));
}

void test_et_anticipate_duration(void)
{
    earth_transition_t et = et_create();
    camera_pose_t current = et_space_pose();
    et = et_begin(et, ET_DIR_TO_EARTH, current);

    /* Tick half of anticipation — should still be ANTICIPATE */
    et = et_tick(et, ET_ANTICIPATE_DURATION * 0.5f);
    TEST_ASSERT_EQUAL_INT(ET_ANTICIPATE, et_phase(et));

    /* Tick just under remaining time */
    et = et_tick(et, ET_ANTICIPATE_DURATION * 0.4f);
    TEST_ASSERT_EQUAL_INT(ET_ANTICIPATE, et_phase(et));
}

void test_et_anticipate_to_fly(void)
{
    earth_transition_t et = et_create();
    camera_pose_t current = et_space_pose();
    et = et_begin(et, ET_DIR_TO_EARTH, current);

    /* Tick past anticipation duration */
    et = et_tick(et, ET_ANTICIPATE_DURATION + 0.001f);
    TEST_ASSERT_EQUAL_INT(ET_FLYING, et_phase(et));
}

void test_et_fly_phase_active(void)
{
    earth_transition_t et = et_create();
    camera_pose_t current = et_space_pose();
    et = et_begin(et, ET_DIR_TO_EARTH, current);

    /* Advance into FLY phase */
    et = et_tick(et, ET_ANTICIPATE_DURATION + 0.01f);
    TEST_ASSERT_EQUAL_INT(ET_FLYING, et_phase(et));
    TEST_ASSERT_EQUAL_INT(1, et_active(et));

    /* Advance partway through FLY — still flying */
    et = et_tick(et, ET_FLY_DURATION * 0.5f);
    TEST_ASSERT_EQUAL_INT(ET_FLYING, et_phase(et));
}

void test_et_fly_to_settle(void)
{
    earth_transition_t et = et_create();
    camera_pose_t current = et_space_pose();
    et = et_begin(et, ET_DIR_TO_EARTH, current);

    /* Advance past anticipation */
    et = et_tick(et, ET_ANTICIPATE_DURATION + 0.001f);
    TEST_ASSERT_EQUAL_INT(ET_FLYING, et_phase(et));

    /* Advance past fly duration */
    et = et_tick(et, ET_FLY_DURATION + 0.001f);
    TEST_ASSERT_EQUAL_INT(ET_SETTLING, et_phase(et));
}

void test_et_settle_to_arrived(void)
{
    earth_transition_t et = et_create();
    camera_pose_t current = et_space_pose();
    et = et_begin(et, ET_DIR_TO_EARTH, current);

    /* Fast forward to SETTLING */
    et = run_to_phase(et, ET_SETTLING, 0.016f, 200);
    TEST_ASSERT_EQUAL_INT(ET_SETTLING, et_phase(et));

    /* Run settle springs until they converge */
    et = run_to_phase(et, ET_ARRIVED, 0.016f, 500);
    TEST_ASSERT_EQUAL_INT(ET_ARRIVED, et_phase(et));
}

void test_et_arrived_to_idle(void)
{
    earth_transition_t et = et_create();
    camera_pose_t current = et_space_pose();
    et = et_begin(et, ET_DIR_TO_EARTH, current);

    /* Run to ARRIVED */
    et = run_to_phase(et, ET_ARRIVED, 0.016f, 500);
    TEST_ASSERT_EQUAL_INT(ET_ARRIVED, et_phase(et));

    /* One more tick clears to IDLE */
    et = et_tick(et, 0.016f);
    TEST_ASSERT_EQUAL_INT(ET_IDLE, et_phase(et));
}

void test_et_progress_zero_at_start(void)
{
    earth_transition_t et = et_create();
    camera_pose_t current = et_space_pose();
    et = et_begin(et, ET_DIR_TO_EARTH, current);

    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, et_progress(et));
}

void test_et_progress_increases(void)
{
    earth_transition_t et = et_create();
    camera_pose_t current = et_space_pose();
    et = et_begin(et, ET_DIR_TO_EARTH, current);

    float p0 = et_progress(et);
    et = et_tick(et, 0.2f);
    float p1 = et_progress(et);
    et = et_tick(et, 0.3f);
    float p2 = et_progress(et);

    TEST_ASSERT_TRUE(p1 > p0);
    TEST_ASSERT_TRUE(p2 > p1);
}

void test_et_progress_one_at_arrived(void)
{
    earth_transition_t et = et_create();
    camera_pose_t current = et_space_pose();
    et = et_begin(et, ET_DIR_TO_EARTH, current);

    /* Run to ARRIVED */
    et = run_to_phase(et, ET_ARRIVED, 0.016f, 500);
    TEST_ASSERT_TRUE(et_progress(et) >= 0.99f);
}

void test_et_pose_starts_at_origin(void)
{
    earth_transition_t et = et_create();
    camera_pose_t current = et_space_pose();
    et = et_begin(et, ET_DIR_TO_EARTH, current);

    camera_pose_t pose = et_pose(et);
    /* Before any tick, the pose should be close to origin (counter-zoom not yet applied) */
    TEST_ASSERT_FLOAT_WITHIN(0.5f, current.azimuth, pose.azimuth);
    TEST_ASSERT_FLOAT_WITHIN(0.5f, current.elevation, pose.elevation);
}

void test_et_pose_at_destination(void)
{
    earth_transition_t et = et_create();
    camera_pose_t current = et_space_pose();
    et = et_begin(et, ET_DIR_TO_EARTH, current);

    /* Run to ARRIVED */
    et = run_to_phase(et, ET_ARRIVED, 0.016f, 500);
    camera_pose_t dest = et_earth_pose();
    camera_pose_t pose = et_pose(et);

    TEST_ASSERT_FLOAT_WITHIN(0.05f, dest.azimuth, pose.azimuth);
    TEST_ASSERT_FLOAT_WITHIN(0.05f, dest.elevation, pose.elevation);
    TEST_ASSERT_FLOAT_WITHIN(0.05f, dest.log_zoom, pose.log_zoom);
}

void test_et_cancel_returns_idle(void)
{
    earth_transition_t et = et_create();
    camera_pose_t current = et_space_pose();
    et = et_begin(et, ET_DIR_TO_EARTH, current);

    /* Cancel from ANTICIPATE */
    et = et_cancel(et);
    TEST_ASSERT_EQUAL_INT(ET_IDLE, et_phase(et));
    TEST_ASSERT_EQUAL_INT(0, et_active(et));
}

void test_et_cancel_during_fly(void)
{
    earth_transition_t et = et_create();
    camera_pose_t current = et_space_pose();
    et = et_begin(et, ET_DIR_TO_EARTH, current);

    /* Advance into FLY */
    et = et_tick(et, ET_ANTICIPATE_DURATION + 0.1f);
    TEST_ASSERT_EQUAL_INT(ET_FLYING, et_phase(et));

    et = et_cancel(et);
    TEST_ASSERT_EQUAL_INT(ET_IDLE, et_phase(et));
}

void test_et_space_pose_values(void)
{
    camera_pose_t sp = et_space_pose();
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, sp.azimuth);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.3f, sp.elevation);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 2.0f, sp.log_zoom);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, sp.target.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, sp.target.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, sp.target.z);
}

void test_et_earth_pose_values(void)
{
    camera_pose_t ep = et_earth_pose();
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, ep.azimuth);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.8f, ep.elevation);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, -1.0f, ep.log_zoom);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, ep.target.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, ep.target.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, ep.target.z);
}

void test_et_not_active_when_idle(void)
{
    earth_transition_t et = et_create();
    TEST_ASSERT_EQUAL_INT(0, et_active(et));
}

void test_et_active_during_transition(void)
{
    earth_transition_t et = et_create();
    camera_pose_t current = et_space_pose();
    et = et_begin(et, ET_DIR_TO_EARTH, current);

    /* Active during anticipation */
    TEST_ASSERT_EQUAL_INT(1, et_active(et));

    /* Active during flight */
    et = et_tick(et, ET_ANTICIPATE_DURATION + 0.01f);
    TEST_ASSERT_EQUAL_INT(1, et_active(et));
}

void test_et_begin_while_active_restarts(void)
{
    earth_transition_t et = et_create();
    camera_pose_t current = et_space_pose();
    et = et_begin(et, ET_DIR_TO_EARTH, current);

    /* Advance partway */
    et = et_tick(et, ET_ANTICIPATE_DURATION + 0.3f);
    TEST_ASSERT_EQUAL_INT(ET_FLYING, et_phase(et));

    /* Begin again from a different pose */
    camera_pose_t mid_pose = et_pose(et);
    et = et_begin(et, ET_DIR_TO_SPACE, mid_pose);

    /* Should restart from ANTICIPATE with new direction */
    TEST_ASSERT_EQUAL_INT(ET_ANTICIPATE, et_phase(et));
    TEST_ASSERT_EQUAL_INT(ET_DIR_TO_SPACE, et_direction(et));
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, et_progress(et));
}

void test_et_just_arrived_flag(void)
{
    earth_transition_t et = et_create();

    /* Not arrived when idle */
    TEST_ASSERT_EQUAL_INT(0, et_just_arrived(et));

    camera_pose_t current = et_space_pose();
    et = et_begin(et, ET_DIR_TO_EARTH, current);

    /* Not arrived during flight */
    et = et_tick(et, ET_ANTICIPATE_DURATION + 0.1f);
    TEST_ASSERT_EQUAL_INT(0, et_just_arrived(et));

    /* Run to ARRIVED */
    et = run_to_phase(et, ET_ARRIVED, 0.016f, 500);
    TEST_ASSERT_EQUAL_INT(1, et_just_arrived(et));

    /* After next tick, no longer "just arrived" */
    et = et_tick(et, 0.016f);
    TEST_ASSERT_EQUAL_INT(0, et_just_arrived(et));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_et_create_idle);
    RUN_TEST(test_et_begin_to_earth_starts_anticipate);
    RUN_TEST(test_et_begin_to_space_starts_anticipate);
    RUN_TEST(test_et_anticipate_duration);
    RUN_TEST(test_et_anticipate_to_fly);
    RUN_TEST(test_et_fly_phase_active);
    RUN_TEST(test_et_fly_to_settle);
    RUN_TEST(test_et_settle_to_arrived);
    RUN_TEST(test_et_arrived_to_idle);
    RUN_TEST(test_et_progress_zero_at_start);
    RUN_TEST(test_et_progress_increases);
    RUN_TEST(test_et_progress_one_at_arrived);
    RUN_TEST(test_et_pose_starts_at_origin);
    RUN_TEST(test_et_pose_at_destination);
    RUN_TEST(test_et_cancel_returns_idle);
    RUN_TEST(test_et_cancel_during_fly);
    RUN_TEST(test_et_space_pose_values);
    RUN_TEST(test_et_earth_pose_values);
    RUN_TEST(test_et_not_active_when_idle);
    RUN_TEST(test_et_active_during_transition);
    RUN_TEST(test_et_begin_while_active_restarts);
    RUN_TEST(test_et_just_arrived_flag);
    return UNITY_END();
}
