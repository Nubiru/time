/* toast_message.c -- Transient notification message data layer.
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "toast_message.h"
#include <stdio.h>
#include <string.h>

/* --- Default durations by level (seconds) --- */
static const float DEFAULT_DURATIONS[TOAST_COUNT] = {
    2.0f,   /* TOAST_INFO */
    2.5f,   /* TOAST_SUCCESS */
    3.0f    /* TOAST_WARNING */
};

/* --- Level names --- */
static const char *LEVEL_NAMES[TOAST_COUNT] = {
    "Info",
    "Success",
    "Warning"
};

/* --- Internal: compute opacity for a toast --- */
static float compute_opacity(float elapsed, float duration)
{
    float remaining = duration - elapsed;
    if (remaining <= 0.0f) return 0.0f;
    if (remaining >= TOAST_FADE_WINDOW) return 1.0f;
    return remaining / TOAST_FADE_WINDOW;
}

/* --- Internal: shift items left by one, dropping oldest --- */
static void shift_left(toast_queue_t *q)
{
    int i;
    for (i = 1; i < q->count; i++) {
        q->items[i - 1] = q->items[i];
    }
    if (q->count > 0) {
        q->count--;
    }
}

/* --- Queue Management --- */

toast_queue_t toast_queue_init(void)
{
    toast_queue_t q;
    memset(&q, 0, sizeof(q));
    return q;
}

toast_queue_t toast_push(toast_queue_t queue, const char *text,
                          toast_level_t level, float duration)
{
    /* If full, drop oldest */
    if (queue.count >= TOAST_MAX_QUEUE) {
        shift_left(&queue);
    }

    /* Use default duration if 0 or negative */
    if (duration <= 0.0f) {
        duration = toast_default_duration(level);
    }

    /* Build new toast */
    toast_t t;
    memset(&t, 0, sizeof(t));
    snprintf(t.text, TOAST_MAX_TEXT, "%s", text ? text : "");
    t.level = level;
    t.duration = duration;
    t.elapsed = 0.0f;
    t.opacity = 1.0f;
    t.active = 1;

    queue.items[queue.count] = t;
    queue.count++;

    return queue;
}

toast_queue_t toast_update(toast_queue_t queue, float dt)
{
    int i = 0;
    while (i < queue.count) {
        queue.items[i].elapsed += dt;
        queue.items[i].opacity = compute_opacity(
            queue.items[i].elapsed, queue.items[i].duration);

        if (queue.items[i].elapsed >= queue.items[i].duration) {
            /* Expired: remove by shifting remaining items left */
            queue.items[i].active = 0;
            int j;
            for (j = i + 1; j < queue.count; j++) {
                queue.items[j - 1] = queue.items[j];
            }
            queue.count--;
            /* Don't increment i — shifted item is now at i */
        } else {
            i++;
        }
    }

    return queue;
}

toast_queue_t toast_clear(toast_queue_t queue)
{
    queue.count = 0;
    return queue;
}

/* --- Query --- */

toast_t toast_newest(toast_queue_t queue)
{
    if (queue.count > 0) {
        return queue.items[queue.count - 1];
    }
    /* Return inactive toast */
    toast_t empty;
    memset(&empty, 0, sizeof(empty));
    return empty;
}

int toast_active_count(toast_queue_t queue)
{
    return queue.count;
}

int toast_is_empty(toast_queue_t queue)
{
    return queue.count == 0;
}

/* --- Convenience Constructors --- */

toast_queue_t toast_speed(toast_queue_t queue, double speed)
{
    char buf[TOAST_MAX_TEXT];

    if (speed == 0.0) {
        snprintf(buf, TOAST_MAX_TEXT, "Speed: PAUSED");
    } else if (speed == 1.0) {
        snprintf(buf, TOAST_MAX_TEXT, "Speed: Real-time");
    } else {
        snprintf(buf, TOAST_MAX_TEXT, "Speed: %.0fx", speed);
    }

    return toast_push(queue, buf, TOAST_INFO, 0.0f);
}

toast_queue_t toast_toggle(toast_queue_t queue, const char *name, int enabled)
{
    char buf[TOAST_MAX_TEXT];
    snprintf(buf, TOAST_MAX_TEXT, "%s: %s",
             name ? name : "Toggle",
             enabled ? "ON" : "OFF");
    return toast_push(queue, buf, TOAST_INFO, 0.0f);
}

toast_queue_t toast_view(toast_queue_t queue, const char *view_name)
{
    char buf[TOAST_MAX_TEXT];
    snprintf(buf, TOAST_MAX_TEXT, "%s", view_name ? view_name : "View");
    return toast_push(queue, buf, TOAST_SUCCESS, 0.0f);
}

toast_queue_t toast_jump(toast_queue_t queue, const char *date_str)
{
    char buf[TOAST_MAX_TEXT];
    snprintf(buf, TOAST_MAX_TEXT, "Jumped to %s",
             date_str ? date_str : "unknown");
    return toast_push(queue, buf, TOAST_SUCCESS, 0.0f);
}

/* --- Display --- */

const char *toast_level_name(toast_level_t level)
{
    if (level >= 0 && level < TOAST_COUNT) {
        return LEVEL_NAMES[level];
    }
    return "Unknown";
}

float toast_default_duration(toast_level_t level)
{
    if (level >= 0 && level < TOAST_COUNT) {
        return DEFAULT_DURATIONS[level];
    }
    return 2.0f;  /* fallback */
}
