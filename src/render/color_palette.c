#include "color_palette.h"
#include "color_theory.h"

static color_rgb_t rgb(float r, float g, float b)
{
    color_rgb_t c = {r, g, b};
    return c;
}

static const color_rgb_t GRAY = {0.5f, 0.5f, 0.5f};

/* --- Zodiac Colors ---
 * Derived from ct_mood_color() bases with per-sign HSL adjustments.
 * Fire signs from ENERGY hue range, Earth from WARMTH/HARMONY,
 * Air from CLARITY, Water from TRUST/WISDOM/MYSTERY. */

static color_rgb_t zodiac_color(int sign)
{
    color_hsl_t hsl;

    switch (sign) {
    case 0: /* Aries — fire red */
        hsl = ct_mood_color(CT_MOOD_ENERGY);
        hsl.s = 0.75f; hsl.l = 0.55f;
        break;
    case 1: /* Taurus — earth green */
        hsl = ct_mood_color(CT_MOOD_HARMONY);
        hsl.h = 96.0f; hsl.s = 0.40f; hsl.l = 0.48f;
        break;
    case 2: /* Gemini — air yellow */
        hsl = ct_mood_color(CT_MOOD_CLARITY);
        hsl.h = 55.0f; hsl.s = 0.80f; hsl.l = 0.60f;
        break;
    case 3: /* Cancer — water blue */
        hsl = ct_mood_color(CT_MOOD_TRUST);
        hsl.s = 0.56f; hsl.l = 0.55f;
        break;
    case 4: /* Leo — fire orange */
        hsl = ct_mood_color(CT_MOOD_WARMTH);
        hsl.h = 30.0f; hsl.s = 0.85f; hsl.l = 0.55f;
        break;
    case 5: /* Virgo — earth brown (matches ASTROLOGY secondary exactly) */
        return ct_to_srgb(ct_system_secondary(CT_SYSTEM_ASTROLOGY));
    case 6: /* Libra — air green-yellow */
        hsl = ct_mood_color(CT_MOOD_CLARITY);
        hsl.h = 90.0f; hsl.s = 0.50f; hsl.l = 0.70f;
        break;
    case 7: /* Scorpio — water indigo */
        hsl = ct_mood_color(CT_MOOD_WISDOM);
        hsl.s = 0.42f; hsl.l = 0.43f;
        break;
    case 8: /* Sagittarius — fire orange-red */
        hsl = ct_mood_color(CT_MOOD_ENERGY);
        hsl.h = 5.0f; hsl.s = 0.56f; hsl.l = 0.58f;
        break;
    case 9: /* Capricorn — earth dark gray */
        hsl = ct_mood_color(CT_MOOD_NEUTRAL);
        hsl.l = 0.40f;
        break;
    case 10: /* Aquarius — air cyan */
        hsl = ct_mood_color(CT_MOOD_CALM);
        hsl.h = 195.0f; hsl.s = 0.65f; hsl.l = 0.58f;
        break;
    case 11: /* Pisces — water purple */
        hsl = ct_mood_color(CT_MOOD_MYSTERY);
        hsl.h = 265.0f; hsl.s = 0.38f; hsl.l = 0.57f;
        break;
    default:
        return GRAY;
    }
    return color_hsl_to_rgb(hsl);
}

/* --- Element Colors ---
 * Derived from ct_mood_color() with per-element HSL adjustments.
 * Fire=ENERGY, Earth=WARMTH, Air=CLARITY, Water=TRUST. */

static color_rgb_t element_color(int element)
{
    color_hsl_t hsl;

    switch (element) {
    case 0: /* Fire — red: ENERGY mood with adjusted S/L */
        hsl = ct_mood_color(CT_MOOD_ENERGY);
        hsl.s = 0.70f; hsl.l = 0.55f;
        break;
    case 1: /* Earth — brown: WARMTH mood desaturated */
        hsl = ct_mood_color(CT_MOOD_WARMTH);
        hsl.s = 0.35f; hsl.l = 0.43f;
        break;
    case 2: /* Air — yellow: CLARITY mood shifted toward yellow */
        hsl = ct_mood_color(CT_MOOD_CLARITY);
        hsl.h = 60.0f; hsl.s = 0.75f; hsl.l = 0.62f;
        break;
    case 3: /* Water — blue: TRUST mood adjusted */
        hsl = ct_mood_color(CT_MOOD_TRUST);
        hsl.s = 0.56f; hsl.l = 0.52f;
        break;
    default:
        return GRAY;
    }
    return color_hsl_to_rgb(hsl);
}

/* --- Planet Colors ---
 * Derived from ASTRONOMY system palette and mood colors.
 * Sun/Mars from system_secondary/accent, others from mood-based HSL. */

static color_rgb_t planet_color(int planet_id)
{
    color_hsl_t hsl;

    switch (planet_id) {
    case 0: { /* Sun — gold: ASTRONOMY secondary (star gold) brightened */
        color_rgb_t s = ct_to_srgb(ct_system_secondary(CT_SYSTEM_ASTRONOMY));
        color_hsl_t sh = color_rgb_to_hsl(s);
        sh.l += 0.05f;
        return color_hsl_to_rgb(sh);
    }
    case 1: /* Mercury — silver-gray: NEUTRAL mood desaturated cold */
        hsl = ct_mood_color(CT_MOOD_NEUTRAL);
        hsl.h = 210.0f; hsl.s = 0.08f; hsl.l = 0.75f;
        break;
    case 2: /* Venus — rose pink: AUTHORITY mood softened */
        hsl = ct_mood_color(CT_MOOD_AUTHORITY);
        hsl.h = 330.0f; hsl.s = 0.45f; hsl.l = 0.70f;
        break;
    case 3: /* Mars — red: ASTRONOMY accent (mars red) */
        return ct_to_srgb(ct_system_accent(CT_SYSTEM_ASTRONOMY));
    case 4: /* Jupiter — orange-gold: WARMTH mood adjusted */
        hsl = ct_mood_color(CT_MOOD_WARMTH);
        hsl.s = 0.55f; hsl.l = 0.52f;
        break;
    case 5: /* Saturn — tan: WARMTH mood desaturated */
        hsl = ct_mood_color(CT_MOOD_WARMTH);
        hsl.s = 0.22f; hsl.l = 0.55f;
        break;
    case 6: /* Uranus — cyan: CALM mood shifted to blue-green */
        hsl = ct_mood_color(CT_MOOD_CALM);
        hsl.h = 190.0f; hsl.s = 0.52f; hsl.l = 0.60f;
        break;
    case 7: /* Neptune — deep blue: TRUST hue at indigo boundary */
        hsl = ct_mood_color(CT_MOOD_TRUST);
        hsl.h = 228.0f; hsl.s = 0.50f; hsl.l = 0.50f;
        break;
    default:
        return GRAY;
    }
    return color_hsl_to_rgb(hsl);
}

/* --- Aspect Colors ---
 * Derived from ASTRONOMY/ASTROLOGY system palettes and mood colors.
 * Gold conjunction from ASTRONOMY secondary, others from mood/system. */

static color_rgb_t aspect_color_derive(int aspect_type)
{
    color_hsl_t hsl;

    switch (aspect_type) {
    case 0: { /* Conjunction — gold: ASTRONOMY secondary brightened */
        color_rgb_t s = ct_to_srgb(ct_system_secondary(CT_SYSTEM_ASTRONOMY));
        color_hsl_t sh = color_rgb_to_hsl(s);
        sh.l += 0.04f;
        return color_hsl_to_rgb(sh);
    }
    case 1: /* Opposition — red: ENERGY mood adjusted */
        hsl = ct_mood_color(CT_MOOD_ENERGY);
        hsl.h = 4.0f; hsl.s = 0.80f; hsl.l = 0.53f;
        break;
    case 2: /* Trine — blue: ASTRONOMY primary (celestial blue) */
        return ct_to_srgb(ct_system_primary(CT_SYSTEM_ASTRONOMY));
    case 3: /* Square — orange: WARMTH mood boosted saturation */
        hsl = ct_mood_color(CT_MOOD_WARMTH);
        hsl.s = 0.82f; hsl.l = 0.52f;
        break;
    case 4: /* Sextile — green: HARMONY mood adjusted */
        hsl = ct_mood_color(CT_MOOD_HARMONY);
        hsl.h = 130.0f; hsl.s = 0.50f; hsl.l = 0.52f;
        break;
    default:
        return GRAY;
    }
    return color_hsl_to_rgb(hsl);
}

/* --- Tzolkin Colors ---
 * Red/White/Blue from ct_system_primary/secondary/accent(CT_SYSTEM_TZOLKIN).
 * Yellow derived as 4th golden-palette color of the primary hue. */

static color_rgb_t tzolkin_color(int family)
{
    switch (family) {
    case 0: /* Red — TZOLKIN primary (east red) */
        return ct_to_srgb(ct_system_primary(CT_SYSTEM_TZOLKIN));
    case 1: /* White — TZOLKIN secondary (north white) */
        return ct_to_srgb(ct_system_secondary(CT_SYSTEM_TZOLKIN));
    case 2: /* Blue — TZOLKIN accent (west blue) */
        return ct_to_srgb(ct_system_accent(CT_SYSTEM_TZOLKIN));
    case 3: { /* Yellow — 4th golden-palette entry from primary hue */
        color_rgb_t p = ct_to_srgb(ct_system_primary(CT_SYSTEM_TZOLKIN));
        color_hsl_t ph = color_rgb_to_hsl(p);
        color_hsl_t pal[4];
        ct_golden_palette(ph, 4, pal);
        pal[3].s = 0.82f;
        pal[3].l = 0.58f;
        return color_hsl_to_rgb(pal[3]);
    }
    default:
        return GRAY;
    }
}

/* --- Chinese Element Colors ---
 * Wood/Fire/Earth from ct_system_primary/secondary/accent(CT_SYSTEM_CHINESE).
 * Metal derived from NEUTRAL mood (high lightness).
 * Water derived from WISDOM mood (very dark). */

static color_rgb_t chinese_color(int element)
{
    color_hsl_t hsl;

    switch (element) {
    case 0: /* Wood — CHINESE primary (wood green) */
        return ct_to_srgb(ct_system_primary(CT_SYSTEM_CHINESE));
    case 1: /* Fire — CHINESE secondary (fire red) */
        return ct_to_srgb(ct_system_secondary(CT_SYSTEM_CHINESE));
    case 2: /* Earth — CHINESE accent (earth gold) */
        return ct_to_srgb(ct_system_accent(CT_SYSTEM_CHINESE));
    case 3: /* Metal — NEUTRAL mood at high lightness (silver/white) */
        hsl = ct_mood_color(CT_MOOD_NEUTRAL);
        hsl.l = 0.85f;
        return color_hsl_to_rgb(hsl);
    case 4: /* Water — WISDOM mood very dark (dark/black) */
        hsl = ct_mood_color(CT_MOOD_WISDOM);
        hsl.s = 0.18f; hsl.l = 0.17f;
        return color_hsl_to_rgb(hsl);
    default:
        return GRAY;
    }
}

/* --- Public API --- */

color_rgb_t color_zodiac_sign(int sign)
{
    if (sign < 0 || sign > 11) return GRAY;
    return zodiac_color(sign);
}

color_rgb_t color_zodiac_element(int element)
{
    if (element < 0 || element > 3) return GRAY;
    return element_color(element);
}

color_rgb_t color_planet(int planet_id)
{
    if (planet_id < 0 || planet_id > 7) return GRAY;
    return planet_color(planet_id);
}

color_rgb_t color_aspect(int aspect_type)
{
    if (aspect_type < 0 || aspect_type > 4) return GRAY;
    return aspect_color_derive(aspect_type);
}

color_rgb_t color_tzolkin_family(int family)
{
    if (family < 0 || family > 3) return GRAY;
    return tzolkin_color(family);
}

color_rgb_t color_chinese_element(int element)
{
    if (element < 0 || element > 4) return GRAY;
    return chinese_color(element);
}

color_rgb_t color_scale(color_rgb_t c, float factor)
{
    return rgb(c.r * factor, c.g * factor, c.b * factor);
}

color_rgb_t color_clamp(color_rgb_t c)
{
    if (c.r < 0.0f) c.r = 0.0f; else if (c.r > 1.0f) c.r = 1.0f;
    if (c.g < 0.0f) c.g = 0.0f; else if (c.g > 1.0f) c.g = 1.0f;
    if (c.b < 0.0f) c.b = 0.0f; else if (c.b > 1.0f) c.b = 1.0f;
    return c;
}
