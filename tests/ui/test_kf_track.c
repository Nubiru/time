#include "../unity/unity.h"
#include "ui/kf_track.h"

#define TOL 0.01f

void setUp(void) {}
void tearDown(void) {}

/* --- test_kf_track_create_empty --- */
void test_kf_track_create_empty(void)
{
    kf_track_t tr = kf_track_create();
    TEST_ASSERT_EQUAL_INT(0, tr.count);
    TEST_ASSERT_EQUAL_INT(0, tr.started);
    TEST_ASSERT_EQUAL_INT(1, kf_track_done(tr)); /* empty = done */
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, kf_track_duration(tr));
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, kf_track_value(tr));
}

/* --- test_kf_track_add_keys --- */
void test_kf_track_add_keys(void)
{
    kf_track_t tr = kf_track_create();
    tr = kf_track_add(tr, 0.0f, 10.0f, EASE_TYPE_LINEAR);
    TEST_ASSERT_EQUAL_INT(1, tr.count);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, tr.keys[0].time);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 10.0f, tr.keys[0].value);

    tr = kf_track_add(tr, 1.0f, 20.0f, EASE_TYPE_OUT_CUBIC);
    TEST_ASSERT_EQUAL_INT(2, tr.count);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, tr.keys[1].time);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 20.0f, tr.keys[1].value);
    TEST_ASSERT_EQUAL_INT(EASE_TYPE_OUT_CUBIC, tr.keys[1].easing);

    tr = kf_track_add(tr, 3.0f, 5.0f, EASE_TYPE_IN_QUAD);
    TEST_ASSERT_EQUAL_INT(3, tr.count);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 3.0f, tr.keys[2].time);
}

/* --- test_kf_track_single_key --- */
void test_kf_track_single_key(void)
{
    kf_track_t tr = kf_track_create();
    tr = kf_track_add(tr, 1.0f, 42.0f, EASE_TYPE_LINEAR);
    tr = kf_track_start(tr);

    /* before key time */
    tr = kf_track_tick(tr, 0.5f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 42.0f, kf_track_value(tr));
    TEST_ASSERT_EQUAL_INT(0, kf_track_done(tr));

    /* at key time */
    tr = kf_track_tick(tr, 0.5f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 42.0f, kf_track_value(tr));
    TEST_ASSERT_EQUAL_INT(1, kf_track_done(tr));

    /* after key time */
    tr = kf_track_tick(tr, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 42.0f, kf_track_value(tr));
}

/* --- test_kf_track_two_keys_linear --- */
void test_kf_track_two_keys_linear(void)
{
    kf_track_t tr = kf_track_create();
    tr = kf_track_add(tr, 0.0f, 0.0f, EASE_TYPE_LINEAR);
    tr = kf_track_add(tr, 2.0f, 100.0f, EASE_TYPE_LINEAR);
    tr = kf_track_start(tr);

    /* midpoint: t=1.0, local_t=0.5, linear -> 50.0 */
    tr = kf_track_tick(tr, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 50.0f, kf_track_value(tr));

    /* quarter: t=0.5 total, so tick 0 more — already at 1.0.
     * Go to 1.5 total -> local_t=0.75 -> 75.0 */
    tr = kf_track_tick(tr, 0.5f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 75.0f, kf_track_value(tr));
}

/* --- test_kf_track_two_keys_eased --- */
void test_kf_track_two_keys_eased(void)
{
    kf_track_t tr = kf_track_create();
    tr = kf_track_add(tr, 0.0f, 0.0f, EASE_TYPE_LINEAR);
    tr = kf_track_add(tr, 2.0f, 100.0f, EASE_TYPE_OUT_CUBIC);
    tr = kf_track_start(tr);

    /* midpoint: local_t=0.5, out_cubic(0.5) = 1 - (1-0.5)^3 = 1 - 0.125 = 0.875 */
    tr = kf_track_tick(tr, 1.0f);
    float eased_val = kf_track_value(tr);
    /* OUT_CUBIC at 0.5 should give ~87.5, definitely not 50 */
    TEST_ASSERT_TRUE(eased_val > 60.0f);
    TEST_ASSERT_TRUE(eased_val < 95.0f);
    /* Should differ from linear */
    TEST_ASSERT_TRUE(eased_val != 50.0f);
}

/* --- test_kf_track_value_before_first --- */
void test_kf_track_value_before_first(void)
{
    kf_track_t tr = kf_track_create();
    tr = kf_track_add(tr, 1.0f, 77.0f, EASE_TYPE_LINEAR);
    tr = kf_track_add(tr, 3.0f, 99.0f, EASE_TYPE_LINEAR);
    tr = kf_track_start(tr);

    /* elapsed=0 < first key time=1.0: return first value */
    TEST_ASSERT_FLOAT_WITHIN(TOL, 77.0f, kf_track_value(tr));

    tr = kf_track_tick(tr, 0.5f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 77.0f, kf_track_value(tr));
}

/* --- test_kf_track_value_after_last --- */
void test_kf_track_value_after_last(void)
{
    kf_track_t tr = kf_track_create();
    tr = kf_track_add(tr, 0.0f, 10.0f, EASE_TYPE_LINEAR);
    tr = kf_track_add(tr, 1.0f, 50.0f, EASE_TYPE_LINEAR);
    tr = kf_track_start(tr);

    tr = kf_track_tick(tr, 5.0f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 50.0f, kf_track_value(tr));
    TEST_ASSERT_EQUAL_INT(1, kf_track_done(tr));
}

/* --- test_kf_track_three_keys --- */
void test_kf_track_three_keys(void)
{
    kf_track_t tr = kf_track_create();
    tr = kf_track_add(tr, 0.0f, 0.0f, EASE_TYPE_LINEAR);
    tr = kf_track_add(tr, 1.0f, 100.0f, EASE_TYPE_LINEAR);
    tr = kf_track_add(tr, 2.0f, 50.0f, EASE_TYPE_LINEAR);
    tr = kf_track_start(tr);

    /* segment 0 midpoint: 0->100, local_t=0.5 -> 50 */
    tr = kf_track_tick(tr, 0.5f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 50.0f, kf_track_value(tr));

    /* at key 1: value=100 */
    tr = kf_track_tick(tr, 0.5f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 100.0f, kf_track_value(tr));

    /* segment 1 midpoint: 100->50, local_t=0.5 -> 75 */
    tr = kf_track_tick(tr, 0.5f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 75.0f, kf_track_value(tr));

    /* at end: value=50 */
    tr = kf_track_tick(tr, 0.5f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 50.0f, kf_track_value(tr));
}

/* --- test_kf_track_hold_pattern --- */
void test_kf_track_hold_pattern(void)
{
    /* A(0s) -> A(2s) -> B(3s) — hold at A for 2 seconds, then ease to B */
    kf_track_t tr = kf_track_create();
    tr = kf_track_add(tr, 0.0f, 10.0f, EASE_TYPE_LINEAR);
    tr = kf_track_add(tr, 2.0f, 10.0f, EASE_TYPE_LINEAR);
    tr = kf_track_add(tr, 3.0f, 30.0f, EASE_TYPE_LINEAR);
    tr = kf_track_start(tr);

    /* During hold: value stays at 10 */
    tr = kf_track_tick(tr, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 10.0f, kf_track_value(tr));

    /* At end of hold: still 10 */
    tr = kf_track_tick(tr, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 10.0f, kf_track_value(tr));

    /* Midway through transition: 10->30 at 0.5 -> 20 */
    tr = kf_track_tick(tr, 0.5f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 20.0f, kf_track_value(tr));

    /* At end: 30 */
    tr = kf_track_tick(tr, 0.5f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 30.0f, kf_track_value(tr));
}

/* --- test_kf_track_bounce_pattern --- */
void test_kf_track_bounce_pattern(void)
{
    /* A(0s) -> B(1s) -> A(2s) — out and back */
    kf_track_t tr = kf_track_create();
    tr = kf_track_add(tr, 0.0f, 0.0f, EASE_TYPE_LINEAR);
    tr = kf_track_add(tr, 1.0f, 100.0f, EASE_TYPE_LINEAR);
    tr = kf_track_add(tr, 2.0f, 0.0f, EASE_TYPE_LINEAR);
    tr = kf_track_start(tr);

    /* up: midpoint -> 50 */
    tr = kf_track_tick(tr, 0.5f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 50.0f, kf_track_value(tr));

    /* peak -> 100 */
    tr = kf_track_tick(tr, 0.5f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 100.0f, kf_track_value(tr));

    /* down: midpoint -> 50 */
    tr = kf_track_tick(tr, 0.5f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 50.0f, kf_track_value(tr));

    /* back to 0 */
    tr = kf_track_tick(tr, 0.5f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, kf_track_value(tr));
}

/* --- test_kf_track_done_at_end --- */
void test_kf_track_done_at_end(void)
{
    kf_track_t tr = kf_track_create();
    tr = kf_track_add(tr, 0.0f, 0.0f, EASE_TYPE_LINEAR);
    tr = kf_track_add(tr, 2.0f, 10.0f, EASE_TYPE_LINEAR);
    tr = kf_track_start(tr);

    tr = kf_track_tick(tr, 2.0f);
    TEST_ASSERT_EQUAL_INT(1, kf_track_done(tr));
}

/* --- test_kf_track_not_done_midway --- */
void test_kf_track_not_done_midway(void)
{
    kf_track_t tr = kf_track_create();
    tr = kf_track_add(tr, 0.0f, 0.0f, EASE_TYPE_LINEAR);
    tr = kf_track_add(tr, 4.0f, 10.0f, EASE_TYPE_LINEAR);
    tr = kf_track_start(tr);

    tr = kf_track_tick(tr, 2.0f);
    TEST_ASSERT_EQUAL_INT(0, kf_track_done(tr));
}

/* --- test_kf_track_progress_linear --- */
void test_kf_track_progress_linear(void)
{
    kf_track_t tr = kf_track_create();
    tr = kf_track_add(tr, 0.0f, 0.0f, EASE_TYPE_LINEAR);
    tr = kf_track_add(tr, 4.0f, 10.0f, EASE_TYPE_LINEAR);
    tr = kf_track_start(tr);

    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, kf_track_progress(tr));

    tr = kf_track_tick(tr, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.25f, kf_track_progress(tr));

    tr = kf_track_tick(tr, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.5f, kf_track_progress(tr));

    tr = kf_track_tick(tr, 2.0f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, kf_track_progress(tr));
}

/* --- test_kf_track_segment_tracking --- */
void test_kf_track_segment_tracking(void)
{
    kf_track_t tr = kf_track_create();
    tr = kf_track_add(tr, 0.0f, 0.0f, EASE_TYPE_LINEAR);
    tr = kf_track_add(tr, 1.0f, 10.0f, EASE_TYPE_LINEAR);
    tr = kf_track_add(tr, 2.0f, 20.0f, EASE_TYPE_LINEAR);
    tr = kf_track_add(tr, 3.0f, 30.0f, EASE_TYPE_LINEAR);
    tr = kf_track_start(tr);

    /* at start (elapsed=0): segment 0 */
    TEST_ASSERT_EQUAL_INT(0, kf_track_segment(tr));

    /* in first segment */
    tr = kf_track_tick(tr, 0.5f);
    TEST_ASSERT_EQUAL_INT(0, kf_track_segment(tr));

    /* in second segment */
    tr = kf_track_tick(tr, 1.0f);
    TEST_ASSERT_EQUAL_INT(1, kf_track_segment(tr));

    /* in third segment */
    tr = kf_track_tick(tr, 1.0f);
    TEST_ASSERT_EQUAL_INT(2, kf_track_segment(tr));

    /* past end: last segment */
    tr = kf_track_tick(tr, 1.0f);
    TEST_ASSERT_EQUAL_INT(2, kf_track_segment(tr));
}

/* --- test_kf_track_not_started --- */
void test_kf_track_not_started(void)
{
    kf_track_t tr = kf_track_create();
    tr = kf_track_add(tr, 0.0f, 55.0f, EASE_TYPE_LINEAR);
    tr = kf_track_add(tr, 1.0f, 99.0f, EASE_TYPE_LINEAR);

    /* not started: value = first key value */
    TEST_ASSERT_FLOAT_WITHIN(TOL, 55.0f, kf_track_value(tr));
    TEST_ASSERT_EQUAL_INT(-1, kf_track_segment(tr));
}

/* --- test_kf_track_empty_track --- */
void test_kf_track_empty_track(void)
{
    kf_track_t tr = kf_track_create();

    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, kf_track_value(tr));
    TEST_ASSERT_EQUAL_INT(1, kf_track_done(tr));
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, kf_track_duration(tr));
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, kf_track_progress(tr));
    TEST_ASSERT_EQUAL_INT(-1, kf_track_segment(tr));
}

/* --- test_kf_track_max_keys --- */
void test_kf_track_max_keys(void)
{
    kf_track_t tr = kf_track_create();
    for (int i = 0; i < KF_MAX_KEYS; i++) {
        tr = kf_track_add(tr, (float)i, (float)(i * 10), EASE_TYPE_LINEAR);
    }
    TEST_ASSERT_EQUAL_INT(KF_MAX_KEYS, tr.count);

    /* 33rd key is ignored */
    tr = kf_track_add(tr, 100.0f, 999.0f, EASE_TYPE_LINEAR);
    TEST_ASSERT_EQUAL_INT(KF_MAX_KEYS, tr.count);
}

/* --- test_kf_track_large_dt --- */
void test_kf_track_large_dt(void)
{
    kf_track_t tr = kf_track_create();
    tr = kf_track_add(tr, 0.0f, 0.0f, EASE_TYPE_LINEAR);
    tr = kf_track_add(tr, 1.0f, 50.0f, EASE_TYPE_LINEAR);
    tr = kf_track_add(tr, 2.0f, 100.0f, EASE_TYPE_LINEAR);
    tr = kf_track_start(tr);

    /* Jump far past end */
    tr = kf_track_tick(tr, 1000.0f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 100.0f, kf_track_value(tr));
    TEST_ASSERT_EQUAL_INT(1, kf_track_done(tr));
}

/* --- test_kf_track_zero_segment --- */
void test_kf_track_zero_segment(void)
{
    /* Two keys at the same time: snap to later value */
    kf_track_t tr = kf_track_create();
    tr = kf_track_add(tr, 0.0f, 10.0f, EASE_TYPE_LINEAR);
    tr = kf_track_add(tr, 0.0f, 50.0f, EASE_TYPE_LINEAR);
    tr = kf_track_add(tr, 1.0f, 100.0f, EASE_TYPE_LINEAR);
    tr = kf_track_start(tr);

    /* At time 0: zero-length segment should snap to 50 */
    TEST_ASSERT_FLOAT_WITHIN(TOL, 50.0f, kf_track_value(tr));

    /* Then normal interpolation 50->100 */
    tr = kf_track_tick(tr, 0.5f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 75.0f, kf_track_value(tr));
}

/* --- test_kf_track_tick_after_done --- */
void test_kf_track_tick_after_done(void)
{
    kf_track_t tr = kf_track_create();
    tr = kf_track_add(tr, 0.0f, 0.0f, EASE_TYPE_LINEAR);
    tr = kf_track_add(tr, 1.0f, 100.0f, EASE_TYPE_LINEAR);
    tr = kf_track_start(tr);

    /* Run to completion */
    tr = kf_track_tick(tr, 1.0f);
    TEST_ASSERT_EQUAL_INT(1, kf_track_done(tr));
    TEST_ASSERT_FLOAT_WITHIN(TOL, 100.0f, kf_track_value(tr));

    /* Tick after done: no change */
    tr = kf_track_tick(tr, 5.0f);
    TEST_ASSERT_EQUAL_INT(1, kf_track_done(tr));
    TEST_ASSERT_FLOAT_WITHIN(TOL, 100.0f, kf_track_value(tr));
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, kf_track_progress(tr));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_kf_track_create_empty);
    RUN_TEST(test_kf_track_add_keys);
    RUN_TEST(test_kf_track_single_key);
    RUN_TEST(test_kf_track_two_keys_linear);
    RUN_TEST(test_kf_track_two_keys_eased);
    RUN_TEST(test_kf_track_value_before_first);
    RUN_TEST(test_kf_track_value_after_last);
    RUN_TEST(test_kf_track_three_keys);
    RUN_TEST(test_kf_track_hold_pattern);
    RUN_TEST(test_kf_track_bounce_pattern);
    RUN_TEST(test_kf_track_done_at_end);
    RUN_TEST(test_kf_track_not_done_midway);
    RUN_TEST(test_kf_track_progress_linear);
    RUN_TEST(test_kf_track_segment_tracking);
    RUN_TEST(test_kf_track_not_started);
    RUN_TEST(test_kf_track_empty_track);
    RUN_TEST(test_kf_track_max_keys);
    RUN_TEST(test_kf_track_large_dt);
    RUN_TEST(test_kf_track_zero_segment);
    RUN_TEST(test_kf_track_tick_after_done);
    return UNITY_END();
}
