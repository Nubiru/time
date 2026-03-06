#include "zodiac_glyphs.h"

#include <stddef.h>

/* --- Zodiac sign glyphs (8x12 each, row-major) --- */

/* Aries: ram horns — two upward curves meeting at top */
static const unsigned char GLYPH_ARIES[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,0,1,0,0,1,0,0,
    0,1,0,1,1,0,1,0,
    1,0,0,1,1,0,0,1,
    1,0,0,1,1,0,0,1,
    0,0,0,1,1,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

/* Taurus: bull head — circle with horns */
static const unsigned char GLYPH_TAURUS[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    1,0,0,0,0,0,0,1,
    0,1,0,0,0,0,1,0,
    0,0,1,0,0,1,0,0,
    0,0,1,1,1,1,0,0,
    0,1,0,0,0,0,1,0,
    0,1,0,0,0,0,1,0,
    0,1,0,0,0,0,1,0,
    0,0,1,1,1,1,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

/* Gemini: twins — two vertical lines with bars */
static const unsigned char GLYPH_GEMINI[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,1,1,1,1,1,1,0,
    0,0,0,1,1,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,1,0,0,1,0,0,
    0,0,1,0,0,1,0,0,
    0,0,1,0,0,1,0,0,
    0,0,1,0,0,1,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,1,1,0,0,0,
    0,1,1,1,1,1,1,0,
    0,0,0,0,0,0,0,0,
};

/* Cancer: 69 shape — two interlocking curves */
static const unsigned char GLYPH_CANCER[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,0,1,1,1,1,0,0,
    0,1,0,0,0,0,1,0,
    0,1,0,0,0,0,0,0,
    0,1,1,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,1,1,0,
    0,0,0,0,0,0,1,0,
    0,1,0,0,0,0,1,0,
    0,0,1,1,1,1,0,0,
    0,0,0,0,0,0,0,0,
};

/* Leo: swoosh/tail — curved shape like a musical note */
static const unsigned char GLYPH_LEO[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,0,1,1,0,0,0,0,
    0,1,0,0,1,0,0,0,
    0,1,0,0,1,0,0,0,
    0,0,1,1,0,0,0,0,
    0,0,0,1,0,0,0,0,
    0,0,0,1,0,0,0,0,
    0,0,0,0,1,0,0,0,
    0,0,0,0,0,1,0,0,
    0,0,0,0,0,1,1,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

/* Virgo: M with tail — M shape with descending stroke */
static const unsigned char GLYPH_VIRGO[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    1,0,0,1,0,0,1,0,
    1,1,0,1,1,0,1,0,
    1,0,1,1,0,1,1,0,
    1,0,0,1,0,0,1,0,
    1,0,0,1,0,0,1,0,
    0,0,0,0,0,0,1,0,
    0,0,0,0,0,0,1,0,
    0,0,0,0,0,1,0,0,
    0,0,0,0,1,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

/* Libra: scales — horizontal line with bump on top */
static const unsigned char GLYPH_LIBRA[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,1,0,0,1,0,0,
    0,0,1,0,0,1,0,0,
    0,0,0,1,1,0,0,0,
    0,1,1,1,1,1,1,0,
    0,0,0,0,0,0,0,0,
    0,1,1,1,1,1,1,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

/* Scorpio: M with arrow — M shape with arrow tail */
static const unsigned char GLYPH_SCORPIO[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    1,0,0,1,0,0,1,0,
    1,1,0,1,1,0,1,0,
    1,0,1,1,0,1,1,0,
    1,0,0,1,0,0,1,0,
    1,0,0,1,0,0,1,0,
    0,0,0,0,0,0,1,0,
    0,0,0,0,0,0,0,1,
    0,0,0,0,0,0,1,1,
    0,0,0,0,0,1,0,1,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

/* Sagittarius: arrow — diagonal arrow pointing upper-right */
static const unsigned char GLYPH_SAGITTARIUS[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,0,0,0,1,1,1,0,
    0,0,0,0,0,1,1,0,
    0,0,0,0,1,0,1,0,
    0,0,0,1,0,0,0,0,
    0,0,1,0,0,0,0,0,
    0,1,0,1,0,0,0,0,
    1,0,0,0,1,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

/* Capricorn: goat — V with curved tail */
static const unsigned char GLYPH_CAPRICORN[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,1,0,0,0,0,0,0,
    0,1,0,0,0,0,0,0,
    0,0,1,0,0,0,0,0,
    0,0,1,0,0,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,0,0,1,0,0,
    0,0,0,0,0,0,1,0,
    0,0,0,0,0,0,1,0,
    0,0,0,0,0,1,0,0,
    0,0,0,0,1,0,0,0,
    0,0,0,0,0,0,0,0,
};

/* Aquarius: waves — two horizontal wavy lines */
static const unsigned char GLYPH_AQUARIUS[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,1,0,0,1,0,0,0,
    1,0,1,1,0,1,1,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,1,0,0,1,0,0,0,
    1,0,1,1,0,1,1,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

/* Pisces: fish — two curved brackets with bar */
static const unsigned char GLYPH_PISCES[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,0,1,0,0,1,0,0,
    0,1,0,0,0,0,1,0,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    0,1,1,1,1,1,1,0,
    0,1,1,1,1,1,1,0,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    0,1,0,0,0,0,1,0,
    0,0,1,0,0,1,0,0,
    0,0,0,0,0,0,0,0,
};

static const unsigned char *ZODIAC_GLYPHS[12] = {
    GLYPH_ARIES, GLYPH_TAURUS, GLYPH_GEMINI, GLYPH_CANCER,
    GLYPH_LEO, GLYPH_VIRGO, GLYPH_LIBRA, GLYPH_SCORPIO,
    GLYPH_SAGITTARIUS, GLYPH_CAPRICORN, GLYPH_AQUARIUS, GLYPH_PISCES
};

/* --- Planet glyphs (8x12 each) --- */

/* Mercury: circle with horns and cross */
static const unsigned char GLYPH_MERCURY[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,1,0,0,1,0,0,
    0,0,0,1,1,0,0,0,
    0,0,1,1,1,1,0,0,
    0,1,0,0,0,0,1,0,
    0,1,0,0,0,0,1,0,
    0,0,1,1,1,1,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,1,1,1,1,0,0,
    0,0,0,0,0,0,0,0,
};

/* Venus: circle with cross below */
static const unsigned char GLYPH_VENUS[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,0,1,1,1,1,0,0,
    0,1,0,0,0,0,1,0,
    0,1,0,0,0,0,1,0,
    0,1,0,0,0,0,1,0,
    0,0,1,1,1,1,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,1,1,1,1,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

/* Earth: circle with cross inside */
static const unsigned char GLYPH_EARTH[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,0,1,1,1,1,0,0,
    0,1,0,1,1,0,1,0,
    0,1,0,1,1,0,1,0,
    0,1,1,1,1,1,1,0,
    0,1,0,1,1,0,1,0,
    0,1,0,1,1,0,1,0,
    0,0,1,1,1,1,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

/* Mars: circle with arrow upper-right */
static const unsigned char GLYPH_MARS[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,1,1,0,
    0,0,0,0,0,0,1,0,
    0,0,0,0,0,1,1,0,
    0,0,0,0,1,0,0,0,
    0,0,1,1,0,0,0,0,
    0,1,0,0,1,0,0,0,
    0,1,0,0,0,1,0,0,
    0,1,0,0,0,1,0,0,
    0,0,1,1,1,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

/* Jupiter: stylized 4 shape */
static const unsigned char GLYPH_JUPITER[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,0,0,0,1,1,0,0,
    0,0,0,0,0,1,0,0,
    0,0,0,0,0,1,0,0,
    0,1,1,1,1,1,0,0,
    0,0,0,1,0,1,0,0,
    0,0,1,0,0,1,0,0,
    0,1,0,0,0,1,0,0,
    0,0,0,0,0,1,0,0,
    0,0,0,0,0,1,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

/* Saturn: cross with curved top — h shape */
static const unsigned char GLYPH_SATURN[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,0,1,0,0,0,0,0,
    0,0,1,0,0,0,0,0,
    0,1,1,1,0,0,0,0,
    0,0,1,0,0,0,0,0,
    0,0,1,1,1,0,0,0,
    0,0,0,0,0,1,0,0,
    0,0,0,0,0,1,0,0,
    0,0,0,0,1,0,0,0,
    0,0,0,1,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

/* Uranus: H with dot on top line */
static const unsigned char GLYPH_URANUS[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,1,1,0,0,0,
    0,1,0,1,1,0,1,0,
    0,1,0,1,1,0,1,0,
    0,1,1,1,1,1,1,0,
    0,1,0,0,0,0,1,0,
    0,1,0,0,0,0,1,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

/* Neptune: trident — fork shape */
static const unsigned char GLYPH_NEPTUNE[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,1,0,0,0,0,1,0,
    0,1,0,0,0,0,1,0,
    0,1,0,1,1,0,1,0,
    0,0,1,1,1,1,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,1,1,1,1,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

static const unsigned char *PLANET_GLYPHS[8] = {
    GLYPH_MERCURY, GLYPH_VENUS, GLYPH_EARTH, GLYPH_MARS,
    GLYPH_JUPITER, GLYPH_SATURN, GLYPH_URANUS, GLYPH_NEPTUNE
};

/* --- Aspect glyphs (8x12 each) --- */

/* Conjunction: filled circle */
static const unsigned char GLYPH_CONJUNCTION[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,1,1,1,1,0,0,
    0,1,1,1,1,1,1,0,
    0,1,1,1,1,1,1,0,
    0,1,1,1,1,1,1,0,
    0,0,1,1,1,1,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

/* Opposition: circle-line-circle */
static const unsigned char GLYPH_OPPOSITION[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,1,0,0,1,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,1,0,0,1,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

/* Trine: triangle */
static const unsigned char GLYPH_TRINE[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,1,0,0,1,0,0,
    0,0,1,0,0,1,0,0,
    0,1,0,0,0,0,1,0,
    0,1,0,0,0,0,1,0,
    1,1,1,1,1,1,1,1,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

/* Square: square outline */
static const unsigned char GLYPH_SQUARE[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,1,1,1,1,1,1,0,
    0,1,0,0,0,0,1,0,
    0,1,0,0,0,0,1,0,
    0,1,0,0,0,0,1,0,
    0,1,0,0,0,0,1,0,
    0,1,0,0,0,0,1,0,
    0,1,1,1,1,1,1,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

/* Sextile: six-pointed star (asterisk) */
static const unsigned char GLYPH_SEXTILE[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,1,1,0,0,0,
    0,1,0,1,1,0,1,0,
    0,0,1,0,0,1,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,1,0,0,1,0,0,
    0,1,0,1,1,0,1,0,
    0,0,0,1,1,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

static const unsigned char *ASPECT_GLYPHS[5] = {
    GLYPH_CONJUNCTION, GLYPH_OPPOSITION, GLYPH_TRINE,
    GLYPH_SQUARE, GLYPH_SEXTILE
};

/* --- Public API --- */

const unsigned char *zodiac_glyph(int sign)
{
    if (sign < 0 || sign > 11) return NULL;
    return ZODIAC_GLYPHS[sign];
}

const unsigned char *planet_glyph(int planet_id)
{
    if (planet_id < 0 || planet_id > 7) return NULL;
    return PLANET_GLYPHS[planet_id];
}

const unsigned char *aspect_glyph(int aspect_type)
{
    if (aspect_type < 0 || aspect_type > 4) return NULL;
    return ASPECT_GLYPHS[aspect_type];
}

int zodiac_glyph_count(void)
{
    return 12;
}

int planet_glyph_count(void)
{
    return 8;
}

int glyph_pixel(const unsigned char *glyph, int x, int y)
{
    if (!glyph) return 0;
    if (x < 0 || x >= GLYPH_WIDTH) return 0;
    if (y < 0 || y >= GLYPH_HEIGHT) return 0;
    return glyph[y * GLYPH_WIDTH + x] != 0;
}
