#include "color_theory.h"

#include <math.h>

#define PI 3.14159265358979323846
#define PHI 1.6180339887498948482
#define PHI_INV 0.6180339887498948482

/* --- sRGB / Linear Space Conversion --- */

float ct_srgb_to_linear(float srgb)
{
    if (srgb <= 0.04045f)
        return srgb / 12.92f;
    return powf((srgb + 0.055f) / 1.055f, 2.4f);
}

float ct_linear_to_srgb(float linear)
{
    if (linear <= 0.0031308f)
        return linear * 12.92f;
    return 1.055f * powf(linear, 1.0f / 2.4f) - 0.055f;
}

color_rgb_t ct_to_linear(color_rgb_t srgb)
{
    color_rgb_t c;
    c.r = ct_srgb_to_linear(srgb.r);
    c.g = ct_srgb_to_linear(srgb.g);
    c.b = ct_srgb_to_linear(srgb.b);
    return c;
}

color_rgb_t ct_to_srgb(color_rgb_t linear)
{
    color_rgb_t c;
    c.r = ct_linear_to_srgb(linear.r);
    c.g = ct_linear_to_srgb(linear.g);
    c.b = ct_linear_to_srgb(linear.b);
    return c;
}

/* --- Golden Ratio Color Harmony --- */

void ct_golden_palette(color_hsl_t base, int n, color_hsl_t *out)
{
    for (int i = 0; i < n; i++) {
        out[i].h = fmodf(base.h + (float)CT_GOLDEN_ANGLE * (float)i, 360.0f);
        out[i].s = base.s;
        out[i].l = base.l;
    }
}

color_hsl_t ct_golden_complement(color_hsl_t c)
{
    color_hsl_t gc = c;
    gc.h = fmodf(c.h + (float)CT_GOLDEN_ANGLE, 360.0f);
    return gc;
}

void ct_golden_triad(color_hsl_t c, color_hsl_t out[3])
{
    out[0] = c;
    out[1].h = fmodf(c.h + (float)CT_GOLDEN_ANGLE, 360.0f);
    out[1].s = c.s;
    out[1].l = c.l;
    out[2].h = fmodf(c.h + 2.0f * (float)CT_GOLDEN_ANGLE, 360.0f);
    out[2].s = c.s;
    out[2].l = c.l;
}

/* --- Color Psychology --- */

/* Hues mapped to psychological effects (HSL).
 * Based on color psychology research:
 * - Red (0-15): energy, urgency, activation
 * - Orange (30): warmth, optimism, sociability
 * - Yellow-Green (75): clarity, intellect, growth
 * - Green (120): harmony, balance, nature
 * - Teal (170): calm, depth, water
 * - Blue (210): trust, reliability, infinity
 * - Indigo (250): wisdom, intuition, cosmos
 * - Violet (280): mystery, spirituality, transformation
 * - Deep Magenta (320): authority, sovereignty
 * - Gray (0, low-sat): objectivity, time itself */
static const float MOOD_HUES[CT_MOOD_COUNT] = {
    8.0f,     /* ENERGY: red-orange */
    35.0f,    /* WARMTH: orange */
    75.0f,    /* CLARITY: yellow-green */
    120.0f,   /* HARMONY: green */
    170.0f,   /* CALM: blue-green */
    210.0f,   /* TRUST: blue */
    250.0f,   /* WISDOM: indigo */
    280.0f,   /* MYSTERY: violet */
    320.0f,   /* AUTHORITY: deep magenta */
    0.0f      /* NEUTRAL: achromatic */
};

static const float MOOD_SATS[CT_MOOD_COUNT] = {
    0.80f, 0.75f, 0.70f, 0.60f, 0.55f,
    0.65f, 0.70f, 0.65f, 0.60f, 0.05f
};

static const float MOOD_LIGHTS[CT_MOOD_COUNT] = {
    0.50f, 0.55f, 0.55f, 0.45f, 0.45f,
    0.45f, 0.35f, 0.40f, 0.35f, 0.50f
};

color_hsl_t ct_mood_color(ct_mood_t mood)
{
    color_hsl_t c = {0.0f, 0.0f, 0.5f};
    if (mood < 0 || mood >= CT_MOOD_COUNT) return c;
    c.h = MOOD_HUES[mood];
    c.s = MOOD_SATS[mood];
    c.l = MOOD_LIGHTS[mood];
    return c;
}

static const char *MOOD_NAMES[CT_MOOD_COUNT] = {
    "Energy", "Warmth", "Clarity", "Harmony", "Calm",
    "Trust", "Wisdom", "Mystery", "Authority", "Neutral"
};

const char *ct_mood_name(ct_mood_t mood)
{
    if (mood < 0 || mood >= CT_MOOD_COUNT) return "?";
    return MOOD_NAMES[mood];
}

ct_mood_t ct_color_mood(color_hsl_t c)
{
    if (c.s < 0.1f) return CT_MOOD_NEUTRAL;

    float h = fmodf(c.h, 360.0f);
    if (h < 0.0f) h += 360.0f;

    /* Map hue to closest mood */
    if (h < 22.0f)  return CT_MOOD_ENERGY;
    if (h < 55.0f)  return CT_MOOD_WARMTH;
    if (h < 97.0f)  return CT_MOOD_CLARITY;
    if (h < 145.0f) return CT_MOOD_HARMONY;
    if (h < 190.0f) return CT_MOOD_CALM;
    if (h < 230.0f) return CT_MOOD_TRUST;
    if (h < 265.0f) return CT_MOOD_WISDOM;
    if (h < 300.0f) return CT_MOOD_MYSTERY;
    if (h < 345.0f) return CT_MOOD_AUTHORITY;
    return CT_MOOD_ENERGY; /* wraps back to red */
}

/* --- Semantic Color Roles --- */

/* Dark cosmic theme. Colors chosen for:
 * - Readability on dark backgrounds
 * - Psychological appropriateness (trust=blue, danger=red, etc.)
 * - Golden ratio saturation/lightness relationships
 * All values in sRGB, converted to linear on return. */

color_rgba_t ct_role_color(ct_role_t role)
{
    color_rgba_t c = {0.0f, 0.0f, 0.0f, 1.0f};
    color_rgb_t srgb;

    switch (role) {
    case CT_ROLE_PRIMARY:
        srgb = color_hsl_to_rgb(color_hsl(210.0f, 0.65f, 0.55f)); break;
    case CT_ROLE_SECONDARY:
        srgb = color_hsl_to_rgb(color_hsl(210.0f, 0.30f, 0.45f)); break;
    case CT_ROLE_ACCENT:
        srgb = color_hsl_to_rgb(color_hsl(45.0f, 0.85f, 0.55f)); break;
    case CT_ROLE_BACKGROUND:
        srgb = (color_rgb_t){0.031f, 0.031f, 0.047f}; break; /* near-black, blue tint */
    case CT_ROLE_SURFACE:
        srgb = (color_rgb_t){0.067f, 0.067f, 0.090f}; break; /* card panels */
    case CT_ROLE_TEXT_PRIMARY:
        srgb = (color_rgb_t){0.90f, 0.90f, 0.92f}; break; /* high contrast */
    case CT_ROLE_TEXT_SECONDARY:
        /* phi^-1 opacity relative to primary */
        srgb = (color_rgb_t){0.90f * (float)PHI_INV,
                             0.90f * (float)PHI_INV,
                             0.92f * (float)PHI_INV}; break;
    case CT_ROLE_BORDER:
        srgb = (color_rgb_t){0.20f, 0.20f, 0.25f}; break;
    case CT_ROLE_SUCCESS:
        srgb = color_hsl_to_rgb(color_hsl(145.0f, 0.65f, 0.45f)); break;
    case CT_ROLE_WARNING:
        srgb = color_hsl_to_rgb(color_hsl(40.0f, 0.85f, 0.55f)); break;
    case CT_ROLE_DANGER:
        srgb = color_hsl_to_rgb(color_hsl(0.0f, 0.75f, 0.50f)); break;
    case CT_ROLE_INFO:
        srgb = color_hsl_to_rgb(color_hsl(200.0f, 0.70f, 0.50f)); break;
    default:
        srgb = (color_rgb_t){0.5f, 0.5f, 0.5f}; break;
    }

    color_rgb_t lin = ct_to_linear(srgb);
    c.r = lin.r;
    c.g = lin.g;
    c.b = lin.b;
    return c;
}

static const char *ROLE_NAMES[CT_ROLE_COUNT] = {
    "Primary", "Secondary", "Accent", "Background", "Surface",
    "Text Primary", "Text Secondary", "Border",
    "Success", "Warning", "Danger", "Info"
};

const char *ct_role_name(ct_role_t role)
{
    if (role < 0 || role >= CT_ROLE_COUNT) return "?";
    return ROLE_NAMES[role];
}

/* --- System-Specific Palettes ---
 * Each system's colors are chosen for psychological/cultural meaning:
 *
 * ASTRONOMY: cool blue (sky/infinity) + warm gold (sun/stars)
 * ASTROLOGY: element-coded (fire=red, earth=brown, air=yellow, water=blue)
 * TZOLKIN: 4 directional colors per Arguelles (red=east, white=north, blue=west, yellow=south)
 * ICHING: yin/yang duality (deep indigo night + ivory light)
 * CHINESE: Wu Xing (wood=green, fire=red, earth=gold, metal=silver, water=black)
 * HUMAN_DESIGN: body-energy (amber warmth, crimson drive, teal calm, violet intuition)
 * GREGORIAN: neutral modernity (silver precision, clean white)
 * KABBALAH: 4 Worlds (Atziluth=gold, Briah=silver, Yetzirah=indigo, Assiah=olive)
 * HINDU: sacred colors (saffron devotion, vermillion power, turmeric auspicious)
 * BUDDHIST: dharma (ochre robes, gold enlightenment, deep blue depth)
 * HEBREW: Torah tradition (royal blue tekhelet, gold, parchment white)
 * ISLAMIC: geometric beauty (emerald paradise, gold, lapis lazuli)
 * GEOLOGY: Earth itself (sienna soil, amber minerals, slate stone, jade life) */

static const color_rgb_t SYSTEM_PRIMARY[CT_SYSTEM_COUNT] = {
    {0.25f, 0.50f, 0.85f},  /* ASTRONOMY: celestial blue */
    {0.70f, 0.45f, 0.20f},  /* ASTROLOGY: amber */
    {0.85f, 0.20f, 0.20f},  /* TZOLKIN: east red */
    {0.15f, 0.15f, 0.35f},  /* ICHING: deep indigo */
    {0.20f, 0.65f, 0.30f},  /* CHINESE: wood green */
    {0.80f, 0.55f, 0.15f},  /* HUMAN_DESIGN: amber */
    {0.70f, 0.72f, 0.75f},  /* GREGORIAN: silver */
    {0.85f, 0.70f, 0.20f},  /* KABBALAH: Atziluth gold */
    {0.90f, 0.55f, 0.10f},  /* HINDU: saffron */
    {0.75f, 0.55f, 0.15f},  /* BUDDHIST: ochre */
    {0.15f, 0.25f, 0.65f},  /* HEBREW: tekhelet blue */
    {0.10f, 0.55f, 0.35f},  /* ISLAMIC: emerald */
    {0.55f, 0.35f, 0.20f},  /* GEOLOGY: sienna */
};

static const color_rgb_t SYSTEM_SECONDARY[CT_SYSTEM_COUNT] = {
    {0.95f, 0.80f, 0.20f},  /* ASTRONOMY: star gold */
    {0.55f, 0.50f, 0.35f},  /* ASTROLOGY: earth brown */
    {0.90f, 0.90f, 0.90f},  /* TZOLKIN: north white */
    {0.90f, 0.88f, 0.82f},  /* ICHING: ivory yang */
    {0.85f, 0.25f, 0.20f},  /* CHINESE: fire red */
    {0.75f, 0.20f, 0.25f},  /* HUMAN_DESIGN: crimson */
    {0.92f, 0.92f, 0.94f},  /* GREGORIAN: clean white */
    {0.72f, 0.75f, 0.80f},  /* KABBALAH: Briah silver */
    {0.80f, 0.15f, 0.15f},  /* HINDU: vermillion */
    {0.90f, 0.78f, 0.20f},  /* BUDDHIST: gold */
    {0.85f, 0.75f, 0.25f},  /* HEBREW: gold */
    {0.85f, 0.72f, 0.18f},  /* ISLAMIC: gold */
    {0.75f, 0.60f, 0.25f},  /* GEOLOGY: amber */
};

static const color_rgb_t SYSTEM_ACCENT[CT_SYSTEM_COUNT] = {
    {0.90f, 0.35f, 0.15f},  /* ASTRONOMY: mars red */
    {0.30f, 0.55f, 0.80f},  /* ASTROLOGY: water blue */
    {0.20f, 0.40f, 0.80f},  /* TZOLKIN: west blue */
    {0.45f, 0.45f, 0.55f},  /* ICHING: muted gray-blue */
    {0.80f, 0.70f, 0.30f},  /* CHINESE: earth gold */
    {0.25f, 0.60f, 0.65f},  /* HUMAN_DESIGN: teal */
    {0.40f, 0.45f, 0.55f},  /* GREGORIAN: slate */
    {0.20f, 0.20f, 0.50f},  /* KABBALAH: Yetzirah indigo */
    {0.85f, 0.70f, 0.10f},  /* HINDU: turmeric */
    {0.15f, 0.20f, 0.55f},  /* BUDDHIST: deep blue */
    {0.90f, 0.88f, 0.80f},  /* HEBREW: parchment */
    {0.15f, 0.25f, 0.55f},  /* ISLAMIC: lapis lazuli */
    {0.35f, 0.60f, 0.40f},  /* GEOLOGY: jade */
};

color_rgb_t ct_system_primary(ct_system_t system)
{
    if (system < 0 || system >= CT_SYSTEM_COUNT)
        return (color_rgb_t){0.5f, 0.5f, 0.5f};
    return ct_to_linear(SYSTEM_PRIMARY[system]);
}

color_rgb_t ct_system_secondary(ct_system_t system)
{
    if (system < 0 || system >= CT_SYSTEM_COUNT)
        return (color_rgb_t){0.5f, 0.5f, 0.5f};
    return ct_to_linear(SYSTEM_SECONDARY[system]);
}

color_rgb_t ct_system_accent(ct_system_t system)
{
    if (system < 0 || system >= CT_SYSTEM_COUNT)
        return (color_rgb_t){0.5f, 0.5f, 0.5f};
    return ct_to_linear(SYSTEM_ACCENT[system]);
}

static const char *SYSTEM_NAMES[CT_SYSTEM_COUNT] = {
    "Astronomy", "Astrology", "Tzolkin", "I Ching", "Chinese",
    "Human Design", "Gregorian", "Kabbalah", "Hindu", "Buddhist",
    "Hebrew", "Islamic", "Geology"
};

const char *ct_system_name(ct_system_t system)
{
    if (system < 0 || system >= CT_SYSTEM_COUNT) return "?";
    return SYSTEM_NAMES[system];
}

/* --- Depth & Atmosphere --- */

color_rgb_t ct_space_black(void)
{
    /* Not pure black — subtly tinted deep blue for richness.
     * HSL(230, 0.20, 0.03) — almost invisible, but alive. */
    return ct_to_linear((color_rgb_t){0.024f, 0.027f, 0.036f});
}

color_rgb_t ct_depth_fog(float depth)
{
    if (depth < 0.0f) depth = 0.0f;
    if (depth > 1.0f) depth = 1.0f;

    /* Fog is space-black at maximum depth, transparent at zero. */
    color_rgb_t space = ct_space_black();
    /* Near objects have a subtle warm tint (starlight glow). */
    color_rgb_t warm = ct_to_linear((color_rgb_t){0.06f, 0.05f, 0.04f});
    return color_lerp(warm, space, depth);
}

color_rgb_t ct_star_glow(void)
{
    /* Warm white with subtle gold — the color of starlight.
     * Blackbody ~5500K approximation. */
    return ct_to_linear((color_rgb_t){1.00f, 0.95f, 0.85f});
}

/* --- Accessibility --- */

static float relative_luminance(color_rgb_t c)
{
    /* Already in linear space — use standard coefficients */
    return 0.2126f * c.r + 0.7152f * c.g + 0.0722f * c.b;
}

float ct_contrast_ratio(color_rgb_t fg, color_rgb_t bg)
{
    float l1 = relative_luminance(ct_to_linear(fg));
    float l2 = relative_luminance(ct_to_linear(bg));

    if (l1 < l2) {
        float tmp = l1;
        l1 = l2;
        l2 = tmp;
    }
    return (l1 + 0.05f) / (l2 + 0.05f);
}

int ct_meets_aa(color_rgb_t fg, color_rgb_t bg)
{
    return ct_contrast_ratio(fg, bg) >= 4.5f;
}

color_rgb_t ct_ensure_contrast(color_rgb_t text, color_rgb_t bg,
                                float min_ratio)
{
    color_rgb_t result = text;
    float bg_lum = relative_luminance(ct_to_linear(bg));

    /* If dark background, lighten text; if light background, darken. */
    int lighten = (bg_lum < 0.5f) ? 1 : 0;

    for (int i = 0; i < 20; i++) {
        if (ct_contrast_ratio(result, bg) >= min_ratio)
            return result;

        float step = 0.05f;
        if (lighten) {
            result.r += step; if (result.r > 1.0f) result.r = 1.0f;
            result.g += step; if (result.g > 1.0f) result.g = 1.0f;
            result.b += step; if (result.b > 1.0f) result.b = 1.0f;
        } else {
            result.r -= step; if (result.r < 0.0f) result.r = 0.0f;
            result.g -= step; if (result.g < 0.0f) result.g = 0.0f;
            result.b -= step; if (result.b < 0.0f) result.b = 0.0f;
        }
    }
    return result;
}

/* --- Phi-Based Saturation & Lightness --- */

void ct_phi_saturations(float base_sat, int n, float *out)
{
    if (n <= 0) return;
    out[0] = base_sat;
    for (int i = 1; i < n; i++) {
        out[i] = out[i - 1] * (float)PHI_INV;
        if (out[i] < 0.0f) out[i] = 0.0f;
    }
}

void ct_phi_lightness(float base_light, int n, float *out)
{
    if (n <= 0) return;
    out[0] = base_light;
    for (int i = 1; i < n; i++) {
        out[i] = out[i - 1] * (float)PHI_INV;
        if (out[i] < 0.0f) out[i] = 0.0f;
    }
}
