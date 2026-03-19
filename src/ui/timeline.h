#ifndef TIME_TIMELINE_H
#define TIME_TIMELINE_H

/* Animation timeline: orchestrate multiple animations with timing.
 * Supports sequential, parallel, and staggered execution modes.
 * Pure: no GL, no globals, no I/O. */

#include "animation.h"

#define TIMELINE_MAX_ENTRIES 16

typedef enum {
    TL_SEQUENTIAL, /* each starts after previous ends */
    TL_PARALLEL,   /* all start simultaneously */
    TL_STAGGERED   /* each starts delay after previous */
} timeline_mode_e;

typedef struct {
    animation_t anim;
    float start_time; /* when this entry begins (seconds from timeline start) */
    int started;      /* 1 if anim_start has been called */
    int done;         /* 1 when animation complete */
} timeline_entry_t;

typedef struct {
    timeline_entry_t entries[TIMELINE_MAX_ENTRIES];
    int count;
    float elapsed;       /* total time since timeline started */
    float stagger_delay; /* seconds between entries (for TL_STAGGERED) */
    timeline_mode_e mode;
    int started; /* 1 if timeline has been started */
} timeline_t;

/* Create empty timeline */
timeline_t timeline_create(timeline_mode_e mode);

/* Create staggered timeline with specified delay */
timeline_t timeline_create_staggered(float delay_seconds);

/* Add animation entry. Computes start_time based on mode. */
timeline_t timeline_add(timeline_t tl, animation_t anim);

/* Start the timeline (begins ticking) */
timeline_t timeline_start(timeline_t tl);

/* Advance by dt seconds */
timeline_t timeline_tick(timeline_t tl, float dt);

/* Is the entire timeline complete? */
int timeline_done(timeline_t tl);

/* Get current interpolated value of entry at index */
float timeline_entry_value(timeline_t tl, int index);

/* Is entry at index complete? */
int timeline_entry_done(timeline_t tl, int index);

/* Overall progress 0.0 to 1.0 */
float timeline_progress(timeline_t tl);

/* Total duration of the timeline */
float timeline_duration(timeline_t tl);

#endif
