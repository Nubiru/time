#ifndef TIME_CARD_LAYOUT_H
#define TIME_CARD_LAYOUT_H

#define CARD_TYPE_COUNT 5

typedef enum {
    CARD_TZOLKIN   = 0,
    CARD_ICHING    = 1,
    CARD_CHINESE   = 2,
    CARD_HD        = 3,
    CARD_ASTROLOGY = 4
} card_type_t;

typedef struct {
    float x;           /* screen-space X (0.0 = left edge, 1.0 = right edge) */
    float y;           /* screen-space Y (0.0 = top, 1.0 = bottom) */
    float w;           /* width as fraction of screen */
    float h;           /* height as fraction of screen */
    float opacity;     /* 0.0 = invisible, 1.0 = fully visible */
    int visible;       /* 1 = should render, 0 = hidden */
    card_type_t type;
} card_rect_t;

typedef struct {
    card_rect_t cards[CARD_TYPE_COUNT];
} card_layout_t;

/* Compute full card layout for a given screen aspect ratio and set of visible cards.
 * visible_mask: bitmask (bit 0 = Tzolkin, bit 1 = I Ching, etc.)
 * aspect_ratio: screen width / height */
card_layout_t card_layout_compute(int visible_mask, float aspect_ratio);

/* Toggle a card's visibility in a mask. Returns new mask. */
int card_toggle(int visible_mask, card_type_t type);

/* Check if a card type is visible in the mask. */
int card_is_visible(int visible_mask, card_type_t type);

/* Card type name: "Tzolkin", "I Ching", etc. */
const char *card_type_name(card_type_t type);

/* Card toggle key character: 'K', 'I', 'C', 'D', 'A'. */
char card_toggle_key(card_type_t type);

/* Default visible mask (all cards hidden). */
int card_default_mask(void);

/* All cards visible mask. */
int card_all_mask(void);

/* Compute card opacity during a transition.
 * t: 0.0 = start (hidden), 1.0 = end (visible).
 * Returns eased opacity 0.0-1.0. */
float card_transition_opacity(float t);

/* Compute card slide offset during a transition.
 * t: 0.0 = start (off-screen), 1.0 = end (final position).
 * Returns X offset to add (0.0 = final position, negative = off left). */
float card_transition_slide(float t);

#endif
