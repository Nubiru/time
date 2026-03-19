/* text_choreo.h — Text Animation Choreography
 * Defines animation presets for each text category (wisdom, toast, card,
 * narrative, HUD). Each preset specifies enter/hold/exit timing using
 * phi-derived durations. Uses text_reveal for enter animations and
 * simple opacity fade for exit.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_TEXT_CHOREO_H
#define TIME_TEXT_CHOREO_H

#include "text_reveal.h"

/* Text categories — each gets a distinct animation choreography */
typedef enum {
    TC_WISDOM = 0,    /* Typewriter in, long hold, gentle fade out */
    TC_TOAST,         /* Instant appear, medium hold, fade out */
    TC_CARD,          /* Word-by-word fade in, no auto-exit */
    TC_NARRATIVE,     /* Word-by-word staggered, long hold, fade out */
    TC_HUD,           /* Instant, persistent (no exit) */
    TC_CATEGORY_COUNT
} tc_category_t;

/* Animation phase */
typedef enum {
    TC_PHASE_ENTER = 0,
    TC_PHASE_HOLD,
    TC_PHASE_EXIT,
    TC_PHASE_DONE
} tc_phase_t;

/* Choreography preset — phi-derived timing for a text category */
typedef struct {
    text_reveal_e enter_style;   /* reveal type for enter phase */
    ease_type_e enter_easing;    /* easing curve for enter */
    float enter_duration;        /* seconds for enter phase */
    float hold_duration;         /* seconds to stay fully visible (0=persistent) */
    float exit_duration;         /* seconds for fade out (0=no exit) */
    ease_type_e exit_easing;     /* easing curve for exit fade */
} tc_preset_t;

/* Animation sequence state */
typedef struct {
    tc_category_t category;
    tc_phase_t phase;
    text_reveal_t reveal;    /* enter animation state */
    float elapsed;           /* total time elapsed */
    float phase_time;        /* time in current phase */
    int element_count;       /* number of chars/words */
    int done;
} tc_seq_t;

/* Get the choreography preset for a text category.
 * Invalid category returns TC_HUD preset (safe default). */
tc_preset_t tc_preset(tc_category_t category);

/* Create an animation sequence for a text category.
 * count = number of characters (for typewriter) or words (for fade_word). */
tc_seq_t tc_create(tc_category_t category, int count);

/* Advance the sequence by dt seconds. Handles phase transitions. */
tc_seq_t tc_tick(tc_seq_t seq, float dt);

/* Overall opacity (1.0 during hold, fading during enter/exit, 0.0 when done).
 * For ENTER phase: returns reveal progress.
 * For HOLD phase: returns 1.0.
 * For EXIT phase: returns fade-out value (1→0). */
float tc_opacity(tc_seq_t seq);

/* Per-character/word visibility during enter phase.
 * Delegates to text_reveal during ENTER. Returns 1.0 during HOLD.
 * Returns tc_opacity() during EXIT (uniform fade). */
float tc_char_visibility(tc_seq_t seq, int index);

/* Category display name. */
const char *tc_category_name(tc_category_t category);

#endif /* TIME_TEXT_CHOREO_H */
