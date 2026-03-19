#include "../unity/unity.h"
#include "ui/timeline.h"

#define TOL 0.01f
#define NEAR(exp, act) ((act) >= (exp) - TOL && (act) <= (exp) + TOL)

void setUp(void) {}
void tearDown(void) {}

/* 1. Create: count=0, not started */
void test_timeline_create_empty(void)
{
    timeline_t tl = timeline_create(TL_SEQUENTIAL);
    TEST_ASSERT_EQUAL_INT(0, tl.count);
    TEST_ASSERT_EQUAL_INT(0, tl.started);
    TEST_ASSERT_EQUAL_INT(TL_SEQUENTIAL, tl.mode);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, tl.elapsed);
}

/* 2. Sequential: start times = 0, dur0, dur0+dur1 */
void test_timeline_add_sequential(void)
{
    timeline_t tl = timeline_create(TL_SEQUENTIAL);
    tl = timeline_add(tl, anim_create(0, 1, 1.0f, EASE_TYPE_LINEAR));
    tl = timeline_add(tl, anim_create(0, 1, 2.0f, EASE_TYPE_LINEAR));
    tl = timeline_add(tl, anim_create(0, 1, 0.5f, EASE_TYPE_LINEAR));

    TEST_ASSERT_EQUAL_INT(3, tl.count);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, tl.entries[0].start_time);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, tl.entries[1].start_time);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 3.0f, tl.entries[2].start_time);
}

/* 3. Parallel: all start times = 0 */
void test_timeline_add_parallel(void)
{
    timeline_t tl = timeline_create(TL_PARALLEL);
    tl = timeline_add(tl, anim_create(0, 1, 1.0f, EASE_TYPE_LINEAR));
    tl = timeline_add(tl, anim_create(0, 1, 2.0f, EASE_TYPE_LINEAR));
    tl = timeline_add(tl, anim_create(0, 1, 0.5f, EASE_TYPE_LINEAR));

    TEST_ASSERT_EQUAL_INT(3, tl.count);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, tl.entries[0].start_time);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, tl.entries[1].start_time);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, tl.entries[2].start_time);
}

/* 4. Staggered: start times = 0, delay, 2*delay */
void test_timeline_add_staggered(void)
{
    timeline_t tl = timeline_create_staggered(0.3f);
    tl = timeline_add(tl, anim_create(0, 1, 1.0f, EASE_TYPE_LINEAR));
    tl = timeline_add(tl, anim_create(0, 1, 1.0f, EASE_TYPE_LINEAR));
    tl = timeline_add(tl, anim_create(0, 1, 1.0f, EASE_TYPE_LINEAR));

    TEST_ASSERT_EQUAL_INT(3, tl.count);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, tl.entries[0].start_time);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.3f, tl.entries[1].start_time);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.6f, tl.entries[2].start_time);
}

/* 5. Start: started flag set */
void test_timeline_start(void)
{
    timeline_t tl = timeline_create(TL_SEQUENTIAL);
    tl = timeline_add(tl, anim_create(0, 1, 1.0f, EASE_TYPE_LINEAR));
    TEST_ASSERT_EQUAL_INT(0, tl.started);
    tl = timeline_start(tl);
    TEST_ASSERT_EQUAL_INT(1, tl.started);
}

/* 6. Sequential order: second starts only after first ends */
void test_timeline_sequential_order(void)
{
    timeline_t tl = timeline_create(TL_SEQUENTIAL);
    tl = timeline_add(tl, anim_create(0, 100, 1.0f, EASE_TYPE_LINEAR));
    tl = timeline_add(tl, anim_create(0, 200, 1.0f, EASE_TYPE_LINEAR));
    tl = timeline_start(tl);

    /* Tick 0.5s: first running, second not started */
    tl = timeline_tick(tl, 0.5f);
    TEST_ASSERT_EQUAL_INT(1, tl.entries[0].started);
    TEST_ASSERT_EQUAL_INT(0, tl.entries[1].started);

    /* Tick another 0.6s (total 1.1s): first done, second started */
    tl = timeline_tick(tl, 0.6f);
    TEST_ASSERT_EQUAL_INT(1, tl.entries[0].done);
    TEST_ASSERT_EQUAL_INT(1, tl.entries[1].started);
    TEST_ASSERT_EQUAL_INT(0, tl.entries[1].done);
}

/* 7. Sequential values: correct values during each phase */
void test_timeline_sequential_values(void)
{
    timeline_t tl = timeline_create(TL_SEQUENTIAL);
    tl = timeline_add(tl, anim_create(0, 100, 1.0f, EASE_TYPE_LINEAR));
    tl = timeline_add(tl, anim_create(0, 200, 1.0f, EASE_TYPE_LINEAR));
    tl = timeline_start(tl);

    /* At t=0.5: first anim at 50% -> value ~50 */
    tl = timeline_tick(tl, 0.5f);
    TEST_ASSERT_FLOAT_WITHIN(5.0f, 50.0f, timeline_entry_value(tl, 0));
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, timeline_entry_value(tl, 1));

    /* At t=1.5: first done (100), second at 50% -> ~100 */
    tl = timeline_tick(tl, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 100.0f, timeline_entry_value(tl, 0));
    TEST_ASSERT_FLOAT_WITHIN(20.0f, 100.0f, timeline_entry_value(tl, 1));
}

/* 8. Parallel: all entries run at same time */
void test_timeline_parallel_simultaneous(void)
{
    timeline_t tl = timeline_create(TL_PARALLEL);
    tl = timeline_add(tl, anim_create(0, 100, 1.0f, EASE_TYPE_LINEAR));
    tl = timeline_add(tl, anim_create(0, 200, 2.0f, EASE_TYPE_LINEAR));
    tl = timeline_start(tl);

    tl = timeline_tick(tl, 0.5f);
    /* Both should be started and running */
    TEST_ASSERT_EQUAL_INT(1, tl.entries[0].started);
    TEST_ASSERT_EQUAL_INT(1, tl.entries[1].started);

    /* First at 50% of 100 = ~50 */
    TEST_ASSERT_FLOAT_WITHIN(5.0f, 50.0f, timeline_entry_value(tl, 0));
    /* Second at 25% of 200 = ~50 */
    TEST_ASSERT_FLOAT_WITHIN(5.0f, 50.0f, timeline_entry_value(tl, 1));
}

/* 9. Staggered: entries start at correct intervals */
void test_timeline_staggered_delay(void)
{
    timeline_t tl = timeline_create_staggered(0.5f);
    tl = timeline_add(tl, anim_create(0, 100, 1.0f, EASE_TYPE_LINEAR));
    tl = timeline_add(tl, anim_create(0, 100, 1.0f, EASE_TYPE_LINEAR));
    tl = timeline_start(tl);

    /* At t=0.3: first started, second not yet */
    tl = timeline_tick(tl, 0.3f);
    TEST_ASSERT_EQUAL_INT(1, tl.entries[0].started);
    TEST_ASSERT_EQUAL_INT(0, tl.entries[1].started);

    /* At t=0.7: both started. Second got 0.2s of time */
    tl = timeline_tick(tl, 0.4f);
    TEST_ASSERT_EQUAL_INT(1, tl.entries[0].started);
    TEST_ASSERT_EQUAL_INT(1, tl.entries[1].started);
    /* First at 70% -> ~70 */
    TEST_ASSERT_FLOAT_WITHIN(5.0f, 70.0f, timeline_entry_value(tl, 0));
    /* Second started at t=0.5, now at t=0.7 so 0.2s in -> ~20 */
    TEST_ASSERT_FLOAT_WITHIN(5.0f, 20.0f, timeline_entry_value(tl, 1));
}

/* 10. Done when all entries finished */
void test_timeline_done_all_complete(void)
{
    timeline_t tl = timeline_create(TL_SEQUENTIAL);
    tl = timeline_add(tl, anim_create(0, 1, 0.5f, EASE_TYPE_LINEAR));
    tl = timeline_add(tl, anim_create(0, 1, 0.5f, EASE_TYPE_LINEAR));
    tl = timeline_start(tl);

    tl = timeline_tick(tl, 1.5f);
    TEST_ASSERT_EQUAL_INT(1, timeline_done(tl));
}

/* 11. Not done while entries still running */
void test_timeline_not_done_mid(void)
{
    timeline_t tl = timeline_create(TL_SEQUENTIAL);
    tl = timeline_add(tl, anim_create(0, 1, 1.0f, EASE_TYPE_LINEAR));
    tl = timeline_add(tl, anim_create(0, 1, 1.0f, EASE_TYPE_LINEAR));
    tl = timeline_start(tl);

    tl = timeline_tick(tl, 0.5f);
    TEST_ASSERT_EQUAL_INT(0, timeline_done(tl));

    tl = timeline_tick(tl, 0.8f);
    TEST_ASSERT_EQUAL_INT(0, timeline_done(tl));
}

/* 12. Per-entry interpolated value */
void test_timeline_entry_value(void)
{
    timeline_t tl = timeline_create(TL_PARALLEL);
    tl = timeline_add(tl, anim_create(10, 20, 1.0f, EASE_TYPE_LINEAR));
    tl = timeline_start(tl);

    /* Before tick: value should be from value */
    TEST_ASSERT_FLOAT_WITHIN(TOL, 10.0f, timeline_entry_value(tl, 0));

    tl = timeline_tick(tl, 0.5f);
    /* At 50%: value should be ~15 */
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 15.0f, timeline_entry_value(tl, 0));

    /* Out of range returns 0 */
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, timeline_entry_value(tl, 5));
}

/* 13. Per-entry done check */
void test_timeline_entry_done(void)
{
    timeline_t tl = timeline_create(TL_SEQUENTIAL);
    tl = timeline_add(tl, anim_create(0, 1, 0.5f, EASE_TYPE_LINEAR));
    tl = timeline_add(tl, anim_create(0, 1, 1.0f, EASE_TYPE_LINEAR));
    tl = timeline_start(tl);

    tl = timeline_tick(tl, 0.3f);
    TEST_ASSERT_EQUAL_INT(0, timeline_entry_done(tl, 0));
    TEST_ASSERT_EQUAL_INT(0, timeline_entry_done(tl, 1));

    tl = timeline_tick(tl, 0.4f);
    TEST_ASSERT_EQUAL_INT(1, timeline_entry_done(tl, 0));
    TEST_ASSERT_EQUAL_INT(0, timeline_entry_done(tl, 1));

    /* Out of range returns 0 */
    TEST_ASSERT_EQUAL_INT(0, timeline_entry_done(tl, -1));
    TEST_ASSERT_EQUAL_INT(0, timeline_entry_done(tl, 10));
}

/* 14. Progress 0-1 correct at midpoint */
void test_timeline_progress(void)
{
    timeline_t tl = timeline_create(TL_SEQUENTIAL);
    tl = timeline_add(tl, anim_create(0, 1, 1.0f, EASE_TYPE_LINEAR));
    tl = timeline_add(tl, anim_create(0, 1, 1.0f, EASE_TYPE_LINEAR));
    tl = timeline_start(tl);

    /* Duration = 2.0s, tick 1.0s = 50% */
    tl = timeline_tick(tl, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.5f, timeline_progress(tl));

    /* Tick past end: clamped to 1.0 */
    tl = timeline_tick(tl, 2.0f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, timeline_progress(tl));
}

/* 15. Empty timeline is immediately done */
void test_timeline_empty_done(void)
{
    timeline_t tl = timeline_create(TL_SEQUENTIAL);
    TEST_ASSERT_EQUAL_INT(1, timeline_done(tl));
}

/* 16. Sequential duration = sum of all durations */
void test_timeline_duration_sequential(void)
{
    timeline_t tl = timeline_create(TL_SEQUENTIAL);
    tl = timeline_add(tl, anim_create(0, 1, 1.0f, EASE_TYPE_LINEAR));
    tl = timeline_add(tl, anim_create(0, 1, 2.0f, EASE_TYPE_LINEAR));
    tl = timeline_add(tl, anim_create(0, 1, 0.5f, EASE_TYPE_LINEAR));

    TEST_ASSERT_FLOAT_WITHIN(TOL, 3.5f, timeline_duration(tl));
}

/* 17. Parallel duration = max of all durations */
void test_timeline_duration_parallel(void)
{
    timeline_t tl = timeline_create(TL_PARALLEL);
    tl = timeline_add(tl, anim_create(0, 1, 1.0f, EASE_TYPE_LINEAR));
    tl = timeline_add(tl, anim_create(0, 1, 3.0f, EASE_TYPE_LINEAR));
    tl = timeline_add(tl, anim_create(0, 1, 2.0f, EASE_TYPE_LINEAR));

    TEST_ASSERT_FLOAT_WITHIN(TOL, 3.0f, timeline_duration(tl));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_timeline_create_empty);
    RUN_TEST(test_timeline_add_sequential);
    RUN_TEST(test_timeline_add_parallel);
    RUN_TEST(test_timeline_add_staggered);
    RUN_TEST(test_timeline_start);
    RUN_TEST(test_timeline_sequential_order);
    RUN_TEST(test_timeline_sequential_values);
    RUN_TEST(test_timeline_parallel_simultaneous);
    RUN_TEST(test_timeline_staggered_delay);
    RUN_TEST(test_timeline_done_all_complete);
    RUN_TEST(test_timeline_not_done_mid);
    RUN_TEST(test_timeline_entry_value);
    RUN_TEST(test_timeline_entry_done);
    RUN_TEST(test_timeline_progress);
    RUN_TEST(test_timeline_empty_done);
    RUN_TEST(test_timeline_duration_sequential);
    RUN_TEST(test_timeline_duration_parallel);
    return UNITY_END();
}
