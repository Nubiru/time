#include "../unity/unity.h"
#include "ui/drama_seq.h"

#define TOL 0.01f

void setUp(void) {}
void tearDown(void) {}

/* --- test_drama_seq_create_empty --- */
void test_drama_seq_create_empty(void)
{
    drama_seq_t seq = drama_seq_create();
    TEST_ASSERT_EQUAL_INT(0, seq.started);
    TEST_ASSERT_EQUAL_INT(0, seq.marker_count);
    TEST_ASSERT_EQUAL_INT(0, seq.paused);
    TEST_ASSERT_EQUAL_INT(1, drama_seq_done(seq)); /* empty = done */
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, drama_seq_duration(seq));
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, seq.elapsed);
}

/* --- test_drama_seq_add_key_creates_track --- */
void test_drama_seq_add_key_creates_track(void)
{
    drama_seq_t seq = drama_seq_create();
    TEST_ASSERT_EQUAL_INT(0, seq.track_active[DRAMA_CH_CAMERA_AZ]);

    seq = drama_seq_add_key(seq, DRAMA_CH_CAMERA_AZ,
                            0.0f, 1.0f, EASE_TYPE_LINEAR);
    TEST_ASSERT_EQUAL_INT(1, seq.track_active[DRAMA_CH_CAMERA_AZ]);
    TEST_ASSERT_EQUAL_INT(1, seq.tracks[DRAMA_CH_CAMERA_AZ].count);
}

/* --- test_drama_seq_add_marker --- */
void test_drama_seq_add_marker(void)
{
    drama_seq_t seq = drama_seq_create();
    TEST_ASSERT_EQUAL_INT(0, seq.marker_count);

    seq = drama_seq_add_marker(seq, 1.5f, 42);
    TEST_ASSERT_EQUAL_INT(1, seq.marker_count);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.5f, seq.markers[0].time);
    TEST_ASSERT_EQUAL_INT(42, seq.markers[0].id);

    seq = drama_seq_add_marker(seq, 3.0f, 99);
    TEST_ASSERT_EQUAL_INT(2, seq.marker_count);
}

/* --- test_drama_seq_start --- */
void test_drama_seq_start(void)
{
    drama_seq_t seq = drama_seq_create();
    seq = drama_seq_add_key(seq, DRAMA_CH_OVERLAY,
                            0.0f, 0.0f, EASE_TYPE_LINEAR);
    seq = drama_seq_add_key(seq, DRAMA_CH_OVERLAY,
                            2.0f, 1.0f, EASE_TYPE_LINEAR);
    seq = drama_seq_start(seq);

    TEST_ASSERT_EQUAL_INT(1, seq.started);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, seq.elapsed);
    TEST_ASSERT_EQUAL_INT(0, seq.done);
}

/* --- test_drama_seq_single_channel --- */
void test_drama_seq_single_channel(void)
{
    drama_seq_t seq = drama_seq_create();
    seq = drama_seq_add_key(seq, DRAMA_CH_CAMERA_DIST,
                            0.0f, 10.0f, EASE_TYPE_LINEAR);
    seq = drama_seq_add_key(seq, DRAMA_CH_CAMERA_DIST,
                            2.0f, 30.0f, EASE_TYPE_LINEAR);
    seq = drama_seq_start(seq);

    /* midpoint: t=1.0, linear -> 20.0 */
    seq = drama_seq_tick(seq, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 20.0f,
                             drama_seq_value(seq, DRAMA_CH_CAMERA_DIST));

    /* at end: t=2.0 -> 30.0 */
    seq = drama_seq_tick(seq, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 30.0f,
                             drama_seq_value(seq, DRAMA_CH_CAMERA_DIST));
}

/* --- test_drama_seq_multi_channel --- */
void test_drama_seq_multi_channel(void)
{
    drama_seq_t seq = drama_seq_create();

    /* Channel 0: azimuth 0 -> 3.14 over 2s */
    seq = drama_seq_add_key(seq, DRAMA_CH_CAMERA_AZ,
                            0.0f, 0.0f, EASE_TYPE_LINEAR);
    seq = drama_seq_add_key(seq, DRAMA_CH_CAMERA_AZ,
                            2.0f, 3.14f, EASE_TYPE_LINEAR);

    /* Channel 1: elevation 1.0 -> 0.5 over 4s */
    seq = drama_seq_add_key(seq, DRAMA_CH_CAMERA_EL,
                            0.0f, 1.0f, EASE_TYPE_LINEAR);
    seq = drama_seq_add_key(seq, DRAMA_CH_CAMERA_EL,
                            4.0f, 0.5f, EASE_TYPE_LINEAR);

    /* Channel 3: overlay 0 -> 1 over 1s */
    seq = drama_seq_add_key(seq, DRAMA_CH_OVERLAY,
                            0.0f, 0.0f, EASE_TYPE_LINEAR);
    seq = drama_seq_add_key(seq, DRAMA_CH_OVERLAY,
                            1.0f, 1.0f, EASE_TYPE_LINEAR);

    seq = drama_seq_start(seq);
    seq = drama_seq_tick(seq, 1.0f);

    /* AZ at t=1.0: midpoint of 0->3.14 over 2s = 1.57 */
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.57f,
                             drama_seq_value(seq, DRAMA_CH_CAMERA_AZ));

    /* EL at t=1.0: 1/4 of 1.0->0.5 over 4s = 0.875 */
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.875f,
                             drama_seq_value(seq, DRAMA_CH_CAMERA_EL));

    /* Overlay at t=1.0: end -> 1.0 */
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f,
                             drama_seq_value(seq, DRAMA_CH_OVERLAY));
}

/* --- test_drama_seq_value_inactive_channel --- */
void test_drama_seq_value_inactive_channel(void)
{
    drama_seq_t seq = drama_seq_create();
    seq = drama_seq_add_key(seq, DRAMA_CH_CAMERA_AZ,
                            0.0f, 5.0f, EASE_TYPE_LINEAR);
    seq = drama_seq_start(seq);

    /* Inactive channel returns 0.0 */
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f,
                             drama_seq_value(seq, DRAMA_CH_OVERLAY));
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f,
                             drama_seq_value(seq, DRAMA_CH_CUSTOM_2));
}

/* --- test_drama_seq_marker_fires_on_cross --- */
void test_drama_seq_marker_fires_on_cross(void)
{
    drama_seq_t seq = drama_seq_create();
    /* Need a track so the sequence has duration */
    seq = drama_seq_add_key(seq, DRAMA_CH_DIM,
                            0.0f, 0.0f, EASE_TYPE_LINEAR);
    seq = drama_seq_add_key(seq, DRAMA_CH_DIM,
                            3.0f, 1.0f, EASE_TYPE_LINEAR);
    seq = drama_seq_add_marker(seq, 1.0f, 10);
    seq = drama_seq_start(seq);

    /* Tick to 1.5: crosses marker at 1.0 */
    seq = drama_seq_tick(seq, 1.5f);
    TEST_ASSERT_EQUAL_INT(1, seq.markers_fired[0]);
}

/* --- test_drama_seq_marker_not_before_time --- */
void test_drama_seq_marker_not_before_time(void)
{
    drama_seq_t seq = drama_seq_create();
    seq = drama_seq_add_key(seq, DRAMA_CH_DIM,
                            0.0f, 0.0f, EASE_TYPE_LINEAR);
    seq = drama_seq_add_key(seq, DRAMA_CH_DIM,
                            3.0f, 1.0f, EASE_TYPE_LINEAR);
    seq = drama_seq_add_marker(seq, 1.0f, 10);
    seq = drama_seq_start(seq);

    /* Tick to 0.5: doesn't reach marker at 1.0 */
    seq = drama_seq_tick(seq, 0.5f);
    TEST_ASSERT_EQUAL_INT(0, seq.markers_fired[0]);
}

/* --- test_drama_seq_marker_fires_once --- */
void test_drama_seq_marker_fires_once(void)
{
    drama_seq_t seq = drama_seq_create();
    seq = drama_seq_add_key(seq, DRAMA_CH_DIM,
                            0.0f, 0.0f, EASE_TYPE_LINEAR);
    seq = drama_seq_add_key(seq, DRAMA_CH_DIM,
                            5.0f, 1.0f, EASE_TYPE_LINEAR);
    seq = drama_seq_add_marker(seq, 1.0f, 10);
    seq = drama_seq_start(seq);

    /* First tick crosses marker */
    seq = drama_seq_tick(seq, 1.5f);
    TEST_ASSERT_EQUAL_INT(1, drama_seq_marker_fired(seq, 10));

    /* Second tick: marker should NOT fire again */
    seq = drama_seq_tick(seq, 1.0f);
    TEST_ASSERT_EQUAL_INT(0, drama_seq_marker_fired(seq, 10));
}

/* --- test_drama_seq_marker_fired_by_id --- */
void test_drama_seq_marker_fired_by_id(void)
{
    drama_seq_t seq = drama_seq_create();
    seq = drama_seq_add_key(seq, DRAMA_CH_DIM,
                            0.0f, 0.0f, EASE_TYPE_LINEAR);
    seq = drama_seq_add_key(seq, DRAMA_CH_DIM,
                            5.0f, 1.0f, EASE_TYPE_LINEAR);
    seq = drama_seq_add_marker(seq, 1.0f, 10);
    seq = drama_seq_add_marker(seq, 3.0f, 20);
    seq = drama_seq_start(seq);

    /* Tick to 1.5: only marker 10 fires */
    seq = drama_seq_tick(seq, 1.5f);
    TEST_ASSERT_EQUAL_INT(1, drama_seq_marker_fired(seq, 10));
    TEST_ASSERT_EQUAL_INT(0, drama_seq_marker_fired(seq, 20));
}

/* --- test_drama_seq_last_marker --- */
void test_drama_seq_last_marker(void)
{
    drama_seq_t seq = drama_seq_create();
    seq = drama_seq_add_key(seq, DRAMA_CH_DIM,
                            0.0f, 0.0f, EASE_TYPE_LINEAR);
    seq = drama_seq_add_key(seq, DRAMA_CH_DIM,
                            5.0f, 1.0f, EASE_TYPE_LINEAR);
    seq = drama_seq_add_marker(seq, 0.5f, 10);
    seq = drama_seq_add_marker(seq, 1.0f, 20);
    seq = drama_seq_start(seq);

    /* No markers fired yet */
    TEST_ASSERT_EQUAL_INT(-1, drama_seq_last_marker(seq));

    /* Tick to 1.5: both markers fire, last is 20 */
    seq = drama_seq_tick(seq, 1.5f);
    TEST_ASSERT_EQUAL_INT(20, drama_seq_last_marker(seq));
}

/* --- test_drama_seq_multiple_markers --- */
void test_drama_seq_multiple_markers(void)
{
    drama_seq_t seq = drama_seq_create();
    seq = drama_seq_add_key(seq, DRAMA_CH_DIM,
                            0.0f, 0.0f, EASE_TYPE_LINEAR);
    seq = drama_seq_add_key(seq, DRAMA_CH_DIM,
                            5.0f, 1.0f, EASE_TYPE_LINEAR);
    seq = drama_seq_add_marker(seq, 1.0f, 10);
    seq = drama_seq_add_marker(seq, 2.0f, 20);
    seq = drama_seq_add_marker(seq, 3.0f, 30);
    seq = drama_seq_start(seq);

    /* Tick to 1.5: marker 10 fires */
    seq = drama_seq_tick(seq, 1.5f);
    TEST_ASSERT_EQUAL_INT(1, drama_seq_marker_fired(seq, 10));
    TEST_ASSERT_EQUAL_INT(0, drama_seq_marker_fired(seq, 20));
    TEST_ASSERT_EQUAL_INT(0, drama_seq_marker_fired(seq, 30));

    /* Tick to 2.5: marker 20 fires */
    seq = drama_seq_tick(seq, 1.0f);
    TEST_ASSERT_EQUAL_INT(0, drama_seq_marker_fired(seq, 10)); /* already fired before */
    TEST_ASSERT_EQUAL_INT(1, drama_seq_marker_fired(seq, 20));
    TEST_ASSERT_EQUAL_INT(0, drama_seq_marker_fired(seq, 30));

    /* Tick to 3.5: marker 30 fires */
    seq = drama_seq_tick(seq, 1.0f);
    TEST_ASSERT_EQUAL_INT(1, drama_seq_marker_fired(seq, 30));
}

/* --- test_drama_seq_done_all_tracks --- */
void test_drama_seq_done_all_tracks(void)
{
    drama_seq_t seq = drama_seq_create();
    seq = drama_seq_add_key(seq, DRAMA_CH_CAMERA_AZ,
                            0.0f, 0.0f, EASE_TYPE_LINEAR);
    seq = drama_seq_add_key(seq, DRAMA_CH_CAMERA_AZ,
                            2.0f, 1.0f, EASE_TYPE_LINEAR);
    seq = drama_seq_add_key(seq, DRAMA_CH_OVERLAY,
                            0.0f, 0.0f, EASE_TYPE_LINEAR);
    seq = drama_seq_add_key(seq, DRAMA_CH_OVERLAY,
                            3.0f, 1.0f, EASE_TYPE_LINEAR);
    seq = drama_seq_start(seq);

    /* Tick past longest track (3.0s) */
    seq = drama_seq_tick(seq, 3.5f);
    TEST_ASSERT_EQUAL_INT(1, drama_seq_done(seq));
}

/* --- test_drama_seq_not_done_midway --- */
void test_drama_seq_not_done_midway(void)
{
    drama_seq_t seq = drama_seq_create();
    seq = drama_seq_add_key(seq, DRAMA_CH_CAMERA_AZ,
                            0.0f, 0.0f, EASE_TYPE_LINEAR);
    seq = drama_seq_add_key(seq, DRAMA_CH_CAMERA_AZ,
                            4.0f, 1.0f, EASE_TYPE_LINEAR);
    seq = drama_seq_start(seq);

    seq = drama_seq_tick(seq, 2.0f);
    TEST_ASSERT_EQUAL_INT(0, drama_seq_done(seq));
}

/* --- test_drama_seq_progress --- */
void test_drama_seq_progress(void)
{
    drama_seq_t seq = drama_seq_create();
    seq = drama_seq_add_key(seq, DRAMA_CH_CAMERA_AZ,
                            0.0f, 0.0f, EASE_TYPE_LINEAR);
    seq = drama_seq_add_key(seq, DRAMA_CH_CAMERA_AZ,
                            4.0f, 1.0f, EASE_TYPE_LINEAR);
    seq = drama_seq_start(seq);

    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, drama_seq_progress(seq));

    seq = drama_seq_tick(seq, 2.0f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.5f, drama_seq_progress(seq));

    seq = drama_seq_tick(seq, 2.0f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, drama_seq_progress(seq));
}

/* --- test_drama_seq_duration --- */
void test_drama_seq_duration(void)
{
    drama_seq_t seq = drama_seq_create();
    /* Track: 2s */
    seq = drama_seq_add_key(seq, DRAMA_CH_CAMERA_AZ,
                            0.0f, 0.0f, EASE_TYPE_LINEAR);
    seq = drama_seq_add_key(seq, DRAMA_CH_CAMERA_AZ,
                            2.0f, 1.0f, EASE_TYPE_LINEAR);
    /* Track: 3s */
    seq = drama_seq_add_key(seq, DRAMA_CH_OVERLAY,
                            0.0f, 0.0f, EASE_TYPE_LINEAR);
    seq = drama_seq_add_key(seq, DRAMA_CH_OVERLAY,
                            3.0f, 1.0f, EASE_TYPE_LINEAR);
    /* Marker at 5s */
    seq = drama_seq_add_marker(seq, 5.0f, 99);

    /* Duration = max(2, 3, 5) = 5 */
    TEST_ASSERT_FLOAT_WITHIN(TOL, 5.0f, drama_seq_duration(seq));
}

/* --- test_drama_seq_pause_resume --- */
void test_drama_seq_pause_resume(void)
{
    drama_seq_t seq = drama_seq_create();
    seq = drama_seq_add_key(seq, DRAMA_CH_CAMERA_AZ,
                            0.0f, 0.0f, EASE_TYPE_LINEAR);
    seq = drama_seq_add_key(seq, DRAMA_CH_CAMERA_AZ,
                            4.0f, 100.0f, EASE_TYPE_LINEAR);
    seq = drama_seq_start(seq);

    /* Tick to 1s */
    seq = drama_seq_tick(seq, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 25.0f,
                             drama_seq_value(seq, DRAMA_CH_CAMERA_AZ));

    /* Pause */
    seq = drama_seq_pause(seq);
    TEST_ASSERT_EQUAL_INT(1, seq.paused);

    /* Tick while paused: no advancement */
    seq = drama_seq_tick(seq, 2.0f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 25.0f,
                             drama_seq_value(seq, DRAMA_CH_CAMERA_AZ));

    /* Resume */
    seq = drama_seq_resume(seq);
    TEST_ASSERT_EQUAL_INT(0, seq.paused);

    /* Tick to 2s total (1s more): value should be 50 */
    seq = drama_seq_tick(seq, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 50.0f,
                             drama_seq_value(seq, DRAMA_CH_CAMERA_AZ));
}

/* --- test_drama_seq_channel_active --- */
void test_drama_seq_channel_active(void)
{
    drama_seq_t seq = drama_seq_create();
    seq = drama_seq_add_key(seq, DRAMA_CH_CAMERA_AZ,
                            0.0f, 0.0f, EASE_TYPE_LINEAR);
    seq = drama_seq_add_key(seq, DRAMA_CH_DIM,
                            0.0f, 0.0f, EASE_TYPE_LINEAR);

    TEST_ASSERT_EQUAL_INT(1, drama_seq_channel_active(seq, DRAMA_CH_CAMERA_AZ));
    TEST_ASSERT_EQUAL_INT(0, drama_seq_channel_active(seq, DRAMA_CH_CAMERA_EL));
    TEST_ASSERT_EQUAL_INT(1, drama_seq_channel_active(seq, DRAMA_CH_DIM));
    TEST_ASSERT_EQUAL_INT(0, drama_seq_channel_active(seq, DRAMA_CH_CUSTOM_0));
}

/* --- test_drama_seq_empty_done --- */
void test_drama_seq_empty_done(void)
{
    drama_seq_t seq = drama_seq_create();
    /* Empty sequence is immediately done */
    TEST_ASSERT_EQUAL_INT(1, drama_seq_done(seq));

    /* Even after start */
    seq = drama_seq_start(seq);
    TEST_ASSERT_EQUAL_INT(1, drama_seq_done(seq));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_drama_seq_create_empty);
    RUN_TEST(test_drama_seq_add_key_creates_track);
    RUN_TEST(test_drama_seq_add_marker);
    RUN_TEST(test_drama_seq_start);
    RUN_TEST(test_drama_seq_single_channel);
    RUN_TEST(test_drama_seq_multi_channel);
    RUN_TEST(test_drama_seq_value_inactive_channel);
    RUN_TEST(test_drama_seq_marker_fires_on_cross);
    RUN_TEST(test_drama_seq_marker_not_before_time);
    RUN_TEST(test_drama_seq_marker_fires_once);
    RUN_TEST(test_drama_seq_marker_fired_by_id);
    RUN_TEST(test_drama_seq_last_marker);
    RUN_TEST(test_drama_seq_multiple_markers);
    RUN_TEST(test_drama_seq_done_all_tracks);
    RUN_TEST(test_drama_seq_not_done_midway);
    RUN_TEST(test_drama_seq_progress);
    RUN_TEST(test_drama_seq_duration);
    RUN_TEST(test_drama_seq_pause_resume);
    RUN_TEST(test_drama_seq_channel_active);
    RUN_TEST(test_drama_seq_empty_done);
    return UNITY_END();
}
