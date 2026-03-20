/* test_birth_flight.c — Tests for birth moment transition choreography. */

#include "../../tests/unity/unity.h"
#include "../../src/ui/birth_flight.h"
#include <math.h>

void setUp(void) {}
void tearDown(void) {}

/* --- Helper --- */

static camera_pose_t test_pose(void)
{
    return camera_pose_create(0.5f, 0.3f, 2.0f, vec3_create(0, 0, 0));
}

/* Advance bf through a full phase by ticking in small steps */
static birth_flight_t tick_through(birth_flight_t bf, float duration)
{
    float step = 1.0f / 60.0f;
    float t = 0.0f;
    while (t < duration) {
        float dt = step;
        if (t + dt > duration) { dt = duration - t; }
        bf = bf_tick(bf, dt);
        t += dt;
    }
    return bf;
}

/* --- Tests --- */

void test_bf_create_idle(void)
{
    birth_flight_t bf = bf_create();
    TEST_ASSERT_EQUAL_INT(BF_IDLE, bf_phase(bf));
    TEST_ASSERT_EQUAL_INT(0, bf_active(bf));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, bf_dim(bf));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, bf_scrub(bf));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, bf_reveal(bf));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, bf_progress(bf));
}

void test_bf_begin_starts_prepare(void)
{
    birth_flight_t bf = bf_create();
    bf = bf_begin(bf, test_pose());
    TEST_ASSERT_EQUAL_INT(BF_PREPARE, bf_phase(bf));
    TEST_ASSERT_EQUAL_INT(1, bf_active(bf));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, bf_dim(bf));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, bf_scrub(bf));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, bf_reveal(bf));
}

void test_bf_prepare_dims(void)
{
    birth_flight_t bf = bf_create();
    bf = bf_begin(bf, test_pose());

    /* Halfway through PREPARE */
    bf = bf_tick(bf, BF_PREPARE_DURATION * 0.5f);
    TEST_ASSERT_EQUAL_INT(BF_PREPARE, bf_phase(bf));
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 0.5f, bf_dim(bf));
}

void test_bf_prepare_zooms_out(void)
{
    birth_flight_t bf = bf_create();
    camera_pose_t start = test_pose();
    bf = bf_begin(bf, start);

    /* Halfway through PREPARE: camera zoom should be between home and travel */
    bf = bf_tick(bf, BF_PREPARE_DURATION * 0.5f);
    camera_pose_t pose = bf_pose(bf);
    TEST_ASSERT_TRUE(pose.log_zoom > start.log_zoom);
    TEST_ASSERT_TRUE(pose.log_zoom < start.log_zoom + BF_TRAVEL_ZOOM_OFFSET);
}

void test_bf_prepare_to_travel(void)
{
    birth_flight_t bf = bf_create();
    bf = bf_begin(bf, test_pose());

    /* Tick past PREPARE duration */
    bf = bf_tick(bf, BF_PREPARE_DURATION + 0.01f);
    TEST_ASSERT_EQUAL_INT(BF_TRAVELING, bf_phase(bf));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, bf_dim(bf));
}

void test_bf_travel_scrubs(void)
{
    birth_flight_t bf = bf_create();
    bf = bf_begin(bf, test_pose());
    bf = tick_through(bf, BF_PREPARE_DURATION + 0.001f);
    TEST_ASSERT_EQUAL_INT(BF_TRAVELING, bf_phase(bf));

    /* Halfway through TRAVEL */
    bf = bf_tick(bf, BF_TRAVEL_DURATION * 0.5f);
    TEST_ASSERT_EQUAL_INT(BF_TRAVELING, bf_phase(bf));
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 0.5f, bf_scrub(bf));
}

void test_bf_travel_dim_stays(void)
{
    birth_flight_t bf = bf_create();
    bf = bf_begin(bf, test_pose());
    bf = tick_through(bf, BF_PREPARE_DURATION + 0.001f);
    TEST_ASSERT_EQUAL_INT(BF_TRAVELING, bf_phase(bf));

    bf = bf_tick(bf, BF_TRAVEL_DURATION * 0.5f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, bf_dim(bf));
}

void test_bf_travel_camera_steady(void)
{
    birth_flight_t bf = bf_create();
    camera_pose_t start = test_pose();
    bf = bf_begin(bf, start);
    bf = tick_through(bf, BF_PREPARE_DURATION + 0.001f);
    TEST_ASSERT_EQUAL_INT(BF_TRAVELING, bf_phase(bf));

    /* Camera should be at travel_pose (zoomed out) during travel */
    camera_pose_t pose = bf_pose(bf);
    float expected_zoom = start.log_zoom + BF_TRAVEL_ZOOM_OFFSET;
    TEST_ASSERT_FLOAT_WITHIN(0.01f, expected_zoom, pose.log_zoom);

    /* Tick further — still at travel_pose */
    bf = bf_tick(bf, BF_TRAVEL_DURATION * 0.5f);
    pose = bf_pose(bf);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, expected_zoom, pose.log_zoom);
}

void test_bf_travel_to_arrive(void)
{
    birth_flight_t bf = bf_create();
    bf = bf_begin(bf, test_pose());
    bf = tick_through(bf, BF_PREPARE_DURATION + 0.001f);
    TEST_ASSERT_EQUAL_INT(BF_TRAVELING, bf_phase(bf));

    /* Tick past TRAVEL duration */
    bf = bf_tick(bf, BF_TRAVEL_DURATION + 0.01f);
    TEST_ASSERT_EQUAL_INT(BF_ARRIVING, bf_phase(bf));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, bf_scrub(bf));
}

void test_bf_arrive_reveals(void)
{
    birth_flight_t bf = bf_create();
    bf = bf_begin(bf, test_pose());
    bf = tick_through(bf, BF_PREPARE_DURATION + 0.001f);
    bf = tick_through(bf, BF_TRAVEL_DURATION + 0.001f);
    TEST_ASSERT_EQUAL_INT(BF_ARRIVING, bf_phase(bf));

    /* Halfway through ARRIVE */
    bf = bf_tick(bf, BF_ARRIVE_DURATION * 0.5f);
    TEST_ASSERT_EQUAL_INT(BF_ARRIVING, bf_phase(bf));
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 0.5f, bf_reveal(bf));
}

void test_bf_arrive_undims(void)
{
    birth_flight_t bf = bf_create();
    bf = bf_begin(bf, test_pose());
    bf = tick_through(bf, BF_PREPARE_DURATION + 0.001f);
    bf = tick_through(bf, BF_TRAVEL_DURATION + 0.001f);
    TEST_ASSERT_EQUAL_INT(BF_ARRIVING, bf_phase(bf));

    /* Halfway through ARRIVE: dim should be decreasing */
    bf = bf_tick(bf, BF_ARRIVE_DURATION * 0.5f);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 0.5f, bf_dim(bf));
    TEST_ASSERT_TRUE(bf_dim(bf) < 1.0f);
}

void test_bf_arrive_zooms_back(void)
{
    birth_flight_t bf = bf_create();
    camera_pose_t start = test_pose();
    bf = bf_begin(bf, start);
    bf = tick_through(bf, BF_PREPARE_DURATION + 0.001f);
    bf = tick_through(bf, BF_TRAVEL_DURATION + 0.001f);
    TEST_ASSERT_EQUAL_INT(BF_ARRIVING, bf_phase(bf));

    /* At start of ARRIVE, camera should be near travel zoom */
    camera_pose_t pose = bf_pose(bf);
    float travel_zoom = start.log_zoom + BF_TRAVEL_ZOOM_OFFSET;
    TEST_ASSERT_FLOAT_WITHIN(0.05f, travel_zoom, pose.log_zoom);

    /* After some time, spring should move zoom toward home */
    bf = bf_tick(bf, BF_ARRIVE_DURATION * 0.5f);
    pose = bf_pose(bf);
    /* Zoom should be between travel and home */
    TEST_ASSERT_TRUE(pose.log_zoom < travel_zoom + 0.01f);
}

void test_bf_arrive_to_done(void)
{
    birth_flight_t bf = bf_create();
    bf = bf_begin(bf, test_pose());
    bf = tick_through(bf, BF_PREPARE_DURATION + 0.001f);
    bf = tick_through(bf, BF_TRAVEL_DURATION + 0.001f);
    TEST_ASSERT_EQUAL_INT(BF_ARRIVING, bf_phase(bf));

    /* Tick past ARRIVE duration */
    bf = tick_through(bf, BF_ARRIVE_DURATION + 0.01f);
    TEST_ASSERT_EQUAL_INT(BF_DONE, bf_phase(bf));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, bf_dim(bf));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, bf_reveal(bf));
}

void test_bf_done_to_idle(void)
{
    birth_flight_t bf = bf_create();
    bf = bf_begin(bf, test_pose());
    bf = tick_through(bf, BF_PREPARE_DURATION + 0.001f);
    bf = tick_through(bf, BF_TRAVEL_DURATION + 0.001f);
    bf = tick_through(bf, BF_ARRIVE_DURATION + 0.01f);
    TEST_ASSERT_EQUAL_INT(BF_DONE, bf_phase(bf));

    /* One more tick: DONE -> IDLE */
    bf = bf_tick(bf, 1.0f / 60.0f);
    TEST_ASSERT_EQUAL_INT(BF_IDLE, bf_phase(bf));
}

void test_bf_progress_monotonic(void)
{
    birth_flight_t bf = bf_create();
    bf = bf_begin(bf, test_pose());

    float prev = bf_progress(bf);
    float step = 1.0f / 60.0f;
    int max_ticks = 300; /* ~5 seconds, well beyond total duration */
    int ticks = 0;

    while (bf_phase(bf) != BF_DONE && bf_phase(bf) != BF_IDLE && ticks < max_ticks) {
        bf = bf_tick(bf, step);
        ticks++;
        float cur = bf_progress(bf);
        TEST_ASSERT_TRUE(cur >= prev - 0.001f); /* monotonically non-decreasing */
        prev = cur;
    }

    /* Should have reached DONE within reasonable time */
    TEST_ASSERT_TRUE(ticks < max_ticks);
    TEST_ASSERT_TRUE(bf_phase(bf) == BF_DONE || bf_phase(bf) == BF_IDLE);
}

void test_bf_cancel_returns_idle(void)
{
    birth_flight_t bf = bf_create();
    bf = bf_begin(bf, test_pose());
    bf = bf_tick(bf, BF_PREPARE_DURATION * 0.5f);
    TEST_ASSERT_EQUAL_INT(BF_PREPARE, bf_phase(bf));

    bf = bf_cancel(bf);
    TEST_ASSERT_EQUAL_INT(BF_IDLE, bf_phase(bf));
    TEST_ASSERT_EQUAL_INT(0, bf_active(bf));
}

void test_bf_cancel_resets_blends(void)
{
    birth_flight_t bf = bf_create();
    bf = bf_begin(bf, test_pose());
    bf = tick_through(bf, BF_PREPARE_DURATION + 0.001f);
    bf = bf_tick(bf, BF_TRAVEL_DURATION * 0.5f);

    /* Mid-travel: scrub and dim should be nonzero */
    TEST_ASSERT_TRUE(bf_scrub(bf) > 0.0f);
    TEST_ASSERT_TRUE(bf_dim(bf) > 0.0f);

    bf = bf_cancel(bf);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, bf_dim(bf));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, bf_scrub(bf));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, bf_reveal(bf));
}

void test_bf_skip_snaps_to_end(void)
{
    birth_flight_t bf = bf_create();
    bf = bf_begin(bf, test_pose());
    bf = bf_tick(bf, BF_PREPARE_DURATION * 0.5f);

    bf = bf_skip(bf);
    TEST_ASSERT_EQUAL_INT(BF_DONE, bf_phase(bf));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, bf_scrub(bf));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, bf_reveal(bf));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, bf_dim(bf));
}

void test_bf_active_during_transition(void)
{
    birth_flight_t bf = bf_create();
    bf = bf_begin(bf, test_pose());
    TEST_ASSERT_EQUAL_INT(1, bf_active(bf));

    bf = bf_tick(bf, BF_PREPARE_DURATION * 0.5f);
    TEST_ASSERT_EQUAL_INT(1, bf_active(bf));

    bf = tick_through(bf, BF_PREPARE_DURATION);
    TEST_ASSERT_EQUAL_INT(1, bf_active(bf)); /* TRAVELING */

    bf = tick_through(bf, BF_TRAVEL_DURATION + 0.001f);
    TEST_ASSERT_EQUAL_INT(1, bf_active(bf)); /* ARRIVING */
}

void test_bf_not_active_idle(void)
{
    birth_flight_t bf = bf_create();
    TEST_ASSERT_EQUAL_INT(0, bf_active(bf));
}

void test_bf_just_done_flag(void)
{
    birth_flight_t bf = bf_create();
    bf = bf_begin(bf, test_pose());
    TEST_ASSERT_EQUAL_INT(0, bf_just_done(bf));

    /* Run through entire sequence */
    bf = tick_through(bf, BF_PREPARE_DURATION + 0.001f);
    TEST_ASSERT_EQUAL_INT(0, bf_just_done(bf));

    bf = tick_through(bf, BF_TRAVEL_DURATION + 0.001f);
    TEST_ASSERT_EQUAL_INT(0, bf_just_done(bf));

    bf = tick_through(bf, BF_ARRIVE_DURATION + 0.01f);
    TEST_ASSERT_EQUAL_INT(1, bf_just_done(bf)); /* BF_DONE */

    /* Next tick: DONE -> IDLE, no longer just_done */
    bf = bf_tick(bf, 1.0f / 60.0f);
    TEST_ASSERT_EQUAL_INT(0, bf_just_done(bf));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_bf_create_idle);
    RUN_TEST(test_bf_begin_starts_prepare);
    RUN_TEST(test_bf_prepare_dims);
    RUN_TEST(test_bf_prepare_zooms_out);
    RUN_TEST(test_bf_prepare_to_travel);
    RUN_TEST(test_bf_travel_scrubs);
    RUN_TEST(test_bf_travel_dim_stays);
    RUN_TEST(test_bf_travel_camera_steady);
    RUN_TEST(test_bf_travel_to_arrive);
    RUN_TEST(test_bf_arrive_reveals);
    RUN_TEST(test_bf_arrive_undims);
    RUN_TEST(test_bf_arrive_zooms_back);
    RUN_TEST(test_bf_arrive_to_done);
    RUN_TEST(test_bf_done_to_idle);
    RUN_TEST(test_bf_progress_monotonic);
    RUN_TEST(test_bf_cancel_returns_idle);
    RUN_TEST(test_bf_cancel_resets_blends);
    RUN_TEST(test_bf_skip_snaps_to_end);
    RUN_TEST(test_bf_active_during_transition);
    RUN_TEST(test_bf_not_active_idle);
    RUN_TEST(test_bf_just_done_flag);
    return UNITY_END();
}
