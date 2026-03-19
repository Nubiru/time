#ifndef TIME_DRAMA_SEQ_H
#define TIME_DRAMA_SEQ_H

#include "kf_track.h"

#define DRAMA_MAX_TRACKS  8
#define DRAMA_MAX_MARKERS 16

/* Channel names (indices into track array) */
typedef enum {
    DRAMA_CH_CAMERA_AZ = 0,   /* camera azimuth (radians) */
    DRAMA_CH_CAMERA_EL,        /* camera elevation (radians) */
    DRAMA_CH_CAMERA_DIST,      /* camera distance/zoom */
    DRAMA_CH_OVERLAY,          /* text overlay opacity 0-1 */
    DRAMA_CH_DIM,              /* background dim level 0-1 */
    DRAMA_CH_CUSTOM_0,         /* user-defined */
    DRAMA_CH_CUSTOM_1,         /* user-defined */
    DRAMA_CH_CUSTOM_2,         /* user-defined */
    DRAMA_CH_COUNT
} drama_channel_e;

/* Timed event marker */
typedef struct {
    float time;       /* when this marker triggers (seconds) */
    int id;           /* caller-defined event ID */
} drama_marker_t;

typedef struct {
    kf_track_t tracks[DRAMA_MAX_TRACKS];
    int track_active[DRAMA_MAX_TRACKS];  /* 1 if track has keyframes */
    drama_marker_t markers[DRAMA_MAX_MARKERS];
    int marker_count;
    int markers_fired[DRAMA_MAX_MARKERS]; /* 1 if marker has triggered */
    float elapsed;
    float prev_elapsed;  /* elapsed BEFORE last tick (for marker detection) */
    int started;
    int done;
    int paused;
    int last_marker_id;  /* id of last marker fired this tick, -1 if none */
} drama_seq_t;

/* Create empty sequence */
drama_seq_t drama_seq_create(void);

/* Add a keyframe to a specific channel.
 * Creates the track if it doesn't exist yet. */
drama_seq_t drama_seq_add_key(drama_seq_t seq, drama_channel_e ch,
                               float time, float value, ease_type_e easing);

/* Add a timed event marker */
drama_seq_t drama_seq_add_marker(drama_seq_t seq, float time, int id);

/* Start the sequence */
drama_seq_t drama_seq_start(drama_seq_t seq);

/* Pause/resume */
drama_seq_t drama_seq_pause(drama_seq_t seq);
drama_seq_t drama_seq_resume(drama_seq_t seq);

/* Advance by dt seconds */
drama_seq_t drama_seq_tick(drama_seq_t seq, float dt);

/* Get current interpolated value of a channel */
float drama_seq_value(drama_seq_t seq, drama_channel_e ch);

/* Check if a specific marker fired THIS tick (was crossed between prev and current elapsed).
 * Returns 1 if marker with given id fired, 0 otherwise.
 * Call after tick to check for events. */
int drama_seq_marker_fired(drama_seq_t seq, int marker_id);

/* Get the most recently fired marker id. Returns -1 if none fired this tick. */
int drama_seq_last_marker(drama_seq_t seq);

/* Is the sequence complete? (all tracks done AND past last marker) */
int drama_seq_done(drama_seq_t seq);

/* Overall progress 0.0 to 1.0 (based on longest track or latest marker) */
float drama_seq_progress(drama_seq_t seq);

/* Total duration (max of all track durations and marker times) */
float drama_seq_duration(drama_seq_t seq);

/* Is a specific channel active (has keyframes)? */
int drama_seq_channel_active(drama_seq_t seq, drama_channel_e ch);

#endif
