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
    {0.75f, 0.40f, 0.18f},  /* ASTROLOGY: burnt amber (~18° hue, separated from cluster) */
    {0.85f, 0.20f, 0.20f},  /* TZOLKIN: east red */
    {0.30f, 0.28f, 0.55f},  /* ICHING: visible indigo (was 0.15,0.15,0.35 — invisible on dark) */
    {0.20f, 0.65f, 0.30f},  /* CHINESE: wood green */
    {0.82f, 0.50f, 0.22f},  /* HUMAN_DESIGN: warm copper (~35° hue, separated from amber) */
    {0.70f, 0.72f, 0.75f},  /* GREGORIAN: silver */
    {0.88f, 0.72f, 0.28f},  /* KABBALAH: bright gold (~52° hue, separated from ochre) */
    {0.92f, 0.52f, 0.12f},  /* HINDU: deep saffron (~28° hue, separated from astrology) */
    {0.78f, 0.60f, 0.18f},  /* BUDDHIST: warm ochre (~40° hue, separated from HD) */
    {0.15f, 0.25f, 0.65f},  /* HEBREW: tekhelet blue */
    {0.10f, 0.55f, 0.35f},  /* ISLAMIC: emerald */
    {0.55f, 0.35f, 0.20f},  /* GEOLOGY: sienna */
    {0.70f, 0.50f, 0.20f},  /* COPTIC: desert gold */
    {0.15f, 0.55f, 0.25f},  /* ETHIOPIAN: highland green */
    {0.20f, 0.35f, 0.70f},  /* PERSIAN: tile blue */
    {0.85f, 0.65f, 0.70f},  /* JAPANESE: sakura pink */
    {0.80f, 0.30f, 0.20f},  /* KOREAN: dancheong red */
    {0.90f, 0.75f, 0.20f},  /* THAI: Buddhist yellow */
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
    {0.92f, 0.80f, 0.22f},  /* BUDDHIST: bright gold (~45° hue, warmest) */
    {0.88f, 0.72f, 0.20f},  /* HEBREW: antique gold (~40° hue) */
    {0.82f, 0.70f, 0.25f},  /* ISLAMIC: warm gold (~52° hue, toward amber) */
    {0.75f, 0.60f, 0.25f},  /* GEOLOGY: amber */
    {0.20f, 0.55f, 0.50f},  /* COPTIC: Nile teal */
    {0.80f, 0.68f, 0.18f},  /* ETHIOPIAN: dark gold (~56° hue, coolest gold) */
    {0.30f, 0.65f, 0.60f},  /* PERSIAN: turquoise */
    {0.45f, 0.60f, 0.35f},  /* JAPANESE: matcha green */
    {0.55f, 0.70f, 0.55f},  /* KOREAN: celadon green */
    {0.20f, 0.35f, 0.60f},  /* THAI: democracy blue */
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
    {0.75f, 0.25f, 0.20f},  /* COPTIC: liturgical red */
    {0.80f, 0.25f, 0.15f},  /* ETHIOPIAN: sovereign red */
    {0.80f, 0.40f, 0.45f},  /* PERSIAN: garden rose */
    {0.20f, 0.25f, 0.55f},  /* JAPANESE: ai-zome indigo */
    {0.30f, 0.40f, 0.65f},  /* KOREAN: hanbok blue */
    {0.75f, 0.40f, 0.50f},  /* THAI: royal pink */
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
    "Hebrew", "Islamic", "Geology",
    "Coptic", "Ethiopian", "Persian", "Japanese", "Korean", "Thai"
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

/* =====================================================================
 *  Itten Color System
 *  Source: Johannes Itten, The Art of Color (1961)
 * ===================================================================== */

/* Itten's 12-hue circle mapped to standard HSL hue degrees.
 * 12 entries at 30-degree intervals: index 0 = 0 deg (Red), index 1 = 30 deg, etc.
 * Natural luminosity values from Goethe/Itten scale, normalized 0.0-1.0. */
static const float ITTEN_NATURAL_LIGHTNESS[12] = {
    0.60f,   /* red (0 deg) */
    0.70f,   /* red-orange (30 deg) */
    0.80f,   /* orange (60 deg) — Itten has yellow here but HSL 60=yellow */
    0.75f,   /* yellow-green (90 deg) */
    0.60f,   /* green (120 deg) */
    0.55f,   /* blue-green light (150 deg) */
    0.50f,   /* blue-green / cyan (180 deg) */
    0.42f,   /* blue (210 deg) — interpolated between blue-green and blue */
    0.40f,   /* blue deep (240 deg) */
    0.30f,   /* violet (270 deg) */
    0.45f,   /* red-violet / magenta (300 deg) */
    0.52f    /* cool red (330 deg) — interpolated red-violet to red */
};

/* Normalize hue to [0, 360). */
static float normalize_hue(float h)
{
    h = fmodf(h, 360.0f);
    if (h < 0.0f) h += 360.0f;
    return h;
}

float ct_hue_natural_lightness(float hue_degrees)
{
    float h = normalize_hue(hue_degrees);

    /* Find the two bracketing entries in the 12-step table (30 deg steps). */
    float idx_f = h / 30.0f;
    int idx_lo = (int)idx_f % 12;
    int idx_hi = (idx_lo + 1) % 12;
    float t = idx_f - (float)(int)idx_f;

    float lo = ITTEN_NATURAL_LIGHTNESS[idx_lo];
    float hi = ITTEN_NATURAL_LIGHTNESS[idx_hi];
    return lo + (hi - lo) * t;
}

float ct_extension_ratio(float hue_a_deg, float hue_b_deg)
{
    float la = ct_hue_natural_lightness(hue_a_deg);
    float lb = ct_hue_natural_lightness(hue_b_deg);
    float sum = la + lb;
    if (sum < 0.001f) return 0.5f;
    /* Brighter hues need LESS area. Area_a proportional to 1/la.
     * ratio_a = (1/la) / (1/la + 1/lb) = lb / (la + lb). */
    return lb / sum;
}

float ct_simultaneous_shift(float bg_hue_deg)
{
    return normalize_hue(bg_hue_deg + 180.0f);
}

/* Helper: check if hue is in a warm zone (red-orange side). */
static int hue_is_warm(float h)
{
    h = normalize_hue(h);
    /* Warm: 0-60 (red through yellow) or 300-360 (magenta through red) */
    return (h <= 60.0f) || (h >= 300.0f);
}

/* Helper: check if hue is in a cold zone (blue-green side). */
static int hue_is_cold(float h)
{
    h = normalize_hue(h);
    /* Cold: 150-270 (cyan through blue through violet) */
    return (h >= 150.0f) && (h <= 270.0f);
}

ct_contrast_t ct_dominant_contrast(float r1, float g1, float b1,
                                    float r2, float g2, float b2)
{
    color_rgb_t c1 = {r1, g1, b1};
    color_rgb_t c2 = {r2, g2, b2};
    color_hsl_t h1 = color_rgb_to_hsl(c1);
    color_hsl_t h2 = color_rgb_to_hsl(c2);

    float hue1 = normalize_hue(h1.h);
    float hue2 = normalize_hue(h2.h);

    /* Hue difference (shortest arc). */
    float hue_diff = fabsf(hue1 - hue2);
    if (hue_diff > 180.0f) hue_diff = 360.0f - hue_diff;

    float light_diff = fabsf(h1.l - h2.l);
    float sat_diff = fabsf(h1.s - h2.s);

    /* Check lightness first — strongest visual impact. */
    if (light_diff > 0.5f)
        return CT_CONTRAST_LIGHT_DARK;

    /* Complementary: hue difference near 180 deg (within 30 deg). */
    if (hue_diff >= 150.0f && h1.s > 0.2f && h2.s > 0.2f)
        return CT_CONTRAST_COMPLEMENTARY;

    /* Cold-warm: one warm, one cold, both saturated. */
    if (h1.s > 0.2f && h2.s > 0.2f) {
        int w1 = hue_is_warm(hue1);
        int w2 = hue_is_warm(hue2);
        int c1_cold = hue_is_cold(hue1);
        int c2_cold = hue_is_cold(hue2);
        if ((w1 && c2_cold) || (w2 && c1_cold))
            return CT_CONTRAST_COLD_WARM;
    }

    /* Saturation contrast. */
    if (sat_diff > 0.4f)
        return CT_CONTRAST_SATURATION;

    /* Hue contrast: large hue difference, both saturated. */
    if (hue_diff > 90.0f && h1.s > 0.3f && h2.s > 0.3f)
        return CT_CONTRAST_HUE;

    /* Default: simultaneous (always present). */
    return CT_CONTRAST_SIMULTANEOUS;
}

static const char *CONTRAST_NAMES[CT_CONTRAST_COUNT] = {
    "Hue",
    "Light-Dark",
    "Cold-Warm",
    "Complementary",
    "Simultaneous",
    "Saturation",
    "Extension"
};

const char *ct_contrast_name(ct_contrast_t contrast)
{
    if (contrast < 0 || contrast >= CT_CONTRAST_COUNT) return "?";
    return CONTRAST_NAMES[contrast];
}

float ct_hue_temperature(float hue_degrees)
{
    float h = normalize_hue(hue_degrees);

    /* Warmest at red-orange ~15 deg = +1.0
     * Coldest at blue-green ~195 deg = -1.0
     * These are 180 deg apart. Cosine curve centered on the warm-cold axis. */
    float warm_center = 15.0f;
    float angle_from_warm = h - warm_center;
    /* Convert to radians. */
    float rad = angle_from_warm * (float)(PI / 180.0);
    /* Cosine gives +1 at warm center, -1 at 180 deg away (cold center). */
    return cosf(rad);
}
