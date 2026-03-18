/* view_adapt.h -- Responsive View Adaptation
 *
 * Per-view breakpoint configuration. Given a view ID and screen width,
 * returns the responsive layout: panel modes, element visibility,
 * touch priority, typography and spacing scales.
 *
 * 7 views x 4 screen tiers = 28 configurations.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects.
 * Self-contained: does NOT depend on view_registry.h or any other
 * Time header. View IDs are used as plain ints (0-6). */

#ifndef TIME_VIEW_ADAPT_H
#define TIME_VIEW_ADAPT_H

/* Screen size tiers matching CSS breakpoints */
typedef enum {
    VA_DESKTOP = 0,    /* > 1024px */
    VA_TABLET,         /* 768-1024px */
    VA_PHONE_LAND,     /* 481-767px */
    VA_PHONE_PORT,     /* <= 480px */
    VA_TIER_COUNT
} va_screen_tier_t;

/* Layout mode for panels */
typedef enum {
    VA_LAYOUT_CARD = 0,     /* floating card (desktop default) */
    VA_LAYOUT_SHEET,        /* bottom/side sheet (mobile) */
    VA_LAYOUT_FULLSCREEN,   /* takes over whole screen (small phones) */
    VA_LAYOUT_HIDDEN        /* not available at this tier */
} va_layout_mode_t;

/* Complete responsive config for a view at a screen tier */
typedef struct {
    int view_id;                  /* which view (0-6) */
    va_screen_tier_t tier;        /* which screen tier */

    /* Panel behavior */
    va_layout_mode_t help_layout;
    va_layout_mode_t layer_layout;
    va_layout_mode_t card_layout;
    va_layout_mode_t palette_layout;

    /* Element visibility */
    int show_hud;           /* 1 = show full HUD, 0 = condensed */
    int show_time_bar;      /* 1 = always visible, 0 = edge-proximity only */
    int show_corner_help;   /* 1 = show ? button */
    int show_labels;        /* 1 = show text labels on objects */

    /* Touch/input adaptation */
    int touch_priority;     /* 0 = mouse first, 1 = touch first, 2 = touch only */
    int min_tap_target;     /* minimum tap target size in pixels */

    /* Typography */
    float font_scale;       /* multiplier on base font (1.0 = desktop default) */
    float hud_font_scale;   /* multiplier on HUD font specifically */

    /* Spacing */
    float spacing_scale;    /* multiplier on base spacing (1.0 = desktop) */
} va_adapt_t;

/* Get screen tier from width in pixels */
va_screen_tier_t va_tier_from_width(int screen_width);

/* Tier name string */
const char *va_tier_name(va_screen_tier_t tier);

/* Get config for a view at a tier.
 * Out-of-range view_id (negative, >= 7): returns desktop Space config
 * with the requested tier. Out-of-range tier: clamps to VA_DESKTOP. */
va_adapt_t va_get(int view_id, va_screen_tier_t tier);

/* Get config for a view at a screen width (convenience) */
va_adapt_t va_get_for_width(int view_id, int screen_width);

/* Check if a panel type (0=help, 1=layer, 2=card, 3=palette) is usable.
 * Returns 1 if the layout is not HIDDEN, 0 otherwise.
 * Invalid panel_type returns 0. */
int va_panel_available(va_adapt_t config, int panel_type);

/* Number of screen tiers */
int va_tier_count(void);

/* Get breakpoint upper-bound width for a tier.
 * Desktop returns INT32_MAX (no upper bound). */
int va_tier_breakpoint(va_screen_tier_t tier);

/* Get layout mode name as string */
const char *va_layout_name(va_layout_mode_t mode);

#endif /* TIME_VIEW_ADAPT_H */
