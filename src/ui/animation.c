#include "animation.h"
#include "../math/easing.h"

/* --- Create --- */

animation_t anim_create(float from, float to, float duration, ease_type_e easing)
{
    animation_t a;
    a.from = from;
    a.to = to;
    a.current = from;
    a.duration = duration;
    a.elapsed = 0.0f;
    a.progress = 0.0f;
    a.eased = 0.0f;
    a.state = ANIM_IDLE;
    a.easing = easing;
    a.delay = 0.0f;
    a.loop = 0;
    a.iteration = 0;
    return a;
}

animation_t anim_create_delayed(float from, float to, float duration,
                                 ease_type_e easing, float delay)
{
    animation_t a = anim_create(from, to, duration, easing);
    a.delay = delay;
    return a;
}

/* --- State transitions --- */

animation_t anim_start(animation_t a)
{
    if (a.state == ANIM_IDLE || a.state == ANIM_PAUSED)
        a.state = ANIM_RUNNING;
    return a;
}

animation_t anim_pause(animation_t a)
{
    if (a.state == ANIM_RUNNING)
        a.state = ANIM_PAUSED;
    return a;
}

animation_t anim_reset(animation_t a)
{
    a.state = ANIM_IDLE;
    a.elapsed = 0.0f;
    a.progress = 0.0f;
    a.eased = 0.0f;
    a.current = a.from;
    a.iteration = 0;
    return a;
}

animation_t anim_restart(animation_t a)
{
    a = anim_reset(a);
    a.state = ANIM_RUNNING;
    return a;
}

/* --- Easing dispatch --- */

float anim_apply_easing(ease_type_e type, float t)
{
    double d = (double)t;
    switch (type) {
    case EASE_TYPE_LINEAR:      return (float)ease_linear(d);
    case EASE_TYPE_IN_QUAD:     return (float)ease_in_quad(d);
    case EASE_TYPE_OUT_QUAD:    return (float)ease_out_quad(d);
    case EASE_TYPE_INOUT_QUAD:  return (float)ease_in_out_quad(d);
    case EASE_TYPE_IN_CUBIC:    return (float)ease_in_cubic(d);
    case EASE_TYPE_OUT_CUBIC:   return (float)ease_out_cubic(d);
    case EASE_TYPE_INOUT_CUBIC: return (float)ease_in_out_cubic(d);
    case EASE_TYPE_IN_SINE:     return (float)ease_in_sine(d);
    case EASE_TYPE_OUT_SINE:    return (float)ease_out_sine(d);
    case EASE_TYPE_INOUT_SINE:  return (float)ease_in_out_sine(d);
    case EASE_TYPE_IN_EXPO:     return (float)ease_in_expo(d);
    case EASE_TYPE_OUT_EXPO:    return (float)ease_out_expo(d);
    case EASE_TYPE_COUNT:       break;
    }
    return (float)ease_linear(d);
}

/* --- Tick --- */

animation_t anim_tick(animation_t a, float dt)
{
    if (a.state != ANIM_RUNNING) return a;

    /* Handle delay */
    if (a.elapsed < a.delay) {
        a.elapsed += dt;
        if (a.elapsed < a.delay) {
            a.current = a.from;
            return a;
        }
        dt = a.elapsed - a.delay;
        a.elapsed = a.delay;
    }

    a.elapsed += dt;
    float raw_t = (a.duration > 0.0f)
        ? (a.elapsed - a.delay) / a.duration
        : 1.0f;

    if (raw_t >= 1.0f) {
        raw_t = 1.0f;
        if (a.loop == 0) {
            a.state = ANIM_COMPLETE;
        } else if (a.loop == 1) {
            a.elapsed = a.delay;
            a.iteration++;
            raw_t = 0.0f;
        } else if (a.loop == 2) {
            float tmp = a.from;
            a.from = a.to;
            a.to = tmp;
            a.elapsed = a.delay;
            a.iteration++;
            raw_t = 0.0f;
        }
    }

    a.progress = raw_t;
    a.eased = anim_apply_easing(a.easing, raw_t);
    a.current = a.from + (a.to - a.from) * a.eased;
    return a;
}

/* --- Query --- */

int anim_is_running(animation_t a)
{
    return a.state == ANIM_RUNNING;
}

int anim_is_complete(animation_t a)
{
    return a.state == ANIM_COMPLETE;
}

float animation_value(animation_t a)
{
    return a.current;
}

/* --- Loop --- */

animation_t anim_set_loop(animation_t a, int loop_mode)
{
    a.loop = loop_mode;
    return a;
}

/* --- Sequence --- */

anim_sequence_t anim_seq_create(void)
{
    anim_sequence_t seq;
    seq.count = 0;
    seq.current_index = 0;
    return seq;
}

anim_sequence_t anim_seq_add(anim_sequence_t seq, animation_t a)
{
    if (seq.count < 16)
        seq.anims[seq.count++] = a;
    return seq;
}

anim_sequence_t anim_seq_tick(anim_sequence_t seq, float dt)
{
    if (seq.count == 0 || seq.current_index >= seq.count)
        return seq;

    /* Auto-start if idle */
    if (seq.anims[seq.current_index].state == ANIM_IDLE)
        seq.anims[seq.current_index] = anim_start(seq.anims[seq.current_index]);

    seq.anims[seq.current_index] = anim_tick(seq.anims[seq.current_index], dt);

    /* Advance to next if complete */
    if (seq.anims[seq.current_index].state == ANIM_COMPLETE)
        seq.current_index++;

    return seq;
}

int anim_seq_is_complete(anim_sequence_t seq)
{
    return seq.count == 0 || seq.current_index >= seq.count;
}

float anim_seq_value(anim_sequence_t seq)
{
    if (seq.count == 0) return 0.0f;
    int idx = seq.current_index;
    if (idx >= seq.count) idx = seq.count - 1;
    return seq.anims[idx].current;
}
