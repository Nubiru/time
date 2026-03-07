#include "card_layout.h"

#include <math.h>

/* Card dimensions as fraction of screen */
#define CARD_W_WIDE    0.22f  /* wide screens (aspect > 1.5) */
#define CARD_W_NARROW  0.35f  /* narrow screens */
#define CARD_H         0.28f  /* card height */
#define CARD_MARGIN    0.015f /* gap between cards */
#define CARD_EDGE      0.01f  /* inset from screen edge */

static const char *TYPE_NAMES[CARD_TYPE_COUNT] = {
    "Tzolkin", "I Ching", "Chinese", "Human Design", "Astrology"
};

static const char TOGGLE_KEYS[CARD_TYPE_COUNT] = {
    'K', 'I', 'C', 'D', 'A'
};

card_layout_t card_layout_compute(int visible_mask, float aspect_ratio)
{
    card_layout_t layout;
    float card_w = (aspect_ratio > 1.5f) ? CARD_W_WIDE : CARD_W_NARROW;

    /* Stack visible cards vertically along the right edge */
    int vis_index = 0;
    for (int i = 0; i < CARD_TYPE_COUNT; i++) {
        card_rect_t *c = &layout.cards[i];
        c->type = (card_type_t)i;
        c->w = card_w;
        c->h = CARD_H;

        int is_vis = (visible_mask & (1 << i)) != 0;
        c->visible = is_vis;
        c->opacity = is_vis ? 1.0f : 0.0f;

        if (is_vis) {
            c->x = 1.0f - card_w - CARD_EDGE;
            c->y = CARD_EDGE + (float)vis_index * (CARD_H + CARD_MARGIN);
            vis_index++;
        } else {
            /* Off-screen position for hidden cards */
            c->x = 1.0f + CARD_EDGE;
            c->y = CARD_EDGE + (float)i * (CARD_H + CARD_MARGIN);
        }
    }

    return layout;
}

int card_toggle(int visible_mask, card_type_t type)
{
    if (type < 0 || type >= CARD_TYPE_COUNT) return visible_mask;
    return visible_mask ^ (1 << type);
}

int card_is_visible(int visible_mask, card_type_t type)
{
    if (type < 0 || type >= CARD_TYPE_COUNT) return 0;
    return (visible_mask & (1 << type)) != 0;
}

const char *card_type_name(card_type_t type)
{
    if (type < 0 || type >= CARD_TYPE_COUNT) return "?";
    return TYPE_NAMES[type];
}

char card_toggle_key(card_type_t type)
{
    if (type < 0 || type >= CARD_TYPE_COUNT) return '?';
    return TOGGLE_KEYS[type];
}

int card_default_mask(void)
{
    return 0;
}

int card_all_mask(void)
{
    return (1 << CARD_TYPE_COUNT) - 1;
}

float card_transition_opacity(float t)
{
    if (t <= 0.0f) return 0.0f;
    if (t >= 1.0f) return 1.0f;
    /* Ease-out cubic: 1 - (1-t)^3 */
    float inv = 1.0f - t;
    return 1.0f - inv * inv * inv;
}

float card_transition_slide(float t)
{
    if (t <= 0.0f) return -0.3f; /* off-screen to the right */
    if (t >= 1.0f) return 0.0f;  /* final position */
    /* Ease-out cubic: slide from -0.3 to 0.0 */
    float inv = 1.0f - t;
    float eased = 1.0f - inv * inv * inv;
    return -0.3f * (1.0f - eased);
}
