/* card_style.h — Per-system card visual styling.
 *
 * Maps any knowledge system to a complete set of card colors
 * derived from the theme and color_theory system palettes.
 * Mapped systems get their unique accent; unmapped systems
 * fall back to the theme's brand colors.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_CARD_STYLE_H
#define TIME_CARD_STYLE_H

#include "../math/color.h"
#include "../render/color_theory.h"
#include "../systems/unified/today_summary.h"
#include "theme.h"

/* Complete visual style for rendering one card */
typedef struct {
    color_rgba_t background;    /* System-tinted surface color */
    color_rgba_t border;        /* System accent at subtle alpha */
    color_rgba_t title;         /* System accent for title text */
    color_rgba_t body;          /* Theme text_primary for body text */
    color_rgba_t muted;         /* Theme text_secondary for secondary text */
} card_style_t;

/* Get card style for a specific knowledge system.
 * system_id: ts_system_t value (0-19)
 * opacity: overall card opacity (0.0-1.0), scales background and border alpha
 * theme_id: THEME_COSMOS or THEME_DAWN */
card_style_t card_style_for_system(int system_id, float opacity,
                                    theme_id_t theme_id);

/* Get default card style (no system-specific tinting).
 * Uses theme brand_primary for title, bg_surface for background. */
card_style_t card_style_default(float opacity, theme_id_t theme_id);

/* Map ts_system_t to ct_system_t for color palette lookup.
 * Returns ct_system_t value (0-12) or -1 if no mapping exists. */
int card_style_to_ct_system(int ts_system_id);

#endif /* TIME_CARD_STYLE_H */
