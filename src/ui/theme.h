/* theme.h -- Unified Design Token System
 *
 * The keystone module for Time's visual style system.
 * Every renderer reads from a theme_t struct instead of
 * hardcoding visual constants. Changing theme = changing
 * everything in one struct swap.
 *
 * Brand Colors (normalized float RGBA):
 *   TIME primary:   Solar gold       {1.0, 0.85, 0.55, 1.0}
 *     The Sun, source of all earthly time.
 *   TIME secondary: Celestial teal   {0.2, 0.75, 0.8, 1.0}
 *     Cosmic bridge between knowledge systems.
 *
 * Cosmos theme: Deep space blue-black bg, warm text, gold+teal accents.
 * Dawn theme:   Pale warm white bg, dark text, same gold+teal accents
 *               (inverted luminance).
 *
 * Dependencies:
 *   src/math/color.h         -- base color types
 *   src/render/color_theory.h -- system palettes, role colors
 *   src/ui/golden_layout.h   -- phi-derived proportions
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_THEME_H
#define TIME_THEME_H

#include "../math/color.h"
#include "../render/color_theory.h"
#include "golden_layout.h"

/* Theme identity -- which theme is active */
typedef enum {
    THEME_COSMOS = 0,   /* dark: the default -- deep space aesthetic */
    THEME_DAWN,         /* light: pale gold/warm white for bright environments */
    THEME_COUNT
} theme_id_t;

/* User customization preferences */
typedef struct {
    float font_scale;         /* multiplier on base font size (0.5-2.0) */
    float opacity_scale;      /* multiplier on opacity cascade (0.5-1.0) */
    float animation_speed;    /* multiplier on timing (0.5-2.0): 0.5=fast, 2.0=slow */
    color_hsl_t accent_hue;   /* user-chosen accent color */
    int use_custom_accent;    /* 0=use theme default, 1=use accent_hue */
} theme_prefs_t;

/* The central design token struct -- ALL visual constants live here */
typedef struct {
    theme_id_t id;
    const char *name;

    /* Colors -- semantic roles (normalized float RGBA) */
    color_rgba_t bg_space;         /* deepest background */
    color_rgba_t bg_surface;       /* card/panel surface */
    color_rgba_t bg_elevated;      /* floating element surface */
    color_rgba_t text_primary;     /* most readable text */
    color_rgba_t text_secondary;   /* phi^-1 opacity text */
    color_rgba_t text_muted;       /* phi^-2 opacity text */
    color_rgba_t brand_primary;    /* TIME identity: solar gold */
    color_rgba_t brand_secondary;  /* TIME identity: celestial teal */
    color_rgba_t accent;           /* interactive highlights */
    color_rgba_t danger;           /* error/destructive */
    color_rgba_t success;          /* positive state */
    color_rgba_t warning;          /* attention */
    color_rgba_t border;           /* subtle dividers */
    color_rgba_t star_glow;        /* warm star highlight */

    /* Layout -- from golden_layout */
    gl_type_scale_t typography;
    gl_spacing_t spacing;
    gl_timing_t timing;
    gl_opacity_t opacity;

    /* Derived metrics */
    float corner_radius;           /* phi-derived */
    float border_width;            /* phi-derived */
    float shadow_blur;             /* phi-derived */
    float icon_size;               /* phi-derived from typography */
} theme_t;

/* --- Theme Construction --- */

/* Get a built-in theme by ID. Pure function. */
theme_t theme_get(theme_id_t id);

/* Get the default theme (THEME_COSMOS). */
theme_t theme_default(void);

/* Apply user preferences to a base theme. Returns modified theme. */
theme_t theme_apply_prefs(theme_t base, theme_prefs_t prefs);

/* Get default user preferences (no customization). */
theme_prefs_t theme_default_prefs(void);

/* Validate user preferences (clamp to safe ranges). */
theme_prefs_t theme_validate_prefs(theme_prefs_t prefs);

/* --- System-Aware Colors --- */

/* Resolve accent color based on active knowledge system.
 * When a system is focused, its personality color becomes the accent.
 * system = CT_SYSTEM_COUNT means no system focused (use theme accent). */
color_rgba_t theme_system_accent(const theme_t *theme, ct_system_t system);

/* Get background tint for a knowledge system's card/panel.
 * Subtle: theme surface + hint of system primary at ~0.05 alpha. */
color_rgba_t theme_system_surface(const theme_t *theme, ct_system_t system);

/* --- Theme Interpolation --- */

/* Interpolate between two themes (for smooth theme transitions).
 * t: 0.0 = theme_a, 1.0 = theme_b. Returns blended theme. */
theme_t theme_lerp(theme_t a, theme_t b, float t);

/* --- CSS Export (for DOM HUD) --- */

/* Format a theme's colors as CSS custom property declarations.
 * Writes to buf, returns number of chars written.
 * Format: "--bg-space: R, G, B;\n--text-primary: R, G, B;\n..." */
int theme_to_css(const theme_t *theme, char *buf, int buf_size);

/* --- Theme Naming --- */

/* Get theme name string. */
const char *theme_name(theme_id_t id);

#endif /* TIME_THEME_H */
