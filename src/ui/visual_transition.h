#ifndef TIME_VISUAL_TRANSITION_H
#define TIME_VISUAL_TRANSITION_H

#include "animation.h"
#include "card_layout.h"
#include "toast_message.h"
#include "../render/render_layers.h"

/* Animated transition state for a single card */
typedef struct {
    animation_t opacity;   /* 0→1 show, 1→0 hide */
    animation_t slide_x;   /* off-screen → 0 (final position) */
} card_transition_t;

/* Animated transition state for a single render layer */
typedef struct {
    animation_t opacity;   /* current→target fade */
    float target;          /* final opacity value */
} layer_transition_t;

/* Animated transition state for a toast notification */
typedef struct {
    animation_t opacity;   /* 0→1 show, 1→0 dismiss */
    animation_t slide_y;   /* off-screen → 0 (slide in from top) */
} toast_transition_t;

/* Full transition state for all animated UI elements */
typedef struct {
    card_transition_t cards[CARD_TYPE_COUNT];
    layer_transition_t layers[LAYER_COUNT];
    toast_transition_t toasts[TOAST_MAX_QUEUE];
    float card_dur;   /* phi-timed card animation duration (seconds) */
    float layer_dur;  /* phi-timed layer fade duration (seconds) */
    float toast_dur;  /* phi-timed toast animation duration (seconds) */
} visual_transitions_t;

/* Initialize: all cards hidden, all layers at full opacity. */
visual_transitions_t vtrans_init(void);

/* --- Card transitions --- */
visual_transitions_t vtrans_card_show(visual_transitions_t vt, int card);
visual_transitions_t vtrans_card_hide(visual_transitions_t vt, int card);

/* --- Layer transitions --- */
visual_transitions_t vtrans_layer_fade_in(visual_transitions_t vt, int layer);
visual_transitions_t vtrans_layer_fade_out(visual_transitions_t vt, int layer);
visual_transitions_t vtrans_layer_fade_to(visual_transitions_t vt, int layer,
                                           float target_opacity);

/* --- Toast transitions --- */
visual_transitions_t vtrans_toast_show(visual_transitions_t vt, int slot);
visual_transitions_t vtrans_toast_dismiss(visual_transitions_t vt, int slot);

/* --- Stagger: show multiple cards with fibonacci-timed delays --- */
visual_transitions_t vtrans_stagger_cards(visual_transitions_t vt,
                                           const card_type_t *order, int count);

/* --- Advance all animations by dt seconds --- */
visual_transitions_t vtrans_tick(visual_transitions_t vt, float dt);

/* --- Query current values --- */
float vtrans_card_opacity(visual_transitions_t vt, int card);
float vtrans_card_slide_x(visual_transitions_t vt, int card);
int   vtrans_card_is_animating(visual_transitions_t vt, int card);

float vtrans_layer_opacity(visual_transitions_t vt, int layer);
int   vtrans_layer_is_animating(visual_transitions_t vt, int layer);

float vtrans_toast_opacity(visual_transitions_t vt, int slot);
float vtrans_toast_slide_y(visual_transitions_t vt, int slot);

/* --- Duration query --- */
float vtrans_card_duration(visual_transitions_t vt);
float vtrans_layer_duration(visual_transitions_t vt);

#endif
