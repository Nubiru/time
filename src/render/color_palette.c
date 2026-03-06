#include "color_palette.h"

static color_rgb_t rgb(float r, float g, float b)
{
    color_rgb_t c = {r, g, b};
    return c;
}

static const color_rgb_t GRAY = {0.5f, 0.5f, 0.5f};

/* Zodiac sign colors — element-based with per-sign variation */
static const color_rgb_t ZODIAC_COLORS[12] = {
    {0.90f, 0.25f, 0.20f},  /* Aries       — fire red */
    {0.45f, 0.65f, 0.30f},  /* Taurus      — earth green */
    {0.90f, 0.85f, 0.30f},  /* Gemini      — air yellow */
    {0.30f, 0.55f, 0.80f},  /* Cancer      — water blue */
    {0.95f, 0.55f, 0.15f},  /* Leo         — fire orange */
    {0.55f, 0.50f, 0.35f},  /* Virgo       — earth brown */
    {0.70f, 0.85f, 0.55f},  /* Libra       — air green-yellow */
    {0.35f, 0.25f, 0.60f},  /* Scorpio     — water indigo */
    {0.85f, 0.35f, 0.30f},  /* Sagittarius — fire orange-red */
    {0.40f, 0.40f, 0.40f},  /* Capricorn   — earth dark gray */
    {0.30f, 0.70f, 0.85f},  /* Aquarius    — air cyan */
    {0.50f, 0.40f, 0.75f},  /* Pisces      — water purple */
};

/* Element colors */
static const color_rgb_t ELEMENT_COLORS[4] = {
    {0.90f, 0.30f, 0.20f},  /* Fire  — red */
    {0.55f, 0.45f, 0.30f},  /* Earth — brown */
    {0.90f, 0.85f, 0.35f},  /* Air   — yellow */
    {0.25f, 0.50f, 0.80f},  /* Water — blue */
};

/* Planet colors — traditional associations */
static const color_rgb_t PLANET_COLORS[8] = {
    {1.00f, 0.85f, 0.20f},  /* Sun     — gold */
    {0.70f, 0.75f, 0.80f},  /* Mercury — silver-gray */
    {0.85f, 0.55f, 0.70f},  /* Venus   — rose pink */
    {0.85f, 0.30f, 0.20f},  /* Mars    — red */
    {0.80f, 0.60f, 0.25f},  /* Jupiter — orange-gold */
    {0.65f, 0.60f, 0.45f},  /* Saturn  — tan */
    {0.40f, 0.75f, 0.80f},  /* Uranus  — cyan */
    {0.25f, 0.35f, 0.75f},  /* Neptune — deep blue */
};

/* Aspect colors */
static const color_rgb_t ASPECT_COLORS[5] = {
    {1.00f, 0.85f, 0.25f},  /* Conjunction — gold */
    {0.90f, 0.20f, 0.15f},  /* Opposition  — red */
    {0.25f, 0.50f, 0.85f},  /* Trine       — blue */
    {0.90f, 0.55f, 0.15f},  /* Square      — orange */
    {0.30f, 0.75f, 0.35f},  /* Sextile     — green */
};

/* Tzolkin seal families */
static const color_rgb_t TZOLKIN_COLORS[4] = {
    {0.85f, 0.20f, 0.20f},  /* Red family    */
    {0.90f, 0.90f, 0.90f},  /* White family  */
    {0.20f, 0.40f, 0.80f},  /* Blue family   */
    {0.95f, 0.85f, 0.20f},  /* Yellow family */
};

/* Chinese elements */
static const color_rgb_t CHINESE_COLORS[5] = {
    {0.20f, 0.65f, 0.30f},  /* Wood  — green */
    {0.85f, 0.25f, 0.20f},  /* Fire  — red */
    {0.80f, 0.70f, 0.30f},  /* Earth — yellow */
    {0.85f, 0.85f, 0.85f},  /* Metal — white/silver */
    {0.15f, 0.15f, 0.20f},  /* Water — dark/black */
};

color_rgb_t color_zodiac_sign(int sign)
{
    if (sign < 0 || sign > 11) return GRAY;
    return ZODIAC_COLORS[sign];
}

color_rgb_t color_zodiac_element(int element)
{
    if (element < 0 || element > 3) return GRAY;
    return ELEMENT_COLORS[element];
}

color_rgb_t color_planet(int planet_id)
{
    if (planet_id < 0 || planet_id > 7) return GRAY;
    return PLANET_COLORS[planet_id];
}

color_rgb_t color_aspect(int aspect_type)
{
    if (aspect_type < 0 || aspect_type > 4) return GRAY;
    return ASPECT_COLORS[aspect_type];
}

color_rgb_t color_tzolkin_family(int family)
{
    if (family < 0 || family > 3) return GRAY;
    return TZOLKIN_COLORS[family];
}

color_rgb_t color_chinese_element(int element)
{
    if (element < 0 || element > 4) return GRAY;
    return CHINESE_COLORS[element];
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
