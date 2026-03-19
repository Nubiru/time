/* test_audio_queue.c -- Tests for audio event queue (ring buffer). */

#include "unity.h"
#include "ui/audio_queue.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* --- aq_init tests --- */

void test_aq_init(void)
{
    aq_queue_t q;
    aq_init(&q);
    TEST_ASSERT_EQUAL_INT(0, aq_count(&q));
    TEST_ASSERT_TRUE(aq_empty(&q));
    TEST_ASSERT_FALSE(aq_full(&q));
    TEST_ASSERT_EQUAL_INT(0, aq_dropped(&q));
}

/* --- aq_push / aq_pop tests --- */

void test_aq_push_pop_single(void)
{
    aq_queue_t q;
    aq_init(&q);

    aq_event_t evt = aq_note_on(0, 440.0f, 0.8f, 1.0);
    TEST_ASSERT_EQUAL_INT(1, aq_push(&q, &evt));
    TEST_ASSERT_EQUAL_INT(1, aq_count(&q));

    aq_event_t out;
    TEST_ASSERT_EQUAL_INT(1, aq_pop(&q, &out));
    TEST_ASSERT_EQUAL_INT(AQ_EVENT_NOTE_ON, out.type);
    TEST_ASSERT_EQUAL_INT(0, out.channel);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 440.0f, out.value);
    TEST_ASSERT_EQUAL_INT(0, aq_count(&q));
}

void test_aq_push_pop_fifo(void)
{
    aq_queue_t q;
    aq_init(&q);

    aq_event_t e1 = aq_note_on(0, 440.0f, 0.5f, 1.0);
    aq_event_t e2 = aq_note_on(1, 880.0f, 0.3f, 2.0);
    aq_push(&q, &e1);
    aq_push(&q, &e2);

    aq_event_t out;
    aq_pop(&q, &out);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 440.0f, out.value); /* first in */
    aq_pop(&q, &out);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 880.0f, out.value); /* second in */
}

void test_aq_pop_empty(void)
{
    aq_queue_t q;
    aq_init(&q);

    aq_event_t out;
    TEST_ASSERT_EQUAL_INT(0, aq_pop(&q, &out));
}

/* --- aq_peek tests --- */

void test_aq_peek(void)
{
    aq_queue_t q;
    aq_init(&q);

    aq_event_t evt = aq_note_on(2, 330.0f, 0.6f, 0.5);
    aq_push(&q, &evt);

    const aq_event_t *p = aq_peek(&q);
    TEST_ASSERT_NOT_NULL(p);
    TEST_ASSERT_EQUAL_INT(AQ_EVENT_NOTE_ON, p->type);
    TEST_ASSERT_EQUAL_INT(2, p->channel);
    TEST_ASSERT_EQUAL_INT(1, aq_count(&q)); /* not removed */
}

void test_aq_peek_empty(void)
{
    aq_queue_t q;
    aq_init(&q);
    TEST_ASSERT_NULL(aq_peek(&q));
}

/* --- overflow tests --- */

void test_aq_overflow(void)
{
    aq_queue_t q;
    aq_init(&q);

    aq_event_t evt = aq_note_on(0, 440.0f, 0.5f, 0.0);
    for (int i = 0; i < AQ_MAX_EVENTS; i++) {
        TEST_ASSERT_EQUAL_INT(1, aq_push(&q, &evt));
    }
    TEST_ASSERT_TRUE(aq_full(&q));
    TEST_ASSERT_EQUAL_INT(0, aq_push(&q, &evt)); /* overflow */
    TEST_ASSERT_EQUAL_INT(1, aq_dropped(&q));
}

/* --- aq_clear tests --- */

void test_aq_clear(void)
{
    aq_queue_t q;
    aq_init(&q);

    aq_event_t evt = aq_note_on(0, 440.0f, 0.5f, 0.0);
    aq_push(&q, &evt);
    aq_push(&q, &evt);
    TEST_ASSERT_EQUAL_INT(2, aq_count(&q));

    aq_clear(&q);
    TEST_ASSERT_EQUAL_INT(0, aq_count(&q));
    TEST_ASSERT_TRUE(aq_empty(&q));
}

/* --- wrap-around test --- */

void test_aq_wraparound(void)
{
    aq_queue_t q;
    aq_init(&q);

    aq_event_t evt = aq_note_on(0, 440.0f, 0.5f, 0.0);
    aq_event_t out;

    /* Fill and drain multiple times to force wrap-around */
    for (int cycle = 0; cycle < 3; cycle++) {
        for (int i = 0; i < AQ_MAX_EVENTS / 2; i++) {
            evt.channel = i;
            aq_push(&q, &evt);
        }
        for (int i = 0; i < AQ_MAX_EVENTS / 2; i++) {
            TEST_ASSERT_EQUAL_INT(1, aq_pop(&q, &out));
            TEST_ASSERT_EQUAL_INT(i, out.channel);
        }
    }
    TEST_ASSERT_TRUE(aq_empty(&q));
    TEST_ASSERT_EQUAL_INT(0, aq_dropped(&q));
}

/* --- event factory tests --- */

void test_aq_note_on_factory(void)
{
    aq_event_t e = aq_note_on(3, 440.0f, 0.7f, 1.5);
    TEST_ASSERT_EQUAL_INT(AQ_EVENT_NOTE_ON, e.type);
    TEST_ASSERT_EQUAL_INT(3, e.channel);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 440.0f, e.value);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 1.5, e.schedule_time);
}

void test_aq_note_off_factory(void)
{
    aq_event_t e = aq_note_off(5, 2.0);
    TEST_ASSERT_EQUAL_INT(AQ_EVENT_NOTE_OFF, e.type);
    TEST_ASSERT_EQUAL_INT(5, e.channel);
}

void test_aq_ramp_factory(void)
{
    aq_event_t e = aq_ramp(1, AQ_TARGET_VOLUME, 0.5f, 0.3f, 0.0);
    TEST_ASSERT_EQUAL_INT(AQ_EVENT_RAMP, e.type);
    TEST_ASSERT_EQUAL_INT(1, e.channel);
    TEST_ASSERT_EQUAL_INT(AQ_TARGET_VOLUME, e.target);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.5f, e.value);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.3f, e.duration_sec);
}

void test_aq_crossfade_factory(void)
{
    aq_event_t e = aq_crossfade(2.0f, 5.0);
    TEST_ASSERT_EQUAL_INT(AQ_EVENT_CROSSFADE, e.type);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 2.0f, e.duration_sec);
}

/* --- name tests --- */

void test_aq_event_type_names(void)
{
    TEST_ASSERT_EQUAL_STRING("none", aq_event_type_name(AQ_EVENT_NONE));
    TEST_ASSERT_EQUAL_STRING("note_on", aq_event_type_name(AQ_EVENT_NOTE_ON));
    TEST_ASSERT_EQUAL_STRING("note_off", aq_event_type_name(AQ_EVENT_NOTE_OFF));
    TEST_ASSERT_EQUAL_STRING("ramp", aq_event_type_name(AQ_EVENT_RAMP));
    TEST_ASSERT_EQUAL_STRING("crossfade", aq_event_type_name(AQ_EVENT_CROSSFADE));
}

void test_aq_target_names(void)
{
    TEST_ASSERT_EQUAL_STRING("volume", aq_target_name(AQ_TARGET_VOLUME));
    TEST_ASSERT_EQUAL_STRING("pan", aq_target_name(AQ_TARGET_PAN));
    TEST_ASSERT_EQUAL_STRING("frequency", aq_target_name(AQ_TARGET_FREQUENCY));
}

int main(void)
{
    UNITY_BEGIN();

    /* init */
    RUN_TEST(test_aq_init);

    /* push/pop */
    RUN_TEST(test_aq_push_pop_single);
    RUN_TEST(test_aq_push_pop_fifo);
    RUN_TEST(test_aq_pop_empty);

    /* peek */
    RUN_TEST(test_aq_peek);
    RUN_TEST(test_aq_peek_empty);

    /* overflow */
    RUN_TEST(test_aq_overflow);

    /* clear */
    RUN_TEST(test_aq_clear);

    /* wrap-around */
    RUN_TEST(test_aq_wraparound);

    /* factories */
    RUN_TEST(test_aq_note_on_factory);
    RUN_TEST(test_aq_note_off_factory);
    RUN_TEST(test_aq_ramp_factory);
    RUN_TEST(test_aq_crossfade_factory);

    /* names */
    RUN_TEST(test_aq_event_type_names);
    RUN_TEST(test_aq_target_names);

    return UNITY_END();
}
