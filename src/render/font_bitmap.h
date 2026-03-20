#ifndef TIME_FONT_BITMAP_H
#define TIME_FONT_BITMAP_H

#include <stdint.h>

/* Embedded 5x7 bitmap font for ASCII 32-126.
 * Each glyph is 7 bytes (5 bits per row, MSB-first).
 * Atlas layout: 16 cols x 6 rows, each cell 8x16 px.
 * Total atlas: 128x96 pixels, R8 format. */

#define FONT_BITMAP_CELL_W   8
#define FONT_BITMAP_CELL_H   16
#define FONT_BITMAP_COLS     16
#define FONT_BITMAP_ROWS     6
#define FONT_BITMAP_ATLAS_W  (FONT_BITMAP_COLS * FONT_BITMAP_CELL_W)   /* 128 */
#define FONT_BITMAP_ATLAS_H  (FONT_BITMAP_ROWS * FONT_BITMAP_CELL_H)  /* 96 */
#define FONT_BITMAP_GLYPH_W  5
#define FONT_BITMAP_GLYPH_H  7
#define FONT_BITMAP_FIRST    32
#define FONT_BITMAP_LAST     126
#define FONT_BITMAP_COUNT    (FONT_BITMAP_LAST - FONT_BITMAP_FIRST + 1)  /* 95 */

/* Atlas dimensions */
int font_bitmap_atlas_width(void);
int font_bitmap_atlas_height(void);
int font_bitmap_atlas_size(void);     /* width * height bytes */

/* Number of defined glyphs */
int font_bitmap_glyph_count(void);

/* Get raw 5x7 bitmap data for a character.
 * Returns pointer to 7 bytes (static data), or NULL if out of range.
 * Each byte: bits 7-3 are the 5 pixel columns (MSB = leftmost). */
const uint8_t *font_bitmap_glyph_data(char c);

/* Generate full atlas texture into caller-provided buffer.
 * Buffer must be at least font_bitmap_atlas_size() bytes (128*96 = 12288).
 * Output: R8 format (0x00 = background, 0xFF = foreground).
 * Returns 0 on success, -1 on NULL buffer. */
int font_bitmap_generate(uint8_t *out_pixels);

#endif
