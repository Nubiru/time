#include "visual_transition.h"
#include "golden_layout.h"

/* Phi-based timing constants.
 * Card slides use "normal" timing (~0.236s at 1.0 base).
 * Layer fades use "medium" (~0.382s).
 * Toast slides use "fast" (~0.146s). */
static const float BASE_TIMING = 1.0f; /* 1 second base */

/* Slide distance: cards enter from this far off-screen */
static const float CARD_SLIDE_DISTANCE = -0.15f; /* 15% screen width, from left */
static const float TOAST_SLIDE_DISTANCE = -0.05f; /* 5% screen height, from top */

/* Fibonacci stagger base delay (seconds) */
static const float STAGGER_BASE = 0.05f;

static float phi_pow(float base, int power)
{
    float result = base;
    for (int i = 0; i < power; i++) result *= (float)PHI_INV;
    return result;
}

visual_transitions_t vtrans_init(void)
{
    visual_transitions_t vt;

    /* Phi-timed durations */
    vt.card_dur  = phi_pow(BASE_TIMING, 3);  /* ~0.236s: snappy */
    vt.layer_dur = phi_pow(BASE_TIMING, 2);  /* ~0.382s: medium */
    vt.toast_dur = phi_pow(BASE_TIMING, 4);  /* ~0.146s: fast */

    /* Cards: hidden, idle */
    for (int i = 0; i < CARD_TYPE_COUNT; i++) {
        vt.cards[i].opacity = anim_create(0.0f, 0.0f, vt.card_dur, EASE_TYPE_OUT_CUBIC);
        vt.cards[i].slide_x = anim_create(0.0f, 0.0f, vt.card_dur, EASE_TYPE_OUT_CUBIC);
    }

    /* Layers: fully opaque, idle */
    for (int i = 0; i < LAYER_COUNT; i++) {
        vt.layers[i].opacity = anim_create(1.0f, 1.0f, vt.layer_dur, EASE_TYPE_INOUT_QUAD);
        vt.layers[i].target = 1.0f;
    }

    /* Toasts: hidden, idle */
    for (int i = 0; i < TOAST_MAX_QUEUE; i++) {
        vt.toasts[i].opacity = anim_create(0.0f, 0.0f, vt.toast_dur, EASE_TYPE_OUT_QUAD);
        vt.toasts[i].slide_y = anim_create(0.0f, 0.0f, vt.toast_dur, EASE_TYPE_OUT_QUAD);
    }

    return vt;
}

/* --- Card transitions --- */

visual_transitions_t vtrans_card_show(visual_transitions_t vt, int card)
{
    if (card < 0 || card >= CARD_TYPE_COUNT) return vt;
    vt.cards[card].opacity = anim_create(0.0f, 1.0f, vt.card_dur, EASE_TYPE_OUT_CUBIC);
    vt.cards[card].opacity = anim_start(vt.cards[card].opacity);
    vt.cards[card].slide_x = anim_create(CARD_SLIDE_DISTANCE, 0.0f, vt.card_dur,
                                          EASE_TYPE_OUT_CUBIC);
    vt.cards[card].slide_x = anim_start(vt.cards[card].slide_x);
    return vt;
}

visual_transitions_t vtrans_card_hide(visual_transitions_t vt, int card)
{
    if (card < 0 || card >= CARD_TYPE_COUNT) return vt;
    float current_op = animation_value(vt.cards[card].opacity);
    vt.cards[card].opacity = anim_create(current_op, 0.0f, vt.card_dur, EASE_TYPE_IN_CUBIC);
    vt.cards[card].opacity = anim_start(vt.cards[card].opacity);
    vt.cards[card].slide_x = anim_create(0.0f, CARD_SLIDE_DISTANCE, vt.card_dur,
                                          EASE_TYPE_IN_CUBIC);
    vt.cards[card].slide_x = anim_start(vt.cards[card].slide_x);
    return vt;
}

/* --- Layer transitions --- */

visual_transitions_t vtrans_layer_fade_in(visual_transitions_t vt, int layer)
{
    return vtrans_layer_fade_to(vt, layer, 1.0f);
}

visual_transitions_t vtrans_layer_fade_out(visual_transitions_t vt, int layer)
{
    return vtrans_layer_fade_to(vt, layer, 0.0f);
}

visual_transitions_t vtrans_layer_fade_to(visual_transitions_t vt, int layer,
                                           float target_opacity)
{
    if (layer < 0 || layer >= LAYER_COUNT) return vt;
    float current = animation_value(vt.layers[layer].opacity);
    vt.layers[layer].target = target_opacity;
    vt.layers[layer].opacity = anim_create(current, target_opacity, vt.layer_dur,
                                            EASE_TYPE_INOUT_QUAD);
    vt.layers[layer].opacity = anim_start(vt.layers[layer].opacity);
    return vt;
}

/* --- Toast transitions --- */

visual_transitions_t vtrans_toast_show(visual_transitions_t vt, int slot)
{
    if (slot < 0 || slot >= TOAST_MAX_QUEUE) return vt;
    vt.toasts[slot].opacity = anim_create(0.0f, 1.0f, vt.toast_dur, EASE_TYPE_OUT_QUAD);
    vt.toasts[slot].opacity = anim_start(vt.toasts[slot].opacity);
    vt.toasts[slot].slide_y = anim_create(TOAST_SLIDE_DISTANCE, 0.0f, vt.toast_dur,
                                           EASE_TYPE_OUT_QUAD);
    vt.toasts[slot].slide_y = anim_start(vt.toasts[slot].slide_y);
    return vt;
}

visual_transitions_t vtrans_toast_dismiss(visual_transitions_t vt, int slot)
{
    if (slot < 0 || slot >= TOAST_MAX_QUEUE) return vt;
    float current_op = animation_value(vt.toasts[slot].opacity);
    vt.toasts[slot].opacity = anim_create(current_op, 0.0f, vt.toast_dur,
                                           EASE_TYPE_IN_QUAD);
    vt.toasts[slot].opacity = anim_start(vt.toasts[slot].opacity);
    /* No slide on dismiss — just fade */
    vt.toasts[slot].slide_y = anim_create(0.0f, 0.0f, vt.toast_dur, EASE_TYPE_LINEAR);
    return vt;
}

/* --- Stagger --- */

visual_transitions_t vtrans_stagger_cards(visual_transitions_t vt,
                                           const card_type_t *order, int count)
{
    /* Fibonacci delays: 0, 1, 1, 2, 3, 5... * STAGGER_BASE */
    int fib_a = 0, fib_b = 1;
    for (int i = 0; i < count && i < CARD_TYPE_COUNT; i++) {
        int card = order[i];
        if (card < 0 || card >= CARD_TYPE_COUNT) continue;

        float delay = (float)fib_a * STAGGER_BASE;

        vt.cards[card].opacity = anim_create_delayed(0.0f, 1.0f, vt.card_dur,
                                                      EASE_TYPE_OUT_CUBIC, delay);
        vt.cards[card].opacity = anim_start(vt.cards[card].opacity);
        vt.cards[card].slide_x = anim_create_delayed(CARD_SLIDE_DISTANCE, 0.0f,
                                                      vt.card_dur, EASE_TYPE_OUT_CUBIC,
                                                      delay);
        vt.cards[card].slide_x = anim_start(vt.cards[card].slide_x);

        int next = fib_a + fib_b;
        fib_a = fib_b;
        fib_b = next;
    }
    return vt;
}

/* --- Tick --- */

visual_transitions_t vtrans_tick(visual_transitions_t vt, float dt)
{
    if (dt <= 0.0f) return vt;

    for (int i = 0; i < CARD_TYPE_COUNT; i++) {
        vt.cards[i].opacity = anim_tick(vt.cards[i].opacity, dt);
        vt.cards[i].slide_x = anim_tick(vt.cards[i].slide_x, dt);
    }
    for (int i = 0; i < LAYER_COUNT; i++) {
        vt.layers[i].opacity = anim_tick(vt.layers[i].opacity, dt);
    }
    for (int i = 0; i < TOAST_MAX_QUEUE; i++) {
        vt.toasts[i].opacity = anim_tick(vt.toasts[i].opacity, dt);
        vt.toasts[i].slide_y = anim_tick(vt.toasts[i].slide_y, dt);
    }
    return vt;
}

/* --- Query --- */

float vtrans_card_opacity(visual_transitions_t vt, int card)
{
    if (card < 0 || card >= CARD_TYPE_COUNT) return 0.0f;
    return animation_value(vt.cards[card].opacity);
}

float vtrans_card_slide_x(visual_transitions_t vt, int card)
{
    if (card < 0 || card >= CARD_TYPE_COUNT) return 0.0f;
    return animation_value(vt.cards[card].slide_x);
}

int vtrans_card_is_animating(visual_transitions_t vt, int card)
{
    if (card < 0 || card >= CARD_TYPE_COUNT) return 0;
    return anim_is_running(vt.cards[card].opacity) ||
           anim_is_running(vt.cards[card].slide_x);
}

float vtrans_layer_opacity(visual_transitions_t vt, int layer)
{
    if (layer < 0 || layer >= LAYER_COUNT) return 0.0f;
    return animation_value(vt.layers[layer].opacity);
}

int vtrans_layer_is_animating(visual_transitions_t vt, int layer)
{
    if (layer < 0 || layer >= LAYER_COUNT) return 0;
    return anim_is_running(vt.layers[layer].opacity);
}

float vtrans_toast_opacity(visual_transitions_t vt, int slot)
{
    if (slot < 0 || slot >= TOAST_MAX_QUEUE) return 0.0f;
    return animation_value(vt.toasts[slot].opacity);
}

float vtrans_toast_slide_y(visual_transitions_t vt, int slot)
{
    if (slot < 0 || slot >= TOAST_MAX_QUEUE) return 0.0f;
    return animation_value(vt.toasts[slot].slide_y);
}

float vtrans_card_duration(visual_transitions_t vt)
{
    return vt.card_dur;
}

float vtrans_layer_duration(visual_transitions_t vt)
{
    return vt.layer_dur;
}
