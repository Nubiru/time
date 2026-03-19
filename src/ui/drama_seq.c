#include "drama_seq.h"

/* --- Helpers --- */

static float ds_clampf(float v, float lo, float hi)
{
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

/* --- Create --- */

drama_seq_t drama_seq_create(void)
{
    drama_seq_t seq;
    for (int i = 0; i < DRAMA_MAX_TRACKS; i++) {
        seq.tracks[i] = kf_track_create();
        seq.track_active[i] = 0;
    }
    seq.marker_count = 0;
    for (int i = 0; i < DRAMA_MAX_MARKERS; i++) {
        seq.markers[i].time = 0.0f;
        seq.markers[i].id = 0;
        seq.markers_fired[i] = 0;
    }
    seq.elapsed = 0.0f;
    seq.prev_elapsed = 0.0f;
    seq.started = 0;
    seq.done = 0;
    seq.paused = 0;
    seq.last_marker_id = -1;
    return seq;
}

/* --- Add Key --- */

drama_seq_t drama_seq_add_key(drama_seq_t seq, drama_channel_e ch,
                               float time, float value, ease_type_e easing)
{
    if (ch < 0 || ch >= DRAMA_CH_COUNT) return seq;

    if (!seq.track_active[ch]) {
        seq.tracks[ch] = kf_track_create();
        seq.track_active[ch] = 1;
    }
    seq.tracks[ch] = kf_track_add(seq.tracks[ch], time, value, easing);
    return seq;
}

/* --- Add Marker --- */

drama_seq_t drama_seq_add_marker(drama_seq_t seq, float time, int id)
{
    if (seq.marker_count >= DRAMA_MAX_MARKERS) return seq;

    seq.markers[seq.marker_count].time = time;
    seq.markers[seq.marker_count].id = id;
    seq.markers_fired[seq.marker_count] = 0;
    seq.marker_count++;
    return seq;
}

/* --- Start --- */

drama_seq_t drama_seq_start(drama_seq_t seq)
{
    seq.started = 1;
    seq.elapsed = 0.0f;
    seq.prev_elapsed = 0.0f;
    seq.done = 0;
    seq.paused = 0;
    seq.last_marker_id = -1;

    /* Start all active tracks */
    for (int i = 0; i < DRAMA_MAX_TRACKS; i++) {
        if (seq.track_active[i]) {
            seq.tracks[i] = kf_track_start(seq.tracks[i]);
        }
    }

    /* Reset markers */
    for (int i = 0; i < seq.marker_count; i++) {
        seq.markers_fired[i] = 0;
    }

    return seq;
}

/* --- Pause --- */

drama_seq_t drama_seq_pause(drama_seq_t seq)
{
    seq.paused = 1;
    return seq;
}

/* --- Resume --- */

drama_seq_t drama_seq_resume(drama_seq_t seq)
{
    seq.paused = 0;
    return seq;
}

/* --- Tick --- */

drama_seq_t drama_seq_tick(drama_seq_t seq, float dt)
{
    if (!seq.started || seq.done || seq.paused) return seq;

    float prev = seq.elapsed;
    seq.prev_elapsed = prev;
    seq.elapsed += dt;
    seq.last_marker_id = -1;

    /* Tick all active tracks */
    for (int i = 0; i < DRAMA_MAX_TRACKS; i++) {
        if (seq.track_active[i]) {
            seq.tracks[i] = kf_track_tick(seq.tracks[i], dt);
        }
    }

    /* Check markers: fire if prev < marker.time <= elapsed */
    for (int i = 0; i < seq.marker_count; i++) {
        if (!seq.markers_fired[i] &&
            seq.markers[i].time > prev &&
            seq.markers[i].time <= seq.elapsed) {
            seq.markers_fired[i] = 1;
            seq.last_marker_id = seq.markers[i].id;
        }
    }

    /* Check if all active tracks are done */
    float dur = drama_seq_duration(seq);
    if (seq.elapsed >= dur) {
        seq.done = 1;
    }

    return seq;
}

/* --- Value --- */

float drama_seq_value(drama_seq_t seq, drama_channel_e ch)
{
    if (ch < 0 || ch >= DRAMA_CH_COUNT) return 0.0f;
    if (!seq.track_active[ch]) return 0.0f;
    return kf_track_value(seq.tracks[ch]);
}

/* --- Marker Fired --- */

int drama_seq_marker_fired(drama_seq_t seq, int marker_id)
{
    /* Check if any marker with this id fired THIS tick
     * (was crossed between prev_elapsed and elapsed) */
    for (int i = 0; i < seq.marker_count; i++) {
        if (seq.markers[i].id == marker_id &&
            seq.markers_fired[i] &&
            seq.markers[i].time > seq.prev_elapsed &&
            seq.markers[i].time <= seq.elapsed) {
            return 1;
        }
    }
    return 0;
}

/* --- Last Marker --- */

int drama_seq_last_marker(drama_seq_t seq)
{
    return seq.last_marker_id;
}

/* --- Done --- */

int drama_seq_done(drama_seq_t seq)
{
    /* Empty sequence (no tracks, no markers) is immediately done */
    int has_content = 0;
    for (int i = 0; i < DRAMA_MAX_TRACKS; i++) {
        if (seq.track_active[i]) { has_content = 1; break; }
    }
    if (!has_content && seq.marker_count == 0) return 1;

    if (!seq.started) return 0;
    return seq.done;
}

/* --- Progress --- */

float drama_seq_progress(drama_seq_t seq)
{
    if (!seq.started) return 0.0f;
    float dur = drama_seq_duration(seq);
    if (dur <= 0.0f) return 1.0f;
    return ds_clampf(seq.elapsed / dur, 0.0f, 1.0f);
}

/* --- Duration --- */

float drama_seq_duration(drama_seq_t seq)
{
    float max_dur = 0.0f;

    /* Max of all active track durations */
    for (int i = 0; i < DRAMA_MAX_TRACKS; i++) {
        if (seq.track_active[i]) {
            float td = kf_track_duration(seq.tracks[i]);
            if (td > max_dur) max_dur = td;
        }
    }

    /* Also consider marker times */
    for (int i = 0; i < seq.marker_count; i++) {
        if (seq.markers[i].time > max_dur) {
            max_dur = seq.markers[i].time;
        }
    }

    return max_dur;
}

/* --- Channel Active --- */

int drama_seq_channel_active(drama_seq_t seq, drama_channel_e ch)
{
    if (ch < 0 || ch >= DRAMA_CH_COUNT) return 0;
    return seq.track_active[ch];
}
