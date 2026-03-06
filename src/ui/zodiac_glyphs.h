#ifndef TIME_ZODIAC_GLYPHS_H
#define TIME_ZODIAC_GLYPHS_H

#define GLYPH_WIDTH  8
#define GLYPH_HEIGHT 12
#define GLYPH_BYTES  (GLYPH_WIDTH * GLYPH_HEIGHT)  /* 96 bytes per glyph */

/* Get the bitmap data for a zodiac sign (0=Aries..11=Pisces).
 * Returns pointer to GLYPH_BYTES-sized array.
 * Each byte is 0 (transparent) or 1 (filled).
 * Row-major, top-to-bottom. Returns NULL for invalid index. */
const unsigned char *zodiac_glyph(int sign);

/* Get the bitmap data for a planet (0=Mercury..7=Neptune).
 * Same format as zodiac glyphs. Returns NULL for invalid index. */
const unsigned char *planet_glyph(int planet_id);

/* Get the bitmap for an aspect symbol (0=conjunction..4=sextile).
 * Returns NULL for invalid index. */
const unsigned char *aspect_glyph(int aspect_type);

/* Total number of available zodiac glyphs. */
int zodiac_glyph_count(void);

/* Total number of available planet glyphs. */
int planet_glyph_count(void);

/* Check if a pixel is set at (x, y) in a glyph.
 * Returns 0 if glyph is NULL or coords out of bounds. */
int glyph_pixel(const unsigned char *glyph, int x, int y);

#endif
