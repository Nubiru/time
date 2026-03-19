/* audio_queue.h -- Audio event queue (ring buffer).
 *
 * Fixed-size ring buffer for scheduling audio events.
 * The engine reads events each frame and processes them
 * via WebAudio timeline (ramps, one-shots, crossfades).
 *
 * Event types:
 *   NOTE_ON  — trigger a cultural timbre or planet note
 *   NOTE_OFF — release a note
 *   RAMP     — smoothly change a parameter (volume, pan, etc.)
 *   CROSSFADE — transition between two audio states
 *
 * No malloc. Caller provides the queue struct (stack or global).
 * Queue operations modify the struct via pointer. */

#ifndef TIME_AUDIO_QUEUE_H
#define TIME_AUDIO_QUEUE_H

#define AQ_MAX_EVENTS 64

/* Audio event types */
typedef enum {
    AQ_EVENT_NONE = 0,
    AQ_EVENT_NOTE_ON,      /* trigger a note */
    AQ_EVENT_NOTE_OFF,     /* release a note */
    AQ_EVENT_RAMP,         /* smooth parameter change */
    AQ_EVENT_CROSSFADE,    /* blend between two states */
    AQ_EVENT_COUNT
} aq_event_type_t;

/* Audio event target parameter */
typedef enum {
    AQ_TARGET_VOLUME = 0,
    AQ_TARGET_PAN,
    AQ_TARGET_FREQUENCY,
    AQ_TARGET_REVERB,
    AQ_TARGET_CONSONANCE,
    AQ_TARGET_COUNT
} aq_target_t;

/* A single audio event */
typedef struct {
    aq_event_type_t type;
    int channel;           /* 0-8 (planet/voice index) */
    aq_target_t target;    /* which parameter to modify */
    float value;           /* target value (or note frequency for NOTE_ON) */
    float duration_sec;    /* ramp/crossfade duration (0 = immediate) */
    double schedule_time;  /* when to execute (real-time seconds) */
    int priority;          /* 0=low, 1=normal, 2=high */
} aq_event_t;

/* Ring buffer queue */
typedef struct {
    aq_event_t events[AQ_MAX_EVENTS];
    int head;              /* read position */
    int tail;              /* write position */
    int count;             /* number of events in queue */
    int dropped;           /* events dropped due to overflow */
} aq_queue_t;

/* Initialize a queue (zero all fields). */
void aq_init(aq_queue_t *q);

/* Push an event onto the queue.
 * Returns 1 on success, 0 if queue is full (event dropped). */
int aq_push(aq_queue_t *q, const aq_event_t *event);

/* Pop the oldest event from the queue.
 * Returns 1 if an event was popped (written to *out), 0 if empty. */
int aq_pop(aq_queue_t *q, aq_event_t *out);

/* Peek at the oldest event without removing it.
 * Returns pointer to the event, or NULL if empty.
 * Pointer is valid until next push/pop. */
const aq_event_t *aq_peek(const aq_queue_t *q);

/* Number of events currently in the queue. */
int aq_count(const aq_queue_t *q);

/* Check if queue is empty. */
int aq_empty(const aq_queue_t *q);

/* Check if queue is full. */
int aq_full(const aq_queue_t *q);

/* Number of events dropped due to overflow. */
int aq_dropped(const aq_queue_t *q);

/* Clear all events from the queue. */
void aq_clear(aq_queue_t *q);

/* Create a NOTE_ON event. */
aq_event_t aq_note_on(int channel, float freq_hz, float velocity,
                       double schedule_time);

/* Create a NOTE_OFF event. */
aq_event_t aq_note_off(int channel, double schedule_time);

/* Create a RAMP event (smooth parameter change). */
aq_event_t aq_ramp(int channel, aq_target_t target, float value,
                    float duration_sec, double schedule_time);

/* Create a CROSSFADE event. */
aq_event_t aq_crossfade(float duration_sec, double schedule_time);

/* Get event type name string. */
const char *aq_event_type_name(aq_event_type_t type);

/* Get target parameter name string. */
const char *aq_target_name(aq_target_t target);

#endif /* TIME_AUDIO_QUEUE_H */
