/* color_theory.h — Psychologically Meaningful Color System
 *
 * Every color in Time has MEANING. No random choices.
 * Colors derived from:
 *   - Color psychology (what each hue evokes neurologically)
 *   - Golden ratio harmony (phi-spaced hue relationships)
 *   - sRGB/linear-space correctness for WebGL rendering
 *   - Cultural/symbolic associations per knowledge system
 *
 * All colors: normalized float [0.0-1.0] RGBA.
 * All math: linear space. Apply sRGB gamma only at final output.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_COLOR_THEORY_H
#define TIME_COLOR_THEORY_H

#include "../math/color.h"

/* --- sRGB / Linear Space Conversion ---
 * WebGL shaders operate in linear space.
 * Human-authored colors are typically sRGB.
 * ALWAYS convert to linear before math, back to sRGB for display. */

/* Convert a single channel from sRGB to linear. */
float ct_srgb_to_linear(float srgb);

/* Convert a single channel from linear to sRGB. */
float ct_linear_to_srgb(float linear);

/* Convert full color sRGB -> linear. */
color_rgb_t ct_to_linear(color_rgb_t srgb);

/* Convert full color linear -> sRGB. */
color_rgb_t ct_to_srgb(color_rgb_t linear);

/* --- Golden Ratio Color Harmony ---
 * Instead of arbitrary hue steps, use phi-based angular relationships. */

/* Golden angle: 360 / phi^2 = ~137.508 degrees.
 * Nature uses this for sunflower seed packing. We use it for color spacing. */
#define CT_GOLDEN_ANGLE 137.50776405003785

/* Generate N harmonious hues spaced by the golden angle.
 * base_hue: starting hue (0-360 degrees).
 * out: array of N color_hsl_t values with golden-spaced hues.
 * Saturation and lightness are preserved from the base. */
void ct_golden_palette(color_hsl_t base, int n, color_hsl_t *out);

/* Generate a phi-complementary color (hue + golden angle). */
color_hsl_t ct_golden_complement(color_hsl_t c);

/* Generate phi-triadic colors (hue +/- golden angle). */
void ct_golden_triad(color_hsl_t c, color_hsl_t out[3]);

/* --- Color Psychology ---
 * What each hue range communicates to the human brain. */

typedef enum {
    CT_MOOD_ENERGY = 0,     /* red-orange: activation, urgency, fire */
    CT_MOOD_WARMTH,         /* orange-yellow: comfort, optimism, sun */
    CT_MOOD_CLARITY,        /* yellow-green: intellect, growth, spring */
    CT_MOOD_HARMONY,        /* green: balance, nature, stability */
    CT_MOOD_CALM,           /* blue-green: tranquility, depth, water */
    CT_MOOD_TRUST,          /* blue: reliability, sky, infinity */
    CT_MOOD_WISDOM,         /* indigo: intuition, depth, cosmos */
    CT_MOOD_MYSTERY,        /* violet: spirituality, transformation */
    CT_MOOD_AUTHORITY,      /* deep red-purple: power, sovereignty */
    CT_MOOD_NEUTRAL,        /* gray tones: objectivity, time itself */
    CT_MOOD_COUNT
} ct_mood_t;

/* Get the base hue (HSL) for a psychological mood. */
color_hsl_t ct_mood_color(ct_mood_t mood);

/* Get mood name as string. */
const char *ct_mood_name(ct_mood_t mood);

/* Determine the dominant mood of a color. */
ct_mood_t ct_color_mood(color_hsl_t c);

/* --- Semantic Color Roles ---
 * What visual PURPOSE a color serves in the UI. */

typedef enum {
    CT_ROLE_PRIMARY = 0,    /* main interactive elements */
    CT_ROLE_SECONDARY,      /* supporting elements */
    CT_ROLE_ACCENT,         /* highlights, call to action */
    CT_ROLE_BACKGROUND,     /* deep space / canvas */
    CT_ROLE_SURFACE,        /* card/panel backgrounds */
    CT_ROLE_TEXT_PRIMARY,   /* main readable text */
    CT_ROLE_TEXT_SECONDARY, /* dimmed/supporting text */
    CT_ROLE_BORDER,         /* subtle dividers */
    CT_ROLE_SUCCESS,        /* positive state */
    CT_ROLE_WARNING,        /* attention needed */
    CT_ROLE_DANGER,         /* destructive/error */
    CT_ROLE_INFO,           /* informational */
    CT_ROLE_COUNT
} ct_role_t;

/* Get the color for a semantic role (in the cosmic/dark theme).
 * All colors returned in linear space, ready for WebGL. */
color_rgba_t ct_role_color(ct_role_t role);

/* Get role name as string. */
const char *ct_role_name(ct_role_t role);

/* --- System-Specific Palettes ---
 * Each knowledge system gets a psychologically appropriate palette. */

typedef enum {
    CT_SYSTEM_ASTRONOMY = 0,   /* cool blues + warm golds (sky + sun) */
    CT_SYSTEM_ASTROLOGY,       /* element-coded: fire/earth/air/water */
    CT_SYSTEM_TZOLKIN,         /* 4 directional colors: red/white/blue/yellow */
    CT_SYSTEM_ICHING,          /* yin/yang: deep indigo + ivory */
    CT_SYSTEM_CHINESE,         /* 5 elements: wood/fire/earth/metal/water */
    CT_SYSTEM_HUMAN_DESIGN,    /* body-energy: amber/crimson/teal/violet */
    CT_SYSTEM_GREGORIAN,       /* neutral/modern: silver/white/gray */
    CT_SYSTEM_KABBALAH,        /* Tree of Life: 4 Worlds colors */
    CT_SYSTEM_HINDU,           /* Panchanga: saffron/vermillion/turmeric */
    CT_SYSTEM_BUDDHIST,        /* Dharma: ochre/gold/deep blue/white */
    CT_SYSTEM_HEBREW,          /* Torah: royal blue/gold/white */
    CT_SYSTEM_ISLAMIC,         /* geometric: emerald/gold/lapis */
    CT_SYSTEM_GEOLOGY,         /* Earth: sienna/amber/slate/jade */
    CT_SYSTEM_COUNT
} ct_system_t;

/* Get the primary color for a knowledge system. Linear space. */
color_rgb_t ct_system_primary(ct_system_t system);

/* Get the secondary color for a knowledge system. Linear space. */
color_rgb_t ct_system_secondary(ct_system_t system);

/* Get the accent color for a knowledge system. Linear space. */
color_rgb_t ct_system_accent(ct_system_t system);

/* Get system name as string. */
const char *ct_system_name(ct_system_t system);

/* --- Depth & Atmosphere ---
 * Colors that create spatial depth (background -> foreground). */

/* Cosmic background: the black of space (not pure #000000).
 * Subtly tinted deep blue-black for richness. */
color_rgb_t ct_space_black(void);

/* Atmospheric fog color at a given depth (0.0=near, 1.0=far).
 * Blends from foreground clarity to space-black. */
color_rgb_t ct_depth_fog(float depth);

/* Star glow tint: warm highlight for bright objects. */
color_rgb_t ct_star_glow(void);

/* --- Accessibility ---
 * Ensure sufficient contrast for readability. */

/* WCAG contrast ratio between two colors (1:1 to 21:1). */
float ct_contrast_ratio(color_rgb_t fg, color_rgb_t bg);

/* Check if contrast meets WCAG AA standard (>= 4.5:1 for normal text). */
int ct_meets_aa(color_rgb_t fg, color_rgb_t bg);

/* Adjust text color to meet minimum contrast against background.
 * Returns adjusted color. Prefers lighter text on dark backgrounds. */
color_rgb_t ct_ensure_contrast(color_rgb_t text, color_rgb_t bg,
                                float min_ratio);

/* --- Phi-Based Saturation & Lightness ---
 * For creating depth in color sets using golden proportions. */

/* Compute N saturation levels from a base, each step *= phi^-1.
 * out: array of N floats. [0] = base, [1] = base*phi^-1, etc. */
void ct_phi_saturations(float base_sat, int n, float *out);

/* Compute N lightness levels from a base, each step *= phi^-1.
 * out: array of N floats. */
void ct_phi_lightness(float base_light, int n, float *out);

#endif /* TIME_COLOR_THEORY_H */
