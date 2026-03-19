#ifndef TIME_TEXT_REVEAL_H
#define TIME_TEXT_REVEAL_H

#include "animation.h"

typedef enum {
    TEXT_REVEAL_INSTANT,     /* all visible at once */
    TEXT_REVEAL_TYPEWRITER,  /* char by char, instant appear */
    TEXT_REVEAL_FADE_CHAR,   /* char by char with smooth fade-in */
    TEXT_REVEAL_FADE_WORD    /* word by word with smooth fade-in (same math, semantic alias) */
} text_reveal_e;

typedef struct {
    text_reveal_e style;
    int count;              /* number of elements */
    float duration;         /* total animation seconds */
    float element_fade;     /* per-element fade-in time (seconds) */
    ease_type_e easing;     /* easing for per-element fade */
    float elapsed;
    int started;
    int done;
} text_reveal_t;

/* Create with style, element count, and total duration.
 * Default element_fade = duration/count for fade styles.
 * Default easing = EASE_TYPE_OUT_CUBIC. */
text_reveal_t text_reveal_create(text_reveal_e style, int count, float duration);

/* Configure per-element fade time (only matters for FADE_CHAR/FADE_WORD) */
text_reveal_t text_reveal_set_element_fade(text_reveal_t ta, float fade_time);

/* Configure easing for per-element fade */
text_reveal_t text_reveal_set_easing(text_reveal_t ta, ease_type_e easing);

/* Start the animation */
text_reveal_t text_reveal_start(text_reveal_t ta);

/* Advance by dt seconds */
text_reveal_t text_reveal_tick(text_reveal_t ta, float dt);

/* Per-element visibility: 0.0 (hidden) to 1.0 (fully visible).
 * Returns 0.0 for out-of-bounds index or not-started animation. */
float text_reveal_visibility(text_reveal_t ta, int index);

/* Count of elements with visibility >= 0.99 */
int text_reveal_visible_count(text_reveal_t ta);

/* Is the entire animation complete? */
int text_reveal_done(text_reveal_t ta);

/* Overall progress 0.0 to 1.0 */
float text_reveal_progress(text_reveal_t ta);

#endif
