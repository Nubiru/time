#include "timeline.h"

timeline_t timeline_create(timeline_mode_e mode)
{
    timeline_t tl = {0};
    tl.mode = mode;
    return tl;
}

timeline_t timeline_create_staggered(float delay_seconds)
{
    timeline_t tl = timeline_create(TL_STAGGERED);
    tl.stagger_delay = delay_seconds;
    return tl;
}

timeline_t timeline_add(timeline_t tl, animation_t anim)
{
    if (tl.count >= TIMELINE_MAX_ENTRIES) return tl;

    timeline_entry_t entry = {0};
    entry.anim = anim;
    entry.started = 0;
    entry.done = 0;

    switch (tl.mode) {
    case TL_SEQUENTIAL: {
        /* Sum of all previous (start_time + duration) */
        float offset = 0.0f;
        for (int i = 0; i < tl.count; i++) {
            offset = tl.entries[i].start_time + tl.entries[i].anim.duration;
        }
        entry.start_time = offset;
        break;
    }
    case TL_PARALLEL:
        entry.start_time = 0.0f;
        break;
    case TL_STAGGERED:
        entry.start_time = (float)tl.count * tl.stagger_delay;
        break;
    }

    tl.entries[tl.count] = entry;
    tl.count++;
    return tl;
}

timeline_t timeline_start(timeline_t tl)
{
    tl.started = 1;
    tl.elapsed = 0.0f;
    return tl;
}

timeline_t timeline_tick(timeline_t tl, float dt)
{
    if (!tl.started) return tl;

    float prev_elapsed = tl.elapsed;
    tl.elapsed += dt;

    for (int i = 0; i < tl.count; i++) {
        timeline_entry_t *e = &tl.entries[i];
        if (e->done) continue;

        if (tl.elapsed >= e->start_time) {
            if (!e->started) {
                e->anim = anim_start(e->anim);
                e->started = 1;
                /* Only tick the portion of dt after start_time */
                float effective_dt;
                if (prev_elapsed < e->start_time) {
                    effective_dt = tl.elapsed - e->start_time;
                } else {
                    effective_dt = dt;
                }
                e->anim = anim_tick(e->anim, effective_dt);
            } else {
                e->anim = anim_tick(e->anim, dt);
            }

            if (anim_is_complete(e->anim)) {
                e->done = 1;
            }
        }
    }

    return tl;
}

int timeline_done(timeline_t tl)
{
    if (tl.count == 0) return 1;
    for (int i = 0; i < tl.count; i++) {
        if (!tl.entries[i].done) return 0;
    }
    return 1;
}

float timeline_entry_value(timeline_t tl, int index)
{
    if (index < 0 || index >= tl.count) return 0.0f;
    return animation_value(tl.entries[index].anim);
}

int timeline_entry_done(timeline_t tl, int index)
{
    if (index < 0 || index >= tl.count) return 0;
    return tl.entries[index].done;
}

float timeline_duration(timeline_t tl)
{
    if (tl.count == 0) return 0.0f;

    switch (tl.mode) {
    case TL_SEQUENTIAL: {
        /* Last entry's start_time + its duration */
        timeline_entry_t *last = &tl.entries[tl.count - 1];
        return last->start_time + last->anim.duration;
    }
    case TL_PARALLEL: {
        float max_dur = 0.0f;
        for (int i = 0; i < tl.count; i++) {
            if (tl.entries[i].anim.duration > max_dur)
                max_dur = tl.entries[i].anim.duration;
        }
        return max_dur;
    }
    case TL_STAGGERED: {
        timeline_entry_t *last = &tl.entries[tl.count - 1];
        return last->start_time + last->anim.duration;
    }
    }
    return 0.0f;
}

float timeline_progress(timeline_t tl)
{
    float dur = timeline_duration(tl);
    if (dur <= 0.0f) return 1.0f;
    float p = tl.elapsed / dur;
    if (p < 0.0f) return 0.0f;
    if (p > 1.0f) return 1.0f;
    return p;
}
