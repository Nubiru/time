#ifndef TIME_SYMBOL_ATLAS_H
#define TIME_SYMBOL_ATLAS_H

#include "zodiac_glyphs.h"
#include "../render/font_atlas.h"

/* Symbol categories — ordered as they appear in the atlas grid */
enum {
    SYM_CAT_ZODIAC      = 0,  /* 12: Aries..Pisces */
    SYM_CAT_PLANET       = 1,  /*  8: Mercury..Neptune */
    SYM_CAT_ASPECT       = 2,  /*  5: Conjunction..Sextile */
    SYM_CAT_TRIGRAM      = 3,  /*  8: Qian..Kun */
    SYM_CAT_MAYAN_SEAL   = 4,  /* 20: Dragon..Sun */
    SYM_CAT_LUNAR_PHASE  = 5,  /*  8: New..Waning Crescent */
    SYM_CAT_COUNT        = 6
};

/* Atlas grid metadata */
typedef struct {
    int cols;    /* columns in atlas grid */
    int rows;    /* rows in atlas grid */
    int cell_w;  /* pixel width per cell (GLYPH_WIDTH) */
    int cell_h;  /* pixel height per cell (GLYPH_HEIGHT) */
} symbol_atlas_info_t;

/* Get number of symbols in a category. Returns 0 for invalid. */
int symbol_atlas_count(int category);

/* Total symbols across all categories. */
int symbol_atlas_total(void);

/* Get bitmap data for symbol (category, index).
 * Returns pointer to GLYPH_BYTES-sized array (8x12, 0/1 per byte).
 * Returns NULL for invalid category or index. */
const unsigned char *symbol_atlas_glyph(int category, int index);

/* Get UV coordinates for a symbol in the packed atlas texture.
 * Returns zero UV rect for invalid category or index. */
glyph_uv_t symbol_atlas_uv(int category, int index);

/* Atlas grid info (dimensions, cell size). */
symbol_atlas_info_t symbol_atlas_info(void);

/* Required buffer size for the RGBA texture (in bytes). */
int symbol_atlas_texture_size(void);

/* Pack all glyphs into an RGBA texture buffer.
 * White pixels (255,255,255,255) where glyph=1, transparent (0,0,0,0) where glyph=0.
 * buf must be at least symbol_atlas_texture_size() bytes. */
void symbol_atlas_pack(unsigned char *buf, int buf_size);

/* Human-readable category name. Returns NULL for invalid. */
const char *symbol_atlas_category_name(int category);

/* Human-readable symbol name (e.g. "Aries", "Dragon").
 * Returns NULL for invalid category or index. */
const char *symbol_atlas_symbol_name(int category, int index);

#endif
