#include "kf_track.h"

/* --- Helpers --- */

static float clampf(float v, float lo, float hi)
{
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

/* Find the segment index: the largest i where keys[i].time <= elapsed.
 * Returns -1 if elapsed < keys[0].time (before first key). */
static int find_segment(kf_track_t tr)
{
    if (tr.count == 0) return -1;

    /* Before first key */
    if (tr.elapsed < tr.keys[0].time) return -1;

    /* Walk forward to find segment */
    for (int i = tr.count - 1; i >= 0; i--) {
        if (tr.elapsed >= tr.keys[i].time) {
            return i;
        }
    }
    return -1;
}

/* --- Create --- */

kf_track_t kf_track_create(void)
{
    kf_track_t tr;
    tr.count = 0;
    tr.elapsed = 0.0f;
    tr.started = 0;
    tr.done = 0;
    return tr;
}

/* --- Add --- */

kf_track_t kf_track_add(kf_track_t tr, float time, float value, ease_type_e easing)
{
    if (tr.count >= KF_MAX_KEYS) return tr;

    tr.keys[tr.count].time = time;
    tr.keys[tr.count].value = value;
    tr.keys[tr.count].easing = easing;
    tr.count++;
    return tr;
}

/* --- Start --- */

kf_track_t kf_track_start(kf_track_t tr)
{
    tr.started = 1;
    tr.elapsed = 0.0f;
    tr.done = 0;
    return tr;
}

/* --- Tick --- */

kf_track_t kf_track_tick(kf_track_t tr, float dt)
{
    if (!tr.started || tr.done) return tr;

    tr.elapsed += dt;

    /* Check done */
    float dur = kf_track_duration(tr);
    if (tr.elapsed >= dur) {
        tr.done = 1;
    }

    return tr;
}

/* --- Value --- */

float kf_track_value(kf_track_t tr)
{
    if (tr.count == 0) return 0.0f;

    /* Not started: return first key value */
    if (!tr.started) return tr.keys[0].value;

    /* Before first key */
    if (tr.elapsed < tr.keys[0].time) return tr.keys[0].value;

    /* At or past last key */
    if (tr.elapsed >= tr.keys[tr.count - 1].time) {
        return tr.keys[tr.count - 1].value;
    }

    /* Find the segment */
    int i = find_segment(tr);
    if (i < 0) return tr.keys[0].value;
    if (i >= tr.count - 1) return tr.keys[tr.count - 1].value;

    /* Interpolate between keys[i] and keys[i+1] */
    float seg_start = tr.keys[i].time;
    float seg_end = tr.keys[i + 1].time;
    float seg_len = seg_end - seg_start;

    /* Zero-length segment: snap to next key */
    if (seg_len <= 0.0f) return tr.keys[i + 1].value;

    float local_t = (tr.elapsed - seg_start) / seg_len;
    local_t = clampf(local_t, 0.0f, 1.0f);

    /* Apply easing from the NEXT key (it controls how we ease INTO it) */
    float eased_t = anim_apply_easing(tr.keys[i + 1].easing, local_t);

    float from = tr.keys[i].value;
    float to = tr.keys[i + 1].value;
    return from + (to - from) * eased_t;
}

/* --- Duration --- */

float kf_track_duration(kf_track_t tr)
{
    if (tr.count == 0) return 0.0f;
    return tr.keys[tr.count - 1].time;
}

/* --- Done --- */

int kf_track_done(kf_track_t tr)
{
    if (tr.count == 0) return 1;
    if (!tr.started) return 0;
    return tr.done;
}

/* --- Progress --- */

float kf_track_progress(kf_track_t tr)
{
    if (!tr.started) return 0.0f;
    float dur = kf_track_duration(tr);
    if (dur <= 0.0f) return 1.0f;
    return clampf(tr.elapsed / dur, 0.0f, 1.0f);
}

/* --- Segment --- */

int kf_track_segment(kf_track_t tr)
{
    if (!tr.started || tr.count == 0) return -1;

    int i = find_segment(tr);
    if (i < 0) return 0; /* before first key but started: segment 0 */

    /* Clamp to last valid segment index */
    if (i >= tr.count - 1) return tr.count - 2;

    return i;
}
