/* audio_queue.c -- Audio event queue (ring buffer).
 *
 * Fixed-size ring buffer for scheduling audio events.
 * No malloc — all storage is inline in the aq_queue_t struct.
 * Caller owns the queue (stack-allocated or in app_state).
 *
 * FIFO order: events are processed in insertion order.
 * When full, new events are dropped (not overwritten). */

#include "audio_queue.h"

#include <string.h>

/* --- Queue operations --- */

void aq_init(aq_queue_t *q)
{
    if (q == NULL) return;
    memset(q, 0, sizeof(*q));
}

int aq_push(aq_queue_t *q, const aq_event_t *event)
{
    if (q == NULL || event == NULL) return 0;

    if (q->count >= AQ_MAX_EVENTS) {
        q->dropped++;
        return 0;
    }

    q->events[q->tail] = *event;
    q->tail = (q->tail + 1) % AQ_MAX_EVENTS;
    q->count++;
    return 1;
}

int aq_pop(aq_queue_t *q, aq_event_t *out)
{
    if (q == NULL || q->count <= 0) return 0;

    if (out != NULL) {
        *out = q->events[q->head];
    }
    q->head = (q->head + 1) % AQ_MAX_EVENTS;
    q->count--;
    return 1;
}

const aq_event_t *aq_peek(const aq_queue_t *q)
{
    if (q == NULL || q->count <= 0) return NULL;
    return &q->events[q->head];
}

int aq_count(const aq_queue_t *q)
{
    if (q == NULL) return 0;
    return q->count;
}

int aq_empty(const aq_queue_t *q)
{
    return aq_count(q) == 0;
}

int aq_full(const aq_queue_t *q)
{
    if (q == NULL) return 0;
    return q->count >= AQ_MAX_EVENTS;
}

int aq_dropped(const aq_queue_t *q)
{
    if (q == NULL) return 0;
    return q->dropped;
}

void aq_clear(aq_queue_t *q)
{
    if (q == NULL) return;
    q->head = 0;
    q->tail = 0;
    q->count = 0;
    /* Keep dropped count for diagnostics */
}

/* --- Event factories --- */

aq_event_t aq_note_on(int channel, float freq_hz, float velocity,
                       double schedule_time)
{
    aq_event_t e;
    memset(&e, 0, sizeof(e));
    e.type = AQ_EVENT_NOTE_ON;
    e.channel = channel;
    e.target = AQ_TARGET_FREQUENCY;
    e.value = freq_hz;
    e.duration_sec = velocity; /* velocity stored in duration for note_on */
    e.schedule_time = schedule_time;
    e.priority = 1;
    return e;
}

aq_event_t aq_note_off(int channel, double schedule_time)
{
    aq_event_t e;
    memset(&e, 0, sizeof(e));
    e.type = AQ_EVENT_NOTE_OFF;
    e.channel = channel;
    e.schedule_time = schedule_time;
    e.priority = 1;
    return e;
}

aq_event_t aq_ramp(int channel, aq_target_t target, float value,
                    float duration_sec, double schedule_time)
{
    aq_event_t e;
    memset(&e, 0, sizeof(e));
    e.type = AQ_EVENT_RAMP;
    e.channel = channel;
    e.target = target;
    e.value = value;
    e.duration_sec = duration_sec;
    e.schedule_time = schedule_time;
    e.priority = 1;
    return e;
}

aq_event_t aq_crossfade(float duration_sec, double schedule_time)
{
    aq_event_t e;
    memset(&e, 0, sizeof(e));
    e.type = AQ_EVENT_CROSSFADE;
    e.duration_sec = duration_sec;
    e.schedule_time = schedule_time;
    e.priority = 2; /* high priority */
    return e;
}

/* --- Name strings --- */

static const char *const EVENT_TYPE_NAMES[AQ_EVENT_COUNT] = {
    "none", "note_on", "note_off", "ramp", "crossfade"
};

static const char *const TARGET_NAMES[AQ_TARGET_COUNT] = {
    "volume", "pan", "frequency", "reverb", "consonance"
};

const char *aq_event_type_name(aq_event_type_t type)
{
    if (type < 0 || type >= AQ_EVENT_COUNT) return "unknown";
    return EVENT_TYPE_NAMES[type];
}

const char *aq_target_name(aq_target_t target)
{
    if (target < 0 || target >= AQ_TARGET_COUNT) return "unknown";
    return TARGET_NAMES[target];
}
