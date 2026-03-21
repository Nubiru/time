/*
 * msdf_text.h — MSDF text layout (pure module, P1)
 *
 * Computes glyph quad positions and UV coordinates from MSDF atlas metrics.
 * Works with both msdf_mono_data.h and msdf_sans_data.h glyph data.
 * No GL, no state — pure math, testable natively.
 */

#ifndef TIME_MSDF_TEXT_H
#define TIME_MSDF_TEXT_H

#define MSDF_TEXT_MAX_QUADS 256

/* One positioned glyph quad ready for rendering. */
typedef struct {
    float x, y;         /* top-left position in caller units (pixels) */
    float w, h;         /* quad size in caller units */
    float u0, v0;       /* UV top-left [0,1] */
    float u1, v1;       /* UV bottom-right [0,1] */
    int codepoint;      /* unicode codepoint */
} msdf_quad_t;

/* Result of a text layout operation. */
typedef struct {
    msdf_quad_t quads[MSDF_TEXT_MAX_QUADS];
    int count;
    float total_width;  /* total advance width in caller units */
    float line_height;  /* font_size-based line height */
} msdf_layout_t;

/* Font selection. */
typedef enum {
    MSDF_FONT_MONO = 0,
    MSDF_FONT_SANS = 1
} msdf_font_id_t;

/* Layout text at position (x, y) with given font_size (in caller units).
 * y is the baseline position. Glyphs extend above and below. */
msdf_layout_t msdf_text_layout(msdf_font_id_t font, const char *text,
                               float x, float y, float font_size);

/* Measure text advance width without generating quads. */
float msdf_text_width(msdf_font_id_t font, const char *text, float font_size);

/* Layout text centered horizontally at center_x. */
msdf_layout_t msdf_text_centered(msdf_font_id_t font, const char *text,
                                 float center_x, float y, float font_size);

/* Layout with word wrapping at max_width.
 * line_spacing is the multiplier on line_height between lines (e.g., 1.5). */
msdf_layout_t msdf_text_wrapped(msdf_font_id_t font, const char *text,
                                float x, float y, float font_size,
                                float max_width, float line_spacing);

/* Get atlas pixel dimensions for a font. */
int msdf_text_atlas_width(msdf_font_id_t font);
int msdf_text_atlas_height(msdf_font_id_t font);

/* Number of glyphs available in a font. */
int msdf_text_glyph_count(msdf_font_id_t font);

#endif /* TIME_MSDF_TEXT_H */
