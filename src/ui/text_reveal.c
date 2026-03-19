#include "text_reveal.h"

/* --- Helpers --- */

static float clampf(float v, float lo, float hi)
{
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

/* --- Create --- */

text_reveal_t text_reveal_create(text_reveal_e style, int count, float duration)
{
    text_reveal_t ta;
    ta.style = style;
    ta.count = count;
    ta.duration = duration;
    ta.element_fade = (count > 0) ? duration / (float)count : 0.0f;
    ta.easing = EASE_TYPE_OUT_CUBIC;
    ta.elapsed = 0.0f;
    ta.started = 0;
    ta.done = 0;
    return ta;
}

/* --- Configuration --- */

text_reveal_t text_reveal_set_element_fade(text_reveal_t ta, float fade_time)
{
    ta.element_fade = fade_time;
    return ta;
}

text_reveal_t text_reveal_set_easing(text_reveal_t ta, ease_type_e easing)
{
    ta.easing = easing;
    return ta;
}

/* --- State --- */

text_reveal_t text_reveal_start(text_reveal_t ta)
{
    ta.started = 1;
    ta.elapsed = 0.0f;
    ta.done = 0;
    return ta;
}

text_reveal_t text_reveal_tick(text_reveal_t ta, float dt)
{
    if (!ta.started || ta.done) return ta;

    ta.elapsed += dt;

    /* Check done condition */
    if (ta.style == TEXT_REVEAL_INSTANT) {
        ta.done = 1;
    } else if (ta.duration <= 0.0f) {
        ta.done = 1;
    } else if (ta.elapsed >= ta.duration) {
        ta.done = 1;
    }

    return ta;
}

/* --- Visibility --- */

static float text_reveal_visibility_typewriter(text_reveal_t ta, int index)
{
    if (ta.duration <= 0.0f) return 1.0f;
    float interval = ta.duration / (float)ta.count;
    float start_time = (float)index * interval;
    return (ta.elapsed >= start_time) ? 1.0f : 0.0f;
}

static float text_reveal_visibility_fade(text_reveal_t ta, int index)
{
    float spread = ta.duration - ta.element_fade;
    if (spread < 0.0f) spread = 0.0f;

    float start_time;
    if (ta.count <= 1) {
        start_time = 0.0f;
    } else {
        start_time = (float)index * spread / (float)(ta.count - 1);
    }

    if (ta.element_fade <= 0.0f) {
        return (ta.elapsed >= start_time) ? 1.0f : 0.0f;
    }

    float local_t = (ta.elapsed - start_time) / ta.element_fade;
    float clamped = clampf(local_t, 0.0f, 1.0f);
    return anim_apply_easing(ta.easing, clamped);
}

float text_reveal_visibility(text_reveal_t ta, int index)
{
    if (!ta.started) return 0.0f;
    if (index < 0 || index >= ta.count) return 0.0f;

    switch (ta.style) {
    case TEXT_REVEAL_INSTANT:
        return 1.0f;
    case TEXT_REVEAL_TYPEWRITER:
        return text_reveal_visibility_typewriter(ta, index);
    case TEXT_REVEAL_FADE_CHAR:
    case TEXT_REVEAL_FADE_WORD:
        return text_reveal_visibility_fade(ta, index);
    }
    return 0.0f;
}

/* --- Queries --- */

int text_reveal_visible_count(text_reveal_t ta)
{
    int n = 0;
    for (int i = 0; i < ta.count; i++) {
        if (text_reveal_visibility(ta, i) >= 0.99f) {
            n++;
        }
    }
    return n;
}

int text_reveal_done(text_reveal_t ta)
{
    return ta.done;
}

float text_reveal_progress(text_reveal_t ta)
{
    if (!ta.started) return 0.0f;
    if (ta.duration <= 0.0f) return 1.0f;
    return clampf(ta.elapsed / ta.duration, 0.0f, 1.0f);
}
