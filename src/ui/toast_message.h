/* toast_message.h -- Transient notification message data layer.
 *
 * Manages a FIFO queue of short-lived "toast" messages for user feedback.
 * Examples: "Speed: 3600x", "Jumped to Now", "Stars: ON", "Earth View".
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 * Functional style: take queue by value, return new queue by value. */

#ifndef TIME_TOAST_MESSAGE_H
#define TIME_TOAST_MESSAGE_H

#define TOAST_MAX_TEXT   128
#define TOAST_MAX_QUEUE  8
#define TOAST_FADE_WINDOW 0.5f  /* seconds before expiry to begin fading */

/* Toast urgency level */
typedef enum {
    TOAST_INFO = 0,      /* Normal feedback: "Speed: 3600x" */
    TOAST_SUCCESS,       /* Positive: "Jumped to Now" */
    TOAST_WARNING,       /* Attention: "Reverse mode" */
    TOAST_COUNT
} toast_level_t;

/* A single toast message */
typedef struct {
    char text[TOAST_MAX_TEXT];    /* Message content */
    toast_level_t level;         /* Urgency level */
    float duration;              /* Total display time in seconds */
    float elapsed;               /* Time since shown */
    float opacity;               /* Current opacity (1.0 -> 0.0 as it fades) */
    int active;                  /* 1 = currently showing, 0 = expired */
} toast_t;

/* Toast queue -- manages multiple toasts with FIFO ordering */
typedef struct {
    toast_t items[TOAST_MAX_QUEUE];
    int count;                   /* Number of active toasts */
} toast_queue_t;

/* --- Queue Management --- */

/* Create empty toast queue. */
toast_queue_t toast_queue_init(void);

/* Add a toast to the queue. If queue is full, oldest is removed.
 * text: message to display.
 * level: urgency.
 * duration: display time in seconds (use 0 for default). */
toast_queue_t toast_push(toast_queue_t queue, const char *text,
                          toast_level_t level, float duration);

/* Advance all toasts by dt seconds.
 * Updates elapsed, computes opacity (fade out in last 0.5s), removes expired. */
toast_queue_t toast_update(toast_queue_t queue, float dt);

/* Remove all toasts. */
toast_queue_t toast_clear(toast_queue_t queue);

/* --- Query --- */

/* Get the newest (most recent) active toast. Returns toast with active=0 if empty. */
toast_t toast_newest(toast_queue_t queue);

/* Get number of active toasts. */
int toast_active_count(toast_queue_t queue);

/* Is the queue empty? */
int toast_is_empty(toast_queue_t queue);

/* --- Convenience Constructors --- */

/* Create a speed change toast: "Speed: 3600x" */
toast_queue_t toast_speed(toast_queue_t queue, double speed);

/* Create a toggle toast: "Stars: ON" / "Stars: OFF" */
toast_queue_t toast_toggle(toast_queue_t queue, const char *name, int enabled);

/* Create a view change toast: "Earth View" / "Space View" */
toast_queue_t toast_view(toast_queue_t queue, const char *view_name);

/* Create a time jump toast: "Jumped to Jan 1, 2000" */
toast_queue_t toast_jump(toast_queue_t queue, const char *date_str);

/* --- Display --- */

/* Get the level name. Returns static string. */
const char *toast_level_name(toast_level_t level);

/* Get the default duration for a level (seconds). */
float toast_default_duration(toast_level_t level);

#endif /* TIME_TOAST_MESSAGE_H */
